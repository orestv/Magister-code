#ifndef __nn_h__
#define __nn_h__

#include "DataContainer.h"
#include "AbstractMetric.h"
#include "Cluster.h"

class NN {
    public:
        NN(DataContainer *pDataContainer);
        void clusterize(AbstractMetric *pMetric);

        ~NN();
    private:
        DataContainer *_pContainer;
        Cluster *nearestNeighbor(Cluster*, list<Cluster*>, AbstractMetric*);
};

#endif
