/* Mudflap: narrow-pointer bounds-checking by tree rewriting.
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
   Contributed by Frank Ch. Eigler <fche@redhat.com>
   and Graydon Hoare <graydon@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */


#include "config.h"
#include "errors.h"
#include "system.h"
#include "tree.h"
#include "tree-inline.h"
#include "c-tree.h"
#include "c-common.h"
#include "tree-simple.h"
#include "diagnostic.h"
#include "hashtab.h"
#include "output.h"
#include "varray.h"
#include "langhooks.h"
#include "tree-mudflap.h"
#include "ggc.h"
#include "target.h"
#include "flags.h"
#include "rtl.h"
#include "toplev.h"
#include "function.h"


/* {{{ Internal function decls */

static void mf_xform_derefs PARAMS ((tree));
static void mf_xform_decls PARAMS ((tree, tree));
static void mf_init_extern_trees PARAMS ((void));
static void mf_decl_extern_trees PARAMS ((void));
static tree mf_find_addrof PARAMS ((tree, tree));
static tree mf_varname_tree PARAMS ((tree));
static tree mf_file_function_line_tree PARAMS ((const char *, int));
static void mf_enqueue_register_call PARAMS ((const char*, tree, tree, tree));
static void mf_flush_enqueued_calls PARAMS ((void));
static tree mx_external_ref PARAMS ((tree, int));
static tree mx_flag PARAMS ((tree));
static tree mx_xfn_indirect_ref PARAMS ((tree *, int *, void *));
static tree mx_xfn_xform_decls PARAMS ((tree *, int *, void *));
static tree mx_xfn_find_addrof PARAMS ((tree *, int *, void *));

static tree mf_offset_expr_of_array_ref PARAMS ((tree, tree, tree *));
static tree mf_build_check_statement_for PARAMS ((tree, tree*, const char *, int));
static void mx_register_decl PARAMS ((tree *, tree, tree));


/* These macros are used to mark tree nodes, so that they are not
   repeatedly transformed.  The `bounded' flag is not otherwise used.  */

#define MARK_TREE_MUDFLAPPED(tree)  do { TREE_BOUNDED (tree) = 1; } while (0)
#define TREE_MUDFLAPPED_P(tree)  TREE_BOUNDED (tree)

/* }}} */
/* {{{ extern mudflap functions */

/* Perform the mudflap tree transforms on the given function.  */
 
void 
mudflap_c_function (t)
     tree t;
{
  tree fnbody = DECL_SAVED_TREE (t);
  tree fnparams = DECL_ARGUMENTS (t);

  if (getenv ("UNPARSE"))  /* XXX */
    {
      print_c_tree (stderr, DECL_RESULT (t));
      fprintf (stderr, " ");
      print_c_tree (stderr, DECL_NAME (t));
      fprintf (stderr, " (");
      print_c_tree (stderr, DECL_ARGUMENTS (t));
      fprintf (stderr, " )\n");
      print_c_tree (stderr, DECL_SAVED_TREE (t));
    }

  mf_init_extern_trees ();

  pushlevel (0);

  mf_decl_extern_trees ();
  mf_xform_decls (fnbody, fnparams);
  mf_xform_derefs (fnbody);

  poplevel (1, 1, 0);

  if (getenv ("UNPARSE"))  /* XXX */
    {
      fprintf (stderr, "/* after -fmudflap: */\n");
      print_c_tree (stderr, DECL_SAVED_TREE (t));
    }
}


/* ------------------------------------------------------------------------ */


/* Remember given node as a static of some kind: global data,
   function-scope static, or an anonymous constant.  Its assembler
   label is given.
*/


/* A list of globals whose incomplete declarations we encountered.
   Instead of emitting the __mf_register call for them here, it's
   delayed until program finish time.  If they're still incomplete by
   then, warnings are emitted.  */

static varray_type GTY ((param_is(union tree_node))) deferred_static_decls;
static varray_type GTY ((skip(""))) deferred_static_decl_labels;
static int deferred_static_decls_init;

/* What I really want is a std::map<union tree_node,std::string> .. :-(  */


void 
mudflap_enqueue_decl (obj, label)
     tree obj;
     const char *label;
{
  if (TREE_MUDFLAPPED_P (obj))
    return;

  /*
  fprintf (stderr, "enqueue_decl obj=`");
  print_c_tree (stderr, obj);
  fprintf (stderr, "' label=`%s'\n", label);
  */

  if (COMPLETE_OR_VOID_TYPE_P (TREE_TYPE (obj)))
    {
      mf_enqueue_register_call (label,
				c_size_in_bytes (TREE_TYPE (obj)),
				build_int_2 (3, 0), /* __MF_TYPE_STATIC */
				mf_varname_tree (obj));
    }
  else
    {
      unsigned i;
      int found_p;
      
      if (! deferred_static_decls_init)
	{
	  deferred_static_decls_init = 1;
	  VARRAY_TREE_INIT (deferred_static_decls, 10, "deferred static list");
	  VARRAY_CHAR_PTR_INIT (deferred_static_decl_labels, 10, "label list");
	}
      
      /* Ugh, linear search... */
      found_p = 0;
      for (i=0; i < VARRAY_SIZE (deferred_static_decls); i++)
	if (VARRAY_TREE (deferred_static_decls, i) == obj)
	  found_p = 1;
      
      if (found_p)
	warning_with_decl (obj, "mudflap cannot track lifetime of `%s'", 
			   IDENTIFIER_POINTER (DECL_NAME (obj)));
      else
	{
	  VARRAY_PUSH_TREE (deferred_static_decls, obj);
	  VARRAY_PUSH_CHAR_PTR (deferred_static_decl_labels, label);
	}
    }
}


