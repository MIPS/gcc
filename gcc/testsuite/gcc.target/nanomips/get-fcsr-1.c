/* { dg-options "-mhard-float" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

unsigned int
foo (void)
{
  return __builtin_mips_get_fcsr ();
}

/* { dg-final { scan-assembler "cfc1\t\\\$a0,\\\$31" } } */
