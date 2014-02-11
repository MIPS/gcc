#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "libgccjit.h"

#include "harness.h"

/**********************************************************************
 Unary ops
 **********************************************************************/

static const char *
make_test_of_unary_op (gcc_jit_context *ctxt,
		       gcc_jit_type *type,
		       enum gcc_jit_unary_op op,
		       const char *funcname)
{
  /* Make a test function of the form:
       T test_unary_op (T a)
       {
	  return OP a;
       }
     and return a debug dump of the OP so that
     the caller can sanity-check the debug dump implementation.
  */
  gcc_jit_param *param_a =
    gcc_jit_context_new_param (ctxt, NULL, type, "a");
  gcc_jit_function *test_fn =
    gcc_jit_context_new_function (ctxt, NULL,
				  GCC_JIT_FUNCTION_EXPORTED,
				  type,
				  funcname,
				  1, &param_a,
				  0);
  gcc_jit_rvalue *unary_op = gcc_jit_context_new_unary_op (
    ctxt,
    NULL,
    op,
    type,
    gcc_jit_param_as_rvalue (param_a));

  gcc_jit_function_add_return (test_fn, NULL, unary_op);

  return gcc_jit_object_get_debug_string (
    gcc_jit_rvalue_as_object (unary_op));
}


static void
make_tests_of_unary_ops (gcc_jit_context *ctxt)
{
  gcc_jit_type *int_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_INT);

  CHECK_STRING_VALUE (
    make_test_of_unary_op (ctxt,
			   int_type,
			   GCC_JIT_UNARY_OP_MINUS,
			   "test_UNARY_OP_MINUS_on_int"),
    "-(a)");
  CHECK_STRING_VALUE (
    make_test_of_unary_op (ctxt,
			   int_type,
			   GCC_JIT_UNARY_OP_BITWISE_NEGATE,
			   "test_UNARY_OP_BITWISE_NEGATE_on_int"),
    "~(a)");
  CHECK_STRING_VALUE (
    make_test_of_unary_op (ctxt,
			   int_type,
			   GCC_JIT_UNARY_OP_LOGICAL_NEGATE,
			   "test_UNARY_OP_LOGICAL_NEGATE_on_int"),
    "!(a)");
}

static void
verify_unary_ops (gcc_jit_result *result)
{
  typedef int (*test_fn) (int);

  test_fn test_UNARY_OP_MINUS_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_UNARY_OP_MINUS_on_int");
  CHECK_NON_NULL (test_UNARY_OP_MINUS_on_int);
  CHECK_VALUE (test_UNARY_OP_MINUS_on_int (0), 0);
  CHECK_VALUE (test_UNARY_OP_MINUS_on_int (42), -42);
  CHECK_VALUE (test_UNARY_OP_MINUS_on_int (-5), 5);

  test_fn test_UNARY_OP_BITWISE_NEGATE_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_UNARY_OP_BITWISE_NEGATE_on_int");
  CHECK_NON_NULL (test_UNARY_OP_BITWISE_NEGATE_on_int);
  CHECK_VALUE (test_UNARY_OP_BITWISE_NEGATE_on_int (0), ~0);
  CHECK_VALUE (test_UNARY_OP_BITWISE_NEGATE_on_int (42), ~42);
  CHECK_VALUE (test_UNARY_OP_BITWISE_NEGATE_on_int (-5), ~-5);

  test_fn test_UNARY_OP_LOGICAL_NEGATE_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_UNARY_OP_LOGICAL_NEGATE_on_int");
  CHECK_NON_NULL (test_UNARY_OP_LOGICAL_NEGATE_on_int);
  CHECK_VALUE (test_UNARY_OP_LOGICAL_NEGATE_on_int (0), 1);
  CHECK_VALUE (test_UNARY_OP_LOGICAL_NEGATE_on_int (42), 0);
  CHECK_VALUE (test_UNARY_OP_LOGICAL_NEGATE_on_int (-5), 0);

}

/**********************************************************************
 Binary ops
 **********************************************************************/

