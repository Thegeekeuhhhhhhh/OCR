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
	size_t hidden1Nodes;
	size_t hidden2Nodes;
	size_t outputNodes;

	struct Matrix *input_hidden1Weights;
    // Represents weights between Input layer and hidden layer
	
	struct Matrix *hidden1_hidden2Weights;
    // Represents weights between the hidden layers

    struct Matrix *hidden2_outputWeights;
    // Represents weights between hidden layer and output layer
        
        struct Matrix *hidden1_biases;
	struct Matrix *hidden2_biases;
	struct Matrix *output_biases;

    double lr;
} NeuralNetwork;

void init_network(NeuralNetwork *nn, size_t inputNumber, size_t hidden1Number,
	size_t hidden2Number, size_t outputNumber, double learning_rate);

void free_network(NeuralNetwork *nn);

double sigmoid(double value);
double dsigmoid(double value);
double simpledsigmoid(double value);

struct Matrix *feedforward_algo(NeuralNetwork *nn, double inputs[], size_t len);
void train(NeuralNetwork *nn, double inputs[], size_t len, struct Matrix *wanted);
void shuffle(size_t array[], size_t len);

#endif
