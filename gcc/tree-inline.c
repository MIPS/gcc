/* Tree inlining.
   Copyright 2001, 2002, 2003, 2004 Free Software Foundation, Inc.
   Contributed by Alexandre Oliva <aoliva@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "toplev.h"
#include "tree.h"
#include "tree-inline.h"
#include "rtl.h"
#include "expr.h"
#include "flags.h"
#include "params.h"
#include "input.h"
#include "insn-config.h"
#include "integrate.h"
#include "varray.h"
#include "hashtab.h"
#include "pointer-set.h"
#include "splay-tree.h"
#include "langhooks.h"
#include "basic-block.h"
#include "tree-iterator.h"
#include "cgraph.h"
#include "intl.h"
#include "tree-mudflap.h"
#include "function.h"
#include "ggc.h"
#include "tree-flow.h"
#include "diagnostic.h"
#include "except.h"

/* I'm not real happy about this, but we need to handle gimple and
   non-gimple trees.  */
#include "tree-gimple.h"

/* Inlining, Saving, Cloning

   Inlining: a function body is duplicated, but the PARM_DECLs are
   remapped into VAR_DECLs, and non-void RETURN_EXPRs become
   MODIFY_EXPRs that store to a dedicated returned-value variable.
   The duplicated eh_region info of the copy will later be appended 
   to the info for the caller; the eh_region info in copied throwing 
   statements and RESX_EXPRs is adjusted accordingly.

   Saving: make a semantically-identical copy of the function body.
   Necessary when we want to generate code for the body (a destructive
   operation), but we expect to need this body in the future (e.g. for
   inlining into another function).

   Cloning: (only in C++) We have one body for a con/de/structor, and
   multiple function decls, each with a unique parameter list.
   Duplicate the body, using the given splay tree; some parameters
   will become constants (like 0 or 1).

   All of these will simultaneously lookup any callgraph edges.  If
   we're going to inline the duplicated function body, and the given
   function has some cloned callgraph nodes (one for each place this
   function will be inlined) those callgraph edges will be duplicated.
   If we're saving or cloning the body, those callgraph edges will be
   updated to point into the new body.  (Note that the original
   callgraph node and edge list will not be altered.)

   See the CALL_EXPR handling case in copy_body_r ().  */

/* 0 if we should not perform inlining.
   1 if we should expand functions calls inline at the tree level.
   2 if we should consider *all* functions to be inline
   candidates.  */

int flag_inline_trees = 0;

/* To Do:

   o In order to make inlining-on-trees work, we pessimized
     function-local static constants.  In particular, they are now
     always output, even when not addressed.  Fix this by treating
     function-local static constants just like global static
     constants; the back-end already knows not to output them if they
     are not needed.

   o Provide heuristics to clamp inlining of recursive template
     calls?  */

/* Data required for function inlining.  */

typedef struct inline_data
{
  /* FUNCTION_DECL for function being inlined.  */
  tree callee;
  /* FUNCTION_DECL for function being inlined into.  */
  tree caller;
  /* struct function for function being inlined.  Usually this is the same
     as DECL_STRUCT_FUNCTION (callee), but can be different if saved_cfg
     and saved_eh are in use.  */
  struct function *callee_cfun;
  /* The label to jump to when a return statement is encountered.  If
     this value is NULL, then return statements will simply be
     remapped as return statements, rather than as jumps.  */
  tree ret_label;
  /* The VAR_DECL for the return value.  */
  tree retvar;
  /* The number of RETURN_EXPRs encountered.  */
  unsigned int ret_count;
  /* The map from local declarations in the inlined function to
     equivalents in the function into which it is being inlined.  */
  splay_tree decl_map;
  /* We use the same mechanism to build clones that we do to perform
     inlining.  However, there are a few places where we need to
     distinguish between those two situations.  This flag is true if
     we are cloning, rather than inlining.  */
  bool cloning_p;
  /* Similarly for saving function body.  */
  bool saving_p;
  /* Callgraph node of function we are inlining into.  */
  struct cgraph_node *node;
  /* Callgraph node of currently inlined function.  */
  struct cgraph_node *current_node;
  /* Statement iterator.  We need this so we can keep the tree in
     gimple form when we insert the inlined function.   It is not
     used when we are not dealing with gimple trees.  */
  tree_stmt_iterator original_tsi;
  /* Iterator for walking over a copied function body.  When one
     statement is rewritten in place (e.g. when a RETURN_EXPR becomes
     a MODIFY followed by a GOTO), this iterator facilitates
     modifications on the copied function body.  */
  tree_stmt_iterator copy_tsi;
  /* Current BIND_EXPR.  */
  tree bind_expr;
  /* copy_body/copy_body_r use this to walk over the blocks of the
     original callee function body.  */
  basic_block copy_basic_block;
  /* optimize_inline_calls() uses this to walks through the basic
     blocks of the outermost caller function, discovering CALL_EXPRs
     and replacing them with callee bodies.  */
  basic_block oic_basic_block;
  /* Inlined RETURN_EXPRs become GOTOs to this basic_block.  */
  basic_block return_block;
} inline_data;

static GTY (()) varray_type eh_dst;

/* Prototypes.  */

static tree declare_return_variable (inline_data *, tree, tree, tree *);
static tree copy_body_r (tree *, int *, void *);
static tree copy_cfg_body (inline_data *, gcov_type, int);
static tree copy_generic_body (inline_data *);
static tree copy_body (inline_data *, gcov_type, int);
static bool expand_call_inline (tree *, int *, void *);
static bool gimple_expand_calls_inline (tree *, inline_data *);
static bool inlinable_function_p (tree);
static tree remap_decl (tree, inline_data *);
static tree remap_type (tree, inline_data *);
static tree initialize_inlined_parameters (inline_data *, tree,
					   tree, tree, tree);
static void remap_block (tree *, inline_data *);
static tree remap_decl (tree, inline_data *);
static tree remap_decls (tree, inline_data *);
static void copy_bind_expr (tree *, int *, inline_data *);
static tree mark_local_for_remap_r (tree *, int *, void *);
static void unsave_expr_1 (tree);
static tree unsave_r (tree *, int *, void *);
static void declare_inline_vars (tree bind_expr, tree vars);
static void add_lexical_block (tree, tree);
static void remap_save_expr (tree *, void *, int *);

/* Insert a tree->tree mapping for ID.  Despite the name suggests
   that the trees should be variables, it is used for more than that.  */

static void
insert_decl_map (inline_data *id, tree key, tree value)
{
  splay_tree_insert (id->decl_map, (splay_tree_key) key,
		     (splay_tree_value) value);

  /* Always insert an identity map as well.  If we see this same new
     node again, we won't want to duplicate it a second time.  */
  if (key != value)
    splay_tree_insert (id->decl_map, (splay_tree_key) value,
		       (splay_tree_value) value);
}

/* Remap DECL during the copying of the BLOCK tree for the function.  */

static tree
remap_decl (tree decl, inline_data *id)
{
  splay_tree_node n;
  tree fn;

  /* We only remap local variables in the current function.  */
  fn = id->callee;

  /* See if we have remapped this declaration.  */

  n = splay_tree_lookup (id->decl_map, (splay_tree_key) decl);

  /* If we didn't already have an equivalent for this declaration, 
     create one now.  */
  if (!n)
    {
      /* Make a copy of the variable or label.  */
      tree t = copy_decl_for_inlining (decl, fn, id->caller);

      /* Remap types, if necessary.  */
      TREE_TYPE (t) = remap_type (TREE_TYPE (t), id);
      if (TREE_CODE (t) == TYPE_DECL)
        DECL_ORIGINAL_TYPE (t) = remap_type (DECL_ORIGINAL_TYPE (t), id);
      else if (TREE_CODE (t) == PARM_DECL)
        DECL_ARG_TYPE_AS_WRITTEN (t)
	  = remap_type (DECL_ARG_TYPE_AS_WRITTEN (t), id);

      /* Remap sizes as necessary.  */
      walk_tree (&DECL_SIZE (t), copy_body_r, id, NULL);
      walk_tree (&DECL_SIZE_UNIT (t), copy_body_r, id, NULL);

#if 0
      /* FIXME handle anon aggrs.  */
      if (! DECL_NAME (t) && TREE_TYPE (t)
	  && lang_hooks.tree_inlining.anon_aggr_type_p (TREE_TYPE (t)))
	{
	  /* For a VAR_DECL of anonymous type, we must also copy the
	     member VAR_DECLS here and rechain the DECL_ANON_UNION_ELEMS.  */
	  tree members = NULL;
	  tree src;

	  for (src = DECL_ANON_UNION_ELEMS (t); src;
	       src = TREE_CHAIN (src))
	    {
	      tree member = remap_decl (TREE_VALUE (src), id);

	      gcc_assert (!TREE_PURPOSE (src));
	      members = tree_cons (NULL, member, members);
	    }
	  DECL_ANON_UNION_ELEMS (t) = nreverse (members);
	}
#endif

      /* If we're not saving and we're not cloning, we must be
	 inlining; if this is a variable (not a label), declare the
	 remapped variable in the callers' body.  */
      if (!id->saving_p
	  && !id->cloning_p
	  && (TREE_CODE (t) == VAR_DECL
	      || TREE_CODE (t) == PARM_DECL))
	declare_inline_vars (id->bind_expr, t);

      /* Remember it, so that if we encounter this local entity
	 again we can reuse this copy.  */
      insert_decl_map (id, decl, t);
      return t;
    }

  return unshare_expr ((tree) n->value);
}

/* External access for remap_decl ().  */
tree
remap_decl_v (tree decl, void *id)
{
  return remap_decl (decl, (inline_data *) id);
}

static tree
remap_type (tree type, inline_data *id)
{
  splay_tree_node node;
  tree new, t;

  if (type == NULL)
    return type;

  /* See if we have remapped this type.  */
  node = splay_tree_lookup (id->decl_map, (splay_tree_key) type);
  if (node)
    return (tree) node->value;

  /* The type only needs remapping if it's variably modified.  */
  if (! variably_modified_type_p (type, id->callee))
    {
      insert_decl_map (id, type, type);
      return type;
    }

  /* We do need a copy.  build and register it now.  If this is a pointer or
     reference type, remap the designated type and make a new pointer or
     reference type.  */
  if (TREE_CODE (type) == POINTER_TYPE)
    {
      new = build_pointer_type_for_mode (remap_type (TREE_TYPE (type), id),
					 TYPE_MODE (type),
					 TYPE_REF_CAN_ALIAS_ALL (type));
      insert_decl_map (id, type, new);
      return new;
    }
  else if (TREE_CODE (type) == REFERENCE_TYPE)
    {
      new = build_reference_type_for_mode (remap_type (TREE_TYPE (type), id),
					    TYPE_MODE (type),
					    TYPE_REF_CAN_ALIAS_ALL (type));
      insert_decl_map (id, type, new);
      return new;
    }
  else
    new = copy_node (type);

  insert_decl_map (id, type, new);

  /* This is a new type, not a copy of an old type.  Need to reassociate
     variants.  We can handle everything except the main variant lazily.  */
  t = TYPE_MAIN_VARIANT (type);
  if (type != t)
    {
      t = remap_type (t, id);
      TYPE_MAIN_VARIANT (new) = t;
      TYPE_NEXT_VARIANT (new) = TYPE_MAIN_VARIANT (t);
      TYPE_NEXT_VARIANT (t) = new;
    }
  else
    {
      TYPE_MAIN_VARIANT (new) = new;
      TYPE_NEXT_VARIANT (new) = NULL;
    }

  /* Lazily create pointer and reference types.  */
  TYPE_POINTER_TO (new) = NULL;
  TYPE_REFERENCE_TO (new) = NULL;

  switch (TREE_CODE (new))
    {
    case INTEGER_TYPE:
    case REAL_TYPE:
    case ENUMERAL_TYPE:
    case BOOLEAN_TYPE:
    case CHAR_TYPE:
      t = TYPE_MIN_VALUE (new);
      if (t && TREE_CODE (t) != INTEGER_CST)
        walk_tree (&TYPE_MIN_VALUE (new), copy_body_r, id, NULL);

      t = TYPE_MAX_VALUE (new);
      if (t && TREE_CODE (t) != INTEGER_CST)
        walk_tree (&TYPE_MAX_VALUE (new), copy_body_r, id, NULL);
      return new;

    case POINTER_TYPE:
      TREE_TYPE (new) = t = remap_type (TREE_TYPE (new), id);
      TYPE_NEXT_PTR_TO (new) = TYPE_POINTER_TO (t);
      TYPE_POINTER_TO (t) = new;
      return new;
      
    case REFERENCE_TYPE:
      TREE_TYPE (new) = t = remap_type (TREE_TYPE (new), id);
      TYPE_NEXT_REF_TO (new) = TYPE_REFERENCE_TO (t);
      TYPE_REFERENCE_TO (t) = new;
      return new;

    case METHOD_TYPE:
    case FUNCTION_TYPE:
      TREE_TYPE (new) = remap_type (TREE_TYPE (new), id);
      walk_tree (&TYPE_ARG_TYPES (new), copy_body_r, id, NULL);
      return new;

    case ARRAY_TYPE:
      TREE_TYPE (new) = remap_type (TREE_TYPE (new), id);
      TYPE_DOMAIN (new) = remap_type (TYPE_DOMAIN (new), id);
      break;

    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      walk_tree (&TYPE_FIELDS (new), copy_body_r, id, NULL);
      break;

    case FILE_TYPE:
    case OFFSET_TYPE:
    default:
      /* Shouldn't have been thought variable sized.  */
      gcc_unreachable ();
    }

  walk_tree (&TYPE_SIZE (new), copy_body_r, id, NULL);
  walk_tree (&TYPE_SIZE_UNIT (new), copy_body_r, id, NULL);

  return new;
}

static tree
remap_decls (tree decls, inline_data *id)
{
  tree old_var;
  tree new_decls = NULL_TREE;

  /* Remap its variables.  */
  for (old_var = decls; old_var; old_var = TREE_CHAIN (old_var))
    {
      tree new_var;

      /* Remap the variable.  */
      new_var = remap_decl (old_var, id);

      /* If we didn't remap this variable, so we can't mess with its
	 TREE_CHAIN.  If we remapped this variable to the return slot, it's
	 already declared somewhere else, so don't declare it here.  */
      if (!new_var || new_var == id->retvar)
	;
      else
	{
	  gcc_assert (DECL_P (new_var));
	  TREE_CHAIN (new_var) = new_decls;
	  new_decls = new_var;
	}
    }

  return nreverse (new_decls);
}

/* Copy the BLOCK to contain remapped versions of the variables
   therein.  And hook the new block into the block-tree.  */

