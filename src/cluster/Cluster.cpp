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
	_ids = orig._ids;
	_centerValid = false;
}

Cluster& Cluster::operator=(Cluster &other) {
    if (_pCenter)
        delete _pCenter;
	_pCenter = 0;
	_pContainer = other._pContainer;
	_ids = other._ids;
	_centerValid = false;
}

void Cluster::clear() {
	if (_pCenter) {
		delete _pCenter;
		_pCenter = 0;
	}
	_ids.clear();
	_centerValid = false;
}

Object* Cluster::center(AbstractMetric *pMetric) {
	if (!_centerValid) {
		if (_pCenter)
			delete _pCenter;
        //_pCenter = calculateCenter(pMetric);
        _pCenter = pMetric->center(_pContainer, _ids);
        _centerValid = true;
	}
    return _pCenter;
}

list<int>& Cluster::ids() {
    return _ids;
}

list<Cluster*>& Cluster::clusters() {
    return _clusters;
}

void Cluster::add(int id) {
	_ids.push_back(id);
	_centerValid = false;
}

void Cluster::add(Cluster *pCluster) {
    _clusters.insert(_clusters.end(), pCluster);
    _ids.insert(_ids.begin(), pCluster->ids().begin(), pCluster->ids().end());
}

void Cluster::remove(int id) {
    _ids.remove(id);
    /*
	for (list<int>::iterator iter = _ids.begin(); \
		iter != _ids.end(); iter++) {
		
		if (*iter == id) {
			_ids.remove(iter);
			return;
		}
	}
    */
	_centerValid = false;
}

bool Cluster::contains(int id) {
	for (list<int>::iterator iId = _ids.begin(); \
		iId != _ids.end(); iId++) {
		
		if (*iId == id)
			return true;
	}
	return false;
}

Object* Cluster::calculateCenter(AbstractMetric* pMetric) {
	_centerValid = true;
    return pMetric->center(_pContainer, _ids);
}

bool Cluster::operator==(Cluster &other) {
    return _ids == other._ids;
    if (_pContainer != other._pContainer)
        return false;
    /*
    if (_ids.size() != other._ids.size()) {
        std::cout<<"Diff: "<<_ids.size()-other._ids.size()<<" elements"<<std::endl;
        return false;
    }
    */
    bool bFound = false;
    for (list<int>::iterator idOuter = _ids.begin(); idOuter != _ids.end(); idOuter++) {
        bFound = false;

        for (list<int>::iterator idInner = other._ids.begin(); idInner != other._ids.end(); idInner++) {
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

Object *Cluster::get(int id) {
    return _pContainer->get(id);
}

float Cluster::distance(Cluster &c1, Cluster &c2, AbstractMetric *pMetric) {
    float result = 0.;
    list<int> ids1 = c1.ids();
    list<int> ids2 = c2.ids();

    for (list<int>::iterator iOuter = ids1.begin();
            iOuter != ids1.end(); iOuter++) {

        for (list<int>::iterator iInner = ids2.begin();
                iInner != ids2.end(); iInner++) {
            result += pMetric->distance(*c1.get(*iOuter), *c2.get(*iInner));
        }
    }
    result /= (float) (ids1.size() + ids2.size());
    return result;
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

