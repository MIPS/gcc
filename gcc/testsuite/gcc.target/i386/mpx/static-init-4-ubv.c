/* { dg-do run { xfail *-*-* } } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

#define XFAIL

#include "mpx-check.h"

int buf[100];
int *p[1] = { buf };

int mpx_test (int argc, const char *argv[])
{
  printf ("%d\n", p[0][100]);

  return 0;
}
