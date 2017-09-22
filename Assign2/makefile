
CC=g++
#OBJ = id3.o tree_functions.o main.o
OBJ = main.o
DEPS = tree_functions.h id3.h

all:perceptron

perceptron:$(OBJ)
	$(CC) -g -o perceptron $(OBJ)

main.o:main.cpp
	$(CC) -c -g main.cpp

clean:
	rm perceptron $(OBJ)
	