/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-mcpu=power8" } */
/* { dg-shouldfail } */

#include <altivec.h>

int doTestBCDSignificance (_Decimal128 *p)
{
  _Decimal128 source = *p;

  /* { dg-error "error: Builtin function __builtin_dtstsfi_gt_td requires" } */
  return __builtin_dfp_dtstsfi_gt (5, source);
}


