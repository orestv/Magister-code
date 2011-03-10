/* 
 * File:   Cluster.cpp
 * Author: ovoloshchuk
 * 
 * Created on March 9, 2011, 3:55 PM
 */

#include "Cluster.h"

Cluster::Cluster(map<int, Object*> *pObjects) {
    int nAttributeCount = 0;
    this->_pObjects = pObjects;
    _pCenter = 0;
	_centerValid = false;
    _attributeCount = nAttributeCount;
}

Cluster::Cluster(const Cluster& orig) {
}

Object* Cluster::center(AbstractMetric *pMetric) {
    if (!_centerValid || !_pCenter)
        _pCenter = calculateCenter(pMetric);
    return _pCenter;
}

list<int> Cluster::indices() {
	return _indices;
}

void Cluster::add(int index) {
	_indices.push_back(index);
	_centerValid = false;
}

void Cluster::remove(int index) {
	_indices.remove(index);
	_centerValid = false;
}

Object* Cluster::calculateCenter(AbstractMetric* pMetric) {
	_centerValid = true;
    return pMetric->center(*_pObjects, _indices);
}

Cluster::~Cluster() {
    if (_pCenter)
        delete _pCenter;
}

