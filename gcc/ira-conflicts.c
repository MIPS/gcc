/* IRA conflict builder.
   Copyright (C) 2006, 2007
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

static void set_allocno_live (allocno_t);
static void clear_allocno_live (allocno_t);
static void record_regno_conflict (int);
static void mark_reg_store (rtx, const_rtx, void *);
static void mark_reg_clobber (rtx, const_rtx, void *);
static void mark_reg_conflicts (rtx);
static void mark_reg_death (rtx);
static int commutative_constraint_p (const char *);
static int get_dup_num (int, int);
static rtx get_dup (int, int);
static void add_allocno_copy_to_list (copy_t);
static void remove_allocno_copy_from_list (copy_t);
static void swap_allocno_copy_ends_if_necessary (copy_t);
static void add_allocno_copies (rtx);
static enum reg_class single_reg_class (const char *, rtx op, rtx);
static enum reg_class single_reg_operand_class (int);
static void process_single_reg_class_operands (int);
static void process_bb_node_for_conflicts (struct ira_loop_tree_node *);
static void build_conflict_bit_table (void);
static void propagate_allocno_info (allocno_t);
static void propagate_info (void);
static void mirror_conflicts (void);
static void remove_conflict_allocno_copies (void);
static void build_allocno_conflict_vects (void);
static void propagate_modified_regnos (struct ira_loop_tree_node *);
static void print_conflicts (FILE *);

/* The number of bits in each element of `conflicts' and what
   type that element has.  We use the largest integer format on the
   host machine.  */
#define INT_BITS HOST_BITS_PER_WIDE_INT
#define INT_TYPE HOST_WIDE_INT

/* Bit mask for allocnos live at current point in the scan.  */
static INT_TYPE *allocnos_live;

/* The same as previous but as bitmap.  */
static bitmap allocnos_live_bitmap;

/* Set, clear or test bit number I in R, a bit vector indexed by
   allocno number.  */
#define SET_ALLOCNO_CONFLICT_ROW(R, I)				\
  ((R)[(unsigned) (I) / INT_BITS]				\
   |= ((INT_TYPE) 1 << ((unsigned) (I) % INT_BITS)))

#define CLEAR_ALLOCNO_CONFLICT_ROW(R, I)				\
  ((R) [(unsigned) (I) / INT_BITS]				\
   &= ~((INT_TYPE) 1 << ((unsigned) (I) % INT_BITS)))

#define TEST_ALLOCNO_CONFLICT_ROW(R, I)				\
  ((R) [(unsigned) (I) / INT_BITS]				\
   & ((INT_TYPE) 1 << ((unsigned) (I) % INT_BITS)))

/* Set, clear or test bit number I in `allocnos_live',
   a bit vector indexed by allocno number.  */
#define SET_ALLOCNO_LIVE(I) SET_ALLOCNO_CONFLICT_ROW (allocnos_live, I)

#define CLEAR_ALLOCNO_LIVE(I) CLEAR_ALLOCNO_CONFLICT_ROW (allocnos_live, I)

#define TEST_ALLOCNO_LIVE(I) TEST_ALLOCNO_CONFLICT_ROW (allocnos_live, I)

/* allocnos_num by allocnos_num array of bits, recording whether two
   allocno's conflict (can't go in the same hardware register).

   `conflicts' is symmetric after the call to mirror_conflicts.  */
static INT_TYPE *conflicts;

/* Number of ints required to hold allocnos_num bits.  This is the
   length of a row in `conflicts'.  */
static int allocno_row_words;

/* Two macros to test 1 in an element of `conflicts'.  */
#define CONFLICTP(I, J) \
 (conflicts[(I) * allocno_row_words + (unsigned) (J) / INT_BITS]	\
  & ((INT_TYPE) 1 << ((unsigned) (J) % INT_BITS)))

/* For each allocno set in ALLOCNO_SET, set ALLOCNO to that allocno, and
   execute CODE.  */
