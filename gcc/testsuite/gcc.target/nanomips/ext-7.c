/* No need to use ext if we can use andi.  */
/* { dg-do compile } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler "\tandi\t" } } */
/* { dg-final { scan-assembler-not "\td?ext\t" } } */

unsigned
f (unsigned i)
{
  return i & 0x7ff;
}
