#include "../utils/utils.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int t, T = 1;
unsigned long N = 1UL << 30;
int TARGET = 3;
int *arr;
int total = 0;
pthread_mutex_t mutex;

int count(int target, int *arr, int start, int end) {

  int ret = 0;

  for (int i = start; i < end; i++) {
    if (arr[i] == target)
      ret++;
  }

  return ret;
}

void *thread(void *args) {
  int id = *(int *)args;
  int work_load = N / t;
  int c = count(TARGET, arr, id * work_load, (id + 1) * work_load);
  pthread_mutex_lock(&mutex);
  total += c;
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
    arr[i] = rand() % 100;
  }

  start = dwalltime();
  check = count(TARGET, arr, 0, N);
  seq_time = dwalltime() - start;

  print_performance(1, seq_time, seq_time);

  for (t = 2; t <= T; t *= 2) {
    pthread_t threads[t];
    int ids[t];
    total = 0;

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

  if (check == total) {
    printf("La busqueda se hizo correctamente.");
  } else {
    printf("Hubo un error en la busqueda.");
  }
  return 0;
}
