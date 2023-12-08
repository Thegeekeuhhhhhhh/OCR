#include "neuralnetwork.h"
#include "matrix.h"
#include <unistd.h>
#include <SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void init_network(NeuralNetwork *nn, size_t inputNumber, size_t hidden1Number,
        size_t hidden2Number, size_t outputNumber, double learning_rate)
{
    nn->lr = learning_rate;
    nn->inputNodes = inputNumber;
    nn->hidden1Nodes = hidden1Number;
    nn->hidden2Nodes = hidden2Number;
    nn->outputNodes = outputNumber;

    nn->input_hidden1Weights = malloc(sizeof(Matrix));
    // Represents weights between Input layer and hidden layer
    matrix_init(nn->input_hidden1Weights, nn->hidden1Nodes, nn->inputNodes);

    // Randomizing weights values between input and hidden layer
    for (size_t i = 0; i < nn->input_hidden1Weights->row; i++)
    {
        for (size_t j = 0; j < nn->input_hidden1Weights->col; j++)
        {
            double temp = (((double)rand()) / ((double)RAND_MAX))*2 - 1;
            matrix_set(nn->input_hidden1Weights, i, j, temp);
        }
    }

    // Randomizing weights values between hidden layers
    nn->hidden1_hidden2Weights = malloc(sizeof(Matrix));
    // Represents weights between hidden layers

    matrix_init(nn->hidden1_hidden2Weights, nn->hidden2Nodes, nn->hidden1Nodes);

    for (size_t i = 0; i < nn->hidden1_hidden2Weights->row; i++)
    {
        for (size_t j = 0; j < nn->hidden1_hidden2Weights->col; j++)
        {
            double temp = (((double)rand()) / ((double)RAND_MAX))*2 - 1;
            matrix_set(nn->hidden1_hidden2Weights, i, j, temp);
        }
    }

    // Randomizing weights values between second hidden and output layer
    nn->hidden2_outputWeights = malloc(sizeof(Matrix));
    // Represents weights between hidden layer and Output layer

    matrix_init(nn->hidden2_outputWeights, nn->outputNodes, nn->hidden2Nodes);

    for (size_t i = 0; i < nn->hidden2_outputWeights->row; i++)
    {
        for (size_t j = 0; j < nn->hidden2_outputWeights->col; j++)
        {
            double temp = (((double)rand()) / ((double)RAND_MAX))*2 - 1;
            matrix_set(nn->hidden2_outputWeights, i, j, temp);
        }
    }


    nn->hidden1_biases = malloc(sizeof(Matrix));
    matrix_init(nn->hidden1_biases, nn->hidden1Nodes, 1);
    // This is a single column matrix

    for (size_t i = 0; i < nn->hidden1_biases->row; i++)
    {
        double temp = (((double)rand()) / ((double)RAND_MAX))*2 - 1;
        matrix_set(nn->hidden1_biases, i, 0, temp);
    }


    nn->hidden2_biases = malloc(sizeof(Matrix));
    matrix_init(nn->hidden2_biases, nn->hidden2Nodes, 1);
    // This is a single column matrix

    for (size_t i = 0; i < nn->hidden2_biases->row; i++)
    {
        double temp = (((double)rand()) / ((double)RAND_MAX))*2 - 1;
        matrix_set(nn->hidden2_biases, i, 0, temp);
    }

    nn->output_biases = malloc(sizeof(Matrix));
    matrix_init(nn->output_biases, nn->outputNodes, 1);
    // Single column matrix too

    for (size_t i = 0; i < nn->output_biases->row; i++)
    {
        double temp = (((double)rand()) / ((double)RAND_MAX))*2 - 1;
        matrix_set(nn->output_biases, i, 0, temp);
    }
}

void free_network(NeuralNetwork *nn)
{
    matrix_free(nn->input_hidden1Weights);
    matrix_free(nn->hidden1_hidden2Weights);
    matrix_free(nn->hidden2_outputWeights);
    matrix_free(nn->hidden1_biases);
    matrix_free(nn->hidden2_biases);
    matrix_free(nn->output_biases);
    free(nn);
}

