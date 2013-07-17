/* { dg-do compile } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */

#include "mpx-check.h"

void *p;
int mpx_test (int argc, const char **argv)
{
  p = &&label;

 label:
  return 0;
}
