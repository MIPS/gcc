/* { dg-options "(-mips16) -mgp64" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* The sign extend to be able to store into d should be free,
   there should be no sll or move here. */
/* { dg-final { scan-assembler-not "\tsll\t" } } */
/* { dg-final { scan-assembler-not "move\t" } } */

long long d;
int test1(int fParm)
{
  d = fParm + 1;
  return fParm + 1;
}

