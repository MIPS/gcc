/* Implementation of the C API; all wrappers into a C++ API */
#include "config.h"
#include "system.h"
#include "ansidecl.h"
#include "coretypes.h"
#include "opts.h"

#include "libgccjit.h"
#include "internal-api.h"

#define IS_ASCII_ALPHA(CHAR) \
  (					\
    ((CHAR) >= 'a' && (CHAR) <='z')	\
    ||					\
    ((CHAR) >= 'A' && (CHAR) <= 'Z')	\
  )

#define IS_ASCII_DIGIT(CHAR) \
  ((CHAR) >= '0' && (CHAR) <='9')

#define IS_ASCII_ALNUM(CHAR) \
  (IS_ASCII_ALPHA (CHAR) || IS_ASCII_DIGIT (CHAR))

struct gcc_jit_context : public gcc::jit::recording::context
{
  gcc_jit_context (gcc_jit_context *parent_ctxt) :
    context (parent_ctxt)
  {}
};

struct gcc_jit_result : public gcc::jit::result
{
};

struct gcc_jit_object : public gcc::jit::recording::memento
{
};

struct gcc_jit_location : public gcc::jit::recording::location
{
};

struct gcc_jit_type : public gcc::jit::recording::type
{
};

struct gcc_jit_struct : public gcc::jit::recording::struct_
{
};

struct gcc_jit_field : public gcc::jit::recording::field
{
};

struct gcc_jit_function : public gcc::jit::recording::function
{
};

struct gcc_jit_block : public gcc::jit::recording::block
{
};

struct gcc_jit_rvalue : public gcc::jit::recording::rvalue
{
};

struct gcc_jit_lvalue : public gcc::jit::recording::lvalue
{
};

struct gcc_jit_param : public gcc::jit::recording::param
{
};

/**********************************************************************
 Error-handling.

 We try to gracefully handle API usage errors by being defensive
 at the API boundary.
 **********************************************************************/

#define JIT_BEGIN_STMT do {
#define JIT_END_STMT   } while(0)

/* TODO: mark failure branches as unlikely? */

#define RETURN_VAL_IF_FAIL(TEST_EXPR, RETURN_EXPR, CTXT, LOC, ERR_MSG)	\
  JIT_BEGIN_STMT							\
    if (!(TEST_EXPR))							\
      {								\
	jit_error ((CTXT), (LOC), "%s: %s", __func__, (ERR_MSG));	\
	return (RETURN_EXPR);						\
      }								\
  JIT_END_STMT

#define RETURN_VAL_IF_FAIL_PRINTF1(TEST_EXPR, RETURN_EXPR, CTXT, LOC, ERR_FMT, A0) \
  JIT_BEGIN_STMT							\
    if (!(TEST_EXPR))							\
      {								\
	jit_error ((CTXT), (LOC), "%s: " ERR_FMT,			\
		   __func__, (A0));				\
	return (RETURN_EXPR);						\
      }								\
  JIT_END_STMT

#define RETURN_VAL_IF_FAIL_PRINTF2(TEST_EXPR, RETURN_EXPR, CTXT, LOC, ERR_FMT, A0, A1) \
  JIT_BEGIN_STMT							\
    if (!(TEST_EXPR))							\
      {								\
	jit_error ((CTXT), (LOC), "%s: " ERR_FMT,				\
		   __func__, (A0), (A1));				\
	return (RETURN_EXPR);						\
      }								\
  JIT_END_STMT

#define RETURN_VAL_IF_FAIL_PRINTF3(TEST_EXPR, RETURN_EXPR, CTXT, LOC, ERR_FMT, A0, A1, A2) \
  JIT_BEGIN_STMT							\
    if (!(TEST_EXPR))							\
      {								\
	jit_error ((CTXT), (LOC), "%s: " ERR_FMT,				\
		   __func__, (A0), (A1), (A2));			\
	return (RETURN_EXPR);						\
      }								\
  JIT_END_STMT

#define RETURN_VAL_IF_FAIL_PRINTF4(TEST_EXPR, RETURN_EXPR, CTXT, LOC, ERR_FMT, A0, A1, A2, A3) \
  JIT_BEGIN_STMT							\
    if (!(TEST_EXPR))							\
      {								\
	jit_error ((CTXT), (LOC), "%s: " ERR_FMT,				\
		   __func__, (A0), (A1), (A2), (A3));			\
	return (RETURN_EXPR);						\
      }								\
  JIT_END_STMT

#define RETURN_VAL_IF_FAIL_PRINTF6(TEST_EXPR, RETURN_EXPR, CTXT, LOC, ERR_FMT, A0, A1, A2, A3, A4, A5) \
  JIT_BEGIN_STMT							\
    if (!(TEST_EXPR))							\
      {								\
	jit_error ((CTXT), (LOC), "%s: " ERR_FMT,				\
		   __func__, (A0), (A1), (A2), (A3), (A4), (A5));	\
	return (RETURN_EXPR);						\
      }								\
  JIT_END_STMT

#define RETURN_NULL_IF_FAIL(TEST_EXPR, CTXT, LOC, ERR_MSG) \
  RETURN_VAL_IF_FAIL ((TEST_EXPR), NULL, (CTXT), (LOC), (ERR_MSG))

#define RETURN_NULL_IF_FAIL_PRINTF1(TEST_EXPR, CTXT, LOC, ERR_FMT, A0) \
  RETURN_VAL_IF_FAIL_PRINTF1 (TEST_EXPR, NULL, CTXT, LOC, ERR_FMT, A0)

#define RETURN_NULL_IF_FAIL_PRINTF2(TEST_EXPR, CTXT, LOC, ERR_FMT, A0, A1) \
  RETURN_VAL_IF_FAIL_PRINTF2 (TEST_EXPR, NULL, CTXT, LOC, ERR_FMT, A0, A1)

#define RETURN_NULL_IF_FAIL_PRINTF3(TEST_EXPR, CTXT, LOC, ERR_FMT, A0, A1, A2) \
  RETURN_VAL_IF_FAIL_PRINTF3 (TEST_EXPR, NULL, CTXT, LOC, ERR_FMT, A0, A1, A2)

