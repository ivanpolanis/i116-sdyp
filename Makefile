# Variables
CC = gcc
MPIC = mpicc
MPIR = mpirun
TARGET = build
SRC ?= /home/ivan/Documents/i116-sdyp/p4/ej1col.c
UTILS ?= utils/utils.c
# N ?= 8

# Compilar el programa
all: $(TARGET)

# Target for default compilation
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(UTILS)


pt: clean
	$(CC) -pthread -o $(TARGET) $(SRC) $(UTILS)

mp: clean
	$(CC) -lm -fopenmp -o $(TARGET) $(SRC) $(UTILS)

mpi: clean
	$(MPIC) -o $(TARGET) $(SRC)
	$(MPIR) -np $(T) $(TARGET) $(N)


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

all-run: clean all run
pt-run: pt run
mp-run: mp run
single-run: single run
double-run: double run

# Limpiar los archivos generados
clean:
	$(RM) $(TARGET)

.PHONY: all clean run single double all-run pt-run mp-run single-run double-run mpi
