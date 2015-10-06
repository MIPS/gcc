/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -mno-avx512f -mno-iamcu -mavx -mno-push-args -maccumulate-outgoing-args" } */

extern void foo (int) __attribute__ ((no_caller_saved_registers));
extern void bar (int) __attribute__ ((no_caller_saved_registers));
extern int x;

void
foo (int i)
{
  bar (i);
  x = i;
}

/* { dg-final { scan-assembler-not "movups\[\\t \]*%(x|y|z)mm\[0-9\]+,\[\\t \]-*\[0-9\]*\\(%\[re\]?bp\\)" } } */
/* { dg-final { scan-assembler-not "movups\[\\t \]*-\[0-9\]*\\(%\[re\]?bp\\),\[\\t \]*%(x|y|z)mm\[0-9\]+" } } */
/* { dg-final { scan-assembler-not "(push|pop)(l|q)\[\\t \]*%(r|e)(b|c|d)x" } } */
/* { dg-final { scan-assembler-not "(push|pop)(l|q)\[\\t \]*%(r|e)si" } } */
/* { dg-final { scan-assembler-not "(push|pop)(l|q)\[\\t \]*%(r|e)bp" } } */
/* { dg-final { scan-assembler-not "(push|pop)l\[\\t \]*%edi" { target ia32 } } } */
/* { dg-final { scan-assembler-not "(push|pop)q\[\\t \]*%r\[0-9\]+" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "push(?:l|q)\[\\t \]*%(?:r|e)ax" 1 } } */
/* { dg-final { scan-assembler-times "pop(?:l|q)\[\\t \]*%(?:r|e)ax" 1 } } */
/* { dg-final { scan-assembler-times "pushq\[\\t \]*%rdi" 1 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "popq\[\\t \]*%rdi" 1 { target { ! ia32 } } } } */