#define RETURN_NULL_IF_FAIL_PRINTF4(TEST_EXPR, CTXT, LOC, ERR_FMT, A0, A1, A2, A3) \
  RETURN_VAL_IF_FAIL_PRINTF4 (TEST_EXPR, NULL, CTXT, LOC, ERR_FMT, A0, A1, A2, A3)

#define RETURN_NULL_IF_FAIL_PRINTF6(TEST_EXPR, CTXT, LOC, ERR_FMT, A0, A1, A2, A3, A4, A5) \
  RETURN_VAL_IF_FAIL_PRINTF6 (TEST_EXPR, NULL, CTXT, LOC, ERR_FMT, A0, A1, A2, A3, A4, A5)

#define RETURN_IF_FAIL(TEST_EXPR, CTXT, LOC, ERR_MSG)			\
  JIT_BEGIN_STMT							\
    if (!(TEST_EXPR))							\
      {								\
	jit_error ((CTXT), (LOC), "%s: %s", __func__, (ERR_MSG));		\
	return;							\
      }								\
  JIT_END_STMT

#define RETURN_IF_FAIL_PRINTF1(TEST_EXPR, CTXT, LOC, ERR_FMT, A0) \
  JIT_BEGIN_STMT							\
    if (!(TEST_EXPR))							\
      {								\
	jit_error ((CTXT), (LOC), "%s: " ERR_FMT,				\
		   __func__, (A0));					\
	return;							\
      }								\
  JIT_END_STMT

#define RETURN_IF_FAIL_PRINTF2(TEST_EXPR, CTXT, LOC, ERR_FMT, A0, A1) \
  JIT_BEGIN_STMT							\
    if (!(TEST_EXPR))							\
      {								\
	jit_error ((CTXT), (LOC), "%s: " ERR_FMT,				\
		   __func__, (A0), (A1));				\
	return;							\
      }								\
  JIT_END_STMT

#define RETURN_IF_FAIL_PRINTF4(TEST_EXPR, CTXT, LOC, ERR_FMT, A0, A1, A2, A3) \
  JIT_BEGIN_STMT							\
    if (!(TEST_EXPR))							\
      {								\
	jit_error ((CTXT), (LOC), "%s: " ERR_FMT,				\
		   __func__, (A0), (A1), (A2), (A3));			\
	return;							\
      }								\
  JIT_END_STMT

/* Check that BLOCK is non-NULL, and that it's OK to add statements to
   it.  */
#define RETURN_IF_NOT_VALID_BLOCK(BLOCK, LOC)				\
  JIT_BEGIN_STMT							\
    RETURN_IF_FAIL ((BLOCK), NULL, (LOC), "NULL block");		\
    RETURN_IF_FAIL_PRINTF2 (						\
      !(BLOCK)->has_been_terminated (),				\
      (BLOCK)->get_context (),						\
      (LOC),								\
      "adding to terminated block: %s (already terminated by: %s)",	\
      (BLOCK)->get_debug_string (),					\
      (BLOCK)->get_last_statement ()->get_debug_string ());		\
  JIT_END_STMT

#define RETURN_NULL_IF_NOT_VALID_BLOCK(BLOCK, LOC)			\
  JIT_BEGIN_STMT							\
    RETURN_NULL_IF_FAIL ((BLOCK), NULL, (LOC), "NULL block");		\
    RETURN_NULL_IF_FAIL_PRINTF2 (					\
      !(BLOCK)->has_been_terminated (),				\
      (BLOCK)->get_context (),						\
      (LOC),								\
      "adding to terminated block: %s (already terminated by: %s)",	\
      (BLOCK)->get_debug_string (),					\
      (BLOCK)->get_last_statement ()->get_debug_string ());		\
  JIT_END_STMT

static void
jit_error (gcc::jit::recording::context *ctxt,
	   gcc_jit_location *loc,
	   const char *fmt, ...)
  GNU_PRINTF(3, 4);

static void
jit_error (gcc::jit::recording::context *ctxt,
	   gcc_jit_location *loc,
	   const char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);

  if (ctxt)
    ctxt->add_error_va (loc, fmt, ap);
  else
    {
      /* No context?  Send to stderr.  */
      vfprintf (stderr, fmt, ap);
      fprintf (stderr, "\n");
    }

  va_end (ap);
}

static bool
compatible_types (gcc::jit::recording::type *ltype,
		  gcc::jit::recording::type *rtype)
{
  return ltype->accepts_writes_from (rtype);
}

gcc_jit_context *
gcc_jit_context_acquire (void)
{
  return new gcc_jit_context (NULL);
}

void
gcc_jit_context_release (gcc_jit_context *ctxt)
{
  delete ctxt;
}

gcc_jit_context *
gcc_jit_context_new_child_context (gcc_jit_context *parent_ctxt)
{
  return new gcc_jit_context (parent_ctxt);
}

gcc_jit_location *
gcc_jit_context_new_location (gcc_jit_context *ctxt,
			      const char *filename,
			      int line,
			      int column)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");

  return (gcc_jit_location *)ctxt->new_location (filename, line, column);
}

gcc_jit_object *
gcc_jit_location_as_object (gcc_jit_location *loc)
{
  RETURN_NULL_IF_FAIL (loc, NULL, NULL, "NULL location");

  return static_cast <gcc_jit_object *> (loc->as_object ());
}

gcc_jit_object *
gcc_jit_type_as_object (gcc_jit_type *type)
{
  RETURN_NULL_IF_FAIL (type, NULL, NULL, "NULL type");

  return static_cast <gcc_jit_object *> (type->as_object ());
}

gcc_jit_type *
gcc_jit_context_get_type (gcc_jit_context *ctxt,
			  enum gcc_jit_types type)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  /* The inner function checks "type" for us.  */

  return (gcc_jit_type *)ctxt->get_type (type);
}

gcc_jit_type *
gcc_jit_context_get_int_type (gcc_jit_context *ctxt,
			      int num_bytes, int is_signed)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  RETURN_NULL_IF_FAIL (num_bytes >= 0, ctxt, NULL, "negative size");

  return (gcc_jit_type *)ctxt->get_int_type (num_bytes, is_signed);
}

gcc_jit_type *
gcc_jit_type_get_pointer (gcc_jit_type *type)
{
  RETURN_NULL_IF_FAIL (type, NULL, NULL, "NULL type");

  return (gcc_jit_type *)type->get_pointer ();
}

