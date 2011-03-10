/* 
 * File:   main.cpp
 * Author: ovoloshchuk
 *
 * Created on March 9, 2011, 12:04 PM
 */

#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>

#include "Reader.h"
#include "KMeans.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    Reader rdr("../../data/dmc2008_train.txt");
    DataContainer container;
    rdr.fill(container);
	
	KMeans km(&container, 5);

    return (EXIT_SUCCESS);
}

