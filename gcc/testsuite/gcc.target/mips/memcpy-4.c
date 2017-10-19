/* { dg-do compile } */
/* { dg-options "isa=interaptiv-mr2 -mno-memcpy -mno-use-copyw-ucopyw (-mips16)" } */
/* { dg-skip-if "code quality test" { *-*-* } { "" } { "" } } */

char * ref = "123456789012";

__attribute__((mips16))
void
f1 (int *p)
{
  __builtin_memcpy (p, ref, 12);
}

/* { dg-final { scan-assembler-not "\tucopyw\t" } } */
