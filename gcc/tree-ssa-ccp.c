/* Conditional constant propagation pass for the GNU compiler.
   Copyright (C) 2000, 2001, 2002, 2003 Free Software Foundation, Inc.
   Adapted from original RTL SSA-CCP by Daniel Berlin <dberlin@dberlin.org>
   Adapted to GIMPLE trees by Diego Novillo <dnovillo@redhat.com>

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

/* Conditional constant propagation.

   References:

     Constant propagation with conditional branches,
     Wegman and Zadeck, ACM TOPLAS 13(2):181-210.

     Building an Optimizing Compiler,
     Robert Morgan, Butterworth-Heinemann, 1998, Section 8.9.

     Advanced Compiler Design and Implementation,
     Steven Muchnick, Morgan Kaufmann, 1997, Section 12.6  */

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
#include "tree-inline.h"
#include "tree-flow.h"
#include "tree-simple.h"
#include "tree-dump.h"
#include "timevar.h"
#include "expr.h"

/* Possible lattice values.  */
typedef enum
{
  UNINITIALIZED = 0,
  UNDEFINED,
  CONSTANT,
  VARYING
} latticevalue;

/* Use the TREE_VISITED bitflag to mark statements and PHI nodes that have
   been deemed VARYING and shouldn't be simulated again.  */
#define DONT_SIMULATE_AGAIN(T)	TREE_VISITED (T)

/* Main structure for CCP.  Contains the lattice value and, if it's a
    constant, the constant value.  */
typedef struct
{
  latticevalue lattice_val;
  tree const_val;
} value;

/* A bitmap to keep track of executable blocks in the CFG.  */
static sbitmap executable_blocks;

/* Array of control flow edges on the worklist.  */
static varray_type cfg_blocks;

static unsigned int cfg_blocks_num = 0;
static int cfg_blocks_tail;
static int cfg_blocks_head;

static sbitmap bb_in_list;

/* This is used to track the current value of each variable.  */
static value *value_vector;

/* Worklist of SSA edges which will need reexamination as their definition
   has changed.  SSA edges are def-use edges in the SSA web.  For each
   edge, we store the definition statement or PHI node D.  The destination
   nodes that need to be visited are accessed using immediate_uses (D).  */
static GTY(()) varray_type ssa_edges;

static void initialize (void);
static void finalize (void);
static void visit_phi_node (tree);
static tree ccp_fold (tree);
static value cp_lattice_meet (value, value);
static void visit_stmt (tree);
static void visit_cond_stmt (tree);
static void visit_assignment (tree);
static void add_var_to_ssa_edges_worklist (tree);
static void add_outgoing_control_edges (basic_block);
static void add_control_edge (edge);
static void def_to_undefined (tree);
static void def_to_varying (tree);
static void set_lattice_value (tree, value);
static void simulate_block (basic_block);
static void simulate_stmt (tree);
static void substitute_and_fold (sbitmap);
static value evaluate_stmt (tree);
static void dump_lattice_value (FILE *, const char *, value);
static bool replace_uses_in (tree);
static latticevalue likely_value (tree);
static tree get_rhs (tree);
static void set_rhs (tree *, tree);
static inline value *get_value (tree);
static value get_default_value (tree);
static tree ccp_fold_builtin (tree, tree);
static tree get_strlen (tree);
static inline bool cfg_blocks_empty_p (void);
static void cfg_blocks_add (basic_block);
static basic_block cfg_blocks_get (void);

/* Debugging dumps.  */
static FILE *dump_file;
static int dump_flags;


/* Main entry point for SSA Conditional Constant Propagation.  FNDECL is
   the declaration for the function to optimize.
   
   On exit, VARS_TO_RENAME will contain the symbols that have been exposed by
   the propagation of ADDR_EXPR expressions into pointer derferences and need
   to be renamed into SSA.

   PHASE indicates which dump file from the DUMP_FILES array to use when
   dumping debugging information.  */

void
tree_ssa_ccp (tree fndecl, sbitmap vars_to_rename, enum tree_dump_index phase)
{
  timevar_push (TV_TREE_CCP);

  /* Initialize debugging dumps.  */
  dump_file = dump_begin (phase, &dump_flags);

  initialize ();

  /* Iterate until the worklists are empty.  */
  while (!cfg_blocks_empty_p () || VARRAY_ACTIVE_SIZE (ssa_edges) > 0)
    {
      if (!cfg_blocks_empty_p ())
	{
	  /* Pull the next block to simulate off the worklist.  */
	  basic_block dest_block = cfg_blocks_get ();
	  simulate_block (dest_block);
	}

      /* The SSA_EDGES worklist can get rather large.  Go ahead and
         drain the entire worklist each iteration through this loop.  */
      while (VARRAY_ACTIVE_SIZE (ssa_edges) > 0)
	{
	  /* Pull the statement to simulate off the worklist.  */
	  tree stmt = VARRAY_TOP_TREE (ssa_edges);
	  VARRAY_POP (ssa_edges);

	  /* visit_stmt can "cancel" reevaluation of some statements.
	     If it does, then in_ccp_worklist will be zero.  */
	  if (stmt_ann (stmt)->in_ccp_worklist)
	    {
	      stmt_ann (stmt)->in_ccp_worklist = 0;
	      simulate_stmt (stmt);
	    }
	}
    }

  /* Now perform substitutions based on the known constant values.  */
  substitute_and_fold (vars_to_rename);

  /* Now cleanup any unreachable code.  */
  cleanup_tree_cfg (false);

  /* Free allocated memory.  */
  finalize ();

  timevar_pop (TV_TREE_CCP);

  /* Debugging dumps.  */
  if (dump_file)
    {
      if (dump_flags & TDF_DETAILS)
	{
	  dump_referenced_vars (dump_file);
	  fprintf (dump_file, "\n\n");
	  fprintf (dump_file, "\n");
	}

      dump_cfg_function_to_file (fndecl, dump_file, dump_flags);
      dump_end (phase, dump_file);
    }
}


/* Get the constant value associated with variable VAR.  */

