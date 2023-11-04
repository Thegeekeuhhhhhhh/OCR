#ifndef GRAY_IM_HEADER_FILE
#define GRAY_IM_HEADER_FILE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>

struct my_image {
    Uint8** pixels;  // 2d array of pixels[h][w] as grayscales values
    int h;           // height
    int w;           // width
    SDL_PixelFormat* format;
};

SDL_Surface* load_image(const char* path);
Uint8** init_pixels_mat(int h, int w);
void free_pixels(Uint8** pixels, int h);
Uint8** init_pixels(Uint32* sur_pixels, int w, int h);
struct my_image* init_image(SDL_Surface* surface);
void update_surface(SDL_Surface* surface, struct my_image* image);

#endif
