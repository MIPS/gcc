/* Perform optimizations on tree structure.
   Copyright (C) 1998, 1999, 2000, 2001 Free Software Foundation, Inc.
   Written by Mark Michell (mark@codesourcery.com).

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "tree.h"
#include "cp-tree.h"
#include "rtl.h"
#include "insn-config.h"
#include "input.h"
#include "integrate.h"
#include "toplev.h"
#include "varray.h"
#include "ggc.h"
#include "params.h"
#include "hashtab.h"
#include "debug.h"

/* To Do:

   o In order to make inlining-on-trees work, we pessimized
     function-local static constants.  In particular, they are now
     always output, even when not addressed.  Fix this by treating
     function-local static constants just like global static
     constants; the back-end already knows not to output them if they
     are not needed.

   o Provide heuristics to clamp inlining of recursive template
     calls?  */

typedef struct call_site_data
{
  tree *site_ptr;		/* Pointer containing the CALL_EXPR
				   ptr */
  tree callee;			/* Callee. */
  tree target_expr;             /* Containing TARGET_EXPR. */
  unsigned in_target_cleanup_p;	/* We're inside a TARGET_EXPR
				   cleanup. */
  HOST_WIDE_INT usage;		/* Use estimate. */
} call_site_data;

/* Data required for function inlining.  */

typedef struct inline_data
{
  /* A stack of the functions we are inlining. This is used to detect
     and inhibit recursive inlining. The first few entries (below
     first_fn), are functions that are not yet complete (this happens
     for nested functions). The function we are inlining is
     VARRAY_TOP_TREE.  */
  varray_type fns;
  /* The index of the first element of FNS that really represents an
     inlined function.  */
  unsigned first_fn;
  /* The function we are inlining into. */
  tree fn;
  /* Array of pointers to CALL_SITE_DATA for CALL_EXPRs that can be
     inlined. */
  varray_type inlinable_call_sites;
  /* The label to jump to when a return statement is encountered.  If
     this value is NULL, then return statements will simply be
     remapped as return statements, rather than as jumps.  */
  tree ret_label;
  /* The map from local declarations in the inlined function to
     equivalents in the function into which it is being inlined.  */
  splay_tree decl_map;
  /* Nonzero if we are currently within the cleanup for a
     TARGET_EXPR.  */
  int in_target_cleanup_p;
  /* A stack of the TARGET_EXPRs that we are currently processing.  */
  varray_type target_exprs;
  /* We use the same mechanism to build clones that we do to perform
     inlining.  However, there are a few places where we need to
     distinguish between those two situations.  This flag is true if
     we are cloning, rather than inlining.  */
  bool cloning_p;
  /* Hash table used to prevent walk_tree from visiting the same node
     umpteen million times.  */
  htab_t tree_pruner;
  /* Relative estimate of number of times we reach this point.

     XXX We should divide this by (say) N when doing branches of an
     N-way if or switch, and multiply by (say) 10 when doing the body
     of a loop. The tree walker is not sophisticated enough to acheive
     that yet. Best would be to extract real profile data.  */
  HOST_WIDE_INT usage;
} inline_data;

/* Prototypes.  */

static tree initialize_inlined_parameters PARAMS ((inline_data *, tree, tree));
static tree declare_return_variable PARAMS ((inline_data *, tree *));
static tree copy_body_r PARAMS ((tree *, int *, void *));
static tree copy_body PARAMS ((inline_data *));
static tree remap_decl PARAMS ((tree, inline_data *));
static void remap_block PARAMS ((tree, tree, inline_data *));
static void copy_scope_stmt PARAMS ((tree *, int *, inline_data *));
static void expand_call_inline PARAMS ((call_site_data *, inline_data *));
static bool inlinable_size_p PARAMS ((tree));
static tree find_inlinable_call_sites PARAMS ((tree *, int *, void *));
static int compare_inlinable_call_sites PARAMS ((const void *, const void *));
static int optimize_inline_calls PARAMS ((tree, inline_data *));
static void optimize_function_real PARAMS ((tree, inline_data *));
static tree calls_setjmp_r PARAMS ((tree *, int *, void *));
static void update_cloned_parm PARAMS ((tree, tree));
static FILE *dump_function PARAMS ((enum tree_dump_index, tree, int *));

/* Remap DECL during the copying of the BLOCK tree for the function.  */

static tree
remap_decl (decl, id)
     tree decl;
     inline_data *id;
{
  splay_tree_node n;
  tree fn;

  /* We only remap local variables in the current function.  */
  fn = VARRAY_TOP_TREE (id->fns);
  if (!nonstatic_local_decl_p (decl) || DECL_CONTEXT (decl) != fn)
    return NULL_TREE;

  /* See if we have remapped this declaration.  */
  n = splay_tree_lookup (id->decl_map, (splay_tree_key) decl);
  /* If we didn't already have an equivalent for this declaration,
     create one now.  */
  if (!n)
    {
      tree t;

      /* Make a copy of the variable or label.  */
      t = copy_decl_for_inlining (decl, fn, id->fn);

      /* The decl T could be a dynamic array or other variable size type,
	 in which case some fields need to be remapped because they may
	 contain SAVE_EXPRs. We don't need to deal with DECL_SIZE and
	 DECL_SIZE_UNIT here, as walk_tree will do that for us because
	 the first location of this _DECL will be in a
	 DECL_STMT. Walking twice is wrong.  We have to do the type,
	 as that won't be covered by walk_tree.  */
      if (TREE_TYPE (t) && TREE_CODE (TREE_TYPE (t)) == ARRAY_TYPE
	  && TYPE_DOMAIN (TREE_TYPE (t))
	  && !TREE_CONSTANT (TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (t)))))
	{
	  TREE_TYPE (t) = copy_node (TREE_TYPE (t));
	  TYPE_DOMAIN (TREE_TYPE (t))
	    = copy_node (TYPE_DOMAIN (TREE_TYPE (t)));
	  walk_tree (&TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (t))),
		     copy_body_r, id, id->tree_pruner);
	}

      if (!DECL_NAME (t) && TREE_TYPE (t)
	  && ANON_AGGR_TYPE_P (TREE_TYPE ((t))))
	{
	  /* For a VAR_DECL of anonymous type, we must also copy the
	     member VAR_DECLS here and rechain the
	     DECL_ANON_UNION_ELEMS. */
	  tree members = NULL;
	  tree src;
	  
	  for (src = DECL_ANON_UNION_ELEMS (t); src;
	       src = TREE_CHAIN (src))
	    {
	      tree member = remap_decl (TREE_VALUE (src), id);

	      my_friendly_assert (!TREE_PURPOSE (src), 20010529);
	      members = tree_cons (NULL, member, members);
	    }
	  DECL_ANON_UNION_ELEMS (t) = nreverse (members);
	}
      
      /* Remember it, so that if we encounter this local entity
	 again we can reuse this copy.  */
      n = splay_tree_insert (id->decl_map,
			     (splay_tree_key) decl,
			     (splay_tree_value) t);
    }

  return (tree) n->value;
}

