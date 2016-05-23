/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-mcpu=power5" } */

/* This test should result in a compile-time warning because the
 * __builtin_cmpb () function is not supported when -mcpu=power5
 */
#include <altivec.h>

int int cmpb (int arg1, int arg2)
{
  /* { dg-warning "implicit declaration of function" } */
  return __builtin_cmpb_32 (arg1, arg2);
}

