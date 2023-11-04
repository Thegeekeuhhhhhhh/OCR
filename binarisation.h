#ifndef BINARISATION_H
#define BINARISATION_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>

void merge(Uint8* arr, int l, int m, int r);
void mergeSort(Uint8* arr, int l, int r);
int median_value(struct my_image* image);
int weak_edge_handling(Uint8** pixels, int h, int w, int height, int width, int upper);
Uint8** binarisation(struct my_image* image);
// Function prototypes for binarization functions in binarisation.c
// Include any necessary data types and function prototypes

#endif

