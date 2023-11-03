CC = gcc #-fsanitize=address
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: gaussian_blur binarisation

SRC = gray_im.c gaussian_blur.c binarisation.c
OBJ = ${SRC:.c=.o}

# Define dependencies for each target
gaussian_blur: gray_im.o gaussian_blur.o
	$(CC) -o $@ $^ $(LDLIBS)

binarisation: gray_im.o gaussian_blur.o binarisation.o
	$(CC) -o $@ $^ $(LDLIBS)

# Compilation rules for source files
gray_im.o: gray_im.c gray_im.h
	$(CC) $(CFLAGS) -c -o $@ $<

gaussian_blur.o: gaussian_blur.c gaussian_blur.h gray_im.h
	$(CC) $(CFLAGS) -c -o $@ $<

binarisation.o: binarisation.c binarisation.h gray_im.h gaussian_blur.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f gaussian_blur binarisation $(OBJ)

