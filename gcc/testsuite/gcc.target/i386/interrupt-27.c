/* { dg-do compile } */
/* { dg-options "-O0 -mno-cld -g -mno-avx -mno-iamcu -msse" } */

typedef float __v4sf __attribute__ ((__vector_size__ (16)));
__v4sf x, y;

void
__attribute__((interrupt))
fn (void *frame)
{
  x = y;
}

/* { dg-final { scan-assembler-times "movups\[\\t \]*%xmm0,\[\\t \]*-?\[0-9\]*\\(%\[re\]?bp\\)" 1 } } */
/* { dg-final { scan-assembler-times "movups\[\\t \]*-?\[0-9\]*\\(%\[re\]?bp\\),\[\\t \]*%xmm0" 1 } } */
/* { dg-final { scan-assembler-times "iret" 1 { target ia32 } } } */
/* { dg-final { scan-assembler-times "iretq" 1 { target { ! ia32 } } } } */
/* { dg-final { scan-assembler-not "\tcld" } } */
