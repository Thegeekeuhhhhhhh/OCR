#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>

#include "gray_im.h"

struct my_image {
    Uint8** pixels;  // 2d array of pixels[h][w] as grayscales values
    int h;           // height
    int w;           // width
    SDL_PixelFormat* format; 
};


SDL_Surface* load_image(const char* path){
        // loads an image in a surface
    SDL_Surface* tmpSurface = IMG_Load(path);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmpSurface, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(tmpSurface);
    return surface;
}


Uint8** init_pixels_mat(int h, int w){
    Uint8** pixels = malloc(h * sizeof(Uint8*));
    for(int i = 0; i < h; i++){
        pixels[i] = calloc(w, sizeof(Uint8));
    }
    return pixels;
}


void free_pixels(Uint8** pixels, int h){
    for(int i = 0; i < h; i++){
        free(pixels[i]);
    }
    free(pixels);
}


Uint8** init_pixels(Uint32* sur_pixels, int w, int h){
        // function that inits the pixels array of an image
        // from the pixels of a surface
    Uint8** pixels = init_pixels_mat(h, w);
    Uint8 r, g, b;
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            r = (sur_pixels[i*w+j] & 0x00ff0000) >> 16;
            g = (sur_pixels[i*w+j] & 0x0000ff00) >> 8;
            b = sur_pixels[i*w+j] & 0x000000ff;
            Uint8 grayscale = r*0.299 + g*0.587 + b*0.114;
            pixels[i][j] = grayscale;
        }
    }
    return pixels;
}


struct my_image* init_image(SDL_Surface* surface){
        // function that initialize an image from a surface
    Uint8** pixels = init_pixels(surface->pixels, surface->w, surface->h);
    int h = surface->h;
    int w = surface->w;
    SDL_PixelFormat* format = surface->format; 

    struct my_image* image = malloc(sizeof(struct my_image));
    image->pixels = pixels;
    image->h = h;
    image->w = w;
    image->format = format;
    return image;
}

void update_surface(SDL_Surface* surface, struct my_image* image);

/*
int main(int argc, char** argv){
    if(argc != 2)
           errx(EXIT_FAILURE, "prout"); 
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
       errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    
    SDL_Surface* surface = load_image(argv[1]);
    struct my_image* image = init_image(surface);
    Uint8** pixels = image->pixels;

    for(int i = 0; i < 10; i++){    
        printf("%5hhu", pixels[0][i]);
        printf("\n");
    }
    free_pixels(pixels, image->h);
    SDL_FreeSurface(surface);
    return 0;
}*/
