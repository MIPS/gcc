/* Scalar Replacement of Aggregates (SRA) converts some structure
   references into scalar references, exposing them to the scalar
   optimizers.
   Copyright (C) 2003 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

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

/* These RTL headers are needed for basic-block.h.  */
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "diagnostic.h"
#include "langhooks.h"
#include "tree-inline.h"
#include "tree-flow.h"
#include "tree-simple.h"
#include "tree-dump.h"
#include "timevar.h"

/* Maximum number of fields that a structure should have to be scalarized.
   FIXME  This limit has been arbitrarily set to 5.  Experiment to find a
	  sensible setting.  */
#define MAX_NFIELDS_FOR_SRA	5

/* Codes indicating how to copy one structure into another.  */
enum sra_copy_mode { SCALAR_SCALAR, FIELD_SCALAR, SCALAR_FIELD };

/* Local functions.  */
static inline bool can_be_scalarized_p (tree);
static inline tree get_scalar_for (tree, tree);
static inline tree lookup_scalar (int var_ix, int field_ix, tree);
static inline void insert_edge_copies (tree stmt, basic_block bb);
static tree create_scalar_copies (tree lhs, tree rhs, enum sra_copy_mode mode);
static inline void scalarize_component_ref (tree, tree *tp);
static void find_candidates_for_sra (void);
static void scalarize_structures (void);
static void scalarize_stmt (block_stmt_iterator *);
static void scalarize_modify_expr (block_stmt_iterator *);
static void scalarize_call_expr (block_stmt_iterator *);
static void scalarize_asm_expr (block_stmt_iterator *);
static void scalarize_return_expr (block_stmt_iterator *);
static void scalarize_tree_list (tree, block_stmt_iterator *);

/* Local variables.  */
static FILE *dump_file;
static int dump_flags;

/* The set of aggregate variables that are candidates for scalarization.  */
static sbitmap sra_candidates;

/* Set of variables to be renamed.  This bitmap does not include the new
   scalar replacements created during this pass.  At the end of the pass,
   a new set is created with all the variables in this set and all the new
   scalar replacements.  */
static sbitmap vars_to_rename;

/* Set of scalarizable PARM_DECLs that need copy-in operations at the
   beginning of the function.  */
static sbitmap needs_copy_in;

/* A matrix of NUM_REFERENCED_VARIABLES x MAX_NFIELDS_FOR_SRA to map the
   temporary variables to the aggregate reference that they represent.
   For example, suppose that variable 'A' is a scalarizable aggregate with
   fields 'a', 'b' and 'c'.  If the UID of 'A' is 6, then SRA_MAP[6][1]
   will contain the temporary variable representing 'A.b'.  */
static tree **sra_map;


/* Given a structure reference VAR.FIELD, return a scalar representing it.
   If no scalar is found, a new one is created and added to the SRA_MAP
   matrix.  */

static inline tree
get_scalar_for (tree var, tree field)
{
  int var_ix, f_ix;
  tree f, type;

  var_ix = var_ann (var)->uid;

  /* Find the index number for FIELD.  */
  type = TREE_TYPE (var);
  f_ix = 0;
  for (f = TYPE_FIELDS (type); f; f = TREE_CHAIN (f))
    {
      if (TREE_CODE (f) != FIELD_DECL)
	continue;

      if (field == f)
	break;

      f_ix++;
    }

  /* We should have found FIELD.  */
  if (f == NULL_TREE)
    abort ();

  return lookup_scalar (var_ix, f_ix, TREE_TYPE (field));
}


/* Return the scalar in SRA_MAP[VAR_IX][FIELD_IX].  If none exists, create
   a new scalar with type TYPE.  */

static inline tree
lookup_scalar (int var_ix, int field_ix, tree type)
{
#if defined ENABLE_CHECKING
  if (var_ix < 0 || var_ix >= (int)num_referenced_vars)
    abort ();

  if (field_ix < 0 || field_ix >= MAX_NFIELDS_FOR_SRA)
    abort ();
#endif

  /* Create a new row for VAR in SRA_MAP, if necessary.  */
  if (sra_map[var_ix] == NULL)
    sra_map[var_ix] = xcalloc (MAX_NFIELDS_FOR_SRA, sizeof (tree));

  /* If we still have not created a new scalar for FIELD, create one and
     add it to the list of referenced variables.  */
  if (sra_map[var_ix][field_ix] == NULL_TREE)
    {
      sra_map[var_ix][field_ix] = create_tmp_var (type, "SR");
      add_referenced_tmp_var (sra_map[var_ix][field_ix]);
    }

  return sra_map[var_ix][field_ix];
}


