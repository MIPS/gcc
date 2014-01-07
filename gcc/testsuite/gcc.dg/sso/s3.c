#include <stdio.h>

#include "init3.h"
#include "dump.h"

int main (void)
{
  struct R1 A1 = My_R1;
  struct R2 A2 = My_R2;

  struct Nested1 N1;
  struct Nested2 N2;

  unsigned C1;
  unsigned C2;
  unsigned C3;

  put ("A1 :");
  dump (&A1, sizeof (struct R1));
  new_line ();

  put ("A2 :");
  dump (&A2, sizeof (struct R2));
  new_line ();

  N1 = A1.N;
  C1 = N1.C1;
  C2 = N1.C2;
  C3 = N1.C3;

  printf ("C1 : %d\n", C1);
  printf ("C2 : %d\n", C2);
  printf ("C3 : %d\n", C3);

  N1.C1 = C1;
  N1.C2 = C2;
  N1.C3 = C3;
  A1.N = N1;

  N2 = A2.N;
  C1 = N2.C1;
  C2 = N2.C2;
  C3 = N2.C3;

  printf ("C1 : %d\n", C1);
  printf ("C2 : %d\n", C2);
  printf ("C3 : %d\n", C3);

  N2.C1 = C1;
  N2.C2 = C2;
  N2.C3 = C3;
  A2.N = N2;

  put ("A1 :");
  dump (&A1, sizeof (struct R1));
  new_line ();

  put ("A2 :");
  dump (&A2, sizeof (struct R2));
  new_line ();

  return 0;
}
