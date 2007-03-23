/* Search for variables spilled to the stack by reload, coalesce them or
 * move the stack pointer in between accesses 
   Copyright (C) 1987, 1988, 1989, 1992, 1993, 1994, 1995, 1996, 1997, 1998,
   1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006  Free Software Foundation,
   Inc.

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
02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tm_p.h"
#include "insn-config.h"
#include "expr.h"
#include "optabs.h"
#include "recog.h"
#include "reload.h"
#include "regs.h"
#include "addresses.h"
#include "hard-reg-set.h"
#include "flags.h"
#include "real.h"
#include "output.h"
#include "function.h"
#include "toplev.h"
#include "params.h"
#include "target.h"
#include "machmode.h"
#include "tree-pass.h"
#include "timevar.h"
#include "vecprim.h"
#include "tree.h"
#include "tm_p.h"
#include "ggc.h"
#include "basic-block.h"
#include "output.h"
#include "expr.h"
#include "function.h"
#include "diagnostic.h"
#include "timevar.h"
#include "tree-dump.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "tree-ssa-live.h"
#include "langhooks.h"
#include "domwalk.h"
#include "tree-dump.h"



void
x (rtx rtx)
{
  print_rtl_single (stderr, rtx);
}

void
xx (rtx rtx)
{
  x(rtx);
}

#if 0

static rtx
get_offset (rtx mem)
{
  if (!MEM_P (mem))
    return NULL;

  rtx plus = XEXP (mem, 0);
  if (GET_CODE (plus) != PLUS)
    return NULL;

  rtx bp = XEXP (plus, 0);
  if (!REG_P (bp))
    return NULL;

  rtx offset = XEXP (plus, 1);
  if (GET_CODE (offset) != CONST_INT)
    return NULL;

  return offset;

}

static int
int_offset (rtx mem)
{
  rtx offset = get_offset (mem);
  gcc_assert (offset);
  return INTVAL (offset);
}

/* TODO SIZE should be max frame slot / 4, and other things should be
 * offset */
int size = -1;
int top_of_saved_regs = INT_MIN;

/* This macro prevents us from using the inverted column/row numbers. Its
 * a macro so we can assign to it */
#define INTERFERES(A,B) interference_graph [A>B?B:A] [A>B?A:B]

static rtx* live;
bool **interference_graph;
int *interference_count;
static rtx* all_slots;
static rtx* final_use;
static rtx* def;


static int
sind (rtx slot) /* short for slot index */
{
  return (-(int_offset(slot) - top_of_saved_regs) / 4);
}

static void
init_data_structs (void)
{
  int i;
  /* it is probably a lot more complicated than this */
  size = get_frame_size () / 4;
  live = XCNEWVEC (rtx, size);
  all_slots = XCNEWVEC (rtx, size);
  final_use = XCNEWVEC (rtx, size);
  def = XCNEWVEC (rtx, size);
  interference_graph = XCNEWVEC (bool*, size);
  interference_count = XCNEWVEC (int, size);
  for (i = 0; i < size; i++)
    {
      interference_graph [i] = XCNEWVEC (bool, size);
    }
}

static bool
is_live (rtx slot)
{
  return live [sind (slot)] != NULL;
}

static void
add_to_interference_list (rtx slot1, rtx slot2)
{
  gcc_assert (slot1);
  gcc_assert (slot2);
  int off1 = sind (slot1);
  int off2 = sind (slot2);
  INTERFERES (off1, off2) = true;
  interference_count [off1] ++;
  interference_count [off2] ++;
}

static void
mark_live (rtx insn, rtx slot)
{
  int i;
  int offset = sind (slot);

  if (live [offset]) /* avoid duplicate info */
    return;

  live [offset] = slot;
  final_use [offset] = insn;

  /* add all live vars to "interference graph" */
  for (i = 0; i < size; i++)
    {
      if (live [i])
	{
	  add_to_interference_list (slot, live [i]);
	  fprintf (stderr, "%d(bp) is live at the same time as %d(bp)\n",
		   int_offset (slot),
		   int_offset (live [i]));
	}
    }
}

