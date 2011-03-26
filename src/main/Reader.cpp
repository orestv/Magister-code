/* 
 * File:   Reader.cpp
 * Author: ovoloshchuk
 * 
 * Created on March 9, 2011, 12:36 PM
 */

#include "Reader.h"
#include <string>
#include <cstring>
#include <memory>
#include <climits>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <list>
#include "EuclidMetric.h"

using namespace std;

Reader::Reader(char *filename) {
    _filename = string(filename);
}

void Reader::fill(DataContainer &container) {
    _dataFile.open(_filename.data());
    if (!_dataFile.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return;
    }

    time_t start, end;
    string strBuf;

    int nCounter = 0;
    int nID = 0;
    Object *pObject = 0;
    time(&start);
    while (_dataFile.good()) {
        getline(_dataFile, strBuf);
        if (strBuf.length() == 0)
            break;
        pObject = parseObject(strBuf, nID);
        container.add(nID, &pObject);
        nCounter++;
        if (nCounter % 10000 == 0)
            cout << nCounter << " lines read!" << endl;
    }
    time(&end);
    double dif = difftime(end, start);
    printf("Done! Processing %i lines took %.2lf seconds\r\n", nCounter, dif);
    _dataFile.close();
}

Object* Reader::parseObject(string sRow, int &id) {
    char delimiter = ';';
    int nRowLength = sRow.length();
    char *pRowStart = new char[nRowLength + 1];
    char *pRow = pRowStart;

    //Copy the string so that we could modify it in place
    memcpy(pRow, sRow.data(), nRowLength);

    pRow[nRowLength] = '\0';
    char *pEnd = pRow + nRowLength;
    int nNumberLength = 0;
    id = getNumber(&pRow, nNumberLength, delimiter);
    //cout<<"ID:"<<id<<endl;
    pRow += nNumberLength + 1;
    int nActualClass = getNumber(&pRow, nNumberLength);
    pRow += nNumberLength + 1;

    int nAttributeCount = 0;
    for (char *pTemp = pRow; pTemp < pEnd; pTemp++) {
        if (*pTemp == ';')
            nAttributeCount++;
    }
    nAttributeCount++;
    //cout<<"Attribute count: "<<nAttributeCount<<endl;

    Object *pObj = new Object(nAttributeCount);
    pObj->setActualClass(nActualClass);
    double attrValue = 0;
    int nAttribute = 0;

    while (pRow < pEnd) {

        attrValue = getNumber(&pRow, nNumberLength, delimiter);
        //cout<<"Attribute value: "<<attrValue<<endl;

        if (nNumberLength > 0)
            pObj->setAttr(nAttribute, attrValue);

        nAttribute++;
        pRow += nNumberLength + 1;
    }
    delete[] pRowStart;
    //cout<<endl<<endl;
    return pObj;
}

double Reader::getNumber(char **data, int &length, char delimiter) {
    length = 0;
    char *pIndex = *data;
    while (pIndex < *data + strlen(*data)) {
        if (*pIndex == delimiter || *pIndex == '\r' || *pIndex == '\n') {
            length = (pIndex - *data);
            break;
        }
        pIndex++;
        length++;
    }
    if (length > 0) {
        (*data)[length] = '\0';
        double result = atof(*data);
        return result;
    } else
        return 0;
}

Reader::Reader(const Reader& orig) {
}

Reader::~Reader() {
    if (_dataFile.is_open())
        _dataFile.close();
}
