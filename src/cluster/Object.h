/* 
 * File:   Row.h
 * Author: ovoloshchuk
 *
 * Created on March 9, 2011, 12:08 PM
 */

#ifndef _ROW_H
#define	_ROW_H

#include "Attribute.h"
#include <vector>


using namespace std;

class Object {
public:
    Object();
    Object(int attributeCount);
    Object(const Object& orig);

    int attributeCount();

    bool isAttrValid(int index);
    double attr(int index);

    void setAttr(int index, double value);

    virtual ~Object();
private:
    int _attributeCount;
    Attribute* _attributes;

};

#endif	/* _ROW_H */

