/* Tree lowering pass.  Lowers GIMPLE into unstructured form.

   Copyright (C) 2003 Free Software Foundation, Inc.

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
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "errors.h"
#include "varray.h"
#include "tree-simple.h"
#include "tree-inline.h"
#include "diagnostic.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "tree-flow.h"
#include "timevar.h"
#include "except.h"
#include "hashtab.h"
#include "flags.h"
#include "function.h"
#include "expr.h"
#include "toplev.h"

struct lower_data
{
  /* Block the current statement belongs to.  */
  tree block;
};

static void lower_stmt_body (tree *, struct lower_data *);
static void lower_stmt (tree_stmt_iterator *, struct lower_data *);
static void lower_bind_expr (tree_stmt_iterator *, struct lower_data *);
static void lower_cond_expr (tree_stmt_iterator *, struct lower_data *);
static bool expand_var_p (tree);

/* Lowers the BODY.  */
void
lower_function_body (tree *body)
{
  struct lower_data data;

  if (TREE_CODE (*body) != BIND_EXPR)
    abort ();

  data.block = DECL_INITIAL (current_function_decl);
  BLOCK_SUBBLOCKS (data.block) = NULL_TREE;
  BLOCK_CHAIN (data.block) = NULL_TREE;

  record_vars (BIND_EXPR_VARS (*body));
  *body = BIND_EXPR_BODY (*body);
  lower_stmt_body (body, &data);

  if (data.block != DECL_INITIAL (current_function_decl))
    abort ();
  BLOCK_SUBBLOCKS (data.block)
    = blocks_nreverse (BLOCK_SUBBLOCKS (data.block));
}

/* Lowers the EXPR.  Unlike gimplification the statements are not relowered
   when they are changed -- if this has to be done, the lowering routine must
   do it explicitly.  DATA is passed through the recursion.  */

static void
lower_stmt_body (tree *expr, struct lower_data *data)
{
  tree_stmt_iterator tsi;

  for (tsi = tsi_start (*expr); !tsi_end_p (tsi); )
    lower_stmt (&tsi, data);
}

/* Lowers statement TSI.  DATA is passed through the recursion.  */
static void
lower_stmt (tree_stmt_iterator *tsi, struct lower_data *data)
{
  tree stmt = tsi_stmt (*tsi);

  if (EXPR_LOCUS (stmt))
    TREE_BLOCK (stmt) = data->block;

  switch (TREE_CODE (stmt))
    {
    case BIND_EXPR:
      lower_bind_expr (tsi, data);
      break;

    case COMPOUND_EXPR:
      abort ();

    case NOP_EXPR:
    case ASM_EXPR:
    case RETURN_EXPR:
    case MODIFY_EXPR:
    case CALL_EXPR:
    case GOTO_EXPR:
    case LABEL_EXPR:
    case VA_ARG_EXPR:
    case RESX_EXPR:
    case SWITCH_EXPR:
      tsi_next (tsi);
      break;

    case COND_EXPR:
      lower_cond_expr (tsi, data);
      break;

    default:
      print_node_brief (stderr, "", stmt, 0);
      abort ();
    }
}

/* Record the variables in VARS.  */

void
record_vars (tree vars)
{
  for (; vars; vars = TREE_CHAIN (vars))
    {
      tree var = vars;

      /* Nothing to do in this case.  */
      if (DECL_EXTERNAL (var))
	continue;

      /* Record the variable.  */
      cfun->unexpanded_var_list = tree_cons (NULL_TREE, var,
					     cfun->unexpanded_var_list);
    }
}

/* Lowers a bind_expr TSI.  DATA is passed through the recursion.  */

static void
lower_bind_expr (tree_stmt_iterator *tsi, struct lower_data *data)
{
  tree old_block = data->block;
  tree stmt = tsi_stmt (*tsi);

  if (BIND_EXPR_BLOCK (stmt))
    {
      data->block = BIND_EXPR_BLOCK (stmt);

      /* Block tree may get clobbered by inlining.  Normally this would be
	 fixed in rest_of_decl_compilation using block notes, but since we
	 are not going to emit them, it is up to us.  */
      BLOCK_CHAIN (data->block) = BLOCK_SUBBLOCKS (old_block);
      BLOCK_SUBBLOCKS (old_block) = data->block;
      BLOCK_SUBBLOCKS (data->block) = NULL_TREE;
      BLOCK_SUPERCONTEXT (data->block) = old_block;
    }

  record_vars (BIND_EXPR_VARS (stmt));
  lower_stmt_body (&BIND_EXPR_BODY (stmt), data);

  if (BIND_EXPR_BLOCK (stmt))
    {
      if (data->block != BIND_EXPR_BLOCK (stmt))
	abort ();

      BLOCK_SUBBLOCKS (data->block) =
	      blocks_nreverse (BLOCK_SUBBLOCKS (data->block));
      data->block = old_block;
    }

  /* The BIND_EXPR no longer carries any useful information, so get rid
     of it.  */
  tsi_link_before (tsi, BIND_EXPR_BODY (stmt), TSI_SAME_STMT);
  tsi_delink (tsi);
}