static void
remap_block (tree *block, inline_data *id)
{
  tree old_block;
  tree new_block;
  tree fn;

  /* Make the new block.  */
  old_block = *block;
  new_block = make_node (BLOCK);
  TREE_USED (new_block) = TREE_USED (old_block);
  BLOCK_ABSTRACT_ORIGIN (new_block) = old_block;
  *block = new_block;

  /* Remap its variables.  */
  BLOCK_VARS (new_block) = remap_decls (BLOCK_VARS (old_block), id);

  fn = id->caller;
#if 1
  /* FIXME!  It shouldn't be so hard to manage blocks.  Rebuilding them in
     rest_of_compilation is a good start.  */
  if (id->cloning_p)
    /* We're building a clone; DECL_INITIAL is still
       error_mark_node, and current_binding_level is the parm
       binding level.  */
    lang_hooks.decls.insert_block (new_block);
  else
    {
      /* Attach this new block after the DECL_INITIAL block for the
	 function into which this block is being inlined.  In
	 rest_of_compilation we will straighten out the BLOCK tree.  */
      tree *first_block;
      if (DECL_INITIAL (fn))
	first_block = &BLOCK_CHAIN (DECL_INITIAL (fn));
      else
	first_block = &DECL_INITIAL (fn);
      BLOCK_CHAIN (new_block) = *first_block;
      *first_block = new_block;
    }
#endif
  /* Remember the remapped block.  */
  insert_decl_map (id, old_block, new_block);
}

static void
copy_statement_list (tree *tp)
{
  tree_stmt_iterator oi, ni;
  tree newlist;

  newlist = alloc_stmt_list ();
  ni = tsi_start (newlist);
  for (oi = tsi_start (*tp); !tsi_end_p (oi); tsi_next (&oi))
    tsi_link_after (&ni, tsi_stmt (oi), TSI_NEW_STMT);

  *tp = newlist;
}

static void
copy_bind_expr (tree *tp, int *walk_subtrees, inline_data *id)
{
  tree block = BIND_EXPR_BLOCK (*tp);
  /* Copy (and replace) the statement.  */
  copy_tree_r (tp, walk_subtrees, NULL);
  if (block)
    {
      remap_block (&block, id);
      BIND_EXPR_BLOCK (*tp) = block;
    }

  if (BIND_EXPR_VARS (*tp))
    /* This will remap a lot of the same decls again, but this should be
       harmless.  */
    BIND_EXPR_VARS (*tp) = remap_decls (BIND_EXPR_VARS (*tp), id);
}

/* Called from copy_body_id via walk_tree.  DATA is really an 
   `inline_data *'.  */

static tree
copy_body_r (tree *tp, int *walk_subtrees, void *data)
{
  inline_data *id = (inline_data *) data;
  tree fn = id->callee;

  /* Begin by recognizing trees that we'll completely rewrite for the
     inlining context.  Our output for these trees is completely
     different from out input (e.g. RETURN_EXPR is deleted, and morphs
     into an edge).  Further down, we'll handle trees that get
     duplicated and/or tweaked.  */

  /* If this is a RETURN_EXPR, change it into a MODIFY_EXPR and a
     GOTO_EXPR with the RET_LABEL as its target.  */
  if (TREE_CODE (*tp) == RETURN_EXPR && id->ret_label)
    {
      tree assignment = TREE_OPERAND (*tp, 0);

      TREE_USED (id->ret_label) = 1;
      id->ret_count++;

      /* If we're returning something, just turn that into an
	 assignment into the equivalent of the original
	 RESULT_DECL.  If the "assignment" is just the result decl, 
	 the result decl has already been set (e.g. a recent "foo (&result_decl,
	 ...)"); just toss the entire RETURN_EXPR.  */
      if (assignment && TREE_CODE (assignment) == MODIFY_EXPR)
	{
	  edge new;

	  /* Replace the RETURN_EXPR with (a copy of) the
	     MODIFY_EXPR hangning underneath.  */
	  *tp = copy_node (assignment);

	  /* If we're working on CFGs, add an outgoing CFG edge to the
	     return block.  */
	  new = make_edge (id->copy_basic_block, id->return_block,
		           EDGE_FALLTHRU);
	  new->count = id->copy_basic_block->count;
	  new->probability = REG_BR_PROB_BASE;
	}
      else /* Else the RETURN_EXPR returns no value.  */
	{
	  edge new;
	  /* Since we're not returning anything, just delete the
	     RETURN_EXPR.  This is spooky, as we're altering a
	     tree_stmt_iterator owned by our caller, who is using
	     it to iterate through the statemetns of this
	     block.  */
	  tsi_delink (&id->copy_tsi);

	  /* If we're working on CFGs, add an outgoing CFG edge to the
	     return block.  */
	  new = make_edge (id->copy_basic_block, id->return_block,
		           EDGE_FALLTHRU);
	  new->count = id->copy_basic_block->count;
	  new->probability = REG_BR_PROB_BASE;
	  /* Return something to stop iterating.  */
	  return (void *)1;
	}
    }

  /* Local variables and labels need to be replaced by equivalent
     variables.  We don't want to copy static variables; there's only
     one of those, no matter how many times we inline the containing
     function.  Similarly for globals from an outer function.  */
  else if (lang_hooks.tree_inlining.auto_var_in_fn_p (*tp, fn))
    {
      tree new_decl;

      /* Remap the declaration.  */
      new_decl = remap_decl (*tp, id);
      gcc_assert (new_decl);
      /* Replace this variable with the copy.  */
      STRIP_TYPE_NOPS (new_decl);
      *tp = new_decl;
    }
  else if (TREE_CODE (*tp) == STATEMENT_LIST)
    {
      tree *step_p = tp;
      for (step_p = tp; step_p && *step_p; step_p = &TREE_CHAIN (*step_p))
	copy_statement_list (step_p);
    }
  else if (TREE_CODE (*tp) == SAVE_EXPR)
    remap_save_expr (tp, id->decl_map, walk_subtrees);
  else if (TREE_CODE (*tp) == LABEL_DECL)
    /* These may need to be remapped for EH handling.  */
    remap_decl (*tp, id);
  else if (TREE_CODE (*tp) == BIND_EXPR)
    copy_bind_expr (tp, walk_subtrees, id);
  /* Types may need remapping as well.  */
  else if (TYPE_P (*tp))
    *tp = remap_type (*tp, id);

  /* If this is a constant, we have to copy the node iff the type will be
     remapped.  copy_tree_r will not copy a constant.  */
  else if (TREE_CODE_CLASS (TREE_CODE (*tp)) == tcc_constant)
    {
      tree new_type = remap_type (TREE_TYPE (*tp), id);

      if (new_type == TREE_TYPE (*tp))
	*walk_subtrees = 0;

      else if (TREE_CODE (*tp) == INTEGER_CST)
	*tp = build_int_cst_wide (new_type, TREE_INT_CST_LOW (*tp),
				  TREE_INT_CST_HIGH (*tp));
      else
	{
	  *tp = copy_node (*tp);
	  TREE_TYPE (*tp) = new_type;
	}
    }

  /* Otherwise, just copy the node.  Note that copy_tree_r already
     knows not to copy VAR_DECLs, etc., so this is safe.  */
  else
    {
      tree old_node = *tp;

      /* Here we handle trees that are not completely rewritten.
	 First we detect some inlining-induced bogosities for
	 discarding.  */
      if (TREE_CODE (*tp) == MODIFY_EXPR
	  && TREE_OPERAND (*tp, 0) == TREE_OPERAND (*tp, 1)
	  && (lang_hooks.tree_inlining.auto_var_in_fn_p
	      (TREE_OPERAND (*tp, 0), fn)))
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
		{
		  *tp = build_empty_stmt ();
		  return copy_body_r (tp, walk_subtrees, data);
		}
	    }
	}
      else if (TREE_CODE (*tp) == INDIRECT_REF)
	{
	  /* Get rid of *& from inline substitutions that can happen when a
	     pointer argument is an ADDR_EXPR.  */
	  tree decl = TREE_OPERAND (*tp, 0), value;
	  splay_tree_node n;

	  n = splay_tree_lookup (id->decl_map, (splay_tree_key) decl);
	  if (n)
	    {
	      value = (tree) n->value;
	      STRIP_NOPS (value);
	      if (TREE_CODE (value) == ADDR_EXPR
		  && (lang_hooks.types_compatible_p
		      (TREE_TYPE (*tp), TREE_TYPE (TREE_OPERAND (value, 0)))))
		{
		  *tp = TREE_OPERAND (value, 0);
		  return copy_body_r (tp, walk_subtrees, data);
		}
	    }
	}

      /* Here is the "usual case".  Copy this tree node, and then
	 tweak some special cases.  */
      copy_tree_r (tp, walk_subtrees, NULL);
      if (id->bind_expr 
	  && IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (TREE_CODE (*tp))))
	TREE_BLOCK (*tp) = BIND_EXPR_BLOCK (id->bind_expr);

      /* We're duplicationg a CALL_EXPR.  Find any corresponding
	 callgraph edges and update or duplicate them.  */
      if (TREE_CODE (*tp) == CALL_EXPR && id->node && get_callee_fndecl (*tp))
	{
	  if (id->saving_p)
	    {
	      struct cgraph_node *node;
              struct cgraph_edge *edge;

	      /* We're saving a copy of the body, so we'll update the
		 callgraph nodes in place.  Note that we avoid
		 altering the original callgraph node; we begin with
		 the first clone.  */
	      for (node = id->node->next_clone; node; node = node->next_clone)
		{
		  edge = cgraph_edge (node, old_node);
		  gcc_assert (edge);
		  edge->call_expr = *tp;
		}
	    }
	  else
	    {
              struct cgraph_edge *edge;

	      /* We're cloning or inlining this body; duplicate the
		 associate callgraph nodes.  */
	      edge = cgraph_edge (id->current_node, old_node);

	      if (edge)
	        cgraph_clone_edge (edge, id->node, *tp, REG_BR_PROB_BASE);
	    }
	}
      else if (TREE_CODE (*tp) == RESX_EXPR)
	{
	  /* If we're inlining (e.g. not saving and not cloning),
	     adjust the region number of this resume expression.  */
	  if (!id->saving_p && !id->cloning_p)
	    {
	      struct function *outermost_function
		    = DECL_STRUCT_FUNCTION (id->node->decl);
	      TREE_OPERAND (*tp, 0) = build_int_cst
		    (NULL_TREE,
		     get_eh_last_region_number (outermost_function)
		     + TREE_INT_CST_LOW (TREE_OPERAND (*tp, 0)));
	    }
	}

      if (tree_could_throw_p (*tp))
	{
	  /* Add an entry for the copied tree in the EH hashtable.
	     When saving or cloning, use the hashtable in cfun,
	     and just copy the EH number.  When inlining, use the
	     hashtable in the caller, and adjust the region number.
	     (Note that the EH region table hasn't yet been updated
	     to reflect the callee's region, so don't look at that.)  */
	  if (id->saving_p || id->cloning_p)
	    duplicate_stmt_eh_region_mapping (
			id->callee_cfun, cfun,
			old_node, *tp, 0);
	  else
	    duplicate_stmt_eh_region_mapping (
			id->callee_cfun,
			DECL_STRUCT_FUNCTION (id->caller),
			old_node, *tp, 1);
	}
      TREE_TYPE (*tp) = remap_type (TREE_TYPE (*tp), id);

      /* The copied TARGET_EXPR has never been expanded, even if the
	 original node was expanded already.  */
      if (TREE_CODE (*tp) == TARGET_EXPR && TREE_OPERAND (*tp, 3))
	{
	  TREE_OPERAND (*tp, 1) = TREE_OPERAND (*tp, 3);
	  TREE_OPERAND (*tp, 3) = NULL_TREE;
	}

      /* Variable substitution need not be simple.  In particular, the
	 INDIRECT_REF substitution above.  Make sure that TREE_CONSTANT
	 and friends are up-to-date.  */
      else if (TREE_CODE (*tp) == ADDR_EXPR)
	{
	  walk_tree (&TREE_OPERAND (*tp, 0), copy_body_r, id, NULL);
	  recompute_tree_invarant_for_addr_expr (*tp);
	  *walk_subtrees = 0;
	}
    }

  /* Keep iterating.  */
  return NULL_TREE;
}

/* Make a copy of the body of FN so that it can be inserted inline in
   another function.  Walks FN via CFG, returns new fndecl.  */

