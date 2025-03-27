#include "../utils/utils.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum { ASCENDING = 1, DESCENDING = -1, UNORDERED = 0 } order_t;

int t, T = 1;
unsigned long N = 1UL << 30;
int *arr;
int is_monotonic = 1; // 0 If its not.
order_t order;
pthread_mutex_t mutex;

order_t get_monotony(int *arr, int start, int end) {

  if (end - start < 2)
    return ASCENDING;

  int asc = 1, desc = 1;

  for (int i = start; i < end - 1; i++) {
    if (arr[i] < arr[i + 1]) {
      desc = 0;
    } else if (arr[i] > arr[i + 1]) {
      asc = 0;
    }

    if (!asc && !desc) {
      return UNORDERED;
    }
  }

  return asc ? ASCENDING : DESCENDING;
}

void *thread(void *args) {
  int id = *(int *)args;
  int work_load = N / t;
  int start = id * work_load,
      end = (id + 1 == t) ? N : (id + 1) * work_load + 1;
  order_t monotony = get_monotony(arr, start, end);

  if (is_monotonic && monotony != order) {
    pthread_mutex_lock(&mutex);
    is_monotonic = 0;
    pthread_mutex_unlock(&mutex);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  int i;
  double start, end, seq_time;
  if (argc > 1)
    T = atoi(argv[1]);

  srand(time(NULL));

  arr = (int *)malloc(sizeof(int) * N);
  for (i = 0; i < N; i++) {
    arr[i] = i;
  }
  arr[N / 2] = -1;

  if (arr[0] > arr[N - 1])
    order = DESCENDING;
  else
    order = ASCENDING;

  start = dwalltime();
  is_monotonic = (order == get_monotony(arr, 0, N - 1));
  seq_time = dwalltime() - start;

  print_performance(1, seq_time, seq_time);

  for (t = 2; t <= T; t *= 2) {
    pthread_t threads[t];
    int ids[t];

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

  free(arr);
  return 0;
}
