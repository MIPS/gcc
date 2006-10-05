/* Coalesce SSA_NAMES together for the out-of-ssa pass.
   Copyright (C) 2004, 2005, 2006 Free Software Foundation, Inc.
   Contributed by Andrew MacLeod <amacleod@redhat.com>

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
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "flags.h"
#include "diagnostic.h"
#include "bitmap.h"
#include "tree-flow.h"
#include "hashtab.h"
#include "tree-dump.h"
#include "tree-ssa-live.h"
#include "toplev.h"


/* This set of routines implements a coalesce_list.  This is an object which
   is used to track pairs of ssa_names which are desirable to coalesce
   together to avoid copies.  Costs are associated with each pair, and when 
   all desired information has been collected, the object can be used to 
   order the pairs for processing.  */

/* This structure defines a pair entry.  */

typedef struct coalesce_pair
{
  int first_element;
  int second_element;
  int cost;
} * coalesce_pair_p;

/* This structure maintains the list of coalesce pairs.  */

typedef struct coalesce_list_d 
{
  htab_t list;			/* Hash table.  */
  coalesce_pair_p *sorted;	/* List when sorted.  */
  int num_sorted;		/* Number in the sorted list.  */
  bitmap present;		/* True if element number is in the list.  */
} *coalesce_list_p;

#define NO_BEST_COALESCE	-1
#define MUST_COALESCE_COST	INT_MAX


/* Return true if V is present in CL.  */

static inline bool 
version_in_coalesce_list_p (coalesce_list_p cl, int v)
{
  return bitmap_bit_p (cl->present, v);
}


/* Return cost of execution of copy instruction with FREQUENCY
   possibly on CRITICAL edge and in HOT basic block.  */

static inline int
coalesce_cost (int frequency, bool hot, bool critical)
{
  /* Base costs on BB frequencies bounded by 1.  */
  int cost = frequency;

  if (!cost)
    cost = 1;
  if (optimize_size || hot)
    cost = 1;

  /* Inserting copy on critical edge costs more than inserting it elsewhere.  */
  if (critical)
    cost *= 2;
  return cost;
}


/* Return the cost of executing a copy instruction in basic block BB.  */

static inline int 
coalesce_cost_bb (basic_block bb)
{
  return coalesce_cost (bb->frequency, maybe_hot_bb_p (bb), false);
}


/* Return the cost of executing a copy instruction on edge E.  */

static inline int 
coalesce_cost_edge (edge e)
{
  if (e->flags & EDGE_ABNORMAL)
    return MUST_COALESCE_COST;

  return coalesce_cost (EDGE_FREQUENCY (e), 
			maybe_hot_bb_p (e->src), 
			EDGE_CRITICAL_P (e));
}


/* Retrieve the most expensive remaining pair to coalesce from CL.  Returns the 
   2 elements via P1 and P2.  Their calculated cost is returned by the function.
   NO_BEST_COALESCE is returned if the coalesce list is empty.  */

static inline int
pop_best_coalesce (coalesce_list_p cl, int *p1, int *p2)
{
  coalesce_pair_p node;
  int ret;

  if (cl->sorted == NULL)
    return NO_BEST_COALESCE;

  if (cl->num_sorted == 0)
    return NO_BEST_COALESCE;

  node = cl->sorted[--(cl->num_sorted)];
  *p1 = node->first_element;
  *p2 = node->second_element;
  ret = node->cost;
  free (node);

  return ret;
}


#define COALESCE_HASH_FN(R1, R2) ((R2) * ((R2) - 1) / 2 + (R1))

/* Hash function for coalesce list.  Calculate hash for PAIR.   */

static unsigned int 
coalesce_pair_map_hash (const void *pair)
{
  hashval_t a = (hashval_t)(((coalesce_pair_p)pair)->first_element);
  hashval_t b = (hashval_t)(((coalesce_pair_p)pair)->second_element);

  return COALESCE_HASH_FN (a,b);
}


/* Equality function for coalesce list hash table.  Compare PAIR1 and PAIR2,
   returning TRUE if the two pairs are equivilent. */

static int 
coalesce_pair_map_eq (const void *pair1, const void *pair2)
{
  coalesce_pair_p p1 = (coalesce_pair_p) pair1;
  coalesce_pair_p p2 = (coalesce_pair_p) pair2;

  return (p1->first_element == p2->first_element
	  && p1->second_element == p2->second_element);
}


