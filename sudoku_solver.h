#ifndef SOLVER_H
#define SOLVER_H

int main_solver(int argc, char** argv);
int valid(int row, int col, int guess);
int solve_cell(int row, int col);
void print_sudoku(int sudo[9][9]);

#endif
