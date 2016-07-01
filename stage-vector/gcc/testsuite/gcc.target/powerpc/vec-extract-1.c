/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_p8vector_ok } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power8" } } */
/* { dg-options "-mcpu=power8 -O2 -mupper-regs-df -mupper-regs-di" } */

#include <altivec.h>

double
get_double (vector double a, int n)
{
  return vec_extract (a, n);
}

long
get_long (vector long a, int n)
{
  return vec_extract (a, n);
}

/* { dg-final { scan-assembler     "vslo"    } } */
/* { dg-final { scan-assembler     "mtvsrd"  } } */
/* { dg-final { scan-assembler     "mfvsrd"  } } */
/* { dg-final { scan-assembler-not "stxvd2x" } } */
/* { dg-final { scan-assembler-not "stxvx"   } } */
/* { dg-final { scan-assembler-not "stxv"    } } */
/* { dg-final { scan-assembler-not "ldx"     } } */
