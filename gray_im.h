#ifndef HEADER_FILE
#define HEADER_FILE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>

struct my_image;
SDL_Surface* load_image(const char* path);
Uint8** init_pixels_mat(int h, int w);
void free_pixels(Uint8** pixels, int h);
Uint8** init_pixels(Uint32* sur_pixels, int w, int h);
struct my_image* init_image(SDL_Surface* surface);
void update_surface(SDL_Surface* surface, struct my_image* image);

#endif
