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
        else {
            pObj = *(_remainingObjects.begin());
        }

        set<Object*> neighbors = this->neighbors(pObj, pMetric, eps);
        printf("Neighbor count: %i\n", neighbors.size());
        if (neighbors.size() >= nRequiredNeighborCount) {
            if (pCluster == 0) {
                printf("Starting a new cluster!\n");
                pCluster = new Cluster();
            }
            pCluster->addObject(pObj);
            for (set<Object*>::iterator iNeighbor = neighbors.begin();
                    iNeighbor != neighbors.end(); iNeighbor++) {
                if (_remainingObjects.find(*iNeighbor) != _remainingObjects.end())
                    currentObjects.insert(*iNeighbor);
            }
        }
        _remainingObjects.erase(pObj);
        printf("Current object count: %i\n", currentObjects.size());
        if (pCluster && (currentObjects.size() == 0 || _remainingObjects.size() == 0)) {
            printf("Cluster finalized.\n");
            lsClusters.push_back(pCluster);
            printf("Cluster array size: %i\n", lsClusters.size());
            pCluster = 0;
        }
        printf("\n");
    }
    return new Clustering(lsClusters);
}

void* threadedNeighbors(void* data) {
   NeighborData *pData = (NeighborData*)data;
   Object *pObj = pData->pObject;
   set<Object*> &result = pData->result;
   set<Object*> &toScan = pData->toScan;
   AbstractMetric *pMetric = pData->pMetric;
   float eps = pData->eps;

   float dist;
   for (set<Object*>::iterator iO = toScan.begin();
           iO != toScan.end(); iO++) {
       dist = pMetric->distance(*pObj, **iO);
       if (dist < eps)
           result.insert(*iO);
   }
   pthread_exit(data);
}

NeighborData::NeighborData(Object *pObject, AbstractMetric *pMetric, set<Object*> toScan, float eps) {
    this->pObject = pObject;
    this->pMetric = pMetric;
    this->toScan = toScan;
    this->eps = eps;
}

set<Object*> DBScan::neighbors(Object *pCurrentObject, AbstractMetric *pMetric, float eps) {
    float dist;
    Object *pObj;
    set<Object*> result;
    int nThreadCount = 15;
    if (nThreadCount >= _nObjectCount)
        nThreadCount = _nObjectCount / 3;
    int nObjectsPerthread = _nObjectCount / nThreadCount;
    list<pthread_t> lsThreads;
    set<Object*> toScan;
    for (int i = 0; i < _nObjectCount; i++) {
        pObj = _pContainer->getByIndex(i);
        if (pObj == pCurrentObject)
            continue;
        toScan.insert(pObj);
        if (i % nObjectsPerthread == 0) {
            pthread_t thrd;
            NeighborData *pData = new NeighborData (pCurrentObject, pMetric, toScan, eps);
            pthread_create(&thrd, NULL, threadedNeighbors, (void*)pData);
            lsThreads.push_back(thrd);
            toScan.clear();
        }
    }
    void *data;
    NeighborData *pData;
    for (list<pthread_t>::iterator iThread = lsThreads.begin();
            iThread != lsThreads.end(); iThread++) {
        pthread_join(*iThread, &data);
        pData = (NeighborData*)data;
        result.insert(pData->result.begin(), pData->result.end());
        delete pData;
    }
    return result;
}
