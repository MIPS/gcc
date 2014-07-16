/* { dg-options "-march=octeon2" }  */
int f(int a, int b)
{
  return a== 10 ? b : 0;
}
int f1(int a, int b)
{
  return b & -(a==10);
}

/* We should be able to convert f to f1 and not
   use movz/movn as those have a higher latency than just an neg
   followed by an and on Octeon2. */
/* { dg-final { scan-assembler-not "movz\t" } } */
/* { dg-final { scan-assembler-not "movn\t" } } */
/* { dg-final { scan-assembler "and\t" } } */
/* { dg-final { scan-assembler "subu\t" } } */
