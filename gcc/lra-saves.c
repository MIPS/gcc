/* Save/restore placement optimization based on register allocation.
   Copyright (C) 2010, 2011
   Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */


/* This file contains code for placement save/restore insns.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "hard-reg-set.h"
#include "rtl.h"
#include "tm_p.h"
#include "regs.h"
#include "insn-config.h"
#include "recog.h"
#include "output.h"
#include "regs.h"
#include "target.h"
#include "function.h"
#include "expr.h"
#include "basic-block.h"
#include "except.h"
#include "cfgloop.h"
#include "df.h"
#include "ira.h"
#include "lra-int.h"

/* Pseudos living through calls and assigned to call used hard
   registers.  */
static bitmap consideration_pseudos;
/* Currently live pseudos which are subset of the consideration
   pseudos.  */
static bitmap live_pseudos;

/* This page contains code for global analysis for placement
   save/restore insn.

   First, we calculate local BB info in function calculate_local_save_info:
     o consideration pseudos set/mentioned in basic blocks (they prevents
       moving corresponding saves/restores further up/down in CFG) --
       see comments for save_kill/restore_kill.
     o consideration pseudos needed to be saved/restored around calls and
       which are not set/mentioned before/after the call in the basic
       block -- see comments for save_gen/restore_gen.

   Second, we calculate the global info for placement saves/restores
   in functions -- see comments for save_in, save_out, restore_in,
   restore_out.

   The equations for the global info calculation are not trivial
   because we don't want to put saves/restores on CFG edges.  For
   example, it means that all save_out set of BB should be a subset of
   intersections of save_in of all successors of the BB.  Preventing
   putting the code on edges makes data flow problem bidirectional.

   The equations do not permit to propagate info into the loops
   because we don't want to put saves/restores in more frequently
   executed points.  But if the loop does not contains references for
   a pseudo, we permit to propagate info for the pseudo into the loop
   because the info will be propagated through all the loop and as
   consequence corresponding saves/restores will be moved through the
   loop.

   Third, using this info we place saves/restores. */

/* The following structure contains basic block data flow information
   used to optimize save/restore placement.  Data flow equations are
   bidirectional because we don't want to put save/restore code on CFG
   edges.  */
struct bb_info
{
  /* True if save_out/restore_in should be empty for this block.  */
  int empty_save_out_p, empty_restore_in_p;
  /* Consideration pseudos set/mentioned in the BB.  */
  bitmap save_kill;
  bitmap restore_kill;
  /* Consideration pseudos needed to be saved and this save not killed
     (see above) by an insn in the BB before that.  */
  bitmap save_gen;
  /* Consideration pseudos needed to be restored and this restore not
     killed by an insn in the BB after that.  */
  bitmap restore_gen;
  /* Consideration pseudos needed to be saved/restored at the start
     and end of the basic block.  */
  bitmap save_in, save_out, restore_in, restore_out;
  /* We don't want to generate save/restore insns on edges because it
     changes CFG.  To prevent this we use the following set.  These
     sets define what consideration pseudos should be saved/restored
     at the start/end of basic block.  */
  bitmap save_here, restore_here;
};

/* The above info for each BB.  */
struct bb_info *bb_infos;

/* Macros for accessing data flow information of basic blocks.  */
#define BB_INFO_BY_INDEX(N) (bb_infos + (N))
#define BB_INFO(BB) BB_INFO_BY_INDEX (BB->index)

/* The following structure contains loop info necessary for
   save/restore placement optimization.  */
struct loop_info
{
  /* All consideration pseudos mentioned in the loop.  */
  bitmap mentioned_pseudos;
};

/* Macro for accessing data flow information of LOOP.  */
#define LOOP_INFO(LOOP) ((struct loop_info *) (LOOP)->aux)



/* The number elements in arrays correspondingly REFERENCED_PSEUDOS
   and MODIFIED_PSEUDOS.  */
static int referenced_pseudos_num, modified_pseudos_num;

/* Enough space to keep all operands and two address registers for an
   operand.  */
#define MAX_INSN_PSEUDOS (MAX_RECOG_OPERANDS * 3)

/* Arrays used to store pseudos referenced and modified in the current
   insn.  */
static int referenced_pseudos[MAX_INSN_PSEUDOS];
static int modified_pseudos[MAX_INSN_PSEUDOS];

/* Add pseudos referenced and modified in OP to ones referenced and
   modified in the current insn.  */
