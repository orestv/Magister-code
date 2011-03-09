/* 
 * File:   Cluster.cpp
 * Author: ovoloshchuk
 * 
 * Created on March 9, 2011, 3:55 PM
 */

#include "Cluster.h"

Cluster::Cluster(map<int, Object*> *pObjects, int nAttributeCount) {
    this->_pObjects = pObjects;
    _pCenter = 0;
    _attributeCount = nAttributeCount;
}

Cluster::Cluster(const Cluster& orig) {
}

Object* Cluster::center(AbstractMetric *pMetric) {
    if (!_pCenter)
        _pCenter = calculateCenter(pMetric);
    return _pCenter;
}

Object* Cluster::calculateCenter(AbstractMetric* pMetric) {
    return pMetric->center(*_pObjects, _indices);
}

Cluster::~Cluster() {
    if (_pCenter)
        delete _pCenter;
}

