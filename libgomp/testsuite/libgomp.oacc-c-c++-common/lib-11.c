/* { dg-do run } */

#include <stdlib.h>
#include <openacc.h>
#include <stdint.h>

int
main (int argc, char **argv)
{
  const int N = 512;
  void *d;

  d = acc_malloc (N);
  if (d == NULL)
    abort ();

  acc_free ((void *)((uintptr_t) d + (uintptr_t) (N >> 1)));

  return 0;
}

/* { dg-shouldfail "libgomp: mem free failed 1" } */
