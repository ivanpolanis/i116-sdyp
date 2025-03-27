#include "../utils/utils.h"
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int t, T = 1;
unsigned long N = 1UL << 25;
int *arr;
int min = INT_MAX, max = INT_MIN;
pthread_mutex_t mutex;

void get_minimax(int *min, int *max, int *arr, int start, int end) {

  for (int i = start; i < end; i++) {
    if (arr[i] > *max)
      *max = arr[i];

    if (arr[i] < *min)
      *min = arr[i];
  }
}

void *thread(void *args) {
  int id = *(int *)args;
  int work_load = N / t;
  int local_min = INT_MAX, local_max = INT_MIN;
  get_minimax(&local_min, &local_max, arr, id * work_load,
              (id + 1) * work_load);
  pthread_mutex_lock(&mutex);
  if (local_max > max)
    max = local_max;

  if (local_min < min)
    min = local_min;
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  int i, check;
  double start, end, seq_time;
  if (argc > 1)
    T = atoi(argv[1]);

  srand(time(NULL));

  arr = (int *)malloc(sizeof(int) * N);
  for (i = 0; i < N; i++) {
    arr[i] = rand();
  }

  start = dwalltime();
  get_minimax(&min, &max, arr, 0, N);
  seq_time = dwalltime() - start;

  print_performance(1, seq_time, seq_time);

  printf("MIN: %d - MAX: %d\n", min, max);
  for (t = 2; t <= T; t *= 2) {
    pthread_t threads[t];
    int ids[t];
    min = INT_MAX, max = INT_MIN;

    start = dwalltime();
    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < t; i++) {
      ids[i] = i;
      pthread_create(&threads[i], NULL, &thread, &ids[i]);
    }

    for (i = 0; i < t; i++) {
      pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    end = dwalltime();
    print_performance(t, seq_time, end - start);
  }

  printf("MIN: %d - MAX: %d\n", min, max);

  free(arr);
  return 0;
}
