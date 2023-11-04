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

struct Line houghTransform(Uint8** pixels, int width, int height);

void findLocalMaxima(struct Line* newLine, int *accumulator
        , int max_rho, int theta_count, int width);

void addLine(struct Line* myLine, double rho, double theta);

void cropImage(SDL_Surface* image, struct l* lines, int numLines);

#endif
