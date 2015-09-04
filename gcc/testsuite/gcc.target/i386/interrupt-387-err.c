/* { dg-do compile } */
/* { dg-options "-O2 -m80387 -mlong-double-80 -mno-iamcu -mno-sse" } */

typedef unsigned int uword_t __attribute__ ((mode (__word__)));

extern long double y, x;

void
fn0 (void)
{
  x = y;
  y = 0;
}

void
__attribute__((interrupt))
fn1 (void *frame, uword_t error)
{
  x = 0; /* { dg-error "80387 instructions aren't allowed in exception service routine" } */
}

void
__attribute__((interrupt))
fn2 (void *frame)
{
  x = y; /* { dg-error "80387 instructions aren't allowed in interrupt service routine" } */
}
