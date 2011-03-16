#include "Upgma.h"

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

void Upgma::clusterize(AbstractMetric *pMetric) {
    list<int> ids = _pContainer->ids();
    Cluster *pCluster = 0;

    for (list<int>::iterator iId = ids.begin();
            iId != ids.end(); iId++) {

        pCluster = new Cluster(_pContainer);        
        pCluster->add(*iId);
        _clusters.push_back(pCluster);
    }
}

