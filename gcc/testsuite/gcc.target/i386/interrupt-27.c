/* { dg-do compile } */
/* { dg-options "-O0 -g -mno-avx -mno-iamcu -msse" } */

typedef unsigned int uword_t __attribute__ ((mode (__word__)));
typedef float __v4sf __attribute__ ((__vector_size__ (16)));
__v4sf x, y;

void
__attribute__((interrupt))
fn (uword_t ip, uword_t cs, uword_t eflags, uword_t sp, uword_t ss)
{
  x = y;
}

/* { dg-final { scan-assembler-times "movups\[\\t \]*%xmm0,\[\\t \]*-?\[0-9\]*\\(%\[re\]?bp\\)" 1 } } */
/* { dg-final { scan-assembler-times "movups\[\\t \]*-?\[0-9\]*\\(%\[re\]?bp\\),\[\\t \]*%xmm0" 1 } } */
/* { dg-final { scan-assembler "iret" } }*/
