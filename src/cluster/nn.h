#ifndef __nn_h__
#define __nn_h__

#include "DataContainer.h"
#include "Cluster.h"

class NN {
    public:
        NN(DataContainer *pDataContainer);
        void clusterize();

        ~NN();
    private:
        DataContainer *_pContainer;
};

#endif
