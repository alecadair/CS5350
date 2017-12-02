
CC=g++
OBJ = svm.o main.o naive_bayes.o logistic_regression.o vector_funcs.o id3.o
DEPS = svm.h naive_bayes.h logistic_regression.h vector_funcs.h id3.h
FLAGS = -g -std=c++11
all:assign5

assign5:$(OBJ)
	$(CC) $(FLAGS) -o assign5 $(OBJ)
main.o:main.cpp
	$(CC) $(FLAGS) -c main.cpp
svm.o:svm.cpp
	$(CC) $(FLAGS) -c svm.cpp
naive_bayes.o:naive_bayes.cpp
	$(CC) $(FLAGS) -c naive_bayes.cpp
logistic_regression.o:logistic_regression.cpp
	$(CC) $(FLAGS) -c logistic_regression.cpp
vector_funcs.o:vector_funcs.cpp
	$(CC) $(FLAGS) -c vector_funcs.cpp
id3.o:id3.cpp
	$(CC) $(FLAGS) -c id3.cpp
clean:
	rm -f svm $(OBJ)
	
