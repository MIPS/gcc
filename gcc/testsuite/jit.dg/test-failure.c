#include <stdlib.h>
#include <stdio.h>

#include "libgccjit.h"

#include "harness.h"

int
code_making_callback (gcc_jit_context *ctxt, void *user_data)
{
  return 1;
}

void
verify_code (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  CHECK_VALUE (result, NULL);
}

