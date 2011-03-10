/* 
 * File:   Cluster.h
 * Author: ovoloshchuk
 *
 * Created on March 9, 2011, 3:55 PM
 */

#ifndef _CLUSTER_H
#define	_CLUSTER_H

#include "Object.h"
#include "AbstractMetric.h"
#include <map>
#include <list>

class Cluster {
public:
    Cluster(map<int, Object*> *_pObjects);
    Cluster(const Cluster& orig);

    Object *center(AbstractMetric *pMetric);
	list<int> indices();
	
	void add(int index);
	void remove(int index);

    virtual ~Cluster();
private:
    Object *calculateCenter(AbstractMetric *pMetric);

    Object *_pCenter;
    map<int, Object*> *_pObjects;
    list<int> _indices;
    int _attributeCount;
	bool _centerValid;
};

#endif	/* _CLUSTER_H */

