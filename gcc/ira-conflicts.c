/* IRA conflict builder.
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

static void build_conflict_bit_table (void);
static void mirror_conflicts (void);
static int commutative_constraint_p (const char *);
static int get_dup_num (int, int);
static rtx get_dup (int, int);
static void add_insn_allocno_copies (rtx);
static void add_copies (loop_tree_node_t);
static void propagate_allocno_info (allocno_t);
static void propagate_info (void);
static void remove_conflict_allocno_copies (void);
static void build_allocno_conflict_vects (void);
static void propagate_modified_regnos (loop_tree_node_t);
static void print_hard_reg_set (FILE *, const char *, HARD_REG_SET);
static void print_conflicts (FILE *, int);

/* Set, clear or test bit number I in `allocnos_live',
   a bit vector indexed by allocno number.  */
#define SET_ALLOCNO_LIVE(I) SET_ALLOCNO_SET_BIT (allocnos_live, I)
#define CLEAR_ALLOCNO_LIVE(I) CLEAR_ALLOCNO_SET_BIT (allocnos_live, I)
#define TEST_ALLOCNO_LIVE(I) TEST_ALLOCNO_SET_BIT (allocnos_live, I)

/* allocnos_num by allocnos_num array of bits, recording whether two
   allocno's conflict (can't go in the same hardware register).

   `conflicts' is symmetric after the call to mirror_conflicts.  */
static INT_TYPE *conflicts;

/* Two macros to test 1 in an element of `conflicts'.  */
#define CONFLICT_P(I, J)						\
 (conflicts[(I) * allocno_set_words + (unsigned) (J) / INT_BITS]	\
  & ((INT_TYPE) 1 << ((unsigned) (J) % INT_BITS)))

/* Bit mask for allocnos live at current point in the scan.  */
static INT_TYPE *allocnos_live;



/* The function builds allocno conflict table by processing allocno
   live ranges.  */
static void
build_conflict_bit_table (void)
{
  int i, j, pn, pn_prod;
  allocno_live_range_t r;

  allocno_set_words = (allocnos_num + INT_BITS - 1) / INT_BITS;
  allocnos_live = ira_allocate (sizeof (INT_TYPE) * allocno_set_words);
  memset (allocnos_live, 0, sizeof (INT_TYPE) * allocno_set_words);
  conflicts = ira_allocate (sizeof (INT_TYPE)
			    * allocnos_num * allocno_set_words);
  memset (conflicts, 0, sizeof (INT_TYPE) * allocnos_num * allocno_set_words);
  for (i = 0; i < max_point; i++)
    {
      for (r = start_point_ranges [i]; r != NULL; r = r->start_next)
	{
	  pn = ALLOCNO_NUM (r->allocno);
	  SET_ALLOCNO_LIVE (pn);
	  pn_prod = pn * allocno_set_words;
	  for (j = allocno_set_words - 1; j >= 0; j--)
	    conflicts [pn_prod + j] |= allocnos_live [j];
	  /* Don't set up conflict for the allocno with itself.  */
	  CLEAR_ALLOCNO_SET_BIT (conflicts + pn_prod, pn);
	}
	  
      for (r = finish_point_ranges [i]; r != NULL; r = r->finish_next)
	CLEAR_ALLOCNO_LIVE (ALLOCNO_NUM (r->allocno));
    }
}

