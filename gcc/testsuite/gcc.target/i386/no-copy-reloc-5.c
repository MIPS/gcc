/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fno-pic -mno-copy-reloc" } */

extern int bar;

int
foo (void)
{
  return bar;
}

/* { dg-final { scan-assembler "mov\(l|q\)\[ \t\]*bar@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "movl\[ \t\]*bar@GOT," { target { ia32 && got32x_reloc } } } } */
/* { dg-final { scan-assembler-not "\(mov|lea\)\(l|q\)\[ \t\]*\(\\\$|\)bar\\(%rip\\)" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "\(mov|lea\)l\[ \t\]*\(\\\$|\)bar," { target { ia32 && got32x_reloc } } } } */
