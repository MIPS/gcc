/* Mudflap: narrow-pointer bounds-checking by tree rewriting.
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.
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
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-inline.h"
#include "tree-simple.h"
#include "c-common.h"
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
#include "demangle.h"
#include "tree-dump.h"

/* Internal function decls */

static void mf_xform_derefs (tree);
static void mf_xform_decls (tree, tree);
static void mf_init_extern_trees (void);
static void mf_decl_cache_locals (tree *);
static void mf_decl_clear_locals (void);
static tree mf_varname_tree (tree);
static tree mf_file_function_line_tree (const char *, int);
static tree mf_mostly_copy_tree_r (tree *, int *, void *);
static tree mx_xfn_indirect_ref (tree *, int *, void *);
static tree mx_xfn_xform_decls (tree *, int *, void *);

static tree mf_offset_expr_of_array_ref (tree, tree *, tree *, tree *);
static tree mf_build_check_statement_for (tree, tree, tree, tree, tree,
					  const char *, int);
static void mx_register_decls (tree, tree *);


/* extern mudflap functions */




static GTY ((param_is (union tree_node))) htab_t marked_trees = NULL;


/* Mark and return the given tree node to prevent further mudflap
   transforms.  */
tree
mf_mark (tree t)
{
  void **slot;

  if (marked_trees == NULL)
    marked_trees = htab_create_ggc (31, htab_hash_pointer, htab_eq_pointer, NULL);

  slot = htab_find_slot (marked_trees, t, INSERT);
  *slot = t;
  return t;
}


int
mf_marked_p (tree t)
{
  void *entry;

  if (marked_trees == NULL)
    return 0;

  entry = htab_find (marked_trees, t);
  return (entry != NULL);
}



/* Perform the mudflap tree transforms on the given function.  Return
   the modified function body.  */

tree
mudflap_c_function (tree t)
{
  tree original, result;

  mf_init_extern_trees ();

  result = DECL_SAVED_TREE (t);
  original = result;
  walk_tree (& result, mf_mostly_copy_tree_r, NULL, NULL);

  /* In multithreaded mode, don't cache the lookup cache parameters.  */
  if (! (flag_mudflap > 1))
    mf_decl_cache_locals (& result);

  mf_xform_decls (result, DECL_ARGUMENTS (t));
  mf_xform_derefs (result);

  if (! (flag_mudflap > 1))
    mf_decl_clear_locals ();

  /* Gimplify the resulting function, mainly so that tree
     pretty-printing works.  It's expensive though.  */
  DECL_SAVED_TREE (t) = result;
  gimplify_function_tree (t);
  result = DECL_SAVED_TREE (t);

  {
    FILE *dump_file;
    int dump_flags;
    dump_file = dump_begin (TDI_mudflap, &dump_flags);
    if (dump_file)
      {
	fprintf (dump_file, "\n\n;;; Function %s\n", IDENTIFIER_POINTER (DECL_NAME (t)));
	print_generic_expr (dump_file, result, 0);
	fprintf (dump_file, "\n");
	dump_end (TDI_mudflap, dump_file);
      }
  }

  /* Restore original tree to function body, in case it is used
     for inlining later.  */
  DECL_SAVED_TREE (t) = original;

  return result;
}



/* global tree nodes */

/* Global tree objects for global variables and functions exported by
   mudflap runtime library.  mf_init_extern_trees must be called
   before using these.  */

static GTY (()) tree mf_uintptr_type;      /* uintptr_t (usually "unsigned long") */
static GTY (()) tree mf_cache_struct_type; /* struct __mf_cache { uintptr_t low; uintptr_t high; }; */
static GTY (()) tree mf_cache_structptr_type; /* struct __mf_cache * const */

static GTY (()) tree mf_cache_array_decl;  /* extern struct __mf_cache __mf_lookup_cache []; */
static GTY (()) tree mf_cache_shift_decl;  /* extern const unsigned char __mf_lc_shift; */
static GTY (()) tree mf_cache_mask_decl;   /* extern const uintptr_t __mf_lc_mask; */

/* Their function-scope local shadows, used in single-threaded mode only. */
static GTY (()) tree mf_cache_shift_decl_l; /* auto const unsigned char __mf_lc_shift_l; */
static GTY (()) tree mf_cache_mask_decl_l;  /* auto const uintptr_t __mf_lc_mask_l; */

static GTY (()) tree mf_check_fndecl;      /* extern void __mf_check (void *ptr, size_t sz, int type, const char *); */
static GTY (()) tree mf_register_fndecl;   /* extern void __mf_register (void *ptr, size_t sz, int type, const char *); */
static GTY (()) tree mf_unregister_fndecl; /* extern void __mf_unregister (void *ptr, size_t sz); */



/* Initialize the global tree nodes that correspond to mf-runtime.h
   declarations.  */
static void
mf_init_extern_trees (void)
{
  static int done = 0;

  if (done) return;

  mf_uintptr_type = TREE_TYPE (mflang_lookup_decl ("uintptr_t"));
  mf_cache_array_decl = mf_mark (mflang_lookup_decl ("__mf_lookup_cache"));
  mf_cache_struct_type = TREE_TYPE (TREE_TYPE (mf_cache_array_decl));
  mf_cache_structptr_type = build_pointer_type (mf_cache_struct_type);
  mf_cache_shift_decl = mf_mark (mflang_lookup_decl ("__mf_lc_shift"));
  mf_cache_mask_decl = mf_mark (mflang_lookup_decl ("__mf_lc_mask"));
  mf_check_fndecl = mflang_lookup_decl ("__mf_check");
  mf_register_fndecl = mflang_lookup_decl ("__mf_register");
  mf_unregister_fndecl = mflang_lookup_decl ("__mf_unregister");

  done = 1;
}



/* Create and initialize local shadow variables for the lookup cache
   globals.  Put their decls in the *_l globals for use by
   mf_build_check_statement_for. */

