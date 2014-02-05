/* { dg-do run { xfail *-*-* } } */
/* { dg-options "-fcheck-pointer-bounds -mmpx" } */


#define XFAIL

#include "mpx-check.h"

int buf[100];

int mpx_test (int argc, const char **argv)
{
  printf("%d\n", buf[-1]);
  return 0;
}
