/* IRA processing allocno lives.
   Copyright (C) 2006, 2007, 2008
   Free Software Foundation, Inc.
   Contributed by Vladimir Makarov <vmakarov@redhat.com>.

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
#include "regs.h"
#include "rtl.h"
#include "tm_p.h"
#include "target.h"
#include "flags.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "insn-config.h"
#include "recog.h"
#include "toplev.h"
#include "params.h"
#include "df.h"
#include "ira-int.h"

/* The file contains code is analogous to one in global but the code
   works on the allocno basis.  */

static void make_regno_born (int);
static void update_allocno_pressure_excess_length (allocno_t);
static void set_allocno_live (allocno_t);
static void clear_allocno_live (allocno_t);
static void mark_reg_store (rtx, const_rtx, void *);
static void mark_reg_clobber (rtx, const_rtx, void *);
static void mark_reg_conflicts (rtx);
static void mark_reg_death (rtx);
static enum reg_class single_reg_class (const char *, rtx op, rtx);
static enum reg_class single_reg_operand_class (int);
static void process_single_reg_class_operands (int, int);
static void process_bb_node_lives (loop_tree_node_t);

/* Program points are enumerated by number from range
   0..MAX_POINT-1.  */
int max_point;

/* Arrays of size MAX_POINT mapping a program point to the allocno
   live ranges with given start/finish point.  */
allocno_live_range_t *start_point_ranges, *finish_point_ranges;

/* Number of the current program point.  */
static int curr_point;

/* Point where register pressure excess started of -1 if there is no
   register pressure excess.  */
static int high_pressure_start_point [N_REG_CLASSES];

/* Number of ints required to hold allocnos_num bits.  */
int allocno_set_words;

/* Set, clear or test bit number I in `allocnos_live',
   a bit vector indexed by allocno number.  */
#define SET_ALLOCNO_LIVE(I) SET_ALLOCNO_SET_BIT (allocnos_live, I)
#define CLEAR_ALLOCNO_LIVE(I) CLEAR_ALLOCNO_SET_BIT (allocnos_live, I)
#define TEST_ALLOCNO_LIVE(I) TEST_ALLOCNO_SET_BIT (allocnos_live, I)

/* Bit mask for allocnos live at current point in the scan.  */
static INT_TYPE *allocnos_live;

/* The same as previous but as bitmap.  */
static bitmap allocnos_live_bitmap;

/* Set of hard regs (except eliminable ones) currently live (during
   scan of all insns).  */
static HARD_REG_SET hard_regs_live;

/* Loop tree node corresponding to the current basic block.  */
static loop_tree_node_t curr_bb_node;

/* The function processing birth of register REGNO.  It updates living
   hard regs and conflict hard regs for living allocnos or starts a
   new live range for allocno corresponding to REGNO.  */
static void
make_regno_born (int regno)
{
  int i;
  allocno_t a;
  allocno_live_range_t p;

  if (regno < FIRST_PSEUDO_REGISTER)
    {
      SET_HARD_REG_BIT (hard_regs_live, regno);
      EXECUTE_IF_SET_IN_ALLOCNO_SET (allocnos_live, i,
        {
	  SET_HARD_REG_BIT (ALLOCNO_CONFLICT_HARD_REGS (allocnos [i]), regno);
	  SET_HARD_REG_BIT (ALLOCNO_TOTAL_CONFLICT_HARD_REGS (allocnos [i]),
			    regno);
	});
      return;
    }
  a = ira_curr_regno_allocno_map [regno];
  if (a == NULL)
    return;
  if ((p = ALLOCNO_LIVE_RANGES (a)) == NULL
      || (p->finish != curr_point && p->finish + 1 != curr_point))
    ALLOCNO_LIVE_RANGES (a)
      = create_allocno_live_range (a, curr_point, -1, ALLOCNO_LIVE_RANGES (a));
}

