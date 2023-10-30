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
        matrix_set(nn->hidden_biases, i, 1, temp);                
    }

    nn->output_biases = malloc(sizeof(Matrix));
    matrix_init(nn->output_biases, nn->outputNodes, 1);
    // Single column matrix too

    for (size_t i = 0; i < nn->output_biases->row; i++)
    {
        double temp = ((double)rand()) / ((double)RAND_MAX);
        matrix_set(nn->output_biases, i, 1, temp);
    }						    

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

    // Computes hidden outputs
    Matrix *hidden = matrix_dot_product(nn->input_hiddenWeights, inputMatrix);
    matrix_add_in_place(hidden, nn->hidden_biases);

    // Activation
    matrix_apply_function_in_place(hidden, sigmoid);
    // Sets all the values between 0 and 1

    // We obtained the results at the hidden layer, now we are going to compute,
    // with the same process, the results for the output layer

    // Computes output outputs
    Matrix *output = matrix_dot_product(nn->hidden_outputWeights, hidden);
    matrix_add_in_place(output, nn->output_biases);

    // Acvtivation
    matrix_apply_function_in_place(output, sigmoid);

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

    // Computes hidden outputs
    
    
    //matrix_print(nn->input_hiddenWeights);
    //separator();
    //matrix_print(inputMatrix);
    //separator();

    Matrix *hidden = matrix_dot_product(nn->input_hiddenWeights, inputMatrix);
    matrix_add_in_place(hidden, nn->hidden_biases);

    // Activation
    matrix_apply_function_in_place(hidden, sigmoid);
    // Sets all the values between 0 and 1

    // We obtained the results at the hidden layer, now we are going to compute,
    // with the same process, the results for the output layer

    // Computes output outputs
    Matrix *output = matrix_dot_product(nn->hidden_outputWeights, hidden);
    matrix_add_in_place(output, nn->output_biases);

    // Acvtivation
    matrix_apply_function_in_place(output, sigmoid);



    //Matrix *output = feedforward_algo(nn, inputs, len);



    printf("Feed Forward executed !\n");




    // Computes errors : Error = wanted - output
    Matrix *outputLayer_error = matrix_sub(wanted, output);

    // Computes gradient
    Matrix *gradient = matrix_apply_function(output, simpledsigmoid);
    matrix_mul_in_place(gradient, outputLayer_error);
    matrix_scalar_mul_in_place(gradient, nn->lr);

    // Computes deltas of hidden to output layer's weights
    Matrix *transposed_hidden = matrix_transpose(hidden);
    Matrix *dhidden_outputWeights = matrix_dot_product(gradient, transposed_hidden);

    // MAYBE WE NEED TO SUB
    // Merge (add) old values of weights between hidden and output layer
    // with the new changes, (corresponds to the gradient)
    // Adjusting weights and biases by infinitesimal values
    
    //printf("Step before add in place\n");

    /*
    matrix_print(gradient);
    separator();
    matrix_print(transposed_hidden);
    separator();
    matrix_print(nn->hidden_outputWeights);
    separator();
    matrix_print(dhidden_outputWeights);
    separator();
    matrix_print(nn->output_biases);
    separator();
    matrix_print(gradient);
    separator();
    */

    matrix_add_in_place(nn->hidden_outputWeights, dhidden_outputWeights);
    matrix_add_in_place(nn->output_biases, gradient);
    printf("Infinitesimal values applied\n");

    // THEN, we need to do the same process as for hidden to output layer's weights

    Matrix *transposed_matrix = matrix_transpose(nn->hidden_outputWeights);
    // And then, computes the dot product of this transposed matrix with
    // the errors found for the output layer to find the hidden layer errors
    Matrix *hiddenLayer_error = matrix_dot_product(transposed_matrix, outputLayer_error);

    gradient = matrix_apply_function(hidden, dsigmoid);
    matrix_mul_in_place(gradient, hiddenLayer_error);
    matrix_scalar_mul_in_place(gradient, nn->lr);

    // Computes deltas of input to hidden layer's weights
    // Now, remember that the equivalent of variable "hidden" is "inputMatrix"
    // for the inputs layer
    Matrix *transposed_input = matrix_transpose(inputMatrix);
    Matrix *dinput_hiddenWeights = matrix_dot_product(gradient, transposed_input);

    // FINAL STEP
    // Adjusting weights and biases with infinitesimal values
    matrix_add_in_place(nn->input_hiddenWeights, dinput_hiddenWeights);
    matrix_add_in_place(nn->hidden_biases, gradient);


    // Now, we have to compute infinitesimal values
    // Formulas :
    // ? output = sigmoid(weight * input + bias)
    // ? output = mx + b
    // dB (delta bias) = lr * error
    // dM (delta m) = lr * error * x
    // dW_h->o (delta weight from hidden layer to output layer)
    //         = lr * output_error * dsigmoid(output) [DOT_PROD] transposed(hidden_layer)
    // dW_i->h (delta weight fron input layer to hidden layer)
    //         = lr * hidden_error * dsigmoid(hidden) [DOT_PROD] transposed(input_layer)
    //
    // Do not forget that this is BACKPROPAGATION



    printf("Outputs :\n");
    matrix_print(output);
    //separator();

    printf("Wanted :\n");
    matrix_print(wanted);
    //separator();

    printf("Errors :\n");
    matrix_print(outputLayer_error);
    //separator();


}

int main()
{
    NeuralNetwork *nn = malloc(sizeof(NeuralNetwork));
    size_t NumberOfOutputs = 1;
    init_network(nn, 2, 2, NumberOfOutputs, 0.1f);
    // Nodes needed to let XOR AI work
    // Input layer : 2 Nodes
    // Hidden layer(s) : 2 Nodes
    // Output layer : 1 Node

    /*
       double input[] = {1, 0};
       Matrix *m = feedforward_algo(nn, input, 2);
       for (size_t i = 0; i < m->dataLen; i++)
       {
       printf("%f ", m->data[i]);
       }
       printf("\n");
       */

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

    // Principal loop (Loop on data n times, with n = epoch)
    for (size_t b = 0; b < 100; b++)
    {
        for (size_t a = 0; a < 4; a++)
        {

            for (size_t i = 0; i < wantedOutputsMatrix->row; i++)
            {
                for (size_t j = 0; j < wantedOutputsMatrix->col; j++)
                {
                    double temp = wantedOutputs[a][i * wantedOutputsMatrix->col + j];
                    matrix_set(wantedOutputsMatrix, i, j, temp);
                }
            }

            //matrix_print(wantedOutputsMatrix);
            //array_print(inputs[a], 2);

            train(nn, inputs[a], 2, wantedOutputsMatrix);
            matrix_print(wantedOutputsMatrix);
            array_print(inputs[a], 2);
            separator();


        }
    }

    double testInput[] = { 0.0f, 0.0f };
    matrix_print(feedforward_algo(nn, testInput, 2));
    return 0;
}