#define EXECUTE_IF_SET_IN_ALLOCNO_SET(ALLOCNO_SET, ALLOCNO, CODE)		\
do {									\
  int i_;								\
  int allocno_;								\
  INT_TYPE *p_ = (ALLOCNO_SET);						\
									\
  for (i_ = allocno_row_words - 1, allocno_ = 0; i_ >= 0;			\
       i_--, allocno_ += INT_BITS)					\
    {									\
      unsigned INT_TYPE word_ = (unsigned INT_TYPE) *p_++;		\
									\
      for ((ALLOCNO) = allocno_; word_; word_ >>= 1, (ALLOCNO)++)		\
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

/* Current map regno -> allocno.  */
static allocno_t *curr_regno_allocno_map;

/* The current pressure for the current basic block.  */
static int curr_reg_pressure [N_REG_CLASSES];

/* The function marks allocno A as currently living.  */
static void
set_allocno_live (allocno_t a)
{
  enum reg_class cover_class;

  if (TEST_ALLOCNO_LIVE (ALLOCNO_NUM (a)))
    return;
  SET_ALLOCNO_LIVE (ALLOCNO_NUM (a));
  bitmap_set_bit (allocnos_live_bitmap, ALLOCNO_NUM (a));
  cover_class = ALLOCNO_COVER_CLASS (a);
  curr_reg_pressure [cover_class]
    += reg_class_nregs [cover_class] [ALLOCNO_MODE (a)];
  if (curr_bb_node->reg_pressure [cover_class]
      < curr_reg_pressure [cover_class])
    curr_bb_node->reg_pressure [cover_class]
      = curr_reg_pressure [cover_class];
}

/* The function marks allocno A as currently not living.  */
static void
clear_allocno_live (allocno_t a)
{
  enum reg_class cover_class;

  if (bitmap_bit_p (allocnos_live_bitmap, ALLOCNO_NUM (a)))
    {
      cover_class = ALLOCNO_COVER_CLASS (a);
      curr_reg_pressure [cover_class]
	-= reg_class_nregs [cover_class] [ALLOCNO_MODE (a)];
      ira_assert (curr_reg_pressure [cover_class] >= 0);
    }
  CLEAR_ALLOCNO_LIVE (ALLOCNO_NUM (a));
  bitmap_clear_bit (allocnos_live_bitmap, ALLOCNO_NUM (a));
}

/* Record all regs that are set in any one insn.  Communication from
   mark_reg_{store,clobber} and build_conflict_bit_table.  */
static VEC(rtx, heap) *regs_set;

/* Record a conflict between hard register REGNO or allocno
   corresponding to pseudo-register REGNO and everything currently
   live.  */
static void
record_regno_conflict (int regno)
{
  int j;

  if (regno < FIRST_PSEUDO_REGISTER)
    {
      /* When a hard register becomes live, record conflicts with live
	 allocno regs.  */
      EXECUTE_IF_SET_IN_ALLOCNO_SET (allocnos_live, j,
        {
	  SET_HARD_REG_BIT (ALLOCNO_CONFLICT_HARD_REGS (allocnos [j]), regno);
	});
    }
  else
    {
      /* When a pseudo-register becomes live, record conflicts first
	 with hard regs, then with other allocnos.  */
      allocno_t a = curr_regno_allocno_map [regno];
      int pn, pn_prod;

      ira_assert (a != NULL || REG_N_REFS (regno) == 0);
      if (a == NULL)
	return;
      pn = ALLOCNO_NUM (a);
      pn_prod = pn * allocno_row_words;
      IOR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a), hard_regs_live);
      for (j = allocno_row_words - 1; j >= 0; j--)
	conflicts [pn_prod + j] |= allocnos_live [j];
      /* Don't set up conflict for the allocno with itself.  */
      CLEAR_ALLOCNO_CONFLICT_ROW (conflicts + pn_prod, pn);
    }
}

/* Handle the case where REG is set by the insn being scanned, during
   the scan to accumulate conflicts.  Store a 1 in hard_regs_live or
   allocnos_live for this register or the corresponding allocno, record
   how many consecutive hardware registers it actually needs, and
   record a conflict with all other allocnos already live.

   Note that even if REG does not remain alive after this insn, we
   must mark it here as live, to ensure a conflict between REG and any
   other allocnos set in this insn that really do live.  This is
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
      allocno_t a = curr_regno_allocno_map [regno];

      ira_assert (a != NULL || REG_N_REFS (regno) == 0);
      if (a != NULL)
	set_allocno_live (a);
      record_regno_conflict (regno);
    }
  else if (! TEST_HARD_REG_BIT (no_alloc_regs, regno))
    {
      int last = regno + hard_regno_nregs [regno] [GET_MODE (reg)];
      enum reg_class cover_class;

      while (regno < last)
	{
	  record_regno_conflict (regno);
	  if (! TEST_HARD_REG_BIT (eliminable_regset, regno)
	      && ! TEST_HARD_REG_BIT (hard_regs_live, regno))
	    {
	      cover_class = class_translate [REGNO_REG_CLASS (regno)];
	      curr_reg_pressure [cover_class]++;
	      SET_HARD_REG_BIT (hard_regs_live, regno);
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

/* Mark REG (or the corresponding allocno) as being dead (following the
   insn being scanned now).  Store a 0 in hard_regs_live or
   allocnos_live for this register.  */
