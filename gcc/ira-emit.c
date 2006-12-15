/* Integrated Register Allocator.  Channging code and generating moves.
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
#include "obstack.h"
#include "bitmap.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "expr.h"
#include "recog.h"
#include "params.h"
#include "timevar.h"
#include "tree-pass.h"
#include "output.h"
#include "reload.h"
#include "errors.h"
#include "df.h"
#include "ira-int.h"

struct move;

static struct move *create_move (pseudo_t, pseudo_t);
static void free_move (struct move *);
static void free_move_list (struct move *);
static int eq_move_lists_p (struct move *, struct move *);
static void change_regs (rtx *);
static void add_to_edge_list (edge, struct move *, int);
static rtx create_new_reg (rtx);
static int subloop_tree_node_p (struct ira_loop_tree_node *,
				struct ira_loop_tree_node *);
static void set_pseudo_reg (pseudo_t, rtx);
static int not_modified_p (pseudo_t, pseudo_t);
static void generate_edge_moves (edge);
static void change_loop (struct ira_loop_tree_node *);
static int eq_edge_move_lists_p (VEC(edge,gc) *);
static int can_move_through_p (rtx *, struct move *, int);
static void unify_moves (basic_block, int);
static void traverse_moves (struct move *);
static struct move *modify_move_list (struct move *);
static rtx emit_move_list (struct move *, int);
static void emit_moves (void);

/* The structure represents pseudo shuffling.  */
struct move
{
  /* The shuffled pseudos.  */
  pseudo_t from, to;
  /* The next move in the sequence.  */
  struct move *next;
  /* Use for finding dependencies.  */
  int visited_p;
  /* The size of the following array. */
  int deps_num;
  /* Moves on which given move depends on.  Dependency can be cyclic.
     It means we need a temporary to generates the moves.  */
  struct move **deps;
};

/* Array of moves (indexed by BB index) which should be put at the
   start/end of the corresponding blocks.  */
static struct move **at_bb_start, **at_bb_end;

/* Max regno before renaming some pseudo-registers.  For example, the
   same pseudo-register can be renamed in loop if its allocation is
   different outside the loop.  */
static int max_regno_before_changing;

/* The function returns new move of pseudos TO and FROM.  */
static struct move *
create_move (pseudo_t to, pseudo_t from)
{
  struct move *move;

  move = ira_allocate (sizeof (struct move));
  move->deps = NULL;
  move->deps_num = 0;
  move->to = to;
  move->from = from;
  move->next = NULL;
  move->visited_p = FALSE;
  return move;
}

/* The function frees memory for MOVE and its dependencies.  */
static void
free_move (struct move *move)
{
  if (move->deps != NULL)
    ira_free (move->deps);
  ira_free (move);
}

/* The function frees memory for list of the moves given by its
   HEAD.  */
static void
free_move_list (struct move *head)
{
  struct move *next;
  
  for (; head != NULL; head = next)
    {
      next = head->next;
      free_move (head);
    }
}

/* The function returns nonzero if the the move list LIST1 and LIST2
   are equal (two moves are equal if they shuffles the same
   pseudos).  */
static int
eq_move_lists_p (struct move *list1, struct move *list2)
{
  for (; list1 != NULL && list2 != NULL;
       list1 = list1->next, list2 = list2->next)
    if (list1->from != list2->from || list1->to != list2->to)
      return FALSE;
  return list1 == list2;
}

/* This recursive function changes pseudo-registers in *LOC if it is
   necessary.  */
static void
change_regs (rtx *loc)
{
  int i, regno;
  const char *fmt;
  enum rtx_code code;

  if (*loc == NULL_RTX)
    return;
  code = GET_CODE (*loc);
  if (code == REG)
    {
      regno = REGNO (*loc);
      if (regno < FIRST_PSEUDO_REGISTER)
	return;
      if (regno >= max_regno_before_changing)
	/* It is a shared register which was changed already.  */
	return;
      /* ??? That is for reg_equal.  */
      if (ira_curr_loop_tree_node->regno_pseudo_map [regno] != NULL)
	*loc = PSEUDO_REG (ira_curr_loop_tree_node->regno_pseudo_map [regno]);
      return;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	change_regs (&XEXP (*loc, i));
      else if (fmt[i] == 'E')
	{
	  int j;

	  for (j = XVECLEN (*loc, i) - 1; j >= 0; j--)
	    change_regs (&XVECEXP (*loc, i, j));
	}
    }
}

