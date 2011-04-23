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
	list<int>& ids();
	
	void add(int id);
    void add(Cluster *pCluster);
	void remove(int id);
    list<Cluster*>& clusters();
	
	bool contains(int id);
    
    Object *get(int id);
	
	void clear();

    bool operator==(Cluster &other);
    Cluster& operator=(Cluster &other);

    static float distance(Cluster &c1, Cluster &c2, AbstractMetric *pMetric);

private:
    Object *calculateCenter(AbstractMetric *pMetric);
    void init();

    Object *_pCenter;
    DataContainer *_pContainer;
    list<int> _ids;
    list<Cluster*> _clusters;
	bool _centerValid;
};

#endif	/* _CLUSTER_H */

