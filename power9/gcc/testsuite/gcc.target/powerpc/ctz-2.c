/* { dg-do compile { target { powerpc*-*-* && i32 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_p9vector_ok } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power9" } } */
/* { dg-options "-mcpu=power9 -O3" } */

int i_trailing_zero (int a) { return __builtin_ctz (a); }

/* { dg-final { scan-assembler     "cnttzw " } } */
/* { dg-final { scan-assembler-not "cntlzw " } } */
