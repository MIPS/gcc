/* Override the compiler's "avoid offloading" decision.
   { dg-additional-options "-foffload-force" } */

#include <stdlib.h>

#define EXEC_DIRECTIVE kernels
#include "data-clauses.h"
