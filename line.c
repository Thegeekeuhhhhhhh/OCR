#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gray_im.h"

struct l {
    double rho;
    double theta;
};

struct Line{
    struct l* line;
    int nblines;  
};

// Fonction de transformation de Hough
struct Line houghTransform(unsigned char *image, int width, int height) {
    // Initialiser l'espace Hough
    int max_rho = (int) sqrt(width * width + height * height);
    int theta_count = 180;
    int *accumulator = (int *)calloc(max_rho * theta_count, sizeof(int));

    // Boucle sur les points de l'image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (image[y * width + x] > 0) {
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
    findLocalMaxima(newLine->line, accumulator, max_rho, theta_count);

    // Libérer la mémoire
    free(accumulator);
}

// Fonction pour identifier les maxima locaux dans l'espace Hough
void findLocalMaxima(struct Line* newLine, int *accumulator, int max_rho, int theta_count) {
    int threshold = 100; // Choisir un seuil approprié
    for (int r = 0; r < max_rho; r++) {
        for (int t = 0; t < theta_count; t++) {
            int votes = accumulator[r * theta_count + t];
            if (votes > threshold) {
                // Vérifier si le point est un maximum local
                int is_max = 1;
                for (int dr = -1; dr <= 1; dr++) {
                    for (int dt = -1; dt <= 1; dt++) {
                        if ((dr != 0 || dt != 0) && r + dr >= 0 && r + dr < max_rho && t + dt >= 0 && t + dt < theta_count) {
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

int main() {
    // Charger l'image et la convertir en niveau de gris
    // ...

    // Appeler la fonction de transformation de Hough
    // houghTransform(image, width, height);

    return 0;
}