double sigmoid(double value)
{
    return 1.0f / (1.0f + exp(value * (-1)));
}

double dsigmoid(double value)
{
    double temp = sigmoid(value);
    return (temp * (1 - temp));
}

double ReLU(double value)
{
    if (value >= 0)
    {
        return value;
    }
    return 0.01f * value;
}

double dReLU(double value)
{
    if (value <= 0)
    {
        return 0.0f;
    }
    return 1.0f;
}

double simpledsigmoid(double value)
{
    /*
       Will be called sometimes
       We consider that value is already a sigmoid value
       */
    return value * (1-value);
}

void digit_print(double input[])
{
    int p = 0;
    for (int f = 0; f < 784; f++)
    {
        if (input[f] >= 0.5) { printf("1 "); }
        else { printf("  "); }
        if (p % 28 == 0) { printf("\n"); }
        p++;
    }
    printf("\n");
}

Matrix *feedforward_algo(NeuralNetwork *nn, double inputs[], size_t len)
{
    Matrix *inputMatrix = malloc(sizeof(Matrix));
    matrix_init(inputMatrix, len, 1); // inputs[] is a column matrix
    for (size_t i = 0; i < len; i++)
    {
        inputMatrix->data[i] = inputs[i];
        // Transpose values present in the input[] argument in the new matrix
    }

    printf("\n\nFEEDFORWARD\n\n");

    //matrix_print(inputMatrix);
    //printf("\n");

    // Computes hidden1's outputs
    Matrix *hidden1 = matrix_dot_product(nn->input_hidden1Weights, inputMatrix);
    matrix_add_in_place(hidden1, nn->hidden1_biases);

    //printf("Hidden 1 Matrix :\n");
    //matrix_print(hidden1);
    //printf("\n");
    //printf("Step 1 done\n");

    // Activation
    //matrix_softmax(hidden1);
    matrix_apply_function_in_place(hidden1, sigmoid);
    // Sets all the values between 0 and 1
    
    //matrix_print(hidden1);
    //printf("\n");
    //printf("Step 2 done\n");

    // Computes hidden2's outputs
    Matrix *hidden2 = matrix_dot_product(nn->hidden1_hidden2Weights, hidden1);
    matrix_add_in_place(hidden2, nn->hidden2_biases);

    //printf("Hidden 2 Matrix :\n");
    //matrix_print(hidden2);
    //printf("\n");

    // Activation
    //matrix_softmax(hidden2);
    matrix_apply_function_in_place(hidden2, sigmoid);
    // Sets all the values between 0 and 1
    //matrix_print(hidden2);
    //printf("\n");


    // We obtained the results at the hidden layer, now we are going to compute,
    // with the same process, the results for the output layer
    // Computes output's outputs
    Matrix *output = matrix_dot_product(nn->hidden2_outputWeights, hidden2);
    matrix_add_in_place(output, nn->output_biases);

    //printf("OUTPUT Matrix :\n");
    //matrix_print(output);
    //printf("\n");

    // Acvtivation
    matrix_apply_function_in_place(output, sigmoid);

    //matrix_print(output);
    //printf("\n");


    matrix_free(inputMatrix); 
    matrix_free(hidden1);
    matrix_free(hidden2);
    
    /*
    free(inputMatrix->data);
    free(hidden1->data);
    free(hidden2->data);

    free(inputMatrix);
    free(hidden1);
    free(hidden2);
    */

    //matrix_print(output);

    return output;
}