/* Create a new empty coalesce list object and return it.  */

static inline coalesce_list_p 
create_coalesce_list (void)
{
  coalesce_list_p list;
  unsigned size = num_ssa_names * 3;

  if (size < 40) 
    size = 40;

  list = (coalesce_list_p) xmalloc (sizeof (struct coalesce_list_d));
  list->list = htab_create (size, coalesce_pair_map_hash,
  			    coalesce_pair_map_eq, NULL);
  list->sorted = NULL;
  list->num_sorted = 0;
  list->present = BITMAP_ALLOC (NULL);
  return list;
}


/* Delete coalesce list CL.  */

static inline void 
delete_coalesce_list (coalesce_list_p cl)
{
  htab_delete (cl->list);
  if (cl->sorted)
    free (cl->sorted);
  BITMAP_FREE (cl->present);
  gcc_assert (cl->num_sorted == 0);
  free (cl);
}


/* Find a matching coalesce pair object in CL for the pair P1 and P2.  If 
   one isn't found, return NULL if CREATE is false, otherwise create a new 
   coalesce pair object and return it.  */

static coalesce_pair_p
find_coalesce_pair (coalesce_list_p cl, int p1, int p2, bool create)
{
  struct coalesce_pair p, *pair;
  void **slot;
  unsigned int hash;
    
  /* Normalize so that p1 is the smaller value.  */
  if (p2 < p1)
    {
      p.first_element = p2;
      p.second_element = p1;
    }
  else
    {
      p.first_element = p1;
      p.second_element = p2;
    }
  
  
  hash = coalesce_pair_map_hash (&p);
  pair = (struct coalesce_pair *) htab_find_with_hash (cl->list, &p, hash);

  if (create && !pair)
    {
      gcc_assert (cl->sorted == NULL);
      bitmap_set_bit (cl->present, p1);
      bitmap_set_bit (cl->present, p2);
      pair = xmalloc (sizeof (struct coalesce_pair));
      pair->first_element = p.first_element;
      pair->second_element = p.second_element;
      pair->cost = 0;
      slot = htab_find_slot_with_hash (cl->list, pair, hash, INSERT);
      *(struct coalesce_pair **)slot = pair;
    }

  return pair;
}


/* Add a coalesce between P1 and P2 in list CL with a cost of VALUE.  */

static inline void 
add_coalesce (coalesce_list_p cl, int p1, int p2,
	      int value)
{
  coalesce_pair_p node;

  gcc_assert (cl->sorted == NULL);
  if (p1 == p2)
    return;

  node = find_coalesce_pair (cl, p1, p2, true);

  /* Once the value is MUST_COALESCE_COST, leave it that way.  */
  if (node->cost != MUST_COALESCE_COST)
    {
      if (value == MUST_COALESCE_COST)
	node->cost = value;
      else
	node->cost += value;
    }
}


/* Comparison function to allow qsort to sort P1 and P2 in Ascendiong order.  */

static
int compare_pairs (const void *p1, const void *p2)
{
  return (*(coalesce_pair_p *)p1)->cost - (*(coalesce_pair_p *)p2)->cost;
}


/* Return the number of unique coalesce pairs in CL.  */

static inline int
num_coalesce_pairs (coalesce_list_p cl)
{
  return htab_elements (cl->list);
}


/* Iterator over hash table pairs.  */
typedef struct
{
  htab_iterator hti;
} coalesce_pair_iterator;


/* Return first partition pair from list CL, initializing iterator ITER.  */

static inline coalesce_pair_p
first_coalesce_pair (coalesce_list_p cl, coalesce_pair_iterator *iter)
{
  coalesce_pair_p pair;

  pair = (coalesce_pair_p) first_htab_element (&(iter->hti), cl->list);
  return pair;
}


/* Return TRUE if there are no more partitions in for ITER to process.  */

static inline bool
end_coalesce_pair_p (coalesce_pair_iterator *iter)
{
  return end_htab_p (&(iter->hti));
}


/* Return the next parttition pair to be visited by ITER.  */

static inline coalesce_pair_p
next_coalesce_pair (coalesce_pair_iterator *iter)
{
  coalesce_pair_p pair;

  pair = (coalesce_pair_p) next_htab_element (&(iter->hti));
  return pair;
}


/* Iterate over CL using ITER, returning values in PAIR.  */

