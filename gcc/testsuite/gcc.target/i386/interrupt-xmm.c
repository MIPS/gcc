/* { dg-do compile } */
/* { dg-options "-O2 -mno-avx -mno-iamcu -msse" } */
typedef unsigned int uword_t __attribute__ ((mode (__word__)));

void
__attribute__((interrupt))
fn (uword_t error, uword_t ip, uword_t cs, uword_t eflags)
{
  asm ("#"
       :
       :
       : "xmm3");
}

/* { dg-final { scan-assembler-times "movups\[\\t \]*%xmm3,\[\\t \]*-?\[0-9\]*\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "movups\[\\t \]*-?\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%xmm3" 1 } } */
/* { dg-final { scan-assembler "iret" } }*/
