/* Conditional constant propagation pass for the GNU compiler.
   Copyright (C) 2000,2001 Free Software Foundation, Inc.
   Adapted from original RTL SSA-CCP by Daniel Berlin <dberlin@dberlin.org>
   Adapted to SIMPLE trees by Diego Novillo <dnovillo@redhat.com>

This file is part of GNU CC.
   
GNU CC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GNU CC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.
   
You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/* Conditional constant propagation.

   References:

     Constant propagation with conditional branches,
     Wegman and Zadeck, ACM TOPLAS 13(2):181-210.

     Building an Optimizing Compiler,
     Robert Morgan, Butterworth-Heinemann, 1998, Section 8.9.

     Advanced Compiler Design and Implementation,
     Steven Muchnick, Morgan Kaufmann, 1997, Section 12.6

   The overall structure is as follows:

	1. Run a simple SSA based DCE pass to remove any dead code.
	2. Run CCP to compute what variables are known constants
	   and what edges are not executable.  Remove unexecutable
	   edges from the CFG and simplify PHI nodes.
	3. Replace variables with constants where possible.
	4. Remove unreachable blocks computed in step #2.
	5. Another simple SSA DCE pass to remove dead code exposed
	   by CCP.

   When we exit, we are still in SSA form.  */

#include "config.h"
#include "system.h"
#include "errors.h"
#include "ggc.h"
#include "tree.h"

/* These RTL headers are needed for basic-block.h.  */
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"

#include "tree-flow.h"
#include "tree-optimize.h"
#include "tree-simple.h"

/* This should be eventually be generalized to other languages, but
   this would require a shared function-as-trees infrastructure.  */
#include "c-common.h"
#include "c-tree.h"

/* Possible lattice values.  */
typedef enum
{
  UNDEFINED,
  CONSTANT,
  VARYING
} latticevalue;


/* Main structure for CCP.  Contains the lattice value and, if it's a
   constant, the constant value.  */
typedef struct
{
  latticevalue lattice_val;
  tree const_value;
} value;

/* Array of values indexed by reference ID.  Each reference will be
   assigned a value out of the constant lattice: UNDEFINED (top), meaning
   that the reference is to a constant of unknown value, CONSTANT,
   meaning that the reference has a known constant value and VARYING
   (bottom), meaning that the reference is to a non-constant value.  */
static value *values;

/* A bitmap to keep track of executable blocks in the CFG.  */
static sbitmap executable_blocks;

/* A bitmap for all executable edges in the CFG.  */
static sbitmap executable_edges;

/* Array of edges on the work list.  */
static edge *edge_info;

/* We need an edge list to be able to get indexes easily.  */
static struct edge_list *edges;

/* Current edge we are operating on, from the worklist.  */
static edge flow_edges;

/* Stack of SSA edges which will need reexamination as their definition
   has changed.  SSA edges are def-use edges in the SSA web.  For each
   edge, we store the originating definition.  */
static varray_type ssa_edges;

/* Simple macros to simplify code */
#define SSA_NAME(x) VARREF_ID (x)
#define PHI_PARMS(x) VARDEF_PHI_CHAIN (x)
#define EIE(x,y) EDGE_INDEX (edges, x, y)

static void visit_phi_node             PARAMS ((varref));
static void visit_expression           PARAMS ((varref));
static void visit_assignment           PARAMS ((varref));
static void def_to_undefined           PARAMS ((varref));
static void def_to_varying             PARAMS ((varref));
static void examine_flow_edges         PARAMS ((void));
static void follow_def_use_chains      PARAMS ((void));
static void optimize_unexecutable_edges PARAMS ((struct edge_list *, sbitmap));
static void ssa_ccp_substitute_constants PARAMS ((void));
static void ssa_ccp_df_delete_unreachable_insns PARAMS ((void));
static value evaluate_expr_for         PARAMS ((varref));
static void dump_lattice_value         PARAMS ((FILE *, const char *, value));

