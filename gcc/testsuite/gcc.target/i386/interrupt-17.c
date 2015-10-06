/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -mno-avx512f -mno-iamcu -mavx -mpush-args -mno-accumulate-outgoing-args" } */

extern void bar (int) __attribute__ ((no_caller_saved_registers));
typedef unsigned int uword_t __attribute__ ((mode (__word__)));

void
 __attribute__ ((interrupt))
fn1 (uword_t ip, uword_t cs, uword_t eflags, uword_t sp, uword_t ss)
{
  bar (3);
}

void
 __attribute__ ((interrupt))
fn2 (uword_t ip, uword_t cs, uword_t eflags, uword_t sp, uword_t ss)
{
  bar (3);
}

/* { dg-final { scan-assembler-not "movups\[\\t .\]*%(x|y|z)mm\[0-9\]+" } } */
/* { dg-final { scan-assembler-not "(push|pop)(l|q)\[\\t \]*%(r|e)(a|b|c|d)x" } } */
/* { dg-final { scan-assembler-not "(push|pop)(l|q)\[\\t \]*%(r|e)si" } } */
/* { dg-final { scan-assembler-not "(push|pop)q\[\\t \]*%r\[8-9\]+" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "(push|pop)q\[\\t \]*%r1\[0-2\]+" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "(push|pop)q\[\\t \]*%r1\[4-5\]+" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "push(?:l|q)\[\\t \]*%(?:r|e)bp" 2 } } */
/* { dg-final { scan-assembler-times "leave" 2 { target ia32 } } } */
/* { dg-final { scan-assembler-times "pushl\[\\t \]*%edi" 2 { target ia32 } } } */
/* { dg-final { scan-assembler-times "pushq\[\\t \]*%rdi" 2 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "popq\[\\t \]*%rdi" 2 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "pushq\[\\t \]*%r13" 2 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "popq\[\\t \]*%r13" 2 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "iret" 2 } }*/
