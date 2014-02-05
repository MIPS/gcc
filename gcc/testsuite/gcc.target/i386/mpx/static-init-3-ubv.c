/* { dg-do run { xfail *-*-* } } */
/* { dg-options "-fcheck-pointer-bounds -mmpx" } */


#define XFAIL

#include "mpx-check.h"

int buf[100];
int *p = buf;

int foo (int i)
{
  static int **pp = &p;

  return (*pp)[i];
}

int mpx_test (int argc, const char *argv[])
{
  printf ("%d\n", foo (100));

  return 0;
}
