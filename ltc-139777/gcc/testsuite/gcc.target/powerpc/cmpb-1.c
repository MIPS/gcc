/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-mcpu=power6" } */
/* { dg-require-effective-target lp64 } */

/* This test should succeed on only 64-bit configurations.  */
#include <altivec.h>

int int cmpb (int arg1, int arg2)
{
  return __builtin_cmpb (arg1, arg2);
}

/* { dg-final { scan-assembler	   "cmpb" } } */
