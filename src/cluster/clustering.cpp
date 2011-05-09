#include "clustering.h"

Clustering::Clustering(list<Cluster*> lsClusters) {
    this->_lsClusters = lsClusters;
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
}
