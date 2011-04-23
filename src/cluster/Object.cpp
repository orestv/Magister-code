/* 
 * File:   Row.cpp
 * Author: ovoloshchuk
 * 
 * Created on March 9, 2011, 12:08 PM
 */

#include "Object.h"

Object::Object() {
}

Object::Object(int attributeCount) {
    _attributeCount = attributeCount;
    _attributes = new Attribute[attributeCount];
}

Object::Object(const Object& orig) {
    this->_attributes = orig._attributes;
    this->_attributeCount = orig._attributeCount;
}

int Object::attributeCount() {
    return _attributeCount;
}

bool Object::isAttrValid(int id) {
    if (id >= _attributeCount || id < 0)
        return false;
    return _attributes[id].valid();
}

float Object::attr(int id) {
    if (!isAttrValid(id))
        return 0;
    return _attributes[id].value();
}

void Object::setAttr(int id, float value) {
    if (id >= _attributeCount)
        return;
    _attributes[id].setValue(value);
}

void Object::setActualClass(int actualClass) {
    _actualClass = actualClass;
}

int Object::actualClass() {
    return _actualClass;
}

Object::~Object() {
    if (_attributes)
        delete[] _attributes;
}

