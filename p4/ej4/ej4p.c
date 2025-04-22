#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_array(int *arr, int size) {
  for (int i = 0; i < size; i++)
    printf("%d ", arr[i]);
  printf("\n");
}
void merge(int *arr, int left, int middle, int right) {
  int i, j, k;
  int n1 = middle - left + 1;
  int n2 = right - middle;

  // Create temporary arrays
  int arr1[n1], arr2[n2];

  for (i = 0; i < n1; i++)
    arr1[i] = arr[left + i];
  for (i = 0; i < n2; i++)
    arr2[i] = arr[middle + 1 + i];

  i = 0, j = 0, k = left;

  while (i < n1 && j < n2) {
    if (arr1[i] <= arr2[j]) {
      arr[k] = arr1[i];
      i++;
    } else {
      arr[k] = arr2[j];
      j++;
    }
    k++;
  }

  while (i < n1) {
    arr[k] = arr1[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = arr2[j];
    j++;
    k++;
  }
};

void mergesort(int *arr, int left, int right) {
  if (left < right) {
    int middle = left + (right - left) / 2;

    mergesort(arr, left, middle);
    mergesort(arr, middle + 1, right);

    merge(arr, left, middle, right);
  }
};

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  int id, comm_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  if (argc < 2)
    return EXIT_FAILURE;
  int N = atoi(argv[1]);
  int work_load = N / comm_size;

  int *A = NULL;
  int *local_a = malloc(sizeof(int) * work_load);

  double start, end;

  if (id == 0) {
    A = malloc(sizeof(int) * N);
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
      A[i] = rand() % 100;
    }
    start = MPI_Wtime();
  }

  MPI_Scatter(A, work_load, MPI_INT, local_a, work_load, MPI_INT, 0,
              MPI_COMM_WORLD);

  // Orden local
  mergesort(local_a, 0, work_load - 1);

  int current_size = work_load;
  int step = 1;
  while (step < comm_size) {
    if (id % (2 * step) == 0) {
      if (id + step < comm_size) {
        int recv_size;
        MPI_Recv(&recv_size, 1, MPI_INT, id + step, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        int *recv_data = malloc(sizeof(int) * recv_size);
        MPI_Recv(recv_data, recv_size, MPI_INT, id + step, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        // Copiar ambos arrays a uno nuevo
        int *merged = malloc(sizeof(int) * (current_size + recv_size));
        for (int i = 0; i < current_size; i++)
          merged[i] = local_a[i];
        for (int i = 0; i < recv_size; i++)
          merged[current_size + i] = recv_data[i];

        // Hacer merge (tu función requiere índices)
        merge(merged, 0, current_size - 1, current_size + recv_size - 1);

        free(local_a);
        free(recv_data);
        local_a = merged;
        current_size += recv_size;
      }
    } else {
      int dest = id - step;
      MPI_Send(&current_size, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
      MPI_Send(local_a, current_size, MPI_INT, dest, 0, MPI_COMM_WORLD);
      free(local_a);
      break;
    }
    step *= 2;
  }

  if (id == 0) {
    end = MPI_Wtime();
    printf("Tiempo total: %f segundos\n", end - start);
    // print_array(local_a, N);  // si querés ver el resultado
    free(A);
    free(local_a);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}
