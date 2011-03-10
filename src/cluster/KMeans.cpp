#include "KMeans.h"
#include <math.h>
#include <list>
#include <time.h>
#include <stdlib.h>
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
	for (list<int>::iterator iIndex = sample.begin(); \
		iIndex != sample.end(); iIndex++, nCluster++) {
		
		_clusters[nCluster].add(*iIndex);
	}
	
	Cluster *clustersTemp = new Cluster[_clusterCount];
	for (int i = 0; i < _clusterCount; i++)
		clustersTemp[i].setContainer(_pContainer);
		
	
	double dist;
	vector<double> distances;
	distances.resize(_clusterCount);
	Object *pObj;
	bool bClustersChanged = true;
	while (bClustersChanged) {
		bClustersChanged = false;
		
		time_t start, end;
		start = time(NULL);
		int nIndexCounter = 0;
		for (list<int>::iterator iIndex = indices.begin(); \
			iIndex != indices.end(); iIndex++) {
			
			pObj = _pContainer->get(*iIndex);
			for (int nCluster = 0; nCluster < _clusterCount; nCluster++) {
				dist = pMetric->distance(*pObj, *_clusters[nCluster].center(pMetric));
				distances[nCluster] = dist;
			}
			nIndexCounter++;
			if (nIndexCounter % 1000 == 0)
				printf("%i objects processed.\r\n", nIndexCounter);
		}
		end = time(NULL);
		printf("Calculating all the distances took %i seconds.\r\n", end-start);
	}
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