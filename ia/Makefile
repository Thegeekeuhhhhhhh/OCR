CC = gcc
CFLAGS = -g -Wall -I/usr/include/SDL2
LDFLAGS = -lSDL2
SRC = ia.c

all: ia

ia: ia.o
	$(CC) $(CFLAGS) -o ia ia.o $(LDFLAGS)

ia.o: $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

clean:
	rm -f *.o ia

