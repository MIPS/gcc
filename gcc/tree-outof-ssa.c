/* Convert a program in SSA form into Normal form.
   Copyright (C) 2004, 2005 Free Software Foundation, Inc.
   Contributed by Andrew Macleod <amacleod@redhat.com>

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
#include "rtl.h"
#include "tm_p.h"
#include "ggc.h"
#include "langhooks.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "expr.h"
#include "function.h"
#include "diagnostic.h"
#include "bitmap.h"
#include "tree-flow.h"
#include "tree-gimple.h"
#include "tree-inline.h"
#include "varray.h"
#include "timevar.h"
#include "hashtab.h"
#include "tree-dump.h"
#include "tree-ssa-live.h"
#include "tree-pass.h"
#include "toplev.h"
#include "vecprim.h"


/* Used to hold all the components required to do SSA PHI elimination.
   The node and pred/succ list is a simple linear list of nodes and
   edges represented as pairs of nodes.

   The predecessor and successor list:  Nodes are entered in pairs, where
   [0] ->PRED, [1]->SUCC.  All the even indexes in the array represent 
   predecessors, all the odd elements are successors. 
   
   Rationale:
   When implemented as bitmaps, very large programs SSA->Normal times were 
   being dominated by clearing the interference graph.

   Typically this list of edges is extremely small since it only includes 
   PHI results and uses from a single edge which have not coalesced with 
   each other.  This means that no virtual PHI nodes are included, and
   empirical evidence suggests that the number of edges rarely exceed
   3, and in a bootstrap of GCC, the maximum size encountered was 7.
   This also limits the number of possible nodes that are involved to
   rarely more than 6, and in the bootstrap of gcc, the maximum number
   of nodes encountered was 12.  */
 
typedef struct _elim_graph {
  /* Size of the elimination vectors.  */
  int size;

  /* List of nodes in the elimination graph.  */
  VEC(tree,heap) *nodes;

  /*  The predecessor and successor edge list.  */
  VEC(int,heap) *edge_list;

  /* Visited vector.  */
  sbitmap visited;

  /* Stack for visited nodes.  */
  VEC(int,heap) *stack;
  
  /* The variable partition map.  */
  var_map map;

  /* Edge being eliminated by this graph.  */
  edge e;

  /* List of constant copies to emit.  These are pushed on in pairs.  */
  VEC(tree,heap) *const_copies;
} *elim_graph;


/* Create a temporary variable based on the type of variable T.  Use T's name
   as the prefix.  */

static tree
create_temp (tree t)
{
  tree tmp;
  const char *name = NULL;
  tree type;

  if (TREE_CODE (t) == SSA_NAME)
    t = SSA_NAME_VAR (t);

  gcc_assert (TREE_CODE (t) == VAR_DECL || TREE_CODE (t) == PARM_DECL);

  type = TREE_TYPE (t);
  tmp = DECL_NAME (t);
  if (tmp)
    name = IDENTIFIER_POINTER (tmp);

  if (name == NULL)
    name = "temp";
  tmp = create_tmp_var (type, name);

  if (DECL_DEBUG_EXPR_IS_FROM (t) && DECL_DEBUG_EXPR (t))
    {
      SET_DECL_DEBUG_EXPR (tmp, DECL_DEBUG_EXPR (t));  
      DECL_DEBUG_EXPR_IS_FROM (tmp) = 1;
    }
  else if (!DECL_IGNORED_P (t))
    {
      SET_DECL_DEBUG_EXPR (tmp, t);
      DECL_DEBUG_EXPR_IS_FROM (tmp) = 1;
    }
  DECL_ARTIFICIAL (tmp) = DECL_ARTIFICIAL (t);
  DECL_IGNORED_P (tmp) = DECL_IGNORED_P (t);
  add_referenced_var (tmp);

  /* add_referenced_var will create the annotation and set up some
     of the flags in the annotation.  However, some flags we need to
     inherit from our original variable.  */
  var_ann (tmp)->symbol_mem_tag = var_ann (t)->symbol_mem_tag;
  if (is_call_clobbered (t))
    mark_call_clobbered (tmp, var_ann (t)->escape_mask);

  return tmp;
}


/* This helper function fill insert a copy from a constant or variable SRC to 
   variable DEST on edge E.  */

static void
insert_copy_on_edge (edge e, tree dest, tree src)
{
  tree copy;

  copy = build2 (MODIFY_EXPR, TREE_TYPE (dest), dest, src);
  set_is_used (dest);

  if (TREE_CODE (src) == ADDR_EXPR)
    src = TREE_OPERAND (src, 0);
  if (TREE_CODE (src) == VAR_DECL || TREE_CODE (src) == PARM_DECL)
    set_is_used (src);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file,
	       "Inserting a copy on edge BB%d->BB%d :",
	       e->src->index,
	       e->dest->index);
      print_generic_expr (dump_file, copy, dump_flags);
      fprintf (dump_file, "\n");
    }

  bsi_insert_on_edge (e, copy);
}


