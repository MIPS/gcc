/* { dg-options "(-mmicromips) -mips32r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

int MICROMIPS
foo (int *x)
{
  return x[5000];
}

/* { dg-final { scan-assembler "\tlw\t\\\$2,20000\\(\\\$4\\)\n\tjrc\t\\\$31" } } */
