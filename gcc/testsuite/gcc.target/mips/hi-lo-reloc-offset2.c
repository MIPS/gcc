/* { dg-options "-mcode-readable=no -mno-gpopt" } */
/* { dg-final { scan-assembler "\tsb\t\\\$\[0-9\],%lo\\(c\\)" } } */
/* { dg-final { scan-assembler "\tsh\t\\\$\[0-9\],%lo\\(s\\)" } } */
/* { dg-final { scan-assembler "\tsw\t\\\$\[0-9\],%lo\\(i\\)" } } */
/* { dg-final { scan-assembler "%lo\\(l\\+4\\)" } } */
/* { dg-final { scan-assembler "\tsw\t\\\$\[0-9\],%lo\\(f\\)" } } */
/* { dg-final { scan-assembler "%lo\\(d\\+4\\)" } } */
/* { dg-final { scan-assembler "\tsb\t\\\$\[0-9\],%lo\\(a_c\\)" } } */
/* { dg-final { scan-assembler-not "\tsh\t\\\$\[0-9\],%lo\\(a_s\\)" } } */
/* { dg-final { scan-assembler-not "\tsw\t\\\$\[0-9\],%lo\\(a_i\\)" } } */
/* { dg-final { scan-assembler-not "%lo\\(a_l\\+4\\)" } } */
/* { dg-final { scan-assembler-not "\tsw\t\\\$\[0-9\],%lo\\(a_f\\)" } } */
/* { dg-final { scan-assembler-not "%lo\\(a_d\\+4\\)" } } */

char c;
short s;
int i;
unsigned long long l;
float f;
double d;
char a_c __attribute__((aligned(1)));
short a_s __attribute__((aligned(1)));
int __attribute__((aligned(1))) a_i;
unsigned long long a_l __attribute__((aligned(1)));
float a_f __attribute__((aligned(1)));
double a_d __attribute__((aligned(1)));

void
foo (void)
{
  c = 0;
  s = 0;
  i = 0;
  l = 0;
  f = 0;
  d = 0;
  a_c = 0;
  a_s = 0;
  a_i = 0;
  a_l = 0;
  a_f = 0;
  a_d = 0;
}
