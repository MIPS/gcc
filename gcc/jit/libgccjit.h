/*
  A pure C API to enable client code to embed GCC as a JIT-compiler.
 */
#ifndef LIBGCCJIT_H
#define LIBGCCJIT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Various opaque structs. */
typedef struct gcc_jit_context gcc_jit_context;
typedef struct gcc_jit_result gcc_jit_result;
typedef struct gcc_jit_location gcc_jit_location;
typedef struct gcc_jit_type gcc_jit_type;
typedef struct gcc_jit_field gcc_jit_field;
typedef struct gcc_jit_function gcc_jit_function;
typedef struct gcc_jit_label gcc_jit_label;
typedef struct gcc_jit_rvalue gcc_jit_rvalue;
typedef struct gcc_jit_lvalue gcc_jit_lvalue;
typedef struct gcc_jit_param gcc_jit_param;
typedef struct gcc_jit_local gcc_jit_local;
typedef struct gcc_jit_loop gcc_jit_loop;

/*
   Acquire a JIT-compilation context.

   FIXME: error-handling?
*/
extern gcc_jit_context *
gcc_jit_context_acquire (void);

/* Release the context.  After this call, it's no longer valid to use
   the ctxt.  */
extern void
gcc_jit_context_release (gcc_jit_context *ctxt);

/* A function-pointer type.
   This is the client-provided hook for calling into the code-generation
   API.
   It should return 0 if there were no errors, or nonzero if errors
   occurred (e.g. within client code that the library has no knowledge
   of).  */
typedef int (*gcc_jit_code_callback) (gcc_jit_context *ctxt,
				      void *user_data);

extern void
gcc_jit_context_set_code_factory (gcc_jit_context *ctxt,
				  gcc_jit_code_callback cb,
				  void *user_data);
     /* clearly I'm handwaving for now about the details of what that
	callback looks like, but by doing it in a callback we also
	avoid dealing with lifetimes of the results: entities can
	only be referenced within the lifetime of the callback - once
	control returns out of the callback, you can't use them.  This
	avoids having to expose details of GCC's garbage-collector,
	I hope. */

/**********************************************************************
 Functions for use within the code factory.
 **********************************************************************/

/* Access to specific types.  */
extern gcc_jit_type *
gcc_jit_context_get_void_type (gcc_jit_context *ctxt);

extern gcc_jit_type *
gcc_jit_context_get_char_type (gcc_jit_context *ctxt);

extern gcc_jit_type *
gcc_jit_context_get_int_type (gcc_jit_context *ctxt);

extern gcc_jit_type *
gcc_jit_context_get_float_type (gcc_jit_context *ctxt);

extern gcc_jit_type *
gcc_jit_context_get_double_type (gcc_jit_context *ctxt);

/* Constructing new types. */
extern gcc_jit_type *
gcc_jit_type_get_pointer (gcc_jit_type *type);

extern gcc_jit_type *
gcc_jit_type_get_const (gcc_jit_type *type);

/* Struct-handling.  */
extern gcc_jit_field *
gcc_jit_context_new_field (gcc_jit_context *ctxt,
			   gcc_jit_location *loc,
			   gcc_jit_type *type,
			   const char *name);

extern gcc_jit_type *
gcc_jit_context_new_struct_type (gcc_jit_context *ctxt,
				 gcc_jit_location *loc,
				 const char *name,
				 int num_fields,
				 gcc_jit_field **fields);

/* Constructing functions.  */
extern gcc_jit_param *
gcc_jit_context_new_param (gcc_jit_context *ctxt,
			   gcc_jit_location *loc,
			   gcc_jit_type *type,
			   const char *name);

extern gcc_jit_lvalue *
gcc_jit_param_as_lvalue (gcc_jit_param *param);

extern gcc_jit_rvalue *
gcc_jit_param_as_rvalue (gcc_jit_param *param);

enum gcc_jit_function_kind
{
  /* Function is defined by the client code and visible
     by name outside of the JIT.  */
  GCC_JIT_FUNCTION_EXPORTED,

  /* Function is defined by the client code, but is invisible
     outside of the JIT.  Analogous to a "static" function.  */
  GCC_JIT_FUNCTION_INTERNAL,

  /* Function is not defined by the client code; we're merely
     referring to it.  Analogous to using an "extern" function from a
     header file.  */
  GCC_JIT_FUNCTION_IMPORTED

};


