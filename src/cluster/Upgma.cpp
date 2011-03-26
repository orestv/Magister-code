#include "Upgma.h"
#include <iostream>

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
    int nClusterCount = ids.size();
    list<Cluster*>::iterator iJoin1, iJoin2;
    iJoin1 = _clusters.begin();
    iJoin2 = _clusters.begin();
    iJoin2++;
    int nDist = Cluster::distance(**iJoin1, **iJoin2, pMetric);
    int nCurrentDist;
    std::cout<<"Distance: "<<nDist<<std::endl;

    list<Cluster*>::iterator iOuter, iInner;
    int n = 0;
    for (iOuter = _clusters.begin(); iOuter != _clusters.end();
            iOuter++) {
        for (iInner = iOuter; iInner != _clusters.end();
                iInner++) {
            if (iOuter == iInner)
                continue;
            nCurrentDist = Cluster::distance(**iOuter, **iInner, pMetric);
        }
        n++;
        std::cout<<"Another cluster done: "<<n<<std::endl;
    }

    while (_clusters.size() > 0) {

        break;
    }
}