/* Update ALLOCNO_EXCESS_PRESSURE_POINTS_NUM for allocno A.  */
static void
update_allocno_pressure_excess_length (allocno_t a)
{
  int start;
  enum reg_class cover_class;
  allocno_live_range_t p;

  cover_class = ALLOCNO_COVER_CLASS (a);
  if (high_pressure_start_point [cover_class] < 0)
    return;
  p = ALLOCNO_LIVE_RANGES (a);
  ira_assert (p != NULL);
  start = (high_pressure_start_point [cover_class] > p->start
	   ? high_pressure_start_point [cover_class] : p->start);
  ALLOCNO_EXCESS_PRESSURE_POINTS_NUM (a) += curr_point - start + 1;
}

/* The function processing death of register REGNO.  It updates live
   hard regs or finish the current live range for allocno
   corresponding to REGNO.  */
static void
make_regno_dead (int regno)
{
  allocno_t a;
  allocno_live_range_t p;

  if (regno < FIRST_PSEUDO_REGISTER)
    {
      CLEAR_HARD_REG_BIT (hard_regs_live, regno);
      return;
    }
  a = ira_curr_regno_allocno_map [regno];
  if (a == NULL)
    return;
  p = ALLOCNO_LIVE_RANGES (a);
  ira_assert (p != NULL);
  p->finish = curr_point;
  update_allocno_pressure_excess_length (a);
}

/* The function processing birth and, right after then, death of
   register REGNO.  */
static void
make_regno_born_and_died (int regno)
{
  make_regno_born (regno);
  make_regno_dead (regno);
}

/* The current pressure for the current basic block.  */
static int curr_reg_pressure [N_REG_CLASSES];

/* The function marks allocno A as currently living.  */
static void
set_allocno_live (allocno_t a)
{
  int nregs;
  enum reg_class cover_class;

  if (TEST_ALLOCNO_LIVE (ALLOCNO_NUM (a)))
    return;
  SET_ALLOCNO_LIVE (ALLOCNO_NUM (a));
  IOR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a), hard_regs_live);
  IOR_HARD_REG_SET (ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a), hard_regs_live);
  bitmap_set_bit (allocnos_live_bitmap, ALLOCNO_NUM (a));
  cover_class = ALLOCNO_COVER_CLASS (a);
  nregs = reg_class_nregs [cover_class] [ALLOCNO_MODE (a)];
  curr_reg_pressure [cover_class] += nregs;
  if (high_pressure_start_point [cover_class] < 0
      && curr_reg_pressure [cover_class] > available_class_regs [cover_class])
    high_pressure_start_point [cover_class] = curr_point;
  if (curr_bb_node->reg_pressure [cover_class]
      < curr_reg_pressure [cover_class])
    curr_bb_node->reg_pressure [cover_class] = curr_reg_pressure [cover_class];
}

/* The function marks allocno A as currently not living.  */
static void
clear_allocno_live (allocno_t a)
{
  int i;
  enum reg_class cover_class;

  if (bitmap_bit_p (allocnos_live_bitmap, ALLOCNO_NUM (a)))
    {
      cover_class = ALLOCNO_COVER_CLASS (a);
      curr_reg_pressure [cover_class]
	-= reg_class_nregs [cover_class] [ALLOCNO_MODE (a)];
      ira_assert (curr_reg_pressure [cover_class] >= 0);
      if (high_pressure_start_point [cover_class] >= 0
	  && (curr_reg_pressure [cover_class]
	      <= available_class_regs [cover_class]))
	{
	  EXECUTE_IF_SET_IN_ALLOCNO_SET (allocnos_live, i,
	    {
	      update_allocno_pressure_excess_length (allocnos [i]);
	    });
	  high_pressure_start_point [cover_class] = -1;
	}
    }
  CLEAR_ALLOCNO_LIVE (ALLOCNO_NUM (a));
  bitmap_clear_bit (allocnos_live_bitmap, ALLOCNO_NUM (a));
}

/* Record all regs that are set in any one insn.  Communication from
   mark_reg_{store,clobber}.  */
static VEC(rtx, heap) *regs_set;

/* Handle the case where REG is set by the insn being scanned, during
   the scan to accumulate conflicts.  Store a 1 in hard_regs_live or
   allocnos_live for this register or the
   corresponding allocno, record how many consecutive hardware
   registers it actually needs, and record a conflict with all other
   reg allocnos already live.

   Note that even if REG does not remain alive after this insn, we
   must mark it here as live, to ensure a conflict between REG and any
   other reg allocnos set in this insn that really do live.  This is
   because those other allocnos could be considered after this.

   REG might actually be something other than a register; if so, we do
   nothing.

   SETTER is 0 if this register was modified by an auto-increment
   (i.e., a REG_INC note was found for it).  */