static inline value *
get_value (tree var)
{
  value *val;
#if defined ENABLE_CHECKING
  if (TREE_CODE (var) != SSA_NAME)
    abort ();
#endif

  val = &(value_vector[SSA_NAME_VERSION (var)]);
  if (val->lattice_val == UNINITIALIZED)
    *val = get_default_value (var);
    
  return val;
}


/* Simulate the execution of BLOCK.  Evaluate the statement associated
   with each variable reference inside the block.  */

static void
simulate_block (basic_block block)
{
  tree phi;

  /* There is nothing to do for the exit block.  */
  if (block == EXIT_BLOCK_PTR)
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nSimulating block %d\n", block->index);

  /* Always simulate PHI nodes, even if we have simulated this block
     before.  */
  for (phi = phi_nodes (block); phi; phi = TREE_CHAIN (phi))
    visit_phi_node (phi);

  /* If this is the first time we've simulated this block, then we
     must simulate each of its statements.  */
  if (!TEST_BIT (executable_blocks, block->index))
    {
      block_stmt_iterator j;
      unsigned int normal_edge_count;
      edge e, normal_edge;

      /* Note that we have simulated this block.  */
      SET_BIT (executable_blocks, block->index);

      for (j = bsi_start (block); !bsi_end_p (j); bsi_next (&j))
	visit_stmt (bsi_stmt (j));

      /* We can not predict when abnormal edges will be executed, so
	 once a block is considered executable, we consider any
	 outgoing abnormal edges as executable.

	 At the same time, if this block has only one successor that is
	 reached by non-abnormal edges, then add that successor to the
	 worklist.  */
      normal_edge_count = 0;
      normal_edge = NULL;
      for (e = block->succ; e; e = e->succ_next)
        {
	  if (e->flags & EDGE_ABNORMAL)
	    {
	      add_control_edge (e);
	    }
	  else
	    {
	      normal_edge_count++;
	      normal_edge = e;
	    }
        }

        if (normal_edge_count == 1)
	  add_control_edge (normal_edge);
    }
}


/* Follow the def-use edges for statement DEF_STMT and simulate all the
   statements reached by it.  */

static void
simulate_stmt (tree use_stmt)
{
  basic_block use_bb = bb_for_stmt (use_stmt);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nSimulating statement (from ssa_edges): ");
      print_generic_stmt (dump_file, use_stmt, 0);
    }

  if (TREE_CODE (use_stmt) == PHI_NODE)
    {
      /* PHI nodes are always visited, regardless of whether or not the
         destination block is executable.  */
      visit_phi_node (use_stmt);
    }
  else if (TEST_BIT (executable_blocks, use_bb->index))
    {
      /* Otherwise, visit the statement containing the use reached by
         DEF, only if the destination block is marked executable.  */
      visit_stmt (use_stmt);
    }
}


/* Perform final substitution and folding.  After this pass the program
   should still be in SSA form.  */

static void
substitute_and_fold (sbitmap vars_to_rename)
{
  basic_block bb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nSubstituing constants and folding statements\n\n");

  /* Substitute constants in every statement of every basic block.  */
  FOR_EACH_BB (bb)
    {
      block_stmt_iterator i;
      tree phi;

      /* Propagate our known constants into PHI nodes.  */
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	{
	  int i;

	  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	    {
	      value *new_val;
	      tree *orig_p = &PHI_ARG_DEF (phi, i);

	      if (! SSA_VAR_P (*orig_p))
		break;

	      new_val = get_value (*orig_p);
	      if (new_val->lattice_val == CONSTANT
		  && may_propagate_copy (*orig_p, new_val->const_val))
		*orig_p = new_val->const_val;
	    }
	}

      for (i = bsi_start (bb); !bsi_end_p (i); bsi_next (&i))
	{
	  tree stmt = bsi_stmt (i);

	  /* Skip statements that have been folded already.  */
	  if (stmt_modified_p (stmt) || !is_exec_stmt (stmt))
	    continue;

	  /* Replace the statement with its folded version and mark it
	     folded.  */
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Line %d: replaced ", get_lineno (stmt));
	      print_generic_stmt (dump_file, stmt, TDF_SLIM);
	    }

	  if (replace_uses_in (stmt))
	    {
	      fold_stmt (bsi_stmt_ptr (i));
	      modify_stmt (stmt);

	      /* If the statement is an assignment involving pointer
		 dereferences, we may have exposed new symbols.  */
	      if (TREE_CODE (stmt) == MODIFY_EXPR
		  && (TREE_CODE (TREE_OPERAND (stmt, 0)) == INDIRECT_REF
		      || TREE_CODE (TREE_OPERAND (stmt, 1)) == INDIRECT_REF))
		mark_new_vars_to_rename (stmt, vars_to_rename);
	    }

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, " with ");
	      print_generic_stmt (dump_file, stmt, TDF_SLIM);
	      fprintf (dump_file, "\n");
	    }
	}
    }
}


/* Loop through the PHI_NODE's parameters for BLOCK and compare their
   lattice values to determine PHI_NODE's lattice value.  The value of a
   PHI node is determined calling cp_lattice_meet() with all the arguments
   of the PHI node that are incoming via executable edges.  */