gcc_jit_type *
gcc_jit_type_get_const (gcc_jit_type *type)
{
  RETURN_NULL_IF_FAIL (type, NULL, NULL, "NULL type");

  return (gcc_jit_type *)type->get_const ();
}

gcc_jit_type *
gcc_jit_type_get_volatile (gcc_jit_type *type)
{
  RETURN_NULL_IF_FAIL (type, NULL, NULL, "NULL type");

  return (gcc_jit_type *)type->get_volatile ();
}

gcc_jit_type *
gcc_jit_context_new_array_type (gcc_jit_context *ctxt,
				gcc_jit_location *loc,
				gcc_jit_type *element_type,
				int num_elements)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, loc, "NULL context");
  RETURN_NULL_IF_FAIL (element_type, ctxt, loc, "NULL type");

  return (gcc_jit_type *)ctxt->new_array_type (loc,
					       element_type,
					       num_elements);
}

gcc_jit_field *
gcc_jit_context_new_field (gcc_jit_context *ctxt,
			   gcc_jit_location *loc,
			   gcc_jit_type *type,
			   const char *name)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  RETURN_NULL_IF_FAIL (type, ctxt, loc, "NULL type");
  RETURN_NULL_IF_FAIL (name, ctxt, loc, "NULL name");

  return (gcc_jit_field *)ctxt->new_field (loc, type, name);
}

gcc_jit_object *
gcc_jit_field_as_object (gcc_jit_field *field)
{
  RETURN_NULL_IF_FAIL (field, NULL, NULL, "NULL field");

  return static_cast <gcc_jit_object *> (field->as_object ());
}

gcc_jit_struct *
gcc_jit_context_new_struct_type (gcc_jit_context *ctxt,
				 gcc_jit_location *loc,
				 const char *name,
				 int num_fields,
				 gcc_jit_field **fields)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  RETURN_NULL_IF_FAIL (name, ctxt, loc, "NULL name");
  if (num_fields)
    RETURN_NULL_IF_FAIL (fields, ctxt, loc, "NULL fields ptr");
  for (int i = 0; i < num_fields; i++)
    {
      RETURN_NULL_IF_FAIL (fields[i], ctxt, loc, "NULL field ptr");
      RETURN_NULL_IF_FAIL_PRINTF2 (
	NULL == fields[i]->get_container (),
	ctxt, loc,
	"%s is already a field of %s",
	fields[i]->get_debug_string (),
	fields[i]->get_container ()->get_debug_string ());
    }

  gcc::jit::recording::struct_ *result =
    ctxt->new_struct_type (loc, name);
  result->set_fields (loc,
		      num_fields,
		      (gcc::jit::recording::field **)fields);
  return static_cast<gcc_jit_struct *> (result);
}

gcc_jit_struct *
gcc_jit_context_new_opaque_struct (gcc_jit_context *ctxt,
				   gcc_jit_location *loc,
				   const char *name)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, loc, "NULL context");
  RETURN_NULL_IF_FAIL (name, ctxt, loc, "NULL name");

  return (gcc_jit_struct *)ctxt->new_struct_type (loc, name);
}

gcc_jit_type *
gcc_jit_struct_as_type (gcc_jit_struct *struct_type)
{
  RETURN_NULL_IF_FAIL (struct_type, NULL, NULL, "NULL struct_type");

  return static_cast <gcc_jit_type *> (struct_type->as_type ());
}

void
gcc_jit_struct_set_fields (gcc_jit_struct *struct_type,
			   gcc_jit_location *loc,
			   int num_fields,
			   gcc_jit_field **fields)
{
  RETURN_IF_FAIL (struct_type, NULL, loc, "NULL struct_type");
  gcc::jit::recording::context *ctxt = struct_type->m_ctxt;
  RETURN_IF_FAIL_PRINTF1 (
    NULL == struct_type->get_fields (), ctxt, loc,
    "%s already has had fields set",
    struct_type->get_debug_string ());
  if (num_fields)
    RETURN_IF_FAIL (fields, ctxt, loc, "NULL fields ptr");
  for (int i = 0; i < num_fields; i++)
    {
      RETURN_IF_FAIL (fields[i], ctxt, loc, "NULL field ptr");
      RETURN_IF_FAIL_PRINTF2 (
	NULL == fields[i]->get_container (),
	ctxt, loc,
	"%s is already a field of %s",
	fields[i]->get_debug_string (),
	fields[i]->get_container ()->get_debug_string ());
    }

  struct_type->set_fields (loc, num_fields,
			   (gcc::jit::recording::field **)fields);
}

/* Constructing functions.  */
gcc_jit_param *
gcc_jit_context_new_param (gcc_jit_context *ctxt,
			   gcc_jit_location *loc,
			   gcc_jit_type *type,
			   const char *name)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, loc, "NULL context");
  RETURN_NULL_IF_FAIL (type, ctxt, loc, "NULL type");
  RETURN_NULL_IF_FAIL (name, ctxt, loc, "NULL name");

  return (gcc_jit_param *)ctxt->new_param (loc, type, name);
}

gcc_jit_object *
gcc_jit_param_as_object (gcc_jit_param *param)
{
  RETURN_NULL_IF_FAIL (param, NULL, NULL, "NULL param");

  return static_cast <gcc_jit_object *> (param->as_object ());
}

gcc_jit_lvalue *
gcc_jit_param_as_lvalue (gcc_jit_param *param)
{
  RETURN_NULL_IF_FAIL (param, NULL, NULL, "NULL param");

  return (gcc_jit_lvalue *)param->as_lvalue ();
}

