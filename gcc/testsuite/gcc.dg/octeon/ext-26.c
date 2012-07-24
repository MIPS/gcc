/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "uld\t" } } */

struct TRIAL
{
  char c;
  unsigned long long s;
} __attribute__ ((packed));

unsigned long long
check (struct TRIAL * t)
{
  return t->s;
}
