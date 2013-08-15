/* { dg-do compile } */
/* { dg-options "-fcheck-pointers -mmpx" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

#include "mpx-check.h"

#ifndef __MPX__
#error -mmpx is required
#endif

#ifndef __CHKP__
#error -fcheck-pointers is required
#endif

int mpx_test (int argc, const char **argv)
{
  return 0;
}
