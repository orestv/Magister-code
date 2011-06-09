#ifndef _validity_h_
#define _validity_h_

#include "clustering.h"
#include "AbstractMetric.h"

struct ThreadDistanceData {
    public:
        ThreadDistanceData(Cluster *, Cluster *, AbstractMetric *);
        static void* threaded_distance(void*);
        double distance();
        Cluster* pCluster1;
        Cluster* pCluster2;
        AbstractMetric *pMetric;
    private:
        double _distance;
};

class Validity {
    public:
        static double dunn(Clustering &, AbstractMetric *);
        static double bezderk(Clustering &, AbstractMetric *);

};

#endif
