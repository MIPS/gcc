/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fno-pic -fno-plt" } */

extern void bar (void);

void *
foo (void)
{
  return &bar;
}

/* { dg-final { scan-assembler "mov\(l|q\)\[ \t\]*bar@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "mov\(l|q\)\[ \t\]*\\\$bar," { target { ! ia32 } } } } */
