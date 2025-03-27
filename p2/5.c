#include "../utils/utils.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int t, T = 1;
int *arr;
unsigned long N = 1UL << 21;
pthread_barrier_t barrier;

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

void *thread(void *args) {
  int id = *(int *)args;
  int work_load = N / t;
  int left = id * work_load, right = id + 1 == t ? N : (id + 1) * work_load;
  mergesort(arr, left, right);

  pthread_barrier_wait(&barrier); // Synchronize threads before merging
  // Merge phase: progressively merge sections in log2(T) steps
  int step = 1;
  while (step < T) {
    if (id % (2 * step) == 0) { // Only some threads participate at each step
      int merge_right = left + step * work_load;
      if (merge_right < N) { // Merge only if the range is valid
        int merge_end = (merge_right + step * work_load < N)
                            ? merge_right + step * work_load
                            : N;
        merge(arr, left, merge_right - 1, merge_end - 1);
      }
    }
    step *= 2;
    pthread_barrier_wait(&barrier); // Sync at each merge step
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  int i, j;
  double start, end, seq_time;

  if (argc > 1)
    T = atoi(argv[1]);

  arr = (int *)malloc(sizeof(int) * N);

  for (i = 0; i < N; i++) {
    arr[i] = N - i;
  }

  start = dwalltime();
  mergesort(arr, 0, N - 1);
  seq_time = dwalltime() - start;
  print_performance(1, seq_time, seq_time);

  for (t = 2; t <= T; t *= 2) {
    pthread_t threads[t];
    int ids[t];

    for (i = 0; i < N; i++) {
      arr[i] = N - i;
    }

    start = dwalltime();
    pthread_barrier_init(&barrier, NULL, t);
    for (i = 0; i < t; i++) {
      ids[i] = i;
      pthread_create(&threads[i], NULL, &thread, &ids[i]);
    }

    for (i = 0; i < t; i++)
      pthread_join(threads[i], NULL);
    pthread_barrier_destroy(&barrier);
    end = dwalltime();

    print_performance(t, seq_time, end - start);
  }

  return 0;
}
