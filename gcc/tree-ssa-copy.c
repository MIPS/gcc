/* Copy propagation and SSA_NAME replacement support routines.
   Copyright (C) 2004 Free Software Foundation, Inc.

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
#include "tree.h"
#include "flags.h"
#include "rtl.h"
#include "tm_p.h"
#include "ggc.h"
#include "basic-block.h"
#include "output.h"
#include "errors.h"
#include "expr.h"
#include "function.h"
#include "diagnostic.h"
#include "timevar.h"
#include "tree-dump.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "tree-ssa-propagate.h"
#include "langhooks.h"

/* This file implements the copy propagation pass and provides a
   handful of interfaces for performing const/copy propagation and
   simple expression replacement which keep variable annotations
   up-to-date.

   We require that for any copy operation where the RHS and LHS have
   a non-null memory tag that the memory tag be the same.   It is OK
   for one or both of the memory tags to be NULL.

   We also require tracking if a variable is dereferenced in a load or
   store operation.

   We enforce these requirements by having all copy propagation and
   replacements of one SSA_NAME with a different SSA_NAME to use the
   APIs defined in this file.  */

/* Return true if we may propagate ORIG into DEST, false otherwise.  */

bool
may_propagate_copy (tree dest, tree orig)
{
  tree type_d = TREE_TYPE (dest);
  tree type_o = TREE_TYPE (orig);

  /* Do not copy between types for which we *do* need a conversion.  */
  if (!tree_ssa_useless_type_conversion_1 (type_d, type_o))
    return false;

  /* FIXME.  GIMPLE is allowing pointer assignments and comparisons of
     pointers that have different alias sets.  This means that these
     pointers will have different memory tags associated to them.

     If we allow copy propagation in these cases, statements de-referencing
     the new pointer will now have a reference to a different memory tag
     with potentially incorrect SSA information.

     This was showing up in libjava/java/util/zip/ZipFile.java with code
     like:

     	struct java.io.BufferedInputStream *T.660;
	struct java.io.BufferedInputStream *T.647;
	struct java.io.InputStream *is;
	struct java.io.InputStream *is.662;
	[ ... ]
	T.660 = T.647;
	is = T.660;	<-- This ought to be type-casted
	is.662 = is;

     Also, f/name.c exposed a similar problem with a COND_EXPR predicate
     that was causing DOM to generate and equivalence with two pointers of
     alias-incompatible types:

     	struct _ffename_space *n;
	struct _ffename *ns;
	[ ... ]
	if (n == ns)
	  goto lab;
	...
	lab:
	return n;

     I think that GIMPLE should emit the appropriate type-casts.  For the
     time being, blocking copy-propagation in these cases is the safe thing
     to do.  */
  if (TREE_CODE (dest) == SSA_NAME && TREE_CODE (orig) == SSA_NAME
      && POINTER_TYPE_P (type_d) && POINTER_TYPE_P (type_o))
    {
      tree mt_dest = var_ann (SSA_NAME_VAR (dest))->type_mem_tag;
      tree mt_orig = var_ann (SSA_NAME_VAR (orig))->type_mem_tag;
      if (mt_dest && mt_orig && mt_dest != mt_orig)
	return false;
      else if (!lang_hooks.types_compatible_p (type_d, type_o))
	return false;
      else if (get_alias_set (TREE_TYPE (type_d)) != 
	       get_alias_set (TREE_TYPE (type_o)))
	return false;
    }

  /* If the destination is a SSA_NAME for a virtual operand, then we have
     some special cases to handle.  */
  if (TREE_CODE (dest) == SSA_NAME && !is_gimple_reg (dest))
    {
      /* If both operands are SSA_NAMEs referring to virtual operands, then
	 we can always propagate.  */
      if (TREE_CODE (orig) == SSA_NAME
	  && !is_gimple_reg (orig))
	return true;

      /* We have a "copy" from something like a constant into a virtual
	 operand.  Reject these.  */
      return false;
    }

  /* If ORIG flows in from an abnormal edge, it cannot be propagated.  */
  if (TREE_CODE (orig) == SSA_NAME
      && SSA_NAME_OCCURS_IN_ABNORMAL_PHI (orig))
    return false;

  /* If DEST is an SSA_NAME that flows from an abnormal edge, then it
     cannot be replaced.  */
  if (TREE_CODE (dest) == SSA_NAME
      && SSA_NAME_OCCURS_IN_ABNORMAL_PHI (dest))
    return false;

  /* Anything else is OK.  */
  return true;
}

