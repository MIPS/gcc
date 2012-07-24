/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "dext" } } */
/* { dg-final { scan-assembler-not "addiu" { xfail *-octeon-* } } } */
/* { dg-final { scan-assembler "dins" { xfail *-octeon-* } } } */
/* { dg-final { scan-assembler-not "sw" { xfail *-octeon-* } } } */
/* { dg-final { scan-assembler-not "ld" { xfail *-octeon-* } } } */

struct b
{
  int t;
  int tt;
};

union a
{
  long long t;
  struct b b;
};

struct b f(int i, int t)
{
  return (struct b){i, t};
}