static void
mf_decl_cache_locals (tree* body)
{
  tree init_exprs = NULL_TREE;
  tree t;

  /* Create the chain of VAR_DECL nodes.  */
  mf_cache_shift_decl_l
    = mf_mark (build_decl (VAR_DECL,
			   get_identifier ("__mf_lookup_shift_l"),
			   TREE_TYPE (mf_cache_shift_decl)));
  DECL_ARTIFICIAL (mf_cache_shift_decl_l) = 1;
  DECL_CONTEXT (mf_cache_shift_decl_l) = current_function_decl;

  mf_cache_mask_decl_l
    = mf_mark (build_decl (VAR_DECL,
			   get_identifier ("__mf_lookup_mask_l"),
			   TREE_TYPE (mf_cache_mask_decl)));
  DECL_ARTIFICIAL (mf_cache_mask_decl_l) = 1;
  DECL_CONTEXT (mf_cache_mask_decl_l) = current_function_decl;
  TREE_CHAIN (mf_cache_shift_decl_l) = mf_cache_mask_decl_l;

  /* Build initialization nodes for them.  */
  t = build (INIT_EXPR, TREE_TYPE (mf_cache_shift_decl_l),
	     mf_cache_shift_decl_l, mf_cache_shift_decl);
  append_to_statement_list (t, &init_exprs);
  t = build (INIT_EXPR, TREE_TYPE (mf_cache_mask_decl_l),
	     mf_cache_mask_decl_l, mf_cache_mask_decl);
  append_to_statement_list (t, &init_exprs);

  /* Add the function body to the end. */
  append_to_statement_list (*body, &init_exprs);

  *body = build (BIND_EXPR, TREE_TYPE (init_exprs),
		 mf_cache_shift_decl_l, init_exprs,
		 NULL_TREE); /* XXX: BLOCK == NULL */
  TREE_SIDE_EFFECTS (*body) = 1;
}


static void
mf_decl_clear_locals (void)
{
  /* Unset local shadows. */
  mf_cache_shift_decl_l = NULL_TREE;
  mf_cache_mask_decl_l = NULL_TREE;
}




/* A copy of c-simplify.c's mostly_copy_tree_r.  */
static tree
mf_mostly_copy_tree_r (tree *tp, int *walk_subtrees, void *data)
{
  /* Retain mudflap marked-ness across copy.  */
  int was_marked = mf_marked_p (*tp);

  if (TREE_CODE (*tp) == SAVE_EXPR)
    *walk_subtrees = 0;
  else
    copy_tree_r (tp, walk_subtrees, data);

  if (was_marked) mf_mark (*tp);

  return NULL_TREE;
}



/* Create a properly typed STRING_CST node that describes the given
   declaration.  It will be used as an argument for __mf_register().
   Try to construct a helpful string, including file/function/variable
   name.
*/

static tree
mf_varname_tree (tree decl)
{
  static pretty_printer buf_rec;
  static int initialized = 0;
  pretty_printer *buf = & buf_rec;
  const char *buf_contents;
  tree result;

  if (decl == NULL_TREE)
    abort ();

  if (!initialized)
    {
      pp_construct (buf, /* prefix */ NULL, /* line-width */ 0);
      initialized = 1;
    }
  pp_clear_output_area (buf);

  /* Add FILENAME[:LINENUMBER]. */
  {
    const char *sourcefile;
    unsigned sourceline;

    sourcefile = DECL_SOURCE_FILE (decl);
    if (sourcefile == NULL && current_function_decl != NULL_TREE)
      sourcefile = DECL_SOURCE_FILE (current_function_decl);
    if (sourcefile == NULL)
      sourcefile = "<unknown file>";

    pp_string (buf, sourcefile);

    sourceline = DECL_SOURCE_LINE (decl);
    if (sourceline != 0)
      {
	pp_string (buf, ":");
	pp_decimal_int (buf, sourceline);
      }
  }

  if (current_function_decl != NULL_TREE)
    {
      /* Add (FUNCTION): */
      pp_string (buf, " (");
      {
	const char *funcname = NULL;
	if (DECL_NAME (current_function_decl))
	  funcname = (*lang_hooks.decl_printable_name) (current_function_decl, 1);
	if (funcname == NULL)
	  funcname = "anonymous fn";

	pp_string (buf, funcname);
      }
      pp_string (buf, ") ");
    }
  else
    pp_string (buf, " ");

  /* Add <variable-declaration>, possibly demangled.  */
  {
    const char *declname = NULL;

    if (strcmp ("GNU C++", lang_hooks.name) == 0 &&
	DECL_NAME (decl) != NULL)
      {
	/* The gcc/cp decl_printable_name hook doesn't do as good a job as
	   the libiberty demangler.  */
	declname = cplus_demangle (IDENTIFIER_POINTER (DECL_NAME (decl)),
				   DMGL_AUTO | DMGL_VERBOSE);
      }

    if (declname == NULL)
      declname = (*lang_hooks.decl_printable_name) (decl, 3);

    if (declname == NULL)
      declname = "<unnamed variable>";

    pp_string (buf, declname);
  }

  /* Return the lot as a new STRING_CST.  */
  buf_contents = pp_base_formatted_text (buf);
  result = fix_string_type (build_string (strlen (buf_contents) + 1, buf_contents));
  pp_clear_output_area (buf);

  return mf_mark (result);
}