/* integrate slot2 into slot1. Make slot1 interfere everywhere */
static void
coalesce (rtx slot1, rtx slot2)
{
  int i;
  rtx insn;
  gcc_assert (sind (slot1) < sind (slot2));

  int off1 = sind (slot1);
  int off2 = sind (slot2);

  /* Combine the interference graphs */
  for (i = 0; i < size; i++)
    {
      if (INTERFERES (off2, i))
	  INTERFERES (off1, i) = true;

      INTERFERES (off2, i) = true;
    }
  all_slots [off2] = NULL;

  /* rewrite the insns to use the new address. CONST_INTs are shared
   * throughout RTL, so safe to just replace in the insns. */
 for (insn = get_last_insn (); insn; insn = PREV_INSN (insn))
    {
      if (!INSN_P (insn))
	continue;

      rtx pattern = PATTERN (insn);
      if (GET_CODE (pattern) != SET)
	continue;

      rtx dest = SET_DEST (pattern);
      rtx src = SET_SRC (pattern);
      rtx offset;
      if (REG_P (src) 
	  && (offset = get_offset (dest))
	  && (INTVAL (offset) == INTVAL (get_offset (slot2))))
	XEXP (XEXP (dest, 0), 1) = 
	  XEXP (XEXP (slot1, 0), 1);

      else if (REG_P (dest) 
	       && (offset = get_offset (src))
	       && (INTVAL (offset) == INTVAL (get_offset (slot2))))
	XEXP (XEXP (src, 0), 1) = 
	  XEXP (XEXP (slot1, 0), 1);
    }

}

static void
handle_non_interfering_slots (void)
{
  rtx insn;
/* does simple coalescing */
  /*
  int i, j;
  for (i = 0; i < size; i++)
    {
      for (j = 0; j < size; j++)
	{
	  if (!all_slots [j]) continue;
	  if (!INTERFERES (i, j) && j > i)
	    {
	      fprintf (stderr, "Coalesce %d(bp) and %d(bp) together\n",
		       int_offset (all_slots [i]),
		       int_offset (all_slots [j]));
	      coalesce (all_slots [i], all_slots [j]);
	    }
	}
    }
    */

  VEC (rtx, heap)* slot_order = VEC_alloc (rtx, heap, size);
  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    {
      if (VEC_empty (rtx, slot_order))
	{
	  // add to front
	}
      else
	{

	}
    }
}

static void
mark_dead (rtx insn, rtx slot)
{
  int offset = sind (slot);
  live [offset] = NULL;
  def [offset] = insn;
  all_slots [offset] = slot;
}

static void
note_use (rtx insn, rtx slot)
{
  fprintf (stderr, "USE at %d(bp)\n", int_offset (slot));
  x (slot);

  if (is_live (slot)) /* avoid duplicate info */
    return;

  mark_live (insn, slot);

}

static void
go (void)
{
  rtx insn;
  fprintf (stderr, "Function: %s\n", IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (cfun->decl)));

  /* HACK - find the smallest stack slot var to use to offset indices */
  for (insn = get_last_insn (); insn; insn = PREV_INSN (insn))
    {
      if (!INSN_P (insn))
	continue;

      rtx pattern = PATTERN (insn);
      if (GET_CODE (pattern) != SET)
	continue;

      rtx src = SET_SRC (pattern);
      rtx dest = SET_DEST (pattern);
      rtx offset;
      /* only DEFS are interesting */
      if (REG_P (src) 
	  && (offset = get_offset (dest))
	  && INTVAL (offset) < 0)
	{
	  if (INTVAL (offset) > top_of_saved_regs)
	    top_of_saved_regs = INTVAL (offset);
	}
    }
   

  init_data_structs ();

  /* look for instructions of the pattern: 
   *   (insn (set (slot) (reg))) or
   *   (insn (set (reg) (slot))),
   * where slot is the pattern (mem (plus (reg bp) (const_int))) */
  for (insn = get_last_insn (); insn; insn = PREV_INSN (insn))
    {
      if (!INSN_P (insn))
	continue;

      rtx pattern = PATTERN (insn);
      if (GET_CODE (pattern) != SET)
	continue;

      rtx dest = SET_DEST (pattern);
      rtx src = SET_SRC (pattern);
      rtx offset;
      if (REG_P (src) 
	  && (offset = get_offset (dest))
	  && INTVAL (offset) < 0)
	mark_dead (insn, dest);

      else if (REG_P (dest) 
	       && (offset = get_offset (src))
	       && INTVAL (offset) < 0)
	mark_live (insn, src);
    }
  handle_non_interfering_slots ();
}
#endif

/* Hooking it in at cfgexpand */


VEC(tree,heap)* stack_vars = NULL;

static bool
is_suitable_stack_var (tree var)
{
  return false;
}

static bool 
is_first_def (tree stmt, tree var)
{
  return false;
}

static bool
is_last_use (tree stmt, tree var)
{
  return false;
}

static rtx
get_size (tree var)
{
  int size = tree_low_cst (DECL_SIZE_UNIT (var), 1);
  return gen_rtx_CONST_INT (SImode, size);
}

