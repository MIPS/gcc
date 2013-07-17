/* { dg-do compile } */
/* { dg-options "-fmpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

#include "mpx-check.h"

#ifndef __MPX__
#error -fmpx is required
#endif

int mpx_test (int argc, const char **argv)
{
  return 0;
}
