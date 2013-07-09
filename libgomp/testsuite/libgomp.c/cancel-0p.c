#include <stdlib.h>

int main()
{
  #pragma omp parallel num_threads (32)
  {
    #pragma omp cancel parallel
    abort ();
  }
  return 0;
}