/* Create an elimination graph with SIZE nodes and associated data
   structures.  */

static elim_graph
new_elim_graph (int size)
{
  elim_graph g = (elim_graph) xmalloc (sizeof (struct _elim_graph));

  g->nodes = VEC_alloc (tree, heap, 30);
  g->const_copies = VEC_alloc (tree, heap, 20);
  g->edge_list = VEC_alloc (int, heap, 20);
  g->stack = VEC_alloc (int, heap, 30);
  
  g->visited = sbitmap_alloc (size);

  return g;
}


/* Empty elimination graph G.  */

static inline void
clear_elim_graph (elim_graph g)
{
  VEC_truncate (tree, g->nodes, 0);
  VEC_truncate (int, g->edge_list, 0);
}


/* Delete elimination graph G.  */

static inline void
delete_elim_graph (elim_graph g)
{
  sbitmap_free (g->visited);
  VEC_free (int, heap, g->stack);
  VEC_free (int, heap, g->edge_list);
  VEC_free (tree, heap, g->const_copies);
  VEC_free (tree, heap, g->nodes);
  free (g);
}


/* Return the number of nodes in graph G.  */

static inline int
elim_graph_size (elim_graph g)
{
  return VEC_length (tree, g->nodes);
}


/* Add NODE to graph G, if it doesn't exist already.  */

static inline void 
elim_graph_add_node (elim_graph g, tree node)
{
  int x;
  tree t;

  for (x = 0; VEC_iterate (tree, g->nodes, x, t); x++)
    if (t == node)
      return;
  VEC_safe_push (tree, heap, g->nodes, node);
}


/* Add the edge PRED->SUCC to graph G.  */

static inline void
elim_graph_add_edge (elim_graph g, int pred, int succ)
{
  VEC_safe_push (int, heap, g->edge_list, pred);
  VEC_safe_push (int, heap, g->edge_list, succ);
}


/* Remove an edge from graph G for which NODE is the predecessor, and
   return the successor node.  -1 is returned if there is no such edge.  */

static inline int
elim_graph_remove_succ_edge (elim_graph g, int node)
{
  int y;
  unsigned x;
  for (x = 0; x < VEC_length (int, g->edge_list); x += 2)
    if (VEC_index (int, g->edge_list, x) == node)
      {
        VEC_replace (int, g->edge_list, x, -1);
	y = VEC_index (int, g->edge_list, x + 1);
	VEC_replace (int, g->edge_list, x + 1, -1);
	return y;
      }
  return -1;
}


/* Find all the nodes in GRAPH which are successors to NODE in the
   edge list.  VAR will hold the partition number found.  CODE is the
   code fragment executed for every node found.  */

#define FOR_EACH_ELIM_GRAPH_SUCC(GRAPH, NODE, VAR, CODE)		\
do {									\
  unsigned x_;								\
  int y_;								\
  for (x_ = 0; x_ < VEC_length (int, (GRAPH)->edge_list); x_ += 2)	\
    {									\
      y_ = VEC_index (int, (GRAPH)->edge_list, x_);			\
      if (y_ != (NODE))							\
        continue;							\
      (VAR) = VEC_index (int, (GRAPH)->edge_list, x_ + 1);		\
      CODE;								\
    }									\
} while (0)


/* Find all the nodes which are predecessors of NODE in the edge list for
   GRAPH.  VAR will hold the partition number found.  CODE is the
   code fragment executed for every node found.  */

#define FOR_EACH_ELIM_GRAPH_PRED(GRAPH, NODE, VAR, CODE)		\
do {									\
  unsigned x_;								\
  int y_;								\
  for (x_ = 0; x_ < VEC_length (int, (GRAPH)->edge_list); x_ += 2)	\
    {									\
      y_ = VEC_index (int, (GRAPH)->edge_list, x_ + 1);			\
      if (y_ != (NODE))							\
        continue;							\
      (VAR) = VEC_index (int, (GRAPH)->edge_list, x_);			\
      CODE;								\
    }									\
} while (0)


/* Add T to elimination graph G.  */

static inline void
eliminate_name (elim_graph g, tree T)
{
  elim_graph_add_node (g, T);
}


/* Build elimination graph G for basic block BB on incoming PHI edge
   G->e.  */

static void
eliminate_build (elim_graph g, basic_block B)
{
  tree phi;
  tree T0, Ti;
  int p0, pi;

  clear_elim_graph (g);
  
  for (phi = phi_nodes (B); phi; phi = PHI_CHAIN (phi))
    {
      T0 = var_to_partition_to_var (g->map, PHI_RESULT (phi));
      
      /* Ignore results which are not in partitions.  */
      if (T0 == NULL_TREE)
	continue;

      Ti = PHI_ARG_DEF (phi, g->e->dest_idx);

      /* If this argument is a constant, or a SSA_NAME which is being
	 left in SSA form, just queue a copy to be emitted on this
	 edge.  */
      if (!phi_ssa_name_p (Ti)
	  || (TREE_CODE (Ti) == SSA_NAME
	      && var_to_partition (g->map, Ti) == NO_PARTITION))
        {
	  /* Save constant copies until all other copies have been emitted
	     on this edge.  */
	  VEC_safe_push (tree, heap, g->const_copies, T0);
	  VEC_safe_push (tree, heap, g->const_copies, Ti);
	}
      else
        {
	  Ti = var_to_partition_to_var (g->map, Ti);
	  if (T0 != Ti)
	    {
	      eliminate_name (g, T0);
	      eliminate_name (g, Ti);
	      p0 = var_to_partition (g->map, T0);
	      pi = var_to_partition (g->map, Ti);
	      elim_graph_add_edge (g, p0, pi);
	    }
	}
    }
}


