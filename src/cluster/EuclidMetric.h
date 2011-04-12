/* 
 * File:   EuclidMetric.h
 * Author: ovoloshchuk
 *
 * Created on March 9, 2011, 4:14 PM
 */

#ifndef _EUCLIDMETRIC_H
#define	_EUCLIDMETRIC_H

#include "AbstractMetric.h"
#include "DataContainer.h"

struct ObjectRange {
    Object *pObject;
    double nRange;

    ObjectRange (Object *pObject, double nRange) {
        this->pObject = pObject;
        this->nRange = nRange;
    }
};

struct AttributeRange {
    int nAttribute;
    double nRange;

    AttributeRange(int nAttribute, double nRange) {
        this->nAttribute = nAttribute;
        this->nRange = nRange;
    }
};

class EuclidMetric : public AbstractMetric {
public:
    EuclidMetric(DataContainer *);
    EuclidMetric(const EuclidMetric& orig);

    double distance(Object &o1, Object &o2, bool bUseIntegratedPrediction = false);
    Object* center(DataContainer *pContainer, list<int> &indices);

    void predictMissingData(DataContainer *pContainer);
    void predictAttributes(Object *pObj, DataContainer *pContainer);
    void predictAttribute(Object *pObj, int nAttr, DataContainer *pContainer);
		
    virtual ~EuclidMetric();
private:
    DataContainer *m_pContainer;
    int m_nAttributeCount;
    double *m_arrAverageDeltas;
    int *m_arrValidAttrCount;

    static bool compareObjectRanges(ObjectRange, ObjectRange);
    static bool compareAttributeRanges(AttributeRange, AttributeRange);

};

#endif	/* _EUCLIDMETRIC_H */

