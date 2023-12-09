#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#include "gray_im.h"

int ROWS = 9;
int COLS = 9;


char** createMatrix() {
    char** matrix = (char**)malloc(ROWS * sizeof(char*));
    for (int i = 0; i < ROWS; ++i) {
        matrix[i] = (char*)malloc(COLS * sizeof(char));
    }
    return matrix;
}


void deleteMatrix(char** matrix) {
    for (int i = 0; i < ROWS; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}


char** parser(const char* filename) {
    char** matrix = createMatrix();

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
    
    char c = fgetc(file);

    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            while(c == ' ' || c == '\n'){
                c = fgetc(file);
            }
            matrix[y][x] = c;
            c = fgetc(file);
        }
    }



    fclose(file);
    return matrix;
}


void printMatrix(char** matrix) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
}


void fixed_apply_digit(SDL_Surface* surface,const char* filename,int x, int y){
    if(*filename == '\0'){
        return;
    }
    SDL_Surface* digit = SDL_LoadBMP(filename);
    if(digit == NULL){
        err(1, "cant load image!");
    }
    SDL_Rect rect = {x, y, digit->w, digit->h};
    SDL_Rect dig = {0, 0, digit->w, digit->h};
    if(SDL_BlitSurface(digit, &dig, surface, &rect)){
        err(1, "cant copy surface to surface");
    }
    SDL_FreeSurface(digit);
}


char* return_digit_path(int digit, int red){// red = 1 / black = 0
    switch(red){
        case 0:
            switch(digit){
                case 1:return "digits/1.bmp";
                case 2:return "digits/2.bmp";
                case 3:return "digits/3.bmp";
                case 4:return "digits/4.bmp";
                case 5:return "digits/5.bmp";
                case 6:return "digits/6.bmp";
                case 7:return "digits/7.bmp";
                case 8:return "digits/8.bmp";
                case 9:return "digits/9.bmp";
                default:return "\0";
            }
         case 1:
             switch(digit){
                case 1:return "digits/1-red.bmp";
                case 2:return "digits/2-red.bmp";
                case 3:return "digits/3-red.bmp";
                case 4:return "digits/4-red.bmp";
                case 5:return "digits/5-red.bmp";
                case 6:return "digits/6-red.bmp";
                case 7:return "digits/7-red.bmp";
                case 8:return "digits/8-red.bmp";
                case 9:return "digits/9-red.bmp";
                default:return "\0";
            }
         default:err(1,"incorect value for attribute \"red\"");
    }
}


char** new_char_matrix(char** matrix, char* filename){
    char** final_mat = parser(filename);
    for(int j = 0; j < 9; j++){
        for(int i = 0; i < 9; i++){
            if(matrix[j][i] != '.'){
                final_mat[j][i] = '\0';
            }
        }
    }
    return final_mat;
}


void reco(const char* init_grid, char* final_grid){
    char** first_mat = parser(init_grid);
    char** secon_mat = new_char_matrix(first_mat, final_grid);
    SDL_Surface* surface = SDL_LoadBMP("digits/blank.bmp");
    for(int i = 0; i < 9; i++){
        for(int j =  0; j < 9; j++){
            fixed_apply_digit(surface, return_digit_path(first_mat[i][j] -  48,0), 111*j + 15, 111*i + 15);
        }
    }
    

    for(int i = 0; i < 9; i++){
        for(int j =  0; j < 9; j++){
            fixed_apply_digit(surface, return_digit_path(secon_mat[i][j] -  48,1), 111*j + 15, 111*i + 15);
        }
    }

    SDL_SaveBMP(surface, "grid.bmp");
    SDL_FreeSurface(surface);
}

/*
int main(int argc, char** argv){
    
    reco(argv[1],argv[2]);


    return 0;
}*/
