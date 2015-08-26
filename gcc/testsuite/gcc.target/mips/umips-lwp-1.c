/* { dg-options "-mgp32 -fpeephole2 -mtune=m14k (-mmicromips) isa_rev<=5" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-O1" } { "" } } */

void MICROMIPS
foo (int *r4)
{
  register int r5 asm ("$5") = r4[0];
  register int r6 asm ("$6") = r4[1];
  r4[2] = (r5 << 1) + r6;
  asm ("#foo" : "=m" (r4[3]) : "d" (r5), "d" (r6));
}

/* { dg-final { scan-assembler "\tlwp\t\\\$5,0\\(\\\$4\\)" } }*/
