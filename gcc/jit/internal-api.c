#include "config.h"
#include "system.h"
#include "ansidecl.h"
#include "coretypes.h"
#include "opts.h"
#include "tree.h"
#include "debug.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "cgraph.h"
#include "toplev.h"
#include "tree-iterator.h"
#include "gimple.h"
#include "gimple-pretty-print.h"
#include "timevar.h"
#include "dumpfile.h"
#include "tree-dump.h"

#include <pthread.h>

#include "internal-api.h"

gcc::jit::context::
~context ()
{
  if (m_bool_options[GCC_JIT_BOOL_OPTION_KEEP_INTERMEDIATES])
      fprintf (stderr, "intermediate files written to %s\n", m_path_tempdir);
  else
    {
      /* Clean up .s/.so and tempdir. */
      if (m_path_s_file)
        unlink (m_path_s_file);
      if (m_path_so_file)
        unlink (m_path_so_file);
      if (m_path_tempdir)
        rmdir (m_path_tempdir);

      free (m_path_template);
      /* m_path_tempdir aliases m_path_template, or is NULL, so don't
         attempt to free it .  */
      free (m_path_c_file);
      free (m_path_s_file);
      free (m_path_so_file);
      m_functions.release ();
    }
}

void
gcc::jit::context::
gt_ggc_mx ()
{
  int i;
  function *func;
  FOR_EACH_VEC_ELT (m_functions, i, func)
    {
      if (ggc_test_and_set_mark (func))
	func->gt_ggc_mx ();
    }
}

void
gcc::jit::context::
set_code_factory (gcc_jit_code_callback cb,
		  void *user_data)
{
  m_code_factory = cb;
  m_user_data = user_data;
}

gcc::jit::type *
gcc::jit::context::
get_void_type ()
{
  return new type (void_type_node);
}

gcc::jit::type *
gcc::jit::context::
get_char_type ()
{
  return new type (char_type_node);
}

gcc::jit::type *
gcc::jit::context::
get_int_type ()
{
  return new type (intSI_type_node);
}

gcc::jit::type *
gcc::jit::context::
get_float_type ()
{
  return new type (float_type_node);
}

gcc::jit::type *
gcc::jit::context::
get_double_type ()
{
  return new type (double_type_node);
}

gcc::jit::field *
gcc::jit::context::
new_field (gcc::jit::location *loc,
	   gcc::jit::type *type,
	   const char *name)
{
  gcc_assert (NULL == loc);
  gcc_assert (type);
  gcc_assert (name);

  /* compare with c/c-decl.c:grokfield and grokdeclarator.  */
  tree decl = build_decl (UNKNOWN_LOCATION, FIELD_DECL,
			  get_identifier (name), type->as_tree ());

  return new field (decl);
}

gcc::jit::type *
gcc::jit::context::
new_struct_type (gcc::jit::location *loc,
		 const char *name,
		 int num_fields,
		 gcc::jit::field **fields)
{
  gcc_assert (NULL == loc);
  gcc_assert (name);
  gcc_assert ((num_fields == 0) || fields);

  /* Compare with c/c-decl.c: start_struct and finish_struct. */

  tree t = make_node (RECORD_TYPE);
  TYPE_NAME (t) = get_identifier (name);
  TYPE_SIZE (t) = 0;

 tree fieldlist = NULL;
  for (int i = 0; i < num_fields; i++)
    {
      field *f = fields[i];
      DECL_CONTEXT (f->as_tree ()) = t;
      fieldlist = chainon (f->as_tree (), fieldlist);
    }
  fieldlist = nreverse (fieldlist);
  TYPE_FIELDS (t) = fieldlist;

  layout_type (t);

  return new type (t);
}


gcc::jit::param *
gcc::jit::context::
new_param (location *loc,
	   type *type,
	   const char *name)
{
  gcc_assert (NULL == loc);
  gcc_assert (type);
  gcc_assert (name);
  tree inner = build_decl (UNKNOWN_LOCATION, PARM_DECL,
			   get_identifier (name), type->as_tree ());
  return new param (inner);
}

