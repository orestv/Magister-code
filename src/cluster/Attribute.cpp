/* 
 * File:   Attribute.cpp
 * Author: ovoloshchuk
 * 
 * Created on March 9, 2011, 12:09 PM
 */

#include "Attribute.h"

Attribute::Attribute() {
    this->_valid = false;
    this->_value = 0;
}

Attribute::Attribute(const Attribute& orig) {
    this->_value = orig._value;
    this->_valid = orig._valid;
}

Attribute::Attribute(double value) {
    this->_value = value;
}

void Attribute::setValid(bool valid) {
    this->_valid = valid;
}

void Attribute::setValue(double value) {
    this->_value = value;
    this->_valid = true;
}

double Attribute::value() {
    return this->_value;
}

bool Attribute::valid() {
    return this->_valid;
}

Attribute::~Attribute() {
}