/* Copy the SCOPE_STMT_BLOCK associated with SCOPE_STMT to contain
   remapped versions of the variables therein.  And hook the new block
   into the block-tree.  If non-NULL, the DECLS are declarations to
   add to use instead of the BLOCK_VARS in the old block.  */

static void
remap_block (scope_stmt, decls, id)
     tree scope_stmt;
     tree decls;
     inline_data *id;
{
  /* We cannot do this in the cleanup for a TARGET_EXPR since we do
     not know whether or not expand_expr will actually write out the
     code we put there.  If it does not, then we'll have more BLOCKs
     than block-notes, and things will go awry.  At some point, we
     should make the back-end handle BLOCK notes in a tidier way,
     without requiring a strict correspondence to the block-tree; then
     this check can go.  */
  if (id->in_target_cleanup_p)
    {
      SCOPE_STMT_BLOCK (scope_stmt) = NULL_TREE;
      return;
    }

  /* If this is the beginning of a scope, remap the associated BLOCK.  */
  if (SCOPE_BEGIN_P (scope_stmt) && SCOPE_STMT_BLOCK (scope_stmt))
    {
      tree old_block;
      tree new_block;
      tree old_var;

      /* Make the new block.  */
      old_block = SCOPE_STMT_BLOCK (scope_stmt);
      new_block = make_node (BLOCK);
      TREE_USED (new_block) = TREE_USED (old_block);
      BLOCK_ABSTRACT_ORIGIN (new_block) = old_block;
      SCOPE_STMT_BLOCK (scope_stmt) = new_block;

      /* Remap its variables.  */
      for (old_var = decls ? decls : BLOCK_VARS (old_block);
	   old_var;
	   old_var = TREE_CHAIN (old_var))
	{
	  tree new_var;

	  /* Remap the variable.  */
	  new_var = remap_decl (old_var, id);
	  /* If we didn't remap this variable, so we can't mess with
	     its TREE_CHAIN.  If we remapped this variable to
	     something other than a declaration (say, if we mapped it
	     to a constant), then we must similarly omit any mention
	     of it here.  */
	  if (!new_var || !DECL_P (new_var))
	    ;
	  else
	    {
	      TREE_CHAIN (new_var) = BLOCK_VARS (new_block);
	      BLOCK_VARS (new_block) = new_var;
	    }
	}
      /* We put the BLOCK_VARS in reverse order; fix that now.  */
      BLOCK_VARS (new_block) = nreverse (BLOCK_VARS (new_block));
      if (id->cloning_p)
	/* We're building a clone; DECL_INITIAL is still
	   error_mark_node, and current_binding_level is the parm
	   binding level.  */
	insert_block (new_block);
      else
	{
	  /* Attach this new block after the DECL_INITIAL block for the
	     function into which this block is being inlined.  In
	     rest_of_compilation we will straighten out the BLOCK tree.  */
	  tree *first_block;
	  if (DECL_INITIAL (id->fn))
	    first_block = &BLOCK_CHAIN (DECL_INITIAL (id->fn));
	  else
	    first_block = &DECL_INITIAL (id->fn);
	  BLOCK_CHAIN (new_block) = *first_block;
	  *first_block = new_block;
	}
      /* Remember the remapped block.  */
      splay_tree_insert (id->decl_map,
			 (splay_tree_key) old_block,
			 (splay_tree_value) new_block);
    }
  /* If this is the end of a scope, set the SCOPE_STMT_BLOCK to be the
     remapped block.  */
  else if (SCOPE_END_P (scope_stmt) && SCOPE_STMT_BLOCK (scope_stmt))
    {
      splay_tree_node n;

      /* Find this block in the table of remapped things.  */
      n = splay_tree_lookup (id->decl_map,
			     (splay_tree_key) SCOPE_STMT_BLOCK (scope_stmt));
      my_friendly_assert (n != NULL, 19991203);
      SCOPE_STMT_BLOCK (scope_stmt) = (tree) n->value;
    }
}

/* Copy the SCOPE_STMT pointed to by TP.  */

static void
copy_scope_stmt (tp, walk_subtrees, id)
     tree *tp;
     int *walk_subtrees;
     inline_data *id;
{
  tree block;

  /* Remember whether or not this statement was nullified.  When
     making a copy, copy_tree_r always sets SCOPE_NULLIFIED_P (and
     doesn't copy the SCOPE_STMT_BLOCK) to free callers from having to
     deal with copying BLOCKs if they do not wish to do so.  */
  block = SCOPE_STMT_BLOCK (*tp);
  /* Copy (and replace) the statement.  */
  copy_tree_r (tp, walk_subtrees, NULL);
  /* Restore the SCOPE_STMT_BLOCK.  */
  SCOPE_STMT_BLOCK (*tp) = block;

  /* Remap the associated block.  */
  remap_block (*tp, NULL_TREE, id);
}

/* Called from copy_body via walk_tree.  DATA is really an
   `inline_data *'.  */

