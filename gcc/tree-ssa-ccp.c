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

/* Main structure for CCP. 

   Contains the lattice value and, if it's a constant, the constant
   value.  */
typedef struct
{
  latticevalue lattice_val;
  tree const_value;
} value;

/* Array of values indexed by register number.  */
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

/* Bitmap of SSA edges which will need reexamination as their definition
   has changed.  */
static sbitmap ssa_edges;
static varray_type decl_map = NULL;

/* Simple macros to simplify code */
#define SSA_NAME(x) DECL_UID (VARREF_SYM (x))
#define PHI_PARMS(x) VARDEF_PHI_CHAIN (x)
#define EIE(x,y) EDGE_INDEX (edges, x, y)

static void visit_phi_node             PARAMS ((varref, basic_block));
static void visit_expression           PARAMS ((varref, basic_block));
static void def_to_undefined           PARAMS ((varref));
static void def_to_varying             PARAMS ((varref));
static void examine_flow_edges         PARAMS ((void));
static void follow_def_use_chains      PARAMS ((void));
static void optimize_unexecutable_edges PARAMS ((struct edge_list *, sbitmap));
static void ssa_ccp_substitute_constants PARAMS ((void));
static void ssa_ccp_df_delete_unreachable_insns PARAMS ((void));

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
  int i;
  unsigned int j;
  edge curredge;
  tree fnbody;

  fnbody = COMPOUND_BODY (DECL_SAVED_TREE (fndecl));
  if (fnbody == NULL_TREE)
    abort ();

  /* Debugging dumps.  */
  dump_file = dump_begin (TDI_ccp, &dump_flags);
  if (dump_file)
    {
      fprintf (dump_file, "\n%s()    (ORIGINAL)\n",
	       IDENTIFIER_POINTER (DECL_NAME (fndecl)));

      if (dump_flags & TDF_RAW)
	dump_node (fnbody, TDF_SLIM | dump_flags, dump_file);
      else
	print_c_tree (dump_file, fnbody);
    }

  VARRAY_TREE_INIT (decl_map, next_decl_uid, "Decl map");
  for (j = 0; j < NREF_SYMBOLS; j++)
    VARRAY_TREE (decl_map, DECL_UID (REF_SYMBOL (j))) =  REF_SYMBOL (j);
  /* Build an edge list from the CFG.  */
  edges = create_edge_list ();

  /* Initialize the values array with everything as undefined.  */
  values = (value *) xmalloc (next_decl_uid * sizeof (value));
  for (i = 0; i < next_decl_uid; i++)
    {
      values[i].lattice_val = UNDEFINED;
      values[i].const_value = NULL;
    }

  ssa_edges = sbitmap_alloc (next_decl_uid);
  sbitmap_zero (ssa_edges);

  executable_blocks = sbitmap_alloc (n_basic_blocks);
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

  /* Iterate until until the worklists are empty.  */
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

#if 0
  /* The DF analyzer expects the number of blocks to remain constant,
     so we can't remove unreachable blocks.

     Code the DF analyzer calls expects there to be no unreachable
     blocks in the CFG.  So we can't leave unreachable blocks in the
     CFG.

     So, there is no way to do an incremental update of the DF data
     at this point.  */
  df_analyse (df_analyzer, 0,
	      DF_RD_CHAIN | DF_RU_CHAIN | DF_REG_INFO | DF_HARD_REGS);
