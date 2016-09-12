CC=g++
CFLAGS= -g -std=c++11
TARGET = id3

all:$(TARGET)

$(TARGET):$(TARGET).o
	$(CC) $(CFLAGS) $(TARGET).o -o $(TARGET)
     
$(TARGET).o: $(TARGET).cpp
	$(CC) -g -c $(TARGET).cpp
	
clean:
	rm *o id3