static void
mark_reg_death (rtx reg)
{
  int regno = REGNO (reg);

  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      allocno_t a = curr_regno_allocno_map [regno];

      ira_assert (a != NULL || REG_N_REFS (regno) == 0);
      if (a != NULL)
	clear_allocno_live (a);
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
	      ira_assert (curr_reg_pressure [cover_class] >= 0);
	      CLEAR_HARD_REG_BIT (hard_regs_live, regno);
	    }
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

/* The function attaches copy CP to allocnos involved into the copy.  */
static void
add_allocno_copy_to_list (copy_t cp)
{
  allocno_t first = cp->first, second = cp->second;

  cp->prev_first_allocno_copy = NULL;
  cp->prev_second_allocno_copy = NULL;
  cp->next_first_allocno_copy = ALLOCNO_COPIES (first);
  if (cp->next_first_allocno_copy != NULL)
    {
      if (cp->next_first_allocno_copy->first == first)
	cp->next_first_allocno_copy->prev_first_allocno_copy = cp;
      else
	cp->next_first_allocno_copy->prev_second_allocno_copy = cp;
    }
  cp->next_second_allocno_copy = ALLOCNO_COPIES (second);
  if (cp->next_second_allocno_copy != NULL)
    {
      if (cp->next_second_allocno_copy->second == second)
	cp->next_second_allocno_copy->prev_second_allocno_copy = cp;
      else
	cp->next_second_allocno_copy->prev_first_allocno_copy = cp;
    }
  ALLOCNO_COPIES (first) = cp;
  ALLOCNO_COPIES (second) = cp;
}

/* The function detaches copy CP from allocnos involved into the copy.  */
static void
remove_allocno_copy_from_list (copy_t cp)
{
  allocno_t first = cp->first, second = cp->second;
  copy_t prev, next;

  next = cp->next_first_allocno_copy;
  prev = cp->prev_first_allocno_copy;
  if (prev == NULL)
    ALLOCNO_COPIES (first) = next;
  else if (prev->first == first)
    prev->next_first_allocno_copy = next;
  else
    prev->next_second_allocno_copy = next;
  if (next != NULL)
    {
      if (next->first == first)
	next->prev_first_allocno_copy = prev;
      else
	next->prev_second_allocno_copy = prev;
    }
  cp->prev_first_allocno_copy = cp->next_first_allocno_copy = NULL;

  next = cp->next_second_allocno_copy;
  prev = cp->prev_second_allocno_copy;
  if (prev == NULL)
    ALLOCNO_COPIES (second) = next;
  else if (prev->second == second)
    prev->next_second_allocno_copy = next;
  else
    prev->next_first_allocno_copy = next;
  if (next != NULL)
    {
      if (next->second == second)
	next->prev_second_allocno_copy = prev;
      else
	next->prev_first_allocno_copy = prev;
    }
  cp->prev_second_allocno_copy = cp->next_second_allocno_copy = NULL;
}

/* The function makes copy CP a canonical copy where number of the
   first allocno is less than the second one.  */
static void
swap_allocno_copy_ends_if_necessary (copy_t cp)
{
  allocno_t temp;
  copy_t temp_cp;

  if (ALLOCNO_NUM (cp->first) <= ALLOCNO_NUM (cp->second))
    return;

  temp = cp->first;
  cp->first = cp->second;
  cp->second = temp;

  temp_cp = cp->prev_first_allocno_copy;
  cp->prev_first_allocno_copy = cp->prev_second_allocno_copy;
  cp->prev_second_allocno_copy = temp_cp;

  temp_cp = cp->next_first_allocno_copy;
  cp->next_first_allocno_copy = cp->next_second_allocno_copy;
  cp->next_second_allocno_copy = temp_cp;
}

/* The function creates and returns new copy of allocnos FIRST and
   SECOND with frequency FREQ corresponding to move insn INSN (if
   any).  */
copy_t
add_allocno_copy (allocno_t first, allocno_t second, int freq, rtx insn)
{
  copy_t cp;

  cp = create_copy (first, second, freq, insn);
  ira_assert (first != NULL && second != NULL);
  add_allocno_copy_to_list (cp);
  swap_allocno_copy_ends_if_necessary (cp);
  return cp;
}

