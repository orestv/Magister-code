#include "nn.h"
#include <time.h>
#include <stdlib.h>

NN::NN(DataContainer *pContainer) {
    this->_pContainer = pContainer;
}

NN::~NN() {
}

void NN::clusterize(AbstractMetric *pMetric) {
    int nObjectCount = _pContainer->ids().size();

    list<Cluster*> lsClusters;
    for (int i = 0; i < nObjectCount; i++) {
        Cluster *pC = new Cluster(_pContainer);
        pC->add(i);
        lsClusters.push_back(pC);
    }
    srand(time(NULL));
    //Pick a random cluster index
    int nRandomIndex = rand() % nObjectCount;
    Cluster *pC = NULL, pC2 = NULL;
    //Find the cluster for that index
    {
        list<Cluster*>::iterator iC;
        int i = 0;
        for (iC = lsClusters.begin();
                iC != lsClusters.end() && i < nObjectCount;
                iC++, i++) {
            if (i == nRandomIndex) {
                pC = *iC;
                break;
            }
        }
    }
    if (pC == NULL) {
        fprintf(stderr, "Failed to generate random cluster for NN!\n");
        return;
    }
    Cluster *pC0 = *lsClusters.begin();
    while (lsClusters.size() > 1){
        pC = nearestNeighbor(pC0, lsClusters, pMetric);
        pC2 = nearestNeighbor(pC, lsClusters, pMetric);
        if (pC0 == pC2) {
            Cluster *pC_new = new Cluster(pMetric);
            pC_new.add(pC);
            pC_new.add(pC2);
            lsClusters.push_back(pC_new);
            lsClusters.remove(pC);
            lsClusters.remove(*iC);
            pC0 = lsClusters.begin();
        } else {
            pC0 = pC2;
        }
    }
};

Cluster *nearestNeighbor(Cluster *pCluster, list<Cluster*> lsClusters, AbstractMetric *pMetric) {
    float minDist = -1;
    float dist;
    Cluster *pRet = NULL;
    for (list<Cluster*>::iterator iC = lsClusters.begin();
            iC != lsClusters.end(); iC++) {
        if (*iC == pCluster)
            continue;
        dist = Cluster::distance(*pCluster, **iC, pMetric);
        if (dist < minDist || pRet == NULL) {
            pRet = *iC;
            minDist = dist;
        }
    }
    return pRet;
};
