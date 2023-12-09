#include "neuralnetwork.h"
#include <err.h>
#include "train.h"
#include "matrix.h"
#include "nn.h"
#include <unistd.h>
#include <SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int convert(char *str)
{
    int res = 0;
    while (*str != '\0')
    {
        if (*str >= '0' && *str <= '9')
        {
            res = res*10 + (*str - '0');
        }
        else
        {
            return -1;
        }
        str++;
    }
    return res;
}

/*
int guess(char *database, char *filename)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        errx(1, "Erreur d'init SDL");
        return -1;
    }

    struct NeuralNetwork *nn = load_network(database);
    if (nn == NULL)
    {
        errx(1, "Failed to open %s", database);
    }

    double inputs[784];
    SDL_Surface* tmpSurface = IMG_Load(filename);
    if (tmpSurface == NULL)
    {
        errx(1, "Can not open %s", filename);
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
            inputs[f] = 1;
        }
        else
        {
            inputs[f] = 0;
        }
    }
    SDL_FreeSurface(surface);
    Matrix *wantedOutputsMatrix = malloc(sizeof(Matrix));
    matrix_init(wantedOutputsMatrix, 10, 1);
    digit_print(inputs);
    Matrix *test1 = feedforward_algo(nn, inputs, 784);
    matrix_print(test1);

    int maxi = matrix_max_index(test1);

    printf("Result : %li\n", matrix_max_index(test1));
    matrix_free(wantedOutputsMatrix);

    SDL_Quit();
    free_network(nn);
    matrix_free(test1);
    return maxi;
}
*/

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        errx(1,
                "\nUsage :\n./neuralnetwork [train / test] [args]\n"
                "train : [Number of EPOCHS] [learning rate]\n"
                "test : [Number of random tests OR name of an image file]");
    }

    srand(time(NULL));
    argv++;
    if (strcmp(*argv, "train") == 0)
    {
        if (argc != 4)
        {
            errx(1, "Train must be used as following\n"
                    "./train [Number of EPOCHS] [learning rate]");
        }

        argv++;
        size_t EPOCHS = 0;
        double learning_rate = 1.0f;
        char *temp = *argv; 
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
        argv++;
        learning_rate = strtod(*argv, NULL);
        train(EPOCHS, learning_rate);
        return 0;
    }

    if (strcmp(*argv, "test") == 0)
    {
        if (argc != 3)
        {
            errx(1, "Test must be used as following\n"
                    "./test [Number of random tests OR name of an image file]");
        }

        argv++;
        char *filename = *argv;
        if (*filename == '0')
        {
            errx(1, "Not funny -_-");
        }

        int number = convert(filename);
        if (number != -1)
        {
            struct NeuralNetwork *nn = load_network("Values.txt");
            if (nn == NULL)
            {
                errx(1, "Failed to open Values.txt");
            }

            if (SDL_Init(SDL_INIT_VIDEO) < 0)
            {
                errx(1, "Erreur d'init SDL");
                return -1;
            }

            if (number > 1200)
            {
                errx(1, "Number is too large");
            }
            printf("----------------------------------------------------\n");
            double inputs[number][784];
            size_t simpleOutputs[number];
            for (int index = 1; index <= number; index++)
            {
                int picIndex = (rand() % 300) + 1;
                int randindex = (rand() % 9) + 1;
                char str[60];
                sprintf(str, "digitalwritten_digits/%i/%i.jpg", randindex, picIndex);
                SDL_Surface* tmpSurface = IMG_Load(str);
                if (tmpSurface == NULL)
                {
                    errx(1, "Skill issue ca ouvre pas SDL");
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
                        inputs[index-1][f] = 1;
                    }
                    else
                    {
                        inputs[index-1][f] = 0;
                    }
                }
                simpleOutputs[index-1] = randindex;
                SDL_FreeSurface(surface);
                surface = NULL;
            }

            // Converts wantedOutputs into a Matrix
            Matrix *wantedOutputsMatrix = malloc(sizeof(Matrix));
            matrix_init(wantedOutputsMatrix, 10, 1);

            size_t trainingIndex[number];
            for (int i = 0; i < number; i++) { trainingIndex[i] = i; }
            shuffle(trainingIndex, number); 

            int success = 0;
            for (int r = 0; r < number; r++)
            {
                digit_print(inputs[r]);
                Matrix *test1 = feedforward_algo(nn, inputs[r], 784);
                matrix_print(test1);
                size_t ind_test = matrix_max_index(test1);
                printf("Result : %li, Expected : %li\n", ind_test, simpleOutputs[r]);
                if (ind_test == simpleOutputs[r])
                {
                    success += 1;
                }
                matrix_free(test1);
                sleep(1);
            }
            printf("ACCURACY : %f%%\n", ((double)success / (double)number) * 100.0f);
            matrix_free(wantedOutputsMatrix);
            free_network(nn);
            SDL_Quit();
        }
        else
        {
            guess("1K_Training", *argv);
        }

        return 0;
    }


    errx(1, "The first argument must be 'train' or 'test'");
    return 1;
}


