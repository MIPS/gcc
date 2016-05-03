/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-mcpu=power8" } */
/* { dg-shouldfail } */

#include <altivec.h>

int doTestBCDSignificance (_Decimal64 *p)
{
  _Decimal64 source = *p;

  /* { dg-error "error: Builtin function __builtin_dtstsfi_ov_dd requires" } */
  return __builtin_dfp_dtstsfi_ov_dd (5, source);
}

