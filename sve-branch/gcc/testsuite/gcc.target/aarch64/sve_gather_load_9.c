/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve" } */

void
f (int *__restrict a,
   int *__restrict b,
   int *__restrict c,
   int count)
{
  for (int i = 0; i < count; ++i)
    a[i] = (b[i * 4] + b[i * 4 + 1] + b[i * 4 + 3]
	    + c[i * 5] + c[i * 5 + 3]);
}

/* There's no need for a scalar tail here.  */
/* { dg-final { scan-assembler-times {\tld4w\t{z[0-9]+.*}} 1 } } */
/* { dg-final { scan-assembler-not {\tldr\t} } } */
/* { dg-final { scan-assembler-not {\tstr\t} } } */
