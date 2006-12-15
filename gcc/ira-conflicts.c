/* IRA conflict builder.
   Contributed by Vladimir Makarov.
   Copyright (C) 2006 Free Software Foundation, Inc.

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
   works on the pseudo basis.  */

static void set_pseudo_live (pseudo_t);
static void clear_pseudo_live (pseudo_t);
static void record_regno_conflict (int);
static void mark_reg_store (rtx, rtx, void *);
static void mark_reg_clobber (rtx, rtx, void *);
static void mark_reg_conflicts (rtx);
static void mark_reg_death (rtx);
static int commutative_constraint_p (const char *);
static int get_dup_num (int, int);
static rtx get_dup (int, int);
static void add_pseudo_copy_to_list (copy_t);
static void remove_pseudo_copy_from_list (copy_t);
static void swap_pseudo_copy_ends_if_necessary (copy_t);
static void add_pseudo_copies (rtx);
static enum reg_class single_reg_class (const char *, rtx op, rtx);
static enum reg_class single_reg_operand_class (int);
static void process_single_reg_class_operands (int);
static void process_bb_node_for_conflicts (struct ira_loop_tree_node *);
static void build_conflict_bit_table (void);
static void propagate_pseudo_info (pseudo_t);
static void propagate_info (void);
static void mirror_conflicts (void);
static void remove_conflict_pseudo_copies (void);
static void build_pseudo_conflict_vects (void);
static void propagate_modified_regnos (struct ira_loop_tree_node *);
static void print_conflicts (FILE *);

/* The number of bits in each element of `conflicts' and what
   type that element has.  We use the largest integer format on the
   host machine.  */
#define INT_BITS HOST_BITS_PER_WIDE_INT
#define INT_TYPE HOST_WIDE_INT

/* Bit mask for pseudos live at current point in the scan.  */
static INT_TYPE *pseudos_live;

/* The same as previous but as bitmap.  */
static bitmap pseudos_live_bitmap;

/* Set, clear or test bit number I in R, a bit vector indexed by
   pseudo number.  */
#define SET_PSEUDO_CONFLICT_ROW(R, I)				\
  ((R)[(unsigned) (I) / INT_BITS]				\
   |= ((INT_TYPE) 1 << ((unsigned) (I) % INT_BITS)))

#define CLEAR_PSEUDO_CONFLICT_ROW(R, I)				\
  ((R) [(unsigned) (I) / INT_BITS]				\
   &= ~((INT_TYPE) 1 << ((unsigned) (I) % INT_BITS)))

#define TEST_PSEUDO_CONFLICT_ROW(R, I)				\
  ((R) [(unsigned) (I) / INT_BITS]				\
   & ((INT_TYPE) 1 << ((unsigned) (I) % INT_BITS)))

/* Set, clear or test bit number I in `pseudos_live',
   a bit vector indexed by pseudo number.  */
#define SET_PSEUDO_LIVE(I) SET_PSEUDO_CONFLICT_ROW (pseudos_live, I)

#define CLEAR_PSEUDO_LIVE(I) CLEAR_PSEUDO_CONFLICT_ROW (pseudos_live, I)

#define TEST_PSEUDO_LIVE(I) TEST_PSEUDO_CONFLICT_ROW (pseudos_live, I)

/* pseudos_num by pseudos_num array of bits, recording whether two
   pseudo's conflict (can't go in the same hardware register).

   `conflicts' is symmetric after the call to mirror_conflicts.  */
static INT_TYPE *conflicts;

/* Number of ints required to hold pseudos_num bits.  This is the
   length of a row in `conflicts'.  */
static int pseudo_row_words;

/* Two macros to test 1 in an element of `conflicts'.  */
#define CONFLICTP(I, J) \
 (conflicts[(I) * pseudo_row_words + (unsigned) (J) / INT_BITS]	\
  & ((INT_TYPE) 1 << ((unsigned) (J) % INT_BITS)))

/* For each pseudo set in PSEUDO_SET, set PSEUDO to that pseudo, and
   execute CODE.  */
#define EXECUTE_IF_SET_IN_PSEUDO_SET(PSEUDO_SET, PSEUDO, CODE)		\
do {									\
  int i_;								\
  int pseudo_;								\
  INT_TYPE *p_ = (PSEUDO_SET);						\
									\
  for (i_ = pseudo_row_words - 1, pseudo_ = 0; i_ >= 0;			\
       i_--, pseudo_ += INT_BITS)					\
    {									\
      unsigned INT_TYPE word_ = (unsigned INT_TYPE) *p_++;		\
									\
      for ((PSEUDO) = pseudo_; word_; word_ >>= 1, (PSEUDO)++)		\
	{								\
	  if (word_ & 1)						\
	    {CODE;}							\
	}								\
    }									\
} while (0)


/* Set of hard regs (except eliminable ones) currently live (during
   scan of all insns).  */
static HARD_REG_SET hard_regs_live;

/* Loop tree node corresponding to the current basic block.  */
static struct ira_loop_tree_node *curr_bb_node;

/* Current map regno -> pseudo.  */
static pseudo_t *curr_regno_pseudo_map;

/* The function marks pseudo P as currently living.  */
static void
set_pseudo_live (pseudo_t p)
{
  if (TEST_PSEUDO_LIVE (PSEUDO_NUM (p)))
    return;
  SET_PSEUDO_LIVE (PSEUDO_NUM (p));
  bitmap_set_bit (pseudos_live_bitmap, PSEUDO_NUM (p));
}

/* The function marks pseudo P as currently not living.  */
static void
clear_pseudo_live (pseudo_t p)
{
  CLEAR_PSEUDO_LIVE (PSEUDO_NUM (p));
  bitmap_clear_bit (pseudos_live_bitmap, PSEUDO_NUM (p));
}