/* Push successors of T onto the elimination stack for G.  */

static void 
elim_forward (elim_graph g, int T)
{
  int S;
  SET_BIT (g->visited, T);
  FOR_EACH_ELIM_GRAPH_SUCC (g, T, S,
    {
      if (!TEST_BIT (g->visited, S))
        elim_forward (g, S);
    });
  VEC_safe_push (int, heap, g->stack, T);
}


/* Return 1 if there unvisited predecessors of T in graph G.  */

static int
elim_unvisited_predecessor (elim_graph g, int T)
{
  int P;
  FOR_EACH_ELIM_GRAPH_PRED (g, T, P, 
    {
      if (!TEST_BIT (g->visited, P))
        return 1;
    });
  return 0;
}

/* Process predecessors first, and insert a copy.  */

static void
elim_backward (elim_graph g, int T)
{
  int P;
  SET_BIT (g->visited, T);
  FOR_EACH_ELIM_GRAPH_PRED (g, T, P, 
    {
      if (!TEST_BIT (g->visited, P))
        {
	  elim_backward (g, P);
	  insert_copy_on_edge (g->e, 
			       partition_to_var (g->map, P), 
			       partition_to_var (g->map, T));
	}
    });
}

/* Insert required copies for T in graph G.  Check for a strongly connected 
   region, and create a temporary to break the cycle if one is found.  */

static void 
elim_create (elim_graph g, int T)
{
  tree U;
  int P, S;

  if (elim_unvisited_predecessor (g, T))
    {
      U = create_temp (partition_to_var (g->map, T));
      insert_copy_on_edge (g->e, U, partition_to_var (g->map, T));
      FOR_EACH_ELIM_GRAPH_PRED (g, T, P, 
	{
	  if (!TEST_BIT (g->visited, P))
	    {
	      elim_backward (g, P);
	      insert_copy_on_edge (g->e, partition_to_var (g->map, P), U);
	    }
	});
    }
  else
    {
      S = elim_graph_remove_succ_edge (g, T);
      if (S != -1)
	{
	  SET_BIT (g->visited, T);
	  insert_copy_on_edge (g->e, 
			       partition_to_var (g->map, T), 
			       partition_to_var (g->map, S));
	}
    }
  
}


/* Eliminate all the phi nodes on edge E in graph G.  */

static void
eliminate_phi (edge e, elim_graph g)
{
  int x;
  basic_block B = e->dest;

  gcc_assert (VEC_length (tree, g->const_copies) == 0);

  /* Abnormal edges already have everything coalesced.  */
  if (e->flags & EDGE_ABNORMAL)
    return;

  g->e = e;

  eliminate_build (g, B);

  if (elim_graph_size (g) != 0)
    {
      tree var;

      sbitmap_zero (g->visited);
      VEC_truncate (int, g->stack, 0);

      for (x = 0; VEC_iterate (tree, g->nodes, x, var); x++)
        {
	  int p = var_to_partition (g->map, var);
	  if (!TEST_BIT (g->visited, p))
	    elim_forward (g, p);
	}
       
      sbitmap_zero (g->visited);
      while (VEC_length (int, g->stack) > 0)
	{
	  x = VEC_pop (int, g->stack);
	  if (!TEST_BIT (g->visited, x))
	    elim_create (g, x);
	}
    }

  /* If there are any pending constant copies, issue them now.  */
  while (VEC_length (tree, g->const_copies) > 0)
    {
      tree src, dest;
      src = VEC_pop (tree, g->const_copies);
      dest = VEC_pop (tree, g->const_copies);
      insert_copy_on_edge (e, dest, src);
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

static var_map
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
  partition_view_coalesce (map, cl, true);

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


/* Take the ssa-name var_map MAP, and assign real variables to each 
   partition.  */

static void
assign_vars (var_map map)
{
  int x, num;
  tree var;
  var_ann_t ann;
  tree root;

  num = num_var_partitions (map);
  for (x = 0; x < num; x++)
    {
      var = partition_to_var (map, x);
      if (TREE_CODE (var) != SSA_NAME)
	{
	  ann = var_ann (var);
	  /* It must already be coalesced.  */
	  gcc_assert (ann->out_of_ssa_tag == 1);
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "partition %d already has variable ", x);
	      print_generic_expr (dump_file, var, TDF_SLIM);
	      fprintf (dump_file, " assigned to it.\n");
	    }
	}
      else
        {
	  root = SSA_NAME_VAR (var);
	  ann = var_ann (root);
	  /* If ROOT is already associated, create a new one.  */
	  if (ann->out_of_ssa_tag)
	    {
	      root = create_temp (root);
	      ann = var_ann (root);
	    }
	  /* ROOT has not been coalesced yet, so use it.  */
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Partition %d is assigned to var ", x);
	      print_generic_stmt (dump_file, root, TDF_SLIM);
	    }
	  change_partition_var (map, root, x);
	}
    }
}


