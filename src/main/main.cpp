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
#include <stdio.h>

#include "Reader.h"
#include "KMeans.h"
#include "Upgma.h"
#include "EuclidMetric.h"
#include "Regression.h"
#include <time.h>
#include <list>
#include <cmath>


using namespace std;

int main(int argc, char** argv) {

    char *filename;
    int nClusters = 5;
    int nMethod = 0;
    if (argc > 1) {
        if (strcmp(argv[1], "short") == 0)
            filename = "../../data/dmc2008_train_short.txt";
        else if (strcmp(argv[1], "medium") == 0)
            filename = "../../data/dmc2008_train_medium.txt";
        else if (strcmp(argv[1], "test") == 0)
            filename = "../../data/train.txt";
        else if (strcmp(argv[1], "p") == 0)
            filename = "../../data/dmc2008_train_predicted.txt";
        else if (strcmp(argv[1], "np") == 0)
            filename = "../../data/dmc2008_train_predicted_normalized.txt";
        else if (strcmp(argv[1], "snp") == 0)
            filename = "../../data/short_normalized.txt";
        else
            filename = "../../data/dmc2008_train.txt";

        if (strcmp(argv[2], "kmeans") == 0) {
            nMethod = 0;
            nClusters = atoi(argv[3]);
        } else if (strcmp(argv[2], "upgma") == 0) {
            nMethod = 1;
        }
    }
    else
        return 0;
	
    Reader rdr(filename);
    DataContainer container;
    rdr.fill(container);
    container.normalize();
	
	KMeans km(&container, nClusters);
    Upgma up(&container);
	
	AbstractMetric *pMetric = new EuclidMetric(&container);
    pMetric->predictMissingData(&container);

    list<int> ids = container.ids();
    Object *pObj;
    FILE *pFile = fopen("predicted_data.txt", "w");
    for (list<int>::iterator id = ids.begin();
            id != ids.end(); id++) {
        
        pObj = container.get(*id);
        fprintf(pFile, "%i;%i;", *id, pObj->actualClass());
        for (int i = 0; i < pObj->attributeCount(); i++) {
            fprintf(pFile, "%.2f;", pObj->attr(i));
        }
        fprintf(pFile, "\n");
    }
    fclose(pFile);
	
    time_t start, end;
    start = time(NULL);

    switch (nMethod) {
        case 0:
            km.clusterize(pMetric);
            break;
        case 1:
            up.clusterize(pMetric);
            break;
    };

    end = time(NULL);

    printf("Clusterized, %i seconds spent.\r\n", (int)(end-start));
	
	delete pMetric;

    return (EXIT_SUCCESS);
}

