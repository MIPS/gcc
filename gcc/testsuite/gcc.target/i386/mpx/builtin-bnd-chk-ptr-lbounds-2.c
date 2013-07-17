/* { dg-do run } */
/* { dg-options "-fno-mpx" } */
/* { dg-additional-options "-L/opt/intel/lib -lpl-runtime64" } */

#include "mpx-check.h"

int buf[100];

int mpx_test (int argc, const char **argv)
{
  __bnd_chk_ptr_lbounds (buf - 1);
  return 0;
}
