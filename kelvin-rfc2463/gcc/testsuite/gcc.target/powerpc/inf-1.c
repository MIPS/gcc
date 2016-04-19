/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_p8vector_ok } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power8" } } */
/* { dg-options "-mcpu=power8 -O2 -mfloat128" } */

int isinf_float (float a) { return __builtin_isinf (a); }
int isinf_double (double a) { return __builtin_isinf (a); }
int isinf_float128 (__float128 a) { return __builtin_isinf (a); }
int isinf_ldouble (long double a) { return __builtin_isinf (a); }
int isinf_ibm128 (__ibm128 a) { return __builtin_isinf (a); }

/* { dg-final { scan-assembler     "sldi"       } } */
/* { dg-final { scan-assembler     "srdi"       } } */
/* { dg-final { scan-assembler     "cntlzd"     } } */
/* { dg-final { scan-assembler     "mfvsrd"     } } */
/* { dg-final { scan-assembler-not "fabs"       } } */
/* { dg-final { scan-assembler-not "fcmpu"      } } */
/* { dg-final { scan-assembler-not "mfcr"       } } */
/* { dg-final { scan-assembler-not "bl __gekf2" } } */

