/* { dg-do compile { target { *-*-linux* && ia32 } } } */
/* { dg-require-effective-target got32x_reloc } */
/* { dg-options "-fcf-protection -mcet -O2 -fno-pic -fplt" } */

extern void bar (int) __attribute__ ((regparm (3)));

int
foo (int i)
{
  bar (i);
  return 0;
}

/* { dg-final { scan-assembler "call\[ \t\]*.bar@GOT" } } */
