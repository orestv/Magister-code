#include "Regression.h"
#include <cmath>
#include <stdio.h>

float Regression::predict(float *x, float *y, float arg, int nCount) {
    //y(x) = a + bx
    float a = 0, b = 0;
    float n = (float)nCount;

    float xy = 0, x_ = 0, y_ = 0, x_2 = 0;
    for (int i = 0; i < n; i++) {
        xy += x[i]*y[i];
        x_ += x[i];
        y_ += y[i];
        x_2 += pow(x[i], 2);
     }
    float top = x_*y_ - n*xy;
    float bottom = x_*x_ - n*x_2;

    b = top/bottom;
    a = y_/n - b*x_/n;

    return a + b*arg;
}