/* And another friend, for producing a simpler message.  */
static tree
mf_file_function_line_tree (const char * file, int line)
{
  static pretty_printer buf_rec;
  static int initialized = 0;
  pretty_printer *buf = & buf_rec;
  const char *buf_contents;
  tree result;

  if (!initialized)
    {
      pp_construct (buf, /* prefix */ NULL, /* line-width */ 0);
      initialized = 1;
    }
  pp_clear_output_area (buf);

  /* Add FILENAME[:LINENUMBER]. */
  if (file == NULL && current_function_decl != NULL_TREE)
    file = DECL_SOURCE_FILE (current_function_decl);
  if (file == NULL)
    file = "<unknown file>";
  pp_string (buf, file);

  if (line > 0)
    {
      pp_string (buf, ":");
      pp_decimal_int (buf, line);
    }

  /* Add (FUNCTION) */
  if (current_function_decl != NULL_TREE)
    {
      pp_string (buf, " (");
      {
	const char *funcname = NULL;
	if (DECL_NAME (current_function_decl))
	  funcname = (*lang_hooks.decl_printable_name) (current_function_decl, 1);
	if (funcname == NULL)
	  funcname = "anonymous fn";

	pp_string (buf, funcname);
      }
      pp_string (buf, ")");
    }

  /* Return the lot as a new STRING_CST.  */
  buf_contents = pp_base_formatted_text (buf);
  result = fix_string_type (build_string (strlen (buf_contents) + 1, buf_contents));

  return mf_mark (result);
}



/*
   Assuming the declaration "foo a[xdim][ydim][zdim];", we will get
   an expression "a[x][y][z]" as a tree structure something like

   {ARRAY_REF, z, type = foo,
    {ARRAY_REF, y, type = foo[zdim],
     {ARRAY_REF, x, type = foo[ydim][zdim],
      {ARRAY, a, type = foo[xdim][ydim][zdim] }}}

   from which we will produce an offset value of the form:

   {PLUS_EXPR z, {MULT_EXPR zdim,
    {PLUS_EXPR y, {MULT_EXPR ydim,
     x }}}}

   The offset expression may be folded to a constant, or may
   require reference to temporary variables whose decls are
   pushed on to the decls chain.
*/

static tree
mf_offset_expr_of_array_ref (tree t, tree *offset, tree *base, tree *decls)
{
  /* Replace the array index operand [1] with a temporary variable.
     This is meant to emulate SAVE_EXPRs that are sometimes screwed up
     by other parts of gcc.  */
  if ((TREE_CODE (t) == ARRAY_REF ||
       TREE_CODE (TREE_TYPE (t)) == ARRAY_TYPE) &&
      ! really_constant_p (*offset))
    {
      static unsigned declindex;
      char declname[20];
      tree newdecl, idxexpr;

      idxexpr = *offset;
      sprintf (declname, "__mf_index_%u", declindex++);

      newdecl = build_decl (VAR_DECL, get_identifier (declname),
			    TREE_TYPE (idxexpr));
      DECL_ARTIFICIAL (newdecl) = 1;
      DECL_INITIAL (newdecl) = idxexpr;
      DECL_CONTEXT (newdecl) = current_function_decl;

      /* Accumulate this new decl. */
      *decls = tree_cons (NULL_TREE, newdecl, *decls);

      /* Replace the index expression with the plain VAR_DECL reference.  */
      *offset = newdecl;
    }

  if (TREE_CODE (t) == ARRAY_REF)
    {
      /* It's a sub-array-ref; recurse. */

      tree factor = fold (build (PLUS_EXPR,
				 integer_type_node,
				 integer_one_node,
				 TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (t)))));

      /* Mark this node to inhibit further transformation.  */
      mf_mark (t);

      return
	fold (build (PLUS_EXPR, integer_type_node, *offset,
		     fold (build (MULT_EXPR, integer_type_node, factor,
				  mf_offset_expr_of_array_ref (TREE_OPERAND (t, 0),
							       & TREE_OPERAND (t, 1),
							       base,
							       decls)))));
    }
  else if (TREE_CODE (TREE_TYPE (t)) == ARRAY_TYPE)
    {
      /* It's *not* an ARRAY_REF, but it *is* an ARRAY_TYPE; we are at
	 the bottom of the ARRAY_REF expression.  */
      *base = t;

      return *offset;
    }
  else
    {
      /* It's an array ref of a non-array -> failure. */
      abort ();
    }
}



