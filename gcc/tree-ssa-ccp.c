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

/* Array of control flow edges on the worklist.  */
static varray_type edge_info;

/* We need an control flow edge list to be able to get indexes easily.  */
static struct edge_list *edges;

/* Worklist of SSA edges which will need reexamination as their definition
   has changed.  SSA edges are def-use edges in the SSA web.  For each
   edge, we store the originating VARDEF/VARPHI reference D.  The destination
   nodes that need to be visited are accessed using VARDEF_IMM_USES (D).  */
ref_list ssa_edges;

static void initialize                 PARAMS ((void));
static void finalize                   PARAMS ((void));
static void visit_phi_node             PARAMS ((varref));
static void visit_expression_for       PARAMS ((varref));
static void visit_condexpr_for         PARAMS ((varref));
static void visit_assignment_for       PARAMS ((varref));
static void def_to_undefined           PARAMS ((varref));
static void def_to_varying             PARAMS ((varref));
static void set_lattice_value          PARAMS ((varref, value));
static void simulate_block             PARAMS ((basic_block));
static void simulate_def_use_chains    PARAMS ((varref));
static void optimize_unexecutable_edges PARAMS ((struct edge_list *));
static void ssa_ccp_substitute_constants PARAMS ((void));
static void ssa_ccp_df_delete_unreachable_insns PARAMS ((void));
static value evaluate_expr             PARAMS ((tree));
static void dump_lattice_value         PARAMS ((FILE *, const char *, value));
static tree widen_bitfield             PARAMS ((tree, tree, tree));

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
  tree fnbody;

  fnbody = COMPOUND_BODY (DECL_SAVED_TREE (fndecl));

#if defined ENABLE_CHECKING
  if (fnbody == NULL_TREE)
    abort ();
#endif

  initialize ();

  /* Iterate until the worklists are empty.  */
  while (VARRAY_ACTIVE_SIZE (edge_info) > 0 || ssa_edges->last != NULL)
    {
      if (VARRAY_ACTIVE_SIZE (edge_info) > 0)
	{
	  /* Pull the next block to simulate off the worklist.  */
	  basic_block dest_block;

	  dest_block = ((edge)VARRAY_TOP_GENERIC_PTR (edge_info))->dest;
	  VARRAY_POP (edge_info);
	  simulate_block (dest_block);
	}

      if (ssa_edges->last)
	{
	  /* Pull the next reference off the worklist.  The SSA edges
	     worklist stores the origination definition for each edge.  */
	  varref def = ssa_edges->last->ref;
	  remove_ref_from_list (ssa_edges, def);
	  simulate_def_use_chains (def);
	}
    }

  /* Now perform substitutions based on the known constant values.  */
  ssa_ccp_substitute_constants ();

  /* Remove unexecutable edges from the CFG and make appropriate
     adjustments to PHI nodes.  */
  optimize_unexecutable_edges (edges);

  /* Now cleanup any unreachable code.  */
  tree_cleanup_cfg ();

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

  finalize ();
}


/* Simulate the execution of BLOCK.  Evaluate the expression associated
   with each variable reference inside the block.  */

static void
simulate_block (block)
     basic_block block;
{
  ref_list blockrefs;
  varref ref;
  struct ref_list_node *tmp;

  /* There is nothing to do for the exit block.  */
  if (block == EXIT_BLOCK_PTR)
    return;

  /* Similarly, if the block contains no references, we have nothing to do.  */
  blockrefs = BB_REFS (block);
  if (blockrefs == NULL)
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nSimulating block %d\n", block->index);

  /* Always simulate PHI nodes, even if we have simulated this block
     before.  Note that all PHI nodes are consecutive within a block.  */
  FOR_EACH_REF (ref, tmp, blockrefs)
    {
      if (VARREF_TYPE (ref) == VARPHI)
	visit_phi_node (ref);
    }

#if defined ENABLE_CHECKING
  if (block->index < 0 || block->index > last_basic_block)
    abort ();
#endif

  /* If this is the first time we've simulated this block, then we
     must simulate each of its insns.  */
  if (!TEST_BIT (executable_blocks, block->index))
    {
      edge succ_edge = block->succ;

      /* Note that we have simulated this block.  */
      SET_BIT (executable_blocks, block->index);

      FOR_EACH_REF (ref, tmp, blockrefs)
	{
	  /* Simulate each reference within the block.  */
	  if (VARREF_TYPE (ref) != VARPHI)
	    visit_expression_for (ref);
	} 

      /* If we haven't looked at the next block, and it has a
	 single successor, add it onto the worklist.  This is because
	 if we only have one successor, we know it gets executed,
	 so we don't have to wait for cprop to tell us. */
      if (succ_edge != NULL
	  && succ_edge->succ_next == NULL
	  && !(succ_edge->flags & EDGE_EXECUTABLE))
	{
	  succ_edge->flags |= EDGE_EXECUTABLE;
	  VARRAY_PUSH_GENERIC_PTR (edge_info, succ_edge);

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "Adding edge (%d -> %d) to worklist\n\n",
		      succ_edge->src->index, succ_edge->dest->index);
	}
    }
}