/* Return a stack-allocatable variable from stmt */
static tree
var_used_in_stmt (tree stmt)
{
  return NULL;
}

static VEC(tree, heap)* stmts;
static VEC(tree, heap)* vars;

/* This is called from cfgexpand.c, at which point the SSA form is gone,
 * and the gimple is very low */
void
possibly_add_stack_motion (tree stmt)
{
  fprintf (stderr, "\nFor stmt:\n");
  tree var;
  int i;
  gcc_assert (VEC_length (tree, stmts) == VEC_length (tree, vars));

  t (stmt);
  for (i = 0; VEC_iterate (tree, vars, i, var); i++)
    {
      if (VEC_index (tree, stmts, i) == stmt)
	{
	  t (var);
	}
    }

  
/*  adjust_stack (gen_rtx_CONST_INT (SImode, 128));
  anti_adjust_stack (gen_rtx_CONST_INT (SImode, 128));*/
  // foreach USE/DEF in stmt
  //   if is_stack_var_definition
  //     adjust_stack (size_of_var)
  //   elseif if_stack_var_final_use
  //     anti_adjust_stack (size_of_var)

  /* Trivial test: get all the variables. If they are stack vars, adjust
   * the stack first */

/*
  var = var_used_in_stmt (stmt);
  if (var)
    {
      rtx size = get_size (var);
      if (is_first_def (var, stmt))
	{
	  adjust_stack (size);
	}
      else if (is_last_use (var, stmt))
	{
	  anti_adjust_stack (size);
	}
    }
    */
}

static void
init_liveness_info (void)
{
  /*
  num_blocks = last_basic_block;
  work_stack = XNEWVEC (int, last_basic_block);
  stack_top = work_stack;

  livein = XNEWVEC (last_basic_block, bitmap);
  for (x = 0; x < last_basic_block; x++)
    livein[x] = BITMAP_ALLOC (NULL);

  liveout = XNEWVEC (last_basic_block, bitmap);
  for (x = 0; x < last_basic_block; x++)
    liveout[x] = BITMAP_ALLOC (NULL);
    */
}

static void
visit_block (basic_block bb)
{
  /*
  SET_BIT (visited, bb->index);
  FOR_EACH_EDGE (e, ei, bb->preds)
    {
      pred_bb = e->src;
      if (pred_bb == ENTRY_BLOCK_PTR)
	continue;
*/
}

static void
init_data (void)
{
  stmts = VEC_alloc (tree, heap, 50);
  vars = VEC_alloc (tree, heap, 50);
}

static void
clean_up_data (void)
{
}

static void
add_statement_for_var (tree stmt, tree var)
{
  gcc_assert (is_gimple_variable (var));
  fprintf (stderr, "Adding var for stmt:");
  t (stmt);
  t (var);
  tree real_var = SSA_NAME_VAR (var);
  t (real_var);

  VEC_safe_push (tree, heap, stmts, stmt);
  VEC_safe_push (tree, heap, vars, real_var);
}

static void
go (void)
{
  tree var;
  ssa_op_iter iter;
  tree stmt;
  basic_block bb;
  block_stmt_iterator bsi;

  /* copy the iteration stuff from tree-ssa-live */
  init_liveness_info ();

#if 0
  FOR_EACH_BB_REVERSE (bb)
    visit_block (bb, visited, tmp);

  /* Process any blocks which require further iteration.  */
  while (live->stack_top != live->work_stack)
    {
      b = *--(live->stack_top);
      loe_visit_block (live, BASIC_BLOCK (b), visited, tmp);
    }
#endif

  FOR_EACH_BB (bb)
    {
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
	  tree stmt = bsi_stmt (bsi);
	  tree var;
	  FOR_EACH_SSA_TREE_OPERAND (var, stmt, iter, SSA_OP_ALL_OPERANDS)
	    {
	      add_statement_for_var (stmt, var);
	    }
	}
    }
 
}

static bool
gate_stack_motion (void)
{
  if (flag_ipa_stack_allocate == 6) return true;
  return false;
}

unsigned int
execute_stack_motion (void)
{
  init_data ();
  /* this isnt always called by the pass manager */
  if (gate_stack_motion ())
    go ();

  clean_up_data ();
  return 0;
}

struct tree_opt_pass pass_stack_motion =
{
  "stack-motion",                       /* name */
  gate_stack_motion,                    /* gate */
  execute_stack_motion,                 /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  0,                                    /* tv_id */
  PROP_ssa,                             /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func,                       /* todo_flags_finish */
  0                                     /* letter */
};

