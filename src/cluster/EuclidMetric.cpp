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
#include <sys/time.h>

float timeSpan(timeval start, timeval end) {
    return end.tv_sec-start.tv_sec + (end.tv_usec-start.tv_usec)/1000000.;
}

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
    timeval s, e;
    for (int nObject = 0; nObject < nObjectCount; nObject++) {
        pObj = pContainer->getByIndex(nObject);
        if (nAttributeCount == 0)
            nAttributeCount = pObj->attributeCount();
        printf("Object %i of %i: ", i, nObjectCount);
        i++;

        for (int nAttr = 0; 
                nAttr < nAttributeCount;
                nAttr++) {
            if (!pObj->isAttrValid(nAttr)) {
                printf("predicting...");
                gettimeofday(&s, NULL);
                predictAttributes(pObj, pContainer);
                gettimeofday(&e, NULL);
                printf(" done! spent %.5f seconds.\n", timeSpan(s, e));
                break;
            }

        }
        printf("\n");
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
    timeval start, end;
    timeval s, e, s1, e1, s2, e2;
    gettimeofday(&start, NULL);
    list<ObjectRange> lsObjectRanges;

    float nBias = 2.;

    Object *pObj;
    float range;

    int nMaxRanges = 15;
    ObjectRange *arrRanges = new ObjectRange[nMaxRanges];
    gettimeofday(&s, NULL);
    ObjectRange *pRange;
    float dTimeSpan = 0, dTimeSpan2 = 0;
    int nObjectCount = pContainer->ids().size();
    for (int i = 0; i < nObjectCount; i++) {
        pObj = pContainer->getByIndex(i);
        if (!pObj->isAttrValid(nAttr))
            continue;

        gettimeofday(&s1, NULL);
        range = this->competence(*pObj, *pCurrentObj);
        gettimeofday(&e1, NULL);
        dTimeSpan += timeSpan(s1, e1);
        gettimeofday(&s2, NULL);
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
        gettimeofday(&e2, NULL);
        dTimeSpan2 += timeSpan(s2, e2);
        //lsObjectRanges.push_back(ObjectRange(pObj, range));
    }
    gettimeofday(&e, NULL);
    printf("Actual competence calculation time: %.4f.\n", dTimeSpan);
    printf("Time spent putting the object in place: %.4f.\n", dTimeSpan2);
    printf("Calculated competences: %.4f.\n", timeSpan(s, e));

	//Object **arrObjects = new Object*[nObjectCount];
	int nObject = 0;
    int nRangeCount = 0;
    float dValue = 0;
	for (int nRange = 0; nRange < nMaxRanges; nRange++) {
        if (arrRanges[nRange].pObject == NULL)
            continue;
        dValue += arrRanges[nRange].pObject->attr(nAttr);
        nRangeCount++;
	}
    dValue /= (float)nRangeCount;
    pCurrentObj->setAttr(nAttr, dValue);
    gettimeofday(&end, NULL);
    float d = timeSpan(start, end);
    printf("Attribute %i predicted, %.4f seconds spent.\n\n", nAttr, d);

/*
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
                lsAttrRanges.push_back(AttributeRange(i, c));
            }
        }
        lsAttrRanges.sort(EuclidMetric::compareAttributeRanges);
        list<AttributeRange>::iterator iA = lsAttrRanges.begin();
        diff = abs((++iA)->nRange - lsAttrRanges.begin()->nRange);
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

        FILE *pFile = fopen("competence.txt", "w");
        fprintf(pFile, "Object to be filled: \n");
        for (int i = 0; i < pCurrentObj->attributeCount(); i++) {
            if (pCurrentObj->isAttrValid(arrAttrs[i]))
                fprintf(pFile, "%.3f,\t", pCurrentObj->attr(arrAttrs[i]));
            else
                fprintf(pFile, "N/A,\t");
        }
        fprintf(pFile, "\nFound following objects: \n");

        Object *pO;
        for (int i = 0; i < nObjectCount; i++) {
            pO = arrObjects[i];
            for (int j = 0; j < nAttributeCount; j++) {
                if (pO->isAttrValid(i))
                    fprintf(pFile, "%.3f,\t", pO->attr(arrAttrs[j]));
                else
                    fprintf(pFile, "N/A,\t");
            }
            fprintf(pFile, "\n");
        }

        fclose(pFile);
        delete[] arrAttrs;
*/
}

float EuclidMetric::competence(Object &o1, Object &o2) {
	float nResult = 0;
	int nValidAttributes = 0;
    int nCount = o1.attributeCount();
    timeval s, e;
	for (int nAttr = 0; nAttr < nCount; nAttr++) {
		if (o1.isAttrValid(nAttr) && o2.isAttrValid(nAttr)) {
			nValidAttributes++;
			nResult += pow(o1.attr(nAttr)-o2.attr(nAttr), 2);
		}		
	}
    //gettimeofday(&s, NULL);
    //nResult = (1. - sqrt(nResult)) * nValidAttributes;
    nResult = (1. - sqrt(nResult)) * nValidAttributes;
    //gettimeofday(&e, NULL);
    //printf("Range calculated: %.8f.\n", timeSpan(s, e));

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

    delete[] arrValues1, arrValues2;

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
