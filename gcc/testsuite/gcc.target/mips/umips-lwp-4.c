/* { dg-options "-mgp32 -fpeephole2 -mtune=m14k (-mmicromips)" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-O1" } { "" } } */

void MICROMIPS
foo (int *r4)
{
  register int r5 asm ("$5") = r4[511];
  register int r6 asm ("$6") = r4[512];
  r4[2] = (r6 << 1) + r5;
  asm ("#foo" : "=m" (r4[3]) : "d" (r5), "d" (r6));
}

/* { dg-final { scan-assembler "\tlwp\t\\\$5,2044\\(\\\$4\\)" } }*/
