#include <stdlib.h>

int main()
{
  #pragma omp parallel num_threads (32)
  {
    int i;
    #pragma omp for
    for (i = 0; i < 1000; ++i)
      {
        #pragma omp cancel for
        abort();
      }
  }
  return 0;
}
