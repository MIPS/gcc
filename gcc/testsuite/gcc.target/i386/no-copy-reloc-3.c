/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fno-pic -mno-copy-reloc" } */

static int bar;

int *
foo (void)
{
  return &bar;
}

/* { dg-final { scan-assembler "mov\(l|q\)\[ \t\]*\\\$bar," } } */
/* { dg-final { scan-assembler-not "mov\(l|q\)\[ \t\]*bar@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "movl\[ \t\]*bar@GOT," { target { ia32 && got32x_reloc } } } } */
