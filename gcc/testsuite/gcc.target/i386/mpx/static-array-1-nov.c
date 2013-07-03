/* { dg-do run } */

#include "mpx-check.h"

int buf[100];

int mpx_test (int argc, const char **argv)
{
  buf[0] = argc;
  buf[99] = argc;
  return 0;
}
