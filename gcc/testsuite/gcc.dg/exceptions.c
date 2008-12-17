/* { dg-do compile } */
/* { dg-options "-fexceptions" } */
/* { dg-final { scan-assembler ".cfi_personality" } } */

int f(void)
{
  return 42;
}
