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
    set<Object*> currentObjects;

    Object *pObj = 0;
    while (_remainingObjects.size() > 0) {
        printf("Unscanned objects left: %i\n", _remainingObjects.size());
        if (pCluster) {
            pObj = *(currentObjects.begin());
            currentObjects.erase(currentObjects.begin());
        }
        else
            pObj = *(_remainingObjects.begin());

        set<Object*> neighbors = this->neighbors(pObj, pMetric, eps);
        printf("Neighbor count: %i\n", neighbors.size());
        if (neighbors.size() >= nRequiredNeighborCount) {
            if (pCluster == 0) {
                printf("Starting a new cluster!\n");
                pCluster = new Cluster(_pContainer);
                pCluster->addObject(pObj);
            } else 
                pCluster->addObject(pObj);
            for (set<Object*>::iterator iNeighbor = neighbors.begin();
                    iNeighbor != neighbors.end(); iNeighbor++) {
                if (_remainingObjects.find(*iNeighbor) != _remainingObjects.end())
                    currentObjects.insert(*iNeighbor);
            }
        }
        printf("Current object count: %i\n", currentObjects.size());
        if (pCluster && currentObjects.size() == 0) {
            printf("Cluster finalized.\n");
            lsClusters.push_back(pCluster);
            pCluster = 0;
        }
        _remainingObjects.erase(pObj);
        printf("\n");
    }
    return new Clustering(lsClusters);
}

set<Object*> DBScan::neighbors(Object *pCurrentObject, AbstractMetric *pMetric, float eps) {
    float dist;
    Object *pObj;
    set<Object*> result;
    for (int i = 0; i < _nObjectCount; i++) {
        pObj = _pContainer->getByIndex(i);
        if (pObj == pCurrentObject)
            continue;
        dist = pMetric->distance(*pObj, *pCurrentObject);
        if (dist < eps)
            result.insert(pObj);
    }
    return result;
}