static void
mark_pseudos (rtx op, bool out_p)
{
  enum rtx_code code = GET_CODE (op);
  const char *fmt;
  int i, j, regno;

  switch (code)
    {
    case SUBREG:
      op = SUBREG_REG (op);
      if (! REG_P (op))
	{
	  mark_pseudos (op, out_p);
	  return;
	}
      /* Fall through:  */

    case REG:
      if ((regno = REGNO (op)) >= FIRST_PSEUDO_REGISTER
	  && bitmap_bit_p (consideration_pseudos, regno))
	{
	  gcc_assert (referenced_pseudos_num < MAX_INSN_PSEUDOS);
	  if (out_p)
	    modified_pseudos[modified_pseudos_num++] = regno;
	  referenced_pseudos[referenced_pseudos_num++] = regno;
	}
      return;
     
    case PRE_INC:
    case POST_INC:
    case PRE_DEC:
    case POST_DEC:
      mark_pseudos (XEXP (op, 0), true);
      return;

    case PRE_MODIFY:
    case POST_MODIFY:
      mark_pseudos (XEXP (op, 0), true);
      mark_pseudos (XEXP (op, 1), false);
      return;
    default:
      fmt = GET_RTX_FORMAT (code);
      for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
	{
	  if (fmt[i] == 'e')
	    mark_pseudos (XEXP (op, i), false);
	  else if (fmt[i] == 'E')
	    for (j = XVECLEN (op, i) - 1; j >= 0; j--)
	      mark_pseudos (XVECEXP (op, i, j), false);
	}
    }
}

/* Process INSN and record all referenced registers in
   REFERENCED_PSEUDOS and modified registers in MODIFIED_PSEUDOS.
   Only pseudos in CONSIDERATION_PSEUDOS are considered.  */
static void
mark_insn_pseudos (rtx insn)
{
  int i;
  lra_insn_recog_data_t id;
  struct lra_static_insn_data *static_id;

  modified_pseudos_num = referenced_pseudos_num = 0;
  id = lra_get_insn_recog_data (insn);
  static_id = id->insn_static_data;
  for (i = static_id->n_operands - 1; i >= 0; i--)
    mark_pseudos (*id->operand_loc[i], static_id->operand[i].type != OP_IN);
}

/* Process storing of REG to update live info.  */
static void
mark_pseudo_store (rtx reg, const_rtx setter ATTRIBUTE_UNUSED,
		   void *data ATTRIBUTE_UNUSED)
{
  int regno;

  if (GET_CODE (reg) == SUBREG)
    reg = SUBREG_REG (reg);

  if (! REG_P (reg))
    return;

  regno = REGNO (reg);

  if (regno >= FIRST_PSEUDO_REGISTER
      && bitmap_bit_p (consideration_pseudos, regno))
    bitmap_set_bit (live_pseudos, regno);
}

/* Temporary bitmap used for different calculation on bitmaps.  */
static bitmap temp_set;

/* The function calculates consideration pseudos and sets with kill,
   gen in their names.  */
