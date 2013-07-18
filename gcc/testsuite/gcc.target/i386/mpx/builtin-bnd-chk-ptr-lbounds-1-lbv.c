/* { dg-do run { xfail *-*-* } } */
/* { dg-options "-fmpx" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */

#define XFAIL

#include "mpx-check.h"

int buf[100];

int mpx_test (int argc, const char **argv)
{
  __bnd_chk_ptr_lbounds (buf - 1);
  return 0;
}