/* The function processes INSN and create allocno copies if
   necessary.  For example, it might be because INSN is a
   pseudo-register move or INSN is two operand insn.  */
static void
add_allocno_copies (rtx insn)
{
  rtx set, operand, dup;
  const char *str;
  int commut_p, bound_p;
  int i, j, freq, hard_regno, cost, index;
  copy_t cp;
  allocno_t a;
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
	      a = curr_regno_allocno_map [REGNO (SET_SRC (set))];
	    }
	  else
	    {
	      hard_regno = REGNO (SET_SRC (set));
	      a = curr_regno_allocno_map [REGNO (SET_DEST (set))];
	    }
	  class = REGNO_REG_CLASS (hard_regno);
	  mode = ALLOCNO_MODE (a);
	  cover_class = ALLOCNO_COVER_CLASS (a);
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
	  ALLOCNO_HARD_REG_COSTS (a) [index] -= cost;
	  ALLOCNO_CONFLICT_HARD_REG_COSTS (a) [index] -= cost;
	}
      else
	{
	  cp = add_allocno_copy (curr_regno_allocno_map [REGNO (SET_DEST (set))],
				 curr_regno_allocno_map [REGNO (SET_SRC (set))],
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
	      bound_p = FALSE;
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
			    a = curr_regno_allocno_map [REGNO (dup)];
			  }
			else
			  {
			    hard_regno = REGNO (dup);
			    a = curr_regno_allocno_map [REGNO (operand)];
			  }
			class = REGNO_REG_CLASS (hard_regno);
			mode = ALLOCNO_MODE (a);
			cover_class = ALLOCNO_COVER_CLASS (a);
			if (! class_subset_p [class] [cover_class])
			  continue;
			index
			  = class_hard_reg_index [cover_class] [hard_regno];
			if (index < 0)
			  continue;
			if (HARD_REGISTER_P (operand))
			  cost
			    = register_move_cost [mode] [cover_class] [class];
			else
			  cost
			    = register_move_cost [mode] [class] [cover_class];
			cost *= freq;
			ALLOCNO_HARD_REG_COSTS (a) [index] -= cost;
			ALLOCNO_CONFLICT_HARD_REG_COSTS (a) [index] -= cost;
			bound_p = TRUE;
		      }
		    else
		      {
			bound_p = TRUE;
			cp = add_allocno_copy
			     (curr_regno_allocno_map [REGNO (dup)],
			      curr_regno_allocno_map [REGNO (operand)],
			      freq, NULL_RTX);
			bitmap_set_bit
			  (ira_curr_loop_tree_node->local_copies, cp->num);
		      }
		  }
	      if (bound_p)
		continue;
	      /* If an operand dies, prefer its hard register for the
		 output operands by decreasing the hard register cost
		 or creating the corresponding allocno copies.  */
	      for (j = 0; j < recog_data.n_operands; j++)
		{
		  dup = recog_data.operand [j];

		  if (i == j || recog_data.operand_type [j] != OP_OUT
		      || !REG_P (dup))
		    continue;
		  
		  if (HARD_REGISTER_NUM_P (REGNO (operand))
		      || HARD_REGISTER_NUM_P (REGNO (dup)))
		    {
			if (HARD_REGISTER_P (operand))
			  {
			    if (HARD_REGISTER_P (dup))
			      continue;
			    hard_regno = REGNO (operand);
			    a = curr_regno_allocno_map [REGNO (dup)];
			  }
			else
			  {
			    hard_regno = REGNO (dup);
			    a = curr_regno_allocno_map [REGNO (operand)];
			  }
			class = REGNO_REG_CLASS (hard_regno);
			mode = ALLOCNO_MODE (a);
			cover_class = ALLOCNO_COVER_CLASS (a);
			if (! class_subset_p [class] [cover_class])
			  continue;
			index
			  = class_hard_reg_index [cover_class] [hard_regno];
			if (index < 0)
			  continue;
			if (HARD_REGISTER_P (operand))
			  cost
			    = register_move_cost [mode] [cover_class] [class];
			else
			  cost
			    = register_move_cost [mode] [class] [cover_class];
			cost *= (freq < 8 ? 1 : freq / 8);
			ALLOCNO_HARD_REG_COSTS (a) [index] -= cost;
			ALLOCNO_CONFLICT_HARD_REG_COSTS (a) [index] -= cost;
		      }
		    else
		      {
			cp = add_allocno_copy
			     (curr_regno_allocno_map [REGNO (dup)],
			      curr_regno_allocno_map [REGNO (operand)],
			      (freq < 8 ? 1 : freq / 8), NULL_RTX);
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
   allocno which can use only one hard register and makes other
   currently living allocnos conflicting with the hard register.  */
static void
process_single_reg_class_operands (int in_p)
{
  int i, regno, px;
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

	  operand_a = curr_regno_allocno_map [regno];
	  mode = ALLOCNO_MODE (operand_a);
	  cover_class = ALLOCNO_MODE (operand_a);
	  if (class_subset_p [cl] [cover_class]
	      && (reg_class_size [cl]
		  <= (unsigned) CLASS_MAX_NREGS (cl, mode)))
	    ALLOCNO_CONFLICT_HARD_REG_COSTS (operand_a)
	      [class_hard_reg_index [cover_class] [class_hard_regs [cl] [0]]]
	      -= (in_p
		  ? register_move_cost [mode] [cover_class] [cl]
		  : register_move_cost [mode] [cl] [cover_class]);
	}

      EXECUTE_IF_SET_IN_ALLOCNO_SET (allocnos_live, px,
        {
	  a = allocnos [px];
	  cover_class = ALLOCNO_COVER_CLASS (a);
	  if (a != operand_a)
	    /* We could increase costs of A instead of making it
	       conflicting with the hard register.  But it works worse
	       because it will be spilled in reload in anyway.  */
	    IOR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a),
			      reg_class_contents [cl]);
	});
    }
}

