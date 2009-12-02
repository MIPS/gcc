/* Simplify non-SSA GIMPLE trees before control flow-graph construction and
   optimization passes.

   Copyright (C) 2007-2009 Free Software Foundation, Inc.

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
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

Authors:
   Andrea Ornstein
   Erven Rohou
   Gabriele Svelto

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "timevar.h"
#include "gimple.h"
#include "tree.h"
#include "tree-pass.h"
#include "pointer-set.h"

/******************************************************************************
 * Type definitions                                                           *
 ******************************************************************************/

/* Entry of the equivalent labels hash-table */
struct eqv_label_entry_t
{
  tree label; /* Pointer to the label declaration */
  unsigned int val; /* Equivalence class */
};

/******************************************************************************
 * Local function prototypes, macros and variables                            *
 ******************************************************************************/

/* Label manipulation functions */
static bool is_eqv_label (tree, tree);
static void group_labels (void);
static bool eqv_label_dispose (const void *, void **, void *data);

/* Switch-conversion functions */
static void merge_cases_into_ranges (gimple);
static void case_to_cond_expr (gimple, tree, tree, gimple_stmt_iterator *);
static void case_range_to_cond_expr (gimple, tree, tree,
				     gimple_stmt_iterator *);
static void cases_to_switch (gimple, unsigned int, unsigned int,
			     tree, gimple_stmt_iterator *);
static gimple_seq simp_cil_switch (gimple);

/* Top-level functionality */
static unsigned int simp_cil_early (void);
static bool simp_cil_early_gate (void);

/******************************************************************************
 * Globals                                                                    *
 ******************************************************************************/

/* Values used by the switch-conversion heuristics */
#define SIMP_SWITCH_RANGE_SIZE (3)
#define SIMP_SWITCH_HOLE_SIZE (3)

/* Hash-table used for storing equivalent labels */
static struct pointer_map_t *eqv_labels = NULL;

/******************************************************************************
 * Label manipulation functions                                               *
 ******************************************************************************/

/* Returns true if two labels are equivalent. */

static bool
is_eqv_label (tree label1, tree label2)
{
  struct eqv_label_entry_t *t1, *t2;

  t1 = (struct eqv_label_entry_t *) *pointer_map_contains (eqv_labels, label1);
  t2 = (struct eqv_label_entry_t *) *pointer_map_contains (eqv_labels, label2);

  if (t1 == NULL || t2 == NULL)
    return false;
  else
    return t1->val == t2->val;
}

/* Scan a function for groups of adjacent labels, associate a value with the
   group and store it in the 'eqv_labels' hash table. This is done by grouping
   adjacent labels by numbering them with the same value of the equivalent
   labels. A hash-table is used for storing the labels to speed up lookups
   subsequently. Since the control flow-graph has not been built yet we might
   not group equivalent labels which are non-adjacent. */

static void
group_labels (void)
{
  gimple_stmt_iterator gsi;
  gimple stmt = NULL;
  unsigned int value = 0;
  bool prev_label = false;
  struct eqv_label_entry_t *entry;

  gsi = gsi_start (gimple_body (current_function_decl));

  while (!gsi_end_p (gsi))
    {
      stmt = gsi_stmt (gsi);
      gsi_next (&gsi);

      if (gimple_code (stmt) == GIMPLE_LABEL)
	{
	  /* Insert the label in the hash-table */
	  entry = XCNEW (struct eqv_label_entry_t);
	  entry->label = gimple_label_label (stmt);
	  entry->val = value;
	  *pointer_map_insert (eqv_labels, gimple_label_label (stmt)) = entry;
	  prev_label = true;
	}
      else if (prev_label)
	{
	  prev_label = false;
	  value++;
	}
    }
}

/* Used in pointer_map_traverse () to free eqv_label_t entries */

static bool
eqv_label_dispose (const void *key ATTRIBUTE_UNUSED, void **value,
		   void *data ATTRIBUTE_UNUSED)
{
  XDELETE (*value);

  return true;
}

/******************************************************************************
 * Switch-conversion functions                                                *
 ******************************************************************************/

/* Merge adjacent cases which jump to the same label into ranges, i.e.
   case 1: case 2: case 3: -> case 1 ... 3: */

