#include "libgccjit.h"

#include "tree.h"
#include "tree-iterator.h"

namespace gcc {

namespace jit {

class result;
class location;
class type;
class field;
class function;
class label;
class rvalue;
class lvalue;
class param;
class local;
class loop;

/* A JIT-compilation context.  */
class context
{
public:
  ~context ();

  void gt_ggc_mx ();

  void
  set_code_factory (gcc_jit_code_callback cb,
		    void *user_data);

  type *
  get_void_type ();

  type *
  get_char_type ();

  type *
  get_int_type ();

  type *
  get_float_type ();

  type *
  get_double_type ();

  field *
  new_field (location *loc,
	     type *type,
	     const char *name);

  type *
  new_struct_type (location *loc,
		   const char *name,
		   int num_fields,
		   field **fields);


  param *
  new_param (location *loc,
	     type *type,
	     const char *name);

  function *
  new_function (location *loc,
		enum gcc_jit_function_kind kind,
		type *return_type,
		const char *name,
		int num_params,
		param **params,
		int is_variadic);

  lvalue *
  new_global (location *loc,
	      type *type,
	      const char *name);

  local *
  new_local (location *loc,
	     type *type,
	     const char *name);

  rvalue *
  new_rvalue_from_int (type *type,
		       int value);

  rvalue *
  new_string_literal (const char *value);

  rvalue *
  new_binary_op (location *loc,
		 enum gcc_jit_binary_op op,
		 type *result_type,
		 rvalue *a, rvalue *b);

  rvalue *
  new_comparison (location *loc,
		  enum gcc_jit_comparison op,
		  rvalue *a, rvalue *b);

  rvalue *
  new_call (location *loc,
	    function *func,
	    int numargs , rvalue **args);

  rvalue *
  new_array_lookup (location *loc,
		    rvalue *ptr,
		    rvalue *index);

  lvalue *
  new_field_access (location *loc,
		    rvalue *ptr_or_struct,
		    const char *fieldname);

  void
  set_str_option (enum gcc_jit_str_option opt,
		  const char *value);

  void
  set_int_option (enum gcc_jit_int_option opt,
		  int value);

  void
  set_bool_option (enum gcc_jit_bool_option opt,
		   int value);

  int
  get_bool_option (enum gcc_jit_bool_option opt) const
  {
    return m_bool_options[opt];
  }

  result *
  compile ();

  void
  invoke_code_factory ();

  bool
  within_code_factory () const { return m_within_code_factory; }

  void
  add_error (const char *msg);

private:
  gcc_jit_code_callback m_code_factory;
  bool m_within_code_factory;
  void *m_user_data;
  int m_cb_result; /* Result from client-provided code factory.  */
  vec<function *> m_functions;
  const char *m_str_options[GCC_JIT_NUM_STR_OPTIONS];
  int m_int_options[GCC_JIT_NUM_INT_OPTIONS];
  bool m_bool_options[GCC_JIT_NUM_BOOL_OPTIONS];
  tree m_char_array_type_node;
  tree m_const_char_ptr;
};

/* The result of JIT-compilation.  */
class result
{
public:
  result(void *dso_handle);

  virtual ~result();

  void *
  get_code (const char *funcname);

private:
  void *m_dso_handle;
};

extern context *active_jit_ctxt;

/* A temporary wrapper object.
   These objects are (mostly) only valid within the code factory callback.
   We allocate them on the GC heap, so that they will be cleaned
   the next time the GC collects.
   The exception is the "function" class, which is tracked and marked by
   the jit::context, since it needs to stay alive during post-processing
   (when the GC could run). */
class wrapper
{
public:
  /* Allocate in the GC heap.  */
  void *operator new (size_t sz);

};

class location : public wrapper
{
};

class type : public wrapper
{
public:
  type (tree inner)
    : m_inner(inner)
  {}

  tree as_tree () const { return m_inner; }

  type *get_pointer () const { return new type (build_pointer_type (m_inner)); }

  type *get_const () const
  {
    return new type (build_qualified_type (m_inner, TYPE_QUAL_CONST));
  }

private:
  tree m_inner;
};

class field : public wrapper
{
public:
  field (tree inner)
    : m_inner(inner)
  {}

  tree as_tree () const { return m_inner; }

private:
  tree m_inner;
};

class function : public wrapper
{
public:
  function(context *ctxt, tree fndecl, enum gcc_jit_function_kind kind);

  void gt_ggc_mx ();

  tree get_return_type_as_tree () const;

  tree as_fndecl () const { return m_inner_fndecl; }

  label*
  new_forward_label (const char *name);

  void
  add_eval (location *loc,
	    rvalue *rvalue);

  void
  add_assignment (location *loc,
		  lvalue *lvalue,
		  rvalue *rvalue);

  void
  add_conditional (location *loc,
		   rvalue *boolval,
		   label *on_true,
		   label *on_false);

  label *
  add_label (location *loc,
	     const char *name);

  void
  place_forward_label (label *lab);

  void
  add_jump (location *loc,
	    label *target);

  void
  add_return (location *loc,
	      rvalue *rvalue);

  loop *
  new_loop (location *loc,
	    rvalue *boolval);

  void
  postprocess ();

public:
  context *m_ctxt;

private:
  tree m_inner_fndecl;
  enum gcc_jit_function_kind m_kind;
  tree m_stmt_list;
  tree_stmt_iterator m_stmt_iter;
};

class label : public wrapper
{
public:
  label (function *func,
	 const char *name);

  tree as_label_decl () const { return m_label_decl; }

private:
  tree m_label_decl;

public: // for now
  tree m_label_expr;
};

class rvalue : public wrapper
{
public:
  rvalue (tree inner)
    : m_inner(inner)
  {}

  rvalue *
  as_rvalue () { return this; }

  tree as_tree () const { return m_inner; }

  type *
  get_type () { return new type (TREE_TYPE (m_inner)); }

private:
  tree m_inner;
};

class lvalue : public rvalue
{
public:
  lvalue (tree inner)
    : rvalue(inner)
  {}

  lvalue *
  as_lvalue () { return this; }
};

class param : public lvalue
{
public:
  param (tree inner)
    : lvalue(inner)
  {}
};

class local : public lvalue
{
public:
  local (tree inner)
    : lvalue(inner)
  {}
};

class loop : public wrapper
{
public:
  loop (function *func, location *loc, rvalue *boolval);

  void end (location *loc);

private:
  function *m_func;
  label *m_label_cond;
  label *m_label_body;
  label *m_label_end;
};


} // namespace gcc::jit

} // namespace gcc

