#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libgccjit.h"

#include "harness.h"

void
create_code (gcc_jit_context *ctxt, void *user_data)
{
  /* Let's try to inject the equivalent of:

      int
      test_fn (void)
      {
	foo:
	   return 0;

	foo:
	   return 1;

      }

     and verify that the 2nd attempt to place the label fails.
   */
  gcc_jit_type *int_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_INT);

  gcc_jit_function *func =
    gcc_jit_context_new_function (ctxt, NULL,
				  GCC_JIT_FUNCTION_EXPORTED,
				  int_type,
				  "test_fn",
				  0, NULL, 0);

  /* Create forward label: */
  gcc_jit_label *label =
    gcc_jit_function_new_forward_label (func, "foo");

  gcc_jit_function_place_forward_label (func, NULL, label);
  gcc_jit_function_add_return (func, NULL,
			       gcc_jit_context_zero (ctxt, int_type));

  /* Erroneous 2nd placement of label: */
  gcc_jit_function_place_forward_label (func, NULL, label);
  gcc_jit_function_add_return (func, NULL,
			       gcc_jit_context_one (ctxt, int_type));
}

void
verify_code (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  CHECK_VALUE (result, NULL);

  /* Verify that the correct error message was emitted.  */
  CHECK_STRING_VALUE (gcc_jit_context_get_first_error (ctxt),
		      "label foo has already been placed");
}