/* The function attaches MOVE to the edge E.  The move is attached to
   the head of the list if HEAD_P is nonzero.  */
static void
add_to_edge_list (edge e, struct move *move, int head_p)
{
  struct move *last;

  if (head_p || e->aux == NULL)
    {
      move->next = e->aux;
      e->aux = move;
    }
  else
    {
      for (last = e->aux; last->next != NULL; last = last->next)
	;
      last->next = move;
      move->next = NULL;
    }
}

/* The function creates and returns new pseudo-register with the same
   attributes as ORIGINAL_REG.  */
static rtx
create_new_reg (rtx original_reg)
{
  rtx new_reg;

  new_reg = gen_reg_rtx (GET_MODE (original_reg));
  ORIGINAL_REGNO (new_reg) = ORIGINAL_REGNO (original_reg);
  REG_USERVAR_P (new_reg) = REG_USERVAR_P (original_reg);
  REG_POINTER (new_reg) = REG_POINTER (original_reg);
  REG_ATTRS (new_reg) = REG_ATTRS (original_reg);
  if (ira_dump_file)
    fprintf (ira_dump_file, "  Creating newreg=%i from oldreg=%i\n",
	     REGNO (new_reg), REGNO (original_reg));
  return new_reg;
}

/* The function returns non-zero if loop given by SUBNODE inside the
   loop given by NODE.  */
static int
subloop_tree_node_p (struct ira_loop_tree_node *subnode,
		     struct ira_loop_tree_node *node)
{
  for (; subnode != NULL; subnode = subnode->father)
    if (subnode == node)
      return TRUE;
  return FALSE;
}

/* The function sets up field `reg' to REG for pseudos which has the
   same regno as PSEUDO and which are inside the loop corresponding to
   PSEUDO. */
static void
set_pseudo_reg (pseudo_t pseudo, rtx reg)
{
  pseudo_t p;
  struct ira_loop_tree_node *node;

  node = PSEUDO_LOOP_TREE_NODE (pseudo);
  for (p = regno_pseudo_map [PSEUDO_REGNO (pseudo)];
       p != NULL;
       p = PSEUDO_NEXT_REGNO_PSEUDO (p))
    if (subloop_tree_node_p (PSEUDO_LOOP_TREE_NODE (p), node))
      PSEUDO_REG (p) = reg;
}

/* The following function returns nonzero if move insn of SRC_PSEUDO
   to DEST_PSEUDO does not change value of the destination.  */
static int
not_modified_p (pseudo_t src_pseudo, pseudo_t dest_pseudo)
{
  int regno, orig_regno;
  pseudo_t p;
  struct ira_loop_tree_node *node;

  orig_regno = PSEUDO_REGNO (src_pseudo);
  regno = REGNO (PSEUDO_REG (dest_pseudo));
  for (node = PSEUDO_LOOP_TREE_NODE (src_pseudo);
       node != NULL;
       node = node->father)
    if ((p = node->regno_pseudo_map [orig_regno]) == NULL)
      break;
    else if (REGNO (PSEUDO_REG (p)) == (unsigned) regno)
      return TRUE;
    else if (bitmap_bit_p (node->modified_regnos, orig_regno))
      return FALSE;
  return node != NULL;
}

/* The function generates and attaches moves to the edge E.  It looks
   at the final regnos of pseudos living on the edge with the same
   original regno to find what moves should be generated.  */
