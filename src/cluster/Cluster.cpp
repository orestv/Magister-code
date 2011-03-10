/* 
 * File:   Cluster.cpp
 * Author: ovoloshchuk
 * 
 * Created on March 9, 2011, 3:55 PM
 */

#include "Cluster.h"

Cluster::Cluster() {
	_centerValid = false;
	_pCenter = 0;
	_pContainer = 0;
}

Cluster::Cluster(DataContainer *pContainer) {
    _pContainer = pContainer;
    _pCenter = 0;
	_centerValid = false;
}

void Cluster::setContainer(DataContainer *pContainer) {
	_pContainer = pContainer;
	_centerValid = false;
}

Cluster::Cluster(const Cluster& orig) {
}

Object* Cluster::center(AbstractMetric *pMetric) {
	if (!_centerValid) {
		if (_pCenter)
			delete _pCenter;
        _pCenter = calculateCenter(pMetric);
	}
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

bool Cluster::contains(int index) {
	for (list<int>::iterator iIndex = _indices.begin(); \
		iIndex != _indices.end(); iIndex++) {
		
		if (*iIndex == index)
			return true;
	}
	return false;
}

Object* Cluster::calculateCenter(AbstractMetric* pMetric) {
	_centerValid = true;
    return pMetric->center(_pContainer, _indices);
}

Cluster::~Cluster() {
    if (_pCenter)
        delete _pCenter;
}