extern gcc_jit_function *
gcc_jit_context_new_function (gcc_jit_context *ctxt,
			      gcc_jit_location *loc,
			      enum gcc_jit_function_kind kind,
			      gcc_jit_type *return_type,
			      const char *name,
			      int num_params,
			      gcc_jit_param **params,
			      int is_variadic);

extern gcc_jit_label *
gcc_jit_function_new_forward_label (gcc_jit_function *func,
				    const char *name);

extern gcc_jit_lvalue *
gcc_jit_context_new_global (gcc_jit_context *ctxt,
			    gcc_jit_location *loc,
			    gcc_jit_type *type,
			    const char *name);

extern gcc_jit_local *
gcc_jit_context_new_local (gcc_jit_context *ctxt,
			   gcc_jit_location *loc,
			   gcc_jit_type *type,
			   const char *name);

extern gcc_jit_lvalue *
gcc_jit_local_as_lvalue (gcc_jit_local *local);

extern gcc_jit_rvalue *
gcc_jit_local_as_rvalue (gcc_jit_local *local);

extern gcc_jit_rvalue *
gcc_jit_lvalue_as_rvalue (gcc_jit_lvalue *lvalue);

/* Integer constants. */
extern gcc_jit_rvalue *
gcc_jit_context_new_rvalue_from_int (gcc_jit_context *ctxt,
				     gcc_jit_type *type,
				     int value);

extern gcc_jit_rvalue *
gcc_jit_context_zero (gcc_jit_context *ctxt,
		      gcc_jit_type *type);

extern gcc_jit_rvalue *
gcc_jit_context_one (gcc_jit_context *ctxt,
		     gcc_jit_type *type);

/* String literals. */
extern gcc_jit_rvalue *
gcc_jit_context_new_string_literal (gcc_jit_context *ctxt,
				    const char *value);


enum gcc_jit_binary_op
{
  GCC_JIT_BINARY_OP_PLUS,
  GCC_JIT_BINARY_OP_MINUS,
  GCC_JIT_BINARY_OP_MULT
};

extern gcc_jit_rvalue *
gcc_jit_context_new_binary_op (gcc_jit_context *ctxt,
			       gcc_jit_location *loc,
			       enum gcc_jit_binary_op op,
			       gcc_jit_type *result_type,
			       gcc_jit_rvalue *a, gcc_jit_rvalue *b);

enum gcc_jit_comparison
{
  GCC_JIT_COMPARISON_LT,
  GCC_JIT_COMPARISON_GE
};

extern gcc_jit_rvalue *
gcc_jit_context_new_comparison (gcc_jit_context *ctxt,
				gcc_jit_location *loc,
				enum gcc_jit_comparison op,
				gcc_jit_rvalue *a, gcc_jit_rvalue *b);

extern gcc_jit_rvalue *
gcc_jit_context_new_call (gcc_jit_context *ctxt,
			  gcc_jit_location *loc,
			  gcc_jit_function *func,
			  int numargs , gcc_jit_rvalue **args);

extern gcc_jit_rvalue *
gcc_jit_context_new_array_lookup (gcc_jit_context *ctxt,
				  gcc_jit_location *loc,
				  gcc_jit_rvalue *ptr,
				  gcc_jit_rvalue *index);

/* Field access, either s.field or s->field.  */
extern gcc_jit_lvalue *
gcc_jit_context_new_field_access (gcc_jit_context *ctxt,
				  gcc_jit_location *loc,
				  gcc_jit_rvalue *ptr_or_struct,
				  const char *fieldname);

/* Add evaluation of an rvalue, discarding the result
   (e.g. a function call that "returns" void).  */
extern void
gcc_jit_function_add_eval (gcc_jit_function *func,
			   gcc_jit_location *loc,
			   gcc_jit_rvalue *rvalue);

extern void
gcc_jit_function_add_assignment (gcc_jit_function *func,
				 gcc_jit_location *loc,
				 gcc_jit_lvalue *lvalue,
				 gcc_jit_rvalue *rvalue);

/**
   Modify an lvalue (analogous to "+=" and friends)
 */
