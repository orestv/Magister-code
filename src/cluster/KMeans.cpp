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
	list<int> ids = _pContainer->ids();
	
	list<int> sample = KMeans::randomSample(ids, _clusterCount);
    printf("ID selection for random sample: ");
    for (list<int>::iterator iEl = sample.begin();
            iEl != sample.end(); iEl++) {

        printf("%i, ", *iEl);
    }
    printf("\n");
	int nCluster = 0;
	for (list<int>::iterator iObjectId = sample.begin(); \
		iObjectId != sample.end(); iObjectId++, nCluster++) {
		
		_clusters[nCluster].add(*iObjectId);
	}
	
	Cluster *pTempClusters = new Cluster[_clusterCount];
	for (int i = 0; i < _clusterCount; i++)
		pTempClusters[i].setContainer(_pContainer);
	
	float dist, minDist;
	int nSelectedCluster = 0;
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
		for (list<int>::iterator iObjectId = ids.begin(); \
			iObjectId != ids.end(); iObjectId++) {			
			pObj = _pContainer->get(*iObjectId);
			
			minDist = -1;
			nCluster = 0;
			nSelectedCluster = 0;
            Object *pCenter = NULL;
			for (nCluster = 0; nCluster < _clusterCount; nCluster++) {
                pCenter = _clusters[nCluster].center(pMetric);

				dist = pMetric->distance(*pObj, *_clusters[nCluster].center(pMetric));
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

        int nChangedClusters = 0;

        for (nCluster = 0; nCluster < _clusterCount; nCluster++) {
            if (! (_clusters[nCluster] == pTempClusters[nCluster])) {
                bClustersChanged = true;
                nChangedClusters++;
                //break;
            }
        }
        end = time(NULL);
        printf("Differences calculated, %i seconds spent. Clusters out of order: %i\r\n", (int)(end-start), nChangedClusters);
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
    FILE *pFile = 0;
    char *filename = "results2.txt";
    pFile = fopen(filename, "w");
    int *pActualClasses = new int[_clusterCount];
    memset(pActualClasses, 0, _clusterCount*sizeof(int));
    for (nCluster = 0; nCluster < _clusterCount; nCluster++) {
        memset(pActualClasses, 0, _clusterCount*sizeof(int));
        list<int> ids = _clusters[nCluster].ids();
        fprintf(pFile, "Cluster %i: ", nCluster);
        for (list<int>::iterator iId = ids.begin();
                iId != ids.end(); iId++) {

            pActualClasses[_pContainer->get(*iId)->actualClass()]++;
        }
        for (int nClusterInner = 0; nClusterInner < _clusterCount; nClusterInner++) {
            fprintf(pFile, "%i: %i, ", nClusterInner, pActualClasses[nClusterInner]);
        }
        fprintf(pFile, "\n");
    }
    delete[] pActualClasses;
    fclose(pFile);
}

list<int> KMeans::randomSample(list<int> ids, int nIndexCount) {	
	list<int> result;
	int nItemIndex = 0;
	int nRandomIndex = 0;
	list<int>::iterator iList;
	int nTemp = 0;
    srand(time(NULL));
	for (int i = 0; i < nIndexCount; i++) {
		nItemIndex = (rand() % ids.size());
		nTemp = 0;
		for (iList = ids.begin(); iList != ids.end() && nTemp < nItemIndex; \
			iList++, nTemp++) {} 
		nRandomIndex = *iList;		//Attention! Might be outside the list!
		ids.remove(nRandomIndex);
		result.push_back(nRandomIndex);
	}
	
	return result;
}
