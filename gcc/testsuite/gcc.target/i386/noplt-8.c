/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fpic -fno-plt=xxx,bar" } */

extern void bar (void);
extern void yyy (void);

int
foo (void)
{
  bar ();
  yyy ();
  return 0;
}

/* { dg-final { scan-assembler "call\[ \t\]*.bar@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "call\[ \t\]*.yyy@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "call\[ \t\]*.bar@GOT\\(" { target ia32 } } } */
/* { dg-final { scan-assembler-not "call\[ \t\]*.yyy@GOT\\(" { target ia32 } } } */
