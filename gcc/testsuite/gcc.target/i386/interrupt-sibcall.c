/* { dg-do compile } */
/* { dg-options "-O3" } */

extern void bar (void);
typedef unsigned int uword_t __attribute__ ((mode (__word__)));

__attribute__ ((interrupt))
void
foo (uword_t ip, uword_t cs, uword_t eflags, uword_t sp, uword_t ss)
{
  bar ();
}
/* { dg-final { scan-assembler-not "jmp" } }*/
/* { dg-final { scan-assembler "iret" } }*/
