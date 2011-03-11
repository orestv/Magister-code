#include "KMeans.h"
#include <math.h>
#include <list>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <vector>
#include <iostream>

using namespace std;

KMeans::KMeans(DataContainer *pContainer, int clusterCount) {
	_pContainer = pContainer;
	_clusterCount = clusterCount;
	_clusters = new Cluster[clusterCount];
	for (Cluster *pCluster = _clusters; \
			pCluster != _clusters + clusterCount; pCluster++) {
		
		pCluster->setContainer(pContainer);
	}
}

KMeans::~KMeans() {
	if (_clusters)
		delete[] _clusters;
}

int KMeans::clusterCount() {
	return _clusterCount;
}

Cluster* KMeans::clusters() {
	return _clusters;
}

void KMeans::clusterize(AbstractMetric *pMetric) {
	list<int> indices = _pContainer->indices();
	
	list<int> sample = KMeans::randomSample(indices, _clusterCount);
	int nCluster = 0;
	for (list<int>::iterator iObjectId = sample.begin(); \
		iObjectId != sample.end(); iObjectId++, nCluster++) {
		
		_clusters[nCluster].add(*iObjectId);
	}
	
	Cluster *pTempClusters = new Cluster[_clusterCount];
	for (int i = 0; i < _clusterCount; i++)
		pTempClusters[i].setContainer(_pContainer);
	
	double dist, minDist;
	int nSelectedCluster = 0;
	vector<double> distances;
	distances.resize(_clusterCount);
	Object *pObj;
	
	int nClusterChanges = 0;
    int nClusterChangeTreshold = 500;
	
	bool bClustersChanged = true;
	while (bClustersChanged) {
		nClusterChanges = 0;
		bClustersChanged = false;
		
		time_t start, end;
		start = time(NULL);
		int nIndexCounter = 0;
		for (list<int>::iterator iObjectId = indices.begin(); \
			iObjectId != indices.end(); iObjectId++) {			
			pObj = _pContainer->get(*iObjectId);
			
			minDist = -1;
			nCluster = 0;
			nSelectedCluster = 0;
			for (nCluster = 0; nCluster < _clusterCount; nCluster++) {
				dist = pMetric->distance(*pObj, *_clusters[nCluster].center(pMetric));
				distances[nCluster] = dist;
				if (minDist < 0 || dist < minDist) {
					nSelectedCluster = nCluster;
					minDist = dist;
				}
			}
			
			//if (!bClustersChanged && !_clusters[nSelectedCluster].contains(*iObjectId))
            /*
			if (nClusterChanges < nClusterChangeTreshold && !_clusters[nSelectedCluster].contains(*iObjectId)) {
				bClustersChanged = true;
				nClusterChanges++;
			}
            */
				
			pTempClusters[nSelectedCluster].add(*iObjectId);
			
			nIndexCounter++;
			if (nIndexCounter % 10000 == 0)
				printf("%i objects processed.\r\n", nIndexCounter);
		}
		end = time(NULL);
		printf("Calculating all the distances took %i seconds.\r\n", (int)(end-start));
        printf("Calculating differences...\r\n");
        start = time(NULL);

        for (nCluster = 0; nCluster < _clusterCount; nCluster++) {
            if (! (_clusters[nCluster] == pTempClusters[nCluster])) {
                bClustersChanged = true;
                break;
            }
        }
        end = time(NULL);
        printf("Differences calculated, %i seconds spent.\r\n", (int)(end-start));
        if (bClustersChanged)
            printf("Differences found!\r\n");
        else
            printf("No differences found!\r\n");

		for (nCluster = 0; nCluster < _clusterCount; nCluster++) {
			_clusters[nCluster] = pTempClusters[nCluster];
			pTempClusters[nCluster].clear();
		}
	}
    printf("Done!\r\n");
	delete[] pTempClusters;
}

list<int> KMeans::randomSample(list<int> indices, int nIndexCount) {	
	list<int> result;
	int nItemIndex = 0;
	int nRandomIndex = 0;
	list<int>::iterator iList;
	int nTemp = 0;
	for (int i = 0; i < nIndexCount; i++) {
		nItemIndex = (rand() % indices.size());
		nTemp = 0;
		for (iList = indices.begin(); iList != indices.end() && nTemp < nItemIndex; \
			iList++, nTemp++) {} 
		nRandomIndex = *iList;		//Attention! Might be outside the list!
		indices.remove(nRandomIndex);
		result.push_back(nRandomIndex);
	}
	
	return result;
}
