/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-mcpu=power9" } */

#include <altivec.h>

int doTestBCDSignificance (_Decimal128 *p)
{
  _Decimal128 source = *p;

  if (__builtin_dfp_dtstsfi_lt_td (63, source))
    return 3;
  else
    return 5;
}

/* { dg-final { scan-assembler	   "dtstsfiq" } } */

