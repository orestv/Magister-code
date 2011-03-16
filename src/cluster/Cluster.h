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
	list<int>& indices();
	
	void add(int index);
    void add(Cluster *pCluster);
	void remove(int index);
    list<Cluster*>& clusters();
	
	bool contains(int id);
    
    Object *get(int id);
	
	void clear();

    bool operator==(Cluster &other);
    Cluster& operator=(Cluster &other);

    static double distance(Cluster &c1, Cluster &c2, AbstractMetric *pMetric);

private:
    Object *calculateCenter(AbstractMetric *pMetric);
    void init();

    Object *_pCenter;
    DataContainer *_pContainer;
    list<int> _indices;
    list<Cluster*> _clusters;
	bool _centerValid;
};

#endif	/* _CLUSTER_H */

