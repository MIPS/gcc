/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "ins\t" } } */
/* { dg-final { scan-assembler-not "\texts\t" } } */
/* { dg-final { scan-assembler-not "\tsll\t" } } */
/* { dg-final { scan-assembler-not "\tandi\t" } } */
struct x
{
  int a:1;
  int b:31;
  long long c:32;
};

struct x f(int a, struct x b)
{
   a = a>>17;
   a^=1;
   int c = a;
   b.a = c;
   return b;
}

