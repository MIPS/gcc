#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libgccjit.h"

#include "harness.h"

void
create_code (gcc_jit_context *ctxt, void *user_data)
{
  /* Let's try to inject the equivalent of:

	double
	my_dot_product (int n, double *a, double *b)
	{
	  double result = 0.;
	  for (int i = 0; i < n; i++)
	    result += a[i] * b[i];
	  return result
	}

     and see what the optimizer can do.  */
  gcc_jit_type *val_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_DOUBLE);
  gcc_jit_type *ptr_type = gcc_jit_type_get_pointer (val_type);
  gcc_jit_type *int_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_INT);

  gcc_jit_type *return_type = val_type;
  gcc_jit_param *param_n =
    gcc_jit_context_new_param (ctxt, NULL, int_type, "n");
  gcc_jit_param *param_a =
    gcc_jit_context_new_param (ctxt, NULL, ptr_type, "a");
  gcc_jit_param *param_b =
    gcc_jit_context_new_param (ctxt, NULL, ptr_type, "b");
  gcc_jit_param *params[3] = {param_n, param_a, param_b};
  gcc_jit_function *func =
    gcc_jit_context_new_function (ctxt, NULL,
				  GCC_JIT_FUNCTION_EXPORTED,
				  return_type,
				  "my_dot_product",
				  3, params, 0);

  /* Build: "double result = 0.;" */
  gcc_jit_lvalue *result =
    gcc_jit_function_new_local (func, NULL, val_type, "result");

  gcc_jit_function_add_assignment (func, NULL,
    result,
    gcc_jit_context_zero (ctxt, val_type));

  /* Build: "for (int i = 0; i < n; i++)" */
  gcc_jit_lvalue *i =
    gcc_jit_function_new_local (func, NULL, int_type, "i");
  gcc_jit_function_add_assignment (func, NULL,
    i, gcc_jit_context_zero (ctxt, int_type));
  gcc_jit_loop *loop = gcc_jit_function_new_loop (func, NULL,
    /* (i < n) */
    gcc_jit_context_new_comparison (
      ctxt, NULL,
      GCC_JIT_COMPARISON_LT,
      gcc_jit_lvalue_as_rvalue (i),
      gcc_jit_param_as_rvalue (param_n)));

  /* Build: "result += a[i] * b[i];" */
  gcc_jit_function_add_assignment_op (
    func, NULL,
    result,
    GCC_JIT_BINARY_OP_PLUS,
    gcc_jit_context_new_binary_op (
      ctxt, NULL,
      GCC_JIT_BINARY_OP_MULT,
      val_type,
      gcc_jit_lvalue_as_rvalue (
	gcc_jit_context_new_array_access (
          ctxt, NULL,
	  gcc_jit_param_as_rvalue (param_a),
	  gcc_jit_lvalue_as_rvalue (i))),
      gcc_jit_lvalue_as_rvalue (
	gcc_jit_context_new_array_access (
          ctxt, NULL,
	  gcc_jit_param_as_rvalue (param_b),
	  gcc_jit_lvalue_as_rvalue (i)))));

  /* Build: "i++" */
  gcc_jit_function_add_assignment_op (
    func, NULL,
    i,
    GCC_JIT_BINARY_OP_PLUS,
    gcc_jit_context_one (ctxt, int_type));

  gcc_jit_loop_end (loop, NULL);

  /* Build: "return result;" */
  gcc_jit_function_add_return (
    func,
    NULL,
    gcc_jit_lvalue_as_rvalue (result));
}

void
verify_code (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  typedef double (*my_dot_product_fn_type) (int n, double *a, double *b);
  CHECK_NON_NULL (result);

  my_dot_product_fn_type my_dot_product =
    (my_dot_product_fn_type)gcc_jit_result_get_code (result,
						     "my_dot_product");
  CHECK_NON_NULL (my_dot_product);
  double test_array[] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 10.};
  double val = my_dot_product (10, test_array, test_array);
  printf("my_dot_product returned: %f\n", val);
  CHECK_VALUE (val, 385.0);
}

