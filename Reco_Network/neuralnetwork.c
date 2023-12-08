#include "neuralnetwork.h"
#include "matrix.h"
#include <unistd.h>
#include <SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void init_network(NeuralNetwork *nn, size_t inputNumber, size_t hiddenNumber, size_t outputNumber, double learning_rate)
{
    nn->lr = learning_rate;
    nn->inputNodes = inputNumber;
    nn->hiddenNodes = hiddenNumber;
    nn->outputNodes = outputNumber;

    nn->input_hiddenWeights = malloc(sizeof(Matrix));
    // Represents weights between Input layer and hidden layer
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
    nn->hidden_outputWeights = malloc(sizeof(Matrix));
    // Represents weights between hidden layer and Output layer

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
    matrix_init(nn->hidden_biases, nn->hiddenNodes, 1);
    // This is a single column matrix

    for (size_t i = 0; i < nn->hidden_biases->row; i++)
    {             
        double temp = ((double)rand()) / ((double)RAND_MAX);
        matrix_set(nn->hidden_biases, i, 0, temp);                
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
    matrix_free(nn->input_hiddenWeights);
    matrix_free(nn->hidden_outputWeights);
    matrix_free(nn->hidden_biases);
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

    // Computes hidden's outputs
    Matrix *hidden = matrix_dot_product(nn->input_hiddenWeights, inputMatrix);
    matrix_add_in_place(hidden, nn->hidden_biases);

    // Activation
    matrix_apply_function_in_place(hidden, sigmoid);
    // Sets all the values between 0 and 1

    // We obtained the results at the hidden layer, now we are going to compute,
    // with the same process, the results for the output layer
    // Computes output's outputs
    Matrix *output = matrix_dot_product(nn->hidden_outputWeights, hidden);
    matrix_add_in_place(output, nn->output_biases);

    // Acvtivation
    matrix_apply_function_in_place(output, sigmoid);

    matrix_free(inputMatrix);
    matrix_free(hidden);

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

    // Computes hidden's outputs
    Matrix *hidden = matrix_dot_product(nn->input_hiddenWeights, inputMatrix);
    matrix_add_in_place(hidden, nn->hidden_biases);

    // Activation, sets all values between 0.0 and 1.0
    matrix_apply_function_in_place(hidden, sigmoid);

    // We obtained the results at the hidden layer, now we are going to compute,
    // with the same process, the results for the output layer
    // Computes output's outputs
    Matrix *output = matrix_dot_product(nn->hidden_outputWeights, hidden);
    matrix_add_in_place(output, nn->output_biases);

    // Acvtivation
    matrix_apply_function_in_place(output, sigmoid);


    // =========================================================================
    // Feed Forward processed
    // =========================================================================


    // Computes errors : Error = wanted - output
    Matrix *outputLayer_error = matrix_sub(wanted, output);

    // Computes gradient
    Matrix *gradient = matrix_apply_function(output, simpledsigmoid);
    matrix_mul_in_place(gradient, outputLayer_error);
    matrix_scalar_mul_in_place(gradient, nn->lr);

    // Computes deltas of hidden to output layer's weights
    Matrix *transposed_hidden = matrix_transpose(hidden);
    Matrix *dhidden_outputWeights = matrix_dot_product(
            gradient, transposed_hidden);


    // Merge (add) old values of weights between hidden and output layer
    // with the new changes, (corresponds to the gradient)
    // Adjusting weights and biases by infinitesimal values
    matrix_add_in_place(nn->hidden_outputWeights, dhidden_outputWeights);
    matrix_add_in_place(nn->output_biases, gradient);

    // THEN, we need the same process as for hidden to output layer's weights
    // Matrix *transposed_matrix = matrix_transpose(nn->hidden_outputWeights);
    // And then, computes the dot product of this transposed matrix with
    // the errors found for the output layer to find the hidden layer errors
    Matrix *hiddenLayer_error_T = matrix_transpose(nn->hidden_outputWeights);
    Matrix *hiddenLayer_error = matrix_dot_product(hiddenLayer_error_T
            , outputLayer_error);

    Matrix *hiddenLayer_gradient = matrix_apply_function(
            hidden, simpledsigmoid);
    matrix_mul_in_place(hiddenLayer_gradient, hiddenLayer_error);
    matrix_scalar_mul_in_place(hiddenLayer_gradient, nn->lr);

    // Computes deltas of input to hidden layer's weights
    // Now, remember that the equivalent of variable "hidden" is "inputMatrix"
    // for the inputs layer
    Matrix *transposed_input = matrix_transpose(inputMatrix);
    Matrix *dinput_hiddenWeights = matrix_dot_product(
            hiddenLayer_gradient, transposed_input);

    // FINAL STEP
    // Adjusting weights and biases with infinitesimal values
    matrix_add_in_place(nn->input_hiddenWeights, dinput_hiddenWeights);
    matrix_add_in_place(nn->hidden_biases, hiddenLayer_gradient);

    // Frees the Matrixes
    matrix_free(transposed_hidden);
    matrix_free(dhidden_outputWeights);
    matrix_free(dinput_hiddenWeights);

    matrix_free(outputLayer_error); // DEL if problems
    matrix_free(hiddenLayer_error);

    matrix_free(hiddenLayer_gradient); // Dip
    matrix_free(gradient); // Dip

    matrix_free(hiddenLayer_error_T);
    matrix_free(transposed_input); // Dip

    matrix_free(inputMatrix); // Dip +1
    matrix_free(hidden); // Dip
    matrix_free(output); // Dip +1


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
        errx(1, "Erreur d'init SDL");
        return -1;
    }

    size_t trainingNumber = 2700; // Multiple of 9 pls
    static double inputs[2700][784];
    static double wantedOutputs[2700][10];
    static size_t simpleOutputs[2700];
    for (size_t tx = 1; tx < ite+1; tx++)
    {
        NeuralNetwork *nn = malloc(sizeof(NeuralNetwork));
        init_network(nn, 784, 100, 10, learning_rate);
        // Nodes needed to let XOR AI work
        // Input layer : 2 Nodes
        // Hidden layer(s) : 2 Nodes
        // Output layer : 1 Node

        int max = trainingNumber/9;
        for (int chapter = 1; chapter <= 9; chapter++)
        {
            for (int index = 1; index <= max; index++)
            {
                char str[60];
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
                simpleOutputs[(chapter-1) * max + index-1] = chapter;
                SDL_FreeSurface(surface);
                surface = NULL;

                //printf("%i et %i\n", chapter, index);
            }
        }

        // Converts wantedOutputs into a Matrix
        Matrix *wantedOutputsMatrix = malloc(sizeof(Matrix));
        matrix_init(wantedOutputsMatrix, 10, 1);

        size_t trainingIndex[trainingNumber];
        for (size_t i = 0; i < trainingNumber; i++)
        {
            trainingIndex[i] = i;   
        }

        printf("Dataset loaded !\n");
        
        // Principal loop (Loop on data n times, with n = epoch)
        for (size_t n = 1; n <= EPOCHS; n++)
        {
            char beautiful[150];
            int percent = (int)(((double)n / (double)EPOCHS) * 100);
            for (int i = 0; i < percent; i++)
            {
                beautiful[i] = '=';
            }
            beautiful[percent] = '>';
            for (int i = percent + 1; i < 100; i++)
            {
                beautiful[i] = '_';
            }
            beautiful[100] = '\0';
            
            printf("%i[%s]\n\033[1A\033[K", percent, beautiful);
            shuffle(trainingIndex, trainingNumber);
            for (size_t a = 0; a < trainingNumber; a++)
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
            sleep(1);
        }
       
        printf("\nTraining n°%li :\n", tx);
        printf("----------------------------------------------------\n");

        int TestNumber = 10;
        double tests[TestNumber][784];
        size_t wantedO[TestNumber];
        for (int index = 1; index <= TestNumber; index++)
        {
            for (int j = 0; j < 784; j++)
            {
                tests[index-1][j] = inputs[trainingIndex[index-1]][j];
            }
            wantedO[index-1] = simpleOutputs[trainingIndex[index-1]];
            /*
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
            */
        }

        int success = 0;
        for (int r = 0; r < TestNumber; r++)
        {
           digit_print(tests[r]);
           Matrix *test1 = feedforward_algo(nn, tests[r], 784);

           matrix_print(test1);
           size_t ind_test = matrix_max_index(test1);
           printf("\nResult : %li, Expected : %li\n", ind_test, wantedO[r]);
           if (ind_test == wantedO[r])
           {
               success += 1;
           }
           matrix_free(test1);
           sleep(1);
        }
        printf("ACCURACY : %f%%\n", ((double)success / (double)TestNumber) * 100.0f);        
        if (tx == ite)
        {
            // Save the data of the neural network in a file
            FILE *fileptr = NULL;
            fileptr = fopen("Values.txt", "w");
            //fprintf(fileptr, "Characteristics : ");
            fprintf(fileptr, "Input to hidden layer Weights :\n");
            for (size_t i = 0; i < nn->inputNodes; i++)
            {
                fprintf(fileptr, "|");
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
                fprintf(fileptr, "|%lf|\n", temp);
            }

            fprintf(fileptr, "\nHidden to output layer Weights :\n");
            for (size_t i = 0; i < nn->outputNodes; i++)
            {
                fprintf(fileptr, "|");
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
                fprintf(fileptr, "|%lf|\n", temp);
            }

            fclose(fileptr);
        }



        // Frees malloced variables
        matrix_free(wantedOutputsMatrix);
        free_network(nn);
    }

    return 0;
}
