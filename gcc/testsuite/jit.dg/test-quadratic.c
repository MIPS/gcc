#include <stdlib.h>
#include <stdio.h>

#include "libgccjit.h"

#include "harness.h"

struct quadratic
{
  double a;
  double b;
  double c;
  double discriminant;
};

/* Let's try to inject the equivalent of:

     extern double sqrt (double);

     void
     calc_discriminant (struct quadratic *q)
     {
       // (b^2 - 4ac)
       q->discriminant = (q->b * q->b) - (4 * q->a * q->c);
     }

     int
     test_quadratic (double a, double b, double c, double *r1, double *r2)
     {
       struct quadratic q;
       q.a = a;
       q.b = b;
       q.c = c;
       calc_discriminant (&q);
       if (q.discriminant > 0)
	 {
	    double s = sqrt (q.discriminant);
	    *r1 = (-b + s) / (2 * a);
	    *r2 = (-b - s) / (2 * a);
	    return 2;
	 }
       else if (q.discriminant == 0)
	 {
	    *r1 = -b / (2 * a);
	    return 1;
	 }
	 else return 0;
     }
*/

struct quadratic_test
{
  gcc_jit_context *ctxt;

  /* "double" and "(double *)".  */
  gcc_jit_type *numeric_type;
  gcc_jit_type *numeric_type_ptr;

  /* The value (double)0.  */
  gcc_jit_rvalue *zero;

  gcc_jit_type *int_type;

  /* "struct quadratic" */
  gcc_jit_type *quadratic;
  gcc_jit_field *a;
  gcc_jit_field *b;
  gcc_jit_field *c;
  gcc_jit_field *discriminant;

  /* "(struct quadratic *)" */
  gcc_jit_type *quadratic_ptr;

  gcc_jit_function *calc_discriminant;

  gcc_jit_function *sqrt;

};

static void
make_types (struct quadratic_test *testcase)
{
  testcase->numeric_type =
    gcc_jit_context_get_type (testcase->ctxt, GCC_JIT_TYPE_DOUBLE);
  testcase->numeric_type_ptr =
    gcc_jit_type_get_pointer (testcase->numeric_type);
  testcase->zero =
    gcc_jit_context_zero (testcase->ctxt, testcase->numeric_type);

  testcase->int_type =
    gcc_jit_context_get_type (testcase->ctxt, GCC_JIT_TYPE_INT);

  testcase->a =
    gcc_jit_context_new_field (testcase->ctxt,
			       NULL,
			       testcase->numeric_type,
			       "a");
  testcase->b =
    gcc_jit_context_new_field (testcase->ctxt,
			       NULL,
			       testcase->numeric_type,
			       "b");
  testcase->c =
    gcc_jit_context_new_field (testcase->ctxt,
			       NULL,
			       testcase->numeric_type,
			       "c");
  testcase->discriminant =
    gcc_jit_context_new_field (testcase->ctxt,
			       NULL,
			       testcase->numeric_type,
			       "discriminant");
  gcc_jit_field *fields[] = {testcase->a,
			     testcase->b,
			     testcase->c,
			     testcase->discriminant};
  testcase->quadratic =
    gcc_jit_struct_as_type (
      gcc_jit_context_new_struct_type (testcase->ctxt, NULL,
				       "quadratic", 4, fields));
  testcase->quadratic_ptr = gcc_jit_type_get_pointer (testcase->quadratic);
}

static void
make_sqrt (struct quadratic_test *testcase)
{
  gcc_jit_param *param_x =
    gcc_jit_context_new_param (testcase->ctxt, NULL,
			       testcase->numeric_type, "x");
  testcase->sqrt =
    gcc_jit_context_new_function (testcase->ctxt, NULL,
				  GCC_JIT_FUNCTION_IMPORTED,
				  testcase->numeric_type,
				  "sqrt",
				  1, &param_x,
				  0);
}