void 
mudflap_enqueue_constant (obj, label)
     tree obj;
     const char *label;
{
  if (TREE_MUDFLAPPED_P (obj))
    return;

  if (TREE_CODE (obj) == STRING_CST)
    {
      mf_enqueue_register_call (label,
				build_int_2 (TREE_STRING_LENGTH (obj), 0),
				build_int_2 (3, 0), /* __MF_TYPE_STATIC */
				mx_flag (fix_string_type
					 (build_string (15, "string literal"))));
    }
  else
    {
      mf_enqueue_register_call (label,
				c_size_in_bytes (TREE_TYPE (obj)),
				build_int_2 (3, 0), /* __MF_TYPE_STATIC */
				mx_flag (fix_string_type
					 (build_string (9, "constant"))));
    }
}



/* Emit any file-wide instrumentation.  */
void 
mudflap_finish_file ()
{
  /* Try to give the deferred objects one final try.  */
  if (deferred_static_decls_init)
    {
      unsigned i;

      for (i = 0; i < VARRAY_ACTIVE_SIZE (deferred_static_decls); i++)
	{
	  tree obj = VARRAY_TREE (deferred_static_decls, i);
	  const char *label = VARRAY_CHAR_PTR (deferred_static_decl_labels, i);

	  /* Call enqueue_decl again on the same object it has previously
	     put into the table.  (It won't modify the table this time, so
	     infinite iteration is not a problem.)  */
	  mudflap_enqueue_decl (obj, label);
	}

      VARRAY_CLEAR (deferred_static_decls);
      VARRAY_CLEAR (deferred_static_decl_labels);
    }
	     
  mf_flush_enqueued_calls ();
}

/* }}} */
/* {{{ global tree nodes */

/* Global tree objects for global variables and functions exported by
   mudflap runtime library.  mf_init_extern_trees must be called
   before using these.  */

static GTY (()) tree mf_uintptr_type;      /* uintptr_t (usually "unsigned long") */
static GTY (()) tree mf_cache_struct_type; /* struct __mf_cache { uintptr_t low; uintptr_t high; }; */
static GTY (()) tree mf_cache_structptr_type; /* struct __mf_cache * const */
static GTY (()) tree mf_cache_array_decl;  /* extern struct __mf_cache __mf_lookup_cache []; */
static GTY (()) tree mf_cache_shift_decl;  /* extern const unsigned char __mf_lc_shift; */
static GTY (()) tree mf_cache_mask_decl;   /* extern const uintptr_t __mf_lc_mask; */
static GTY (()) tree mf_check_fndecl;      /* extern void __mf_check (void *ptr, size_t sz, const char *); */
static GTY (()) tree mf_register_fndecl;   /* extern void __mf_register (void *ptr, size_t sz, int type, const char *); */
static GTY (()) tree mf_unregister_fndecl; /* extern void __mf_unregister (void *ptr, size_t sz); */



/* Initialize the global tree nodes that correspond to mf-runtime.h
   declarations.  */
static void
mf_init_extern_trees ()
{
  static int done = 0;
  if (done) return;

  mf_uintptr_type = long_unsigned_type_node;

  {
    tree field1, field2;

    mf_cache_struct_type = make_node (RECORD_TYPE);

    field1 = build_decl (FIELD_DECL,
			 get_identifier ("low"),
			 mf_uintptr_type);
    DECL_CONTEXT (field1) = mf_cache_struct_type;
    field2 = build_decl (FIELD_DECL,
			 get_identifier ("high"),
			 mf_uintptr_type);
    DECL_CONTEXT (field2) = mf_cache_struct_type;

    TREE_CHAIN (field1) = field2;
    TYPE_FIELDS (mf_cache_struct_type) = field1;
    TYPE_NAME (mf_cache_struct_type) = get_identifier ("__mf_cache");

    layout_type (mf_cache_struct_type);
  }

  mf_cache_structptr_type = build_qualified_type (build_pointer_type (mf_cache_struct_type),
						  TYPE_QUAL_CONST);

  mf_cache_array_decl = build_decl (VAR_DECL,
				    get_identifier ("__mf_lookup_cache"),
				    build_array_type (mf_cache_struct_type, NULL_TREE)); /* [] */
  DECL_EXTERNAL (mf_cache_array_decl) = 1;
  DECL_ARTIFICIAL (mf_cache_array_decl) = 1;
  TREE_PUBLIC (mf_cache_array_decl) = 1;
  mx_flag (mf_cache_array_decl);

  mf_cache_shift_decl = build_decl (VAR_DECL,
				    get_identifier ("__mf_lc_shift"),
				    unsigned_char_type_node);
  DECL_EXTERNAL (mf_cache_shift_decl) = 1;
  DECL_ARTIFICIAL (mf_cache_shift_decl) = 1;
  TREE_PUBLIC (mf_cache_shift_decl) = 1;
  mx_flag (mf_cache_shift_decl);

  mf_cache_mask_decl = build_decl (VAR_DECL,
				    get_identifier ("__mf_lc_mask"),
				    mf_uintptr_type);
  DECL_EXTERNAL (mf_cache_mask_decl) = 1;
  DECL_ARTIFICIAL (mf_cache_mask_decl) = 1;
  TREE_PUBLIC (mf_cache_mask_decl) = 1;
  mx_flag (mf_cache_mask_decl);

  mf_check_fndecl = build_decl (FUNCTION_DECL, get_identifier ("__mf_check"),
				build_function_type 
				(void_type_node,
				 tree_cons (NULL_TREE,
					    mf_uintptr_type,
					    tree_cons (NULL_TREE, 
						       mf_uintptr_type,
						       tree_cons (NULL_TREE,
								  const_string_type_node,
								  NULL_TREE)))));
  DECL_EXTERNAL (mf_check_fndecl) = 1;
  DECL_ARTIFICIAL (mf_check_fndecl) = 1;
  TREE_PUBLIC (mf_check_fndecl) = 1;

  mf_register_fndecl = build_decl (FUNCTION_DECL, get_identifier ("__mf_register"),
				   build_function_type
				   (void_type_node,
				    tree_cons (NULL_TREE,
					       mf_uintptr_type,
					       tree_cons (NULL_TREE, 
							  mf_uintptr_type,
							  tree_cons (NULL_TREE,
								     integer_type_node,
								     tree_cons (NULL_TREE,
										const_string_type_node,
										NULL_TREE))))));
  
  DECL_EXTERNAL (mf_register_fndecl) = 1;
  DECL_ARTIFICIAL (mf_register_fndecl) = 1;
  TREE_PUBLIC (mf_register_fndecl) = 1;

  mf_unregister_fndecl = build_decl (FUNCTION_DECL, get_identifier ("__mf_unregister"),
				   build_function_type 
				     (void_type_node,
				      tree_cons (NULL_TREE,
						 mf_uintptr_type,
						 tree_cons (NULL_TREE, 
							    mf_uintptr_type,
							    NULL_TREE))));
  DECL_EXTERNAL (mf_unregister_fndecl) = 1;
  DECL_ARTIFICIAL (mf_unregister_fndecl) = 1;
  TREE_PUBLIC (mf_unregister_fndecl) = 1;

  done = 1;
}


