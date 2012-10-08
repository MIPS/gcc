/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "andi" 1 } } */
/* { dg-final { scan-assembler-not "\tsll\t" } } */

struct a
{
  char a, b;
  short c;
};

static struct a x = { 1, 2, ~1 }, y = {65, 2, ~2};
int
a1 ()
{
  return (x.a == (y.a & ~64) && x.b == y.b);
}
