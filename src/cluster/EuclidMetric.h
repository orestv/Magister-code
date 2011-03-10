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
    EuclidMetric();
    EuclidMetric(const EuclidMetric& orig);

    double distance(Object &o1, Object &o2);
    Object* center(DataContainer *pContainer, list<int> &indices);

    virtual ~EuclidMetric();
private:

};

#endif	/* _EUCLIDMETRIC_H */