static void
mark_reg_store (rtx reg, const_rtx setter ATTRIBUTE_UNUSED,
		void *data ATTRIBUTE_UNUSED)
{
  int regno;

  if (GET_CODE (reg) == SUBREG)
    reg = SUBREG_REG (reg);

  if (! REG_P (reg))
    return;

  VEC_safe_push (rtx, heap, regs_set, reg);

  regno = REGNO (reg);

  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      allocno_t a = ira_curr_regno_allocno_map [regno];

      if (a != NULL)
	{
	  if (bitmap_bit_p (allocnos_live_bitmap, ALLOCNO_NUM (a)))
	    return;
	  set_allocno_live (a);
	}
      make_regno_born (regno);
    }
  else if (! TEST_HARD_REG_BIT (no_alloc_regs, regno))
    {
      int last = regno + hard_regno_nregs [regno] [GET_MODE (reg)];
      enum reg_class cover_class;

      while (regno < last)
	{
	  if (! TEST_HARD_REG_BIT (hard_regs_live, regno)
	      && ! TEST_HARD_REG_BIT (eliminable_regset, regno))
	    {
	      cover_class = class_translate [REGNO_REG_CLASS (regno)];
	      curr_reg_pressure [cover_class]++;
	      if (high_pressure_start_point [cover_class] < 0
		  && (curr_reg_pressure [cover_class]
		      > available_class_regs [cover_class]))
		high_pressure_start_point [cover_class] = curr_point;
	      make_regno_born (regno);
	      if (curr_bb_node->reg_pressure [cover_class]
		  < curr_reg_pressure [cover_class])
		curr_bb_node->reg_pressure [cover_class]
		  = curr_reg_pressure [cover_class];
	    }
	  regno++;
	}
    }
}

/* Like mark_reg_store except notice just CLOBBERs; ignore SETs.  */
static void
mark_reg_clobber (rtx reg, const_rtx setter, void *data)
{
  if (GET_CODE (setter) == CLOBBER)
    mark_reg_store (reg, setter, data);
}

/* Record that REG (or the corresponding allocno) has conflicts with
   all the allocno currently live.  Do not mark REG (or the allocno)
   itself as live.  */
static void
mark_reg_conflicts (rtx reg)
{
  int regno;

  if (GET_CODE (reg) == SUBREG)
    reg = SUBREG_REG (reg);

  if (! REG_P (reg))
    return;

  regno = REGNO (reg);

  if (regno >= FIRST_PSEUDO_REGISTER)
    make_regno_born_and_died (regno);
  else if (! TEST_HARD_REG_BIT (no_alloc_regs, regno))
    {
      int last = regno + hard_regno_nregs [regno] [GET_MODE (reg)];

      while (regno < last)
	{
	  make_regno_born_and_died (regno);
	  regno++;
	}
    }
}

/* Mark REG (or the corresponding allocno) as being dead (following
   the insn being scanned now).  Store a 0 in hard_regs_live or
   allocnos_live for this register.  */
static void
mark_reg_death (rtx reg)
{
  int i;
  int regno = REGNO (reg);

  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      allocno_t a = ira_curr_regno_allocno_map [regno];

      if (a != NULL)
	{
	  if (! bitmap_bit_p (allocnos_live_bitmap, ALLOCNO_NUM (a)))
	    return;
	  clear_allocno_live (a);
	}
      make_regno_dead (regno);
    }
  else if (! TEST_HARD_REG_BIT (no_alloc_regs, regno))
    {
      int last = regno + hard_regno_nregs [regno] [GET_MODE (reg)];
      enum reg_class cover_class;

      while (regno < last)
	{
	  if (TEST_HARD_REG_BIT (hard_regs_live, regno))
	    {
	      cover_class = class_translate [REGNO_REG_CLASS (regno)];
	      curr_reg_pressure [cover_class]--;
	      if (high_pressure_start_point [cover_class] >= 0
		  && (curr_reg_pressure [cover_class]
		      <= available_class_regs [cover_class]))
		{
		  EXECUTE_IF_SET_IN_ALLOCNO_SET (allocnos_live, i,
		    {
		      update_allocno_pressure_excess_length (allocnos [i]);
		    });
		  high_pressure_start_point [cover_class] = -1;
		}
	      ira_assert (curr_reg_pressure [cover_class] >= 0);
	      make_regno_dead (regno);
	    }
	  regno++;
	}
    }
}

