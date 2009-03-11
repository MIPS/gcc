/* This pass tries to simplify conditional expression made of a couple of
   conditional/unconditional branches if one of the two outgoing paths falls
   through to the next basic block.

   Copyright (C) 2006-2008 Free Software Foundation, Inc.

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
#include "flags.h"
#include "timevar.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "cil-stack.h"
#include "cil-stmt.h"
#include "cil-types.h"

/******************************************************************************
 * Local functions prototypes                                                 *
 ******************************************************************************/

static void simplify_cond_branch (cil_stmt_iterator *, cil_stack);
static unsigned int simp_cond (void);
static bool simp_cond_gate (void);

/******************************************************************************
 * Pass implementation                                                        *
 ******************************************************************************/

static void
simplify_cond_branch (cil_stmt_iterator *csi, cil_stack stack)
{
  cil_stmt stmt;
  cil_type_t type;
  enum cil_opcode opcode;
  tree label_then, label_else;
  basic_block then_bb;
  edge true_edge, false_edge;

  /* Extract the condition's edges.  */
  extract_true_false_edges_from_block (csi_bb (*csi), &true_edge, &false_edge);
  label_then = tree_block_label (true_edge->dest);
  label_else = tree_block_label (false_edge->dest);

  then_bb = label_to_block (label_then);

  /* Simplify the condition only if the 'then' edge can be turned into a
     fall through to the next basic block.  */
  if (csi_bb (*csi)->next_bb == then_bb)
    {
      type = cil_stack_top (stack);

      if (cil_integer_p (type) || cil_pointer_p (type))
	{
	  switch (cil_opcode (csi_stmt (*csi)))
	    {
	    case CIL_BEQ:    opcode = CIL_BNE_UN; break;
	    case CIL_BGE:    opcode = CIL_BLT;    break;
	    case CIL_BGE_UN: opcode = CIL_BLT_UN; break;
	    case CIL_BGT:    opcode = CIL_BLE;    break;
	    case CIL_BGT_UN: opcode = CIL_BLE_UN; break;
	    case CIL_BLE:    opcode = CIL_BGT;    break;
	    case CIL_BLE_UN: opcode = CIL_BGT_UN; break;
	    case CIL_BLT:    opcode = CIL_BGE;    break;
	    case CIL_BLT_UN: opcode = CIL_BGE_UN; break;
	    case CIL_BNE_UN: opcode = CIL_BEQ;    break;
	    default:
	      return; /* Do nothing, we cannot change this branch.  */
	    }

	  stmt = cil_build_stmt_arg (opcode, label_else);
	  csi_replace (csi, stmt);
	  csi_next (csi);
	  csi_remove (csi);

	  /* Invert the out-going edges */
	  true_edge->flags ^= (EDGE_TRUE_VALUE | EDGE_FALSE_VALUE);
	  false_edge->flags ^= (EDGE_TRUE_VALUE | EDGE_FALSE_VALUE);
	}
    }
}

/* Main function of this pass.  Looks for conditional expressions and
   inverts their condition if it allows removing the unconditional branch
   following it.  */

static unsigned int
simp_cond (void)
{
  basic_block bb;
  cil_stmt_iterator csi;
  cil_stmt stmt;
  enum cil_opcode opcode;
  cil_bb_stacks bbs;
  cil_stack stack;
  edge_iterator ei;
  edge e;

  bbs = cil_bb_stacks_create ();

  FOR_EACH_BB (bb)
    {
      stack = cil_stack_for_bb (bbs, bb);
      csi = csi_start_bb (bb);

      while (!csi_end_p (csi))
	{
	  stmt = csi_stmt (csi);
	  opcode = cil_opcode (stmt);

	  if (cil_cond_branch_p (stmt) && !csi_one_before_end_p (csi))
	    {
	      csi_next (&csi);

	      if (cil_opcode (csi_stmt (csi)) == CIL_BR)
		{
		  csi_prev (&csi);
		  simplify_cond_branch (&csi, stack);
		  break;
		}
	    }

	  cil_stack_after_stmt (stack, stmt);
	  csi_next (&csi);
	}

      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  cil_set_stack_for_bb (bbs, e->dest, stack);
	}
    }

  cil_bb_stacks_destroy (bbs);
  return 0;
}

/* Gate function of the peephole-optimizations pass.  */

static bool
simp_cond_gate (void)
{
  return true;
}

/* Define the parameters of the cond-simp pass.  */

struct tree_opt_pass pass_simp_cond =
{
  "cond_simp",                          /* name */
  simp_cond_gate,                       /* gate */
  simp_cond,                            /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_SIMP_COND,                         /* tv_id */
  PROP_cfg,                             /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,
  TODO_ggc_collect,                     /* todo_flags_finish */
  0                                     /* letter */
};

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
