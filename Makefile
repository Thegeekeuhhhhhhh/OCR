# Makefile

CC = gcc
CPPFLAGS= -MMD -D_XOPEN_SOURCE=500
CFLAGS= -Wall -Wextra -std=c99 -O2 -O3 -lSDL2 -lSDL_image `pkg-config --cflags sdl2 SDL_image` `pkg-config --cflags gtk+-3.0`
LDFLAGS= -lm `pkg-config --libs sdl2 SDL_image`
LDLIBS= -lm `pkg-config --libs sdl2 SDL_image` `pkg-config --libs gtk+-3.0`

all: ui.o
	mv ui.o ocr
	echo "Make complete !"

ui.o: ui.c Reco_Network/matrix.c Reco_Network/train.c Reco_Network/load.c Reco_Network/nn.c
	$(CC) $(CFLAGS) ui.c Reco_Network/train.c Reco_Network/matrix.c Reco_Network/load.c Reco_Network/nn.c -o ui.o $(LDLIBS)

clean:
	rm -f *.o ocr *.d
	rm -f *.o ui *.d
	rm -f *.bmp