/* Similarly, but we know that we're propagating into an ASM_EXPR.  */

bool
may_propagate_copy_into_asm (tree dest)
{
  /* Hard register operands of asms are special.  Do not bypass.  */
  return !(TREE_CODE (dest) == SSA_NAME
	   && TREE_CODE (SSA_NAME_VAR (dest)) == VAR_DECL
	   && DECL_HARD_REGISTER (SSA_NAME_VAR (dest)));
}


/* Given two SSA_NAMEs pointers ORIG and NEW such that we are copy
   propagating NEW into ORIG, consolidate aliasing information so that
   they both share the same memory tags.  */

static void
merge_alias_info (tree orig, tree new)
{
  tree new_sym = SSA_NAME_VAR (new);
  tree orig_sym = SSA_NAME_VAR (orig);
  var_ann_t new_ann = var_ann (new_sym);
  var_ann_t orig_ann = var_ann (orig_sym);
  struct ptr_info_def *new_ptr_info;
  struct ptr_info_def *orig_ptr_info;

  gcc_assert (POINTER_TYPE_P (TREE_TYPE (orig)));
  gcc_assert (POINTER_TYPE_P (TREE_TYPE (new)));
#if defined ENABLE_CHECKING
  gcc_assert (lang_hooks.types_compatible_p (TREE_TYPE (orig),
					     TREE_TYPE (new)));

  /* If the pointed-to alias sets are different, these two pointers
     would never have the same memory tag.  In this case, NEW should
     not have been propagated into ORIG.  */
  gcc_assert (get_alias_set (TREE_TYPE (TREE_TYPE (new_sym)))
	      == get_alias_set (TREE_TYPE (TREE_TYPE (orig_sym))));
#endif

  /* Synchronize the type tags.  If both pointers had a tag and they
     are different, then something has gone wrong.  */
  if (new_ann->type_mem_tag == NULL_TREE)
    new_ann->type_mem_tag = orig_ann->type_mem_tag;
  else if (orig_ann->type_mem_tag == NULL_TREE)
    orig_ann->type_mem_tag = new_ann->type_mem_tag;
  else
    gcc_assert (new_ann->type_mem_tag == orig_ann->type_mem_tag);

  /* Synchronize flow sensitive alias information.  If both pointers
     had flow information and they are inconsistent, then something
     has gone wrong.  */
  new_ptr_info = get_ptr_info (new);
  orig_ptr_info = get_ptr_info (orig);

  if (new_ptr_info->name_mem_tag == NULL_TREE)
    memcpy (new_ptr_info, orig_ptr_info, sizeof (*new_ptr_info));
  else if (orig_ptr_info->name_mem_tag == NULL_TREE)
    memcpy (orig_ptr_info, new_ptr_info, sizeof (*orig_ptr_info));
  else if (orig_ptr_info->name_mem_tag != new_ptr_info->name_mem_tag)
    abort ();
}   


/* Common code for propagate_value and replace_exp.

   Replace use operand OP_P with VAL.  FOR_PROPAGATION indicates if the
   replacement is done to propagate a value or not.  */

static void
replace_exp_1 (use_operand_p op_p, tree val,
	       bool for_propagation ATTRIBUTE_UNUSED)
{
  tree op = USE_FROM_PTR (op_p);

#if defined ENABLE_CHECKING
  gcc_assert (!(for_propagation
		&& TREE_CODE (op) == SSA_NAME
		&& TREE_CODE (val) == SSA_NAME
		&& !may_propagate_copy (op, val)));
#endif

  if (TREE_CODE (val) == SSA_NAME)
    {
      if (TREE_CODE (op) == SSA_NAME && POINTER_TYPE_P (TREE_TYPE (op)))
	merge_alias_info (op, val);
      SET_USE (op_p, val);
    }
  else
    SET_USE (op_p, unsave_expr_now (val));
}


