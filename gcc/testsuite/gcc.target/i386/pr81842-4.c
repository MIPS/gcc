/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-fcf-protection -mcet -mno-check-ucontext-functions" } */

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
