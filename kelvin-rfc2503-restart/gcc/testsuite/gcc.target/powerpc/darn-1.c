/* { dg-do compile { target { powerpc64*-*-* } } } */
/* { dg-options "-mcpu=power9" } */
/* { dg-require-effective-target llp64 } */

#include <altivec.h>

long long get_conditioned_random() 
{
  return __builtin_darn_conditioned ();
}

/* { dg-final { scan-assembler	   "darn" } } */
