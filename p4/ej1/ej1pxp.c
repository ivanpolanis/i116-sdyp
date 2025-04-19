#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void multiply(int *a, int *b, int *c, int id, int work_load, int N) {
  for (int i = 0; i < work_load / N; i++) {
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
  int *a, *b, *c;
  if (argc < 2)
    return EXIT_FAILURE;
  int N = atoi(argv[1]);

  a = (int *)malloc(sizeof(int) * N * N);
  b = (int *)malloc(sizeof(int) * N * N);
  c = (int *)malloc(sizeof(int) * N * N);
  MPI_Init(&argc, &argv);
  int id, comm_size, work_load;
  int i, j;
  double start, end;

  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  work_load = N * N / comm_size;

  if (id == 0) { // Master process
    for (i = 0; i < N; i++) {
      int aux = i * N;
      for (int j = 0; j < N; j++) {
        a[aux + j] = 1; // Stored by rows
        b[aux + j] = 1; // Stored by columns
        c[aux + j] = 0;
      }
    }

    start = MPI_Wtime();

    for (i = 1; i < comm_size; i++) {
      MPI_Send(&(a[i * work_load]), work_load, MPI_INT, i, i, MPI_COMM_WORLD);
      MPI_Send(b, N * N, MPI_INT, i, i, MPI_COMM_WORLD);
    }

    multiply(a, b, c, id, work_load, N);

    for (i = 1; i < comm_size; i++) {
      MPI_Recv(&(c[i * work_load]), work_load, MPI_INT, i, MPI_ANY_TAG,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    end = MPI_Wtime();
    printf("%f\n", end - start);

  } else {
    MPI_Recv(a, work_load, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Recv(b, N * N, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);

    multiply(a, b, c, id, work_load, N);

    MPI_Send(c, work_load, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return EXIT_SUCCESS;
}