static tree
copy_cfg_body (inline_data *id, gcov_type count, int frequency)
{
  tree callee_fndecl = id->callee;
  tree caller_fndecl = id->caller;
  struct function *caller_cfun = DECL_STRUCT_FUNCTION (caller_fndecl);
  /* Original cfun for the callee, doesn't change.  */
  struct function *callee_cfun = DECL_STRUCT_FUNCTION (callee_fndecl);
   /*	(struct function *)ggc_alloc_cleared (sizeof (struct function)); */
  /* Copy, built by this function.  */
  struct function *new_cfun = 
	(struct function *)ggc_alloc_cleared (sizeof (struct function));
  /* Place to copy from; when a copy of the function was saved off earlier,
     use that instead of the main copy.  */
  struct function *cfun_to_copy = 
	(struct function *)ggc_alloc_cleared (sizeof (struct function));
  struct control_flow_graph *new_cfg;
  basic_block bb;
  edge old_edge;
  varray_type new_bb_info;
  int old_src_index;
  int old_dest_index;
  tree new_fndecl;
  bool saving_or_cloning;
  tree copy_stmt;
  tree orig_stmt;
  tree_stmt_iterator tsi, tsi_copy, tsi_end;
  int current_bb_index = 0;
  int count_scale, frequency_scale;

  if (ENTRY_BLOCK_PTR_FOR_FUNCTION (callee_cfun)->count)
    count_scale = (REG_BR_PROB_BASE * count
		   / ENTRY_BLOCK_PTR_FOR_FUNCTION (callee_cfun)->count);
  else
    count_scale = 1;

  if (ENTRY_BLOCK_PTR_FOR_FUNCTION (callee_cfun)->frequency)
    frequency_scale = (REG_BR_PROB_BASE * frequency
		       / ENTRY_BLOCK_PTR_FOR_FUNCTION (callee_cfun)->frequency);
  else
    frequency_scale = count_scale;

  /* Register specific tree functions.  */
  tree_register_cfg_hooks ();

  /* Must have a CFG here at this point.  */
  gcc_assert (ENTRY_BLOCK_PTR_FOR_FUNCTION (DECL_STRUCT_FUNCTION (callee_fndecl)));

  *new_cfun = *DECL_STRUCT_FUNCTION (callee_fndecl);
  *cfun_to_copy = *DECL_STRUCT_FUNCTION (callee_fndecl);
  new_cfun->decl = new_fndecl = copy_node (callee_fndecl);
  new_cfg = (struct control_flow_graph *)
		ggc_alloc_cleared (sizeof (struct control_flow_graph));
  *new_cfg = *DECL_STRUCT_FUNCTION (callee_fndecl)->cfg;
  new_cfun->cfg = new_cfg;
  new_cfun->ib_boundaries_block = (varray_type)0;

  /* If there is a saved_cfg+saved_tree+saved_args lurking in the
     struct function, a copy of the callee body was saved there, and
     the 'struct cgraph edge' nodes have been fudged to point into the
     saved body.  Accordingly, we want to copy that saved body so the
     callgraph edges will be recognized and cloned properly.  */     
  if (cfun_to_copy->saved_cfg)
    {
      cfun_to_copy->cfg = cfun_to_copy->saved_cfg;
      cfun_to_copy->eh = cfun_to_copy->saved_eh;
      DECL_SAVED_TREE (new_fndecl) = cfun_to_copy->saved_tree;
      DECL_ARGUMENTS (new_fndecl) = cfun_to_copy->saved_args;
    }
  id->callee_cfun = cfun_to_copy;
  DECL_STRUCT_FUNCTION (new_fndecl) = new_cfun;
  push_cfun (new_cfun);

  /* If saving or cloning a function body, create new basic_block_info
     and label_to_block_maps.  Otherwise, we're duplicaing a function
     body for inlining; insert our new blocks and labels into the
     existing varrays.  */
  saving_or_cloning = (id->saving_p || id->cloning_p);
  if (saving_or_cloning)
    {
      VARRAY_BB_INIT (basic_block_info, 
		      VARRAY_SIZE (basic_block_info_for_function (callee_cfun)),
		      "basic_block_info (copy_body)");
      VARRAY_BB_INIT (label_to_block_map, 
		      VARRAY_SIZE (label_to_block_map_for_function (callee_cfun)),
		      "label_to_block_map (copy_body)");
      new_bb_info = basic_block_info;
      last_basic_block = n_basic_blocks = 0;
    }
  else	/* Else we're inlining normally.  */
    {
      /* create_block() will append new blocks onto the ends of the
	 callers basic_block_info/label_to_block_map varrays.
	 basic_block_info will be re-written by compact_blocks() when
	 we're done inlining into this caller.  */
      basic_block_info = basic_block_info_for_function (caller_cfun);
      label_to_block_map = label_to_block_map_for_function (caller_cfun);
      last_basic_block = last_basic_block_for_function (caller_cfun);
      n_basic_blocks = n_basic_blocks_for_function (caller_cfun);
      VARRAY_GENERIC_PTR_NOGC_INIT (new_bb_info,
		    VARRAY_SIZE (basic_block_info_for_function (callee_cfun)),
		    "temporary basic_block_info (copy_body)");
      new_cfun->last_label_uid = caller_cfun->last_label_uid;
    }

  /* Duplicate any exception-handling regions.  */
  if (cfun->eh)
    {
      init_eh_for_function ();
      duplicate_eh_regions (cfun_to_copy, id, true);
    }
  n_edges = 0;
  ENTRY_BLOCK_PTR = ggc_alloc_cleared (sizeof (*ENTRY_BLOCK_PTR));
  ENTRY_BLOCK_PTR->index = ENTRY_BLOCK;
  ENTRY_BLOCK_PTR->count = (ENTRY_BLOCK_PTR_FOR_FUNCTION (callee_cfun)->count
		            * count_scale / REG_BR_PROB_BASE);
  ENTRY_BLOCK_PTR->frequency
    = (ENTRY_BLOCK_PTR_FOR_FUNCTION (callee_cfun)->frequency
       * frequency_scale / REG_BR_PROB_BASE);
  EXIT_BLOCK_PTR = ggc_alloc_cleared (sizeof (*EXIT_BLOCK_PTR));
  EXIT_BLOCK_PTR->index = EXIT_BLOCK;
  ENTRY_BLOCK_PTR->next_bb = EXIT_BLOCK_PTR;
  EXIT_BLOCK_PTR->prev_bb = ENTRY_BLOCK_PTR;
  id->copy_basic_block = ENTRY_BLOCK_PTR;
  EXIT_BLOCK_PTR->count = (EXIT_BLOCK_PTR_FOR_FUNCTION (callee_cfun)->count
		           * count_scale / REG_BR_PROB_BASE);
  EXIT_BLOCK_PTR->frequency
    = (EXIT_BLOCK_PTR_FOR_FUNCTION (callee_cfun)->frequency
       * frequency_scale / REG_BR_PROB_BASE);

  /* Standard tree walk doesn't iterate over CFG-built basic_blocks
     yet.  */
  FOR_EACH_BB_FN (bb, cfun_to_copy)
    {
      /* create_basic_block() will append every new block to
	 basic_block_info automatically.  */
      id->copy_basic_block = create_basic_block (bb->stmt_list, 
			    (void*)0, id->copy_basic_block);
      id->copy_basic_block->count = bb->count * count_scale / REG_BR_PROB_BASE;
      id->copy_basic_block->frequency = (bb->frequency
		      			 * frequency_scale / REG_BR_PROB_BASE);
      /* Number the blocks.  When we duplicate the edges of this
	 funciton body, we need an easy way to map edges
	 from the original body into our newly-copied body.  We could
	 use a hashtable, but unique block indicies are much simpler.
	 Alas, we cannot expect the blocks to arrive here with
	 rational indicies.  */
      /* Note this is a counter-intuitive side effect: making a copy
	 of a function body will alter the basic-block indices of the
	 copied body.  */
      id->copy_basic_block->index = bb->index = current_bb_index;
      /* If we're saving or cloning, we'll use the basic_block_info
	 varray.  Otherwise, create a parallel varray here for edge
	 creation.  */
      if (!saving_or_cloning)
	VARRAY_BB (new_bb_info, current_bb_index) = id->copy_basic_block;
      current_bb_index++;
      /* We could invoke walk_tree() and have it copy everything below
	 here, but RETURN_EXPR processing often requires tsi_delink(),
	 requiring access to the associated active tree_stmt_iterator.
	 Since the generic tree walk cannot pass down its own
	 tree_stmt_iterator, we run iteration from here, and pass our
	 own tree_stmt_iterator down.  See copy_body_r():RETURN_EXPR
	 processing for the rest of the story.  */
      copy_statement_list (&id->copy_basic_block->stmt_list);
      for (id->copy_tsi = tsi_start (id->copy_basic_block->stmt_list);
	   !tsi_end_p (id->copy_tsi); tsi_next (&id->copy_tsi))
	{
	  walk_tree (tsi_stmt_ptr (id->copy_tsi), copy_body_r, id, NULL);
	  /* A RETURN_EXPR that returns no value will be deleted;
	     detect this occurance and exit this loop early.  */
	  if (tsi_end_p (id->copy_tsi))
	    break;
	}
      tsi = tsi_start (id->copy_basic_block->stmt_list);
      /* If non-empty block, fixup the statement->block uplinks.  */
      if (!tsi_end_p (tsi))
	set_bb_for_stmt (id->copy_basic_block->stmt_list, id->copy_basic_block);
      /* Insure prompt crash if this is used out-of-context.  */
      id->copy_tsi.container = NULL_TREE;
    }

  /* Now that we've duplicated the blocks, duplicate their edges.  */
  FOR_EACH_BB_FN (bb, cfun_to_copy)
    {
      basic_block new_bb = VARRAY_BB (new_bb_info, bb->index);
      edge_iterator ei;

      /* Use the indices from the original blocks to create edges for the
	 new ones.  */
      old_src_index = bb->index;
      FOR_EACH_EDGE (old_edge, ei, bb->succs)
	{
	  edge new;

	  old_dest_index = old_edge->dest->index;
	  /* Ignore edges that reach ENTRY and EXIT blocks
	     (bb->index < 0).  The first non-ENTRY block will be spliced
	     and handled specially; RETURN_EXPRs have been discarded,
	     and they have created their own edges already (a
	     side-effect of copying the callee body).  Abnormal edges
	     (e.g. noreturn functions) should be redirected to the
	     next exit block.  */
	  if (old_src_index >= 0 && old_dest_index >= 0)
	    {
	      new = make_edge (new_bb,
			       VARRAY_BB (new_bb_info, old_dest_index),
			       old_edge->flags);
	      new->count = old_edge->count * count_scale / REG_BR_PROB_BASE;
	      new->probability = old_edge->probability;
	    }
	}

      tsi_end = tsi_last (bb->stmt_list);
      for (tsi = tsi_start (bb->stmt_list),
	     tsi_copy = tsi_start (new_bb->stmt_list);
	   !tsi_end_p (tsi_copy);
	   tsi_next (&tsi))
	{
	  orig_stmt = tsi_stmt (tsi);
	  copy_stmt = tsi_stmt (tsi_copy);
	  /* Do this before the possible split_block.  */
	  tsi_next (&tsi_copy);

	  /* If this tree could throw an exception, there are two
	     cases where we need to add abnormal edge(s): the
	     tree wasn't in a region and there is a "current
	     region" in the caller; or the original tree had
	     EH edges.  In both cases split the block after the tree,
	     and add abnormal edge(s) as needed; we need both
	     those from the callee and the caller.
	     We check whether the copy can throw, because the const
	     propagation can change an INDIRECT_REF which throws
	     into a COMPONENT_REF which doesn't.  If the copy
	     can throw, the original could also throw.  */

	  if (tree_could_throw_p (copy_stmt)
	      && ((lookup_stmt_eh_region_fn (
			    DECL_STRUCT_FUNCTION (id->callee),
			    orig_stmt) <= 0
	      && get_eh_cur_region (caller_cfun) > 0)
		|| (eh_dst
		   && tree_could_throw_p (copy_stmt)
		   && VARRAY_ACTIVE_SIZE (eh_dst) > 0)))
	    {
	      unsigned int i;
	      basic_block copied_call_bb = new_bb;
	      if (tsi_stmt (tsi_end) != tsi_stmt (tsi))
		/* Note that bb's predecessor edges aren't necessarily
		   right at this point; split_block doesn't care.  */
		{
		  edge e = split_block (new_bb, copy_stmt);
		  new_bb = e->dest;
		}

	      /* If this tree doesn't have a region associated with it,
		 and there is a "current region,"
		 then associate this tree with the current region
		 and add edges associated with this region.  */
	      if (lookup_stmt_eh_region_fn (
			    id->callee_cfun,
			    orig_stmt) <= 0
		  && get_eh_cur_region (caller_cfun) > 0)
		{
		  struct eh_status *saved_eh = cfun->eh;
		  cfun->eh = caller_cfun->eh;
		  add_stmt_to_eh_region (copy_stmt,
					 get_eh_cur_region (caller_cfun));
		  make_eh_edges (copy_stmt);
		  cfun->eh = saved_eh;
		}

	      /* If the original call that we're inlining had EH
		 edges, attach EH edges with the same destination
		 to this block.  */
	      if (eh_dst)
	      for (i = 0; i < VARRAY_ACTIVE_SIZE (eh_dst); i++)
	        make_edge (copied_call_bb, VARRAY_BB (eh_dst, i), 
			    EDGE_EH | EDGE_ABNORMAL);
	    }
	}
    }
  if (saving_or_cloning)
    {
      edge new;
      int exit_probability = REG_BR_PROB_BASE;
      gcov_type exit_count = BASIC_BLOCK (last_basic_block - 1)->count;
      edge_iterator ei;

      new = make_edge (ENTRY_BLOCK_PTR, BASIC_BLOCK (0), EDGE_FALLTHRU);
      new->probability = REG_BR_PROB_BASE;
      new->count = ENTRY_BLOCK_PTR->count;
      FOR_EACH_EDGE (old_edge, ei, BASIC_BLOCK (last_basic_block-1)->succs)
	{
	  exit_probability -= old_edge->probability;
	  exit_count -= old_edge->count;
	}
      new = make_edge (BASIC_BLOCK (last_basic_block-1), EXIT_BLOCK_PTR,
		       EDGE_FALLTHRU);
      new->probability = exit_probability;
      new->count = exit_count;
    }
  id->copy_basic_block = (basic_block)0;
  if (!saving_or_cloning)
    VARRAY_FREE (new_bb_info);

  DECL_SAVED_TREE (new_fndecl) = ENTRY_BLOCK_PTR->next_bb->stmt_list;

  pop_cfun ();

  /* These varrays have grown, and probably have been realloced.  */
  if (!saving_or_cloning)
    {
      caller_cfun->last_label_uid = new_cfun->last_label_uid;
      basic_block_info = basic_block_info_for_function (new_cfun);
      label_to_block_map = label_to_block_map_for_function (new_cfun);
      n_basic_blocks = n_basic_blocks_for_function (new_cfun);
      last_basic_block = last_basic_block_for_function (new_cfun);
    }
  return new_fndecl;
}

/* Make a copy of the body of FN so that it can be inserted inline in
   another function.  */

static tree
copy_generic_body (inline_data *id)
{
  tree body;
  tree fndecl = id->callee;

  if (fndecl == current_function_decl
      && cfun->saved_tree)
    body = cfun->saved_tree;
  else
    body = DECL_SAVED_TREE (fndecl);
  walk_tree (&body, copy_body_r, id, NULL);

  return body;
}

static tree
copy_body (inline_data *id, gcov_type count, int frequency)
{
  tree fndecl = id->callee;
  tree body;

  /* If this body has a CFG, walk CFG and copy.  */
  gcc_assert (ENTRY_BLOCK_PTR_FOR_FUNCTION (DECL_STRUCT_FUNCTION (fndecl)));
  body = copy_cfg_body (id, count, frequency);

  return body;
}

/* Return true if VALUE is an ADDR_EXPR of an automatic variable
   defined in function FN, or of a data member thereof.  */

static bool
self_inlining_addr_expr (tree value, tree fn)
{
  tree var;

  if (TREE_CODE (value) != ADDR_EXPR)
    return false;

  var = get_base_address (TREE_OPERAND (value, 0));
	      
  return var && lang_hooks.tree_inlining.auto_var_in_fn_p (var, fn);
}

