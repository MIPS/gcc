/* { dg-do compile { target { powerpc64*-*-* } } } */
/* { dg-options "-mcpu=power9" } */
/* { dg-require-effective-target llp64 } */


#include <altivec.h>

int get_random() 
{
  return __builtin_darn_32 ();
}

/* { dg-final { scan-assembler	   "darn" } } */
