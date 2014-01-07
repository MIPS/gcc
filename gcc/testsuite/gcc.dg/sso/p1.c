#include <stdio.h>

#include "init1.h"
#include "dump.h"

int main (void)
{
  struct R1 Local_R1;
  struct R2 Local_R2;

  put ("My_R1    :");
  dump (&My_R1, sizeof (struct R1));
  new_line ();

  put ("My_R2    :");
  dump (&My_R2, sizeof (struct R2));
  new_line ();

  Local_R1 = My_R1;
  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();

  Local_R2 = My_R2;
  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();

  Local_R1.I = 0x12345678;

  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();

  Local_R2.I = 0x12345678;

  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();

  Local_R2.I = Local_R1.I;

  put ("Local_R1 :");
  dump (&Local_R1, sizeof (struct R1));
  new_line ();

  Local_R1.I = Local_R2.I;

  put ("Local_R2 :");
  dump (&Local_R2, sizeof (struct R2));
  new_line ();

  return 0;
}
