/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "uld\t" { target mips*-*-elf } } } */
/* { dg-final { scan-assembler "ldr\t" { target mips*-*-linux-gnu } } } */
/* { dg-final { scan-assembler "ldl\t" { target mips*-*-linux-gnu } } } */

struct TRIAL
{
  char c;
  long long s;
} __attribute__ ((packed));

long long
check (struct TRIAL * t)
{
  return t->s;
}