static void
generate_edge_moves (edge e)
{
  struct ira_loop_tree_node *src_loop_node, *dest_loop_node;
  unsigned int regno;
  bitmap_iterator bi;
  pseudo_t src_pseudo, dest_pseudo, *src_map, *dest_map;
  struct move *move;

  src_loop_node = IRA_BB_NODE (e->src)->father;
  dest_loop_node = IRA_BB_NODE (e->dest)->father;
  e->aux = NULL;
  if (src_loop_node == dest_loop_node)
    return;
  src_map = src_loop_node->regno_pseudo_map;
  dest_map = dest_loop_node->regno_pseudo_map;
  EXECUTE_IF_SET_IN_REG_SET (DF_UPWARD_LIVE_IN (build_df, e->dest),
			     FIRST_PSEUDO_REGISTER, regno, bi)
    if (bitmap_bit_p (DF_UPWARD_LIVE_OUT (build_df, e->src), regno))
      {
	src_pseudo = src_map [regno];
	dest_pseudo = dest_map [regno];
	if (REGNO (PSEUDO_REG (src_pseudo))
	    == REGNO (PSEUDO_REG (dest_pseudo)))
	  continue;
	/* Actually it is not a optimization we need this code because
	   the memory (remember about equivalent memory) might be ROM
	   (or placed in read only section).  */
 	if (PSEUDO_HARD_REGNO (dest_pseudo) < 0
	    && PSEUDO_HARD_REGNO (src_pseudo) >= 0
	    && not_modified_p (src_pseudo, dest_pseudo))
	  {
	    if (ira_dump_file != NULL)
	      fprintf (ira_dump_file, "Remove r%d:%d->%d\n", regno,
		       PSEUDO_NUM (src_pseudo), PSEUDO_NUM (dest_pseudo));
	    continue;
	  }
	move = create_move (dest_pseudo, src_pseudo);
	add_to_edge_list (e, move, TRUE);
    }
}

/* Bitmap of pseudos local for the current loop.  */
static bitmap local_pseudo_bitmap;

/* This bitmap is used to find that we need to generate and use a new
   pseudo-register when processing pseudos with the same original
   regno.  */
static bitmap used_regno_bitmap;

/* The following function changes (if necessary) pseudo-registers
   inside loop given by loop tree node NODE.  */
static void
change_loop (struct ira_loop_tree_node *node)
{
  bitmap_iterator bi;
  unsigned int i;
  int regno, used_p;
  pseudo_t pseudo, father_pseudo, *map;
  rtx insn, original_reg;
  
  if (node != ira_loop_tree_root)
    {
      
      if (node->bb != NULL)
	{
	  FOR_BB_INSNS (node->bb, insn)
	    if (INSN_P (insn))
	      change_regs (&insn);
	  return;
	}
      
      if (ira_dump_file != NULL)
	fprintf (ira_dump_file, "Changing RTL for loop %d (header bb%d)\n",
		 node->loop->num, node->loop->header->index);
      
      map = ira_curr_loop_tree_node->father->regno_pseudo_map;
      EXECUTE_IF_SET_IN_REG_SET (ira_curr_loop_tree_node->border_pseudos,
				 0, i, bi)
	{
	  pseudo = pseudos [i];
	  regno = PSEUDO_REGNO (pseudo);
	  father_pseudo = map [regno];
	  /* We generate the same register move because the reload can
	     put a pseudo into memory in this case we will have live
	     range splitting.  If it does not happen such the same
	     hard register moves will be removed.  The worst case when
	     the both pseudos are put into memory by the reload is
	     very rare.  */
	  if (father_pseudo != NULL
	      && (PSEUDO_HARD_REGNO (pseudo)
		  == PSEUDO_HARD_REGNO (father_pseudo))
	      && (PSEUDO_HARD_REGNO (pseudo) < 0
		  /* don't create copies because reload can spill a
		     pseudo set by copy although pseudo will not get
		     memory slot.  */
		  || reg_equiv_invariant_p [regno]
		  || reg_equiv_const [regno] != NULL_RTX))
	    continue;
	  original_reg = PSEUDO_REG (pseudo);
	  if (father_pseudo == NULL
	      || REGNO (PSEUDO_REG (father_pseudo)) == REGNO (original_reg))
	    set_pseudo_reg (pseudo, create_new_reg (original_reg));
	}
    }
  /* Rename locals: Local pseudos with same regno in different loops
     might get the different hard register.  So we need to change
     PSEUDO_REG.  */
  bitmap_and_compl (local_pseudo_bitmap,
		    ira_curr_loop_tree_node->mentioned_pseudos,
		    ira_curr_loop_tree_node->border_pseudos);
  EXECUTE_IF_SET_IN_REG_SET (local_pseudo_bitmap, 0, i, bi)
    {
      pseudo = pseudos [i];
      regno = PSEUDO_REGNO (pseudo);
      if (regno < 0)
	continue;
      used_p = bitmap_bit_p (used_regno_bitmap, regno);
      bitmap_set_bit (used_regno_bitmap, regno);
      if (! used_p)
	continue;
      set_pseudo_reg (pseudo, create_new_reg (PSEUDO_REG (pseudo)));
    }
}

