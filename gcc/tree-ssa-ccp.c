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
  tree const_val;
} value;

/* Hash table of constant values indexed by SSA name.   Each variable will
    be assigned a value out of the constant lattice: UNDEFINED (top),
    meaning that the variable has a constant of unknown value, CONSTANT,
    meaning that the variable has a known constant value and VARYING
    (bottom), meaning that the variable has a non-constant value.  */
static htab_t const_values;

/* Structure to map a variable to its constant value.  */
struct value_map_d
{
  tree var;
  value val;
};

/* A bitmap to keep track of executable blocks in the CFG.  */
static sbitmap executable_blocks;

/* Array of control flow edges on the worklist.  */
static varray_type cfg_edges;

/* Worklist of SSA edges which will need reexamination as their definition
    has changed.  SSA edges are def-use edges in the SSA web.  For each
    edge, we store the definition statement or PHI node D.  The destination
    nodes that need to be visited are accessed using immediate_uses (D).  */
static varray_type ssa_edges;

static void initialize			PARAMS ((void));
static void finalize			PARAMS ((void));
static void visit_phi_node		PARAMS ((tree));
static value cp_lattice_meet		PARAMS ((value, value));
static void visit_stmt			PARAMS ((tree));
static void visit_cond_stmt		PARAMS ((tree));
static void visit_assignment		PARAMS ((tree));
static void add_outgoing_control_edges	PARAMS ((basic_block));
static void add_control_edge		PARAMS ((edge));
static void def_to_undefined		PARAMS ((tree));
static void def_to_varying		PARAMS ((tree));
static void set_lattice_value		PARAMS ((tree, value));
static void simulate_block		PARAMS ((basic_block));
static void simulate_def_use_edges	PARAMS ((tree));
static void substitute_and_fold		PARAMS ((void));
static value evaluate_stmt		PARAMS ((tree));
static void dump_lattice_value		PARAMS ((FILE *, const char *, value));
static tree widen_bitfield		PARAMS ((tree, tree, tree));
static bool replace_uses_in		PARAMS ((tree));
static void fold_stmt			PARAMS ((tree));
static tree get_rhs			PARAMS ((tree));
static void set_rhs			PARAMS ((tree, tree));
static void set_value			PARAMS ((tree, latticevalue, tree));
static value *get_value			PARAMS ((tree));
static value get_default_value		PARAMS ((tree));
static hashval_t value_map_hash		PARAMS ((const void *));
static int value_map_eq			PARAMS ((const void *, const void *));


/* Debugging dumps.  */
static FILE *dump_file;
static int dump_flags;


/* Main entry point for SSA Conditional Constant Propagation.  FNDECL is
    the declaration for the function to optimize.  */

void
tree_ssa_ccp (fndecl)
     tree fndecl;
{
  timevar_push (TV_TREE_CCP);

  initialize ();

  /* Iterate until the worklists are empty.  */
  while (VARRAY_ACTIVE_SIZE (cfg_edges) > 0
	 || VARRAY_ACTIVE_SIZE (ssa_edges) > 0)
    {
      if (VARRAY_ACTIVE_SIZE (cfg_edges) > 0)
	{
	  /* Pull the next block to simulate off the worklist.  */
	  basic_block dest_block;
	  dest_block = ((edge)VARRAY_TOP_GENERIC_PTR (cfg_edges))->dest;
	  VARRAY_POP (cfg_edges);
	  simulate_block (dest_block);
	}

      if (VARRAY_ACTIVE_SIZE (ssa_edges) > 0)
	{
	  /* Pull the next reference off the worklist.  The SSA edges
	     worklist stores the origination definition for each edge.  */
	  tree def = VARRAY_TOP_TREE (ssa_edges);
	  VARRAY_POP (ssa_edges);
	  simulate_def_use_edges (def);
	}
    }

  /* Now perform substitutions based on the known constant values.  */
  substitute_and_fold ();

  /* Now cleanup any unreachable code.  */
  cleanup_tree_cfg ();

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

      dump_end (TDI_ccp, dump_file);
    }

  dump_function (TDI_ccp, fndecl);
}


