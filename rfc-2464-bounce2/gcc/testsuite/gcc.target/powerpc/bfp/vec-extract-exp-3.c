/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power8" } } */
/* { dg-require-effective-target powerpc_p9vector_ok } */
/* { dg-skip-if "" { powerpc*-*-aix* } } */
/* { dg-options "-mcpu=power8" } */

#include <altivec.h>

__vector int
get_exponents (__vector float *p)
{
  __vector float source = *p;

  return vec_extract_exp (source);	/* { dg-error "Builtin function __builtin_vsx_vector_extract_expy requires" } */
}
