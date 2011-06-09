#include "Regression.h"
#include <cmath>
#include <stdio.h>

double Regression::predict(double *x, double *y, double arg, int nCount) {
    //y(x) = a + bx
    double a = 0, b = 0;
    double n = (double)nCount;

    double xy = 0, x_ = 0, y_ = 0, x_2 = 0;
    for (int i = 0; i < n; i++) {
        xy += x[i]*y[i];
        x_ += x[i];
        y_ += y[i];
        x_2 += pow(x[i], 2);
     }
    double top = x_*y_ - n*xy;
    double bottom = x_*x_ - n*x_2;

    b = top/bottom;
    a = y_/n - b*x_/n;

    return a + b*arg;
}
