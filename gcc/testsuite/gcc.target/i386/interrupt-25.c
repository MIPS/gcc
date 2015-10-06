/* { dg-do compile { target ia32 } } */
/* { dg-options "-O2 -mno-avx -mno-iamcu -msse -mincoming-stack-boundary=2" } */

void
__attribute__((no_caller_saved_registers))
fn (void)
{
  asm ("#"
       :
       :
       : "xmm3");
}

/* { dg-final { scan-assembler-times "movups\[\\t \]*%xmm3,\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "movups\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%xmm3" 1 } } */
/* { dg-final { scan-assembler-not "mov(a|u)ps\[\\t \]*%(x|y|z)mm\[0-2\]+,\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\)" } } */
/* { dg-final { scan-assembler-not "mov(a|u)ps\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%(x|y|z)mm\[0-2\]+" } } */
/* { dg-final { scan-assembler-not "mov(a|u)ps\[\\t \]*%(x|y|z)mm\[4-9\]+,\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\)" } } */
/* { dg-final { scan-assembler-not "mov(a|u)ps\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%(x|y|z)mm\[4-9\]+" } } */
/* { dg-final { scan-assembler-not "mov(a|u)ps\[\\t \]*%(x|y|z)mm1\[0-9\]+,\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\)" } } */
/* { dg-final { scan-assembler-not "mov(a|u)ps\[\\t \]*\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%(x|y|z)mm1\[0-9\]+" } } */
/* { dg-final { scan-assembler-not "(push|pop)l\[\\t \]*%e(a|b|c|d)x" } } */
/* { dg-final { scan-assembler-not "(push|pop)l\[\\t \]*%e(s|d)i" } } */
/* { dg-final { scan-assembler-not "(push|pop)l\[\\t \]*%ebp" } } */
