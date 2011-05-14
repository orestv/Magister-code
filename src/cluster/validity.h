#ifndef _validity_h_
#define _validity_h_

#include "clustering.h"
#include "AbstractMetric.h"

class Validity {
    public:
        static float dunn(Clustering &, AbstractMetric *);
        static float bezderk(Clustering &, AbstractMetric *);

};

#endif