/* Follow the def-use chains for definition DEF and simulate all the
   expressions reached by it.  */

static void
simulate_def_use_chains (def)
     varref def;
{
  varref ref;
  struct ref_list_node *tmp;

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_varref (dump_file, "\nSimulating def-use edges for definition: ",
		 def, 0, 0);

  FOR_EACH_REF (ref, tmp, VARDEF_IMM_USES (def))
    {
      if (VARREF_TYPE (ref) == VARUSE
	  && TEST_BIT (executable_blocks, VARREF_BB (ref)->index))
	visit_expression_for (ref);

      /* PHI nodes are always visited, regardless of whether or not the
	 destination block is executable.  */
      else if (VARREF_TYPE (ref) == VARPHI)
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
optimize_unexecutable_edges (edges)
     struct edge_list *edges ATTRIBUTE_UNUSED;
{
  int i;

  for (i = 0; i < NUM_EDGES (edges); i++)
    {
      edge edge = INDEX_EDGE (edges, i);

      if (! (edge->flags & EDGE_EXECUTABLE))
	{
	  if (edge->flags & EDGE_ABNORMAL)
	    continue;

	  /* We found an edge that is not executable.  First simplify
	     the PHI nodes in the target block.  This may make 
	     simplifications possible in other optimizers.  */
	  if (edge->dest != EXIT_BLOCK_PTR)
	    {
	      ref_list blockrefs = BB_REFS (edge->dest);
	      varref ref;
	      struct ref_list_node *tmp;

	      FOR_EACH_REF (ref, tmp, blockrefs)
		{
		  if (VARREF_TYPE (ref) == VARPHI)
		    tree_ssa_remove_phi_alternative (ref, edge->src);
		}
	    }

	  /* Since the edge was not executable, remove it from the CFG.  */
	  remove_edge (edge);
	}
    }
}
 

/* Perform substitution and folding.   */

static void
ssa_ccp_substitute_constants ()
{
  basic_block bb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nSubstituing constants and folding expressions\n\n");

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

	  rdef = VARUSE_IMM_RDEF (ref);
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
	      *(VARREF_OPERAND_P (ref)) = values[id].const_value;
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
	      TREE_FLAGS (VARREF_EXPR (ref)) &= ~TF_FOLD;
	      
	      if (TREE_CODE (expr) == MODIFY_EXPR
		  || TREE_CODE (expr) == INIT_EXPR)
		expr = TREE_OPERAND (expr, 1);

	      replace_expr_in_tree (stmt, expr, fold (expr));
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

  unsigned int phi_node_name = VARREF_ID (phi_node);
  latticevalue phi_node_lattice_val = UNDEFINED;
  unsigned int num_elem = get_num_phi_args (phi_node);
  tree phi_node_expr = NULL;

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_varref (dump_file, "\nVisiting PHI node: ", phi_node, 0, 0);

  for (i = 0; i < num_elem; i++)
    {
      phi_arg arg;

      arg = get_phi_arg (phi_node, i);

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\n    Examining argument #%d: ", i);
	  dump_varref (dump_file, "", arg->def, 0, 0);
	  fprintf (dump_file, "    incoming via basic block %d\n",
	           arg->e->src->index);
	  fprintf (dump_file, "    Edge (%d -> %d) is %sexecutable\n",
	           arg->e->src->index, arg->e->dest->index,
		   (arg->e->flags & EDGE_EXECUTABLE)
		   ? ""
		   : "not ");
	}

      /* Compute the meet operator for the current PHI argument.  */
      if (arg->e->flags & EDGE_EXECUTABLE)
	{
	  latticevalue current_parm_lattice_val;
	  unsigned int current_parm = VARREF_ID (arg->def);
	    
	  current_parm_lattice_val = values[current_parm].lattice_val;

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      dump_lattice_value (dump_file, "    argument's value: ",
				  values[current_parm]);
	      fprintf (dump_file, "\n");
	    }

	  /* If any parameter is VARYING, then the new value of PHI_NODE
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
    }

  /* If the value of PHI_NODE changed, then we will need to re-execute uses of
     the output of PHI_NODE.  */
  if (phi_node_lattice_val != values[phi_node_name].lattice_val)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "    Lattice value changed.  Adding PHI node to SSA edges.\n");

      values[phi_node_name].lattice_val = phi_node_lattice_val;
      values[phi_node_name].const_value = phi_node_expr;
      add_ref_to_list_end (ssa_edges, phi_node);
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
	  worklist.
	. If the expression is constant, add the edge executed as the
	  result of the branch.  */

static void
visit_expression_for (ref)
     varref ref;
{
  tree expr;

#if defined ENABLE_CHECKING
  /* PHI references should be handled by visit_phi_node.  */
  if (VARREF_TYPE (ref) == VARPHI)
    abort ();
#endif

  expr = VARREF_EXPR (ref);
  if (expr == NULL)
    {
      /* If this is a definition with no associated expression, clobber the
	 reference.  Except if this is a default definition for a local
	 variable (which should be considered UNDEFINED, not VARYING).
	 
	 Note that we must clobber PARM_DECLs here.  Their incoming value
	 may not change, but we don't know what it is.  */
      if (IS_DEFAULT_DEF (ref) && TREE_CODE (VARREF_SYM (ref)) != PARM_DECL)
	def_to_undefined (ref);
      else
	def_to_varying (ref);

      return;
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nVisiting expression: ");
      print_c_node (dump_file, expr);
      dump_varref (dump_file, "\nfor reference: ", ref, 0, 0);
    }

  /* If the expression is an assignment, evaluate its RHS to determine the
     lattice value for the reference at the LHS of the assignment.  */
  if (TREE_CODE (expr) == MODIFY_EXPR || TREE_CODE (expr) == INIT_EXPR)
    {
      if (VARREF_TYPE (ref) == VARDEF
	  && ref != TREE_CURRDEF (expr))
	def_to_varying (ref);
      else
	visit_assignment_for (TREE_CURRDEF (expr));
    }

  /* If the expression is the predicate of a control statement, see if we
     can determine which branch will be taken.  */
  else if (VARREF_BB (ref)->flags & BB_CONTROL_EXPR)
    visit_condexpr_for (ref);

  /* If this reference is the declaration of a static variable, set its
     lattice value to VARYING.  Static initializers are only executed the
     first time the function is called.  We could assume them constant if
     there are no other definitions to the variable in the function, but
     that hardly seems worth it.  */
  else if (VARREF_TYPE (ref) == VARDEF
           && TREE_CODE (VARREF_STMT (ref)) == DECL_STMT
           && TREE_STATIC (DECL_STMT_DECL (VARREF_STMT (ref))))
    {
      value val;
      tree decl = DECL_STMT_DECL (VARREF_STMT (ref));

      val.lattice_val = VARYING;
      val.const_value = NULL;

      /* Exception.  If this is a 'const' declaration, use the value.  */
      if (TREE_READONLY (decl) && really_constant_p (DECL_INITIAL (decl)))
	{
	  val.lattice_val = CONSTANT;
	  val.const_value = DECL_INITIAL (VARREF_SYM (ref));
	}

      set_lattice_value (ref, val);
    }

  /* Any other definition that we do not recognize clobbers the variable.  */
  else if (VARREF_TYPE (ref) == VARDEF)
    def_to_varying (ref);
}


/* Visit an assignment expression for DEF.  This function evaluates the RHS
   of the expression holding DEF.  If it's found to be constant, it
   modifies DEF's lattice value.  */

static void
visit_assignment_for (def)
     varref def;
{
  tree assignment, lhs;
  value val;

  if (def == NULL)
    return;

  assignment = VARREF_EXPR (def);
  lhs = TREE_OPERAND (assignment, 0);

  val = evaluate_expr (assignment);

  /* FIXME: Hack.  If this was a definition of a bitfield, we need to
	    widen the constant value into the type of the destination
	    variable.  This should not be necessary if GCC represented
	    bitfields properly.  */
  if (val.lattice_val == CONSTANT
      && TREE_CODE (lhs) == COMPONENT_REF
      && DECL_BIT_FIELD (TREE_OPERAND (lhs, 1)))
    {
      tree w = widen_bitfield (val.const_value, TREE_OPERAND (lhs, 1), lhs);

      if (w)
	val.const_value = w;
      else
	{
	  val.lattice_val = VARYING;
	  val.const_value = NULL;
	}
    }

  set_lattice_value (def, val);
}


static void
visit_condexpr_for (ref)
     varref ref;
{
  edge curredge;
  value val;
  basic_block block = VARREF_BB (ref);
  tree expr = VARREF_EXPR (ref);

  val.lattice_val = UNDEFINED;
  val.const_value = NULL_TREE;

  /* If the expression is the predicate for the control statement,
     evaluate it to see if we can determine whether the TRUE or FALSE
     branch will be taken.  */
  if (is_simple_condexpr (expr))
    val = evaluate_expr (expr);

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
      int index;

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
      if (curredge->flags & EDGE_EXECUTABLE)
	continue;

      curredge->flags |= EDGE_EXECUTABLE;
      VARRAY_PUSH_GENERIC_PTR (edge_info, curredge);

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Adding edge %d (%d -> %d) to worklist\n\n",
	    index, curredge->src->index, curredge->dest->index);
    }
}