/* Record all regs that are set in any one insn.  Communication from
   mark_reg_{store,clobber} and build_conflict_bit_table.  */
static rtx *regs_set;

/* Number elelments in the previous array.  */
static int n_regs_set;

/* Record a conflict between hard register REGNO or pseudo
   corresponding to pseudo-register REGNO and everything currently
   live.  */
static void
record_regno_conflict (int regno)
{
  int j;

  if (regno < FIRST_PSEUDO_REGISTER)
    {
      /* When a hard register becomes live, record conflicts with live
	 pseudo regs.  */
      EXECUTE_IF_SET_IN_PSEUDO_SET (pseudos_live, j,
        {
	  SET_HARD_REG_BIT (PSEUDO_CONFLICT_HARD_REGS (pseudos [j]), regno);
	});
    }
  else
    {
      /* When a pseudo-register becomes live, record conflicts first
	 with hard regs, then with other pseudos.  */
      pseudo_t p = curr_regno_pseudo_map [regno];
      int pn, pn_prod;

      ira_assert (p != NULL || REG_N_REFS (regno) == 0);
      if (p == NULL)
	return;
      pn = PSEUDO_NUM (p);
      pn_prod = pn * pseudo_row_words;
      IOR_HARD_REG_SET (PSEUDO_CONFLICT_HARD_REGS (p), hard_regs_live);
      for (j = pseudo_row_words - 1; j >= 0; j--)
	conflicts [pn_prod + j] |= pseudos_live [j];
      /* Don't set up conflict for the pseudo with itself.  */
      CLEAR_PSEUDO_CONFLICT_ROW (conflicts + pn_prod, pn);
    }
}

/* Handle the case where REG is set by the insn being scanned, during
   the scan to accumulate conflicts.  Store a 1 in hard_regs_live or
   pseudos_live for this register or the corresponding pseudo, record
   how many consecutive hardware registers it actually needs, and
   record a conflict with all other pseudos already live.

   Note that even if REG does not remain alive after this insn, we
   must mark it here as live, to ensure a conflict between REG and any
   other pseudos set in this insn that really do live.  This is
   because those other pseudos could be considered after this.

   REG might actually be something other than a register; if so, we do
   nothing.

   SETTER is 0 if this register was modified by an auto-increment
   (i.e., a REG_INC note was found for it).  */
static void
mark_reg_store (rtx reg, rtx setter ATTRIBUTE_UNUSED,
		void *data ATTRIBUTE_UNUSED)
{
  int regno;

  if (GET_CODE (reg) == SUBREG)
    reg = SUBREG_REG (reg);

  if (! REG_P (reg))
    return;

  regs_set [n_regs_set++] = reg;

  regno = REGNO (reg);

  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      pseudo_t p = curr_regno_pseudo_map [regno];

      ira_assert (p != NULL || REG_N_REFS (regno) == 0);
      if (p != NULL)
	set_pseudo_live (p);
      record_regno_conflict (regno);
    }
  else if (! TEST_HARD_REG_BIT (no_alloc_regs, regno))
    {
      int last = regno + hard_regno_nregs [regno] [GET_MODE (reg)];

      while (regno < last)
	{
	  record_regno_conflict (regno);
	  if (! TEST_HARD_REG_BIT (eliminable_regset, regno))
	    SET_HARD_REG_BIT (hard_regs_live, regno);
	  regno++;
	}
    }
}

/* Like mark_reg_store except notice just CLOBBERs; ignore SETs.  */
static void
mark_reg_clobber (rtx reg, rtx setter, void *data)
{
  if (GET_CODE (setter) == CLOBBER)
    mark_reg_store (reg, setter, data);
}

/* Record that REG (or the corresponding pseudo) has conflicts with
   all the pseudo currently live.  Do not mark REG (or the pseudo)
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
    record_regno_conflict (regno);
  else if (! TEST_HARD_REG_BIT (no_alloc_regs, regno))
    {
      int last = regno + hard_regno_nregs [regno] [GET_MODE (reg)];

      while (regno < last)
	{
	  record_regno_conflict (regno);
	  regno++;
	}
    }
}

/* Mark REG (or the corresponding pseudo) as being dead (following the
   insn being scanned now).  Store a 0 in hard_regs_live or
   pseudos_live for this register.  */
static void
mark_reg_death (rtx reg)
{
  int regno = REGNO (reg);

  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      pseudo_t p = curr_regno_pseudo_map [regno];

      ira_assert (p != NULL || REG_N_REFS (regno) == 0);
      if (p != NULL)
	clear_pseudo_live (p);
    }
  else if (! TEST_HARD_REG_BIT (no_alloc_regs, regno))
    {
      int last = regno + hard_regno_nregs [regno] [GET_MODE (reg)];

      while (regno < last)
	{
	  CLEAR_HARD_REG_BIT (hard_regs_live, regno);
	  regno++;
	}
    }
}

/* The function returns nonzero, if the operand constraint STR is
   commutative.  */
static int
commutative_constraint_p (const char *str)
{
  int ignore_p;
  int c;

  for (ignore_p = FALSE;;)
    {
      c = *str;
      if (c == '\0')
	break;
      str += CONSTRAINT_LEN (c, str);
      if (c == '#')
	ignore_p = TRUE;
      else if (c == ',')
	ignore_p = FALSE;
      else if (! ignore_p)
	{
	  if (c == '%')
	    return TRUE;
	}
    }
  return FALSE;
}

/* The function returns number of the operand which should be the same
   in any case as operand with number OP_NUM.  If USE_COMMUT_OP_P is
   non-zero, the function makes temporarily commutative operand
   exchange before this.  The function takes only really possible
   alternatives into consideration.  */