static void
setup_one_parameter (inline_data *id, tree p, tree value, tree fn,
		     tree *init_stmts, tree *vars, bool *gimplify_init_stmts_p)
{
  tree init_stmt;
  tree var;
  tree var_sub;

  /* If the parameter is never assigned to, we may not need to
     create a new variable here at all.  Instead, we may be able
     to just use the argument value.  */
  /* In theory this should not be necessary for correctness, but
     in practice there are a lot of dejagnu failures (especially
     in C++ library) if it is removed...  */
  if (TREE_READONLY (p)
      && !TREE_ADDRESSABLE (p)
      && value && !TREE_SIDE_EFFECTS (value))
    {
      /* We may produce non-gimple trees by adding NOPs or introduce
	 invalid sharing when operand is not really constant.
	 It is not big deal to prohibit constant propagation here as
	 we will constant propagate in DOM1 pass anyway.  */
      if (is_gimple_min_invariant (value)
	  && lang_hooks.types_compatible_p (TREE_TYPE (value), TREE_TYPE (p))
	  /* We have to be very careful about ADDR_EXPR.  Make sure
	     the base variable isn't a local variable of the inlined
	     function, e.g., when doing recursive inlining, direct or
	     mutually-recursive or whatever, which is why we don't
	     just test whether fn == current_function_decl.  */
	  && ! self_inlining_addr_expr (value, fn))
	{
	  insert_decl_map (id, p, value);
	  return;
	}
    }

  /* Make an equivalent VAR_DECL.  Note that we must NOT remap the type
     here since the type of this decl must be visible to the calling
     function.  */
  var = copy_decl_for_inlining (p, fn, id->caller);

  /* See if the frontend wants to pass this by invisible reference.  If
     so, our new VAR_DECL will have REFERENCE_TYPE, and we need to
     replace uses of the PARM_DECL with dereferences.  */
  if (TREE_TYPE (var) != TREE_TYPE (p)
      && POINTER_TYPE_P (TREE_TYPE (var))
      && TREE_TYPE (TREE_TYPE (var)) == TREE_TYPE (p))
    {
      insert_decl_map (id, var, var);
      var_sub = build1 (INDIRECT_REF, TREE_TYPE (p), var);
    }
  else
    var_sub = var;
  
  /* Register the VAR_DECL as the equivalent for the PARM_DECL;
     that way, when the PARM_DECL is encountered, it will be
     automatically replaced by the VAR_DECL.  */
  insert_decl_map (id, p, var_sub);

  /* Declare this new variable.  */
  TREE_CHAIN (var) = *vars;
  *vars = var;

  /* Make gimplifier happy about this variable.  */
  DECL_SEEN_IN_BIND_EXPR_P (var) = 1;

  /* Even if P was TREE_READONLY, the new VAR should not be.
     In the original code, we would have constructed a
     temporary, and then the function body would have never
     changed the value of P.  However, now, we will be
     constructing VAR directly.  The constructor body may
     change its value multiple times as it is being
     constructed.  Therefore, it must not be TREE_READONLY;
     the back-end assumes that TREE_READONLY variable is
     assigned to only once.  */
  if (TYPE_NEEDS_CONSTRUCTING (TREE_TYPE (p)))
    TREE_READONLY (var) = 0;

  /* Initialize this VAR_DECL from the equivalent argument.  Convert
     the argument to the proper type in case it was promoted.  */
  if (value)
    {
      tree rhs = fold_convert (TREE_TYPE (var), value);

      if (rhs == error_mark_node)
	return;

      /* We want to use MODIFY_EXPR, not INIT_EXPR here so that we
	 keep our trees in gimple form.  */
      init_stmt = build (MODIFY_EXPR, TREE_TYPE (var), var, rhs);
      append_to_statement_list (init_stmt, init_stmts);

      /* If we did not create a gimple value and we did not create a gimple
	 cast of a gimple value, then we will need to gimplify INIT_STMTS
	 at the end.  Note that is_gimple_cast only checks the outer
	 tree code, not its operand.  Thus the explicit check that it's
	 operand is a gimple value.  */
      if (!is_gimple_val (rhs)
	  && (!is_gimple_cast (rhs)
	      || !is_gimple_val (TREE_OPERAND (rhs, 0))))
	*gimplify_init_stmts_p = true;
    }
}

/* Generate code to initialize the parameters of the function at the
   top of the stack in ID from the ARGS (presented as a TREE_LIST).  */

static tree
initialize_inlined_parameters (inline_data *id, tree args, tree static_chain,
			       tree fn, tree bind_expr)
{
  tree init_stmts = NULL_TREE;
  tree parms;
  tree a;
  tree p;
  tree vars = NULL_TREE;
  bool gimplify_init_stmts_p = false;
  int argnum = 0;

  /* Figure out what the parameters are.  */
  parms = DECL_ARGUMENTS (fn);
  if (fn == current_function_decl)
    parms = cfun->saved_args;

  /* Loop through the parameter declarations, replacing each with an
     equivalent VAR_DECL, appropriately initialized.  */
  for (p = parms, a = args; p;
       a = a ? TREE_CHAIN (a) : a, p = TREE_CHAIN (p))
    {
      tree value;

      ++argnum;

      /* Find the initializer.  */
      value = lang_hooks.tree_inlining.convert_parm_for_inlining
	      (p, a ? TREE_VALUE (a) : NULL_TREE, fn, argnum);

      setup_one_parameter (id, p, value, fn, &init_stmts, &vars,
			   &gimplify_init_stmts_p);
    }

  /* Evaluate trailing arguments.  */
  for (; a; a = TREE_CHAIN (a))
    {
      tree value = TREE_VALUE (a);
      append_to_statement_list (value, &init_stmts);
    }

  /* Initialize the static chain.  */
  p = DECL_STRUCT_FUNCTION (fn)->static_chain_decl;
  if (p)
    {
      /* No static chain?  Seems like a bug in tree-nested.c.  */
      gcc_assert (static_chain);

      setup_one_parameter (id, p, static_chain, fn, &init_stmts, &vars,
			   &gimplify_init_stmts_p);
    }

  if (gimplify_init_stmts_p)
    {
      gimplify_body (&init_stmts, fn);
      /* Remove BIND_EXPR added by gimplify_body.  */
      init_stmts = BIND_EXPR_BODY (init_stmts);
    }

  declare_inline_vars (bind_expr, vars);
  return init_stmts;
}

/* Declare a return variable to replace the RESULT_DECL for the 
   function we are calling.  An appropriate DECL_STMT is returned.
   The USE_STMT is filled to contain a use of the declaration to
   indicate the return value of the function.

   RETURN_SLOT_ADDR, if non-null, was a fake parameter that
   took the address of the result.  MODIFY_DEST, if non-null, was the LHS of
   the MODIFY_EXPR to which this call is the RHS.

   The return value is a (possibly null) value that is the result of the
   function as seen by the callee.  *USE_P is a (possibly null) value that
   holds the result as seen by the caller.  */

static tree
declare_return_variable (inline_data *id, tree return_slot_addr,
			 tree modify_dest, tree *use_p)
{
  tree callee = id->callee;
  tree caller = id->caller;
  tree result = DECL_RESULT (callee);
  tree callee_type = TREE_TYPE (result);
  tree caller_type = TREE_TYPE (TREE_TYPE (callee));
  tree var, use;
  int need_return_decl = 1;  

  /* We don't need to do anything for functions that don't return
     anything.  */
  if (!result || VOID_TYPE_P (callee_type))
    {
      *use_p = NULL_TREE;
      return NULL_TREE;
    }

  /* If there was a return slot, then the return value is the
     dereferenced address of that object.  */
  if (return_slot_addr)
    {
      /* The front end shouldn't have used both return_slot_addr and
	 a modify expression.  */
      gcc_assert (!modify_dest);
      if (DECL_BY_REFERENCE (result))
	var = return_slot_addr;
      else
	var = build_fold_indirect_ref (return_slot_addr);
      use = NULL;
      goto done;
    }

  /* All types requiring non-trivial constructors should have been handled.  */
  gcc_assert (!TREE_ADDRESSABLE (callee_type));

  /* Attempt to avoid creating a new temporary variable.  */
  if (modify_dest)
    {
      bool use_it = false;

      /* We can't use MODIFY_DEST if there's type promotion involved.  */
      if (!lang_hooks.types_compatible_p (caller_type, callee_type))
	use_it = false;

      /* ??? If we're assigning to a variable sized type, then we must
	 reuse the destination variable, because we've no good way to
	 create variable sized temporaries at this point.  */
      else if (TREE_CODE (TYPE_SIZE_UNIT (caller_type)) != INTEGER_CST)
	use_it = true;

      /* If the callee cannot possibly modify MODIFY_DEST, then we can
	 reuse it as the result of the call directly.  Don't do this if
	 it would promote MODIFY_DEST to addressable.  */
      else if (!TREE_STATIC (modify_dest)
	       && !TREE_ADDRESSABLE (modify_dest)
	       && !TREE_ADDRESSABLE (result))
	use_it = true;

      if (use_it)
	{
	  var = modify_dest;
	  use = NULL;
	  goto done;
	}
    }

  gcc_assert (TREE_CODE (TYPE_SIZE_UNIT (callee_type)) == INTEGER_CST);

  var = (lang_hooks.tree_inlining.copy_res_decl_for_inlining
	 (result, callee, id->caller, id->decl_map,
	  &need_return_decl, return_slot_addr));
  /* var = copy_decl_for_inlining (result, callee, caller); */
  DECL_SEEN_IN_BIND_EXPR_P (var) = 1;
  DECL_STRUCT_FUNCTION (caller)->unexpanded_var_list
    = tree_cons (NULL_TREE, var,
		 DECL_STRUCT_FUNCTION (caller)->unexpanded_var_list);

  /* Do not have the rest of GCC warn about this variable as it should
     not be visible to the user.  */
  TREE_NO_WARNING (var) = 1;

  /* Build the use expr.  If the return type of the function was
     promoted, convert it back to the expected type.  */
  use = var;
  if (!lang_hooks.types_compatible_p (TREE_TYPE (var), caller_type))
    use = fold_convert (caller_type, var);

 done:
  /* Register the VAR_DECL as the equivalent for the RESULT_DECL; that
     way, when the RESULT_DECL is encountered, it will be
     automatically replaced by the VAR_DECL.  */
  insert_decl_map (id, result, var);

  /* Remember this so we can ignore it in remap_decls.  */
  id->retvar = var;

  *use_p = use;
  return var;
}

/* Returns nonzero if a function can be inlined as a tree.  */

bool
tree_inlinable_function_p (tree fn)
{
  return inlinable_function_p (fn);
}

static const char *inline_forbidden_reason;

static tree
inline_forbidden_p_1 (tree *nodep, int *walk_subtrees ATTRIBUTE_UNUSED,
		      void *fnp)
{
  tree node = *nodep;
  tree fn = (tree) fnp;
  tree t;

  switch (TREE_CODE (node))
    {
    case CALL_EXPR:
      /* Refuse to inline alloca call unless user explicitly forced so as
	 this may change program's memory overhead drastically when the
	 function using alloca is called in loop.  In GCC present in
	 SPEC2000 inlining into schedule_block cause it to require 2GB of
	 RAM instead of 256MB.  */
      if (alloca_call_p (node)
	  && !lookup_attribute ("always_inline", DECL_ATTRIBUTES (fn)))
	{
	  inline_forbidden_reason
	    = N_("%Jfunction %qF can never be inlined because it uses "
		 "alloca (override using the always_inline attribute)");
	  return node;
	}
      t = get_callee_fndecl (node);
      if (! t)
	break;

      /* We cannot inline functions that call setjmp.  */
      if (setjmp_call_p (t))
	{
	  inline_forbidden_reason
	    = N_("%Jfunction %qF can never be inlined because it uses setjmp");
	  return node;
	}

      if (DECL_BUILT_IN_CLASS (t) == BUILT_IN_NORMAL)
	switch (DECL_FUNCTION_CODE (t))
	  {
	    /* We cannot inline functions that take a variable number of
	       arguments.  */
	  case BUILT_IN_VA_START:
	  case BUILT_IN_STDARG_START:
	  case BUILT_IN_NEXT_ARG:
	  case BUILT_IN_VA_END:
	    inline_forbidden_reason
	      = N_("%Jfunction %qF can never be inlined because it "
		   "uses variable argument lists");
	    return node;

	  case BUILT_IN_LONGJMP:
	    /* We can't inline functions that call __builtin_longjmp at
	       all.  The non-local goto machinery really requires the
	       destination be in a different function.  If we allow the
	       function calling __builtin_longjmp to be inlined into the
	       function calling __builtin_setjmp, Things will Go Awry.  */
	    inline_forbidden_reason
	      = N_("%Jfunction %qF can never be inlined because "
		   "it uses setjmp-longjmp exception handling");
	    return node;

	  case BUILT_IN_NONLOCAL_GOTO:
	    /* Similarly.  */
	    inline_forbidden_reason
	      = N_("%Jfunction %qF can never be inlined because "
		   "it uses non-local goto");
	    return node;

	  default:
	    break;
	  }
      break;

    case GOTO_EXPR:
      t = TREE_OPERAND (node, 0);

      /* We will not inline a function which uses computed goto.  The
	 addresses of its local labels, which may be tucked into
	 global storage, are of course not constant across
	 instantiations, which causes unexpected behavior.  */
      if (TREE_CODE (t) != LABEL_DECL)
	{
	  inline_forbidden_reason
	    = N_("%Jfunction %qF can never be inlined "
		 "because it contains a computed goto");
	  return node;
	}
      break;

    case LABEL_EXPR:
      t = TREE_OPERAND (node, 0);
      if (DECL_NONLOCAL (t))
	{
	  /* We cannot inline a function that receives a non-local goto
	     because we cannot remap the destination label used in the
	     function that is performing the non-local goto.  */
	  inline_forbidden_reason
	    = N_("%Jfunction %qF can never be inlined "
		 "because it receives a non-local goto");
	  return node;
	}
      break;

    case RECORD_TYPE:
    case UNION_TYPE:
      /* We cannot inline a function of the form

	   void F (int i) { struct S { int ar[i]; } s; }

	 Attempting to do so produces a catch-22.
	 If walk_tree examines the TYPE_FIELDS chain of RECORD_TYPE/
	 UNION_TYPE nodes, then it goes into infinite recursion on a
	 structure containing a pointer to its own type.  If it doesn't,
	 then the type node for S doesn't get adjusted properly when
	 F is inlined, and we abort in find_function_data.

	 ??? This is likely no longer true, but it's too late in the 4.0
	 cycle to try to find out.  This should be checked for 4.1.  */
      for (t = TYPE_FIELDS (node); t; t = TREE_CHAIN (t))
	if (variably_modified_type_p (TREE_TYPE (t), NULL))
	  {
	    inline_forbidden_reason
	      = N_("%Jfunction %qF can never be inlined "
		   "because it uses variable sized variables");
	    return node;
	  }

    default:
      break;
    }

  return NULL_TREE;
}

