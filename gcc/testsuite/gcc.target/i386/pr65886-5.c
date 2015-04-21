/* Check that GOTPCREL/GOT isn't used to access glob_a.  */
/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fpic -fsymbolic" } */

int glob_a __attribute__((weak));

int foo ()
{
  if (&glob_a != 0)
    return glob_a;
  else
    return 0;
}

/* { dg-final { scan-assembler "glob_a\\(%rip\\)" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "glob_a@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "glob_a@GOTOFF\\(" { target ia32 } } } */
/* { dg-final { scan-assembler-not "glob_a@GOT\\(" { target ia32 } } } */