/* Return true if the fields of VAR can be replaced by scalar temporaries.
   This only happens if VAR is not call-clobbered and it contains less
   than MAX_NFIELDS_FOR_SRA scalar fields.  */

static inline bool
can_be_scalarized_p (tree var)
{
  tree field, type;
  int nfields;

  if (!is_gimple_non_addressable (var))
    return false;

  type = TREE_TYPE (var);
  nfields = 0;
  for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
    {
      if (TREE_CODE (field) != FIELD_DECL)
	continue;

      if (AGGREGATE_TYPE_P (TREE_TYPE (field)))
	return false;

      /* FIXME.  We don't scalarize structures with bit fields yet.  To
	 support this, we should make sure that all the fields fit in one
	 word and modify every operation done on the scalarized bit fields
	 to mask them properly.  */
      if (DECL_BIT_FIELD (field))
	return false;

      nfields++;
      if (nfields > MAX_NFIELDS_FOR_SRA)
	return false;
    }

  /* If the structure had no FIELD_DECLs, then don't bother
     scalarizing it.  */
  return nfields > 0;
}


/* Replace the COMPONENT_REF pointed-to by TP inside STMT with the
   corresponding scalar replacement from SRA_MAP.  */

static inline void
scalarize_component_ref (tree stmt, tree *tp)
{
  /* When scalarizing a function argument, we will need to insert copy-in
     operations from the original PARM_DECLs. Note that these copy-in
     operations may end up being dead, but we won't know until we rename
     the new variables into SSA.  */
  if (TREE_CODE (TREE_OPERAND (*tp, 0)) == PARM_DECL)
    SET_BIT (needs_copy_in, var_ann (TREE_OPERAND (*tp, 0))->uid);

  *tp = get_scalar_for (TREE_OPERAND (*tp, 0), TREE_OPERAND (*tp, 1));
  modify_stmt (stmt);
}


/* Scalarize the structure assignment for the statement pointed by SI_P.  */

static void
scalarize_structure_assignment (block_stmt_iterator *si_p)
{
  var_ann_t lhs_ann, rhs_ann;
  tree lhs, rhs, list, orig_stmt;

  orig_stmt = bsi_stmt (*si_p);
  lhs = TREE_OPERAND (orig_stmt, 0);
  rhs = TREE_OPERAND (orig_stmt, 1);
  list = NULL_TREE;

#if defined ENABLE_CHECKING
  if (TREE_CODE (orig_stmt) != MODIFY_EXPR)
    abort ();
#endif

  /* Remove unnecessary casts from RHS.  */
  STRIP_USELESS_TYPE_CONVERSION (rhs);

  lhs_ann = DECL_P (lhs) ? var_ann (lhs) : NULL;
  rhs_ann = DECL_P (rhs) ? var_ann (rhs) : NULL;

#if defined ENABLE_CHECKING
  /* Two different variables should not have the same UID.  */
  if (lhs_ann
      && rhs_ann
      && lhs != rhs
      && lhs_ann->uid == rhs_ann->uid)
    abort ();
#endif

  /* Both LHS and RHS are scalarizable.  */
  if (lhs_ann
      && TEST_BIT (sra_candidates, lhs_ann->uid)
      && rhs_ann
      && TEST_BIT (sra_candidates, rhs_ann->uid))
    list = create_scalar_copies (lhs, rhs, SCALAR_SCALAR);

  /* Only RHS is scalarizable.  */
  else if (rhs_ann
      && TEST_BIT (sra_candidates, rhs_ann->uid))
    list = create_scalar_copies (lhs, rhs, FIELD_SCALAR);

  /* Only LHS is scalarizable.  */
  else if (lhs_ann
      && TEST_BIT (sra_candidates, lhs_ann->uid))
    list = create_scalar_copies (lhs, rhs, SCALAR_FIELD);


  /* If neither side is scalarizable, do nothing else.  */
  if (list == NULL_TREE)
    return;

  /* Replace the existing statement with the newly created list of
     scalarized copies.  When replacing the original statement, the first
     copy replaces it and the remaining copies are inserted either after
     the first copy or on the outgoing edges of the original statement's
     block.  */
  {
    tree_stmt_iterator tsi = tsi_start (list);
    bsi_replace (si_p, tsi_stmt (tsi), true);
    tsi_delink (&tsi);
    if (stmt_ends_bb_p (orig_stmt))
      insert_edge_copies (list, bb_for_stmt (orig_stmt));
    else
      bsi_insert_after (si_p, list, BSI_CONTINUE_LINKING);
  }
}


