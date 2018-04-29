#Compilador a usar
CC=g++

#Flags
CPPFLAGS= -lm -std=c++11 -Wall

#Objetivo
TARGET=main

#Dependencias (archivo.h)
DEPS = Klondike.hpp

#Objetos (archivo.o)
OBJ =  Klondike.o $(TARGET).o

#Compila todos los archivos objeto
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

#Linkea todos los archivos objeto
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

#Borra los archivos objeto
clean:
	rm -f *.o
