/* { dg-do compile } */
/* { dg-options "-O2 -mavx512f -mno-iamcu" } */

void
__attribute__((no_caller_saved_registers))
fn (void)
{
  asm ("#"
       :
       :
       : "xmm3");
}

/* { dg-final { scan-assembler-times "movups\[\\t \]*%zmm3,\[\\t \]*-?\[0-9\]*\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "movups\[\\t \]*-?\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%zmm3" 1 } } */
/* { dg-final { scan-assembler-not "mov(a|u)ps\[\\t \]*%(x|y|z)mm\[0-2\]+,\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\)" } } */
/* { dg-final { scan-assembler-not "mov(a|u)ps\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%(x|y|z)mm\[0-2\]+" } } */
/* { dg-final { scan-assembler-not "mov(a|u)ps\[\\t \]*%(x|y|z)mm\[4-9\]+,\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\)" } } */
/* { dg-final { scan-assembler-not "mov(a|u)ps\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%(x|y|z)mm\[4-9\]+" } } */
/* { dg-final { scan-assembler-not "mov(a|u)ps\[\\t \]*%(x|y|z)mm1\[0-9\]+,\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\)" } } */
/* { dg-final { scan-assembler-not "mov(a|u)ps\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%(x|y|z)mm1\[0-9\]+" } } */
/* { dg-final { scan-assembler-not "(push|pop)(l|q)\[\\t \]*%(r|e)(a|b|c|d)x" } } */
/* { dg-final { scan-assembler-not "(push|pop)(l|q)\[\\t \]*%(r|e)(s|d)i" } } */
/* { dg-final { scan-assembler-not "(push|pop)(l|q)\[\\t \]*%(r|e)bp" } } */
/* { dg-final { scan-assembler-not "(push|pop)q\[\\t \]*%r\[0-9\]+" { target { ! ia32 } } } } */
