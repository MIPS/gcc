#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "libgccjit.h"

#include "harness.h"

static void
create_test_of_builtin_strcmp (gcc_jit_context *ctxt)
{
  /* Let's try to inject the equivalent of:
       int
       test_of_builtin_strcmp (const char *a, const char *b)
       {
         return __builtin_strcmp (a, b);
       }
  */
  gcc_jit_type *int_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_INT);
  gcc_jit_type *const_char_ptr_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_CONST_CHAR_PTR);

  /* Get the built-in function.  */
  gcc_jit_function *builtin_fn =
    gcc_jit_context_get_builtin_function (ctxt, "strcmp");

  CHECK_STRING_VALUE (
    gcc_jit_object_get_debug_string (gcc_jit_function_as_object (builtin_fn)),
    "strcmp");

  /* Build the test_fn.  */
  gcc_jit_param *param_a =
    gcc_jit_context_new_param (ctxt, NULL, const_char_ptr_type, "a");
  gcc_jit_param *param_b =
    gcc_jit_context_new_param (ctxt, NULL, const_char_ptr_type, "b");
  gcc_jit_param *params[2] = {param_a, param_b};
  gcc_jit_function *test_fn =
    gcc_jit_context_new_function (ctxt, NULL,
                                  GCC_JIT_FUNCTION_EXPORTED,
                                  int_type,
                                  "test_of_builtin_strcmp",
                                  2, params,
                                  0);
  gcc_jit_rvalue *args[2] = {gcc_jit_param_as_rvalue (param_a),
			     gcc_jit_param_as_rvalue (param_b)};
  gcc_jit_rvalue *call =
    gcc_jit_context_new_call (ctxt,
                              NULL,
                              builtin_fn,
                              2, args);
  CHECK_STRING_VALUE (
    gcc_jit_object_get_debug_string (gcc_jit_rvalue_as_object (call)),
    "strcmp (a, b)");

  gcc_jit_function_add_return (test_fn, NULL, call);
}

static void
create_test_of_builtin_trig (gcc_jit_context *ctxt)
{
  /* Let's try to inject the equivalent of:
       int
       test_of_builtin_trig (double theta)
       {
         return 2 * sin (theta) * cos (theta);
       }
       (in theory, optimizable to sin (2 * theta))
  */
  gcc_jit_type *double_t =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_DOUBLE);

  /* Get the built-in functions.  */
  gcc_jit_function *builtin_sin =
    gcc_jit_context_get_builtin_function (ctxt, "sin");
  gcc_jit_function *builtin_cos =
    gcc_jit_context_get_builtin_function (ctxt, "cos");

  /* Build the test_fn.  */
  gcc_jit_param *param_theta =
    gcc_jit_context_new_param (ctxt, NULL, double_t, "theta");
  gcc_jit_function *test_fn =
    gcc_jit_context_new_function (ctxt, NULL,
                                  GCC_JIT_FUNCTION_EXPORTED,
                                  double_t,
                                  "test_of_builtin_trig",
                                  1, &param_theta,
                                  0);
  gcc_jit_rvalue *args[1] = {gcc_jit_param_as_rvalue (param_theta)};
  gcc_jit_rvalue *two =
    gcc_jit_context_new_rvalue_from_int (ctxt, double_t, 2);
  gcc_jit_rvalue *ret =
    gcc_jit_context_new_binary_op (
      ctxt, NULL,
      GCC_JIT_BINARY_OP_MULT,
      double_t,
      two,
      gcc_jit_context_new_binary_op (
        ctxt, NULL,
	GCC_JIT_BINARY_OP_MULT,
	double_t,
	gcc_jit_context_new_call (ctxt, NULL,
				  builtin_sin,
				  1, args),
	gcc_jit_context_new_call (ctxt, NULL,
				  builtin_cos,
				  1, args)));
  CHECK_STRING_VALUE (
    gcc_jit_object_get_debug_string (gcc_jit_rvalue_as_object (ret)),
    "(double)2 * sin (theta) * cos (theta)");

  gcc_jit_function_add_return (test_fn, NULL, ret);
}

