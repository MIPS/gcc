/******************************************************************************
* OpenMP Example - Parallel region with an orphaned directive - C/C++ Version
* FILE: omp_orphan.c
* DESCRIPTION:
*   This example demonstrates a dot product  being performed by an orphaned
*   loop reduction construct.  Scoping of the reduction variable is critical.
* SOURCE: Blaise Barney  5/99
* LAST REVISED:
******************************************************************************/

#include <omp.h>
#define VECLEN 100
float a[VECLEN], b[VECLEN], sum;

float dotprod ()
{
int i,tid;

tid = omp_get_thread_num();
#pragma omp for reduction(+:sum)
  for (i=0; i < VECLEN; i++)
    {
    sum = sum + (a[i]*b[i]);
    printf("  tid= %d i=%d\n",tid,i);
    }

return(sum);
}


main ()
{

int i;

for (i=0; i < VECLEN; i++)
  a[i] = b[i] = 1.0 * i;
sum = 0.0;

#pragma omp parallel
  sum = dotprod();

printf("Sum = %f\n",sum);

}

