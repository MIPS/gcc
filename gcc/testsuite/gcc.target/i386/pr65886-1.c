/* Check that GOTPCREL/GOT is used to access glob_a.  */
/* { dg-do compile { target *-*-linux* } } */
/* { dg-require-effective-target pie } */
/* { dg-options "-O2 -fpie -fsymbolic" } */

extern int glob_a;

int foo ()
{
  if (&glob_a != 0)
    return glob_a;
  else
    return 0;
}

/* { dg-final { scan-assembler "glob_a@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "glob_a@GOT\\(" { target ia32 } } } */