void train(NeuralNetwork *nn, double inputs[], size_t len, Matrix *wanted)
{
    Matrix *inputMatrix = malloc(sizeof(Matrix));
    matrix_init(inputMatrix, len, 1); // inputs[] is a column matrix
    for (size_t i = 0; i < len; i++)
    {
        inputMatrix->data[i] = inputs[i];
        // Set values present in the input[] argument in the new matrix
    }

    // Computes hidden1's outputs
    Matrix *hidden1 = matrix_dot_product(nn->input_hidden1Weights, inputMatrix);
    matrix_add_in_place(hidden1, nn->hidden1_biases);

    //printf("Hidden 1 Matrix :\n");
    //matrix_print(hidden1);
    //printf("Step 1 Done\n");

    // Activation, sets all values between 0.0 and 1.0
    matrix_apply_function_in_place(hidden1, sigmoid);

    //matrix_print(hidden1);
    //printf("Step 2 Done\n");

    // Computes hidden2's outputs
    Matrix *hidden2 = matrix_dot_product(nn->hidden1_hidden2Weights, hidden1);
    matrix_add_in_place(hidden2, nn->hidden2_biases);

    //printf("Hidden 2 Matrix :\n");
    //matrix_print(hidden2);
    //printf("Step 1 Done\n");

    // Activation, sets all values between 0.0 and 1.0
    matrix_apply_function_in_place(hidden2, sigmoid);
    //matrix_print(hidden2);
    //printf("Step 2 Done\n");


    // We obtained the results at the hidden layer, now we are going to compute,
    // with the same process, the results for the output layer
    // Computes output's outputs
    Matrix *output = matrix_dot_product(nn->hidden2_outputWeights, hidden2);
    matrix_add_in_place(output, nn->output_biases);

    //printf("OUTPUT MATRIX :\n");
    //matrix_print(output);
    //printf("\n");

    // Acvtivation
    matrix_apply_function_in_place(output, sigmoid);
    //matrix_print(output);
    //printf("Step 1 DONE\n");

    /*    
          matrix_print(output);
          printf("After softmax\n");

          matrix_softmax(output);
          */

    /*
       printf("Output : \n");
       matrix_print(output);
       printf("\nWanted Output : \n");
       matrix_print(wanted);

       digit_print(inputs); 
       */

    // =========================================================================
    // Feed Forward processed
    // =========================================================================


    // Computes errors : Error = wanted - output
    Matrix *outputLayer_error = matrix_sub(wanted, output);

    // Computes gradient
    Matrix *gradient_out = matrix_apply_function(output, simpledsigmoid);
    //Matrix *gradient_out = matrix_apply_function(output, dReLU);
    matrix_mul_in_place(gradient_out, outputLayer_error);
    matrix_scalar_mul_in_place(gradient_out, nn->lr);

    // Computes deltas of hidden to output layer's weights
    Matrix *transposed_hidden2 = matrix_transpose(hidden2);
    Matrix *dhidden2_outputWeights = matrix_dot_product(
            gradient_out, transposed_hidden2);


    // Merge (add) old values of weights between hidden and output layer
    // with the new changes, (corresponds to the gradient)
    // Adjusting weights and biases by infinitesimal values
    matrix_add_in_place(nn->hidden2_outputWeights, dhidden2_outputWeights);
    matrix_add_in_place(nn->output_biases, gradient_out);


    // THEN, we need the same process as for hidden to output layer's weights
    // Matrix *transposed_matrix = matrix_transpose(nn->hidden_outputWeights);
    // And then, computes the dot product of this transposed matrix with
    // the errors found for the output layer to find the hidden layer errors
    Matrix *transposed_h2_oWeights = matrix_transpose(nn->hidden2_outputWeights);
    Matrix *hidden2Layer_error = matrix_dot_product(
            transposed_h2_oWeights, outputLayer_error);

    Matrix *gradient_h2 = matrix_apply_function(hidden2, simpledsigmoid);
    //Matrix *gradient_h2 = matrix_apply_function(hidden2, dReLU);

    matrix_mul_in_place(gradient_h2, hidden2Layer_error);
    matrix_scalar_mul_in_place(gradient_h2, nn->lr);

    // Computes deltas of input to hidden layer's weights
    // Now, remember that the equivalent of variable "hidden" is "inputMatrix"
    // for the inputs layer
    Matrix *transposed_hidden1 = matrix_transpose(hidden1);
    Matrix *dhidden1_hidden2Weights = matrix_dot_product(
            gradient_h2, transposed_hidden1);

    // Adjusting weights and biases with infinitesimal values
    matrix_add_in_place(nn->hidden1_hidden2Weights, dhidden1_hidden2Weights);
    matrix_add_in_place(nn->hidden2_biases, gradient_h2);


    // FINAL STEP
    Matrix *transposed_h1_h2Weights = matrix_transpose(nn->hidden1_hidden2Weights);
    Matrix *hidden1Layer_error = matrix_dot_product(
            transposed_h1_h2Weights, hidden2Layer_error);

    Matrix *gradient_h1 = matrix_apply_function(hidden1, simpledsigmoid);
    //Matrix *gradient_h1 = matrix_apply_function(hidden1, dReLU);
    matrix_mul_in_place(gradient_h1, hidden1Layer_error);
    matrix_scalar_mul_in_place(gradient_h1, nn->lr);

    // Computes deltas of input to hidden layer's weights
    // Now, remember that the equivalent of variable "hidden" is "inputMatrix"
    // for the inputs layer
    Matrix *transposed_input = matrix_transpose(inputMatrix);
    Matrix *dinput_hidden1Weights = matrix_dot_product(
            gradient_h1, transposed_input);

    // FINAL STEP
    // Adjusting weights and biases with infinitesimal values
    matrix_add_in_place(nn->input_hidden1Weights, dinput_hidden1Weights);
    matrix_add_in_place(nn->hidden1_biases, gradient_h1);


    // Frees the Matrixes
    matrix_free(transposed_h2_oWeights);
    matrix_free(transposed_h1_h2Weights);
    matrix_free(dhidden2_outputWeights);
    matrix_free(dhidden1_hidden2Weights);
    matrix_free(dinput_hidden1Weights);

    matrix_free(outputLayer_error); // DEL if problems
    matrix_free(hidden2Layer_error);
    matrix_free(hidden1Layer_error);

    matrix_free(gradient_h1); // Dip
    matrix_free(gradient_h2); // Dip
    matrix_free(gradient_out); // Dip

    matrix_free(transposed_hidden1); // Dip
    matrix_free(transposed_hidden2); // Dip

    matrix_free(inputMatrix); // Dip +1
    matrix_free(hidden1); // Dip 
    matrix_free(hidden2); // Dip
    matrix_free(output); // Dip +1

    matrix_free(transposed_input); // Dip

    // Now, we have to compute infinitesimal values
    // Formulas :
    // ? output = sigmoid(weight * input + bias)
    // ? output = mx + b
    // dB (delta bias) = lr * error
    // dM (delta m) = lr * error * x
    //
    // dW_h->o (delta weight from hidden layer to output layer)
    //         = lr * output_error * dsigmoid(output)
    //          [DOT_PROD] transposed(hidden_layer)
    //
    // dW_i->h (delta weight fron input layer to hidden layer)
    //         = lr * hidden_error * dsigmoid(hidden)
    //         [DOT_PROD] transposed(input_layer)
    //
    // Do not forget that this is BACKPROPAGATION, later, if we need another
    // layer, we will need to calculate an other layer of errors
}