/* Replace use operand P with whatever variable it has been rewritten to based 
   on the partitions in MAP.  EXPR is an optional expression vector over SSA 
   versions which is used to replace P with an expression instead of a variable.
   If the stmt is changed, return true.  */ 

static inline bool
replace_use_variable (var_map map, use_operand_p p, tree *expr)
{
  tree new_var;
  tree var = USE_FROM_PTR (p);

  /* Check if we are replacing this variable with an expression.  */
  if (expr)
    {
      int version = SSA_NAME_VERSION (var);
      if (expr[version])
        {
	  tree new_expr = TREE_OPERAND (expr[version], 1);
	  SET_USE (p, new_expr);

	  /* Clear the stmt's RHS since it now belongs here.  */
	  TREE_OPERAND (expr[version], 1) = NULL_TREE;
	  return true;
	}
    }

  new_var = var_to_partition_to_var (map, var);
  if (new_var)
    {
      SET_USE (p, new_var);
      set_is_used (new_var);
      return true;
    }
  return false;
}


/* Replace def operand DEF_P with whatever variable it has been rewritten to 
   based on the partitions in MAP.  EXPR is an optional expression vector over
   SSA versions which is used to replace DEF_P with an expression instead of a 
   variable.  If the stmt is changed, return true.  */ 

static inline bool
replace_def_variable (var_map map, def_operand_p def_p, tree *expr)
{
  tree new_var;
  tree var = DEF_FROM_PTR (def_p);

  /* Do nothing if we are replacing this variable with an expression.  */
  if (expr && expr[SSA_NAME_VERSION (var)])
    return true;

  new_var = var_to_partition_to_var (map, var);
  if (new_var)
    {
      SET_DEF (def_p, new_var);
      set_is_used (new_var);
      return true;
    }
  return false;
}


/* Remove any PHI node which is a virtual PHI.  */

static void
eliminate_virtual_phis (void)
{
  basic_block bb;
  tree phi, next;

  FOR_EACH_BB (bb)
    {
      for (phi = phi_nodes (bb); phi; phi = next)
        {
	  next = PHI_CHAIN (phi);
	  if (!is_gimple_reg (SSA_NAME_VAR (PHI_RESULT (phi))))
	    {
#ifdef ENABLE_CHECKING
	      int i;
	      /* There should be no arguments of this PHI which are in
		 the partition list, or we get incorrect results.  */
	      for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	        {
		  tree arg = PHI_ARG_DEF (phi, i);
		  if (TREE_CODE (arg) == SSA_NAME 
		      && is_gimple_reg (SSA_NAME_VAR (arg)))
		    {
		      fprintf (stderr, "Argument of PHI is not virtual (");
		      print_generic_expr (stderr, arg, TDF_SLIM);
		      fprintf (stderr, "), but the result is :");
		      print_generic_stmt (stderr, phi, TDF_SLIM);
		      internal_error ("SSA corruption");
		    }
		}
#endif
	      remove_phi_node (phi, NULL_TREE);
	    }
	}
    }
}


/* This function will rewrite the current program using the variable mapping
   found in MAP.  If the replacement vector VALUES is provided, any 
   occurrences of partitions with non-null entries in the vector will be 
   replaced with the expression in the vector instead of its mapped 
   variable.  */