/* The function checks that CONSTRAINTS permits to use only one hard
   register.  If it is so, the function returns the class of the hard
   register.  Otherwise it returns NO_REGS.

   EQUIV_COSNT ??? */
static enum reg_class
single_reg_class (const char *constraints, rtx op, rtx equiv_const)
{
  int ignore_p;
  enum reg_class cl, next_cl;
  int c;

  cl = NO_REGS;
  for (ignore_p = FALSE;
       (c = *constraints);
       constraints += CONSTRAINT_LEN (c, constraints))
    if (c == '#')
      ignore_p = TRUE;
    else if (c == ',')
      ignore_p = FALSE;
    else if (! ignore_p)
      switch (c)
	{
	case ' ':
	case '\t':
	case '=':
	case '+':
	case '*':
	case '&':
	case '%':
	case '!':
	case '?':
	  break;
	case 'i':
	  if (CONSTANT_P (op)
	      || (equiv_const != NULL_RTX && CONSTANT_P (equiv_const)))
	    return NO_REGS;
	  break;

	case 'n':
	  if (GET_CODE (op) == CONST_INT
	      || (GET_CODE (op) == CONST_DOUBLE && GET_MODE (op) == VOIDmode)
	      || (equiv_const != NULL_RTX
		  && (GET_CODE (equiv_const) == CONST_INT
		      || (GET_CODE (equiv_const) == CONST_DOUBLE
			  && GET_MODE (equiv_const) == VOIDmode))))
	    return NO_REGS;
	  break;
	  
	case 's':
	  if ((CONSTANT_P (op) && GET_CODE (op) != CONST_INT
	       && (GET_CODE (op) != CONST_DOUBLE || GET_MODE (op) != VOIDmode))
	      || (equiv_const != NULL_RTX
		  && CONSTANT_P (equiv_const)
		  && GET_CODE (equiv_const) != CONST_INT
		  && (GET_CODE (equiv_const) != CONST_DOUBLE
		      || GET_MODE (equiv_const) != VOIDmode)))
	    return NO_REGS;
	  break;
	  
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	  if ((GET_CODE (op) == CONST_INT
	       && CONST_OK_FOR_CONSTRAINT_P (INTVAL (op), c, constraints))
	      || (equiv_const != NULL_RTX
		  && GET_CODE (equiv_const) == CONST_INT
		  && CONST_OK_FOR_CONSTRAINT_P (INTVAL (equiv_const),
						c, constraints)))
	    return NO_REGS;
	  break;
	  
	case 'E':
	case 'F':
	  if (GET_CODE (op) == CONST_DOUBLE
	      || (GET_CODE (op) == CONST_VECTOR
		  && GET_MODE_CLASS (GET_MODE (op)) == MODE_VECTOR_FLOAT)
	      || (equiv_const != NULL_RTX
		  && (GET_CODE (equiv_const) == CONST_DOUBLE
		      || (GET_CODE (equiv_const) == CONST_VECTOR
			  && (GET_MODE_CLASS (GET_MODE (equiv_const))
			      == MODE_VECTOR_FLOAT)))))
	    return NO_REGS;
	  break;
	  
	case 'G':
	case 'H':
	  if ((GET_CODE (op) == CONST_DOUBLE
	       && CONST_DOUBLE_OK_FOR_CONSTRAINT_P (op, c, constraints))
	      || (equiv_const != NULL_RTX
		  && GET_CODE (equiv_const) == CONST_DOUBLE
		  && CONST_DOUBLE_OK_FOR_CONSTRAINT_P (equiv_const,
						       c, constraints)))
	    return NO_REGS;
	  /* ??? what about memory */
	case 'r':
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
	case 'h': case 'j': case 'k': case 'l':
	case 'q': case 't': case 'u':
	case 'v': case 'w': case 'x': case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D':
	case 'Q': case 'R': case 'S': case 'T': case 'U':
	case 'W': case 'Y': case 'Z':
	  next_cl = (c == 'r'
		     ? GENERAL_REGS
		     : REG_CLASS_FROM_CONSTRAINT (c, constraints));
	  if ((cl != NO_REGS && next_cl != cl)
	      || available_class_regs [next_cl] > 1)
	    return NO_REGS;
	  cl = next_cl;
	  break;
	  
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	  next_cl
	    = single_reg_class (recog_data.constraints [c - '0'],
				recog_data.operand [c - '0'], NULL_RTX);
	  if ((cl != NO_REGS && next_cl != cl) || next_cl == NO_REGS
	      || available_class_regs [next_cl] > 1)
	    return NO_REGS;
	  cl = next_cl;
	  break;
	  
	default:
	  return NO_REGS;
	}
  return cl;
}