gcc::jit::function *
gcc::jit::context::
new_function (location *loc,
	      enum gcc_jit_function_kind kind,
	      type *return_type,
	      const char *name,
	      int num_params,
	      param **params,
	      int is_variadic)
 {
  gcc_assert (NULL == loc);
  //can return_type be NULL?
  gcc_assert (name);
  gcc_assert ((num_params == 0) || params);

  tree *arg_types = (tree *)xcalloc(num_params, sizeof(tree*));
  for (int i = 0; i < num_params; i++)
    {
      arg_types[i] = TREE_TYPE (params[i]->as_tree ());
    }
  tree fn_type;
  if (is_variadic)
    fn_type = build_varargs_function_type_array (return_type->as_tree (),
						 num_params, arg_types);
  else
    fn_type = build_function_type_array (return_type->as_tree (),
					 num_params, arg_types);
  free (arg_types);

  /* FIXME: this uses input_location: */
  tree fndecl = build_fn_decl (name, fn_type);

  tree resdecl = build_decl (UNKNOWN_LOCATION, RESULT_DECL,
			     NULL_TREE, return_type->as_tree ());
  DECL_ARTIFICIAL (resdecl) = 1;
  DECL_IGNORED_P (resdecl) = 1;
  DECL_RESULT (fndecl) = resdecl;

  if (kind != GCC_JIT_FUNCTION_IMPORTED)
    {
      tree param_decl_list = NULL;
      for (int i = 0; i < num_params; i++)
	{
	  param_decl_list = chainon (params[i]->as_tree (), param_decl_list);
	}

      /* The param list was created in reverse order; fix it: */
      param_decl_list = nreverse (param_decl_list);

      tree t;
      for (t = param_decl_list; t; t = DECL_CHAIN (t))
	{
	  DECL_CONTEXT (t) = fndecl;
	  DECL_ARG_TYPE (t) = TREE_TYPE (t);
	}

      /* Set it up on DECL_ARGUMENTS */
      DECL_ARGUMENTS(fndecl) = param_decl_list;
    }

  function *func = new function (this, fndecl, kind);
  m_functions.safe_push (func);
  return func;
}

gcc::jit::lvalue *
gcc::jit::context::
new_global (location *loc,
            type *type,
            const char *name)
{
  gcc_assert (NULL == loc);
  gcc_assert (type);
  gcc_assert (name);
  tree inner = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			   get_identifier (name),
			   type->as_tree ());
  TREE_PUBLIC (inner) = 1;
  DECL_COMMON (inner) = 1;
  DECL_EXTERNAL (inner) = 1;

  return new lvalue (inner);
}

gcc::jit::local *
gcc::jit::context::
new_local (location *loc,
	   type *type,
	   const char *name)
{
  gcc_assert (NULL == loc);
  gcc_assert (type);
  gcc_assert (name);
  tree inner = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			   get_identifier (name),
			   type->as_tree ());
  return new local (inner);
}


gcc::jit::rvalue *
gcc::jit::context::
new_rvalue_from_int (type *type,
		     int value)
{
  // FIXME: type-checking, or coercion?
  tree inner_type = type->as_tree ();
  if (INTEGRAL_TYPE_P (inner_type))
    {
      tree inner = build_int_cst (inner_type, value);
      return new rvalue (inner);
    }
  else
    {
      REAL_VALUE_TYPE real_value;
      real_from_integer (&real_value, VOIDmode, value, 0, 0);
      tree inner = build_real (inner_type, real_value);
      return new rvalue (inner);
    }
}

gcc::jit::rvalue *
gcc::jit::context::
new_string_literal (const char *value)
{
  tree t_str = build_string (strlen (value), value);
  gcc_assert (m_char_array_type_node);
  TREE_TYPE (t_str) = m_char_array_type_node;

  /* Convert to (const char*), loosely based on
     c/c-typeck.c: array_to_pointer_conversion,
     by taking address of start of string.  */
  tree t_addr = build1 (ADDR_EXPR, m_const_char_ptr, t_str);

  return new rvalue (t_addr);
}

gcc::jit::rvalue *
gcc::jit::context::
new_binary_op (location *loc,
	       enum gcc_jit_binary_op op,
	       type *result_type,
	       rvalue *a, rvalue *b)
{
  // FIXME: type-checking, or coercion?
  enum tree_code inner_op;

  gcc_assert (NULL == loc);
  gcc_assert (result_type);
  gcc_assert (a);
  gcc_assert (b);

  switch (op)
    {
    default: gcc_unreachable ();
    case GCC_JIT_BINARY_OP_PLUS:
      inner_op = PLUS_EXPR;
      break;

    case GCC_JIT_BINARY_OP_MINUS:
      inner_op = MINUS_EXPR;
      break;

    case GCC_JIT_BINARY_OP_MULT:
      inner_op = MULT_EXPR;
      break;
    }

  tree inner_expr = build2 (inner_op,
			    result_type->as_tree (),
			    a->as_tree (),
			    b->as_tree ());
  return new rvalue (inner_expr);
}

