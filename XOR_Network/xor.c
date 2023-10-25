#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#define HIDDENLAYERNODE = 2

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
	float* bows;
	size_t bows_len;
} node;

float sigmoid(float value)
{
/*
 THE necessary function
*/
	return 1/(1 + expf(value * (-1)));
}


node* create_node(int value, int number_of_weights)
{
/*
 Creates a node
*/
	node* n = malloc(sizeof(node));
	n->bias = value;
	n->bows = malloc(number_of_weights * sizeof(float));
	n->bows_len = number_of_weights;
	return n;
}

node* initialize_weights(node* node, float* bows, size_t bows_len)
{
/*
 Set bows values of a node to bows values
*/
	if (bows_len != node->bows_len)
	{
		errx(1, "Node's bows length is different from bows_len entered as parameter");
	}

	float* p = node->bows;
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
	float* p = node->bows;
	size_t bows_len = node->bows_len;
	for (size_t i = 0; i < bows_len; i++)
	{
		*(p++) = (float)rand()/RAND_MAX;
	}
	return node;
}

float xor(char* inputs, size_t len)
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
	








	return 1.0;
}

int main()
{
	//printf("exp(0) == %lf\n", exp(0));
	//printf("%s\n", "Quoicoubehhhh");
	node* n = create_node(1, 2); // Initialize a node with value 1, containing 2 bows to next neurons
	initialize_weights_randomly(n);
	for(int i = 0; n->bows[i]; i++)
	{
		printf("%f\n",n->bows[i]);
	}



	return 0;
}