static void
make_calc_discriminant (struct quadratic_test *testcase)
{
  /* Build "calc_discriminant".  */
  gcc_jit_param *param_q =
    gcc_jit_context_new_param (testcase->ctxt, NULL,
			       testcase->quadratic_ptr, "q");
  testcase->calc_discriminant =
    gcc_jit_context_new_function (testcase->ctxt, NULL,
				  GCC_JIT_FUNCTION_EXPORTED,
				  testcase->numeric_type,
				  "calc_discriminant",
				  1, &param_q,
				  0);
  gcc_jit_function_add_comment (
    testcase->calc_discriminant, NULL,
    "(b^2 - 4ac)");

  gcc_jit_rvalue *q_a =
    gcc_jit_lvalue_as_rvalue (
	gcc_jit_rvalue_dereference_field (
	  gcc_jit_param_as_rvalue (param_q),
	  NULL, testcase->a));
  gcc_jit_rvalue *q_b =
    gcc_jit_lvalue_as_rvalue (
	gcc_jit_rvalue_dereference_field (
	  gcc_jit_param_as_rvalue (param_q),
	  NULL, testcase->b));
  gcc_jit_rvalue *q_c =
    gcc_jit_lvalue_as_rvalue (
	gcc_jit_rvalue_dereference_field (
	  gcc_jit_param_as_rvalue (param_q),
	  NULL, testcase->c));

  gcc_jit_function_add_assignment (
    testcase->calc_discriminant, NULL,

    /* q->discriminant =...  */
    gcc_jit_rvalue_dereference_field (
      gcc_jit_param_as_rvalue (param_q),
      NULL,
      testcase->discriminant),

    /* (q->b * q->b) - (4 * q->a * q->c) */
    gcc_jit_context_new_binary_op (
      testcase->ctxt, NULL,
      GCC_JIT_BINARY_OP_MINUS,
      testcase->numeric_type,

      /* (q->b * q->b) */
      gcc_jit_context_new_binary_op (
	testcase->ctxt, NULL,
	GCC_JIT_BINARY_OP_MULT,
	testcase->numeric_type,
	q_b, q_b),

      /* (4 * (q->a * q->c)) */
      gcc_jit_context_new_binary_op (
	testcase->ctxt, NULL,
	GCC_JIT_BINARY_OP_MULT,
	testcase->numeric_type,
	/* 4.0 */
	gcc_jit_context_new_rvalue_from_int (
	  testcase->ctxt,
	  testcase->numeric_type,
	  4),
	/* (q->a * q->c) */
	gcc_jit_context_new_binary_op (
	  testcase->ctxt, NULL,
	  GCC_JIT_BINARY_OP_MULT,
	  testcase->numeric_type,
	  q_a, q_c)))); /* end of gcc_jit_function_add_assignment call.  */
}

