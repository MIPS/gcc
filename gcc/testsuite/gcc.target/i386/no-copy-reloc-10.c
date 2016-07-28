/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fpic -mno-copy-reloc" } */

extern int bar __attribute__((visibility("protected")));

int *
foo (void)
{
  return &bar;
}

/* { dg-final { scan-assembler "lea\(l|q\)\[ \t\]*bar\\(%rip\\)" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "leal\[ \t\]*bar@GOTOFF\\(" { target { ia32 && got32x_reloc } } } } */
/* { dg-final { scan-assembler-not "mov\(l|q\)\[ \t\]*bar@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "movl\[ \t\]*bar@GOT\\(" { target { ia32 && got32x_reloc } } } } */
