CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lSDL2 -lSDL2_image

all: rotation sudoku_solver

rotation: rotation.c rotation.h
	$(CC) $(CFLAGS) -o rotation rotation.c $(LDFLAGS)

sudoku_solver: sudoku_solver.c sudoku_solver.h
	$(CC) $(CFLAGS) -o sudoku_solver sudoku_solver.c

clean:
	rm -f rotation sudoku_solver

.PHONY: all clean
