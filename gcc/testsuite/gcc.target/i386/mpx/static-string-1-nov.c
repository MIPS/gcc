/* { dg-do run } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

#include "mpx-check.h"

char *str="dddd";

int foo1 (char** ptr)
{
  *ptr=str;
}

int foo (char *ptr)
{
  char *tmp = 0;
  foo1 (&ptr);
  tmp = ptr;
  return tmp[0] + tmp[4];
}

int mpx_test (int argc, const char **argv)
{
  printf ("%d\n", foo ("ddddd"));
  return 0;
}