gcc::jit::rvalue *
gcc::jit::context::
new_comparison (location *loc,
		enum gcc_jit_comparison op,
		rvalue *a, rvalue *b)
{
  // FIXME: type-checking, or coercion?
  enum tree_code inner_op;

  gcc_assert (NULL == loc);
  gcc_assert (a);
  gcc_assert (b);

  switch (op)
    {
    default: gcc_unreachable ();
    case GCC_JIT_COMPARISON_LT:
      inner_op = LT_EXPR;
      break;
    case GCC_JIT_COMPARISON_GE:
      inner_op = GE_EXPR;
      break;
    }

  tree inner_expr = build2 (inner_op,
			    boolean_type_node,
			    a->as_tree (),
			    b->as_tree ());
  return new rvalue (inner_expr);
}

gcc::jit::rvalue *
gcc::jit::context::
new_call (location *loc,
	  function *func,
	  int numargs , rvalue **args)
{
  tree fndecl;
  vec<tree, va_gc> *tree_args;

  gcc_assert (NULL == loc);
  gcc_assert (func);
  gcc_assert (numargs >= 0);
  gcc_assert ((args == 0) || (args != NULL));

  // FIXME: type checking
  // FIXME: check num args and types

  fndecl = func->as_fndecl ();

  vec_alloc (tree_args, numargs);
  for (int i = 0; i < numargs; i++)
    tree_args->quick_push (args[i]->as_tree ());

  tree fntype = TREE_TYPE (fndecl);
  tree fn = build1 (ADDR_EXPR, build_pointer_type (fntype), fndecl);

  return new rvalue (build_call_vec (func->get_return_type_as_tree (),
				     fn, tree_args));

  /* see c-typeck.c: build_function_call
     which calls build_function_call_vec

     which does lots of checking, then:
    result = build_call_array_loc (loc, TREE_TYPE (fntype),
				   function, nargs, argarray);
    which is in tree.c
    (see also build_call_vec)
   */
}

gcc::jit::rvalue *
gcc::jit::context::
new_array_lookup (location *loc,
		  rvalue *ptr,
		  rvalue *index)
{
  gcc_assert (NULL == loc);
  gcc_assert (ptr);
  gcc_assert (index);

  /* For comparison, see:
       c/c-typeck.c: build_array_ref
       c-family/c-common.c: pointer_int_sum
  */
  tree t_ptr = ptr->as_tree ();
  tree t_index = index->as_tree ();
  tree t_type_ptr = TREE_TYPE (t_ptr);
  tree t_type_star_ptr = TREE_TYPE (t_type_ptr);

  if (TREE_CODE (t_type_ptr) == ARRAY_TYPE)
    {
      tree t_result = build4 (ARRAY_REF, t_type_star_ptr, t_ptr, t_index,
			      NULL_TREE, NULL_TREE);
      return new rvalue (t_result);
    }
  else
    {
      /* Convert index to an offset in bytes.  */
      tree t_sizeof = size_in_bytes (t_type_star_ptr);
      t_index = fold_build1 (CONVERT_EXPR, sizetype, t_index);
      tree t_offset = build2 (MULT_EXPR, sizetype, t_index, t_sizeof);

      /* Locate (ptr + offset).  */
      tree t_address = build2 (POINTER_PLUS_EXPR, t_type_ptr, t_ptr, t_offset);

      tree t_indirection = build1 (INDIRECT_REF, t_type_star_ptr, t_address);
      return new rvalue (t_indirection);
    }
}

