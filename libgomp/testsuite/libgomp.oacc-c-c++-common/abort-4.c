/* Override the compiler's "avoid offloading" decision.
   { dg-additional-options "-foffload-force" } */

#include <stdlib.h>

int
main (int argc, char **argv)
{

#pragma acc kernels
  {
    if (argc != 1)
      abort ();
  }

  return 0;
}

