#ifndef LINE_H
#define LINE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gray_im.h"


struct l {
    double rho;
    double theta;
};

struct Line{
    struct l* line;
    int nblines;
};

struct Point{
    int x;
    int y;
}; 

struct Line houghTransform(Uint8** pixels, int width, int height, int threshold);

void findLocalMaxima(struct Line* newLine, int *accumulator
        , int max_rho, int theta_count, int threshold);

void addLine(struct Line* myLine, double rho, double theta);


#endif
