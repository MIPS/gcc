
/* { dg-do run } */

#include <stdlib.h>

#pragma acc routine nohost
int
foo (int n)
{
  if (n == 0 || n == 1)
    return 1;

  return n * n;
}

int
main()
{
  int a, n = 10;

#pragma acc parallel copy (a, n)
  {
    a = foo (n);
  }

  if (a != n * n)
    abort ();

  return 0;
}

/* { dg-output "foo not found" { target openacc_host_selected } } */
