/* { dg-options "-mcode-readable=no -mfpxx -mabi=32 -fpic -mabicalls" } */
/* { dg-final { scan-assembler "\tsb\t\\\$\[0-9\],%lo\\(c\\)" } } */
/* { dg-final { scan-assembler "\tsh\t\\\$\[0-9\],%lo\\(s\\)" } } */
/* { dg-final { scan-assembler "\tsw\t\\\$\[0-9\],%lo\\(i\\)" } } */
/* { dg-final { scan-assembler "\tsw\t\\\$\[0-9\],%lo\\(l\\+4\\)" } } */
/* { dg-final { scan-assembler "\tswc1\t\\\$f\[0-9\],%lo\\(f\\)" } } */
/* { dg-final { scan-assembler "\tsdc1\t\\\$f\[0-9\],%lo\\(d\\)" } } */
/* { dg-final { scan-assembler "\tsb\t\\\$\[0-9\],%lo\\(a_c\\)" } } */
/* { dg-final { scan-assembler-not "\tsh\t\\\$\[0-9\],%lo\\(a_s\\)" } } */
/* { dg-final { scan-assembler-not "\tsw\t\\\$\[0-9\],%lo\\(a_i\\)" } } */
/* { dg-final { scan-assembler-not "\tsw\t\\\$\[0-9\],%lo\\(a_l\\+4\\)" } } */
/* { dg-final { scan-assembler-not "\tswc1\t\\\$f\[0-9\],%lo\\(a_f\\)" } } */
/* { dg-final { scan-assembler-not "\tsdc1\t\\\$f\[0-9\],%lo\\(a_d\\)" } } */

static char __attribute__((aligned(1))) a_c;
static short __attribute__((aligned(1))) a_s;
static int __attribute__((aligned(1))) a_i;
static long long __attribute__((aligned(1))) a_l;
static float __attribute__((aligned(1))) a_f;
static double __attribute__((aligned(1))) a_d;
static char c;
static short s;
static int i;
static long long l;
static float f;
static double d;

void foo (char v)
{
 c += v;
 s += v;
 i += v;
 l += v;
 f += v;
 d += v;
 a_c += v;
 a_s += v;
 a_i += v;
 a_l += v;
 a_f += v;
 a_d += v;
}

