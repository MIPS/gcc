/* Check that GOTPCREL/GOT isn't used to access glob_a.  */
/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fpic -fsymbolic" } */

extern __attribute__((visibility("protected"))) int glob_a;

int foo ()
{
  return glob_a;
}

/* { dg-final { scan-assembler "glob_a\\(%rip\\)" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "glob_a@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "glob_a@GOTOFF\\(" { target ia32 } } } */
/* { dg-final { scan-assembler-not "glob_a@GOT\\(" { target ia32 } } } */
