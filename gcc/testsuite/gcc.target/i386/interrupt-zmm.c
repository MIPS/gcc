/* { dg-do compile } */
/* { dg-options "-O2 -mno-cld -mavx512f -mno-iamcu" } */

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
/* { dg-final { scan-assembler-times "iret" 1 { target ia32 } } } */
/* { dg-final { scan-assembler-times "iretq" 1 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "\tcld" } } */
/* { dg-final { scan-assembler-not "vzeroupper" } } */