gcc_jit_rvalue *
gcc_jit_param_as_rvalue (gcc_jit_param *param)
{
  RETURN_NULL_IF_FAIL (param, NULL, NULL, "NULL param");

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
  RETURN_NULL_IF_FAIL (ctxt, NULL, loc, "NULL context");
  RETURN_NULL_IF_FAIL (return_type, ctxt, loc, "NULL return_type");
  RETURN_NULL_IF_FAIL (name, ctxt, loc, "NULL name");
  /* The assembler can only handle certain names, so for now, enforce
     C's rules for identiers upon the name.
     Eventually we'll need some way to interact with e.g. C++ name mangling.  */
  {
    /* Leading char: */
    char ch = *name;
    RETURN_NULL_IF_FAIL_PRINTF2 (
	IS_ASCII_ALPHA (ch) || ch == '_',
	ctxt, loc,
	"name \"%s\" contains invalid character: '%c'",
	name, ch);
    /* Subsequent chars: */
    for (const char *ptr = name + 1; (ch = *ptr); ptr++)
      {
	RETURN_NULL_IF_FAIL_PRINTF2 (
	  IS_ASCII_ALNUM (ch) || ch == '_',
	  ctxt, loc,
	  "name \"%s\" contains invalid character: '%c'",
	  name, ch);
      }
  }
  RETURN_NULL_IF_FAIL_PRINTF1 (
    (num_params == 0) || params,
    ctxt, loc,
    "NULL params creating function %s", name);
  for (int i = 0; i < num_params; i++)
    RETURN_NULL_IF_FAIL_PRINTF2 (
      params[i],
      ctxt, loc,
      "NULL parameter %i creating function %s", i, name);

  return (gcc_jit_function*)
    ctxt->new_function (loc, kind, return_type, name,
			num_params,
			(gcc::jit::recording::param **)params,
			is_variadic,
			BUILT_IN_NONE);
}

gcc_jit_function *
gcc_jit_context_get_builtin_function (gcc_jit_context *ctxt,
				      const char *name)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  RETURN_NULL_IF_FAIL (name, ctxt, NULL, "NULL name");

  return static_cast <gcc_jit_function *> (ctxt->get_builtin_function (name));
}

gcc_jit_object *
gcc_jit_function_as_object (gcc_jit_function *func)
{
  RETURN_NULL_IF_FAIL (func, NULL, NULL, "NULL function");

  return static_cast <gcc_jit_object *> (func->as_object ());
}

gcc_jit_param *
gcc_jit_function_get_param (gcc_jit_function *func, int index)
{
  RETURN_NULL_IF_FAIL (func, NULL, NULL, "NULL function");
  gcc::jit::recording::context *ctxt = func->m_ctxt;
  RETURN_NULL_IF_FAIL (index >= 0, ctxt, NULL, "negative index");
  int num_params = func->get_params ().length ();
  RETURN_NULL_IF_FAIL_PRINTF3 (index < num_params,
			       ctxt, NULL,
			       "index of %d is too large (%s has %d params)",
			       index,
			       func->get_debug_string (),
			       num_params);

  return static_cast <gcc_jit_param *> (func->get_param (index));
}

void
gcc_jit_function_dump_to_dot (gcc_jit_function *func,
			      const char *path)
{
  RETURN_IF_FAIL (func, NULL, NULL, "NULL function");
  gcc::jit::recording::context *ctxt = func->m_ctxt;
  RETURN_IF_FAIL (path, ctxt, NULL, "NULL path");

  func->dump_to_dot (path);
}

gcc_jit_block*
gcc_jit_function_new_block (gcc_jit_function *func,
			    const char *name)
{
  RETURN_NULL_IF_FAIL (func, NULL, NULL, "NULL function");
  RETURN_NULL_IF_FAIL (func->get_kind () != GCC_JIT_FUNCTION_IMPORTED,
		       func->get_context (), NULL,
		       "cannot add block to an imported function");
  /* name can be NULL.  */

  return (gcc_jit_block *)func->new_block (name);
}

gcc_jit_object *
gcc_jit_block_as_object (gcc_jit_block *block)
{
  RETURN_NULL_IF_FAIL (block, NULL, NULL, "NULL block");

  return static_cast <gcc_jit_object *> (block->as_object ());
}

gcc_jit_function *
gcc_jit_block_get_function (gcc_jit_block *block)
{
  RETURN_NULL_IF_FAIL (block, NULL, NULL, "NULL block");

  return static_cast <gcc_jit_function *> (block->get_function ());
}

gcc_jit_lvalue *
gcc_jit_context_new_global (gcc_jit_context *ctxt,
			    gcc_jit_location *loc,
			    gcc_jit_type *type,
			    const char *name)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, loc, "NULL context");
  RETURN_NULL_IF_FAIL (type, ctxt, loc, "NULL type");
  RETURN_NULL_IF_FAIL (name, ctxt, loc, "NULL name");

  return (gcc_jit_lvalue *)ctxt->new_global (loc, type, name);
}

gcc_jit_object *
gcc_jit_lvalue_as_object (gcc_jit_lvalue *lvalue)
{
  RETURN_NULL_IF_FAIL (lvalue, NULL, NULL, "NULL lvalue");

  return static_cast <gcc_jit_object *> (lvalue->as_object ());
}

gcc_jit_rvalue *
gcc_jit_lvalue_as_rvalue (gcc_jit_lvalue *lvalue)
{
  RETURN_NULL_IF_FAIL (lvalue, NULL, NULL, "NULL lvalue");

  return (gcc_jit_rvalue *)lvalue->as_rvalue ();
}

gcc_jit_object *
gcc_jit_rvalue_as_object (gcc_jit_rvalue *rvalue)
{
  RETURN_NULL_IF_FAIL (rvalue, NULL, NULL, "NULL rvalue");

  return static_cast <gcc_jit_object *> (rvalue->as_object ());
}

gcc_jit_type *
gcc_jit_rvalue_get_type (gcc_jit_rvalue *rvalue)
{
  RETURN_NULL_IF_FAIL (rvalue, NULL, NULL, "NULL rvalue");

  return static_cast <gcc_jit_type *> (rvalue->get_type ());
}

#define RETURN_NULL_IF_FAIL_NONNULL_NUMERIC_TYPE(CTXT, NUMERIC_TYPE) \
  RETURN_NULL_IF_FAIL (NUMERIC_TYPE, CTXT, NULL, "NULL type"); \
  RETURN_NULL_IF_FAIL_PRINTF1 (                                \
    NUMERIC_TYPE->is_numeric (), ctxt, NULL,                   \
    "not a numeric type: %s",                                  \
    NUMERIC_TYPE->get_debug_string ());

gcc_jit_rvalue *
gcc_jit_context_new_rvalue_from_int (gcc_jit_context *ctxt,
				     gcc_jit_type *numeric_type,
				     int value)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  RETURN_NULL_IF_FAIL_NONNULL_NUMERIC_TYPE (ctxt, numeric_type);

  return (gcc_jit_rvalue *)ctxt->new_rvalue_from_int (numeric_type, value);
}

