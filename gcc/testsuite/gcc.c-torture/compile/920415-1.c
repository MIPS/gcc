/* APPLE LOCAL testsuite nested functions */
/* { dg-options "-fnested-functions" } */

/* APPLE LOCAL testsuite added stdlib.h */
#include <stdlib.h>

#ifndef NO_LABEL_VALUES
f ()
{
  __label__ l;
  void *x()
    {
      return &&l;
    }
  goto *x ();
  abort ();
  return;
 l:
  exit (0);
}
#else
int x;
#endif
