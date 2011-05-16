#include "dbscan.h"
#include <stdio.h>
#include <stdlib.h>

DBScan::DBScan(DataContainer *pContainer) {
    this->_pContainer = pContainer;
}

DBScan::~DBScan() {
}

Clustering* DBScan::clusterize(float eps, int nRequiredPointCount, AbstractMetric *pMetric) {

}
