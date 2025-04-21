#include "../../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  if (argc < 2)
    return EXIT_FAILURE;

  double start, end;
  start = dwalltime();
  int N = atoi(argv[1]);

  int *A, *B;

  A = (int *)malloc(sizeof(int) * N);
  B = (int *)malloc(sizeof(int) * N);

  for (int i = 1; i < N + 1; i++) {
    A[i] = i;
  }

  int changed = 0;

  for (int i = 0; i < N; i++) {
    for (int j = i + 1; j < N; j++) {
      if (A[j] == 2 * A[i]) {
        B[i] = j - i;
        changed = 1;
        break;
      }
    }
    if (!changed)
      B[i] = -1;
    changed = 0;
  }

  for (int i = 0; i < N; i++)
    printf("%d ", A[i]);
  printf("\n");
  for (int i = 0; i < N; i++)
    printf("%d ", B[i]);

  free(A);
  free(B);
  end = dwalltime();
  printf("%f", end - start);

  return EXIT_SUCCESS;
}
