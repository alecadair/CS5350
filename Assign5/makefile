
CC=g++
OBJ = svm.o main.o naive_bayes.o
DEPS = svm.h naive_bayes.h
FLAGS = -g -std=c++11
all:svm

svm:$(OBJ)
	$(CC) $(FLAGS) -o svm $(OBJ)

main.o:main.cpp
	$(CC) $(FLAGS) -c main.cpp
svm.o:svm.cpp
	$(CC) $(FLAGS) -c svm.cpp
naive_bayes.o::naive_bayes.cpp
	$(CC) $(FLAGS) -c naive_bayes.cpp
clean:
	rm -f svm $(OBJ)
	
