/* 
 * File:   Cluster.h
 * Author: ovoloshchuk
 *
 * Created on March 9, 2011, 3:55 PM
 */

#ifndef _CLUSTER_H
#define	_CLUSTER_H

#include "Object.h"
#include "DataContainer.h"
#include "AbstractMetric.h"
#include <map>
#include <vector>

class Cluster {
public:
	Cluster();
    Cluster(DataContainer *pContainer);
    Cluster(const Cluster& orig);
    virtual ~Cluster();
	
	void setContainer(DataContainer *pContainer);

    Object *center(AbstractMetric *pMetric);
	vector<int> indices();
	
	void add(int index);
	void remove(int index);
	
	bool contains(int index);
	
	void clear();

    bool operator==(Cluster &other);

private:
    Object *calculateCenter(AbstractMetric *pMetric);

    Object *_pCenter;
    DataContainer *_pContainer;
    vector<int> _indices;
	bool _centerValid;
};

#endif	/* _CLUSTER_H */