/* Debugging dumps.  */
static FILE *dump_file;
static int dump_flags;


/* Main entry point for SSA Conditional Constant Propagation.  FNDECL is
   the declaration for the function to optimize.

   Long term it should accept as input the specific flow graph to
   operate on so that it can be called for sub-graphs.  */

void
tree_ssa_ccp (fndecl)
     tree fndecl;
{
  unsigned int i;
  edge curredge;
  tree fnbody;

  fnbody = COMPOUND_BODY (DECL_SAVED_TREE (fndecl));

#if defined ENABLE_CHECKING
  if (fnbody == NULL_TREE)
    abort ();
#endif

  /* Initialize debugging dumps.  */
  dump_file = dump_begin (TDI_ccp, &dump_flags);

  /* Build an edge list from the CFG.  */
  edges = create_edge_list ();

  /* Initialize the values array with everything as undefined.  */
  values = (value *) xmalloc (next_varref_id * sizeof (value));
  for (i = 0; i < next_varref_id; i++)
    {
      values[i].lattice_val = UNDEFINED;
      values[i].const_value = NULL_TREE;
    }

  /* Stack of SSA (def-use) edges.  FIXME: We shouldn't need more than a
     fraction of entries.  See if we can get away with less than half.  */
  VARRAY_GENERIC_PTR_INIT (ssa_edges, next_varref_id / 2, "ssa_edges");

  executable_blocks = sbitmap_alloc (last_basic_block);
  sbitmap_zero (executable_blocks);

  executable_edges = sbitmap_alloc (NUM_EDGES (edges));
  sbitmap_zero (executable_edges);

  edge_info = (edge *) xmalloc (NUM_EDGES (edges) * sizeof (edge));
  flow_edges = ENTRY_BLOCK_PTR->succ;

  /* Add the successors of the entry block to the edge worklist.  That
     is enough of a seed to get SSA-CCP started.  */
  for (curredge = ENTRY_BLOCK_PTR->succ; curredge;
       curredge = curredge->succ_next)
    {
      int index = EIE (curredge->src, curredge->dest);
      SET_BIT (executable_edges, index);
      edge_info[index] = curredge->succ_next;
    }

  /* Iterate until the worklists are empty.  */
  do
    {
      examine_flow_edges ();
      follow_def_use_chains ();
    }
  while (flow_edges != NULL);

  /* Now perform substitutions based on the known constant values.  */
  ssa_ccp_substitute_constants ();

  /* Remove unexecutable edges from the CFG and make appropriate
     adjustments to PHI nodes.  */
  optimize_unexecutable_edges (edges, executable_edges);

  /* Now remove all unreachable insns and update the DF information.
     as appropriate.  */
  ssa_ccp_df_delete_unreachable_insns ();

  /* Debugging dumps.  */
  if (dump_file)
    {
      fprintf (dump_file, "%s()\n", IDENTIFIER_POINTER (DECL_NAME (fndecl)));

      if (dump_flags & TDF_RAW)
	dump_node (fnbody, TDF_SLIM | dump_flags, dump_file);
      else
	print_c_tree (dump_file, fnbody);

      fprintf (dump_file, "\n");
      dump_end (TDI_ccp, dump_file);
    }

  free (values);
  values = NULL;

  free (edge_info);
  edge_info = NULL;

  sbitmap_free (executable_blocks);
  executable_blocks = NULL;

  free_edge_list (edges);
  edges = NULL;

  sbitmap_free (executable_edges);
  executable_edges = NULL;
}


/* Loop through the PHI_NODE's parameters for BLOCK and compare their
   lattice values to determine PHI_NODE's lattice value.  The value of a
   PHI node is determined using the following meet operator M:

   			any M UNDEFINED = any
			any M VARYING	= VARYING
			Ci  M Cj	= Ci		if (i == j)
			Ci  M Cj	= VARYING	if (i != j)  */

