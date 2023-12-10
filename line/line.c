#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gray_im.h"
#include "line.h"

#define THRESHOLD (M_PI / 90.0)

// Initialisation de la fenêtre SDL
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int SCREEN_WIDTH;
int SCREEN_HEIGHT;


void printLines(struct Line* lineStruct) {
    if (lineStruct == NULL || lineStruct->line == NULL || lineStruct->nblines <= 0) {
        printf("Invalid Line structure or empty lines.\n");
        return;
    }

    printf("Printing lines:\n");
    for (int i = 0; i < lineStruct->nblines; i++) {
        printf("Line %d: rho = %f, theta = %f\n", i + 1, lineStruct->line[i].rho, lineStruct->line[i].theta);
    }
}

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
    
    int threshold = width/2; // Choisir un seuil approprié
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
                    double theta = t * M_PI / 180.0;  
                    addLine(newLine, r, theta);
                    
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

void calculateIntersection(struct Line lines, struct Point* intersectionPoints) {
    int i, j;
    for (i = 0; i < lines.nblines; i++) {
        double rho1 = lines.line[i].rho;
        double theta1 = lines.line[i].theta;

        for (j = i + 1; j < lines.nblines; j++) {
            double rho2 = lines.line[j].rho;
            double theta2 = lines.line[j].theta;

            // Check if lines are nearly perpendicular
            if (fabs(fabs(theta1 - theta2) - M_PI / 2.0) < THRESHOLD) {
                // Calculate intersection point for nearly perpendicular lines
                intersectionPoints[i].x = rho1;
                intersectionPoints[i].y = rho2;
            }
        }
    }
}

void resizeImage(SDL_Surface* srcSurface, SDL_Surface* destSurface) {
    SDL_Rect destRect = {0, 0, 28, 28}; // Nouvelles dimensions 28x28

    SDL_BlitScaled(srcSurface, NULL, destSurface, &destRect);
}

void extractSudokuCells(SDL_Surface* surface, int x1, int y1, int x2, int y2) {
    int cellWidth = (x2 - x1) / 9;
    int cellHeight = (y2 - y1) / 9;
    int border = 15; // Taille de la bordure à supprimer (ajustez selon vos besoins)
    int nonBlackThreshold = (cellWidth - 2 * border) * (cellHeight - 2 * border) * 0.02; // Seuil de 10% de pixels non noirs

    for (int row = 0, y = y1; row < 9; row++, y += cellHeight) {
        for (int col = 0, x = x1; col < 9; col++, x += cellWidth) {
            SDL_Rect cellRect = {x + border, y + border, cellWidth - 2 * border, cellHeight - 2 * border};

            SDL_Surface* cellSurface = SDL_CreateRGBSurface(0, cellWidth - 2 * border, cellHeight - 2 * border, 32, 0, 0, 0, 0);
            SDL_BlitSurface(surface, &cellRect, cellSurface, NULL);

            int nonBlackPixels = 0;

            for (int i = 0; i < cellSurface->w; i++) {
                for (int j = 0; j < cellSurface->h; j++) {
                    Uint32 pixel = *((Uint32*)cellSurface->pixels + j * cellSurface->pitch / 4 + i);
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, cellSurface->format, &r, &g, &b);

                    // Vérifier si le pixel n'est pas presque noir
                    if (r > 10 || g > 10 || b > 10) {
                        nonBlackPixels++;
                    }
                }
            }

            // Enregistrer la cellule si elle a au moins le seuil de pixels non noirs
            if (nonBlackPixels >= nonBlackThreshold) {
                // Redimensionner la cellule en 28x28
                SDL_Surface* resizedSurface = SDL_CreateRGBSurface(0, 28, 28, 32, 0, 0, 0, 0);
                resizeImage(cellSurface, resizedSurface);

                char filename[50];
                sprintf(filename, "output/%d_%d.png", col, row);

                SDL_SaveBMP(resizedSurface, filename);

                SDL_FreeSurface(resizedSurface);
            }

            SDL_FreeSurface(cellSurface);
        }
    }
}

// Fonction pour afficher les lignes sur la surface SDL
void drawLines(struct my_image *img, struct Line* myLine) {
    for (int i = 0; i < myLine->nblines; i++) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Couleur red
        double rho = myLine->line[i].rho;
        double theta = myLine->line[i].theta;
        
        int width = img->w;
        int height = img->h;
        
        // Calculer les coordonnées de début de la ligne
        int x_start = (int)(rho * cos(theta));
        int y_start = (int)(rho * sin(theta));

        // Calculer les coordonnées de fin de la ligne
        int x_end, y_end;
        if (sin(theta) != 0) {
            // Si sin(theta) est différent de zéro
            x_end = width;
            y_end = y_start;
        } else {
            // Si sin(theta) est égal à zéro, gérer le cas spécifique
            x_end = x_start;
            y_end = height;
        }
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
        fprintf(stderr, "Usage: %s imagefile (optional param \"-cut\")\n", argv[0]);
        exit(1);
    }

    SDL_Surface *surf = load_image(argv[1]);
    SDL_Surface *surf2 = load_image(argv[2]);
    struct my_image* img = init_image(surf);

    // Créer une instance de Line et ajouter des lignes (à titre d'exemple)
    struct Line myLine = houghTransform(img->pixels, img->w, img->h);

    printLines(&myLine);

    struct Point* intersectionPoints = (struct Point*)malloc(myLine.nblines * (myLine.nblines - 1) / 2 * sizeof(struct Point));

    calculateIntersection(myLine, intersectionPoints);

        // Using the calculated intersection points
    for (int i = 0; i < myLine.nblines * (myLine.nblines - 1) / 2; i++) {
        printf("Intersection %d: (%d, %d)\n", i + 1, intersectionPoints[i].x, intersectionPoints[i].y);
    }

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
    drawLines(img,&myLine);
    
    printf("%i\n", myLine.nblines);

    int x1 = myLine.line[0].rho;
    int y1 = myLine.line[1].rho;
    int x2 = myLine.line[myLine.nblines-2].rho;
    int y2 = myLine.line[myLine.nblines-1].rho;

    extractSudokuCells(surf2, x1, y1, x2, y2);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE); // Vert

    for (int i = 0; i < myLine.nblines * (myLine.nblines - 1) / 2; i++) {
    int x = intersectionPoints[i].x;
    int y = intersectionPoints[i].y;

        for (int offsetX = -1; offsetX <= 1; offsetX++) {
            for (int offsetY = -1; offsetY <= 1; offsetY++) {
                SDL_RenderDrawPoint(renderer, x + offsetX, y + offsetY);
            }
        }
    }

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
    free(intersectionPoints);
    SDL_FreeSurface(surf);
    SDL_FreeSurface(surf2);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

