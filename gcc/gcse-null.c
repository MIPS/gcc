/* Global common subexpression elimination/Partial redundancy elimination
   and global constant/copy propagation for GNU compiler.
   Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002
   Free Software Foundation, Inc.

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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "toplev.h"

#include "rtl.h"
#include "tm_p.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "flags.h"
#include "real.h"
#include "insn-config.h"
#include "recog.h"
#include "basic-block.h"
#include "output.h"
#include "function.h"
#include "expr.h"
#include "except.h"
#include "ggc.h"
#include "params.h"

#include "obstack.h"

#include "gcse-globals.h"
#include "gcse-utils.h"

static int get_bitmap_width		PARAMS ((int, int, int));
static void invalidate_nonnull_info	PARAMS ((rtx, rtx, void *));
static int delete_null_pointer_checks_1	PARAMS ((unsigned int *,
					  sbitmap *, sbitmap *,
					  struct null_pointer_info *));

/* Removal of useless null pointer checks */

/* Many of the global optimization algorithms work by solving dataflow
   equations for various expressions.  Initially, some local value is
   computed for each expression in each block.  Then, the values across the
   various blocks are combined (by following flow graph edges) to arrive at
   global values.  Conceptually, each set of equations is independent.  We
   may therefore solve all the equations in parallel, solve them one at a
   time, or pick any intermediate approach.

   When you're going to need N two-dimensional bitmaps, each X (say, the
   number of blocks) by Y (say, the number of expressions), call this
   function.  It's not important what X and Y represent; only that Y
   correspond to the things that can be done in parallel.  This function will
   return an appropriate chunking factor C; you should solve C sets of
   equations in parallel.  By going through this function, we can easily
   trade space against time; by solving fewer equations in parallel we use
   less space.  */

static int
get_bitmap_width (n, x, y)
     int n;
     int x;
     int y;
{
  /* It's not really worth figuring out *exactly* how much memory will
     be used by a particular choice.  The important thing is to get
     something approximately right.  */
  size_t max_bitmap_memory = 10 * 1024 * 1024;

  /* The number of bytes we'd use for a single column of minimum
     width.  */
  size_t column_size = n * x * sizeof (SBITMAP_ELT_TYPE);

  /* Often, it's reasonable just to solve all the equations in
     parallel.  */
  if (column_size * SBITMAP_SET_SIZE (y) <= max_bitmap_memory)
    return y;

  /* Otherwise, pick the largest width we can, without going over the
     limit.  */
  return SBITMAP_ELT_BITS * ((max_bitmap_memory + column_size - 1)
			     / column_size);
}

/* Called via note_stores.  X is set by SETTER.  If X is a register we must
   invalidate nonnull_local and set nonnull_killed.  DATA is really a
   `null_pointer_info *'.

   We ignore hard registers.  */

static void
invalidate_nonnull_info (x, setter, data)
     rtx x;
     rtx setter ATTRIBUTE_UNUSED;
     void *data;
{
  unsigned int regno;
  struct null_pointer_info *npi = (struct null_pointer_info *) data;

  while (GET_CODE (x) == SUBREG)
    x = SUBREG_REG (x);

  /* Ignore anything that is not a register or is a hard register.  */
  if (GET_CODE (x) != REG
      || REGNO (x) < npi->min_reg
      || REGNO (x) >= npi->max_reg)
    return;

  regno = REGNO (x) - npi->min_reg;

  RESET_BIT (npi->nonnull_local[npi->current_block->index], regno);
  SET_BIT (npi->nonnull_killed[npi->current_block->index], regno);
}

/* Do null-pointer check elimination for the registers indicated in
   NPI.  NONNULL_AVIN and NONNULL_AVOUT are pre-allocated sbitmaps;
   they are not our responsibility to free.  */

static int
delete_null_pointer_checks_1 (block_reg, nonnull_avin,
			      nonnull_avout, npi)
     unsigned int *block_reg;
     sbitmap *nonnull_avin;
     sbitmap *nonnull_avout;
     struct null_pointer_info *npi;
{
  basic_block bb, current_block;
  sbitmap *nonnull_local = npi->nonnull_local;
  sbitmap *nonnull_killed = npi->nonnull_killed;
  int something_changed = 0;

  /* Compute local properties, nonnull and killed.  A register will have
     the nonnull property if at the end of the current block its value is
     known to be nonnull.  The killed property indicates that somewhere in
     the block any information we had about the register is killed.

     Note that a register can have both properties in a single block.  That
     indicates that it's killed, then later in the block a new value is
     computed.  */
  sbitmap_vector_zero (nonnull_local, last_basic_block);
  sbitmap_vector_zero (nonnull_killed, last_basic_block);

