/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler {0x123} } } */

struct s
{
  long long a:4;
  long long b:4;
  long long c:4;
  long long d:52;
};

f ()
{
  struct s s;
  s.a = 1;
  s.b = 2;
  s.c = 3;
  g (s);
}
