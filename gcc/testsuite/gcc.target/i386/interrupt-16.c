/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -mno-avx512f -mno-iamcu -mavx -mno-push-args" } */

typedef unsigned int uword_t __attribute__ ((mode (__word__)));
extern void bar (int) __attribute__ ((no_caller_saved_registers));

void
 __attribute__ ((interrupt))
fn1 (void *frame, uword_t error)
{
  bar (error);
}

/* { dg-final { scan-assembler-not "movups\[\\t \]*%(x|y|z)mm\[0-9\]+,\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\)" } } */
/* { dg-final { scan-assembler-not "movups\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%(x|y|z)mm\[0-9\]+" } } */
/* { dg-final { scan-assembler-not "(push|pop)(l|q)\[\\t \]*%(r|e)(a|b|c|d)x" } } */
/* { dg-final { scan-assembler-not "(push|pop)(l|q)\[\\t \]*%(r|e)si" } } */
/* { dg-final { scan-assembler-not "(push|pop)l\[\\t \]*%edi" { target ia32 } } } */
/* { dg-final { scan-assembler-not "(push|pop)q\[\\t \]*%rbp" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "(push|pop)q\[\\t \]*%r\[0-9\]+" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "pushl\[\\t \]*%ebp" 1 { target ia32 } } } */
/* { dg-final { scan-assembler-times "popl\[\\t \]*%ebp" 1 { target ia32 } } } */
/* { dg-final { scan-assembler-times "pushq\[\\t \]*%rdi" 1 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "popq\[\\t \]*%rdi" 1 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "addl\[\\t \]*\\\$4,\[\\t \]*%esp" 1 { target ia32 } } } */
/* { dg-final { scan-assembler-times "add(?:l|q)\[\\t \]*\\\$8,\[\\t \]*%\[re\]?sp" 1 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "iret" 1 } }*/
