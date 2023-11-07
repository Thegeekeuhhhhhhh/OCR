#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "gray_im.h"
#include "gaussian_blur.h"
#include "binarisation.h"


int median_value(struct my_image* image){
    Uint8** pixels = image->pixels;
    int height = image->h;
    int width = image->w;
    //printf("h = %d\nw = %d\n", height, width);
    int list2[256] = {0};
    int index = 0;
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            list2[pixels[j][i]] += 1;
            index++;
        }
    }
    
    int mid = height*width/2;
    int i = 0;
    while(mid != 0){
        if(list2[i] == 0){
            i++;
        }else{
            list2[i]--;
            mid--;
        }
    }
    int median = i;
    return (int)median;
}


int weak_edge_handling(Uint8** pixels, int h, int w, int height, int width, int lower){
    for(int j = -1; j < 2; j++){
        for(int i = -1; i < 2; i++){
            if(h + j >= 0 && h + j < height && w + i >= 0 && w + i < width){
                if(pixels[h + j][w + i] <= lower){
                    return 255;
                }
            }
        }
    }
    return 0;
}


Uint8** binarisation(struct my_image* image){
    int median = median_value(image);
    int upper = 1.15*median; // 1.33
    if(upper > 255){
        upper = 255;
    }
    int lower = 0.66*median;
    int height = image->h;
    int width =  image->w;

    Uint8** pixels = image->pixels;
    Uint8** res_pix = init_pixels_mat(height, width);

    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){
            
            if(pixels[h][w] > upper){
                res_pix[h][w] = 255;
            }
            else if (pixels[h][w] < lower){
                res_pix[h][w] = 0;
            }
            else{
                res_pix[h][w] = weak_edge_handling(res_pix, h, w, height, width, lower);
            }
        }
    }
    //struct my_image res = {res_pix, height, width, image->format};
    return res_pix;
}


int main(int argc, char** argv){
    if(argc != 4){
        errx(1, "incorect arguments");
    }
    SDL_Surface* surface = load_image(argv[1]);
    struct my_image* image = init_image(surface);
    //float** kernel = init_kernel(2.5, 7);

    int height = image->h;
    int size = atoll(argv[2]);
    float sigma = atof(argv[3]);

    Uint8** blured = apply_convolution(image, size, sigma);//13, 0.2);
    free_pixels(image->pixels, height);
    image->pixels = blured; 
    //free_kernel(kernel);


    update_surface(surface, image);
    SDL_SaveBMP(surface, "blured.bmp");
    Uint8** bin = binarisation(image);
    free_pixels(image->pixels, height);
    image->pixels = bin;

    update_surface(surface, image);
    SDL_SaveBMP(surface, "binarised.bmp");

    SDL_FreeSurface(surface);
    free_pixels(image->pixels, image->h);


    return 0;    
}
