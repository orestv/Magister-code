/* 
 * File:   Row.cpp
 * Author: ovoloshchuk
 * 
 * Created on March 9, 2011, 12:08 PM
 */

#include "Object.h"
#include <memory.h>
#include <stdio.h>

Object::Object() {
}

Object::Object(int attributeCount) {
    _attributeCount = attributeCount;
    _attributes = new Attribute[attributeCount];
}

Object::Object(const Object& orig) {
    this->_attributeCount = orig._attributeCount;
    this->_attributes = new Attribute[_attributeCount];
    memcpy(orig._attributes, _attributes, _attributeCount*sizeof(Attribute));
    //this->_attributes = orig._attributes;
}

int Object::attributeCount() {
    return _attributeCount;
}

bool Object::isAttrValid(int id) {
    if (id >= _attributeCount || id < 0)
        return false;
    return _attributes[id].valid();
}

double Object::attr(int id) {
    if (!isAttrValid(id))
        return 0;
    return _attributes[id].value();
}

void Object::setAttr(int id, double value) {
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

void Object::print() {
    int nAttrCount = this->attributeCount();
    for (int i = 0; i < nAttrCount; i++) {
        printf("%.4f, ", this->attr(i));
    }
    printf("\n");
}

int Object::id() {
    return this->_id;
}

void Object::setId(int id) {
    this->_id = id;
}

Object::~Object() {
    if (_attributes)
        delete[] _attributes;
}

