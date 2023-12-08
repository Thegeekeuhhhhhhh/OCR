CC = gcc
CFLAGS = -Wall -Werror -std=c11
LDFLAGS = `pkg-config --libs sdl2 SDL2_image` -lm #-lSDL2 -lSDL2_image

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXECUTABLE = my_program

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLE)