static void
calculate_local_save_info (void)
{
  bool empty_save_out_p, empty_restore_in_p;
  basic_block bb;
  rtx insn;
  struct bb_info *bb_info;
  /* Computed in mark_set_pseudos, holds all consideration pseudos set
     by the current instruction.  */
  bitmap save_kill, restore_kill;
  bitmap save_gen, restore_gen;

  save_kill = BITMAP_ALLOC (&reg_obstack);
  restore_kill = BITMAP_ALLOC (&reg_obstack);
  save_gen = BITMAP_ALLOC (&reg_obstack);
  restore_gen = BITMAP_ALLOC (&reg_obstack);
  FOR_EACH_BB (bb)
    {
      edge e;
      edge_iterator ei;
      loop_p loop;

      bb_info = BB_INFO (bb);

      bitmap_clear (bb_info->save_in);
      bitmap_clear (bb_info->save_out);
      bitmap_clear (bb_info->restore_in);
      bitmap_clear (bb_info->restore_out);
      bitmap_clear (bb_info->save_here);
      bitmap_clear (bb_info->restore_here);
      bitmap_clear (save_gen);
      bitmap_clear (restore_gen);
      bitmap_clear (save_kill);
      bitmap_clear (restore_kill);

      bitmap_and (live_pseudos, DF_LR_IN (bb), consideration_pseudos);

      FOR_BB_INSNS (bb, insn)
	{
	  int i, regno;
	  rtx link;
	  enum rtx_code code = GET_CODE (insn);
	  
	  if (! NONDEBUG_INSN_P (insn))
	    continue;

	  mark_insn_pseudos (insn);
	  for (i = 0; i < referenced_pseudos_num; i++)
	    {
	      regno = referenced_pseudos[i];
	      bitmap_clear_bit (restore_gen, regno);
	      bitmap_set_bit (restore_kill, regno);
	    }
	  for (i = 0; i < modified_pseudos_num; i++)
	    {
	      regno = modified_pseudos[i];
	      bitmap_set_bit (save_kill, regno);
	    }
	  
	  for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
	    if (REG_NOTE_KIND (link) == REG_DEAD)
	      bitmap_clear_bit (live_pseudos, REGNO (XEXP (link, 0)));
	  
	  if (code == CALL_INSN && find_reg_note (insn, REG_NORETURN, NULL))
	    {
	      bitmap_copy (save_kill, consideration_pseudos);
	      bitmap_copy (restore_kill, consideration_pseudos);
	    }
	  else if (code == CALL_INSN)
	    {
	      /* Compute which hard pseudos must be saved before this call.  */
	      bitmap_ior_into (restore_gen, live_pseudos);
	      bitmap_ior_and_compl_into (save_gen, live_pseudos, save_kill);
	    }

	  /* Mark any registers set in INSN as live, and mark them as
	     conflicting with all other live pseudos.  */
	  note_stores (PATTERN (insn), mark_pseudo_store, NULL);
	  
#ifdef AUTO_INC_DEC
	  for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
	    if (REG_NOTE_KIND (link) == REG_INC)
	      mark_pseudo_store (XEXP (link, 0), NULL_RTX, NULL);
#endif
	  for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
	    if (REG_NOTE_KIND (link) == REG_UNUSED)
	      bitmap_clear_bit (live_pseudos, REGNO (XEXP (link, 0)));
	  
	}

      empty_save_out_p = false;
      FOR_EACH_EDGE (e, ei, bb->succs)
	if (e->flags & EDGE_ABNORMAL)
	  {
	    empty_save_out_p = true;
	    break;
	  }
      bb_info->empty_save_out_p = empty_save_out_p;

      empty_restore_in_p = false;
      FOR_EACH_EDGE (e, ei, bb->preds)
	if (e->flags & EDGE_ABNORMAL)
	  {
	    empty_restore_in_p = true;
	    break;
	  }
      bb_info->empty_restore_in_p = empty_restore_in_p;

      bitmap_copy (bb_info->save_gen, save_gen);
      bitmap_copy (bb_info->restore_gen, restore_gen);
      bitmap_copy (bb_info->save_kill, save_kill);
      bitmap_copy (bb_info->restore_kill, restore_kill);

      for (loop = bb->loop_father;
	   loop != current_loops->tree_root;
	   loop = loop_outer (loop))
	{
	  struct loop_info *loop_info = LOOP_INFO (loop);
	  
	  bitmap_ior_into (loop_info->mentioned_pseudos, restore_kill);
	}
    }
  BB_INFO_BY_INDEX (EXIT_BLOCK)->empty_restore_in_p = true;
  BB_INFO_BY_INDEX (ENTRY_BLOCK)->empty_save_out_p = true;
  BITMAP_FREE (save_kill);
  BITMAP_FREE (restore_kill);
  BITMAP_FREE (save_gen);
  BITMAP_FREE (restore_gen);
}

/* The transfer function used by the DF equation solver to propagate
   restore info through block with BB_INDEX according to the following
   equation:

   bb.restore_out = (bb.restore_in - bb.restore_kill) U bb.restore_gen.  */
static bool
restore_trans_fun (int bb_index)
{
  struct bb_info *bb_info = BB_INFO_BY_INDEX (bb_index);

  bitmap_ior_and_compl (temp_set, bb_info->restore_gen,
			bb_info->restore_in, bb_info->restore_kill);
  if (! bitmap_equal_p (temp_set, bb_info->restore_out))
    {
      bitmap_copy (bb_info->restore_out, temp_set);
      return true;
    }
  return false;
}

/* The confluence function used by the DF equation solver to set up
   restore info for a block BB without predecessor.  */
static void
restore_con_fun_0 (basic_block bb)
{
  bitmap_clear (BB_INFO (bb)->restore_in);
}

/* The confluence function used by the DF equation solver to propagate
   restore info from predecessor to successor on edge E (pred->bb)
   according to the following equation:

     bb.restore_in  = empty for entry block or one with empty_restore_in_p
                    | ^(pred.restore_out - pred.restore_here) ^ bb.live_at_start
                      - bb.save_here
		    
   If the edge is a loop enter we do not propagate the info because we
   don't want to put restores in more frequently executed places.
 */
static bool
restore_con_fun_n (edge e)
{
  basic_block pred = e->src;
  basic_block bb = e->dest;
  struct bb_info *bb_info = BB_INFO (bb);
  
  if (bb_info->empty_restore_in_p)
    return false;

  bitmap_and_compl (temp_set,
		    BB_INFO (pred)->restore_out, BB_INFO (pred)->restore_here);
  bitmap_and_compl_into (temp_set, BB_INFO (bb)->save_here);
  if (bb->loop_depth > pred->loop_depth)
    bitmap_and_compl_into (temp_set,
			   LOOP_INFO (bb->loop_father)->mentioned_pseudos);
  bitmap_and_into (temp_set, DF_LR_IN (bb));
  if (EDGE_PRED (bb, 0) == e)
    {
      if (bitmap_equal_p (bb_info->restore_in, temp_set))
	return false;
      bitmap_copy (bb_info->restore_in, temp_set);
    }
  else
    {
      if (! bitmap_intersect_compl_p (bb_info->restore_in, temp_set))
	return false;
      bitmap_and_into (bb_info->restore_in, temp_set);
    }
  return true;
}

