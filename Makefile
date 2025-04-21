# Makefile

CC = gcc
LDFLAGS = -lm -lSDL2
CFLAGS= -Wall -Wextra -std=c99 -lSDL2 -lm `pkg-config --cflags --libs gtk+-3.0` `sdl2-config --cflags --libs` -lSDL2_image

all: ui.o
	mv ui.o ocr
	echo "Make complete !"

ui.o: ui.c Reco_Network/matrix.c Reco_Network/train.c Reco_Network/load.c Reco_Network/nn.c
	$(CC) $+ $(CFLAGS) -o ui.o $(LDFLAGS)

clean:
	rm -f *.o ocr *.d
	rm -f *.o ui *.d
	rm -f *.bmp