/* Evaluate the expression EXPR.  */

static value
evaluate_expr (expr)
     tree expr;
{
  value val;
  struct ref_list_node *tmp;
  varref r;
  tree simplified;
  ref_list refs;

  refs = TREE_REFS (expr);

  val.lattice_val = VARYING;
  val.const_value = NULL_TREE;

  /* If any USE reference in the expression is known to be VARYING or
     UNDEFINED, then the expression is not a constant.  */
  FOR_EACH_REF (r, tmp, refs)
    {
      unsigned int id;
      varref rdef;

      if (VARREF_TYPE (r) != VARUSE)
	continue;

      /* The lattice value of a USE reference is the value of its
	 immediately reaching definition.  */
      rdef = VARUSE_IMM_RDEF (r);
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
	  *(VARREF_OPERAND_P (r)) = values[id].const_value;
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
  simplified = fold (deep_copy_node (expr));
  if (simplified && really_constant_p (simplified))
    {
      val.lattice_val = CONSTANT;
      val.const_value = simplified;
    }

dont_fold:
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Expression ");
      print_c_node (dump_file, expr);
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

  /* Restore the expression to its original form.  */
  FOR_EACH_REF (r, tmp, refs)
    {
      if (VARREF_TYPE (r) == VARUSE)
	*(VARREF_OPERAND_P (r)) = VARREF_SYM (r);
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

/* Given a constant value VAL for bitfield FIELD, and a destination
   variable VAR, return VAL appropriately widened to fit into VAR.  If
   FIELD is wider than HOST_WIDE_INT, NULL is returned.  */

static tree
widen_bitfield (val, field, var)
     tree val;
     tree field;
     tree var;
{
  unsigned var_size, field_size;
  tree wide_val;
  unsigned HOST_WIDE_INT mask;
  unsigned i;

  var_size = TREE_INT_CST_LOW (TYPE_SIZE (TREE_TYPE ((var))));
  field_size = TREE_INT_CST_LOW (DECL_SIZE (field));

  /* Give up if either the bitfield or the variable are too wide.  */
  if (field_size > HOST_BITS_PER_WIDE_INT || var_size > HOST_BITS_PER_WIDE_INT)
    return NULL;

#ifdef ENABLE_CHECKING
  if (var_size < field_size)
    abort ();
#endif

  /* If the sign bit of the value is not set, return it unmodified.  */
  if ((TREE_INT_CST_LOW (val) & (1 << (field_size - 1))) == 0)
    return val;

  if (TREE_UNSIGNED (field))
    {
      /* Zero extension.  Build a mask with the lower 'field_size' bits
	 set and a BIT_AND_EXPR node to clear the high order bits of
	 the value.  */
      for (i = 0, mask = 0; i < field_size; i++)
	mask |= 1 << i;

      wide_val = build (BIT_AND_EXPR, TREE_TYPE (var), val, 
			build_int_2 (mask, 0));
    }
  else
    {
      /* Sign extension.  Create a mask with the upper 'field_size'
	 bits set and a BIT_IOR_EXPR to set the high order bits of the
	 value.  */
      for (i = 0, mask = 0; i < (var_size - field_size); i++)
	mask |= 1 << (var_size - i - 1);

      wide_val = build (BIT_IOR_EXPR, TREE_TYPE (var), val,
			build_int_2 (mask, 0));
    }

  return fold (wide_val);
}


/* Initialize local data structures and worklists for CCP.  */

static void
initialize ()
{
  unsigned i;
  edge curredge;

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

  /* Worklist of SSA edges.  */
  ssa_edges = create_ref_list ();

  executable_blocks = sbitmap_alloc (last_basic_block);
  sbitmap_zero (executable_blocks);

  for (i = 0; i < (unsigned)(NUM_EDGES (edges)); i++)
    edges->index_to_edge[i]->flags &= ~EDGE_EXECUTABLE;

  VARRAY_GENERIC_PTR_INIT (edge_info, NUM_EDGES (edges) / 2, "edge_info");

  /* Add the successors of the entry block to the edge worklist.  That
     is enough of a seed to get SSA-CCP started.  */
  for (curredge = ENTRY_BLOCK_PTR->succ; curredge;
       curredge = curredge->succ_next)
    {
      curredge->flags |= EDGE_EXECUTABLE;
      VARRAY_PUSH_GENERIC_PTR (edge_info, curredge);
    }
}


/* Free allocated storage.  */

static void
finalize ()
{
  free (values);
  values = NULL;

  free_edge_list (edges);
  edges = NULL;
}

/* Set the lattice value for the variable defined by REF to UNDEFINED.  */

static void
def_to_undefined (ref)
     varref ref;
{
  if (VARREF_TYPE (ref) != VARDEF)
    return;

  if (values[VARREF_ID (ref)].lattice_val != UNDEFINED)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Lattice value changed.  Adding definition to SSA edges.\n");

      add_ref_to_list_end (ssa_edges, ref);
    }

  values[VARREF_ID (ref)].lattice_val = UNDEFINED;
}


/* Set the lattice value for the variable defined by REF to VARYING.  */

static void
def_to_varying (ref)
     varref ref;
{
  if (VARREF_TYPE (ref) != VARDEF)
    return;

  if (values[VARREF_ID (ref)].lattice_val != VARYING)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Lattice value changed.  Adding definition to SSA edges.\n");

      add_ref_to_list_end (ssa_edges, ref);
    }

  values[VARREF_ID (ref)].lattice_val = VARYING;
}


/* Set the lattice value for reference DEF to VAL.  */

static void
set_lattice_value (def, val)
     varref def;
     value val;
{
  tree base_sym;

  base_sym = get_base_symbol (VARREF_SYM (def));

  /* Certain types of variables cannot be proven constant even if they
     were assigned one.  Examples include: globals, volatiles and
     variables that have had their address taken.

     FIXME: Additional analysis might help us make a better decision in
	    the future.  */
  if (base_sym
      && (TREE_ADDRESSABLE (base_sym)
	  || DECL_CONTEXT (base_sym) == NULL
	  || TREE_THIS_VOLATILE (base_sym)))
    {
      val.lattice_val = VARYING;
      val.const_value = NULL;
    }

  if (val.lattice_val == UNDEFINED)
    def_to_undefined (def);
  else if (val.lattice_val == VARYING)
    def_to_varying (def);
  else
    {
      /* If the RHS is a constant value that is different from a previous
	 value for this reference, add its SSA edge to the worklist.  */
      if (values[VARREF_ID (def)].lattice_val != CONSTANT
	  || !(simple_cst_equal (values[VARREF_ID (def)].const_value,
	                         val.const_value)) == 1)
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Lattice value changed to ");
	      print_c_node (dump_file, val.const_value);
	      fprintf (dump_file, ".  Adding definition to SSA edges.\n");
	    }

	  add_ref_to_list_end (ssa_edges, def);
	  values[VARREF_ID (def)].lattice_val = CONSTANT;
	  values[VARREF_ID (def)].const_value = val.const_value;
	}
    }
}
