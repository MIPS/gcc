/* { dg-do compile { target { ! x32 } } } */
/* { dg-options "-O2 -mno-iamcu -mmpx" } */

void
__attribute__((no_caller_saved_registers))
fn (void)
{
  asm ("#"
       :
       :
       : "bnd3");
}

/* { dg-final { scan-assembler-times "bndmov\[\\t \]*%bnd3,\[\\t \]*\[\\-\]?\[0-9\]*\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "bndmov\[\\t \]*\[\\-\]?\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%bnd3" 1 } } */
/* { dg-final { scan-assembler-not "mov(a|u)ps\[\\t \]*%(x|y|z)mm\[0-9\]+,\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\)" } } */
/* { dg-final { scan-assembler-not "mov(a|u)ps\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%(x|y|z)mm\[0-9\]+" } } */
/* { dg-final { scan-assembler-not "(push|pop)(l|q)\[\\t \]*%(r|e)(a|b|c|d)x" } } */
/* { dg-final { scan-assembler-not "(push|pop)(l|q)\[\\t \]*%(r|e)(s|d)i" } } */
/* { dg-final { scan-assembler-not "(push|pop)(l|q)\[\\t \]*%(r|e)bp" } } */
/* { dg-final { scan-assembler-not "(push|pop)q\[\\t \]*%r\[0-9\]+" { target { ! ia32 } } } } */
