/* { dg-do compile { target { ! x32 } } } */
/* { dg-options "-O2 -mno-iamcu -mmpx" } */

void
__attribute__((interrupt))
fn (void *frame)
{
  asm ("#"
       :
       :
       : "bnd3");
}

/* { dg-final { scan-assembler-times "bndmov\[\\t \]*%bnd3,\[\\t \]*\[\\-\]?\[0-9\]*\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "bndmov\[\\t \]*\[\\-\]?\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%bnd3" 1 } } */
/* { dg-final { scan-assembler "iret" } }*/