  FOR_EACH_BB (current_block)
    {
      rtx insn, stop_insn;

      /* Set the current block for invalidate_nonnull_info.  */
      npi->current_block = current_block;

      /* Scan each insn in the basic block looking for memory references and
	 register sets.  */
      stop_insn = NEXT_INSN (current_block->end);
      for (insn = current_block->head;
	   insn != stop_insn;
	   insn = NEXT_INSN (insn))
	{
	  rtx set;
	  rtx reg;

	  /* Ignore anything that is not a normal insn.  */
	  if (! INSN_P (insn))
	    continue;

	  /* Basically ignore anything that is not a simple SET.  We do have
	     to make sure to invalidate nonnull_local and set nonnull_killed
	     for such insns though.  */
	  set = single_set (insn);
	  if (!set)
	    {
	      note_stores (PATTERN (insn), invalidate_nonnull_info, npi);
	      continue;
	    }

	  /* See if we've got a usable memory load.  We handle it first
	     in case it uses its address register as a dest (which kills
	     the nonnull property).  */
	  if (GET_CODE (SET_SRC (set)) == MEM
	      && GET_CODE ((reg = XEXP (SET_SRC (set), 0))) == REG
	      && REGNO (reg) >= npi->min_reg
	      && REGNO (reg) < npi->max_reg)
	    SET_BIT (nonnull_local[current_block->index],
		     REGNO (reg) - npi->min_reg);

	  /* Now invalidate stuff clobbered by this insn.  */
	  note_stores (PATTERN (insn), invalidate_nonnull_info, npi);

	  /* And handle stores, we do these last since any sets in INSN can
	     not kill the nonnull property if it is derived from a MEM
	     appearing in a SET_DEST.  */
	  if (GET_CODE (SET_DEST (set)) == MEM
	      && GET_CODE ((reg = XEXP (SET_DEST (set), 0))) == REG
	      && REGNO (reg) >= npi->min_reg
	      && REGNO (reg) < npi->max_reg)
	    SET_BIT (nonnull_local[current_block->index],
		     REGNO (reg) - npi->min_reg);
	}
    }

  /* Now compute global properties based on the local properties.   This
     is a classic global availablity algorithm.  */
  compute_available (nonnull_local, nonnull_killed,
		     nonnull_avout, nonnull_avin);

  /* Now look at each bb and see if it ends with a compare of a value
     against zero.  */
  FOR_EACH_BB (bb)
    {
      rtx last_insn = bb->end;
      rtx condition, earliest;
      int compare_and_branch;

      /* Since MIN_REG is always at least FIRST_PSEUDO_REGISTER, and
	 since BLOCK_REG[BB] is zero if this block did not end with a
	 comparison against zero, this condition works.  */
      if (block_reg[bb->index] < npi->min_reg
	  || block_reg[bb->index] >= npi->max_reg)
	continue;

      /* LAST_INSN is a conditional jump.  Get its condition.  */
      condition = get_condition (last_insn, &earliest);

      /* If we can't determine the condition then skip.  */
      if (! condition)
	continue;

      /* Is the register known to have a nonzero value?  */
      if (!TEST_BIT (nonnull_avout[bb->index], block_reg[bb->index] - npi->min_reg))
	continue;

      /* Try to compute whether the compare/branch at the loop end is one or
	 two instructions.  */
      if (earliest == last_insn)
	compare_and_branch = 1;
      else if (earliest == prev_nonnote_insn (last_insn))
	compare_and_branch = 2;
      else
	continue;

      /* We know the register in this comparison is nonnull at exit from
	 this block.  We can optimize this comparison.  */
      if (GET_CODE (condition) == NE)
	{
	  rtx new_jump;

	  new_jump = emit_jump_insn_after (gen_jump (JUMP_LABEL (last_insn)),
					   last_insn);
	  JUMP_LABEL (new_jump) = JUMP_LABEL (last_insn);
	  LABEL_NUSES (JUMP_LABEL (new_jump))++;
	  emit_barrier_after (new_jump);
	}

      something_changed = 1;
      delete_insn (last_insn);
      if (compare_and_branch == 2)
	delete_insn (earliest);
      purge_dead_edges (bb);

      /* Don't check this block again.  (Note that BLOCK_END is
	 invalid here; we deleted the last instruction in the
	 block.)  */
      block_reg[bb->index] = 0;
    }

  return something_changed;
}

