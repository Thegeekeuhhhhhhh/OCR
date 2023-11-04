#include "neuralnetwork.h"
#include "matrix.h"

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
    Matrix *hiddenLayer_error = matrix_dot_product(
            matrix_transpose(nn->hidden_outputWeights), outputLayer_error);

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
    matrix_free(inputMatrix);
    matrix_free(hidden);
    //matrix_free(output);
    //matrix_free(outputLayer_error); 
    //matrix_free(gradient);
    matrix_free(transposed_hidden);
    matrix_free(dhidden_outputWeights); 
    //matrix_free(hiddenLayer_error);
    //matrix_free(hiddenLayer_gradient); 
    //matrix_free(transposed_input);
    matrix_free(dinput_hiddenWeights);


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



    double t1[] = { 0.0f, 0.0f };
    double t2[] = { 1.0f, 0.0f };
    double t3[] = { 0.0f, 1.0f };
    double t4[] = { 1.0f, 1.0f };



    for (size_t tx = 1; tx < ite+1; tx++)
    {
        NeuralNetwork *nn = malloc(sizeof(NeuralNetwork));
        size_t NumberOfOutputs = 1;
        init_network(nn, 2, 4, NumberOfOutputs, learning_rate);
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

        // Converts wantedOutputs into a Matrix
        Matrix *wantedOutputsMatrix = malloc(sizeof(Matrix));
        matrix_init(wantedOutputsMatrix, NumberOfOutputs, 1);

        size_t trainingIndex[4] = {0, 1, 2, 3};
        //double t[2] = { 0.0f, 0.0f };


        // Principal loop (Loop on data n times, with n = epoch)
        for (size_t n = 0; n < EPOCHS; n++)
        {
            shuffle(trainingIndex, 4);
            //for (size_t a = 0; a < 4; a++)
            //{
            //size_t index = trainingIndex[a];
            //size_t a = rand() % 4;
            //}

            for (size_t a = 0; a < 4; a++)
            {
                matrix_set(wantedOutputsMatrix, 0, 0,
                        wantedOutputs[trainingIndex[a]][0]);
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

                train(nn, inputs[trainingIndex[a]], 2, wantedOutputsMatrix);
                //matrix_output_print(feedforward_algo(nn, t, 2));
                //matrix_print(wantedOutputsMatrix);
                //array_print(inputs[index], 2);
                //separator();
            }
        }

        //separator();

        printf("Training n°%li :\n", tx);
        printf("----------------------------------------------------\n");
        Matrix *test = feedforward_algo(nn, t1, 2);
        printf("| Inputs : 0 0 | Outputs : %lf | Expected : 0 |\n"
                , matrix_get(test, 0, 0));

        //matrix_free(test);
        test = feedforward_algo(nn, t2, 2);
        printf("| Inputs : 1 0 | Outputs : %lf | Expected : 1 |\n"
                , matrix_get(test, 0, 0));

        //matrix_free(test);
        test = feedforward_algo(nn, t3, 2);
        printf("| Inputs : 0 1 | Outputs : %lf | Expected : 1 |\n"
                , matrix_get(test, 0, 0));

        //matrix_free(test);
        test = feedforward_algo(nn, t4, 2);
        printf("| Inputs : 1 1 | Outputs : %lf | Expected : 0 |\n"
                , matrix_get(test, 0, 0));
        printf("----------------------------------------------------\n");

        //matrix_free(test);




        if (tx == ite)
        {
            // Save the data of the neural network in a file
            FILE *fileptr = NULL;
            fileptr = fopen("Values.txt", "w");
            fprintf(fileptr, "Input to hidden layer Weights :\n");
            for (size_t i = 0; i < nn->inputNodes; i++)
            {
                fprintf(fileptr, "|");
                for (size_t j = 0; j < nn->hiddenNodes; j++)
                {
                    double temp = matrix_get(nn->input_hiddenWeights, i, j);
                    fprintf(fileptr, "%lf\t|", temp);
                }
                fprintf(fileptr, "\n");
            }

            fprintf(fileptr, "\nHidden layer biases :\n");
            for (size_t i = 0; i < nn->hidden_biases->row; i++)
            {
                double temp = matrix_get(nn->hidden_biases, i, 0);
                fprintf(fileptr, "|%lf\t|\n", temp);
            }

            fprintf(fileptr, "\nHidden to output layer Weights :\n");
            for (size_t i = 0; i < nn->outputNodes; i++)
            {
                fprintf(fileptr, "|");
                for (size_t j = 0; j < nn->hiddenNodes; j++)
                {
                    double temp = matrix_get(nn->hidden_outputWeights, i, j);
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

    return 0;
}