/* The function returns nonzero if move lists on all edges in vector
   VEC are equal.  */
static int
eq_edge_move_lists_p (VEC(edge,gc) *vec)
{
  struct move *list;
  int i;

  list = EDGE_I (vec, 0)->aux;
  for (i = EDGE_COUNT (vec) - 1; i > 0; i--)
    if (! eq_move_lists_p (list, EDGE_I (vec, i)->aux))
      return FALSE;
  return TRUE;
}

/* Current regno pseudo map used to check moves in function
   `can_move_through_p'.  */
static pseudo_t *curr_jump_map;

/* This recursive function returns nonzero if list of moves LIST can
   be moved through setting (if OUTPUT_P is nonzero) or reading
   LOC.  */
static int
can_move_through_p (rtx *loc, struct move *list, int output_p)
{
  int i;
  const char *fmt;
  enum rtx_code code = GET_CODE (*loc);

  code = GET_CODE (*loc);
  if (code == REG)
    {
      int hard_regno, regno;
      HARD_REG_SET regs, move_regs;

      regno = ORIGINAL_REGNO (*loc);
      hard_regno = REGNO (*loc);
      if (hard_regno >= FIRST_PSEUDO_REGISTER)
	hard_regno = PSEUDO_HARD_REGNO (curr_jump_map [regno]);
      if (hard_regno < 0)
	CLEAR_HARD_REG_SET (regs);
      else
	COPY_HARD_REG_SET
	  (regs, reg_mode_hard_regset [hard_regno] [GET_MODE (*loc)]);
      for (;list != NULL; list = list->next)
	if (output_p
	    && regno == (int) ORIGINAL_REGNO (PSEUDO_REG (list->from)))
	  return FALSE;
	else
	  {
	    hard_regno = PSEUDO_HARD_REGNO (list->to);
	    if (hard_regno < 0)
	      CLEAR_HARD_REG_SET (move_regs);
	    else
	      COPY_HARD_REG_SET
		(move_regs,
		 reg_mode_hard_regset [hard_regno] [PSEUDO_MODE (list->to)]);
	    if (output_p)
	      {
		hard_regno = PSEUDO_HARD_REGNO (list->from);
		if (hard_regno >= 0)
		  IOR_HARD_REG_SET (move_regs,
				    reg_mode_hard_regset
				    [hard_regno] [PSEUDO_MODE (list->from)]);
	      }
	    AND_HARD_REG_SET (move_regs, regs);
	    GO_IF_HARD_REG_EQUAL (move_regs, zero_hard_reg_set, cont);
	    return FALSE;
	  cont:
	    ;
	  }
      return TRUE;
    }
  else if (code == SET)
    {
      if (! can_move_through_p (&SET_DEST (*loc), list, TRUE))
	return FALSE;
      if (! can_move_through_p (&SET_SRC (*loc), list, FALSE))
	return FALSE;
      return TRUE;
    }
  else if (code == CLOBBER)
    {
      if (! can_move_through_p (&XEXP (*loc, 0), list, TRUE))
	return FALSE;
      return TRUE;
    }
  else if (code == MEM)
    {
      if (! can_move_through_p (&XEXP (*loc, 0), list, FALSE))
	return FALSE;
      return TRUE;
    }
  else if (code == PRE_DEC || code == POST_DEC || code == PRE_INC || 
	   code == POST_INC || code == POST_MODIFY || code == PRE_MODIFY)
    {
      if (! can_move_through_p (&XEXP (*loc, 0), list, TRUE))
	return FALSE;
      if (! can_move_through_p (&XEXP (*loc, 0), list, FALSE))
	return FALSE;
      return TRUE;
    }

  fmt = GET_RTX_FORMAT (code);
  for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  if (! can_move_through_p (&XEXP (*loc, i), list, output_p))
	    return FALSE;
	}
      else if (fmt[i] == 'E')
	{
	  int j;

	  for (j = XVECLEN (*loc, i) - 1; j >= 0; j--)
	    {
	      if (! can_move_through_p (&XVECEXP (*loc, i, j), list, output_p))
		return FALSE;
	    }
	}
    }
  return TRUE;
}

/* The function looks at all enter edges (if START_P) or exit edges of
   basic block BB and puts move lists at the BB start or end if it is
   possible.  In other words, it decreases code duplication of
   shuffling pseudos.  */
