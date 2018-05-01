#Compilador a usar
CC=g++

#Flags
CPPFLAGS= -lm -std=c++11 -Wall -Isrc -lm -lGL -lGLU -lglut

#Objetivo
TARGET=Explorador

all:
	$(CC) $(TARGET).cpp src/Klondike.cpp $(CPPFLAGS) -o $(TARGET)

#Borra los archivos objeto
clean:
	rm -f *.o $(TARGET) src/*.o