static void
rewrite_trees (var_map map, tree *values)
{
  elim_graph g;
  basic_block bb;
  block_stmt_iterator si;
  edge e;
  tree phi;
  bool changed;
 
#ifdef ENABLE_CHECKING
  /* Search for PHIs where the destination has no partition, but one
     or more arguments has a partition.  This should not happen and can
     create incorrect code.  */
  FOR_EACH_BB (bb)
    {
      tree phi;
      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	{
	  tree T0 = var_to_partition_to_var (map, PHI_RESULT (phi));
	  if (T0 == NULL_TREE)
	    {
	      int i;
	      for (i = 0; i < PHI_NUM_ARGS (phi); i++)
		{
		  tree arg = PHI_ARG_DEF (phi, i);

		  if (TREE_CODE (arg) == SSA_NAME
		      && var_to_partition (map, arg) != NO_PARTITION)
		    {
		      fprintf (stderr, "Argument of PHI is in a partition :(");
		      print_generic_expr (stderr, arg, TDF_SLIM);
		      fprintf (stderr, "), but the result is not :");
		      print_generic_stmt (stderr, phi, TDF_SLIM);
		      internal_error ("SSA corruption");
		    }
		}
	    }
	}
    }
#endif

  /* Replace PHI nodes with any required copies.  */
  g = new_elim_graph (map->num_partitions);
  g->map = map;
  FOR_EACH_BB (bb)
    {
      for (si = bsi_start (bb); !bsi_end_p (si); )
	{
	  tree stmt = bsi_stmt (si);
	  use_operand_p use_p, copy_use_p;
	  def_operand_p def_p;
	  bool remove = false, is_copy = false;
	  int num_uses = 0;
	  stmt_ann_t ann;
	  ssa_op_iter iter;

	  ann = stmt_ann (stmt);
	  changed = false;

	  if (TREE_CODE (stmt) == MODIFY_EXPR 
	      && (TREE_CODE (TREE_OPERAND (stmt, 1)) == SSA_NAME))
	    is_copy = true;

	  copy_use_p = NULL_USE_OPERAND_P;
	  FOR_EACH_SSA_USE_OPERAND (use_p, stmt, iter, SSA_OP_USE)
	    {
	      if (replace_use_variable (map, use_p, values))
		changed = true;
	      copy_use_p = use_p;
	      num_uses++;
	    }

	  if (num_uses != 1)
	    is_copy = false;

	  def_p = SINGLE_SSA_DEF_OPERAND (stmt, SSA_OP_DEF);

	  if (def_p != NULL)
	    {
	      /* Mark this stmt for removal if it is the list of replaceable 
		 expressions.  */
	      if (values && values[SSA_NAME_VERSION (DEF_FROM_PTR (def_p))])
		remove = true;
	      else
		{
		  if (replace_def_variable (map, def_p, NULL))
		    changed = true;
		  /* If both SSA_NAMEs coalesce to the same variable,
		     mark the now redundant copy for removal.  */
		  if (is_copy)
		    {
		      gcc_assert (copy_use_p != NULL_USE_OPERAND_P);
		      if (DEF_FROM_PTR (def_p) == USE_FROM_PTR (copy_use_p))
			remove = true;
		    }
		}
	    }
	  else
	    FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, iter, SSA_OP_DEF)
	      if (replace_def_variable (map, def_p, NULL))
		changed = true;

	  /* Remove any stmts marked for removal.  */
	  if (remove)
	    bsi_remove (&si, true);
	  else
	    bsi_next (&si);
	}

      phi = phi_nodes (bb);
      if (phi)
        {
	  edge_iterator ei;
	  FOR_EACH_EDGE (e, ei, bb->preds)
	    eliminate_phi (e, g);
	}
    }

  delete_elim_graph (g);
}


DEF_VEC_ALLOC_P(edge,heap);

/* These are the local work structures used to determine the best place to 
   insert the copies that were placed on edges by the SSA->normal pass..  */
static VEC(edge,heap) *edge_leader;
static VEC(tree,heap) *stmt_list;
static bitmap leader_has_match = NULL;
static edge leader_match = NULL;


/* Pass this function to make_forwarder_block so that all the edges with
   matching PENDING_STMT lists to 'curr_stmt_list' get redirected.  E is the
   edge to test for a match.  */

static inline bool 
same_stmt_list_p (edge e)
{
  return (e->aux == (PTR) leader_match) ? true : false;
}


/* Return TRUE if S1 and S2 are equivalent copies.  */

static inline bool
identical_copies_p (tree s1, tree s2)
{
#ifdef ENABLE_CHECKING
  gcc_assert (TREE_CODE (s1) == MODIFY_EXPR);
  gcc_assert (TREE_CODE (s2) == MODIFY_EXPR);
  gcc_assert (DECL_P (TREE_OPERAND (s1, 0)));
  gcc_assert (DECL_P (TREE_OPERAND (s2, 0)));
#endif

  if (TREE_OPERAND (s1, 0) != TREE_OPERAND (s2, 0))
    return false;

  s1 = TREE_OPERAND (s1, 1);
  s2 = TREE_OPERAND (s2, 1);

  if (s1 != s2)
    return false;

  return true;
}


/* Compare the PENDING_STMT list for edges E1 and E2. Return true if the lists
   contain the same sequence of copies.  */

static inline bool 
identical_stmt_lists_p (edge e1, edge e2)
{
  tree t1 = PENDING_STMT (e1);
  tree t2 = PENDING_STMT (e2);
  tree_stmt_iterator tsi1, tsi2;

  gcc_assert (TREE_CODE (t1) == STATEMENT_LIST);
  gcc_assert (TREE_CODE (t2) == STATEMENT_LIST);

  for (tsi1 = tsi_start (t1), tsi2 = tsi_start (t2);
       !tsi_end_p (tsi1) && !tsi_end_p (tsi2); 
       tsi_next (&tsi1), tsi_next (&tsi2))
    {
      if (!identical_copies_p (tsi_stmt (tsi1), tsi_stmt (tsi2)))
        break;
    }

  if (!tsi_end_p (tsi1) || ! tsi_end_p (tsi2))
    return false;

  return true;
}


/* Allocate data structures used in analyze_edges_for_bb.   */

