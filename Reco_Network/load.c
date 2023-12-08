#include "load.h"
#include "train.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct NeuralNetwork* load(char *str)
{
    FILE *fileptr = fopen(str, "r");
    if (fileptr == NULL)
    {
        errx(1, "Problem occured while opening the file %s", str);
    }

    NeuralNetwork *nn = malloc(sizeof(NeuralNetwork));
    init_network(nn, 784, 100, 10, 0.5f);

    fprintf(fileptr, "%li\n%li\n%li\n", nn->inputNodes,
            nn->hiddenNodes, nn->outputNodes);
    fprintf(fileptr, "Input to hidden layer Weights :\n");
    for (size_t i = 0; i < nn->inputNodes; i++)
    {
        for (size_t j = 0; j < nn->hiddenNodes; j++)
        {
            double temp = matrix_get(nn->input_hiddenWeights, j, i);
            fprintf(fileptr, "%lf|", temp);
        }
        fprintf(fileptr, "\n");
    }

    fprintf(fileptr, "\nHidden layer biases :\n");
    for (size_t i = 0; i < nn->hidden_biases->row; i++)
    {
        double temp = matrix_get(nn->hidden_biases, i, 0);
        fprintf(fileptr, "%lf|", temp);
    }

    fprintf(fileptr, "\nHidden to output layer Weights :\n");
    for (size_t i = 0; i < nn->outputNodes; i++)
    {
        for (size_t j = 0; j < nn->hiddenNodes; j++)
        {
            double temp = matrix_get(nn->hidden_outputWeights, j, i);
            fprintf(fileptr, "%lf|", temp);
        }
        fprintf(fileptr, "\n");
    }
    fprintf(fileptr, "\nOutput layer biases :\n");
    for (size_t i = 0; i < nn->output_biases->row; i++)
    {
        double temp = matrix_get(nn->output_biases, i, 0);
        fprintf(fileptr, "%lf|", temp);
    }

    fprintf(fileptr, "\n");
    fclose(fileptr);
    return nn;
}
