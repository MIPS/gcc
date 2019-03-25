/* { dg-do compile } */
/* { dg-options "-m32 -msoft-float" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-times "\trestore\\.jrc\t32" 1 } } */
/* { dg-final { scan-assembler-not "\\.reloc" } } */

int foo (int a, ...)
{
	return a;
}
