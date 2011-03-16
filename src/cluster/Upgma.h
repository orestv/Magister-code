#ifndef _upgma_h_
#define _upgma_h_

#include <list>
#include "Cluster.h"

using namespace std;

class Upgma {
    public:
        Upgma(DataContainer *pContainer);
        ~Upgma();

        void clusterize(AbstractMetric *pMetric);

        Cluster* result();

    private:
        DataContainer *_pContainer;
        list<Cluster*> _clusters;
        Cluster *_pCluster;

};

#endif

