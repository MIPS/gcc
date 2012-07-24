/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "ins\t" } } */
/* { dg-final { scan-assembler-not "\tsll\t" } } */

struct s
{
  int a:2;
  int b:21;
};

struct s
f (struct s s, int i)
{
  s.b = i;
  return s;
}
