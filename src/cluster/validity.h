#ifndef _validity_h_
#define _validity_h_

#include "clustering.h"
#include "AbstractMetric.h"

struct ThreadDistanceData {
    public:
        ThreadDistanceData(Cluster *, Cluster *, AbstractMetric *);
        static void* threaded_distance(void*);
        float distance();
        Cluster* pCluster1;
        Cluster* pCluster2;
        AbstractMetric *pMetric;
    private:
        float _distance;
};

class Validity {
    public:
        static float dunn(Clustering &, AbstractMetric *);
        static float bezderk(Clustering &, AbstractMetric *);

};

#endif
