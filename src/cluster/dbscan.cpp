#include "dbscan.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <algorithm>

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

set<Object*>::iterator randomObject(set<Object*> *pList) {
    int nIndex = rand() % pList->size();
    int n = 0;
    set<Object*>::iterator iO = pList->begin();
    while (n < nIndex) {
        iO++;
        n++;
    }
    return iO;
}

Clustering* DBScan::clusterize(float eps, int nRequiredNeighborCount, AbstractMetric *pMetric) {
    list<Cluster*> lsClusters;
    Cluster *pCluster = 0;
    set<Object*> currentObjects;

    Object *pObj = 0;
    srand(time(NULL));
    timeval t_start, t_end;
    FILE *fTime = fopen("time_dbscan.txt", "w");
    while (_remainingObjects.size() > 0) {
        printf("Unscanned objects left: %i\n", _remainingObjects.size());
        gettimeofday(&t_start, 0);
        if (pCluster) {
            set<Object*>::iterator iO = currentObjects.begin();//randomObject(&currentObjects);
            pObj = *iO;
            currentObjects.erase(iO);
        }
        else {
            set<Object*>::iterator iO = _remainingObjects.begin();//randomObject(&_remainingObjects);
            pObj = *iO;
        }

        set<Object*> neighbors = this->neighbors(pObj, pMetric, eps);
        printf("Neighbor count: %i\n", neighbors.size());
        if (neighbors.size() >= nRequiredNeighborCount) {
            if (pCluster == 0) {
                printf("Starting a new cluster!\n");
                pCluster = new Cluster();
            }
            pCluster->addObject(pObj);
            set_intersection(neighbors.begin(), neighbors.end(),
                    _remainingObjects.begin(), _remainingObjects.end(),
                    inserter(currentObjects, currentObjects.begin()));
            /*
            for (set<Object*>::iterator iNeighbor = neighbors.begin();
                    iNeighbor != neighbors.end(); iNeighbor++) {
                if (_remainingObjects.find(*iNeighbor) != _remainingObjects.end())
                    currentObjects.insert(*iNeighbor);
            }
            */
        }
        _remainingObjects.erase(pObj);
        printf("Current object count: %i\n", currentObjects.size());
        if (pCluster && (currentObjects.size() == 0 || _remainingObjects.size() == 0)) {
            if (pCluster->objects().size() == 1) {
                delete pCluster;
                pCluster = 0;
                printf("Cluster only contains 1 element, deleting.\n");
            } else {
                printf("Cluster finalized.\n");
                lsClusters.push_back(pCluster);
                printf("Cluster array size: %i\n", lsClusters.size());
                pCluster = 0;
            }
        }
        gettimeofday(&t_end, 0);
        double fIterationTime = (t_end.tv_sec-t_start.tv_sec) + (double)(t_end.tv_usec-t_start.tv_usec)/1000000;
        fprintf(fTime, "%.5f\n", fIterationTime);
        printf("Iteration time: %.5f\n", fIterationTime);
        printf("\n");
    }
    fclose(fTime);
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
