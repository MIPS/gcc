/* { dg-options "(-mmicromips) -mips32r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

int MICROMIPS
foo (void)
{
  return 0;
}

/* { dg-final { scan-assembler "\tmove\t\\\$2,\\\$0\n\tjrc\t\\\$31" } } */
