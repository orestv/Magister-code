/* 
 * File:   EuclidMetric.cpp
 * Author: ovoloshchuk
 * 
 * Created on March 9, 2011, 4:14 PM
 */
 

#define DEBUG

#include "EuclidMetric.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <memory.h>
#include <pthread.h>

void recordObject(char *filename, Object *pObj) {
    FILE *pFile = fopen(filename, "w");
    for (int i = 0; i < pObj->attributeCount(); i++) {
        fprintf(pFile, "%i;", pObj->attr(i));
    }
    fprintf(pFile, "\n");
    fclose(pFile);
}

EuclidMetric::EuclidMetric(DataContainer *pContainer) {
    m_pContainer = pContainer;
    m_nAttributeCount = 0;
    m_arrValidAttrCount = NULL;
    m_arrAverageDeltas = NULL;
    return;
}

EuclidMetric::EuclidMetric(const EuclidMetric& orig) {
    m_nAttributeCount = orig.m_nAttributeCount;
    m_arrAverageDeltas = new float[m_nAttributeCount];
    m_arrValidAttrCount = new int[m_nAttributeCount];

    memcpy(m_arrAverageDeltas, orig.m_arrAverageDeltas, m_nAttributeCount*sizeof(float));
    memcpy(m_arrValidAttrCount, orig.m_arrValidAttrCount, m_nAttributeCount*sizeof(int));

    m_pContainer = orig.m_pContainer;
}

struct Thread_competence_data {
    Object **arrObjects;
    Object *pCurrentObj;
    int nObjectCount;
    ObjectRange *arrRanges;

    Thread_competence_data(Object **arrObjects, Object *pCurrentObj, int nObjectCount) {
        this->arrObjects = arrObjects;
        this->nObjectCount = nObjectCount;
        this->pCurrentObj = pCurrentObj;
    }

    ~Thread_competence_data() {
        delete[] arrRanges;
    }
};

void* thrd_object_competence(void *data) {
    Thread_competence_data *pData = (Thread_competence_data*)data;
    Object *pCurrentObj = pData->pCurrentObj;
    int nObjectCount = pData->nObjectCount;
    Object **arrObjects = pData->arrObjects;
    pData->arrRanges = new ObjectRange[nObjectCount];
    double range = 0;
    for (int i = 0; i < nObjectCount; i++) {
        range = EuclidMetric::competence(*arrObjects[i], *pCurrentObj);
        pData->arrRanges[i].pObject = arrObjects[i];
        pData->arrRanges[i].nRange = range;
    }
    pthread_exit((void*)pData);
}

float EuclidMetric::distance(Object& o1, Object& o2, bool bUseIntegratedPrediction) {
    float dResult = 0;
    int nAttributeCount = max(o1.attributeCount(), o2.attributeCount());
    for (int nAttr = 0; nAttr < nAttributeCount; nAttr++) {
        if (bUseIntegratedPrediction) {
            if (o1.isAttrValid(nAttr) && o2.isAttrValid(nAttr)) {
                float d1 = o1.isAttrValid(nAttr) ? o1.attr(nAttr) : m_arrAverageDeltas[nAttr];
                float d2 = o2.isAttrValid(nAttr) ? o2.attr(nAttr) : m_arrAverageDeltas[nAttr];
                dResult += pow(d1 - d2, 2);
                }
            else {
                dResult += m_arrAverageDeltas[nAttr]*m_arrAverageDeltas[nAttr];
            }
        }
        else {
            if (o1.isAttrValid(nAttr) && o2.isAttrValid(nAttr))
                dResult += pow(o1.attr(nAttr) - o2.attr(nAttr), 2);

        }
    }
    dResult = sqrt(dResult);
    return dResult;
}

