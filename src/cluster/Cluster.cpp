/* 
 * File:   Cluster.cpp
 * Author: ovoloshchuk
 * 
 * Created on March 9, 2011, 3:55 PM
 */

#include "Cluster.h"
#include <iostream>

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
    if (_pCenter)
        delete _pCenter;
	_pCenter = 0;
	_pContainer = orig._pContainer;
	_indices = orig._indices;
	_centerValid = false;
}

Cluster& Cluster::operator=(Cluster &other) {
    if (_pCenter)
        delete _pCenter;
	_pCenter = 0;
	_pContainer = other._pContainer;
	_indices = other._indices;
	_centerValid = false;
}

void Cluster::clear() {
	if (_pCenter) {
		delete _pCenter;
		_pCenter = 0;
	}
	_indices.clear();
	_centerValid = false;
}

Object* Cluster::center(AbstractMetric *pMetric) {
	if (!_centerValid) {
		if (_pCenter)
			delete _pCenter;
        //_pCenter = calculateCenter(pMetric);
        _pCenter = pMetric->center(_pContainer, _indices);
        _centerValid = true;
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
    /*
	for (list<int>::iterator iter = _indices.begin(); \
		iter != _indices.end(); iter++) {
		
		if (*iter == index) {
			_indices.remove(iter);
			return;
		}
	}
    */
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

bool Cluster::operator==(Cluster &other) {
    return _indices == other._indices;
    if (_pContainer != other._pContainer)
        return false;
    /*
    if (_indices.size() != other._indices.size()) {
        std::cout<<"Diff: "<<_indices.size()-other._indices.size()<<" elements"<<std::endl;
        return false;
    }
    */
    bool bFound = false;
    for (list<int>::iterator idOuter = _indices.begin(); idOuter != _indices.end(); idOuter++) {
        bFound = false;

        for (list<int>::iterator idInner = other._indices.begin(); idInner != other._indices.end(); idInner++) {
            if (*idOuter == *idInner) {
                bFound = true;
                break;
            }
        }
        std::cout<<bFound<<std::endl;
        if (!bFound)
            return false;
    }
    return true;
}

Cluster::~Cluster() {
    if (_pCenter)
        delete _pCenter;
}

