/* 
 * File:   Row.cpp
 * Author: ovoloshchuk
 * 
 * Created on March 9, 2011, 12:08 PM
 */

#include "Object.h"
#include <memory.h>

Object::Object() {
}

Object::Object(int attributeCount) {
    _attributeCount = attributeCount;
    _attrValues = new float[attributeCount];
    _attrValid = new bool[attributeCount];
    memset(_attrValues, 0, attributeCount*sizeof(float));
    memset(_attrValid, 0, attributeCount*sizeof(bool));
}

Object::Object(const Object& orig) {
    //this->_attributeCount = orig._attributeCount;
    //this->_attributes = new Attribute[_attributeCount];
    //memcpy(orig._attributes, _attributes, _attributeCount*sizeof(Attribute));
    //this->_attributes = orig._attributes;
}

int Object::attributeCount() {
    return _attributeCount;
}

bool Object::isAttrValid(int id) {
    if (id >= _attributeCount || id < 0)
        return false;
    return _attrValid[id];
    //return _attributes[id].valid();
}

float Object::attr(int id) {
    if (!isAttrValid(id))
        return 0;
    return _attrValues[id];
    //return _attributes[id].value();
}

void Object::setAttr(int id, float value) {
    if (id >= _attributeCount)
        return;
    _attrValues[id] = value;
    _attrValid[id] = true;
    //_attributes[id].setValue(value);
}

void Object::setActualClass(int actualClass) {
    _actualClass = actualClass;
}

int Object::actualClass() {
    return _actualClass;
}

Object::~Object() {
    if (_attrValues)
        delete[] _attrValues;
    if (_attrValid)
        delete[] _attrValid;
}

