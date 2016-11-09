/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power6" } } */
/* { dg-require-effective-target powerpc_p7vector_ok } */
/* { dg-skip-if "" { powerpc*-*-aix* } } */
/* { dg-options "-mcpu=power6" } */

/* This test should succeed on both 32- and 64-bit configurations.  */
#include <altivec.h>

/* Though the command line specifies power7 target, this function is
   to support power7.  */
__attribute__((target("cpu=power7")))
int
div_we (int a, int b)
{
  return __builtin_divwe (a, b);
}

/* { dg-final { scan-assembler-times "divwe "   1 } } */