static void
mf_decl_extern_trees ()
{
  pushdecl (mf_cache_array_decl);
  pushdecl (mf_cache_shift_decl);
  pushdecl (mf_cache_mask_decl);
  pushdecl (mf_check_fndecl);
  pushdecl (mf_register_fndecl);
  pushdecl (mf_unregister_fndecl);
}

/* }}} */
/* {{{ utility functions */

/* Mark and return the given tree node to prevent further mudflap
   transforms.  */
static tree
mx_flag (t)
     tree t;
{
  MARK_TREE_MUDFLAPPED(t);
  return t;
}



/* This is a derivative / subset of build_external_ref in c-typeck.c.  */
static tree
mx_external_ref (t, fun)
     tree t;
     int fun;
{
  assemble_external (t);
  TREE_USED (t) = 1;
  return t;
}



/* Create a properly typed STRING_CST node that describes the given
   declaration.  It will be used as an argument for __mf_register().
   Try to construct a helpful string, including file/function/variable
   name.
*/

static tree
mf_varname_tree (decl)
     tree decl;
{
  static output_buffer buf_rec;
  static int initialized = 0;
  output_buffer *buf = & buf_rec;
  const char *buf_contents;
  tree result;

  if (decl == NULL_TREE)
    abort ();

  if (!initialized)
    {
      init_output_buffer (buf, /* prefix */ NULL, /* line-width */ 0);
      initialized = 1;
    }
  output_clear_message_text (buf);

  /* Add FILENAME[:LINENUMBER]. */
  {
    const char *sourcefile;
    unsigned sourceline;

    sourcefile = DECL_SOURCE_FILE (decl);
    if (sourcefile == NULL && current_function_decl != NULL_TREE)
      sourcefile = DECL_SOURCE_FILE (current_function_decl);
    if (sourcefile == NULL)
      sourcefile = "<unknown file>";

    output_add_string (buf, sourcefile);

    sourceline = DECL_SOURCE_LINE (decl);
    if (sourceline != 0)
      {
	output_add_string (buf, ":");
	output_decimal (buf, sourceline);
      }
  }

  if (current_function_decl != NULL_TREE)
    {
      /* Add (FUNCTION): */
      output_add_string (buf, " (");
      {
	const char *funcname;
	if (DECL_NAME (current_function_decl))
	  funcname = (*lang_hooks.decl_printable_name) (current_function_decl, 2);
	if (funcname == NULL)
	  funcname = "anonymous fn";
	
	output_add_string (buf, funcname);
      }
      output_add_string (buf, ") ");
    }
  else
    output_add_string (buf, " ");

  /* Add <variable-declaration> */
  dump_c_node (buf, decl, 0, 0);

  /* Return the lot as a new STRING_CST.  */
  buf_contents = output_finalize_message (buf);
  result = fix_string_type (build_string (strlen (buf_contents) + 1, buf_contents));
  output_clear_message_text (buf);

  return mx_flag (result);
}