/* Propagate the value VAL (assumed to be a constant or another SSA_NAME)
   into the operand pointed by OP_P.

   Use this version for const/copy propagation as it will perform additional
   checks to ensure validity of the const/copy propagation.  */

void
propagate_value (use_operand_p op_p, tree val)
{
  replace_exp_1 (op_p, val, true);
}


/* Propagate the value VAL (assumed to be a constant or another SSA_NAME)
   into the tree pointed by OP_P.

   Use this version for const/copy propagation when SSA operands are not
   available.  It will perform the additional checks to ensure validity of
   the const/copy propagation, but will not update any operand information.
   Be sure to mark the stmt as modified.  */

void
propagate_tree_value (tree *op_p, tree val)
{
#if defined ENABLE_CHECKING
  gcc_assert (!(TREE_CODE (val) == SSA_NAME
		&& TREE_CODE (*op_p) == SSA_NAME
		&& !may_propagate_copy (*op_p, val)));
#endif

  if (TREE_CODE (val) == SSA_NAME)
    {
      if (TREE_CODE (*op_p) == SSA_NAME && POINTER_TYPE_P (TREE_TYPE (*op_p)))
	merge_alias_info (*op_p, val);
      *op_p = val;
    }
  else
    *op_p = unsave_expr_now (val);
}


/* Replace *OP_P with value VAL (assumed to be a constant or another SSA_NAME).

   Use this version when not const/copy propagating values.  For example,
   PRE uses this version when building expressions as they would appear
   in specific blocks taking into account actions of PHI nodes.  */

void
replace_exp (use_operand_p op_p, tree val)
{
  replace_exp_1 (op_p, val, false);
}


/*---------------------------------------------------------------------------
		 Copy propagation and simplification
---------------------------------------------------------------------------*/
/* During propagation, we keep chains of variables that are copies of
   one another.  If variable X_i is a copy of X_j and X_j is a copy of
   X_k, COPY_CHAINS will contain:

   	COPY_CHAINS[i] = X_j
	COPY_CHAINS[j] = X_k
	COPY_CHAINS[k] = X_k  */
static tree *copy_chains;


/* Return true if we should compute immediate uses for SSA_NAME VAR.  */

static inline bool
need_imm_uses_for (tree var)
{
  /* The only statements that are interesting as seeds for copy
     propagation are those assignments whose RHS is an SSA_NAME.  */
  tree def_stmt = SSA_NAME_DEF_STMT (var);
  return ((TREE_CODE (def_stmt) == MODIFY_EXPR
	   && TREE_CODE (TREE_OPERAND (def_stmt, 1)) == SSA_NAME)
          || TREE_CODE (def_stmt) == PHI_NODE);
}


/* Return the first variable in the copy-of chain for VAR.  */

static inline tree
get_first_copy_of (tree var)
{
  unsigned ver = SSA_NAME_VERSION (var);

  if (copy_chains[ver] == NULL_TREE && !need_imm_uses_for (var))
    {
      /* If the variable will never generate a useful copy relation,
	 make it its own copy in COPY_CHAINS and set its value to
	 NULL.  */
      copy_chains[ver] = var;
      SSA_NAME_VALUE (var) = NULL_TREE;
    }

  return copy_chains[ver];
}


/* Return last variable in the copy-of chain for variable VAR.  */

static tree
get_last_copy_of (tree var)
{
  tree copy_of = var;

  while (copy_chains[SSA_NAME_VERSION (copy_of)]
         && copy_chains[SSA_NAME_VERSION (copy_of)] != copy_of)
    copy_of = copy_chains[SSA_NAME_VERSION (copy_of)];

  return (copy_of) ? copy_of : var;
}


/* Set ORIG to be the first variable in the copy-of chain for DEST.
   Also set DEST's SSA_NAME_VALUE to the last variable in the copy-of
   chain for DEST.  If either value has changed, return true.  */

