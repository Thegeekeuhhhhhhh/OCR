#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gray_im.h"
#include "line.h"

// Initialisation de la fenêtre SDL
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int SCREEN_WIDTH;
int SCREEN_HEIGHT;


// Fonction de transformation de Hough
struct Line houghTransform(Uint8** pixels, int width, int height) {
    // Initialiser l'espace Hough
    int max_rho = (int) sqrt(width * width + height * height);
    int theta_count = 180;
    int *accumulator = (int *)calloc(max_rho * theta_count, sizeof(int));

    // Boucle sur les points de l'image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (pixels[x][y] > 0) {
                // Boucle sur les valeurs de theta
                for (int t = 0; t < theta_count; t++) {
                    double theta = t * M_PI / 180.0;
                    int rho = (int) (x * cos(theta) + y * sin(theta));
                    if (rho >= 0 && rho < max_rho) {
                        accumulator[rho * theta_count + t] += 1;
                    }
                }
            }
        }
    }
    
    struct Line* newLine = (struct Line*)malloc(sizeof(struct Line));
    newLine->nblines = 0;
    newLine->line = NULL;

    // Identifier les maxima locaux dans l'espace Hough pour détecter les lignes
    findLocalMaxima(newLine, accumulator, max_rho, theta_count, width);

    // Libérer la mémoire
    free(accumulator);
    return *newLine;
}

// Fonction pour identifier les maxima locaux dans l'espace Hough
void findLocalMaxima(struct Line* newLine, int *accumulator
                , int max_rho, int theta_count, int width) {
    
    int threshold = 1/2*width; // Choisir un seuil approprié
    for (int r = 0; r < max_rho; r++) {
        for (int t = 0; t < theta_count; t++) {
            int votes = accumulator[r * theta_count + t];
            if (votes > threshold) {
                // Vérifier si le point est un maximum local
                int is_max = 1;
                for (int dr = -1; dr <= 1; dr++) {
                    for (int dt = -1; dt <= 1; dt++) {
                        if ((dr != 0 || dt != 0) 
                                && r + dr >= 0 
                                && r + dr < max_rho 
                                && t + dt < theta_count
                                && ((r + dr) * theta_count + (t + dt)) < (max_rho * theta_count)) {

                            if (accumulator[(r + dr) * theta_count + (t + dt)] >= votes) {
                                is_max = 0;
                                break;
                            }
                        }
                    }
                    if (!is_max) {
                        break;
                    }
                }
                if (is_max) {
                    addLine(newLine, r, t);
                    
                }
            }
        }
    }
}

void addLine(struct Line* myLine, double rho, double theta) {
    myLine->nblines++;
    myLine->line = (struct l*)realloc(myLine->line, myLine->nblines * sizeof(struct l));
    if (myLine->line == NULL) {
        fprintf(stderr, "Memory reallocation failed for struct l.\n");
        exit(EXIT_FAILURE);
    }

    myLine->line[myLine->nblines - 1].rho = rho;
    myLine->line[myLine->nblines - 1].theta = theta;
}

/* // TODO
void cropImage(SDL_Surface* image, struct l* lines, int numLines) {
    for (int i = 0; i < numLines; i++) {
        for (int j = i + 1; j < numLines; j++) {
            // Vérifier si les lignes se croisent 
            // ou sont suffisamment proches pour être 
            // considérées comme intersectantes
            if (fabs(lines[i].theta - lines[j].theta) > 0.1) {
                // Calculer les coordonnées de l'intersection des lignes
                int x_intersect = (int)((lines[j].rho * sin(lines[i].theta)
                            - lines[i].rho * sin(lines[j].theta)) 
                            / sin(lines[j].theta - lines[i].theta));
                int y_intersect = (int)((lines[i].rho * cos(lines[j].theta)
                            - lines[j].rho * cos(lines[i].theta)) 
                            / sin(lines[i].theta - lines[j].theta));

                // Déterminer les valeurs de width and height
                // TODO


                // Utiliser ces coordonnées pour définir les limites de découpe et découper l'image
                SDL_Surface* croppedImage = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
                SDL_Rect rect;
                rect.x = 0; // Mettez les coordonnées appropriées ici
                rect.y = 0; // Mettez les coordonnées appropriées ici
                rect.w = width; // Mettez la largeur appropriée ici
                rect.h = height; // Mettez la hauteur appropriée ici
                SDL_BlitSurface(image, &rect, croppedImage, NULL);

                // Enregistrez l'image découpée
                // TODO
            }
        }
    }
}
*/

// Fonction pour afficher les lignes sur la surface SDL
void drawLines(struct Line* myLine) {
    for (int i = 0; i < myLine->nblines; i++) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Couleur red
        double rho = myLine->line[i].rho;
        double theta = myLine->line[i].theta;
        
        int lineLength = 300; // Longueur de la ligne à afficher

        // Calculer les coordonnées de début de la ligne
        int x_start = (int)(rho * cos(theta));
        int y_start = (int)(rho * sin(theta));

        // Calculer les coordonnées de fin de la ligne
        int x_end = (int)(x_start + lineLength * cos(theta));
        int y_end = (int)(y_start + lineLength * sin(theta));

        // Dessiner la ligne
        SDL_RenderDrawLine(renderer, x_start, y_start, x_end, y_end);
    }
}

// Fonction pour initialiser SDL
int initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    window = SDL_CreateWindow("Line Detection", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    return 1;
}


int main(int argc, char** argv) {

    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s imagefile (optional param)\n", argv[0]);
        exit(1);
    }

    SDL_Surface *surf = load_image(argv[1]);
    struct my_image* img = init_image(surf);

    // Créer une instance de Line et ajouter des lignes (à titre d'exemple)
    struct Line myLine = houghTransform(img->pixels, img->w, img->h);
    
    // Fixe les tailles de la fenêtre 
    SCREEN_WIDTH = surf->w;
    SCREEN_HEIGHT = surf->h;

    // Initialiser SDL
    if (!initSDL()) {
        return -1;
    }
    

    // Dessiner les lignes sur la surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
    
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    drawLines(&myLine);
    
    printf("%i\n", myLine.nblines);

    SDL_RenderPresent(renderer);

    // Boucle d'événements principale
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }
    
    // Nettoyage et fermeture de SDL
    free(myLine.line);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

