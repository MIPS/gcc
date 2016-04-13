/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-mcpu=power9" } */

/* This test should succeed on both 32- and 64-bit configurations.  */
#include <altivec.h>

__vector unsigned char 
doAbsoluteDifferenceUnsignedCharMacro (__vector unsigned char *p, 
				       __vector unsigned char *q)
{
  __vector unsigned char result, source_1, source_2;

  source_1 = *p;
  source_2 = *q;

  result = vec_adu (source_1, source_2);
  return result;
}

/* { dg-final { scan-assembler	   "vadsdub" } } */
