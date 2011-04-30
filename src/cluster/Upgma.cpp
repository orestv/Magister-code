#include "Upgma.h"
#include <iostream>
#include <pthread.h>

Upgma::Upgma(DataContainer *pContainer) {
    _pContainer = pContainer;
    _pCluster = 0;
}


Upgma::~Upgma() {
    if (_pCluster)
        delete _pCluster;
    else {
        for (list<Cluster*>::iterator iCluster = _clusters.begin();
                iCluster != _clusters.end(); iCluster++) {

            delete *iCluster;
        }
    }
}

Cluster* Upgma::result() {
    return _pCluster;
}

void* findLeastDistance(void *p) {
    Upgma::ThreadData *pData = (Upgma::ThreadData*)p;
    list<Cluster*>::iterator iOuter = pData->iStart, iInner = pData->iStart;
    iInner++;
    float dist = Cluster::distance(**iOuter, **iInner, pData->pMetric);


    for (list<Cluster*>::iterator iOuter = pData->iStart;
            iOuter != pData->plsClusters->end(); iOuter++) {

        for (list<Cluster*>::iterator iInner = iOuter;
                iInner != pData->plsClusters->end(); iInner++) {

            if (iInner == iOuter)
                continue;


            //TODO: add actual distance computation

        }
    }

    pthread_exit(pData);
    return NULL;
}

void Upgma::clusterize(AbstractMetric *pMetric) {
    list<int> ids = _pContainer->ids();
    Cluster *pCluster = 0;

    for (list<int>::iterator iId = ids.begin();
            iId != ids.end(); iId++) {

        pCluster = new Cluster(_pContainer);        
        pCluster->add(*iId);
        _clusters.push_back(pCluster);
    }

    int nThreads = 5;

    list<pthread_t> lsThreads;

    while (_clusters.size() > 0) {
        int nClustersPerThread = _clusters.size()/nThreads;
        int nCluster = 0;
        list<Cluster*>::iterator iStart, iEnd;
        iStart = _clusters.begin();
        iEnd = iStart;
        while (iEnd != _clusters.end()) {
            for (nCluster = 0; nCluster < nClustersPerThread && iEnd != _clusters.end();
                    nCluster++) {
                iEnd++;
            }
            ThreadData *pData = new ThreadData;
            pData->iStart = iStart;
            pData->iEnd = iEnd;
            pData->plsClusters = &_clusters;
            pData->pMetric = pMetric;

            pthread_t thrd;
            pthread_create(&thrd, NULL, findLeastDistance, (void*)pData);
            lsThreads.push_back(thrd);

            iStart = iEnd;

        }

        for (list<pthread_t>::iterator iThread = lsThreads.begin();
                iThread != lsThreads.end(); iThread++) {

            ThreadData *pData = NULL;
            pthread_join(*iThread, (void**)&pData);
            //TODO: process output data
            

            delete pData;
        }

        break;
    }
}

