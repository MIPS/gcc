/* { dg-do assemble } */
/* { dg-options "-O3 -march=armv8-a+sve --save-temps" } */

int
loop (short b)
{
  int c = 0;
l1:
  b++;
  c |= b;
  if (b)
    goto l1;
  return c;
}

/* { dg-final { scan-assembler-times {\tadd\tx[0-9], x[0-9], 1\n} 1 } } */
