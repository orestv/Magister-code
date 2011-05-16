#ifndef _dbscan_h_
#define _dbscan_h_

#include "Object.h"
#include "Cluster.h"
#include "clustering.h"
#include "AbstractMetric.h"

class DBScan {
    public:
        DBScan(DataContainer *);
        ~DBScan();

        Clustering* clusterize(float eps = 0.05, int nRequiredPointCount = 3, AbstractMetric *pMetric);
    private:
        DataContainer *_pContainer;

};

#endif
