/* { dg-options "-mabi=p32 -mhard-float isa_rev>=6 -mnanomips" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

void
foo (float a[], float b, float c, float d)
{
  float a1 = b + c;
  float a2 = c - d;

  a[1023] = a1;
  a[1024] = a2;
}
/* { dg-final { scan-assembler "\tswc1\t\\\$f1,(4092|-4)\\\(\\\$4\\\)\n" } } */
/* { dg-final { scan-assembler "\tswc1\t\\\$f2,0\\\(\\\$.\\\)\n" } } */