/* And another friend, for producing a simpler message.  */
static tree
mf_file_function_line_tree (file, line)
     const char * file;
     int line;
{
  static output_buffer buf_rec;
  static int initialized = 0;
  output_buffer *buf = & buf_rec;
  const char *buf_contents;
  tree result;

  if (!initialized)
    {
      init_output_buffer (buf, /* prefix */ NULL, /* line-width */ 0);
      initialized = 1;
    }
  output_clear_message_text (buf);

  /* Add FILENAME[:LINENUMBER]. */
  if (file == NULL && current_function_decl != NULL_TREE)
    file = DECL_SOURCE_FILE (current_function_decl);
  if (file == NULL)
    file = "<unknown file>";
  output_add_string (buf, file);

  if (line > 0)
    {
      output_add_string (buf, ":");
      output_decimal (buf, line);
    }

  /* Add (FUNCTION) */
  if (current_function_decl != NULL_TREE)
    {
      output_add_string (buf, " (");
      {
	const char *funcname;
	if (DECL_NAME (current_function_decl))
	  funcname = (*lang_hooks.decl_printable_name) (current_function_decl, 2);
	if (funcname == NULL)
	  funcname = "anonymous fn";
	
	output_add_string (buf, funcname);
      }
      output_add_string (buf, ")");
    }

  /* Return the lot as a new STRING_CST.  */
  buf_contents = output_finalize_message (buf);
  result = fix_string_type (build_string (strlen (buf_contents) + 1, buf_contents));

  return mx_flag (result);
}



/* 
   assuming the declaration "foo a[xdim][ydim][zdim];", we will get
   an expression "a[x][y][z]" as a tree structure something like
   
   {ARRAY_REF, z, type = foo,
    {ARRAY_REF, y, type = foo[zdim],
     {ARRAY_REF, x, type = foo[ydim][zdim],
      {ARRAY, a, type = foo[xdim][ydim][zdim] }}}
   
   from which we will produce an offset value of the form:
   
   {PLUS_EXPR z, {MULT_EXPR zdim,
    {PLUS_EXPR y, {MULT_EXPR ydim, 
     x }}}}
   
*/

static tree 
mf_offset_expr_of_array_ref (t, offset, base)
     tree t;
     tree offset;
     tree *base;
{

  if ( TREE_CODE (t) == ARRAY_REF )
    {
      /* It's a sub-array-ref; recurse. */
      
      tree factor = mx_flag (build (PLUS_EXPR, 
				    integer_type_node, 
				    integer_one_node, 
				    TYPE_MAX_VALUE 
				    (TYPE_DOMAIN (TREE_TYPE (t)))));
      tree child = TREE_OPERAND (t, 0);
      tree next_offset = TREE_OPERAND (t, 1);

      /* mark this node to inhibit further transformation */
      mx_flag (t);
      
      return 
	mx_flag 
	(build (PLUS_EXPR, integer_type_node, offset, 
		mx_flag
		(build (MULT_EXPR, integer_type_node, factor, 
			mf_offset_expr_of_array_ref (child, next_offset, base)))));
    } 
  else if ( TREE_CODE (TREE_TYPE (t)) == ARRAY_TYPE )
    {
      /* It's *not* an ARRAY_REF, but it *is* an ARRAY_TYPE; we are at the
	 bottom of the ARRAY_REF expression */ 
      *base = t;
      return offset;
    }
  else 
    {
      /* It's an array ref of a non-array -> failure. */
      abort ();
    }
}


