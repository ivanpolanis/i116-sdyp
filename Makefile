# Variables
CC = gcc
TARGET = build
SRC ?= p3/src/ejercicio1.c
UTILS ?= utils/utils.c
# N ?= 8

# Compilar el programa
all: $(TARGET)

# Target for default compilation
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)


pt: clean
	$(CC) -pthread -o $(TARGET) $(SRC) $(UTILS)

mp: clean
	$(CC) -fopenmp -o $(TARGET) $(SRC) $(UTILS)


# Target for single precision
single: clean
	$(CC) -O2 -lm -o $(TARGET) $(SRC)

# Target for double precision
double: clean
	$(CC) -O2 -DDOUBLE -lm -o $(TARGET) $(SRC)

# Ejecutar el programa con argumentos
run: 
	@echo "Ejecutando con N=$(N) - T=$(T) - SRC = $(SRC)"
	./$(TARGET) $(N) $(T)

all-run: all run
pt-run: pt run
mp-run: mp run
single-run: single run
double-run: double run

# Limpiar los archivos generados
clean:
	$(RM) $(TARGET)

.PHONY: all clean run single double all-run pt-run mp-run single-run double-run