/* The function processes insns of the basic block given by its
   LOOP_TREE_NODE to update allocno conflict table.  */
static void
process_bb_node_for_conflicts (struct ira_loop_tree_node *loop_tree_node)
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
	curr_reg_pressure [reg_class_cover [i]] = 0;
      curr_bb_node = loop_tree_node;
      curr_regno_allocno_map = ira_curr_loop_tree_node->regno_allocno_map;
      reg_live_in = DF_LR_IN (bb);
      memset (allocnos_live, 0, allocno_row_words * sizeof (INT_TYPE));
      REG_SET_TO_HARD_REG_SET (hard_regs_live, reg_live_in);
      AND_COMPL_HARD_REG_SET (hard_regs_live, eliminable_regset);
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
	  }
      bitmap_clear (allocnos_live_bitmap);
      EXECUTE_IF_SET_IN_BITMAP (reg_live_in, FIRST_PSEUDO_REGISTER, j, bi)
	{
	  allocno_t a = curr_regno_allocno_map [j];
	  
	  ira_assert (a != NULL || REG_N_REFS (j) == 0);
	  if (a == NULL)
	    continue;
	  set_allocno_live (a);
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
      
      /* Allocnos can't go in stack regs at the start of a basic block
	 that is reached by an abnormal edge. Likewise for call
	 clobbered regs, because caller-save, fixup_abnormal_edges and
	 possibly the table driven EH machinery are not quite ready to
	 handle such allocnos live across such edges.  */
      FOR_EACH_EDGE (e, ei, bb->preds)
	if (e->flags & EDGE_ABNORMAL)
	  break;
      
      if (e != NULL)
	{
#ifdef STACK_REGS
	  EXECUTE_IF_SET_IN_ALLOCNO_SET (allocnos_live, px,
	    {
	      ALLOCNO_NO_STACK_REG_P (allocnos [px]) = TRUE;
	    });
	  for (px = FIRST_STACK_REG; px <= LAST_STACK_REG; px++)
	    record_regno_conflict (px);
#endif
	  /* No need to record conflicts for call clobbered regs if we
	     have nonlocal labels around, as we don't ever try to
	     allocate such regs in this case.  */
	  if (! current_function_has_nonlocal_label)
	    for (px = 0; px < FIRST_PSEUDO_REGISTER; px++)
	      if (call_used_regs [px])
		record_regno_conflict (px);
	}
  
      /* Scan the code of this basic block, noting which allocnos and
	 hard regs are born or die.  When one is born, record a
	 conflict with all others currently live.  */
      FOR_BB_INSNS (bb, insn)
	{
	  rtx link;
	  
	  if (! INSN_P (insn))
	    continue;
	  
	  /* Check regs_set is an empty set.  */
	  gcc_assert (VEC_empty (rtx, regs_set));
      
	  /* Mark any allocnos clobbered by INSN as live, so they
	     conflict with the inputs.  */
	  note_stores (PATTERN (insn), mark_reg_clobber, NULL);
	  
	  extract_insn (insn);
	  process_single_reg_class_operands (TRUE);
	  
	  /* Mark any allocnos dead after INSN as dead now.  */
	  for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
	    if (REG_NOTE_KIND (link) == REG_DEAD)
	      mark_reg_death (XEXP (link, 0));
	  
	  if (CALL_P (insn))
	    {
	      HARD_REG_SET clobbered_regs;
	      
	      get_call_invalidated_used_regs (insn, &clobbered_regs, FALSE);
	      IOR_HARD_REG_SET (cfun->emit->call_used_regs, clobbered_regs);
	      EXECUTE_IF_SET_IN_ALLOCNO_SET (allocnos_live, i,
	        {
		  int freq;
		  allocno_t a = allocnos [i];
		  
		  freq = REG_FREQ_FROM_BB (BLOCK_FOR_INSN (insn));
		  if (freq == 0)
		    freq = 1;
		  ALLOCNO_CALL_FREQ (a) += freq;
		  index = add_regno_call (ALLOCNO_REGNO (a), insn);
		  if (ALLOCNO_CALLS_CROSSED_START (a) < 0)
		    ALLOCNO_CALLS_CROSSED_START (a) = index;
		  ALLOCNO_CALLS_CROSSED_NUM (a)++;
		  /* Don't allocate allocnos that cross calls, if this
		     function receives a nonlocal goto.  */
		  if (current_function_has_nonlocal_label)
		    SET_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a));
		});
	    }
	  
	  /* Mark any allocnos set in INSN as live, and mark them as
	     conflicting with all other live allocnos.  Clobbers are
	     processed again, so they conflict with the allocnos that
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
	  
	  process_single_reg_class_operands (FALSE);
	  
	  /* Mark any allocnos set in INSN and then never used.  */
	  while (! VEC_empty (rtx, regs_set))
	    {
	      rtx reg = VEC_pop (rtx, regs_set);
	      rtx note = find_regno_note (insn, REG_UNUSED, REGNO (reg));

	      if (note)
		mark_reg_death (XEXP (note, 0));
	    }
	  add_allocno_copies (insn);
	}
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

