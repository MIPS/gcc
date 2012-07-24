/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler {0x6789} } } */
/* { dg-final { scan-assembler-not {\$0} } } */

union u
{
  unsigned long long l;

  struct
  {
    unsigned long long a: 48;
    unsigned long long b: 2;
    unsigned long long c: 2;
    unsigned long long d: 4;
    unsigned long long e: 4;
    unsigned long long f: 4;
  } s;
};

f (int i)
{
  union u u;

  u.l = 0;
  u.s.f = 9;
  u.s.d = 7;
  u.s.b = 6 >> 2;
  u.s.e = 8;
  u.s.c = 6 & 0x3;
  u.s.a = i;
  g (u);
}
