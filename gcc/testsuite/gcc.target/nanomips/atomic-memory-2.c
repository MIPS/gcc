/* { dg-do compile } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler "\taddiu|li\t" } } */
/* { dg-final { scan-assembler-not "\tsubu" } } */

unsigned long
f(unsigned long *p)
{
  return __sync_fetch_and_sub (p, 5);
}