static void
visit_phi_node (phi_node)
     varref phi_node;
{
  unsigned int i;

  unsigned int phi_node_name = SSA_NAME (phi_node);
  latticevalue phi_node_lattice_val = UNDEFINED;
  varray_type phi_vec = PHI_PARMS (phi_node);
  unsigned int num_elem = VARRAY_ACTIVE_SIZE (phi_vec);
  tree phi_node_expr = NULL;

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_varref (dump_file, "\nVisiting PHI node: ", phi_node, 0, 0);

  for (i = 0; i < num_elem; i++)
    {
      varref ref;
      basic_block phiargbb, block;

      ref = (varref) VARRAY_GENERIC_PTR (phi_vec, i);
      phiargbb = VARRAY_BB (VARDEF_PHI_CHAIN_BB (phi_node), i);
      block = VARREF_BB (phi_node);

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\n    Examining argument #%d: ", i);
	  dump_varref (dump_file, "", ref, 0, 0);
	  fprintf (dump_file, "    incoming via basic block %d\n",
	           phiargbb->index);
	  fprintf (dump_file, "    Edge (%d -> %d) is %sexecutable\n",
	           phiargbb->index, block->index,
		   (TEST_BIT (executable_edges, EIE (phiargbb, block)))
		   ? ""
		   : "not ");
	}

      /* Compute the meet operator for the current PHI argument.  */
      if (TEST_BIT (executable_edges, EIE (phiargbb, block)))
	{
	  latticevalue current_parm_lattice_val;
	  unsigned int current_parm = SSA_NAME (ref);
	    
	  current_parm_lattice_val = values[current_parm].lattice_val;

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      dump_lattice_value (dump_file, "    argument's value: ",
				  values[current_parm]);
	      fprintf (dump_file, "\n");
	    }

	  /* If any node is VARYING, then new value of PHI_NODE
	      is VARYING.  */
	  if (current_parm_lattice_val == VARYING)
	    {
	      phi_node_lattice_val = VARYING;
	      phi_node_expr = NULL_TREE;
	      break;
	    }

	  /* If we have more than one distinct constant, then the new
	      value of PHI_NODE is VARYING.  */
	  if (current_parm_lattice_val == CONSTANT
	      && phi_node_lattice_val == CONSTANT
	      && !(simple_cst_equal (values[current_parm].const_value,
				     phi_node_expr) == 1))
	    {
	      phi_node_lattice_val = VARYING;
	      phi_node_expr = NULL_TREE;
	      break;
	    }

	  /* If the current value of PHI_NODE is UNDEFINED and one
	      node in PHI_NODE is CONSTANT, then the new value of the
	      PHI is that CONSTANT.  Note this can turn into VARYING
	      if we find another distinct constant later.  */ 
	  if (phi_node_lattice_val == UNDEFINED
	      && phi_node_expr == NULL
	      && current_parm_lattice_val == CONSTANT)
	    {
	      phi_node_lattice_val = CONSTANT;
	      phi_node_expr = values[current_parm].const_value;
	    }
	}
      else
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "    Rescheduling this PHI expression into the queue\n");

	  phi_node_lattice_val = UNDEFINED;
	  phi_node_expr = NULL_TREE;
	  break;
	}
    }

  /* If the value of PHI_NODE changed, then we will
     need to re-execute uses of the output of PHI_NODE.  */
  if (phi_node_lattice_val != values[phi_node_name].lattice_val)
    {
      values[phi_node_name].lattice_val = phi_node_lattice_val;
      values[phi_node_name].const_value = phi_node_expr;
      VARRAY_PUSH_GENERIC_PTR (ssa_edges, phi_node);
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      dump_lattice_value (dump_file, "\n    Lattice value for this PHI node: ",
			  values[phi_node_name]);
      fprintf (dump_file, "\n");
    }
}