/* Traverse all the referenced variables in the program looking for
   structures that could be replaced with scalars.  */

static void
find_candidates_for_sra (void)
{
  size_t i;

  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);

      if (TREE_CODE (TREE_TYPE (var)) == RECORD_TYPE
	  && can_be_scalarized_p (var))
	SET_BIT (sra_candidates, var_ann (var)->uid);
    }
}


/* Insert STMT on all the outgoing edges out of BB.  Note that if BB
   has more than one edge, STMT will be replicated for each edge.  Also,
   abnormal edges will be ignored.  */

static inline void
insert_edge_copies (tree stmt, basic_block bb)
{
  edge e;
  bool first_copy;

  first_copy = true;
  for (e = bb->succ; e; e = e->succ_next)
    {
      /* We don't need to insert copies on abnormal edges.  The
	 value of the scalar replacement is not guaranteed to
	 be valid through an abnormal edge.  */
      if (!(e->flags & EDGE_ABNORMAL))
	{
	  if (first_copy)
	    {
	      bsi_insert_on_edge (e, stmt);
	      first_copy = false;
	    }
	  else
	    bsi_insert_on_edge (e, lhd_unsave_expr_now (stmt));
	}
    }
}


/* Create and return a list of assignments to perform a scalarized
   structure assignment 'LHS = RHS'.  Both LHS and RHS are assumed to be
   of an aggregate type.  Three types of copies may be specified:

   SCALAR_SCALAR will emit assignments for all the scalar temporaries
      corresponding to the fields of LHS and RHS.

   FIELD_SCALAR will emit assignments from the scalar replacements of
      RHS into each of the fields of the LHS.

   SCALAR_FIELD will emit assignments from each field of the RHS into
      the scalar replacements of the LHS.  */