static void
visit_phi_node (tree phi)
{
  int i, short_circuit = 0;
  value phi_val, *curr_val;

  /* If the PHI node has already been deemed to be VARYING, don't simulate
     it again.  */
  if (DONT_SIMULATE_AGAIN (phi))
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nVisiting PHI node: ");
      print_generic_expr (dump_file, phi, 0);
    }

  curr_val = get_value (PHI_RESULT (phi));
  if (curr_val->lattice_val == VARYING)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "\n   Shortcircuit. Default of VARYING.");
      short_circuit = 1;
    }
  else
    if (curr_val->lattice_val != CONSTANT)
      {
	phi_val.lattice_val = UNDEFINED;
	phi_val.const_val = NULL_TREE;
      }
    else
      {
	phi_val.lattice_val = curr_val->lattice_val;
	phi_val.const_val = curr_val->const_val;
      }

  /* If the variable is volatile or the variable is never referenced in a
     real operand, then consider the PHI node VARYING.  */
  if (short_circuit || TREE_THIS_VOLATILE (SSA_NAME_VAR (PHI_RESULT (phi))))
    phi_val.lattice_val = VARYING;
  else
    for (i = 0; i < PHI_NUM_ARGS (phi); i++)
      {
	/* Compute the meet operator over all the PHI arguments. */
	edge e = PHI_ARG_EDGE (phi, i);

	if (dump_file && (dump_flags & TDF_DETAILS))
	  {
	    fprintf (dump_file, "\n    Argument #%d (%d -> %d %sexecutable)\n",
		    i, e->src->index, e->dest->index,
		    (e->flags & EDGE_EXECUTABLE) ? "" : "not ");
	  }

	/* If the incoming edge is executable, Compute the meet operator for
	   the existing value of the PHI node and the current PHI argument.  */
	if (e->flags & EDGE_EXECUTABLE)
	  {
	    tree rdef = PHI_ARG_DEF (phi, i);
	    value *rdef_val, val;

	    if (TREE_CONSTANT (rdef))
	      {
		val.lattice_val = CONSTANT;
		val.const_val = rdef;
		rdef_val = &val;
	      }
	    else
	      rdef_val = get_value (rdef);

	    phi_val = cp_lattice_meet (phi_val, *rdef_val);

	    if (dump_file && (dump_flags & TDF_DETAILS))
	      {
		fprintf (dump_file, "\t");
		print_generic_expr (dump_file, rdef, 0);
		dump_lattice_value (dump_file, "\tValue: ", *rdef_val);
		fprintf (dump_file, "\n");
	      }

	    if (phi_val.lattice_val == VARYING)
	      break;
	  }
      }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      dump_lattice_value (dump_file, "\n    PHI node value: ", phi_val);
      fprintf (dump_file, "\n\n");
    }

  set_lattice_value (PHI_RESULT (phi), phi_val);
  if (phi_val.lattice_val == VARYING)
    DONT_SIMULATE_AGAIN (phi) = 1;
}


/* Compute the meet operator between VAL1 and VAL2:

   		any M UNDEFINED = any
		any M VARYING	= VARYING
		Ci  M Cj	= Ci		if (i == j)
		Ci  M Cj	= VARYING	if (i != j)  */
static value
cp_lattice_meet (value val1, value val2)
{
  value result;

  /* any M UNDEFINED = any.  */
  if (val1.lattice_val == UNDEFINED)
    return val2;
  else if (val2.lattice_val == UNDEFINED)
    return val1;

  /* any M VARYING = VARYING.  */
  if (val1.lattice_val == VARYING || val2.lattice_val == VARYING)
    {
      result.lattice_val = VARYING;
      result.const_val = NULL_TREE;
      return result;
    }

  /* Ci M Cj = Ci	if (i == j)
     Ci M Cj = VARYING	if (i != j)  */
  if (simple_cst_equal (val1.const_val, val2.const_val) == 1)
    {
      result.lattice_val = CONSTANT;
      result.const_val = val1.const_val;
    }
  else
    {
      result.lattice_val = VARYING;
      result.const_val = NULL_TREE;
    }

  return result;
}


/* Evaluate statement STMT.  If the statement produces an output value and
   its evaluation changes the lattice value of its output, do the following:

   - If the statement is an assignment, add all the SSA edges starting at
     this definition.

   - If the statement is a conditional branch:
   	. If the statement evaluates to non-constant, add all edges to
	  worklist.
	. If the statement is constant, add the edge executed as the
	  result of the branch.  */

static void
visit_stmt (tree stmt)
{
  size_t i;
  varray_type ops;

  /* If the statement has already been deemed to be VARYING, don't simulate
     it again.  */
  if (DONT_SIMULATE_AGAIN (stmt))
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nVisiting statement: ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  /* If this statement is already in the worklist then "cancel" it.  The
     reevaluation implied by the worklist entry will produce the same
     value we generate here and thus reevaluting it again from the
     worklist is pointless.  */
  if (stmt_ann (stmt)->in_ccp_worklist)
    stmt_ann (stmt)->in_ccp_worklist = 0;

  /* Now examine the statement.  If the statement is an assignment that
     produces a single output value, evaluate its RHS to see if the lattice
     value of its output has changed.  */
  if (TREE_CODE (stmt) == MODIFY_EXPR
      && TREE_CODE (TREE_OPERAND (stmt, 0)) == SSA_NAME)
    visit_assignment (stmt);

  /* Definitions made by statements other than assignments to SSA_NAMEs
     represent unknown modifications to their outputs.  Mark them VARYING.  */
  else if (def_ops (stmt))
    {
      DONT_SIMULATE_AGAIN (stmt) = 1;
      ops = def_ops (stmt);
      for (i = 0; i < VARRAY_ACTIVE_SIZE (ops); i++)
	{
	  tree *def_p = VARRAY_TREE_PTR (ops, i);
	  def_to_varying (*def_p);
	}
    }

  /* If STMT is a conditional branch, see if we can determine which branch
     will be taken.  */
  else if (TREE_CODE (stmt) == COND_EXPR || TREE_CODE (stmt) == SWITCH_EXPR)
    visit_cond_stmt (stmt);

  /* Any other kind of statement is not interesting for constant
     propagation and, therefore, not worth simulating.  */
  else
    {
      DONT_SIMULATE_AGAIN (stmt) = 1;

      /* If STMT is a control structure or a computed goto, then mark all
	 the output edges executable.  */
      if (is_ctrl_stmt (stmt) || is_computed_goto (stmt))
	add_outgoing_control_edges (bb_for_stmt (stmt));
    }

  /* Mark all VDEF operands VARYING.  */
  ops = vdef_ops (stmt);
  if (ops)
    {
      for (i = 0; i < VARRAY_ACTIVE_SIZE (ops); i++)
	def_to_varying (VDEF_RESULT (VARRAY_TREE (ops, i)));
    }
}


/* Visit the assignment statement STMT.  Set the value of its LHS to the
   value computed by the RHS.  */