static void
unify_moves (basic_block bb, int start_p)
{
  int i;
  edge e;
  struct move *list;
  VEC(edge,gc) *vec;

  vec = (start_p ? bb->preds : bb->succs);
  if (EDGE_COUNT (vec) == 0 || ! eq_edge_move_lists_p (vec))
    return;
  e = EDGE_I (vec, 0);
  list = e->aux;
  curr_jump_map = IRA_BB_NODE (bb)->father->regno_pseudo_map;
  if (! start_p
      && (control_flow_insn_p (BB_END (bb))
	  && ! can_move_through_p (&PATTERN (BB_END (bb)), list, FALSE)))
    return;
  e->aux = NULL;
  for (i = EDGE_COUNT (vec) - 1; i > 0; i--)
    {
      e = EDGE_I (vec, i);
      free_move_list (e->aux);
      e->aux = NULL;
    }
  if (start_p)
    at_bb_start [bb->index] = list;
  else
    at_bb_end [bb->index] = list;
}

/* Last move (in move sequence being processed) setting up the
   corresponding hard register.  */
static struct move *hard_regno_last_set [FIRST_PSEUDO_REGISTER];

/* If the element value is equal to CURR_TICK then the corresponding
   element in `hard_regno_last_set' is defined and correct.  */
static int hard_regno_last_set_check [FIRST_PSEUDO_REGISTER];

/* Last move (in move sequence being processed) setting up the
   corresponding pseudo.  */
static struct move **pseudo_last_set;

/* If the element value is equal to CURR_TICK then the corresponding
   element in . `pseudo_last_set' is defined and correct.  */
static int *pseudo_last_set_check;

/* This array contains moves sorted topologically (depth-first) on
   their dependency graph.  */
static varray_type move_varray;

/* The variable value is used to check correctness of values of
   elements of arrays `hard_regno_last_set' and
   `pseudo_last_set_check'.  */
static int curr_tick;

/* This recursive function traverses dependecies of MOVE and do
   toplogical sorting (in depth-first order).  */
static void
traverse_moves (struct move *move)
{
  int i;

  if (move->visited_p)
    return;
  move->visited_p = TRUE;
  for (i = move->deps_num - 1; i >= 0; i--)
    traverse_moves (move->deps [i]);
  VARRAY_PUSH_GENERIC_PTR (move_varray, move);
}

/* The function removes unnecessary moves in the LIST, makes
   topological sorting, and removes cycles on hard reg dependencies by
   introducing new pseudos assigned to memory and additional moves.
   It returns the result move list.  */
