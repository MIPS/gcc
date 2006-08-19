/* { dg-do compile } */
/* { dg-require-effective-target arm32 } */
/* { dg-options "-O2 -mfpu=vfp -mfloat-abi=softfp" } */
/* { dg-skip-if "" { *-*-* } { "-mcpu=iwmmxt" "-march=iwmmxt" } { "" } } */

void
foo (double *p, double a, double b, int n)
{
  double c = a + b;
  do
    *--p = c;
  while (n--);
}

/* { dg-final { scan-assembler "fstmdbd" } } */
