/* { dg-options "-m32 -EL -mhard-float" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

double
foo (double d)
{
  register double l1 asm ("$a4") = d;
  register double l2 asm ("$f20") = 0.0;
  asm ("#foo" : "=d" (l1) : "d" (l1));
  asm volatile ("#foo" :: "f" (l2));
  return l1;
}

/* { dg-final { scan-assembler "\tmfc1\t\\\$a4,\\\$f0\n" } } */
/* { dg-final { scan-assembler "\tmfhc1\t\\\$a5,\\\$f0\n" } } */
/* { dg-final { scan-assembler "\tmtc1\t\\\$zero,\\\$f20\n" } } */
/* { dg-final { scan-assembler "\tmthc1\t\\\$zero,\\\$f20\n" } } */
/* { dg-final { scan-assembler "\tmtc1\t\\\$a4,\\\$f0\n" } } */
/* { dg-final { scan-assembler "\tmthc1\t\\\$a5,\\\$f0\n" } } */