/* Basic blocks for data flow problem -- all bocks except the special
   ones.  */
static bitmap all_blocks;

/* The function calculates global restore information according
   to the following equations:

     bb.restore_in  = empty for entry block or one with empty_restore_in_p
                    | ^(pred.restore_out - pred.restore_here) ^ bb.live_at_start
                      - bb.save_here
     bb.restore_out = (bb.restore_in - bb.restore_kill) U bb.restore_gen.  */
static void
calculate_restore_in_out (void)
{
  df_simple_dataflow (DF_FORWARD, NULL, restore_con_fun_0, restore_con_fun_n,
		      restore_trans_fun, all_blocks,
		      df_get_postorder (DF_FORWARD),
		      df_get_n_blocks (DF_FORWARD));
}

/* Bitmap used to calculate BB restore_here and compare it with the
   previous value.  */
static bitmap restore_here;

/* The function calculates RESTORE_HERE according to the equation
   bb.restore_here = U ((bb.restore_out - succ.restore_in)
                        ^ succ.live_at_start).  */
static int
calculate_restore_here (void)
{
  basic_block bb;
  edge e;
  edge_iterator ei;
  bool changed_p = false;

  FOR_EACH_BB (bb)
    {
      bitmap_clear (restore_here);
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  basic_block dest = e->dest;

	  bitmap_and_compl (temp_set, BB_INFO (bb)->restore_out,
			    BB_INFO (dest)->restore_in);
	  bitmap_and_into (temp_set, DF_LR_IN (dest));
	  bitmap_ior_into (restore_here, temp_set);
	}

      if (! bitmap_equal_p (restore_here, BB_INFO (bb)->restore_here))
	{
	  bitmap_copy (BB_INFO (bb)->restore_here, restore_here);
	  changed_p = true;
	}
    }
  return changed_p;
}

/* The transfer function used by the DF equation solver to propagate
   save info through block with BB_INDEX according to the following
   equation:

   bb.save_in = (bb.save_out - bb.save_kill) U bb.save_gen.

   The function also propagates saving modes of the hard
   registers.  */

static bool
save_trans_fun (int bb_index)
{
  struct bb_info *bb_info = BB_INFO_BY_INDEX (bb_index);

  bitmap_ior_and_compl (temp_set, bb_info->save_gen,
			bb_info->save_out, bb_info->save_kill);
  if (! bitmap_equal_p (temp_set, bb_info->save_in))
    {
      bitmap_copy (bb_info->save_in, temp_set);
      return true;
    }
  return false;
}

/* The confluence function used by the DF equation solver to set up
   save info for a block BB without successor.  */
static void
save_con_fun_0 (basic_block bb)
{
  bitmap_clear (BB_INFO (bb)->save_out);
}

/* The confluence function used by the DF equation solver to propagate
   save info from successor to predecessor on edge E (bb->succ)
   according to the following equation:

     bb.save_out  = empty for exit block or one with empty_save_out_p
                    | ^(succ.save_in - succ.save_here) ^ bb.live_at_end
                      - bb.restore_here

   If the edge is a loop exit we do not propagate the info because we
   don't want to put saves in more frequently executed places.
*/
static bool
save_con_fun_n (edge e)
{
  basic_block succ = e->dest;
  basic_block bb = e->src;
  struct bb_info *bb_info = BB_INFO (bb);
  
  if (bb_info->empty_save_out_p)
    return false;

  bitmap_and_compl (temp_set,
		    BB_INFO (succ)->save_in, BB_INFO (succ)->save_here);
  bitmap_and_compl_into (temp_set, BB_INFO (bb)->restore_here);
  if (bb->loop_depth > succ->loop_depth)
    bitmap_and_compl_into (temp_set,
			   LOOP_INFO (bb->loop_father)->mentioned_pseudos);
  bitmap_and_into (temp_set, DF_LR_OUT (bb));
  if (EDGE_SUCC (bb, 0) == e)
    {
      if (bitmap_equal_p (bb_info->save_out, temp_set))
	return false;
      bitmap_copy (bb_info->save_out, temp_set);
    }
  else
    {
      if (! bitmap_intersect_compl_p (bb_info->save_out, temp_set))
	return false;
      bitmap_and_into (bb_info->save_out, temp_set);
    }
  return true;
}

