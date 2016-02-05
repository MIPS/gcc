/* { dg-do compile } */
/* { dg-options "-O2 -mno-cld -g -mno-avx -mno-iamcu -msse" } */

typedef float xmm_t __attribute__ ((__vector_size__ (16)));
extern xmm_t x, y;

void
__attribute__((no_caller_saved_registers))
fn (void)
{
  x = __builtin_ia32_addss (x, y);
}

/* { dg-final { scan-assembler-times "movaps\[\\t \]*%xmm0,\[\\t \]*-?\[0-9\]*\\(%\[re\]?sp\\)" 1 } } */
/* { dg-final { scan-assembler-times "movaps\[\\t \]*-?\[0-9\]*\\(%\[re\]?sp\\),\[\\t \]*%xmm0" 1 } } */
/* { dg-final { scan-assembler-not "\tcld" } } */
