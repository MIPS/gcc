/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "usd\t" } } */

struct TRIAL
{
  char c;
  unsigned long long s;
} __attribute__ ((packed));

void
check (struct TRIAL * t, unsigned long long l)
{
  t->s = l;
}
