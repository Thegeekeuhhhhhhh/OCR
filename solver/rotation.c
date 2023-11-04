#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <string.h>




int main(int argc, char** argv)
{
    //initialisation SDL et cas d'erreur;
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: image-file angle");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow("Display", 0, 0, 
        640, 400, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Texture* texture = IMG_LoadTexture(renderer, argv[1]);
    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int w, h;
    if (SDL_QueryTexture(texture, NULL, NULL, &w, &h) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_SetWindowSize(window, w, h);
    //fin init :
    // Image set up et affiché

    //fait la rotation
    SDL_Event event;
    int test_rota = 0;
    do
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            break;
        case SDL_WINDOWEVENT:
            //test changement d'image;
            if (event.window.event == SDL_WINDOWEVENT_RESIZED && !test_rota)
            {
                //rotation actuel;
                int rota = SDL_RenderCopyEx(renderer, texture, NULL,
                    NULL, -atoi(argv[2]), NULL, SDL_FLIP_NONE);
                if (rota)
                {
                    errx(EXIT_FAILURE, "%s", SDL_GetError());
                }
                SDL_RenderPresent(renderer);
                test_rota = 1;
            }
            break;
        }
    }while(1)

    //save l'image dans un new fichier;
    SDL_SetRenderTarget(renderer, texture);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, w, 
        h, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, 
        surface->pixels, surface->pitch);
    char* outName = argv[1];
    outName = strcat("result_", outName);
    IMG_SavePNG(surface, outName);
    SDL_FreeSurface(surface);
    SDL_Texture* target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, target);

    //Dégage le SDL
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}