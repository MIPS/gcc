/* { dg-do compile { target *-octeon-linux* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "sdl\t" 1 } } */
/* { dg-final { scan-assembler-times "sdr\t" 1 } } */
/* { dg-final { scan-assembler-times "ldl\t" 1 } } */
/* { dg-final { scan-assembler-times "ldr\t" 1 } } */
/* { dg-final { scan-assembler-times "swl\t" 1 } } */
/* { dg-final { scan-assembler-times "swr\t" 1 } } */
/* { dg-final { scan-assembler-times "lwl\t" 1 } } */
/* { dg-final { scan-assembler-times "lwr\t" 1 } } */
/* { dg-final { scan-assembler-not "nop" } } */

struct s
{
  char c;
  int i;
  long long l;
} __attribute__ ((packed)) s __attribute__((aligned(1) ));

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
