/* { dg-do run { xfail *-*-* } } */

#define XFAIL

#include "mpx-check.h"

int buf[100];

int mpx_test (int argc, const char **argv)
{
  buf[-1] = argc;
  return 0;
}
