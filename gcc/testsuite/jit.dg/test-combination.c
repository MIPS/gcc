/* Construct a test case by combining other test cases, to try to shake
   out state issues: all of the test cases are run in one process, inside
   one gcc_jit_context (per iteration).  */

/* Include various other test cases, defining TEST_COMBINATION so that
   harness.h doesn't duplicate copes of e.g. main, and renaming the
   hooks provided by each test case.  */
#define TEST_COMBINATION

/* test-accessing-struct.c */
#define code_making_callback code_making_callback_accessing_struct
#define verify_code verify_code_accessing_struct
#include "test-accessing-struct.c"
#undef code_making_callback
#undef verify_code

/* test-calling-external-function.c */
#define code_making_callback code_making_callback_calling_external_function
#define verify_code verify_code_calling_external_function
#include "test-calling-external-function.c"
#undef code_making_callback
#undef verify_code

/* test-dot-product.c */
#define code_making_callback code_making_callback_dot_product
#define verify_code verify_code_dot_product
#include "test-dot-product.c"
#undef code_making_callback
#undef verify_code

/* test-expressions.c */
#define code_making_callback code_making_callback_expressions
#define verify_code verify_code_expressions
#include "test-expressions.c"
#undef code_making_callback
#undef verify_code

/* test-factorial.c */
#define code_making_callback code_making_callback_factorial
#define verify_code verify_code_factorial
#include "test-factorial.c"
#undef code_making_callback
#undef verify_code

/* We don't use test-failure.c; we don't want its failure to affect our
   combined case.  */

/* test-fibonacci.c */
#define code_making_callback code_making_callback_fibonacci
#define verify_code verify_code_fibonacci
#include "test-fibonacci.c"
#undef code_making_callback
#undef verify_code

/* test-hello-world.c */
#define code_making_callback code_making_callback_hello_world
#define verify_code verify_code_hello_world
#include "test-hello-world.c"
#undef code_making_callback
#undef verify_code

/* test-quadratic.c */
#define code_making_callback code_making_callback_quadratic
#define verify_code verify_code_quadratic
#include "test-quadratic.c"
#undef code_making_callback
#undef verify_code

/* test-reading-struct.c */
#define code_making_callback code_making_callback_reading_struct
#define verify_code verify_code_reading_struct
#include "test-reading-struct.c"
#undef code_making_callback
#undef verify_code

/* test-string-literal.c */
#define code_making_callback code_making_callback_string_literal
#define verify_code verify_code_string_literal
#include "test-string-literal.c"
#undef code_making_callback
#undef verify_code

/* test-sum-of-squares.c */
#define code_making_callback code_making_callback_sum_of_squares
#define verify_code verify_code_sum_of_squares
#include "test-sum-of-squares.c"
#undef code_making_callback
#undef verify_code

/* test-types.c */
#define code_making_callback code_making_callback_types
#define verify_code verify_code_types
#include "test-types.c"
#undef code_making_callback
#undef verify_code

/* test-using-global.c */
#define code_making_callback code_making_callback_using_global
#define verify_code verify_code_using_global
#include "test-using-global.c"
#undef code_making_callback
#undef verify_code

/* Now construct a test case from all the other test cases.

   We undefine TEST_COMBINATION so that we can now include harness.h
   "for real".  */
#undef TEST_COMBINATION
#include "harness.h"

/* Our testing hooks are the combination of the other test cases.  */
int
code_making_callback (gcc_jit_context *ctxt, void * user_data)
{
  int errors = 0;
  errors += code_making_callback_accessing_struct (ctxt, user_data);
  errors += code_making_callback_calling_external_function (ctxt, user_data);
  errors += code_making_callback_dot_product (ctxt, user_data);
  errors += code_making_callback_expressions (ctxt, user_data);
  errors += code_making_callback_factorial (ctxt, user_data);
  errors += code_making_callback_fibonacci (ctxt, user_data);
  errors += code_making_callback_hello_world (ctxt, user_data);
  errors += code_making_callback_string_literal (ctxt, user_data);
  errors += code_making_callback_sum_of_squares (ctxt, user_data);
  errors += code_making_callback_types (ctxt, user_data);
  errors += code_making_callback_using_global (ctxt, user_data);

  return errors;
}

void
verify_code (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  verify_code_accessing_struct (ctxt, result);
  verify_code_calling_external_function (ctxt, result);
  verify_code_dot_product (ctxt, result);
  verify_code_expressions (ctxt, result);
  verify_code_factorial (ctxt, result);
  verify_code_fibonacci (ctxt, result);
  verify_code_hello_world (ctxt, result);
  verify_code_string_literal (ctxt, result);
  verify_code_sum_of_squares (ctxt, result);
  verify_code_types (ctxt, result);
  verify_code_using_global (ctxt, result);
}