static tree 
mf_build_check_statement_for (ptrvalue, finale, filename, lineno)
     tree ptrvalue;
     tree *finale;
     const char *filename;
     int lineno;
{
  tree ptrtype = TREE_TYPE (ptrvalue);
  tree myptrtype = build_qualified_type (ptrtype, TYPE_QUAL_CONST);
  tree location_string;

  tree t1_1;
  tree t1_2, t1_2_1;
  tree t1_3, t1_3_1;
  tree t1_4, t1_4_1, t1_4_2;
  tree t1_98;
  tree t1_99;
  tree t1;
  tree t0;

  tree return_type, return_value;

  location_string = mf_file_function_line_tree (filename, lineno);
  
  /* ({ */
  t1_1 = build_stmt (SCOPE_STMT, NULL_TREE);
  SCOPE_BEGIN_P (t1_1) = 1;
  
  pushlevel (0);

  /* <TYPE> const __mf_value = <EXPR>; */
  t1_2_1 = build_decl (VAR_DECL, get_identifier ("__mf_value"), myptrtype);
  DECL_ARTIFICIAL (t1_2_1) = 1;
  DECL_INITIAL (t1_2_1) = ptrvalue;
  t1_2 = build1 (DECL_STMT, myptrtype, pushdecl (t1_2_1));
  TREE_CHAIN (t1_1) = t1_2;

  /* struct __mf_cache * const __mf_elem = [...] */
  t1_3_1 = build_decl (VAR_DECL, get_identifier ("__mf_elem"), mf_cache_structptr_type);
  DECL_ARTIFICIAL (t1_3_1) = 1;
  DECL_INITIAL (t1_3_1) =
    /* & __mf_lookup_cache [(((uintptr_t)__mf_value) >> __mf_shift) & __mf_mask] */
    mx_flag (build1 (ADDR_EXPR, mf_cache_structptr_type,
		     mx_flag (build (ARRAY_REF, TYPE_MAIN_VARIANT (TREE_TYPE
								   (TREE_TYPE
								    (mf_cache_array_decl))),
				     mx_external_ref (mf_cache_array_decl, 0),
				     build (BIT_AND_EXPR, mf_uintptr_type,
					    build (RSHIFT_EXPR, mf_uintptr_type,
						   convert (mf_uintptr_type, t1_2_1),
						   mx_external_ref (mf_cache_shift_decl, 0)),
					    mx_external_ref (mf_cache_mask_decl, 0))))));
  
  t1_3 = build1 (DECL_STMT, mf_cache_structptr_type, pushdecl (t1_3_1));
  TREE_CHAIN (t1_2) = t1_3;
  
  /* Quick validity check.  */
  t1_4_1 = build (BIT_IOR_EXPR, integer_type_node,
		  build (GT_EXPR, integer_type_node,
			 build (COMPONENT_REF, mf_uintptr_type, /* __mf_elem->low */
				mx_flag (build1 (INDIRECT_REF, 
						 mf_cache_struct_type, t1_3_1)),
				TYPE_FIELDS (mf_cache_struct_type)),
			 t1_2_1), /* __mf_value */
		  build (LT_EXPR, integer_type_node,
			 build (COMPONENT_REF, mf_uintptr_type, /* __mf_elem->high */
				mx_flag (build1 (INDIRECT_REF, 
						 mf_cache_struct_type, t1_3_1)),
				TREE_CHAIN (TYPE_FIELDS (mf_cache_struct_type))),
			 build (PLUS_EXPR, mf_uintptr_type, /* __mf_value + sizeof(T) - 1 */
				t1_2_1,
				fold (build (MINUS_EXPR, mf_uintptr_type,
					     convert (mf_uintptr_type,
						      TYPE_SIZE_UNIT 
						      (TREE_TYPE 
						       (TREE_TYPE (ptrvalue)))),
					     integer_one_node)))));
  
  /* Mark condition as UNLIKELY using __builtin_expect.  */
  t1_4_1 = build_function_call (built_in_decls[BUILT_IN_EXPECT],
				tree_cons (NULL_TREE,
					   convert (long_integer_type_node, t1_4_1),
					   tree_cons (NULL_TREE,
						      integer_zero_node,
						      NULL_TREE)));
  
  t1_4_2 = build_function_call (mx_external_ref (mf_check_fndecl, 1),
				tree_cons (NULL_TREE,
					   convert (mf_uintptr_type, t1_2_1),
					   tree_cons (NULL_TREE, 
						      convert (mf_uintptr_type, 
							       TYPE_SIZE_UNIT 
							       (TREE_TYPE (TREE_TYPE (ptrvalue)))),
						      tree_cons (NULL_TREE,
								 location_string,
								 NULL_TREE))));
  
  t1_4 = build_stmt (IF_STMT, 
		     t1_4_1,
		     build1 (EXPR_STMT, void_type_node, t1_4_2),
		     NULL_TREE);
  TREE_CHAIN (t1_3) = t1_4;

  if (finale)
    {
      return_type = TREE_TYPE (*finale);
      return_value = *finale;
    } 
  else 
    {
      return_type = myptrtype;
      return_value = t1_2_1;
    }

  /* "return" __mf_value, or provided finale */
  t1_98 = build1 (EXPR_STMT, return_type, return_value);
  TREE_CHAIN (t1_4) = t1_98;
  
  t1_99 = build_stmt (SCOPE_STMT, NULL_TREE);
  TREE_CHAIN (t1_98) = t1_99;
  
  t1 = build1 (COMPOUND_STMT, return_type, t1_1);
  t0 = build1 (STMT_EXPR, return_type, t1);
  TREE_SIDE_EFFECTS (t0) = 1;

  poplevel (1, 1, 0);

  return t0;
}

/* }}} */
/* ------------------------------------------------------------------------ */
/* {{{ INDIRECT_REF transform */

/* Perform the mudflap bounds-checking transform on the given function
   tree.  The tree is mutated in place, with possibly copied subtree
   nodes.

   (1)  (INDIRECT_REF (tree))
        ==>
        (INDIRECT_REF ({TYPE * const value = (tree); // <== RECURSE HERE
	                struct __mf_cache * elem = 
			     & __mf_lookup_cache [((unsigned)value >> __mf_shift) &
                                                  __mf_mask];
			if (UNLIKELY ((elem->low > value) ||
			              (elem->high < value+sizeof(TYPE)-1)))
			   __mf_check (value, sizeof(TYPE));
                        value;})

   (not yet implemented:)
   (2) (ARRAY_REF ({ARRAY_REF ... (tree, indexM)}, indexN))  //  tree[N][M][O]...
       ==> (as if)
       (INDIRECT_REF (tree + (.... + sizeM*sizeO*indexO + sizeM*indexM + indexN))
*/


