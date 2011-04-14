#include "DataContainer.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
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
    int nAttrCount = 0;
    nAttrCount = _objects.begin()->second->attributeCount();
    double *arrCoeff = new double[nAttrCount];
    memset(arrCoeff, 0, nAttrCount*sizeof(double));

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
    double value;
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
    }

    delete[] arrCoeff;
}
