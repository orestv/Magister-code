#include "nn.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

NN::NN(DataContainer *pContainer) {
    this->_pContainer = pContainer;
}

NN::~NN() {
}

Cluster* randomCluster(list<Cluster*> &lsClusters) {
    int nRandomIndex = rand() % lsClusters.size();
    list<Cluster*>::iterator iC;
    int i = 0;
    for (iC = lsClusters.begin();
            iC != lsClusters.end();
            iC++, i++) {
        if (i == nRandomIndex) {
            return *iC;
        }
    }
}

Clustering* NN::clusterize(AbstractMetric *pMetric) {
    int nObjectCount = _pContainer->ids().size();

    list<Cluster*> lsClusters;
    for (int i = 0; i < nObjectCount; i++) {
        Cluster *pC = new Cluster(_pContainer);
        pC->addObject(_pContainer->getByIndex(i));
        lsClusters.push_back(pC);
    }
    srand(time(NULL));
    //Pick a random cluster index
    Cluster *pC = randomCluster(lsClusters);
    if (pC == NULL) {
        fprintf(stderr, "Failed to generate random cluster for NN!\n");
        return 0;
    }
    Cluster *pC0 = *(lsClusters.begin());
    list<Cluster*> lsChain;
    lsChain.push_back(pC0);
    timeval t_start, t_end;
    FILE *fTime = fopen("time_nj.txt", "w");
    while (lsClusters.size() > 1){
        gettimeofday(&t_start, 0);
        printf("Iteration!\n");
        if (lsChain.size() == 0) {
            printf("Chain is empty!\n");
            Cluster *pRandom = randomCluster(lsClusters);
            lsChain.push_back(pRandom);
            printf("Added a random element to the chain.\n");
        }
        printf("Chain size: %i\n", lsChain.size());

        list<Cluster*>::iterator preLast = lsChain.end();
        preLast--;
        pC0 = *preLast;

        pC = nearestNeighbor(pC0, lsClusters, pMetric);
        Cluster *pC2 = nearestNeighbor(pC, lsClusters, pMetric);
        printf("Neighbors located!\n");
        if (pC0 == pC2) {
            printf("Chain is full!\n");
            Cluster *pC_new = new Cluster(_pContainer);
            pC_new->addCluster(pC);
            pC_new->addCluster(pC2);
            lsClusters.push_back(pC_new);
            lsClusters.remove(pC);
            lsClusters.remove(pC2);
            lsChain.remove(pC);
            lsChain.remove(pC2);
        } else {
            printf("Chain is not full, extending.\n");
            lsChain.push_back(pC);
        }
        gettimeofday(&t_end, 0);
        double fIterationTime = (t_end.tv_sec-t_start.tv_sec) + (double)(t_end.tv_usec-t_start.tv_usec)/1000000;
        fprintf(fTime, "%.5f\n", fIterationTime);
        printf("Iteration time: %.5f\n", fIterationTime);
    }
    fclose(fTime);
    return 0;
};

Cluster *NN::nearestNeighbor(Cluster *pCluster, list<Cluster*> lsClusters, AbstractMetric *pMetric) {
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