/* If CONFLICT_P (i, j) is TRUE, make sure CONFLICT_P (j, i) is also TRUE.  */
static void
mirror_conflicts (void)
{
  int i, j;
  unsigned INT_TYPE mask;
  int rw = allocno_set_words;
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
      for (j = allocno_set_words - 1, q1 = q0; j >= 0; j--, q1 += rwb)
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
  rtx op, equiv_const = NULL_RTX;

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

/* The function processes INSN and create allocno copies if
   necessary.  For example, it might be because INSN is a
   pseudo-register move or INSN is two operand insn.  */
static void
add_insn_allocno_copies (rtx insn)
{
  rtx set, operand, dup;
  const char *str;
  int commut_p, bound_p;
  int i, j, freq, hard_regno, cost, index, hard_regs_num;
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
	      a = ira_curr_regno_allocno_map [REGNO (SET_SRC (set))];
	    }
	  else
	    {
	      hard_regno = REGNO (SET_SRC (set));
	      a = ira_curr_regno_allocno_map [REGNO (SET_DEST (set))];
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
	  hard_regs_num = class_hard_regs_num [cover_class];
	  allocate_and_set_costs
	    (&ALLOCNO_HARD_REG_COSTS (a), hard_regs_num,
	     ALLOCNO_COVER_CLASS_COST (a));
	  allocate_and_set_costs
	    (&ALLOCNO_CONFLICT_HARD_REG_COSTS (a), hard_regs_num, 0);
	  ALLOCNO_HARD_REG_COSTS (a) [index] -= cost;
	  ALLOCNO_CONFLICT_HARD_REG_COSTS (a) [index] -= cost;
	}
      else
	{
	  cp = add_allocno_copy
	       (ira_curr_regno_allocno_map [REGNO (SET_DEST (set))],
		ira_curr_regno_allocno_map [REGNO (SET_SRC (set))],
		freq, insn, ira_curr_loop_tree_node);
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
			    a = ira_curr_regno_allocno_map [REGNO (dup)];
			  }
			else
			  {
			    hard_regno = REGNO (dup);
			    a = ira_curr_regno_allocno_map [REGNO (operand)];
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
			hard_regs_num = class_hard_regs_num [cover_class];
			allocate_and_set_costs
			  (&ALLOCNO_HARD_REG_COSTS (a), hard_regs_num,
			   ALLOCNO_COVER_CLASS_COST (a));
			allocate_and_set_costs
			  (&ALLOCNO_CONFLICT_HARD_REG_COSTS (a),
			   hard_regs_num, 0);
			ALLOCNO_HARD_REG_COSTS (a) [index] -= cost;
			ALLOCNO_CONFLICT_HARD_REG_COSTS (a) [index] -= cost;
			bound_p = TRUE;
		      }
		    else
		      {
			bound_p = TRUE;
			cp = add_allocno_copy
			     (ira_curr_regno_allocno_map [REGNO (dup)],
			      ira_curr_regno_allocno_map [REGNO (operand)],
			      freq, NULL_RTX, ira_curr_loop_tree_node);
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
			    a = ira_curr_regno_allocno_map [REGNO (dup)];
			  }
			else
			  {
			    hard_regno = REGNO (dup);
			    a = ira_curr_regno_allocno_map [REGNO (operand)];
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
			hard_regs_num = class_hard_regs_num [cover_class];
			allocate_and_set_costs
			  (&ALLOCNO_HARD_REG_COSTS (a), hard_regs_num,
			   ALLOCNO_COVER_CLASS_COST (a));
			allocate_and_set_costs
			  (&ALLOCNO_CONFLICT_HARD_REG_COSTS (a),
			   hard_regs_num, 0);
			ALLOCNO_HARD_REG_COSTS (a) [index] -= cost;
			ALLOCNO_CONFLICT_HARD_REG_COSTS (a) [index] -= cost;
		      }
		    else
		      {
			cp = add_allocno_copy
			     (ira_curr_regno_allocno_map [REGNO (dup)],
			      ira_curr_regno_allocno_map [REGNO (operand)],
			      (freq < 8 ? 1 : freq / 8), NULL_RTX,
			      ira_curr_loop_tree_node);
			bitmap_set_bit
			  (ira_curr_loop_tree_node->local_copies, cp->num);
		      }
		}
	    }
	}
    }
}

/* The function adds copies originated from bb given by
   LOOP_TREE_NODE.  */
static void
add_copies (loop_tree_node_t loop_tree_node)
{
  basic_block bb;
  rtx insn;

  bb = loop_tree_node->bb;
  if (bb == NULL)
    return;
  FOR_BB_INSNS (bb, insn)
    if (INSN_P (insn))
      add_insn_allocno_copies (insn);
}

/* The function propagates info about allocno A to the corresponding
   allocno on upper loop tree level.  So allocnos on upper levels
   accumulate information about the corresponding allocnos in nested
   loops.  */
