/* { dg-options "-mcode-readable=no -mno-gpopt" } */
/* { dg-final { scan-assembler "\tsb\t\\\$\[0-9\],%lo\\(c\\)" } } */
/* { dg-final { scan-assembler "\tsh\t\\\$\[0-9\],%lo\\(s\\)" } } */
/* { dg-final { scan-assembler "\tsw\t\\\$\[0-9\],%lo\\(i\\)" } } */
/* { dg-final { scan-assembler "\tsb\t\\\$\[0-9\],%lo\\(a_c\\)" } } */
/* { dg-final { scan-assembler-not "\tsh\t\\\$\[0-9\],%lo\\(a_s\\)" } } */
/* { dg-final { scan-assembler-not "\tsw\t\\\$\[0-9\],%lo\\(a_i\\)" } } */

char __attribute__((aligned(1))) a_c;
short __attribute__((aligned(1))) a_s;
int __attribute__((aligned(1))) a_i;
char c;
short s;
int i;

void
foo (void)
{
  c = 0;
  s = 0;
  i = 0;
  a_c = 0;
  a_s = 0;
  a_i = 0;
}

