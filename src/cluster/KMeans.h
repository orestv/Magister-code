#ifndef _kmeans_h_
#define _kmeans_h_

#include "DataContainer.h"
#include "AbstractMetric.h"
#include "Cluster.h"
#include "clustering.h"
#include <list>

class KMeans {
    public:
		KMeans(DataContainer *pContainer, int clusterCount);
		~KMeans();
		
		Clustering* clusterize(AbstractMetric *pMetric);
		
		Cluster* clusters();
		int clusterCount();
		
		static list<int> randomSample(list<int> indices, int nIndexCount);
		
    private:
        DataContainer *_pContainer;
		Cluster *_clusters;
		int _clusterCount;

};

#endif
