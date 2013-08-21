/* { dg-do compile } */
/* { dg-options "-fcheck-pointers -mmpx" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

#include "mpx-check.h"

void *p;
int mpx_test (int argc, const char **argv)
{
  p = &&label;

 label:
  return 0;
}
