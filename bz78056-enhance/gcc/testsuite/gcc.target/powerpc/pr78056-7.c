/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power5" } } */
/* { dg-require-effective-target powerpc_p6vector_ok } */
/* { dg-skip-if "" { powerpc*-*-aix* } } */
/* { dg-options "-mcpu=power5" } */

/* This test should succeed on both 32- and 64-bit configurations.  */
#include <altivec.h>

/* Though the command line specifies power5 target, this function is
   to support power6.  */
__attribute__((target("cpu=power6")))
/* fabs/fnabs/fsel */
double normal1 (double a, double b)
{
  return __builtin_copysign (a, b);
}
/* { dg-final { scan-assembler-times "fabs" 1 } } */