static void
visit_assignment (tree stmt)
{
  value val;
  tree lhs, rhs;

  lhs = TREE_OPERAND (stmt, 0);
  rhs = TREE_OPERAND (stmt, 1);

  if (TREE_THIS_VOLATILE (SSA_NAME_VAR (lhs)))
    {
      /* Volatile variables are always VARYING.  */
      val.lattice_val = VARYING;
      val.const_val = NULL_TREE;
    }
  else if (TREE_CODE (rhs) == SSA_NAME)
    {
      /* For a simple copy operation, we copy the lattice values.  */
      value *nval = get_value (rhs);
      val.lattice_val = nval->lattice_val;
      val.const_val = nval->const_val;
    }
  else
    {
      /* Evaluate the statement.  */
      val = evaluate_stmt (stmt);
    }

  /* FIXME: Hack.  If this was a definition of a bitfield, we need to widen
     the constant value into the type of the destination variable.  This
     should not be necessary if GCC represented bitfields properly.  */
  {
    tree lhs = TREE_OPERAND (stmt, 0);
    if (val.lattice_val == CONSTANT
	&& TREE_CODE (lhs) == COMPONENT_REF
	&& DECL_BIT_FIELD (TREE_OPERAND (lhs, 1)))
      {
	tree w = widen_bitfield (val.const_val, TREE_OPERAND (lhs, 1), lhs);

	if (w)
	  val.const_val = w;
	else
	  {
	    val.lattice_val = VARYING;
	    val.const_val = NULL;
	  }
      }
  }

  /* Set the lattice value of the statement's output.  */
  set_lattice_value (lhs, val);
  if (val.lattice_val == VARYING)
    DONT_SIMULATE_AGAIN (stmt) = 1;
}


/* Visit the conditional statement STMT.  If it evaluates to a constant value,
   mark outgoing edges appropriately.  */

static void
visit_cond_stmt (tree stmt)
{
  edge e;
  value val;
  basic_block block;

  block = bb_for_stmt (stmt);
  val = evaluate_stmt (stmt);

  /* Find which edge out of the conditional block will be taken and add it
     to the worklist.  If no single edge can be determined statically, add
     all outgoing edges from BLOCK.  */
  e = find_taken_edge (block, val.const_val);
  if (e)
    add_control_edge (e);
  else
    {
      DONT_SIMULATE_AGAIN (stmt) = 1;
      add_outgoing_control_edges (block);
    }
}


/* Add all the edges coming out of BB to the control flow worklist.  */

static void
add_outgoing_control_edges (basic_block bb)
{
  edge e;

  for (e = bb->succ; e; e = e->succ_next)
    add_control_edge (e);
}


/* Add edge E to the control flow worklist.  */

static void
add_control_edge (edge e)
{
  basic_block bb = e->dest;
  if (bb == EXIT_BLOCK_PTR)
    return;

  /* If the edge had already been executed, skip it.  */
  if (e->flags & EDGE_EXECUTABLE)
      return;

  e->flags |= EDGE_EXECUTABLE;

  /* If the block is already in the list, we're done.  */
  if (TEST_BIT (bb_in_list, bb->index))
    return;

  cfg_blocks_add (bb);

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Adding Destination of edge (%d -> %d) to worklist\n\n",
	     e->src->index, e->dest->index);
}


/* CCP specific front-end to the non-destructive constant folding routines.

   Attempt to simplify the RHS of STMT knowing that one or more
   operands are constants.

   If simplification is possible, return the simplified RHS,
   otherwise return the original RHS.  */

static tree
ccp_fold (tree stmt)
{
  tree rhs = get_rhs (stmt);
  enum tree_code code = TREE_CODE (rhs);
  int kind = TREE_CODE_CLASS (code);
  tree retval = NULL_TREE;

  /* If the RHS is just a variable, then that variable must now have
     a constant value that we can return directly.  */
  if (TREE_CODE (rhs) == SSA_NAME)
    return get_value (rhs)->const_val;

  /* Unary operators.  Note that we know the single operand must
     be a constant.  So this should almost always return a
     simplified RHS.  */
  if (kind == '1')
    {
      /* Handle unary operators which can appear in GIMPLE form.  */
      tree op0 = TREE_OPERAND (rhs, 0);

      /* Simplify the operand down to a constant.  */
      if (TREE_CODE (op0) == SSA_NAME)
	{
	  value *val = get_value (op0);
	  if (val->lattice_val == CONSTANT)
	    op0 = get_value (op0)->const_val;
	}

      retval = nondestructive_fold_unary_to_constant (code,
		     				      TREE_TYPE (rhs),
						      op0);

      /* If we could not fold the expression, but the arguments are all
         constants and gimple values, then build and return the new
	 expression. 

	 In some cases the new expression is still something we can
	 use as a replacement for an argument.  This happens with
	 NOP conversions of types for example.

	 In other cases the new expression can not be used as a
	 replacement for an argument (as it would create non-gimple
	 code).  But the new expression can still be used to derive
	 other constants.  */
      if (! retval && is_unchanging_value (op0))
	return build1 (code, TREE_TYPE (rhs), op0);
    }

  /* Binary and comparison operators.  We know one or both of the
     operands are constants.  */
  else if (kind == '2'
           || kind == '<'
           || code == TRUTH_AND_EXPR
           || code == TRUTH_OR_EXPR
           || code == TRUTH_XOR_EXPR)
    {
      /* Handle binary and comparison operators that can appear in
         GIMPLE form.  */
      tree op0 = TREE_OPERAND (rhs, 0);
      tree op1 = TREE_OPERAND (rhs, 1);

      /* Simplify the operands down to constants when appropriate.  */
      if (TREE_CODE (op0) == SSA_NAME)
	{
	  value *val = get_value (op0);
	  if (val->lattice_val == CONSTANT)
	    op0 = val->const_val;
	}

      if (TREE_CODE (op1) == SSA_NAME)
	{
	  value *val = get_value (op1);
	  if (val->lattice_val == CONSTANT)
	    op1 = val->const_val;
	}

      retval = nondestructive_fold_binary_to_constant (code,
		     				       TREE_TYPE (rhs),
						       op0, op1);

      /* If we could not fold the expression, but the arguments are all
         constants and gimple values, then build and return the new
	 expression. 

	 In some cases the new expression is still something we can
	 use as a replacement for an argument.  This happens with
	 NOP conversions of types for example.

	 In other cases the new expression can not be used as a
	 replacement for an argument (as it would create non-gimple
	 code).  But the new expression can still be used to derive
	 other constants.  */
      if (! retval
	  && is_unchanging_value (op0)
	  && is_unchanging_value (op1))
	return build (code, TREE_TYPE (rhs), op0, op1);
    }

  /* We may be able to fold away calls to builtin functions if their
     arguments are constants. */
  else if (code == CALL_EXPR
	   && TREE_CODE (TREE_OPERAND (rhs, 0)) == ADDR_EXPR
	   && (TREE_CODE (TREE_OPERAND (TREE_OPERAND (rhs, 0), 0))
	       == FUNCTION_DECL)
	   && DECL_BUILT_IN (TREE_OPERAND (TREE_OPERAND (rhs, 0), 0)))
    {
      varray_type uses = use_ops (stmt);
      if (uses)
	{
	  tree *orig;
	  size_t i;

	  /* Preserve the original values of every operand.  */
	  orig = xmalloc (sizeof (tree) * VARRAY_ACTIVE_SIZE (uses));
	  for (i = 0; i < VARRAY_ACTIVE_SIZE (uses); i++)
	    orig[i] = *(VARRAY_TREE_PTR (uses, i));

	  /* Substitute operands with their values and try to fold.  */
	  replace_uses_in (stmt);
	  retval = fold_builtin (rhs);

	  /* Restore operands to their original form.  */
	  for (i = 0; i < VARRAY_ACTIVE_SIZE (uses); i++)
	    *(VARRAY_TREE_PTR (uses, i)) = orig[i];
	  free (orig);
	}
    }
  else
    return rhs;

  /* If we got a simplified form, see if we need to convert its type.  */
  if (retval)
    {
      if (TREE_TYPE (retval) != TREE_TYPE (rhs))
	retval = convert (TREE_TYPE (rhs), retval);

      if (TREE_TYPE (retval) == TREE_TYPE (rhs))
	return retval;
    }

  /* No simplification was possible.  */
  return rhs;
}


