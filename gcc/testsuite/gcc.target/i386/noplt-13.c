/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fno-pic -fno-plt=bar" } */

extern void bar (void);

int
__attribute__((regparm(1)))
foo (void *p)
{
  return p == &bar;
}

/* { dg-final { scan-assembler "cmp\(l|q\)\[ \t\]*bar@GOTPCREL" { target { { ! ia32 } && r_x86_64_gotpcrelx } } } } */
/* { dg-final { scan-assembler "cmpl\[ \t\]*bar@GOT," { target { ia32 && r_386_got32x } } } } */
