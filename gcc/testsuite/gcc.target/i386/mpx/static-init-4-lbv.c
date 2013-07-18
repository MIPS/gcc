/* { dg-do run { xfail *-*-* } } */
/* { dg-options "-fmpx" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

#define XFAIL

#include "mpx-check.h"

int buf[100];
int *p[1] = { buf };

int mpx_test (int argc, const char *argv[])
{
  printf ("%d\n", p[0][-1]);

  return 0;
}
