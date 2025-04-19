#include "../../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>

void multiply(int *a, int *b, int *c, int N) {
  for (int i = 0; i < N; i++) {
    int aux_i = i * N;
    for (int j = 0; j < N; j++) {
      int aux_j = j * N;
      for (int k = 0; k < N; k++) {
        c[aux_i + j] += a[aux_i + k] * b[aux_j + k];
      }
    }
  }
}
int main(int argc, char *argv[]) {
  int *a, *b, *c, N;

  if (argc < 2)
    return EXIT_FAILURE;

  N = atoi(argv[1]);

  a = (int *)malloc(sizeof(int) * N * N);
  b = (int *)malloc(sizeof(int) * N * N);
  c = (int *)calloc(N * N, sizeof(int));

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      a[i * N + j] = 1;
      b[i * N + j] = 1;
    }
  }

  double start, end;

  start = dwalltime();
  multiply(a, b, c, N);
  end = dwalltime();

  printf("%f\n", end - start);

  return EXIT_SUCCESS;
}
