#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#define 
typedef struct
{
	double weights[2];
	size_t inputsLen;
	double bias;
	double lr; // Learning rate
} Perceptron;

void init_percetron(Perceptron *p, size_t len, double bias);
double guess(Perceptron *p, double inputs[], size_t len);

#endif
