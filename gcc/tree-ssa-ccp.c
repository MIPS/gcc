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
   edge, we store the originating V_DEF/V_PHI reference D.  The destination
   nodes that need to be visited are accessed using imm_uses (D).  */
ref_list ssa_edges;

static void initialize			PARAMS ((void));
static void finalize			PARAMS ((void));
static void visit_phi_node		PARAMS ((tree_ref));
static value cp_lattice_meet		PARAMS ((value, value));
static void visit_stmt			PARAMS ((tree));
static void visit_cond_stmt		PARAMS ((tree));
static void visit_assignment		PARAMS ((tree));
static void add_outgoing_control_edges	PARAMS ((basic_block));
static void add_control_edge		PARAMS ((edge));
static void def_to_undefined		PARAMS ((tree_ref));
static void def_to_varying		PARAMS ((tree_ref));
static void set_lattice_value		PARAMS ((tree_ref, value));
static void simulate_block		PARAMS ((basic_block));
static void simulate_def_use_chains	PARAMS ((tree_ref));
static void substitute_and_fold		PARAMS ((void));
static value evaluate_stmt		PARAMS ((tree));
static void dump_lattice_value		PARAMS ((FILE *, const char *, value));
static tree widen_bitfield		PARAMS ((tree, tree, tree));
static bool replace_uses_in		PARAMS ((tree, int));
static void fold_stmt			PARAMS ((tree));
static tree get_rhs			PARAMS ((tree));
static void set_rhs			PARAMS ((tree, tree));


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
  timevar_push (TV_TREE_CCP);

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
	  tree_ref def = get_last_ref (ssa_edges);
	  remove_ref_from_list (ssa_edges, def);
	  simulate_def_use_chains (def);
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
	  compute_reaching_defs ();
	  dump_referenced_vars (dump_file, 1);
	  fprintf (dump_file, "\n\n");
	  dump_reaching_defs (dump_file);
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
  ref_list blockrefs;
  ref_list_iterator i;

  /* There is nothing to do for the exit block.  */
  if (block == EXIT_BLOCK_PTR)
    return;

#if defined ENABLE_CHECKING
  if (block->index < 0 || block->index > last_basic_block)
    abort ();
#endif

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nSimulating block %d\n", block->index);

  /* Always simulate PHI nodes, even if we have simulated this block
     before.  Note that all PHI nodes are consecutive within a block.  */
  blockrefs = bb_refs (block);
  for (i = rli_start (blockrefs); !rli_after_end (i); rli_step (&i))
    if (ref_type (rli_ref (i)) == V_PHI)
      visit_phi_node (rli_ref (i));

  /* If this is the first time we've simulated this block, then we
     must simulate each of its statements.  */
  if (!TEST_BIT (executable_blocks, block->index))
    {
      gimple_stmt_iterator j;

      /* Note that we have simulated this block.  */
      SET_BIT (executable_blocks, block->index);

      for (j = gsi_start_bb (block); !gsi_end_bb (j); gsi_step_bb (&j))
	visit_stmt (gsi_stmt (j));

      /* If the block has a single successor, it will always get executed.
	 Add it to the worklist.  */
      if (block->succ && block->succ->succ_next == NULL)
	add_control_edge (block->succ);
    }
}


/* Follow the def-use chains for definition DEF and simulate all the
   statements reached by it.  */

