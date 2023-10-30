#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"
#include "perceptron.h"

typedef struct NeuralNetwork
{
	size_t inputNodes;
	size_t hiddenNodes;
	size_t outputNodes;

	Matrix *input_hiddenWeights; // Represents weights between Input layer and hidden layer
	Matrix *hidden_outputWeights; // Represents weights between hidden layer and output layer
	Matrix *hidden_biases;
	Matrix *output_biases;

} NeuralNetwork;

void init_network();
double sigmoid(double value);

#endif
