#include "Upgma.h"
#include <stdio.h>
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
    list<Cluster*>::iterator iOuter = pData->plsClusters->begin();
    while (iOuter != pData->plsClusters->end() && *iOuter != pData->pStart)
        iOuter++;
    list<Cluster*>::iterator iInner = iOuter;
    iInner++;
    float dist = Cluster::distance(**iOuter, **iInner, pData->pMetric);
    pData->nResult = dist;
    pData->pResult1 = *iOuter;
    pData->pResult2 = *iInner;

    for (; iOuter != pData->plsClusters->end() && *iOuter != pData->pEnd; 
            iOuter++) {

        for (iInner = iOuter; iInner != pData->plsClusters->end(); iInner++) {

            if (iInner == iOuter)
                continue;


            dist = Cluster::distance(**iOuter, **iInner, pData->pMetric);
            if (dist < pData->nResult) {
                pData->nResult = dist;
                pData->pResult1 = *iOuter;
                pData->pResult2 = *iInner;
            }
        }
    }

    pthread_exit(pData);
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

    int nThreads = 8;


    while (_clusters.size() > 2) {
        list<pthread_t> lsThreads;
        int nClusters = _clusters.size(), nClustersPerThread = nClusters / nThreads;
        list<Cluster*>::iterator iStart, iEnd, iTotalEnd;
        iStart = _clusters.begin();
        iEnd = _clusters.begin();
        iTotalEnd = _clusters.end(); iTotalEnd--;
        int nProcessedClusters = 0;

        while (iEnd != iTotalEnd && nProcessedClusters < nClusters) {
            int nCluster;
            for (nCluster = 1;
                    nCluster < nClustersPerThread && iEnd != iTotalEnd;
                    nCluster++) {
                iEnd++;
            }
            nProcessedClusters += nCluster;
            //fprintf(stderr, "Processed %i of %i clusters.\n", nProcessedClusters, nClusters);
            if (nClusters - nProcessedClusters > 0 && nClusters - nProcessedClusters < 3)
                iEnd = iTotalEnd;

            ThreadData *pData = new ThreadData;
            pData->pStart = *iStart;
            pData->pEnd = *iEnd;
            pData->pMetric = pMetric;
            pData->plsClusters = &_clusters;

            pthread_t thrd;
            pthread_create(&thrd, NULL, findLeastDistance, (void*)pData);

            lsThreads.push_back(thrd);
            iStart = iEnd;
        }


        Cluster *pC1 = NULL, *pC2 = NULL;
        float dist = 0;

        int nThread = 1;
        for (list<pthread_t>::iterator iThread = lsThreads.begin();
                iThread != lsThreads.end(); iThread++) {

            ThreadData *pData = NULL;
            pthread_join(*iThread, (void**)&pData);
            fprintf(stderr, "Thread %i exited!\n", nThread);

            if (pC1 == NULL || pC2 == NULL || pData->nResult < dist) {
                pC1 = pData->pResult1;
                pC2 = pData->pResult2;
                dist = pData->nResult;
            }

            delete pData;
            nThread++;
        }
        fprintf(stderr, "Smallest distance detected: %.8f.\n", dist);

        if (pC1 == NULL || pC2 == NULL) { 
            fprintf(stderr, "fail!\n");
            return;
        }
        Cluster *pC = new Cluster(_pContainer);
        pC->add(pC1);
        pC->add(pC2);;
        _clusters.remove(pC1);
        _clusters.remove(pC2);
        _clusters.push_back(pC);
        fprintf(stderr, "Current cluster list size: %i.\n\n", _clusters.size());
    }
}

