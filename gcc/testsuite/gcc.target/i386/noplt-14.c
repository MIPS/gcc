/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fno-pic -fno-plt=bar" } */

extern void bar1 (void) asm ("bar");

void
foo (void)
{
  bar1 ();
}

/* { dg-final { scan-assembler "jmp\[ \t\]*.bar@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "jmp\[ \t\]*.bar@GOT" { target { ia32 && r_386_got32x } } } } */
