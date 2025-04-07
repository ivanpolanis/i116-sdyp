// Ejercicio 2
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  double x, scale;
  int i;
  int numThreads = atoi(argv[2]);
  int N = atoi(argv[1]);
  omp_set_num_threads(numThreads);
  scale = 2.78;
  x = 0.0;

  // Instead of all threads fighting to write in x, they all will make the sum
  // in their own x, and finally will add up to original x sequentially.
#pragma omp parallel
  {
    double local_x = 0.0;
#pragma omp for
    for (i = 1; i <= N; i++) {
      local_x += sqrt(i * scale) + 2 * local_x;
    }

#pragma omp critical
    x += local_x;
  }
  printf("\n Resultado: %f \n", x);

  return (0);
}
