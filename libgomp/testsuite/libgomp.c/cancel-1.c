#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

static void fn1 (int *x)
{
  #pragma omp parallel firstprivate(x) num_threads (32)
  {
    int thr = omp_get_thread_num ();
    switch (x[thr])
      {
      case 3:
        #pragma omp cancel parallel
        break;
      case 2:
        usleep(1000);
        /* FALLTHRU */
      case 1:
        #pragma omp cancellation point parallel
        break;
      }
    #pragma omp barrier
    abort ();
  }
}

int main()
{
  int x[32] = { 0, 1, 2, 3, 2, 2, 1, 0 };
  fn1(x);
  return 0;
}
