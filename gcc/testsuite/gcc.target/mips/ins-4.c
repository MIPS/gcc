/* { dg-options "isa_rev>=2 -mgp64" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-times "ins\t" 2 } } */
/* { dg-final { scan-assembler-not "or\t" } } */
/* { dg-final { scan-assembler-not "cins\t" } } */

#define shift 0
#define mask (0xfffffull<<shift)

/* Check that simple ins are produced by manually doing
   bitfield insertations (no shifts in this case). */

NOMIPS16 int f(int a, int b)
{
  a = (a&~mask) | ((b<<shift)&mask);
  return a;
}

NOMIPS16 long long fll(long long a, long long b)
{
  a = (a&~mask) | ((b<<shift)&mask);
  return a;
}