static void
create_use_of_builtins (gcc_jit_context *ctxt)
{
  create_test_of_builtin_strcmp (ctxt);
  create_test_of_builtin_trig (ctxt);
}

static void
create_use_of_void_return (gcc_jit_context *ctxt)
{
  /* Let's try to inject the equivalent of:
       void
       test_of_void_return (int *out)
       {
         *out = 1;
	 return;
	 *out = 2;
       }
     where the second assignment is unreachable.
  */
  gcc_jit_type *void_t =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_VOID);
  gcc_jit_type *int_t =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_INT);
  gcc_jit_type *int_ptr_t =
    gcc_jit_type_get_pointer (int_t);

  /* Build the test_fn.  */
  gcc_jit_param *param_out =
    gcc_jit_context_new_param (ctxt, NULL, int_ptr_t, "out");
  gcc_jit_function *test_fn =
    gcc_jit_context_new_function (ctxt, NULL,
                                  GCC_JIT_FUNCTION_EXPORTED,
                                  void_t,
                                  "test_of_void_return",
                                  1, &param_out,
                                  0);
  gcc_jit_function_add_assignment (
    test_fn, NULL,
    /* "*out = ..." */
    gcc_jit_rvalue_dereference (gcc_jit_param_as_rvalue (param_out),
				NULL),
    gcc_jit_context_one (ctxt, int_t));
  gcc_jit_function_add_void_return (test_fn, NULL);
  gcc_jit_function_add_assignment (
    test_fn, NULL,
    /* "*out = ..." */
    gcc_jit_rvalue_dereference (gcc_jit_param_as_rvalue (param_out),
				NULL),
    gcc_jit_context_new_rvalue_from_int (ctxt, int_t, 2));
}

void
create_code (gcc_jit_context *ctxt, void *user_data)
{
  create_use_of_builtins (ctxt);
  create_use_of_void_return (ctxt);
}

static void
verify_test_of_builtin_strcmp (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  typedef int (*fn_type) (const char *, const char *);
  CHECK_NON_NULL (result);

  fn_type test_of_builtin_strcmp =
    (fn_type)gcc_jit_result_get_code (result, "test_of_builtin_strcmp");
  CHECK_NON_NULL (test_of_builtin_strcmp);

  /* Verify that it correctly called strcmp.  */
  CHECK_VALUE (test_of_builtin_strcmp ("foo", "foo"), 0);
  CHECK (test_of_builtin_strcmp ("foo", "bar") > 0);
  CHECK (test_of_builtin_strcmp ("bar", "foo") < 0);
}

static void
verify_test_of_builtin_trig (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  typedef double (*fn_type) (double);
  CHECK_NON_NULL (result);

  fn_type test_of_builtin_trig =
    (fn_type)gcc_jit_result_get_code (result, "test_of_builtin_trig");
  CHECK_NON_NULL (test_of_builtin_trig);

  /* Verify that it correctly computes
        sin (2 * theta)
     (perhaps calling sin and cos). */
  CHECK_DOUBLE_VALUE (test_of_builtin_trig (0.0         ),  0.0);
  CHECK_DOUBLE_VALUE (test_of_builtin_trig (M_PI_4      ),  1.0);
  CHECK_DOUBLE_VALUE (test_of_builtin_trig (M_PI_2      ),  0.0);
  CHECK_DOUBLE_VALUE (test_of_builtin_trig (M_PI_4 * 3.0), -1.0);
  CHECK_DOUBLE_VALUE (test_of_builtin_trig (M_PI        ),  0.0);
}

static void
verify_use_of_builtins (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  verify_test_of_builtin_strcmp (ctxt, result);
  verify_test_of_builtin_trig (ctxt, result);
}

static void
verify_void_return (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  typedef void (*fn_type) (int *);
  CHECK_NON_NULL (result);

  fn_type test_of_void_return =
    (fn_type)gcc_jit_result_get_code (result, "test_of_void_return");
  CHECK_NON_NULL (test_of_void_return);

  int i;
  test_of_void_return (&i);
  CHECK_VALUE (i, 1); /* ensure correct value was written back */
}

void
verify_code (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  verify_use_of_builtins (ctxt, result);
  verify_void_return (ctxt, result);
}
