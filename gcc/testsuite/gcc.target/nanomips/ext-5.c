/* For MIPS32r2 use EXT when ANDing with low-order bitmasks.  */
/* { dg-do compile } */
/* { dg-options "-march=32r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler "\text\t" } } */
/* { dg-final { scan-assembler-not "\tandi?\t" } } */

unsigned
f (unsigned i)
{
  return i & 0x7ffffff;
}
