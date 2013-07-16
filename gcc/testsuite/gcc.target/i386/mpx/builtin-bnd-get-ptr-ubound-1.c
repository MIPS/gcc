/* { dg-do run } */

#include "mpx-check.h"

char buf[100];

int mpx_test (int argc, const char **argv)
{
  assert (buf + 99 == __bnd_get_ptr_ubound (buf));
  return 0;
}
