# Variables
CC = gcc
# CFLAGS = -fopenmp -Wall -O2
# CFLAGS = -pthread 
TARGET = build
SRC ?= Fuentes_Practica1/matrices.c
N ?= 16
BS ?= 16

# Compilar el programa
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Ejecutar el programa con argumentos
run: $(TARGET)
	@echo "Ejecutando con N=$(N), SRC = $(SRC)"
	./$(TARGET) $(N) $(BS)

all-run: all run

# Limpiar los archivos generados
clean:
	$(RM) $(TARGET)

.PHONY: all clean run all-run   
