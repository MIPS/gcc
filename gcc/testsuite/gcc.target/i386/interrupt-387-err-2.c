/* { dg-do compile } */
/* { dg-options "-O2 -mno-cld -m80387 -mlong-double-80 -mno-iamcu -mno-sse" } */

extern long double y, x;

void
__attribute__((no_caller_saved_registers))
fn1 (void)
{
  x = 0; /* { dg-error "80387 instructions aren't allowed in function with no_caller_saved_registers attribute" } */
}
