#ifndef _dbscan_h_
#define _dbscan_h_

#include "Object.h"
#include "Cluster.h"
#include "clustering.h"
#include "AbstractMetric.h"
#include <set>
#include <list>
#include <pthread.h>

struct NeighborData {
    public:
        NeighborData(Object*, AbstractMetric *, std::set<Object*>, float);

        AbstractMetric *pMetric;
        Object *pObject;
        std::set<Object*> toScan;
        std::set<Object*> result;
        float eps;
};

void* threadedNeighbors(void*);

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
