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

class EuclidMetric : public AbstractMetric {
public:
    EuclidMetric(DataContainer *);
    EuclidMetric(const EuclidMetric& orig);

    double distance(Object &o1, Object &o2, bool bUseIntegratedPrediction = true);
    Object* center(DataContainer *pContainer, list<int> &indices);

    virtual ~EuclidMetric();
private:
    DataContainer *m_pContainer;
    int m_nAttributeCount;
    double *m_arrAverageDeltas;
    int *m_arrValidAttrCount;

};

#endif	/* _EUCLIDMETRIC_H */