static void
merge_cases_into_ranges (gimple switch_stmt)
{
  unsigned int old_size = gimple_switch_num_labels (switch_stmt);
  unsigned int i, j, new_size = old_size;
  tree default_case = gimple_switch_default_label (switch_stmt);
  tree default_label = CASE_LABEL (default_case);

  /* Look for possible opportunities to merge cases.  Ignore the first label
     of the switch because it is the default case.  */
  i = 1;

  while (i < old_size)
    {
      tree base_case, base_label, base_high;
      base_case = gimple_switch_label (switch_stmt, i);
      base_label = CASE_LABEL (base_case);

      /* Discard cases that have the same destination as the default case.  */
      if (is_eqv_label (base_label, default_label))
	{
	  gimple_switch_set_label (switch_stmt, i, NULL_TREE);
	  i++;
	  new_size--;
	  continue;
	}

      base_high = CASE_HIGH (base_case) ? CASE_HIGH (base_case)
					: CASE_LOW (base_case);
      i++;
      /* Try to merge case labels.  Break out when we reach the end
	 of the label vector or when we cannot merge the next case
	 label with the current one.  */
      while (i < old_size)
	{
	  tree merge_case = gimple_switch_label (switch_stmt, i);
          tree merge_label = CASE_LABEL (merge_case);
	  tree t = int_const_binop (PLUS_EXPR, base_high,
				    integer_one_node, 1);

	  /* Merge the cases if they jump to the same place,
	     and their ranges are consecutive.  */
	  if (is_eqv_label (merge_label, base_label)
	      && tree_int_cst_equal (CASE_LOW (merge_case), t))
	    {
	      base_high = CASE_HIGH (merge_case) ? CASE_HIGH (merge_case)
						 : CASE_LOW (merge_case);
	      CASE_HIGH (base_case) = base_high;
	      gimple_switch_set_label (switch_stmt, i, NULL_TREE);
	      new_size--;
	      i++;
	    }
	  else
	    break;
	}
    }

  /* Compress the case labels in the label vector, and adjust the
     length of the vector.  */
  for (i = 0, j = 0; i < new_size; i++)
    {
      while ((j < gimple_switch_num_labels (switch_stmt))
	     && (gimple_switch_label (switch_stmt, j) == NULL_TREE))
	{
	  j++;
	}

      gimple_switch_set_label (switch_stmt, i,
			       gimple_switch_label (switch_stmt, j++));
    }

  gimple_switch_set_num_labels (switch_stmt, new_size);
}

/* Turn a single isolated case into a COND_EXPR */

static void
case_to_cond_expr (gimple switch_stmt, tree single_case, tree deft,
		   gimple_stmt_iterator *gsi)
{
  gimple cmp_stmt;

  /* Build the GIMPLE_COND statement and append it */
  cmp_stmt = gimple_build_cond (EQ_EXPR, gimple_switch_index (switch_stmt),
				CASE_LOW (single_case),
				CASE_LABEL (single_case), deft);
  gsi_insert_after (gsi, cmp_stmt, GSI_CONTINUE_LINKING);
}

/* Turn a case range into a couple of COND_EXPRs */

static void
case_range_to_cond_expr (gimple switch_stmt, tree case_range, tree deft,
			 gimple_stmt_iterator *gsi)
{
  tree label_decl = create_artificial_label ();
  gimple cmp1_stmt, cmp2_stmt, label_stmt;

  gcc_assert (CASE_HIGH (case_range) != NULL_TREE
              && CASE_LOW (case_range) != NULL_TREE);

  /* Build and append the 1st GIMPLE_COND */
  cmp1_stmt = gimple_build_cond (GE_EXPR, gimple_switch_index (switch_stmt),
				 CASE_LOW (case_range), label_decl, deft);
  gsi_insert_after (gsi, cmp1_stmt, GSI_CONTINUE_LINKING);

  /* Create a new label and append it */
  label_stmt = gimple_build_label (label_decl);
  gsi_insert_after (gsi, label_stmt, GSI_CONTINUE_LINKING);

  /* Build and append the 2nd GIMPLE_COND */
  cmp2_stmt = gimple_build_cond (LE_EXPR, gimple_switch_index (switch_stmt),
				 CASE_HIGH (case_range),
				 CASE_LABEL (case_range), deft);
  gsi_insert_after (gsi, cmp2_stmt, GSI_CONTINUE_LINKING);
}

/* Turn the cases between START and END (included) into a new switch
   statement */

