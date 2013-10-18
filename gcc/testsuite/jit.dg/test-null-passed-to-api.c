#include <stdlib.h>
#include <stdio.h>

#include "libgccjit.h"

#include "harness.h"

int
code_making_callback (gcc_jit_context *ctxt, void *user_data)
{
  /* Trigger an API error by passing bad data.  */
  gcc_jit_context_new_function (ctxt, NULL,
                                GCC_JIT_FUNCTION_EXPORTED,
                                NULL, /* error: this must be non-NULL */
                                "hello_world",
                                0, NULL,
                                0);

  /* Client code erroneously considers that it succeeded, so returns 0.  */
  return 0;
}

void
verify_code (gcc_jit_result *result)
{
  /* Ensure that the bad API usage prevents the API giving a bogus
     result back.  */
  CHECK_VALUE (result, NULL);
}

