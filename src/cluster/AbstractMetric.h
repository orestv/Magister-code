/* 
 * File:   AbstractMetric.h
 * Author: ovoloshchuk
 *
 * Created on March 9, 2011, 4:04 PM
 */

#ifndef _ABSTRACTMETRIC_H
#define	_ABSTRACTMETRIC_H

#include "Object.h"
#include "DataContainer.h"
#include <map>
#include <list>

class AbstractMetric {
public:

    virtual float distance(Object &o1, Object &o2, bool bUseIntegratedPrediction = false) = 0;
    virtual Object* center(list<Object*> &lsObjects) = 0;
    virtual void predictMissingData(DataContainer *pContainer) = 0;
private:

};

#endif	/* _ABSTRACTMETRIC_H */

