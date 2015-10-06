/* { dg-do compile } */
/* { dg-options "-O2 -mmmx -mno-iamcu" } */

typedef unsigned int uword_t __attribute__ ((mode (__word__)));
typedef short __v4hi __attribute__ ((__vector_size__ (8)));
typedef int __m64 __attribute__ ((__vector_size__ (8), __may_alias__));

extern __m64 y, x;

void
fn0 (void)
{
  x = __extension__ (__m64){ 0 };
  x = (__m64) __builtin_ia32_packsswb ((__v4hi) x, (__v4hi) y);
  y = x;
}

void
__attribute__((interrupt))
fn1 (void *frame)
{
  x = (__m64) __builtin_ia32_packsswb ((__v4hi) x, (__v4hi) y); /* { dg-error "MMX/3Dnow (instructions|intrinsics) aren't allowed in interrupt service routine" } */
}

void
__attribute__((interrupt))
fn2 (void *frame)
{
  x = y; /* { dg-error "MMX/3Dnow instructions aren't allowed in interrupt service routine" } */
}

void
__attribute__((interrupt))
fn3 (void *frame, uword_t error)
{
  x = __extension__ (__m64){ 0 }; /* { dg-error "MMX/3Dnow instructions aren't allowed in exception service routine" } */
}
