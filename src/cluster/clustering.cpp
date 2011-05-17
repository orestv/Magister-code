#include "clustering.h"
#include <stdio.h>

Clustering::Clustering(list<Cluster*> lsClusters) {
    for (list<Cluster*>::iterator iC = lsClusters.begin();
            iC != lsClusters.end(); iC++) {
        Cluster *pC = new Cluster(**iC);
        _lsClusters.push_back(pC);
    }
}

Clustering::Clustering(Cluster *arrClusters, int nClusters) {
    for (int i = 0; i < nClusters; i++) {
        Cluster *pC = new Cluster(arrClusters[i]);
        _lsClusters.push_back(pC);
    }
}

Clustering::~Clustering() {
    for (list<Cluster*>::iterator iC = _lsClusters.begin();
            iC != _lsClusters.end(); iC++) {
        delete *iC;
    }
    printf("Clustering destructor!\n");
}

list<Cluster*>& Clustering::clusters() {
    return _lsClusters;
}
