#include <stdio.h>
#include <stdlib.h>

//D'apres google ca me permettrait de changer la dimension plus facilement
#define SIZE 9




int main(int argc, char* argv) {
  FILE *inFile = fopen(argv[1], "r");
  if (!inFile) {
      printf("Impossible d'ouvrir le fichier d'entrée.\n");
      return 1;
  }
  
  //création du Tableau d'entrée
  int sudoku_ori[SIZE][SIZE];
  int sudoku_res[SIZE][SIZE];

  int row = 0;
  int col = 0;

  char n;
  do {
      n = fgetc(file);
      if (n == '\n' && c == 0) {continue;} //cas de la ligne au milieu
      if (n == '\n') {r++; c = 0; continue;} //cas de fin ligne
      if (n == ' ' || n == '\0') {continue;} //cas de l'espaces
      //if n is a point translates it in a zero
      if (n == '.') { sudoku_ori[row][cow] = 0; sudoku_res[row][cow] = 0; c++; continue;}
      if ('0' > n && n > '9') {return 0;}
      //cas normal
      sudoku_ori[row][col] = n - '0';
      sudoku_res[row][col] = n - '0';
      col++;
  } while (n != EOF && row < 11);

  fclose(file);
  return 1;

  row = 0;
  col = 0;

  if (is_solvable(argv[1])){
    if (solve(sudoku, 0, 0)) {
        // Écriture de la solution dans le fichier de sortie
        char *resFileName = strcat(argv[1], ".result");
        FILE *resFile = fopen(resFileName, "w");
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                fprintf(resFile, "%d ", sudoku[i][j]); 
                if (j == 4 || j == 7)
                    fprintf(resFile, "%d ", ' ');
            }
            fprintf(resFile, "%d ", '\n');
        }
        fclose(resFile);
    } 
  }
  return 0;
}

int create_solved_sudoku(char* name)
{
    FILE* solved;
    char* solvedName = name;

    solvedName = strcat(solvedName, ".result");
    solved = fopen(solvedName, "w");

    if (solved == NULL) {
        return 0;
    }

    //add the element of the grid,
    //the spaces and the newlines
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (j == 3 || j == 6)
                fputc(' ', solved);
            fputc('0' + grid[i][j], solved);
        }
        if (i == 2 || i == 5)
            fputc('\n', solved);
        fputc('\n', solved);

    }

    fclose(solved);

    return 1;
}

//Affichage du sudoku (Debug)
void printSudoku(int sudoku[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", sudoku[i][j]);
            if (j%3 - 1 == 0){
                printf(" ");
            }
        }
        printf("\n");
    }
}


//Résolution du sudoku
int is_solvable (int puzzle[][SIZE]){
    int indice = 0;

    for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      if (!puzzle[x][y]) {
        indice ++;
        if (indice > 16){return 1;}
      }
    }
  }
  return 0;
}

int valid(int puzzle[][SIZE], int row, int col, int guess) {
  int corner_x = row / 3 * 3;
  int corner_y = col / 3 * 3;

  for (int x = 0; x < SIZE; ++x) {
    if (puzzle[row][x] == guess) return 0;
    if (puzzle[x][col] == guess) return 0;
    if (puzzle[corner_x + (x % 3)][corner_y + (x / 3)] == guess) return 0;
  }
  return 1;
}

int find_empty_cell(int puzzle[][SIZE], int *row, int *col) {
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      if (!puzzle[x][y]) {
        *row = x;
        *col = y;
        return 1;
      }
    }
  }
  return 0;
}

int solve(int puzzle[][SIZE]) {
  int row;
  int col;
  if(!find_empty_cell(puzzle, &row, &col)) return 1;
  for (int guess = 1; guess < 10; guess++) {
    if (valid(puzzle, row, col, guess)) {
      puzzle[row][col] = guess;
      if(solve(puzzle)) return 1;
      puzzle[row][col] = 0;
    }
  }
  return 0;
}