/* Evaluate the expression associated with REF.  If the evaluation changes
   the lattice value of the expression, do the following:

   - If the expression is an assignment, add all the SSA edges starting at
     this definition.

   - If the expression controls a conditional branch:
   	. If the expression evaluates to non-constant, add all edges to
	  flow_edges.
	. If the expression is constant, add the edge executed as the
	  result of the branch.  */

static void
visit_expression (ref)
     varref ref;
{
  tree expr;

#if defined ENABLE_CHECKING
  /* PHI references should be dealt with by visit_phi_node.  */
  if (VARREF_TYPE (ref) == VARPHI)
    abort ();
#endif

  /* Special case for ghost definitions.  Set their lattice value to
     VARYING. */
  if (IS_GHOST_DEF (ref))
    {
      values[SSA_NAME (ref)].lattice_val = VARYING;
      return;
    }

  expr = VARREF_EXPR (ref);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nVisiting expression: ");
      print_c_node (dump_file, expr);
      dump_varref (dump_file, "\nfor reference: ", ref, 0, 0);
    }

  /* If REF is a definition in an assignment expression, visit the
     assignment to determine the lattice value for the variable.  */
  if (VARREF_TYPE (ref) == VARDEF)
    {
      if (TREE_CODE (expr) == MODIFY_EXPR || TREE_CODE (expr) == INIT_EXPR)
	visit_assignment (ref);
      else
	def_to_varying (ref);
    }

  /* If the expression is the predicate of a control statement, see if we
     can determine which branch will be taken.  */
  else if (VARREF_BB (ref)->flags & BB_CONTROL_EXPR)
    {
      edge curredge;
      value val;
      basic_block block = VARREF_BB (ref);
      
      val.lattice_val = UNDEFINED;
      val.const_value = NULL_TREE;

      /* If the expression is the predicate for the control statement,
	 evaluate it to see if we can determine whether the TRUE or FALSE
	 branch will be taken.  */
      if (is_simple_condexpr (expr))
	val = evaluate_expr_for (ref);

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Predicate is for a control statement: %s\n",
		   tree_code_name[TREE_CODE (VARREF_STMT (ref))]);
	  dump_lattice_value (dump_file, "value: ", val);
	  fprintf (dump_file, "\n");
	}

      /* Mark successor blocks on executable edges as executable (if they
	 have not already been marked).   */
      for (curredge = block->succ; curredge; curredge = curredge->succ_next)
	{
	  int index = EIE (curredge->src, curredge->dest);

	  /* If this is an edge for TRUE values but the predicate is false,
	     then skip it.  */
	  if ((curredge->flags & EDGE_TRUE_VALUE)
	      && simple_cst_equal (val.const_value, integer_zero_node) == 1)
	    continue;

	  /* Similarly for FALSE edges.  */
	  if ((curredge->flags & EDGE_FALSE_VALUE)
	      && simple_cst_equal (val.const_value, integer_one_node) == 1)
	    continue;

	  /* If the edge had already been added, skip it.  */
	  if (TEST_BIT (executable_edges, index))
	    continue;

	  SET_BIT (executable_edges, index);
	  edge_info[index] = flow_edges;
	  flow_edges = curredge;

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "Adding edge %d (%d -> %d) to worklist\n\n",
		     index, flow_edges->src->index, flow_edges->dest->index);
	}
    }
}


/* Visit an assignment statement for reference REF.  */