/* Simulate the execution of BLOCK.  Evaluate the statement associated
   with each variable reference inside the block.  */

static void
simulate_block (block)
     basic_block block;
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
      gimple_stmt_iterator j;

      /* Note that we have simulated this block.  */
      SET_BIT (executable_blocks, block->index);

      for (j = gsi_start_bb (block); !gsi_end_bb_p (j); gsi_step_bb (&j))
	visit_stmt (gsi_stmt (j));

      /* If the block has a single successor, it will always get executed.
	 Add it to the worklist.  */
      if (block->succ && block->succ->succ_next == NULL)
	add_control_edge (block->succ);
    }
}


/* Follow the def-use edges for statement DEF_STMT and simulate all the
   statements reached by it.  */

static void
simulate_def_use_edges (def_stmt)
     tree def_stmt;
{
  size_t i;
  varray_type imm_uses;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nSimulating def-use edges for statement: ");
      print_generic_stmt (dump_file, def_stmt, 0);
    }

  imm_uses = immediate_uses (def_stmt);
  if (imm_uses)
    for (i = 0; i < VARRAY_ACTIVE_SIZE (imm_uses); i++)
      {
	tree use_stmt = VARRAY_TREE (imm_uses, i);
	basic_block use_bb = bb_for_stmt (use_stmt);

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
}


/* Perform final substitution and folding.  After this pass the program
   should still be in SSA form.  */