static tree
mf_build_check_statement_for (tree ptrvalue, tree chkbase, tree chksize,
			      tree acctype, tree chkdecls,
			      const char *filename, int lineno)
{
  tree ptrtype = TREE_TYPE (ptrvalue);
  tree myptrtype = build_qualified_type (ptrtype, TYPE_QUAL_CONST);
  tree location_string = mf_file_function_line_tree (filename, lineno);

  tree bind_decls = NULL_TREE;  /* A chain of VAR_DECL nodes. */
  tree bind_exprs = NULL_TREE;  /* Eventually a COMPOUND_EXPR. */

  tree t1_2_1;
  tree t1_2a_1;
  tree t1_2b_1;
  tree t1_3_1;
  tree t1_4_1, t1_4_2;

  /* Provide a default access type number */
  if (acctype == NULL_TREE)
    acctype = integer_zero_node; /* __MF_CHECK_READ */

  /* Insert any supplied helper declarations.  */
  while (chkdecls != NULL_TREE)
    {
      tree decl = TREE_VALUE (chkdecls);
      tree declstmt = build_stmt (DECL_STMT, decl);
      DECL_ARTIFICIAL (decl) = 1;
      DECL_CONTEXT (decl) = current_function_decl;
      bind_decls = chainon (bind_decls, decl);
      bind_exprs = chainon (bind_exprs, declstmt);

      chkdecls = TREE_CHAIN (chkdecls);
    }

  /* <TYPE> const __mf_value = <EXPR>; */
  t1_2_1 = build_decl (VAR_DECL, get_identifier ("__mf_value"), myptrtype);
  DECL_ARTIFICIAL (t1_2_1) = 1;
  DECL_INITIAL (t1_2_1) = ptrvalue;
  DECL_CONTEXT (t1_2_1) = current_function_decl;
  bind_decls = chainon (bind_decls, t1_2_1);
  bind_exprs = chainon (bind_exprs, build_stmt (DECL_STMT, t1_2_1));

  /* uintptr_t __mf_base = <EXPR2>; */
  t1_2a_1 = build_decl (VAR_DECL, get_identifier ("__mf_base"),
                        mf_uintptr_type);
  DECL_ARTIFICIAL (t1_2a_1) = 1;
  DECL_INITIAL (t1_2a_1) = convert (mf_uintptr_type,
	                            ((chkbase == ptrvalue) ? t1_2_1 : chkbase));
  DECL_CONTEXT (t1_2a_1) = current_function_decl;
  bind_exprs = chainon (bind_exprs, build_stmt (DECL_STMT, t1_2a_1));
  bind_decls = chainon (bind_decls, t1_2a_1);

  /* uintptr_t __mf_size = <EXPR>; */
  t1_2b_1 = build_decl (VAR_DECL, get_identifier ("__mf_size"),
                        mf_uintptr_type);
  DECL_ARTIFICIAL (t1_2b_1) = 1;
  DECL_INITIAL (t1_2b_1) = convert (mf_uintptr_type,
				    ((chksize == NULL_TREE)
				     ? integer_one_node
				     : chksize));
  DECL_CONTEXT (t1_2b_1) = current_function_decl;
  bind_decls = chainon (bind_decls, t1_2b_1);
  bind_exprs = chainon (bind_exprs, build_stmt (DECL_STMT, t1_2b_1));

  /* struct __mf_cache * const __mf_elem = [...] */
  t1_3_1 = build_decl (VAR_DECL, get_identifier ("__mf_elem"),
                       mf_cache_structptr_type);
  DECL_ARTIFICIAL (t1_3_1) = 1;
  DECL_CONTEXT (t1_3_1) = current_function_decl;
  bind_decls = chainon (bind_decls, t1_3_1);
  bind_exprs = chainon (bind_exprs, build_stmt (DECL_STMT, t1_3_1));

  /* & __mf_lookup_cache [(((uintptr_t)__mf_base) >> __mf_shift) & __mf_mask] */
  {
    tree t0, t1, t2, t3;

    t0 = build (RSHIFT_EXPR, mf_uintptr_type,
		convert (mf_uintptr_type, t1_2a_1),
		(flag_mudflap > 1 ? mf_cache_shift_decl : mf_cache_shift_decl_l));

    t1 = build (BIT_AND_EXPR, mf_uintptr_type, t0,
		(flag_mudflap > 1 ? mf_cache_mask_decl : mf_cache_mask_decl_l));

    t2 = mf_mark (build (ARRAY_REF,
			 TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (mf_cache_array_decl))),
			 mf_cache_array_decl,
			 t1));

    t3 = mf_mark (build1 (ADDR_EXPR, mf_cache_structptr_type, t2));

    bind_exprs = chainon (bind_exprs,
			  build_stmt (EXPR_STMT,
				      build (INIT_EXPR, mf_cache_structptr_type,
					     t1_3_1, t3)));
  }

  /* Quick validity check.
     if (__builtin_expect ((__mf_elem->low > __mf_base)
                           | (__mf_elem_high < __mf_base + __mf_size - 1),
			   0))
	{
	  __mf_check ();
          ... and only if single-threaded:
	  __mf_lookup_shift_1 = f...;
	  __mf_lookup_mask_l = ...;
	}  */
  {
    tree t0, t1, t2;

    /* __mf_elem->low  */
    t0 = mf_mark (build (COMPONENT_REF, mf_uintptr_type,
			 mf_mark (build1 (INDIRECT_REF, mf_cache_struct_type,
			                  t1_3_1)),
			 TYPE_FIELDS (mf_cache_struct_type)));

    /* __mf_elem->high  */
    t1 = mf_mark (build (COMPONENT_REF, mf_uintptr_type,
			 mf_mark (build1 (INDIRECT_REF, mf_cache_struct_type,
			                  t1_3_1)),
		         TREE_CHAIN (TYPE_FIELDS (mf_cache_struct_type))));

    /* __mf_elem + sizeof (T) - 1  */
    t2 = build (PLUS_EXPR, mf_uintptr_type,
		t1_2a_1,
		fold (build (MINUS_EXPR, mf_uintptr_type,
		             t1_2b_1,
			     integer_one_node)));

    t1_4_1 = build (TRUTH_OR_EXPR, integer_type_node,
		    build (GT_EXPR, integer_type_node, t0, t1_2a_1),
		    build (LT_EXPR, integer_type_node, t1, t2));

    /* Mark condition as UNLIKELY using __builtin_expect.  */
    t1_4_1 = build_function_call (built_in_decls[BUILT_IN_EXPECT],
				  tree_cons (NULL_TREE,
					     convert (long_integer_type_node,
					              t1_4_1),
					     tree_cons (NULL_TREE,
						        integer_zero_node,
						        NULL_TREE)));
  }

  /* Build up the body of the cache-miss handling:
     __mf_check(); refresh *_l vars.  */
  {
    tree t0, t1;

    t1_4_2 = NULL_TREE;
    t0 = build_tree_list (NULL_TREE, convert (ptr_type_node, t1_2a_1));
    t0 = chainon (t0, build_tree_list (NULL_TREE,
	                               convert (size_type_node, t1_2b_1)));
    t0 = chainon (t0, build_tree_list (NULL_TREE, acctype));
    t0 = chainon (t0, build_tree_list (NULL_TREE, location_string));
    t1_4_2 = chainon (t1_4_2,
	              build_stmt (EXPR_STMT,
				  build_function_call (mf_check_fndecl, t0)));

    if (! (flag_mudflap > 1))
      {
	t1_4_2 = chainon (t1_4_2,
			  build_stmt (EXPR_STMT,
				      build (MODIFY_EXPR,
					     TREE_TYPE (mf_cache_shift_decl_l),
					     mf_cache_shift_decl_l,
					     mf_cache_shift_decl)));

	t1_4_2 = chainon (t1_4_2,
			  build_stmt (EXPR_STMT,
				      build (MODIFY_EXPR,
					     TREE_TYPE (mf_cache_mask_decl_l),
					     mf_cache_mask_decl_l,
					     mf_cache_mask_decl)));
      }

    t0 = build_stmt (SCOPE_STMT, NULL_TREE);
    SCOPE_BEGIN_P (t0) = 1;

    t1 = build_stmt (SCOPE_STMT, NULL_TREE);
    SCOPE_BEGIN_P (t1) = 0;

    t0 = chainon (t0, t1_4_2);
    t0 = chainon (t0, t1);
    t1_4_2 = build_stmt (COMPOUND_STMT, t0);
  }

  bind_exprs = chainon (bind_exprs,
		        build_stmt (IF_STMT, t1_4_1, t1_4_2, NULL_TREE));

  /* "return" __mf_value, or provided finale.  */
  bind_exprs = chainon (bind_exprs, build_stmt (EXPR_STMT, t1_2_1));

  /* Build the body of the statement-expression ({ decls; exprs; })  */
  {
    tree t0, t1, block;

    block = build (BLOCK, void_type_node, NULL_TREE);
    BLOCK_VARS (block) = bind_decls;

    t0 = build_stmt (SCOPE_STMT, block);
    SCOPE_BEGIN_P (t0) = 1;

    t1 = build_stmt (SCOPE_STMT, block);
    SCOPE_BEGIN_P (t1) = 0;

    t0 = chainon (t0, bind_exprs);
    t0 = chainon (t0, t1);
    t0 = build_stmt (COMPOUND_STMT, t0);
    t0 = build1 (STMT_EXPR, myptrtype, t0);
    TREE_SIDE_EFFECTS (t0) = 1;

    return t0;
  }
}



