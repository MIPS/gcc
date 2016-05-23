/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-mcpu=power6" } */

/* This test should succeed on both 32- and 64-bit configurations.  */
#include <altivec.h>

int int cmpb_32 (int arg1, int arg2)
{
  return __builtin_cmpb_32 (arg1, arg2);
}

/* { dg-final { scan-assembler	   "cmpb" } } */