static void
substitute_and_fold ()
{
  basic_block bb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nSubstituing constants and folding statements\n\n");

  /* Substitute constants in every statement of every basic block.  */
  FOR_EACH_BB (bb)
    {
      gimple_stmt_iterator i;

      for (i = gsi_start_bb (bb); !gsi_end_bb_p (i); gsi_step_bb (&i))
	{
	  tree stmt = gsi_stmt (i);

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
	      fold_stmt (stmt);
	      modify_stmt (stmt);
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
visit_phi_node (phi)
     tree phi;
{
  int i;
  value phi_val;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nVisiting PHI node: ");
      print_generic_expr (dump_file, phi, 0);
    }

  phi_val.lattice_val = UNDEFINED;
  phi_val.const_val = NULL_TREE;

  if (!TREE_THIS_VOLATILE (SSA_NAME_VAR (PHI_RESULT (phi))))
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

	    if (!TREE_THIS_VOLATILE (SSA_NAME_VAR (rdef)))
	      {
		value *rdef_val = get_value (rdef);
		phi_val = cp_lattice_meet (phi_val, *rdef_val);

		if (dump_file && (dump_flags & TDF_DETAILS))
		  {
		    fprintf (dump_file, "\t");
		    print_generic_expr (dump_file, rdef, 0);
		    dump_lattice_value (dump_file, "\tValue: ", *rdef_val);
		    fprintf (dump_file, "\n");
		  }
	      }
	    else
	      {
		/* If the variable is volatile, we cannot assume anything
		   about this PHI's node value.  In that case, set its
		   value to VARYING.  */
		phi_val.lattice_val = VARYING;
		phi_val.const_val = NULL_TREE;
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
}


/* Compute the meet operator between VAL1 and VAL2:

   		any M UNDEFINED = any
		any M VARYING	= VARYING
		Ci  M Cj	= Ci		if (i == j)
		Ci  M Cj	= VARYING	if (i != j)  */
static value
cp_lattice_meet (val1, val2)
     value val1;
     value val2;
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
visit_stmt (stmt)
     tree stmt;
{
  varray_type ops;
  size_t i;

#if defined ENABLE_CHECKING
  /* FIXME: This is lame.  All statements should be in GIMPLE form.  */
  if (TREE_NOT_GIMPLE (stmt))
    return;
#endif

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nVisiting statement: ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }
  
  /* Now examine the statement.  If the statement produces an output
     value, evaluate it to see if the lattice value of its output has
     changed.  */
  if (def_op (stmt))
    visit_assignment (stmt);

  /* If STMT is a control statement, see if we can determine which branch
     will be taken.  */
  else if (is_ctrl_stmt (stmt))
    visit_cond_stmt (stmt);

  /* If STMT is a computed goto, mark all the output edges
     executable.  */
  else if (is_computed_goto (stmt))
    add_outgoing_control_edges (bb_for_stmt (stmt));

  /* Mark all VDEF operands VARYING.  */
  ops = vdef_ops (stmt);
  for (i = 0; ops && i < VARRAY_ACTIVE_SIZE (ops); i++)
    set_value (VDEF_RESULT (VARRAY_TREE (ops, i)), VARYING, NULL_TREE);
}


/* Visit the assignment statement STMT.  Set the value of its LHS to the
   value computed by the RHS.  */

static void
visit_assignment (stmt)
     tree stmt;
{
  value val;

#if defined ENABLE_CHECKING
  if (!def_op (stmt))
    abort ();
#endif

  /* Evaluate the statement.  */
  val = evaluate_stmt (stmt);

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
  set_lattice_value (*(def_op (stmt)), val);
}


/* Visit the conditional statement STMT.  If it evaluates to a constant value,
   mark outgoing edges appropriately.  */

static void
visit_cond_stmt (stmt)
     tree stmt;
{
  edge e;
  value val;
  basic_block block;

#if defined ENABLE_CHECKING
  if (!is_ctrl_stmt (stmt))
    abort ();
#endif

  block = bb_for_stmt (stmt);
  val = evaluate_stmt (stmt);

  /* If the predicate is undefined, do nothing.  */
  if (val.lattice_val == UNDEFINED)
    return;

  /* Find which edge out of the conditional block will be taken and add it
     to the worklist.  If no single edge can be determined statically, add
     all outgoing edges from BLOCK.  */
  e = find_taken_edge (block, val.const_val);
  if (e)
    add_control_edge (e);
  else
    add_outgoing_control_edges (block);
}


/* Add all the edges coming out of BB to the control flow worklist.  */

static void
add_outgoing_control_edges (bb)
     basic_block bb;
{
  edge e;

  for (e = bb->succ; e; e = e->succ_next)
    add_control_edge (e);
}


/* Add edge E to the control flow worklist.  */

static void
add_control_edge (e)
     edge e;
{
  /* If the edge had already been added, skip it.  */
  if (e->flags & EDGE_EXECUTABLE)
    return;

  e->flags |= EDGE_EXECUTABLE;
  VARRAY_PUSH_GENERIC_PTR (cfg_edges, e);

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Adding edge (%d -> %d) to worklist\n\n", e->src->index,
	     e->dest->index);
}


/* Evaluate statement STMT.  */

static value
evaluate_stmt (stmt)
     tree stmt;
{
  value val;
  tree simplified, copy;

  val.lattice_val = VARYING;
  val.const_val = NULL_TREE;

  /* Evaluate a copy of the original statement.  */
  copy = copy_stmt (stmt);
  if (replace_uses_in (copy))
    fold_stmt (copy);

  /* Extract the folded value from the statement.  */
  simplified = get_rhs (copy);
  if (simplified && really_constant_p (simplified))
    {
      val.lattice_val = CONSTANT;
      val.const_val = simplified;
    }

  /* Debugging dumps.  */
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Statement evaluates to ");
      print_generic_stmt (dump_file, copy, TDF_SLIM);
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
      print_generic_expr (outf, val.const_val, 0);
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
initialize ()
{
  edge e;
  basic_block bb;

  /* Initialize debugging dumps.  */
  dump_file = dump_begin (TDI_ccp, &dump_flags);

  /* Initialize the values array with everything as undefined.  */
  const_values = htab_create (50, value_map_hash, value_map_eq, NULL);

  /* Compute immediate uses.  */
  compute_immediate_uses (TDFA_USE_OPS);

  if (dump_file && dump_flags & TDF_DETAILS)
    dump_immediate_uses (dump_file);

  /* Worklist of SSA edges.  */
  VARRAY_TREE_INIT (ssa_edges, 20, "ssa_edges");

  executable_blocks = sbitmap_alloc (last_basic_block);
  sbitmap_zero (executable_blocks);

  /* Mark all edges not executable.  */
  FOR_EACH_BB (bb)
    for (e = bb->succ; e; e = e->succ_next)
      e->flags &= ~EDGE_EXECUTABLE;

  VARRAY_GENERIC_PTR_INIT (cfg_edges, 20, "cfg_edges");

  /* Seed the algorithm by adding the successors of the entry block to the
     edge worklist.  */
  for (e = ENTRY_BLOCK_PTR->succ; e; e = e->succ_next)
    {
      e->flags |= EDGE_EXECUTABLE;
      VARRAY_PUSH_GENERIC_PTR (cfg_edges, e);
    }
}


/* Free allocated storage.  */

static void
finalize ()
{
  htab_delete (const_values);
}


/* Set the lattice value for the variable VAR to UNDEFINED.  */

static void
def_to_undefined (var)
     tree var;
{
  if (get_value (var)->lattice_val != UNDEFINED)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Lattice value changed.  Adding definition to SSA edges.\n");

      VARRAY_PUSH_TREE (ssa_edges, SSA_NAME_DEF_STMT (var));
    }

  set_value (var, UNDEFINED, NULL_TREE);
}


/* Set the lattice value for the variable VAR to VARYING.  */

static void
def_to_varying (var)
     tree var;
{
  if (get_value (var)->lattice_val != VARYING)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Lattice value changed.  Adding definition to SSA edges.\n");

      VARRAY_PUSH_TREE (ssa_edges, SSA_NAME_DEF_STMT (var));
    }

  set_value (var, VARYING, NULL_TREE);
}


