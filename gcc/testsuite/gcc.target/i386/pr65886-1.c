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

/* weak glob_a should be accessed with a GOTPCREL.  */
/* { dg-final { scan-assembler "glob_a@GOTPCREL" { target { ! ia32 } } } } */
/* weak glob_a should be accessed with a GOT.  */
/* { dg-final { scan-assembler "glob_a@GOT\\(" { target ia32 } } } */
