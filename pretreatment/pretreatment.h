#ifndef PRETREATMENT
#define PRETREATMENT

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>
#include <math.h>

#include "pretreatment.c"

void convertToGrayscale(SDL_Surface *inputSurface, SDL_Surface *outputSurface);
void dilateImage(SDL_Surface *inputSurface, SDL_Surface *outputSurface, int dilationSize);
void applySauvolaFilter(SDL_Surface *inputSurface, SDL_Surface *outputSurface, int windowSize, double k);
void applySobelFilter(SDL_Surface *inputSurface, SDL_Surface *outputSurface);
void reverseColors(SDL_Surface* surface);

#endif
