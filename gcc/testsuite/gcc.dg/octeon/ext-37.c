/* Based on ext-1.c.  */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "dext" } } */
/* { dg-final { scan-assembler-not "and" } } */

struct {
  unsigned long a:14;
  unsigned long long d:33;
  unsigned long e:10;
  unsigned long f:10;
} t;
                                                                                  
unsigned long long get5(void)
{
   return t.d;
}
