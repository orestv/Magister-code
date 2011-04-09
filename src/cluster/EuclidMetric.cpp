/* 
 * File:   EuclidMetric.cpp
 * Author: ovoloshchuk
 * 
 * Created on March 9, 2011, 4:14 PM
 */

#include "EuclidMetric.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

EuclidMetric::EuclidMetric(DataContainer *pContainer) {
    m_pContainer = pContainer;
    m_nAttributeCount = 0;

    list<int> &ids = pContainer->ids();
    Object *pObj = NULL, *pPrevObj = NULL;
    m_nAttributeCount = pContainer->get(*ids.begin())->attributeCount();

    m_arrValidAttrCount = new int[m_nAttributeCount];
    m_arrAverageDeltas = new double[m_nAttributeCount];

    memset(m_arrValidAttrCount, 0, m_nAttributeCount*sizeof(int));
    memset(m_arrAverageDeltas, 0, m_nAttributeCount*sizeof(double));

    for (list<int>::iterator iID = ids.begin();
            iID != ids.end(); iID++) {

        pObj = pContainer->get(*iID);

        if (!pPrevObj) {
            pPrevObj = pObj;
            continue;
        }

        for (int i = 0; i < m_nAttributeCount; i++) {
            if (pObj->isAttrValid(i)) {
                m_arrValidAttrCount[i]++;
                m_arrAverageDeltas[i] += abs(pObj->attr(i) - pPrevObj->attr(i));
                //m_arrAverageDeltas[i] += pObj->attr(i);
            }
        }
        pPrevObj = pObj;
    }
    for (int i = 0; i < m_nAttributeCount; i++) {
        m_arrAverageDeltas[i] /= (double)m_arrValidAttrCount[i];
        //m_arrAverageDeltas[i] /= (double)ids.size();
        printf("%f, ", m_arrAverageDeltas[i]);
    }
    printf("Attribute count: %i\n", m_nAttributeCount);
}

EuclidMetric::EuclidMetric(const EuclidMetric& orig) {
    m_nAttributeCount = orig.m_nAttributeCount;
    m_arrAverageDeltas = new double[m_nAttributeCount];
    m_arrValidAttrCount = new int[m_nAttributeCount];

    memcpy(m_arrAverageDeltas, orig.m_arrAverageDeltas, m_nAttributeCount*sizeof(double));
    memcpy(m_arrValidAttrCount, orig.m_arrValidAttrCount, m_nAttributeCount*sizeof(int));

    m_pContainer = orig.m_pContainer;
}

double EuclidMetric::distance(Object& o1, Object& o2, bool bUseIntegratedPrediction) {
    double dResult = 0;
    int nAttributeCount = max(o1.attributeCount(), o2.attributeCount());
    for (int nAttr = 0; nAttr < nAttributeCount; nAttr++) {
        if (bUseIntegratedPrediction) {
            if (o1.isAttrValid(nAttr) && o2.isAttrValid(nAttr)) {
                double d1 = o1.isAttrValid(nAttr) ? o1.attr(nAttr) : m_arrAverageDeltas[nAttr];
                double d2 = o2.isAttrValid(nAttr) ? o2.attr(nAttr) : m_arrAverageDeltas[nAttr];
                dResult += pow(d1 - d2, 2);
                }
            else {
                dResult += m_arrAverageDeltas[nAttr]*m_arrAverageDeltas[nAttr];
            }
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
    double *pAttributes = new double[nAttributeCount];
    bool *pValids = new bool[nAttributeCount];
    memset(pAttributes, 0, nAttributeCount*sizeof(double));
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
        pAttributes[nAttribute] /= (double)nIndexCount;
        if (pValids[nAttribute])
            pResult->setAttr(nAttribute, pAttributes[nAttribute]);
    }
    delete[] pAttributes;
    delete[] pValids;
    return pResult;
}

EuclidMetric::~EuclidMetric() {
    if (m_arrAverageDeltas)
        delete[] m_arrAverageDeltas;
    if (m_arrValidAttrCount)
        delete[] m_arrValidAttrCount;
}

