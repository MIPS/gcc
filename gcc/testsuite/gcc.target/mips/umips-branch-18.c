/* { dg-options "(-mmicromips) isa_rev>=6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

int MICROMIPS
foo (int *x)
{
  return x[5000];
}

/* { dg-final { scan-assembler "lw\t\\\$2,20000\\(\\\$4\\)\n\tjrc\t\\\$31" } } */