void shuffle(size_t array[], size_t len)
{
    for (size_t i = 0; i < len-1; i++)
    {
        size_t j = i + rand() / (RAND_MAX / (len-i)+1);
        size_t temp = array[j];
        array[j] = array[i];
        array[i] = temp;
    }
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        errx(1,
                "\nUsage :\n./neuralnetwork [Number of trainings] [EPOCHS] [Learning rate]"
            );
    }
    srand(time(NULL));

    size_t ite = 0;
    size_t EPOCHS = 0;
    double learning_rate = 1.0f;
    char **p = argv;
    p++;
    char *temp = *p;
    while (*temp != '\0')
    {
        if (*temp >= '0' && *temp <= '9')
        {
            ite *= 10;
            ite += (size_t)(*temp - '0');
        }
        else
        {
            errx(1, "\n[Trainings] Detected something that is not a digit");
        }
        temp++;
    }
    p++;
    temp = *p;
    while (*temp != '\0')
    {
        if (*temp >= '0' && *temp <= '9')
        {
            EPOCHS *= 10;
            EPOCHS += (size_t)(*temp - '0');
        }
        else
        {
            errx(1, "\n[EPOCH] Detected something that is not a digit");
        }
        temp++;
    }
    p++;
    temp = *p;
    learning_rate = strtod(temp, NULL);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        errx(1, "Erreur d'initialisation de SDL");
        return -1;
    }

    double inputs[900][784];
    double wantedOutputs[900][10];

    for (size_t tx = 1; tx < ite+1; tx++)
    {
        NeuralNetwork *nn = malloc(sizeof(NeuralNetwork));
        init_network(nn, 784, 16, 16, 10, learning_rate);

        int max = 100;
        for (int chapter = 1; chapter <= 9; chapter++)
        {
            for (int index = 1; index <= max; index++)
            {
                char str[50];
                sprintf(str, "digitalwritten_digits/%i/%i.jpg", chapter, index);
                //printf("%s\n", str);

                SDL_Surface* tmpSurface = IMG_Load(str);
                if (tmpSurface == NULL)
                {
                    errx(1, "Quoicouprout");
                }
                SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmpSurface,
                        SDL_PIXELFORMAT_RGB888, 0);

                SDL_FreeSurface(tmpSurface);
                if(surface == NULL)
                {
                    SDL_FreeSurface(tmpSurface);
                    errx(1, "AMPANYANE");
                }
                tmpSurface = NULL;
                Uint32 *pixels = surface->pixels;

                //int p = 0;
                //printf("Number : %i\n", chapter);
                for (size_t f = 0; f < 784; f++)
                {
                    Uint8 r, g, b;
                    SDL_GetRGB(pixels[f], surface->format, &r, &g, &b);
                    double gs =  (0.32*r + 0.57*g + 0.11*b) / 255.0f;
                    if (gs >= 0.5f)
                    {
                        inputs[(chapter-1)*max + index-1][f] = 1;
                    }
                    else
                    {
                        inputs[(chapter-1)*max + index-1][f] = 0;
                    }

                    //if (inputs[(chapter-1)*max + index-1][f] >= 0.5) { printf("1 "); }
                    //else { printf("  "); }
                    //if (p % 28 == 0) { printf("\n"); }
                    //p++;

                    //printf("%f ", inputs[(chapter-1)*max + index-1][f]);
                }

                //printf("\n");

                for (size_t o = 0; o < 10; o++)
                {
                    wantedOutputs[(chapter-1) * max + index-1][o] = 0.0f;
                }
                wantedOutputs[(chapter-1) * max + index-1][chapter] = 1.0f;
                SDL_FreeSurface(surface);
                surface = NULL;
            }
        }

        // Converts wantedOutputs into a Matrix
        Matrix *wantedOutputsMatrix = malloc(sizeof(Matrix));
        matrix_init(wantedOutputsMatrix, 10, 1);

        size_t trainingIndex[900];
        for (size_t i = 0; i < 900; i++)
        {
            trainingIndex[i] = i;
        }

        // Principal loop (Loop on data n times, with n = epoch)
        for (size_t n = 0; n < EPOCHS; n++)
        {
            shuffle(trainingIndex, 900);
            for (int a = 0; a < 900; a++)
            {
                for (size_t i = 0; i < wantedOutputsMatrix->row; i++)
                {
                    double temp = wantedOutputs[trainingIndex[a]][i];
                    //double temp = wantedOutputs[id_temp][i];
                    matrix_set(wantedOutputsMatrix, i, 0, temp);
                }

                //printf("Matrice voulue\n");
                //matrix_print(wantedOutputsMatrix);
                train(nn, inputs[trainingIndex[a]], 784, wantedOutputsMatrix); 
                //train(nn, inputs[id_temp], 784, wantedOutputsMatrix);
            }

        }

        printf("Training n°%li :\n", tx);
        printf("----------------------------------------------------\n");

        double tests[76][784];

        for (int index = 1; index <= 76; index++)
        {
            char str[50];
            sprintf(str, "digitalwritten_digits/tests/%i.jpg", index);
            SDL_Surface* tmpSurface = IMG_Load(str);
            if (tmpSurface == NULL)
            {
                errx(1, "Quoicouprout");
            }
            SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmpSurface,
                    SDL_PIXELFORMAT_RGB888, 0);
            SDL_FreeSurface(tmpSurface);
            if(surface == NULL)
            {
                SDL_FreeSurface(tmpSurface);
                errx(1, "AMPANYANE");
            }
            tmpSurface = NULL;
            Uint32 *pixels = surface->pixels;
            for (size_t f = 0; f < 784; f++)
            {
                Uint8 r, g, b;
                SDL_GetRGB(pixels[f], surface->format, &r, &g, &b);
                double gs =  (0.32*r + 0.57*g + 0.11*b) / 255.0f;
                if (gs >= 0.5f)
                {
                    tests[index-1][f] = 1;
                }
                else
                {
                    tests[index-1][f] = 0;
                }
            }
            SDL_FreeSurface(surface);
            surface = NULL;
        }

        for (size_t r = 0; r < 2; r++)
        {
           digit_print(tests[r]);
           Matrix *test1 = feedforward_algo(nn, tests[r], 784);
           matrix_print(test1);
           printf("\n");
           matrix_free(test1);
           sleep(1);
        }


        /*
           Matrix *test2 = feedforward_algo(nn, inputs[1], 784);
           matrix_print(test2);
           printf("\n"); 
           matrix_free(test2);

           Matrix *test3 = feedforward_algo(nn, inputs[2], 784);
           matrix_print(test3);
           printf("\n");
           matrix_free(test3);

           Matrix *test4 = feedforward_algo(nn, inputs[3], 784);
           matrix_print(test4);
        //printf("\n");
        matrix_free(test4);

*/

        printf("----------------------------------------------------\n");

        if (tx == ite)
        {
            // Save the data of the neural network in a file
            FILE *fileptr = NULL;
            fileptr = fopen("Values.txt", "w");
            fprintf(fileptr, "Input to hidden 1 layer Weights :\n");
            for (size_t i = 0; i < nn->inputNodes; i++)
            {
                fprintf(fileptr, "|");
                for (size_t j = 0; j < nn->hidden1Nodes; j++)
                {
                    double temp = matrix_get(nn->input_hidden1Weights, j, i);
                    fprintf(fileptr, "%lf\t|", temp);
                }
                fprintf(fileptr, "\n");
            }

            fprintf(fileptr, "\nHidden 1 layer biases :\n");
            for (size_t i = 0; i < nn->hidden1_biases->row; i++)
            {
                double temp = matrix_get(nn->hidden1_biases, i, 0);
                fprintf(fileptr, "|%lf\t|\n", temp);
            }

            fprintf(fileptr, "\nHidden 1 to hidden 2 layer Weights :\n");
            for (size_t i = 0; i < nn->hidden1Nodes; i++)
            {
                fprintf(fileptr, "|");
                for (size_t j = 0; j < nn->hidden2Nodes; j++)
                {
                    double temp = matrix_get(nn->hidden1_hidden2Weights, j, i);
                    fprintf(fileptr, "%lf\t|", temp);
                }
                fprintf(fileptr, "\n");
            }
            fprintf(fileptr, "\nHidden 2 layer biases :\n");
            for (size_t i = 0; i < nn->hidden2_biases->row; i++)
            {
                double temp = matrix_get(nn->hidden2_biases, i, 0);
                fprintf(fileptr, "|%lf\t|\n", temp);
            }

            fprintf(fileptr, "\nHidden 2 to output layer Weights :\n");
            for (size_t i = 0; i < nn->hidden2Nodes; i++)
            {
                fprintf(fileptr, "|");
                for (size_t j = 0; j < nn->outputNodes; j++)
                {
                    double temp = matrix_get(nn->hidden2_outputWeights, j, i);
                    fprintf(fileptr, "%lf\t|", temp);
                }
                fprintf(fileptr, "\n");
            }

            fprintf(fileptr, "\nOutput layer biases :\n");
            for (size_t i = 0; i < nn->output_biases->row; i++)
            {
                double temp = matrix_get(nn->output_biases, i, 0);
                fprintf(fileptr, "|%lf\t|\n", temp);
            }

            fclose(fileptr);
        }



        // Frees malloced variables
        matrix_free(wantedOutputsMatrix);
        free_network(nn);
    }

    SDL_Quit();
    return 0;
}
