/* { dg-do run } */

#include "libgomp_g.h"

extern void abort ();

volatile int i;

void
f (void *data)
{
  if (i != -1)
    abort ();
  i = 42;
}

int main(void)
{
  i = -1;
  GOACC_parallel (0, f, (const void *) 0, 0, (void *) 0, (void *) 0, (void *) 0);
  if (i != 42)
    abort ();

  return 0;
}
