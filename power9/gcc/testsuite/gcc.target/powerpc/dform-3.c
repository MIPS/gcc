/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-require-effective-target powerpc_p9vector_ok } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power9" } } */
/* { dg-options "-mcpu=power9 -mpower9-dform -O2" } */

void foo (vector double *p) { p[1] = p[2] + p[3]; }
static vector double x[4];
vector double *p_x = x;
void bar (void) { x[1] = x[2] + x[3]; }

/* { dg-final { scan-assembler     "lxv "     } } */
/* { dg-final { scan-assembler     "stxv "    } } */
/* { dg-final { scan-assembler-not "lxvd2x "  } } */
/* { dg-final { scan-assembler-not "lxvw4x "  } } */
/* { dg-final { scan-assembler-not "stxvd2x " } } */
/* { dg-final { scan-assembler-not "stxvw4x " } } */
/* { dg-final { scan-assembler-not "mfvsrd "  } } */
/* { dg-final { scan-assembler-not "mtvsrd "  } } */
