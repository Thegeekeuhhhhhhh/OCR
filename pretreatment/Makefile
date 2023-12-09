CC = gcc
CFLAGS = -Wall -Werror -std=c11
LDFLAGS = `pkg-config --libs sdl2 SDL2_image` -lm #-lSDL2 -lSDL2_image

all: pretreatment reconstruction

pretreatment: pretreatment.o gray_im.o
	$(CC) $(CFLAGS) -o pretreatment pretreatment.o gray_im.o $(LDFLAGS)

pretreatment.o: pretreatment.c pretreatment.h gray_im.c gray_im.h 
	$(CC) $(CFLAGS) -c pretreatment.c

gray_im.o: gray_im.c
	$(CC) $(CFLAGS) -c gray_im.c

reconstruction: reconstruction.o
	$(CC) $(CFLAGS) -o reconstruction reconstruction.o  $(LDFLAGS)

reconstruction.o: reconstruction.c
	$(CC) $(CFLAGS) -c reconstruction.c

clean:
	rm -f *.o gray_im *.d
	rm -f *.o reconstruction *.d
	rm -f *.o pretreatment *.d
	rm -f *.bmp
