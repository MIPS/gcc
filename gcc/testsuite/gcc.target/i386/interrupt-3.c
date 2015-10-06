/* { dg-do compile } */
/* { dg-options "-O2 -g" } */

typedef unsigned int uword_t __attribute__ ((mode (__word__)));

void
__attribute__((interrupt))
fn (uword_t error, uword_t ip, uword_t cs, uword_t eflags, uword_t sp,
    uword_t ss)
{
}

/* { dg-final { scan-assembler-times "add(?:l|q)\[\\t \]*\\\$8,\[\\t \]*%\[re\]?sp" 1 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "addl\[\\t \]*\\\$4,\[\\t \]*%esp" 1 { target ia32 } } } */
/* { dg-final { scan-assembler "iret" } }*/