/* Find EQ/NE comparisons against zero which can be (indirectly) evaluated
   at compile time.

   This is conceptually similar to global constant/copy propagation and
   classic global CSE (it even uses the same dataflow equations as cprop).

   If a register is used as memory address with the form (mem (reg)), then we
   know that REG can not be zero at that point in the program.  Any instruction
   which sets REG "kills" this property.

   So, if every path leading to a conditional branch has an available memory
   reference of that form, then we know the register can not have the value
   zero at the conditional branch.

   So we merely need to compute the local properies and propagate that data
   around the cfg, then optimize where possible.

   We run this pass two times.  Once before CSE, then again after CSE.  This
   has proven to be the most profitable approach.  It is rare for new
   optimization opportunities of this nature to appear after the first CSE
   pass.

   This could probably be integrated with global cprop with a little work.  */

int
delete_null_pointer_checks (f)
     rtx f ATTRIBUTE_UNUSED;
{
  sbitmap *nonnull_avin, *nonnull_avout;
  unsigned int *block_reg;
  basic_block bb;
  int reg;
  int regs_per_pass;
  int max_reg;
  struct null_pointer_info npi;
  int something_changed = 0;

  /* If we have only a single block, then there's nothing to do.  */
  if (n_basic_blocks <= 1)
    return 0;

  /* Trying to perform global optimizations on flow graphs which have
     a high connectivity will take a long time and is unlikely to be
     particularly useful.

     In normal circumstances a cfg should have about twice as many edges
     as blocks.  But we do not want to punish small functions which have
     a couple switch statements.  So we require a relatively large number
     of basic blocks and the ratio of edges to blocks to be high.  */
  if (n_basic_blocks > 1000 && n_edges / n_basic_blocks >= 20)
    return 0;

  /* We need four bitmaps, each with a bit for each register in each
     basic block.  */
  max_reg = max_reg_num ();
  regs_per_pass = get_bitmap_width (4, last_basic_block, max_reg);

  /* Allocate bitmaps to hold local and global properties.  */
  npi.nonnull_local = sbitmap_vector_alloc (last_basic_block, regs_per_pass);
  npi.nonnull_killed = sbitmap_vector_alloc (last_basic_block, regs_per_pass);
  nonnull_avin = sbitmap_vector_alloc (last_basic_block, regs_per_pass);
  nonnull_avout = sbitmap_vector_alloc (last_basic_block, regs_per_pass);

  /* Go through the basic blocks, seeing whether or not each block
     ends with a conditional branch whose condition is a comparison
     against zero.  Record the register compared in BLOCK_REG.  */
  block_reg = (unsigned int *) xcalloc (last_basic_block, sizeof (int));
  FOR_EACH_BB (bb)
    {
      rtx last_insn = bb->end;
      rtx condition, earliest, reg;

      /* We only want conditional branches.  */
      if (GET_CODE (last_insn) != JUMP_INSN
	  || !any_condjump_p (last_insn)
	  || !onlyjump_p (last_insn))
	continue;

      /* LAST_INSN is a conditional jump.  Get its condition.  */
      condition = get_condition (last_insn, &earliest);

      /* If we were unable to get the condition, or it is not an equality
	 comparison against zero then there's nothing we can do.  */
      if (!condition
	  || (GET_CODE (condition) != NE && GET_CODE (condition) != EQ)
	  || GET_CODE (XEXP (condition, 1)) != CONST_INT
	  || (XEXP (condition, 1)
	      != CONST0_RTX (GET_MODE (XEXP (condition, 0)))))
	continue;

      /* We must be checking a register against zero.  */
      reg = XEXP (condition, 0);
      if (GET_CODE (reg) != REG)
	continue;

      block_reg[bb->index] = REGNO (reg);
    }

  /* Go through the algorithm for each block of registers.  */
  for (reg = FIRST_PSEUDO_REGISTER; reg < max_reg; reg += regs_per_pass)
    {
      npi.min_reg = reg;
      npi.max_reg = MIN (reg + regs_per_pass, max_reg);
      something_changed |= delete_null_pointer_checks_1 (block_reg,
							 nonnull_avin,
							 nonnull_avout,
							 &npi);
    }

  /* Free the table of registers compared at the end of every block.  */
  free (block_reg);

  /* Free bitmaps.  */
  sbitmap_vector_free (npi.nonnull_local);
  sbitmap_vector_free (npi.nonnull_killed);
  sbitmap_vector_free (nonnull_avin);
  sbitmap_vector_free (nonnull_avout);

  return something_changed;
}
