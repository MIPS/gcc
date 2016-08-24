/* { dg-do compile { target { powerpc64*-*-* && lp64 } } } */
/* { dg-skip-if "do not override -mcpu" { powerpc*-*-* } { "-mcpu=*" } { "-mcpu=power8" } } */
/* { dg-require-effective-target powerpc_p8vector_ok } */
/* { dg-options "-mcpu=power8 -O2 -mupper-regs-di" } */

void
splat_int_to_mem (vector int *p, int a)
{
  *p = (vector int) { a, a, a, a };
}

void
splat_short_to_mem (vector short *p, short a)
{
  *p = (vector short) { a, a, a, a, a, a, a, a };
}

void
splat_char_to_mem (vector char *p, char a)
{
  *p = (vector char) { a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a };
}

/* { dg-final { scan-assembler-not "lvewx"   } } */
/* { dg-final { scan-assembler-not "lvehx"   } } */
/* { dg-final { scan-assembler-not "lvebx"   } } */
/* { dg-final { scan-assembler-not "stxvw4x" } } */
