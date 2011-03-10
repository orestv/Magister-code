#include "KMeans.h"
#include <math.h>
#include <list>
#include <time.h>
#include <stdlib.h>

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

void KMeans::clusterize() {
		
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