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
#include "EuclidMetric.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
	
    Reader rdr("../../data/dmc2008_train_short.txt");
    DataContainer container;
    rdr.fill(container);
	
	KMeans km(&container, 5);
	
	AbstractMetric *pMetric = new EuclidMetric();
	
	km.clusterize(pMetric);
	
	delete pMetric;

    return (EXIT_SUCCESS);
}

