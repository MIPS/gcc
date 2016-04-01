/* With -O0, variables are on the stack, not in registers.  Check that worker
   state propagation handles the stack frame.  */

int
main (int argc, char *argv[])
{
  int w0 = 0;
  int w1 = 0;
  int w2 = 0;
  int w3 = 0;
  int w4 = 0;
  int w5 = 0;
  int w6 = 0;
  int w7 = 0;

  int i;

#pragma acc parallel num_gangs (1) num_workers (8) copy (w0, w1, w2, w3, w4, w5, w6, w7)
  {
    int internal = 100;

#pragma acc loop worker
    for (i = 0; i < 8; i++)
      {
	switch (i)
	  {
	  case 0: w0 = internal; break;
	  case 1: w1 = internal; break;
	  case 2: w2 = internal; break;
	  case 3: w3 = internal; break;
	  case 4: w4 = internal; break;
	  case 5: w5 = internal; break;
	  case 6: w6 = internal; break;
	  case 7: w7 = internal; break;
	  default: break;
	  }
      }
  }

  if (w0 != 100
      || w1 != 100
      || w2 != 100
      || w3 != 100
      || w4 != 100
      || w5 != 100
      || w6 != 100
      || w7 != 100)
    __builtin_abort ();

  return 0;
}
