/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */

/* { dg-final { scan-assembler-not "and\t\|andi\t\|ext\t\|sll\t\|srl\t" } } */
/* { dg-final { scan-assembler-times "seq\t" 1 } } */
/* { dg-final { scan-assembler-times "sne\t" 1 } } */

long long
f (int a, int b)
{
  return a == b;
}

long long
g (int a, int b)
{
  return a != b;
}