static inline bool
set_first_copy_of (tree dest, tree orig)
{
  unsigned int ver = SSA_NAME_VERSION (dest);
  tree old_first = copy_chains[ver];
  tree old_val = SSA_NAME_VALUE (dest);
  copy_chains[ver] = orig;
  SSA_NAME_VALUE (dest) = get_last_copy_of (orig);

  return (old_first != orig || old_val != SSA_NAME_VALUE (dest));
}


/* Dump the copy-of value for variable VAR to DUMP_FILE.  */

static void
dump_copy_of (FILE *dump_file, tree var)
{
  tree val;
  tree copy_of;

  print_generic_expr (dump_file, var, dump_flags);

  if (TREE_CODE (var) != SSA_NAME)
    return;

  fprintf (dump_file, " copy-of chain: ");

  copy_of = var;
  print_generic_expr (dump_file, copy_of, 0);
  fprintf (dump_file, " ");
  while (copy_chains[SSA_NAME_VERSION (copy_of)]
         && copy_chains[SSA_NAME_VERSION (copy_of)] != copy_of)
    {
      fprintf (dump_file, "-> ");
      copy_of = SSA_NAME_VALUE (copy_of);
      print_generic_expr (dump_file, copy_of, 0);
      fprintf (dump_file, " ");
    }

  val = get_first_copy_of (var);
  if (val == NULL_TREE)
    fprintf (dump_file, "[UNDEFINED]");
  else if (val != var)
    fprintf (dump_file, "[COPY]");
  else
    fprintf (dump_file, "[NOT A COPY]");
}


/* Evaluate the RHS of STMT.  If it produces a new or simpler value
   than what its LHS currently holds, modify it and store the LHS into
   *RESULT_P.  */

static enum ssa_prop_result
copy_prop_visit_assignment (tree stmt, tree *result_p)
{
  tree rhs;

  *result_p = TREE_OPERAND (stmt, 0);
  rhs = TREE_OPERAND (stmt, 1);

#if defined ENABLE_CHECKING
  if (TREE_CODE (*result_p) != SSA_NAME)
    abort ();
#endif
  
  if (TREE_CODE (rhs) == SSA_NAME)
    {
      if  (may_propagate_copy (*result_p, rhs))
	{
	  /* FIXME, some copy operations fail 'may_propagate_copy'
	     because of the typecasts being stripped by
	     tree_ssa_useless_type_conversion_1.  These type casts are
	     removed even though the types are not compatible
	     according to lang_hook.types_compatible_p.  */
	  if (set_first_copy_of (*result_p, rhs))
	    return SSA_PROP_INTERESTING;
	  else
	    return SSA_PROP_NOT_INTERESTING;
	}
    }

  return SSA_PROP_VARYING;
}


/* Visit the COND_EXPR STMT.  Return SSA_PROP_INTERESTING
   if it can determine which edge will be taken.  Otherwise, return
   SSA_PROP_VARYING.  */

static enum ssa_prop_result
copy_prop_visit_cond_stmt (tree stmt, edge *taken_edge_p)
{
  enum ssa_prop_result retval;
  tree cond;
  use_optype uses;

  cond = COND_EXPR_COND (stmt);
  uses = STMT_USE_OPS (stmt);
  retval = SSA_PROP_VARYING;

  /* The only conditionals that we may be able to compute statically
     are predicates involving at least one SSA_NAME.  */
  if (TREE_CODE_CLASS (TREE_CODE (cond)) == tcc_comparison
      && NUM_USES (uses) >= 1)
    {
      unsigned i;
      tree *orig;

      /* Save the original operands.  */
      orig = xmalloc (sizeof (tree) * NUM_USES (uses));
      for (i = 0; i < NUM_USES (uses); i++)
	orig[i] = USE_OP (uses, i);

      /* See if we can determine the predicate's value.  */
      replace_uses_in (stmt, NULL);

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Trying to determine truth value of ");
	  fprintf (dump_file, "predicate ");
	  print_generic_stmt (dump_file, cond, 0);
	}

      *taken_edge_p = find_taken_edge (bb_for_stmt (stmt), cond);
      if (*taken_edge_p)
	retval = SSA_PROP_INTERESTING;

      /* Restore the original operands.  */
      for (i = 0; i < NUM_USES (uses); i++)
	SET_USE_OP (uses, i, orig[i]);
      free (orig);
    }

  if (dump_file && (dump_flags & TDF_DETAILS) && *taken_edge_p)
    fprintf (dump_file, "\nConditional will always take edge %d->%d\n",
	     (*taken_edge_p)->src->index, (*taken_edge_p)->dest->index);

  return retval;
}