static void
init_analyze_edges_for_bb (void)
{
  edge_leader = VEC_alloc (edge, heap, 25);
  stmt_list = VEC_alloc (tree, heap, 25);
  leader_has_match = BITMAP_ALLOC (NULL);
}


/* Free data structures used in analyze_edges_for_bb.   */

static void
fini_analyze_edges_for_bb (void)
{
  VEC_free (edge, heap, edge_leader);
  VEC_free (tree, heap, stmt_list);
  BITMAP_FREE (leader_has_match);
}


/* Look at all the incoming edges to block BB, and decide where the best place
   to insert the stmts on each edge are, and perform those insertions.  */

static void
analyze_edges_for_bb (basic_block bb)
{
  edge e;
  edge_iterator ei;
  int count;
  unsigned int x;
  bool have_opportunity;
  block_stmt_iterator bsi;
  tree stmt;
  edge single_edge = NULL;
  bool is_label;
  edge leader;

  count = 0;

  /* Blocks which contain at least one abnormal edge cannot use 
     make_forwarder_block.  Look for these blocks, and commit any PENDING_STMTs
     found on edges in these block.  */
  have_opportunity = true;
  FOR_EACH_EDGE (e, ei, bb->preds)
    if (e->flags & EDGE_ABNORMAL)
      {
        have_opportunity = false;
	break;
      }

  if (!have_opportunity)
    {
      FOR_EACH_EDGE (e, ei, bb->preds)
	if (PENDING_STMT (e))
	  bsi_commit_one_edge_insert (e, NULL);
      return;
    }

  /* Find out how many edges there are with interesting pending stmts on them.  
     Commit the stmts on edges we are not interested in.  */
  FOR_EACH_EDGE (e, ei, bb->preds)
    {
      if (PENDING_STMT (e))
        {
	  gcc_assert (!(e->flags & EDGE_ABNORMAL));
	  if (e->flags & EDGE_FALLTHRU)
	    {
	      bsi = bsi_start (e->src);
	      if (!bsi_end_p (bsi))
	        {
		  stmt = bsi_stmt (bsi);
		  bsi_next (&bsi);
		  gcc_assert (stmt != NULL_TREE);
		  is_label = (TREE_CODE (stmt) == LABEL_EXPR);
		  /* Punt if it has non-label stmts, or isn't local.  */
		  if (!is_label || DECL_NONLOCAL (TREE_OPERAND (stmt, 0)) 
		      || !bsi_end_p (bsi))
		    {
		      bsi_commit_one_edge_insert (e, NULL);
		      continue;
		    }
		}
	    }
	  single_edge = e;
	  count++;
	}
    }

  /* If there aren't at least 2 edges, no sharing will happen.  */
  if (count < 2)
    {
      if (single_edge)
        bsi_commit_one_edge_insert (single_edge, NULL);
      return;
    }

  /* Ensure that we have empty worklists.  */
#ifdef ENABLE_CHECKING
  gcc_assert (VEC_length (edge, edge_leader) == 0);
  gcc_assert (VEC_length (tree, stmt_list) == 0);
  gcc_assert (bitmap_empty_p (leader_has_match));
#endif

  /* Find the "leader" block for each set of unique stmt lists.  Preference is
     given to FALLTHRU blocks since they would need a GOTO to arrive at another
     block.  The leader edge destination is the block which all the other edges
     with the same stmt list will be redirected to.  */
  have_opportunity = false;
  FOR_EACH_EDGE (e, ei, bb->preds)
    {
      if (PENDING_STMT (e))
	{
	  bool found = false;

	  /* Look for the same stmt list in edge leaders list.  */
	  for (x = 0; VEC_iterate (edge, edge_leader, x, leader); x++)
	    {
	      if (identical_stmt_lists_p (leader, e))
		{
		  /* Give this edge the same stmt list pointer.  */
		  PENDING_STMT (e) = NULL;
		  e->aux = leader;
		  bitmap_set_bit (leader_has_match, x);
		  have_opportunity = found = true;
		  break;
		}
	    }

	  /* If no similar stmt list, add this edge to the leader list.  */
	  if (!found)
	    {
	      VEC_safe_push (edge, heap, edge_leader, e);
	      VEC_safe_push (tree, heap, stmt_list, PENDING_STMT (e));
	    }
	}
     }

  /* If there are no similar lists, just issue the stmts.  */
  if (!have_opportunity)
    {
      for (x = 0; VEC_iterate (edge, edge_leader, x, leader); x++)
	bsi_commit_one_edge_insert (leader, NULL);
      VEC_truncate (edge, edge_leader, 0);
      VEC_truncate (tree, stmt_list, 0);
      bitmap_clear (leader_has_match);
      return;
    }

  if (dump_file)
    fprintf (dump_file, "\nOpportunities in BB %d for stmt/block reduction:\n",
	     bb->index);
  
  /* For each common list, create a forwarding block and issue the stmt's
     in that block.  */
  for (x = 0; VEC_iterate (edge, edge_leader, x, leader); x++)
    if (bitmap_bit_p (leader_has_match, x))
      {
	edge new_edge;
	block_stmt_iterator bsi;
	tree curr_stmt_list;

	leader_match = leader;

	/* The tree_* cfg manipulation routines use the PENDING_EDGE field
	   for various PHI manipulations, so it gets cleared when calls are 
	   made to make_forwarder_block(). So make sure the edge is clear, 
	   and use the saved stmt list.  */
	PENDING_STMT (leader) = NULL;
	leader->aux = leader;
	curr_stmt_list = VEC_index (tree, stmt_list, x);

        new_edge = make_forwarder_block (leader->dest, same_stmt_list_p, 
					 NULL);
	bb = new_edge->dest;
	if (dump_file)
	  {
	    fprintf (dump_file, "Splitting BB %d for Common stmt list.  ", 
		     leader->dest->index);
	    fprintf (dump_file, "Original block is now BB%d.\n", bb->index);
	    print_generic_stmt (dump_file, curr_stmt_list, TDF_VOPS);
	  }

	FOR_EACH_EDGE (e, ei, new_edge->src->preds)
	  {
	    e->aux = NULL;
	    if (dump_file)
	      fprintf (dump_file, "  Edge (%d->%d) lands here.\n", 
		       e->src->index, e->dest->index);
	  }

	bsi = bsi_last (leader->dest);
	bsi_insert_after (&bsi, curr_stmt_list, BSI_NEW_STMT);

	leader_match = NULL;
	/* We should never get a new block now.  */
      }
    else
      {
	PENDING_STMT (leader) = VEC_index (tree, stmt_list, x);
	bsi_commit_one_edge_insert (leader, NULL);
      }

   
  /* Clear the working data structures.  */
  VEC_truncate (edge, edge_leader, 0);
  VEC_truncate (tree, stmt_list, 0);
  bitmap_clear (leader_has_match);
}


