/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-fcf-protection -mcet -mno-check-ucontext-functions" } */

#include <ucontext.h>

int
func1 (ucontext_t *ucp)
{
  return getcontext (ucp);
}

int
func2 (const ucontext_t *ucp)
{
  return setcontext (ucp);
}

int
func3 (ucontext_t *oucp, const ucontext_t *ucp)
{
  return swapcontext (oucp, ucp);
}

void
func4 (ucontext_t *ucp, void (*func) (void))
{
  makecontext (ucp, func, 0);
}
