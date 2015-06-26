/* Test if the .reloc directive is not emitted for indirect functions.  */
/* { dg-options "-mrelax-pic-calls -fPIC" } */
/* { dg-require-ifunc "" } */
/* { dg-do compile } */
/* { dg-final { scan-assembler-not "\\.reloc\t1f,R_MIPS_JALR,foo\n1" } } */
/* { dg-final { scan-assembler "\\.reloc\t1f,R_MIPS_JALR,foo_relax\n1" } } */

int foo_generic (void) { return 0; }
extern void foo_relax (void);

void *resolver ()
{
  return foo_generic;
}

int foo() __attribute__((ifunc("resolver")));

int main (int *argc, char *argv)
{
  foo_relax();
  return foo() != 0;
}
