/* { dg-do run } */

#include <stdlib.h>

int
main (int argc, char **argv)
{
  float a, b;

  a = 2.0;
  b = 0.0;

#pragma acc kernels default (none)
  {
    float c, d;

    c = 2.0;
    d = c;
    c = 1.0;
    c = c + d;
  }

#pragma acc parallel default (none)
  {
    float c, d;

    c = 2.0;
    d = c;
    c = 1.0;
    c = c + d;
  }

#pragma acc parallel copy (a) create (b) default (none)
  {
    b = a;
    a = 1.0;
    a = a + b;
  }

  if (a != 3.0)
    abort ();

#pragma acc kernels copy (a) create (b) default (none)
  {
    b = a;
    a = 1.0;
    a = a + b;
  }

  if (a != 4.0)
    abort ();

#pragma acc parallel default (none) copy (a) create (b)
  {
    b = a;
    a = 1.0;
    a = a + b;
  }

  if (a != 5.0)
    abort ();

  return 0;
}
