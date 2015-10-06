/* { dg-do compile { target { ! x32 } } } */
/* { dg-options "-O2 -mno-iamcu -mmpx" } */
typedef unsigned int uword_t __attribute__ ((mode (__word__)));

void
__attribute__((interrupt))
fn (uword_t ip, uword_t cs, uword_t eflags, uword_t sp, uword_t ss)
{
  asm ("#"
       :
       :
       : "bnd3");
}

/* { dg-final { scan-assembler-times "bndmov\[\\t \]*%bnd3,\[\\t \]*\[\\-\]?\[0-9\]*\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "bndmov\[\\t \]*\[\\-\]?\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%bnd3" 1 } } */
/* { dg-final { scan-assembler "iret" } }*/