static int
get_dup_num (int op_num, int use_commut_op_p)
{
  int curr_alt, c, original, dup;
  int ignore_p, commut_op_used_p;
  const char *str;
  rtx op, equiv_const = NULL_RTX; /* ??? */

  if (op_num < 0 || recog_data.n_alternatives == 0)
    return -1;
  op = recog_data.operand [op_num];
  ira_assert (REG_P (op));
  commut_op_used_p = TRUE;
  if (use_commut_op_p)
    {
      if (commutative_constraint_p (recog_data.constraints [op_num]))
	op_num++;
      else if (op_num > 0 && commutative_constraint_p (recog_data.constraints
						       [op_num - 1]))
	op_num--;
      else
	commut_op_used_p = FALSE;
    }
  str = recog_data.constraints [op_num];
  for (ignore_p = FALSE, original = -1, curr_alt = 0;;)
    {
      c = *str;
      if (c == '\0')
	break;
      if (c == '#')
	ignore_p = TRUE;
      else if (c == ',')
	{
	  curr_alt++;
	  ignore_p = FALSE;
	}
      else if (! ignore_p)
	switch (c)
	  {
	  case 'X':
	    return -1;
	    
	  case 'i':
	    if (equiv_const != NULL_RTX && CONSTANT_P (equiv_const))
	      return -1;
	    break;

	  case 'n':
	    if (equiv_const != NULL_RTX
		&& (GET_CODE (equiv_const) == CONST_INT
		    || (GET_CODE (equiv_const) == CONST_DOUBLE
			&& GET_MODE (equiv_const) == VOIDmode)))
	      return -1;
	    break;
	    
	  case 's':
	    if (equiv_const != NULL_RTX
		&& CONSTANT_P (equiv_const)
		&& GET_CODE (equiv_const) != CONST_INT
		&& (GET_CODE (equiv_const) != CONST_DOUBLE
		    || GET_MODE (equiv_const) != VOIDmode))
	      return -1;
	    break;
	    
	  case 'I':
	  case 'J':
	  case 'K':
	  case 'L':
	  case 'M':
	  case 'N':
	  case 'O':
	  case 'P':
	    if ((equiv_const != NULL_RTX
		 && GET_CODE (equiv_const) == CONST_INT
		 && CONST_OK_FOR_CONSTRAINT_P (INTVAL (equiv_const),
					       c, str)))
	      return -1;
	    break;
	    
	  case 'E':
	  case 'F':
	    if (equiv_const != NULL_RTX
		&& (GET_CODE (equiv_const) == CONST_DOUBLE
		    || (GET_CODE (equiv_const) == CONST_VECTOR
			&& (GET_MODE_CLASS (GET_MODE (equiv_const))
			    == MODE_VECTOR_FLOAT))))
	      return -1;
	    break;
	    
	  case 'G':
	  case 'H':
	    if (equiv_const != NULL_RTX
		&& GET_CODE (equiv_const) == CONST_DOUBLE
		&& CONST_DOUBLE_OK_FOR_CONSTRAINT_P (equiv_const, c, str))
	      return -1;
	    break;

	  case 'm':
	  case 'o':
	    /* Accept a register which might be placed in memory.  */
	    return -1;
	    break;

	  case 'V':
	  case '<':
	  case '>':
	    break;

	  case 'p':
	    GO_IF_LEGITIMATE_ADDRESS (VOIDmode, op, win_p);
	    break;
	    
	  win_p:
	    return -1;
	  
	  case 'g':
	    return -1;
	    
	  case 'r':
	  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
	  case 'h': case 'j': case 'k': case 'l':
	  case 'q': case 't': case 'u':
	  case 'v': case 'w': case 'x': case 'y': case 'z':
	  case 'A': case 'B': case 'C': case 'D':
	  case 'Q': case 'R': case 'S': case 'T': case 'U':
	  case 'W': case 'Y': case 'Z':
	    {
	      enum reg_class cl;

	      cl = (c == 'r'
		    ? GENERAL_REGS : REG_CLASS_FROM_CONSTRAINT (c, str));
	      if (cl != NO_REGS)
		return -1;
#ifdef EXTRA_CONSTRAINT_STR
	      else if (EXTRA_CONSTRAINT_STR (op, c, str))
		return -1;
#endif
	      break;
	    }
	    
	  case '0': case '1': case '2': case '3': case '4':
	  case '5': case '6': case '7': case '8': case '9':
	    if (original != -1 && original != c)
	      return -1;
	    original = c;
	    break;
	  }
      str += CONSTRAINT_LEN (c, str);
    }
  if (original == -1)
    return -1;
  dup = original - '0';
  if (use_commut_op_p)
    {
      if (commutative_constraint_p (recog_data.constraints [dup]))
	dup++;
      else if (dup > 0
	       && commutative_constraint_p (recog_data.constraints [dup -1]))
	dup--;
      else if (! commut_op_used_p)
	return -1;
    }
  return dup;
}

/* The function returns operand which should be in any case the same
   as operand with number OP_NUM.  If USE_COMMUT_OP_P is non-zero, the
   function makes temporarily commutative operand exchange before
   this.  */
static rtx
get_dup (int op_num, int use_commut_op_p)
{
  int n = get_dup_num (op_num, use_commut_op_p);

  if (n < 0)
    return NULL_RTX;
  else
    return recog_data.operand [n];
}