#define FOR_EACH_PARTITION_PAIR(PAIR, ITER, CL)		\
  for ((PAIR) = first_coalesce_pair ((CL), &(ITER));	\
       !end_coalesce_pair_p (&(ITER));			\
       (PAIR) = next_coalesce_pair (&(ITER)))


/* Prepare CL for removal of preferred pairs.  When finished they are sorted
   in order from most important coalesce to least important.  */

static void
sort_coalesce_list (coalesce_list_p cl)
{
  unsigned x, num;
  coalesce_pair_p p;
  coalesce_pair_iterator ppi;

  gcc_assert (cl->sorted == NULL);

  num = num_coalesce_pairs (cl);
  cl->num_sorted = num;
  if (num == 0)
    return;

  /* Allocate a vector for the pair pointers.  */
  cl->sorted = XNEWVEC (coalesce_pair_p, num);

  /* Populate the vector with pointers to the pairs.  */
  x = 0;
  FOR_EACH_PARTITION_PAIR (p, ppi, cl)
    cl->sorted[x++] = p;
  gcc_assert (x == num);

  /* Already sorted.  */
  if (num == 1)
    return;

  /* If there are only 2, just pick swap them if the order isn't correct.  */
  if (num == 2)
    {
      if (cl->sorted[0]->cost > cl->sorted[1]->cost)
        {
	  p = cl->sorted[0];
	  cl->sorted[0] = cl->sorted[1];
	  cl->sorted[1] = p;
	}
      return;
    }

  /* Only call qsort if there are more than 2 items.  */
  if (num > 2)
      qsort (cl->sorted, num, sizeof (coalesce_pair_p), compare_pairs);
}


/* Send debug info for coalesce list CL to file F.  */

static void 
dump_coalesce_list (FILE *f, coalesce_list_p cl)
{
  coalesce_pair_p node;
  coalesce_pair_iterator ppi;
  int x;
  tree var;

  if (cl->sorted == NULL)
    {
      fprintf (f, "Coalesce List:\n");
      FOR_EACH_PARTITION_PAIR (node, ppi, cl)
        {
	  tree var1 = ssa_name (node->first_element);
	  tree var2 = ssa_name (node->second_element);
	  print_generic_expr (f, var1, TDF_SLIM);
	  fprintf (f, " <-> ");
	  print_generic_expr (f, var2, TDF_SLIM);
	  fprintf (f, "  (%1d), ", node->cost);
	  fprintf (f, "\n");
	}
    }
  else
    {
      fprintf (f, "Sorted Coalesce list:\n");
      for (x = cl->num_sorted - 1 ; x >=0; x--)
        {
	  node = cl->sorted[x];
	  fprintf (f, "(%d) ", node->cost);
	  var = ssa_name (node->first_element);
	  print_generic_expr (f, var, TDF_SLIM);
	  fprintf (f, " <-> ");
	  var = ssa_name (node->second_element);
	  print_generic_expr (f, var, TDF_SLIM);
	  fprintf (f, "\n");
	}
    }
}



/* Shortcut routine to print messages to file F of the form:
   "STR1 EXPR1 STR2 EXPR2 STR3."  */

static inline void
print_exprs (FILE *f, const char *str1, tree expr1, const char *str2,
	     tree expr2, const char *str3)
{
  fprintf (f, "%s", str1);
  print_generic_expr (f, expr1, TDF_SLIM);
  fprintf (f, "%s", str2);
  print_generic_expr (f, expr2, TDF_SLIM);
  fprintf (f, "%s", str3);
}


/* Called if a coalesce across and abnormal edge cannot be performed.  PHI is
   the phi node at fault, I is the argument index at fault.  A message is 
   printed and compilation is then terminated.  */

static inline void
abnormal_corrupt (tree phi, int i)
{
  edge e = PHI_ARG_EDGE (phi, i);
  tree res = PHI_RESULT (phi);
  tree arg = PHI_ARG_DEF (phi, i);

  fprintf (stderr, " Corrupt SSA across abnormal edge BB%d->BB%d\n",
	   e->src->index, e->dest->index);
  fprintf (stderr, "Argument %d (", i);
  print_generic_expr (stderr, arg, TDF_SLIM);
  if (TREE_CODE (arg) != SSA_NAME)
    fprintf (stderr, ") is not an SSA_NAME.\n");
  else
    {
      gcc_assert (SSA_NAME_VAR (res) != SSA_NAME_VAR (arg));
      fprintf (stderr, ") does not have the same base variable as the result ");
      print_generic_stmt (stderr, res, TDF_SLIM);
    }

  internal_error ("SSA corruption");
}


