#include "DataContainer.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
using namespace std;

DataContainer::DataContainer(){
    _objs = NULL;
}

DataContainer::~DataContainer(){
    for (map<int, Object*>::iterator iObj = _objects.begin(); \
            iObj != _objects.end(); iObj++) {
        if (iObj->second)
            delete iObj->second;
    }
    if (_objs)
        delete[] _objs;
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

Object* DataContainer::getByIndex(int index) {
    if (_objs)
        return _objs[index];
    else
        return NULL;
}

std::list<int>& DataContainer::ids() {
	return _ids;
}

void DataContainer::normalize() {
    int nAttrCount = 0;
    nAttrCount = _objects.begin()->second->attributeCount();
    float *arrCoeff = new float[nAttrCount];
    memset(arrCoeff, 0, nAttrCount*sizeof(float));

    Object *pObj = NULL;
    for (map<int, Object*>::iterator obj = _objects.begin();
            obj != _objects.end(); obj++) {

        pObj = obj->second;
        for (int i = 0; i < nAttrCount; i++) {
            if (pObj->isAttrValid(i)) {
                arrCoeff[i] = max(arrCoeff[i], pObj->attr(i));
				//printf("Coefficient: %.2f, ", arrCoeff[i]);
            }
        }
    }
    float value;
    if (_objs != NULL)
        delete[] _objs;
    _objs = new Object*[_objects.size()];
    int nObject = 0;
    for (map<int, Object*>::iterator obj = _objects.begin();
            obj != _objects.end(); obj++) {

        pObj = obj->second;
        for (int i = 0; i < nAttrCount; i++) {
            if (pObj->isAttrValid(i)) {
                value = pObj->attr(i) / arrCoeff[i];
				//printf("value: %.2f, ", value);
                pObj->setAttr(i, value);
            }
        }
        _objs[nObject] = pObj;
        nObject++;
    }

    delete[] arrCoeff;
}
