/* { dg-do run { target *-octeon-elf* } } */
/* { dg-options "-O2" } */
/* { dg-additional-files { ../test-backtrace.h } } */

#include <cvmx.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "test-backtrace.h"

int *i = 0;

int f (void);
void g (void);
int main (void);

func_t functions[] = { F (f), F (g), F (main), FEND };

int (*p) (void) = f;
void (*pp) (void) = g;
int main (void);

int f (void)
{
  puts ("clobber ra");
  *i = 0;
  puts ("don't pop frame");
  /* Prevent tail call optimization.  */
  return 1;
}

void g (void)
{
  p ();
  puts ("no tail call");
}

int
main (void )
{
  cvmx_user_app_init ();
  testbt_setup ();
  pp ();
  puts ("no tail call");
  return 0;
}