/* The transfer function calculates global save information according
   to the following equations:

     bb.save_out  = empty for exit block or one with empty_save_out_p
                    | ^(succ.save_in - succ.save_here) ^ bb.live_at_end
                      - bb.restore_here
     bb.save_in = (bb.save_out - bb.save_kill) U bb.save_gen.

*/
static void
calculate_save_in_out (void)
{
  df_simple_dataflow (DF_BACKWARD, NULL, save_con_fun_0, save_con_fun_n,
		      save_trans_fun, all_blocks,
		      df_get_postorder (DF_BACKWARD),
		      df_get_n_blocks (DF_BACKWARD));
}

/* Bitmap used to calculate BB save_here and compare it with the
   previous value.  */
static bitmap save_here;

/* The function calculates SAVE_HERE according to the equation
   bb.save_here = U ((bb.save_in - pred.save_out) ^ pred.live_at_end).  */
static bool
calculate_save_here (void)
{
  basic_block bb;
  edge e;
  edge_iterator ei;
  bool changed_p = false;

  FOR_EACH_BB (bb)
    {
      bitmap_clear (save_here);
      FOR_EACH_EDGE (e, ei, bb->preds)
	{
	  basic_block src = e->src;

	  bitmap_and_compl (temp_set,
			    BB_INFO (bb)->save_in, BB_INFO (src)->save_out);
	  bitmap_and_into (temp_set, DF_LR_OUT (src));
	  bitmap_ior_into (save_here, temp_set);
	}

      if (! bitmap_equal_p (save_here, BB_INFO (bb)->save_here))
	{
	  bitmap_copy (BB_INFO (bb)->save_here, save_here);
	  changed_p = true;
	}
    }
  return changed_p;
}

/* The function calculates the global save/restore information used to
   put save/restore code without generating new blocks.  This is a
   bidirectional data flow problem (calculation of SAVE_IN and
   SAVE_OUT is a backward data flow problem and SAVE_HERE is forward
   one; calculation of RESTORE_IN and RESTORE_OUT is a forward data
   flow problem and RESTORE_HERE is backward one).  */
static void
make_global_save_analysis (void)
{
  basic_block bb;
  int iter;
  bool changed_p;

  all_blocks = BITMAP_ALLOC (NULL);
  save_here = BITMAP_ALLOC (&reg_obstack);
  restore_here = BITMAP_ALLOC (&reg_obstack);
  FOR_ALL_BB (bb)
    bitmap_set_bit (all_blocks, bb->index);
  calculate_local_save_info ();
  for (iter = 1;; iter++)
    {
      calculate_restore_in_out ();
      changed_p = calculate_restore_here ();
      calculate_save_in_out ();
      changed_p = calculate_save_here () || changed_p;
      if (! changed_p)
	break;
    }
  if (lra_dump_file != NULL)
    fprintf (lra_dump_file,
	     " Number of global save/restore analysis iterations %d\n", iter);
  BITMAP_FREE (all_blocks);
  BITMAP_FREE (save_here);
  BITMAP_FREE (restore_here);
}

/* Print pseudos in SET to file F.  */  
static void
print_pseudo_set (FILE *f, bitmap set)
{
  unsigned int i;
  bitmap_iterator bi;

  EXECUTE_IF_SET_IN_BITMAP (set, FIRST_PSEUDO_REGISTER, i, bi)
    fprintf (f, " %d", i);
}

/* Print the save information for each block to file F.  */  
static void
print_save_data (FILE *f)
{
  basic_block bb;
  struct bb_info *bb_info;
  edge e;
  edge_iterator ei;

  FOR_EACH_BB (bb)
    {
      bb_info = BB_INFO (bb);
      fprintf (f, "bb %d (preds", bb->index);
      FOR_EACH_EDGE (e, ei, bb->preds)
	{
	  fprintf (f, " %d", e->src->index);
	}
      fprintf (f, ") (succs");
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  fprintf (f, " %d", e->dest->index);
	}
      fprintf (f, ")\n  empty_save_out_p=%d", bb_info->empty_save_out_p);
      fprintf (f, ", empty_restore_in_p=%d", bb_info->empty_restore_in_p);
      fprintf (f, "\n  save_in:");
      print_pseudo_set (f, bb_info->save_in);
      fprintf (f, "\n  save_out:");
      print_pseudo_set (f, bb_info->save_out);
      fprintf (f, "\n  restore_in:");
      print_pseudo_set (f, bb_info->restore_in);
      fprintf (f, "\n  restore_out:");
      print_pseudo_set (f, bb_info->restore_out);
      fprintf (f, "\n  live_at_start:");
      bitmap_and (temp_set, DF_LR_IN (bb), consideration_pseudos);
      print_pseudo_set (f, temp_set);
      fprintf (f, "\n  live_at_end:");
      bitmap_and (temp_set, DF_LR_OUT (bb), consideration_pseudos);
      print_pseudo_set (f, temp_set);
      fprintf (f, "\n  save_here:");
      print_pseudo_set (f, bb_info->save_here);
      fprintf (f, "\n  restore_here:");
      print_pseudo_set (f, bb_info->restore_here);
      fprintf (f, "\n  save_kill:");
      print_pseudo_set (f, bb_info->save_kill);
      fprintf (f, "\n  restore_kill:");
      print_pseudo_set (f, bb_info->restore_kill);
      fprintf (f, "\n  save_gen:");
      print_pseudo_set (f, bb_info->save_gen);
      fprintf (f, "\n  restore_gen:");
      print_pseudo_set (f, bb_info->restore_gen);
      fprintf (f, "\n\n");
    }
}