/* The function attaches copy CP to pseudos involved into the copy.  */
static void
add_pseudo_copy_to_list (copy_t cp)
{
  pseudo_t first = cp->first, second = cp->second;

  cp->prev_first_pseudo_copy = NULL;
  cp->prev_second_pseudo_copy = NULL;
  cp->next_first_pseudo_copy = PSEUDO_COPIES (first);
  if (cp->next_first_pseudo_copy != NULL)
    {
      if (cp->next_first_pseudo_copy->first == first)
	cp->next_first_pseudo_copy->prev_first_pseudo_copy = cp;
      else
	cp->next_first_pseudo_copy->prev_second_pseudo_copy = cp;
    }
  cp->next_second_pseudo_copy = PSEUDO_COPIES (second);
  if (cp->next_second_pseudo_copy != NULL)
    {
      if (cp->next_second_pseudo_copy->second == second)
	cp->next_second_pseudo_copy->prev_second_pseudo_copy = cp;
      else
	cp->next_second_pseudo_copy->prev_first_pseudo_copy = cp;
    }
  PSEUDO_COPIES (first) = cp;
  PSEUDO_COPIES (second) = cp;
}

/* The function detaches copy CP from pseudos involved into the copy.  */
static void
remove_pseudo_copy_from_list (copy_t cp)
{
  pseudo_t first = cp->first, second = cp->second;
  copy_t prev, next;

  next = cp->next_first_pseudo_copy;
  prev = cp->prev_first_pseudo_copy;
  if (prev == NULL)
    PSEUDO_COPIES (first) = next;
  else if (prev->first == first)
    prev->next_first_pseudo_copy = next;
  else
    prev->next_second_pseudo_copy = next;
  if (next != NULL)
    {
      if (next->first == first)
	next->prev_first_pseudo_copy = prev;
      else
	next->prev_second_pseudo_copy = prev;
    }
  cp->prev_first_pseudo_copy = cp->next_first_pseudo_copy = NULL;

  next = cp->next_second_pseudo_copy;
  prev = cp->prev_second_pseudo_copy;
  if (prev == NULL)
    PSEUDO_COPIES (second) = next;
  else if (prev->second == second)
    prev->next_second_pseudo_copy = next;
  else
    prev->next_first_pseudo_copy = next;
  if (next != NULL)
    {
      if (next->second == second)
	next->prev_second_pseudo_copy = prev;
      else
	next->prev_first_pseudo_copy = prev;
    }
  cp->prev_second_pseudo_copy = cp->next_second_pseudo_copy = NULL;
}

/* The function makes copy CP a canonical copy where number of the
   first pseudo is less than the second one.  */
static void
swap_pseudo_copy_ends_if_necessary (copy_t cp)
{
  pseudo_t temp;
  copy_t temp_cp;

  if (PSEUDO_NUM (cp->first) <= PSEUDO_NUM (cp->second))
    return;

  temp = cp->first;
  cp->first = cp->second;
  cp->second = temp;

  temp_cp = cp->prev_first_pseudo_copy;
  cp->prev_first_pseudo_copy = cp->prev_second_pseudo_copy;
  cp->prev_second_pseudo_copy = temp_cp;

  temp_cp = cp->next_first_pseudo_copy;
  cp->next_first_pseudo_copy = cp->next_second_pseudo_copy;
  cp->next_second_pseudo_copy = temp_cp;
}

/* The function creates and returns new copy of pseudos FIRST and
   SECOND with frequency FREQ corresponding to move insn INSN (if
   any).  */
copy_t
add_pseudo_copy (pseudo_t first, pseudo_t second, int freq, rtx insn)
{
  copy_t cp;

  cp = create_copy (first, second, freq, insn);
  ira_assert (first != NULL && second != NULL);
  add_pseudo_copy_to_list (cp);
  swap_pseudo_copy_ends_if_necessary (cp);
  return cp;
}

/* The function processes INSN and create pseudo copies if
   necessary.  For example, it might be because INSN is a
   pseudo-register move or INSN is two operand insn.  */
