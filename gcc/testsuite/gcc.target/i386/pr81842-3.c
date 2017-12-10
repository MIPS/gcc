/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-fcf-protection -mcet" } */

#include <ucontext.h>

void *
func1 (void)
{
  return getcontext;
}

void *
func2 (void)
{
  return setcontext;
}

void *
func3 (void)
{
  return swapcontext;
}

void *
func4 (void)
{
  return makecontext;
}
/* { dg-error "'getcontext' cannot be used with -mshstk" "" { target *-*-* } 0 } */
/* { dg-error "'setcontext' cannot be used with -mshstk" "" { target *-*-* } 0 } */
/* { dg-error "'swapcontext' cannot be used with -mshstk" "" { target *-*-* } 0 } */
/* { dg-error "'makecontext' cannot be used with -mshstk" "" { target *-*-* } 0 } */
