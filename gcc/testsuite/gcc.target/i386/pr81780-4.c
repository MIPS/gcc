/* { dg-do compile { target { *-*-linux* && ia32 } } } */
/* { dg-require-effective-target got32x_reloc } */
/* { dg-options "-fcf-protection -mcet -O2 -fpic -fplt -mregparm=3" } */

extern void bar (int);

int
foo (int i)
{
  bar (i);
  return 0;
}

/* { dg-final { scan-assembler "call\[ \t\]*.bar@GOT" } } */
