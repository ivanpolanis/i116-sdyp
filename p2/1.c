#include "../utils/utils.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int t, T = 1;
int N = 1024;
int *A, *B, *C;

/// This function does the operation A . B. B must be stored by rows, otherwise
/// the output will be wrong.
void multiply_matrix(int *A, int *B, int *C, int start_row, int end_row) {
  for (int i = start_row; i < end_row; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < N; k++) {
        C[i * N + j] += A[i * N + k] * B[j * N + k];
      }
    }
  }
}

void *thread(void *args) {
  int id = *(int *)args;
  int work_load = N / t;

  multiply_matrix(A, B, C, id * work_load, (id + 1) * work_load);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  int i, j, check = 1;
  double seq_time;
  double start, end;
  if (argc > 1)
    T = atoi(argv[1]);

  A = (int *)malloc(sizeof(int) * N * N);
  B = (int *)malloc(sizeof(int) * N * N);
  C = (int *)malloc(sizeof(int) * N * N);

  //7//////////////////////////// Sequential \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
  ///////////////////////////////////////////////////////////////////////////////////////
  // Initializing matrices
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      A[i * N + j] = 1;
      B[i + j * N] = 1;
      C[i * N + j] = 0;
    }
  }

  start = dwalltime();
  multiply_matrix(A, B, C, 0, N);
  seq_time = dwalltime() - start;
  print_performance(1, seq_time, seq_time);

  ///////////////////////////////////////////////////////////////////////////////////////

  for (t = 2; t <= T; t *= 2) {

    pthread_t threads[t];
    int ids[t];
    // Initializing matrices
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
        A[i * N + j] = 1;
        B[i + j * N] = 1;
        C[i * N + j] = 0;
      }
    }

    start = dwalltime();
    // Creating threads
    for (i = 0; i < t; i++) {
      ids[i] = i;
      pthread_create(&threads[i], NULL, &thread, &ids[i]);
    }

    // Wait for threads
    for (i = 0; i < t; i++) {
      pthread_join(threads[i], NULL);
    }
    end = dwalltime();

    print_performance(t, seq_time, end - start);
  }

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      check = check && (C[i * N + j] == N);
    }
  }
  if (check) {
    printf("Multiplicacion de matrices resultado correcto\n");
  } else {
    printf("Multiplicacion de matrices resultado erroneo\n");
  }
  free(A);
  free(B);
  free(C);

  return 0;
}
