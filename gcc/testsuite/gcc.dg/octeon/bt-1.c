/* { dg-do run { target *-octeon-elf* } } */
/* { dg-options "-fexceptions -O2" } */
/* { dg-output {  #0  0x.*  #1  0x} }  */

#include <execinfo.h>

int i;

__attribute__ ((noinline))
void f ()
{
  __octeon_print_backtrace ();
  i = 1;
}

main ()
{
  f ();
  return 0;
}
