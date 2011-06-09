#include "validity.h"
#include <stdio.h>
#include <pthread.h>

ThreadDistanceData::ThreadDistanceData(Cluster *pCluster1, Cluster *pCluster2, AbstractMetric *pMetric) {
    this->pCluster1 = pCluster1;
    this->pCluster2 = pCluster2;
    this->pMetric = pMetric;
    this->_distance = 0;
}

double ThreadDistanceData::distance() {
    return this->_distance;
}

void* ThreadDistanceData::threaded_distance(void *data) {
    ThreadDistanceData *pData = (ThreadDistanceData*)data;
    double dist = 0;
    for (list<Object*>::iterator iO_outer = pData->pCluster1->objects().begin();
            iO_outer != pData->pCluster1->objects().end(); iO_outer++) {
        for (list<Object*>::iterator iO_inner = pData->pCluster2->objects().begin();
                iO_inner != pData->pCluster2->objects().end(); iO_inner++) {
            double d = pData->pMetric->distance(**iO_outer, **iO_inner);
            if (dist == 0 || d < dist)
                dist = d;
        }
    }
    pData->_distance = dist;
    pthread_exit((void*)pData);
}

double Validity::dunn(Clustering &clustering, AbstractMetric *pMetric) {
    double minDist = -1, maxDiam = -1;
    double dist = 0, diam = 0;
    int nObject = 0;
    list<pthread_t> lsThreads;
    for (list<Cluster*>::iterator iC_outer = clustering.clusters().begin();
            iC_outer != clustering.clusters().end(); iC_outer++) {
        for (list<Cluster*>::iterator iC_inner = iC_outer;
                iC_inner != clustering.clusters().end(); iC_inner++) {
            if (iC_inner == iC_outer)
                continue;
            dist = 0;

            pthread_t thrd;
            ThreadDistanceData *pData = new ThreadDistanceData(*iC_outer, *iC_inner, pMetric);
            pthread_create(&thrd, NULL, ThreadDistanceData::threaded_distance, (void*)pData);
            lsThreads.push_back(thrd);
        }
        diam = (*iC_outer)->diameter(pMetric);
        if (diam > maxDiam)
            maxDiam = diam;
    }
    int nThread = 0;
    for (list<pthread_t>::iterator iThread = lsThreads.begin();
            iThread != lsThreads.end(); iThread++) {
        void *data;
        pthread_join(*iThread, &data);
        ThreadDistanceData *pData = (ThreadDistanceData*)data;
        dist = pData->distance();
        if (minDist < 0 || dist < minDist)
            minDist = dist;
        nThread++;
        printf("Joined thread %i\n", nThread);
        delete pData;
    }

    return minDist / maxDiam;
}

double Validity::bezderk(Clustering &clustering, AbstractMetric *pMetric) {
    double minDist = -1, maxDiam = -1;
    double dist = 0, diam = 0;
    for (list<Cluster*>::iterator iC_outer = clustering.clusters().begin();
            iC_outer != clustering.clusters().end(); iC_outer++) {
        for (list<Cluster*>::iterator iC_inner = iC_outer;
                iC_inner != clustering.clusters().end(); iC_inner++) {
            if (iC_inner == iC_outer)
                continue;
            dist = Cluster::distance(**iC_outer, **iC_inner, pMetric);
            if (minDist < 0 || dist < minDist)
                minDist = dist;
        }
        Object *pCenter = (*iC_outer)->center(pMetric);
        diam = 0;
        for (list<Object*>::iterator iO = (*iC_outer)->objects().begin();
                iO != (*iC_outer)->objects().end(); iO++) {
           diam += pMetric->distance(**iO, *pCenter);
        }
        diam *= 2./(double)(*iC_outer)->objects().size();
        if (diam > maxDiam)
            maxDiam = diam;
    }
}