static tree
mx_xfn_indirect_ref (t, continue_p, data)
     tree *t;
     int *continue_p;
     void *data; /* NOTUSED */
{
  static const char *last_filename = NULL;
  static int last_lineno = -1;

#if 0
  fprintf (stderr, "expr=%s\n", tree_code_name [TREE_CODE (*t)]);
#endif
  
  *continue_p = 1;

  /* Avoid infinite recursion of transforming transformed code. */
  if (TREE_MUDFLAPPED_P (*t))
    return NULL_TREE;

  /* Track file-name/line-numbers.  */
  if (statement_code_p (TREE_CODE (*t)))
    last_lineno = STMT_LINENO (*t);
  if (TREE_CODE (*t) == FILE_STMT)
    last_filename = FILE_STMT_FILENAME (*t);
  if (TREE_CODE (*t) == EXPR_WITH_FILE_LOCATION)
    {
      last_filename = EXPR_WFL_FILENAME (*t);
      last_lineno = EXPR_WFL_LINENO (*t);
    }
      
  /* Process some node types.  */
  switch (TREE_CODE (*t))
    {
    default:
      ; /* Continue traversal.  */
      break;

    case ARRAY_REF:
      {
	tree base_array, base_obj_type, base_ptr_type;
	tree offset_expr;
	tree check_ptr;
	tree tmp;

	offset_expr = mf_offset_expr_of_array_ref (TREE_OPERAND (*t,0), 
						   TREE_OPERAND (*t,1), 
						   &base_array);
	
	/* We now have a tree representing the array in base_array, 
	   and a tree representing the complete desired offset in
	   offset_expr. */
	
	base_obj_type = TREE_TYPE (TREE_TYPE (TREE_OPERAND(*t,0)));
	base_ptr_type = build_pointer_type (base_obj_type);

	check_ptr = 
	  mx_flag 
	  (build (PLUS_EXPR, 
		  base_ptr_type,
		  mx_flag 
		  (build1 (ADDR_EXPR, 
			   base_ptr_type, 
			   mx_flag 
			   (build (ARRAY_REF, 
				   base_obj_type, 
				   base_array, integer_zero_node)))),
		  mx_flag 
		  (build (MULT_EXPR, 
			  integer_type_node,
			  TYPE_SIZE_UNIT (base_obj_type),
			  offset_expr))));
	
	tmp = mf_build_check_statement_for (check_ptr, NULL, last_filename, last_lineno);
	*t = build1 (INDIRECT_REF, base_obj_type, mx_flag (tmp));;
	mx_flag (*t);

	/*
	fprintf (stderr, "\n");
 	print_c_tree (stderr, *t);
	fprintf (stderr, "\n");
	*/

      }
      break;
      
    case ARRAY_RANGE_REF:
      /* not yet implemented */
      warning ("mudflap checking not yet implemented for ARRAY_RANGE_REF");
      break;

    case INDIRECT_REF:
      /*
	fprintf (stderr, "\n");
	print_c_tree (stderr, *t);
	fprintf (stderr, "\n");
      */
      
      /* Substitute check statement for ptrvalue in INDIRECT_REF.  */
      TREE_OPERAND (*t, 0) = 
	mf_build_check_statement_for (TREE_OPERAND (*t, 0), NULL, 
				      last_filename, last_lineno);
	
	/*
	  fprintf (stderr, "\n");
	  print_c_tree (stderr, *t);
	  fprintf (stderr, "\n");
	*/
	
	/* Prevent this transform's reapplication to this tree node.
	   Note that we do not prevent recusion in walk_tree toward
	   subtrees of this node, in case of nested pointer expressions.  */
	mx_flag (*t);
      break;
    }

  return NULL_TREE;
}



static void
mf_xform_derefs (fnbody)
     tree fnbody;
{
  walk_tree_without_duplicates (& fnbody, mx_xfn_indirect_ref, NULL);
}

/* }}} */
/* ------------------------------------------------------------------------ */
/* {{{ ADDR_EXPR transform */

/* This struct is passed between mf_xform_decls to store state needed
during the traversal searching for objects that have their addresses
taken. */
struct mf_xform_decls_data
{
  tree last_compound_stmt;
  tree param_decls;
  varray_type compound_stmt_stack;  /* track nesting level: SCOPE_BEGIN_P pushes, END_P pops. */
};



/* Destructively insert, between *posn and TREE_CHAIN(*posn), a pair of
   register / cleanup statements, corresponding to variable described in
   decl, in the scope of the containing_stmt. Appending is done by  */

static void
mx_register_decl (posn, decl, containing_stmt)
     tree *posn;
     tree decl;
     tree containing_stmt;
{
  /* Is the address of this decl taken anyplace?  */
  if ((TREE_CODE (decl) == VAR_DECL || TREE_CODE (decl) == PARM_DECL) &&
      (! TREE_STATIC (decl)) &&
      mf_find_addrof (containing_stmt, decl))
    {
      /* Synthesize, for this DECL_STMT, a CLEANUP_DECL for the same
	 VAR_DECL.  Arrange to call the __mf_register function now, and the
	 __mf_unregister function later.  */
      
      /* (& VARIABLE, sizeof (VARIABLE)) */
      tree unregister_fncall_params =
	tree_cons (NULL_TREE,
		   convert (mf_uintptr_type, 
			    mx_flag (build1 (ADDR_EXPR, 
					     build_pointer_type (TREE_TYPE (decl)),
					     decl))),
		   tree_cons (NULL_TREE, 
			      convert (mf_uintptr_type, 
				       TYPE_SIZE_UNIT (TREE_TYPE (decl))),
			      NULL_TREE));
      /* __mf_unregister (...) */
      tree unregister_fncall =
	build_function_call (mx_external_ref (mf_unregister_fndecl, 1),
			     unregister_fncall_params);
      
      tree cleanup_stmt = build_stmt (CLEANUP_STMT, decl, unregister_fncall);
      
      /* (& VARIABLE, sizeof (VARIABLE), __MF_LIFETIME_STACK=2) */
      tree variable_name = mf_varname_tree (decl);
      tree register_fncall_params =
	tree_cons (NULL_TREE,
		   convert (mf_uintptr_type, 
			    mx_flag (build1 (ADDR_EXPR, 
					     build_pointer_type (TREE_TYPE (decl)),
					     decl))),
		   tree_cons (NULL_TREE, 
			      convert (mf_uintptr_type, 
				       TYPE_SIZE_UNIT (TREE_TYPE (decl))),
			      tree_cons (NULL_TREE,
					 build_int_2 (2, 0),
					 tree_cons (NULL_TREE,
						    variable_name,
						    NULL_TREE))));
      /* __mf_register (...) */
      tree register_fncall =
	build_function_call (mx_external_ref (mf_register_fndecl, 1),
			     register_fncall_params);
      
      tree register_fncall_stmt =
	build1 (EXPR_STMT, void_type_node, register_fncall);
      
      /* Add the CLEANUP_STMT and register() call after *posn.  */
      TREE_CHAIN (cleanup_stmt) = register_fncall_stmt;
      TREE_CHAIN (register_fncall_stmt) = TREE_CHAIN (*posn);
      TREE_CHAIN (*posn) = cleanup_stmt;
    }  
}


