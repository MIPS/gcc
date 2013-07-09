#include <stdlib.h>

int main()
{
  #pragma omp parallel num_threads (32)
  {
    #pragma omp sections
      {
	{
          #pragma omp cancel sections
          abort();
	}
      #pragma omp section
	{
          #pragma omp cancel sections
          abort();
	}
      #pragma omp section
	{
          #pragma omp cancel sections
          abort();
	}
      #pragma omp section
	{
          #pragma omp cancel sections
          abort();
	}
      }
  }
  return 0;
}