gcc_jit_rvalue *
gcc_jit_context_zero (gcc_jit_context *ctxt,
		      gcc_jit_type *numeric_type)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  RETURN_NULL_IF_FAIL_NONNULL_NUMERIC_TYPE (ctxt, numeric_type);

  return gcc_jit_context_new_rvalue_from_int (ctxt, numeric_type, 0);
}

gcc_jit_rvalue *
gcc_jit_context_one (gcc_jit_context *ctxt,
		     gcc_jit_type *numeric_type)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  RETURN_NULL_IF_FAIL_NONNULL_NUMERIC_TYPE (ctxt, numeric_type);

  return gcc_jit_context_new_rvalue_from_int (ctxt, numeric_type, 1);
}

gcc_jit_rvalue *
gcc_jit_context_new_rvalue_from_double (gcc_jit_context *ctxt,
					gcc_jit_type *numeric_type,
					double value)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  RETURN_NULL_IF_FAIL_NONNULL_NUMERIC_TYPE (ctxt, numeric_type);

  return (gcc_jit_rvalue *)ctxt->new_rvalue_from_double (numeric_type, value);
}

gcc_jit_rvalue *
gcc_jit_context_new_rvalue_from_ptr (gcc_jit_context *ctxt,
				     gcc_jit_type *pointer_type,
				     void *value)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  RETURN_NULL_IF_FAIL (pointer_type, ctxt, NULL, "NULL type");
  RETURN_NULL_IF_FAIL_PRINTF1 (
    pointer_type->is_pointer (),
    ctxt, NULL,
    "not a pointer type (type: %s)",
    pointer_type->get_debug_string ());

  return (gcc_jit_rvalue *)ctxt->new_rvalue_from_ptr (pointer_type, value);
}

gcc_jit_rvalue *
gcc_jit_context_null (gcc_jit_context *ctxt,
		      gcc_jit_type *pointer_type)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  RETURN_NULL_IF_FAIL (pointer_type, ctxt, NULL, "NULL type");
  RETURN_NULL_IF_FAIL_PRINTF1 (
    pointer_type->is_pointer (),
    ctxt, NULL,
    "not a pointer type (type: %s)",
    pointer_type->get_debug_string ());

  return gcc_jit_context_new_rvalue_from_ptr (ctxt, pointer_type, NULL);
}

gcc_jit_rvalue *
gcc_jit_context_new_string_literal (gcc_jit_context *ctxt,
				    const char *value)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  RETURN_NULL_IF_FAIL (value, ctxt, NULL, "NULL value");

  return (gcc_jit_rvalue *)ctxt->new_string_literal (value);
}

gcc_jit_rvalue *
gcc_jit_context_new_unary_op (gcc_jit_context *ctxt,
			      gcc_jit_location *loc,
			      enum gcc_jit_unary_op op,
			      gcc_jit_type *result_type,
			      gcc_jit_rvalue *rvalue)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, loc, "NULL context");
  /* op is checked by the inner function.  */
  RETURN_NULL_IF_FAIL (result_type, ctxt, loc, "NULL result_type");
  RETURN_NULL_IF_FAIL (rvalue, ctxt, loc, "NULL rvalue");

  return (gcc_jit_rvalue *)ctxt->new_unary_op (loc, op, result_type, rvalue);
}

gcc_jit_rvalue *
gcc_jit_context_new_binary_op (gcc_jit_context *ctxt,
			       gcc_jit_location *loc,
			       enum gcc_jit_binary_op op,
			       gcc_jit_type *result_type,
			       gcc_jit_rvalue *a, gcc_jit_rvalue *b)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, loc, "NULL context");
  /* op is checked by the inner function.  */
  RETURN_NULL_IF_FAIL (result_type, ctxt, loc, "NULL result_type");
  RETURN_NULL_IF_FAIL (a, ctxt, loc, "NULL a");
  RETURN_NULL_IF_FAIL (b, ctxt, loc, "NULL b");
  RETURN_NULL_IF_FAIL_PRINTF4 (
    a->get_type () == b->get_type (),
    ctxt, loc,
    "mismatching types for binary op:"
    " a: %s (type: %s) b: %s (type: %s)",
    a->get_debug_string (),
    a->get_type ()->get_debug_string (),
    b->get_debug_string (),
    b->get_type ()->get_debug_string ());

  return (gcc_jit_rvalue *)ctxt->new_binary_op (loc, op, result_type, a, b);
}

gcc_jit_rvalue *
gcc_jit_context_new_comparison (gcc_jit_context *ctxt,
				gcc_jit_location *loc,
				enum gcc_jit_comparison op,
				gcc_jit_rvalue *a, gcc_jit_rvalue *b)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, loc, "NULL context");
  /* op is checked by the inner function.  */
  RETURN_NULL_IF_FAIL (a, ctxt, loc, "NULL a");
  RETURN_NULL_IF_FAIL (b, ctxt, loc, "NULL b");
  RETURN_NULL_IF_FAIL_PRINTF4 (
    a->get_type ()->unqualified () == b->get_type ()->unqualified (),
    ctxt, loc,
    "mismatching types for comparison:"
    " a: %s (type: %s) b: %s (type: %s)",
    a->get_debug_string (),
    a->get_type ()->get_debug_string (),
    b->get_debug_string (),
    b->get_type ()->get_debug_string ());

  return (gcc_jit_rvalue *)ctxt->new_comparison (loc, op, a, b);
}

