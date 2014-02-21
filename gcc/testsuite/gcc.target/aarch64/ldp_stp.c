/* { dg-do compile } */
/* { dg-options "-O2" } */

void
f (long long *ptr, long long a)
{
  ptr[0] = a;
  ptr[1] = a;
}

void
f1 (long long *ptr, long long a, long long b)
{
  ptr[0] = a;
  ptr[1] = b;
}

long long
f2 (long long *ptr, long long a, long long b)
{
  a = ptr[0];
  b = ptr[1];
  return a + b;
}

/* { dg-final { scan-assembler-times "ldp\\t" 1 } } */
/* { dg-final { scan-assembler-times "stp\\t" 2 } } */