/* Evaluate statement STMT.  */

static value
evaluate_stmt (tree stmt)
{
  value val;
  tree simplified;
  latticevalue likelyvalue = likely_value (stmt);

  /* If the statement is likely to have a CONSTANT result, then try
     to fold the statement to determine the constant value.  */
  if (likelyvalue == CONSTANT)
    simplified = ccp_fold (stmt);
  /* If the statement is likely to have a VARYING result, then do not
     bother folding the statement.  */
  else if (likelyvalue == VARYING)
    simplified = get_rhs (stmt);
  /* Otherwise the statement is likely to have an UNDEFINED value and
     there will be nothing to do.  */
  else
    simplified = NULL_TREE;

  if (simplified && is_unchanging_value (simplified))
    {
      /* The statement produced a constant value.  */
      val.lattice_val = CONSTANT;
      val.const_val = simplified;
    }
  else
    {
      /* The statement produced a nonconstant value.  If the statement
         had undefined operands, then the result of the statement should
	 be undefined.  Else the result of the statement is VARYING.  */
      val.lattice_val = (likelyvalue == UNDEFINED ? UNDEFINED : VARYING);
      val.const_val = NULL_TREE;
    }

  /* Debugging dumps.  */
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Statement evaluates to ");
      print_generic_stmt (dump_file, simplified, TDF_SLIM);
      fprintf (dump_file, " which is ");
      if (val.lattice_val == CONSTANT)
	{
	  fprintf (dump_file, "constant ");
	  print_generic_expr (dump_file, simplified, 0);
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
dump_lattice_value (FILE *outf, const char *prefix, value val)
{
  if (val.lattice_val == UNDEFINED)
    fprintf (outf, "%sUNDEFINED", prefix);
  else if (val.lattice_val == VARYING)
    fprintf (outf, "%sVARYING", prefix);
  else
    {
      fprintf (outf, "%sCONSTANT ", prefix);
      print_generic_expr (outf, val.const_val, 0);
    }
}

/* Given a constant value VAL for bitfield FIELD, and a destination
   variable VAR, return VAL appropriately widened to fit into VAR.  If
   FIELD is wider than HOST_WIDE_INT, NULL is returned.  */

tree
widen_bitfield (tree val, tree field, tree var)
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

#if defined ENABLE_CHECKING
  if (var_size < field_size)
    abort ();
#endif

  /* If VAL is not an integer constant, then give up.  */
  if (TREE_CODE (val) != INTEGER_CST)
    return NULL;

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
initialize (void)
{
  edge e;
  basic_block bb;

  /* Compute immediate uses.  */
  compute_immediate_uses (TDFA_USE_OPS);

  if (dump_file && dump_flags & TDF_DETAILS)
    dump_immediate_uses (dump_file);

  /* Worklist of SSA edges.  */
  VARRAY_TREE_INIT (ssa_edges, 20, "ssa_edges");

  executable_blocks = sbitmap_alloc (last_basic_block);
  sbitmap_zero (executable_blocks);

  bb_in_list = sbitmap_alloc (last_basic_block);
  sbitmap_zero (bb_in_list);

  value_vector = (value *) xmalloc (next_ssa_version * sizeof (value));
  memset (value_vector, 0, next_ssa_version * sizeof (value));

  /* Initialize simulation flags for PHI nodes, statements and edges.  */
  FOR_EACH_BB (bb)
    {
      block_stmt_iterator i;
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	DONT_SIMULATE_AGAIN (phi) = 0;

      for (i = bsi_start (bb); !bsi_end_p (i); bsi_next (&i))
	DONT_SIMULATE_AGAIN (bsi_stmt (i)) = 0;

      for (e = bb->succ; e; e = e->succ_next)
	e->flags &= ~EDGE_EXECUTABLE;
    }


  VARRAY_BB_INIT (cfg_blocks, 20, "cfg_blocks");

  /* Seed the algorithm by adding the successors of the entry block to the
     edge worklist.  */
  for (e = ENTRY_BLOCK_PTR->succ; e; e = e->succ_next)
    {
      if (e->dest != EXIT_BLOCK_PTR)
        {
	  e->flags |= EDGE_EXECUTABLE;
	  cfg_blocks_add (e->dest);
	}
    }
}


/* Free allocated storage.  */

static void
finalize (void)
{
  free (value_vector);
  sbitmap_free (bb_in_list);
  sbitmap_free (executable_blocks);
}

/* Is the block worklist empty.  */

static inline bool
cfg_blocks_empty_p ()
{
  return (cfg_blocks_num == 0);
}

/* Add a basic block to the worklist.  */

static void 
cfg_blocks_add (basic_block bb)
{
   if (bb == ENTRY_BLOCK_PTR || bb == EXIT_BLOCK_PTR)
     return;

   if (TEST_BIT (bb_in_list, bb->index))
     return;

    if (cfg_blocks_empty_p ())
      {
	cfg_blocks_tail = cfg_blocks_head = 0;
	cfg_blocks_num = 1;
      }
    else
      {
	cfg_blocks_num++;
	if (cfg_blocks_num > VARRAY_SIZE (cfg_blocks))
	  {
	    /* We have to grow the array now.  Adjust to queue to occupy the
	       full space of the original array.  */
	    cfg_blocks_tail = VARRAY_SIZE (cfg_blocks);
	    cfg_blocks_head = 0;
	    VARRAY_GROW (cfg_blocks, 2 * VARRAY_SIZE (cfg_blocks));
	  }
	else
	  cfg_blocks_tail = (cfg_blocks_tail + 1) % VARRAY_SIZE (cfg_blocks);
      }
    VARRAY_BB (cfg_blocks, cfg_blocks_tail) = bb;
    SET_BIT (bb_in_list, bb->index);
}

/* Remove a block from the worklist.  */

static basic_block
cfg_blocks_get ()
{
  basic_block bb;

  bb = VARRAY_BB (cfg_blocks, cfg_blocks_head);

#ifdef ENABLE_CHECKING
  if (cfg_blocks_empty_p () || !bb)
    abort ();
#endif

  cfg_blocks_head = (cfg_blocks_head + 1) % VARRAY_SIZE (cfg_blocks);
  --cfg_blocks_num;
  RESET_BIT (bb_in_list, bb->index);

  return bb;
}

/* We have just definited a new value for VAR.  Add all immediate uses
   of VAR to the ssa_edges worklist.  */
static void
add_var_to_ssa_edges_worklist (tree var)
{
  tree stmt = SSA_NAME_DEF_STMT (var);
  dataflow_t df = get_immediate_uses (stmt);
  int num_uses = num_immediate_uses (df);
  int i;

  for (i = 0; i < num_uses; i++)
    {
      tree use = immediate_use (df, i);

      if (!DONT_SIMULATE_AGAIN (use) && stmt_ann (use)->in_ccp_worklist == 0)
	{
	  stmt_ann (use)->in_ccp_worklist = 1;
	  VARRAY_PUSH_TREE (ssa_edges, use);
	}
    }
}

/* Set the lattice value for the variable VAR to UNDEFINED.  */

static void
def_to_undefined (tree var)
{
  value *value = get_value (var);

#ifdef ENABLE_CHECKING
  /* CONSTANT->UNDEFINED is never a valid state transition.  */
  if (value->lattice_val == CONSTANT)
    abort ();

  /* VARYING->UNDEFINED is generally not a valid state transition,
     except for values which are initialized to VARYING.  */
  if (value->lattice_val == VARYING
      && get_default_value (var).lattice_val != VARYING)
    abort ();
#endif

  if (value->lattice_val != UNDEFINED)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Lattice value changed.  Adding definition to SSA edges.\n");

      add_var_to_ssa_edges_worklist (var);
      value->lattice_val = UNDEFINED;
      value->const_val = NULL_TREE;
    }
}


/* Set the lattice value for the variable VAR to VARYING.  */

static void
def_to_varying (tree var)
{
  value *value = get_value (var);

  if (value->lattice_val != VARYING)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Lattice value changed.  Adding definition to SSA edges.\n");

      add_var_to_ssa_edges_worklist (var);
      value->lattice_val = VARYING;
      value->const_val = NULL_TREE;
    }
}


