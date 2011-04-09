/* 
 * File:   main.cpp
 * Author: ovoloshchuk
 *
 * Created on March 9, 2011, 12:04 PM
 */

#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <string>
#include <fstream>

#include "Reader.h"
#include "KMeans.h"
#include "Upgma.h"
#include "EuclidMetric.h"
#include <time.h>

using namespace std;

int main(int argc, char** argv) {


    char *filename;
    int nClusters = 5;
    if (argc > 1) {
        if (strcmp(argv[1], "short") == 0)
            filename = "../../data/dmc2008_train_short.txt";
        else if (strcmp(argv[1], "medium") == 0)
            filename = "../../data/dmc2008_train_medium.txt";
        else if (strcmp(argv[1], "test") == 0)
            filename = "../../data/train.txt";
        else
            filename = "../../data/dmc2008_train.txt";

        if (strcmp(argv[2], "kmeans") == 0)
            nClusters = atoi(argv[3]);

    }
    else
        return 0;
	
    Reader rdr(filename);
    DataContainer container;
    rdr.fill(container);
    //container.normalize();
	
	KMeans km(&container, nClusters);
    Upgma up(&container);
	
	AbstractMetric *pMetric = new EuclidMetric(&container);
	
    time_t start, end;
    start = time(NULL);

	km.clusterize(pMetric);
    //up.clusterize(pMetric);

    end = time(NULL);

    printf("Clusterized, %i seconds spent.\r\n", (int)(end-start));
	
	delete pMetric;

    return (EXIT_SUCCESS);
}