static struct move *
modify_move_list (struct move *list)
{
  int i, n, nregs, hard_regno;
  pseudo_t to, from, new_pseudo;
  struct move *move, *new_move, *set_move, *first, *last;

  if (list == NULL)
    return NULL;
  /* Creat move deps.  */
  curr_tick++;
  for (move = list; move != NULL; move = move->next)
    {
      to = move->to;
      if ((hard_regno = PSEUDO_HARD_REGNO (to)) < 0)
	continue;
      nregs = hard_regno_nregs [hard_regno] [PSEUDO_MODE (to)];
      for (i = 0; i < nregs; i++)
	{
	  hard_regno_last_set [hard_regno + i] = move;
	  hard_regno_last_set_check [hard_regno + i] = curr_tick;
	}
    }
  for (move = list; move != NULL; move = move->next)
    {
      from = move->from;
      to = move->to;
      if ((hard_regno = PSEUDO_HARD_REGNO (from)) >= 0)
	{
	  nregs = hard_regno_nregs [hard_regno] [PSEUDO_MODE (from)];
	  for (n = i = 0; i < nregs; i++)
	    if (hard_regno_last_set_check [hard_regno + i] == curr_tick
		&& (PSEUDO_REGNO (hard_regno_last_set [hard_regno + i]->to)
		    != PSEUDO_REGNO (from)))
	      n++;
	  move->deps = ira_allocate (n * sizeof (struct move *));
	  for (n = i = 0; i < nregs; i++)
	    if (hard_regno_last_set_check [hard_regno + i] == curr_tick
		&& (PSEUDO_REGNO (hard_regno_last_set [hard_regno + i]->to)
		    != PSEUDO_REGNO (from)))
	      move->deps [n++] = hard_regno_last_set [hard_regno + i];
	  move->deps_num = n;
	}
    }
  /* Toplogical sorting:  */
  VARRAY_POP_ALL (move_varray);
  for (move = list; move != NULL; move = move->next)
    traverse_moves (move);
  last = NULL;
  for (i = VARRAY_ACTIVE_SIZE (move_varray) - 1; i >= 0; i--)
    {
      move = VARRAY_GENERIC_PTR (move_varray, i);
      move->next = NULL;
      if (last != NULL)
	last->next = move;
      last = move;
    }
  first = VARRAY_TOP_GENERIC_PTR (move_varray);
  /* Removing cycles:  */
  curr_tick++;
  VARRAY_POP_ALL (move_varray);
  for (move = first; move != NULL; move = move->next)
    {
      from = move->from;
      to = move->to;
      if ((hard_regno = PSEUDO_HARD_REGNO (from)) >= 0)
	{
	  nregs = hard_regno_nregs [hard_regno] [PSEUDO_MODE (from)];
	  for (i = 0; i < nregs; i++)
	    if (hard_regno_last_set_check [hard_regno + i] == curr_tick
		&& PSEUDO_HARD_REGNO (hard_regno_last_set
				      [hard_regno + i]->to) >= 0)
	      {
		set_move = hard_regno_last_set [hard_regno + i];
		new_pseudo
		  = create_pseudo (PSEUDO_REGNO (set_move->to),
				   PSEUDO_LOOP_TREE_NODE (set_move->to));
		/* That is a minimum to emit pseudos correctly and for
		   setting reg_renumber.  */
		PSEUDO_HARD_REGNO (new_pseudo) = -1;
		PSEUDO_REG (new_pseudo)
		  = create_new_reg (PSEUDO_REG (set_move->to));
		new_move = create_move (set_move->to, new_pseudo);
		set_move->to = new_pseudo;
		VARRAY_PUSH_GENERIC_PTR (move_varray, new_move);
		move_loops_num++;
	      }
	}
      if ((hard_regno = PSEUDO_HARD_REGNO (to)) < 0)
	continue;
      nregs = hard_regno_nregs [hard_regno] [PSEUDO_MODE (to)];
      for (i = 0; i < nregs; i++)
	{
	  hard_regno_last_set [hard_regno + i] = move;
	  hard_regno_last_set_check [hard_regno + i] = curr_tick;
	}
    }
  for (i = VARRAY_ACTIVE_SIZE (move_varray) - 1; i >= 0; i--)
    {
      move = VARRAY_GENERIC_PTR (move_varray, i);
      move->next = NULL;
      last->next = move;
      last = move;
    }
  return first;
}

/* The function generates rtx move insns from the move list LIST.  It
   updates allocation cost using move execution frequency FERQ.  */
static rtx
emit_move_list (struct move *list, int freq)
{
  int cost;
  rtx result;
  enum machine_mode mode;
  enum reg_class cover_class;

  list = modify_move_list (list);
  start_sequence ();
  for (; list != NULL; list = list->next)
    {
      emit_move_insn (PSEUDO_REG (list->to), PSEUDO_REG (list->from));
      mode = PSEUDO_MODE (list->to);
      cover_class = PSEUDO_COVER_CLASS (list->to);
      cost = 0;
      if (PSEUDO_HARD_REGNO (list->to) < 0)
	{
	  if (PSEUDO_HARD_REGNO (list->from) >= 0)
	    {
	      cost = memory_move_cost [mode] [cover_class] [0] * freq;
	      store_cost += cost;
	    }
	}
      else if (PSEUDO_HARD_REGNO (list->from) < 0)
	{
	  if (PSEUDO_HARD_REGNO (list->to) >= 0)
	    {
	      cost = memory_move_cost [mode] [cover_class] [0] * freq;
	      load_cost += cost;
	    }
	}
      else
	{
	  cost = register_move_cost [mode] [cover_class] [cover_class] * freq;
	  shuffle_cost += cost;
	}
      overall_cost += cost;
    }
  result = get_insns ();
  end_sequence ();
  return result;
}

/* The function generates rtx move insns from move lists attached to
   basic blocks and edges.  */
