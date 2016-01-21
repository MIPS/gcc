/* Override the compiler's "avoid offloading" decision.
   { dg-additional-options "-foffload-force" } */

int
main (void)
{
#pragma acc kernels
  ;
}