static const char *
make_test_of_binary_op (gcc_jit_context *ctxt,
			gcc_jit_type *type,
			enum gcc_jit_binary_op op,
			const char *funcname)
{
  /* Make a test function of the form:
       T test_binary_op (T a, T b)
       {
	  return a OP b;
       }
  */
  gcc_jit_param *param_a =
    gcc_jit_context_new_param (ctxt, NULL, type, "a");
  gcc_jit_param *param_b =
    gcc_jit_context_new_param (ctxt, NULL, type, "b");
  gcc_jit_param *params[] = {param_a, param_b};
  gcc_jit_function *test_fn =
    gcc_jit_context_new_function (ctxt, NULL,
				  GCC_JIT_FUNCTION_EXPORTED,
				  type,
				  funcname,
				  2, params,
				  0);
  gcc_jit_rvalue *binary_op =
    gcc_jit_context_new_binary_op (
      ctxt,
      NULL,
      op,
      type,
      gcc_jit_param_as_rvalue (param_a),
      gcc_jit_param_as_rvalue (param_b));

  gcc_jit_function_add_return (test_fn, NULL, binary_op);

  return gcc_jit_object_get_debug_string (
    gcc_jit_rvalue_as_object (binary_op));
}


static void
make_tests_of_binary_ops (gcc_jit_context *ctxt)
{
  gcc_jit_type *int_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_INT);

  /* Test binary ops.  */
  CHECK_STRING_VALUE (
    make_test_of_binary_op (ctxt,
			    int_type,
			    GCC_JIT_BINARY_OP_PLUS,
			    "test_BINARY_OP_PLUS_on_int"),
    "a + b");
  CHECK_STRING_VALUE (
    make_test_of_binary_op (ctxt,
			    int_type,
			    GCC_JIT_BINARY_OP_MINUS,
			    "test_BINARY_OP_MINUS_on_int"),
    "a - b");
  CHECK_STRING_VALUE (
    make_test_of_binary_op (ctxt,
			    int_type,
			    GCC_JIT_BINARY_OP_MULT,
			    "test_BINARY_OP_MULT_on_int"),
    "a * b");
  CHECK_STRING_VALUE (
    make_test_of_binary_op (ctxt,
			    int_type,
			    GCC_JIT_BINARY_OP_DIVIDE,
			    "test_BINARY_OP_DIVIDE_on_int"),
    "a / b");
  /* TODO: test for DIVIDE on float or double */
  CHECK_STRING_VALUE (
    make_test_of_binary_op (ctxt,
			    int_type,
			    GCC_JIT_BINARY_OP_MODULO,
			    "test_BINARY_OP_MODULO_on_int"),
    "a % b");
  CHECK_STRING_VALUE (
    make_test_of_binary_op (ctxt,
			    int_type,
			    GCC_JIT_BINARY_OP_BITWISE_AND,
			    "test_BINARY_OP_BITWISE_AND_on_int"),
    "a & b");
  CHECK_STRING_VALUE (
    make_test_of_binary_op (ctxt,
			    int_type,
			    GCC_JIT_BINARY_OP_BITWISE_XOR,
			    "test_BINARY_OP_BITWISE_XOR_on_int"),
    "a ^ b");
  CHECK_STRING_VALUE (
    make_test_of_binary_op (ctxt,
			    int_type,
			    GCC_JIT_BINARY_OP_BITWISE_OR,
			    "test_BINARY_OP_BITWISE_OR_on_int"),
    "a | b");
  CHECK_STRING_VALUE (
    make_test_of_binary_op (ctxt,
			    int_type,
			    GCC_JIT_BINARY_OP_LOGICAL_AND,
			    "test_BINARY_OP_LOGICAL_AND_on_int"),
    "a && b");
  CHECK_STRING_VALUE (
    make_test_of_binary_op (ctxt,
			    int_type,
			    GCC_JIT_BINARY_OP_LOGICAL_OR,
			    "test_BINARY_OP_LOGICAL_OR_on_int"),
    "a || b");
}

