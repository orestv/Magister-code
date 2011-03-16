#ifndef _upgma_h_
#define _upgma_h_

#include <list>
#include "Cluster.h"

using namespace std;

class Upgma {
    public:
        UPGMA(DataContainer *pContainer);
        ~UPGMA();

        void clusterize();

        Cluster* result();

    private:
        DataContainer *_pContainer;
        list<Cluster*> _clusters;

};

#endif

