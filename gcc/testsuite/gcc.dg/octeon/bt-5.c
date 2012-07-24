/* { dg-do run { target *-octeon-* } } */
/* { dg-options "-funwind-tables -O2" } */

#include <sys/types.h>  /* __UCLIBC__ is defined */

#ifndef __UCLIBC__
#include "test-backtrace.h"

extern func_t functions[];

int main (void);

int libc_callback (const void *a, const void *b)
{
  void *bt[10];
  int len;

  len = backtrace (bt, 10);
  if (!len)
    abort ();

  if (find_function (main, bt, len, 0) == -1)
    abort ();

  return 1;
}

int a[7];

int
main ()
{
  testbt_setup ();
  qsort (a, 7, sizeof (int), libc_callback);
  return 0;
}

func_t functions[] = { F (compare), F (qsort), F (main), FEND };
#else
int main () { return 0; }
#endif /* __UCLIBC__ */
