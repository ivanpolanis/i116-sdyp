#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int *A, *Ac, *tot;
  int N, check = 1;
  int i, j, k;

  if (argc < 2) {
    printf("\n Falta un argumento:: N dimension de la matriz \n");
    return 0;
  }

  N = atoi(argv[1]);

  A = (int *)malloc(sizeof(int) * N * N);
  Ac = (int *)malloc(sizeof(int) * N * N);
  tot = (int *)malloc(sizeof(int) * N * N);

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      A[i * N + j] = 1;
      tot[i * N + j] = 0;
    }
  }

  double timetick = dwalltime();

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      for (k = 0; k < N; k++) {
        tot[i * N + j] = tot[i * N + j] + A[i * N + k] * A[k * N + j];
      }
    }
  }

  printf("Tiempo en segundos CON A ORDENADA POR FILAS %f \n",
         dwalltime() - timetick);

  // Verifica el resultado
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      check = check && (tot[i * N + j] == N);
    }
  }

  if (check) {
    printf("Multiplicacion de matrices resultado correcto\n");
  } else {
    printf("Multiplicacion de matrices resultado erroneo\n");
  }

  timetick = dwalltime();

  // Construccion de la matriz A ordenada por columnas.
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      Ac[i * N + j] = A[i + j * N];
    }
  }

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      for (k = 0; k < N; k++) {
        tot[i * N + j] = tot[i * N + j] + A[i * N + k] * Ac[k + j * N];
      }
    }
  }

  printf("Tiempo en segundos CON Ac ORDENADA POR COLUMNAS %f \n",
         dwalltime() - timetick);

  // Verifica el resultado
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      check = check && (tot[i * N + j] == N * 2);
    }
  }

  if (check) {
    printf("Multiplicacion de matrices resultado correcto\n");
  } else {
    printf("Multiplicacion de matrices resultado erroneo\n");
  }

  free(A);
  free(Ac);
  free(tot);
}