/* This function creates a var_map for the current function as well as creating
   a coalesce list for use later in the out of ssa process.  */

static var_map
create_outofssa_var_map (coalesce_list_p cl)
{
  block_stmt_iterator bsi;
  basic_block bb;
  tree var;
  tree stmt;
  tree first;
  var_map map;
  ssa_op_iter iter;
  int v1, v2, cost;
  unsigned i;

#ifdef ENABLE_CHECKING
  bitmap used_in_real_ops;
  bitmap used_in_virtual_ops;

  used_in_real_ops = BITMAP_ALLOC (NULL);
  used_in_virtual_ops = BITMAP_ALLOC (NULL);
#endif

  map = init_var_map (num_ssa_names + 1);

  FOR_EACH_BB (bb)
    {
      tree phi, arg;

      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	{
	  int i;
	  int ver;
	  tree res;

	  res = PHI_RESULT (phi);
	  ver = SSA_NAME_VERSION (res);
	  register_ssa_partition (map, res);

	  /* Register ssa_names and coalesces between the args and the result 
	     of all PHI.  */
	  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	    {
	      edge e = PHI_ARG_EDGE (phi, i);
	      arg = PHI_ARG_DEF (phi, i);
	      if (TREE_CODE (arg) == SSA_NAME)
		register_ssa_partition (map, arg);
	      if (TREE_CODE (arg) == SSA_NAME 
		  && SSA_NAME_VAR (arg) == SSA_NAME_VAR (res))
	        {
		  int cost = coalesce_cost_edge (e);
		  add_coalesce (cl, ver, SSA_NAME_VERSION (arg), cost);
		}
	      else
	        if (e->flags & EDGE_ABNORMAL)
		  abnormal_corrupt (phi, i);
	    }
	}

      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
	  stmt = bsi_stmt (bsi);

	  /* Register USE and DEF operands in each statement.  */
	  FOR_EACH_SSA_TREE_OPERAND (var, stmt, iter, (SSA_OP_DEF|SSA_OP_USE))
	    register_ssa_partition (map, var);

	  /* Check for copy coalesces.  */
	  switch (TREE_CODE (stmt))
	    {
	    case MODIFY_EXPR:
	      {
		tree op1 = TREE_OPERAND (stmt, 0);
		tree op2 = TREE_OPERAND (stmt, 1);
		if (TREE_CODE (op1) == SSA_NAME 
		    && TREE_CODE (op2) == SSA_NAME
		    && SSA_NAME_VAR (op1) == SSA_NAME_VAR (op2))
		  {
		    v1 = SSA_NAME_VERSION (op1);
		    v2 = SSA_NAME_VERSION (op2);
		    cost = coalesce_cost_bb (bb);
		    add_coalesce (cl, v1, v2, cost);
		  }
	      }
	      break;

	    case ASM_EXPR:
	      {
		unsigned long noutputs, i;
		tree *outputs, link;
		noutputs = list_length (ASM_OUTPUTS (stmt));
		outputs = (tree *) alloca (noutputs * sizeof (tree));
		for (i = 0, link = ASM_OUTPUTS (stmt); link;
		     ++i, link = TREE_CHAIN (link))
		  outputs[i] = TREE_VALUE (link);

		for (link = ASM_INPUTS (stmt); link; link = TREE_CHAIN (link))
		  {
		    const char *constraint
		      = TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));
		    tree input = TREE_VALUE (link);
		    char *end;
		    unsigned long match;

		    if (TREE_CODE (input) != SSA_NAME && !DECL_P (input))
		      continue;

		    match = strtoul (constraint, &end, 10);
		    if (match >= noutputs || end == constraint)
		      continue;

		    if (TREE_CODE (outputs[match]) != SSA_NAME)
		      continue;

		    v1 = SSA_NAME_VERSION (outputs[match]);
		    v2 = SSA_NAME_VERSION (input);
		    cost = coalesce_cost (REG_BR_PROB_BASE, 
					  maybe_hot_bb_p (bb),
					  false);

		    if (SSA_NAME_VAR (outputs[match]) == SSA_NAME_VAR (input))
		      add_coalesce (cl, v1, v2, cost);
		  }
		break;
	      }

	    default:
	      break;
	    }
	    