/* Evaluate statement STMT.  If the statement produces a new output
   value, return SSA_PROP_INTERESTING and store the SSA_NAME holding
   the new value in *RESULT_P.

   If STMT is a conditional branch and we can determine its truth
   value, set *TAKEN_EDGE_P accordingly.

   If the new value produced by STMT is varying, return
   SSA_PROP_VARYING.  */

static enum ssa_prop_result
copy_prop_visit_stmt (tree stmt, edge *taken_edge_p, tree *result_p)
{
  stmt_ann_t ann;
  enum ssa_prop_result retval;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nVisiting statement: ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  ann = stmt_ann (stmt);

  if (TREE_CODE (stmt) == MODIFY_EXPR
      && TREE_CODE (TREE_OPERAND (stmt, 0)) == SSA_NAME)
    {
      /* If the statement is an assignment that produces a single
	 output value, evaluate its RHS to see if the lattice value of
	 its output has changed.  */
      retval = copy_prop_visit_assignment (stmt, result_p);
    }
  else if (TREE_CODE (stmt) == COND_EXPR)
    {
      /* See if we can determine which edge goes out of a conditional
	 jump.  */
      retval = copy_prop_visit_cond_stmt (stmt, taken_edge_p);
    }
  else
    retval = SSA_PROP_VARYING;

  if (retval == SSA_PROP_VARYING)
    {
      tree def;
      ssa_op_iter i;

      /* Any other kind of statement is not interesting for constant
	 propagation and, therefore, not worth simulating.  */
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "No interesting values produced.\n");

      /* The assignment is not a copy operation.  Don't visit this
	 statement again and mark all the definitions in the statement
	 to be copies of nothing.  */
      FOR_EACH_SSA_TREE_OPERAND (def, stmt, i, SSA_OP_ALL_DEFS)
	set_first_copy_of (def, def);
    }

  return retval;
}


/* Visit PHI node PHI.  If all the arguments produce the same value,
   set the value of LHS.  */

static enum ssa_prop_result
copy_prop_visit_phi_node (tree phi)
{
  enum ssa_prop_result retval;
  int i;
  tree lhs, phi_val;

  lhs = PHI_RESULT (phi);
  phi_val = get_first_copy_of (lhs);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nVisiting PHI node: ");
      print_generic_expr (dump_file, phi, dump_flags);
      fprintf (dump_file, "\nCurrent value: ");
      dump_copy_of (dump_file, lhs);
      fprintf (dump_file, "\n\n");
    }

  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
    {
      tree arg = PHI_ARG_DEF (phi, i);
      edge e = PHI_ARG_EDGE (phi, i);

      /* We don't care about values flowing through non-executable
	 edges.  */
      if (!(e->flags & EDGE_EXECUTABLE))
	continue;

      /* Constants in the argument list never generate a useful copy.  */
      if (TREE_CODE (arg) != SSA_NAME)
	{
	  phi_val = lhs;
	  break;
	}

      /* If the LHS appears in the argument list, ignore it.  It is
	 irrelevant as a copy.  */
      if (arg == lhs || get_last_copy_of (arg) == lhs)
	continue;

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\tArgument #%d: ", i);
	  dump_copy_of (dump_file, arg);
	  fprintf (dump_file, "\n");
	}

      /* If the LHS didn't have a value yet, make it be a copy of the
	 first argument we find.  */
      if (phi_val == NULL_TREE)
	phi_val = arg;

      /* If PHI_VAL and ARG don't have a common copy-of chain, then
	 this PHI node cannot be a copy operation.  */
      if (get_last_copy_of (phi_val) != get_last_copy_of (arg))
	{
	  phi_val = lhs;
	  break;
	}
    }

  /* FIXME, see note regarding tree_ssa_useless_type_conversion_1 in
     copy_prop_visit_assignment.  */
  if (phi_val && !may_propagate_copy (lhs, phi_val))
    phi_val = lhs;

  if (phi_val && set_first_copy_of (lhs, phi_val))
    retval = (phi_val != lhs) ? SSA_PROP_INTERESTING : SSA_PROP_VARYING;
  else
    retval = SSA_PROP_NOT_INTERESTING;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nPHI node ");
      dump_copy_of (dump_file, lhs);
      fprintf (dump_file, "\nTelling the propagator to ");
      if (retval == SSA_PROP_INTERESTING)
	fprintf (dump_file, "add SSA edges out of this PHI and continue.");
      else if (retval == SSA_PROP_VARYING)
	fprintf (dump_file, "add SSA edges out of this PHI and never visit again.");
      else
	fprintf (dump_file, "do nothing with SSA edges and keep iterating.");
      fprintf (dump_file, "\n\n");
    }

  return retval;
}


