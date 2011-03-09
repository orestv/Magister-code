/* 
 * File:   Reader.h
 * Author: ovoloshchuk
 *
 * Created on March 9, 2011, 12:36 PM
 */

#ifndef _READER_H
#define	_READER_H

#include "Object.h"
#include "DataContainer.h"
#include <fstream>
#include <map>
#include <string>

class Reader {
public:
    Reader(char *filename);
    Reader(const Reader& orig);
    void fill(DataContainer &container);
    virtual ~Reader();
    static Object* parseObject(string data, int &id);
private:
    static double getNumber(char **data, int &length, char delimiter = ';');

    std::ifstream _dataFile;
    std::string _filename;


};

#endif	/* _READER_H */

