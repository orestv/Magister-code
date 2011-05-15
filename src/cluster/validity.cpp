#include "validity.h"
#include <stdio.h>

float Validity::dunn(Clustering &clustering, AbstractMetric *pMetric) {
    float minDist = -1, maxDiam = -1;
    float dist = 0, diam = 0;
    int nObject = 0;
    for (list<Cluster*>::iterator iC_outer = clustering.clusters().begin();
            iC_outer != clustering.clusters().end(); iC_outer++) {
        for (list<Cluster*>::iterator iC_inner = iC_outer;
                iC_inner != clustering.clusters().end(); iC_inner++) {
            if (iC_inner == iC_outer)
                continue;
            dist = 0;

            for (list<Object*>::iterator iO_outer = (*iC_outer)->objects().begin();
                    iO_outer != (*iC_outer)->objects().end(); iO_outer++) {
                for (list<Object*>::iterator iO_inner = (*iC_inner)->objects().begin();
                            iO_inner != (*iC_inner)->objects().end(); iO_inner++) {
                    float d = pMetric->distance(**iO_outer, **iO_inner);
                    if (dist == 0 || d < dist)
                        dist = d;
                    nObject++;
                    if (nObject % 1000000 == 0)
                        printf("Object pair # %i processed!\n", nObject);
                }
            }
            if (minDist < 0 || dist < minDist)
                minDist = dist;
        }
        diam = (*iC_outer)->diameter(pMetric);
        if (diam > maxDiam)
            maxDiam = diam;
    }
    return minDist / maxDiam;
}

float Validity::bezderk(Clustering &clustering, AbstractMetric *pMetric) {
    float minDist = -1, maxDiam = -1;
    float dist = 0, diam = 0;
    for (list<Cluster*>::iterator iC_outer = clustering.clusters().begin();
            iC_outer != clustering.clusters().end(); iC_outer++) {
        for (list<Cluster*>::iterator iC_inner = iC_outer;
                iC_inner != clustering.clusters().end(); iC_inner++) {
            if (iC_inner == iC_outer)
                continue;
            dist = Cluster::distance(**iC_outer, **iC_inner, pMetric);
            if (minDist < 0 || dist < minDist)
                minDist = dist;
        }
        Object *pCenter = (*iC_outer)->center(pMetric);
        diam = 0;
        for (list<Object*>::iterator iO = (*iC_outer)->objects().begin();
                iO != (*iC_outer)->objects().end(); iO++) {
           diam += pMetric->distance(**iO, *pCenter);
        }
        diam *= 2./(float)(*iC_outer)->objects().size();
        if (diam > maxDiam)
            maxDiam = diam;
    }
}
