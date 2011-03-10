#include "KMeans.h"
#include <math.h>
#include <list>
#include <time.h>

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
	
	list<int> indices = _pContainer->indices();
	int nRandomIndex;
	
	for (int nCluster = 0; nCluster < _clusterCount; nCluster++) {
		
	}
	
}

list<int> KMeans::randomIndices(list<int> indices, int nIndexCount) {	
	//srand(time(NULL));
	list<int> result;
	int nItemIndex = 0;
	int nRandomIndex = 0;
	for (int i = 0; i < nIndexCount; i++) {
		//nItemIndex = (int) (rand() * indices.size());
		nRandomIndex = indices[nItemIndex];
		indices.remove(nRandomIndex);
		result.push_back(nRandomIndex);
	}
	
	return result;
}