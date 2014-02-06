/* { dg-do run { target *-octeon-elf* } } */
/* { dg-options "-O2" } */
/* { dg-output {  #0  0x\w+\s* #1  0x\w+\s* #2  0x\w+\s*  #3  0x\w+} }  */

#include <cvmx.h>
#include <unistd.h>

int *i = 0;

void f (void)
{
  *i = 0;
}

void (*p) (void) = f;

int main (void)
{
  cvmx_user_app_init ();
  /* Make the default execution test pass but wait a little for the
     backtrace to complete.  */
  write (1, "WAIT-FOR-OUTPUT\n*** EXIT code 0", 32);
  p ();
  return 0;
}
