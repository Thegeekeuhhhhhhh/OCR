#include "neuralnetwork.h"
#include "matrix.h"
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
            double temp = ((double)rand()) / ((double)RAND_MAX);
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
            double temp = ((double)rand()) / ((double)RAND_MAX);
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
            double temp = ((double)rand()) / ((double)RAND_MAX);
            matrix_set(nn->hidden2_outputWeights, i, j, temp);
        }
    }


    nn->hidden1_biases = malloc(sizeof(Matrix));
    matrix_init(nn->hidden1_biases, nn->hidden1Nodes, 1);
    // This is a single column matrix

    for (size_t i = 0; i < nn->hidden1_biases->row; i++)
    {
        double temp = ((double)rand()) / ((double)RAND_MAX);
        matrix_set(nn->hidden1_biases, i, 0, temp);
    }


    nn->hidden2_biases = malloc(sizeof(Matrix));
    matrix_init(nn->hidden2_biases, nn->hidden2Nodes, 1);
    // This is a single column matrix

    for (size_t i = 0; i < nn->hidden2_biases->row; i++)
    {
        double temp = ((double)rand()) / ((double)RAND_MAX);
        matrix_set(nn->hidden2_biases, i, 0, temp);
    }

    nn->output_biases = malloc(sizeof(Matrix));
    matrix_init(nn->output_biases, nn->outputNodes, 1);
    // Single column matrix too

    for (size_t i = 0; i < nn->output_biases->row; i++)
    {
        double temp = ((double)rand()) / ((double)RAND_MAX);
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
    return 1 / (1 + exp(value * (-1)));
}

double dsigmoid(double value)
{
    double temp = sigmoid(value);
    return (temp * (1 - temp));
}

