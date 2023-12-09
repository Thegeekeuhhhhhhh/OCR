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
    /*
       int fd = open(str, O_RDONLY);
       if (fd == -1)
       {
       errx(1, "Problem occured while opening the file %s", str);
       }

       char buffer[200];
       read(fd, buffer, 11);
       */
    NeuralNetwork *nn = malloc(sizeof(NeuralNetwork));
    init_network(nn, 784, 50, 10, 0.5f);

    FILE *fileptr = NULL;
    fileptr = fopen(str, "r");

    if (fileptr == NULL)
    {
        errx(1, "Problems while opening file");
    }

    // Read the node counts
    //read(*fileptr, NULL, 11);
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

    printf("Closing\n");
    //fclose(fileptr);

    printf("Return\n");
    return nn;



    /*
    //return nn;
    //printf("After init\n");
    //matrix_print(nn->hidden_biases);

    size_t redchars;
    char next[200];
    size_t next_index = 0;
    size_t buf_index;
    int continueI_HWeights = 1;
    int continueH_Biases = 1;
    int continueH_OWeights = 1;
    int continueO_Biases = 1;
    size_t firstIndex = 0;
    size_t secondIndex = 0;
    double val = 0;
    while ((redchars = read(fd, buffer, 150)))
    {
    buf_index = 0;
    while (buf_index < redchars)
    {
    next[next_index] = buffer[buf_index];
    if (buffer[buf_index] == '|')
    {
    next[next_index] = '\0';
    //printf("%s ", next);
    next_index = -1; 
    if (strcmp(next, "nan") == 0)
    {
    val = INFINITY;//212135568434083691824563425426542564345434;
    }
    else if (strcmp(next, "-nan") == 0)
    {
    val = -INFINITY;//212135568434083691851456104561245624564256;
    }
    else
    {
    val = strtod(next, NULL);
    } 

    if (continueI_HWeights == 1)
    {
    //printf("%li et %li\n", firstIndex, secondIndex);
    matrix_set(nn->input_hiddenWeights, secondIndex, firstIndex, val);
    secondIndex += 1;
    if (secondIndex == nn->hiddenNodes)
    {
    firstIndex += 1;
    secondIndex = 0;
    }
    if (firstIndex == nn->inputNodes)
    {
    firstIndex = 0;
    continueI_HWeights = 0;
    }
    }

    else if (continueH_Biases == 1)
    {
    //printf("%li\n", firstIndex);
    matrix_set(nn->hidden_biases, firstIndex, 0, val);
    firstIndex += 1;
    if (firstIndex == nn->hidden_biases->row)
    {
    firstIndex = 0;
    continueH_Biases = 0;
    }
    }

    else if (continueH_OWeights == 1)
    {
    //printf("%li et %li -> %li et %li\n", firstIndex, secondIndex, nn->hiddenNodes, nn->outputNodes);
    matrix_set(nn->hidden_outputWeights, firstIndex, secondIndex, val);
    secondIndex += 1;
    if (secondIndex == nn->hiddenNodes)
    {
        firstIndex += 1;
        secondIndex = 0;
    }
    if (firstIndex == nn->outputNodes)
    {
        firstIndex = 0;
        continueH_OWeights = 0;
    }
}

else if (continueO_Biases == 1)
{
    //printf("%li\n", firstIndex);
    matrix_set(nn->output_biases, firstIndex, 0, val);
    firstIndex += 1;
    if (firstIndex == nn->output_biases->row)
    {
        firstIndex = 0;
        continueO_Biases = 0;
    }
}
}
next_index += 1;
if (next_index > 199)
{
    next_index = 0;
}
buf_index += 1;
}

*/
}
