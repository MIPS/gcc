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


/* { dg-final { scan-assembler-times {\tmov\tx[0-9], 65536\n} 1 } } */
/* { dg-final { scan-assembler-times {\tands\tx[0-9], x[0-9], 65535\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcsel\tx[0-9], x[0-9], x[0-9], ne\n} 1 } } */
