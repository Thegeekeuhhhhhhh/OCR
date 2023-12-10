#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#include "gray_im.h"

// Function to convert an image to grayscale
void convertToGrayscale(SDL_Surface *inputSurface, SDL_Surface *outputSurface) {
    // Loop through each pixel in the image
    for (int y = 0; y < inputSurface->h; y++) {
        for (int x = 0; x < inputSurface->w; x++) {
            // Get the pixel at the current position
            Uint8 *inputPixel = (Uint8 *)inputSurface->pixels + (y * inputSurface->pitch) + (x * sizeof(Uint32));

            // Calculate grayscale intensity using the luminosity method
            Uint8 intensity = (Uint8)(0.299 * inputPixel[2] + 0.587 * inputPixel[1] + 0.114 * inputPixel[0]);

            // Set the pixel in the output surface
            Uint8 *outputPixel = (Uint8 *)outputSurface->pixels + (y * outputSurface->pitch) + (x * sizeof(Uint32));
            outputPixel[0] = outputPixel[1] = outputPixel[2] = intensity;
            outputPixel[3] = 255; // Alpha channel (fully opaque)
        }
    }
}


// Function to perform image dilation
void dilateImage(SDL_Surface *inputSurface, SDL_Surface *outputSurface, int dilationSize) {
    // Loop through each pixel in the image
    for (int y = 0; y < inputSurface->h; y++) {
        for (int x = 0; x < inputSurface->w; x++) {
            // Get the pixel at the current position
            Uint8 *inputPixel = (Uint8 *)inputSurface->pixels + (y * inputSurface->pitch) + (x * sizeof(Uint32));

            // Check if the pixel is white (255)
            if (inputPixel[0] == 255) {
                // Perform dilation on neighboring pixels within the specified radius
                for (int dy = -dilationSize; dy <= dilationSize; dy++) {
                    for (int dx = -dilationSize; dx <= dilationSize; dx++) {
                        int newX = x + dx;
                        int newY = y + dy;

                        // Check if the new coordinates are within the image boundaries
                        if (newX >= 0 && newX < inputSurface->w && newY >= 0 && newY < inputSurface->h) {
                            // Set the pixel in the output surface to white (255)
                            Uint8 *outputPixel = (Uint8 *)outputSurface->pixels + (newY * outputSurface->pitch) + (newX * sizeof(Uint32));
                            outputPixel[0] = outputPixel[1] = outputPixel[2] = 255;
                            outputPixel[3] = 255; // Alpha channel (fully opaque)
                        }
                    }
                }
            }
        }
    }
}


// Function to apply Sauvola filter
void applySauvolaFilter(SDL_Surface *inputSurface, SDL_Surface *outputSurface, int windowSize, double k) {
    // Loop through each pixel in the image
    for (int y = 0; y < inputSurface->h; y++) {
        for (int x = 0; x < inputSurface->w; x++) {
            // Calculate the local mean intensity and standard deviation in the specified window
            double sumIntensity = 0.0;
            double sumSquaredIntensity = 0.0;
            int pixelCount = 0;

            for (int dy = -windowSize / 2; dy <= windowSize / 2; dy++) {
                for (int dx = -windowSize / 2; dx <= windowSize / 2; dx++) {
                    int newX = x + dx;
                    int newY = y + dy;

                    // Check if the new coordinates are within the image boundaries
                    if (newX >= 0 && newX < inputSurface->w && newY >= 0 && newY < inputSurface->h) {
                        // Get the pixel at the current position
                        Uint8 *pixel = (Uint8 *)inputSurface->pixels + (newY * inputSurface->pitch) + (newX * sizeof(Uint32));
                        int intensity = pixel[0];

                        // Update the sum and sum of squared intensities
                        sumIntensity += intensity;
                        sumSquaredIntensity += intensity * intensity;
                        pixelCount++;
                    }
                }
            }

            // Calculate the mean and standard deviation
            double meanIntensity = sumIntensity / pixelCount;
            double stdDevIntensity = sqrt(sumSquaredIntensity / pixelCount - meanIntensity * meanIntensity);

            // Calculate the threshold using the Sauvola formula
            Uint8 *inputPixel = (Uint8 *)inputSurface->pixels + (y * inputSurface->pitch) + (x * sizeof(Uint32));
            double threshold = meanIntensity * (1.0 + k * ((stdDevIntensity / 128.0) - 1.0));

            // Apply binary thresholding
            Uint8 intensity = (inputPixel[0] > threshold) ? 255 : 0;

            // Set the pixel in the output surface
            Uint8 *outputPixel = (Uint8 *)outputSurface->pixels + (y * outputSurface->pitch) + (x * sizeof(Uint32));
            outputPixel[0] = outputPixel[1] = outputPixel[2] = intensity;
            outputPixel[3] = 255; // Alpha channel (fully opaque)
        }
    }
}


