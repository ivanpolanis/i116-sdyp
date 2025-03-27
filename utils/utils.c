#include <stdio.h>

#include <stdlib.h>
#include <sys/time.h>
/**********Para calcular tiempo*************************************/
double dwalltime() {
  double sec;
  struct timeval tv;

  gettimeofday(&tv, NULL);
  sec = tv.tv_sec + tv.tv_usec / 1000000.0;
  return sec;
}
/****************************************************************/

void print_performance(int threads, double seq_time, double par_time) {
  double speedup = seq_time / par_time;
  double efficiency = speedup / threads;
  printf("Threads: %d - Speedup: %f - Efficiency: %f - Execution time: %f.\n",
         threads, speedup, efficiency, par_time);
}
