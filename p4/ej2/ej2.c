#include <limits.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_NXN(int *A, int N) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++)
      printf("%d ", A[i * N + j]);
    printf("\n");
  }
}

void get_partials(int *A, int id, int work_load, int N, int *res) {
  int row_per_process = work_load / N;
  int mean = 0, min = INT_MAX, max = INT_MIN, val;
  for (int i = id * row_per_process; i < (id + 1) * row_per_process; i++) {
    int aux = i * N;
    for (int j = 0; j < N; j++) {
      val = A[aux + j];
      mean += val;
      min = val < min ? val : min;
      max = val > max ? val : min;
    }
  }

  res[0] = min;
  res[1] = max;
  res[2] = mean;
}

void get_total(int *partials, int *res, int comm_size, int N) {
  //[MIN; MAX; MEAN]
  res[2] = 0;
  for (int i = 0; i < 3 * comm_size; i += 3) {
    res[0] = partials[i] < res[0] ? partials[i] : res[0];
    res[1] = partials[i + 1] > res[1] ? partials[i + 1] : res[1];
    res[2] += partials[i + 2];
  }
  res[2] /= N * N;
}

void create_b(int *A, int *local_b, int *local_partials, int id,
              int work_load) {
  int offset = id * work_load;
  for (int i = 0; i < work_load + 1; i++) {
    if (A[i + offset] < local_partials[2]) {
      local_b[i] = local_partials[0];
    } else if (A[i + offset] > local_partials[2]) {
      local_b[i] = local_partials[1];
    } else {
      local_b[i] = local_partials[2];
    }
  }
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  int *A;
  MPI_Init(&argc, &argv);
  int N;
  if (argc < 2)
    return EXIT_FAILURE;
  N = atoi(argv[1]);
  int *local_a, *local_partials, *partials;
  int *local_b, *B;
  int id, comm_size, work_load, i, j;

  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  work_load = N * N / comm_size;

  if (id == 0) {
    A = (int *)malloc(sizeof(int) * N * N);
    B = (int *)malloc(sizeof(int) * N * N);
    for (i = 0; i < N; i++) {
      int aux = i * N;
      for (j = 0; j < N; j++) {
        A[aux + j] = rand() % 1000;
      }
    }

    partials = (int *)malloc(sizeof(int) * 3 * comm_size);
  }

  MPI_Bcast(A, N * N, MPI_INT, 0, MPI_COMM_WORLD);

  // Local partials will be stored as an array. [MIN, MAX, MEAN]
  local_partials = (int *)malloc(sizeof(int) * 3);

  get_partials(A, id, work_load, N, local_partials);

  MPI_Gather(local_partials, 3, MPI_INT, partials, 3, MPI_INT, 0,
             MPI_COMM_WORLD);

  if (id == 0) {
    get_total(partials, local_partials, comm_size, N);
  }

  MPI_Bcast(local_partials, 3, MPI_INT, 0, MPI_COMM_WORLD);

  local_b = (int *)malloc(sizeof(int) * work_load);

  create_b(A, local_b, local_partials, id, work_load);

  MPI_Gather(local_b, work_load, MPI_INT, B, work_load, MPI_INT, 0,
             MPI_COMM_WORLD);

  // if (id == 0) {
  //   printf("%d %d %d\n\n", local_partials[0], local_partials[1],
  //          local_partials[2]);
  //
  //   print_NXN(A, N);
  //   printf("\n\n\n");
  //   print_NXN(B, N);
  // }
  MPI_Finalize();
  return EXIT_SUCCESS;
}
