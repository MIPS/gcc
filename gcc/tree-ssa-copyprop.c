/* SSA-Copy propagation for the GNU compiler.
   Copyright (C) 2003 Free Software Foundation, Inc.
   Contributed by Aldy Hernandez <aldy@quesejoda.com>
   and Diego Novillo <dnovillo@redhat.com>
   
This file is part of GCC.
   
GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
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
#include "errors.h"
#include "ggc.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "timevar.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-simple.h"
#include "tree-dump.h"

/* Local variables.  */
static FILE *dump_file;
static int dump_flags;

/* Local functions.  */
static void copyprop_stmt	PARAMS ((tree));
static void copyprop_phi	PARAMS ((tree));
static inline tree get_original	PARAMS ((tree, tree *));


/* Main entry point to the copy propagator.  The algorithm is a simple
   linear scan of the flowgraph.  For every variable X_i used in the
   function, it retrieves its unique reaching definition.  If X_i's
   definition is a copy (i.e., X_i = Y_j), then X_i is replaced with Y_j.  */

void
tree_ssa_copyprop (fndecl)
     tree fndecl;
{
  basic_block bb;

  timevar_push (TV_TREE_COPYPROP);
  dump_file = dump_begin (TDI_copyprop, &dump_flags);

  /* Traverse every block in the flowgraph propagating copies in each
     statement.  */
  FOR_EACH_BB (bb)
    {
      block_stmt_iterator si;
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	copyprop_phi (phi);

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	copyprop_stmt (bsi_stmt (si));
    }

  if (dump_file)
    {
      dump_end (TDI_copyprop, dump_file);
      dump_function (TDI_copyprop, fndecl);
    }

  timevar_pop (TV_TREE_COPYPROP);
}


/* Propagate copies in statement STMT.  If operand X_i in STMT is defined
   by a statement of the form X_i = Y_j, replace the use of X_i with Y_j.  */

static void
copyprop_stmt (stmt)
     tree stmt;
{
  varray_type uses;
  size_t i;
  bool modified;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nPropagating in statement: ");
      print_generic_expr (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  get_stmt_operands (stmt);
  modified = false;

  /* Propagate real uses.  */
  uses = use_ops (stmt);
  for (i = 0; uses && i < VARRAY_ACTIVE_SIZE (uses); i++)
    {
      tree vuse;
      tree *use_p = (tree *) VARRAY_GENERIC_PTR (uses, i);
      tree orig = get_original (*use_p, &vuse);

      if (orig
	  && ! var_ann (SSA_NAME_VAR (*use_p))->occurs_in_abnormal_phi
	  && ! var_ann (SSA_NAME_VAR (orig))->occurs_in_abnormal_phi)
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "\tReplacing ");
	      print_generic_expr (dump_file, *use_p, 0);
	      fprintf (dump_file, " with ");
	      print_generic_expr (dump_file, orig, 0);
	      fprintf (dump_file, "\n");
	    }

	  *use_p = orig;
	  if (vuse)
	    add_vuse (vuse, stmt, NULL);
	  modified = true;
	}
    }

  if (modified)
    modify_stmt (stmt);
}


/* Propagate copies inside PHI node PHI.  If argument X_i of PHI comes from
   a definition of the form X_i = Y_j, replace it with Y_j.  */

static void
copyprop_phi (phi)
     tree phi;
{
  int i;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nPropagating in PHI node: ");
      print_generic_expr (dump_file, phi, 0);
      fprintf (dump_file, "\n");
    }

  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
    {
      tree vuse;
      tree arg = PHI_ARG_DEF (phi, i);
      tree orig = get_original (arg, &vuse);

      if (orig
	  && ! var_ann (SSA_NAME_VAR (arg))->occurs_in_abnormal_phi
	  && ! var_ann (SSA_NAME_VAR (orig))->occurs_in_abnormal_phi)
	{
	  if (dump_file && dump_flags & TDF_DETAILS)
	    {
	      fprintf (dump_file, "\tReplacing ");
	      print_generic_expr (dump_file, arg, 0);
	      fprintf (dump_file, " with ");
	      print_generic_expr (dump_file, orig, 0);
	      fprintf (dump_file, "\n");
	    }

	  PHI_ARG_DEF (phi, i) = orig;
	}
    }
}


/* If the unique definition for VAR comes from an assignment of the form
   VAR = ORIG, return ORIG.  Otherwise, return NULL.  */

static inline tree
get_original (var, vuse_p)
     tree var;
     tree *vuse_p;
{
  tree def_stmt;

  def_stmt = SSA_NAME_DEF_STMT (var);
  *vuse_p = NULL_TREE;

  /* If VAR is not the LHS of its defining statement, it means that VAR is
     defined by a VDEF node.  This implies aliasing or structure updates.
     For instance,

     		# a_2 = VDEF <a_1>
     		a.b = tmp_3;
		return a_2;

     If we allow tmp_3 to propagate into the 'return' statement, we would
     be changing the return type of the function.  */
  if (TREE_CODE (def_stmt) == MODIFY_EXPR
      && TREE_OPERAND (def_stmt, 0) == var
      && TREE_CODE (TREE_OPERAND (def_stmt, 1)) == SSA_NAME)
    return TREE_OPERAND (def_stmt, 1);

  return NULL_TREE;
}
