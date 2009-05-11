/* Simplify non-SSA GIMPLE trees before control flow-graph construction and
   optimization passes.

   Copyright (C) 2007, 2008 Free Software Foundation, Inc.

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
Erven Rohou             <erven.rohou@st.com>
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "timevar.h"
#include "tree.h"
#include "tree-gimple.h"
#include "tree-iterator.h"
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
static void group_labels (tree);
static bool eqv_label_dispose (const void *, void **, void *data);

/* Switch-conversion functions */
static void merge_cases_into_ranges (tree);
static void case_to_cond_expr (tree, tree, tree *);
static void case_range_to_cond_expr (tree, tree, tree *);
static void cases_to_switch (tree, unsigned int, unsigned int, tree *);
static tree simp_cil_switch (tree);

/* Misc functionality */
static void set_statement_list_location (tree, location_t);

/* Top-level functionality */
static tree simp_cil_stmt (tree);
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
 * Misc functions                                                             *
 ******************************************************************************/

/* Set the location of the statements in the statement list pointed by LIST to
   the location passed in LOCUS */

static void
set_statement_list_location (tree list, location_t locus)
{
  tree_stmt_iterator tsi = tsi_start (list);

  while (!tsi_end_p (tsi))
    {
      SET_EXPR_LOCATION (tsi_stmt (tsi), locus);
      tsi_next (&tsi);
    }
}

/******************************************************************************
 * Label manipulation functions                                               *
 ******************************************************************************/

/* Returns true if two labels are equivalent. */

