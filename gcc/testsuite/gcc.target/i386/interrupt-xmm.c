/* { dg-do compile } */
/* { dg-options "-O2 -mno-avx -mno-iamcu -msse" } */

void
__attribute__((interrupt))
fn (void *frame)
{
  asm ("#"
       :
       :
       : "xmm3");
}

/* { dg-final { scan-assembler-times "movups\[\\t \]*%xmm3,\[\\t \]*-?\[0-9\]*\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "movups\[\\t \]*-?\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%xmm3" 1 } } */
/* { dg-final { scan-assembler "iret" } }*/
