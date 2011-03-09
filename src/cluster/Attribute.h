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
    Attribute(double value);
    Attribute(const Attribute& orig);

    void setValue(double value);
    void setValid(bool valid);

    double value();
    bool valid();

    virtual ~Attribute();
private:
    double _value;
    bool _valid;

};

#endif	/* _ATTRIBUTE_H */