/* Return subexpression representing possible alloca call, if any.  */
static tree
inline_forbidden_p (tree fndecl)
{
  location_t saved_loc = input_location;
  block_stmt_iterator bsi;
  basic_block bb;
  tree ret = NULL_TREE;

  /* We should always be called after the CFG is built. */
  if (basic_block_info_for_function (DECL_STRUCT_FUNCTION (fndecl))== (varray_type) 0)
    {
      fprintf (stderr, "\nunlowered function:%s",
	       lang_hooks.decl_printable_name (fndecl, 2));
      abort ();
    }

  FOR_EACH_BB_FN (bb, DECL_STRUCT_FUNCTION (fndecl))
    for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
      {
	ret = walk_tree_without_duplicates (bsi_stmt_ptr (bsi), 
				    inline_forbidden_p_1, fndecl);
	if (ret)
	  goto egress;
      }

egress:
  input_location = saved_loc;
  return ret;
}

/* Returns nonzero if FN is a function that does not have any
   fundamental inline blocking properties.  */

static bool
inlinable_function_p (tree fn)
{
  bool inlinable = true;

  /* If we've already decided this function shouldn't be inlined,
     there's no need to check again.  */
  if (DECL_UNINLINABLE (fn))
    return false;

  /* See if there is any language-specific reason it cannot be
     inlined.  (It is important that this hook be called early because
     in C++ it may result in template instantiation.)
     If the function is not inlinable for language-specific reasons,
     it is left up to the langhook to explain why.  */
  inlinable = !lang_hooks.tree_inlining.cannot_inline_tree_fn (&fn);

  /* If we don't have the function body available, we can't inline it.
     However, this should not be recorded since we also get here for
     forward declared inline functions.  Therefore, return at once.  */
  if (!DECL_SAVED_TREE (fn))
    return false;

  /* If we're not inlining at all, then we cannot inline this function.  */
  else if (!flag_inline_trees)
    inlinable = false;

  /* Only try to inline functions if DECL_INLINE is set.  This should be
     true for all functions declared `inline', and for all other functions
     as well with -finline-functions.

     Don't think of disregarding DECL_INLINE when flag_inline_trees == 2;
     it's the front-end that must set DECL_INLINE in this case, because
     dwarf2out loses if a function that does not have DECL_INLINE set is
     inlined anyway.  That is why we have both DECL_INLINE and
     DECL_DECLARED_INLINE_P.  */
  /* FIXME: When flag_inline_trees dies, the check for flag_unit_at_a_time
	    here should be redundant.  */
  else if (!DECL_INLINE (fn) && !flag_unit_at_a_time)
    inlinable = false;

#ifdef INLINER_FOR_JAVA
  /* Synchronized methods can't be inlined.  This is a bug.  */
  else if (METHOD_SYNCHRONIZED (fn))
    inlinable = false;
#endif /* INLINER_FOR_JAVA */

  else if (inline_forbidden_p (fn))
    {
      /* See if we should warn about uninlinable functions.  Previously,
	 some of these warnings would be issued while trying to expand
	 the function inline, but that would cause multiple warnings
	 about functions that would for example call alloca.  But since
	 this a property of the function, just one warning is enough.
	 As a bonus we can now give more details about the reason why a
	 function is not inlinable.
	 We only warn for functions declared `inline' by the user.  */
      bool do_warning = (warn_inline
			 && DECL_INLINE (fn)
			 && DECL_DECLARED_INLINE_P (fn)
			 && !DECL_IN_SYSTEM_HEADER (fn));

      if (lookup_attribute ("always_inline", DECL_ATTRIBUTES (fn)))
	sorry (inline_forbidden_reason, fn, fn);
      else if (do_warning)
	warning (inline_forbidden_reason, fn, fn);

      inlinable = false;
    }

  /* Squirrel away the result so that we don't have to check again.  */
  DECL_UNINLINABLE (fn) = !inlinable;

  return inlinable;
}

/* Used by estimate_num_insns.  Estimate number of instructions seen
   by given statement.  */

static tree
estimate_num_insns_1 (tree *tp, int *walk_subtrees, void *data)
{
  int *count = data;
  tree x = *tp;

  if (IS_TYPE_OR_DECL_P (x))
    {
      *walk_subtrees = 0;
      return NULL;
    }
  /* Assume that constants and references counts nothing.  These should
     be majorized by amount of operations among them we count later
     and are common target of CSE and similar optimizations.  */
  else if (CONSTANT_CLASS_P (x) || REFERENCE_CLASS_P (x))
    return NULL;

  switch (TREE_CODE (x))
    {
    /* Containers have no cost.  */
    case TREE_LIST:
    case TREE_VEC:
    case BLOCK:
    case COMPONENT_REF:
    case BIT_FIELD_REF:
    case INDIRECT_REF:
    case ARRAY_REF:
    case ARRAY_RANGE_REF:
    case OBJ_TYPE_REF:
    case EXC_PTR_EXPR: /* ??? */
    case FILTER_EXPR: /* ??? */
    case COMPOUND_EXPR:
    case BIND_EXPR:
    case WITH_CLEANUP_EXPR:
    case NOP_EXPR:
    case VIEW_CONVERT_EXPR:
    case SAVE_EXPR:
    case ADDR_EXPR:
    case COMPLEX_EXPR:
    case RANGE_EXPR:
    case CASE_LABEL_EXPR:
    case SSA_NAME:
    case CATCH_EXPR:
    case EH_FILTER_EXPR:
    case STATEMENT_LIST:
    case ERROR_MARK:
    case NON_LVALUE_EXPR:
    case FDESC_EXPR:
    case VA_ARG_EXPR:
    case TRY_CATCH_EXPR:
    case TRY_FINALLY_EXPR:
    case LABEL_EXPR:
    case GOTO_EXPR:
    case RETURN_EXPR:
    case EXIT_EXPR:
    case LOOP_EXPR:
    case PHI_NODE:
    case WITH_SIZE_EXPR:
      break;

    /* We don't account constants for now.  Assume that the cost is amortized
       by operations that do use them.  We may re-consider this decision once
       we are able to optimize the tree before estimating it's size and break
       out static initializers.  */
    case IDENTIFIER_NODE:
    case INTEGER_CST:
    case REAL_CST:
    case COMPLEX_CST:
    case VECTOR_CST:
    case STRING_CST:
      *walk_subtrees = 0;
      return NULL;

    /* Recognize assignments of large structures and constructors of
       big arrays.  */
    case INIT_EXPR:
    case MODIFY_EXPR:
      x = TREE_OPERAND (x, 0);
      /* FALLTHRU */
    case TARGET_EXPR:
    case CONSTRUCTOR:
      {
	HOST_WIDE_INT size;

	size = int_size_in_bytes (TREE_TYPE (x));

	if (size < 0 || size > MOVE_MAX_PIECES * MOVE_RATIO)
	  *count += 10;
	else
	  *count += ((size + MOVE_MAX_PIECES - 1) / MOVE_MAX_PIECES);
      }
      break;

      /* Assign cost of 1 to usual operations.
	 ??? We may consider mapping RTL costs to this.  */
    case COND_EXPR:

    case PLUS_EXPR:
    case MINUS_EXPR:
    case MULT_EXPR:

    case FIX_TRUNC_EXPR:
    case FIX_CEIL_EXPR:
    case FIX_FLOOR_EXPR:
    case FIX_ROUND_EXPR:

    case NEGATE_EXPR:
    case FLOAT_EXPR:
    case MIN_EXPR:
    case MAX_EXPR:
    case ABS_EXPR:

    case LSHIFT_EXPR:
    case RSHIFT_EXPR:
    case LROTATE_EXPR:
    case RROTATE_EXPR:

    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
    case BIT_AND_EXPR:
    case BIT_NOT_EXPR:

    case TRUTH_ANDIF_EXPR:
    case TRUTH_ORIF_EXPR:
    case TRUTH_AND_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_XOR_EXPR:
    case TRUTH_NOT_EXPR:

    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
    case ORDERED_EXPR:
    case UNORDERED_EXPR:

    case UNLT_EXPR:
    case UNLE_EXPR:
    case UNGT_EXPR:
    case UNGE_EXPR:
    case UNEQ_EXPR:
    case LTGT_EXPR:

    case CONVERT_EXPR:

    case CONJ_EXPR:

    case PREDECREMENT_EXPR:
    case PREINCREMENT_EXPR:
    case POSTDECREMENT_EXPR:
    case POSTINCREMENT_EXPR:

    case SWITCH_EXPR:

    case ASM_EXPR:

    case RESX_EXPR:
      *count += 1;
      break;

    /* Few special cases of expensive operations.  This is useful
       to avoid inlining on functions having too many of these.  */
    case TRUNC_DIV_EXPR:
    case CEIL_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case ROUND_DIV_EXPR:
    case EXACT_DIV_EXPR:
    case TRUNC_MOD_EXPR:
    case CEIL_MOD_EXPR:
    case FLOOR_MOD_EXPR:
    case ROUND_MOD_EXPR:
    case RDIV_EXPR:
      *count += 10;
      break;
    case CALL_EXPR:
      {
	tree decl = get_callee_fndecl (x);

	if (decl && DECL_BUILT_IN (decl))
	  switch (DECL_FUNCTION_CODE (decl))
	    {
	    case BUILT_IN_CONSTANT_P:
	      *walk_subtrees = 0;
	      return NULL_TREE;
	    case BUILT_IN_EXPECT:
	      return NULL_TREE;
	    default:
	      break;
	    }
	*count += 10;
	break;
      }
    default:
      /* Abort here se we know we don't miss any nodes.  */
      gcc_unreachable ();
    }
  return NULL;
}

/* Estimate number of instructions that will be created by expanding EXPR.  */

int
estimate_num_insns (tree expr)
{
  int num = 0;
  struct pointer_set_t *visited_nodes;
  basic_block bb;
  block_stmt_iterator bsi;
  struct function *my_function;

  /* If we're given an entire function, walk the CFG.  */
  if (TREE_CODE (expr) == FUNCTION_DECL)
    {
      my_function = DECL_STRUCT_FUNCTION (expr);
      if (!my_function || !my_function->cfg || !ENTRY_BLOCK_PTR_FOR_FUNCTION (my_function))
	abort ();
      visited_nodes = pointer_set_create ();
      FOR_EACH_BB_FN (bb, my_function)
	{
	  for (bsi = bsi_start (bb);
	       !bsi_end_p (bsi);
	       bsi_next (&bsi))
	    {
	      walk_tree (bsi_stmt_ptr (bsi), estimate_num_insns_1,
			 &num, visited_nodes);
	    }
	}
      pointer_set_destroy (visited_nodes);
    }
  else
    walk_tree_without_duplicates (&expr, estimate_num_insns_1, &num);

  return num;
}

/* Initialized with NOGC, making this poisonous to the garbage collector.  */
static varray_type cfun_stack;

void
push_cfun (struct function *new_cfun)
{
  static bool initialized = false;
  
  if (!initialized)
    {
      VARRAY_GENERIC_PTR_NOGC_INIT (cfun_stack, 20, "cfun_stack");
      initialized = true;
    }
  VARRAY_PUSH_GENERIC_PTR (cfun_stack, cfun);
  cfun = new_cfun;
}

void
pop_cfun (void)
{
  cfun = (struct function *)VARRAY_TOP_GENERIC_PTR (cfun_stack);
  VARRAY_POP (cfun_stack);
}

/* Install new lexical TREE_BLOCK underneath 'current_block'.  */
static void
add_lexical_block (tree current_block, tree new_block)
{
  tree *blk_p;
  
  /* Walk to the last sub-block.  */
  for (blk_p = &BLOCK_SUBBLOCKS (current_block);
       *blk_p;
       blk_p = &TREE_CHAIN (*blk_p))
    ;
  *blk_p = new_block;
  BLOCK_SUPERCONTEXT (new_block) = current_block;
  BLOCK_SUBBLOCKS (new_block) = NULL_TREE;
}

/* If *TP is a CALL_EXPR, replace it with its inline expansion.  */

