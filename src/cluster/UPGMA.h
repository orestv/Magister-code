#ifndef _upgma_h_
#define _upgma_h_

#include <list>
#include "Cluster.h"

using namespace std;

class UPGMA {
    public:
        UPGMA(DataContainer *pContainer);
        ~UPGMA();

        void clusterize();


    private:
        list<Cluster*> _clusters;

};

#endif

