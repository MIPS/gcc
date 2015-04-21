/* Check that PLT isn't used to access glob_a.  */
/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fpic -fsymbolic" } */

int
__attribute__((weak))
glob_a (void)
{
  return -1;
}

int foo ()
{
  return glob_a ();
}

/* { dg-final { scan-assembler-not "glob_a@PLT" } } */
/* { dg-final { scan-assembler-times "movl\[ \t\]\\\$-1, %eax" 1 } } */