/* This function will analyze the insertions which were performed on edges,
   and decide whether they should be left on that edge, or whether it is more
   efficient to emit some subset of them in a single block.  All stmts are
   inserted somewhere.  */

static void
perform_edge_inserts (void)
{
  basic_block bb;

  if (dump_file)
    fprintf(dump_file, "Analyzing Edge Insertions.\n");

  /* analyze_edges_for_bb calls make_forwarder_block, which tries to
     incrementally update the dominator information.  Since we don't
     need dominator information after this pass, go ahead and free the
     dominator information.  */
  free_dominance_info (CDI_DOMINATORS);
  free_dominance_info (CDI_POST_DOMINATORS);

  /* Allocate data structures used in analyze_edges_for_bb.   */
  init_analyze_edges_for_bb ();

  FOR_EACH_BB (bb)
    analyze_edges_for_bb (bb);

  analyze_edges_for_bb (EXIT_BLOCK_PTR);

  /* Free data structures used in analyze_edges_for_bb.   */
  fini_analyze_edges_for_bb ();

#ifdef ENABLE_CHECKING
  {
    edge_iterator ei;
    edge e;
    FOR_EACH_BB (bb)
      {
	FOR_EACH_EDGE (e, ei, bb->preds)
	  {
	    if (PENDING_STMT (e))
	      error (" Pending stmts not issued on PRED edge (%d, %d)\n", 
		     e->src->index, e->dest->index);
	  }
	FOR_EACH_EDGE (e, ei, bb->succs)
	  {
	    if (PENDING_STMT (e))
	      error (" Pending stmts not issued on SUCC edge (%d, %d)\n", 
		     e->src->index, e->dest->index);
	  }
      }
    FOR_EACH_EDGE (e, ei, ENTRY_BLOCK_PTR->succs)
      {
	if (PENDING_STMT (e))
	  error (" Pending stmts not issued on ENTRY edge (%d, %d)\n", 
		 e->src->index, e->dest->index);
      }
    FOR_EACH_EDGE (e, ei, EXIT_BLOCK_PTR->preds)
      {
	if (PENDING_STMT (e))
	  error (" Pending stmts not issued on EXIT edge (%d, %d)\n", 
		 e->src->index, e->dest->index);
      }
  }
#endif
}


/* Remove the ssa-names in the current function and translate them into normal
   compiler variables.  PERFORM_TER is true if Temporary Expression Replacement
   should also be used.  */

static void
remove_ssa_form (bool perform_ter)
{
  basic_block bb;
  tree phi, next;
  tree *values = NULL;
  var_map map;

  map = coalesce_ssa_name ();

  /* Return to viewing the variable list as just all reference variables after
     coalescing has been performed.  */
  partition_view_normal (map, false);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "After Coalescing:\n");
      dump_var_map (dump_file, map);
    }

  if (perform_ter)
    {
      values = find_replaceable_exprs (map);
      if (values && dump_file && (dump_flags & TDF_DETAILS))
	dump_replaceable_exprs (dump_file, values);
    }

  /* Assign real variables to the partitions now.  */
  assign_vars (map);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "After Base variable replacement:\n");
      dump_var_map (dump_file, map);
    }

  rewrite_trees (map, values);

  if (values)
    free (values);

  /* Remove phi nodes which have been translated back to real variables.  */
  FOR_EACH_BB (bb)
    {
      for (phi = phi_nodes (bb); phi; phi = next)
	{
	  next = PHI_CHAIN (phi);
	  remove_phi_node (phi, NULL_TREE);
	}
    }

  /* we no longer maintain the SSA operand cache at this point.  */
  fini_ssa_operands ();

  /* If any copies were inserted on edges, analyze and insert them now.  */
  perform_edge_inserts ();

  delete_var_map (map);
}