/* The function builds allocno conflict table by traversing all basic
   blocks and their insns.  */
static void
build_conflict_bit_table (void)
{
  allocno_row_words = (allocnos_num + INT_BITS - 1) / INT_BITS;
  conflicts = ira_allocate (sizeof (INT_TYPE)
			    * allocnos_num * allocno_row_words);
  memset (conflicts, 0, sizeof (INT_TYPE) * allocnos_num * allocno_row_words);
  allocnos_live = ira_allocate (sizeof (INT_TYPE) * allocno_row_words);
  allocnos_live_bitmap = ira_allocate_bitmap ();
  /* Make a vector that mark_reg_{store,clobber} will store in.  */
  if (!regs_set)
    regs_set = VEC_alloc (rtx, heap, 10);
  traverse_loop_tree (ira_loop_tree_root, NULL, process_bb_node_for_conflicts);
  /* Clean up.  */
  ira_free_bitmap (allocnos_live_bitmap);
  ira_free (allocnos_live);
}

/* The function propagates info about allocno A to the corresponding
   allocno on upper loop tree level.  So allocnos on upper levels
   accumulate information about the corresponding allocnos in nested
   loops.  */
static void
propagate_allocno_info (allocno_t a)
{
  int regno, j, pn, father_pn, another_father_pn;
  allocno_t father_a, another_a, another_father_a;
  struct ira_loop_tree_node *father;
  copy_t cp, next_cp;

  regno = ALLOCNO_REGNO (a);
  if ((father = ALLOCNO_LOOP_TREE_NODE (a)->father) != NULL
      && (father_a = father->regno_allocno_map [regno]) != NULL)
    {
      ALLOCNO_CALL_FREQ (father_a) += ALLOCNO_CALL_FREQ (a);
#ifdef STACK_REGS
      if (ALLOCNO_NO_STACK_REG_P (a))
	ALLOCNO_NO_STACK_REG_P (father_a) = TRUE;
#endif
      IOR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (father_a),
			ALLOCNO_CONFLICT_HARD_REGS (a));
      pn = ALLOCNO_NUM (a);
      EXECUTE_IF_SET_IN_ALLOCNO_SET (conflicts + pn * allocno_row_words, j,
        {
	  another_a = allocnos [j];
	  if ((another_father_a = (father->regno_allocno_map
				   [ALLOCNO_REGNO (another_a)])) == NULL)
	    continue;
	  father_pn = ALLOCNO_NUM (father_a);
	  another_father_pn = ALLOCNO_NUM (another_father_a);
	  SET_ALLOCNO_CONFLICT_ROW
	    (conflicts + father_pn * allocno_row_words, another_father_pn);
	  SET_ALLOCNO_CONFLICT_ROW
	    (conflicts + another_father_pn * allocno_row_words, father_pn);
	});
      if (ALLOCNO_CALLS_CROSSED_START (father_a) < 0)
	ALLOCNO_CALLS_CROSSED_START (father_a)
	  = ALLOCNO_CALLS_CROSSED_START (a);
      ALLOCNO_CALLS_CROSSED_NUM (father_a) += ALLOCNO_CALLS_CROSSED_NUM (a);
      for (cp = ALLOCNO_COPIES (a); cp != NULL; cp = next_cp)
	{
	  if (cp->first == a)
	    {
	      next_cp = cp->next_first_allocno_copy;
	      another_a = cp->second;
	    }
	  else if (cp->second == a)
	    {
	      next_cp = cp->next_second_allocno_copy;
	      another_a = cp->first;
	    }
	  else
	    gcc_unreachable ();
	  if ((another_father_a = (father->regno_allocno_map
				   [ALLOCNO_REGNO (another_a)])) != NULL)
	    add_allocno_copy
	      (father_a, another_father_a, cp->freq, cp->move_insn);
	}
    }
}