double simpledsigmoid(double value)
{
    /*
       Will be called sometimes
       We consider that value is already a sigmoid value
       */
    return value * (1-value);
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

    // Computes hidden1's outputs
    Matrix *hidden1 = matrix_dot_product(nn->input_hidden1Weights, inputMatrix);
    matrix_add_in_place(hidden1, nn->hidden1_biases);

    // Activation
    matrix_apply_function_in_place(hidden1, sigmoid);
    // Sets all the values between 0 and 1


    // Computes hidden2's outputs
    Matrix *hidden2 = matrix_dot_product(nn->hidden1_hidden2Weights, hidden1);
    matrix_add_in_place(hidden2, nn->hidden2_biases);

    // Activation
    matrix_apply_function_in_place(hidden2, sigmoid);
    // Sets all the values between 0 and 1


    // We obtained the results at the hidden layer, now we are going to compute,
    // with the same process, the results for the output layer
    // Computes output's outputs
    Matrix *output = matrix_dot_product(nn->hidden2_outputWeights, hidden2);
    matrix_add_in_place(output, nn->output_biases);

    // Acvtivation
    matrix_apply_function_in_place(output, sigmoid);


    free(inputMatrix->data);
    free(hidden1->data);
    free(hidden2->data);

    free(inputMatrix);
    free(hidden1);
    free(hidden2);

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

    // Activation, sets all values between 0.0 and 1.0
    matrix_apply_function_in_place(hidden1, sigmoid);

    // Computes hidden2's outputs
    Matrix *hidden2 = matrix_dot_product(nn->hidden1_hidden2Weights, hidden1);
    matrix_add_in_place(hidden2, nn->hidden2_biases);

    // Activation, sets all values between 0.0 and 1.0
    matrix_apply_function_in_place(hidden2, sigmoid);


    // We obtained the results at the hidden layer, now we are going to compute,
    // with the same process, the results for the output layer
    // Computes output's outputs
    Matrix *output = matrix_dot_product(nn->hidden2_outputWeights, hidden2);
    matrix_add_in_place(output, nn->output_biases);

    // Acvtivation
    matrix_apply_function_in_place(output, sigmoid);


    // =========================================================================
    // Feed Forward processed
    // =========================================================================


    // Computes errors : Error = wanted - output
    Matrix *outputLayer_error = matrix_sub(wanted, output);

    // Computes gradient
    Matrix *gradient_out = matrix_apply_function(output, simpledsigmoid);
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

    Matrix *gradient_h2 = matrix_apply_function(
            hidden2, simpledsigmoid);
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

    Matrix *gradient_h1 = matrix_apply_function(
            hidden1, simpledsigmoid);
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


    //matrix_free(outputLayer_error);
    matrix_free(hidden2Layer_error);
    matrix_free(hidden1Layer_error);


    //matrix_free(gradient_h1);
    //matrix_free(gradient_h2);
    //matrix_free(gradient_out);

    /*
       matrix_free(transposed_hidden1);
       matrix_free(transposed_hidden2);
       */

    /*
    //matrix_free(inputMatrix);
    matrix_free(hidden1);
    matrix_free(hidden2);
    //matrix_free(output);
    */




    /*
       free(outputLayer_error->data);

    //free(gradient_out->data);

    free(outputLayer_error);

    free(gradient_out);
    free(transposed_input);


    free(output->data);
    free(output);
    */


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


    /*
       SDL_Surface* tmpSurface = IMG_Load("dataset/1/1.jpg");
       if (tmpSurface == NULL)
       {
       errx(1, "Quoicouprout");
       }
       SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmpSurface,
       SDL_PIXELFORMAT_RGB888, 0);

       SDL_FreeSurface(tmpSurface);
       tmpSurface = NULL;

       if(surface == NULL)
       {
       SDL_FreeSurface(tmpSurface);
       errx(1, "AMPANYANE");
       }

       Uint32* pixels = surface->pixels;
       for(int i = 0; i < surface->h; i++)
       {
       for(int j = 0; j < surface->w; j++)
       {
       Uint8 r, g, b;
       SDL_GetRGB(pixels[i*surface->w + j], surface->format, &r, &g, &b);
       if(r > 100)
       {
       printf("1 ");
       }
       else
       {
       printf("0 ");
       }
       }
       printf("\n");
       }

       SDL_FreeSurface(surface);
       surface = NULL;
       */







    /*
    // Creates window
    SDL_Window* window = SDL_CreateWindow("LA FONCTION DE LEO",
    0, 0, 280, 280, 0);
    if (window == NULL)
    {
    errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
    errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    SDL_Texture* texture = IMG_LoadTexture(renderer, "dataset/1/img_0.jpg");
    int lenx;
    int leny;

    if (SDL_QueryTexture(texture, NULL, NULL, &lenx, &leny) != 0)
    errx(EXIT_FAILURE, "%s", SDL_GetError());


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    */

    //SDL_LockSurface(surface);


    //SDL_SaveBMP(surface, "somefile.bmp");
    //SDL_FreeSurface(surface);
    //SDL_UnlockSurface(surface);




    //double t1[] = { 0.0f, 0.0f };
    //double t2[] = { 1.0f, 0.0f };
    //double t3[] = { 0.0f, 1.0f };
    //double t4[] = { 1.0f, 1.0f };


    double inputs[10][784];
    double wantedOutputs[10][10];

    for (size_t tx = 1; tx < ite+1; tx++)
    {
        NeuralNetwork *nn = malloc(sizeof(NeuralNetwork));
        size_t NumberOfOutputs = 10;
        init_network(nn, 784, 16, 16, 10, learning_rate);
        // Nodes needed to let XOR AI work
        // Input layer : 2 Nodes
        // Hidden layer(s) : 2 Nodes
        // Output layer : 1 Node


        //double input[] = {1, 0};
        //Matrix *m = feedforward_algo(nn, input, 2);
        //for (size_t i = 0; i < m->dataLen; i++)
        // {
        //printf("%f ", m->data[i]);
        // }
        // printf("\n");



        /* 
           double inputs[4][2] =
           {
           { 0.0f, 0.0f },
           { 1.0f, 0.0f },
           { 0.0f, 1.0f },
           { 1.0f, 1.0f }
           };
           double wantedOutputs[4][1] =
           {
           { 0.0f },
           { 1.0f },
           { 1.0f },
           { 0.0f }
           };
           */

        //double inputs[10][784];
        //double wantedOutputs[10][10]; Premier 10 = nb inputs, autre 10 = nb sorties possibles

        for (int index = 1; index <= 10; index++)
        {
            char str[150];
            sprintf(str, "dataset/1/%i.jpg", index);
            //printf("%s\n", str);

            SDL_Surface* tmpSurface = IMG_Load(str);
            if (tmpSurface == NULL)
            {
                errx(1, "Quoicouprout");
            }
            SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmpSurface,
                    SDL_PIXELFORMAT_RGB888, 0);

            SDL_FreeSurface(tmpSurface);
            tmpSurface = NULL;

            if(surface == NULL)
            {
                SDL_FreeSurface(tmpSurface);
                errx(1, "AMPANYANE");
            }

            Uint32 *pixels = surface->pixels;
            for (size_t f = 0; f < 784; f++)
            {
                Uint8 r, g, b;
                SDL_GetRGB(pixels[f], surface->format, &r, &g, &b);
                inputs[index-1][f] = 0.32*r + 0.57*g + 0.11*b;
            }

            wantedOutputs[index-1][0] = 0.0f;
            wantedOutputs[index-1][1] = 1.0f; // On ne test qu avec des 1 pour le moment
            for (size_t o = 2; o < 10; o++)
            {
                wantedOutputs[index-1][o] = 0.0f;
            }

            SDL_FreeSurface(surface);
            surface = NULL;
        }

        /*
           Uint32* pixels = surface->pixels;
           for(int i = 0; i < surface->h; i++)
           {
           for(int j = 0; j < surface->w; j++)
           {
           Uint8 r, g, b;
           SDL_GetRGB(pixels[i*surface->w + j], surface->format, &r, &g, &b);
           if(r > 100)
           {
           printf("1 ");
           }
           else
           {
           printf("0 ");
           }
           }
           printf("\n");
           }
           */



        // Converts wantedOutputs into a Matrix
        Matrix *wantedOutputsMatrix = malloc(sizeof(Matrix));
        matrix_init(wantedOutputsMatrix, NumberOfOutputs, 1);

        size_t trainingIndex[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        //double t[2] = { 0.0f, 0.0f };


        // Principal loop (Loop on data n times, with n = epoch)
        for (size_t n = 0; n < EPOCHS; n++)
        {
            shuffle(trainingIndex, 10);
            //for (size_t a = 0; a < 4; a++)
            //{
            //size_t index = trainingIndex[a];
            //size_t a = rand() % 4;
            //}

            for (size_t a = 0; a < 10; a++)
            {
                for (size_t b = 0; b < 10; b++)
                {
                matrix_set(wantedOutputsMatrix, 0, 0,
                        wantedOutputs[trainingIndex[a]][b]);
                //
                //  for (size_t i = 0; i < wantedOutputsMatrix->row; i++)
                //   {
                //   for (size_t j = 0; j < wantedOutputsMatrix->col; j++)
                //  {
                // double temp = wantedOutputs[a][i * wantedOutputsMatrix->col + j];
                // matrix_set(wantedOutputsMatrix, i, j, temp);
                // }
                // }
                //


                //matrix_print(wantedOutputsMatrix);
                //array_print(inputs[index], 2);

                train(nn, inputs[trainingIndex[a]], 784, wantedOutputsMatrix);
                }
                //matrix_output_print(feedforward_algo(nn, t, 2));
                //matrix_print(wantedOutputsMatrix);
                //array_print(inputs[index], 2);
                //separator();
            }
        }


        //separator();

        
        printf("Training n°%li :\n", tx);
        printf("----------------------------------------------------\n");
        Matrix *test1 = feedforward_algo(nn, inputs[0], 784);
        matrix_print(test1);
        /*

        printf("| Inputs : 0 0 | Outputs : %lf | Expected : 0 |\n"
                , matrix_get(test1, 0, 0));
        //free(test1);


        
           Matrix *test2 = feedforward_algo(nn, inputs[1], 784);
           printf("| Inputs : 1 0 | Outputs : %lf | Expected : 1 |\n"
           , matrix_get(test2, 0, 0));
        //free(test2);

        Matrix *test3 = feedforward_algo(nn, inputs[2], 784);
        printf("| Inputs : 0 1 | Outputs : %lf | Expected : 1 |\n"
        , matrix_get(test3, 0, 0));
        //free(test3);

        Matrix *test4 = feedforward_algo(nn, inputs[3], 784);
        printf("| Inputs : 1 1 | Outputs : %lf | Expected : 0 |\n"
        , matrix_get(test4, 0, 0));
        printf("----------------------------------------------------\n");
        //free(test4);
        */



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
                    double temp = matrix_get(nn->hidden1_hidden2Weights, i, j);
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
                    double temp = matrix_get(nn->hidden2_outputWeights, i, j);
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
