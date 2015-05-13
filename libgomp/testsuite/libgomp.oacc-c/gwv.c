#include <stdlib.h>
#include <openacc.h>

extern int __builtin_GOACC_ntid (int);
extern int __builtin_GOACC_tid (int);
extern int __builtin_GOACC_nctaid (int);
extern int __builtin_GOACC_ctaid (int);

int main ()
{
  int gangs = -1, vectors = -1;
  int GANGS = 100, VECTORS = 64;

#pragma acc parallel num_gangs (GANGS) vector_length (VECTORS) copyout (gangs, vectors)
  {
    gangs = __builtin_GOACC_nctaid (0);
    vectors = __builtin_GOACC_ntid (0);
  }

  if (acc_get_device_type () == acc_device_nvidia)
    {
      if (gangs != GANGS)
	abort ();
       if (vectors != VECTORS)
 	abort ();
    }
  else
    {
      if (gangs != 0)
	abort ();
       if (vectors != 0)
	abort ();
    }
}