static tree
create_scalar_copies (tree lhs, tree rhs, enum sra_copy_mode mode)
{
  tree type, f, last_stmt, list;
  int f_ix, lhs_ix, rhs_ix;

#if defined ENABLE_CHECKING
  /* Sanity checking.  Check that we are not trying to scalarize a
     non-decl.  */
  if (!DECL_P (lhs)
      && (mode == SCALAR_FIELD
	  || mode == SCALAR_SCALAR))
    abort ();

  if (!DECL_P (rhs)
      && (mode == FIELD_SCALAR
	  || mode == SCALAR_SCALAR))
    abort ();
#endif

  lhs_ix = DECL_P (lhs) ? (int) var_ann (lhs)->uid : -1;
  rhs_ix = DECL_P (rhs) ? (int) var_ann (rhs)->uid : -1;
  type = TREE_TYPE (lhs);

  list = alloc_stmt_list ();

  /* VA_ARG_EXPRs have side effects, so we need to copy it first to a
     temporary before scalarizing.  FIXME: This should disappear once
     VA_ARG_EXPRs are properly lowered.  */
  if (TREE_CODE (rhs) == VA_ARG_EXPR)
    {
      /* Add TMP = VA_ARG_EXPR <>  */
      tree stmt, tmp = create_tmp_var (TREE_TYPE (rhs), "T");
      add_referenced_tmp_var (tmp);
      stmt = build (MODIFY_EXPR, TREE_TYPE (rhs), tmp, rhs);
      modify_stmt (stmt);
      append_to_statement_list (stmt, &list);

      /* Add LHS = TMP  */
      stmt = build (MODIFY_EXPR, TREE_TYPE (lhs), lhs, tmp);
      append_to_statement_list (stmt, &list);

      /* Set RHS to be the new temporary TMP.  */
      rhs = tmp;
    }

  /* Now create scalar copies for each individual field according to MODE.  */
  last_stmt = NULL_TREE;
  for (f_ix = 0, f = TYPE_FIELDS (type); f; f = TREE_CHAIN (f))
    {
      tree lhs_var, rhs_var, stmt;

      /* Only copy FIELD_DECLs.  */
      if (TREE_CODE (f) != FIELD_DECL)
	continue;

      /* Set up the LHS and the RHS of the new copy operation
	 based on MODE.  */
      if (mode == SCALAR_SCALAR)
	{
	  lhs_var = lookup_scalar (lhs_ix, f_ix, TREE_TYPE (f));
	  rhs_var = lookup_scalar (rhs_ix, f_ix, TREE_TYPE (f));
	}
      else if (mode == FIELD_SCALAR)
	{
	  if (DECL_P (lhs))
	    lhs_var = build (COMPONENT_REF, TREE_TYPE (f), lhs, f);
	  else
	    {
	      /* Avoid sharing the LHS when building the different
		 COMPONENT_REFs.  */
	      if (f_ix == 0)
		lhs_var = build (COMPONENT_REF, TREE_TYPE (f), lhs, f);
	      else
		lhs_var = build (COMPONENT_REF, TREE_TYPE (f),
				 unshare_expr (lhs), f);
	    }

	  rhs_var = lookup_scalar (rhs_ix, f_ix, TREE_TYPE (f));
	}
      else if (mode == SCALAR_FIELD)
	{
	  lhs_var = lookup_scalar (lhs_ix, f_ix, TREE_TYPE (f));

	  /* Handle the different types of expressions that we could find
	     on the RHS.  */
	  if (DECL_P (rhs))
	    {
	      /* Common case.  The RHS is a non-scalarizable DECL.  */
	      rhs_var = build (COMPONENT_REF, TREE_TYPE (f), rhs, f);
	    }
	  else if (TREE_CODE (rhs) == CONSTRUCTOR)
	    {
	      /* CONSTRUCTOR nodes mean that every field should be assigned a
		 zero value.  */
	      rhs_var = convert (TREE_TYPE (f), integer_zero_node);
	    }
	  else
	    {
	      /* Any other kind of expression.  Make sure that it doesn't
		 have side effects and make copies to avoid sharing.  */
	      if (TREE_SIDE_EFFECTS (rhs))
		abort ();

	      if (f_ix == 0)
		rhs_var = build (COMPONENT_REF, TREE_TYPE (f), rhs, f);
	      else
		rhs_var = build (COMPONENT_REF, TREE_TYPE (f),
				 unshare_expr (rhs), f);
	    }
	}
      else
	abort ();

      /* When making *_SCALAR copies from PARM_DECLs, we will need to
	 insert copy-in operations from the original PARM_DECLs.  Note that
	 these copy-in operations may end up being dead, but we won't know
	 until we rename the new variables into SSA.  */
      if ((mode == SCALAR_SCALAR
	   || mode == FIELD_SCALAR)
	  && rhs_ix >= 0
	  && TREE_CODE (rhs) == PARM_DECL)
	SET_BIT (needs_copy_in, rhs_ix);

      stmt = build (MODIFY_EXPR, TREE_TYPE (lhs_var), lhs_var, rhs_var);
      modify_stmt (stmt);
      last_stmt = stmt;
      append_to_statement_list (stmt, &list);

      /* Note that we cannot increase the field index in the loop header
	 because we skip non-decl fields.  */
      f_ix++;
    }

  /* For FIELD_SCALAR copies, we will need to rename the structure on the
     LHS, as these copies are introducing new versions of the structure.
     Since all the assignments are to the same structure, we just need
     to look at the last statement inserted.  */
  if (last_stmt && mode == FIELD_SCALAR)
    {
      size_t i;
      varray_type vdefs;

      get_stmt_operands (last_stmt);

      vdefs = vdef_ops (stmt_ann (last_stmt));
      for (i = 0; vdefs && i < VARRAY_ACTIVE_SIZE (vdefs) / 2; i++)
	{
	  tree sym = VDEF_RESULT (vdefs, i);
	  SET_BIT (vars_to_rename, var_ann (sym)->uid);
	}
    }

  return list;
}


/* Traverse all the statements in the function replacing references to
   scalarizable structures with their corresponding scalar temporaries.  */

