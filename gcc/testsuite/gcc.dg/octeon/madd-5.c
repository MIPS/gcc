/* We should be able to produce madd inside the loop and only at the
   end of the loop get the result from the accumulator. */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "\tmul\t" } } */
/* { dg-final { scan-assembler "\tmadd\t" } } */

int
f1 (int *a, int *b, int n)
{
  int x, i;

  x = 0;
  for (i = 0; i < n; i++)
    x += a[i] * b[i];
  return x;
}