/* Initialize structures used for copy propagation.  */

static void
init_copy_prop (void)
{
  basic_block bb;

  copy_chains = xmalloc (num_ssa_names * sizeof (*copy_chains));
  memset (copy_chains, 0, num_ssa_names * sizeof (*copy_chains));

  FOR_EACH_BB (bb)
    {
      block_stmt_iterator si;
      tree phi;

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  tree stmt = bsi_stmt (si);

	  /* The only statements that we care about are those that may
	     generate useful copies.  We also need to mark conditional
	     jumps so that their outgoing edges are added to the work
	     lists of the propagator.  */
	  if (stmt_ends_bb_p (stmt))
	    DONT_SIMULATE_AGAIN (stmt) = false;
	  else if (TREE_CODE (stmt) == MODIFY_EXPR
	           && TREE_CODE (TREE_OPERAND (stmt, 1)) == SSA_NAME)
	    DONT_SIMULATE_AGAIN (stmt) = false;
	  else
	    {
	      /* No need to simulate this statement anymore.  */
	      DONT_SIMULATE_AGAIN (stmt) = true;
	    }
	}

      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	DONT_SIMULATE_AGAIN (phi) = false;
    }

  /* Compute immediate uses for variables we care about.  */
  compute_immediate_uses (TDFA_USE_OPS | TDFA_USE_VOPS, need_imm_uses_for);
}


/* Deallocate memory used in copy propagation and do final
   substitution.  */

static void
fini_copy_prop (void)
{
  size_t i;

  for (i = 1; i < num_ssa_names; i++)
    {
      tree var = ssa_name (i);
      if (var && copy_chains[i] && copy_chains[i] != var)
	SSA_NAME_VALUE (var) = get_last_copy_of (var);
    }

  substitute_and_fold ();
  cleanup_tree_cfg ();
  free_df ();

  free (copy_chains);
}


