#include "neuralnetwork.h"

void init_network(NeuralNetwork *nn, size_t inputNumber, size_t hiddenNumber, size_t outputNumber)
{
	nn->inputNodes = inputNumber;
	nn->hiddenNodes = hiddenNumber;
	nn->outputNodes = outputNumber;
}

int main()
{
	NeuralNetwork *nn = malloc(sizeof(NeuralNetwork));
	init_network(nn, 2, 2, 1);
	// Nodes needed to let XOR AI work
	// Input layer : 2 Nodes
	// Hidden layer(s) : 2 Nodes
	// Output layer : 1 Node
	return 0;
}
