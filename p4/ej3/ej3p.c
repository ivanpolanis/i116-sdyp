#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);
  double start, end;
  if (argc < 2)
    return EXIT_FAILURE;

  start = MPI_Wtime();
  int N = atoi(argv[1]);

  int *A, *B, *local_b;
  int id, comm_size, work_load;

  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  work_load = N / comm_size;

  A = (int *)malloc(sizeof(int) * N);
  local_b = (int *)malloc(sizeof(int) * work_load);

  B = (int *)malloc(sizeof(int) * N);
  if (id == 0) {
    for (int i = 0; i < N; i++) {
      A[i] = i;
    }
  }

  MPI_Bcast(A, N, MPI_INT, 0, MPI_COMM_WORLD);

  int changed = 0;

  int offset = id * work_load;
  for (int i = id * work_load; i < (id + 1) * work_load; i++) {
    for (int j = i + 1; j < N; j++) {
      if (A[j] == 2 * A[i]) {
        local_b[i - offset] = j - i;
        changed = 1;
        break;
      }
    }
    if (!changed)
      local_b[i - offset] = -1;
    changed = 0;
  }

  MPI_Gather(local_b, work_load, MPI_INT, B, work_load, MPI_INT, 0,
             MPI_COMM_WORLD);

  end = MPI_Wtime();
  if (id == 0)
    printf("%f", end - start);
  MPI_Finalize();

  return EXIT_SUCCESS;
}
