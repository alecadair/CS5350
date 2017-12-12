CC = g++
OBJ = main.o nearest_neighbors.o vector_funcs.o file_extraction.o id3.o bagged_forest.o kmeans.o
FLAGS = -g -std=c++11
DEPS = nearest_neighbors.h file_extraction.h vector_funcs.h id3.h bagged_forest.h kmeans.h

all:final_proj

final_proj:$(OBJ)
	$(CC) $(FLAGS) -o final_proj $(OBJ)

main.o : main.cpp
	$(CC) $(FLAGS) -c main.cpp
	
vector_funcs.o:vector_funcs.cpp
	$(CC) $(FLAGS) -c vector_funcs.cpp

nearest_neighbors.o: nearest_neighbors.cpp
	$(CC) $(FLAGS) -c nearest_neighbors.cpp
	
id3.o: id3.cpp
	$(CC) $(FLAGS) -c id3.cpp

file_extraction.o:file_extraction.cpp
	$(CC) $(FLAGS) -c file_extraction.cpp

bagged_forest.o:bagged_forest.cpp
	$(CC) $(FLAGS) -c bagged_forest.cpp
kmeans.o:kmeans.cpp
	$(CC) $(FLAGS) -c kmeans.cpp
	
clean:
	rm -f final_proj $(OBJ)
