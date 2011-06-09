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
        NeighborData(Object*, AbstractMetric *, std::set<Object*>, double);

        AbstractMetric *pMetric;
        Object *pObject;
        std::set<Object*> toScan;
        std::set<Object*> result;
        double eps;
};

void* threadedNeighbors(void*);

class DBScan {
    public:
        DBScan(DataContainer *);
        ~DBScan();

        Clustering* clusterize(double eps, int nRequiredNeighborCount, AbstractMetric *pMetric);
        std::set<Object*> neighbors(Object *, AbstractMetric *, double);
        
    private:
        DataContainer *_pContainer;
        std::set<Object*> _remainingObjects;
        int _nObjectCount;

};

#endif
