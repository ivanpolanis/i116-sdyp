# Variables
CC = gcc
# CFLAGS = -fopenmp -Wall -O2
# CFLAGS = -pthread 
TARGET = build
SRC ?= p2/3.c
UTILS ?= utils/utils.c
N ?= 8

# Compilar el programa
all: $(TARGET)

# Target for default compilation
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)


pt: clean
	$(CC) -pthread -o $(TARGET) $(SRC) $(UTILS)

# Target for single precision
single: clean
	$(CC) -O2 -lm -o $(TARGET) $(SRC)

# Target for double precision
double: clean
	$(CC) -O2 -DDOUBLE -lm -o $(TARGET) $(SRC)

# Ejecutar el programa con argumentos
run: 
	@echo "Ejecutando con T=$(N), SRC = $(SRC)"
	./$(TARGET) $(N) $(BS)

all-run: all run
pt-run: pt run
single-run: single run
double-run: double run

# Limpiar los archivos generados
clean:
	$(RM) $(TARGET)

.PHONY: all clean run single double all-run pt-run single-run double-run
