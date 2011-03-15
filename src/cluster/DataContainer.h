#ifndef _datacontainer_h_
#define _datacontainer_h_

#include <map>
#include <list>
#include "Object.h"

class DataContainer {
    public:
        DataContainer();

        Object* get(int id);
        void add(int id, Object **ppObj);
		
		std::list<int>& indices();

        ~DataContainer();
    private:
        std::map<int, Object*> _objects;
        std::list<int> _indices;


};

#endif
