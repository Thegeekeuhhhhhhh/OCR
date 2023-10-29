#include "perceptron.h"

// Activation function ?
double sigmoid(double value)
{
	return 1 / (1 + exp(value * (-1)));
}

void init_perceptron(Perceptron* p, size_t len, double bias)
{
	p->inputsLen = len;
	p->bias = bias;
	for (size_t i = 0; i < p->inputsLen; i++)
	{
		p->weights[i] = ((double)rand()) / ((double)RAND_MAX);
		//printf("%f\n", p->weights[i]);
	}
}

double guess(Perceptron *p, double inputs[], size_t len)
{
	double result = p->bias;
	for (size_t i = 0; i < len; i++)
	{
		result += inputs[i]*p->weights[i];
	}
	return sigmoid(result);
}

void train(Perceptron *p, double inputs[], size_t len, double target)
{
	double g = guess(p, inputs, len);
	double error = target - g;
	for (size_t i = 0; i < p->inputsLen; i++)
	{
		p->weights[i] += error * inputs[i] * p->lr; // Adjust weights
	}
}

int main()
{
	Perceptron *p = malloc(sizeof(Perceptron));
	init_perceptron(p, 2, 5);
	double inputs[] = {-1.0f, 0.5f};
	printf("%f\n", guess(p, inputs, (size_t)2));

	/*
	for (int i = 0; i < 2; i++)
	{
		printf("%f\n", p->weights[i]);
	}
	*/


	return 0;
}
