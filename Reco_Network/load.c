#include "load.h"
#include "train.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

NeuralNetwork* load_network(char *str)
{
    NeuralNetwork *nn = malloc(sizeof(NeuralNetwork));
    init_network(nn, 784, 50, 10, 0.5f);

    FILE *fileptr = NULL;
    fileptr = fopen(str, "r");

    if (fileptr == NULL)
    {
        errx(1, "Problem while opening the file %s", str);
    }

    // Read the node counts
    if (fscanf(fileptr, "%li\n%li\n%li\n", &nn->inputNodes, &nn->hiddenNodes, &nn->outputNodes) < 0)
    {
        errx(1, "Pb de fscanf");
    }

    // Read input to hidden layer weights
    for (size_t i = 0; i < nn->inputNodes; i++)
    {
        for (size_t j = 0; j < nn->hiddenNodes; j++)
        {
            double temp;
            if (fscanf(fileptr, "%lf|", &temp) < 0)
            {
                errx(1, "Pb de fscanf");
            }
            matrix_set(nn->input_hiddenWeights, j, i, temp);
        }
    }

    // Read hidden layer biases
    for (size_t i = 0; i < nn->hidden_biases->row; i++)
    {
        double temp;
        if (fscanf(fileptr, "%lf|", &temp) < 0)
        {
            errx(1, "Pb de fscanf");
        }
        matrix_set(nn->hidden_biases, i, 0, temp);
    }

    // Read hidden to output layer weights
    for (size_t i = 0; i < nn->outputNodes; i++)
    {
        for (size_t j = 0; j < nn->hiddenNodes; j++)
        {
            double temp;
            if (fscanf(fileptr, "%lf|", &temp) < 0)
            {
                errx(1, "Pb de fscanf");
            }
            matrix_set(nn->hidden_outputWeights, i, j, temp);
        }
    }

    // Read output layer biases
    for (size_t i = 0; i < nn->output_biases->row; i++)
    {
        double temp;
        if (fscanf(fileptr, "%lf|", &temp) < 0)
        {
            errx(1,"Pb de fscanf");
        }
        matrix_set(nn->output_biases, i, 0, temp);
    }

    return nn;
}
