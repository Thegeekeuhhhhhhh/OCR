#include "neuralnetwork.h"

void init_network(NeuralNetwork *nn, size_t inputNumber, size_t hiddenNumber, size_t outputNumber)
{
	nn->inputNodes = inputNumber;
	nn->hiddenNodes = hiddenNumber;
	nn->outputNodes = outputNumber;

	nn->input_hiddenWeights = malloc(sizeof(Matrix)); // Represents weights between Input layer and hidden layer
	matrix_init(nn->input_hiddenWeights, nn->hiddenNodes, nn->inputNodes);

	// Randomizing weights values between input and hidden layer
	for (size_t i = 0; i < nn->input_hiddenWeights->row; i++)
	{
		for (size_t j = 0; j < nn->input_hiddenWeights->col; j++)
		{
			double temp = ((double)rand()) / ((double)RAND_MAX);
			matrix_set(nn->input_hiddenWeights, i, j, temp);
		}
	}

	// Randomizing weights values between hidden and output layer
	nn->hidden_outputWeights = malloc(sizeof(Matrix)); // Represents weights between hidden layer and Output layer
	matrix_init(nn->hidden_outputWeights, nn->outputNodes, nn->hiddenNodes);

	for (size_t i = 0; i < nn->hidden_outputWeights->row; i++)
	{
		for (size_t j = 0; j < nn->hidden_outputWeights->col; j++)
		{
			double temp = ((double)rand()) / ((double)RAND_MAX);
			matrix_set(nn->hidden_outputWeights, i, j, temp);
		}
	}

	nn->hidden_biases = malloc(sizeof(Matrix));
	matrix_init(nn->hidden_biases, nn->hiddenNodes, 1); // This is a single column matrix

	for (size_t i = 0; i < nn->hidden_biases->row; i++)
	{             
		double temp = ((double)rand()) / ((double)RAND_MAX);
		matrix_set(nn->hidden_biases, i, 1, temp);                
	}

	nn->output_biases = malloc(sizeof(Matrix));
	matrix_init(nn->output_biases, nn->outputNodes, 1); // Single column matrix too

	for (size_t i = 0; i < nn->output_biases->row; i++)
	{
		double temp = ((double)rand()) / ((double)RAND_MAX);
		matrix_set(nn->output_biases, i, 1, temp);
	}						    

}

double sigmoid(double value)
{
	return 1 / (1 + exp(value * (-1)));
}

Matrix *feedforward_algo(NeuralNetwork *nn, double input[], size_t len)
{
	Matrix *inputMatrix = malloc(sizeof(Matrix));
	matrix_init(inputMatrix, len, 1); // input[] is a column matrix
	for (size_t i = 0; i < len; i++)
	{
		inputMatrix->data[i] = input[i]; // Transpose values present in the input[] argument in the new matrix
	}

	// Computes hidden outputs
	Matrix *hidden = matrix_dot_product(nn->input_hiddenWeights, inputMatrix); //// Calculating the function that will be add to the biases
	matrix_add_in_place(hidden, nn->hidden_biases);

	// Activation
	matrix_apply_function_in_place(hidden, sigmoid); // Sets all the values between 0 and 1

	// We obtained the results at the hidden layer, now we are going to compute, with the same process, the results for the output layer
	// Computes output outputs
	Matrix *output = matrix_dot_product(nn->hidden_outputWeights, hidden);
	matrix_add_in_place(output, nn->output_biases);

	// Acvtivation
	matrix_apply_function_in_place(output, sigmoid);

	return output;
}

int main()
{
	NeuralNetwork *nn = malloc(sizeof(NeuralNetwork));
	init_network(nn, 2, 2, 1);
	// Nodes needed to let XOR AI work
	// Input layer : 2 Nodes
	// Hidden layer(s) : 2 Nodes
	// Output layer : 1 Node

	double input[] = {1, 0};
	Matrix *m = feedforward_algo(nn, input, 2);
	for (size_t i = 0; i < m->dataLen; i++)
	{
		printf("%f ", m->data[i]);
	}
	printf("\n");
	return 0;
}