extern void
gcc_jit_function_add_assignment_op (gcc_jit_function *func,
				    gcc_jit_location *loc,
				    gcc_jit_lvalue *lvalue,
				    enum gcc_jit_binary_op op,
				    gcc_jit_rvalue *rvalue);

extern void
gcc_jit_function_add_conditional (gcc_jit_function *func,
				  gcc_jit_location *loc,
				  gcc_jit_rvalue *boolval,
				  gcc_jit_label *on_true,
				  gcc_jit_label *on_false);

extern gcc_jit_label *
gcc_jit_function_add_label (gcc_jit_function *func,
			    gcc_jit_location *loc,
			    const char *name);

extern void
gcc_jit_function_place_forward_label (gcc_jit_function *func,
				      gcc_jit_label *lab);

extern void
gcc_jit_function_add_jump (gcc_jit_function *func,
			   gcc_jit_location *loc,
			   gcc_jit_label *target);

extern void
gcc_jit_function_add_return (gcc_jit_function *func,
			     gcc_jit_location *loc,
			     gcc_jit_rvalue *rvalue);

/* Helper function for creating a loop:
      while (boolval)
	{
	   BODY;
	}

  Statements will be added to the body of the loop until
  gcc_jit_loop_end is called.
*/
extern gcc_jit_loop *
gcc_jit_function_new_loop (gcc_jit_function *func,
			   gcc_jit_location *loc,
			   gcc_jit_rvalue *boolval);

extern void
gcc_jit_loop_end (gcc_jit_loop *loop,
		  gcc_jit_location *loc);

/**********************************************************************
 Option-management
 **********************************************************************/

/* Options taking string values */
enum gcc_jit_str_option
{
  GCC_JIT_STR_OPTION_PROGNAME,
  GCC_JIT_NUM_STR_OPTIONS
};

/* Options taking int values */
enum gcc_jit_int_option
{
  GCC_JIT_INT_OPTION_OPTIMIZATION_LEVEL,
  GCC_JIT_NUM_INT_OPTIONS
};

/* Options taking boolean values */
enum gcc_jit_bool_option
{
  GCC_JIT_BOOL_OPTION_DEBUGINFO,
  GCC_JIT_BOOL_OPTION_DUMP_INITIAL_TREE,
  GCC_JIT_BOOL_OPTION_DUMP_INITIAL_GIMPLE,

  /* If true, gcc_jit_context_compile will dump copious
     amount of information on what it's doing to various
     files within a temporary directory.  Use
     GCC_JIT_BOOL_OPTION_KEEP_INTERMEDIATES (see below) to
     see the results.  The files are intended to be human-readable,
     but the exact files and their formats are subject to change.
  */
  GCC_JIT_BOOL_OPTION_DUMP_EVERYTHING,

  /* If true, libgccjit will aggressively run its garbage collector, to
     shake out bugs.  This is likely to only be of interest to
     developers *of* the library.  */
  GCC_JIT_BOOL_OPTION_SELFCHECK_GC,

  /* If true, gcc_jit_context_release will not clean up
     intermediate files written to the filesystem, and will display
     their location on stderr.  */
  GCC_JIT_BOOL_OPTION_KEEP_INTERMEDIATES,

  GCC_JIT_NUM_BOOL_OPTIONS
};

/* The context directly stores the (const char *), so the passed string
   must outlive the context.  */
extern void
gcc_jit_context_set_str_option (gcc_jit_context *ctxt,
				enum gcc_jit_str_option opt,
				const char *value);

extern void
gcc_jit_context_set_int_option (gcc_jit_context *ctxt,
				enum gcc_jit_int_option opt,
				int value);

extern void
gcc_jit_context_set_bool_option (gcc_jit_context *ctxt,
				 enum gcc_jit_bool_option opt,
				 int value);

extern gcc_jit_result *
gcc_jit_context_compile (gcc_jit_context *ctxt);
  /* This actually calls into GCC and runs the build, all
     in a mutex for now.  The result is a wrapper around a .so file.
     Can we only call this once on a given context?  */


/* Locate a given function within the built machine code.
   This will need to be cast to a function pointer of the
   correct type before it can be called. */
extern void *
gcc_jit_result_get_code (gcc_jit_result *result,
			 const char *funcname);

extern void
gcc_jit_result_release (gcc_jit_result *result);
  /* Once we're done with the code, this unloads the built .so file: */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* LIBGCCJIT_H  */