/* Set the lattice value for variable VAR to VAL.  */

static void
set_lattice_value (var, val)
     tree var;
     value val;
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

	  VARRAY_PUSH_TREE (ssa_edges, SSA_NAME_DEF_STMT (var));
	  set_value (var, CONSTANT, val.const_val);
	}
    }
}


/* Replace USE references in statement STMT with their immediate reaching
   definition.  Return true if at least one reference was replaced.  */

static bool
replace_uses_in (stmt)
     tree stmt;
{
  bool replaced = false;
  varray_type uses;
  size_t i;

  get_stmt_operands (stmt);

  uses = use_ops (stmt);
  for (i = 0; uses && i < VARRAY_ACTIVE_SIZE (uses); i++)
    {
      tree *use = VARRAY_GENERIC_PTR (uses, i);
      value *val = get_value (*use);

      if (val->lattice_val == CONSTANT)
	{
	  *use = val->const_val;
	  replaced = true;
	}
    }

  return replaced;
}


/* Fold statement STMT.  */

static void
fold_stmt (stmt)
     tree stmt;
{
  tree rhs, result;

  STRIP_NOPS (stmt);
  rhs = get_rhs (stmt);
  if (rhs)
    {
      result = fold (rhs);
      set_rhs (stmt, result);
    }
}


/* Get the main expression from statement STMT.  */

static tree
get_rhs (stmt)
     tree stmt;
{
  enum tree_code code = TREE_CODE (stmt);

  if (code == MODIFY_EXPR || code == INIT_EXPR)
    return TREE_OPERAND (stmt, 1);
  if (code == COND_EXPR)
    return COND_EXPR_COND (stmt);
  else if (code == SWITCH_EXPR)
    return SWITCH_COND (stmt);
  else if (code == RETURN_EXPR)
    return TREE_OPERAND (stmt, 0);
  else if (code == GOTO_EXPR)
    return GOTO_DESTINATION (stmt);
  else if (code == LABEL_EXPR)
    return LABEL_EXPR_LABEL (stmt);
  else
    return stmt;
}


