#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>

#include "gray_im.h"
#include "gaussian_blur.h"
#include "binarisation.h"

// Merge two subarrays arr[l..m] and arr[m+1..r]
void merge(Uint8* arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temporary arrays
    int L[n1], R[n2];

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temporary arrays back into arr[l..r]
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Recursive merge sort function
void mergeSort(Uint8* arr, int l, int r) {
    if (l < r) {
        // Find the middle point
        int m = l + (r - l) / 2;

        // Recursively sort the first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // Merge the sorted halves
        merge(arr, l, m, r);
    }
}

int median_value(struct my_image* image){
    Uint8** pixels = image->pixels;
    int height = image->h;
    int width = image->w;
    printf("h = %d\nw = %d\n", height, width);
    Uint8* list = malloc(height*width * sizeof(Uint8));
    int index = 0;
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            //printf("(%d , %d)", j, i);
            list[index] = pixels[j][i];
            index++;
        }
    }
    mergeSort(list, 0, height*width - 1);
    Uint8 median = list[height*width/2];
    free(list);
printf("Return median\n");
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
    SDL_Surface* surface = load_image(argv[1]);
    struct my_image* image = init_image(surface);
    //float** kernel = init_kernel(2.5, 7);

    int height = image->h;

    Uint8** blured = apply_convolution(image, 13, 0.2);
    free_pixels(image->pixels, height);
    image->pixels = blured; 
    //free_kernel(kernel);


    update_surface(surface, image);
    SDL_SaveBMP(surface, "blured.bmp");
    Uint8** bin = binarisation(image);
    free_pixels(image->pixels, height);
    image->pixels = bin;

    printf("here\n");
    update_surface(surface, image);
    SDL_SaveBMP(surface, "binarised.bmp");

    SDL_FreeSurface(surface);
    free_pixels(image->pixels, image->h);


    return 0;    
}