/* ------------------------------------------------------------------------ */
/* INDIRECT_REF transform */

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

   (2) (ARRAY_REF ({ARRAY_REF ... (tree, indexM)}, indexN))  //  tree[N][M][O]...
       ==> (as if)
       (INDIRECT_REF (tree + (.... + sizeM*sizeO*indexO + sizeM*indexM + indexN))
       ... except the base value for the check is &tree[0], not &tree[N][M][O]

   (3) (COMPONENT_REF (INDIRECT_REF (tree), field))
       ==> (as if)
       (COMPONENT_REF (INDIRECT_REF (tree), field))
       ... except the size value for the check is offsetof(field)+sizeof(field)-1

   (4) (BIT_FIELD_REF (INDIRECT_REF (tree), bitsize, bitpos))
       ==> (as if)
       (BIT_FIELD_REF (INDIRECT_REF (tree), bitsize, bitpos))
       ... except the size value for the check is to include byte @ bitsize+bitpos
*/


/* for lvalue/rvalue tracking */
static GTY (()) varray_type tree_roles;
static int tree_roles_init;

static tree
mx_xfn_indirect_ref (tree *t, int *continue_p, void *data)
{
  static const char *last_filename = NULL;
  static int last_lineno = -1;
  htab_t verboten = (htab_t) data;
  tree tree_role;

  /* Track lvalue/rvalue status for this subtree.  */
  if (! tree_roles_init)
    {
      VARRAY_TREE_INIT (tree_roles, 10, "tree lvalue/rvalue role stack");
      tree_roles_init = 1;
    }

  if (VARRAY_ACTIVE_SIZE (tree_roles) > 0)
    tree_role = VARRAY_TREE (tree_roles, 0);
  else
    tree_role = NULL_TREE;

  *continue_p = 1;

  /* Track file-name/line-numbers.  */
  if (STATEMENT_CODE_P (TREE_CODE (*t)))
    last_lineno = (STMT_LINENO (*t) > 0 ? STMT_LINENO (*t) : last_lineno);
  if (TREE_CODE (*t) == FILE_STMT)
    last_filename = FILE_STMT_FILENAME (*t);
  /* Try also the LOCUS, but ignore a simply referenced decl.  */
  if (! DECL_P (*t) && EXPR_LOCUS (*t)) 
    {
      last_filename = EXPR_FILENAME (*t);
      last_lineno = (EXPR_LINENO (*t) > 0 ? EXPR_LINENO (*t) : last_lineno);
    }

  /* Avoid traversal into subtrees specifically listed as
     do-not-traverse.  This occurs for certain nested operator/array
     expressions.  */
  if (htab_find (verboten, *t) != NULL)
    {
      *continue_p = 0;
      return NULL_TREE;
    }

  /* Avoid infinite recursion of transforming instrumented or
     instrumentation code.  NB: This check is done second, in case the
     same node is marked verboten as well as mudflapped.  The former
     takes priority, and is meant to prevent further traversal.  */
  if (mf_marked_p (*t))
    return NULL_TREE;

  /* Process some node types.  */
  switch (TREE_CODE (*t))
    {
      /* lvalue / rvalue detection.  */
    case MODIFY_EXPR:
    case INIT_EXPR:
      {
	/* We recurse through the operands manually, so that we can
	   communicate to them a general lvalue-vs-rvalue indication.  */

	VARRAY_PUSH_TREE (tree_roles, integer_one_node); /* __MF_CHECK_WRITE */
	walk_tree_without_duplicates (& TREE_OPERAND (*t, 0),
				      mx_xfn_indirect_ref, (void*) verboten);
	VARRAY_POP (tree_roles);
	VARRAY_PUSH_TREE (tree_roles, integer_zero_node); /* __MF_CHECK_READ */
	walk_tree_without_duplicates (& TREE_OPERAND (*t, 1),
				      mx_xfn_indirect_ref, (void*) verboten);
	VARRAY_POP (tree_roles);

	* continue_p = 0;
      }
      break;

    default:
      ; /* Continue traversal.  */
      break;

      /* Catch unfolded "& ptr->field" constructs that come from the
	 C++ front-end. */
    case ADDR_EXPR:
      {
	tree arg = TREE_OPERAND (*t, 0);
	if (TREE_CODE (arg) == COMPONENT_REF
	    && TREE_CODE (TREE_OPERAND (arg, 0)) == INDIRECT_REF)
	  {
	    mf_mark (TREE_OPERAND (arg, 0));
	    mf_mark (arg);
	    mf_mark (*t);
	  }
      }
      break;

    case ARRAY_REF:
      {
	tree base_array, base_obj_type, base_ptr_type;
	tree offset_expr;
	tree value_ptr, check_ptr, check_size;
	tree check_decls = NULL_TREE;

	offset_expr = mf_offset_expr_of_array_ref (TREE_OPERAND (*t,0),
						   & TREE_OPERAND (*t,1),
						   & base_array,
						   & check_decls);
	check_decls = nreverse (check_decls); /* XXX: evaluation order?  */

	/* We now have a tree representing the array in base_array,
	   and a tree representing the complete desired offset in
	   offset_expr. */

	base_obj_type = TREE_TYPE (TREE_TYPE (TREE_OPERAND (*t,0)));
	base_ptr_type = build_pointer_type (base_obj_type);

	/* Maybe we have an expression like ptr->array[4].  In this
	   case, we don't want to check (*ptr), since ptr+sizeof(*ptr)
	   might be well beyond &(ptr->array[4]).  */
	if ((TREE_CODE (base_array) == COMPONENT_REF) &&
	    (TREE_CODE (TREE_OPERAND (base_array, 0)) == INDIRECT_REF))
	  {
	    /* This is similar to the COMPONENT_REF(INDIRECT_REF(...))
	       code below. */

	    tree field = TREE_OPERAND (base_array, 1);
	    tree field_offset = byte_position (field);

	    check_ptr = TREE_OPERAND (TREE_OPERAND (base_array, 0), 0); /* ptr */
	    check_size = fold (build (PLUS_EXPR, size_type_node,
				      field_offset,
				      fold (build (MULT_EXPR,
						   integer_type_node,
						   TYPE_SIZE_UNIT (base_obj_type),
						   fold (build (PLUS_EXPR, size_type_node,
								integer_one_node,
								offset_expr))))));

	    mf_mark (TREE_OPERAND (base_array, 0));
	    mf_mark (base_array);
	  }
	else /* default: possibly a VAR_DECL */
	  {
	    check_ptr = mf_mark (build1 (ADDR_EXPR,
					 base_ptr_type,
					 mf_mark (build (ARRAY_REF,
							 base_obj_type,
							 base_array,
							 integer_zero_node))));

	    check_size = fold (build (MULT_EXPR,
				      integer_type_node,
				      TYPE_SIZE_UNIT (base_obj_type),
				      fold (build (PLUS_EXPR, size_type_node,
						   integer_one_node,
						   offset_expr))));
	  }

	value_ptr = mf_mark (build1 (ADDR_EXPR,
				     base_ptr_type,
				     mf_mark (*t)));

	/* As an optimization, omit checking if the base object is
	   known to be large enough.  Only certain kinds of
	   declarations and indexes/sizes are trustworthy.  */
	if (TREE_CODE (check_size) == INTEGER_CST /* constant offset */
	    && TREE_CODE (base_array) == VAR_DECL /* not a PARM_DECL */
	    && ! DECL_EXTERNAL (base_array) /* has known size */
	    && TREE_CODE (TREE_TYPE (base_array)) == ARRAY_TYPE /* an array */
	    && ! TREE_ADDRESSABLE (base_array) /* ought not to be __mf_registered */
	    && ((size_t) int_size_in_bytes (TREE_TYPE (base_array)) >= (size_t) TREE_INT_CST_LOW (check_size)
	    && TREE_INT_CST_HIGH (check_size) == 0)) /* offset within bounds */
	  {
	    if (check_decls != NULL_TREE) abort();
	    break;
	  }
	else
	  {
	    tree tmp;

	    /* In case we're instrumenting an expression like a[b[c]], the
	       following call is meant to eliminate the
	       redundant/recursive check of the outer size=b[c] check. */
	    * (htab_find_slot (verboten, check_size, INSERT)) = check_size;
	    * (htab_find_slot (verboten, check_ptr, INSERT)) = check_ptr;

	    tmp = mf_build_check_statement_for (value_ptr, check_ptr, check_size,
						tree_role, check_decls,
						last_filename, last_lineno);
	    *t = mf_mark (build1 (INDIRECT_REF, base_obj_type, tmp));
	  }
      }
      break;

    case ARRAY_RANGE_REF:
      /* not yet implemented */
      warning ("mudflap checking not yet implemented for ARRAY_RANGE_REF");
      break;

    case INDIRECT_REF:
      /* Substitute check statement for ptrvalue in INDIRECT_REF.  */
      TREE_OPERAND (*t, 0) =
	mf_build_check_statement_for (TREE_OPERAND (*t, 0),
				      TREE_OPERAND (*t, 0),
				      TYPE_SIZE_UNIT (TREE_TYPE
						      (TREE_TYPE
						       (TREE_OPERAND (*t, 0)))),
				      tree_role, NULL_TREE,
				      last_filename, last_lineno);
	/* Prevent this transform's reapplication to this tree node.
	   Note that we do not prevent recursion in walk_tree toward
	   subtrees of this node, in case of nested pointer expressions.  */
      mf_mark (*t);
      break;

    case COMPONENT_REF:
      if (TREE_CODE (TREE_OPERAND (*t, 0)) == INDIRECT_REF)
	{
	  tree *pointer = & TREE_OPERAND (TREE_OPERAND (*t, 0), 0);

	  tree field = TREE_OPERAND (*t, 1);
	  tree field_offset = byte_position (field);
	  tree field_size =
	    DECL_BIT_FIELD_TYPE (field) ?
	    size_binop (TRUNC_DIV_EXPR,
			size_binop (PLUS_EXPR,
				    DECL_SIZE (field), /* bitfield width */
				    convert (bitsizetype,
					     build_int_2 (BITS_PER_UNIT - 1, 0))),
			convert (bitsizetype, build_int_2 (BITS_PER_UNIT, 0)))
	    : size_in_bytes (TREE_TYPE (TREE_OPERAND (*t, 1)));
	  tree check_size = fold (build (PLUS_EXPR, size_type_node,
					 field_offset, field_size));

	  *pointer =
	    mf_build_check_statement_for (*pointer, *pointer, check_size,
					  tree_role, NULL_TREE,
					  last_filename, last_lineno);

	  /* Don't instrument the nested INDIRECT_REF. */
	  mf_mark (TREE_OPERAND (*t, 0));
	  mf_mark (*t);
	}
      break;

    case BIT_FIELD_REF:
      if (TREE_CODE (TREE_OPERAND (*t, 0)) == INDIRECT_REF)
	{
	  tree *pointer = & TREE_OPERAND (TREE_OPERAND (*t, 0), 0);

	  tree bitsize = TREE_OPERAND (*t, 1);
	  tree bitpos = TREE_OPERAND (*t, 2);
	  tree check_size =
	    fold (build (TRUNC_DIV_EXPR, size_type_node,
			 fold (build (PLUS_EXPR, size_type_node,
				      bitsize,
				      fold (build (PLUS_EXPR, size_type_node,
						   bitpos,
						   build_int_2 (BITS_PER_UNIT - 1, 0))))),
			 build_int_2 (BITS_PER_UNIT, 0)));

	  *pointer =
	    mf_build_check_statement_for (*pointer, *pointer, check_size,
					  tree_role, NULL_TREE,
					  last_filename, last_lineno);

	  /* Don't instrument the nested INDIRECT_REF. */
	  mf_mark (TREE_OPERAND (*t, 0));
	  mf_mark (*t);
	}
      break;
    }

  return NULL_TREE;
}



static void
mf_xform_derefs (tree fnbody)
{
  htab_t verboten = htab_create (31, htab_hash_pointer, htab_eq_pointer, NULL);
  walk_tree_without_duplicates (& fnbody, mx_xfn_indirect_ref, (void*) verboten);
  htab_delete (verboten);
}



/* ------------------------------------------------------------------------ */
/* ADDR_EXPR transform */


/* This struct is passed between mf_xform_decls to store state needed
   during the traversal searching for objects that have their
   addresses taken. */
struct mf_xform_decls_data
{
  tree param_decls;
};


/* Synthesize a CALL_EXPR and a TRY_FINALLY_EXPR, for this chain of
   _DECLs if appropriate.  Arrange to call the __mf_register function
   now, and the __mf_unregister function later for each.  */
static void
mx_register_decls (tree decl, tree *compound_expr)
{
  tree finally_stmts = NULL_TREE;
  tree initially_stmts = NULL_TREE;

  while (decl != NULL_TREE)
    {
      /* Eligible decl?  */
      if ((TREE_CODE (decl) == VAR_DECL || TREE_CODE (decl) == PARM_DECL) &&
	  (! TREE_STATIC (decl)) && /* auto variable */
	  (! DECL_EXTERNAL (decl)) && /* not extern variable */
	  (COMPLETE_OR_VOID_TYPE_P (TREE_TYPE (decl))) && /* complete type */
	  (! mf_marked_p (decl)) && /* not already processed */
	  (TREE_ADDRESSABLE (decl))) /* has address taken */
	{
	  /* (& VARIABLE, sizeof (VARIABLE)) */
	  tree unregister_fncall_params =
	    tree_cons (NULL_TREE,
		       convert (ptr_type_node,
				mf_mark (build1 (ADDR_EXPR,
						 build_pointer_type (TREE_TYPE (decl)),
						 decl))),
		       tree_cons (NULL_TREE,
				  convert (size_type_node,
					   TYPE_SIZE_UNIT (TREE_TYPE (decl))),
				  NULL_TREE));
	  /* __mf_unregister (...) */
	  tree unregister_fncall =
	    build_function_call (mf_unregister_fndecl,
				 unregister_fncall_params);

	  /* (& VARIABLE, sizeof (VARIABLE), __MF_TYPE_STACK) */
	  tree variable_name = mf_varname_tree (decl);
	  tree register_fncall_params =
	    tree_cons (NULL_TREE,
		   convert (ptr_type_node,
			    mf_mark (build1 (ADDR_EXPR,
					     build_pointer_type (TREE_TYPE (decl)),
					     decl))),
		       tree_cons (NULL_TREE,
				  convert (size_type_node,
					   TYPE_SIZE_UNIT (TREE_TYPE (decl))),
				  tree_cons (NULL_TREE,
					     build_int_2 (3, 0), /* __MF_TYPE_STACK */
					     tree_cons (NULL_TREE,
							variable_name,
							NULL_TREE))));

	  /* __mf_register (...) */
	  tree register_fncall =
	    build_function_call (mf_register_fndecl,
				 register_fncall_params);

	  /* Accumulate the two calls.  */
	  append_to_statement_list (register_fncall, &initially_stmts);
	  append_to_statement_list (unregister_fncall, &finally_stmts);

	  mf_mark (decl);
	}

      decl = TREE_CHAIN (decl);
    }

  /* Now process the pending register/unregister calls, if any.  */
  if (initially_stmts != NULL_TREE)
    {
      /* Prepend them to the existing expression parameter of the
	 enclosing BIND_EXPR.  */
      *compound_expr = build (COMPOUND_EXPR, TREE_TYPE (*compound_expr),
			      initially_stmts, *compound_expr);
      *compound_expr = rationalize_compound_expr (*compound_expr);
    }

  /* Actually, (initially_stmts!=NULL) <=> (finally_stmts!=NULL) */
  if (finally_stmts != NULL_TREE)
    {
      finally_stmts = rationalize_compound_expr (finally_stmts);
      *compound_expr = build (TRY_FINALLY_EXPR, TREE_TYPE (*compound_expr),
			      *compound_expr, finally_stmts);
    }
}


/* Process every variable mentioned in BIND_EXPRs.  */
static tree
mx_xfn_xform_decls (tree *t, int *continue_p, void *data)
{
  struct mf_xform_decls_data* d = (struct mf_xform_decls_data*) data;

  if (*t == NULL_TREE || *t == error_mark_node)
    {
      *continue_p = 0;
      return NULL_TREE;
    }

  *continue_p = 1;

  switch (TREE_CODE (*t))
    {
    case BIND_EXPR:
      {
	/* Process function parameters now (but only once).  */
	mx_register_decls (d->param_decls, & BIND_EXPR_BODY (*t));
	d->param_decls = NULL_TREE;

	mx_register_decls (BIND_EXPR_VARS (*t), & BIND_EXPR_BODY (*t));
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
mf_xform_decls (tree fnbody, tree fnparams)
{
  struct mf_xform_decls_data d;
  d.param_decls = fnparams;
  walk_tree_without_duplicates (& fnbody, mx_xfn_xform_decls, & d);
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

static GTY (()) varray_type deferred_static_decls;
static GTY (()) varray_type deferred_static_decl_labels;
static int deferred_static_decls_init;

/* What I really want is a std::map<union tree_node,std::string> .. :-(  */

/* A chain of EXPR_STMTs for calling __mf_register() at initialization
   time.  */
static GTY (()) tree enqueued_call_stmt_chain;


void
mudflap_enqueue_decl (tree obj, const char *label)
{
  if (mf_marked_p (obj))
    return;

  /* We don't need to process variable decls that are internally
     generated extern.  If we did, we'd end up with warnings for them
     during mudflap_finish_file ().  That would confuse the user,
     since the text would refer to variables that don't show up in the
     user's source code.
  */
  if (DECL_P (obj) && DECL_EXTERNAL (obj) && DECL_ARTIFICIAL (obj))
    {
      return;
    }

  if (COMPLETE_TYPE_P (TREE_TYPE (obj)))
    {
      FILE *dump_file;
      int dump_flags;
      tree object_size;
      tree call_stmt;

      mf_mark (obj);

      object_size = size_in_bytes (TREE_TYPE (obj));

      dump_file = dump_begin (TDI_mudflap, &dump_flags);
      if (dump_file)
	{
	  fprintf (dump_file, "enqueue_decl obj=`");
	  print_generic_expr (dump_file, obj, 0);
	  fprintf (dump_file, "' label=`%s' size=", label);
	  print_generic_expr (dump_file, object_size, 0);
	  fprintf (dump_file, "\n");
	  dump_end (TDI_mudflap, dump_file);
	}

      /* NB: the above condition doesn't require TREE_USED or
         TREE_ADDRESSABLE.  That's because this object may be a global
         only used from other compilation units.  XXX: Maybe static
         objects could require those attributes being set.  */
      call_stmt =
	mflang_register_call (label,
			      object_size,
			      build_int_2 (4, 0), /* __MF_TYPE_STATIC */
			      mf_varname_tree (obj));

      /* Link this call into the chain. */
      TREE_CHAIN (call_stmt) = enqueued_call_stmt_chain;
      enqueued_call_stmt_chain = call_stmt;
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
      for (i=0; i < VARRAY_ACTIVE_SIZE (deferred_static_decls); i++)
	if (VARRAY_TREE (deferred_static_decls, i) == obj)
	  found_p = 1;

      if (found_p)
	warning ("mudflap cannot track lifetime of `%s'",
		 IDENTIFIER_POINTER (DECL_NAME (obj)));
      else
	{
	  VARRAY_PUSH_TREE (deferred_static_decls, obj);
	  VARRAY_PUSH_CHAR_PTR (deferred_static_decl_labels, (char *) label);
	}
    }
}


void
mudflap_enqueue_constant (tree obj, const char *label)
{
  tree call_stmt;
  tree object_size;

  if (mf_marked_p (obj))
    return;

  object_size = (TREE_CODE (obj) == STRING_CST)
    ? build_int_2 (TREE_STRING_LENGTH (obj), 0)
    : size_in_bytes (TREE_TYPE (obj));

  {
    FILE *dump_file;
    int dump_flags;

    dump_file = dump_begin (TDI_mudflap, &dump_flags);
    if (dump_file)
      {
	fprintf (dump_file, "enqueue_constant obj=`");
	print_generic_expr (dump_file, obj, 0);
	fprintf (dump_file, "' label=`%s' size=", label);
	print_generic_expr (dump_file, object_size, 0);
	fprintf (dump_file, "\n");
	dump_end (TDI_mudflap, dump_file);
      }
  }

  call_stmt =
    (TREE_CODE (obj) == STRING_CST)
    ? mflang_register_call (label, object_size,
			    build_int_2 (4, 0), /* __MF_TYPE_STATIC */
			    mf_mark (fix_string_type
				     (build_string (15, "string literal"))))
    : mflang_register_call (label, object_size,
			    build_int_2 (4, 0), /* __MF_TYPE_STATIC */
			    mf_mark (fix_string_type
				     (build_string (9, "constant"))));

  /* Link this call into the chain. */
  TREE_CHAIN (call_stmt) = enqueued_call_stmt_chain;
  enqueued_call_stmt_chain = call_stmt;
}



/* Emit any file-wide instrumentation.  */
void
mudflap_finish_file (void)
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

  mflang_flush_calls (enqueued_call_stmt_chain);
}



#include "gt-tree-mudflap.h"
