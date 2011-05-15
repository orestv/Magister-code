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
#include <list>

class Cluster {
public:
	Cluster();
    Cluster(DataContainer *pContainer);
    Cluster(const Cluster& orig);
    virtual ~Cluster();
	
	void setContainer(DataContainer *pContainer);

    Object *center(AbstractMetric *pMetric);

    list<Object*>& objects();
	
	void addObject(Object *pObject);
    void addCluster(Cluster *pCluster);
	void remove(Object *pObject);
    list<Cluster*>& clusters();
	
	bool contains(Object *pObject);
    
    Object *get(int id);
	
	void clear();

    Cluster *getNeighbor();
    void setNeighbor(Cluster*);

    bool operator==(Cluster &other);
    Cluster& operator=(Cluster &other);

    static float distance(Cluster &c1, Cluster &c2, AbstractMetric *pMetric);
    float diameter(AbstractMetric *pMetric);

private:
    void init();

    Object *_pCenter;
    DataContainer *_pContainer;
    list<Object*> _objects;
    list<Cluster*> _clusters;
	bool _centerValid;
    Cluster *_pNeighbor;
};

#endif	/* _CLUSTER_H */

