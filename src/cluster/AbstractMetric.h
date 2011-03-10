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
#include <vector>

class AbstractMetric {
public:

    virtual double distance(Object &o1, Object &o2){};
    virtual Object* center(DataContainer *pContainer, \
        vector<int> &indices){};
private:

};

#endif	/* _ABSTRACTMETRIC_H */

