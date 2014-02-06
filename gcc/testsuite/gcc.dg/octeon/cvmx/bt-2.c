/* { dg-do run { target *-octeon-elf* } } */
/* { dg-options "-O2" } */

#include <cvmx.h>
#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include <unistd.h>
#include "cvmx.h"

int *i = 0;

extern char fault_addr[];

static void handler(uint64_t registers[32])
{
  void *buffer[10];
  int len, i;

  (void)registers;

  len = backtrace (buffer, 10);
  if (!len)
    exit (1);

  for (i = 0; i < len; i++)
    {
      printf ("#%d  %p\n", i, buffer[i]);
      if (buffer[i] == fault_addr)
	{
	  /* Make it pass.  */
	  write (1, "\n*** EXIT code 0\n", 17);
	  exit (0);
	}
    }
  exit (1);
}

int main (void)
{
  cvmx_user_app_init ();

  cvmx_interrupt_set_exception (handler);
  printf ("fault_addr: %p\n", fault_addr);
  asm volatile ("fault_addr: lw $2, %0" :: "m"(*i));

  return 0;
}