static bool
expand_call_inline (tree *tp, int *walk_subtrees, void *data)
{
  inline_data *id;
  tree t;
  tree use_retvar;
  tree decl;
  tree fn;
  tree arg_inits;
  splay_tree st;
  tree args;
  tree return_slot_addr;
  tree modify_dest;
  location_t saved_location;
  struct cgraph_edge *cg_edge;
  const char *reason;
  basic_block head_copied_body;
  basic_block tail_copied_body;
  basic_block first_half_bb;
  basic_block second_half_bb;
  basic_block orig_next_bb;
  struct function *my_cfun;
  edge e_step;
  edge_iterator ei;
  edge e;
  tree_stmt_iterator test_iter;
  block_stmt_iterator bsi;
  tree tmp_stmt_list;
  tree dup_fndecl;
  bool successfully_inlined = FALSE;
  tree t_step;
  tree var;
  struct cgraph_node *old_node;

  /* See what we've got.  */
  id = (inline_data *) data;
  t = *tp;

  /* Set input_location here so we get the right instantiation context
     if we call instantiate_decl from inlinable_function_p.  */
  saved_location = input_location;
  if (EXPR_HAS_LOCATION (t))
    input_location = EXPR_LOCATION (t);

  /* Recurse, but letting recursive invocations know that we are
     inside the body of a TARGET_EXPR.  */
  if (TREE_CODE (*tp) == TARGET_EXPR)
    {
#if 0
      int i, len = TREE_CODE_LENGTH (TARGET_EXPR);

      /* We're walking our own subtrees.  */
      *walk_subtrees = 0;

      /* Actually walk over them.  This loop is the body of
	 walk_trees, omitting the case where the TARGET_EXPR
	 itself is handled.  */
      for (i = 0; i < len; ++i)
	{
	  if (i == 2)
	    ++id->in_target_cleanup_p;
	  walk_tree (&TREE_OPERAND (*tp, i), expand_call_inline, data,
		     id->tree_pruner);
	  if (i == 2)
	    --id->in_target_cleanup_p;
	}

      goto egress;
#endif
    }

  if (TYPE_P (t))
    /* Because types were not copied in copy_body, CALL_EXPRs beneath
       them should not be expanded.  This can happen if the type is a
       dynamic array type, for example.  */
    *walk_subtrees = 0;

  /* From here on, we're only interested in CALL_EXPRs.  */
  if (TREE_CODE (t) != CALL_EXPR)
    goto egress;

  /* First, see if we can figure out what function is being called.
     If we cannot, then there is no hope of inlining the function.  */
  fn = get_callee_fndecl (t);
  if (!fn)
    goto egress;

  /* Turn forward declarations into real ones.  */
  fn = cgraph_node (fn)->decl;

  /* If fn is a declaration of a function in a nested scope that was
     globally declared inline, we don't set its DECL_INITIAL.
     However, we can't blindly follow DECL_ABSTRACT_ORIGIN because the
     C++ front-end uses it for cdtors to refer to their internal
     declarations, that are not real functions.  Fortunately those
     don't have trees to be saved, so we can tell by checking their
     DECL_SAVED_TREE.  */
  if (! DECL_INITIAL (fn)
      && DECL_ABSTRACT_ORIGIN (fn)
      && DECL_SAVED_TREE (DECL_ABSTRACT_ORIGIN (fn)))
    fn = DECL_ABSTRACT_ORIGIN (fn);

  /* Objective C and fortran still calls tree_rest_of_compilation directly.
     Kill this check once this is fixed.  */
  if (!id->current_node->analyzed)
    goto egress;

  cg_edge = cgraph_edge (id->current_node, t);

  /* Constant propagation on argument done during previous inlining
     may create new direct call.  Produce an edge for it.  */
  if (!cg_edge)
    {
      struct cgraph_node *dest = cgraph_node (fn);

      /* We have missing edge in the callgraph.  This can happen in one case
         where previous inlining turned indirect call into direct call by
         constant propagating arguments.  In all other cases we hit a bug
         (incorrect node sharing is most common reason for missing edges.  */
      gcc_assert (dest->needed || !flag_unit_at_a_time);
      cgraph_create_edge (id->node, dest, t)->inline_failed
	= N_("originally indirect function call not considered for inlining");
      goto egress;
    }

  /* Don't try to inline functions that are not well-suited to
     inlining.  */
  if (!cgraph_inline_p (cg_edge, &reason))
    {
      if (lookup_attribute ("always_inline", DECL_ATTRIBUTES (fn)))
	{
	  sorry ("%Jinlining failed in call to %qF: %s", fn, fn, reason);
	  sorry ("called from here");
	}
      else if (warn_inline && DECL_DECLARED_INLINE_P (fn)
	       && !DECL_IN_SYSTEM_HEADER (fn)
	       && strlen (reason)
	       && !lookup_attribute ("noinline", DECL_ATTRIBUTES (fn)))
	{
	  warning ("%Jinlining failed in call to %qF: %s", fn, fn, reason);
	  warning ("called from here");
	}
      goto egress;
    }

#ifdef ENABLE_CHECKING
  if (cg_edge->callee->decl != id->node->decl)
    verify_cgraph_node (cg_edge->callee);
#endif

  /* We will be inlining this callee.  */

  /* Register specific tree functions.  */
  tree_register_cfg_hooks ();

  /* Build a block containing code to initialize the arguments, the
     actual inline expansion of the body, and a label for the return
     statements within the function to jump to.  The type of the
     statement expression is the return type of the function call.  */
  id->bind_expr = build (BIND_EXPR, TREE_TYPE (TREE_TYPE (fn)), NULL_TREE,
		alloc_stmt_list (), make_node (BLOCK));
  BLOCK_ABSTRACT_ORIGIN (BIND_EXPR_BLOCK (id->bind_expr)) = fn;
  {
    tree tmp;
    test_iter = id->original_tsi;
    tmp = tsi_stmt (test_iter);
    add_lexical_block (TREE_BLOCK (tmp), BIND_EXPR_BLOCK (id->bind_expr));
  }

  /* Local declarations will be replaced by their equivalents in this
     map.  */
  st = id->decl_map;
  id->decl_map = splay_tree_new (splay_tree_compare_pointers,
				 NULL, NULL);

  /* Initialize the parameters.  */
  args = TREE_OPERAND (t, 1);
  if (CALL_EXPR_HAS_RETURN_SLOT_ADDR (t))
    {
      return_slot_addr = TREE_VALUE (args);
      args = TREE_CHAIN (args);
      TREE_TYPE (id->bind_expr) = void_type_node;
    }
  else
    return_slot_addr = NULL_TREE;

  arg_inits = initialize_inlined_parameters (id, args, TREE_OPERAND (t, 2),
					     fn, id->bind_expr);
  /* Record the function we are about to inline.  */
  id->callee = fn;

  /* Return statements in the function body will be replaced by jumps
     to the RET_LABEL.  */
  id->ret_label = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
  DECL_ARTIFICIAL (id->ret_label) = 1;
  DECL_CONTEXT (id->ret_label) = id->caller;
  insert_decl_map (id, id->ret_label, id->ret_label);

  gcc_assert (DECL_INITIAL (fn));
  gcc_assert (TREE_CODE (DECL_INITIAL (fn)) == BLOCK);

  /* Find the lhs to which the result of this call is assigned.  */
  modify_dest = tsi_stmt (id->original_tsi);
  if (TREE_CODE (modify_dest) == MODIFY_EXPR)
    modify_dest = TREE_OPERAND (modify_dest, 0);
  else if (TREE_CODE (modify_dest) == RETURN_EXPR
	   && TREE_CODE (TREE_OPERAND (modify_dest, 0)) == MODIFY_EXPR)
    modify_dest = TREE_OPERAND (TREE_OPERAND (modify_dest, 0), 0);
  else
    modify_dest = NULL;

  /* Declare the return variable for the function.  */
  decl = declare_return_variable (id, return_slot_addr,
				  modify_dest, &use_retvar);
  /* Do this only if declare_return_variable created a new one.  */
  if (decl && !return_slot_addr && decl != modify_dest)
    declare_inline_vars (id->bind_expr, decl);

  /* RETURN_EXPRs will morph into fallthrough edges to this block.  */
  id->return_block = alloc_block ();

  /* After we've initialized the parameters, we insert the body of the
     function itself.  */
  old_node = id->current_node;

  /* Anoint the callee-to-be-duplicated as the "current_node."  When
     CALL_EXPRs within callee are duplicated, the edges from callee to
     callee's callees (caller's grandchildren) will be cloned.  */
  id->current_node = cg_edge->callee;

  /* Record the EH edges leaving the caller's bb. */
  if (!eh_dst)
    VARRAY_BB_INIT (eh_dst, 10, "exception handling destination blocks");
  FOR_EACH_EDGE (e_step, ei, id->oic_basic_block->succs)
    if (e_step->flags & EDGE_EH)
      VARRAY_PUSH_BB (eh_dst, e_step->dest);

  /* This is it.  Duplicate the callee body.  Assume callee is
     pre-gimplified.  Note that we must not alter the caller
     function in any way before this point, as this CALL_EXPR may be
     a self-referential call; if we're calling ourselves, we need to
     duplicate our body before altering anything.  */
  dup_fndecl = copy_body (id, id->oic_basic_block->count, id->oic_basic_block->frequency);
  id->current_node = old_node;
  my_cfun = DECL_STRUCT_FUNCTION (dup_fndecl);
  /* Append copied EH information to that of current function.  */
  duplicate_eh_regions (my_cfun, id, false);

  if (arg_inits)
    {
      edge new;
      head_copied_body = create_basic_block (arg_inits, (void*)0, ENTRY_BLOCK_PTR_FOR_FUNCTION (my_cfun));
      head_copied_body->count = id->oic_basic_block->count;
      head_copied_body->frequency = id->oic_basic_block->frequency;
      set_bb_for_stmt (arg_inits, head_copied_body);
      new = make_edge (head_copied_body, head_copied_body->next_bb, EDGE_FALLTHRU);
      new->probability = REG_BR_PROB_BASE;
      new->count = id->oic_basic_block->count;
    }
  else
    head_copied_body = ENTRY_BLOCK_PTR_FOR_FUNCTION (my_cfun)->next_bb;
  /* Backup from exit block.  */
  tail_copied_body = EXIT_BLOCK_PTR_FOR_FUNCTION (my_cfun)->prev_bb;

  /* FIXME this should be using split_block; however getting it to work
     that way is not that easy...  */
  /* Split the block holding the CALL_EXPR.  */

  first_half_bb = id->oic_basic_block;
  orig_next_bb = first_half_bb->next_bb;
  test_iter = id->original_tsi;
  tsi_prev (&test_iter);
  /* Split the block holding the CALL_EXPR.  Everything up to the
     CALL_EXPR goes into the first_half_bb; the CALL_EXPR itself and
     everything else goes to the second_half_bb.  The duplicated
     callee body will be inserted between these, and the CALL_EXPR
     will be replaced with a reference to the return variable (if
     any).  tsi_split_statement_list_before() loses if the CALL_EXPR
     is the first statement. 
     FIXME: Is this hand implementation of split_basic_block?  */
  if (!tsi_end_p (test_iter))
    {
      second_half_bb
	= create_basic_block (tsi_split_statement_list_before (&id->original_tsi),
			      (void*)0, first_half_bb);
    }
  else
    {
      /* No predecessor statement; create new block after current block,
	 move statement list into new block.  */
      second_half_bb = create_basic_block ((void*)0, (void*)0, first_half_bb);
      tmp_stmt_list = second_half_bb->stmt_list;
      second_half_bb->stmt_list = first_half_bb->stmt_list;
      first_half_bb->stmt_list = tmp_stmt_list;
      set_bb_for_stmt (first_half_bb->stmt_list, first_half_bb);
    }
  /* FIXME: The function call might not return each time.  We can scale the
     other BB frequency by ratio of exit and entry block frequency of inlined
     function.  */
  second_half_bb->count = first_half_bb->count;
  second_half_bb->frequency = first_half_bb->frequency;

  /* The existing id->return_block is a just a place to collect the
     edges created from RETURN_EXPRs.  Move those edges, if any, to
     the real basic block where they should go, and abandon the
     id->return_block.  */
  second_half_bb->preds = id->return_block->preds;
  FOR_EACH_EDGE (e_step, ei, id->return_block->preds)
    e_step->dest = second_half_bb;

  /* Can we free this?  */
  id->return_block = (basic_block)0;

  /* If we found one or more RETURN_EXPRs that have become edges,
     create, hang, and bless the label.  */
  if (TREE_USED (id->ret_label) && id->ret_count)
    {
      tree label = build1 (LABEL_EXPR, void_type_node, id->ret_label);
      tree_stmt_iterator tsi = tsi_start (second_half_bb->stmt_list);
      tsi_link_before (&tsi, label, TSI_NEW_STMT);
    }
  else	/* We didn't need this label after all.  */
    TREE_USED (id->ret_label) = 0;

  set_bb_for_stmt (second_half_bb->stmt_list, second_half_bb);

  /* Fudge successor edges leaving "first_half_bb"; make them leave
     "second_half_bb" instead.  Edges entering first_half_bb are
     correct as-is.  If there are exception-handling edge(s),
     remove them; those are handled separately elsewhere.  */
  second_half_bb->succs = first_half_bb->succs;
  for (ei = ei_start (second_half_bb->succs);
       (e_step = ei_safe_edge (ei)); )
    {
      e_step->src = second_half_bb;
      if (e_step->flags & EDGE_EH)
	remove_edge (e_step);
      else
	ei_next (&ei);
    }

  /* We'll fix this when we splice in the new body.  */
  first_half_bb->succs = NULL;

  e = make_edge (first_half_bb, head_copied_body, EDGE_FALLTHRU);
  e->count = first_half_bb->count;
  e->probability = REG_BR_PROB_BASE;

  n_edges += n_edges_for_function (my_cfun);

  /* Clean up.  */
  splay_tree_delete (id->decl_map);
  id->decl_map = st;
  set_eh_cur_region (cfun, 0);

  /* Although, from the semantic viewpoint, the new expression has
     side-effects only if the old one did, it is not possible, from
     the technical viewpoint, to evaluate the body of a function
     multiple times without serious havoc.  */
  TREE_SIDE_EFFECTS (id->bind_expr) = 1;

  /* Splice the duplicated callee body into the caller.  */
  first_half_bb->next_bb = head_copied_body;
  head_copied_body->prev_bb = first_half_bb;
  tail_copied_body->next_bb = second_half_bb;
  second_half_bb->prev_bb = tail_copied_body;

  /* If the inlined function returns a result that we care about,
     clobber the CALL_EXPR with a reference to the return variable.  */
  if (use_retvar && (TREE_CODE (tsi_stmt (id->original_tsi)) != CALL_EXPR))
    *tp = use_retvar;
  else
    /* We're modifying a TSI owned by gimple_expand_calls_inline();
       tsi_delink() will leave the iterator in a sane state.  */
    tsi_delink (&id->original_tsi);

  /* If this block had an AB,EH (abnormal, exception-handling) edge,
     scan the first & second half blocks to see if either one still
     needs such an edge.  If this block had no such edge, neither of
     the first/second blocks will; this is the "usual case."  */
  if (VARRAY_ACTIVE_SIZE (eh_dst) > 0)
  {
    int eh_dst_index;
    for (bsi = bsi_start (first_half_bb) ; !bsi_end_p (bsi) ; bsi_next (&bsi))
      {
	if (tree_could_throw_p (bsi_stmt (bsi)))
	  {
	    for (eh_dst_index = VARRAY_ACTIVE_SIZE (eh_dst); eh_dst_index; )
	      {
		eh_dst_index--;
		make_edge (first_half_bb, VARRAY_BB (eh_dst, eh_dst_index),
			   EDGE_ABNORMAL | EDGE_EH);
	      }
	    break;
	  }
      }

    for (bsi = bsi_start (second_half_bb) ; !bsi_end_p (bsi) ; bsi_next (&bsi))
      {
	if (tree_could_throw_p (bsi_stmt (bsi)) || TREE_CODE (bsi_stmt (bsi)) == RESX_EXPR)
	  {
	    for (eh_dst_index = VARRAY_ACTIVE_SIZE (eh_dst); eh_dst_index; )
	      {
		eh_dst_index--;
		make_edge (second_half_bb, VARRAY_BB (eh_dst, eh_dst_index),
			   EDGE_ABNORMAL | EDGE_EH);
	      }
	    break;
	  }
      }
  }
  VARRAY_CLEAR (eh_dst);

  /* If the value of the new expression is ignored, that's OK.  We
     don't warn about this for CALL_EXPRs, so we shouldn't warn about
     the equivalent inlined version either.  */
  TREE_USED (*tp) = 1;

  /* Update callgraph if needed.  */
  cgraph_remove_node (cg_edge->callee);

  /* Don't walk into subtrees.  We've already handled them above.  */
  *walk_subtrees = 0;

  /* Declare the 'auto' variables added with this inlined body.  */
  record_vars (BIND_EXPR_VARS (id->bind_expr));
  id->bind_expr = NULL_TREE;

  /* Add local static vars in this inlined callee to caller.  */
  for (t_step = my_cfun->unexpanded_var_list;
       t_step;
       t_step = TREE_CHAIN (t_step))
    {
      var = TREE_VALUE (t_step);
      if (TREE_STATIC (var) && !TREE_ASM_WRITTEN (var))
	record_vars (var);
    }

  successfully_inlined = TRUE;

 egress:
  input_location = saved_location;
  return successfully_inlined;
}

/* Expand call statements reachable from STMT_P.
   We can only have CALL_EXPRs as the "toplevel" tree code or nested
   in a MODIFY_EXPR.  See tree-gimple.c:get_call_expr_in().  We can
   unfortunately not use that function here because we need a pointer
   to the CALL_EXPR, not the tree itself.  */