static void
verify_binary_ops (gcc_jit_result *result)
{
  typedef int (*test_fn) (int, int);

  test_fn test_BINARY_OP_PLUS_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_BINARY_OP_PLUS_on_int");
  CHECK_NON_NULL (test_BINARY_OP_PLUS_on_int);
  CHECK_VALUE (test_BINARY_OP_PLUS_on_int (0, 0), 0);
  CHECK_VALUE (test_BINARY_OP_PLUS_on_int (1, 2), 3);
  CHECK_VALUE (test_BINARY_OP_PLUS_on_int (100, -1), 99);
  CHECK_VALUE (test_BINARY_OP_PLUS_on_int (-1, -4), -5);

  test_fn test_BINARY_OP_MINUS_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_BINARY_OP_MINUS_on_int");
  CHECK_NON_NULL (test_BINARY_OP_MINUS_on_int);
  CHECK_VALUE (test_BINARY_OP_MINUS_on_int (0, 0), 0);
  CHECK_VALUE (test_BINARY_OP_MINUS_on_int (1, 2), -1);
  CHECK_VALUE (test_BINARY_OP_MINUS_on_int (100, -1), 101);
  CHECK_VALUE (test_BINARY_OP_MINUS_on_int (-1, -4), 3);

  test_fn test_BINARY_OP_MULT_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_BINARY_OP_MULT_on_int");
  CHECK_NON_NULL (test_BINARY_OP_MULT_on_int);
  CHECK_VALUE (test_BINARY_OP_MULT_on_int (0, 0), 0);
  CHECK_VALUE (test_BINARY_OP_MULT_on_int (1, 2), 2);
  CHECK_VALUE (test_BINARY_OP_MULT_on_int (100, -1), -100);
  CHECK_VALUE (test_BINARY_OP_MULT_on_int (-1, -4), 4);
  CHECK_VALUE (test_BINARY_OP_MULT_on_int (7, 10), 70);

  test_fn test_BINARY_OP_DIVIDE_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_BINARY_OP_DIVIDE_on_int");
  CHECK_NON_NULL (test_BINARY_OP_DIVIDE_on_int);
  CHECK_VALUE (test_BINARY_OP_DIVIDE_on_int (7, 2), 3);
  CHECK_VALUE (test_BINARY_OP_DIVIDE_on_int (100, -1), (100 / -1));
  CHECK_VALUE (test_BINARY_OP_DIVIDE_on_int (-1, -4), (-1 / -4));
  CHECK_VALUE (test_BINARY_OP_DIVIDE_on_int (60, 5), 12);

  /* TODO: test for DIVIDE on float or double */

  test_fn test_BINARY_OP_MODULO_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_BINARY_OP_MODULO_on_int");
  CHECK_NON_NULL (test_BINARY_OP_MODULO_on_int);
  CHECK_VALUE (test_BINARY_OP_MODULO_on_int (7, 2), 1);
  CHECK_VALUE (test_BINARY_OP_MODULO_on_int (100, -1), (100 % -1));
  CHECK_VALUE (test_BINARY_OP_MODULO_on_int (-1, -4), (-1 % -4));
  CHECK_VALUE (test_BINARY_OP_MODULO_on_int (60, 5), 0);

  test_fn test_BINARY_OP_BITWISE_AND_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_BINARY_OP_BITWISE_AND_on_int");
  CHECK_NON_NULL (test_BINARY_OP_BITWISE_AND_on_int);
  CHECK_VALUE (test_BINARY_OP_BITWISE_AND_on_int (0xf0f0, 0x7777), 0x7070);

  test_fn test_BINARY_OP_BITWISE_XOR_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_BINARY_OP_BITWISE_XOR_on_int");
  CHECK_NON_NULL (test_BINARY_OP_BITWISE_XOR_on_int);
  CHECK_VALUE (test_BINARY_OP_BITWISE_XOR_on_int (0xf0f0, 0x7777), 0x8787);

  test_fn test_BINARY_OP_BITWISE_OR_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_BINARY_OP_BITWISE_OR_on_int");
  CHECK_NON_NULL (test_BINARY_OP_BITWISE_OR_on_int);
  CHECK_VALUE (test_BINARY_OP_BITWISE_OR_on_int (0xf0f0, 0x7777), 0xf7f7);

  test_fn test_BINARY_OP_LOGICAL_AND_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_BINARY_OP_LOGICAL_AND_on_int");
  CHECK_NON_NULL (test_BINARY_OP_LOGICAL_AND_on_int);
  CHECK_VALUE (test_BINARY_OP_LOGICAL_AND_on_int (0, 0), 0);
  CHECK_VALUE (test_BINARY_OP_LOGICAL_AND_on_int (42, 0), 0);
  CHECK_VALUE (test_BINARY_OP_LOGICAL_AND_on_int (0, -13), 0);
  CHECK_VALUE (test_BINARY_OP_LOGICAL_AND_on_int (1997, 1998), 1);

  test_fn test_BINARY_OP_LOGICAL_OR_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_BINARY_OP_LOGICAL_OR_on_int");
  CHECK_NON_NULL (test_BINARY_OP_LOGICAL_OR_on_int);
  CHECK_VALUE (test_BINARY_OP_LOGICAL_OR_on_int (0, 0), 0);
  CHECK_VALUE (test_BINARY_OP_LOGICAL_OR_on_int (42, 0), 1);
  CHECK_VALUE (test_BINARY_OP_LOGICAL_OR_on_int (0, -13), 1);
  CHECK_VALUE (test_BINARY_OP_LOGICAL_OR_on_int (1997, 1998), 1);
}