#ifdef ENABLE_CHECKING
	  /* Mark real uses and defs.  */
	  FOR_EACH_SSA_TREE_OPERAND (var, stmt, iter, (SSA_OP_DEF|SSA_OP_USE))
	    bitmap_set_bit (used_in_real_ops, DECL_UID (SSA_NAME_VAR (var)));

	  /* Validate that virtual ops don't get used in funny ways.  */
	  FOR_EACH_SSA_TREE_OPERAND (var, stmt, iter, 
				     SSA_OP_VIRTUAL_USES | SSA_OP_VMUSTDEF)
	    {
	      bitmap_set_bit (used_in_virtual_ops, 
			      DECL_UID (SSA_NAME_VAR (var)));
	    }

#endif /* ENABLE_CHECKING */
	}
    }

  /* Now process result decls and live on entry variables.  */
  first = NULL_TREE;
  for (i = 1; i < num_ssa_names; i++)
    {
      var = map->partition_to_var[i];
      if (var != NULL_TREE)
        {
	  /* Add coalesces between all the result decls.  */
	  if (TREE_CODE (SSA_NAME_VAR (var)) == RESULT_DECL)
	    {
	      if (first == NULL_TREE)
		first = var;
	      else
		{
		  gcc_assert (SSA_NAME_VAR (var) == SSA_NAME_VAR (first));
		  v1 = SSA_NAME_VERSION (first);
		  v2 = SSA_NAME_VERSION (var);
		  cost = coalesce_cost_bb (EXIT_BLOCK_PTR);
		  add_coalesce (cl, v1, v2, cost);
		}
	    }
	  /* Mark any default_def variables as being in the coalesce list
	     since they will have to be coalesced with the base variable.  If
	     not marked as present, they won't be in the coalesce view. */
	  if (default_def (SSA_NAME_VAR (var)) == var)
	    bitmap_set_bit (cl->present, SSA_NAME_VERSION (var));
	}
    }

#if defined ENABLE_CHECKING
  {
    unsigned i;
    bitmap both = BITMAP_ALLOC (NULL);
    bitmap_and (both, used_in_real_ops, used_in_virtual_ops);
    if (!bitmap_empty_p (both))
      {
	bitmap_iterator bi;

	EXECUTE_IF_SET_IN_BITMAP (both, 0, i, bi)
	  fprintf (stderr, "Variable %s used in real and virtual operands\n",
		   get_name (referenced_var (i)));
	internal_error ("SSA corruption");
      }

    BITMAP_FREE (used_in_real_ops);
    BITMAP_FREE (used_in_virtual_ops);
    BITMAP_FREE (both);
  }
#endif

  return map;
}


/* This prepresents a conflict graph.  Implemented as an array of bitmaps.  
   A full matrix isused for conflicts rather than just upper triangular form.
   this make sit much simpler and faster to perform conflict merges.  */

typedef struct ssa_conflicts_d
{
  unsigned size;
  bitmap *conflicts;
} * ssa_conflicts_p;


/* Return a empty new conflict graph for SIZE elements.  */

static inline ssa_conflicts_p
ssa_conflicts_new (unsigned size)
{
  ssa_conflicts_p ptr;

  ptr = XNEW (struct ssa_conflicts_d);
  ptr->conflicts = XCNEWVEC (bitmap, size);
  ptr->size = size;
  return ptr;
}


/* Free storage for conflict graph PTR.  */

static inline void
ssa_conflicts_delete (ssa_conflicts_p ptr)
{
  unsigned x;
  for (x = 0; x < ptr->size; x++)
    if (ptr->conflicts[x])
      BITMAP_FREE (ptr->conflicts[x]);

  free (ptr->conflicts);
  free (ptr);
}


/* Test if elements X and Y conflict in graph PTR.  */

static inline bool
ssa_conflicts_test_p (ssa_conflicts_p ptr, unsigned x, unsigned y)
{
  bitmap b;

#ifdef ENABLE_CHECKING
  gcc_assert (x < ptr->size);
  gcc_assert (y < ptr->size);
  gcc_assert (x != y);
#endif

  b = ptr->conflicts[x];
  if (b)
    return bitmap_bit_p (b, y);
  else
    return false;
}


/* Add a conflict with Y to the bitmap for X in graph PTR.  */

static inline void
ssa_conflicts_add_one (ssa_conflicts_p ptr, unsigned x, unsigned y)
{
  /* If there are no conflicts yet, allocate the bitmap and set bit.  */
  if (!ptr->conflicts[x])
    ptr->conflicts[x] = BITMAP_ALLOC (NULL);
  bitmap_set_bit (ptr->conflicts[x], y);
}


