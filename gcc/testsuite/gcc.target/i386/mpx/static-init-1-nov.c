/* { dg-do run } */
/* { dg-options "-fcheck-pointers -mmpx" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

#include "mpx-check.h"

int buf[100];
int *p = buf;

int mpx_test (int argc, const char *argv[])
{
  printf ("%d\n", p[0]);
  printf ("%d\n", p[99]);

  return 0;
}