/* Lowers a cond_expr TSI.  DATA is passed through the recursion.  */

static void
lower_cond_expr (tree_stmt_iterator *tsi, struct lower_data *data)
{
  tree stmt = tsi_stmt (*tsi);
  bool then_is_goto, else_is_goto;
  tree then_branch, else_branch;
  tree then_goto, else_goto;
  
  lower_stmt_body (&COND_EXPR_THEN (stmt), data);
  lower_stmt_body (&COND_EXPR_ELSE (stmt), data);

  then_branch = COND_EXPR_THEN (stmt);
  else_branch = COND_EXPR_ELSE (stmt);

  then_goto = expr_only (then_branch);
  then_is_goto = then_goto && simple_goto_p (then_goto);

  else_goto = expr_only (else_branch);
  else_is_goto = else_goto && simple_goto_p (else_goto);

  if (!then_is_goto || !else_is_goto)
    {
      tree then_label, else_label, end_label, t;

      then_label = NULL_TREE;
      else_label = NULL_TREE;
      end_label = NULL_TREE;
 
      /* Replace the cond_expr with explicit gotos.  */
      if (!then_is_goto)
	{
	  t = build1 (LABEL_EXPR, void_type_node, NULL_TREE);
	  if (TREE_SIDE_EFFECTS (then_branch))
	    then_label = t;
	  else
	    end_label = t;
	  then_goto = build_and_jump (&LABEL_EXPR_LABEL (t));
	}

      if (!else_is_goto)
	{
	  t = build1 (LABEL_EXPR, void_type_node, NULL_TREE);
	  if (TREE_SIDE_EFFECTS (else_branch))
	    else_label = t;
	  else
	    {
	      /* Both THEN and ELSE can be no-ops if one or both contained an
	         empty BIND_EXPR that was associated with the toplevel block
	         of an inlined function.  In that case remove_useless_stmts
	         can't have cleaned things up for us; kill the whole 
	         conditional now.  */
	      if (end_label)
		{
		  tsi_delink (tsi);
		  return;
		}
	      else
		end_label = t;
	    }
	  else_goto = build_and_jump (&LABEL_EXPR_LABEL (t));
	}

      if (then_label)
	{
	  tsi_link_after (tsi, then_label, TSI_CONTINUE_LINKING);
	  tsi_link_after (tsi, then_branch, TSI_CONTINUE_LINKING);
  
	  if (else_label)
	    {
	      end_label = build1 (LABEL_EXPR, void_type_node, NULL_TREE);
	      t = build_and_jump (&LABEL_EXPR_LABEL (end_label));
	      tsi_link_after (tsi, t, TSI_CONTINUE_LINKING);
	    }
	}
  
      if (else_label)
	{
	  tsi_link_after (tsi, else_label, TSI_CONTINUE_LINKING);
	  tsi_link_after (tsi, else_branch, TSI_CONTINUE_LINKING);
	}

      if (end_label)
	tsi_link_after (tsi, end_label, TSI_CONTINUE_LINKING);
    }

  COND_EXPR_THEN (stmt) = then_goto;
  COND_EXPR_ELSE (stmt) = else_goto;

  tsi_next (tsi);
}

/* Check whether to expand a variable VAR.  */

static bool
expand_var_p (tree var)
{
  struct var_ann_d *ann;

  if (TREE_CODE (var) != VAR_DECL)
    return true;

  ann = var_ann (var);

  /* Remove all unused, unaliased temporaries.  Also remove unused, unaliased
     local variables during highly optimizing compilations.  */
  ann = var_ann (var);
  if (ann
      && ! ann->may_aliases
      && ! ann->used
      && ! ann->has_hidden_use
      && ! TREE_ADDRESSABLE (var)
      && (DECL_ARTIFICIAL (var) || optimize >= 2))
    return false;

  return true;
}

/* Throw away variables that are unused.  */

void
remove_useless_vars (void)
{
  tree var, *cell;

  for (cell = &cfun->unexpanded_var_list; *cell; )
    {
      var = TREE_VALUE (*cell);

      if (!expand_var_p (var))
	{
	  *cell = TREE_CHAIN (*cell);
	  continue;
	}

      cell = &TREE_CHAIN (*cell);
    }
}

/* Expand variables in the unexpanded_var_list.  */

void
expand_used_vars (void)
{
  tree cell;

  cfun->unexpanded_var_list = nreverse (cfun->unexpanded_var_list);

  for (cell = cfun->unexpanded_var_list; cell; cell = TREE_CHAIN (cell))
    expand_var (TREE_VALUE (cell));

  cfun->unexpanded_var_list = NULL_TREE;
}