/* Print the save information for each block to stderr.  */  
void
lra_debug_save_data (void)
{
  print_save_data (stderr);
}

/* Info about save/restore code for a pseudo.  */
struct save_regs
{
  /* A spilled pseudo which hold value of the corresponding
     saved/restored pseudo around calls.  */
  rtx mem_reg;
  /* Saving/restoring of a pseudo could be done in a mode different
     from the pseudo mode.  The following is the pseudo or a subreg of
     the pseudo and is used in save/restore code.  */
  rtx saved_reg;
};

/* Map: regno -> info about save/restore for REGNO.  */
static struct save_regs *save_regs;

/* Insert save (if SAVE_P) or restore code for REGNO before (if
   BEFORE_P) or after INSN.  */
static void
insert_save_restore (rtx insn, bool before_p, int regno, bool save_p)
{
  rtx x, mem_reg;
  int to_regno, from_regno;
  rtx saved_reg = regno_reg_rtx[regno];

  if ((mem_reg = save_regs[regno].mem_reg) != NULL_RTX)
    saved_reg = save_regs[regno].saved_reg;
  else
    {
      enum machine_mode mode;
      int hard_regno = reg_renumber[regno];

      gcc_assert (hard_regno >= 0);
      mode = (HARD_REGNO_CALLER_SAVE_MODE
	      (hard_regno,
	       hard_regno_nregs[hard_regno][PSEUDO_REGNO_MODE (regno)],
	       PSEUDO_REGNO_MODE (regno)));
      if (mode != PSEUDO_REGNO_MODE (regno))
	saved_reg = gen_rtx_SUBREG (mode, saved_reg, 0);
      save_regs[regno].mem_reg = mem_reg
	= lra_create_new_reg (VOIDmode, saved_reg, NO_REGS, NULL);
      save_regs[regno].saved_reg = saved_reg;
    }
  start_sequence ();
  if (save_p)
    {
      to_regno = REGNO (mem_reg);
      from_regno = regno;
      emit_move_insn (mem_reg, saved_reg);
    }
  else
    {
      to_regno = regno;
      from_regno = REGNO (mem_reg);
      emit_move_insn (saved_reg, mem_reg);
    }
  x = get_insns ();
  end_sequence ();
  ira_update_equiv_info_by_shuffle_insn (to_regno, from_regno, x);
  if (lra_dump_file != NULL)
    fprintf (lra_dump_file, "Inserting insn %u %d:=%d %s i%u bb%d\n",
	     INSN_UID (x),
	     save_p ? (int) REGNO (mem_reg) : regno,
	     save_p ? regno : (int) REGNO (mem_reg),
	     before_p ? "before" : "after", INSN_UID (insn),
	     BLOCK_FOR_INSN (insn)->index);
  if (before_p)
    emit_insn_before (x, insn); 
  else
    emit_insn_after (x, insn); 
}

/* Process storing of REG to update live info.  */
static void
mark_pseudo_dead (rtx reg, const_rtx setter ATTRIBUTE_UNUSED,
		   void *data ATTRIBUTE_UNUSED)
{
  int regno;

  if (GET_CODE (reg) == SUBREG)
    reg = SUBREG_REG (reg);

  if (! REG_P (reg))
    return;

  regno = REGNO (reg);

  if (regno >= FIRST_PSEUDO_REGISTER)
    bitmap_clear_bit (live_pseudos, regno);
}


/* Last UID before generating insns for save/resore code.  */
static int insn_uid_before;

/* Insert save/restore code usinge solution of the data-flow
   equations.  */
