/* Only run this test on big-endian powerpc, because only big-endian
 * supports a -m32 command-line option */
/* { dg-do compile { target { powerpc-*-* } } } */
/* { dg-options "-mcpu=power6 -m32" } */

/* This test should result in a compile-time error because the
 * __builtin_cmpb () function is not supported in 32-bit mode.
 */
#include <altivec.h>

int int cmpb (int arg1, int arg2)
{

  /* { dg-error "replace this string with regexp that matches error message" } */
  return __builtin_cmpb (arg1, arg2);
}

/* { dg-final { scan-assembler	   "cmpb" } } */