/* The function propagates info about allocnos to the corresponding
   allocnos on upper loop tree level.  */
static void
propagate_info (void)
{
  int i;
  allocno_t a;

  for (i = max_reg_num () - 1; i >= FIRST_PSEUDO_REGISTER; i--)
    for (a = regno_allocno_map [i];
	 a != NULL;
	 a = ALLOCNO_NEXT_REGNO_ALLOCNO (a))
      propagate_allocno_info (a);
}

/* If CONFLICTP (i, j) is TRUE, make sure CONFLICTP (j, i) is also TRUE.  */
static void
mirror_conflicts (void)
{
  int i, j;
  unsigned INT_TYPE mask;
  int rw = allocno_row_words;
  int rwb = rw * INT_BITS;
  INT_TYPE *p = conflicts;
  INT_TYPE *q0 = conflicts;
  INT_TYPE *q1, *q2;

  for (i = allocnos_num - 1, mask = 1; i >= 0; i--, mask <<= 1)
    {
      if (! mask)
	{
	  mask = 1;
	  q0++;
	}
      for (j = allocno_row_words - 1, q1 = q0; j >= 0; j--, q1 += rwb)
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
allocno_reg_conflict_p (int regno1, int regno2)
{
  int p_no1, p_no2;

  ira_assert (regno1 >= FIRST_PSEUDO_REGISTER
	      && regno2 >= FIRST_PSEUDO_REGISTER);
  /* Reg info caclulated by dataflow infrastructure can be different
     from one calculated by regclass.  */
  if (curr_regno_allocno_map [regno1] == NULL
      || curr_regno_allocno_map [regno2] == NULL)
    return FALSE;
  p_no1 = ALLOCNO_NUM (curr_regno_allocno_map [regno1]);
  p_no2 = ALLOCNO_NUM (curr_regno_allocno_map [regno2]);
  ira_assert (p_no1 >= 0 && p_no1 < allocnos_num
	      && p_no2 >= 0 && p_no2 < allocnos_num);
  return CONFLICTP (p_no1, p_no2) != 0;
}

/* Remove copies involving conflicting allocnos.  */
static void
remove_conflict_allocno_copies (void)
{
  int i;
  allocno_t a;
  copy_t cp, next_cp;
  varray_type conflict_allocno_copy_varray;

  VARRAY_GENERIC_PTR_NOGC_INIT (conflict_allocno_copy_varray, get_max_uid (),
				"copies of conflicting allocnos");
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      for (cp = ALLOCNO_COPIES (a); cp != NULL; cp = next_cp)
	if (cp->first == a)
	  next_cp = cp->next_first_allocno_copy;
	else
	  {
	    next_cp = cp->next_second_allocno_copy;
	    VARRAY_PUSH_GENERIC_PTR (conflict_allocno_copy_varray, cp);
	  }
    }
  for (i = VARRAY_ACTIVE_SIZE (conflict_allocno_copy_varray) - 1; i >= 0; i--)
    {
      cp = VARRAY_GENERIC_PTR (conflict_allocno_copy_varray, i);
      if (CONFLICTP (ALLOCNO_NUM (cp->first), ALLOCNO_NUM (cp->second)))
	remove_allocno_copy_from_list (cp);
    }
  VARRAY_FREE (conflict_allocno_copy_varray);
}

/* The function builds conflict vectors of all allocnos from the
   conflict table.  */
static void
build_allocno_conflict_vects (void)
{
  int i, j, px;
  allocno_t a, *conflict_allocnos, *vec;

  conflict_allocnos = ira_allocate (sizeof (allocno_t) * allocnos_num);
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      ira_assert (i == ALLOCNO_NUM (a));
      px = 0;
      EXECUTE_IF_SET_IN_ALLOCNO_SET (conflicts + i * allocno_row_words, j,
				    {
				      conflict_allocnos [px++] = allocnos [j];
				    });
      allocate_allocno_conflicts (a, px);
      vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (a);
      memcpy (vec, conflict_allocnos, sizeof (allocno_t) * px);
      vec [px] = NULL;
      ALLOCNO_CONFLICT_ALLOCNO_VEC_ACTIVE_SIZE (a) = px;
    }
  ira_free (conflict_allocnos);
}