static tree
copy_body_r (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees;
     void *data;
{
  inline_data* id;
  tree fn;

  /* Set up.  */
  id = (inline_data *) data;
  fn = VARRAY_TOP_TREE (id->fns);

  /* All automatic variables should have a DECL_CONTEXT indicating
     what function they come from.  */
  if ((TREE_CODE (*tp) == VAR_DECL || TREE_CODE (*tp) == LABEL_DECL)
      && DECL_NAMESPACE_SCOPE_P (*tp))
    my_friendly_assert (DECL_EXTERNAL (*tp) || TREE_STATIC (*tp),
			19991113);

  /* If this is a RETURN_STMT, change it into an EXPR_STMT and a
     GOTO_STMT with the RET_LABEL as its target.  */
  if (TREE_CODE (*tp) == RETURN_STMT && id->ret_label)
    {
      tree return_stmt = *tp;
      tree goto_stmt;

      /* Build the GOTO_STMT.  */
      goto_stmt = build_stmt (GOTO_STMT, id->ret_label);
      TREE_CHAIN (goto_stmt) = TREE_CHAIN (return_stmt);

      /* If we're returning something, just turn that into an
	 assignment into the equivalent of the original
	 RESULT_DECL.  */
      if (RETURN_EXPR (return_stmt))
	{
	  *tp = build_stmt (EXPR_STMT,
			    RETURN_EXPR (return_stmt));
	  STMT_IS_FULL_EXPR_P (*tp) = 1;
	  /* And then jump to the end of the function.  */
	  TREE_CHAIN (*tp) = goto_stmt;
	}
      /* If we're not returning anything just do the jump.  */
      else
	*tp = goto_stmt;
    }
  /* Local variables and labels need to be replaced by equivalent
     variables.  We don't want to copy static variables; there's only
     one of those, no matter how many times we inline the containing
     function.  */
  else if (nonstatic_local_decl_p (*tp))
    {
      if (*tp != id->ret_label)
	{
	  tree new_decl;

	  my_friendly_assert (DECL_CONTEXT (*tp) == fn, 20010702);
      
	  /* Remap the declaration.  */
	  new_decl = remap_decl (*tp, id);
	  my_friendly_assert (new_decl != NULL_TREE, 19991203);
          /* Replace this variable with the copy.  */
	  STRIP_TYPE_NOPS (new_decl);
	  *tp = new_decl;
	  /* The new decl might be an expression - don't walk it. */
	  *walk_subtrees = 0;
	}
    }
  else if (TREE_CODE (*tp) == SAVE_EXPR)
    remap_save_expr (tp, id->decl_map, id->fn, walk_subtrees);
  else if (TREE_CODE (*tp) == UNSAVE_EXPR)
    /* UNSAVE_EXPRs should not be generated until expansion time.  */
    my_friendly_abort (19991113);
  /* For a SCOPE_STMT, we must copy the associated block so that we
     can write out debugging information for the inlined variables.  */
  else if (TREE_CODE (*tp) == SCOPE_STMT && !id->in_target_cleanup_p)
    copy_scope_stmt (tp, walk_subtrees, id);
  /* Otherwise, just copy the node.  Note that copy_tree_r already
     knows not to copy VAR_DECLs, etc., so this is safe.  */
  else
    {
      copy_tree_r (tp, walk_subtrees, NULL);

      /* The copied TARGET_EXPR has never been expanded, even if the
	 original node was expanded already.  */
      if (TREE_CODE (*tp) == TARGET_EXPR && TREE_OPERAND (*tp, 3))
	{
	  TREE_OPERAND (*tp, 1) = TREE_OPERAND (*tp, 3);
	  TREE_OPERAND (*tp, 3) = NULL_TREE;
	}
      else if (TREE_CODE (*tp) == MODIFY_EXPR
	       && TREE_OPERAND (*tp, 0) == TREE_OPERAND (*tp, 1)
	       && nonstatic_local_decl_p (TREE_OPERAND (*tp, 0))
	       && DECL_CONTEXT (TREE_OPERAND (*tp, 0)) == fn)
	{
	  /* Some assignments VAR = VAR; don't generate any rtl code
	     and thus don't count as variable modification.  Avoid
	     keeping bogosities like 0 = 0.  */
	  tree decl = TREE_OPERAND (*tp, 0), value;
	  splay_tree_node n;

	  n = splay_tree_lookup (id->decl_map, (splay_tree_key) decl);
	  if (n)
	    {
	      value = (tree) n->value;
	      STRIP_TYPE_NOPS (value);
	      if (TREE_CONSTANT (value) || TREE_READONLY_DECL_P (value))
		*tp = value;
	    }
	}
    }

  /* Keep iterating.  */
  return NULL_TREE;
}

/* Make a copy of the body of FN so that it can be inserted inline in
   another function.  */

static tree
copy_body (id)
     inline_data *id;
{
  tree body;

  body = DECL_SAVED_TREE (VARRAY_TOP_TREE (id->fns));
  walk_tree (&body, copy_body_r, id, NULL);

  return body;
}

/* ARGS is a TREE_LIST of argument expressions for a call to FN.
   We must use the expression trees within ARGS directly.
   Generate code to initialize the parameters of the function at the
   top of the stack in ID from the ARGS (presented as a TREE_LIST).  */

static tree
initialize_inlined_parameters (id, args, fn)
     inline_data *id;
     tree args;
     tree fn;
{
  tree init_stmts;
  tree parms;
  tree a;
  tree p;

  /* Figure out what the parameters are.  */
  parms = DECL_ARGUMENTS (fn);

  /* Start with no initializations whatsoever.  */
  init_stmts = NULL_TREE;

  /* Loop through the parameter declarations, replacing each with an
     equivalent VAR_DECL, appropriately initialized.  */
  for (p = parms, a = args; p; a = TREE_CHAIN (a), p = TREE_CHAIN (p))
    {
      tree init_stmt;
      tree var;
      tree value;

      /* Find the initializer.  */
      value = TREE_VALUE (a);
      /* If the parameter is never assigned to, we may not need to
	 create a new variable here at all.  Instead, we may be able
	 to just use the argument value.  */
      if (TREE_READONLY (p)
	  && !TREE_ADDRESSABLE (p)
	  && !TREE_SIDE_EFFECTS (value))
	{
	  my_friendly_assert (TREE_CODE (value) != CALL_EXPR, 20010702);
	  
	  /* We can't risk substituting complex expressions.  They
	     might contain variables that will be assigned to later.
	     Theoretically, we could check the expression to see if
	     all of the variables that determine its value are
	     read-only, but we don't bother.  */
	  if (TREE_CONSTANT (value) || TREE_READONLY_DECL_P (value))
	    {
	      /* If this is a _DECL, wrap it a NOP_EXPR so that
		 we don't try to put the VALUE on the list of
		 BLOCK_VARS.  */
	      if (DECL_P (value))
		value = build1 (NOP_EXPR, TREE_TYPE (value), value);

	      splay_tree_insert (id->decl_map,
				 (splay_tree_key) p,
				 (splay_tree_value) value);
	      continue;
	    }
	}

      /* Make an equivalent VAR_DECL.  */
      var = copy_decl_for_inlining (p, fn, id->fn);
      /* Register the VAR_DECL as the equivalent for the PARM_DECL;
	 that way, when the PARM_DECL is encountered, it will be
	 automatically replaced by the VAR_DECL.  */
      splay_tree_insert (id->decl_map,
			 (splay_tree_key) p,
			 (splay_tree_value) var);

      /* Declare this new variable.  */
      init_stmt = build_stmt (DECL_STMT, var);
      TREE_CHAIN (init_stmt) = init_stmts;
      init_stmts = init_stmt;

      /* Initialize this VAR_DECL from the equivalent argument.  If
	 the argument is an object, created via a constructor or copy,
	 this will not result in an extra copy: the TARGET_EXPR
	 representing the argument will be bound to VAR, and the
	 object will be constructed in VAR.  */
      if (! TYPE_NEEDS_CONSTRUCTING (TREE_TYPE (p)))
	DECL_INITIAL (var) = value;
      else
	{
	  /* Even if P was TREE_READONLY, the new VAR should not be.
	     In the original code, we would have constructed a
	     temporary, and then the function body would have never
	     changed the value of P.  However, now, we will be
	     constructing VAR directly.  The constructor body may
	     change its value multiple times as it is being
	     constructed.  Therefore, it must not be TREE_READONLY;
	     the back-end assumes that TREE_READONLY variable is
	     assigned to only once.  */
	  TREE_READONLY (var) = 0;

	  /* Build a run-time initialization.  */
	  init_stmt = build_stmt (EXPR_STMT,
				  build (INIT_EXPR, TREE_TYPE (p),
					 var, value));
	  /* Add this initialization to the list.  Note that we want the
	     declaration *after* the initialization because we are going
	     to reverse all the initialization statements below.  */
	  TREE_CHAIN (init_stmt) = init_stmts;
	  init_stmts = init_stmt;
	}
    }

  /* The initialization statements have been built up in reverse
     order.  Straighten them out now.  */
  return nreverse (init_stmts);
}

/* Declare a return variable to replace the RESULT_DECL for the
   function we are calling.  An appropriate DECL_STMT is returned.
   The USE_STMT is filled in to contain a use of the declaration to
   indicate the return value of the function.  */

static tree
declare_return_variable (id, use_stmt)
     struct inline_data *id;
     tree *use_stmt;
{
  tree fn = VARRAY_TOP_TREE (id->fns);
  tree result = DECL_RESULT (fn);
  tree var;
  int aggregate_return_p;

  /* We don't need to do anything for functions that don't return
     anything.  */
  if (!result || VOID_TYPE_P (TREE_TYPE (result)))
    {
      *use_stmt = NULL_TREE;
      return NULL_TREE;
    }

  /* Figure out whether or not FN returns an aggregate.  */
  aggregate_return_p = IS_AGGR_TYPE (TREE_TYPE (result));

  /* If FN returns an aggregate then the caller will always create the
     temporary (using a TARGET_EXPR) and the call will be the
     initializing expression for the TARGET_EXPR.  If we were just to
     create a new VAR_DECL here, then the result of this function
     would be copied (bitwise) into the variable initialized by the
     TARGET_EXPR.  That's incorrect, so we must transform any
     references to the RESULT into references to the target.  */
  if (aggregate_return_p)
    {
      my_friendly_assert (VARRAY_ACTIVE_SIZE (id->target_exprs) != 0,
			  20000430);
      var = TREE_OPERAND (VARRAY_TOP_TREE (id->target_exprs), 0);
      my_friendly_assert
	(same_type_ignoring_top_level_qualifiers_p (TREE_TYPE (var),
						    TREE_TYPE (result)),
	 20000430);
    }
  /* Otherwise, make an appropriate copy.  */
  else
    var = copy_decl_for_inlining (result, fn, id->fn);

  /* Register the VAR_DECL as the equivalent for the RESULT_DECL; that
     way, when the RESULT_DECL is encountered, it will be
     automatically replaced by the VAR_DECL.  */
  splay_tree_insert (id->decl_map,
		     (splay_tree_key) result,
		     (splay_tree_value) var);

  /* Build the USE_STMT.  */
  *use_stmt = build_stmt (EXPR_STMT, var);

  /* Build the declaration statement if FN does not return an
     aggregate.  */
  if (!aggregate_return_p)
    return build_stmt (DECL_STMT, var);
  /* If FN does return an aggregate, there's no need to declare the
     return variable; we're using a variable in our caller's frame.  */
  else
    return NULL_TREE;
}

/* CALL_SITE describes an inlinable function call in the body of
   ID->FN. Expand that function call inline. Because of the way
   function calling is expanded, we must be careful not to lose the
   argument exprs as they may contain as yet unexpanded function
   calls. */

static void
expand_call_inline (call_site, id)
     call_site_data *call_site;
     inline_data *id;
{
  tree t = *call_site->site_ptr;
  tree fn = call_site->callee;
  tree expr;
  tree chain;
  tree scope_stmt;
  tree use_stmt;
  tree arg_inits;
  tree *inlined_body;
  splay_tree st;

  my_friendly_assert (TREE_CODE (t) == CALL_EXPR, 20010702);
  id->fn = VARRAY_TOP_TREE (id->fns);
  VARRAY_PUSH_TREE (id->fns, fn);
  
  id->in_target_cleanup_p = call_site->in_target_cleanup_p;
  if (call_site->target_expr)
    VARRAY_PUSH_TREE (id->target_exprs, call_site->target_expr);
  
  /* Set the current filename and line number to the function we are
     inlining so that when we create new _STMT nodes here they get
     line numbers corresponding to the function we are calling.  We
     wrap the whole inlined body in an EXPR_WITH_FILE_AND_LINE as well
     because individual statements don't record the filename.  */
  push_srcloc (fn->decl.filename, fn->decl.linenum);

  /* Build a statement-expression containing code to initialize the
     arguments, the actual inline expansion of the body, and a label
     for the return statements within the function to jump to.  The
     type of the statement expression is the return type of the
     function call.  */
  expr = build_min (STMT_EXPR, TREE_TYPE (TREE_TYPE (fn)), NULL_TREE);

  /* Local declarations will be replaced by their equivalents in this
     map.  */
  st = id->decl_map;
  id->decl_map = splay_tree_new (splay_tree_compare_pointers,
				 NULL, NULL);

  /* Initialize the parameters.  */
  arg_inits = initialize_inlined_parameters (id, TREE_OPERAND (t, 1), fn);
  /* And add them to the tree.  */
  STMT_EXPR_STMT (expr) = chainon (STMT_EXPR_STMT (expr), arg_inits);

  /* Return statements in the function body will be replaced by jumps
     to the RET_LABEL.  */
  id->ret_label = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
  DECL_CONTEXT (id->ret_label) = id->fn;

  /* Create a block to put the parameters in.  We have to do this
     after the parameters have been remapped because remapping
     parameters is different from remapping ordinary variables.  */
  scope_stmt = build_stmt (SCOPE_STMT, DECL_INITIAL (fn));
  SCOPE_BEGIN_P (scope_stmt) = 1;
  SCOPE_NO_CLEANUPS_P (scope_stmt) = 1;
  remap_block (scope_stmt, DECL_ARGUMENTS (fn), id);
  TREE_CHAIN (scope_stmt) = STMT_EXPR_STMT (expr);
  STMT_EXPR_STMT (expr) = scope_stmt;

  /* Tell the debugging backends that this block represents the
     outermost scope of the inlined function.  */
  if (SCOPE_STMT_BLOCK (scope_stmt))
    BLOCK_ABSTRACT_ORIGIN (SCOPE_STMT_BLOCK (scope_stmt)) = DECL_ORIGIN (fn);

  /* Declare the return variable for the function.  */
  STMT_EXPR_STMT (expr)
    = chainon (STMT_EXPR_STMT (expr),
	       declare_return_variable (id, &use_stmt));

  /* After we've initialized the parameters, we insert the body of the
     function itself.  */
  inlined_body = &STMT_EXPR_STMT (expr);
  while (*inlined_body)
    inlined_body = &TREE_CHAIN (*inlined_body);
  *inlined_body = copy_body (id);

  /* Close the block for the parameters.  */
  scope_stmt = build_stmt (SCOPE_STMT, DECL_INITIAL (fn));
  SCOPE_NO_CLEANUPS_P (scope_stmt) = 1;
  my_friendly_assert (DECL_INITIAL (fn)
		      && TREE_CODE (DECL_INITIAL (fn)) == BLOCK,
		      19991203);
  remap_block (scope_stmt, NULL_TREE, id);
  STMT_EXPR_STMT (expr)
    = chainon (STMT_EXPR_STMT (expr), scope_stmt);

  /* After the body of the function comes the RET_LABEL.  This must come
     before we evaluate the returned value below, because that evalulation
     may cause RTL to be generated.  */
  STMT_EXPR_STMT (expr)
    = chainon (STMT_EXPR_STMT (expr),
	       build_stmt (LABEL_STMT, id->ret_label));

  /* Finally, mention the returned value so that the value of the
     statement-expression is the returned value of the function.  */
  STMT_EXPR_STMT (expr) = chainon (STMT_EXPR_STMT (expr), use_stmt);

  /* Clean up.  */
  splay_tree_delete (id->decl_map);
  id->decl_map = st;

  /* The new expression has side-effects if the old one did.  */
  TREE_SIDE_EFFECTS (expr) = TREE_SIDE_EFFECTS (t);

  /* Replace the call by the inlined body.  Wrap it in an
     EXPR_WITH_FILE_LOCATION so that we'll get debugging line notes
     pointing to the right place.  */
  chain = TREE_CHAIN (*call_site->site_ptr);
  *call_site->site_ptr = build_expr_wfl (expr, DECL_SOURCE_FILE (fn),
					 DECL_SOURCE_LINE (fn), /*col=*/0);
  EXPR_WFL_EMIT_LINE_NOTE (*call_site->site_ptr) = 1;
  TREE_CHAIN (*call_site->site_ptr) = chain;
  pop_srcloc ();

  /* If the value of the new expression is ignored, that's OK.  We
     don't warn about this for CALL_EXPRs, so we shouldn't warn about
     the equivalent inlined version either.  */
  TREE_USED (*call_site->site_ptr) = 1;

  id->in_target_cleanup_p = 0;
  if (call_site->target_expr)
    VARRAY_POP (id->target_exprs);
  VARRAY_POP (id->fns);
  id->fn = NULL_TREE;
}

/* Return true if FN is inlineable considering its size. */

static bool
inlinable_size_p (fn)
     tree fn;
{
  return DECL_NUM_STMTS (fn)
    < (PARAM_VALUE (PARAM_MAX_INLINE_AST)
       + (PARAM_VALUE (PARAM_ARG_INLINE_AST)
	  * type_num_arguments (TREE_TYPE (fn))));
}

/* Called via walk_trees, DATA is an inline_data. If *TP is a call to
   a potentially inlinable function, remember it. Also deal with tracking
   TARGET_EXPRs.  */

static tree
find_inlinable_call_sites (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees;
     void *data;
{
  inline_data *id = (inline_data *) data;
  tree t = *tp;
  tree fn;
  call_site_data *call_site;
  
  if (TREE_CODE (*tp) == TARGET_EXPR)
    {
      /* Recurse, but letting recursive invocations know that we are
     	 inside the body of a TARGET_EXPR.  */
      int i, len = first_rtl_op (TARGET_EXPR);

      /* Push T on the stack of pending TARGET_EXPRs.  */
      VARRAY_PUSH_TREE (id->target_exprs, t);

      /* Actually walk over them.  This loop is the body of
	 walk_trees, omitting the case where the TARGET_EXPR
	 itself is handled.  */
      for (i = 0; i < len; ++i)
	{
	  if (i == 2)
	    ++id->in_target_cleanup_p;
	  walk_tree (&TREE_OPERAND (t, i), find_inlinable_call_sites, data,
		     id->tree_pruner);
	  if (i == 2)
	    --id->in_target_cleanup_p;
	}

      /* We're done with this TARGET_EXPR now.  */
      VARRAY_POP (id->target_exprs);

      /* We walked our own subtrees. */
      *walk_subtrees = 0;

      return NULL_TREE;
    }

  if (TYPE_P (t))
    /* Because types were not copied in copy_body, CALL_EXPRs beneath
       them should not be expanded.  This can happen if the type is a
       dynamic array type, for example.  */
    *walk_subtrees = 0;

  /* From here on, we're only interested in CALL_EXPRs.  */
  if (TREE_CODE (t) != CALL_EXPR)
    return NULL_TREE;

  /* First, see if we can figure out what function is being called.
     If we cannot, then there is no hope of inlining the function.  */
  fn = get_callee_fndecl (t);
  if (!fn)
    return NULL_TREE;

  if (DECL_UNINLINABLE (fn))
    return NULL_TREE;
  
  if (!DECL_INLINE (fn))
    /* Not an inline function. */
    return NULL_TREE;

  /* We can inline a template instantiation only if it's fully
     instantiated.  */
  if (DECL_TEMPLATE_INFO (fn)
      && TI_PENDING_TEMPLATE_FLAG (DECL_TEMPLATE_INFO (fn)))
    {
      fn = instantiate_decl (fn, /*defer_ok=*/0);
      if (TI_PENDING_TEMPLATE_FLAG (DECL_TEMPLATE_INFO (fn)))
	return NULL_TREE;
    }

  /* If we don't have the function body available, we can't inline
     it.  */
  if (!DECL_SAVED_TREE (fn))
    return NULL_TREE;

  /* We can't check yet if the function cannot be inlined due to size,
     because it might not have been optimized. That can make it
     smaller in some cases. */
  
  /* Remember this call site. */
  call_site = xmalloc (sizeof (call_site_data));
  call_site->site_ptr = tp;
  call_site->callee = fn;
  call_site->in_target_cleanup_p = id->in_target_cleanup_p;
  call_site->target_expr = (VARRAY_ACTIVE_SIZE (id->target_exprs)
			    ? VARRAY_TOP_TREE (id->target_exprs) : NULL_TREE);
  call_site->usage = id->usage;

  VARRAY_PUSH_GENERIC_PTR (id->inlinable_call_sites, call_site);
  
  return NULL_TREE;
}

/* Called via qsort. A_ and B_ are pointers into an array of to
   call_site_data pointers.  We want the largest, most unused function
   at the bottom and the smallest most used function at the top.
   Return <0 if A_ better than B_, >0 if B_ better than A_. If they
   are equal first order by function size then order by function. That
   way equally weighted calls to the same function will be
   consecutive.

   We treat the usage information as a dynamic count to weight the
   function's statement count. If the dynamic count has a large error
   margin, we want to reduce its impact by moving it towards its
   initial value. I.e. usage' = usage_init * (usage/usage_init)^(1/N),
   where N gets bigger the more uncertain the estimate is.  */

static int
compare_inlinable_call_sites (a_, b_)
     const void *a_;
     const void *b_;
{
  call_site_data *a = *(call_site_data **)a_;
  call_site_data *b = *(call_site_data **)b_;
  HOST_WIDE_INT ua = DECL_NUM_STMTS (a->callee) * b->usage;
  HOST_WIDE_INT ub = DECL_NUM_STMTS (b->callee) * a->usage;

  /* Order by dynamic size. */
  if (ua - ub)
    return ua - ub;
  
  /* Ordered by static size. */
  ua = DECL_NUM_STMTS (a->callee);
  ub = DECL_NUM_STMTS (b->callee);
  if (ua - ub)
    return ua - ub;

  /* Ordered (arbitrarily) by uid, don't use the address as that is
     essentially random, the UID indicates something about source ordering. */
  ua = DECL_UID (a->callee);
  ub = DECL_UID (b->callee);
  return ua - ub;
}

/* Expand calls to inline functions in the body of FN.  We do a bottom
   up inlining, the idea being that expanding already optimized
   functions is better than top down inlining of unoptimized
   functions.  This is more memory intensive, as we have the expanded
   bodies of inlinable functions lying about. The approach taken is to
   walk the body of FN, collecting the inlineable function call
   sites. We optimize each inlinable function - so we have a good
   estimate of their sizes. That optimization will do inlining into
   the body of those functions. We have to protect that inlining from
   recursion -- see the check at the start of
   optimize_function_real. We sort the inlinable call sites by size &
   frequency.  Then we perform inlining from smallest to largest,
   stopping when we're about to exceed the inline statement limit.
   Returns a count of call sites we inlined.  */

static int
optimize_inline_calls (fn, id)
     tree fn;
     inline_data *id;
{
  unsigned first_inlinable = VARRAY_ACTIVE_SIZE (id->inlinable_call_sites);
  unsigned ix;
  int flags;
  int count = 0;
  FILE *stream;
  HOST_WIDE_INT orig_stmts, stmts;
  
  VARRAY_PUSH_TREE (id->fns, fn);
  
  /* Locate all the inlinable call sites. */
  id->usage = 65536;
  id->tree_pruner = htab_create (37, htab_hash_pointer,
	 			 htab_eq_pointer, NULL);
  walk_tree (&DECL_SAVED_TREE (fn), &find_inlinable_call_sites,
	     id, id->tree_pruner);
  htab_delete (id->tree_pruner);
  id->tree_pruner = 0;

  /* Make sure each is optimized. */
  for (ix = first_inlinable;
       ix != VARRAY_ACTIVE_SIZE (id->inlinable_call_sites);
       ix++)
    {
      call_site_data *call_site =
	(call_site_data *)VARRAY_GENERIC_PTR (id->inlinable_call_sites, ix);
      
      optimize_function_real (call_site->callee, id);
    }
  if (DECL_OPTIMIZED_P (fn))
    /* We recursively optimized ourselves.  This happens when we
       mutually recurse via some function that is larger than us.  */
    goto cleanup;

  /* Sort them from best to worst. */
  if (first_inlinable != VARRAY_ACTIVE_SIZE (id->inlinable_call_sites))
    qsort (&VARRAY_GENERIC_PTR (id->inlinable_call_sites, first_inlinable),
	   VARRAY_ACTIVE_SIZE (id->inlinable_call_sites) - first_inlinable,
	   sizeof (PTR), &compare_inlinable_call_sites);

  count = 0;
  stmts = orig_stmts = DECL_NUM_STMTS (fn);
  
  /* Now do the actual inlining.  */
  for (ix = first_inlinable;
       ix != VARRAY_ACTIVE_SIZE (id->inlinable_call_sites);
       ix++)
    {
      call_site_data *call_site = (call_site_data *) VARRAY_GENERIC_PTR
					(id->inlinable_call_sites, ix);
      
      if (!DECL_OPTIMIZED_P (call_site->callee)
	  || DECL_UNINLINABLE (call_site->callee))
	/* We failed to optimize this, or we discovered that it is
	   uninlinable. */
	continue;

      if (!inlinable_size_p (call_site->callee))
	break;
      
      stmts += DECL_NUM_STMTS (call_site->callee);
      expand_call_inline (call_site, id);
      count++;
    }
      
  stream = dump_function (TDI_inlined, fn, &flags);
  if (stream)
    {
      fprintf (stream, "\n");
      fprintf (stream, ";; expanded from "
	       HOST_WIDE_INT_PRINT_DEC " to "
	       HOST_WIDE_INT_PRINT_DEC " statements\n",
	       DECL_NUM_STMTS (fn), stmts);
      
      if (!(flags & TDF_SLIM))
	{
	  unsigned jx;
      
	  for (jx = first_inlinable;
	       jx != VARRAY_ACTIVE_SIZE (id->inlinable_call_sites);
	       jx++)
	    {
	      call_site_data *call_site = (call_site_data *) VARRAY_GENERIC_PTR
						(id->inlinable_call_sites, jx);
	      const char *reason = NULL;
	      
	      if (jx >= ix)
		reason = "too big";
	      else if (!DECL_OPTIMIZED_P (call_site->callee))
		reason = "not optimized";
	      else if (DECL_UNINLINABLE (call_site->callee))
		reason = "not inlinable";
	  
	      fprintf (stream, ";; %sinlined %s ", reason ? "not " : "",
		       decl_as_string (call_site->callee,
				       TFF_DECL_SPECIFIERS));
	      fprintf (stream, "(%s) " HOST_WIDE_INT_PRINT_DEC,
		       decl_as_string
		       		(DECL_ASSEMBLER_NAME (call_site->callee), 0),
		       DECL_NUM_STMTS (call_site->callee));
	      if (reason)
		fprintf (stream , " %s", reason);
	      fprintf (stream, "\n");
	    }
	}
      dump_end (TDI_inlined, stream);
    }
  DECL_NUM_STMTS (fn) = stmts;
  
cleanup:;
  while (VARRAY_ACTIVE_SIZE (id->inlinable_call_sites) != first_inlinable)
    {
      free (VARRAY_TOP_GENERIC_PTR (id->inlinable_call_sites));
      VARRAY_POP (id->inlinable_call_sites);
    }
  
  VARRAY_POP (id->fns);

  return count;
}

/* Optimize the body of FN. The function stack in ID is used to detect
   inlining recursion. When we encounter that, we skip the
   optimization of the largest function in the loop.  */

static void
optimize_function_real (fn, id)
     tree fn;
     inline_data *id;
{
  unsigned ix;
  HOST_WIDE_INT biggest = 0;
  
  my_friendly_assert ((!DECL_MAYBE_IN_CHARGE_CONSTRUCTOR_P (fn)
		       && !DECL_MAYBE_IN_CHARGE_DESTRUCTOR_P (fn))
		      || DECL_CLONED_FUNCTION_P (fn), 20010702);

  if (DECL_OPTIMIZED_P (fn))
    /* We've already done this function. */
    return;
  
  for (ix = VARRAY_ACTIVE_SIZE (id->fns); ix--;)
    {
      tree nested_fn = VARRAY_TREE (id->fns, ix);
      
      if (biggest < DECL_NUM_STMTS (nested_fn))
	biggest = DECL_NUM_STMTS (nested_fn);
      if (fn == nested_fn)
	{
	  if (ix < id->first_fn)
	    /* This is an incomplete function. */
	    return;
	  if (biggest == DECL_NUM_STMTS (fn))
	    /* We have mutual recursion, and this is the biggest
       	       function in the recursive loop. This stands the least
       	       chance of being inlined within the loop, so break the
       	       recursion here. */
	    return;

	  /* We _do_ want to optimize now. */
	  break;
	}
    }

  /* While in this function, we may choose to go off and compile
     another function.  For example, we might instantiate a function
     in the hopes of inlining it.  Normally, that wouldn't trigger any
     actual RTL code-generation -- but it will if the template is
     actually needed.  (For example, if it's address is taken, or if
     some other function already refers to the template.)  If
     code-generation occurs, then garbage collection will occur, so we
     must protect ourselves, just as we do while building up the body
     of the function.  */
  ++function_depth;

  
  if (!DECL_OPTIMIZED_PARTIAL_P (fn))
    {
      dump_function (TDI_original, fn, NULL);
      
      /* XXX Do preliminary optimization here, like constant
	 propagation, unreachable code elimination, return value
	 optimization, etc. */

      /* We can't inline varargs functions. (Well actually we could if
     	 they never refer to the varadic arguments. We could have
     	 checked for that in the preceding optimizations.)  */
      if (varargs_function_p (fn))
	DECL_UNINLINABLE (fn) = 1;
  
      DECL_OPTIMIZED_PARTIAL_P (fn) = 1;
    }

  if (VARRAY_ACTIVE_SIZE (id->fns) > id->first_fn)
    {
      /* We're optimizing this function because we want to inline it
         into the function at the top of id->fns.  Check our current
         size does not prevent that.  If it does, we abort the
         optimization now, because it is unlikely that this function
         will get smaller when inlining into its body. Of course,
         we'll do inline expansion in this function when we come to
         really emit it. */
      if (!inlinable_size_p (fn))
	goto cleanup;
    }

  
  if (flag_inline_trees
      /* We do not inline thunks, as (a) the backend tries to optimize
         the call to the thunkee, (b) tree based inlining breaks that
         optimization, (c) virtual functions are rarely inlineable,
         and (d) ASM_OUTPUT_MI_THUNK is there to DTRT anyway.  */
      && !DECL_THUNK_P (fn)
      && optimize_inline_calls (fn, id))
    {
      /* We managed to inline something. */
      
      /* XXX Repeat some optimizations, like constant propagation,
	 unreachable code elimination, etc. */
    }

  if (!inlinable_size_p (fn))
    DECL_UNINLINABLE (fn) = 1;
  
  DECL_OPTIMIZED_P (fn) = 1;
  dump_function (TDI_optimized, fn, NULL);
  
cleanup:
  --function_depth;
}

/* Optimize the body of FN. This will cause the optimization of any
   inlinable calls made in FN (recursively).  */

void
optimize_function (fn)
     tree fn;
{
  inline_data id;
  tree prev_fn;
  struct saved_scope *s;
  
  /* Clear out ID.  */
  memset (&id, 0, sizeof (id));
  VARRAY_TREE_INIT (id.fns, 32, "fns");
  VARRAY_TREE_INIT (id.target_exprs, 32, "target_exprs");

  /* Don't allow optimization of functions that aren't finished yet.  */
  prev_fn = NULL_TREE;
  if (current_function_decl && current_function_decl != fn)
    {
      VARRAY_PUSH_TREE (id.fns, current_function_decl);
      prev_fn = current_function_decl;
    }
  for (s = scope_chain; s; s = s->prev)
    if (s->function_decl && s->function_decl != prev_fn)
      {
	VARRAY_PUSH_TREE (id.fns, s->function_decl);
	prev_fn = s->function_decl;
      }
  /* Keep track of the low-water mark, i.e., the point where the first
     real inlining is represented in ID.FNS.  */
  id.first_fn = VARRAY_ACTIVE_SIZE (id.fns);

  /* Create the list of call sites we can inline inside this function.  */
  VARRAY_GENERIC_PTR_INIT (id.inlinable_call_sites, 32,
			   "inlinable_call_sites");

  optimize_function_real (fn, &id);
  
  VARRAY_FREE (id.target_exprs);
  VARRAY_FREE (id.inlinable_call_sites);
  VARRAY_FREE (id.fns);
}

/* Called from calls_setjmp_p via walk_tree.  */

static tree
calls_setjmp_r (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees ATTRIBUTE_UNUSED;
     void *data ATTRIBUTE_UNUSED;
{
  /* We're only interested in FUNCTION_DECLS.  */
  if (TREE_CODE (*tp) != FUNCTION_DECL)
    return NULL_TREE;

  return setjmp_call_p (*tp) ? *tp : NULL_TREE;
}

/* Returns non-zero if FN calls `setjmp' or some other function that
   can return more than once.  This function is conservative; it may
   occasionally return a non-zero value even when FN does not actually
   call `setjmp'.  */

int
calls_setjmp_p (fn)
     tree fn;
{
  return walk_tree_without_duplicates (&DECL_SAVED_TREE (fn),
				       calls_setjmp_r,
				       NULL) != NULL_TREE;
}

/* CLONED_PARM is a copy of CLONE, generated for a cloned constructor
   or destructor.  Update it to ensure that the source-position for
   the cloned parameter matches that for the original, and that the
   debugging generation code will be able to find the original PARM.  */

static void
update_cloned_parm (parm, cloned_parm)
     tree parm;
     tree cloned_parm;
{
  DECL_ABSTRACT_ORIGIN (cloned_parm) = parm;

  /* We may have taken its address. */
  TREE_ADDRESSABLE (cloned_parm) = TREE_ADDRESSABLE (parm);

  /* The definition might have different constness. */
  TREE_READONLY (cloned_parm) = TREE_READONLY (parm);
  
  TREE_USED (cloned_parm) = TREE_USED (parm);
  
  /* The name may have changed from the declaration. */
  DECL_NAME (cloned_parm) = DECL_NAME (parm);
  DECL_SOURCE_FILE (cloned_parm) = DECL_SOURCE_FILE (parm);
  DECL_SOURCE_LINE (cloned_parm) = DECL_SOURCE_LINE (parm);
}

/* FN is a function that has a complete body.  Clone the body as
   necessary.  Returns non-zero if there's no longer any need to
   process the main body.  */

int
maybe_clone_body (fn)
     tree fn;
{
  inline_data id;
  tree clone;
  int first = 1;

  /* We only clone constructors and destructors.  */
  if (!DECL_MAYBE_IN_CHARGE_CONSTRUCTOR_P (fn)
      && !DECL_MAYBE_IN_CHARGE_DESTRUCTOR_P (fn))
    return 0;

  /* Emit the DWARF1 abstract instance.  */
  (*debug_hooks->deferred_inline_function) (fn);

  /* We know that any clones immediately follow FN in the TYPE_METHODS
     list.  */
  for (clone = TREE_CHAIN (fn);
       clone && DECL_CLONED_FUNCTION_P (clone);
       clone = TREE_CHAIN (clone), first = 0)
    {
      tree parm;
      tree clone_parm;
      int parmno;

      /* Update CLONE's source position information to match FN's.  */
      DECL_SOURCE_FILE (clone) = DECL_SOURCE_FILE (fn);
      DECL_SOURCE_LINE (clone) = DECL_SOURCE_LINE (fn);
      DECL_INLINE (clone) = DECL_INLINE (fn);
      DECL_DECLARED_INLINE_P (clone) = DECL_DECLARED_INLINE_P (fn);
      DECL_COMDAT (clone) = DECL_COMDAT (fn);
      DECL_WEAK (clone) = DECL_WEAK (fn);
      DECL_ONE_ONLY (clone) = DECL_ONE_ONLY (fn);
      DECL_SECTION_NAME (clone) = DECL_SECTION_NAME (fn);
      DECL_USE_TEMPLATE (clone) = DECL_USE_TEMPLATE (fn);
      DECL_EXTERNAL (clone) = DECL_EXTERNAL (fn);
      DECL_INTERFACE_KNOWN (clone) = DECL_INTERFACE_KNOWN (fn);
      DECL_NOT_REALLY_EXTERN (clone) = DECL_NOT_REALLY_EXTERN (fn);
      TREE_PUBLIC (clone) = TREE_PUBLIC (fn);

      /* Adjust the parameter names and locations. */
      parm = DECL_ARGUMENTS (fn);
      clone_parm = DECL_ARGUMENTS (clone);
      /* Update the `this' parameter, which is always first.
	 Sometimes, we end update the `this' parameter twice because
	 we process it again in the loop below.  That is harmless.  */
      update_cloned_parm (parm, clone_parm);
      if (DECL_HAS_IN_CHARGE_PARM_P (fn))
	parm = TREE_CHAIN (parm);
      if (DECL_HAS_VTT_PARM_P (fn))
	parm = TREE_CHAIN (parm);
      if (DECL_HAS_VTT_PARM_P (clone))
	clone_parm = TREE_CHAIN (clone_parm);
      for (; parm;
	   parm = TREE_CHAIN (parm), clone_parm = TREE_CHAIN (clone_parm))
	{
	  /* Update this paramter.  */
	  update_cloned_parm (parm, clone_parm);
	  /* We should only give unused information for one clone. */
	  if (!first)
	    TREE_USED (clone_parm) = 1;
	}

      /* Start processing the function.  */
      push_to_top_level ();
      start_function (NULL_TREE, clone, NULL_TREE, SF_PRE_PARSED);

      /* Just clone the body, as if we were making an inline call.
	 But, remap the parameters in the callee to the parameters of
	 caller.  If there's an in-charge parameter, map it to an
	 appropriate constant.  */
      memset (&id, 0, sizeof (id));
      VARRAY_TREE_INIT (id.fns, 2, "fns");
      VARRAY_PUSH_TREE (id.fns, clone);
      id.fn = clone;
      VARRAY_PUSH_TREE (id.fns, fn);

      /* Cloning is treated slightly differently from inlining.  Set
	 CLONING_P so that its clear which operation we're performing.  */
      id.cloning_p = true;

      /* Remap the parameters.  */
      id.decl_map = splay_tree_new (splay_tree_compare_pointers,
				    NULL, NULL);
      for (parmno = 0,
	     parm = DECL_ARGUMENTS (fn),
	     clone_parm = DECL_ARGUMENTS (clone);
	   parm;
	   ++parmno,
	     parm = TREE_CHAIN (parm))
	{
	  /* Map the in-charge parameter to an appropriate constant.  */
	  if (DECL_HAS_IN_CHARGE_PARM_P (fn) && parmno == 1)
	    {
	      tree in_charge;
	      in_charge = in_charge_arg_for_name (DECL_NAME (clone));
	      splay_tree_insert (id.decl_map,
				 (splay_tree_key) parm,
				 (splay_tree_value) in_charge);
	    }
	  else if (DECL_ARTIFICIAL (parm)
		   && DECL_NAME (parm) == vtt_parm_identifier)
	    {
	      /* For a subobject constructor or destructor, the next
		 argument is the VTT parameter.  Remap the VTT_PARM
		 from the CLONE to this parameter.  */
	      if (DECL_HAS_VTT_PARM_P (clone))
		{
		  DECL_ABSTRACT_ORIGIN (clone_parm) = parm;
		  splay_tree_insert (id.decl_map,
				     (splay_tree_key) parm,
				     (splay_tree_value) clone_parm);
		  clone_parm = TREE_CHAIN (clone_parm);
		}
	      /* Otherwise, map the VTT parameter to `NULL'.  */
	      else
		{
		  splay_tree_insert (id.decl_map,
				     (splay_tree_key) parm,
				     (splay_tree_value) null_pointer_node);
		}
	    }
	  /* Map other parameters to their equivalents in the cloned
	     function.  */
	  else
	    {
	      splay_tree_insert (id.decl_map,
				 (splay_tree_key) parm,
				 (splay_tree_value) clone_parm);
	      clone_parm = TREE_CHAIN (clone_parm);
	    }
	}

      /* Actually copy the body.  */
      TREE_CHAIN (DECL_SAVED_TREE (clone)) = copy_body (&id);

      /* There are as many statements in the clone as in the
	 original.  */
      DECL_NUM_STMTS (clone) = DECL_NUM_STMTS (fn);

      /* Clean up.  */
      splay_tree_delete (id.decl_map);
      VARRAY_FREE (id.fns);

      /* Now, expand this function into RTL, if appropriate.  */
      finish_function (0);
      BLOCK_ABSTRACT_ORIGIN (DECL_INITIAL (clone)) = DECL_INITIAL (fn);
      expand_body (clone);
      pop_from_top_level ();
    }

  /* We don't need to process the original function any further.  */
  return 1;
}

/* Dump FUNCTION_DECL FN as tree dump PHASE.  If FLAG_PTR is non-NULL,
   then return any user supplied flags in there and return the open
   stream after dumping the function.  The caller is expected to
   append more information and then call dump_end. */

static FILE *
dump_function (phase, fn, flag_ptr)
     enum tree_dump_index phase;
     tree fn;
     int *flag_ptr;
{
  FILE *stream = NULL;
  int flags;

  stream = dump_begin (phase, &flags);
  if (stream)
    {
      fprintf (stream, "\n;; Function %s",
	       decl_as_string (fn, TFF_DECL_SPECIFIERS));
      fprintf (stream, " (%s)\n",
	       decl_as_string (DECL_ASSEMBLER_NAME (fn), 0));
      fprintf (stream, ";; enabled by -%s\n", dump_flag_name (phase));
      fprintf (stream, "\n");

      dump_node (fn, TDF_SLIM | flags, stream);
      if (flag_ptr)
	*flag_ptr = flags;
      else
	{
	  dump_end (phase, stream);
	  stream = NULL;
	}
    }
  return stream;
}
