/* { dg-options "-mhard-float" } */
/* { dg-skip-if "requiring \$a0 is a code-quality test" { *-*-* } { "-O0" } { "" } } */

void
foo (unsigned int x)
{
  __builtin_mips_set_fcsr (x);
}

/* { dg-final { scan-assembler "ctc1\t\\\$a0,\\\$31" } } */
