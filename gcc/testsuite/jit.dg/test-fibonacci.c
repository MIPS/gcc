#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libgccjit.h"

#include "harness.h"

int
code_making_callback (gcc_jit_context *ctxt, void *user_data)
{
  /* Let's try to inject the equivalent of:

      fib.c: 1:   int
      fib.c: 2:   my_fibonacci (int x)
      fib.c: 3:   {
      fib.c: 4:     if (x < 2)
      fib.c: 5:       return x;
      fib.c: 6:     else
      fib.c: 7        return my_fibonacci (x - 1) + my_fibonacci (x - 2);
      fib.c: 8    }
   */
  gcc_jit_type *the_type = gcc_jit_context_get_int_type (ctxt);
  gcc_jit_type *return_type = the_type;

  gcc_jit_param *x =
    gcc_jit_context_new_param (ctxt, NULL, the_type, "x");
  gcc_jit_param *params[1] = {x};
  gcc_jit_function *func =
    gcc_jit_context_new_function (ctxt, NULL,
				  GCC_JIT_FUNCTION_EXPORTED,
				  return_type,
				  "my_fibonacci",
				  1, params, 0);

 /* Create forward labels: */
  gcc_jit_label *label_true =
    gcc_jit_function_new_forward_label (func, NULL);

  gcc_jit_label *label_false =
    gcc_jit_function_new_forward_label (func, NULL);

 /* if (x < 2) */
  gcc_jit_function_add_conditional (
    func, NULL,
    gcc_jit_context_new_comparison (
      ctxt, NULL,
      GCC_JIT_COMPARISON_LT,
      gcc_jit_param_as_rvalue (x),
      gcc_jit_context_new_rvalue_from_int (
	ctxt,
	the_type,
	2)),
    label_true,
    label_false);

  /* true branch: */
  gcc_jit_function_place_forward_label (func, label_true);
  /* return x */
  gcc_jit_function_add_return (
    func,
    NULL,
    gcc_jit_param_as_rvalue (x));

  /* false branch: */
  gcc_jit_function_place_forward_label (func, label_false);
  gcc_jit_rvalue *x_minus_1 =
    gcc_jit_context_new_binary_op (
      ctxt, NULL,
      GCC_JIT_BINARY_OP_MINUS, the_type,
      gcc_jit_param_as_rvalue (x),
      gcc_jit_context_new_rvalue_from_int (
	ctxt,
	the_type,
	1));
  gcc_jit_rvalue *x_minus_2 =
    gcc_jit_context_new_binary_op (
      ctxt, NULL,
      GCC_JIT_BINARY_OP_MINUS, the_type,
      gcc_jit_param_as_rvalue (x),
      gcc_jit_context_new_rvalue_from_int (
	ctxt,
	the_type,
	2));
  gcc_jit_function_add_return (
    func,
    NULL,
    gcc_jit_context_new_binary_op (
      ctxt, NULL,
      GCC_JIT_BINARY_OP_PLUS, the_type,
      /* my_fibonacci (x - 1) */
      gcc_jit_context_new_call (
        ctxt, NULL,
        func,
        1, &x_minus_1),
      /* my_fibonacci (x - 2) */
      gcc_jit_context_new_call (
        ctxt, NULL,
        func,
        1, &x_minus_2)));
  return 0;
}

void
verify_code (gcc_jit_result *result)
{
  typedef int (*my_fibonacci_fn_type) (int);
  CHECK_NON_NULL (result);
  my_fibonacci_fn_type my_fibonacci =
    (my_fibonacci_fn_type)gcc_jit_result_get_code (result, "my_fibonacci");
  CHECK_NON_NULL (my_fibonacci);
  int val = my_fibonacci (10);
  printf("my_fibonacci returned: %d\n", val);
  CHECK_VALUE (val, 55);
}
