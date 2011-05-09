#ifndef _clustering_h_
#define _clustering_h_

#include "Cluster.h"
#include <list>
using namespace std;

class Clustering {
    public:
        Clustering(list<Cluster*>);
        Clustering(Cluster*, int);

        ~Clustering();
    private:
        list<Cluster*> _lsClusters;
};

#endif
