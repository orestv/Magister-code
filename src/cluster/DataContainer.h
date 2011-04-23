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
        void normalize();
		std::list<int>& ids();
        Object* getByIndex(int index);

        ~DataContainer();
    private:
        Object** _objs;
        int _objectCount;
        std::map<int, Object*> _objects;
        std::list<int> _ids;


};

#endif
