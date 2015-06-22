/* { dg-options "(-mmicromips) addressing=absolute -mips32r2" } */

void foo (void);

int MICROMIPS
a (void)
{
  foo ();
  return 0;
}

/* { dg-final { scan-assembler "\tjals\tfoo\n\tnop" } } */