/**********************************************************************
 Comparisons
 **********************************************************************/

static const char *
make_test_of_comparison (gcc_jit_context *ctxt,
			 gcc_jit_type *type,
			 enum gcc_jit_comparison op,
			 const char *funcname)
{
  /* Make a test function of the form:
       bool test_comparison_op (T a, T b)
       {
	  return a OP b;
       }
  */
  gcc_jit_param *param_a =
    gcc_jit_context_new_param (ctxt, NULL, type, "a");
  gcc_jit_param *param_b =
    gcc_jit_context_new_param (ctxt, NULL, type, "b");
  gcc_jit_param *params[] = {param_a, param_b};
  gcc_jit_type *bool_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_BOOL);
  gcc_jit_function *test_fn =
    gcc_jit_context_new_function (ctxt, NULL,
				  GCC_JIT_FUNCTION_EXPORTED,
				  bool_type,
				  funcname,
				  2, params,
				  0);
  gcc_jit_rvalue *comparison =
    gcc_jit_context_new_comparison (
      ctxt,
      NULL,
      op,
      gcc_jit_param_as_rvalue (param_a),
      gcc_jit_param_as_rvalue (param_b));

  gcc_jit_function_add_return (test_fn, NULL, comparison);

  return gcc_jit_object_get_debug_string (
    gcc_jit_rvalue_as_object (comparison));
}

static void
make_tests_of_comparisons (gcc_jit_context *ctxt)
{
  gcc_jit_type *int_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_INT);

  CHECK_STRING_VALUE (
    make_test_of_comparison (ctxt,
			     int_type,
			     GCC_JIT_COMPARISON_EQ,
			     "test_COMPARISON_EQ_on_int"),
    "a == b");
  CHECK_STRING_VALUE (
    make_test_of_comparison (ctxt,
			     int_type,
			     GCC_JIT_COMPARISON_NE,
			     "test_COMPARISON_NE_on_int"),
    "a != b");
  CHECK_STRING_VALUE (
    make_test_of_comparison (ctxt,
			     int_type,
			     GCC_JIT_COMPARISON_LT,
			     "test_COMPARISON_LT_on_int"),
    "a < b");
  CHECK_STRING_VALUE (
    make_test_of_comparison (ctxt,
			     int_type,
			     GCC_JIT_COMPARISON_LE,
			     "test_COMPARISON_LE_on_int"),
    "a <= b");
  CHECK_STRING_VALUE (
    make_test_of_comparison (ctxt,
			     int_type,
			     GCC_JIT_COMPARISON_GT,
			     "test_COMPARISON_GT_on_int"),
    "a > b");
  CHECK_STRING_VALUE (
    make_test_of_comparison (ctxt,
			     int_type,
			     GCC_JIT_COMPARISON_GE,
			     "test_COMPARISON_GE_on_int"),
    "a >= b");
}

