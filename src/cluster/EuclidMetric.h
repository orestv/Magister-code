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
#include <list>

struct ObjectRange {
    Object *pObject;
    float nRange;

    ObjectRange (Object *pObject, float nRange) ;
    ObjectRange();
};

struct AttributeRange {
    int nAttribute;
    float nRange;

    AttributeRange(int nAttribute, float nRange) ;
};

struct AttributeProbability {
	map<float, int> occurrences;
	
	void add(float value); 	
	int get(float value); 
};

class EuclidMetric : public AbstractMetric {
public:
    EuclidMetric(DataContainer *);
    EuclidMetric(const EuclidMetric& orig);

    float distance(Object &o1, Object &o2, bool bUseIntegratedPrediction = false);
    Object* center(list<Object*> &lsObjects);

    static float competence(Object &o1, Object &o2);
	static float competence(int attr1, int attr2, Object** ppObjects, int nObjects);

    void predictMissingData(DataContainer *pContainer);
    void predictAttributes(Object *pObj, DataContainer *pContainer);
    void predictAttribute(Object *pObj, int nAttr, DataContainer *pContainer);
    static float expectation (float *arrValues, int nValueCount);
		
    virtual ~EuclidMetric();
private:
    DataContainer *m_pContainer;
    int m_nAttributeCount;
    float *m_arrAverageDeltas;
    int *m_arrValidAttrCount;

    static bool compareObjectRanges(ObjectRange, ObjectRange);
    static bool compareAttributeRanges(AttributeRange, AttributeRange);

};

#endif	/* _EUCLIDMETRIC_H */

