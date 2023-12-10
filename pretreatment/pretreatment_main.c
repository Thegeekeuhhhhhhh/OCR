#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>
#include <math.h>
#include "pretreatment.h"

int main(int argc, char** argv){
    if(argc != 5){
        err(1, "not good my friend");
    }

    SDL_Surface* one = load_image(argv[1]);
    SDL_SaveBMP(one, "no_modif.bmp");
    SDL_Surface* two = SDL_CreateRGBSurfaceWithFormat(0, one->w, one->h, 32, SDL_PIXELFORMAT_RGBA32);

    convertToGrayscale(one,two);
    dilateImage(two, one, atof(argv[2]));
    applySauvolaFilter(one,two, atoi(argv[3]), atof(argv[4]));

    applySobelFilter(two,one);
    SDL_SaveBMP(one, "sob.bmp");
    SDL_SaveBMP(two, "sauv.bmp");

    SDL_FreeSurface(one);
    SDL_FreeSurface(two);

    return 0;
}
