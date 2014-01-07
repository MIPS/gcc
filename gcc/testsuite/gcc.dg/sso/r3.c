#include <stdio.h>

#include "init3.h"
#include "dump.h"

extern void abort (void);

int Get_Elem1 (struct R1 R)
{
  struct R1 Tmp = R;
  return Tmp.I;
}

void Set_Elem1 (struct R1 *R, int I)
{
  struct R1 Tmp = *R;
  Tmp.I = I;
  *R = Tmp;
}

int Get_Elem2 (struct R2 R)
{
  struct R2 Tmp = R;
  return Tmp.I;
}

void Set_Elem2 (struct R2 *R, int I)
{
  struct R2 Tmp = *R;
  Tmp.I = I;
  *R = Tmp;
}

int main (void)
{
  struct R1 A1 = My_R1;
  struct R2 A2 = My_R2;

  put ("A1 :");
  dump (&A1, sizeof (struct R1));
  new_line ();

  put ("A2 :");
  dump (&A2, sizeof (struct R2));
  new_line ();

  if (Get_Elem1 (A1) != 0x78ABCDEF) abort ();

  Set_Elem1 (&A1, 0xCD0034);
  if (Get_Elem1 (A1) != 0xCD0034) abort ();

  if (Get_Elem2 (A2) != 0x78ABCDEF) abort ();

  Set_Elem2 (&A2, 0xCD0034);
  if (Get_Elem2 (A2) != 0xCD0034) abort ();

  return 0;
}
