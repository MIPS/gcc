/* { dg-options "isa_rev>=2 -mgp64" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-times "dins" 1 } } */
/* { dg-final { scan-assembler "dsll" } } */
/* { dg-final { scan-assembler-not "move" } } */

union u
{
  struct
  {
    long long a:2;
    long long b:2;
    long long c:60;
  } s;
  long long l;
};

f (int a, int b)
{
  union u u;
  u.l = 0;
  u.s.a = a;
  u.l |= 0xfff00000;
  g (u);
}
