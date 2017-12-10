/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-fcf-protection -mcet" } */

#include <ucontext.h>

int
func1 (ucontext_t *ucp)
{
  return getcontext (ucp); /* { dg-error "'getcontext' cannot be used with -mshstk" } */
}

int
func2 (const ucontext_t *ucp)
{
  return setcontext (ucp); /* { dg-error "'setcontext' cannot be used with -mshstk" } */
}

int
func3 (ucontext_t *oucp, const ucontext_t *ucp)
{
  return swapcontext (oucp, ucp); /* { dg-error "'swapcontext' cannot be used with -mshstk" } */
}

void
func4 (ucontext_t *ucp, void (*func) (void))
{
  makecontext (ucp, func, 0); /* { dg-error "'makecontext' cannot be used with -mshstk" } */
}
