/* 
 * File:   Cluster.cpp
 * Author: ovoloshchuk
 * 
 * Created on March 9, 2011, 3:55 PM
 */

#include "Cluster.h"
#include <iostream>

void Cluster::init(){
	_centerValid = false;
	_pCenter = 0;
	_pContainer = 0;
}

Cluster::Cluster() {
    init();
}

Cluster::Cluster(DataContainer *pContainer) {
    init();
    _pContainer = pContainer;
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
    _objects = orig._objects;
	_centerValid = false;
}

Cluster& Cluster::operator=(Cluster &other) {
    if (_pCenter)
        delete _pCenter;
	_pCenter = 0;
	_pContainer = other._pContainer;
    _objects = other._objects;
	_centerValid = false;
}

void Cluster::clear() {
	if (_pCenter) {
		delete _pCenter;
		_pCenter = 0;
	}
    _objects.clear();
	_centerValid = false;
}

Object* Cluster::center(AbstractMetric *pMetric) {
	if (!_centerValid) {
		if (_pCenter)
			delete _pCenter;
        //_pCenter = calculateCenter(pMetric);
        _pCenter = pMetric->center(_objects);
        _centerValid = true;
	}
    return _pCenter;
}

list<Object*>& Cluster::objects() {
    return _objects;
}

list<Cluster*>& Cluster::clusters() {
    return _clusters;
}

void Cluster::addObject(Object *pObject) {
    _objects.push_back(pObject);
	_centerValid = false;
}

void Cluster::addCluster(Cluster *pCluster) {
    _clusters.insert(_clusters.end(), pCluster);
    _objects.insert(_objects.end(), pCluster->objects().begin(), pCluster->objects().end());
}

void Cluster::remove(Object *pObject) {
    _objects.remove(pObject);
	_centerValid = false;
}

bool Cluster::contains(Object *pObject) {
	for (list<Object*>::iterator iObject = _objects.begin(); \
		iObject != _objects.end(); iObject++) {
		
		if (*iObject == pObject)
			return true;
	}
	return false;
}

Object* Cluster::calculateCenter(AbstractMetric* pMetric) {
	_centerValid = true;
    return pMetric->center(_objects);
}

bool Cluster::operator==(Cluster &other) {
    return _objects == other._objects;
    /*
    if (_pContainer != other._pContainer)
        return false;
    bool bObjectFound = false;
    for (list<Object*>::iterator iObjectOuter = _objects.begin(); iObjectOuter != _objects.end(); iObjectOuter++) {
        bObjectFound = false;

        for (list<Object*>::iterator iObjectInner = other._objects.begin(); iObjectInner != other._objects.end(); iObjectInner++) {
            if (*iObjectOuter == *iObjectInner) {
                bObjectFound = true;
                break;
            }
        }
        if (!bObjectFound)
            return false;
    }
    return true;
    */
}

Object *Cluster::get(int id) {
    return _pContainer->getByIndex(id);
}

float Cluster::distance(Cluster &c1, Cluster &c2, AbstractMetric *pMetric) {
    float result = 0.;
    list<Object*> lsObjects1 = c1.objects();
    list<Object*> lsObjects2 = c2.objects();

    for (list<Object*>::iterator iOuter = lsObjects1.begin();
            iOuter != lsObjects1.end(); iOuter++) {

        for (list<Object*>::iterator iInner = lsObjects2.begin();
                iInner != lsObjects2.end(); iInner++) {
            result += pMetric->distance(**iOuter, **iInner);
        }
    }
    result /= (float) (lsObjects1.size() + lsObjects2.size());
    return result;
}

Cluster* Cluster::getNeighbor() {
    return _pNeighbor;
}

void Cluster::setNeighbor(Cluster *pNeighbor) {
    _pNeighbor = pNeighbor;
}

Cluster::~Cluster() {
    if (_pCenter)
        delete _pCenter;
    if (!_clusters.empty()) {
        for (list<Cluster*>::iterator iCluster = _clusters.begin();
                iCluster != _clusters.end(); iCluster++) {

            if (*iCluster != 0)
                delete *iCluster;
        }
    }
}

