// { dg-do run }
// { dg-options "-O2" }

#include <stdarg.h>

struct dummy { struct{}__attribute__((aligned (4))) a[7]; };

void
test (struct dummy a, ...)
{
  va_list va_arglist;
  int i;

  va_start (va_arglist, a);
  i = va_arg (va_arglist, int);
  if (i != 0x10)
    __builtin_abort ();
  va_end (va_arglist);
}

struct dummy a0;

int
main ()
{
  test (a0, 0x10);
  return 0;
}
