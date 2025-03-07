#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int *m, *l, *u, *tot;
  int i, j, k;
  int N, check = 1;

  if (argc < 2) {
    printf("\n Falta un argumento:: N dimension de la matriz \n");
    return 0;
  }

  N = atoi(argv[1]);

  m = (int *)malloc(sizeof(int) * N * N);
  l = (int *)malloc(sizeof(int) * N * N);
  u = (int *)malloc(sizeof(int) * N * N);
  tot = (int *)malloc(sizeof(int) * N * N);

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      m[i * N + j] = 1;
      u[i * N + j] = 0;
      l[i * N + j] = 0;
      tot[i * N + j] = 0;
      if (i == j || j > i)
        u[i * N + j] = 1;
      if (i == j || j < i)
        l[i * N + j] = 1;
    }
  }

  double timetick = dwalltime();

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      for (k = 0; k < j + 1; k++) {
        tot[i * N + j] = tot[i * N + j] + m[i * N + k] * u[k * N + j];
      }
      for (k = i; k < N; k++) {
        tot[i * N + j] = tot[i * N + j] + u[i * N + k] * m[k * N + j];
      }
      for (k = j; k < N; k++) {
        tot[i * N + j] = tot[i * N + j] + m[i * N + k] * l[k * N + j];
      }
      for (k = 0; k < i + 1; k++) {
        tot[i * N + j] = tot[i * N + j] + l[i * N + k] * m[k * N + i];
      }
    }
  }

  printf("Tiempo en segundos: %f\n", dwalltime() - timetick);

  free(l);
  free(u);
  l = (int *)malloc(sizeof(int) * N * (N + 1) / 2);
  u = (int *)malloc(sizeof(int) * N * (N + 1) / 2);

  for (i = 0; i < (N * (N + 1)) / 2; i++) {
    u[i] = 1;
    l[i] = 1;
  }

  timetick = dwalltime();

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      for (k = 0; k < j + 1; k++) {
        tot[i * N + j] =
            tot[i * N + j] + m[i * N + k] * u[k * N + j - k * (k + 1) / 2];
      }
      for (k = i; k < N; k++) {
        tot[i * N + j] =
            tot[i * N + j] + u[k * N + j - k * (k + 1) / 2] * m[k * N + i];
      }
      for (k = j; k < N; k++) {
        tot[i * N + j] = tot[i * N + j] + m[i * N + k] * l[j + k * (k + 1) / 2];
      }
      for (k = 0; k < i + 1; k++) {
        tot[i * N + j] = tot[i * N + j] + l[k + i * (i + 1) / 2] * m[k * N + i];
      }
    }
  }

  printf("Tiempo en segundos: %f\n", dwalltime() - timetick);

  // Print matrix
  // for (i = 0; i < N; i++) {
  //   for (j = 0; j < N; j++) {
  //     printf("%d ", tot[i * N + j]);
  //   }
  //   printf("\n");
  // }

  free(m);
  free(l);
  free(u);
  free(tot);

  return 0;
}