static bool
gimple_expand_calls_inline (tree *stmt_p, inline_data *id)
{
  tree stmt = *stmt_p, orig_stmt = stmt;
  enum tree_code code = TREE_CODE (stmt);
  int dummy;
  bool successful_inline = FALSE;
  bool inline_status_a = FALSE;
  bool inline_status_b = FALSE;

  switch (code)
    {
    case STATEMENT_LIST:
      {
	/* tree_stmt_iterator i; */
	tree new;
	tree_stmt_iterator preserved_tsi;
	bool restore_original_tsi;

	/* If we discover a nested STATEMENT_LIST, preserve the current one.  */
	restore_original_tsi = !tsi_end_p (id->original_tsi);
	preserved_tsi = id->original_tsi;
	/* for (i = tsi_start (stmt); !tsi_end_p (i); )  */
	for (id->original_tsi = tsi_start (stmt); !tsi_end_p (id->original_tsi);)
	  {
	    /* After any successful inlining operation,
	       id->original_tsi will wind up in the block *after* the
	       newly-inlined body; since our caller is trudging
	       *forwards* through the blocks, the next block it visits 
	       will be the first newly-inlined block, and when it's 
	       done with those, it will finally re-visit the block
	       where our id->original_tsi lives.  The current block
	       will be split at the stastement owning the CALL that got
	       inlined, so there won't be any more CALL_EXPRs to
	       discover in this block.  */
	    tree original_stmt = tsi_stmt (id->original_tsi);
	    successful_inline = gimple_expand_calls_inline 
		    (tsi_stmt_ptr (id->original_tsi), id);
	    if (successful_inline)
	      {
	        if (lookup_stmt_eh_region (original_stmt) >= 0)
		  remove_stmt_from_eh_region (original_stmt);
	        return TRUE;
	      }

	    new = tsi_stmt (id->original_tsi);
	    gcc_assert (TREE_CODE (new) != STATEMENT_LIST);
	    tsi_next (&id->original_tsi);
	  }
	if (restore_original_tsi)
	  id->original_tsi = preserved_tsi;
      }
      break;

    case BIND_EXPR:
    case COMPOUND_EXPR:
    case CATCH_EXPR:
    case EH_FILTER_EXPR:
    case TRY_CATCH_EXPR:
    case TRY_FINALLY_EXPR:
      /* We're gimple.  We should have gotten rid of all these.  */
      gcc_unreachable ();

    case MODIFY_EXPR:
      stmt_p = &TREE_OPERAND (stmt, 1);
      stmt = *stmt_p;
      if (TREE_CODE (stmt) == WITH_SIZE_EXPR)
	{
	  stmt_p = &TREE_OPERAND (stmt, 0);
	  stmt = *stmt_p;
	}
      if (TREE_CODE (stmt) != CALL_EXPR)
	break;

      /* FALLTHRU */

    case CALL_EXPR:
      /* Declare the current region, if any, so statements from callee
	 that throw will be caught by the current handler.  */
      set_eh_cur_region (cfun, lookup_stmt_eh_region (orig_stmt));
      successful_inline = expand_call_inline (stmt_p, &dummy, id);
      /* If call was in an EH region but could not throw, fix 
	 EH data structs.  */
      if (lookup_stmt_eh_region (orig_stmt) >= 0 
	  && !tree_could_throw_p (orig_stmt))
	remove_stmt_from_eh_region (orig_stmt);
      return successful_inline;
      break;

    default:
      break;
    }

  return (inline_status_a || inline_status_b || successful_inline) ? TRUE: FALSE;
}

/* Expand calls to inline functions in the body of FN.  */

void
optimize_inline_calls (tree fn)
{
  inline_data id;
  tree prev_fn;
  /* There is no point in performing inlining if errors have already
     occurred -- and we might crash if we try to inline invalid
     code.  */
  if (errorcount || sorrycount)
    return;

  /* Clear out ID.  */
  memset (&id, 0, sizeof (id));

  id.current_node = id.node = cgraph_node (fn);
  id.caller = fn;
  /* Or any functions that aren't finished yet.  */
  prev_fn = NULL_TREE;
  if (current_function_decl)
    {
      id.caller = current_function_decl;
      prev_fn = current_function_decl;
    }

  /* Reach the trees by walking over the CFG, and note the
     enclosing basic-blocks in the call edges.  */
  /* We walk the blocks going forward, because inlined function bodies
     will split id->current_basic_block, and the new blocks will
     follow it; we'll trudge through them, processing their CALL_EXPRs
     along the way.  */
  FOR_EACH_BB (id.oic_basic_block)
    {
      if (id.oic_basic_block->stmt_list)
	(void) gimple_expand_calls_inline (&id.oic_basic_block->stmt_list,
					   &id);
    }

  /* Renumber the (code) basic_blocks consecutively.  */
  compact_blocks ();
  /* Renumber the lexical scoping (non-code) blocks consecutively.  */
  number_blocks (fn);

#ifdef ENABLE_CHECKING
    {
      struct cgraph_edge *e;

      verify_cgraph_node (id.node);

      /* Double check that we inlined everything we are supposed to inline.  */
      for (e = id.node->callees; e; e = e->next_callee)
	gcc_assert (e->inline_failed);
    }
#endif
  /* We need to rescale frequencies again to peak at REG_BR_PROB_BASE as inlining
     loops might increase the maximum.  */
  if (ENTRY_BLOCK_PTR->count)
    counts_to_freqs ();
  /* Inlining a function body that contains a call to abort() can
     render some caller code "unreachable", as the CFG-builder knows
     that abort() never returns.  Such unreachable code will cause
     great distress in the early phases of the SSA optimizer, so we
     discard it here.  Of course, deleting blocks can delete calls,
     and this will cause great distress in verify_cgraph_node(), so we
     do that first.  */
  cleanup_tree_cfg ();
}

/* FN is a function that has a complete body, and CLONE is a function whose
   body is to be set to a copy of FN, mapping argument declarations according
   to the ARG_MAP splay_tree.  */

void
clone_body (tree clone, tree fn, void *arg_map)
{
  inline_data id;

  /* Clone the body, as if we were making an inline call.  But, remap the
     parameters in the callee to the parameters of caller.  */
  memset (&id, 0, sizeof (id));
  id.caller = clone;
  id.callee = fn;
  id.callee_cfun = DECL_STRUCT_FUNCTION (fn);
  id.decl_map = (splay_tree)arg_map;

  /* Cloning is treated slightly differently from inlining.  Set
     CLONING_P so that it's clear which operation we're performing.  */
  id.cloning_p = true;

  /* We're not inside any EH region.  */
  set_eh_cur_region (cfun, -1);

  /* Actually copy the body.  */
  append_to_statement_list_force (copy_generic_body (&id), &DECL_SAVED_TREE (clone));
}

/* Save duplicate body in FN.  MAP is used to pass around splay tree
   used to update argments in restore_body.  */

/* Make and return duplicate of body in FN.  Put copies of DECL_ARGUMENTS
   in *arg_copy and of the static chain, if any, in *sc_copy.  */

tree
save_body (tree fn, tree *arg_copy, tree *sc_copy)
{
  inline_data id;
  tree body, *parg;

  memset (&id, 0, sizeof (id));
  id.callee = fn;
  id.callee_cfun = DECL_STRUCT_FUNCTION (fn);
  id.caller = fn;
  id.node = cgraph_node (fn);
  id.saving_p = true;
  id.decl_map = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);
  *arg_copy = DECL_ARGUMENTS (fn);

  for (parg = arg_copy; *parg; parg = &TREE_CHAIN (*parg))
    {
      tree new = copy_node (*parg);

      lang_hooks.dup_lang_specific_decl (new);
      DECL_ABSTRACT_ORIGIN (new) = DECL_ORIGIN (*parg);
      insert_decl_map (&id, *parg, new);
      TREE_CHAIN (new) = TREE_CHAIN (*parg);
      *parg = new;
    }

  *sc_copy = DECL_STRUCT_FUNCTION (fn)->static_chain_decl;
  if (*sc_copy)
    {
      tree new = copy_node (*sc_copy);

      lang_hooks.dup_lang_specific_decl (new);
      DECL_ABSTRACT_ORIGIN (new) = DECL_ORIGIN (*sc_copy);
      insert_decl_map (&id, *sc_copy, new);
      TREE_CHAIN (new) = TREE_CHAIN (*sc_copy);
      *sc_copy = new;
    }

  /* We're not inside any EH region.  */
  set_eh_cur_region (cfun, -1);

  insert_decl_map (&id, DECL_RESULT (fn), DECL_RESULT (fn));

  /* Actually copy the body, including a new (struct function *) and CFG.
     EH info is also duplicated so its labels point into the copied
     CFG, not the original.  */
  body = copy_body (&id,
      		    ENTRY_BLOCK_PTR_FOR_FUNCTION (DECL_STRUCT_FUNCTION (fn))->count,
      		    ENTRY_BLOCK_PTR_FOR_FUNCTION (DECL_STRUCT_FUNCTION (fn))->frequency);
  DECL_STRUCT_FUNCTION (fn)->saved_cfg = DECL_STRUCT_FUNCTION (body)->cfg;
  DECL_STRUCT_FUNCTION (fn)->saved_eh = DECL_STRUCT_FUNCTION (body)->eh;

  /* Clean up.  */
  splay_tree_delete (id.decl_map);
  return body;
}

#define WALK_SUBTREE(NODE)				\
  do							\
    {							\
      result = walk_tree (&(NODE), func, data, pset);	\
      if (result)					\
	return result;					\
    }							\
  while (0)

/* This is a subroutine of walk_tree that walks field of TYPE that are to
   be walked whenever a type is seen in the tree.  Rest of operands and return
   value are as for walk_tree.  */

static tree
walk_type_fields (tree type, walk_tree_fn func, void *data,
		  struct pointer_set_t *pset)
{
  tree result = NULL_TREE;

  switch (TREE_CODE (type))
    {
    case POINTER_TYPE:
    case REFERENCE_TYPE:
      /* We have to worry about mutually recursive pointers.  These can't
	 be written in C.  They can in Ada.  It's pathological, but
	 there's an ACATS test (c38102a) that checks it.  Deal with this
	 by checking if we're pointing to another pointer, that one
	 points to another pointer, that one does too, and we have no htab.
	 If so, get a hash table.  We check three levels deep to avoid
	 the cost of the hash table if we don't need one.  */
      if (POINTER_TYPE_P (TREE_TYPE (type))
	  && POINTER_TYPE_P (TREE_TYPE (TREE_TYPE (type)))
	  && POINTER_TYPE_P (TREE_TYPE (TREE_TYPE (TREE_TYPE (type))))
	  && !pset)
	{
	  result = walk_tree_without_duplicates (&TREE_TYPE (type),
						 func, data);
	  if (result)
	    return result;

	  break;
	}

      /* ... fall through ... */

    case COMPLEX_TYPE:
      WALK_SUBTREE (TREE_TYPE (type));
      break;

    case METHOD_TYPE:
      WALK_SUBTREE (TYPE_METHOD_BASETYPE (type));

      /* Fall through.  */

    case FUNCTION_TYPE:
      WALK_SUBTREE (TREE_TYPE (type));
      {
	tree arg;

	/* We never want to walk into default arguments.  */
	for (arg = TYPE_ARG_TYPES (type); arg; arg = TREE_CHAIN (arg))
	  WALK_SUBTREE (TREE_VALUE (arg));
      }
      break;

    case ARRAY_TYPE:
      /* Don't follow this nodes's type if a pointer for fear that we'll
	 have infinite recursion.  Those types are uninteresting anyway.  */
      if (!POINTER_TYPE_P (TREE_TYPE (type))
	  && TREE_CODE (TREE_TYPE (type)) != OFFSET_TYPE)
	WALK_SUBTREE (TREE_TYPE (type));
      WALK_SUBTREE (TYPE_DOMAIN (type));
      break;

    case BOOLEAN_TYPE:
    case ENUMERAL_TYPE:
    case INTEGER_TYPE:
    case CHAR_TYPE:
    case REAL_TYPE:
      WALK_SUBTREE (TYPE_MIN_VALUE (type));
      WALK_SUBTREE (TYPE_MAX_VALUE (type));
      break;

    case OFFSET_TYPE:
      WALK_SUBTREE (TREE_TYPE (type));
      WALK_SUBTREE (TYPE_OFFSET_BASETYPE (type));
      break;

    default:
      break;
    }

  return NULL_TREE;
}

/* Apply FUNC to all the sub-trees of TP in a pre-order traversal.  FUNC is
   called with the DATA and the address of each sub-tree.  If FUNC returns a
   non-NULL value, the traversal is aborted, and the value returned by FUNC
   is returned.  If PSET is non-NULL it is used to record the nodes visited,
   and to avoid visiting a node more than once.  */

