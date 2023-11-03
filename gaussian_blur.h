#ifndef HEADER_FILE
#define HEADER_FILE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>
#include <math.h>


float** init_kernel(double sigma);
void free_kernel(float** kernel);
Uint8** apply_convolution(struct my_image* image, int size, float sigma);


#endif
