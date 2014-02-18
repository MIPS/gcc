#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "libgccjit.h"

#include "harness.h"

#define ARRAY_SIZE (4)

/* Verify that struct layout works properly when adding an array field.  */
struct array_holder
{
  float m_before;
  int m_ints[ARRAY_SIZE];
  float m_after;
};

void
create_code (gcc_jit_context *ctxt, void *user_data)
{
  /* Let's try to inject the equivalent of:

     void
     test_array (struct array_holder *ah)
     {
	ah->m_before = 4.0f;
	for i in 0 to (ARRAY_SIZE - 1):
	  ah->m_ints[i] = (i * i);
	ah->m_after = 2.0f;
     }
  */
  gcc_jit_type *void_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_VOID);
  gcc_jit_type *float_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_FLOAT);
  gcc_jit_type *int_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_INT);

  gcc_jit_field *field_m_before =
    gcc_jit_context_new_field (ctxt, NULL, float_type, "m_before");
  gcc_jit_field *field_m_ints =
    gcc_jit_context_new_field (
    ctxt, NULL,
    gcc_jit_context_new_array_type (ctxt, NULL, int_type, ARRAY_SIZE),
    "m_ints");
  gcc_jit_field *field_m_after =
    gcc_jit_context_new_field (ctxt, NULL, float_type, "m_after");

  gcc_jit_field *fields[] = {
    field_m_before,
    field_m_ints,
    field_m_after,
  };

  gcc_jit_struct *struct_type =
    gcc_jit_context_new_struct_type (
      ctxt,
      NULL,
      "array_holder",
      3, fields);

  gcc_jit_type *struct_ptr_type =
    gcc_jit_type_get_pointer (gcc_jit_struct_as_type (struct_type));

  /* Build the test_fn.  */
  gcc_jit_param *param_ah =
    gcc_jit_context_new_param (ctxt, NULL, struct_ptr_type, "ah");
  gcc_jit_function *func =
    gcc_jit_context_new_function (ctxt, NULL,
				  GCC_JIT_FUNCTION_EXPORTED,
				  void_type,
				  "test_array",
				  1, &param_ah,
				  0);

  /* "ah->m_before = 4.0f;" */
  gcc_jit_function_add_assignment (
    func, NULL,
    gcc_jit_rvalue_dereference_field (
      gcc_jit_param_as_rvalue (param_ah), NULL, field_m_before),
    gcc_jit_context_new_rvalue_from_int (ctxt, float_type, 4));

  gcc_jit_function_add_comment (func, NULL,
				"for i in 0 to (ARRAY_SIZE - 1):");
  gcc_jit_lvalue *i =
    gcc_jit_function_new_local (func, NULL, int_type, "i");
  gcc_jit_loop *loop =
    gcc_jit_function_new_loop_over_range (
      func, NULL, i,
      NULL,
      gcc_jit_context_new_rvalue_from_int (ctxt, int_type, ARRAY_SIZE),
      NULL);

  gcc_jit_function_add_comment (func, NULL, "ah->m_ints[i] = (i * i);");
  gcc_jit_function_add_assignment (
    func, NULL,
    gcc_jit_context_new_array_access (
      ctxt, NULL,
      gcc_jit_lvalue_as_rvalue (gcc_jit_rvalue_dereference_field (
	gcc_jit_param_as_rvalue (param_ah),
	NULL,
	field_m_ints)),
      gcc_jit_lvalue_as_rvalue (i)),
    gcc_jit_context_new_binary_op (
      ctxt, NULL,
      GCC_JIT_BINARY_OP_MULT,
      int_type,
      gcc_jit_lvalue_as_rvalue (i),
      gcc_jit_lvalue_as_rvalue (i)));

  gcc_jit_loop_end (loop, NULL);

 /* ah->m_after = 2.0f; */
  gcc_jit_function_add_assignment (
    func, NULL,
    gcc_jit_rvalue_dereference_field (
      gcc_jit_param_as_rvalue (param_ah), NULL, field_m_after),
    gcc_jit_context_new_rvalue_from_int (ctxt, float_type, 2));

}

void
verify_code (gcc_jit_context *ctxt, gcc_jit_result *result)
{
  typedef void (*fn_type) (struct array_holder *ah);

  CHECK_NON_NULL (result);
  fn_type test_array =
    (fn_type)gcc_jit_result_get_code (result, "test_array");
  CHECK_NON_NULL (test_array);

  struct array_holder ah;
  memset (&ah, 0xf0, sizeof (ah));

  test_array (&ah);
  CHECK_VALUE (ah.m_before, 4.0f);
  CHECK_VALUE (ah.m_ints[0], 0);
  CHECK_VALUE (ah.m_ints[1], 1);
  CHECK_VALUE (ah.m_ints[2], 4);
  CHECK_VALUE (ah.m_ints[3], 9);
  CHECK_VALUE (ah.m_after, 2.0f);

}