tree
walk_tree (tree *tp, walk_tree_fn func, void *data, struct pointer_set_t *pset)
{
  enum tree_code code;
  int walk_subtrees;
  tree result;

#define WALK_SUBTREE_TAIL(NODE)				\
  do							\
    {							\
       tp = & (NODE);					\
       goto tail_recurse;				\
    }							\
  while (0)

 tail_recurse:
  /* Skip empty subtrees.  */
  if (!*tp)
    return NULL_TREE;

  /* Don't walk the same tree twice, if the user has requested
     that we avoid doing so.  */
  if (pset && pointer_set_insert (pset, *tp))
    return NULL_TREE;

  /* Call the function.  */
  walk_subtrees = 1;
  result = (*func) (tp, &walk_subtrees, data);

  /* If we found something, return it.  */
  if (result)
    return result;

  code = TREE_CODE (*tp);

  /* Even if we didn't, FUNC may have decided that there was nothing
     interesting below this point in the tree.  */
  if (!walk_subtrees)
    {
      if (code == TREE_LIST)
	/* But we still need to check our siblings.  */
	WALK_SUBTREE_TAIL (TREE_CHAIN (*tp));
      else
	return NULL_TREE;
    }

  result = lang_hooks.tree_inlining.walk_subtrees (tp, &walk_subtrees, func,
						   data, pset);
  if (result || ! walk_subtrees)
    return result;

  /* If this is a DECL_EXPR, walk into various fields of the type that it's
     defining.  We only want to walk into these fields of a type in this
     case.  Note that decls get walked as part of the processing of a
     BIND_EXPR.

     ??? Precisely which fields of types that we are supposed to walk in
     this case vs. the normal case aren't well defined.  */
  if (code == DECL_EXPR
      && TREE_CODE (DECL_EXPR_DECL (*tp)) == TYPE_DECL
      && TREE_CODE (TREE_TYPE (DECL_EXPR_DECL (*tp))) != ERROR_MARK)
    {
      tree *type_p = &TREE_TYPE (DECL_EXPR_DECL (*tp));

      /* Call the function for the type.  See if it returns anything or
	 doesn't want us to continue.  If we are to continue, walk both
	 the normal fields and those for the declaration case.  */
      result = (*func) (type_p, &walk_subtrees, data);
      if (result || !walk_subtrees)
	return NULL_TREE;

      result = walk_type_fields (*type_p, func, data, pset);
      if (result)
	return result;

      WALK_SUBTREE (TYPE_SIZE (*type_p));
      WALK_SUBTREE (TYPE_SIZE_UNIT (*type_p));

      /* If this is a record type, also walk the fields.  */
      if (TREE_CODE (*type_p) == RECORD_TYPE
	  || TREE_CODE (*type_p) == UNION_TYPE
	  || TREE_CODE (*type_p) == QUAL_UNION_TYPE)
	{
	  tree field;

	  for (field = TYPE_FIELDS (*type_p); field;
	       field = TREE_CHAIN (field))
	    {
	      /* We'd like to look at the type of the field, but we can easily
		 get infinite recursion.  So assume it's pointed to elsewhere
		 in the tree.  Also, ignore things that aren't fields.  */
	      if (TREE_CODE (field) != FIELD_DECL)
		continue;

	      WALK_SUBTREE (DECL_FIELD_OFFSET (field));
	      WALK_SUBTREE (DECL_SIZE (field));
	      WALK_SUBTREE (DECL_SIZE_UNIT (field));
	      if (TREE_CODE (*type_p) == QUAL_UNION_TYPE)
		WALK_SUBTREE (DECL_QUALIFIER (field));
	    }
	}
    }

  else if (code != SAVE_EXPR
	   && code != BIND_EXPR
	   && IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (code)))
    {
      int i, len;

      /* Walk over all the sub-trees of this operand.  */
      len = TREE_CODE_LENGTH (code);
      /* TARGET_EXPRs are peculiar: operands 1 and 3 can be the same.
	 But, we only want to walk once.  */
      if (code == TARGET_EXPR
	  && TREE_OPERAND (*tp, 3) == TREE_OPERAND (*tp, 1))
	--len;

      /* Go through the subtrees.  We need to do this in forward order so
         that the scope of a FOR_EXPR is handled properly.  */
#ifdef DEBUG_WALK_TREE
      for (i = 0; i < len; ++i)
	WALK_SUBTREE (TREE_OPERAND (*tp, i));
#else
      for (i = 0; i < len - 1; ++i)
	WALK_SUBTREE (TREE_OPERAND (*tp, i));

      if (len)
	{
	  /* The common case is that we may tail recurse here.  */
	  if (code != BIND_EXPR
	      && !TREE_CHAIN (*tp))
	    WALK_SUBTREE_TAIL (TREE_OPERAND (*tp, len - 1));
	  else
	    WALK_SUBTREE (TREE_OPERAND (*tp, len - 1));
	}
#endif
      
	/* Check our siblings.  */
      /* 	WALK_SUBTREE_TAIL (TREE_TYPE (*tp)); */
    }

  /* If this is a type, walk the needed fields in the type.  */
  else if (TYPE_P (*tp))
    {
      result = walk_type_fields (*tp, func, data, pset);
      if (result)
	return result;
    }
  else
    {

      if (TREE_CODE_CLASS (code) == 't')
	{
	  WALK_SUBTREE (TYPE_SIZE (*tp));
	  WALK_SUBTREE (TYPE_SIZE_UNIT (*tp));
	  /* Also examine variaous special fields, below.  */
	}

      /* Not one of the easy cases.  We must explicitly go through the
	 children.  */
      switch (code)
	{
	case ERROR_MARK:
	case IDENTIFIER_NODE:
	case INTEGER_CST:
	case REAL_CST:
	case VECTOR_CST:
	case STRING_CST:
	case REAL_TYPE:
	case COMPLEX_TYPE:
	case VECTOR_TYPE:
	case VOID_TYPE:
	case BOOLEAN_TYPE:
	case UNION_TYPE:
	case ENUMERAL_TYPE:
	case RECORD_TYPE:
	case BLOCK:
	case PLACEHOLDER_EXPR:
	case SSA_NAME:
	case FIELD_DECL:
	case RESULT_DECL:
	  /* None of thse have subtrees other than those already walked
	     above.  */
	  break;

	case POINTER_TYPE:
	case REFERENCE_TYPE:
	  WALK_SUBTREE_TAIL (TREE_TYPE (*tp));
	  break;

	case TREE_LIST:
	  WALK_SUBTREE (TREE_VALUE (*tp));
	  WALK_SUBTREE_TAIL (TREE_CHAIN (*tp));
	  break;

	case TREE_VEC:
	  {
	    int len = TREE_VEC_LENGTH (*tp);

	    if (len == 0)
	      break;

	    /* Walk all elements but the first.  */
	    while (--len)
	      WALK_SUBTREE (TREE_VEC_ELT (*tp, len));

	    /* Now walk the first one as a tail call.  */
	    WALK_SUBTREE_TAIL (TREE_VEC_ELT (*tp, 0));
	  }

	case COMPLEX_CST:
	  WALK_SUBTREE (TREE_REALPART (*tp));
	  WALK_SUBTREE_TAIL (TREE_IMAGPART (*tp));

	case CONSTRUCTOR:
	  WALK_SUBTREE_TAIL (CONSTRUCTOR_ELTS (*tp));

	case SAVE_EXPR:
	  WALK_SUBTREE_TAIL (TREE_OPERAND (*tp, 0));

	case BIND_EXPR:
	  {
	    tree decl;
	    for (decl = BIND_EXPR_VARS (*tp); decl; decl = TREE_CHAIN (decl))
	      {
		/* Walk the DECL_INITIAL and DECL_SIZE.  We don't want to walk
		   into declarations that are just mentioned, rather than
		   declared; they don't really belong to this part of the tree.
		   And, we can see cycles: the initializer for a declaration
		   can refer to the declaration itself.  */
		WALK_SUBTREE (DECL_INITIAL (decl));
		WALK_SUBTREE (DECL_SIZE (decl));
		WALK_SUBTREE (DECL_SIZE_UNIT (decl));
		WALK_SUBTREE (TREE_TYPE (decl));
	      }
	    WALK_SUBTREE_TAIL (BIND_EXPR_BODY (*tp));
	  }

	case STATEMENT_LIST:
	  {
	    tree_stmt_iterator i;
	    for (i = tsi_start (*tp); !tsi_end_p (i); tsi_next (&i))
	      WALK_SUBTREE (*tsi_stmt_ptr (i));
	  }
	  break;

	default:
	  /* ??? This could be a language-defined node.  We really should make
	     a hook for it, but right now just ignore it.  */
	  break;
	}
    }

  /* We didn't find what we were looking for.  */
  return NULL_TREE;

#undef WALK_SUBTREE
#undef WALK_SUBTREE_TAIL
}

/* Like walk_tree, but does not walk duplicate nodes more than once.  */

tree
walk_tree_without_duplicates (tree *tp, walk_tree_fn func, void *data)
{
  tree result;
  struct pointer_set_t *pset;

  pset = pointer_set_create ();
  result = walk_tree (tp, func, data, pset);
  pointer_set_destroy (pset);
  return result;
}

/* Passed to walk_tree.  Copies the node pointed to, if appropriate.  */

tree
copy_tree_r (tree *tp, int *walk_subtrees, void *data ATTRIBUTE_UNUSED)
{
  enum tree_code code = TREE_CODE (*tp);

  /* We make copies of most nodes.  */
  if (IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (code))
      || code == TREE_LIST
      || code == TREE_VEC
      || code == TYPE_DECL)
    {
      /* Because the chain gets clobbered when we make a copy, we save it
	 here.  */
      tree chain = TREE_CHAIN (*tp);
      tree new;

      /* Copy the node.  */
      new = copy_node (*tp);

      /* Propagate mudflap marked-ness.  */
      if (flag_mudflap && mf_marked_p (*tp))
        mf_mark (new);

      *tp = new;

      /* Now, restore the chain, if appropriate.  That will cause
	 walk_tree to walk into the chain as well.  */
      if (code == PARM_DECL || code == TREE_LIST)
	TREE_CHAIN (*tp) = chain;

      /* For now, we don't update BLOCKs when we make copies.  So, we
	 have to nullify all BIND_EXPRs.  */
      if (TREE_CODE (*tp) == BIND_EXPR)
	BIND_EXPR_BLOCK (*tp) = NULL_TREE;
    }

  else if (TREE_CODE_CLASS (code) == tcc_type)
    *walk_subtrees = 0;
  else if (TREE_CODE_CLASS (code) == tcc_declaration)
    *walk_subtrees = 0;
  else if (TREE_CODE_CLASS (code) == tcc_constant)
    *walk_subtrees = 0;
  else
    gcc_assert (code != STATEMENT_LIST);
  return NULL_TREE;
}

/* The SAVE_EXPR pointed to by TP is being copied.  If ST contains
   information indicating to what new SAVE_EXPR this one should be mapped,
   use that one.  Otherwise, create a new node and enter it in ST.  FN is
   the function into which the copy will be placed.  */

static void
remap_save_expr (tree *tp, void *st_, int *walk_subtrees)
{
  splay_tree st = (splay_tree) st_;
  splay_tree_node n;
  tree t;

  /* See if we already encountered this SAVE_EXPR.  */
  n = splay_tree_lookup (st, (splay_tree_key) *tp);

  /* If we didn't already remap this SAVE_EXPR, do so now.  */
  if (!n)
    {
      t = copy_node (*tp);

      /* Remember this SAVE_EXPR.  */
      splay_tree_insert (st, (splay_tree_key) *tp, (splay_tree_value) t);
      /* Make sure we don't remap an already-remapped SAVE_EXPR.  */
      splay_tree_insert (st, (splay_tree_key) t, (splay_tree_value) t);
    }
  else
    {
      /* We've already walked into this SAVE_EXPR; don't do it again.  */
      *walk_subtrees = 0;
      t = (tree) n->value;
    }

  /* Replace this SAVE_EXPR with the copy.  */
  *tp = t;
}

/* Called via walk_tree.  If *TP points to a DECL_STMT for a local label,
   copies the declaration and enters it in the splay_tree in DATA (which is
   really an `inline_data *').  */

static tree
mark_local_for_remap_r (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED,
			void *data)
{
  tree t = *tp;
  inline_data *id = (inline_data *) data;
  tree decl;

  /* Don't walk into types.  */
  if (TYPE_P (t))
    {
      *walk_subtrees = 0;
      return NULL_TREE;
    }

  else if (TREE_CODE (t) == LABEL_EXPR)
    {
      decl = TREE_OPERAND (*tp, 0);

      /* Copy the decl and remember the copy.  */
      insert_decl_map (id, decl,
		       copy_decl_for_inlining (decl, DECL_CONTEXT (decl),
					       DECL_CONTEXT (decl)));
    }

  return NULL_TREE;
}

/* Perform any modifications to EXPR required when it is unsaved.  Does
   not recurse into EXPR's subtrees.  */

static void
unsave_expr_1 (tree expr)
{
  switch (TREE_CODE (expr))
    {
    case TARGET_EXPR:
      /* Don't mess with a TARGET_EXPR that hasn't been expanded.
         It's OK for this to happen if it was part of a subtree that
         isn't immediately expanded, such as operand 2 of another
         TARGET_EXPR.  */
      if (TREE_OPERAND (expr, 1))
	break;

      TREE_OPERAND (expr, 1) = TREE_OPERAND (expr, 3);
      TREE_OPERAND (expr, 3) = NULL_TREE;
      break;

    default:
      break;
    }
}

/* Called via walk_tree when an expression is unsaved.  Using the
   splay_tree pointed to by ST (which is really a `splay_tree'),
   remaps all local declarations to appropriate replacements.  */

static tree
unsave_r (tree *tp, int *walk_subtrees, void *data)
{
  inline_data *id = (inline_data *) data;
  splay_tree st = id->decl_map;
  splay_tree_node n;

  /* Only a local declaration (variable or label).  */
  if ((TREE_CODE (*tp) == VAR_DECL && !TREE_STATIC (*tp))
      || TREE_CODE (*tp) == LABEL_DECL)
    {
      /* Lookup the declaration.  */
      n = splay_tree_lookup (st, (splay_tree_key) *tp);

      /* If it's there, remap it.  */
      if (n)
	*tp = (tree) n->value;
    }

  else if (TREE_CODE (*tp) == STATEMENT_LIST)
    copy_statement_list (tp);
  else if (TREE_CODE (*tp) == BIND_EXPR)
    copy_bind_expr (tp, walk_subtrees, id);
  else if (TREE_CODE (*tp) == SAVE_EXPR)
    remap_save_expr (tp, st, walk_subtrees);
  else
    {
      copy_tree_r (tp, walk_subtrees, NULL);

      /* Do whatever unsaving is required.  */
      unsave_expr_1 (*tp);
    }

  /* Keep iterating.  */
  return NULL_TREE;
}

/* Copies everything in EXPR and replaces variables, labels
   and SAVE_EXPRs local to EXPR.  */

tree
unsave_expr_now (tree expr)
{
  inline_data id;

  /* There's nothing to do for NULL_TREE.  */
  if (expr == 0)
    return expr;

  /* Set up ID.  */
  memset (&id, 0, sizeof (id));
  id.callee = current_function_decl;
  id.caller = current_function_decl;
  id.decl_map = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);

  /* Walk the tree once to find local labels.  */
  walk_tree_without_duplicates (&expr, mark_local_for_remap_r, &id);

  /* Walk the tree again, copying, remapping, and unsaving.  */
  walk_tree (&expr, unsave_r, &id, NULL);

  /* Clean up.  */
  splay_tree_delete (id.decl_map);

  return expr;
}

/* Allow someone to determine if SEARCH is a child of TOP from gdb.  */

static tree
debug_find_tree_1 (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED, void *data)
{
  if (*tp == data)
    return (tree) data;
  else
    return NULL;
}

extern bool debug_find_tree (tree top, tree search);

bool
debug_find_tree (tree top, tree search)
{
  return walk_tree_without_duplicates (&top, debug_find_tree_1, search) != 0;
}

/* Declare the variables created by the inliner.  Add all the variables in
   VARS to BIND_EXPR.  */

static void
declare_inline_vars (tree bind_expr, tree vars)
{
  tree t;
  for (t = vars; t; t = TREE_CHAIN (t))
    DECL_SEEN_IN_BIND_EXPR_P (t) = 1;

  add_var_to_bind_expr (bind_expr, vars);
}

#include "gt-tree-inline.h"