static void
visit_assignment (ref)
     varref ref;
{
  tree set, src, dest;
  value val;

  set = VARREF_EXPR (ref);
  src = TREE_OPERAND (set, 1);
  dest = TREE_OPERAND (set, 0);

  val = evaluate_expr_for (ref);

  if (val.lattice_val == UNDEFINED)
    def_to_undefined (ref);
  else if (val.lattice_val == VARYING)
    def_to_varying (ref);
  else if (TREE_ADDRESSABLE (dest)
	   || DECL_CONTEXT (dest) == NULL
	   || TREE_THIS_VOLATILE (dest))
    {
      /* Certain types of variables cannot be proven constant even if they
	 were assigned one.  Examples include: globals, volatiles and
	 variables that have had their address taken.

	 FIXME: Additional analysis might help us make a better decision in
		the future.  */
      def_to_varying (ref);
    }
  else
    {
      /* If the RHS is a constant value that is different from a previous
	 value for this reference, add its SSA edge to the work list.  */
      if (values[VARREF_ID (ref)].lattice_val != CONSTANT
	  || !(simple_cst_equal (values[VARREF_ID (ref)].const_value,
	                         val.const_value)) == 1)
	VARRAY_PUSH_GENERIC_PTR (ssa_edges, ref);

      values[VARREF_ID (ref)].lattice_val = CONSTANT;
      values[VARREF_ID (ref)].const_value = val.const_value;
    }
}


/* Iterate over the FLOW_EDGES work list.  Simulate the target block
   for each edge.  */

static void
examine_flow_edges (void)
{
  while (flow_edges != NULL)
    {
      basic_block succ_block;
      ref_list blockrefs;
      varref ref;
      struct ref_list_node *tmp;

      /* Pull the next block to simulate off the worklist.  */
      succ_block = flow_edges->dest;
      flow_edges = edge_info[EIE (flow_edges->src, flow_edges->dest)];

      /* There is nothing to do for the exit block.  */
      if (succ_block == EXIT_BLOCK_PTR)
	continue;

      blockrefs = BB_REFS (succ_block);
      if (!blockrefs)
	continue;

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "\nSimulating block %d\n", succ_block->index);

      /* Always simulate PHI nodes, even if we have simulated this block
	 before.  Note that all PHI nodes are consecutive within a block.  */
      FOR_EACH_REF (ref, tmp, blockrefs)
	{
	  if (VARREF_TYPE (ref) == VARPHI)
	    visit_phi_node (ref);
	}

#if defined ENABLE_CHECKING
      if (succ_block->index < 0 || succ_block->index > last_basic_block)
	abort ();
#endif

      /* If this is the first time we've simulated this block, then we
	 must simulate each of its insns.  */
      if (!TEST_BIT (executable_blocks, succ_block->index))
	{
	  edge succ_edge = succ_block->succ;

	  /* Note that we have simulated this block.  */
	  SET_BIT (executable_blocks, succ_block->index);

	  FOR_EACH_REF (ref, tmp, blockrefs)
	    {
	      /* Simulate each reference within the block.  */
	      if (VARREF_TYPE (ref) != VARPHI)
		visit_expression (ref);
	    } 

	  /* If we haven't looked at the next block, and it has a
	     single successor, add it onto the worklist.  This is because
	     if we only have one successor, we know it gets executed,
	     so we don't have to wait for cprop to tell us. */
	  if (succ_edge != NULL
	      && succ_edge->succ_next == NULL
	      && !TEST_BIT (executable_edges,
			    EIE (succ_edge->src, succ_edge->dest)))
	    {
	      int eix = EIE (succ_edge->src, succ_edge->dest);

	      SET_BIT (executable_edges, eix);
	      edge_info[eix] = flow_edges;
	      flow_edges = succ_edge;

	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "Adding edge %d (%d -> %d) to worklist\n\n",
			 eix, flow_edges->src->index, flow_edges->dest->index);
	    }
	}
    }
}


/* Follow the def-use chains for each definition on the worklist and
   simulate the uses of the definition.  */

static void
follow_def_use_chains ()
{
  /* Iterate over all the entries on the SSA_EDGES worklist.  */
  while (VARRAY_ACTIVE_SIZE (ssa_edges) > 0)
    {
      varref ref;

      /* Pick an entry off the worklist (it does not matter which
	 entry we pick).  */
      ref = (varref) VARRAY_TOP_GENERIC_PTR (ssa_edges);
      VARRAY_POP (ssa_edges);

      if (VARREF_TYPE (ref) == VARUSE
	  && TEST_BIT (executable_blocks, VARREF_BB (ref)->index))
	visit_expression (ref);
      else if (VARREF_TYPE (ref) == VARPHI
	       && TEST_BIT (executable_blocks, VARREF_BB (ref)->index))
	visit_phi_node (ref);
    }
}


