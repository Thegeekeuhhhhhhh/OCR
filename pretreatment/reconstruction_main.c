#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>
#include <math.h>
#include "reconstruction.h"

int main(int argc, char** argv)
{
    reco(argv[1],argv[2]);
    return 0;
}