static void
verify_comparisons (gcc_jit_result *result)
{
  typedef bool (*test_fn) (int, int);

  test_fn test_COMPARISON_EQ_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_COMPARISON_EQ_on_int");
  CHECK_NON_NULL (test_COMPARISON_EQ_on_int);
  CHECK_VALUE (test_COMPARISON_EQ_on_int (0, 0), 1);
  CHECK_VALUE (test_COMPARISON_EQ_on_int (1, 2), 0);

  test_fn test_COMPARISON_NE_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_COMPARISON_NE_on_int");
  CHECK_NON_NULL (test_COMPARISON_NE_on_int);
  CHECK_VALUE (test_COMPARISON_NE_on_int (0, 0), 0);
  CHECK_VALUE (test_COMPARISON_NE_on_int (1, 2), 1);

  test_fn test_COMPARISON_LT_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_COMPARISON_LT_on_int");
  CHECK_NON_NULL (test_COMPARISON_LT_on_int);
  CHECK_VALUE (test_COMPARISON_LT_on_int (0, 0), 0);
  CHECK_VALUE (test_COMPARISON_LT_on_int (1, 2), 1);
  CHECK_VALUE (test_COMPARISON_LT_on_int (2, 1), 0);
  CHECK_VALUE (test_COMPARISON_LT_on_int (-2, 1), 1);

  test_fn test_COMPARISON_LE_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_COMPARISON_LE_on_int");
  CHECK_NON_NULL (test_COMPARISON_LE_on_int);
  CHECK_VALUE (test_COMPARISON_LE_on_int (0, 0), 1);
  CHECK_VALUE (test_COMPARISON_LE_on_int (1, 2), 1);
  CHECK_VALUE (test_COMPARISON_LE_on_int (2, 1), 0);

  test_fn test_COMPARISON_GT_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_COMPARISON_GT_on_int");
  CHECK_NON_NULL (test_COMPARISON_GT_on_int);
  CHECK_VALUE (test_COMPARISON_GT_on_int (0, 0), 0);
  CHECK_VALUE (test_COMPARISON_GT_on_int (1, 2), 0);
  CHECK_VALUE (test_COMPARISON_GT_on_int (2, 1), 1);

  test_fn test_COMPARISON_GE_on_int =
    (test_fn)gcc_jit_result_get_code (result,
				      "test_COMPARISON_GE_on_int");
  CHECK_NON_NULL (test_COMPARISON_GE_on_int);
  CHECK_VALUE (test_COMPARISON_GE_on_int (0, 0), 1);
  CHECK_VALUE (test_COMPARISON_GE_on_int (1, 2), 0);
  CHECK_VALUE (test_COMPARISON_GE_on_int (2, 1), 1);
}

/**********************************************************************
 Dereferences
 **********************************************************************/