Object* EuclidMetric::center(DataContainer *pContainer,
        list<int>& indices) {
    int nAttributeCount = 0;
    list<int>::iterator iIndex;
    for (iIndex = indices.begin(); iIndex != indices.end(); iIndex++) {
        nAttributeCount = max(nAttributeCount, pContainer->get(*iIndex)->attributeCount());
    }
    Object *pResult = new Object(nAttributeCount);

    Object *pObj;
    float *pAttributes = new float[nAttributeCount];
    bool *pValids = new bool[nAttributeCount];
    memset(pAttributes, 0, nAttributeCount*sizeof(float));
    memset(pValids, 0, nAttributeCount*sizeof(bool));
    for (iIndex = indices.begin();  \
            iIndex != indices.end(); iIndex++) {

        pObj = pContainer->get(*iIndex);
        for (int nAttribute = 0; nAttribute < nAttributeCount; nAttribute++) {
            if (pObj->isAttrValid(nAttribute)) {
                pAttributes[nAttribute] += pObj->attr(nAttribute);
                pValids[nAttribute] = true;
            }
        }
    }

    int nIndexCount = indices.size();
    for (int nAttribute = 0; nAttribute < nAttributeCount; nAttribute++) {
        pAttributes[nAttribute] /= (float)nIndexCount;
        if (pValids[nAttribute])
            pResult->setAttr(nAttribute, pAttributes[nAttribute]);
    }
    delete[] pAttributes;
    delete[] pValids;
    return pResult;
}

void EuclidMetric::predictMissingData(DataContainer *pContainer) {
    Object *pObj;
    int i = 0;
    int nAttributeCount = 0;
    int nObjectCount = pContainer->ids().size();
    for (int nObject = 0; nObject < nObjectCount; nObject++) {
        pObj = pContainer->getByIndex(nObject);
        if (nAttributeCount == 0)
            nAttributeCount = pObj->attributeCount();
        i++;

        for (int nAttr = 0; 
                nAttr < nAttributeCount;
                nAttr++) {
            if (!pObj->isAttrValid(nAttr)) {
                printf("Object %i of %i being predicted... ", i, nObjectCount);
                predictAttributes(pObj, pContainer);
                break;
            }

        }
    }
}

void EuclidMetric::predictAttributes(Object *pCurrentObj, DataContainer *pContainer) {
    for (int nAttr = 0; nAttr < pCurrentObj->attributeCount();
            nAttr++) {

        if (pCurrentObj->isAttrValid(nAttr))
            continue;

        predictAttribute(pCurrentObj, nAttr, pContainer);
    }
}