/* As a tree traversal function, maintain a scope stack for the
   current traversal stage, so that each DECL_STMT knows its enclosing
   COMPOUND_STMT.  For appropriate DECL_STMTs, perform the mudflap
   lifetime-tracking transform.

   XXX: what about static vars declared in function scope?  They probably
   should never be __mf_unregister'd, and only be registered once per
   program startup.  See TREE_STATIC.

   XXX: it would be nice to reuse the binding_level construct from
   c-decl.c for this stuff.

   XXX: Couldn't we just use TREE_ADDRESSABLE on the decl, instead of
   searching for ADDR_EXPR?
*/

static tree
mx_xfn_xform_decls (t, continue_p, data)
     tree *t;
     int *continue_p;
     void *data;
{
  struct mf_xform_decls_data* d = (struct mf_xform_decls_data*) data;

  *continue_p = 1;

  switch (TREE_CODE (*t))
    {
    case COMPOUND_STMT:
      d->last_compound_stmt = *t;
      break;
      
    case SCOPE_STMT:
      if (SCOPE_BEGIN_P (*t))
	{	
	  VARRAY_PUSH_TREE (d->compound_stmt_stack, d->last_compound_stmt);

	  /* Register any function parameters not-yet-registered. */
	  while (d->param_decls != NULL_TREE)
	    {
	      mx_register_decl 
		(t, d->param_decls, VARRAY_TOP_TREE (d->compound_stmt_stack));
	      d->param_decls = TREE_CHAIN (d->param_decls);
	    }
	}
      else
	VARRAY_POP (d->compound_stmt_stack);
      break;

    case DECL_STMT:
      {
	mx_register_decl 
	  (t, DECL_STMT_DECL (*t), VARRAY_TOP_TREE (d->compound_stmt_stack));
      }
      break;

    default:
      break;
    }

  return NULL;
}



/* Perform the object lifetime tracking mudflap transform on the given function
   tree.  The tree is mutated in place, with possibly copied subtree nodes.

   For every auto variable declared, if its address is ever taken
   within the function, then supply its lifetime to the mudflap
   runtime with the __mf_register and __mf_unregister calls.
*/

static void
mf_xform_decls (fnbody, fnparams)
     tree fnbody;
     tree fnparams;
{
  struct mf_xform_decls_data d;
  d.param_decls = fnparams;
  d.last_compound_stmt = NULL_TREE;
  VARRAY_TREE_INIT (d.compound_stmt_stack, 100, "compound_stmt stack");

  walk_tree_without_duplicates (& fnbody, mx_xfn_xform_decls, & d);
}



/* As a tree traversal function, find the first expression node within
   the given tree that is an ADDR_EXPR or ARRAY_REF of the given
   VAR_DECL.
*/
static tree
mx_xfn_find_addrof (t, continue_p, data)
     tree *t;
     int *continue_p;
     void *data;
{
  tree decl = (tree) data;
  tree gotit = NULL;
  tree operand = NULL;

  *continue_p = 1;

  if (*t == NULL_TREE)
    return gotit;

  if (TREE_MUDFLAPPED_P (*t))
    return gotit;

  switch (TREE_CODE(*t))
    {
    case ARRAY_REF:
    case ADDR_EXPR:
      operand = TREE_OPERAND (*t, 0);

      /* Back out to the largest containing structure. */
      while (TREE_CODE (operand) == COMPONENT_REF)
	operand = TREE_OPERAND (operand, 0);

      /* Is the enclosing object the declared thing we're looking for? */
      if ((TREE_CODE (operand) == VAR_DECL ||
	   TREE_CODE (operand) == PARM_DECL) && 
	  (decl == operand))
	gotit = *t;
      break;

    default:
      break;
    }


#if 0
	if (gotit != NULL)
	  {
	  fprintf (stderr, "matched decl=");
	  print_c_tree (stderr, decl);
	  fprintf (stderr, " in tree=");
	  print_c_tree (stderr, gotit);
	  fprintf (stderr, "\n");
	  }
#endif

  return gotit;
}



/* Find and return any instance of an ADDR_EXPR tree referring to decl
   under the given statement.  */
static tree
mf_find_addrof (stmt, decl)
     tree stmt;
     tree decl;
{
  return walk_tree_without_duplicates (& stmt, mx_xfn_find_addrof, decl);
}

/* }}} */
/* ------------------------------------------------------------------------ */
/* {{{ global variable transform */

/* A chain of EXPR_STMTs for calling __mf_register() at initialization
   time.  */