static void
scalarize_structures (void)
{
  basic_block bb;
  block_stmt_iterator si;
  size_t i;

  FOR_EACH_BB (bb)
    for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
      {
	tree stmt;
	stmt_ann_t ann;

	stmt = bsi_stmt (si);
	ann = stmt_ann (stmt);

	/* If the statement has no virtual operands, then it doesn't make
	   structure references that we care about.  */
	if (vdef_ops (ann) == NULL
	    && vuse_ops (ann) == NULL)
	  continue;

	/* Structure references may only appear in certain statements.  */
	if (TREE_CODE (stmt) != MODIFY_EXPR
	    && TREE_CODE (stmt) != CALL_EXPR
	    && TREE_CODE (stmt) != RETURN_EXPR
	    && TREE_CODE (stmt) != ASM_EXPR)
	  continue;

	scalarize_stmt (&si);
      }

  /* Initialize the scalar replacements for every structure that is a
     function argument.  */
  EXECUTE_IF_SET_IN_SBITMAP (sra_candidates, 0, i,
    {
      tree var = referenced_var (i);
      if (TREE_CODE (var) == PARM_DECL
	  && TEST_BIT (needs_copy_in, var_ann (var)->uid))
	{
	  tree list = create_scalar_copies (var, var, SCALAR_FIELD);
	  bsi_insert_on_edge (ENTRY_BLOCK_PTR->succ, list);
	}
    });

  /* Commit edge insertions.  */
  bsi_commit_edge_inserts (false, NULL);
}


/* Scalarize structure references in the statement pointed by SI_P.  */

static void
scalarize_stmt (block_stmt_iterator *si_p)
{
  tree stmt = bsi_stmt (*si_p);

  /* Handle assignments.  */
  if (TREE_CODE (stmt) == MODIFY_EXPR
      && TREE_CODE (TREE_OPERAND (stmt, 1)) != CALL_EXPR)
    scalarize_modify_expr (si_p);

  /* Handle function calls.  */
  else if (TREE_CODE (stmt) == CALL_EXPR
	   || (TREE_CODE (stmt) == MODIFY_EXPR
	       && TREE_CODE (TREE_OPERAND (stmt, 1)) == CALL_EXPR))
    scalarize_call_expr (si_p);

  /* Handle ASM_EXPRs.  */
  else if (TREE_CODE (stmt) == ASM_EXPR)
    scalarize_asm_expr (si_p);

  /* Handle RETURN_EXPR.  */
  else if (TREE_CODE (stmt) == RETURN_EXPR)
    scalarize_return_expr (si_p);
}


/* Helper for scalarize_stmt to handle assignments.  */

static void
scalarize_modify_expr (block_stmt_iterator *si_p)
{
  tree var;
  tree stmt = bsi_stmt (*si_p);
  tree lhs = TREE_OPERAND (stmt, 0);
  tree rhs = TREE_OPERAND (stmt, 1);

  /* Found AGGREGATE.FIELD = ...  */
  if (TREE_CODE (lhs) == COMPONENT_REF
      && DECL_P (var = TREE_OPERAND (lhs, 0))
      && TEST_BIT (sra_candidates, var_ann (var)->uid))
    scalarize_component_ref (stmt, &TREE_OPERAND (stmt, 0));

  /* Found ... = AGGREGATE.FIELD  */
  else if (TREE_CODE (rhs) == COMPONENT_REF
	   && DECL_P (var = TREE_OPERAND (rhs, 0))
	   && TEST_BIT (sra_candidates, var_ann (var)->uid))
    scalarize_component_ref (stmt, &TREE_OPERAND (stmt, 1));

  /* Found ... = BIT_FIELD_REF <>.  This is similar to a CALL_EXPR, if the
     operand of the BIT_FIELD_REF is a scalarizable structure, we need to
     copy from its scalar replacements before doing the bitfield operation.

     FIXME: BIT_FIELD_REFs are often generated by fold-const.c.  This is
     not always desirable because they obfuscate the original predicates,
     limiting what the tree optimizers may do.  For instance, in
     testsuite/g++.dg/opt/nrv4.C the use of SRA allows the optimizers to
     optimize function main() to 'return 0;'.  However, the folder
     generates a BIT_FIELD_REF operation for one of the comparisons,
     preventing the optimizers from removing all the redundant
     operations.  */
  else if (TREE_CODE (rhs) == BIT_FIELD_REF
           && DECL_P (var = TREE_OPERAND (rhs, 0))
	   && TEST_BIT (sra_candidates, var_ann (var)->uid))
    {
      tree list = create_scalar_copies (var, var, FIELD_SCALAR);
      bsi_insert_before (si_p, list, BSI_SAME_STMT);
    }

  /* Found AGGREGATE = ... or ... = AGGREGATE  */
  else if (DECL_P (lhs) || DECL_P (rhs))
    scalarize_structure_assignment (si_p);
}


