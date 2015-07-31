/* { dg-do run } */
/* { dg-xfail-run-if "TODO" { openacc_host_selected } { "*" } { "" } } */

#include <stdlib.h>

#define n 100

int
main()
{
  int a, b[n], i;

  a = 5;

  for (i = 0; i < n; i++)
    b[i] = -1;

  #pragma acc parallel num_gangs (n) firstprivate (a)
  #pragma acc loop gang
  for (i = 0; i < n; i++)
    {
      a = a + i;
      b[i] = a;
    }

  for (i = 0; i < n; i++)
    if (a + i != b[i])
      abort ();

  return 0;
}
