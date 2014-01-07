#include <stdio.h>

#include "init6.h"
#include "dump.h"

#pragma GCC diagnostic ignored "-Wscalar-storage-order"

int main (void)
{
  struct R1 A1 = My_R1;
  struct R2 A2 = My_R2;

  struct Nested1 N1;
  struct Nested2 N2;

  int C1;
  int C2;
  int C3;

  put ("A1 :");
  dump (&A1, sizeof (struct R1));
  new_line ();

  put ("A2 :");
  dump (&A2, sizeof (struct R2));
  new_line ();

  __builtin_memcpy (N1.A, A1.N.A, sizeof (int) * 3);
  C1 = N1.A[0];
  C2 = N1.A[1];
  C3 = N1.A[2];

  printf ("C1 : %d\n", C1);
  printf ("C2 : %d\n", C2);
  printf ("C3 : %d\n", C3);

  N1.A[0] = C1;
  N1.A[1] = C2;
  N1.A[2] = C3;
  __builtin_memcpy (A1.N.A, N1.A, sizeof (int) * 3);

  __builtin_memcpy (N2.A, A2.N.A, sizeof (int) * 3);
  C1 = N2.A[0];
  C2 = N2.A[1];
  C3 = N2.A[2];

  printf ("C1 : %d\n", C1);
  printf ("C2 : %d\n", C2);
  printf ("C3 : %d\n", C3);

  N2.A[0] = C1;
  N2.A[1] = C2;
  N2.A[2] = C3;
  __builtin_memcpy (A2.N.A, N2.A, sizeof (int) * 3);

  put ("A1 :");
  dump (&A1, sizeof (struct R1));
  new_line ();

  put ("A2 :");
  dump (&A2, sizeof (struct R2));
  new_line ();

  return 0;
}