/* Add conflicts between X and Y in graph PTR.  */

static inline void
ssa_conflicts_add (ssa_conflicts_p ptr, unsigned x, unsigned y)
{
#ifdef ENABLE_CHECKING
  gcc_assert (x < ptr->size);
  gcc_assert (y < ptr->size);
  gcc_assert (x != y);
#endif
  ssa_conflicts_add_one (ptr, x, y);
  ssa_conflicts_add_one (ptr, y, x);
}


/* Merge all Y's conflict into X in graph PTR.  */

static inline void
ssa_conflicts_merge (ssa_conflicts_p ptr, unsigned x, unsigned y)
{
  unsigned z;
  bitmap_iterator bi;

  gcc_assert (x != y);
  if (!(ptr->conflicts[y]))
    return;

  /* Add a conflict for every one Y has.  */
  EXECUTE_IF_SET_IN_BITMAP (ptr->conflicts[y], 0, z, bi)
    ssa_conflicts_add (ptr, z, x);

  /* Won't be referencing this bitmap again.  */
  BITMAP_FREE (ptr->conflicts[y]);
}


/* This structure is used to efficiently record the current status of live 
   SSA_NAMES when building a conflict graph.  
   BASE_LIVE is an array of bitmaps using the basevar table as an index, and 
   is used to track what partitions with each base variable are live.  This 
   makes it easy to add conflicts between just live partitions with the same 
   base variable.  */

typedef struct live_track_d
{
  bitmap *base_live;
  var_map map;
} * live_track_p;


/* This routine will create a new live track structure based on the partitions
   in MAP.  */

static live_track_p
new_live_track (var_map map)
{
  live_track_p ptr;
  int lim, x;

  /* Make sure there is a partition view in place.  */
  gcc_assert (map->partition_to_base_index != NULL);

  ptr = (live_track_p) xmalloc (sizeof (struct live_track_d));
  ptr->map = map;
  lim = num_basevars (map);
  ptr->base_live = (bitmap *) xmalloc(sizeof (bitmap *) * lim);
  for (x = 0; x < lim; x++)
    ptr->base_live[x] = BITMAP_ALLOC (NULL);
  return ptr;
}


/* This routine will free the memory associated with PTR.  */

static void
delete_live_track (live_track_p ptr)
{
  int x, lim;

  lim = num_basevars (ptr->map);
  for (x = 0; x < lim; x++)
    BITMAP_FREE (ptr->base_live[x]);
  free (ptr->base_live);
  free (ptr);
}


/* Clear the live bit for VAR in PTR.  */

static inline void
live_track_clear_var (live_track_p ptr, tree var)
{
  int p, root;

  p = var_to_partition (ptr->map, var);
  if (p != NO_PARTITION)
    {
      root = basevar_index (ptr->map, p);
      bitmap_clear_bit (ptr->base_live[root], p);
    }
}


/* Return TRUE if VAR is live in PTR.  */

static inline bool
live_track_live_p (live_track_p ptr, tree var)
{
  int p, root;

  p = var_to_partition (ptr->map, var);
  if (p != NO_PARTITION)
    {
      root = basevar_index (ptr->map, p);
      return bitmap_bit_p (ptr->base_live[root], p);
    }
  return false;
}


/* This routine will add USE to PTR.  USE will be marked as live in both the 
   ssa live map and the live bitmap for the root of USE.  */

static inline void
live_track_process_use (live_track_p ptr, tree use)
{
  int p, root;

  p = var_to_partition (ptr->map, use);
  if (p == NO_PARTITION)
    return;

  /* Mark as live in the appropriate live list.  */
  root = basevar_index (ptr->map, p);
  bitmap_set_bit (ptr->base_live[root], p);
}


/* This routine will process a DEF in PTR.  DEF will be removed from the live
   lists, and if there are any other live partitions with the same base 
   variable, conflicts will be added to GRAPH.  */

static inline void
live_track_process_def (live_track_p ptr, tree def, ssa_conflicts_p graph)
{
  int p, root;
  bitmap b;
  unsigned x;
  bitmap_iterator bi;

  p = var_to_partition (ptr->map, def);
  if (p == NO_PARTITION)
    return;

  root = basevar_index (ptr->map, p);
  /* Clear the liveness bit.  */
  b = ptr->base_live[root];
  bitmap_clear_bit (b, p);

  /* If the bitmap isn't empty now, conflicts need to be added.  */
  if (!bitmap_empty_p (b))
    {
      EXECUTE_IF_SET_IN_BITMAP (b, 0, x, bi)
        ssa_conflicts_add (graph, p, x);
    }
}


