#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>
#include <math.h>

#include "gray_im.h"
#include "gaussian_blur.h"

short KERNEL_SIZE;
float PI = 3.14159;


float** init_kernel(double sigma, int size){
    KERNEL_SIZE = size;
    double sum = 0;
    int offset = KERNEL_SIZE/2;
    float** kernel = (float**)malloc(KERNEL_SIZE * sizeof(float*));
    for(int y = 0; y < KERNEL_SIZE; y++){
        int y_offset = y - offset;
        kernel[y] = (float*)malloc(KERNEL_SIZE * sizeof(float));
        for(int x = 0; x < KERNEL_SIZE; x++){
            int x_offset = x - offset;
            float value = (1/(2*PI*sigma*sigma))*expf(-(float)(x_offset*x_offset + y_offset*y_offset)/(2*sigma*sigma));
            // printf("here value = %f \n", value);
            kernel[y][x] = value;
            sum += value;
        }
    }
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            kernel[y][x] /= sum;
        }
    }
   
    return kernel;
}


void free_kernel(float** kernel){
    for(int i = 0; i < KERNEL_SIZE; i++){
        free(kernel[i]);
    }
    free(kernel);
}


Uint8** apply_convolution(struct my_image* image, int size, float sigma){
    float** kernel = init_kernel(sigma, size);
    Uint8** pixels = image->pixels;
    int height = image->h;
    int width = image->w;
    Uint8** res_pix = init_pixels_mat(height, width);

    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){
            float sum = 0;            
            int offset = KERNEL_SIZE / 2 + 1;

            for(int y = -offset + 1; y < offset - 1; y++){
                for(int x = -offset + 1; x < offset - 1; x++){
                    if((h+y >= 0 && h + y < height) && (w + x >= 0 && w + x < width)){
                        sum += pixels[h + y][w + x] * kernel[offset + y][offset + x];
                    }
                }
            }
            res_pix[h][w] = sum;
        }
    }
    free_kernel(kernel);
    return res_pix;
}

/*
int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}


Uint8** apply_convolution(struct my_image* image, float** kernel) {
    int height = image->h;
    int width = image->w;
    int kernelSize = KERNEL_SIZE;  // Adjust this as needed based on your kernel size

    // Create a result array to store the filtered image
    Uint8** result = init_pixels_mat(height, width);

    int halfSize = kernelSize / 2;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sum = 0.0;

            for (int ky = -halfSize; ky <= halfSize; ky++) {
                for (int kx = -halfSize; kx <= halfSize; kx++) {
                    int pixelY = clamp(y + ky, 0, height - 1);
                    int pixelX = clamp(x + kx, 0, width - 1);
                    Uint8 pixelValue = image->pixels[pixelY][pixelX];
                    double weight = kernel[ky + halfSize][kx + halfSize];
                    sum += pixelValue * weight;
                }
            }

            result[y][x] = (Uint8)round(sum);
        }
    }

    return result;
}
*/
/*
int main(int argc, char** argv){
    float sum = 0;
    float** kernel = init_kernel(2.5,7);
    for(int i = 0; i <KERNEL_SIZE; i++){
        for(int j = 0; j < KERNEL_SIZE; j++){
            printf("%7.4f", kernel[i][j]);
            sum += kernel[i][j];
        }
        printf("\n");
    }
    printf("sum = %f\n", sum);
    int i = 7/2;
    printf("%d\n",i);
   
    SDL_Surface* surface = load_image(argv[1]);
    struct my_image* image = init_image(surface);

    Uint8** blured = apply_convolution(image, 7, 2.5);
printf("fw\n"); 
    image-> pixels = blured;
    update_surface(surface, image);

    SDL_SaveBMP(surface, "test_blur.bmp");
    SDL_FreeSurface(surface);
    free_pixels(image->pixels, image->h);
    //free_kernel(kernel);


    return 0;
}*/
