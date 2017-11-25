
CC=g++
OBJ = svm.o main.o
DEPS = svm.h
FLAGS = -g -std=c++11
all:svm

svm:$(OBJ)
	$(CC) $(FLAGS) -o svm $(OBJ)

main.o:main.cpp
	$(CC) $(FLAGS) -c main.cpp
svm.o:svm.cpp
	$(CC) $(FLAGS) -c svm.cpp
clean:
	rm -f svm $(OBJ)
	
