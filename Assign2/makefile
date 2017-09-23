
CC=g++
#OBJ = id3.o tree_functions.o main.o
OBJ = main.o
DEPS = tree_functions.h id3.h
FLAGS = -g -std=c++11
all:perceptron

perceptron:$(OBJ)
	$(CC) $(FLAGS) -o perceptron $(OBJ)

main.o:main.cpp
	$(CC) $(FLAGS) -c main.cpp

clean:
	rm perceptron $(OBJ)
	
