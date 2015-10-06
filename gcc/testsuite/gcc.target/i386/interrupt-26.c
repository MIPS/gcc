/* { dg-do compile } */
/* { dg-options "-O2 -Wall -Wunused-parameter -g" } */

typedef unsigned int uword_t __attribute__ ((mode (__word__)));

void
__attribute__((interrupt))
fn (uword_t ip, uword_t cs, uword_t eflags, uword_t sp, uword_t ss)
{
}

/* { dg-final { scan-assembler-not "add(l|q)\[\\t \]*\\$\[0-9\]*,\[\\t \]*%\[re\]?sp" } } */
/* { dg-final { scan-assembler "iret" } }*/
