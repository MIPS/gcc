/* { dg-do compile } */
/* { dg-options "-O2 -mno-cld -mmmx -mno-iamcu" } */

typedef short __v4hi __attribute__ ((__vector_size__ (8)));
typedef int __m64 __attribute__ ((__vector_size__ (8), __may_alias__));

extern __m64 y, x;

void
__attribute__((no_caller_saved_registers))
fn1 (void)
{
  x = (__m64) __builtin_ia32_packsswb ((__v4hi) x, (__v4hi) y); /* { dg-error "MMX/3Dnow (instructions|intrinsics) aren't allowed in function with no_caller_saved_registers attribute" } */
}

void
__attribute__((no_caller_saved_registers))
fn3 (void)
{
  x = __extension__ (__m64){ 0 }; /* { dg-error "MMX/3Dnow instructions aren't allowed in function with no_caller_saved_registers attribute" } */
}