/* Initialize PTR with the partitions set in INIT.  */

static inline void
live_track_init (live_track_p ptr, bitmap init)
{
  int root;
  unsigned p;
  bitmap_iterator bi;

  EXECUTE_IF_SET_IN_BITMAP (init, 0, p, bi)
    {
      /* Mark as live in the appropriate live list.  */
      root = basevar_index (ptr->map, p);
      bitmap_set_bit (ptr->base_live[root], p);
    }
}


/* This routine will clear all live partitions in PTR.   */

static inline void
live_track_clear_conflicts (live_track_p ptr)
{
  int root, lim;
  bitmap b;

  lim = num_basevars (ptr->map);
  for (root = 0; root < lim; root++)
    {
      b = ptr->base_live[root];
      bitmap_clear (b);
    }
}


/* Build a conflict graph based on LIVEINFO.  Any partitions which are in the
   partition view of the var_map liveinfo is based on get entires in the 
   conflict graph.  Only conflicts between ssa_name partitions with the same 
   base variableare added.  */

static ssa_conflicts_p
build_ssa_conflict_graph (tree_live_info_p liveinfo)
{
  ssa_conflicts_p graph;
  var_map map;
  basic_block bb;
  ssa_op_iter iter;
  live_track_p live;

  map = live_var_map (liveinfo);
  graph = ssa_conflicts_new (num_var_partitions (map));

  live = new_live_track (map);

  FOR_EACH_BB (bb)
    {
      block_stmt_iterator bsi;
      tree phi;

      /* Start with live on exit temporaries.  */
      live_track_init (live, live_on_exit (liveinfo, bb));

      for (bsi = bsi_last (bb); !bsi_end_p (bsi); bsi_prev (&bsi))
        {
	  tree var;
	  tree stmt = bsi_stmt (bsi);

	  /* A copy between 2 partitions does not introduce an interference 
	     by itself.  If they did, you would never be able to coalesce 
	     two things which are copied.  If the two variables really do 
	     conflict, they will conflict elsewhere in the program.  
	     
	     This is handled by simply removing the SRC of the copy from the 
	     live list, and processing the stmt normally.  */
	  if (TREE_CODE (stmt) == MODIFY_EXPR)
	    {
	      tree lhs = TREE_OPERAND (stmt, 0);
	      tree rhs = TREE_OPERAND (stmt, 1);
	      if (TREE_CODE (lhs) == SSA_NAME && TREE_CODE (rhs) == SSA_NAME)
		live_track_clear_var (live, rhs);
	    }

	  FOR_EACH_SSA_TREE_OPERAND (var, stmt, iter, SSA_OP_DEF)
	    live_track_process_def (live, var, graph);

	  FOR_EACH_SSA_TREE_OPERAND (var, stmt, iter, SSA_OP_USE)
	    live_track_process_use (live, var);
	}

      /* If result of a PHI is unused, looping over the statements will not 
	 record any conflicts since the def was never live.  Since the PHI node
	 is going to be translated out of SSA form, it will insert a copy.
	 There must be a conflict recorded between the result of the PHI and 
	 any variables that are live.  Otherwise the out-of-ssa translation 
	 may create incorrect code.  */
      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	{
	  tree result = PHI_RESULT (phi);
	  if (live_track_live_p (live, result))
	    live_track_process_def (live, result, graph);
	}

     live_track_clear_conflicts (live);
    }

  delete_live_track (live);
  return graph;
}


/* Print a failure to coalesce a MUST_COALESCE pair X and Y.  */

static inline void
fail_abnormal_edge_coalesce (int x, int y)
{
  fprintf (stderr, "\nUnable to coalesce ssa_names %d  and %d ",x, y);
  fprintf (stderr, " which are marked as MUST COALESCE.\n");
  print_generic_expr (stderr, ssa_name (x), TDF_SLIM);
  fprintf (stderr, " and  ");
  print_generic_stmt (stderr, ssa_name (y), TDF_SLIM);

  internal_error ("SSA corruption");
}


/* Attempt to Coalesce partitions in MAP which occur in the list CL using 
   GRAPH.  Debug output is sent to DEBUG if it is non-NULL.  */

