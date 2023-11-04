#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sudoku_solver.h"

//D'apres google ca me permettrait de changer la dimension plus facilement
#define SIZE 9

//création des Tableau;
int sudoku_ori[SIZE][SIZE];
int sudoku_res[SIZE][SIZE];
int row = 0, col = 0;


int main_solver(int argc, char** argv) {
    if (argc == 2)
    {
        FILE* inFile = fopen(argv[1], "r");
        if (!inFile) {
            printf("Impossible d'ouvrir le fichier d'entrée.\n");
            return 0;
        }
        char n;
        do {
            n = fgetc(inFile);
            //cas de la ligne au milieu
            if (n == '\n' && col == 0) { continue; }
            //cas de fin ligne
            if (n == '\n') { row++; col = 0; continue; } 
            //cas de l'espaces
            if (n == ' ' || n == '\0') { continue; } 
            //if n is a point translates it in a zero
            if (n == '.')
            {sudoku_ori[row][col] = 0;
            sudoku_res[row][col] = 0; col++; continue;}
            //cas d'erreur
            if ('0' > n && '9' < n) { return 0; }
            //cas normal
            sudoku_ori[row][col] = n - '0';
            sudoku_res[row][col] = n - '0';
            col++;
        } while (!feof(n) && row < 11);
        // feof() check si c'est la fin du fichier
        fclose(inFile);
        
        // sudoku solvable ?
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                if (sudoku_res[i][j])
                    if (!valid(i, j, sudoku_res[i][j]))
                        return 0;


        //used to debug: print_sudoku(sudoku_res);
        
        //résolution
        if (solve_cell(0, 0)) {
            printf("\nunsolvable grid !\n");
            return 0;
        }


        //used to debug: print_sudoku(sudoku_res);
        
        // ecriture dans le fichier solution
        FILE* solved;
        char* outName = argv[1];
        outName = strcat(outName, ".result");
        solved = fopen(outName, "w");
        if (solved == NULL) {
            return 0;
        }
        //add the element of the grid,
        //the spaces and the newlines
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (j == 3 || j == 6)
                    fputc(' ', solved);
                fputc('0' + sudoku_res[i][j], solved);
            }
            if (i == 2 || i == 5)
                fputc('\n', solved);
            fputc('\n', solved);
        }
        fclose(solved);
        return 1;
    }
    else
        return 0;
}


//Résolution du sudoku


//solver
int solve_cell(int row, int col)
{
    int n = 1;
    //cas fin de ligne + incrémente ligne
    if (col == 9) {col = 0;row++;}
    //cas tout c'est bien passé :)
    if (row == 9)
        return 0;
    //check values
    while (n < 10) {
        if (valid(row, col, n)) {
            sudoku_res[row][col] = n;
            if (!solve_cell(row, col + 1))
                // cas il y en as un qui peux rien avoir
                return 0;
        }
        sudoku_res[row][col] = 0;
        n++;
    }
    return 1;
}


//verifie si la case est valide
int valid(int row, int col, int guess) {
  int corner_x = row / 3 * 3;
  int corner_y = col / 3 * 3;
  if (sudoku_ori[row][col] != guess) return 0;
  for (int x = 0; x < SIZE; ++x) {
    if (sudoku_res[row][x] == guess) return 0;
    if (sudoku_res[x][col] == guess) return 0;
    if (sudoku_res[corner_x + (x % 3)][corner_y + (x / 3)] == guess) return 0;
  }
  return 1;
}

//print the solved_states
void print_sudoku(int sudo[SIZE][SIZE])
{
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0)
            printf("+-------+-------+-------+\n");
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0)
                printf("| ");
            sudo[i][j] != 0 ?
                printf("%d ", sudo[i][j]) : printf(". ");
        }
        printf("|\n");
    }
}