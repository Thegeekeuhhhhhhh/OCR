# Makefile

CC=gcc
CPPFLAGS= -MMD -D_XOPEN_SOURCE=500
CFLAGS= -Wall -Wextra -std=c99 -O2 `pkg-config --cflags sdl2 SDL2_image` `pkg-config --cflags gtk+-3.0`
LDFLAGS=
LDLIBS= -lm `pkg-config --libs sdl2 SDL2_image` `pkg-config --libs gtk+-3.0`

SRC= ui.c

OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}
EXE = ${SRC:.c=}

ui: ui.o

clean:
	rm -f ${OBJ} ${DEP} ${EXE}

# END