#endif

  /* Debugging dumps.  */
  if (dump_file)
    {
      fprintf (dump_file, "\n%s()    (OPTIMIZED)\n",
	       IDENTIFIER_POINTER (DECL_NAME (fndecl)));

      if (dump_flags & TDF_RAW)
	dump_node (fnbody, TDF_SLIM | dump_flags, dump_file);
      else
	print_c_tree (dump_file, fnbody);
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
   lattice values to determine PHI_NODE's lattice value.  */

static void
visit_phi_node (phi_node, block)
     varref phi_node;
     basic_block block;
{
  unsigned int i;
  unsigned int phi_node_name = SSA_NAME (phi_node);
  latticevalue phi_node_lattice_val = UNDEFINED;
  varray_type phi_vec = PHI_PARMS (phi_node);
  unsigned int num_elem = VARRAY_ACTIVE_SIZE (phi_vec);
  tree phi_node_expr = NULL;
  for (i = 0; i < num_elem; i ++)
    {
      varref ref = (varref) VARRAY_GENERIC_PTR (phi_vec, i);
      if (VARREF_TYPE (ref) == VARDEF)
	{
	  if (TEST_BIT (executable_edges,
			EIE (VARREF_BB (ref), block)))
	      {
	  unsigned int current_parm
	    = SSA_NAME (ref);

	  latticevalue current_parm_lattice_val
	    = values[current_parm].lattice_val;

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
	      && ! (simple_cst_equal (values[current_parm].const_value,
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
	      phi_node_expr = values[current_parm].const_value;
	      phi_node_lattice_val = CONSTANT;
	      continue;
	    }
	}
    }
    }
  /* If the value of PHI_NODE changed, then we will need to
     re-execute uses of the output of PHI_NODE.  */
  if (phi_node_lattice_val != values[phi_node_name].lattice_val)
    {
      values[phi_node_name].lattice_val = phi_node_lattice_val;
      values[phi_node_name].const_value = phi_node_expr;
      SET_BIT (ssa_edges, phi_node_name);
    }
}


/* Sets all defs in an insn to UNDEFINED.  */

static void
def_to_undefined (ref)
     varref ref;
{
  if (VARREF_TYPE (ref) != VARDEF)
    return;

  if (values[SSA_NAME (ref)].lattice_val != UNDEFINED)
    SET_BIT (ssa_edges, SSA_NAME (ref));
  values[SSA_NAME (ref)].lattice_val = UNDEFINED;
}


/* Sets all defs in an insn to VARYING.  */

static void
def_to_varying (ref)
     varref ref;
{
  if (VARREF_TYPE (ref) != VARDEF)
    return;

  if (values[SSA_NAME (ref)].lattice_val != VARYING)
    SET_BIT (ssa_edges, SSA_NAME (ref));
  values[SSA_NAME (ref)].lattice_val = VARYING;
}


/* Go through the expression, call the approriate evaluation routines
   to attempt cprop */

static void
visit_expression (insn, block)
     varref insn;
     basic_block block;
{
  tree src, dest, set;
  set = VARREF_EXPR (insn);
  switch (TREE_CODE (set))
   {
   case MODIFY_EXPR:
   case INIT_EXPR:
     break;
   default:
     /* FIXME: Errr, this is pointless, we only need to reset the single
	def.  */
      def_to_varying (insn);
      return;
    }
  src = TREE_OPERAND (set, 1);
  dest = TREE_OPERAND (set, 0);

  /* If this is assigning DEST to a constant, record that fact.  */
  if (TREE_CODE (dest) == VAR_DECL && really_constant_p (src))
    {
      unsigned int resultreg = DECL_UID (dest);

      values[resultreg].lattice_val = CONSTANT;
      values[resultreg].const_value = src;
      SET_BIT (ssa_edges, resultreg);
    }

  /* If this is a copy operation, then we can copy the lattice values.  */
  else if (TREE_CODE (src) == VAR_DECL && TREE_CODE (dest) == VAR_DECL)
    {
      unsigned int old_value = DECL_UID (src);
      latticevalue old_lattice_value = values[old_value].lattice_val;
      unsigned int new_value = DECL_UID (dest);

      /* Unless the lattice value is going to change, don't bother
         adding the "new value" into the worklist.  */
      if (values[new_value].lattice_val != old_lattice_value
	  || !(simple_cst_equal (values[new_value].const_value,
	                         values[old_value].const_value) == 1))
	SET_BIT (ssa_edges, new_value);

      /* Copy the old lattice node info into the new value lattice node.  */
      values[new_value].lattice_val = old_lattice_value;
      values[new_value].const_value = values[old_value].const_value;
    }

  /* Handle jumps.  */
  else if (is_ctrl_stmt (set))
    {
      edge curredge;

      /* This is a computed jump, table jump, or an unconditional
	 jump.  For all these cases we want to mark all successor
	 blocks as executable if they have not already been
	 marked.

	 One day we may try do better with swtich tables and
	 other computed jumps.  */
      for (curredge = block->succ; curredge;
	   curredge = curredge->succ_next)
	{
	  int index = EIE (curredge->src, curredge->dest);

	  if (TEST_BIT (executable_edges, index))
	    continue;

	  SET_BIT (executable_edges, index);
	  edge_info[index] = flow_edges;
	  flow_edges = curredge;
	}
    }
#if 0
      if (GET_CODE (src) != IF_THEN_ELSE)
	{
	  edge curredge;

	  /* This is a computed jump, table jump, or an unconditional
	     jump.  For all these cases we want to mark all successor
	     blocks as executable if they have not already been
	     marked.

	     One day we may try do better with swtich tables and
	     other computed jumps.  */
	  for (curredge = block->succ; curredge;
	       curredge = curredge->succ_next)
	    {
	      int index = EIE (curredge->src, curredge->dest);

	      if (TEST_BIT (executable_edges, index))
		continue;

	      SET_BIT (executable_edges, index);
	      edge_info[index] = flow_edges;
	      flow_edges = curredge;
	    }
	}
      else
	{
	  edge curredge;
	  enum rtx_code comparison_code;
	  rtx comparison_src0;
	  rtx comparison_src1;

	  comparison_code = GET_CODE (XEXP (src, 0));
	  comparison_src0 = XEXP (XEXP (src, 0), 0);
	  comparison_src1 = XEXP (XEXP (src, 0), 1);

	  /* If either operand is undefined, then there is nothing to
	     do right now.  If/when operands are later defined we will
	     revaluate the condition and take the appropriate action.  */
	  if ((GET_CODE (comparison_src0) == REG
	       && values[REGNO (comparison_src0)].lattice_val == UNDEFINED)
	      || (GET_CODE (comparison_src1) == REG
	          && values[REGNO (comparison_src1)].lattice_val == UNDEFINED))
	    return;

	  /* If either operand is varying, then we must consider all
	     paths as executable.  */
	  if ((GET_CODE (comparison_src0) == REG
	       && values[REGNO (comparison_src0)].lattice_val == VARYING)
	      || (GET_CODE (comparison_src1) == REG
	          && values[REGNO (comparison_src1)].lattice_val == VARYING))
	    {
	      for (curredge = block->succ; curredge;
	           curredge = curredge->succ_next)
	        {
	          int index = EIE (curredge->src, curredge->dest);

	          if (TEST_BIT (executable_edges, index))
		    continue;

	          SET_BIT (executable_edges, index);
	          edge_info[index] = flow_edges;
	          flow_edges = curredge;
	        }
	      return;
	    }

	  /* Try to simplify the comparison.  */
	  if (GET_CODE (comparison_src0) == REG
	      && values[REGNO (comparison_src0)].lattice_val == CONSTANT)
	    comparison_src0 = values[REGNO (comparison_src0)].const_value;

	  if (GET_CODE (comparison_src1) == REG
	      && values[REGNO (comparison_src1)].lattice_val == CONSTANT)
	    comparison_src1 = values[REGNO (comparison_src1)].const_value;

	  x = simplify_ternary_operation (IF_THEN_ELSE,
					  VOIDmode,
					  GET_MODE (XEXP (src, 0)),
					  gen_rtx (comparison_code,
						   GET_MODE (XEXP (src, 0)),
						   comparison_src0,
						   comparison_src1),
					  XEXP (src, 1),
					  XEXP (src, 2));

	  /* Walk through all the outgoing edges from this block and see
	     which (if any) we should mark as executable.  */
	  for (curredge = block->succ; curredge;
	       curredge = curredge->succ_next)
	    {
	      int index = EIE (curredge->src, curredge->dest);

	      if (TEST_BIT (executable_edges, index))
		continue;

	      /* If we were unable to simplify the expression at this
		 point, it's highly unlikely we'll be able to simplify
		 it later.  So consider all edges as executable if the
		 expression did not simplify.

		 If the expression simplified to (pc), then we know we
		 will take the fall-thru edge, so mark it.  Similarly,
		 if the expression simplified to (label_ref ...), then
		 we know the branch will be taken and we mark that
		 edge as taken.  */
	      if (!x
		  || (x == pc_rtx
		      && (curredge->flags & EDGE_FALLTHRU))
		  || (GET_CODE (x) == LABEL_REF
		      && ! (curredge->flags & EDGE_FALLTHRU)))
		{
		  SET_BIT (executable_edges, index);
		  edge_info[index] = flow_edges;
		  flow_edges = curredge;
		}
	    }
	}
#endif
  else if (!(VARREF_TYPE (insn) == VARPHI))
    {
      tree simplified = NULL;

      /* We've got some kind of INSN.  If it's simple, try to evaluate
	 it and record the results. 

	 We already know this insn is a single_set and that it sets
	 a pseudo register.   So we just need to extract the source
	 arguments, simplify them to constants if possible, then
	 simplify the expression as a whole if possible.  */
      switch (TREE_CODE_CLASS (TREE_CODE (src)))
	{
	case '2':
	case 'c':
	  case '<':
	    {
	      tree src0 = TREE_OPERAND (src, 0);
	      tree src1 = TREE_OPERAND (src, 1);
              tree src0new = src0;
	      tree src1new = src1;
	      /* If either is undefined, then the result is undefined.  */
	      if ((TREE_CODE (src0) == VAR_DECL
		   && values[DECL_UID (src0)].lattice_val == UNDEFINED)
		  || (TREE_CODE (src1) == VAR_DECL
		      && values[DECL_UID (src1)].lattice_val == UNDEFINED))
		{
		  def_to_undefined (insn);
		  break;
		}
		
	      /* Simplify source operands to whatever known values they
		 may have.  */
	      if (TREE_CODE (src0) == VAR_DECL
		  && values[DECL_UID (src0)].lattice_val == CONSTANT)
		src0new = values[DECL_UID (src0)].const_value;

	      if (TREE_CODE (src1) == VAR_DECL
		  && values[DECL_UID (src1)].lattice_val == CONSTANT)
		src1new = values[DECL_UID (src1)].const_value;

	      simplified = eval_subst (src, src0, src0new, src1, src1new);
	      break;

	    }
#if 0
	  case '1':
	    {
	      rtx src0 = XEXP (src, 0);

	      /* If the operand is undefined, then the result is undefined.  */
	      if (GET_CODE (src0) == REG
		   && values[REGNO (src0)].lattice_val == UNDEFINED)
		{
		  defs_to_undefined (insn);
		  break;
		}
		
	      /* Simplify source operands to whatever known values they
		 may have.  */
	      if (GET_CODE (src0) == REG
		  && values[REGNO (src0)].lattice_val == CONSTANT)
		src0 = values[REGNO (src0)].const_value;

	      /* See if the simplifier can determine if this operation
		 computes a constant value.  */
	      simplified = simplify_unary_operation (GET_CODE (src),
						     GET_MODE (src),
						     src0,
						     GET_MODE (src0));
	      break;
	    }
	  case '2':
	  case 'c':
	    {
	      rtx src0 = XEXP (src, 0);
	      rtx src1 = XEXP (src, 1);

	      /* If either is undefined, then the result is undefined.  */
	      if ((GET_CODE (src0) == REG
		   && values[REGNO (src0)].lattice_val == UNDEFINED)
		  || (GET_CODE (src1) == REG
		      && values[REGNO (src1)].lattice_val == UNDEFINED))
		{
		  defs_to_undefined (insn);
		  break;
		}
		
	      /* Simplify source operands to whatever known values they
		 may have.  */
	      if (GET_CODE (src0) == REG
		  && values[REGNO (src0)].lattice_val == CONSTANT)
		src0 = values[REGNO (src0)].const_value;

	      if (GET_CODE (src1) == REG
		  && values[REGNO (src1)].lattice_val == CONSTANT)
		src1 = values[REGNO (src1)].const_value;

	      /* See if the simplifier can determine if this operation
		 computes a constant value.  */
	      simplified = simplify_binary_operation (GET_CODE (src),
						      GET_MODE (src),
						      src0, src1);
	      break;
	    }

	  case '3':
	  case 'b':
	    {
	      rtx src0 = XEXP (src, 0);
	      rtx src1 = XEXP (src, 1);
	      rtx src2 = XEXP (src, 2);

	      /* If either is undefined, then the result is undefined.  */
	      if ((GET_CODE (src0) == REG
		   && values[REGNO (src0)].lattice_val == UNDEFINED)
		  || (GET_CODE (src1) == REG
		      && values[REGNO (src1)].lattice_val == UNDEFINED)
		  || (GET_CODE (src2) == REG
		      && values[REGNO (src2)].lattice_val == UNDEFINED))
		{
		  defs_to_undefined (insn);
		  break;
		}
		
	      /* Simplify source operands to whatever known values they
		 may have.  */
	      if (GET_CODE (src0) == REG
		  && values[REGNO (src0)].lattice_val == CONSTANT)
		src0 = values[REGNO (src0)].const_value;

	      if (GET_CODE (src1) == REG
		  && values[REGNO (src1)].lattice_val == CONSTANT)
		src1 = values[REGNO (src1)].const_value;

	      if (GET_CODE (src2) == REG
		  && values[REGNO (src2)].lattice_val == CONSTANT)
		src2 = values[REGNO (src2)].const_value;

	      /* See if the simplifier can determine if this operation
		 computes a constant value.  */
	      simplified = simplify_ternary_operation (GET_CODE (src),
						       GET_MODE (src),
						       GET_MODE (src),
						       src0, src1, src2);
	      break;
	    }
#endif	
	  default:
	    def_to_varying (insn);
	}

      if (simplified && really_constant_p (simplified))
	{
	  if (values[DECL_UID (dest)].lattice_val != CONSTANT
	      || !(simple_cst_equal (values[DECL_UID (dest)].const_value, simplified) == 1))
	    SET_BIT (ssa_edges,DECL_UID (dest));

	  values[DECL_UID (dest)].lattice_val = CONSTANT;
	  values[DECL_UID (dest)].const_value = simplified;
	}
      else
        def_to_varying (insn);
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
      /* Always simulate PHI nodes, even if we have simulated this block
	 before.  Note that all PHI nodes are consecutive within a block.  */
      FOR_EACH_REF (ref, tmp, blockrefs)
	{
	  if (VARREF_TYPE (ref) == VARPHI)
	    visit_phi_node (ref, succ_block);
	}

      /* If this is the first time we've simulated this block, then we
	 must simulate each of its insns.  */
      if (!TEST_BIT (executable_blocks, succ_block->index))
	{
	  edge succ_edge = succ_block->succ;

	  /* Note that we have simulated this block.  */
	  SET_BIT (executable_blocks, succ_block->index);

	  FOR_EACH_REF (ref, tmp, blockrefs)
	    {
	      /* Simulate each insn within the block.  */
	      visit_expression (ref, succ_block);
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
	      SET_BIT (executable_edges,
		       EIE (succ_edge->src, succ_edge->dest));
	      edge_info[EIE (succ_edge->src, succ_edge->dest)] = flow_edges;
	      flow_edges = succ_edge;
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
  while (sbitmap_first_set_bit (ssa_edges) >= 0)
    {
      int member;
      ref_list refs;
      varref ref;
      struct ref_list_node *tmp;

      /* Pick an entry off the worklist (it does not matter which
	 entry we pick).  */
      member = sbitmap_first_set_bit (ssa_edges); 
      RESET_BIT (ssa_edges, member);
      refs = TREE_REFS (VARRAY_TREE (decl_map, member));
      if (!refs)
	continue;

      FOR_EACH_REF (ref, tmp, refs)
	{
	  if (VARREF_TYPE (ref) == VARUSE)
	    {
		if (TEST_BIT (executable_blocks, VARREF_BB (ref)->index))
		  visit_expression (ref, VARREF_BB (ref));
	    }
	  else if (VARREF_TYPE (ref) == VARPHI)
	    {
		if (TEST_BIT (executable_blocks, VARREF_BB (ref)->index))
		  visit_phi_node (ref, VARREF_BB (ref));
	    }
	}
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
#if 0
  int i;

  for (i = 0; i < NUM_EDGES (edges); i++)
    {
      if (!TEST_BIT (executable_edges, i))
	{
	  edge edge = INDEX_EDGE (edges, i);

	  if (edge->flags & EDGE_ABNORMAL)
	    continue;

	  /* We found an edge that is not executable.  First simplify
	     the PHI nodes in the target block.  */
	  if (edge->dest != EXIT_BLOCK_PTR)
	    {
	      rtx insn = first_insn_after_basic_block_note (edge->dest);

	      while (PHI_NODE_P (insn))
		{
		  remove_phi_alternative (PATTERN (insn), edge->src);
		  if (rtl_dump_file)
		    fprintf (rtl_dump_file,
			     "Removing alternative for bb %d of phi %d\n", 
			     edge->src->index, SSA_NAME (PATTERN (insn)));
		  insn = NEXT_INSN (insn);
		}
	    }
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file,
		     "Removing unexecutable edge from %d to %d\n",
		     edge->src->index, edge->dest->index);
	  /* Since the edge was not executable, remove it from the CFG.  */
	  remove_edge (edge);
	}
    }

  /* We have removed all the unexecutable edges from the CFG.  Fix up
     the conditional jumps at the end of any affected block.

     We have three cases to deal with:

       a. Both outgoing edges are not executable.  This happens if the
	  source block is not reachable.  We will deal with this by
	  deleting all the insns in the block later.

       b. The fall-thru edge is not executable.  In this case we
	  change the conditional jump into an unconditional jump and
	  add a BARRIER after the unconditional jump.  Note that since
	  we are working on generic RTL we can change the jump in-place
	  instead of dealing with the headache of reemitting the jump.

       c. The branch taken edge is not executable.  In this case
	  we turn the jump into (set (pc) (pc)) which is a nop-jump
          and we will remove the unrecognizable insn later.

     In cases B & C we are removing uses of registers, so make sure
     to note those changes for the DF analyzer.  */

  for (i = 0; i < n_basic_blocks; i++)
    {
      basic_block bb = BASIC_BLOCK (i);
      rtx insn = bb->end;
      edge edge = bb->succ;

      /* If we have no predecessors, then this block is unreachable and
	 will be cleaned up when we remove unreachable blocks.  */
      if (bb->pred == NULL || GET_CODE (insn) != JUMP_INSN)
	continue;

      /* If this block ends in a conditional jump, but only has one
	 successor, then the jump needs adjustment.  */
      if (condjump_p (insn) && ! simplejump_p (insn)
	  && bb->succ && bb->succ->succ_next == NULL)
	{
	  /* If the fallthru edge is the executable edge, then turn
	     this jump into a nop jump, otherwise make it an unconditinoal
	     jump to its target.  */
	  if (edge->flags & EDGE_FALLTHRU)
	    {
	      PUT_CODE (insn, NOTE);
	      NOTE_LINE_NUMBER (insn) = NOTE_INSN_DELETED;
	    }
	  else
	    {
	      SET_SRC (PATTERN (insn)) = gen_rtx_LABEL_REF (Pmode,
							    JUMP_LABEL (insn));
	      emit_barrier_after (insn);
	      INSN_CODE (insn) = -1;
	    }

	  /* Inform the DF analyzer that this insn changed.  */
	  df_insn_modify (df_analyzer, BLOCK_FOR_INSN (insn), insn);
	}
    }
#endif
}
 
/* Perform substitution of known values for pseudo registers.

   ??? Note we do not do simplifications or constant folding here, it
   is unlikely that any significant simplifications can be done here
   anyway.  Consider that if the simplification would result in an
   expression that produces a constant value that the value would
   have been discovered and recorded already.
   
   We perform two transformations.  First, we initialize pseudos to their
   known constant values at their definition point.  Second, we try to
   replace uses with the known constant value.  */

static void
ssa_ccp_substitute_constants ()
{
  int i;

  for (i = 0; i < next_decl_uid; i++)
    {
      if (values[i].lattice_val == CONSTANT)
	{
	  tree def = VARRAY_TREE (decl_map, i);
	  ref_list refs = TREE_REFS (def);
	  varref ref;
	  struct ref_list_node *tmp;

	  FOR_EACH_REF (ref, tmp, refs)
	    if (VARREF_TYPE (ref) == VARUSE)
		replace_expr_in_tree (VARREF_EXPR (ref), VARREF_SYM (ref),
				      values[i].const_value);
	}
    }
}


/* Now find all unreachable basic blocks.  All the insns in those
   blocks are unreachable, so delete them and mark any necessary
   updates for the DF analyzer.  */

static void
ssa_ccp_df_delete_unreachable_insns ()
{
#if 0
  int i;

  /* Use the CFG to find all the reachable blocks.  */
  find_unreachable_blocks ();

  /* Now we know what blocks are not reachable.  Mark all the insns
     in those blocks as deleted for the DF analyzer.   We'll let the
     normal flow code actually remove the unreachable blocks.  */
  for (i = n_basic_blocks - 1; i >= 0; --i)
    {
      basic_block b = BASIC_BLOCK (i);

      if (b->aux != NULL)
	/* This block was found.  Tidy up the mark.  */
	b->aux = NULL;
      else
	{
	  rtx start = b->head;
	  rtx end = b->end;
	  rtx tmp;

	  /* Include any jump table following the basic block.  */
	  end = b->end;
	  if (GET_CODE (end) == JUMP_INSN
	      && (tmp = JUMP_LABEL (end)) != NULL_RTX
	      && (tmp = NEXT_INSN (tmp)) != NULL_RTX
	      && GET_CODE (tmp) == JUMP_INSN
	      && (GET_CODE (PATTERN (tmp)) == ADDR_VEC
	          || GET_CODE (PATTERN (tmp)) == ADDR_DIFF_VEC))
	    end = tmp;

	  while (1)
	    {
	      rtx next = NEXT_INSN (start);

	      if (GET_CODE (start) == INSN
		  || GET_CODE (start) == CALL_INSN
		  || GET_CODE (start) == JUMP_INSN)
		df_insn_delete (df_analyzer, BLOCK_FOR_INSN (start), start);

	      if (start == end)
		break;
	      start = next;
	    }
	}
    }
#endif
}


