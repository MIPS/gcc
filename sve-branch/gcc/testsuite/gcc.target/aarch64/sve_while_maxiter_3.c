/* { dg-do assemble } */
/* { dg-options "-O3 -march=armv8-a+sve --save-temps" } */

int
loop (short b)
{
  int c = 0;
l1:
  b++;
  c |= b;
  if (b < 32766)
    goto l1;
return c;
}

/* { dg-final { scan-assembler-not {\tmov\tx[0-9], 65536\n} } } */
/* { dg-final { scan-assembler-not {\tcmp\tx[0-9], 0\n} } } */
/* { dg-final { scan-assembler-not {\tcsel\tx[0-9], x[0-9], x[0-9], ne\n} } } */