static void
propagate_allocno_info (allocno_t a)
{
  int regno;
  allocno_t father_a, another_a, another_father_a;
  loop_tree_node_t father;
  copy_t cp, next_cp;

  regno = ALLOCNO_REGNO (a);
  if ((father = ALLOCNO_LOOP_TREE_NODE (a)->father) != NULL
      && (father_a = father->regno_allocno_map [regno]) != NULL)
    {
      ALLOCNO_CALL_FREQ (father_a) += ALLOCNO_CALL_FREQ (a);
#ifdef STACK_REGS
      if (ALLOCNO_TOTAL_NO_STACK_REG_P (a))
	ALLOCNO_TOTAL_NO_STACK_REG_P (father_a) = TRUE;
#endif
      IOR_HARD_REG_SET (ALLOCNO_TOTAL_CONFLICT_HARD_REGS (father_a),
			ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a));
      if (ALLOCNO_CALLS_CROSSED_START (father_a) < 0
	  || (ALLOCNO_CALLS_CROSSED_START (a) >= 0
	      && (ALLOCNO_CALLS_CROSSED_START (father_a)
		  > ALLOCNO_CALLS_CROSSED_START (a))))
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
	    add_allocno_copy (father_a, another_father_a, cp->freq,
			      cp->insn, cp->loop_tree_node);
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

/* The function returns TRUE if allocnos A1 and A2 conflict.  It
   checks intersection of the corresponding live ranges for this.  */
int
allocno_conflict_p (allocno_t a1, allocno_t a2)
{
  allocno_live_range_t r1, r2;

  if (a1 == a2)
    return FALSE;
  if (ALLOCNO_REG (a1) != NULL && ALLOCNO_REG (a2) != NULL
      && (ORIGINAL_REGNO (ALLOCNO_REG (a1))
	  == ORIGINAL_REGNO (ALLOCNO_REG (a2))))
    return FALSE;
  for (r1 = ALLOCNO_LIVE_RANGES (a1), r2 = ALLOCNO_LIVE_RANGES (a2);
       r1 != NULL && r2 != NULL;)
    {
      if (r1->start > r2->finish)
	r1 = r1->next;
      else if (r2->start > r1->finish)
	r2 = r2->next;
      else
	return TRUE;
    }
  return FALSE;
}

/* The function returns TRUE if pseudo-registers REGNO1 and REGNO2
   conflict.  It should be used when there is only one region.  */
int
allocno_reg_conflict_p (int regno1, int regno2)
{
  allocno_t a1, a2;

  ira_assert (regno1 >= FIRST_PSEUDO_REGISTER
	      && regno2 >= FIRST_PSEUDO_REGISTER);
  /* Reg info caclulated by dataflow infrastructure can be different
     from one calculated by regclass.  */
  if ((a1 = ira_loop_tree_root->regno_allocno_map [regno1]) == NULL
      || (a2 = ira_loop_tree_root->regno_allocno_map [regno2]) == NULL)
    return FALSE;
  return allocno_conflict_p (a1, a2);
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
      if (CONFLICT_P (ALLOCNO_NUM (cp->first), ALLOCNO_NUM (cp->second)))
	remove_allocno_copy_from_list (cp);
    }
  VARRAY_FREE (conflict_allocno_copy_varray);
}

/* The function builds conflict vectors of all allocnos from the
   conflict table.  */
