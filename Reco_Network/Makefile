CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -fsanitize=address
LDFLAGS = -lm -fsanitize=address

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
