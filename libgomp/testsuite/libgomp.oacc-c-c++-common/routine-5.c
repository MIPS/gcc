
/* { dg-do run } */
/* { dg-warning "TODO" "implicit" { xfail *-*-* } 17 } */
/* { dg-warning "TODO" "implicit" { xfail *-*-* } 27 } */
/* { dg-xfail-if "unresolved symbol" { *-*-* } } */

#include <stdlib.h>

#pragma acc routine bind (foo)
int
subr1 (int n)
{
  if (n == 0 || n == 1)
    return 1;

  return n * foo (n - 1);
}

#pragma acc routine bind ("bar")
int
subr2 (int n)
{
  if (n == 0 || n == 1)
    return 1;

  return n * bar (n - 1);
}

int
main()
{
  int *a, i, n = 10;

  a = (int *)malloc (sizeof (int) * n);

#pragma acc parallel copy (a[0:n]) vector_length (5)
  {
#pragma acc loop
    for (i = 0; i < n; i++)
      a[i] = foo (i);
  }

  for (i = 0; i < n; i++)
    if (a[i] != subr1 (i))
      abort ();

  for (i = 0; i < n; i++)
    a[i] = 0;

#pragma acc parallel copy (a[0:n]) vector_length (5)
  {
#pragma acc loop
    for (i = 0; i < n; i++)
      a[i] = bar (i);
  }

  for (i = 0; i < n; i++)
    if (a[i] != subr2 (i))
      abort ();

  free (a);

  return 0;
}