/* Set the lattice value for variable VAR to VAL.  */

static void
set_lattice_value (tree var, value val)
{
  if (val.lattice_val == UNDEFINED)
    def_to_undefined (var);
  else if (val.lattice_val == VARYING)
    def_to_varying (var);
  else
    {
      value *old_val = get_value (var);

      /* If the RHS is a constant value that is different from a previous
	 value for this reference, add its SSA edge to the worklist.  */
      if (old_val->lattice_val != CONSTANT
	  || !(simple_cst_equal (old_val->const_val, val.const_val)) == 1)
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Lattice value changed to ");
	      print_generic_expr (dump_file, val.const_val, 0);
	      fprintf (dump_file, ".  Adding definition to SSA edges.\n");
	    }

          add_var_to_ssa_edges_worklist (var);

#ifdef ENABLE_CHECKING
	  /* VARYING -> CONSTANT is an invalid state transition, except
	     for objects which start off in a VARYING state.  */
	  if (old_val->lattice_val == VARYING
	      && get_default_value (var).lattice_val != VARYING)
	    abort ();
#endif

	  /* If the constant for VAR has changed, then this VAR is
	     really varying.  */
	  if (old_val->lattice_val == CONSTANT)
	    {
	      old_val->lattice_val = VARYING;
	      old_val->const_val = NULL_TREE;
	    }
	  else
	    {
	      old_val->lattice_val = CONSTANT;
	      old_val->const_val = val.const_val;
	    }
	}
    }
}


/* Replace USE references in statement STMT with their immediate reaching
   definition.  Return true if at least one reference was replaced.  */

