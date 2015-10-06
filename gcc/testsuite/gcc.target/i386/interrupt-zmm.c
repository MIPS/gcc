/* { dg-do compile } */
/* { dg-options "-O2 -mavx512f -mno-iamcu" } */

void
__attribute__((interrupt))
fn (void *frame)
{
  asm ("#"
       :
       :
       : "xmm3");
}

/* { dg-final { scan-assembler-times "vmovups\[\\t \]*%zmm3,\[\\t \]*-?\[0-9\]*\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "vmovups\[\\t \]*-?\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%zmm3" 1 } } */
/* { dg-final { scan-assembler "iret" } }*/
