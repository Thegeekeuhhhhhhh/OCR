#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>

#include "gray_im.h"
#include "gaussian_blur.h"
#include "binarisation.h"

/*
// function to swap elements
void swap(Uint8 *a, Uint8 *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

// function to find the partition position
int partition(Uint8 array[], int low, int high) {
  
  // select the rightmost element as pivot
  int pivot = array[high];
  
  // pointer for greater element
  int i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot
  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {
        
      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;
      
      // swap element at i with element at j
      swap(&array[i], &array[j]);
    }
  }

  // swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);
  
  // return the partition point
  return (i + 1);
}

void quickSort(Uint8 array[], int low, int high) {
  if (low < high) {
    
    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on right of pivot
    int pi = partition(array, low, high);
    
    // recursive call on the left of pivot
    quickSort(array, low, pi - 1);
    
    // recursive call on the right of pivot
    quickSort(array, pi + 1, high);
  }
}
*/

void merge(Uint8 arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (int i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }

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

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(Uint8 arr[], int n) {
    for (int currSize = 1; currSize <= n - 1; currSize *= 2) {
        for (int leftStart = 0; leftStart < n - 1; leftStart += 2 * currSize) {
            int mid = leftStart + currSize - 1;
            int rightEnd = leftStart + 2 * currSize - 1;
            if (rightEnd >= n) {
                rightEnd = n - 1;
            }
            merge(arr, leftStart, mid, rightEnd);
        }
    }
}

void swap(Uint8* a, Uint8* b) {
    Uint8 temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Uint8 arr[], int low, int high) {
    Uint8 pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}


void quicksort(Uint8 arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}


int median_value(struct my_image* image){
    Uint8** pixels = image->pixels;
    int height = image->h;
    int width = image->w;
    //printf("h = %d\nw = %d\n", height, width);
    Uint8* list = calloc(height*width , sizeof(Uint8));
    int list2[256] = {0};
    int index = 0;
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            //printf("(%d , %d)", j, i);
            list[index] = pixels[j][i];
            list2[pixels[j][i]] += 1;
            index++;
        }
    }
    //mergeSort(list, height*width );
    //quicksort(list, 0, height*width - 1);
    //Uint8 median = list[height*width/2];
    
    
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
    free(list);
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
    if(argc != 2){
        err(1, "binarisation: incorect arguments\n");
    }
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

    update_surface(surface, image);
    SDL_SaveBMP(surface, "binarised.bmp");

    SDL_FreeSurface(surface);
    free_pixels(image->pixels, image->h);


    return 0;    
}
