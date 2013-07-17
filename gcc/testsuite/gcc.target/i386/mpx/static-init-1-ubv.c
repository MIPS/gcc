/* { dg-do run { xfail *-*-* } } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */

#define XFAIL

#include "mpx-check.h"

int buf[100];
int *p = buf;

int mpx_test (int argc, const char *argv[])
{
  printf ("%d\n", p[100]);

  return 0;
}