/* Examine each edge to see if we were able to prove any were
   not executable. 

   If an edge is not executable, then we can remove its alternative
   in PHI nodes as the destination of the edge, we can simplify the
   conditional branch at the source of the edge, and we can remove
   the edge from the CFG.  Note we do not delete unreachable blocks
   yet as the DF analyzer can not deal with that yet.  */

static void
optimize_unexecutable_edges (edges, executable_edges)
     struct edge_list *edges ATTRIBUTE_UNUSED;
     sbitmap executable_edges ATTRIBUTE_UNUSED;
{
}
 

/* Perform substitution and folding.   */

static void
ssa_ccp_substitute_constants ()
{
  basic_block bb;

  /* Substitute constants.  */
  FOR_EACH_BB (bb)
    {
      varref ref;
      struct ref_list_node *tmp;

      FOR_EACH_REF (ref, tmp, BB_REFS (bb))
	{
	  varref rdef;
	  unsigned int id;
	  
	  if (VARREF_TYPE (ref) != VARUSE)
	    continue;

	  rdef = VARUSE_CHAIN (ref);
	  id = VARREF_ID (rdef);

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      dump_varref (dump_file, "Immediate reaching definition for ",
		           ref, 0, 0);
	      dump_varref (dump_file, ": ", rdef, 0, 0);
	      dump_lattice_value (dump_file, "Lattice value: ", values[id]);
	      fprintf (dump_file, "\n");
	      if (values[id].lattice_val == CONSTANT)
		{
		  fprintf (dump_file, "Marking expression ");
		  print_c_node (dump_file, VARREF_EXPR (ref));
		  fprintf (dump_file, " for folding\n");
		}
	      fprintf (dump_file, "\n");
	    }

	  if (values[id].lattice_val == CONSTANT)
	    {
#if defined ENABLE_CHECKING
	      if (values[id].const_value == NULL_TREE)
		abort ();
#endif

	      /* Replace the constant inside the expression and mark the
		 expression for folding.  */
	      replace_expr_in_tree (VARREF_EXPR (ref), VARREF_SYM (ref),
				    values[id].const_value);
	      TREE_FLAGS (VARREF_EXPR (ref)) |= TF_FOLD;
	    }
	}
    }

  /* Fold expressions.  */
  FOR_EACH_BB (bb)
    {
      varref ref;
      struct ref_list_node *tmp;

      FOR_EACH_REF (ref, tmp, BB_REFS (bb))
	{
	  tree stmt = VARREF_STMT (ref);
	  tree expr = VARREF_EXPR (ref);

	  if (VARREF_TYPE (ref) == VARUSE
	      && (TREE_FLAGS (VARREF_EXPR (ref)) & TF_FOLD))
	    {
	      /* Fold the expression and clean the fold bit.  */
	      replace_expr_in_tree (stmt, expr, fold (expr));
	      TREE_FLAGS (VARREF_EXPR (ref)) &= ~TF_FOLD;
	    }
	}
    }
}


/* Now find all unreachable basic blocks.  All the insns in those
   blocks are unreachable, so delete them and mark any necessary
   updates for the DF analyzer.  */

static void
ssa_ccp_df_delete_unreachable_insns ()
{
}


/* Set the lattice value for the variable defined by REF to UNDEFINED.  */

static void
def_to_undefined (ref)
     varref ref;
{
  if (VARREF_TYPE (ref) != VARDEF)
    return;

  if (values[SSA_NAME (ref)].lattice_val != UNDEFINED)
    VARRAY_PUSH_GENERIC_PTR (ssa_edges, ref);

  values[SSA_NAME (ref)].lattice_val = UNDEFINED;
}


