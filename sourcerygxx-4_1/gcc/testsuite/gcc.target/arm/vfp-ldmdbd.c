/* { dg-do compile } */
/* { dg-require-effective-target arm32 } */
/* { dg-options "-O2 -mfpu=vfp -mfloat-abi=softfp" } */
/* { dg-skip-if "" { *-*-* } { "-mcpu=iwmmxt" "-march=iwmmxt" } { "" } } */

extern void bar (double);

void
foo (double *p, double a, int n)
{
  do
    bar (*--p + a);
  while (n--);
}

/* { dg-final { scan-assembler "fldmdbd" } } */
