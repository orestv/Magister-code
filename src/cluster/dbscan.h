#ifndef _dbscan_h_
#define _dbscan_h_

#include "Object.h"
#include "Cluster.h"
#include "clustering.h"
#include "AbstractMetric.h"
#include <set>
#include <list>

class DBScan {
    public:
        DBScan(DataContainer *);
        ~DBScan();

        Clustering* clusterize(float eps, int nRequiredNeighborCount, AbstractMetric *pMetric);
        std::set<Object*> neighbors(Object *, AbstractMetric *, float);
        
    private:
        DataContainer *_pContainer;
        std::set<Object*> _remainingObjects;
        int _nObjectCount;

};

#endif
