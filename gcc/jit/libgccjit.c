/* Implementation of the C API; all wrappers into a C++ API */
#include "config.h"
#include "system.h"
#include "ansidecl.h"
#include "coretypes.h"
#include "opts.h"

#include "libgccjit.h"
#include "internal-api.h"

struct gcc_jit_context : public gcc::jit::context
{
};

struct gcc_jit_result : public gcc::jit::result
{
};

struct gcc_jit_location : public gcc::jit::location
{
};

struct gcc_jit_type : public gcc::jit::type
{
};

struct gcc_jit_field : public gcc::jit::field
{
};

struct gcc_jit_function : public gcc::jit::function
{
};

struct gcc_jit_label : public gcc::jit::label
{
};

struct gcc_jit_rvalue : public gcc::jit::rvalue
{
};

struct gcc_jit_lvalue : public gcc::jit::lvalue
{
};

struct gcc_jit_param : public gcc::jit::param
{
};

struct gcc_jit_local : public gcc::jit::local
{
};

struct gcc_jit_loop : public gcc::jit::loop
{
};

gcc_jit_context *
gcc_jit_context_acquire (void)
{
  return new gcc_jit_context ();
}

void
gcc_jit_context_release (gcc_jit_context *ctxt)
{
  delete ctxt;
}

void
gcc_jit_context_set_code_factory (gcc_jit_context *ctxt,
				  gcc_jit_code_callback cb,
				  void *user_data)
{
  gcc_assert (ctxt);
  ctxt->set_code_factory (cb, user_data);
}

/**********************************************************************
 Functions for use within the code factory.
 **********************************************************************/
#define ASSERT_WITHIN_CALLBACK(CTXT) \
  do {                                           \
    gcc_assert ((CTXT)->within_code_factory ()); \
  } while (0)

#define ASSERT_NOT_WITHIN_CALLBACK(CTXT) \
  do {                                            \
    gcc_assert (!(CTXT)->within_code_factory ()); \
  } while (0)

gcc_jit_type *
gcc_jit_context_get_void_type (gcc_jit_context *ctxt)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_type *)ctxt->get_void_type ();
}

gcc_jit_type *
gcc_jit_context_get_char_type (gcc_jit_context *ctxt)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_type *)ctxt->get_char_type ();
}

gcc_jit_type *
gcc_jit_context_get_int_type (gcc_jit_context *ctxt)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_type *)ctxt->get_int_type ();
}

gcc_jit_type *
gcc_jit_context_get_float_type (gcc_jit_context *ctxt)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_type *)ctxt->get_float_type ();
}

gcc_jit_type *
gcc_jit_context_get_double_type (gcc_jit_context *ctxt)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_type *)ctxt->get_double_type ();
}

gcc_jit_type *
gcc_jit_type_get_pointer (gcc_jit_type *type)
{
  return (gcc_jit_type *)type->get_pointer ();
}

gcc_jit_type *
gcc_jit_type_get_const (gcc_jit_type *type)
{
  return (gcc_jit_type *)type->get_const ();
}

gcc_jit_field *
gcc_jit_context_new_field (gcc_jit_context *ctxt,
			   gcc_jit_location *loc,
			   gcc_jit_type *type,
			   const char *name)
{
  return (gcc_jit_field *)ctxt->new_field (loc, type, name);
}

gcc_jit_type *
gcc_jit_context_new_struct_type (gcc_jit_context *ctxt,
				 gcc_jit_location *loc,
				 const char *name,
				 int num_fields,
				 gcc_jit_field **fields)
{
  return (gcc_jit_type *)ctxt->new_struct_type (loc, name, num_fields,
						(gcc::jit::field **)fields);
}


/* Constructing functions.  */
gcc_jit_param *
gcc_jit_context_new_param (gcc_jit_context *ctxt,
			   gcc_jit_location *loc,
			   gcc_jit_type *type,
			   const char *name)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_param *)ctxt->new_param (loc, type, name);
}

gcc_jit_lvalue *
gcc_jit_param_as_lvalue (gcc_jit_param *param)
{
  return (gcc_jit_lvalue *)param->as_lvalue ();
}

gcc_jit_rvalue *
gcc_jit_param_as_rvalue (gcc_jit_param *param)
{
  return (gcc_jit_rvalue *)param->as_rvalue ();
}

