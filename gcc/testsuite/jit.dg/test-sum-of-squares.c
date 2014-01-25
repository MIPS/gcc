#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libgccjit.h"

#include "harness.h"

void
create_code (gcc_jit_context *ctxt, void *user_data)
{
  /*
    Simple sum-of-squares, to test conditionals and looping

    int loop_test (int n)
    {
      int i;
      int sum = 0;
      for (i = 0; i < n ; i ++)
      {
	sum += i * i;
      }
      return sum;
   */
  gcc_jit_type *the_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_INT);
  gcc_jit_type *return_type = the_type;

  gcc_jit_param *n =
    gcc_jit_context_new_param (ctxt, NULL, the_type, "n");
  gcc_jit_param *params[1] = {n};
  gcc_jit_function *func =
    gcc_jit_context_new_function (ctxt, NULL,
				  GCC_JIT_FUNCTION_EXPORTED,
				  return_type,
				  "loop_test",
				  1, params, 0);

  /* Build locals:  */
  gcc_jit_lvalue *i =
    gcc_jit_function_new_local (func, NULL, the_type, "i");
  gcc_jit_lvalue *sum =
    gcc_jit_function_new_local (func, NULL, the_type, "sum");

  /* Create forward label: */
  gcc_jit_label *label_after_loop =
    gcc_jit_function_new_forward_label (func, "after_loop");

  /* sum = 0; */
  gcc_jit_function_add_assignment (
    func, NULL,
    sum,
    gcc_jit_context_new_rvalue_from_int (ctxt, the_type, 0));

  /* i = 0; */
  gcc_jit_function_add_assignment (
    func, NULL,
    i,
    gcc_jit_context_new_rvalue_from_int (ctxt, the_type, 0));


  /* label "cond:" */
  gcc_jit_label *label_cond =
    gcc_jit_function_add_label (func, NULL, "cond");

  /* if (i >= n) */
  gcc_jit_function_add_conditional (
    func, NULL,
    gcc_jit_context_new_comparison (
       ctxt, NULL,
       GCC_JIT_COMPARISON_GE,
       gcc_jit_lvalue_as_rvalue (i),
       gcc_jit_param_as_rvalue (n)),
    label_after_loop,
    NULL);

  /* sum += i * i */
  gcc_jit_function_add_assignment (
    func, NULL,
    sum,
    gcc_jit_context_new_binary_op (
      ctxt, NULL,
      GCC_JIT_BINARY_OP_PLUS, the_type,
      gcc_jit_lvalue_as_rvalue (sum),
      gcc_jit_context_new_binary_op (
	 ctxt, NULL,
	 GCC_JIT_BINARY_OP_MULT, the_type,
	 gcc_jit_lvalue_as_rvalue (i),
	 gcc_jit_lvalue_as_rvalue (i))));

  /* i++ */
  gcc_jit_function_add_assignment (
    func, NULL,
    i,
    gcc_jit_context_new_binary_op (
      ctxt, NULL,
      GCC_JIT_BINARY_OP_PLUS, the_type,
      gcc_jit_lvalue_as_rvalue (i),
      gcc_jit_context_new_rvalue_from_int (
	ctxt,
	the_type,
	1)));

  /* goto label_cond; */
  gcc_jit_function_add_jump (
    func, NULL,
    label_cond);

  /* label "after_loop:" */
  gcc_jit_function_place_forward_label (func, NULL, label_after_loop);

  /* return sum */
  gcc_jit_function_add_return (
    func,
    NULL,
    gcc_jit_lvalue_as_rvalue (sum));
}

void
verify_code (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  typedef int (*loop_test_fn_type) (int);
  CHECK_NON_NULL (result);
  loop_test_fn_type loop_test =
    (loop_test_fn_type)gcc_jit_result_get_code (result, "loop_test");
  CHECK_NON_NULL (loop_test);
  int val = loop_test (10);
  printf("loop_test returned: %d\n", val);
  CHECK_VALUE (val, 285);
}