gcc_jit_rvalue *
gcc_jit_context_new_call (gcc_jit_context *ctxt,
			  gcc_jit_location *loc,
			  gcc_jit_function *func,
			  int numargs , gcc_jit_rvalue **args)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, loc, "NULL context");
  RETURN_NULL_IF_FAIL (func, ctxt, loc, "NULL function");
  if (numargs)
    RETURN_NULL_IF_FAIL (args, ctxt, loc, "NULL args");

  int min_num_params = func->get_params ().length ();
  bool is_variadic = func->is_variadic ();

  RETURN_NULL_IF_FAIL_PRINTF3 (
    numargs >= min_num_params,
    ctxt, loc,
    "not enough arguments to function \"%s\""
    " (got %i args, expected %i)",
    func->get_name ()->c_str (),
    numargs, min_num_params);

  RETURN_NULL_IF_FAIL_PRINTF3 (
    (numargs == min_num_params || is_variadic),
    ctxt, loc,
    "too many arguments to function \"%s\""
    " (got %i args, expected %i)",
    func->get_name ()->c_str (),
    numargs, min_num_params);

  for (int i = 0; i < min_num_params; i++)
    {
      gcc::jit::recording::param *param = func->get_param (i);
      gcc_jit_rvalue *arg = args[i];

      RETURN_NULL_IF_FAIL_PRINTF4 (
	arg,
	ctxt, loc,
	"NULL argument %i to function \"%s\":"
	" param %s (type: %s)",
	i + 1,
	func->get_name ()->c_str (),
	param->get_debug_string (),
	param->get_type ()->get_debug_string ());

      RETURN_NULL_IF_FAIL_PRINTF6 (
	compatible_types (param->get_type (),
			  arg->get_type ()),
	ctxt, loc,
	"mismatching types for argument %d of function \"%s\":"
	" assignment to param %s (type: %s) from %s (type: %s)",
	i + 1,
	func->get_name ()->c_str (),
	param->get_debug_string (),
	param->get_type ()->get_debug_string (),
	arg->get_debug_string (),
	arg->get_type ()->get_debug_string ());
    }

  return (gcc_jit_rvalue *)ctxt->new_call (loc,
					   func,
					   numargs,
					   (gcc::jit::recording::rvalue **)args);
}

static bool
is_valid_cast (gcc::jit::recording::type *src_type,
	       gcc_jit_type *dst_type)
{
  bool src_is_int = src_type->is_int ();
  bool dst_is_int = dst_type->is_int ();
  bool src_is_float = src_type->is_float ();
  bool dst_is_float = dst_type->is_float ();
  bool src_is_bool = src_type->is_bool ();
  bool dst_is_bool = dst_type->is_bool ();

  if (src_is_int)
    if (dst_is_int || dst_is_float || dst_is_bool)
      return true;

  if (src_is_float)
    if (dst_is_int || dst_is_float)
      return true;

  if (src_is_bool)
    if (dst_is_int || dst_is_bool)
      return true;

  /* Permit casts between pointer types.  */
  gcc::jit::recording::type *deref_src_type = src_type->is_pointer ();
  gcc::jit::recording::type *deref_dst_type = dst_type->is_pointer ();
  if (deref_src_type && deref_dst_type)
    return true;

  return false;
}

gcc_jit_rvalue *
gcc_jit_context_new_cast (gcc_jit_context *ctxt,
			  gcc_jit_location *loc,
			  gcc_jit_rvalue *rvalue,
			  gcc_jit_type *type)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, loc, "NULL context");
  RETURN_NULL_IF_FAIL (rvalue, ctxt, loc, "NULL rvalue");
  RETURN_NULL_IF_FAIL (type, ctxt, loc, "NULL type");
  RETURN_NULL_IF_FAIL_PRINTF3 (
    is_valid_cast (rvalue->get_type (), type),
    ctxt, loc,
    "cannot cast %s from type: %s to type: %s",
    rvalue->get_debug_string (),
    rvalue->get_type ()->get_debug_string (),
    type->get_debug_string ());

  return static_cast <gcc_jit_rvalue *> (ctxt->new_cast (loc, rvalue, type));
}

extern gcc_jit_lvalue *
gcc_jit_context_new_array_access (gcc_jit_context *ctxt,
				  gcc_jit_location *loc,
				  gcc_jit_rvalue *ptr,
				  gcc_jit_rvalue *index)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, loc, "NULL context");
  RETURN_NULL_IF_FAIL (ptr, ctxt, loc, "NULL ptr");
  RETURN_NULL_IF_FAIL (index, ctxt, loc, "NULL index");
  RETURN_NULL_IF_FAIL_PRINTF2 (
    ptr->get_type ()->dereference (),
    ctxt, loc,
    "ptr: %s (type: %s) is not a pointer or array",
    ptr->get_debug_string (),
    ptr->get_type ()->get_debug_string ());
  RETURN_NULL_IF_FAIL_PRINTF2 (
    index->get_type ()->is_numeric (),
    ctxt, loc,
    "index: %s (type: %s) is not of numeric type",
    index->get_debug_string (),
    index->get_type ()->get_debug_string ());

  return (gcc_jit_lvalue *)ctxt->new_array_access (loc, ptr, index);
}

gcc_jit_context *
gcc_jit_object_get_context (gcc_jit_object *obj)
{
  RETURN_NULL_IF_FAIL (obj, NULL, NULL, "NULL object");

  return static_cast <gcc_jit_context *> (obj->get_context ());
}

const char *
gcc_jit_object_get_debug_string (gcc_jit_object *obj)
{
  RETURN_NULL_IF_FAIL (obj, NULL, NULL, "NULL object");

  return obj->get_debug_string ();
}

gcc_jit_lvalue *
gcc_jit_lvalue_access_field (gcc_jit_lvalue *struct_,
			     gcc_jit_location *loc,
			     gcc_jit_field *field)
{
  RETURN_NULL_IF_FAIL (struct_, NULL, loc, "NULL struct");
  gcc::jit::recording::context *ctxt = struct_->m_ctxt;
  RETURN_NULL_IF_FAIL (field, ctxt, loc, "NULL field");
  RETURN_NULL_IF_FAIL_PRINTF1 (field->get_container (), field->m_ctxt, loc,
			       "field %s has not been placed in a struct",
			       field->get_debug_string ());

  return (gcc_jit_lvalue *)struct_->access_field (loc, field);
}

gcc_jit_rvalue *
gcc_jit_rvalue_access_field (gcc_jit_rvalue *struct_,
			     gcc_jit_location *loc,
			     gcc_jit_field *field)
{
  RETURN_NULL_IF_FAIL (struct_, NULL, loc, "NULL struct");
  gcc::jit::recording::context *ctxt = struct_->m_ctxt;
  RETURN_NULL_IF_FAIL (field, ctxt, loc, "NULL field");
  RETURN_NULL_IF_FAIL_PRINTF1 (field->get_container (), field->m_ctxt, loc,
			       "field %s has not been placed in a struct",
			       field->get_debug_string ());

  return (gcc_jit_rvalue *)struct_->access_field (loc, field);
}

