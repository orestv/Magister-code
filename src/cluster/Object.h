/* 
 * File:   Row.h
 * Author: ovoloshchuk
 *
 * Created on March 9, 2011, 12:08 PM
 */

#ifndef _ROW_H
#define	_ROW_H



using namespace std;

class Object {
public:
    Object();
    Object(int attributeCount);
    Object(const Object& orig);

    int attributeCount();

    bool isAttrValid(int id);
    float attr(int id);
    int id();
    void setId(int id);

    void setAttr(int id, float value);

    void setActualClass(int actualClass);
    int actualClass();
    void print();

    virtual ~Object();
private:
    int _attributeCount;
    int _actualClass;
    //Attribute* _attributes;
    float *_attrValues;
    bool *_attrValid;
    int _id;

};

#endif	/* _ROW_H */