/* Scalarize structure references in LIST.  */

static inline void
scalarize_tree_list (tree list, block_stmt_iterator *si_p)
{
  tree op;

  for (op = list; op; op = TREE_CHAIN (op))
    {
      tree var, arg = TREE_VALUE (op);

      if (DECL_P (arg)
	  && TEST_BIT (sra_candidates, var_ann (arg)->uid))
	{
	  tree list = create_scalar_copies (arg, arg, FIELD_SCALAR);
	  bsi_insert_before (si_p, list, BSI_SAME_STMT);
	}
      else if (TREE_CODE (arg) == COMPONENT_REF
	       && DECL_P (var = TREE_OPERAND (arg, 0))
	       && TEST_BIT (sra_candidates, var_ann (var)->uid))
	{
	  tree stmt = bsi_stmt (*si_p);
	  scalarize_component_ref (stmt, &TREE_VALUE (op));
	}
    }
}


/* Helper for scalarize_stmt to handle function calls.  */

static void
scalarize_call_expr (block_stmt_iterator *si_p)
{
  tree stmt = bsi_stmt (*si_p);
  tree call = (TREE_CODE (stmt) == MODIFY_EXPR) ? TREE_OPERAND (stmt, 1) : stmt;

  /* First scalarize the arguments.  Order is important, because the copy
     operations for the arguments need to go before the call.
     Scalarization of the return value needs to go after the call.  */
  scalarize_tree_list (TREE_OPERAND (call, 1), si_p);

  /* Scalarize the return value, if any.  */
  if (TREE_CODE (stmt) == MODIFY_EXPR)
    {
      tree var;

      /* If the LHS of the assignment is a scalarizable structure, insert
	 copies into the scalar replacements after the call.  */
      if (DECL_P (var = TREE_OPERAND (stmt, 0))
	  && TEST_BIT (sra_candidates, var_ann (var)->uid))
	{
	  tree list = create_scalar_copies (var, var, SCALAR_FIELD);
	  if (stmt_ends_bb_p (stmt))
	    insert_edge_copies (list, bb_for_stmt (stmt));
	  else
	    bsi_insert_after (si_p, list, BSI_NEW_STMT);
	}
    }
}


/* Helper for scalarize_stmt to handle ASM_EXPRs.  */

static void
scalarize_asm_expr (block_stmt_iterator *si_p)
{
  tree stmt = bsi_stmt (*si_p);

  scalarize_tree_list (ASM_INPUTS (stmt), si_p);
  scalarize_tree_list (ASM_OUTPUTS (stmt), si_p);
}


/* Helper for scalarize_stmt to handle return expressions.  */

static void
scalarize_return_expr (block_stmt_iterator *si_p)
{
  tree stmt = bsi_stmt (*si_p);
  tree op = TREE_OPERAND (stmt, 0);

  if (op == NULL_TREE)
    return;

  if (TREE_CODE (op) == RESULT_DECL
      && TEST_BIT (sra_candidates, var_ann (op)->uid))
    {
      tree list = create_scalar_copies (op, op, FIELD_SCALAR);
      bsi_insert_before (si_p, list, BSI_SAME_STMT);
    }
  else if (TREE_CODE (op) == MODIFY_EXPR)
    {
      tree var;
      tree *rhs_p = &TREE_OPERAND (op, 1);

      /* Handle 'return STRUCTURE;'  */
      if (DECL_P (*rhs_p)
	  && TEST_BIT (sra_candidates, var_ann (*rhs_p)->uid))
	{
	  tree list = create_scalar_copies (*rhs_p, *rhs_p, FIELD_SCALAR);
	  bsi_insert_before (si_p, list, BSI_SAME_STMT);
	}

      /* Handle 'return STRUCTURE.FIELD;'  */
      else if (TREE_CODE (*rhs_p) == COMPONENT_REF
	       && DECL_P (var = TREE_OPERAND (*rhs_p, 0))
	       && TEST_BIT (sra_candidates, var_ann (var)->uid))
	scalarize_component_ref (stmt, rhs_p);

      /* Handle 'return CALL_EXPR;'  */
      else if (TREE_CODE (*rhs_p) == CALL_EXPR)
	scalarize_tree_list (TREE_OPERAND (*rhs_p, 1), si_p);
    }
}