gcc_jit_lvalue *
gcc_jit_rvalue_dereference_field (gcc_jit_rvalue *ptr,
				  gcc_jit_location *loc,
				  gcc_jit_field *field)
{
  RETURN_NULL_IF_FAIL (ptr, NULL, loc, "NULL ptr");
  RETURN_NULL_IF_FAIL (field, NULL, loc, "NULL field");
  gcc::jit::recording::type *underlying_type =
    ptr->get_type ()->is_pointer ();
  RETURN_NULL_IF_FAIL_PRINTF1 (field->get_container (), field->m_ctxt, loc,
			       "field %s has not been placed in a struct",
			       field->get_debug_string ());
  RETURN_NULL_IF_FAIL_PRINTF3 (
    underlying_type,
    ptr->m_ctxt, loc,
    "dereference of non-pointer %s (type: %s) when accessing ->%s",
    ptr->get_debug_string (),
    ptr->get_type ()->get_debug_string (),
    field->get_debug_string ());
  RETURN_NULL_IF_FAIL_PRINTF2 (
    (field->get_container ()->unqualified ()
     == underlying_type->unqualified ()),
    ptr->m_ctxt, loc,
    "%s is not a field of %s",
    field->get_debug_string (),
    underlying_type->get_debug_string ());

  return (gcc_jit_lvalue *)ptr->dereference_field (loc, field);
}

gcc_jit_lvalue *
gcc_jit_rvalue_dereference (gcc_jit_rvalue *rvalue,
			    gcc_jit_location *loc)
{
  RETURN_NULL_IF_FAIL (rvalue, NULL, loc, "NULL rvalue");

  gcc::jit::recording::type *underlying_type =
    rvalue->get_type ()->is_pointer ();

  RETURN_NULL_IF_FAIL_PRINTF2 (
    underlying_type,
    rvalue->m_ctxt, loc,
    "dereference of non-pointer %s (type: %s)",
    rvalue->get_debug_string (),
    rvalue->get_type ()->get_debug_string ());

  return (gcc_jit_lvalue *)rvalue->dereference (loc);
}

gcc_jit_rvalue *
gcc_jit_lvalue_get_address (gcc_jit_lvalue *lvalue,
			    gcc_jit_location *loc)
{
  RETURN_NULL_IF_FAIL (lvalue, NULL, loc, "NULL lvalue");

  return (gcc_jit_rvalue *)lvalue->get_address (loc);
}

gcc_jit_lvalue *
gcc_jit_function_new_local (gcc_jit_function *func,
			    gcc_jit_location *loc,
			    gcc_jit_type *type,
			    const char *name)
{
  RETURN_NULL_IF_FAIL (func, NULL, loc, "NULL function");
  gcc::jit::recording::context *ctxt = func->m_ctxt;
  RETURN_NULL_IF_FAIL (func->get_kind () != GCC_JIT_FUNCTION_IMPORTED,
		       ctxt, loc,
		       "Cannot add locals to an imported function");
  RETURN_NULL_IF_FAIL (type, ctxt, loc, "NULL type");
  RETURN_NULL_IF_FAIL (name, ctxt, loc, "NULL name");

  return (gcc_jit_lvalue *)func->new_local (loc, type, name);
}

void
gcc_jit_block_add_eval (gcc_jit_block *block,
			gcc_jit_location *loc,
			gcc_jit_rvalue *rvalue)
{
  RETURN_IF_NOT_VALID_BLOCK (block, loc);
  gcc::jit::recording::context *ctxt = block->get_context ();
  RETURN_IF_FAIL (rvalue, ctxt, loc, "NULL rvalue");

  return block->add_eval (loc, rvalue);
}

void
gcc_jit_block_add_assignment (gcc_jit_block *block,
			      gcc_jit_location *loc,
			      gcc_jit_lvalue *lvalue,
			      gcc_jit_rvalue *rvalue)
{
  RETURN_IF_NOT_VALID_BLOCK (block, loc);
  gcc::jit::recording::context *ctxt = block->get_context ();
  RETURN_IF_FAIL (lvalue, ctxt, loc, "NULL lvalue");
  RETURN_IF_FAIL (rvalue, ctxt, loc, "NULL rvalue");
  RETURN_IF_FAIL_PRINTF4 (
    compatible_types (lvalue->get_type (),
		      rvalue->get_type ()),
    ctxt, loc,
    "mismatching types:"
    " assignment to %s (type: %s) from %s (type: %s)",
    lvalue->get_debug_string (),
    lvalue->get_type ()->get_debug_string (),
    rvalue->get_debug_string (),
    rvalue->get_type ()->get_debug_string ());

  return block->add_assignment (loc, lvalue, rvalue);
}

void
gcc_jit_block_add_assignment_op (gcc_jit_block *block,
				 gcc_jit_location *loc,
				 gcc_jit_lvalue *lvalue,
				 enum gcc_jit_binary_op op,
				 gcc_jit_rvalue *rvalue)
{
  RETURN_IF_NOT_VALID_BLOCK (block, loc);
  gcc::jit::recording::context *ctxt = block->get_context ();
  RETURN_IF_FAIL (lvalue, ctxt, loc, "NULL lvalue");
  /* FIXME: op is checked by new_binary_op */
  RETURN_IF_FAIL (rvalue, ctxt, loc, "NULL rvalue");

  return block->add_assignment_op (loc, lvalue, op, rvalue);
}

static bool
is_bool (gcc_jit_rvalue *boolval)
{
  gcc::jit::recording::type *actual_type = boolval->get_type ();
  gcc::jit::recording::type *bool_type =
    boolval->m_ctxt->get_type (GCC_JIT_TYPE_BOOL);
  return actual_type == bool_type;
}