static tree
get_field (tree type, tree component)
{
  for (tree field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
    if (DECL_NAME (field) == component)
      return field;
  return NULL;
}

gcc::jit::lvalue *
gcc::jit::context::
new_field_access (location *loc,
		  rvalue *ptr_or_struct,
		  const char *fieldname)
{
  gcc_assert (NULL == loc);
  gcc_assert (ptr_or_struct);
  gcc_assert (fieldname);

  /* Compare with c/ctypeck.c:lookup_field, build_indirect_ref, and
     build_component_ref. */
  tree datum = ptr_or_struct->as_tree ();
  tree type = TREE_TYPE (datum);

  if (TREE_CODE (type) == POINTER_TYPE)
    {
      datum = build1 (INDIRECT_REF, type, datum);
      type = TREE_TYPE (type);
    }

  tree component = get_identifier (fieldname);
  tree field = get_field (type, component);
  if (!field)
    {
      // FIXME: field not found
      return NULL;
    }
  tree ref = build3 (COMPONENT_REF, TREE_TYPE (field), datum,
		     field, NULL_TREE);
  return new lvalue (ref);
}

void *
gcc::jit::wrapper::
operator new (size_t sz)
{
  return ggc_internal_cleared_alloc_stat (sz MEM_STAT_INFO);
}

gcc::jit::function::
function(gcc::jit::context *ctxt, tree fndecl, enum gcc_jit_function_kind kind)
: m_ctxt(ctxt),
  m_inner_fndecl (fndecl),
  m_kind (kind)
{
  if (m_kind != GCC_JIT_FUNCTION_IMPORTED)
    {
      m_stmt_list = alloc_stmt_list ();
      m_stmt_iter = tsi_start (m_stmt_list);
    }
  else
    {
      m_stmt_list = NULL;
    }
}

void
gcc::jit::function::
gt_ggc_mx ()
{
  gt_ggc_m_9tree_node (m_inner_fndecl);
  gt_ggc_m_9tree_node (m_stmt_list);
}

tree
gcc::jit::function::
get_return_type_as_tree () const
{
  return TREE_TYPE (TREE_TYPE(m_inner_fndecl));
}

gcc::jit::label *
gcc::jit::function::
new_forward_label (const char *name)
{
  gcc_assert (m_kind != GCC_JIT_FUNCTION_IMPORTED);
  return new label (this, name);
}

void
gcc::jit::function::
postprocess ()
{
  if (m_ctxt->get_bool_option (GCC_JIT_BOOL_OPTION_DUMP_INITIAL_TREE))
    debug_tree (m_stmt_list);

  /* Do we need this to force cgraphunit.c to output the function? */
  if (m_kind == GCC_JIT_FUNCTION_EXPORTED)
    {
      DECL_EXTERNAL (m_inner_fndecl) = 0;
      DECL_PRESERVE_P (m_inner_fndecl) = 1;
    }

  if (m_kind != GCC_JIT_FUNCTION_IMPORTED)
    {
      /* Seem to need this in gimple-low.c: */
      DECL_INITIAL (m_inner_fndecl) = make_node (BLOCK);

      /* how to add to function? the following appears to be how to
	 set the body of a m_inner_fndecl: */
      DECL_SAVED_TREE(m_inner_fndecl) = m_stmt_list;
      //debug_tree (m_inner_fndecl);

      /* Convert to gimple: */
      //printf("about to gimplify_function_tree\n");
      gimplify_function_tree (m_inner_fndecl);
      //printf("finished gimplify_function_tree\n");

      current_function_decl = m_inner_fndecl;
      if (m_ctxt->get_bool_option (GCC_JIT_BOOL_OPTION_DUMP_INITIAL_GIMPLE))
	dump_function_to_file (m_inner_fndecl, stderr, TDF_VOPS|TDF_MEMSYMS);
      //debug_tree (m_inner_fndecl);

      //printf("about to add to cgraph\n");
      /* Add to cgraph: */
      cgraph_finalize_function (m_inner_fndecl, false);
      /* This can trigger a collection, so we need to have all of
	 the funcs as roots.  */

      current_function_decl = NULL;
    }
}

void
gcc::jit::function::
add_eval (location *loc,
	  rvalue *rvalue)
{
  gcc_assert (NULL == loc);
  gcc_assert (rvalue);
  gcc_assert (m_kind != GCC_JIT_FUNCTION_IMPORTED);

  tsi_link_after (&m_stmt_iter, rvalue->as_tree (), TSI_CONTINUE_LINKING);
}

void
gcc::jit::function::
add_assignment (location *loc,
		lvalue *lvalue,
		rvalue *rvalue)
{
  gcc_assert (NULL == loc);
  gcc_assert (lvalue);
  gcc_assert (rvalue);
  gcc_assert (m_kind != GCC_JIT_FUNCTION_IMPORTED);

  tree t_lvalue = lvalue->as_tree ();
  tree t_rvalue = rvalue->as_tree ();
  if (TREE_TYPE (t_rvalue) != TREE_TYPE (t_lvalue))
    t_rvalue = build1 (CONVERT_EXPR,
		       TREE_TYPE (t_lvalue),
		       t_rvalue);

  tree stmt =
    build2 (MODIFY_EXPR, TREE_TYPE (t_lvalue),
	    t_lvalue, t_rvalue);
  tsi_link_after (&m_stmt_iter, stmt, TSI_CONTINUE_LINKING);
}

void
gcc::jit::function::
add_conditional (location *loc,
		 rvalue *boolval,
		 label *on_true,
		 label *on_false)
{
  gcc_assert (NULL == loc);
  gcc_assert (boolval);
  gcc_assert (on_true);
  /* on_false can be NULL */
  gcc_assert (m_kind != GCC_JIT_FUNCTION_IMPORTED);

  /* COND_EXPR wants statement lists for the true/false operands, but we
     want labels.
     Shim it by creating jumps to the labels */
  tree true_jump = build1 (GOTO_EXPR, void_type_node,
			   on_true->as_label_decl ());
  tree false_jump;
  if (on_false)
    false_jump = build1 (GOTO_EXPR, void_type_node,
			 on_false->as_label_decl ());
  else
    false_jump = NULL;

  tree stmt =
    build3 (COND_EXPR, void_type_node, boolval->as_tree (),
	    true_jump, false_jump);
  tsi_link_after (&m_stmt_iter, stmt, TSI_CONTINUE_LINKING);
}

gcc::jit::label *
gcc::jit::function::
add_label (location *loc,
	   const char *name)
{
  gcc_assert (NULL == loc);
  gcc_assert (m_kind != GCC_JIT_FUNCTION_IMPORTED);

  label *lab = new label (this, name);
  place_forward_label (lab);
  return lab;
}

void
gcc::jit::function::
place_forward_label (label *lab)
{
  gcc_assert (lab);
  gcc_assert (NULL == lab->m_label_expr); // must not have already been placed
  gcc_assert (m_kind != GCC_JIT_FUNCTION_IMPORTED);

  lab->m_label_expr = build1 (LABEL_EXPR,
			     void_type_node,
			     lab->as_label_decl ());
  tsi_link_after (&m_stmt_iter, lab->m_label_expr, TSI_CONTINUE_LINKING);
}

void
gcc::jit::function::
add_jump (location *loc,
	  label *target)
{
  gcc_assert (NULL == loc);
  gcc_assert (target);
  gcc_assert (m_kind != GCC_JIT_FUNCTION_IMPORTED);

  // see c_finish_loop
  //tree top = build1 (LABEL_EXPR, void_type_node, NULL_TREE);
  //add_stmt (top);

  //tree stmt = build_and_jump (&LABEL_EXPR_LABEL (target->label_));
  TREE_USED (target->as_label_decl ()) = 1;
  tree stmt = build1 (GOTO_EXPR, void_type_node, target->as_label_decl ());
  tsi_link_after (&m_stmt_iter, stmt, TSI_CONTINUE_LINKING);

  /*
  from c-typeck.c:
tree
c_finish_goto_label (location_t loc, tree label)
{
  tree decl = lookup_label_for_goto (loc, label);
  if (!decl)
    return NULL_TREE;
  TREE_USED (decl) = 1;
  {
    tree t = build1 (GOTO_EXPR, void_type_node, decl);
    SET_EXPR_LOCATION (t, loc);
    return add_stmt (t);
  }
}
  */

}

void
gcc::jit::function::
add_return (location *loc,
	    rvalue *rvalue)
{
  gcc_assert (NULL == loc);
  gcc_assert (m_kind != GCC_JIT_FUNCTION_IMPORTED);

  tree return_type = TREE_TYPE (TREE_TYPE (m_inner_fndecl));
  tree t_lvalue = DECL_RESULT (m_inner_fndecl);
  tree t_rvalue = rvalue->as_tree ();
  if (TREE_TYPE (t_rvalue) != TREE_TYPE (t_lvalue))
    t_rvalue = build1 (CONVERT_EXPR,
		       TREE_TYPE (t_lvalue),
		       t_rvalue);
  tree modify_retval = build2 (MODIFY_EXPR, return_type,
			       t_lvalue, t_rvalue);
  tree return_stmt = build1 (RETURN_EXPR, return_type,
			     modify_retval);
  tsi_link_after (&m_stmt_iter, return_stmt, TSI_CONTINUE_LINKING);
}

gcc::jit::loop *
gcc::jit::function::
new_loop (location *loc,
	  rvalue *boolval)
{
  return new loop (this, loc, boolval);
}

gcc::jit::label::
label (function *func,
       const char *name)
{
  tree identifier;

  gcc_assert (func);
  // name can be NULL
  if (name)
    identifier = get_identifier (name);
  else
    identifier = NULL;
  m_label_decl = build_decl (UNKNOWN_LOCATION, LABEL_DECL,
			    identifier, void_type_node);
  DECL_CONTEXT (m_label_decl) = func->as_fndecl ();
  m_label_expr = NULL;
}


gcc::jit::loop::
loop (function *func, location *loc, rvalue *boolval) :
  m_func(func)
{
  m_label_cond = func->add_label (loc, "loop_cond");
  m_label_body = func->new_forward_label ("loop_body");
  m_label_end = func->new_forward_label ("loop_end");
  func->add_conditional (loc, boolval, m_label_body, m_label_end);
  func->place_forward_label (m_label_body);
}

void
gcc::jit::loop::
end (location *loc)
{
  m_func->add_jump (loc, m_label_cond);
  m_func->place_forward_label (m_label_end);
}

void
gcc::jit::context::
set_str_option (enum gcc_jit_str_option opt,
		const char *value)
{
  if (opt < 0 || opt >= GCC_JIT_NUM_STR_OPTIONS)
    {
      add_error ("unrecognized str option");
      return;
    }
  m_str_options[opt] = value;
}

void
gcc::jit::context::
set_int_option (enum gcc_jit_int_option opt,
		int value)
{
  if (opt < 0 || opt >= GCC_JIT_NUM_INT_OPTIONS)
    {
      add_error ("unrecognized int option");
      return;
    }
  m_int_options[opt] = value;
}

void
gcc::jit::context::
set_bool_option (enum gcc_jit_bool_option opt,
		 int value)
{
  if (opt < 0 || opt >= GCC_JIT_NUM_BOOL_OPTIONS)
    {
      add_error ("unrecognized bool option");
      return;
    }
  m_bool_options[opt] = value ? true : false;
}

namespace gcc
{
  namespace jit
  {
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    context *active_jit_ctxt;
  }
}

gcc::jit::result *
gcc::jit::context::
compile ()
{
  void *handle = NULL;
  result *result_obj = NULL;
  const char *progname;
  const char *fake_args[20];
  unsigned int num_args;

  /* Acquire the big GCC mutex. */
  pthread_mutex_lock (&mutex);

  gcc_assert (NULL == active_jit_ctxt);
  active_jit_ctxt = this;

  m_path_template = xstrdup ("/tmp/libgccjit-XXXXXX");
  if (!m_path_template)
    goto error;

  /* Create tempdir using mkdtemp.  This is created with 0700 perms and
     is unique.  Hence no other (non-root) users should have access to
     the paths within it.  */
  m_path_tempdir = mkdtemp (m_path_template);
  if (!m_path_tempdir)
    goto error;
  m_path_c_file = concat (m_path_tempdir, "/fake.c", NULL);
  m_path_s_file = concat (m_path_tempdir, "/fake.s", NULL);
  m_path_so_file = concat (m_path_tempdir, "/fake.so", NULL);

  /* Call into the rest of gcc.
     For now, we have to assemble command-line options to pass into
     toplev_main, so that they can be parsed. */

  /* Pass in user-provided "progname", if any, so that it makes it
     into GCC's "progname" global, used in various diagnostics. */
  progname = m_str_options[GCC_JIT_STR_OPTION_PROGNAME];
  fake_args[0] = progname ? progname : "libgccjit.so";

  fake_args[1] = m_path_c_file;
  num_args = 2;

#define ADD_ARG(arg) \
  do \
    { \
      gcc_assert(num_args < sizeof(fake_args)/sizeof(char*)); \
      fake_args[num_args++] = arg; \
    } \
  while (0)

  ADD_ARG ("-fPIC");

  /* Handle int options: */
  switch (m_int_options[GCC_JIT_INT_OPTION_OPTIMIZATION_LEVEL])
    {
    default:
      add_error ("unrecognized optimization level");
      goto error;

    case 0:
      ADD_ARG ("-O0");
      break;

    case 1:
      ADD_ARG ("-O1");
      break;

    case 2:
      ADD_ARG ("-O2");
      break;

    case 3:
      ADD_ARG ("-O3");
      break;
    }
  /* What about -Os? */

  /* Handle bool options: */
  if (m_bool_options[GCC_JIT_BOOL_OPTION_DEBUGINFO])
    ADD_ARG ("-g");

  /* Suppress timing (and other) info.  */
  if (1)
    {
      ADD_ARG ("-quiet");
      quiet_flag = 1;
    }

  /* Aggressively garbage-collect, to shake out bugs: */
  if (m_bool_options[GCC_JIT_BOOL_OPTION_SELFCHECK_GC])
    {
      ADD_ARG ("--param");
      ADD_ARG ("ggc-min-expand=0");
      ADD_ARG ("--param");
      ADD_ARG ("ggc-min-heapsize=0");
    }

  if (m_bool_options[GCC_JIT_BOOL_OPTION_DUMP_EVERYTHING])
    {
      ADD_ARG ("-fdump-tree-all");
      ADD_ARG ("-fdump-rtl-all");
      ADD_ARG ("-fdump-ipa-all");
    }

  toplev_options toplev_opts;
  toplev_opts.use_TV_TOTAL = false;

  if (time_report || !quiet_flag  || flag_detailed_statistics)
    timevar_init ();

  timevar_start (TV_TOTAL);

  toplev_main (num_args, const_cast <char **> (fake_args), &toplev_opts);
  toplev_finalize ();

  active_jit_ctxt = NULL;

  if (m_cb_result)
    goto error;

  timevar_push (TV_ASSEMBLE);

  /* Gross hacks follow:
     We have a .s file; we want a .so file.
     We could reuse parts of gcc/gcc.c to do this.
     For now, just use the /usr/bin/gcc on the system...
   */
  /* FIXME: totally faking it for now, not even using pex */
  {
    char cmd[1024];
    snprintf (cmd, 1024, "gcc -shared %s -o %s",
              m_path_s_file, m_path_so_file);
    if (0)
      printf ("cmd: %s\n", cmd);
    int ret = system (cmd);
    if (ret)
      goto error;
  }
  timevar_pop (TV_ASSEMBLE);

  // TODO: split out assembles vs linker

  /* dlopen the .so file. */
  {
    const char *error;

    timevar_push (TV_LOAD);

    /* Clear any existing error.  */
    dlerror ();

    handle = dlopen (m_path_so_file, RTLD_NOW | RTLD_LOCAL);
    if ((error = dlerror()) != NULL)  {
      fprintf(stderr, "%s\n", error);
    }
    if (handle)
      result_obj = new result (handle);
    else
      result_obj = NULL;

    timevar_pop (TV_LOAD);
  }

  timevar_stop (TV_TOTAL);
  timevar_print (stderr);

 error:
  /* Release the big GCC mutex. */
  active_jit_ctxt = NULL;
  pthread_mutex_unlock (&mutex);

  return result_obj;
}

void
gcc::jit::context::
invoke_code_factory ()
{
  /* Adapted from c-common.c:c_common_nodes_and_builtins.  */
  tree array_domain_type = build_index_type (size_int (200));
  m_char_array_type_node
    = build_array_type (char_type_node, array_domain_type);

  m_const_char_ptr
    = build_pointer_type (build_qualified_type (char_type_node,
						TYPE_QUAL_CONST));

  /* Call the client-provided code factory:  */
  timevar_push (TV_CLIENT_CALLBACK);
  m_within_code_factory = true;
  m_cb_result = m_code_factory ((gcc_jit_context *)this, m_user_data);
  m_within_code_factory = false;
  timevar_pop (TV_CLIENT_CALLBACK);

  if (!m_cb_result)
    {
      int i;
      function *func;
      FOR_EACH_VEC_ELT (m_functions, i, func)
	{
	  gcc_assert (func);
	  func->postprocess ();
	}
    }
}

void
gcc::jit::context::
add_error (const char */*msg*/)
{
  // TODO
}

gcc::jit::result::
result(void *dso_handle)
  : m_dso_handle(dso_handle)
{
}

gcc::jit::result::
~result()
{
  dlclose (m_dso_handle);
}

void *
gcc::jit::result::
get_code (const char *funcname)
{
  void *code;
  const char *error;

  /* Clear any existing error.  */
  dlerror ();

  code = dlsym (m_dso_handle, funcname);

  if ((error = dlerror()) != NULL)  {
    fprintf(stderr, "%s\n", error);
  }

  return code;
}
