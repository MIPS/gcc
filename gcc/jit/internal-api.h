#include "libgccjit.h"

#include "tree.h"
#include "tree-iterator.h"

#include <utility> // for std::pair

#ifdef GCC_VERSION
#if GCC_VERSION >= 4001
#define GNU_PRINTF(M, N) __attribute__ ((format (gnu_printf, (M), (N))))
#else
#define GNU_PRINTF(M, N)
#endif
#endif

namespace gcc {

namespace jit {

class result;
class source_file;
class source_line;
class location;
class type;
class field;
class function;
class label;
class rvalue;
class lvalue;
class param;
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

  location *
  new_location (const char *filename,
		int line,
		int column);

  type *
  get_type (enum gcc_jit_types type);

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

  rvalue *
  new_rvalue_from_int (type *type,
		       int value);

  rvalue *
  new_rvalue_from_double (type *type,
			  double value);

  rvalue *
  new_rvalue_from_ptr (type *type,
		       void *value);

  rvalue *
  new_string_literal (const char *value);

  rvalue *
  new_unary_op (location *loc,
		enum gcc_jit_unary_op op,
		type *result_type,
		rvalue *a);

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

  void
  set_str_option (enum gcc_jit_str_option opt,
		  const char *value);

  void
  set_int_option (enum gcc_jit_int_option opt,
		  int value);

  void
  set_bool_option (enum gcc_jit_bool_option opt,
		   int value);

  const char *
  get_str_option (enum gcc_jit_str_option opt) const
  {
    return m_str_options[opt];
  }

  int
  get_int_option (enum gcc_jit_int_option opt) const
  {
    return m_int_options[opt];
  }

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
  add_error (const char *fmt, ...)
      GNU_PRINTF(2, 3);

  void
  add_error_va (const char *fmt, va_list ap)
      GNU_PRINTF(2, 0);

  const char *
  get_first_error () const;

  void
  set_tree_location (tree t, location *loc);

  tree
  new_field_access (location *loc,
		    tree datum,
		    const char *fieldname);

  tree
  new_dereference (tree ptr, location *loc);

  tree
  as_truth_value (tree expr, location *loc);

private:
  void dump_generated_code ();

  source_file *
  get_source_file (const char *filename);

  void handle_locations ();


  /* Did errors occur in the client callback (either recorded
     by our internal checking, or reported by the client).  */
  bool errors_occurred () const
  {
    return m_error_count || m_cb_result;
  }


private:
  gcc_jit_code_callback m_code_factory;
  bool m_within_code_factory;
  void *m_user_data;

  int m_error_count;
  char m_first_error_str[1024];
  int m_cb_result; /* Result from client-provided code factory.  */

  /* Allocated using xmalloc (by xstrdup).  */
  char *m_path_template;

  /* This either aliases m_path_template, or is NULL.  */
  char *m_path_tempdir;

  /* The following are allocated using xmalloc.  */
  char *m_path_c_file;
  char *m_path_s_file;
  char *m_path_so_file;

  vec<function *> m_functions;
  const char *m_str_options[GCC_JIT_NUM_STR_OPTIONS];
  int m_int_options[GCC_JIT_NUM_INT_OPTIONS];
  bool m_bool_options[GCC_JIT_NUM_BOOL_OPTIONS];
  tree m_char_array_type_node;
  tree m_const_char_ptr;

  /* Source location handling.  */
  vec<source_file *> m_source_files;

  vec<std::pair<tree, location *> > m_cached_locations;
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

  enum gcc_jit_function_kind get_kind () const { return m_kind; }

  lvalue *
  new_local (location *loc,
	     type *type,
	     const char *name);

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
  add_comment (location *loc,
	       const char *text);

  void
  add_conditional (location *loc,
		   rvalue *boolval,
		   label *on_true,
		   label *on_false);

  label *
  add_label (location *loc,
	     const char *name);

  void
  place_forward_label (location *loc, label *lab);

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
  void add_stmt (tree stmt)
  {
    tsi_link_after (&m_stmt_iter, stmt, TSI_CONTINUE_LINKING);
  }

  void
  set_tree_location (tree t, location *loc)
  {
    m_ctxt->set_tree_location (t, loc);
  }

private:
  tree m_inner_fndecl;
  tree m_inner_bind_expr;
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
  rvalue (context *ctxt, tree inner)
    : m_ctxt (ctxt),
      m_inner (inner)
  {}

  rvalue *
  as_rvalue () { return this; }

  tree as_tree () const { return m_inner; }

  context *get_context () const { return m_ctxt; }

  type *
  get_type () { return new type (TREE_TYPE (m_inner)); }

  rvalue *
  access_field (location *loc,
		const char *fieldname);

  lvalue *
  dereference_field (location *loc,
		     const char *fieldname);

  lvalue *
  dereference (location *loc);

private:
  context *m_ctxt;
  tree m_inner;
};

class lvalue : public rvalue
{
public:
  lvalue (context *ctxt, tree inner)
    : rvalue(ctxt, inner)
  {}

  lvalue *
  as_lvalue () { return this; }

  lvalue *
  access_field (location *loc,
		const char *fieldname);

  rvalue *
  get_address (location *loc);

};

class param : public lvalue
{
public:
  param (context *ctxt, tree inner)
    : lvalue(ctxt, inner)
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

/* Dealing with the linemap API.

   It appears that libcpp requires locations to be created as if by
   a tokenizer, creating them by filename, in ascending order of
   line/column, whereas our API doesn't impose any such constraints:
   we allow client code to create locations in arbitrary orders.

   To square this circle, we need to cache all location creation,
   grouping things up by filename/line, and then creating the linemap
   entries in a post-processing phase.  */

/* A set of locations, all sharing a filename */
class source_file : public wrapper
{
public:
  source_file (tree filename);

  source_line *
  get_source_line (int line_num);

  tree filename_as_tree () const { return m_filename; }

  const char*
  get_filename () const { return IDENTIFIER_POINTER (m_filename); }

  vec<source_line *> m_source_lines;

private:
  tree m_filename;
};

/* A source line, with one or more locations of interest.  */
class source_line : public wrapper
{
public:
  source_line (source_file *file, int line_num);

  location *
  get_location (int column_num);

  int get_line_num () const { return m_line_num; }

  vec<location *> m_locations;

private:
  source_file *m_source_file;
  int m_line_num;
};

/* A specific location on a source line.  This is what we expose
   to the client API.  */
class location : public wrapper
{
public:
  location (source_line *line, int column_num);

  int get_column_num () const { return m_column_num; }

  source_location m_srcloc;

private:
  source_line *m_line;
  int m_column_num;
};

} // namespace gcc::jit

} // namespace gcc

