/* 
 * File:   Attribute.h
 * Author: ovoloshchuk
 *
 * Created on March 9, 2011, 12:09 PM
 */

#ifndef _ATTRIBUTE_H
#define	_ATTRIBUTE_H

class Attribute {
public:
    Attribute();
    Attribute(float value);
    Attribute(const Attribute& orig);

    void setValue(float value);
    void setValid(bool valid);

    float value();
    bool valid();

    virtual ~Attribute();
private:
    float _value;
    bool _valid;

};

#endif	/* _ATTRIBUTE_H */

