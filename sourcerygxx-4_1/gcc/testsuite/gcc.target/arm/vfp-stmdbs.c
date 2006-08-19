/* { dg-do compile } */
/* { dg-require-effective-target arm32 } */
/* { dg-options "-O2 -mfpu=vfp -mfloat-abi=softfp" } */
/* { dg-skip-if "" { *-*-* } { "-mcpu=iwmmxt" "-march=iwmmxt" } { "" } } */

void
foo (float *p, float a, float b, int n)
{
  float c = a + b;
  do
    *--p = c;
  while (n--);
}

/* { dg-final { scan-assembler "fstmdbs" } } */