static void
make_tests_of_dereferences (gcc_jit_context *ctxt)
{
  /*
       int test_dereference_read (int *ptr)
       {
	 return *ptr;
       }
       void test_dereference_write (int *ptr, int i)
       {
	 *ptr = i;
       }
  */
  gcc_jit_type *void_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_VOID);
  gcc_jit_type *int_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_INT);
  gcc_jit_type *int_ptr_type =
    gcc_jit_type_get_pointer (int_type);
  {
    gcc_jit_param *param_ptr =
      gcc_jit_context_new_param (ctxt, NULL, int_ptr_type, "ptr");
    gcc_jit_function *test_dereference_read =
      gcc_jit_context_new_function (ctxt, NULL,
				    GCC_JIT_FUNCTION_EXPORTED,
				    int_type,
				    "test_dereference_read",
				    1, &param_ptr,
				    0);
    gcc_jit_function_add_return (
      test_dereference_read,
      NULL,
      gcc_jit_lvalue_as_rvalue (
	gcc_jit_rvalue_dereference (
	  gcc_jit_param_as_rvalue (param_ptr),
	  NULL)));
  }

  {
    gcc_jit_param *param_ptr =
      gcc_jit_context_new_param (ctxt, NULL, int_ptr_type, "ptr");
    gcc_jit_param *param_i =
      gcc_jit_context_new_param (ctxt, NULL, int_type, "i");
    gcc_jit_param *params[] = {param_ptr, param_i};
    gcc_jit_function *test_dereference_write =
      gcc_jit_context_new_function (ctxt, NULL,
				    GCC_JIT_FUNCTION_EXPORTED,
				    void_type,
				    "test_dereference_write",
				    2, params,
				    0);
    gcc_jit_function_add_assignment (
      test_dereference_write,
      NULL,
      gcc_jit_rvalue_dereference (
	gcc_jit_param_as_rvalue (param_ptr),
	NULL),
      gcc_jit_param_as_rvalue (param_i));
  }
}

static void
verify_dereferences (gcc_jit_result *result)
{
  int a = 42;
  int b = -99;

  {
    typedef int (*test_read) (int *);
    test_read test_dereference_read =
      (test_read)gcc_jit_result_get_code (result,
					  "test_dereference_read");
    CHECK_NON_NULL (test_dereference_read);
    CHECK_VALUE (test_dereference_read (&a), 42);
    CHECK_VALUE (test_dereference_read (&b), -99);
  }

 {
    typedef void (*test_write) (int *, int);
    test_write test_dereference_write =
      (test_write)gcc_jit_result_get_code (result,
					  "test_dereference_write");
    CHECK_NON_NULL (test_dereference_write);
    test_dereference_write (&a, -55);
    CHECK_VALUE (a, -55);

    test_dereference_write (&b, 404);
    CHECK_VALUE (b, 404);
  }
}

/**********************************************************************
 gcc_jit_lvalue_get_address
 **********************************************************************/

int test_global;
static void
make_test_of_get_address (gcc_jit_context *ctxt)
{
  /*
     void *test_get_address (void)
     {
	return &test_global;
     }
  */
  gcc_jit_type *int_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_INT);
  gcc_jit_lvalue *test_global =
    gcc_jit_context_new_global (
      ctxt,
      NULL,
      int_type,
      "test_global");

 gcc_jit_type *void_ptr_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_VOID_PTR);

  gcc_jit_function *test_get_address =
    gcc_jit_context_new_function (ctxt, NULL,
				  GCC_JIT_FUNCTION_EXPORTED,
				  void_ptr_type,
				  "test_get_address",
				  0, NULL,
				  0);
  gcc_jit_function_add_return (
    test_get_address,
    NULL,
    gcc_jit_lvalue_get_address (
      test_global,
      NULL));
}

static void
verify_get_address (gcc_jit_result *result)
{
  typedef void *(*test_fn) (void);
    test_fn test_get_address =
      (test_fn)gcc_jit_result_get_code (result,
					"test_get_address");
  CHECK_NON_NULL (test_get_address);
  CHECK_VALUE (test_get_address (), &test_global);
}

/**********************************************************************
 Code for harness
 **********************************************************************/

void
create_code (gcc_jit_context *ctxt, void *user_data)
{
  make_tests_of_unary_ops (ctxt);
  make_tests_of_binary_ops (ctxt);
  make_tests_of_comparisons (ctxt);
  make_tests_of_dereferences (ctxt);
  make_test_of_get_address (ctxt);
}

void
verify_code (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  CHECK_NON_NULL (result);

  verify_unary_ops (result);
  verify_binary_ops (result);
  verify_comparisons (result);
  verify_dereferences (result);
  verify_get_address (result);
}