/* Search every PHI node for arguments associated with backedges which
   we can trivially determine will need a copy (the argument is either
   not an SSA_NAME or the argument has a different underlying variable
   than the PHI result).

   Insert a copy from the PHI argument to a new destination at the
   end of the block with the backedge to the top of the loop.  Update
   the PHI argument to reference this new destination.  */

static void
insert_backedge_copies (void)
{
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	{
	  tree result = PHI_RESULT (phi);
	  tree result_var;
	  int i;

	  if (!is_gimple_reg (result))
	    continue;

	  result_var = SSA_NAME_VAR (result);
	  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	    {
	      tree arg = PHI_ARG_DEF (phi, i);
	      edge e = PHI_ARG_EDGE (phi, i);

	      /* If the argument is not an SSA_NAME, then we will need a 
		 constant initialization.  If the argument is an SSA_NAME with
		 a different underlying variable then a copy statement will be 
		 needed.  */
	      if ((e->flags & EDGE_DFS_BACK)
		  && (TREE_CODE (arg) != SSA_NAME
		      || SSA_NAME_VAR (arg) != result_var))
		{
		  tree stmt, name, last = NULL;
		  block_stmt_iterator bsi;

		  bsi = bsi_last (PHI_ARG_EDGE (phi, i)->src);
		  if (!bsi_end_p (bsi))
		    last = bsi_stmt (bsi);

		  /* In theory the only way we ought to get back to the
		     start of a loop should be with a COND_EXPR or GOTO_EXPR.
		     However, better safe than sorry. 
		     If the block ends with a control statement or
		     something that might throw, then we have to
		     insert this assignment before the last
		     statement.  Else insert it after the last statement.  */
		  if (last && stmt_ends_bb_p (last))
		    {
		      /* If the last statement in the block is the definition
			 site of the PHI argument, then we can't insert
			 anything after it.  */
		      if (TREE_CODE (arg) == SSA_NAME
			  && SSA_NAME_DEF_STMT (arg) == last)
			continue;
		    }

		  /* Create a new instance of the underlying variable of the 
		     PHI result.  */
		  stmt = build2 (MODIFY_EXPR, TREE_TYPE (result_var),
				 NULL_TREE, PHI_ARG_DEF (phi, i));
		  name = make_ssa_name (result_var, stmt);
		  TREE_OPERAND (stmt, 0) = name;

		  /* Insert the new statement into the block and update
		     the PHI node.  */
		  if (last && stmt_ends_bb_p (last))
		    bsi_insert_before (&bsi, stmt, BSI_NEW_STMT);
		  else
		    bsi_insert_after (&bsi, stmt, BSI_NEW_STMT);
		  SET_PHI_ARG_DEF (phi, i, name);
		}
	    }
	}
    }
}


/* Take the current function out of SSA form, translating PHIs as described in
   R. Morgan, ``Building an Optimizing Compiler'',
   Butterworth-Heinemann, Boston, MA, 1998. pp 176-186.  */

static unsigned int
rewrite_out_of_ssa (void)
{
  /* If elimination of a PHI requires inserting a copy on a backedge,
     then we will have to split the backedge which has numerous
     undesirable performance effects.
     A significant number of such cases can be handled here by inserting
     copies into the loop itself.  */
  insert_backedge_copies ();

  eliminate_virtual_phis ();

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_tree_cfg (dump_file, dump_flags & ~TDF_DETAILS);

  remove_ssa_form (flag_tree_ter && !flag_mudflap);

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_tree_cfg (dump_file, dump_flags & ~TDF_DETAILS);

  in_ssa_p = false;
  return 0;
}


/* Define the parameters of the out of SSA pass.  */

struct tree_opt_pass pass_del_ssa = 
{
  "optimized",				/* name */
  NULL,					/* gate */
  rewrite_out_of_ssa,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_TREE_SSA_TO_NORMAL,		/* tv_id */
  PROP_cfg | PROP_ssa | PROP_alias,	/* properties_required */
  0,					/* properties_provided */
  /* ??? If TER is enabled, we also kill gimple.  */
  PROP_ssa,				/* properties_destroyed */
  TODO_verify_ssa | TODO_verify_flow
    | TODO_verify_stmts,		/* todo_flags_start */
  TODO_dump_func
  | TODO_ggc_collect
  | TODO_remove_unused_locals,		/* todo_flags_finish */
  0					/* letter */
};
