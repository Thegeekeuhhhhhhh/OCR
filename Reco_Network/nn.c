#include "neuralnetwork.h"
#include <err.h>
#include "train.h"
#include "matrix.h"
#include <unistd.h>
#include <SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
