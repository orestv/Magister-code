#include "dbscan.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

DBScan::DBScan(DataContainer *pContainer) {
    this->_pContainer = pContainer;
    int nObjectCount = this->_pContainer->ids().size();
    for (int i = 0; i < nObjectCount; i++) {
        _remainingObjects.insert(this->_pContainer->getByIndex(i));
    }
    this->_nObjectCount = nObjectCount;
}

DBScan::~DBScan() {
}

Clustering* DBScan::clusterize(float eps, int nRequiredNeighborCount, AbstractMetric *pMetric) {
    list<Cluster*> lsClusters;
    Cluster *pCluster = 0;
    srand(time(NULL));
    int nRandomIndex = 0;
    set<Object*> currentObjects;

    Object *pObj = 0;
    while (_remainingObjects.size() > 0) {
        nRandomIndex = (rand() % _remainingObjects.size());
        if (pCluster) {
            pObj = *(currentObjects.begin());
            currentObjects.erase(currentObjects.begin());
        }
        else
            pObj = *(_remainingObjects.begin());

        set<Object*> neighbors = this->neighbors(pObj, pMetric, eps);
        if (neighbors.size() >= nRequiredNeighborCount) {
            if (pCluster == 0) {
                pCluster = new Cluster(_pContainer);
                pCluster->addObject(pObj);
            }
            for (set<Object*>::iterator iNeighbor = neighbors.begin();
                    iNeighbor != neighbors.end(); iNeighbor++) {
                if (_remainingObjects.find(*iNeighbor) != _remainingObjects.end())
                    currentObjects.insert(*iNeighbor);
            }
        }
        _remainingObjects.erase(pObj);
        if (pCluster && currentObjects.size() == 0) {
            lsClusters.push_back(pCluster);
            pCluster = 0;
        }
    }
    return new Clustering(lsClusters);
}

set<Object*> DBScan::neighbors(Object *pCurrentObject, AbstractMetric *pMetric, float eps) {
    float dist;
    Object *pObj;
    set<Object*> result;
    for (int i = 0; i < _nObjectCount; i++) {
        pObj = _pContainer->getByIndex(i);
        dist = pMetric->distance(*pObj, *pCurrentObject);
        if (dist < eps)
            result.insert(pObj);
    }
    return result;
}