static void
emit_moves (void)
{
  basic_block bb;
  edge_iterator ei;
  edge e;
  rtx insns, tmp;

  FOR_EACH_BB (bb)
    {
      if (at_bb_start [bb->index] != NULL)
	{
	  insns = emit_move_list (at_bb_start [bb->index],
				  REG_FREQ_FROM_BB (bb));
	  tmp = BB_HEAD (bb);
	  if (LABEL_P (tmp))
	    tmp = NEXT_INSN (tmp);
	  if (NOTE_INSN_BASIC_BLOCK_P (tmp))
	    tmp = NEXT_INSN (tmp);
	  if (tmp == BB_HEAD (bb))
	    emit_insn_before (insns, tmp);
	  else if (tmp != NULL_RTX)
	    emit_insn_after (insns, PREV_INSN (tmp));
	  else
	    emit_insn_after (insns, get_last_insn ());
	}

      if (at_bb_end [bb->index] != NULL)
	{
	  insns = emit_move_list (at_bb_end [bb->index],
				  REG_FREQ_FROM_BB (bb));
	  if (! control_flow_insn_p (BB_END (bb)))
	    emit_insn_after (insns, BB_END (bb));
	  else
	    emit_insn_before (insns, BB_END (bb));
	}

      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  if (e->aux == NULL)
	    continue;
	  ira_assert ((e->flags & EDGE_ABNORMAL) == 0
		      || ! EDGE_CRITICAL_P (e));
	  insert_insn_on_edge
	    (emit_move_list (e->aux,
			     REG_FREQ_FROM_EDGE_FREQ (EDGE_FREQUENCY (e))),
	     e);
	  if (e->src->next_bb != e->dest)
	    additional_jumps_num++;
	}
    }
}

/* Entry function changing code and generating pseudo shuffling for
   the regional register allocation.  */
void
ira_emit (void)
{
  int i;
  basic_block bb;
  edge_iterator ei;
  edge e;

  at_bb_start = ira_allocate (sizeof (struct move *) * last_basic_block);
  memset (at_bb_start, 0, sizeof (struct move *) * last_basic_block);
  at_bb_end = ira_allocate (sizeof (struct move *) * last_basic_block);
  memset (at_bb_end, 0, sizeof (struct move *) * last_basic_block);
  for (i = 0; i < pseudos_num; i++)
    if (PSEUDO_REGNO (pseudos [i]) >= 0)
      PSEUDO_REG (pseudos [i]) = regno_reg_rtx [PSEUDO_REGNO (pseudos [i])];
  local_pseudo_bitmap = ira_allocate_bitmap ();
  used_regno_bitmap = ira_allocate_bitmap ();
  max_regno_before_changing = max_reg_num ();
  traverse_loop_tree (ira_loop_tree_root, change_loop, NULL);
  ira_free_bitmap (used_regno_bitmap);
  ira_free_bitmap (local_pseudo_bitmap);
  FOR_EACH_BB (bb)
    {
      at_bb_start [bb->index] = NULL;
      at_bb_end [bb->index] = NULL;
      FOR_EACH_EDGE (e, ei, bb->succs)
	if (e->dest != EXIT_BLOCK_PTR)
	  generate_edge_moves (e);
    }
  pseudo_last_set = ira_allocate (sizeof (struct move *) * max_reg_num ());
  pseudo_last_set_check = ira_allocate (sizeof (int) * max_reg_num ());
  memset (pseudo_last_set_check, 0, sizeof (int) * max_reg_num ());
  memset (hard_regno_last_set_check, 0, sizeof (hard_regno_last_set_check));
  curr_tick = 0;
  FOR_EACH_BB (bb)
    unify_moves (bb, TRUE);
  FOR_EACH_BB (bb)
    unify_moves (bb, FALSE);
  VARRAY_GENERIC_PTR_NOGC_INIT (move_varray, pseudos_num, "ordered moves");
  emit_moves ();
  /* Clean up: */
  FOR_EACH_BB (bb)
    {
      free_move_list (at_bb_start [bb->index]);
      free_move_list (at_bb_end [bb->index]);
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  free_move_list (e->aux);
	  e->aux = NULL;
	}
    }
  VARRAY_FREE (move_varray);
  ira_free (pseudo_last_set_check);
  ira_free (pseudo_last_set);
  commit_edge_insertions ();
  ira_free (at_bb_end);
  ira_free (at_bb_start);
}
