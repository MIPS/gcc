/* { dg-do compile { target *-octeon-elf* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "usd\t" 1 } } */
/* { dg-final { scan-assembler-times "uld\t" 1 } } */
/* { dg-final { scan-assembler-times "usw\t" 1 } } */
/* { dg-final { scan-assembler-times "ulw\t" 1 } } */
/* { dg-final { scan-assembler-not "nop" } } */

struct s
{
  char c;
  int i;
  long long l;
} __attribute__ ((packed)) s;

void
sd (long long l)
{
  s.l = l;
}

long long
ld ()
{
  return s.l;
}

void
sw (int i)
{
  s.i = i;
}

int
lw ()
{
  return s.i;
}
