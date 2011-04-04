#include "DataContainer.h"
using namespace std;

DataContainer::DataContainer(){
}

DataContainer::~DataContainer(){
    for (map<int, Object*>::iterator iObj = _objects.begin(); \
            iObj != _objects.end(); iObj++) {
        if (iObj->second)
            delete iObj->second;
    }
}

void DataContainer::add(int id, Object **ppObj) {
    _objects.insert(pair<int, Object*>(id, *ppObj));
    _ids.push_back(id);
}

Object* DataContainer::get(int id) {
    map<int, Object*>::iterator ret = _objects.find(id);
    if (ret != _objects.end())
        return ret->second;
    else
        return 0;
}

std::list<int>& DataContainer::ids() {
	return _ids;
}

void DataContainer::normalize() {

}