static void
cases_to_switch (gimple switch_stmt, unsigned int start, unsigned int end,
		 tree deft, gimple_stmt_iterator *gsi)
{
  gimple stmt;
  VEC (tree, heap) *vec;
  tree type, elt;
  unsigned int i, len;
  tree low, high, range, limit;

  type = TREE_TYPE (CASE_LOW (gimple_switch_label (switch_stmt, start)));
  limit = build_int_cst (type, 8192);

  while (start <= end)
    {
      elt = gimple_switch_label (switch_stmt, start);
      low = CASE_LOW (elt);
      len = 1;

      while (start + len <= end)
        {
	  elt = gimple_switch_label (switch_stmt, start + len);
          high = CASE_HIGH (elt) ? CASE_HIGH (elt) : CASE_LOW (elt);
	  range = size_binop (MINUS_EXPR, high, low);

	  if (tree_int_cst_lt (range, limit))
	    len++;
	  else
	    break;
        }

      /* Build the switch cases */
      vec = VEC_alloc (tree, heap, len);

      for (i = 0; i < len; i++)
	{
	  VEC_quick_push (tree, vec,
			  gimple_switch_label (switch_stmt, start + i));
	}

      /* Build and append the new switch statement */
      stmt = gimple_build_switch_vec (gimple_switch_index (switch_stmt),
				      build3 (CASE_LABEL_EXPR, void_type_node,
				              NULL_TREE, NULL_TREE, deft),
				      vec);
      gsi_insert_after (gsi, stmt, GSI_CONTINUE_LINKING);
      VEC_free (tree, heap, vec);

      /* Move ahead in the labels */
      start += len;
    }
}

/* Break a switch statement pointed by GIMPLE_SWITCH into multiple expressions
   in order to produce better/smaller code in the CIL back-end. Large case
   ranges or isolated cases are turned into GIMPLE_CONDs, large 'holes' inside
   the switch are removed replacing a single switch with two or more distinct
   switches. The sequence holding the replacement is returned. If no
   replacement is needed NULL is returned instead */

static gimple_seq
simp_cil_switch (gimple switch_stmt)
{
  tree curr, next;
  gimple stmt;
  gimple_seq seq = gimple_seq_alloc ();
  gimple_stmt_iterator gsi = gsi_start (seq);
  tree range_size, hole_size, curr_high, next_low;
  tree type, label_decl;
  unsigned int i = 1;
  unsigned int base_idx = 1;

  merge_cases_into_ranges (switch_stmt);

  if (gimple_switch_num_labels (switch_stmt) == 1)
    {
      /* Only the default statement, nothing to do as this will be optimized
         out later by the subsequent passes */
      gimple_seq_free (seq);
      return NULL;
    }

  type = TREE_TYPE (CASE_LOW (gimple_switch_label (switch_stmt, 1)));
  range_size = build_int_cst (type, SIMP_SWITCH_RANGE_SIZE);
  hole_size = build_int_cst (type, SIMP_SWITCH_HOLE_SIZE);

  while (true)
    {
      curr = gimple_switch_label (switch_stmt, i);

      if (i == gimple_switch_num_labels (switch_stmt) - 1)
	{
	  /* This is the last case, emit the previous cases if needed.
	     If base_idx != i and base_idx is 0 then the original switch has
	     been left untouched and it is not replaced */
          label_decl = CASE_LABEL (gimple_switch_default_label (switch_stmt));

	  if (base_idx != i)
	    {
	      if (base_idx == 1)
		{
		  gimple_seq_free (seq);
		  return NULL;
		}
	      else
		cases_to_switch (switch_stmt, base_idx, i, label_decl, &gsi);
	    }
	  else if (CASE_HIGH (curr) != NULL_TREE)
	    case_range_to_cond_expr (switch_stmt, curr, label_decl, &gsi);
	  else
	    case_to_cond_expr (switch_stmt, curr, label_decl, &gsi);

	  break;
	}

      next = gimple_switch_label (switch_stmt, i + 1);

      if (CASE_HIGH (curr) != NULL_TREE)
	{
	  /* This case is a range, if it's large enough we blow it up into two
	     COND_EXPRs. We don't need to check if we have some pending cases to
	     deal with as we have already emitted them in the previous iteration
	     as implemented below. */
	  tree low = size_binop (PLUS_EXPR, CASE_LOW (curr), range_size);
	  tree high = CASE_HIGH (curr);

	  /* if (low + SIMP_SWITCH_RANGE_SIZE > high) */
	  if (tree_int_cst_compare (low, high) == 1)
	    {
	      label_decl = create_artificial_label ();

	      gcc_assert (i == base_idx);
	      case_range_to_cond_expr (switch_stmt, curr, label_decl, &gsi);

	      /* Create the label to the next statement and append it */
	      stmt = gimple_build_label (label_decl);
	      gsi_insert_after (&gsi, stmt, GSI_CONTINUE_LINKING);

	      i++;
	      base_idx = i;
	      continue;
	    }
	}

      if (CASE_HIGH (next) != NULL_TREE)
	{
	  /* The next case is a range, if it's large enough blow it up into two
	     COND_EXPRs then emit the previous cases. */
	  tree low = size_binop (PLUS_EXPR, CASE_LOW (next), range_size);
	  tree high = CASE_HIGH (next);

	  /* if (low + SIMP_SWITCH_RANGE_SIZE > high) */
	  if (tree_int_cst_compare (low, high) == 1)
	    {
	      label_decl = create_artificial_label ();

	      if (base_idx != i)
		cases_to_switch (switch_stmt, base_idx, i, label_decl, &gsi);
	      else if (CASE_HIGH (curr) != NULL_TREE)
		case_range_to_cond_expr (switch_stmt, curr, label_decl, &gsi);
	      else
		case_to_cond_expr (switch_stmt, curr, label_decl, &gsi);

	      /* Create the label to the next statement and append it */
	      stmt = gimple_build_label (label_decl);
	      gsi_insert_after (&gsi, stmt, GSI_CONTINUE_LINKING);

	      i++;
	      base_idx = i;
	      continue;
	    }
	}

	/* Detect 'holes', if a large enough hole is found emit the previous
	   cases. */
	if (CASE_HIGH (curr) != NULL_TREE)
	  curr_high = CASE_HIGH (curr);
	else
	  curr_high = CASE_LOW (curr);

	curr_high = size_binop (PLUS_EXPR, curr_high, hole_size);
	next_low = CASE_LOW (next);

	/* if (curr_high + SIMP_SWITCH_HOLE_SIZE < next_low) */
	if (tree_int_cst_lt (curr_high, next_low))
	  {
            label_decl = create_artificial_label ();

	    if (base_idx != i)
	      cases_to_switch (switch_stmt, base_idx, i, label_decl, &gsi);
	    else if (CASE_HIGH (curr) != NULL_TREE)
	      case_range_to_cond_expr (switch_stmt, curr, label_decl, &gsi);
	    else
	      case_to_cond_expr (switch_stmt, curr, label_decl, &gsi);

	    /* Create the label to the next statement and append it */
	    stmt = gimple_build_label (label_decl);
	    gsi_insert_after (&gsi, stmt, GSI_CONTINUE_LINKING);

	    i++;
	    base_idx = i;
	    continue;
	  }

      i++;
    }

  return seq;
}

