/* { dg-do compile } */
/* { dg-options "-O2 -mavx512f -mno-iamcu" } */
typedef unsigned int uword_t __attribute__ ((mode (__word__)));

void
__attribute__((interrupt))
fn (uword_t error, uword_t ip, uword_t cs, uword_t eflags, uword_t sp,
    uword_t ss)
{
  asm ("#"
       :
       :
       : "xmm3");
}

/* { dg-final { scan-assembler-times "vmovups\[\\t \]*%zmm3,\[\\t \]*-?\[0-9\]*\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "vmovups\[\\t \]*-?\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%zmm3" 1 } } */
/* { dg-final { scan-assembler "iret" } }*/
