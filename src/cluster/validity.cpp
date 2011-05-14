#include "validity.h"

float Validity::dunn(Clustering &clustering, AbstractMetric *pMetric) {
    float minDist = -1, maxDiam = -1;
    float dist = 0, diam = 0;
    for (list<Cluster*>::iterator iC_outer = clustering.clusters().begin();
            iC_outer != clustering.clusters().end(); iC_outer++) {
        for (list<Cluster*>::iterator iC_inner = iC_outer;
                iC_inner != clutering.clusters().end(); iC_inner++) {
            if (iC_inner == iC_outer)
                continue;

    }
}

float Validity::bezderk(Clustering &clustering, AbstractMetric *pMetric) {
    float minDist = -1, maxDiam = -1;
    
}