void applySobelFilter(SDL_Surface *inputSurface, SDL_Surface *outputSurface) {
    // Sobel kernels for x and y directions
    int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Loop through each pixel in the image (excluding border pixels)
    for (int y = 0; y < inputSurface->h; y++) {
        for (int x = 0; x < inputSurface->w; x++) {
            
            if(y == 0 || x == 0 || y == inputSurface->h-1 || x == inputSurface->w-1){
                Uint8 *outputPixel = (Uint8 *)outputSurface->pixels + (y * outputSurface->pitch) + (x * sizeof(Uint32));
                outputPixel[0] = outputPixel[1] = outputPixel[2] = 0;
                continue;
            }
            // Calculate Sobel gradients for both x and y directions
            int gradientX = 0;
            int gradientY = 0;

            for (int ky = 0; ky < 3; ky++) {
                for (int kx = 0; kx < 3; kx++) {
                    Uint8 *pixel = (Uint8 *)inputSurface->pixels + ((y + ky - 1) * inputSurface->pitch) + ((x + kx - 1) * sizeof(Uint32));
                    Uint8 intensity = (Uint8)(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);

                    gradientX += intensity * sobelX[ky][kx];
                    gradientY += intensity * sobelY[ky][kx];
                }
            }

            // Calculate magnitude of the gradient
            int magnitude = (int)sqrt(gradientX * gradientX + gradientY * gradientY);

            // Ensure the magnitude is within the valid range [0, 255]
            magnitude = (magnitude > 255) ? 255 : (magnitude < 0) ? 0 : magnitude;

            // Set the pixel in the output surface
            Uint8 *outputPixel = (Uint8 *)outputSurface->pixels + (y * outputSurface->pitch) + (x * sizeof(Uint32));
            outputPixel[0] = outputPixel[1] = outputPixel[2] = magnitude;
            outputPixel[3] = 255; // Alpha channel (fully opaque)
        }
    }
}

void reverseColors(SDL_Surface* surface) {
    if (surface->format->BytesPerPixel == 4) { // Ensure it's a 32-bit image (4 bytes per pixel)
        Uint32* pixels = (Uint32*)surface->pixels;
        int pixelCount = surface->w * surface->h;

        for (int i = 0; i < pixelCount; ++i) {
            Uint32 pixel = pixels[i];

            // Extract individual color components
            Uint8 red = pixel & 0xFF;
            Uint8 green = (pixel >> 8) & 0xFF;
            Uint8 blue = (pixel >> 16) & 0xFF;
            Uint8 alpha = (pixel >> 24) & 0xFF;

            // Reverse colors
            red = 255 - red;
            green = 255 - green;
            blue = 255 - blue;

            // Recombine color components
            pixel = (alpha << 24) | (blue << 16) | (green << 8) | red;

            pixels[i] = pixel;
        }
    }
}

/*
int main(int argc, char** argv){
    if(argc != 5){
        err(1, "not good my friend");
    }

    SDL_Surface* one = load_image(argv[1]);
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
*/