static bool
replace_uses_in (tree stmt)
{
  bool replaced = false;
  varray_type uses;
  size_t i;

  get_stmt_operands (stmt);

  uses = use_ops (stmt);
  for (i = 0; uses && i < VARRAY_ACTIVE_SIZE (uses); i++)
    {
      tree *use = VARRAY_TREE_PTR (uses, i);
      value *val = get_value (*use);

      if (val->lattice_val == CONSTANT)
	{
	  *use = val->const_val;
	  replaced = true;
	}
    }

  /* Some builtins like strlen may evaluate to a constant value even if
     they have non-constant operands.  For instance,
     'strlen (g++ ? "foo" : "bar")' will always evaluate to 3.  If the
     statement contains a call to one of these builtins, pretend that we
     replaced constant operands in it so that it can be handed to
     fold_stmt().  */
  if (!replaced
      && (TREE_CODE (stmt) == CALL_EXPR
	  || (TREE_CODE (stmt) == MODIFY_EXPR
	      && TREE_CODE (TREE_OPERAND (stmt, 1)) == CALL_EXPR)))

    {
      tree rhs = (TREE_CODE (stmt) == MODIFY_EXPR)
		  ? TREE_OPERAND (stmt, 1)
		  : stmt;
      tree callee = get_callee_fndecl (rhs);

      /* For now, we are only interested in handling a few builtins.  */
      if (callee
	  && DECL_BUILT_IN (callee)
	  && DECL_BUILT_IN_CLASS (callee) != BUILT_IN_MD)
	replaced = (DECL_FUNCTION_CODE (callee) == BUILT_IN_STRLEN
	            || DECL_FUNCTION_CODE (callee) == BUILT_IN_FPUTS
		    || DECL_FUNCTION_CODE (callee) == BUILT_IN_FPUTS_UNLOCKED);
    }

  return replaced;
}

/* Return the likely latticevalue for STMT.

   If STMT has no operands, then return CONSTANT.

   Else if any operands of STMT are undefined, then return UNDEFINED.

   Else if any operands of STMT are constants, then return CONSTANT.

   Else return VARYING.  */

static latticevalue
likely_value (tree stmt)
{
  varray_type uses;
  size_t i;
  int found_constant = 0;
  stmt_ann_t ann;

  /* If the statement makes aliased loads or has volatile operands, it
     won't fold to a constant value.  */
  ann = stmt_ann (stmt);
  if (ann->makes_aliased_loads || ann->has_volatile_ops)
    return VARYING;

  /* A CALL_EXPR is assumed to be varying.  This may be overly conservative,
     in the presence of const and pure calls.  */
  if (TREE_CODE (stmt) == CALL_EXPR
      || (TREE_CODE (stmt) == MODIFY_EXPR
	  && TREE_CODE (TREE_OPERAND (stmt, 1)) == CALL_EXPR)
      || (TREE_CODE (stmt) == RETURN_EXPR
	  && TREE_OPERAND (stmt, 0)
	  && TREE_CODE (TREE_OPERAND (stmt, 0)) == MODIFY_EXPR
	  && TREE_CODE (TREE_OPERAND (TREE_OPERAND (stmt, 0), 1)) == CALL_EXPR))
    return VARYING;

  get_stmt_operands (stmt);

  uses = use_ops (stmt);
  for (i = 0; uses && i < VARRAY_ACTIVE_SIZE (uses); i++)
    {
      tree *use = VARRAY_TREE_PTR (uses, i);
      value *val = get_value (*use);

      if (val->lattice_val == UNDEFINED)
	return UNDEFINED;

      if (val->lattice_val == CONSTANT)
	found_constant = 1;
    }

  return ((found_constant || !uses) ? CONSTANT : VARYING);
}


/* Fold the statement pointed by STMT_P.  In some cases, this function may
   replace the whole statement with a new one.  Returns true iff folding
   makes any changes.  */

bool
fold_stmt (tree *stmt_p)
{
  tree rhs, result, stmt;
  bool changed = false;

  stmt = *stmt_p;
  rhs = get_rhs (stmt);
  if (rhs && !TREE_CONSTANT (rhs))
    {
      result = NULL_TREE;

      /* Check for builtins that CCP can handle using information not
	 available in the generic fold routines.  */
      if (TREE_CODE (rhs) == CALL_EXPR)
	{
	  tree callee = get_callee_fndecl (rhs);
	  if (callee && DECL_BUILT_IN (callee))
	    result = ccp_fold_builtin (stmt, rhs);
	}

      /* Optimize *"foo" into 'f'.  This is done here rather than
         in fold to avoid problems with stuff like &*"foo".  */
      if (TREE_CODE (rhs) == INDIRECT_REF || TREE_CODE (rhs) == ARRAY_REF)
	result = fold_read_from_constant_string (rhs);

      /* If we couldn't fold the RHS, hand it over to the generic fold
	 routines.  */
      if (result == NULL_TREE)
	result = fold (rhs);

      /* Strip away useless type conversions.  */
      if (result != rhs)
	{
	  changed = true;
	  STRIP_MAIN_TYPE_NOPS (result);
	}

      set_rhs (stmt_p, result);
    }

  return changed;
}


/* Get the main expression from statement STMT.  */

static tree
get_rhs (tree stmt)
{
  enum tree_code code = TREE_CODE (stmt);

  if (code == MODIFY_EXPR)
    return TREE_OPERAND (stmt, 1);
  if (code == COND_EXPR)
    return COND_EXPR_COND (stmt);
  else if (code == SWITCH_EXPR)
    return SWITCH_COND (stmt);
  else if (code == RETURN_EXPR)
    {
      if (TREE_CODE (TREE_OPERAND (stmt, 0)) == MODIFY_EXPR)
	return TREE_OPERAND (TREE_OPERAND (stmt, 0), 1);
      else
	return TREE_OPERAND (stmt, 0);
    }
  else if (code == GOTO_EXPR)
    return GOTO_DESTINATION (stmt);
  else if (code == LABEL_EXPR)
    return LABEL_EXPR_LABEL (stmt);
  else
    return stmt;
}


