/* { dg-do compile } */
/* { dg-options " -O2 " } */

int foo_c (int a, int b)
{
  if (a > 9 && b < 34)
    return 4;
  else
    return 26;
}

/* { dg-final { scan-assembler-times "ccmp" 1 } } */
/* { dg-final { scan-assembler-times "cset" 0 } } */