gcc_jit_function *
gcc_jit_context_new_function (gcc_jit_context *ctxt,
			      gcc_jit_location *loc,
			      enum gcc_jit_function_kind kind,
			      gcc_jit_type *return_type,
			      const char *name,
			      int num_params,
			      gcc_jit_param **params,
			      int is_variadic)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_function*)
    ctxt->new_function (loc, kind, return_type, name,
			num_params,
			(gcc::jit::param **)params,
			is_variadic);
}

gcc_jit_label*
gcc_jit_function_new_forward_label (gcc_jit_function *func,
				    const char *name)
{
  return (gcc_jit_label *)func->new_forward_label (name);
}

gcc_jit_lvalue *
gcc_jit_context_new_global (gcc_jit_context *ctxt,
			    gcc_jit_location *loc,
			    gcc_jit_type *type,
			    const char *name)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_lvalue *)ctxt->new_global (loc, type, name);
}

gcc_jit_local *
gcc_jit_context_new_local (gcc_jit_context *ctxt,
			   gcc_jit_location *loc,
			   gcc_jit_type *type,
			   const char *name)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_local *)ctxt->new_local (loc, type, name);
}

gcc_jit_lvalue *
gcc_jit_local_as_lvalue (gcc_jit_local *local)
{
  return (gcc_jit_lvalue *)local->as_lvalue ();
}

gcc_jit_rvalue *
gcc_jit_local_as_rvalue (gcc_jit_local *local)
{
  return (gcc_jit_rvalue *)local->as_rvalue ();
}

gcc_jit_rvalue *
gcc_jit_lvalue_as_rvalue (gcc_jit_lvalue *lvalue)
{
  return (gcc_jit_rvalue *)lvalue->as_rvalue ();
}

gcc_jit_rvalue *
gcc_jit_context_new_rvalue_from_int (gcc_jit_context *ctxt,
				     gcc_jit_type *type,
				     int value)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_rvalue *)ctxt->new_rvalue_from_int (type, value);
}

gcc_jit_rvalue *
gcc_jit_context_zero (gcc_jit_context *ctxt,
		      gcc_jit_type *type)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return gcc_jit_context_new_rvalue_from_int (ctxt, type, 0);
}

gcc_jit_rvalue *
gcc_jit_context_one (gcc_jit_context *ctxt,
		     gcc_jit_type *type)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return gcc_jit_context_new_rvalue_from_int (ctxt, type, 1);
}

gcc_jit_rvalue *
gcc_jit_context_new_string_literal (gcc_jit_context *ctxt,
				    const char *value)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_rvalue *)ctxt->new_string_literal (value);
}


gcc_jit_rvalue *
gcc_jit_context_new_binary_op (gcc_jit_context *ctxt,
			       gcc_jit_location *loc,
			       enum gcc_jit_binary_op op,
			       gcc_jit_type *result_type,
			       gcc_jit_rvalue *a, gcc_jit_rvalue *b)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_rvalue *)ctxt->new_binary_op (loc, op, result_type, a, b);
}

gcc_jit_rvalue *
gcc_jit_context_new_comparison (gcc_jit_context *ctxt,
				gcc_jit_location *loc,
				enum gcc_jit_comparison op,
				gcc_jit_rvalue *a, gcc_jit_rvalue *b)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_rvalue *)ctxt->new_comparison (loc, op, a, b);
}


gcc_jit_rvalue *
gcc_jit_context_new_call (gcc_jit_context *ctxt,
			  gcc_jit_location *loc,
			  gcc_jit_function *func,
			  int numargs , gcc_jit_rvalue **args)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_rvalue *)ctxt->new_call (loc,
					   func,
					   numargs,
					   (gcc::jit::rvalue **)args);
}

extern gcc_jit_rvalue *
gcc_jit_context_new_array_lookup (gcc_jit_context *ctxt,
				  gcc_jit_location *loc,
				  gcc_jit_rvalue *ptr,
				  gcc_jit_rvalue *index)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_rvalue *)ctxt->new_array_lookup (loc, ptr, index);
}

extern gcc_jit_lvalue *
gcc_jit_context_new_field_access (gcc_jit_context *ctxt,
				  gcc_jit_location *loc,
				  gcc_jit_rvalue *ptr_or_struct,
				  const char *fieldname)
{
  ASSERT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_lvalue *)ctxt->new_field_access (loc, ptr_or_struct, fieldname);
}

