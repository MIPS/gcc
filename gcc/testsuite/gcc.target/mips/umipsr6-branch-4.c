/* { dg-options "(-mmicromips) addressing=absolute -mips32r6" } */

void foo (void);

int MICROMIPS
a (void)
{
  foo ();
  return 0;
}

/* { dg-final { scan-assembler "\tbalc\tfoo" } } */