/* The function checks that operand OP_NUM of the current insn can use
   only one hard register.  If it is so, the function returns the
   class of the hard register.  Otherwise it returns NO_REGS.  */
static enum reg_class
single_reg_operand_class (int op_num)
{
  if (op_num < 0 || recog_data.n_alternatives == 0)
    return NO_REGS;
  return single_reg_class (recog_data.constraints [op_num],
			   recog_data.operand [op_num], NULL_RTX);
}

/* The function processes input (if IN_P) or output operands of insn
   with FREQ to find allocno which can use only one hard register and
   makes other currently living reg allocnos conflicting with the hard
   register.  */
static void
process_single_reg_class_operands (int in_p, int freq)
{
  int i, regno, px, cost;
  enum reg_class cl, cover_class;
  rtx operand;
  allocno_t operand_a, a;

  for (i = 0; i < recog_data.n_operands; i++)
    {
      operand = recog_data.operand [i];
      if (in_p && recog_data.operand_type [i] != OP_IN
	  && recog_data.operand_type [i] != OP_INOUT)
	continue;
      if (! in_p && recog_data.operand_type [i] != OP_OUT
	  && recog_data.operand_type [i] != OP_INOUT)
	continue;
      cl = single_reg_operand_class (i);
      if (cl == NO_REGS)
	continue;

      operand_a = NULL;

      if (GET_CODE (operand) == SUBREG)
	operand = SUBREG_REG (operand);
      
      if (REG_P (operand)
	  && (regno = REGNO (operand)) >= FIRST_PSEUDO_REGISTER)
	{
	  enum machine_mode mode;
	  enum reg_class cover_class;

	  operand_a = ira_curr_regno_allocno_map [regno];
	  mode = ALLOCNO_MODE (operand_a);
	  cover_class = ALLOCNO_MODE (operand_a);
	  if (class_subset_p [cl] [cover_class]
	      && class_hard_regs_num [cl] != 0
	      && class_hard_reg_index [cover_class] [class_hard_regs
						     [cl] [0]] >= 0
	      && (reg_class_size [cl]
		  <= (unsigned) CLASS_MAX_NREGS (cl, mode)))
	    {
	      /* ??? FREQ */
	      cost = freq * (in_p
			     ? register_move_cost [mode] [cover_class] [cl]
			     : register_move_cost [mode] [cl] [cover_class]);
	      allocate_and_set_costs
		(&ALLOCNO_CONFLICT_HARD_REG_COSTS (operand_a), cover_class, 0);
	      ALLOCNO_CONFLICT_HARD_REG_COSTS (operand_a)
		[class_hard_reg_index [cover_class] [class_hard_regs [cl] [0]]]
		-= cost;
	    }
	}

      EXECUTE_IF_SET_IN_ALLOCNO_SET (allocnos_live, px,
        {
	  a = allocnos [px];
	  cover_class = ALLOCNO_COVER_CLASS (a);
	  if (a != operand_a)
	    {
	      /* We could increase costs of A instead of making it
		 conflicting with the hard register.  But it works worse
		 because it will be spilled in reload in anyway.  */
	      IOR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a),
				reg_class_contents [cl]);
	      IOR_HARD_REG_SET (ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a),
				reg_class_contents [cl]);
	    }
	});
    }
}

/* The function processes insns of the basic block given by its
   LOOP_TREE_NODE to update allocno conflict table.  */
