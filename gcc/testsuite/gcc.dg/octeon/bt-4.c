/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-funwind-tables -O2" } */

#include <sys/types.h>  /* __UCLIBC__ is defined */

#ifndef __UCLIBC__
#include "test-backtrace.h"

extern func_t functions[];

int i = 1;

int
main ()
{
  testbt_setup ();
  foo (i);
  return 0;
}

int
foo (int j)
{
  return bar (j);
}

int
bar (int i)
{
  if (i)
    bar (i - 1);
  else
    check_backtrace ();
  return 1;
}

func_t functions[] = { F (bar), F (bar), F (main), FEND };
#else
int main () { return 0; }
#endif /* __UCLIBC__ */
