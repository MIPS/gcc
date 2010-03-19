/* This pass tries to simplify conditional expression made of a couple of
   conditional/unconditional branches if one of the two outgoing paths falls
   through to the next basic block.

   Copyright (C) 2006-2010 Free Software Foundation, Inc.

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
   Thierry Lafage

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
Thierry Lafage          <thierry.lafage@inria.fr>
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

/* These 2 lines avoid including options.h */
extern int flag_cil32_simp_cond;
extern int flag_cil32_simp_cond_float;

/******************************************************************************
 * Local functions prototypes                                                 *
 ******************************************************************************/

static inline enum cil_opcode int_condbr_invert (enum cil_opcode);
static inline enum cil_opcode float_condbr_invert (enum cil_opcode);
static void simplify_cond_branch (cil_stmt_iterator *, cil_stack);
static unsigned int simp_cond (void);
static bool simp_cond_gate (void);

/******************************************************************************
 * Pass implementation                                                        *
 ******************************************************************************/

static inline enum cil_opcode
int_condbr_invert (enum cil_opcode condbr)
{
  switch (condbr)
  {
    case CIL_BEQ:     return CIL_BNE_UN;
    case CIL_BGE:     return CIL_BLT;
    case CIL_BGE_UN:  return CIL_BLT_UN;
    case CIL_BGT:     return CIL_BLE;
    case CIL_BGT_UN:  return CIL_BLE_UN;
    case CIL_BLE:     return CIL_BGT;
    case CIL_BLE_UN:  return CIL_BGT_UN;
    case CIL_BLT:     return CIL_BGE;
    case CIL_BLT_UN:  return CIL_BGE_UN;
    case CIL_BNE_UN:  return CIL_BEQ;
    case CIL_BRTRUE:  return CIL_BRFALSE;
    case CIL_BRFALSE: return CIL_BRTRUE;
    default:
      return condbr;  /* Do nothing, we cannot change this branch.  */
  }
}

static inline enum cil_opcode
float_condbr_invert (enum cil_opcode condbr)
{
  switch (condbr)
  {
    case CIL_BEQ:     return CIL_BNE_UN;
    case CIL_BGE:     return CIL_BLT_UN;
    case CIL_BGE_UN:  return CIL_BLT;
    case CIL_BGT:     return CIL_BLE_UN;
    case CIL_BGT_UN:  return CIL_BLE;
    case CIL_BLE:     return CIL_BGT_UN;
    case CIL_BLE_UN:  return CIL_BGT;
    case CIL_BLT:     return CIL_BGE_UN;
    case CIL_BLT_UN:  return CIL_BGE;
    case CIL_BNE_UN:  return CIL_BEQ;
    default:
      return condbr;  /* Do nothing, we cannot change this branch.  */
  }
}

static void
simplify_cond_branch (cil_stmt_iterator *csi, cil_stack stack)
{
  cil_stmt stmt;
  cil_type_t type;
  enum cil_opcode opcode, condbr;
  tree label_then, label_else;
  basic_block then_bb;
  edge true_edge, false_edge;

  /* Extract the condition's edges.  */
  extract_true_false_edges_from_block (csi_bb (*csi), &true_edge, &false_edge);
  label_then = gimple_block_label (true_edge->dest);
  label_else = gimple_block_label (false_edge->dest);

  then_bb = label_to_block (label_then);

  /* Simplify the condition only if the 'then' edge can be turned into a
     fall through to the next basic block.  */
  if (csi_bb (*csi)->next_bb == then_bb)
    {
      type = cil_stack_top (stack);
      opcode = condbr = cil_opcode (csi_stmt (*csi));

      if (flag_cil32_simp_cond && (cil_integer_p (type) || cil_pointer_p (type)))
	  opcode = int_condbr_invert (condbr);
      else if (flag_cil32_simp_cond_float && cil_float_p (type))
	  opcode = float_condbr_invert (condbr);

      if (opcode == condbr)
	return; /* No opcode change */

      stmt = cil_build_stmt_arg (opcode, label_else);
      csi_replace (csi, stmt);
      csi_next (csi);
      csi_remove (csi);

      /* Invert the out-going edges */
      true_edge->flags ^= (EDGE_TRUE_VALUE | EDGE_FALSE_VALUE);
      false_edge->flags ^= (EDGE_TRUE_VALUE | EDGE_FALSE_VALUE);
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
  return flag_cil32_simp_cond != 0 || flag_cil32_simp_cond_float != 0;
}

/* Define the parameters of the cond-simp pass.  */

struct gimple_opt_pass pass_simp_cond =
{
 {
  GIMPLE_PASS,                          /* type */
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
  0,                                    /* todo_flags_start */
  TODO_ggc_collect                      /* todo_flags_finish */
 }
};

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