static void
simulate_def_use_chains (def)
     tree_ref def;
{
  ref_list_iterator i;

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_ref (dump_file, "\nSimulating def-use edges for definition: ",
		 def, 0, 0);

  for (i = rli_start (imm_uses (def)); !rli_after_end (i); rli_step (&i))
    {
      tree_ref ref = rli_ref (i);

      /* Visit the statement containing the use reached by DEF, only if the
	 destination block is marked executable.  Note that we only visit
	 unmodified V_USE references.  We don't want to deal with any
	 modifiers here.  */
      if (is_pure_use (ref)
	  && TEST_BIT (executable_blocks, ref_bb (ref)->index))
	visit_stmt (ref_stmt (ref));

      /* PHI nodes are always visited, regardless of whether or not the
	 destination block is executable.  */
      else if (ref_type (ref) == V_PHI)
	visit_phi_node (ref);
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

      for (i = gsi_start_bb (bb); !gsi_end_bb (i); gsi_step_bb (&i))
	{
	  tree stmt = gsi_stmt (i);

	  /* Skip statements that have been folded already.  */
	  if (tree_flags (stmt) & TF_FOLDED
	      || !is_exec_stmt (stmt))
	    continue;

	  /* Replace the statement with its folded version and mark it
	     folded.  */
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Line %d: replaced ", get_lineno (stmt));
	      print_generic_stmt (dump_file, stmt, TDF_SLIM);
	    }

	  if (replace_uses_in (stmt, true))
	    {
	      fold_stmt (stmt);
	      set_tree_flag (stmt, TF_FOLDED);
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
visit_phi_node (phi_node)
     tree_ref phi_node;
{
  unsigned int i;
  value phi_val;

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_ref (dump_file, "\nVisiting PHI node: ", phi_node, 0, 0);

  phi_val.lattice_val = UNDEFINED;
  phi_val.const_value = NULL_TREE;

  if (!is_volatile_ref (phi_node))
    for (i = 0; i < num_phi_args (phi_node); i++)
      {
	/* Compute the meet operator over all the PHI arguments. */
	phi_node_arg arg = phi_arg (phi_node, i);
	edge e = phi_arg_edge (arg);

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
	    tree_ref rdef;
	    
	    rdef = phi_arg_def (arg);

	    if (is_killing_def (rdef, ref_var (phi_node))
		&& !is_volatile_ref (rdef))
	      {
		value rdef_val;

		rdef_val = values[ref_id (rdef)];
		phi_val = cp_lattice_meet (phi_val, rdef_val);

		if (dump_file && (dump_flags & TDF_DETAILS))
		  {
		    dump_ref (dump_file, "\t", phi_arg_def (arg), 0, 0);
		    dump_lattice_value (dump_file, "\tValue: ", rdef_val);
		    fprintf (dump_file, "\n");
		  }
	      }
	    else
	      {
		/* If RDEF is a non-killing definition, we cannot assume
		   anything about this PHI's node value.  In that case, set
		   its value to VARYING.  */
		phi_val.lattice_val = VARYING;
		phi_val.const_value = NULL_TREE;
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

  set_lattice_value (phi_node, phi_val);
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
      result.const_value = NULL_TREE;
      return result;
    }

  /* Ci M Cj = Ci	if (i == j)
     Ci M Cj = VARYING	if (i != j)  */
  if (simple_cst_equal (val1.const_value, val2.const_value) == 1)
    {
      result.lattice_val = CONSTANT;
      result.const_value = val1.const_value;
    }
  else
    {
      result.lattice_val = VARYING;
      result.const_value = NULL_TREE;
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
  /* FIXME: This is lame.  All statements should be in GIMPLE form.  */
  if (TREE_NOT_GIMPLE (stmt))
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nVisiting statement: ");
      print_generic_stmt (dump_file, stmt, TDF_SLIM);
      fprintf (dump_file, "\n");
    }
  
  /* Now examine the statement.  If the statement produces an output
     value, evaluate it to see if the lattice value of its output has
     changed.  */
  if (is_assignment_stmt (stmt))
    visit_assignment (stmt);

  /* If STMT is a control statement, see if we can determine which branch
     will be taken.  */
  else if (is_ctrl_stmt (stmt))
    visit_cond_stmt (stmt);

  /* If STMT is a computed goto, mark all the output edges
     executable.  */
  else if (is_computed_goto (stmt))
    add_outgoing_control_edges (bb_for_stmt (stmt));
}


/* Visit the assignment statement STMT.  Set the value of its LHS to the
   value computed by the RHS.  */

static void
visit_assignment (stmt)
     tree stmt;
{
  value val;

#if defined ENABLE_CHECKING
  if (!is_assignment_stmt (stmt))
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
	tree w = widen_bitfield (val.const_value, TREE_OPERAND (lhs, 1), lhs);

	if (w)
	  val.const_value = w;
	else
	  {
	    val.lattice_val = VARYING;
	    val.const_value = NULL;
	  }
      }
  }

  /* Set the lattice value of the statement's output.  */
  set_lattice_value (output_ref (stmt), val);
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
  e = find_taken_edge (block, val.const_value);
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
  VARRAY_PUSH_GENERIC_PTR (edge_info, e);

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
  val.const_value = NULL_TREE;

  /* Evaluate a copy of the original statement.  */
  STRIP_NOPS (stmt);
  copy = stmt;
  walk_tree (&copy, copy_tree_r, NULL, NULL);
  if (replace_uses_in (copy, false))
    fold_stmt (copy);

  /* Extract the folded value from the statement.  */
  simplified = get_rhs (copy);
  if (simplified && really_constant_p (simplified))
    {
      val.lattice_val = CONSTANT;
      val.const_value = simplified;
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
      print_generic_expr (outf, val.const_value, 0);
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
  size_t i;
  edge curredge;

  /* Initialize debugging dumps.  */
  dump_file = dump_begin (TDI_ccp, &dump_flags);

  /* Build an edge list from the CFG.  */
  edges = create_edge_list ();

  /* Initialize the values array with everything as undefined.  */
  values = (value *) xmalloc (next_tree_ref_id * sizeof (value));
  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      ref_list_iterator j;

      for (j = rli_start (tree_refs (var)); !rli_after_end (j); rli_step (&j))
	{
	  tree_ref r = rli_ref (j);
	  unsigned long id = ref_id (r);

	  values[id].lattice_val = UNDEFINED;
	  values[id].const_value = NULL_TREE;

	  /* If this reference is the declaration of a static variable, set
	     its lattice value to VARYING.  Static initializers are only
	     executed the first time the function is called.  We could
	     assume them constant if there are no other definitions to the
	     variable in the function, but that hardly seems worth it.  */
	  if (is_initializing_def (r))
	    {
	      values[id].lattice_val = VARYING;

	      /* Exception.  If this is a 'const' declaration, consider the
		 reference constant.  */
	      if (TREE_READONLY (var) && really_constant_p (DECL_INITIAL (var)))
		{
		  values[id].lattice_val = CONSTANT;
		  values[id].const_value = DECL_INITIAL (var);
		}
	    }

	  /* Special definitions (clobbering, partial or may-def) are all
	     considered VARYING.  */
	  else if (!is_pure_def (r))
	    {
	      values[id].lattice_val = VARYING;
	      values[id].const_value = NULL_TREE;
	    }
	}
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
     tree_ref ref;
{
  if (values[ref_id (ref)].lattice_val != UNDEFINED)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Lattice value changed.  Adding definition to SSA edges.\n");

      add_ref_to_list_end (ssa_edges, ref);
    }

  values[ref_id (ref)].lattice_val = UNDEFINED;
}


/* Set the lattice value for the variable defined by REF to VARYING.  */

static void
def_to_varying (ref)
     tree_ref ref;
{
  if (values[ref_id (ref)].lattice_val != VARYING)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Lattice value changed.  Adding definition to SSA edges.\n");

      add_ref_to_list_end (ssa_edges, ref);
    }

  values[ref_id (ref)].lattice_val = VARYING;
}


/* Set the lattice value for reference DEF to VAL.  */

static void
set_lattice_value (def, val)
     tree_ref def;
     value val;
{
  if (val.lattice_val == UNDEFINED)
    def_to_undefined (def);
  else if (val.lattice_val == VARYING)
    def_to_varying (def);
  else
    {
      /* If the RHS is a constant value that is different from a previous
	 value for this reference, add its SSA edge to the worklist.  */
      if (values[ref_id (def)].lattice_val != CONSTANT
	  || !(simple_cst_equal (values[ref_id (def)].const_value,
	                         val.const_value)) == 1)
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Lattice value changed to ");
	      print_generic_expr (dump_file, val.const_value, 0);
	      fprintf (dump_file, ".  Adding definition to SSA edges.\n");
	    }

	  add_ref_to_list_end (ssa_edges, def);
	  values[ref_id (def)].lattice_val = CONSTANT;
	  values[ref_id (def)].const_value = val.const_value;
	}
    }
}


