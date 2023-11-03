#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>
#include <math.h>

#include "gray_im.h"
#include "gaussian_blur.h"

short KERNEL_SIZE;
float PI = 3.14159;


float** init_kernel(double sigma){
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
    KERNEL_SIZE = size;
    float** kernel = init_kernel(sigma);
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
int main(int argc, char** argv){
    KERNEL_SIZE = 7;
    float sum = 0;
    float** kernel = init_kernel(0.84089642);
    for(int i = 0; i <KERNEL_SIZE; i++){
        for(int j = 0; j < KERNEL_SIZE; j++){
            printf("%7.4f", kernel[i][j]);
            sum += kernel[i][j];
        }
        printf("\n");
    }
    printf("sum = %f\n", sum);
    free_kernel(kernel);
    int i = 7/2;
    printf("%d\n",i);
   
    SDL_Surface* surface = load_image(argv[1]);
    struct my_image* image = init_image(surface);
    Uint8** blured = apply_convolution(image, 7, 2.5);
    
    image-> pixels = blured;
    update_surface(surface, image);

    SDL_SaveBMP(surface, "test_blur.bmp");
    SDL_FreeSurface(surface);
    free_pixels(image->pixels, image->h);
    //free_kernel(kernel);


    return 0;
}*/
