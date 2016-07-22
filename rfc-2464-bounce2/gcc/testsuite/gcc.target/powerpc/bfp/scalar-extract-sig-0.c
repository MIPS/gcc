/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power9" } } */
/* { dg-require-effective-target lp64 } */
/* { dg-require-effective-target powerpc_p9vector_ok } */
/* { dg-skip-if "" { powerpc*-*-aix* } } */
/* { dg-options "-mcpu=power9" } */

/* This test should succeed only 64-bit configuration.  */
#include <altivec.h>

unsigned long long int
get_significand (double *p)
{
  double source = *p;

  return scalar_extract_sig (source);
}

/* { dg-final { scan-assembler "xsxsigdp" } } */