/* Set the main expression of STMT to EXPR.  */

static void
set_rhs (stmt, expr)
     tree stmt;
     tree expr;
{
  enum tree_code code = TREE_CODE (stmt);

  if (code == MODIFY_EXPR || code == INIT_EXPR)
    TREE_OPERAND (stmt, 1) = expr;
  if (code == COND_EXPR)
    COND_EXPR_COND (stmt) = expr;
  else if (code == SWITCH_EXPR)
    SWITCH_COND (stmt) = expr;
  else if (code == RETURN_EXPR)
    TREE_OPERAND (stmt, 0) = expr;
  else if (code == GOTO_EXPR)
    GOTO_DESTINATION (stmt) = expr;
  else if (code == LABEL_EXPR)
    LABEL_EXPR_LABEL (stmt) = expr;
}


/* Get the constant value associated with variable VAR.  */

static value *
get_value (var)
     tree var;
{
  void **slot;
  struct value_map_d *vm_p, vm;

#if defined ENABLE_CHECKING
  if (TREE_CODE (var) != SSA_NAME)
    abort ();
#endif

  vm.var = var;
  slot = htab_find_slot (const_values, (void *) &vm, INSERT);
  if (*slot == NULL)
    {
      /* If this is the first time that we need the value for VAR and we
	 still have not seen a definition for it, assume a default value
	 accordingly (see get_default_value).  */
      vm_p = xmalloc (sizeof (*vm_p));
      vm_p->var = var;
      vm_p->val = get_default_value (var);
      *slot = (void *) vm_p;
    }
  else
    vm_p = (struct value_map_d *) *slot;

  return &(vm_p->val);
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
get_default_value (var)
     tree var;
{
  value val;

  if (!DECL_P (var))
    var = get_base_symbol (var);

  val.lattice_val = UNDEFINED;
  val.const_val = NULL_TREE;

  if (TREE_CODE (var) == PARM_DECL)
    {
      /* Function arguments are considered VARYING.  */
      val.lattice_val = VARYING;
    }
  else if (decl_function_context (var) == NULL_TREE || TREE_STATIC (var))
    {
      /* Globals and static variables are considered VARYING.  */
      val.lattice_val = VARYING;

      /* Except if they are declared 'const'.  */
      if (TREE_READONLY (var)
	  && DECL_INITIAL (var)
	  && really_constant_p (DECL_INITIAL (var)))
	{
	  val.lattice_val = CONSTANT;
	  val.const_val = DECL_INITIAL (var);
	}
    }

  return val;
}


/* Set the value for variable VAR to <LATTICE_VAL, CONST_VAL>.  */

static void
set_value (var, lattice_val, const_val)
     tree var;
     latticevalue lattice_val;
     tree const_val;
{
  void **slot;
  struct value_map_d *vm_p, vm;

#if defined ENABLE_CHECKING
  if (TREE_CODE (var) != SSA_NAME)
    abort ();
#endif

  vm.var = var;
  slot = htab_find_slot (const_values, (void *) &vm, INSERT);
  if (*slot == NULL)
    {
      vm_p = xmalloc (sizeof (*vm_p));
      vm_p->var = var;
      *slot = (void *) vm_p;
    }
  else
    vm_p = (struct value_map_d *) *slot;

  vm_p->val.lattice_val = lattice_val;
  vm_p->val.const_val = const_val;
}


/* Hash and compare functions for CONST_VALUES.  */

static hashval_t
value_map_hash (p)
     const void *p;
{
  return htab_hash_var ((const void *)((const struct value_map_d *)p)->var);
}

static int
value_map_eq (p1, p2)
     const void *p1;
     const void *p2;
{
  return same_var_p (((const struct value_map_d *)p1)->var,
		     ((const struct value_map_d *)p2)->var);
}
