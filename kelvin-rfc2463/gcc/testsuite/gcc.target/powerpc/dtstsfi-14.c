/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-mcpu=power9" } */
/* { dg-shouldfail } */

#include <altivec.h>

int doTestBCDSignificance (_Decimal64 *p, unsigned int significance)
{
  _Decimal64 source = *p;

  /* { dg-error "error: argument 1 must be a 6-bit unsigned literal" } */
  return __builtin_dfp_dtstsfi_lt_dd (significance, source);
}

