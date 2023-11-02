#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gray_im.h"

struct Line {
    double rho;
    double theta;
};


// Fonction de transformation de Hough
struct houghTransform(unsigned char *image, int width, int height) {
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

    // Identifier les maxima locaux dans l'espace Hough pour détecter les lignes
    // ...

    // Libérer la mémoire
    free(accumulator);
}

void cropImage(SDL_Surface* image, struct Line* lines, int numLines) {
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

                // Utiliser ces coordonnées pour définir les limites de découpe et découper l'image
                SDL_Surface* croppedImage = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
                SDL_Rect rect;
                rect.x = 0; // Mettez les coordonnées appropriées ici
                rect.y = 0; // Mettez les coordonnées appropriées ici
                rect.w = width; // Mettez la largeur appropriée ici
                rect.h = height; // Mettez la hauteur appropriée ici
                SDL_BlitSurface(image, &rect, croppedImage, NULL);

                // Enregistrez l'image découpée dans un fichier ou traitez-la comme nécessaire
                // ...
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

