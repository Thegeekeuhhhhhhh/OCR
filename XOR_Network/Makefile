# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -lm
LDFLAGS = -lm 
LDLIBS = -lm

EXE = perceptron neuralnetwork matrix
OBJ = perceptron.o neuralnetwork.o matrix.o
DEP = perceptron.d neuralnetwork.d matrix.d

all: $(EXE)

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} $(EXE)

# END
