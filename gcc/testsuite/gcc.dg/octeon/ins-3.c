/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "usd\t" { target mips*-octeon-elf } } } */
/* { dg-final { scan-assembler "sdl\t" { target mips*-octeon-linux-gnu } } } */
/* { dg-final { scan-assembler "sdr\t" { target mips*-octeon-linux-gnu} } } */

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
