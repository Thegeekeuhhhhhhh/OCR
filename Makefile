# Makefile

CC = gcc
CPPFLAGS= -MMD -D_XOPEN_SOURCE=500
CFLAGS= -Wall -Wextra -std=c99 -O2 -O3 -lSDL2 -lSDL2_image `pkg-config --cflags sdl2 SDL2_image` `pkg-config --cflags gtk+-3.0`
LDFLAGS= -lm `pkg-config --libs sdl2 SDL2_image`
LDLIBS= -lm `pkg-config --libs sdl2 SDL2_image` `pkg-config --libs gtk+-3.0`

all: quoicOCR

quoicOCR: ui.o neuralnetwork.o test.o load.o train.o matrix.o
	$(CC) $(CFLAGS) -o quoicOCR ui.o neuralnetwork.o train.o matrix.o load.o $(LDFLAGS) $(LDLIBS)

neuralnetwork.o: Reco_Network/neuralnetwork.c Reco_Network/neuralnetwork.h Reco_Network/train.h Reco_Network/matrix.h Reco_Network/load.h
	$(CC) $(CFLAGS) -c Reco_Network/neuralnetwork.c $(LDLIBS)

ui.o: ui.c ui.h Reco_Network/neuralnetwork.o
	$(CC) $(CFLAGS) -c ui.c $(LDLIBS)

test.o: Reco_Network/test.c
	$(CC) $(CFLAGS) -c Reco_Network/test.c

load.o: Reco_Network/load.c Reco_Network/load.h
	$(CC) $(CFLAGS) -c Reco_Network/load.c

train.o: Reco_Network/train.c Reco_Network/train.h Reco_Network/matrix.h
	$(CC) $(CFLAGS) -c Reco_Network/train.c

matrix.o: Reco_Network/matrix.c Reco_Network/matrix.h
	$(CC) $(CFLAGS) -c Reco_Network/matrix.c

clean:
	rm -f *.o neuralnetwork *.d
	rm -f *.o load *.d
	rm -f *.o ui *.d
	rm -f *.o train *.d
	rm Values.txt