static void
process_bb_node_lives (loop_tree_node_t loop_tree_node)
{
  int i, index;
  unsigned int j;
  basic_block bb;
  rtx insn;
  edge e;
  edge_iterator ei;
  bitmap_iterator bi;
  bitmap reg_live_in;
  int px = 0;

  bb = loop_tree_node->bb;
  if (bb != NULL)
    {
      for (i = 0; i < reg_class_cover_size; i++)
	{
	  curr_reg_pressure [reg_class_cover [i]] = 0;
	  high_pressure_start_point [reg_class_cover [i]] = -1;
	}
      curr_bb_node = loop_tree_node;
      reg_live_in = DF_LR_IN (bb);
      memset (allocnos_live, 0, allocno_set_words * sizeof (INT_TYPE));
      REG_SET_TO_HARD_REG_SET (hard_regs_live, reg_live_in);
      AND_COMPL_HARD_REG_SET (hard_regs_live, eliminable_regset);
      /* ??? !!!! No alloc regs for pressure */
      AND_COMPL_HARD_REG_SET (hard_regs_live, no_alloc_regs);
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	if (TEST_HARD_REG_BIT (hard_regs_live, i))
	  {
	    enum reg_class cover_class;
	    
	    cover_class = class_translate [REGNO_REG_CLASS (i)];
	    curr_reg_pressure [cover_class]++;
	    if (curr_bb_node->reg_pressure [cover_class]
		< curr_reg_pressure [cover_class])
	      curr_bb_node->reg_pressure [cover_class]
		= curr_reg_pressure [cover_class];
	    ira_assert (curr_reg_pressure [cover_class]
			<= available_class_regs [cover_class]);
	  }
      bitmap_clear (allocnos_live_bitmap);
      EXECUTE_IF_SET_IN_BITMAP (reg_live_in, FIRST_PSEUDO_REGISTER, j, bi)
	{
	  allocno_t a = ira_curr_regno_allocno_map [j];
	  
	  if (a == NULL)
	    continue;
	  ira_assert (! bitmap_bit_p (allocnos_live_bitmap, ALLOCNO_NUM (a)));
	  set_allocno_live (a);
	  make_regno_born (j);
	}
      
#ifdef EH_RETURN_DATA_REGNO
      if (bb_has_eh_pred (bb))
	{
	  for (j = 0; ; ++j)
	    {
	      unsigned int regno = EH_RETURN_DATA_REGNO (j);
	      
	      if (regno == INVALID_REGNUM)
		break;
	      make_regno_born_and_died (regno);
	    }
	}
#endif
      
      /* Reg allocnos can't go in stack regs at the start of a basic block
	 that is reached by an abnormal edge. Likewise for call
	 clobbered regs, because caller-save, fixup_abnormal_edges and
	 possibly the table driven EH machinery are not quite ready to
	 handle such reg allocnos live across such edges.  */
      FOR_EACH_EDGE (e, ei, bb->preds)
	if (e->flags & EDGE_ABNORMAL)
	  break;
      
      if (e != NULL)
	{
#ifdef STACK_REGS
	  EXECUTE_IF_SET_IN_ALLOCNO_SET (allocnos_live, px,
	    {
	      ALLOCNO_NO_STACK_REG_P (allocnos [px]) = TRUE;
	      ALLOCNO_TOTAL_NO_STACK_REG_P (allocnos [px]) = TRUE;
	    });
	  for (px = FIRST_STACK_REG; px <= LAST_STACK_REG; px++)
	    make_regno_born_and_died (px);
#endif
	  /* No need to record conflicts for call clobbered regs if we
	     have nonlocal labels around, as we don't ever try to
	     allocate such regs in this case.  */
	  if (! current_function_has_nonlocal_label)
	    for (px = 0; px < FIRST_PSEUDO_REGISTER; px++)
	      if (call_used_regs [px])
		make_regno_born_and_died (px);
	}
  
      /* Scan the code of this basic block, noting which allocnos and
	 hard regs are born or die.  When one is born, record a
	 conflict with all others currently live.  */
      FOR_BB_INSNS (bb, insn)
	{
	  rtx link;
	  int freq;
	  
	  if (! INSN_P (insn))
	    continue;
	  
	  freq = REG_FREQ_FROM_BB (BLOCK_FOR_INSN (insn));
	  if (freq == 0)
	    freq = 1;

	  if (internal_flag_ira_verbose > 2 && ira_dump_file != NULL)
	    fprintf (ira_dump_file, "   Insn %u(l%d): point = %d\n",
		     INSN_UID (insn), loop_tree_node->father->loop->num,
		     curr_point);

	  /* Check regs_set is an empty set.  */
	  gcc_assert (VEC_empty (rtx, regs_set));
      
	  /* Mark any allocnos clobbered by INSN as live, so they
	     conflict with the inputs.  */
	  note_stores (PATTERN (insn), mark_reg_clobber, NULL);
	  
	  extract_insn (insn);
	  process_single_reg_class_operands (TRUE, freq);
	  
	  /* Mark any allocnos dead after INSN as dead now.  */
	  for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
	    if (REG_NOTE_KIND (link) == REG_DEAD)
	      mark_reg_death (XEXP (link, 0));
	  
	  curr_point++;
	  if (CALL_P (insn))
	    {
	      HARD_REG_SET clobbered_regs;
	      
	      get_call_invalidated_used_regs (insn, &clobbered_regs, FALSE);
	      IOR_HARD_REG_SET (cfun->emit->call_used_regs, clobbered_regs);
	      EXECUTE_IF_SET_IN_ALLOCNO_SET (allocnos_live, i,
	        {
		  allocno_t a = allocnos [i];
		  
		  ALLOCNO_CALL_FREQ (a) += freq;
		  index = add_regno_call (ALLOCNO_REGNO (a), insn);
		  if (ALLOCNO_CALLS_CROSSED_START (a) < 0)
		    ALLOCNO_CALLS_CROSSED_START (a) = index;
		  ALLOCNO_CALLS_CROSSED_NUM (a)++;
		  /* Don't allocate allocnos that cross calls, if this
		     function receives a nonlocal goto.  */
		  if (current_function_has_nonlocal_label)
		    {
		      SET_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a));
		      SET_HARD_REG_SET (ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a));
		    }
		});
	    }
	  
	  /* Mark any allocnos set in INSN as live, and mark them as
	     conflicting with all other live reg allocnos.  Clobbers are
	     processed again, so they conflict with the reg allocnos that
	     are set.  */
	  note_stores (PATTERN (insn), mark_reg_store, NULL);
	  
