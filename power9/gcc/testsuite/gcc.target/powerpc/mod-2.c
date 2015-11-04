/* { dg-do compile { target { powerpc*-*-* && i32 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_p9vector_ok } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power9" } } */
/* { dg-options "-mcpu=power9 -O3" } */

int ismod (int a, int b) { return a%b; }
unsigned int iumod (unsigned int a, unsigned int b) { return a%b; }

/* { dg-final { scan-assembler-times "modsw " 1 } } */
/* { dg-final { scan-assembler-times "moduw " 1 } } */
/* { dg-final { scan-assembler-not   "mullw "   } } */
/* { dg-final { scan-assembler-not   "divw "    } } */
/* { dg-final { scan-assembler-not   "divwu "   } } */