static void
add_pseudo_copies (rtx insn)
{
  rtx set, operand, dup;
  const char *str;
  int commut_p;
  int i, j, freq, hard_regno, cost, index;
  copy_t cp;
  pseudo_t p;
  enum reg_class class, cover_class;
  enum machine_mode mode;

  freq = REG_FREQ_FROM_BB (BLOCK_FOR_INSN (insn));
  if (freq == 0)
    freq = 1;
  if ((set = single_set (insn)) != NULL_RTX
      && REG_P (SET_DEST (set)) && REG_P (SET_SRC (set))
      && ! side_effects_p (set)
      && find_reg_note (insn, REG_DEAD, SET_SRC (set)) != NULL_RTX
      && find_reg_note (insn, REG_RETVAL, NULL_RTX) == NULL_RTX)
    {
      if (HARD_REGISTER_P (SET_SRC (set)) || HARD_REGISTER_P (SET_DEST (set)))
	{
	  if (HARD_REGISTER_P (SET_DEST (set)))
	    {
	      if (HARD_REGISTER_P (SET_SRC (set)))
		return;
	      hard_regno = REGNO (SET_DEST (set));
	      p = curr_regno_pseudo_map [REGNO (SET_SRC (set))];
	    }
	  else
	    {
	      hard_regno = REGNO (SET_SRC (set));
	      p = curr_regno_pseudo_map [REGNO (SET_DEST (set))];
	    }
	  class = REGNO_REG_CLASS (hard_regno);
	  mode = PSEUDO_MODE (p);
	  cover_class = PSEUDO_COVER_CLASS (p);
	  if (! class_subset_p [class] [cover_class])
	    return;
	  if (reg_class_size [class]
	      <= (unsigned) CLASS_MAX_NREGS (class, mode))
	    /* It is already taken into account in ira-costs.c.  */
	    return;
	  index = class_hard_reg_index [cover_class] [hard_regno];
	  if (index < 0)
	    return;
	  if (HARD_REGISTER_P (SET_DEST (set)))
	    cost = register_move_cost [mode] [cover_class] [class] * freq;
	  else
	    cost = register_move_cost [mode] [class] [cover_class] * freq;
	  PSEUDO_HARD_REG_COSTS (p) [index] -= cost;
	  PSEUDO_CONFLICT_HARD_REG_COSTS (p) [index] -= cost;
	}
      else
	{
	  cp = add_pseudo_copy (curr_regno_pseudo_map [REGNO (SET_DEST (set))],
				curr_regno_pseudo_map [REGNO (SET_SRC (set))],
				freq, insn);
	  bitmap_set_bit (ira_curr_loop_tree_node->local_copies, cp->num); 
	}
    }
  else
    {
      extract_insn (insn);
      for (i = 0; i < recog_data.n_operands; i++)
	{
	  operand = recog_data.operand [i];
	  if (REG_P (operand)
	      && find_reg_note (insn, REG_DEAD, operand) != NULL_RTX)
	    {
	      str = recog_data.constraints [i];
	      while (*str == ' ' && *str == '\t')
		str++;
	      for (j = 0, commut_p = FALSE; j < 2; j++, commut_p = TRUE)
		if ((dup = get_dup (i, commut_p)) != NULL_RTX
		    && REG_P (dup) && GET_MODE (operand) == GET_MODE (dup))
		  {
		    if (HARD_REGISTER_NUM_P (REGNO (operand))
			|| HARD_REGISTER_NUM_P (REGNO (dup)))
		      {
			if (HARD_REGISTER_P (operand))
			  {
			    if (HARD_REGISTER_P (dup))
			      continue;
			    hard_regno = REGNO (operand);
			    p = curr_regno_pseudo_map [REGNO (dup)];
			  }
			else
			  {
			    hard_regno = REGNO (dup);
			    p = curr_regno_pseudo_map [REGNO (operand)];
			  }
			class = REGNO_REG_CLASS (hard_regno);
			mode = PSEUDO_MODE (p);
			cover_class = PSEUDO_COVER_CLASS (p);
			if (! class_subset_p [class] [cover_class])
			  continue;
			index
			  = class_hard_reg_index [cover_class] [hard_regno];
			if (index < 0)
			  continue;
			if (HARD_REGISTER_P (operand))
			  cost = (register_move_cost
				  [mode] [cover_class] [class] * freq);
			else
			  cost = (register_move_cost
				  [mode] [class] [cover_class] * freq);
			PSEUDO_HARD_REG_COSTS (p) [index] -= cost;
			PSEUDO_CONFLICT_HARD_REG_COSTS (p) [index] -= cost;
		      }
		    else
		      {
			cp = add_pseudo_copy
			     (curr_regno_pseudo_map [REGNO (dup)],
			      curr_regno_pseudo_map [REGNO (operand)],
			      freq, NULL_RTX);
			bitmap_set_bit
			  (ira_curr_loop_tree_node->local_copies, cp->num);
		      }
		  }
	    }
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

/* The function processes input (if IN_P) or output operands to find
   pseudo which can use only one hard register and makes other
   currently living pseudos conflicting with the hard register.  */
static void
process_single_reg_class_operands (int in_p)
{
  int i, regno, px;
  enum reg_class cl, cover_class;
  rtx operand;
  pseudo_t operand_p, p;

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

      operand_p = NULL;

      if (GET_CODE (operand) == SUBREG)
	operand = SUBREG_REG (operand);
      
      if (REG_P (operand)
	  && (regno = REGNO (operand)) >= FIRST_PSEUDO_REGISTER)
	{
	  enum machine_mode mode;
	  enum reg_class cover_class;

	  operand_p = curr_regno_pseudo_map [regno];
	  mode = PSEUDO_MODE (operand_p);
	  cover_class = PSEUDO_MODE (operand_p);
	  if (class_subset_p [cl] [cover_class]
	      && (reg_class_size [cl]
		  <= (unsigned) CLASS_MAX_NREGS (cl, mode)))
	    PSEUDO_CONFLICT_HARD_REG_COSTS (operand_p)
	      [class_hard_reg_index [cover_class] [class_hard_regs [cl] [0]]]
	      -= (in_p
		  ? register_move_cost [mode] [cover_class] [cl]
		  : register_move_cost [mode] [cl] [cover_class]);
	}

      EXECUTE_IF_SET_IN_PSEUDO_SET (pseudos_live, px,
        {
	  p = pseudos [px];
	  cover_class = PSEUDO_COVER_CLASS (p);
	  if (p != operand_p)
	    /* We could increase costs of P instead of making it
	       conflicting with the hard register.  But it works worse
	       because it will be spilled in reload in anyway.  */
	    IOR_HARD_REG_SET (PSEUDO_CONFLICT_HARD_REGS (p),
			      reg_class_contents [cl]);
	});
    }
}

/* The function processes insns of the basic block given by its
   LOOP_TREE_NODE to update pseudo conflict table.  */
static void
process_bb_node_for_conflicts (struct ira_loop_tree_node *loop_tree_node)
{
  int i;
  unsigned int j;
  basic_block bb;
  rtx insn;
  edge e;
  edge_iterator ei;
  bitmap_iterator bi;
  bitmap reg_live_in, reg_live_out;
  int px = 0;

  bb = loop_tree_node->bb;
  if (bb == NULL)
    return;
  curr_bb_node = loop_tree_node;
  curr_regno_pseudo_map = ira_curr_loop_tree_node->regno_pseudo_map;
  reg_live_in = DF_UPWARD_LIVE_IN (build_df, bb);
  reg_live_out = DF_UPWARD_LIVE_OUT (build_df, bb);
  memset (pseudos_live, 0, pseudo_row_words * sizeof (INT_TYPE));
  REG_SET_TO_HARD_REG_SET (hard_regs_live, reg_live_in);
  AND_COMPL_HARD_REG_SET (hard_regs_live, eliminable_regset);
  bitmap_clear (pseudos_live_bitmap);
  EXECUTE_IF_SET_IN_BITMAP (reg_live_in, FIRST_PSEUDO_REGISTER, j, bi)
    {
      pseudo_t p = curr_regno_pseudo_map [j];
      
      ira_assert (p != NULL || REG_N_REFS (j) == 0);
      if (p == NULL)
	continue;
      set_pseudo_live (p);
      record_regno_conflict (j);
    }

#ifdef EH_RETURN_DATA_REGNO
  if (bb_has_eh_pred (bb))
    {
      for (j = 0; ; ++j)
	{
	  unsigned int regno = EH_RETURN_DATA_REGNO (j);

	  if (regno == INVALID_REGNUM)
	    break;
	  record_regno_conflict (regno);
	}
    }
#endif

  /* Pseudos can't go in stack regs at the start of a basic block that
     is reached by an abnormal edge. Likewise for call clobbered regs,
     because caller-save, fixup_abnormal_edges and possibly the table
     driven EH machinery are not quite ready to handle such pseudos
     live across such edges.  */
  FOR_EACH_EDGE (e, ei, bb->preds)
    if (e->flags & EDGE_ABNORMAL)
      break;
  
  if (e != NULL)
    {
#ifdef STACK_REGS
      EXECUTE_IF_SET_IN_PSEUDO_SET (pseudos_live, px,
        {
	  PSEUDO_NO_STACK_REG_P (pseudos [px]) = TRUE;
	});
      for (px = FIRST_STACK_REG; px <= LAST_STACK_REG; px++)
	record_regno_conflict (px);
#endif
      /* No need to record conflicts for call clobbered regs if we
	 have nonlocal labels around, as we don't ever try to allocate
	 such regs in this case.  */
      if (! current_function_has_nonlocal_label)
	for (px = 0; px < FIRST_PSEUDO_REGISTER; px++)
	  if (call_used_regs [px])
	    record_regno_conflict (px);
    }
  
  /* Scan the code of this basic block, noting which pseudos and hard
     regs are born or die.  When one is born, record a conflict with
     all others currently live.  */
  FOR_BB_INSNS (bb, insn)
    {
      rtx link;
      
      if (! INSN_P (insn))
	continue;
      
      /* Make regs_set an empty set.  */
      n_regs_set = 0;
      
      /* Mark any pseudos clobbered by INSN as live, so they
	 conflict with the inputs.  */
      note_stores (PATTERN (insn), mark_reg_clobber, NULL);
      
      extract_insn (insn);
      process_single_reg_class_operands (TRUE);
      
      /* Mark any pseudos dead after INSN as dead now.  */
      for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
	if (REG_NOTE_KIND (link) == REG_DEAD)
	  mark_reg_death (XEXP (link, 0));
      
      if (CALL_P (insn))
	{
	  EXECUTE_IF_SET_IN_PSEUDO_SET (pseudos_live, i,
	    {
	      int freq;
	      pseudo_t p = pseudos [i];
	      
	      freq = REG_FREQ_FROM_BB (BLOCK_FOR_INSN (insn));
	      if (freq == 0)
		freq = 1;
	      PSEUDO_CALL_FREQ (p) += freq;
	      PSEUDO_CALLS_CROSSED (p) [PSEUDO_CALLS_CROSSED_NUM (p)++] = insn;
	      ira_assert (PSEUDO_CALLS_CROSSED_NUM (p)
			  <= REG_N_CALLS_CROSSED (PSEUDO_REGNO (p)));

	      /* Don't allocate pseudos that cross calls, if this
		 function receives a nonlocal goto.  */
	      if (current_function_has_nonlocal_label)
		SET_HARD_REG_SET (PSEUDO_CONFLICT_HARD_REGS (p));
	    });
	}
      
      /* Mark any pseudos set in INSN as live, and mark them as
	 conflicting with all other live pseudos.  Clobbers are
	 processed again, so they conflict with the pseudos that are
	 set.  */
      note_stores (PATTERN (insn), mark_reg_store, NULL);
      
#ifdef AUTO_INC_DEC
      for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
	if (REG_NOTE_KIND (link) == REG_INC)
	  mark_reg_store (XEXP (link, 0), NULL_RTX, NULL);
#endif
      
      /* If INSN has multiple outputs, then any pseudo that dies here
	 and is used inside of an output must conflict with the other
	 outputs.
	 
	 It is unsafe to use !single_set here since it will ignore an
	 unused output.  Just because an output is unused does not
	 mean the compiler can assume the side effect will not occur.
	 Consider if PSEUDO appears in the address of an output and we
	 reload the output.  If we allocate PSEUDO to the same hard
	 register as an unused output we could set the hard register
	 before the output reload insn.  */
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
      
      process_single_reg_class_operands (FALSE);
      
      /* Mark any pseudos set in INSN and then never used.  */
      while (n_regs_set-- > 0)
	{
	  rtx note = find_regno_note (insn, REG_UNUSED,
				      REGNO (regs_set [n_regs_set]));
	  if (note)
	    mark_reg_death (XEXP (note, 0));
	}
      add_pseudo_copies (insn);
    }
}

/* The function builds pseudo conflict table by traversing all basic
   blocks and their insns.  */
static void
build_conflict_bit_table (void)
{
  pseudo_row_words = (pseudos_num + INT_BITS - 1) / INT_BITS;
  conflicts = ira_allocate (sizeof (INT_TYPE)
			    * pseudos_num * pseudo_row_words);
  memset (conflicts, 0, sizeof (INT_TYPE) * pseudos_num * pseudo_row_words);
  pseudos_live = ira_allocate (sizeof (INT_TYPE) * pseudo_row_words);
  pseudos_live_bitmap = ira_allocate_bitmap ();
  /* Make a vector that mark_reg_{store,clobber} will store in.  */
  regs_set = ira_allocate (sizeof (rtx) * max_parallel * 2);
  traverse_loop_tree (ira_loop_tree_root, process_bb_node_for_conflicts, NULL);
  /* Clean up.  */
  ira_free (regs_set);
  ira_free_bitmap (pseudos_live_bitmap);
  ira_free (pseudos_live);
}

/* The function propagates info about pseudo P to the corresponding
   pseudo on upper loop tree level.  So pseudos on upper levels
   accumulate information about the corresponding pseudos in nested
   loops.  */
static void
propagate_pseudo_info (pseudo_t p)
{
  int regno, j, n, pn, father_pn, another_father_pn;
  pseudo_t father_p, another_p, another_father_p;
  struct ira_loop_tree_node *father;
  struct pseudo_copy *cp, *next_cp;

  regno = PSEUDO_REGNO (p);
  if ((father = PSEUDO_LOOP_TREE_NODE (p)->father) != NULL
      && (father_p = father->regno_pseudo_map [regno]) != NULL)
    {
      PSEUDO_CALL_FREQ (father_p) += PSEUDO_CALL_FREQ (p);
#ifdef STACK_REGS
      if (PSEUDO_NO_STACK_REG_P (p))
	PSEUDO_NO_STACK_REG_P (father_p) = TRUE;
#endif
      IOR_HARD_REG_SET (PSEUDO_CONFLICT_HARD_REGS (father_p),
			PSEUDO_CONFLICT_HARD_REGS (p));
      pn = PSEUDO_NUM (p);
      EXECUTE_IF_SET_IN_PSEUDO_SET (conflicts + pn * pseudo_row_words, j,
        {
	  another_p = pseudos [j];
	  if ((another_father_p = (father->regno_pseudo_map
				   [PSEUDO_REGNO (another_p)])) == NULL)
	    continue;
	  father_pn = PSEUDO_NUM (father_p);
	  another_father_pn = PSEUDO_NUM (another_father_p);
	  SET_PSEUDO_CONFLICT_ROW
	    (conflicts + father_pn * pseudo_row_words, another_father_pn);
	  SET_PSEUDO_CONFLICT_ROW
	    (conflicts + another_father_pn * pseudo_row_words, father_pn);
	});
      if ((n = PSEUDO_CALLS_CROSSED_NUM (p)) != 0)
	{
	  memcpy (PSEUDO_CALLS_CROSSED (father_p)
		  + PSEUDO_CALLS_CROSSED_NUM (father_p),
		  PSEUDO_CALLS_CROSSED (p), sizeof (rtx) * n);
	  PSEUDO_CALLS_CROSSED_NUM (father_p) += n;
	  ira_assert (PSEUDO_CALLS_CROSSED_NUM (father_p)
		      <= REG_N_CALLS_CROSSED (regno));
	}
      for (cp = PSEUDO_COPIES (p); cp != NULL; cp = next_cp)
	{
	  if (cp->first == p)
	    {
	      next_cp = cp->next_first_pseudo_copy;
	      another_p = cp->second;
	    }
	  else if (cp->second == p)
	    {
	      next_cp = cp->next_second_pseudo_copy;
	      another_p = cp->first;
	    }
	  else
	    gcc_unreachable ();
	  if ((another_father_p = (father->regno_pseudo_map
				   [PSEUDO_REGNO (another_p)])) != NULL)
	    add_pseudo_copy
	      (father_p, another_father_p, cp->freq, cp->move_insn);
	}
    }
}

/* The function propagates info about pseudos to the corresponding
   pseudos on upper loop tree level.  */
static void
propagate_info (void)
{
  int i;
  pseudo_t p;

  for (i = max_reg_num () - 1; i >= FIRST_PSEUDO_REGISTER; i--)
    for (p = regno_pseudo_map [i]; p != NULL; p = PSEUDO_NEXT_REGNO_PSEUDO (p))
      propagate_pseudo_info (p);
}

/* If CONFLICTP (i, j) is TRUE, make sure CONFLICTP (j, i) is also TRUE.  */
static void
mirror_conflicts (void)
{
  int i, j;
  unsigned INT_TYPE mask;
  int rw = pseudo_row_words;
  int rwb = rw * INT_BITS;
  INT_TYPE *p = conflicts;
  INT_TYPE *q0 = conflicts;
  INT_TYPE *q1, *q2;

  for (i = pseudos_num - 1, mask = 1; i >= 0; i--, mask <<= 1)
    {
      if (! mask)
	{
	  mask = 1;
	  q0++;
	}
      for (j = pseudo_row_words - 1, q1 = q0; j >= 0; j--, q1 += rwb)
	{
	  unsigned INT_TYPE word;

	  for (word = (unsigned INT_TYPE) *p++, q2 = q1;
	       word;
	       word >>= 1, q2 += rw)
	    {
	      if (word & 1)
		*q2 |= mask;
	    }
	}
    }
}

/* The function returns TRUE if pseudo-registers REGNO1 and REGNO2
   conflict.  The function is called from reload.  */
int
pseudo_reg_conflict_p (int regno1, int regno2)
{
  int p_no1, p_no2;

  ira_assert (regno1 >= FIRST_PSEUDO_REGISTER
	      && regno2 >= FIRST_PSEUDO_REGISTER);
  /* Reg info caclulated by dataflow infrastructure can be different
     from one calculated by regclass.  */
  if (curr_regno_pseudo_map [regno1] == NULL
      || curr_regno_pseudo_map [regno2] == NULL)
    return FALSE;
  p_no1 = PSEUDO_NUM (curr_regno_pseudo_map [regno1]);
  p_no2 = PSEUDO_NUM (curr_regno_pseudo_map [regno2]);
  ira_assert (p_no1 >= 0 && p_no1 < pseudos_num
	      && p_no2 >= 0 && p_no2 < pseudos_num);
  return CONFLICTP (p_no1, p_no2) != 0;
}

/* Remove copies involving conflicting pseudos.  */
static void
remove_conflict_pseudo_copies (void)
{
  int i;
  pseudo_t p;
  copy_t cp, next_cp;
  varray_type conflict_pseudo_copy_varray;

  VARRAY_GENERIC_PTR_NOGC_INIT (conflict_pseudo_copy_varray, get_max_uid (),
				"copies of conflicting pseudos");
  for (i = 0; i < pseudos_num; i++)
    {
      p = pseudos [i];
      for (cp = PSEUDO_COPIES (p); cp != NULL; cp = next_cp)
	if (cp->first == p)
	  next_cp = cp->next_first_pseudo_copy;
	else
	  {
	    next_cp = cp->next_second_pseudo_copy;
	    VARRAY_PUSH_GENERIC_PTR (conflict_pseudo_copy_varray, cp);
	  }
    }
  for (i = VARRAY_ACTIVE_SIZE (conflict_pseudo_copy_varray) - 1; i >= 0; i--)
    {
      cp = VARRAY_GENERIC_PTR (conflict_pseudo_copy_varray, i);
      if (CONFLICTP (PSEUDO_NUM (cp->first), PSEUDO_NUM (cp->second)))
	remove_pseudo_copy_from_list (cp);
    }
  VARRAY_FREE (conflict_pseudo_copy_varray);
}

/* The function builds conflict vectors of all pseudos from the
   conflict table.  */
static void
build_pseudo_conflict_vects (void)
{
  int i, j, px;
  pseudo_t p, *conflict_pseudos, *vec;

  conflict_pseudos = ira_allocate (sizeof (pseudo_t) * pseudos_num);
  for (i = 0; i < pseudos_num; i++)
    {
      p = pseudos [i];
      ira_assert (i == PSEUDO_NUM (p));
      px = 0;
      EXECUTE_IF_SET_IN_PSEUDO_SET (conflicts + i * pseudo_row_words, j,
				    {
				      conflict_pseudos [px++] = pseudos [j];
				    });
      allocate_pseudo_conflicts (p, px);
      vec = PSEUDO_CONFLICT_PSEUDO_VEC (p);
      memcpy (vec, conflict_pseudos, sizeof (pseudo_t) * px);
      vec [px] = NULL;
      PSEUDO_CONFLICT_PSEUDO_VEC_ACTIVE_SIZE (p) = px;
    }
  ira_free (conflict_pseudos);
}



/* The function propagates information about pseudos modified inside
   the loops.  */
static void
propagate_modified_regnos (struct ira_loop_tree_node *loop_tree_node)
{
  if (loop_tree_node->bb != NULL || loop_tree_node == ira_loop_tree_root)
    return;
  bitmap_ior_into (loop_tree_node->father->modified_regnos,
		   loop_tree_node->modified_regnos);
}



/* The function outputs information about pseudo conflicts to FILE.  */
static void
print_conflicts (FILE *file)
{
  int i;

  for (i = 0; i < pseudos_num; i++)
    {
      int j;
      pseudo_t p;
      basic_block bb;

      p = pseudos [i];
      fprintf (file, ";; p%d(r%d,", PSEUDO_NUM (p), PSEUDO_REGNO (p));
      if ((bb = PSEUDO_LOOP_TREE_NODE (p)->bb) != NULL)
	fprintf (file, "b%d", bb->index);
      else
	fprintf (file, "l%d", PSEUDO_LOOP_TREE_NODE (p)->loop->num);
      fprintf (file, ") conflicts:");
      for (j = 0; j < pseudos_num; j++)
	if (CONFLICTP (j, i))
	  {
	    fprintf (file, " p%d(r%d,",
		     PSEUDO_NUM (pseudos [j]), PSEUDO_REGNO (pseudos [j]));
	    if ((bb = PSEUDO_LOOP_TREE_NODE (pseudos [j])->bb) != NULL)
	      fprintf (file, "b%d)", bb->index);
	    else
	      fprintf (file, "l%d)",
		       PSEUDO_LOOP_TREE_NODE (pseudos [j])->loop->num);
	  }
      fprintf (file, "\n;;     conflict hard regs:");
      for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
	if (TEST_HARD_REG_BIT (PSEUDO_CONFLICT_HARD_REGS (p), j))
	  fprintf (file, " %d", j);
      fprintf (file, "\n");

    }
  fprintf (file, "\n");
}

/* The function outputs information about pseudo conflicts to
   stderr.  */
void
debug_conflicts (void)
{
  print_conflicts (stderr);
}



/* Entry function which builds pseudo conflicts.  */
void
ira_build_conflicts (void)
{
  int i;
  pseudo_t p;

  build_conflict_bit_table ();
  mirror_conflicts ();
  if (flag_ira_algorithm == IRA_ALGORITHM_REGIONAL)
    propagate_info ();
  /* We need finished conflict table for the subsequent call.  */
  remove_conflict_pseudo_copies ();
  build_pseudo_conflict_vects ();
  for (i = 0; i < pseudos_num; i++)
    {
      p = pseudos [i];
      if (PSEUDO_CALLS_CROSSED_NUM (p) != 0)
	{
	  if (! flag_caller_saves)
            IOR_HARD_REG_SET (PSEUDO_CONFLICT_HARD_REGS (p),
			      call_used_reg_set);
          else
	    IOR_HARD_REG_SET (PSEUDO_CONFLICT_HARD_REGS (p),
			      no_caller_save_reg_set);
	}
    }  
  traverse_loop_tree (ira_loop_tree_root, NULL, propagate_modified_regnos);
  if (ira_dump_file != NULL)
    print_conflicts (ira_dump_file);
}