static void
build_allocno_conflict_vects (void)
{
  int i, j, level, px, another_father_pn, conflict_allocnos_num;
  int *level_init_p;
  enum reg_class cover_class;
  loop_tree_node_t father;
  allocno_t a, father_a, another_a, another_father_a, *conflict_allocnos, *vec;
  INT_TYPE *accumulated_conflicts, *allocno_conflicts, *propagate_conflicts;

  conflict_allocnos = ira_allocate (sizeof (allocno_t) * allocnos_num);
  level_init_p = ira_allocate (ira_loop_tree_height * sizeof (int));
  memset (level_init_p, 0, ira_loop_tree_height * sizeof (int));
  accumulated_conflicts
    = ira_allocate (ira_loop_tree_height
		    * allocno_set_words * sizeof (INT_TYPE));
  for (i = max_reg_num () - 1; i >= FIRST_PSEUDO_REGISTER; i--)
    for (a = regno_allocno_map [i];
	 a != NULL;
	 a = ALLOCNO_NEXT_REGNO_ALLOCNO (a))
      {
	cover_class = ALLOCNO_COVER_CLASS (a);
	level = ALLOCNO_LOOP_TREE_NODE (a)->level;
	allocno_conflicts = conflicts + ALLOCNO_NUM (a) * allocno_set_words;
	px = 0;
	EXECUTE_IF_SET_IN_ALLOCNO_SET (allocno_conflicts, j,
	  {
	    another_a = allocnos [j];
	    if (cover_class == ALLOCNO_COVER_CLASS (another_a))
	      conflict_allocnos [px++] = another_a;
	  });
	conflict_allocnos_num = px;
	if (! level_init_p [level])
	  propagate_conflicts = allocno_conflicts;
	else
	  {
	    propagate_conflicts
	      = accumulated_conflicts + level * allocno_set_words;
	    EXECUTE_IF_SET_IN_ALLOCNO_SET (propagate_conflicts, j,
	      {
		another_a = allocnos [j];
		ira_assert (cover_class == ALLOCNO_COVER_CLASS (another_a));
		if (! TEST_ALLOCNO_SET_BIT (allocno_conflicts, j))
		  conflict_allocnos [px++] = another_a;
	      });
	    for (j = 0; j < allocno_set_words; j++)
	      propagate_conflicts [j] |= allocno_conflicts [j];
	  }
	allocate_allocno_conflicts (a, px);
	vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (a);
	memcpy (vec, conflict_allocnos, sizeof (allocno_t) * px);
	vec [px] = NULL;
	ALLOCNO_CONFLICT_ALLOCNOS_NUM (a) = conflict_allocnos_num;
	ALLOCNO_TOTAL_CONFLICT_ALLOCNOS_NUM (a) = px;
	level_init_p [level] = FALSE;
	if ((father = ALLOCNO_LOOP_TREE_NODE (a)->father) == NULL
	    || (father_a = father->regno_allocno_map [i]) == NULL)
	  continue;
	level--;
	ira_assert (level == ALLOCNO_LOOP_TREE_NODE (father_a)->level
		    && cover_class == ALLOCNO_COVER_CLASS (father_a));
	if (! level_init_p [level])
	  {
	    level_init_p [level] = TRUE;
	    memset (accumulated_conflicts + level * allocno_set_words, 0,
		    allocno_set_words * sizeof (INT_TYPE));
	  }
	EXECUTE_IF_SET_IN_ALLOCNO_SET (propagate_conflicts, j,
	  {
	    another_a = allocnos [j];
	    if ((another_father_a = (father->regno_allocno_map
				     [ALLOCNO_REGNO (another_a)])) == NULL)
	      continue;
	    another_father_pn = ALLOCNO_NUM (another_father_a);
	    if (another_father_pn < 0)
	      continue;
	    ira_assert (ALLOCNO_COVER_CLASS (another_a)
			== ALLOCNO_COVER_CLASS (another_father_a));
	    if (cover_class == ALLOCNO_COVER_CLASS (another_a))
	      SET_ALLOCNO_SET_BIT
		(accumulated_conflicts + allocno_set_words * level,
		 another_father_pn);
	  });
      }
  ira_free (accumulated_conflicts);
  ira_free (level_init_p);
  ira_free (conflict_allocnos);
}



/* The function propagates information about allocnos modified inside
   the loops.  */
static void
propagate_modified_regnos (loop_tree_node_t loop_tree_node)
{
  if (loop_tree_node->bb != NULL || loop_tree_node == ira_loop_tree_root)
    return;
  bitmap_ior_into (loop_tree_node->father->modified_regnos,
		   loop_tree_node->modified_regnos);
}



/* The function prints hard reg set SET with TITLE to FILE.  */
static void
print_hard_reg_set (FILE *file, const char *title, HARD_REG_SET set)
{
  int i, start;

  fprintf (file, title);
  for (start = -1, i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (TEST_HARD_REG_BIT (set, i))
	{
	  if (i == 0 || ! TEST_HARD_REG_BIT (set, i - 1))
	    start = i;
	}
      if (start >= 0
	  && (i == FIRST_PSEUDO_REGISTER - 1 || ! TEST_HARD_REG_BIT (set, i)))
	{
	  if (start == i - 1)
	    fprintf (file, " %d", start);
	  else if (start == i - 2)
	    fprintf (file, " %d %d", start, start + 1);
	  else
	    fprintf (file, " %d-%d", start, i - 1);
	  start = -1;
	}
    }
  fprintf (file, "\n");
}

