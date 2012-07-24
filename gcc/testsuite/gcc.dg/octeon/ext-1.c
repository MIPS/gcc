/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "dext" } } */
/* { dg-final { scan-assembler-not "and" } } */

struct {
  unsigned long a:14;
  unsigned long d:30;
  unsigned long e:10;
  unsigned long f:10;
} t;

unsigned long long get5(void)
{
   return t.d;
}
