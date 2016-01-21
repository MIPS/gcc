/* Override the compiler's "avoid offloading" decision.
   { dg-additional-options "-foffload-force" } */

#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
  fprintf (stderr, "CheCKpOInT\n");
#pragma acc kernels
  {
    abort ();
  }

  return 0;
}

/* { dg-output "CheCKpOInT" } */
/* { dg-shouldfail ""  } */