gcc_jit_label *
gcc_jit_function_add_label (gcc_jit_function *func,
			    gcc_jit_location *loc,
			    const char *name)
{
  return (gcc_jit_label *)func->add_label (loc, name);
}

void
gcc_jit_function_place_forward_label (gcc_jit_function *func,
				      gcc_jit_label *lab)
{
  func->place_forward_label (lab);
}

void
gcc_jit_function_add_eval (gcc_jit_function *func,
			   gcc_jit_location *loc,
			   gcc_jit_rvalue *rvalue)
{
  return func->add_eval (loc, rvalue);
}

void
gcc_jit_function_add_assignment (gcc_jit_function *func,
				 gcc_jit_location *loc,
				 gcc_jit_lvalue *lvalue,
				 gcc_jit_rvalue *rvalue)
{
  return func->add_assignment (loc, lvalue, rvalue);
}

void
gcc_jit_function_add_assignment_op (gcc_jit_function *func,
				    gcc_jit_location *loc,
				    gcc_jit_lvalue *lvalue,
				    enum gcc_jit_binary_op op,
				    gcc_jit_rvalue *rvalue)
{
  gcc_jit_type *type = (gcc_jit_type *)lvalue->get_type ();
  gcc_jit_function_add_assignment (
    func, loc,
    lvalue,
    gcc_jit_context_new_binary_op (
      (gcc_jit_context *)func->m_ctxt,
      loc, op, type,
      gcc_jit_lvalue_as_rvalue (lvalue),
      rvalue));
}

void
gcc_jit_function_add_conditional (gcc_jit_function *func,
				  gcc_jit_location *loc,
				  gcc_jit_rvalue *boolval,
				  gcc_jit_label *on_true,
				  gcc_jit_label *on_false)
{
  return func->add_conditional (loc, boolval, on_true, on_false);
}

void
gcc_jit_function_add_jump (gcc_jit_function *func,
			gcc_jit_location *loc,
			gcc_jit_label *target)
{
  func->add_jump (loc, target);
}

void
gcc_jit_function_add_return (gcc_jit_function *func,
			     gcc_jit_location *loc,
			     gcc_jit_rvalue *rvalue)
{
  return func->add_return (loc, rvalue);
}

gcc_jit_loop *
gcc_jit_function_new_loop (gcc_jit_function *func,
			   gcc_jit_location *loc,
			   gcc_jit_rvalue *boolval)
{
  return (gcc_jit_loop *)func->new_loop (loc, boolval);
}

void
gcc_jit_loop_end (gcc_jit_loop *loop,
		  gcc_jit_location *loc)
{
  loop->end (loc);
}

/**********************************************************************
 Option-management
 **********************************************************************/

void
gcc_jit_context_set_str_option (gcc_jit_context *ctxt,
				enum gcc_jit_str_option opt,
				const char *value)
{
  gcc_assert (ctxt);
  ASSERT_NOT_WITHIN_CALLBACK (ctxt);
  ctxt->set_str_option (opt, value);
}

void
gcc_jit_context_set_int_option (gcc_jit_context *ctxt,
				enum gcc_jit_int_option opt,
				int value)
{
  gcc_assert (ctxt);
  ASSERT_NOT_WITHIN_CALLBACK (ctxt);
  ctxt->set_int_option (opt, value);
}

void
gcc_jit_context_set_bool_option (gcc_jit_context *ctxt,
				 enum gcc_jit_bool_option opt,
				 int value)
{
  gcc_assert (ctxt);
  ASSERT_NOT_WITHIN_CALLBACK (ctxt);
  ctxt->set_bool_option (opt, value);
}

gcc_jit_result *
gcc_jit_context_compile (gcc_jit_context *ctxt)
{
  gcc_assert (ctxt);
  ASSERT_NOT_WITHIN_CALLBACK (ctxt);
  return (gcc_jit_result *)ctxt->compile ();
}

void *
gcc_jit_result_get_code (gcc_jit_result *result,
			 const char *fnname)
{
  gcc_assert (result);
  gcc_assert (fnname);
  return result->get_code (fnname);
}

void
gcc_jit_result_release (gcc_jit_result *result)
{
  delete result;
}
