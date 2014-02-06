/* { dg-do run { target *-octeon-elf* } } */
/* { dg-options "-O2" } */
/* { dg-additional-files { ../test-backtrace.h } } */

#include <cvmx.h>
#include <string.h>

#include "test-backtrace.h"

int main (void);
void f (void);

func_t functions[] = { F(memcpy), F (f), F(main), FEND };

int *i = 0, *j = 0, n = 100;

int (*p) (void) = f;
void f (void)
{
  memcpy (i, j, n);
  puts ("no tail call");
}

int main (void)
{
  cvmx_user_app_init ();
  testbt_setup ();
  p ();
  return 0;
}