/* Replace USE references in statement STMT with their immediate reaching
   definition.  Return true if at least one reference was replaced.  If
   COMMIT is nonzero, the SSA web is also updated to remove all the
   replaced references (used only in the final phase of the algorithm)  */

static bool
replace_uses_in (stmt, commit)
     tree stmt;
     int commit;
{
  bool replaced = false;
  ref_list refs = tree_refs (stmt);
  ref_list_iterator i;

  for (i = rli_start (refs); !rli_after_end (i); rli_step (&i))
    {
      tree_ref use = rli_ref (i);
      HOST_WIDE_INT rdef_id;
      tree_ref rdef;

      /* Only replace unmodified V_USE references.  */
      if (!is_pure_use (use))
	continue;

      /* The lattice value of a USE reference is the value of its immediately
	 reaching definition.  */
      rdef = imm_reaching_def (use);

      /* We are only interested in killing definitions.  If we are reached
	 by a definition of a may-alias of USE's var, we can't assume
	 anything about it.  */
      if (rdef == NULL
	  || !is_killing_def (rdef, ref_var (use))
	  || is_volatile_def (rdef))
	continue;

      rdef_id = ref_id (rdef);
      if (values[rdef_id].lattice_val == CONSTANT)
	{
	  if (commit)
	    replace_ref_with (use, values[rdef_id].const_value);
	  else
	    try_replace_ref_with (stmt, use, values[rdef_id].const_value);

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
