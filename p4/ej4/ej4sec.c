#include "../../utils/utils.h"
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

  double start, end;
  if (argc < 2)
    return EXIT_FAILURE;
  int N = atoi(argv[1]);

  start = dwalltime();
  srand(time(NULL));

  int *A;
  A = (int *)malloc(sizeof(int) * N);
  for (int i = 0; i < N; i++) {
    A[i] = rand() % 100;
  }

  mergesort(A, 0, N - 1);

  free(A);

  end = dwalltime();

  printf("%f", end - start);

  return EXIT_SUCCESS;
}
