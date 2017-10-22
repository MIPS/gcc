/* { dg-do compile } */
/* { dg-options "-O2 -fcf-protection -mcet" } */
/* { dg-final { scan-assembler-times "endbr32" 2 { target ia32 } } } */
/* { dg-final { scan-assembler-times "endbr64" 2 { target { ! ia32 } } } } */

extern int x;

static void
__attribute__ ((noinline, noclone))
test (int i)
{
  x = i;
}

extern __typeof (test) foo __attribute__ ((alias ("test")));

void
bar (int i)
{
  test (i);
}
