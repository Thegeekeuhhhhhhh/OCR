CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 -lSDL2 -lSDL2_image `pkg-config --cflags sdl2 SDL2_image` 
LDFLAGS = -lm `pkg-config --libs sdl2 SDL2_image` -fsanitize=address
LDLIBS = 

all: neuralnetwork

neuralnetwork: neuralnetwork.o train.o matrix.o load.o
	$(CC) $(CFLAGS) -o neuralnetwork neuralnetwork.o train.o matrix.o load.o $(LDFLAGS)

neuralnetwork.o: neuralnetwork.c neuralnetwork.h train.h matrix.h load.h
	$(CC) $(CFLAGS) -c neuralnetwork.c 

test.o: test.c
	$(CC) $(CFLAGS) -c test.c

load.o: load.c load.h
	$(CC) $(CFLAGS) -c load.c

train.o: train.c train.h matrix.h
	$(CC) $(CFLAGS) -c train.c

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) -c matrix.c

clean:
	rm -f *.o neuralnetwork *.d
	rm -f *.o load *.d
	rm -f *.o train *.d
	rm Values.txt