static GTY (()) tree enqueued_call_stmt_chain = NULL_TREE;


/* Build and enqueue EXPR_STMT for calling __mf_register on the object
   given by the parameters.  One odd thing: the object's address is
   given by the given assembler label string (since that's all we may
   know about a string literal, or the static data thingie may be out
   of the future scope).  To turn that into a validish C tree, we
   create a weird synthetic VAR_DECL node.
*/ 

static void
mf_enqueue_register_call (label, regsize, regtype, regname)
     const char* label;
     tree regsize;
     tree regtype;
     tree regname;
{
  tree decltype, decl;
  tree call_params;
  tree call_stmt;

  mf_init_extern_trees ();

  /* See gcc-checker's c-bounds.c (declare_private_statics)  */
  decltype = build_array_type (char_type_node, build_index_type (integer_zero_node));
  decl = mx_flag (build_decl (VAR_DECL, get_identifier (label), decltype));

  TREE_STATIC (decl) = 1;
  TREE_READONLY (decl) = 1;
  TREE_ASM_WRITTEN (decl) = 1;
  DECL_IGNORED_P (decl) = 1;
  DECL_INITIAL (decl) = NULL_TREE;
  layout_decl (decl, 0);
  TREE_USED (decl) = 1;
  SET_DECL_ASSEMBLER_NAME (decl, get_identifier (label));
  DECL_DEFER_OUTPUT (decl) = 1;
  /* XXX: what else? */
  /* rest_of_decl_compilation (decl, build_string (strlen (label) + 1, label), 1, 0); */
  /* make_decl_rtl (decl,  build_string (strlen (label) + 1, label)); */

  call_params = tree_cons (NULL_TREE,
			   convert (mf_uintptr_type, 
				    mx_flag (build1 (ADDR_EXPR, 
						     build_pointer_type (TREE_TYPE (decl)),
						     decl))),
			   tree_cons (NULL_TREE, 
				      convert (mf_uintptr_type, regsize),
				      tree_cons (NULL_TREE,
						 regtype,
						 tree_cons (NULL_TREE,
							    regname,
							    NULL_TREE))));

  call_stmt = build1 (EXPR_STMT, void_type_node,
		      build_function_call (mx_external_ref (mf_register_fndecl, 1),
					   call_params));

  /* Link this call into the chain. */
  TREE_CHAIN (call_stmt) = enqueued_call_stmt_chain;
  enqueued_call_stmt_chain = call_stmt;
}


/* Emit a synthetic CTOR function for the current file.  Populate it
   from the enqueued __mf_register calls.  Call the RTL expanders
   inline.  */

static void
mf_flush_enqueued_calls ()
{
  /* See profile.c (output_func_start_profiler) */
  tree fnname;
  char *nmplus;
  tree fndecl;
  tree body;

  /* Short-circuit!  */
  if (enqueued_call_stmt_chain == NULL_TREE)
    return;

  /* Create the COMPOUND_STMT that becomes the new function's body.  */
  body = make_node (COMPOUND_STMT);
  COMPOUND_BODY (body) = enqueued_call_stmt_chain;

  /* Create a ctor function declaration.  */
  nmplus = concat (IDENTIFIER_POINTER (get_file_function_name ('I')), "_mudflap", NULL);
  fnname = get_identifier (nmplus);
  free (nmplus);
  fndecl = build_decl (FUNCTION_DECL, fnname,
		       build_function_type (void_type_node, NULL_TREE));
  DECL_EXTERNAL (fndecl) = 0;
  TREE_PUBLIC (fndecl) = ! targetm.have_ctors_dtors;
  TREE_USED (fndecl) = 1;
  DECL_RESULT (fndecl) = build_decl (RESULT_DECL, NULL_TREE, void_type_node);

  /* Now compile the sucker as we go.  This is a weird semi-inlined
  form of the guts of the c-parse.y `fndef' production, and a hybrid
  with c_expand_body. */

  /* start_function */
  fndecl = pushdecl (fndecl);
  pushlevel (0);
  rest_of_decl_compilation (fndecl, 0, 1, 0);
  announce_function (fndecl);
  current_function_decl = fndecl;
  DECL_INITIAL (fndecl) = error_mark_node;
  DECL_SAVED_TREE (fndecl) = body;
  make_decl_rtl (fndecl, NULL);

  /* store_parm_decls */
  init_function_start (fndecl, input_filename, lineno);
  cfun->x_whole_function_mode_p = 1;

  /* finish_function */
  poplevel (1, 0, 1);
  BLOCK_SUPERCONTEXT (DECL_INITIAL (fndecl)) = fndecl;

  /* c_expand_body */
  expand_function_start (fndecl, 0);
  expand_stmt (DECL_SAVED_TREE (fndecl));
  if (lang_expand_function_end)
    (*lang_expand_function_end) ();
  expand_function_end (input_filename, lineno, 0);
  rest_of_compilation (fndecl);
  if (! quiet_flag) 
    fflush (asm_out_file);
  current_function_decl = NULL_TREE;
  if (targetm.have_ctors_dtors)
    (* targetm.asm_out.constructor) (XEXP (DECL_RTL (fndecl), 0),
                                     DEFAULT_INIT_PRIORITY);
  else
    static_ctors = tree_cons (NULL_TREE, fndecl, static_ctors);

  /* XXX: We could free up enqueued_call_stmt_chain here. */
}

/* }}} */


#include "gt-tree-mudflap.h"