static bool
is_eqv_label (tree label1, tree label2)
{
  struct eqv_label_entry_t *t1, *t2;

  t1 = *pointer_map_contains (eqv_labels, label1);
  t2 = *pointer_map_contains (eqv_labels, label2);

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
group_labels (tree func)
{
  tree_stmt_iterator tsi;
  tree t = NULL_TREE;
  unsigned int value = 0;
  bool prev_label = false;
  struct eqv_label_entry_t *entry;

  tsi = tsi_start (func);

  while (!tsi_end_p (tsi))
    {
      t = tsi_stmt (tsi);
      tsi_next (&tsi);

      if (TREE_CODE (t) == LABEL_EXPR)
	{
	  /* Insert the label in the hash-table */
	  entry = XCNEW (struct eqv_label_entry_t);
	  entry->label = LABEL_EXPR_LABEL (t);
	  entry->val = value;
	  *pointer_map_insert (eqv_labels, LABEL_EXPR_LABEL (t)) = entry;
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
merge_cases_into_ranges (tree switch_stmt)
{
  tree labels = SWITCH_LABELS (switch_stmt);
  unsigned int old_size = TREE_VEC_LENGTH (labels);
  unsigned int i, j, new_size = old_size;
  tree default_case = TREE_VEC_ELT (labels, old_size - 1);
  tree default_label;

  /* The default label is always the last case in a switch
     statement after gimplification.  */
  default_label = CASE_LABEL (default_case);

  /* Look for possible opportunities to merge cases.
     Ignore the last element of the label vector because it
     must be the default case.  */
  i = 0;
  while (i < old_size - 1)
    {
      tree base_case, base_label, base_high;
      base_case = TREE_VEC_ELT (labels, i);

      gcc_assert (base_case);
      base_label = CASE_LABEL (base_case);

      /* Discard cases that have the same destination as the
	 default case.  */
      if (is_eqv_label (base_label, default_label))
	{
	  TREE_VEC_ELT (labels, i) = NULL_TREE;
	  i++;
	  new_size--;
	  continue;
	}

      base_high = CASE_HIGH (base_case) ?
	CASE_HIGH (base_case) : CASE_LOW (base_case);
      i++;
      /* Try to merge case labels.  Break out when we reach the end
	 of the label vector or when we cannot merge the next case
	 label with the current one.  */
      while (i < old_size - 1)
	{
	  tree merge_case = TREE_VEC_ELT (labels, i);
          tree merge_label = CASE_LABEL (merge_case);
	  tree t = int_const_binop (PLUS_EXPR, base_high,
				    integer_one_node, 1);

	  /* Merge the cases if they jump to the same place,
	     and their ranges are consecutive.  */
	  if (is_eqv_label (merge_label, base_label)
	      && tree_int_cst_equal (CASE_LOW (merge_case), t))
	    {
	      base_high = CASE_HIGH (merge_case) ?
		CASE_HIGH (merge_case) : CASE_LOW (merge_case);
	      CASE_HIGH (base_case) = base_high;
	      TREE_VEC_ELT (labels, i) = NULL_TREE;
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
      while (!TREE_VEC_ELT (labels, j))
	j++;

      TREE_VEC_ELT (labels, i) = TREE_VEC_ELT (labels, j++);
    }

  TREE_VEC_LENGTH (labels) = new_size;
}

/* Turn a single isolated case into a COND_EXPR */

static void
case_to_cond_expr (tree switch_stmt, tree single_case, tree *list)
{
  tree cmp_stmt, label;
  tree label_decl = create_artificial_label ();

  /* Build the COND_EXPR */
  cmp_stmt = build3 (COND_EXPR, void_type_node,
		     build2 (EQ_EXPR, boolean_type_node,
			     SWITCH_COND (switch_stmt),
			     CASE_LOW (single_case)),
		     build1 (GOTO_EXPR, void_type_node,
			     CASE_LABEL (single_case)),
		     build1 (GOTO_EXPR, void_type_node, label_decl));

  /* Append the COND_EXPR to the list */
  append_to_statement_list (cmp_stmt, list);

  /* Create the label to the next statement and append it to the list */
  label = build1 (LABEL_EXPR, void_type_node, label_decl);
  append_to_statement_list (label, list);
}

/* Turn a case range into a couple of COND_EXPRs */

static void
case_range_to_cond_expr (tree switch_stmt, tree case_range, tree *list)
{
  tree label_decl1 = create_artificial_label ();
  tree label_decl2 = create_artificial_label ();
  tree cmp1_stmt, cmp2_stmt, label;

  gcc_assert (CASE_HIGH (case_range) != NULL_TREE
              && CASE_LOW (case_range) != NULL_TREE);

  /* Build the 1st COND_EXPR */
  cmp1_stmt = build3 (COND_EXPR, void_type_node,
		      build2 (GE_EXPR, boolean_type_node,
			      SWITCH_COND (switch_stmt),
			      CASE_LOW (case_range)),
		      build1 (GOTO_EXPR, void_type_node,
			      label_decl1),
		      build1 (GOTO_EXPR, void_type_node,
			      label_decl2));

  /* Append the 1st COND_EXPR to the list */
  append_to_statement_list (cmp1_stmt, list);

  /* Create a new label and append it to the list */
  label = build1 (LABEL_EXPR, void_type_node, label_decl1);
  append_to_statement_list (label, list);

  /* Build the 2nd COND_EXPR */
  cmp2_stmt = build3 (COND_EXPR, void_type_node,
		      build2 (LE_EXPR, boolean_type_node,
			      SWITCH_COND (switch_stmt),
			      CASE_HIGH (case_range)),
		      build1 (GOTO_EXPR, void_type_node,
			      CASE_LABEL (case_range)),
		      build1 (GOTO_EXPR, void_type_node,
			      label_decl2));

  /* Append the 2nd COND_EXPR to the list */
  append_to_statement_list (cmp2_stmt, list);

  /* Create the label to the next statement and append it */
  label = build1 (LABEL_EXPR, void_type_node, label_decl2);
  append_to_statement_list (label, list);
}

/* Turn the cases between <start> and <end> (included) into a new switch
   statement */

static void
cases_to_switch (tree switch_stmt, unsigned int start, unsigned int end,
                 tree *list)
{
  tree label_decl;
  tree labels = SWITCH_LABELS (switch_stmt);
  tree stmt, label, vec, deft, elt;
  unsigned int i, len;
  tree low, high, range, limit;
  tree type;

  type = TREE_TYPE (CASE_LOW (TREE_VEC_ELT (labels, start)));
  limit = build_int_cst (type, 8192);

  while (start <= end)
    {
      label_decl = create_artificial_label ();
      elt = TREE_VEC_ELT (labels, start);
      low = CASE_LOW (elt);
      len = 1;

      while (start + len <= end)
        {
	  elt = TREE_VEC_ELT (labels, start + len);
          high = CASE_HIGH (elt) ? CASE_HIGH (elt) : CASE_LOW (elt);
	  range = size_binop (MINUS_EXPR, high, low);

	  if (tree_int_cst_lt (range, limit))
	    len++;
	  else
	    break;
        }

      /* Build the switch cases */
      vec = make_tree_vec (len + 1);

      for (i = 0; i < len; i++)
	TREE_VEC_ELT (vec, i) = TREE_VEC_ELT (labels, start + i);

      /* Create the new default label */
      deft = build3 (CASE_LABEL_EXPR, void_type_node, NULL_TREE, NULL_TREE,
		     label_decl);
      TREE_VEC_ELT (vec, len) = deft;

      /* Build the switch statement */
      stmt = build3 (SWITCH_EXPR, TREE_TYPE (switch_stmt),
		     SWITCH_COND (switch_stmt), NULL, vec);

      /* Append the SWITCH to the list */
      append_to_statement_list (stmt, list);

      /* Create a new label and append it to the list */
      label = build1 (LABEL_EXPR, void_type_node, label_decl);
      append_to_statement_list (label, list);

      /* Move ahead in the labels vector */
      start += len;
    }
}

/* Break a switch statement pointed by SWITCH_STMT into multiple expressions in
   order to produce better/smaller code in the CIL back-end. Large case ranges
   or isolated cases are turned into COND_EXPRs, large 'holes' inside the switch
   are removed replacing a single switch with two or more distinct switches. The
   list holding the replacement is returned. If no replacement is needed
   NULL_TREE is returned instead */

static tree
simp_cil_switch (tree switch_stmt)
{
  tree curr, next;
  tree labels = SWITCH_LABELS (switch_stmt);
  tree list = NULL_TREE;
  tree range_size, hole_size, curr_high, next_low;
  tree type;
  unsigned int i = 0;
  unsigned int base_idx = 0;

  merge_cases_into_ranges (switch_stmt);

  if (TREE_VEC_LENGTH (labels) == 1)
    {
      /* Only the default statement, nothing to do as this will be optimized
         out later by the subsequent passes */
      return NULL_TREE;
    }

  type = TREE_TYPE (CASE_LOW (TREE_VEC_ELT (labels, 0)));
  range_size = build_int_cst (type, SIMP_SWITCH_RANGE_SIZE);
  hole_size = build_int_cst (type, SIMP_SWITCH_HOLE_SIZE);

  while (true)
    {
      curr = TREE_VEC_ELT (labels, i);
      next = TREE_VEC_ELT (labels, i + 1);

      if (CASE_LOW (next) == NULL_TREE)
	{
	  /* This is the last case, the next one is the default label, emit the
	     previous cases if needed. If base_idx != i and base_idx is 0 then
	     the original switch has been left untouched and it is not
	     replaced */
	  if (base_idx != i)
	    {
	      if (base_idx == 0)
		return NULL_TREE;
	      else
		cases_to_switch (switch_stmt, base_idx, i, &list);
	    }
	  else if (CASE_HIGH (curr) != NULL_TREE)
	    case_range_to_cond_expr (switch_stmt, curr, &list);
	  else
	    case_to_cond_expr (switch_stmt, curr, &list);

	  /* Add a label which jumps to the default label. */
	  append_to_statement_list (build1 (GOTO_EXPR, void_type_node,
				    CASE_LABEL (next)),
				    &list);
	  break;
	}

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
	      gcc_assert (i == base_idx);
	      case_range_to_cond_expr (switch_stmt, curr, &list);

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
	      if (base_idx != i)
		cases_to_switch (switch_stmt, base_idx, i, &list);
	      else if (CASE_HIGH (curr) != NULL_TREE)
		case_range_to_cond_expr (switch_stmt, curr, &list);
	      else
		case_to_cond_expr (switch_stmt, curr, &list);

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
	    if (base_idx != i)
	      cases_to_switch (switch_stmt, base_idx, i, &list);
	    else if (CASE_HIGH (curr) != NULL_TREE)
	      case_range_to_cond_expr (switch_stmt, curr, &list);
	    else
	      case_to_cond_expr (switch_stmt, curr, &list);

	    i++;
	    base_idx = i;
	    continue;
	  }

      i++;
    }

  return list;
}

/******************************************************************************
 * Top-level functionality                                                    *
 ******************************************************************************/

/* Simplify a GIMPLE statement. */

static tree
simp_cil_stmt (tree stmt)
{
  tree list = NULL_TREE; /* Replacement list for the current statement */

  switch (TREE_CODE (stmt))
    {
    case SWITCH_EXPR:
      list = simp_cil_switch (stmt);
      break;

    default:
      break;
    }

    return list;
}

/* Main function of this pass. */

static unsigned int
simp_cil_early (void)
{
  tree_stmt_iterator tsi;

  /* Create the structures used by the pass */
  eqv_labels = pointer_map_create ();

  group_labels (DECL_SAVED_TREE (current_function_decl));

  tsi = tsi_start (DECL_SAVED_TREE (current_function_decl));

  while (!tsi_end_p (tsi))
    {
      tree list = simp_cil_stmt (tsi_stmt (tsi));

      if (list != NULL_TREE)
	{
	  location_t locus = EXPR_LOCATION (tsi_stmt (tsi));

	  set_statement_list_location (list, locus);
	  tsi_link_before (&tsi, list, TSI_SAME_STMT);
	  tsi_delink (&tsi);
	}
      else
	tsi_next (&tsi);
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

struct tree_opt_pass pass_simp_cil_early = {
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
  | TODO_ggc_collect,       /* todo_flags_finish */
  0                         /* letter */
};
