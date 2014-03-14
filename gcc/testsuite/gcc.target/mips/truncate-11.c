/* { dg-options "-mgp64" } */
/* We should only have one shift here. */
/* { dg-final { scan-assembler-times "sll\t" 1 } } */

int f(long long b, long long a)
{
  long long aa = b <<(a&(32-1));
  return aa;
}

