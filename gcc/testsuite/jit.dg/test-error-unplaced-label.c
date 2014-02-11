#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libgccjit.h"

#include "harness.h"

void
create_code (gcc_jit_context *ctxt, void *user_data)
{
  /* Let's try to inject the equivalent of:

      void
      test_fn (void)
      {
        goto foo;
      }

     and verify that an error is issued due to label "foo" not being
     placed.
   */
  gcc_jit_type *void_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_VOID);

  gcc_jit_function *func =
    gcc_jit_context_new_function (ctxt, NULL,
				  GCC_JIT_FUNCTION_EXPORTED,
				  void_type,
				  "test_fn",
				  0, NULL, 0);

  /* Create forward label: */
  gcc_jit_label *unplaced_label =
    gcc_jit_function_new_forward_label (func, "foo");

  /* Use it (but never place it): */
  gcc_jit_function_add_jump (func, NULL, unplaced_label);
}

void
verify_code (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  CHECK_VALUE (result, NULL);

  /* Verify that the correct error message was emitted.  */
  CHECK_STRING_VALUE (gcc_jit_context_get_first_error (ctxt),
		      "unplaced label within test_fn: foo");
}