/* Set the main expression of *STMT_P to EXPR.  */

static void
set_rhs (tree *stmt_p, tree expr)
{
  tree stmt = *stmt_p;
  enum tree_code code = TREE_CODE (stmt);

  if (code == MODIFY_EXPR)
    TREE_OPERAND (stmt, 1) = expr;
  else if (code == COND_EXPR)
    COND_EXPR_COND (stmt) = expr;
  else if (code == SWITCH_EXPR)
    SWITCH_COND (stmt) = expr;
  else if (code == RETURN_EXPR)
    {
      if (TREE_OPERAND (stmt, 0)
	  && TREE_CODE (TREE_OPERAND (stmt, 0)) == MODIFY_EXPR)
	TREE_OPERAND (TREE_OPERAND (stmt, 0), 1) = expr;
      else
	TREE_OPERAND (stmt, 0) = expr;
    }
  else if (code == GOTO_EXPR)
    GOTO_DESTINATION (stmt) = expr;
  else if (code == LABEL_EXPR)
    LABEL_EXPR_LABEL (stmt) = expr;
  else
    {
      stmt_ann_t ann = stmt_ann (stmt);
      *stmt_p = expr;
      (*stmt_p)->common.ann = (tree_ann) ann;
    }
}


/* Return a default value for variable VAR using the following rules:

   1- Global and static variables are considered VARYING, unless they are
      declared const.

   2- Function arguments are considered VARYING.

   3- Any other value is considered UNDEFINED.  This is useful when
      considering PHI nodes.  PHI arguments that are undefined do not
      change the constant value of the PHI node, which allows for more
      constants to be propagated.  */

static value
get_default_value (tree var)
{
  value val;
  tree sym;

  sym = (!DECL_P (var)) ? get_base_symbol (var) : var;

  val.lattice_val = UNDEFINED;
  val.const_val = NULL_TREE;

  if (TREE_CODE (sym) == PARM_DECL || TREE_THIS_VOLATILE (sym))
    {
      /* Function arguments and volatile variables are considered VARYING.  */
      val.lattice_val = VARYING;
    }
  else if (decl_function_context (sym) == NULL_TREE || TREE_STATIC (sym))
    {
      /* Globals and static variables are considered VARYING, unless they
	 are declared 'const'.  */
      val.lattice_val = VARYING;

      if (TREE_READONLY (sym)
	  && DECL_INITIAL (sym)
	  && is_unchanging_value (DECL_INITIAL (sym)))
	{
	  val.lattice_val = CONSTANT;
	  val.const_val = DECL_INITIAL (sym);
	}
    }

  return val;
}


/* Fold builtin call FN in statement STMT.  If it cannot be folded into a
   constant, return NULL_TREE.  Otherwise, return its constant value.  */

static tree
ccp_fold_builtin (tree stmt, tree fn)
{
  tree result;
  tree arglist = TREE_OPERAND (fn, 1);
  tree callee = get_callee_fndecl (fn);

  /* Ignore MD builtins.  */
  if (DECL_BUILT_IN_CLASS (callee) == BUILT_IN_MD)
    return NULL_TREE;

  /* First try the generic builtin folder.  If that succeeds, return the
     result directly.  */
  result = fold_builtin (fn);
  if (result)
    return result;

  /* Otherwise, try to use the dataflow information gathered by the CCP
     process.  */
  switch (DECL_FUNCTION_CODE (callee))
    {
      case BUILT_IN_STRLEN:
	return get_strlen (TREE_VALUE (arglist));

      case BUILT_IN_FPUTS:
	return simplify_builtin_fputs (arglist,
				       TREE_CODE (stmt) != MODIFY_EXPR, 0,
				       get_strlen (TREE_VALUE (arglist)));
      case BUILT_IN_FPUTS_UNLOCKED:
	return simplify_builtin_fputs (arglist,
				       TREE_CODE (stmt) != MODIFY_EXPR, 1,
				       get_strlen (TREE_VALUE (arglist)));

      default:
	break;
    }


  return NULL_TREE;
}


/* Return the string length of ARG.  If ARG is an SSA name variable, follow
   its use-def chains.  If all the reaching definitions for VAR have the
   same length L, this function returns L.  Otherwise, it returns NULL_TREE
   indicating that the length cannot be determined statically.  */

static tree
get_strlen (tree arg)
{
  tree var, def_stmt;
  
  if (TREE_CODE (arg) != SSA_NAME)
    return c_strlen (arg, 1);

  var = arg;
  def_stmt = SSA_NAME_DEF_STMT (var);

  switch (TREE_CODE (def_stmt))
    {
      case MODIFY_EXPR:
	{
	  tree len, rhs;
	  
	  /* The RHS of the statement defining VAR must either have a
	     constant length or come from another SSA_NAME with a constant
	     length.  */
	  rhs = TREE_OPERAND (def_stmt, 1);
	  STRIP_NOPS (rhs);
	  if (TREE_CODE (rhs) == SSA_NAME)
	    return get_strlen (rhs);

	  /* See if the RHS is a constant length.  */
	  len = c_strlen (rhs, 1);
	  if (len)
	    {
	      /* Convert from the internal "sizetype" type to "size_t".  */
	      if (size_type_node)
		len = convert (size_type_node, len);
	      return len;
	    }

	  break;
	}

      case PHI_NODE:
	{
	  /* All the arguments of the PHI node must have the same constant
	     length.  */
	  int i;
	  tree arg_len, prev_arg_len;

	  arg_len = prev_arg_len = NULL_TREE;
	  for (i = 0; i < PHI_NUM_ARGS (def_stmt); i++)
	    {
	      arg_len = get_strlen (PHI_ARG_DEF (def_stmt, i));
	      if (arg_len == NULL_TREE)
		return NULL_TREE;

	      if (prev_arg_len
		  && simple_cst_equal (prev_arg_len, arg_len) != 1)
		return NULL_TREE;

	      prev_arg_len = arg_len;
	    }

	  return arg_len;
	}

      default:
	break;
    }


  return NULL_TREE;
}

#include "gt-tree-ssa-ccp.h"