/* The function propagates information about allocnos modified inside
   the loops.  */
static void
propagate_modified_regnos (struct ira_loop_tree_node *loop_tree_node)
{
  if (loop_tree_node->bb != NULL || loop_tree_node == ira_loop_tree_root)
    return;
  bitmap_ior_into (loop_tree_node->father->modified_regnos,
		   loop_tree_node->modified_regnos);
}



/* The function outputs information about allocno conflicts to FILE.  */
static void
print_conflicts (FILE *file)
{
  int i;

  for (i = 0; i < allocnos_num; i++)
    {
      int j;
      allocno_t a;
      basic_block bb;

      a = allocnos [i];
      fprintf (file, ";; a%d(r%d,", ALLOCNO_NUM (a), ALLOCNO_REGNO (a));
      if ((bb = ALLOCNO_LOOP_TREE_NODE (a)->bb) != NULL)
	fprintf (file, "b%d", bb->index);
      else
	fprintf (file, "l%d", ALLOCNO_LOOP_TREE_NODE (a)->loop->num);
      fprintf (file, ") conflicts:");
      for (j = 0; j < allocnos_num; j++)
	if (CONFLICTP (j, i))
	  {
	    fprintf (file, " a%d(r%d,",
		     ALLOCNO_NUM (allocnos [j]), ALLOCNO_REGNO (allocnos [j]));
	    if ((bb = ALLOCNO_LOOP_TREE_NODE (allocnos [j])->bb) != NULL)
	      fprintf (file, "b%d)", bb->index);
	    else
	      fprintf (file, "l%d)",
		       ALLOCNO_LOOP_TREE_NODE (allocnos [j])->loop->num);
	  }
      fprintf (file, "\n;;     conflict hard regs:");
      for (j = 0; j < FIRST_PSEUDO_REGISTER; j++)
	if (TEST_HARD_REG_BIT (ALLOCNO_CONFLICT_HARD_REGS (a), j))
	  fprintf (file, " %d", j);
      fprintf (file, "\n");

    }
  fprintf (file, "\n");
}

/* The function outputs information about allocno conflicts to
   stderr.  */
void
debug_conflicts (void)
{
  print_conflicts (stderr);
}



/* Entry function which builds allocno conflicts.  */
void
ira_build_conflicts (void)
{
  int i;
  allocno_t a;

  build_conflict_bit_table ();
  mirror_conflicts ();
  if (flag_ira_algorithm == IRA_ALGORITHM_REGIONAL
      || flag_ira_algorithm == IRA_ALGORITHM_MIXED)
    propagate_info ();
  /* We need finished conflict table for the subsequent call.  */
  remove_conflict_allocno_copies ();
  build_allocno_conflict_vects ();
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      if (ALLOCNO_CALLS_CROSSED_NUM (a) != 0)
	{
	  if (! flag_caller_saves
	      || (flag_ira_split_around_calls
		  && ((ira_max_regno_before > ALLOCNO_REGNO (a)
		       && (reg_equiv_const [ALLOCNO_REGNO (a)]
			   || reg_equiv_invariant_p [ALLOCNO_REGNO (a)]))
		      || (ira_max_regno_before <= ALLOCNO_REGNO (a)
			  && ALLOCNO_REGNO (a) < ira_max_regno_call_before))))
	    IOR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a),
			      call_used_reg_set);
          else
	    IOR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a),
			      no_caller_save_reg_set);
	}
    }  
  traverse_loop_tree (ira_loop_tree_root, NULL, propagate_modified_regnos);
  if (ira_dump_file != NULL)
    print_conflicts (ira_dump_file);
}
