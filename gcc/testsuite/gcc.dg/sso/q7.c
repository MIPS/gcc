#include <stdio.h>

#include "init7.h"
#include "dump.h"

extern void abort (void);

int main (void)
{
  struct R1 A1 = My_R1;
  struct R1 B1 = My_R1;

  struct R2 A2 = My_R2;
  struct R2 B2 = My_R2;

  put ("A1 :");
  dump (&A1, sizeof (struct R1));
  new_line ();

  put ("B1 :");
  dump (&B1, sizeof (struct R1));
  new_line ();

  put ("A2 :");
  dump (&A2, sizeof (struct R2));
  new_line ();

  put ("B2 :");
  dump (&B2, sizeof (struct R2));
  new_line ();

  if (A1.I != B1.I || A1.N.C1 != B1.N.C1) abort();

  if (A2.I != B2.I || A2.N.C1 != B2.N.C1) abort ();

  return 0;
}
