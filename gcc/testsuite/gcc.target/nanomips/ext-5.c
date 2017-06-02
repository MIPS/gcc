/* For MIPS32r2 use EXT when ANDing with low-order bitmasks.  */
/* { dg-do compile } */
/* { dg-options "isa_rev>=2 -mxlp" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler "\text\t" } } */
/* { dg-final { scan-assembler-not "\tandi?\t" } } */

unsigned
f (unsigned i)
{
  return i & 0x7ffffff;
}
