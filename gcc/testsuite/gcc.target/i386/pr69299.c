/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-Ofast -mno-avx -msse2 -mtune=bdver2" } */

float *a, *b;
int c, d, e, f;
void
foo (void)
{
  for (; c; c++)
    a[c] = 0;
  if (!d)
    for (; c < f; c++)
      b[c] = (double) e / b[c];
}

/* { dg-final { scan-assembler-not "cvtps2pd\[^\n\r\]*(%xmm|xmm)\[0-9\]+,\[^\n\r\]*(%xmm|xmm)\[0-9\]+" } } */
