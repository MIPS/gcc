/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fno-pic -mno-copy-reloc" } */

extern int bar;

int
check (int *p)
{
  return p != &bar;
}

/* { dg-final { scan-assembler "cmp\(l|q\)\[ \t\]*.*bar@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "movl\[ \t\]*bar@GOT," { target { ia32 && got32x_reloc } } } } */
/* { dg-final { scan-assembler-not "cmp\(l|q\)\[ \t\]*\\\$bar," } } */
