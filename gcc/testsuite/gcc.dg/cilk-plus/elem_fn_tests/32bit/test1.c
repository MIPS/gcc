/* { dg-final { scan-assembler "_ZGV"  } } */

#include <stdlib.h>
#define My_Type float
__attribute__ ((vector(vectorlength(4), processor (pentium_4), uniform (x,y)))) My_Type ef_add (My_Type x, My_Type y);

My_Type vhx2[100];

int
main (int argc, char **argv)
{
  My_Type vhx[100];
  int ii = 9;

  if (argc == 1) 
    for (ii = 0; ii < 10; ii++) 
      vhx[ii] = argc;

  for (ii = 0; ii < 10; ii++)
    vhx2[ii] = ef_add(argc, argc);
 
  for (ii = 0; ii < 10; ii++)
    if (vhx2[ii] != (argc + argc))
      abort ();
  return 0;
}

