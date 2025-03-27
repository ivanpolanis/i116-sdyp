#include "../utils/utils.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int t, T = 1;
unsigned long N = 1UL << 25;
int *arr;
float prom = 0;
pthread_mutex_t mutex;

float get_sum(int *arr, int start, int end) {

  float ret = 0;

  for (int i = start; i < end; i++) {
    ret += arr[i];
  }

  return ret;
}

void *thread(void *args) {
  int id = *(int *)args;
  int work_load = N / t;
  float sum = get_sum(arr, id * work_load, (id + 1) * work_load) / (float)N;
  pthread_mutex_lock(&mutex);
  prom += sum;
  pthread_mutex_unlock(&mutex);

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
    arr[i] = rand() % 10000;
  }

  start = dwalltime();
  prom = get_sum(arr, 0, N) / (float)N;
  seq_time = dwalltime() - start;

  print_performance(1, seq_time, seq_time);

  for (t = 2; t <= T; t *= 2) {
    pthread_t threads[t];
    int ids[t];
    prom = 0;

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

  printf("Prom: %f\n", prom);

  free(arr);
  return 0;
}