static void
coalesce_partitions (var_map map, ssa_conflicts_p graph, coalesce_list_p cl, 
		     FILE *debug)
{
  int x, y, z;
  int p1,p2;
  tree var1, var2;
  int cost;

  while ((cost = pop_best_coalesce (cl, &x, &y)) != NO_BEST_COALESCE)
    {
      var1 = ssa_name (x);
      var2 = ssa_name (y);
      p1 = var_to_partition (map, var1);
      p2 = var_to_partition (map, var2);

      if (debug)
	{
	  fprintf (debug, "Coalesce list: (%d)", x);
	  print_generic_expr (debug, partition_to_var (map, p1), TDF_SLIM);
	  fprintf (debug, " & (%d)", y);
	  print_generic_expr (debug, partition_to_var (map, p2), TDF_SLIM);
	}

      if (p1 == p2) 
        {
	  if (debug)
	    fprintf (debug, ": Already Coalesced.\n");
	  continue;
	}

      /* Assert the coalesces have the same base variable.  */
      gcc_assert (SSA_NAME_VAR (var1) == SSA_NAME_VAR (var2));

      if (debug)
	fprintf (debug, " [map: %d, %d] ", p1, p2);

      if (!ssa_conflicts_test_p (graph, p1, p2))
	{
	  var1 = partition_to_var (map, p1);
	  var2 = partition_to_var (map, p2);
	  z = var_union (map, var1, var2);
	  if (z == NO_PARTITION)
	    {
	      if (debug)
		fprintf (debug, ": Unable to perform partition union.\n");
	      if (cost == MUST_COALESCE_COST)
		fail_abnormal_edge_coalesce (x, y);
	      continue;
	    }

	  /* z is the new combined partition.  Remove the other partition from 
	     the list, and merge the conflicts.  */
	  if (z == p1)
	    ssa_conflicts_merge (graph, p1, p2);
	  else
	    ssa_conflicts_merge (graph, p2, p1);

	  if (debug)
	    fprintf (debug, ": Success -> %d\n", z);
	}
      else
        {
	  if (debug)
	    fprintf (debug, ": Fail due to conflict\n");
	  if (cost == MUST_COALESCE_COST)
	    fail_abnormal_edge_coalesce (x, y);
	}
    }
}


/* Reduce the number of copies by coalescing variables in the function.  Return
   a partition map with the resulting coalesces.  */

extern var_map
coalesce_ssa_name (void)
{
  unsigned num, x;
  tree_live_info_p liveinfo;
  ssa_conflicts_p graph;
  coalesce_list_p cl;
  var_map map;

  cl = create_coalesce_list ();
  map = create_outofssa_var_map (cl);

  /* Don't calculate live ranges for variables not in the coalesce list.  */
  partition_view_bitmap (map, cl->present, true);

  if (num_var_partitions (map) <= 1)
    return map;

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_var_map (dump_file, map);

  liveinfo = calculate_live_ranges (map);

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_live_info (dump_file, liveinfo, LIVEDUMP_ENTRY);

  /* Build a conflict graph.  */
  graph = build_ssa_conflict_graph (liveinfo);
  delete_tree_live_info (liveinfo);

  sort_coalesce_list (cl);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nAfter sorting:\n");
      dump_coalesce_list (dump_file, cl);
    }


  /* First, coalesce all live on entry variables to their base variable. 
     This will ensure the first use is coming from the correct location.  */

  num = num_var_partitions (map);
  for (x = 0 ; x < num; x++)
    {
      tree var = partition_to_var (map, x);
      tree root;

      if (TREE_CODE (var) != SSA_NAME)
	continue;

      root = SSA_NAME_VAR (var);
      if (default_def (root) == var)
        {
	  /* This root variable should have not already been assigned
	     to another partition which is not coalesced with this one.  */
	  gcc_assert (!var_ann (root)->out_of_ssa_tag);

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      print_exprs (dump_file, "Must coalesce ", var,
			   " with the root variable ", root, ".\n");
	    }
	  change_partition_var (map, root, x);
	}
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_var_map (dump_file, map);

  /* Now coalesce everything in the list.  */
  coalesce_partitions (map, graph, cl, 
		       ((dump_flags & TDF_DETAILS) ? dump_file
						   : NULL));

  delete_coalesce_list (cl);
  ssa_conflicts_delete (graph);

  return map;
}