#ifdef AUTO_INC_DEC
	  for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
	    if (REG_NOTE_KIND (link) == REG_INC)
	      mark_reg_store (XEXP (link, 0), NULL_RTX, NULL);
#endif
	  
	  /* If INSN has multiple outputs, then any allocno that dies
	     here and is used inside of an output must conflict with
	     the other outputs.
	     
	     It is unsafe to use !single_set here since it will ignore
	     an unused output.  Just because an output is unused does
	     not mean the compiler can assume the side effect will not
	     occur.  Consider if ALLOCNO appears in the address of an
	     output and we reload the output.  If we allocate ALLOCNO
	     to the same hard register as an unused output we could
	     set the hard register before the output reload insn.  */
	  if (GET_CODE (PATTERN (insn)) == PARALLEL && multiple_sets (insn))
	    for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
	      if (REG_NOTE_KIND (link) == REG_DEAD)
		{
		  int i;
		  int used_in_output = 0;
		  rtx reg = XEXP (link, 0);
		  
		  for (i = XVECLEN (PATTERN (insn), 0) - 1; i >= 0; i--)
		    {
		      rtx set = XVECEXP (PATTERN (insn), 0, i);
		      
		      if (GET_CODE (set) == SET
			  && ! REG_P (SET_DEST (set))
			  && ! rtx_equal_p (reg, SET_DEST (set))
			  && reg_overlap_mentioned_p (reg, SET_DEST (set)))
			used_in_output = 1;
		    }
		  if (used_in_output)
		    mark_reg_conflicts (reg);
		}
	  
	  process_single_reg_class_operands (FALSE, freq);
	  
	  /* Mark any allocnos set in INSN and then never used.  */
	  while (! VEC_empty (rtx, regs_set))
	    {
	      rtx reg = VEC_pop (rtx, regs_set);
	      rtx note = find_regno_note (insn, REG_UNUSED, REGNO (reg));

	      if (note)
		mark_reg_death (XEXP (note, 0));
	    }
	  curr_point++;
	}
      EXECUTE_IF_SET_IN_ALLOCNO_SET (allocnos_live, i,
       {
	 make_regno_dead (ALLOCNO_REGNO (allocnos [i]));
       });
      curr_point++;
    }
  /* Propagate register pressure: */
  if (loop_tree_node != ira_loop_tree_root)
    for (i = 0; i < reg_class_cover_size; i++)
      {
	enum reg_class cover_class;

	cover_class = reg_class_cover [i];
	if (loop_tree_node->reg_pressure [cover_class]
	    > loop_tree_node->father->reg_pressure [cover_class])
	  loop_tree_node->father->reg_pressure [cover_class]
	    = loop_tree_node->reg_pressure [cover_class];
      }
}

