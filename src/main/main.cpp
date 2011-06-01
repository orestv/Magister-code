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
#include "clustering.h"
#include "validity.h"
#include "dbscan.h"


using namespace std;

char* generateFilename(time_t time) {
    struct tm *pTime = localtime(&time);
    char *pResults = "results_";
    char *pExt = ".txt";

    size_t nDateLength = 16;

    char *pchrDate = new char[nDateLength];
    strftime(pchrDate, nDateLength, "%Y%m%d_%H%M%S", pTime);

    size_t nResultLength = strlen(pResults) + strlen(pchrDate) + strlen(pExt)+1;
    char *pResult = new char[nResultLength];
    memset(pResult, 0, sizeof(char)*nResultLength);
    memcpy(pResult, pResults, sizeof(char)*strlen(pResults));
    memcpy(pResult+strlen(pResults), pchrDate, sizeof(char)*strlen(pchrDate));
    memcpy(pResult+strlen(pResults)+strlen(pchrDate), pExt, sizeof(char)*strlen(pExt)+1);
    delete[] pchrDate;
    return pResult;
}

int main(int argc, char** argv) {

    char *filename;
    int nClusters = 5;
    int nMethod = 0;
    int nNeighbors = 4;
    float eps = 0.05;
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
            filename = argv[1];//"../../data/dmc2008_train.txt";

        if (strcmp(argv[2], "kmeans") == 0) {
            nMethod = 0;
            nClusters = atoi(argv[3]);
        } else if (strcmp(argv[2], "upgma") == 0) {
            nMethod = 1;
        } else if (strcmp(argv[2], "dbscan") == 0) {
            nMethod = 2;
            nNeighbors = atoi(argv[3]);
            eps = atof(argv[4]);
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
    DBScan dbscan(&container);
	
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
	

    Clustering *pClus = NULL;
    time_t start, end;
    start = time(NULL);

    switch (nMethod) {
        case 0:
            pClus = km.clusterize(pMetric);
            break;
        case 1:
            pClus = up.clusterize(pMetric);
            break;
        case 2:
            pClus = dbscan.clusterize(eps, nNeighbors, pMetric);
            break;
    };

    end = time(NULL);

    printf("Clusterized, %i seconds spent.\r\n", (int)(end-start));
    printf("Results: \n");

    char *pFilename = generateFilename(time(NULL));
    pFile = fopen(pFilename, "w");
    fprintf(pFile, "run as: ");
    for (int i = 0; i < argc; i++) {
        fprintf(pFile, "%s ", argv[i]);
    }
    fprintf(pFile, "\n");
    for (list<Cluster*>::iterator iC = pClus->clusters().begin(); 
            iC != pClus->clusters().end(); iC++) {
        list<Object*> lsObjects = (*iC)->objects();
        printf("%i objects\n", lsObjects.size());
        for (list<Object*>::iterator iO = lsObjects.begin();
                iO != lsObjects.end(); iO++) {
            fprintf(pFile, "%i\t", (*iO)->id());
        }
        fprintf(pFile, "\n");
    }
    fclose(pFile);
    //printf("Clustering validity: %.5f\n", Validity::dunn(*pClus, pMetric));
	delete pMetric;
    if (pClus)
        delete pClus;

    return (EXIT_SUCCESS);
}

