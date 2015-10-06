/* { dg-do link } */
/* { dg-options "-O -g" } */

#include <stdint.h>

extern void link_error (void);

typedef unsigned int uword_t __attribute__ ((mode (__word__)));

__attribute__ ((used, interrupt))
void
foo (uword_t ip, uword_t cs, uword_t eflags, uword_t sp, uword_t ss)
{
  void *ra = __builtin_return_address (0);
  if ((uintptr_t) ra != (uintptr_t) ip)
    link_error ();
}

int
main (void)
{
  return 0;
}