/* Main entry point to the copy propagator.  The algorithm propagates
   the value COPY-OF using ssa_propagate.  For every variable X_i,
   COPY-OF(X_i) indicates which variable is X_i created from.  The
   following example shows how the algorithm proceeds at a high level:

	    1	a_24 = x_1
	    2	a_2 = PHI <a_24, x_1>
	    3	a_5 = PHI <a_2>
	    4	x_1 = PHI <x_298, a_5, a_2>

   The end result should be that a_2, a_5, a_24 and x_1 are a copy of
   x_298.  Propagation proceeds as follows.

   Visit #1: a_24 is copy-of x_1.  Value changed.
   Visit #2: a_2 is copy-of x_1.  Value changed.
   Visit #3: a_5 is copy-of x_1.  Value changed.
   Visit #4: x_1 is copy-of x_298.  Value changed.
   Visit #1: a_24 is copy-of x_298.  Value changed.
   Visit #2: a_2 is copy-of x_298.  Value changed.
   Visit #3: a_5 is copy-of x_298.  Value changed.
   Visit #4: x_1 is copy-of x_298.  Stable state reached.
   
   When visiting PHI nodes, we only consider arguments that flow
   through edges marked executable by the propagation engine.  So,
   when visiting statement #2 for the first time, we will only look at
   the first argument (a_24) and optimistically assume that its value
   is the copy of a_24 (x_1).

   The problem with this approach is that it may fail to discover copy
   relations in PHI cycles.  Instead of propagating copy-of
   values, we actually propagate copy-of chains.  For instance:

   		A_3 = B_1;
		C_9 = A_3;
		D_4 = C_9;
		X_i = D_4;

   In this code fragment, COPY-OF (X_i) = { D_4, C_9, A_3, B_1 }.
   Obviously, we are only really interested in the last value of the
   chain, however the propagator needs to access the copy-of chain
   when visiting PHI nodes.

   To represent the copy-of chain, we use the array COPY_CHAINS, which
   holds the first link in the copy-of chain for every variable.
   If variable X_i is a copy of X_j, which in turn is a copy of X_k,
   the array will contain:

		COPY_CHAINS[i] = X_j
		COPY_CHAINS[j] = X_k
		COPY_CHAINS[k] = X_k

   Keeping copy-of chains instead of copy-of values directly becomes
   important when visiting PHI nodes.  Suppose that we had the
   following PHI cycle, such that x_52 is already considered a copy of
   x_53:

	    1	x_54 = PHI <x_53, x_52>
	    2	x_53 = PHI <x_898, x_54>
   
   Visit #1: x_54 is copy-of x_53 (because x_52 is copy-of x_53)
   Visit #2: x_53 is copy-of x_898 (because x_54 is a copy of x_53,
				    so it is considered irrelevant
				    as a copy).
   Visit #1: x_54 is copy-of nothing (x_53 is a copy-of x_898 and
				      x_52 is a copy of x_53, so
				      they don't match)
   Visit #2: x_53 is copy-of nothing

   This problem is avoided by keeping a chain of copies, instead of
   the final copy-of value.  Propagation will now only keep the first
   element of a variable's copy-of chain.  When visiting PHI nodes,
   arguments are considered equal if their copy-of chains end in the
   same variable.  So, as long as their copy-of chains overlap, we
   know that they will be a copy of the same variable, regardless of
   which variable that may be).
   
   Propagation would then proceed as follows (the notation a -> b
   means that is a copy-of b):

   Visit #1: x_54 = PHI <x_53, x_52>
		x_53 -> x_53
		x_52 -> x_53
		Result: x_54 -> x_53.  Value changed.  Add SSA edges.

   Visit #2: x_53 = PHI <x_898, x_54>
   		x_898 -> x_898
		x_54 -> x_53
		Result: x_53 -> x_898.  Value changed.  Add SSA edges.

   Visit #1: x_54 = PHI <x_53, x_52>
   		x_53 -> x_898
		x_52 -> x_53 -> x_898
		Result: x_54 -> x_898.  Value changed.  Add SSA edges.

   Visit #2: x_53 = PHI <x_898, x_54>
   		x_53 -> x_898
		x_54 -> x_898
		Result: x_53 -> x_898.  Value didn't change.  Stable state

   Once the propagator stabilizes, we end up with the desired result
   x_53 and x_54 are both copies of x_898.  */

void
execute_copy_prop (void)
{
  init_copy_prop ();
  ssa_propagate (copy_prop_visit_stmt, copy_prop_visit_phi_node);
  fini_copy_prop ();
}


static bool
gate_copy_prop (void)
{
  return flag_tree_copy_prop != 0;
}


struct tree_opt_pass pass_copy_prop =
{
  "copyprop",				/* name */
  gate_copy_prop,			/* gate */
  execute_copy_prop,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_TREE_COPY_PROP,			/* tv_id */
  PROP_ssa | PROP_alias | PROP_cfg,	/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func
    | TODO_ggc_collect
    | TODO_verify_ssa
    | TODO_rename_vars,			/* todo_flags_finish */
  0					/* letter */
};
