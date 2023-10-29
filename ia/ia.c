#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define INPUT_SIZE 256
#define HIDDEN_SIZE 64
#define OUTPUT_SIZE 9

// Fonctions d'initialisation du réseau de neurones, propagation avant, rétropropagation, etc.
// ...

int main(int argc, char *argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Échec de l'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Charger l'image à partir du fichier (vous devez remplacer "image.bmp" par votre propre chemin d'accès)
    SDL_Surface *imageSurface = SDL_LoadBMP("image.bmp");
    if (imageSurface == NULL) {
        printf("Impossible de charger l'image : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Créer la fenêtre SDL
    SDL_Window *window = SDL_CreateWindow("Exemple de réseau de neurones", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    if (window == NULL) {
        printf("Échec de la création de la fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Créer un rendu à partir de la fenêtre
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Échec de la création du rendu : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *imageTexture;
    // Boucle principale de l'application SDL
    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Dessiner l'image sur l'écran
        imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_RenderCopy(renderer, imageTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Libérer la mémoire et quitter
    SDL_DestroyTexture(imageTexture);
    SDL_FreeSurface(imageSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
