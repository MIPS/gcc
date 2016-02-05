/* { dg-do compile } */
/* { dg-options "-O2 -mno-cld -mavx512bw -mno-iamcu -mno-red-zone" } */

void
__attribute__((interrupt))
fn (void *frame)
{
  asm ("#"
       :
       :
       : "k0", "k1", "k2", "k3");
}

/* { dg-final { scan-assembler-times "kmovq\[\\t \]*%k0,\[\\t \]*\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "kmovq\[\\t \]*%k1,\[\\t \]*8\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "kmovq\[\\t \]*%k2,\[\\t \]*16\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "kmovq\[\\t \]*%k3,\[\\t \]*24\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "iret" 1 { target ia32 } } } */
/* { dg-final { scan-assembler-times "iretq" 1 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "\tcld" } } */