static void
create_start_finish_chains (void)
{
  int i;
  allocno_t a;
  allocno_live_range_t r;

  start_point_ranges
    = ira_allocate (max_point * sizeof (allocno_live_range_t));
  memset (start_point_ranges, 0, max_point * sizeof (allocno_live_range_t));
  finish_point_ranges
    = ira_allocate (max_point * sizeof (allocno_live_range_t));
  memset (finish_point_ranges, 0, max_point * sizeof (allocno_live_range_t));
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      for (r = ALLOCNO_LIVE_RANGES (a); r != NULL; r = r->next)
	{
	  r->start_next = start_point_ranges [r->start];
	  start_point_ranges [r->start] = r;
	  r->finish_next = finish_point_ranges [r->finish];
 	  finish_point_ranges [r->finish] = r;
	}
    }
}

void
rebuild_start_finish_chains (void)
{
  ira_free (finish_point_ranges);
  ira_free (start_point_ranges);
  create_start_finish_chains ();
}

void
print_live_range_list (FILE *f, allocno_live_range_t r)
{
  for (; r != NULL; r = r->next)
    fprintf (f, " [%d..%d]", r->start, r->finish);
  fprintf (f, "\n");
}

void
debug_live_range_list (allocno_live_range_t r)
{
  print_live_range_list (stderr, r);
}

static void
print_allocno_live_ranges (FILE *f, allocno_t a)
{
  fprintf (f, " a%d(r%d):", ALLOCNO_NUM (a), ALLOCNO_REGNO (a));
  print_live_range_list (f, ALLOCNO_LIVE_RANGES (a));
}

void
debug_allocno_live_ranges (allocno_t a)
{
  print_allocno_live_ranges (stderr, a);
}

static void
print_live_ranges (FILE *f)
{
  int i;

  for (i = 0; i < allocnos_num; i++)
    print_allocno_live_ranges (f, allocnos [i]);
}

void
debug_live_ranges (void)
{
  print_live_ranges (stderr);
}

/* The function creates live ranges, set up CONFLICT_HARD_REGS and
   TOTAL_CONFLICT_HARD_REGS for allocnos.  It also modifies hard reg
   costs for allocnos whose hard register is actually fixed in an
   insn.  */
void
create_allocno_live_ranges (void)
{
  allocno_set_words = (allocnos_num + INT_BITS - 1) / INT_BITS;
  allocnos_live = ira_allocate (sizeof (INT_TYPE) * allocno_set_words);
  allocnos_live_bitmap = ira_allocate_bitmap ();
  /* Make a vector that mark_reg_{store,clobber} will store in.  */
  if (!regs_set)
    regs_set = VEC_alloc (rtx, heap, 10);
  curr_point = 0;
  traverse_loop_tree (FALSE, ira_loop_tree_root, NULL, process_bb_node_lives);
  max_point = curr_point;
  create_start_finish_chains ();
  if (internal_flag_ira_verbose > 2 && ira_dump_file != NULL)
    print_live_ranges (ira_dump_file);
  /* Clean up.  */
  ira_free_bitmap (allocnos_live_bitmap);
  ira_free (allocnos_live);
}

void
finish_allocno_live_ranges (void)
{
  ira_free (finish_point_ranges);
  ira_free (start_point_ranges);
}