void EuclidMetric::predictAttribute(Object *pCurrentObj, int nAttr, DataContainer *pContainer) {
    list<ObjectRange> lsObjectRanges;

    float nBias = 2.;

    Object *pObj;
    float range;

    int nMaxRanges = 15;
    int nObjectCount = pContainer->ids().size();

    int nThreadCount = 5;
    int nObjectsPerThread = nObjectCount / nThreadCount;

    ObjectRange *arrRanges = new ObjectRange[nMaxRanges];
    ObjectRange *pRange;
    Object **arrObjects = new Object*[nObjectsPerThread];

    int nObjectForThread = 0;
    list<pthread_t> lsThreads;
    for (int i = 0; i < nObjectCount; i++) {
        pObj = pContainer->getByIndex(i);
        if (!pObj->isAttrValid(nAttr))
            continue;
        if (arrObjects == NULL)
            arrObjects = new Object*[nObjectsPerThread];

        arrObjects[nObjectForThread] = pObj;
        nObjectForThread++;
        if (nObjectForThread == nObjectsPerThread || 
               i == nObjectCount-1) {
            Thread_competence_data *pData = 
                new Thread_competence_data(arrObjects, 
                        pCurrentObj, nObjectForThread);

            pthread_t thrd;
            pthread_create(&thrd, NULL, thrd_object_competence, (void*)pData);
            lsThreads.push_back(thrd);
            nObjectForThread = 0;
            arrObjects = NULL;
        }

        //lsObjectRanges.push_back(ObjectRange(pObj, range));
    }
    for (list<pthread_t>::iterator iThread = lsThreads.begin();
            iThread != lsThreads.end(); iThread++) {

        void *data;
        pthread_join(*iThread, &data);
        Thread_competence_data *pData = (Thread_competence_data*)data;
        for (int i = 0; i < pData->nObjectCount; i++) {
            Object *pObj = pData->arrObjects[i];
            double range = pData->arrRanges[i].nRange;
            for (int nRange = 0; nRange < nMaxRanges; nRange++) {
                if (arrRanges[nRange].pObject == NULL) {
                    arrRanges[nRange].pObject = pObj;
                    arrRanges[nRange].nRange = range;
                    break;
                } else if (arrRanges[nRange].nRange > range) {
                    for (int i = nMaxRanges-1;
                            i >= nRange; i--) {
                        if (i>0)
                            arrRanges[i] = arrRanges[i-1];
                    }
                    arrRanges[nRange] = ObjectRange(pObj, range);
                    break;
                }
            }
        }
        delete[] pData->arrObjects;
        delete pData;
    }
    /*
	int nObject = 0;
    int nRangeCount = 0;
    float dValue = 0;
	for (int nRange = 0; nRange < nMaxRanges; nRange++) {
        if (arrRanges[nRange].pObject == NULL)
            continue;
        dValue += arrRanges[nRange].pObject->attr(nAttr);
        nRangeCount++;
	}
    delete[] arrRanges;
    dValue /= (float)nRangeCount;
    pCurrentObj->setAttr(nAttr, dValue);
    //printf("Attribute %i predicted, %.4f seconds spent.\n\n", nAttr, d);
    */

    //arrRanges contains at least nMaxRanges of ObjectRange's
    //Find out the exact number
    nObjectCount = 0;
    while (nObjectCount < nMaxRanges && arrRanges[nObjectCount].pObject != NULL)
        nObjectCount++;

    //Fill the object array with objects
    //arrObjects now contains an array of competent objects
    arrObjects = new Object*[nObjectCount];
    for (int i = 0; i < nObjectCount; i++)
        arrObjects[i] = arrRanges[i].pObject;
    delete[] arrRanges;
    
    list<AttributeRange> lsAttrRanges;
    float c;
    for (int i = 0; i < pCurrentObj->attributeCount(); i++) {
        if (i != nAttr) {
            bool bAllAttributesValid = true;
            for (int nObject = 0; nObject < nObjectCount; nObject++) {
                if (!arrObjects[nObject]->isAttrValid(i)) {
                    bAllAttributesValid = false;
                    break;
                }
            }
            if (!bAllAttributesValid)
                continue;
            if (!pCurrentObj->isAttrValid(i))
                continue;
            c = this->competence(i, nAttr, arrObjects, nObjectCount);
            printf("%.2f, \t", c);
            lsAttrRanges.push_back(AttributeRange(i, c));
        }
    }
    lsAttrRanges.sort(EuclidMetric::compareAttributeRanges);
    list<AttributeRange>::iterator iA = lsAttrRanges.begin();
    float diff = abs((++iA)->nRange - lsAttrRanges.begin()->nRange);
    iA++;

    while (iA != lsAttrRanges.end()) {
        if (abs(iA->nRange - lsAttrRanges.begin()->nRange) > nBias*diff) {
            lsAttrRanges.erase(iA);
            break;
        }
        iA++;
    }
    int nAttributeCount = lsAttrRanges.size();
    printf("Valid attribute count: %i\n", nAttributeCount);
    int *arrAttrs = new int[nAttributeCount];
    {
        iA = lsAttrRanges.begin();
        int i = 0;
        while (iA != lsAttrRanges.end()) {
            arrAttrs[i] = iA->nAttribute;
            i++;
            iA++;
        }
    }

    //TODO: add prediction based on arrAttrs and arrObjects here

    delete[] arrAttrs;
    delete[] arrObjects;
}

float EuclidMetric::competence(Object &o1, Object &o2) {
	float nResult = 0;
	int nValidAttributes = 0;
    int nCount = o1.attributeCount();
	for (int nAttr = 0; nAttr < nCount; nAttr++) {
		if (o1.isAttrValid(nAttr) && o2.isAttrValid(nAttr)) {
			nValidAttributes++;
			nResult += pow(o1.attr(nAttr)-o2.attr(nAttr), 2);
		}		
	}
    nResult = (1. - sqrt(nResult)) * nValidAttributes;

	return nResult;
}