static void
make_test_quadratic (struct quadratic_test *testcase)
{
  gcc_jit_param *a =
    gcc_jit_context_new_param (testcase->ctxt, NULL,
			       testcase->numeric_type, "a");
  gcc_jit_param *b =
    gcc_jit_context_new_param (testcase->ctxt, NULL,
			       testcase->numeric_type, "b");
  gcc_jit_param *c =
    gcc_jit_context_new_param (testcase->ctxt, NULL,
			       testcase->numeric_type, "c");
  gcc_jit_param *r1 =
    gcc_jit_context_new_param (testcase->ctxt, NULL,
			       testcase->numeric_type_ptr, "r1");
  gcc_jit_param *r2 =
    gcc_jit_context_new_param (testcase->ctxt, NULL,
			       testcase->numeric_type_ptr, "r2");
  gcc_jit_param *params[] = {a, b, c, r1, r2};
  gcc_jit_function *test_quadratic =
    gcc_jit_context_new_function (testcase->ctxt, NULL,
				  GCC_JIT_FUNCTION_EXPORTED,
				  testcase->int_type,
				  "test_quadratic",
				  5, params,
				  0);

  /* struct quadratic q; */
  gcc_jit_lvalue *q =
    gcc_jit_function_new_local (
      test_quadratic, NULL,
      testcase->quadratic,
      "q");
  /* q.a = a; */
  gcc_jit_function_add_assignment (
    test_quadratic, NULL,
    gcc_jit_lvalue_access_field (q, NULL, testcase->a),
    gcc_jit_param_as_rvalue (a));
  /* q.b = b; */
  gcc_jit_function_add_assignment (
    test_quadratic, NULL,
    gcc_jit_lvalue_access_field (q, NULL, testcase->b),
    gcc_jit_param_as_rvalue (b));
  /* q.c = c; */
  gcc_jit_function_add_assignment (
    test_quadratic, NULL,
    gcc_jit_lvalue_access_field (q, NULL, testcase->c),
    gcc_jit_param_as_rvalue (c));
  /* calc_discriminant (&q); */
  gcc_jit_rvalue *address_of_q = gcc_jit_lvalue_get_address (q, NULL);
  gcc_jit_function_add_eval (
    test_quadratic, NULL,
    gcc_jit_context_new_call (
      testcase->ctxt, NULL,
      testcase->calc_discriminant,
      1, &address_of_q));

  gcc_jit_label *on_positive_discriminant
    = gcc_jit_function_new_forward_label (test_quadratic,
					  "positive_discriminant");

  gcc_jit_label *on_nonpositive_discriminant
    = gcc_jit_function_new_forward_label (test_quadratic,
					  "nonpositive_discriminant");

  gcc_jit_label *on_zero_discriminant
    = gcc_jit_function_new_forward_label (test_quadratic,
					  "zero_discriminant");

  gcc_jit_label *on_negative_discriminant
    = gcc_jit_function_new_forward_label (test_quadratic,
					  "negative_discriminant");

  gcc_jit_function_add_comment (
    test_quadratic, NULL,
    "if (q.discriminant > 0)");
  gcc_jit_function_add_conditional (
    test_quadratic, NULL,
    gcc_jit_context_new_comparison (
      testcase->ctxt, NULL,
      GCC_JIT_COMPARISON_GT,
      gcc_jit_rvalue_access_field (
	gcc_jit_lvalue_as_rvalue (q),
	NULL,
	testcase->discriminant),
      testcase->zero),
    on_positive_discriminant,
    on_nonpositive_discriminant);

  gcc_jit_function_place_forward_label (
    test_quadratic, NULL,
    on_positive_discriminant);
  /* double s = sqrt (q.discriminant); */
  gcc_jit_lvalue *s = gcc_jit_function_new_local (
    test_quadratic, NULL,
    testcase->numeric_type,
    "s");
  gcc_jit_rvalue *discriminant_of_q =
    gcc_jit_rvalue_access_field (gcc_jit_lvalue_as_rvalue (q),
				 NULL,
				 testcase->discriminant);
  gcc_jit_function_add_assignment (
    test_quadratic, NULL,
    s,
    gcc_jit_context_new_call (
      testcase->ctxt, NULL,
      testcase->sqrt,
      1, &discriminant_of_q));

  gcc_jit_rvalue *minus_b =
    gcc_jit_context_new_unary_op (
      testcase->ctxt, NULL,
      GCC_JIT_UNARY_OP_MINUS,
      testcase->numeric_type,
      gcc_jit_param_as_rvalue (b));
  gcc_jit_rvalue *two_a =
    gcc_jit_context_new_binary_op (
      testcase->ctxt, NULL,
      GCC_JIT_BINARY_OP_MULT,
      testcase->numeric_type,
      gcc_jit_context_new_rvalue_from_int (
	testcase->ctxt,
	testcase->numeric_type,
	2),
      gcc_jit_param_as_rvalue (a));

  gcc_jit_function_add_comment (
    test_quadratic, NULL,
    "*r1 = (-b + s) / (2 * a);");
  gcc_jit_function_add_assignment (
    test_quadratic, NULL,

    /* "*r1 = ..." */
    gcc_jit_rvalue_dereference (
      gcc_jit_param_as_rvalue (r1), NULL),

    /* (-b + s) / (2 * a) */
    gcc_jit_context_new_binary_op (
      testcase->ctxt, NULL,
      GCC_JIT_BINARY_OP_DIVIDE,
      testcase->numeric_type,
      gcc_jit_context_new_binary_op (
	testcase->ctxt, NULL,
	GCC_JIT_BINARY_OP_PLUS,
	testcase->numeric_type,
	minus_b,
	gcc_jit_lvalue_as_rvalue (s)),
      two_a));

  gcc_jit_function_add_comment (
    test_quadratic, NULL,
    "*r2 = (-b - s) / (2 * a)");
  gcc_jit_function_add_assignment (
    test_quadratic, NULL,

    /* "*r2 = ..." */
    gcc_jit_rvalue_dereference (
      gcc_jit_param_as_rvalue (r2), NULL),

    /* (-b - s) / (2 * a) */
    gcc_jit_context_new_binary_op (
      testcase->ctxt, NULL,
      GCC_JIT_BINARY_OP_DIVIDE,
      testcase->numeric_type,
      gcc_jit_context_new_binary_op (
	testcase->ctxt, NULL,
	GCC_JIT_BINARY_OP_MINUS,
	testcase->numeric_type,
	minus_b,
	gcc_jit_lvalue_as_rvalue (s)),
      two_a));

  /* "return 2;" */
  gcc_jit_function_add_return (
    test_quadratic, NULL,
    gcc_jit_context_new_rvalue_from_int (
      testcase->ctxt,
      testcase->int_type,
      2));

  /* "else if (q.discriminant == 0)" */
  gcc_jit_function_place_forward_label (
    test_quadratic, NULL,
    on_nonpositive_discriminant);
  gcc_jit_function_add_comment (
    test_quadratic, NULL,
    "else if (q.discriminant == 0)");
  gcc_jit_function_add_conditional (
    test_quadratic, NULL,
    gcc_jit_context_new_comparison (
      testcase->ctxt, NULL,
      GCC_JIT_COMPARISON_EQ,
      gcc_jit_rvalue_access_field (
	gcc_jit_lvalue_as_rvalue (q),
	NULL,
	testcase->discriminant),
      testcase->zero),
    on_zero_discriminant,
    on_negative_discriminant);

  /* if (q.discriminant == 0) */
  gcc_jit_function_place_forward_label (
    test_quadratic, NULL,
    on_zero_discriminant);

  gcc_jit_function_add_comment (
    test_quadratic, NULL,
    "*r1 = -b / (2 * a);");
  gcc_jit_function_add_assignment (
    test_quadratic, NULL,

    /* "*r1 = ..." */
    gcc_jit_rvalue_dereference (
      gcc_jit_param_as_rvalue (r1), NULL),

    /* -b / (2 * a) */
    gcc_jit_context_new_binary_op (
      testcase->ctxt, NULL,
      GCC_JIT_BINARY_OP_DIVIDE,
      testcase->numeric_type,
      minus_b,
      two_a));

  /* "return 1;" */
  gcc_jit_function_add_return (
    test_quadratic, NULL,
      gcc_jit_context_one (testcase->ctxt, testcase->int_type));

  /* else return 0; */
  gcc_jit_function_place_forward_label (
    test_quadratic, NULL,
    on_negative_discriminant);
  gcc_jit_function_add_return (
    test_quadratic, NULL,
    gcc_jit_context_zero (testcase->ctxt, testcase->int_type));
}

void
create_code (gcc_jit_context *ctxt, void *user_data)
{
  struct quadratic_test testcase;
  memset (&testcase, 0, sizeof (testcase));
  testcase.ctxt = ctxt;
  make_types (&testcase);
  make_sqrt (&testcase);
  make_calc_discriminant (&testcase);
  make_test_quadratic (&testcase);
}

void
verify_code (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  typedef int (*fn_type) (double a, double b, double c,
			  double *r1, double *r2);

  CHECK_NON_NULL (result);

  fn_type test_quadratic =
    (fn_type)gcc_jit_result_get_code (result, "test_quadratic");
  CHECK_NON_NULL (test_quadratic);

  /* Verify that the code correctly solves quadratic equations.  */
  double r1, r2;

  /* This one has two solutions: */
  CHECK_VALUE (test_quadratic (1, 3, -4, &r1, &r2), 2);
  CHECK_VALUE (r1, 1);
  CHECK_VALUE (r2, -4);

  /* This one has one solution: */
  CHECK_VALUE (test_quadratic (4, 4, 1, &r1, &r2), 1);
  CHECK_VALUE (r1, -0.5);

  /* This one has no real solutions: */
  CHECK_VALUE (test_quadratic (4, 1, 1, &r1, &r2), 0);
}
