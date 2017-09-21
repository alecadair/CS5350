CC=g++
OBJ = id3.o tree_functions.o main.o
DEPS = tree_functions.h id3.h

all:id3

id3:$(OBJ)
	$(CC) -o id3 $(OBJ)

main.o:main.cpp id3.h tree_functions.h
	$(CC) -c main.cpp
id3.o:id3.cpp 
	$(CC) -c id3.cpp
tree_functions.o:tree_functions.cpp id3.h
	$(CC) -c tree_functions.cpp

clean:
	rm id3 $(OBJ)
