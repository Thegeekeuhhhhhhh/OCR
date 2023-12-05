CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 -lSDL2 -lSDL2_image `pkg-config --cflags sdl2 SDL2_image` 
LDFLAGS = -lm `pkg-config --libs sdl2 SDL2_image` -fsanitize=address
LDLIBS = 

all: neuralnetwork

neuralnetwork: neuralnetwork.o matrix.o
	$(CC) $(CFLAGS) -o neuralnetwork neuralnetwork.o matrix.o $(LDFLAGS)

neuralnetwork.o: neuralnetwork.c neuralnetwork.h matrix.h
	$(CC) $(CFLAGS) -c neuralnetwork.c

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) -c matrix.c

clean:
	rm -f *.o neuralnetwork *.d
	rm Values.txt
