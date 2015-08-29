/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fno-pic -fno-plt=bar" } */

extern void bar (void) asm ("bar1");

void
foo (void)
{
  bar ();
}

/* { dg-final { scan-assembler-not "jmp\[ \t\]*.bar1@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "jmp\[ \t\]*.bar1@GOT" { target ia32 } } } */
