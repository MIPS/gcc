/* { dg-do run { target lto } } */
/* { dg-additional-options "-fipa-pta -flto -flto-partition=max" } */
/* Override the compiler's "avoid offloading" decision.
   { dg-additional-options "-foffload-force" } */

#include "data-clauses-kernels.c"
