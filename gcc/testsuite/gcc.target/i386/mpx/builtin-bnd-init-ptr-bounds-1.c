/* { dg-do run } */

#include "mpx-check.h"

int buf[100];

int mpx_test (int argc, const char **argv)
{
  assert (buf + 10 == __bnd_init_ptr_bounds (buf + 10));
  return 0;
}