static void
insert_saves_restores (void)
{
  int i;
  unsigned int regno;
  rtx insn, curr, link, last_restore_insn;
  rtx where = NULL_RTX;
  struct bb_info *bb_info;
  basic_block bb;
  bitmap pseudos_saved, pseudos_to_save;
  reg_set_iterator rsi;
  
  pseudos_saved = BITMAP_ALLOC (&reg_obstack);
  FOR_EACH_BB (bb)
    {
      bb_info = BB_INFO (bb);
      bitmap_and (live_pseudos, DF_LR_IN (bb), consideration_pseudos);
      bitmap_copy (pseudos_saved, bb_info->restore_in);

      FOR_BB_INSNS_SAFE (bb, insn, curr)
	{
	  enum rtx_code code = GET_CODE (insn);
	  
	  if (! NONDEBUG_INSN_P (insn))
	    continue;

	  mark_insn_pseudos (insn);
	  for (i = 0; i < referenced_pseudos_num; i++)
	    {
	      regno = referenced_pseudos[i];
	      if (bitmap_bit_p (pseudos_saved, regno)
		  && bitmap_bit_p (live_pseudos, regno))
		{
		  insert_save_restore (insn, true, regno, false);
		  bitmap_clear_bit (pseudos_saved, regno);
		}
	    }
	  for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
	    if (REG_NOTE_KIND (link) == REG_DEAD)
	      bitmap_clear_bit (live_pseudos, REGNO (XEXP (link, 0)));
	  
	  if (code == CALL_INSN && ! SIBLING_CALL_P (insn)
	      && ! find_reg_note (insn, REG_NORETURN, NULL))
	    bitmap_ior_into (pseudos_saved, live_pseudos);

	  /* Mark any registers set in INSN as live, and mark them as
	     conflicting with all other live pseudos.  */
	  note_stores (PATTERN (insn), mark_pseudo_store, NULL);
	  
#ifdef AUTO_INC_DEC
	  for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
	    if (REG_NOTE_KIND (link) == REG_INC)
	      mark_pseudo_store (XEXP (link, 0), NULL_RTX, NULL);
#endif
	  for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
	    if (REG_NOTE_KIND (link) == REG_UNUSED)
	      bitmap_clear_bit (live_pseudos, REGNO (XEXP (link, 0)));
	  
	  where = insn;
	}
      /* At the end of the basic block, we must restore any registers
	 from restore_here.  */
      EXECUTE_IF_SET_IN_REG_SET (BB_INFO (bb)->restore_here,
				 FIRST_PSEUDO_REGISTER, regno, rsi)
	insert_save_restore (where, JUMP_P (where), regno, false);
    }
  BITMAP_FREE (pseudos_saved);

  pseudos_to_save = BITMAP_ALLOC (&reg_obstack);
  FOR_EACH_BB (bb)
    {
      bb_info = BB_INFO (bb);
      bitmap_copy (pseudos_to_save, bb_info->save_out);
      bitmap_and (live_pseudos, DF_LR_OUT (bb), consideration_pseudos);
      last_restore_insn = NULL_RTX;
      FOR_BB_INSNS_REVERSE_SAFE (bb, insn, curr)
	{
	  enum rtx_code code = GET_CODE (insn);

	  if (! NONDEBUG_INSN_P (insn))
	    continue;

	  last_restore_insn = NULL_RTX;
	  mark_insn_pseudos (insn);
	  for (i = 0; i < modified_pseudos_num; i++)
	    {
	      regno = modified_pseudos[i];
	      if (bitmap_bit_p (pseudos_to_save, regno))
		{
		  /* We should put save insns before restore insns
		     between the two calls to make pressure less.  */
		  insert_save_restore
		    (last_restore_insn != NULL ? last_restore_insn : insn,
		     /* Modifying jump case ??? */
		     last_restore_insn != NULL || JUMP_P (insn),
		     regno, true);
		  bitmap_clear_bit (pseudos_to_save, regno);
		}
	    }
	  if (INSN_UID (insn) < insn_uid_before)
	    {
	      /* Mark any registers set in INSN as dead.  */
	      note_stores (PATTERN (insn), mark_pseudo_dead, NULL);
	      
	      if (code == CALL_INSN && ! SIBLING_CALL_P (insn)
		  && ! find_reg_note (insn, REG_NORETURN, NULL))
		/* Use the register life information to compute which
		   pseudos are live during the call.  */
		bitmap_ior_into (pseudos_to_save, live_pseudos);
	  
	      for (i = 0; i < referenced_pseudos_num; i++)
		{
		  regno = referenced_pseudos[i];
		  bitmap_set_bit (live_pseudos, regno);
		}
	    }
	  where = insn;
	}

      /* At the start of the basic block, we must save any
	 registers from save_here.  */
      where = (last_restore_insn != NULL ? last_restore_insn : where);
      EXECUTE_IF_SET_IN_REG_SET (BB_INFO (bb)->save_here,
				 FIRST_PSEUDO_REGISTER, regno, rsi)
	if (bitmap_bit_p (pseudos_to_save, regno))
	  insert_save_restore (where, INSN_P (where), regno, true);
    }
  BITMAP_FREE (pseudos_to_save);
}