/* Main entry point to Scalar Replacement of Aggregates (SRA).  This pass
   re-writes non-aliased structure references into scalar temporaries.  The
   goal is to expose some/all structures to the scalar optimizers.

   FNDECL is the function to process.
   
   VARS_TO_RENAME_P is a pointer to the set of variables that need to be
   renamed into SSA after this pass is done.  These are going to be all the
   new scalars created by the SRA process.  Notice that since this pass
   creates new variables, the bitmap representing all the variables in the
   program will be re-sized here.

   PHASE indicates which dump file from the DUMP_FILES array to use when
   dumping debugging information.

   TODO

   1- Scalarize COMPLEX_TYPEs
   2- Scalarize ARRAY_REFs that are always referenced with a
      constant index.
   3- Timings to determine when scalarization is not profitable.
   4- Determine what's a good value for MAX_NFIELDS_FOR_SRA.  */

void
tree_sra (tree fndecl, sbitmap *vars_to_rename_p, enum tree_dump_index phase)
{
  size_t old_num_referenced_vars;

  timevar_push (TV_TREE_SRA);

  dump_file = dump_begin (phase, &dump_flags);

  /* Initialize local variables.  */
  sra_candidates = sbitmap_alloc (num_referenced_vars);
  sbitmap_zero (sra_candidates);
  sra_map = NULL;
  vars_to_rename = NULL;
  needs_copy_in = NULL;
  old_num_referenced_vars = num_referenced_vars;

  /* Find structures to be scalarized.  */
  find_candidates_for_sra ();

  /* If we found any, re-write structure references with their
     corresponding scalar replacement.  */
  if (sbitmap_first_set_bit (sra_candidates) >= 0)
    {
      sra_map = xcalloc (num_referenced_vars, sizeof (tree *));

      vars_to_rename = sbitmap_alloc (num_referenced_vars);
      sbitmap_zero (vars_to_rename);

      needs_copy_in = sbitmap_alloc (num_referenced_vars);
      sbitmap_zero (needs_copy_in);

      scalarize_structures ();

      /* Mark the new scalar variables to be renamed into SSA.  */
      if (num_referenced_vars > old_num_referenced_vars)
	{
	  size_t i;

	  *vars_to_rename_p = sbitmap_realloc (*vars_to_rename_p,
					       num_referenced_vars);
	  sbitmap_zero (*vars_to_rename_p);

	  /* Mark the new scalar replacements.  */
	  for (i = old_num_referenced_vars; i < num_referenced_vars; i++)
	    SET_BIT (*vars_to_rename_p, i);

	  /* Also mark existing structures that were modified during the
	     scalarization process.  */
	  EXECUTE_IF_SET_IN_SBITMAP (vars_to_rename, 0, i,
	    SET_BIT (*vars_to_rename_p, i));
	}
    }

  /* Debugging dumps.  */
  if (dump_file)
    {
      if (sra_map && 1/*(dump_flags & TDF_DETAILS)*/)
	{
	  size_t i, j;
	  tree f;
	  const char *fname =
	    (*lang_hooks.decl_printable_name) (current_function_decl, 2);

	  fprintf (dump_file, "Scalar replacements for %s:\n\n", fname);
	  for (i = 0; i < old_num_referenced_vars; i++)
	    {
	      if (sra_map[i])
		{
		  for (j = 0, f = TYPE_FIELDS (TREE_TYPE (referenced_var (i)));
		       f;
		       f = TREE_CHAIN (f))
		    {
		      if (TREE_CODE (f) != FIELD_DECL)
			continue;

		      if (sra_map[i][j])
			{
			  print_generic_expr (dump_file, referenced_var (i), 0);
			  fprintf (dump_file, ".%s -> %s\n",
				   get_name (f),
				   get_name (sra_map[i][j]));
			}

		      j++;
		    }

		  fprintf (dump_file, "\n");
		}
	    }
	}

      dump_function_to_file (fndecl, dump_file, dump_flags);
      dump_end (phase, dump_file);
    }

  /* Free allocated memory.  */
  if (sra_map)
    {
      size_t i;
      for (i = 0; i < old_num_referenced_vars; i++)
	free (sra_map[i]);
      free (sra_map);
    }

  sbitmap_free (sra_candidates);

  timevar_pop (TV_TREE_SRA);
}