/******************************************************************************
 * Top-level functionality                                                    *
 ******************************************************************************/

/* Main function of this pass.  */

static unsigned int
simp_cil_early (void)
{
  gimple_stmt_iterator gsi;
  gimple_seq seq;
  gimple stmt;

  /* Create the structures used by the pass */
  eqv_labels = pointer_map_create ();

  group_labels ();

  gsi = gsi_start (gimple_body (current_function_decl));

  while (!gsi_end_p (gsi))
    {
      stmt = gsi_stmt (gsi);

      if (gimple_code (stmt) == GIMPLE_SWITCH)
	{
	  seq = simp_cil_switch (stmt);

	  if (seq != NULL)
	    {
	      annotate_all_with_location (seq, gimple_location (stmt));
	      gsi_insert_seq_before (&gsi, seq, GSI_SAME_STMT);
	      gsi_remove (&gsi, true);
	    }
	  else
	    gsi_next (&gsi);
	}
      else
        gsi_next (&gsi);
    }

  /* Dispose of the structures created by the pass */
  pointer_map_traverse (eqv_labels, eqv_label_dispose, NULL);
  pointer_map_destroy (eqv_labels);

  return 0;
}

/* Gate function */

static bool
simp_cil_early_gate (void)
{
  return true;
}

struct gimple_opt_pass pass_simp_cil_early = {
 {
  GIMPLE_PASS,              /* type */
  "simpcilearly",           /* name */
  simp_cil_early_gate,      /* gate */
  simp_cil_early,           /* execute */
  NULL,                     /* sub */
  NULL,                     /* next */
  0,                        /* static_pass_number */
  TV_SIMP_CIL,              /* tv_id */
  0,                        /* properties_required */
  0,                        /* properties_provided */
  0,                        /* properties_destroyed */
  0,                        /* todo_flags_start */
  TODO_dump_func
  | TODO_ggc_collect        /* todo_flags_finish */
 }
};
