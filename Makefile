#Compilador a usar
CC=g++

#Flags
CFLAGS=-lm -std=c++11 -Wall

#Objetivo
TARGET=main

#Dependencias (archivo.h)
DEPS = 

#Objetos (archivo.o)
OBJ = $(TARGET).o


#Compila todos los archivos objeto
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#Linkea todos los archivos objeto
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

#Borra los archivos objeto
clean:
	rm -f *.o