void
gcc_jit_block_end_with_conditional (gcc_jit_block *block,
				    gcc_jit_location *loc,
				    gcc_jit_rvalue *boolval,
				    gcc_jit_block *on_true,
				    gcc_jit_block *on_false)
{
  RETURN_IF_NOT_VALID_BLOCK (block, loc);
  gcc::jit::recording::context *ctxt = block->get_context ();
  RETURN_IF_FAIL (boolval, ctxt, loc, "NULL boolval");
  RETURN_IF_FAIL_PRINTF2 (
   is_bool (boolval), ctxt, loc,
   "%s (type: %s) is not of boolean type ",
   boolval->get_debug_string (),
   boolval->get_type ()->get_debug_string ());
  RETURN_IF_FAIL (on_true, ctxt, loc, "NULL on_true");
  RETURN_IF_FAIL (on_true, ctxt, loc, "NULL on_false");
  RETURN_IF_FAIL_PRINTF4 (
    block->get_function () == on_true->get_function (),
    ctxt, loc,
    "\"on_true\" block is not in same function:"
    " source block %s is in function %s"
    " whereas target block %s is in function %s",
    block->get_debug_string (),
    block->get_function ()->get_debug_string (),
    on_true->get_debug_string (),
    on_true->get_function ()->get_debug_string ());
  RETURN_IF_FAIL_PRINTF4 (
    block->get_function () == on_false->get_function (),
    ctxt, loc,
    "\"on_false\" block is not in same function:"
    " source block %s is in function %s"
    " whereas target block %s is in function %s",
    block->get_debug_string (),
    block->get_function ()->get_debug_string (),
    on_false->get_debug_string (),
    on_false->get_function ()->get_debug_string ());

  return block->end_with_conditional (loc, boolval, on_true, on_false);
}

void
gcc_jit_block_add_comment (gcc_jit_block *block,
			   gcc_jit_location *loc,
			   const char *text)
{
  RETURN_IF_NOT_VALID_BLOCK (block, loc);
  gcc::jit::recording::context *ctxt = block->get_context ();
  RETURN_IF_FAIL (text, ctxt, loc, "NULL text");

  block->add_comment (loc, text);
}

void
gcc_jit_block_end_with_jump (gcc_jit_block *block,
			     gcc_jit_location *loc,
			     gcc_jit_block *target)
{
  RETURN_IF_NOT_VALID_BLOCK (block, loc);
  gcc::jit::recording::context *ctxt = block->get_context ();
  RETURN_IF_FAIL (target, ctxt, loc, "NULL target");
  RETURN_IF_FAIL_PRINTF4 (
    block->get_function () == target->get_function (),
    ctxt, loc,
    "target block is not in same function:"
    " source block %s is in function %s"
    " whereas target block %s is in function %s",
    block->get_debug_string (),
    block->get_function ()->get_debug_string (),
    target->get_debug_string (),
    target->get_function ()->get_debug_string ());

  block->end_with_jump (loc, target);
}

void
gcc_jit_block_end_with_return (gcc_jit_block *block,
			       gcc_jit_location *loc,
			       gcc_jit_rvalue *rvalue)
{
  RETURN_IF_NOT_VALID_BLOCK (block, loc);
  gcc::jit::recording::context *ctxt = block->get_context ();
  gcc::jit::recording::function *func = block->get_function ();
  RETURN_IF_FAIL (rvalue, ctxt, loc, "NULL rvalue");
  RETURN_IF_FAIL_PRINTF4 (
    compatible_types (
      func->get_return_type (),
      rvalue->get_type ()),
    ctxt, loc,
    "mismatching types:"
    " return of %s (type: %s) in function %s (return type: %s)",
    rvalue->get_debug_string (),
    rvalue->get_type ()->get_debug_string (),
    func->get_debug_string (),
    func->get_return_type ()->get_debug_string ());

  return block->end_with_return (loc, rvalue);
}

void
gcc_jit_block_end_with_void_return (gcc_jit_block *block,
				    gcc_jit_location *loc)
{
  RETURN_IF_NOT_VALID_BLOCK (block, loc);
  gcc::jit::recording::context *ctxt = block->get_context ();
  gcc::jit::recording::function *func = block->get_function ();
  RETURN_IF_FAIL_PRINTF2 (
    func->get_return_type () == ctxt->get_type (GCC_JIT_TYPE_VOID),
    ctxt, loc,
    "mismatching types:"
    " void return in function %s (return type: %s)",
    func->get_debug_string (),
    func->get_return_type ()->get_debug_string ());

  return block->end_with_return (loc, NULL);
}

/**********************************************************************
 Option-management
 **********************************************************************/

void
gcc_jit_context_set_str_option (gcc_jit_context *ctxt,
				enum gcc_jit_str_option opt,
				const char *value)
{
  RETURN_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  /* opt is checked by the inner function.
     value can be NULL.  */

  ctxt->set_str_option (opt, value);
}

void
gcc_jit_context_set_int_option (gcc_jit_context *ctxt,
				enum gcc_jit_int_option opt,
				int value)
{
  RETURN_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  /* opt is checked by the inner function.  */

  ctxt->set_int_option (opt, value);
}

void
gcc_jit_context_set_bool_option (gcc_jit_context *ctxt,
				 enum gcc_jit_bool_option opt,
				 int value)
{
  RETURN_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  /* opt is checked by the inner function.  */

  ctxt->set_bool_option (opt, value);
}

gcc_jit_result *
gcc_jit_context_compile (gcc_jit_context *ctxt)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");

  return (gcc_jit_result *)ctxt->compile ();
}

void
gcc_jit_context_dump_to_file (gcc_jit_context *ctxt,
			      const char *path,
			      int update_locations)
{
  RETURN_IF_FAIL (ctxt, NULL, NULL, "NULL context");
  RETURN_IF_FAIL (path, ctxt, NULL, "NULL path");
  ctxt->dump_to_file (path, update_locations);
}

const char *
gcc_jit_context_get_first_error (gcc_jit_context *ctxt)
{
  RETURN_NULL_IF_FAIL (ctxt, NULL, NULL, "NULL context");

  return ctxt->get_first_error ();
}

void *
gcc_jit_result_get_code (gcc_jit_result *result,
			 const char *fnname)
{
  RETURN_NULL_IF_FAIL (result, NULL, NULL, "NULL result");
  RETURN_NULL_IF_FAIL (fnname, NULL, NULL, "NULL fnname");

  return result->get_code (fnname);
}

void
gcc_jit_result_release (gcc_jit_result *result)
{
  RETURN_IF_FAIL (result, NULL, NULL, "NULL result");

  delete result;
}