/* Set the lattice value for the variable defined by REF to VARYING.  */

static void
def_to_varying (ref)
     varref ref;
{
  if (VARREF_TYPE (ref) != VARDEF)
    return;

  if (values[SSA_NAME (ref)].lattice_val != VARYING)
    VARRAY_PUSH_GENERIC_PTR (ssa_edges, ref);

  values[SSA_NAME (ref)].lattice_val = VARYING;
}

/* Evaluate the expression holding reference REF.  */

static value
evaluate_expr_for (ref)
     varref ref;
{
  value val;
  struct ref_list_node *tmp;
  varref r;
  tree expr, simplified;
  ref_list refs;

  /* Make a deep copy of the expression, but get the expression references
     first because deep copies do not copy the 'aux' field.  */
  refs = TREE_REFS (VARREF_STMT (ref));
  expr = deep_copy_node (VARREF_EXPR (ref));

  /* If any USE reference in the expression is known to be VARYING or
     UNDEFINED, then the expression is not a constant.  */
  val.lattice_val = VARYING;
  val.const_value = NULL_TREE;

  FOR_EACH_REF (r, tmp, refs)
    {
      unsigned int id;
      varref rdef;

      if (VARREF_TYPE (r) != VARUSE)
	continue;

      /* The lattice value of a USE reference is the value of its
	 immediately reaching definition.  */
      rdef = VARUSE_CHAIN (r);
      id = VARREF_ID (rdef);

      if (values[id].lattice_val == VARYING)
	{
	  val.lattice_val = VARYING;
	  val.const_value = NULL_TREE;

	  /* The reference is VARYING, therefore the expression is VARYING
	     regardless of any other values we may have found.  */
	  goto dont_fold;
	}
      else if (values[id].lattice_val == CONSTANT)
	{
#if defined ENABLE_CHECKING
	  if (values[id].const_value == NULL_TREE)
	    abort ();
#endif

	  /* The reference is a constant, substitute it into the
	     expression.  */
	  replace_expr_in_tree (expr, VARREF_SYM (r), values[id].const_value);
	}
    }

  /* Fold the expression and return its value.  Being a SIMPLE expression,
     EXPR can only be an assignment, an RHS or a conditional expression.

     In the case of an assignment, we are only interested in its RHS,
     so we strip it off here.  */
  if (TREE_CODE (expr) == MODIFY_EXPR || TREE_CODE (expr) == INIT_EXPR)
    expr = TREE_OPERAND (expr, 1);

  /* Fold the expression.  If it results in a constant, set the lattice
      value for the LHS of the assignment to CONSTANT.  */
  simplified = fold (expr);
  if (simplified && really_constant_p (simplified))
    {
      val.lattice_val = CONSTANT;
      val.const_value = simplified;
    }

dont_fold:
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Expression ");
      print_c_node (dump_file, VARREF_EXPR (ref));
      fprintf (dump_file, " evaluates to ");
      print_c_node (dump_file, expr);
      fprintf (dump_file, " which is ");
      if (val.lattice_val == CONSTANT)
	{
	  fprintf (dump_file, "constant ");
	  print_c_node (dump_file, simplified);
	}
      else if (val.lattice_val == VARYING)
	fprintf (dump_file, "not a constant");
      else if (val.lattice_val == UNDEFINED)
	fprintf (dump_file, "undefined");

      fprintf (dump_file, "\n");
    }

  return val;
}


/* Debugging dumps.  */

static void
dump_lattice_value (outf, prefix, val)
     FILE *outf;
     const char *prefix;
     value val;
{
  if (val.lattice_val == UNDEFINED)
    fprintf (outf, "%sUNDEFINED", prefix);
  else if (val.lattice_val == VARYING)
    fprintf (outf, "%sVARYING", prefix);
  else
    {
      fprintf (outf, "%sCONSTANT ", prefix);
      print_c_node (outf, val.const_value);
    }
}
