/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -save-temps" } */

void
f (unsigned int *restrict a, unsigned char *restrict b, int n)
{
  for (int i = 0; i < n; ++i)
    a[i] += b[i];
}

/* We used to use a separate iv for "a" and use MUL VL loads and stores.
   At the moment we instead use separate invariant bases for each pair,
   which is OK as an alternative.  What we want to avoid is 4 loads and
   4 stores of simple [xN] addresses.  */
/* { dg-final { scan-assembler-times {\tld1w\tz[0-9]+\.s, p[0-7]/z, \[x[0-9]+, x[0-9]+, lsl 2\]\n} 4 } } */
/* { dg-final { scan-assembler-times {\tst1w\tz[0-9]+\.s, p[0-7], \[x[0-9]+, x[0-9]+, lsl 2\]\n} 4 } } */