/* The function outputs information about allocno or regno (if REG_P)
   conflicts to FILE.  */
static void
print_conflicts (FILE *file, int reg_p)
{
  int i;

  for (i = 0; i < allocnos_num; i++)
    {
      int j;
      allocno_t a, conflict_a, *allocno_vec;
      basic_block bb;

      a = allocnos [i];
      if (reg_p)
	fprintf (file, ";; r%d", ALLOCNO_REGNO (a));
      else
	{
	  fprintf (file, ";; a%d(r%d,", ALLOCNO_NUM (a), ALLOCNO_REGNO (a));
	  if ((bb = ALLOCNO_LOOP_TREE_NODE (a)->bb) != NULL)
	    fprintf (file, "b%d", bb->index);
	  else
	    fprintf (file, "l%d", ALLOCNO_LOOP_TREE_NODE (a)->loop->num);
	  fprintf (file, ")");
	}
      fprintf (file, " conflicts:");
      allocno_vec = ALLOCNO_CONFLICT_ALLOCNO_VEC (a);
      if (allocno_vec != NULL)
	for (j = 0; (conflict_a = allocno_vec [j]) != NULL; j++)
	  {
	    if (reg_p)
	      fprintf (file, " r%d,", ALLOCNO_REGNO (conflict_a));
	    else
	      {
		fprintf (file, " a%d(r%d,", ALLOCNO_NUM (conflict_a),
			 ALLOCNO_REGNO (conflict_a));
		if ((bb = ALLOCNO_LOOP_TREE_NODE (conflict_a)->bb) != NULL)
		  fprintf (file, "b%d)", bb->index);
		else
		  fprintf (file, "l%d)",
			   ALLOCNO_LOOP_TREE_NODE (conflict_a)->loop->num);
	      }
	    if (j + 1 == ALLOCNO_CONFLICT_ALLOCNOS_NUM (a))
	      fprintf (file, "*");
	  }
      print_hard_reg_set (file, "\n;;     total conflict hard regs:",
			  ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a));
      print_hard_reg_set (file, ";;     conflict hard regs:",
			  ALLOCNO_CONFLICT_HARD_REGS (a));
    }
  fprintf (file, "\n");
}

/* The function outputs information about allocno or regno (if REG_P)
   conflicts to stderr.  */
void
debug_conflicts (int reg_p)
{
  print_conflicts (stderr, reg_p);
}



/* Entry function which builds allocno conflicts.  */
void
ira_build_conflicts (void)
{
  int i;
  allocno_t a;

  build_conflict_bit_table ();
  mirror_conflicts ();
  traverse_loop_tree (FALSE, ira_loop_tree_root, NULL, add_copies);
  if (flag_ira_algorithm == IRA_ALGORITHM_REGIONAL
      || flag_ira_algorithm == IRA_ALGORITHM_MIXED)
    propagate_info ();
  /* We need finished conflict table for the subsequent call.  */
  remove_conflict_allocno_copies ();
  build_allocno_conflict_vects ();
  for (i = 0; i < allocnos_num; i++)
    {
      a = allocnos [i];
      if (ALLOCNO_CALLS_CROSSED_NUM (a) == 0)
	continue;
      if (! flag_caller_saves)
	{
	  IOR_HARD_REG_SET (ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a),
			    call_used_reg_set);
	  if (ALLOCNO_CALLS_CROSSED_NUM (a) != 0)
	    IOR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a),
			      call_used_reg_set);
	}
      else
	{
	  IOR_HARD_REG_SET (ALLOCNO_TOTAL_CONFLICT_HARD_REGS (a),
			    no_caller_save_reg_set);
	  if (ALLOCNO_CALLS_CROSSED_NUM (a) != 0)
	    IOR_HARD_REG_SET (ALLOCNO_CONFLICT_HARD_REGS (a),
			      no_caller_save_reg_set);
	}
    }
  traverse_loop_tree (FALSE, ira_loop_tree_root, NULL,
		      propagate_modified_regnos);
  if (internal_flag_ira_verbose > 2 && ira_dump_file != NULL)
    print_conflicts (ira_dump_file, FALSE);
}