/* Major function to insert save/restore code.  It returns true if any
   save/restore insn and any new pseudo was generated.  The function
   needs correct DFA info and REG_N_REFS & REG_N_CALLS_CROSSED before
   the function work.  */
bool
lra_save_restore (int max_consideration_regno)
{
  int i, hard_regno, max_regno_before;
  basic_block bb;
  loop_p loop;
  loop_iterator li;
  struct loops loops;

  insn_uid_before = get_max_uid ();
  max_regno_before = max_reg_num ();

  if (lra_dump_file != NULL)
    fprintf (lra_dump_file, "\n********** Caller saves: **********\n\n");

  gcc_assert (current_loops == NULL);
  flow_loops_find (&loops);
  current_loops = &loops;

  /* We cannot use bb->aux because it is used by DFA.  */
  bb_infos = (struct bb_info *) xmalloc (sizeof (struct bb_info)
					 * last_basic_block);
  /* Do global analysis for placement of spill code. */
  FOR_BB_BETWEEN (bb, ENTRY_BLOCK_PTR, NULL, next_bb)
    {
      BB_INFO (bb)->save_kill = BITMAP_ALLOC (&reg_obstack);
      BB_INFO (bb)->restore_kill = BITMAP_ALLOC (&reg_obstack);
      BB_INFO (bb)->save_gen = BITMAP_ALLOC (&reg_obstack);
      BB_INFO (bb)->restore_gen = BITMAP_ALLOC (&reg_obstack);
      BB_INFO (bb)->save_in = BITMAP_ALLOC (&reg_obstack);
      BB_INFO (bb)->save_out = BITMAP_ALLOC (&reg_obstack);
      BB_INFO (bb)->restore_in = BITMAP_ALLOC (&reg_obstack);
      BB_INFO (bb)->restore_out = BITMAP_ALLOC (&reg_obstack);
      BB_INFO (bb)->save_here = BITMAP_ALLOC (&reg_obstack);
      BB_INFO (bb)->restore_here = BITMAP_ALLOC (&reg_obstack);
    }
  FOR_EACH_LOOP (li, loop, 0)
    {
      loop->aux = xmalloc (sizeof (struct loop_info));
      LOOP_INFO (loop)->mentioned_pseudos = BITMAP_ALLOC (&reg_obstack);
    }
  consideration_pseudos = BITMAP_ALLOC (&reg_obstack);
  for (i = FIRST_PSEUDO_REGISTER; i < max_consideration_regno; i++)
    if (REG_N_REFS (i) != 0
	&& (hard_regno = lra_get_regno_hard_regno (i)) >= 0
	&& REG_N_CALLS_CROSSED (i) != 0
	&& lra_hard_reg_set_intersection_p (hard_regno, PSEUDO_REGNO_MODE (i),
					    call_used_reg_set))
      bitmap_set_bit (consideration_pseudos, i);
  temp_set = BITMAP_ALLOC (&reg_obstack);
  live_pseudos = BITMAP_ALLOC (&reg_obstack);
  make_global_save_analysis ();
  if (lra_dump_file != NULL)
    print_save_data (lra_dump_file);
  save_regs = (struct save_regs *) xmalloc (sizeof (struct save_regs)
					    * max_regno_before);
  memset (save_regs, 0, sizeof (struct save_regs) * max_regno_before);
  insert_saves_restores ();
  free (save_regs);
  BITMAP_FREE (temp_set);
  BITMAP_FREE (consideration_pseudos);
  BITMAP_FREE (live_pseudos);
  FOR_EACH_LOOP (li, loop, 0)
    {
      BITMAP_FREE (LOOP_INFO (loop)->mentioned_pseudos);
      free (loop->aux);
    }
  FOR_BB_BETWEEN (bb, ENTRY_BLOCK_PTR, NULL, next_bb)
    {
      BITMAP_FREE (BB_INFO (bb)->save_kill);
      BITMAP_FREE (BB_INFO (bb)->restore_kill);
      BITMAP_FREE (BB_INFO (bb)->save_gen);
      BITMAP_FREE (BB_INFO (bb)->restore_gen);
      BITMAP_FREE (BB_INFO (bb)->save_in);
      BITMAP_FREE (BB_INFO (bb)->save_out);
      BITMAP_FREE (BB_INFO (bb)->restore_in);
      BITMAP_FREE (BB_INFO (bb)->restore_out);
      BITMAP_FREE (BB_INFO (bb)->save_here);
      BITMAP_FREE (BB_INFO (bb)->restore_here);
    }
  free (bb_infos);
  flow_loops_free (&loops);
  free_dominance_info (CDI_DOMINATORS);
  FOR_ALL_BB (bb)
    bb->loop_father = NULL;
  current_loops = NULL;
  return insn_uid_before != get_max_uid ();
}
