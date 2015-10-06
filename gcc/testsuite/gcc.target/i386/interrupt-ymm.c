/* { dg-do compile } */
/* { dg-options "-O2 -mno-avx512f -mno-iamcu -mavx" } */

void
__attribute__((interrupt))
fn (void *frame)
{
  asm ("#"
       :
       :
       : "xmm3");
}

/* { dg-final { scan-assembler-times "vmovups\[\\t \]*%ymm3,\[\\t \]*-?\[0-9\]*\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "vmovups\[\\t \]*-?\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%ymm3" 1 } } */
/* { dg-final { scan-assembler "iret" } }*/
