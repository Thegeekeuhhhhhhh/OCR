#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "matrix.h"

typedef struct NeuralNetwork
{
	size_t inputNodes;
	size_t hiddenNodes;
	size_t outputNodes;

	struct Matrix *input_hiddenWeights;
    // Represents weights between Input layer and hidden layer
	
    struct Matrix *hidden_outputWeights;
    // Represents weights between hidden layer and output layer
	struct Matrix *hidden_biases;
	struct Matrix *output_biases;

    double lr;
} NeuralNetwork;

void init_network(NeuralNetwork *nn, size_t inputNumber, size_t hiddenNumber,
        size_t outputNumber, double learning_rate);

void free_network(NeuralNetwork *nn);

double sigmoid(double value);
double dsigmoid(double value);
double simpledsigmoid(double value);

void digit_print(double input[]);

struct Matrix *feedforward_algo(NeuralNetwork *nn, double inputs[], size_t len);
void training(NeuralNetwork *nn, double inputs[], size_t len, Matrix *wanted);
void shuffle(size_t array[], size_t len);

int train(size_t EPOCHS, double learning_rate);

#endif
