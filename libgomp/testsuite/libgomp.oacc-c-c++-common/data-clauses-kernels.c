/* Override the compiler's "avoid offloading" decision.
   { dg-additional-options "-foffload-force" } */

#define CONSTRUCT kernels
#include "data-clauses.h"
