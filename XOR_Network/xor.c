#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#define InputLayerNodes 2
#define HiddenLayerNodes 2
#define OutputLayerNodes 1
#define NumberTrainingSequences 4
#define TrainingEpochs 1000

/*
 For information, the goal of this program is to implement an AI that takes
 numbers belonging to [0, 1], XOR them and determine a number, belonging to [0, 1]
 that corresponds to the output of the XOR operation

AI :
  Layers :
    Input layer : 2 Nodes
    Hidden layers : 1 Layer of 2 Nodes
    Output layer : 1 Node

  Objective :
    Read informations from a database, and learn from data, (Machine learning)
    then, be able to recognize the correct output without using XOR operation

*/

typedef struct node
{
	int bias;
	double* bows;
	size_t bows_len;
} node;

double sigmoid(double value)
{
/*
 THE necessary function
*/
	return 1/(1 + exp(value * (-1)));
}


node* create_node(int value, int number_of_weights)
{
/*
 Creates a node
*/
	node* n = malloc(sizeof(node));
	n->bias = value;
	n->bows = malloc(number_of_weights * sizeof(double));
	n->bows_len = number_of_weights;
	return n;
}

node* initialize_weights(node* node, double* bows, size_t bows_len)
{
/*
 Set bows values of a node to bows values
*/
	if (bows_len != node->bows_len)
	{
		errx(1, "Node's bows length is different from bows_len entered as parameter");
	}

	double* p = node->bows;
	for (size_t i = 0; i < bows_len; i++)
	{
		*(p++) = *(bows++);
	}
	return node;
}

node* initialize_weights_randomly(node* node)
{
/*
 Set bows values of a node to random values
*/
	double* p = node->bows;
	size_t bows_len = node->bows_len;
	for (size_t i = 0; i < bows_len; i++)
	{
		*(p++) = (double)rand()/RAND_MAX;
	}
	return node;
}

double xor(char* inputs, size_t len)
{
/*
 Input : Takes a char list as parameter and its length, representing entries,
 Here, the inputs list is a 2-elements array formed only of zeros and ones

 Output : a double, representing an approach of the result
*/

	if (len != 2)
	{
		errx(1, "Length of inputs array is dangerous");
		// Dangerous is not even a word representing the danger
	}

	printf("%c", *inputs);
	
	return 1.0f;
}

int shuffle(int array[], size_t len)
{
/*
 Shuffles a list of int
*/
	for (size_t i = 0; i < (len-1); i++)
	{
		size_t j = i + (rand() / (RAND_MAX/(len-i) + 1));
		int temp = array[i];
		array[i] = array[j];
		array[j] = array[i];
	}

	return 0;
}

//#define InputLayerNodes = 2
//#define HiddenLayerNodes = 2
//#define OutputLayerNodes = 1
//#define NumberTrainingSequences = 4
//#define TrainingEpochs = 1000

int main()
{
	// lr = learning rate
	double lr = 1.0f;
	// Initialization of neuron layers
	// define InputLayer is useless because we already know it

	// Output layer will always be the same, depending of the last hidden layer
	double outputLayer[OutputLayerNodes];
	double outputLayerBias[OutputLayerNodes];
	double outputWeights[HiddenLayerNodes][OutputLayerNodes];	

	// To go further :
	// While implementing character recognition, there won't be only 1 hidden layer, but 2-3
	// SO, arguments will change according to the position of the layer in the network
	// If we are treating the first hidden layer, it will receive arguments from the input layer
	// Whereas the last hidden layer will send arguments to the final layer, the output layer
	
	// We consider that the actual hidden layer is the one between the input and the output layer
	double hiddenLayer[HiddenLayerNodes];
	double hiddenLayerBias[HiddenLayerNodes];
	double hiddenWeights[InputLayerNodes][HiddenLayerNodes];

	// Initialization of Training data set
	// For data sets made of images, we'll see later
	double TrainingInputs[NumberTrainingSequences][InputLayerNodes] = {
									{0.0f, 0.0f},
									{0.0f, 1.0f},
									{1.0f, 0.0f},
									{1.0f, 1.0f}
									};
	double TrainingOutputs[NumberTrainingSequences][OutputLayerNodes] = {
									{0.0f},
									{1.0f},
									{1.0f},
									{0.0f}
									};

	// The data set index will be used to get a random list of indexes, thanks to this, the machine will always learn in a different way
	int DataSetIndex[NumberTrainingSequences];
	for (int i = 0; i < NumberTrainingSequences; i++)
	{
		DataSetIndex[i] = i;
	}


	// Now, we will set some weights and some biases to random values (Starting values are random)
	

	// We need to start learning from random values
	// We begin to initialize output's layer values
	for (int i = 0; i < OutputLayerNodes; i++)
	{
		outputLayerBias[i] =  ((double)rand()) / ((double)RAND_MAX);
	}
	for (int i = 0; i < HiddenLayerNodes; i++)
	{
		for (int j = 0; j < OutputLayerNodes; j++)
		{
			outputWeights[i][j] = ((double)rand()) / ((double)RAND_MAX);
		}
	}

	// Initialization of hidden's layer(s) values
	// To go further : if there is more than 1 layer, the process will change, according to previous / next hidden layers...
	for (int i = 0; i < HiddenLayerNodes; i++)
	{
		hiddenLayerBias[i] = ((double)rand()) / ((double)RAND_MAX);
	}
	for (int i = 0; i < InputLayerNodes; i++)
	{
		for (int j = 0; j < HiddenLayerNodes; j++)
		{
			hiddenWeights[i][j] = ((double)rand()) / ((double)RAND_MAX);
		}
	}


	for (int epoch = 0; epoch < TrainingEpochs; epoch++)
	{
		shuffle(DataSetIndex, NumberTrainingSequences);

		for (int t = 0; t < NumberTrainingSequences; t++)
		{
			int index = DataSetIndex[t];





















		}
	}


	return 0;
}
