#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct NeuralNetwork
{
	size_t inputNodes;
	size_t hiddenNodes;
	size_t outputNodes;

	struct Matrix *input_hiddenWeights; // Represents weights between Input layer and hidden layer
	struct Matrix *hidden_outputWeights; // Represents weights between hidden layer and output layer
	struct Matrix *hidden_biases;
	struct Matrix *output_biases;

    double lr;
} NeuralNetwork;

void init_network();
double sigmoid(double value);

#endif
