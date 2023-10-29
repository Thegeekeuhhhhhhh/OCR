#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct NeuralNetwork
{
	size_t inputNodes;
	size_t hiddenNodes;
	size_t outputNodes;
} NeuralNetwork;

void init_network();