float EuclidMetric::competence(int attr1, int attr2, Object** arrObjects, int nObjects) {
    float *arrValues1 = new float[nObjects];
    float *arrValues2 = new float[nObjects];

    int nValidObjects1 = 0, nValidObjects2 = 0;

    for (int nObject = 0; nObject < nObjects; nObject++) {
        if (arrObjects[nObject]->isAttrValid(attr1)) {
            arrValues1[nValidObjects1] = arrObjects[nObject]->attr(attr1);
            nValidObjects1++;
        }
        if (arrObjects[nObject]->isAttrValid(attr2)) {
            arrValues2[nValidObjects2] = arrObjects[nObject]->attr(attr2);
            nValidObjects2++;
        }
    }

    //TODO: this function must only use valid attributes

    float exp1 = EuclidMetric::expectation(arrValues1, nValidObjects1);
    float exp2 = EuclidMetric::expectation(arrValues2, nValidObjects2);

    memset(arrValues1, 0, nObjects*sizeof(float));
    memset(arrValues2, 0, nObjects*sizeof(float));

    nValidObjects1 = 0;
    nValidObjects2 = 0;
    for (int nObject = 0; nObject < nObjects; nObject++) {
        if (arrObjects[nObject]->isAttrValid(attr1)) {
            arrValues1[nValidObjects1] = pow(arrObjects[nObject]->attr(attr1) - exp1, 2);
            nValidObjects1++;
        }
        if (arrObjects[nObject]->isAttrValid(attr2)) {
            arrValues2[nValidObjects2] = pow(arrObjects[nObject]->attr(attr2) - exp2, 2);
            nValidObjects2++;
        }
    }

    float dispersion1 = EuclidMetric::expectation(arrValues1, nValidObjects1);
    float dispersion2 = EuclidMetric::expectation(arrValues2, nValidObjects2);

    memset(arrValues1, 0, nObjects*sizeof(float));
    nValidObjects1 = 0;

    for (int nObject = 0; nObject < nObjects; nObject++) {
        if (arrObjects[nObject]->isAttrValid(attr1) &&
                arrObjects[nObject]->isAttrValid(attr2)) {

            arrValues1[nValidObjects1] = (arrObjects[nObject]->attr(attr1) - exp1) *
                (arrObjects[nObject]->attr(attr2) - exp2);
            nValidObjects1++;
        }
    }
    float covariance = EuclidMetric::expectation(arrValues1, nValidObjects1);

    delete[] arrValues1;
    delete[] arrValues2;

    float correlation = abs(covariance / (sqrt(dispersion1) * sqrt(dispersion2)));

    int nCoefficient = 0;   //number of values known for both attributes

    for (int nObject = 0; nObject < nObjects; nObject++) {
        if (arrObjects[nObject]->isAttrValid(attr1) &&
                arrObjects[nObject]->isAttrValid(attr2))
            nCoefficient++;
    }

	return correlation*nCoefficient;
}

float EuclidMetric::expectation(float *arrValues, int nValueCount) {
    map<float, int> mp;
    map<float, int>::iterator iter;
    for (int i = 0; i < nValueCount; i++) {
        if ((iter = mp.find(arrValues[i])) != mp.end())
            iter->second++;
        else
            mp[arrValues[i]] = 1;
    }
    float result = 0;
    for (iter = mp.begin(); iter != mp.end(); iter++) {
        result += iter->second * (float)iter->first / (float)nValueCount;
    }
    return result;
}

bool EuclidMetric::compareObjectRanges(ObjectRange r1, ObjectRange r2) {
    return r1.nRange > r2.nRange;
}

bool EuclidMetric::compareAttributeRanges(AttributeRange r1, AttributeRange r2) {
    return r1.nRange > r2.nRange;
}

EuclidMetric::~EuclidMetric() {
    if (m_arrAverageDeltas)
        delete[] m_arrAverageDeltas;
    if (m_arrValidAttrCount)
        delete[] m_arrValidAttrCount;
}


ObjectRange::ObjectRange (Object *pObject, float nRange) {
    this->pObject = pObject;
    this->nRange = nRange;
}

ObjectRange::ObjectRange() {
    this->pObject = NULL;
    this->nRange = 0;
}

AttributeRange::AttributeRange (int nAttribute, float nRange) {
    this->nAttribute = nAttribute;
    this->nRange = nRange;
}

void AttributeProbability::add(float value) {
	map<float, int>::iterator i = occurrences.find(value);
	if (i != occurrences.end())
		i->second++;
	else
		occurrences[value] = 1;
}
	
int AttributeProbability::get(float value) {
	return occurrences[value];
}
