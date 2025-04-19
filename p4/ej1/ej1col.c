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
  int *a, *b, *local_c, *c, *scattered_a;

  MPI_Init(&argc, &argv);
  if (argc < 2)
    return EXIT_FAILURE;
  int N = atoi(argv[1]);

  int id, comm_size, work_load;
  int i, j;
  double start, end;

  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  work_load = N * N / comm_size;

  b = (int *)malloc(sizeof(int) * N * N);
  local_c = (int *)calloc(work_load, sizeof(int));
  scattered_a = (int *)malloc(sizeof(int) * work_load);

  if (id == 0) { // Master process
    a = (int *)malloc(sizeof(int) * N * N);
    c = (int *)malloc(sizeof(int) * N * N);
    for (i = 0; i < N; i++) {
      int aux = i * N;
      for (int j = 0; j < N; j++) {
        a[aux + j] = 1; // Stored by rows
        b[aux + j] = 1; // Stored by columns
        c[aux + j] = 0;
      }
    }

    start = MPI_Wtime();
  }

  MPI_Bcast(b, N * N, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(a, work_load, MPI_INT, scattered_a, work_load, MPI_INT, 0,
              MPI_COMM_WORLD);

  multiply(scattered_a, b, local_c, id, work_load, N);

  MPI_Gather(local_c, work_load, MPI_INT, c, work_load, MPI_INT, 0,
             MPI_COMM_WORLD);

  if (id == 0) {
    end = MPI_Wtime();
    printf("%f\n", end - start);
  }

  MPI_Finalize();

  return EXIT_SUCCESS;
}
