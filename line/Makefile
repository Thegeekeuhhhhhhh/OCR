CC = gcc
CFLAGS = -Wall -Wextra -g
LDLIBS = -lm `sdl2-config --cflags --libs` -lSDL2_image

SRCS = line.c gray_im.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: line

line: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDLIBS)

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) line
