/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-require-effective-target powerpc_vsx_ok } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power7" } } */
/* { dg-options "-O2 -mcpu=power7" } */

#ifndef TYPE
#ifdef __LONG_DOUBLE_IEEE128__
#define TYPE __ibm128
#else
#define TYPE long double
#endif
#endif

TYPE d_to_ld1 (double x) { return (TYPE)x; }
TYPE d_to_ld2 (double *p) { return (TYPE)*p; }

/* { dg-final { scan-assembler-times "xxlxor" 2 } } */
