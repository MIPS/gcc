/* Check that tpoff/ntpoff isn't used to access glob_a.  */
/* { dg-do compile { target *-*-linux* } } */
/* { dg-require-effective-target pie } */
/* { dg-options "-O2 -fpie" } */

extern __thread int glob_a;

int foo ()
{
  return glob_a;
}

/* glob_a should never be accessed with a tpoff.  */
/* { dg-final { scan-assembler-not "glob_a@tpoff" { target { ! ia32 } } } } */
/* glob_a should never be accessed with a ntpoff.  */
/* { dg-final { scan-assembler-not "glob_a@ntpoff" { target ia32 } } } */
