/* Basic block reordering routines for the GNU compiler.
   Copyright (C) 2000 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "insn-config.h"
#include "output.h"
#include "function.h"
#include "obstack.h"
#include "cfglayout.h"

/* The contents of the current function definition are allocated
   in this obstack, and all are freed at the end of the function.
   For top-level functions, this is temporary_obstack.
   Separate obstacks are made for nested functions.  */

extern struct obstack flow_obstack;

/* Structure to hold information about lexical scopes.  */
struct scope_def
{
  int level;

  /* Tree representation of the given scope block.  */
  tree block;

  /* The outer scope or NULL if outermost scope.  */
  struct scope_def *outer;

  /* The first inner scope or NULL if innermost scope.  */
  struct scope_def *inner;

  /* The last inner scope or NULL if innermost scope.  */
  struct scope_def *inner_last;

  /* Link to the next (sibling) scope.  */
  struct scope_def *next;
};

/* Holds the interesting trailing notes for the function.  */
static rtx function_tail_eff_head;

/* The scope forst of current function.  */
static scope scope_tree;

/* Holds scope of each insn.  Indexed by INSN_UID.  */
static varray_type insn_scope;

#define INSN_SCOPE(I) (VARRAY_GENERIC_PTR (insn_scope, INSN_UID (I)))

static rtx skip_insns_after_block	PARAMS ((basic_block));
static void record_effective_endpoints	PARAMS ((void));
static rtx label_for_bb			PARAMS ((basic_block));
static void fixup_reorder_chain		PARAMS ((void));

static scope make_new_scope		PARAMS ((int, rtx));
static scope build_scope_tree		PARAMS ((void));
static void remove_scope_notes		PARAMS ((void));
static void rebuild_scope_notes		PARAMS ((scope));
static void free_scope_tree		PARAMS ((scope));
static void cleanup_unconditional_jumps	PARAMS ((void));
void dump_scope_tree			PARAMS ((FILE *, scope));
static void dump_scope_tree_1		PARAMS ((FILE *, scope, int));
static void fixup_fallthru_exit_predecesor PARAMS ((void));

void verify_insn_chain			PARAMS ((void));
void change_scope			PARAMS ((rtx, scope, scope));

/* Skip over inter-block insns occurring after BB which are typically
   associated with BB (e.g., barriers). If there are any such insns,
   we return the last one. Otherwise, we return the end of BB.  */

static rtx
skip_insns_after_block (bb)
     basic_block bb;
{
  rtx insn, last_insn, next_head, prev;

  next_head = NULL_RTX;
  if (bb->index + 1 != n_basic_blocks)
    next_head = BASIC_BLOCK (bb->index + 1)->head;

  for (last_insn = insn = bb->end; (insn = NEXT_INSN (insn)); )
    {
      if (insn == next_head)
	break;

      switch (GET_CODE (insn))
	{
	case BARRIER:
	  last_insn = insn;
	  continue;

	case NOTE:
	  switch (NOTE_LINE_NUMBER (insn))
	    {
	    case NOTE_INSN_LOOP_END:
	    case NOTE_INSN_BLOCK_END:
	      last_insn = insn;
	      continue;
	    case NOTE_INSN_DELETED:
	    case NOTE_INSN_DELETED_LABEL:
	      continue;

	    default:
	      continue;
	      break;
	    }
	  break;

	case CODE_LABEL:
	  if (NEXT_INSN (insn)
	      && GET_CODE (NEXT_INSN (insn)) == JUMP_INSN
	      && (GET_CODE (PATTERN (NEXT_INSN (insn))) == ADDR_VEC
	          || GET_CODE (PATTERN (NEXT_INSN (insn))) == ADDR_DIFF_VEC))
	    {
	      insn = NEXT_INSN (insn);
	      last_insn = insn;
	      continue;
	    }
          break;

	default:
	  break;
	}

      break;
    }
  /* It is possible to hit contradicting sequence.  For instance:
    
     jump_insn
     NOTE_INSN_LOOP_BEG
     barrier

     Where barrier belongs to jump_insn, but the note does not.
     This can be created by removing the basic block originally
     following NOTE_INSN_LOOP_BEG.

     In such case reorder the notes.  */
  for (insn = last_insn; insn != bb->end; insn = prev)
    {
    prev = PREV_INSN (insn);
    if (GET_CODE (insn) == NOTE)
      switch (NOTE_LINE_NUMBER (insn))
        {
          case NOTE_INSN_LOOP_END:
          case NOTE_INSN_BLOCK_END:
          case NOTE_INSN_DELETED:
          case NOTE_INSN_DELETED_LABEL:
    	continue;
          default:
    	reorder_insns (insn, insn, last_insn);
        }
    }

  return last_insn;
}

/* Locate or create a label for a given basic block.  */

static rtx
label_for_bb (bb)
     basic_block bb;
{
  rtx label = bb->head;

  if (GET_CODE (label) != CODE_LABEL)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, "Emitting label for block %d\n",
		 bb->index);

      label = block_label (bb);
      if (bb->head == PREV_INSN (RBI (bb)->eff_head))
	RBI (bb)->eff_head = label;
    }

  return label;
}

/* Locate the effective beginning and end of the insn chain for each
   block, as defined by skip_insns_after_block above.  */

static void
record_effective_endpoints ()
{
  rtx next_insn = get_insns ();
  int i;
  
  for (i = 0; i < n_basic_blocks; ++i)
    {
      basic_block bb = BASIC_BLOCK (i);
      rtx end;

      RBI (bb)->eff_head = next_insn;
      end = skip_insns_after_block (bb);
      RBI (bb)->eff_end = end;
      next_insn = NEXT_INSN (end);
    }
  function_tail_eff_head = next_insn;
  if (function_tail_eff_head)
    {
      NEXT_INSN (PREV_INSN (function_tail_eff_head)) = NULL;
      set_last_insn (PREV_INSN (function_tail_eff_head));
    }
}

/* Allocate and initialize a new scope structure with scope level LEVEL,
   and record the NOTE beginning the scope.  */

static scope 
make_new_scope (level, note)
     int level;
     rtx note;
{
  scope new_scope = xcalloc (1, sizeof (struct scope_def));
  new_scope->level = level;
  new_scope->block = note ? NOTE_BLOCK (note) : NULL;
  return new_scope;
}


/* Build a tree representing the scope structure of the function.
   Return a pointer to a root of tree.  */

static scope
build_scope_tree ()
{
  rtx x;
  int level, bbi;
  basic_block curr_bb;
  scope root, curr_scope;

  root = curr_scope = make_new_scope (-1, NULL);
  level = -1;
  curr_bb = NULL;
  bbi = 0;
  for (x = get_insns (); x; x = NEXT_INSN (x))
    {
      if (bbi < n_basic_blocks && x == BASIC_BLOCK (bbi)->head)
	curr_bb = BASIC_BLOCK (bbi);

      if (INSN_P (x))
	INSN_SCOPE (x) = curr_scope;

      if (GET_CODE (x) == NOTE)
	{
	  if (NOTE_LINE_NUMBER (x) == NOTE_INSN_BLOCK_BEG)
	    {
	      scope new_scope;
	      if (! curr_scope)
		abort();
	      level++;
	      new_scope = make_new_scope (level, x);
	      new_scope->outer = curr_scope;
	      new_scope->next = NULL;
	      if (! curr_scope->inner)
		{
		  curr_scope->inner = new_scope;
		  curr_scope->inner_last = new_scope;
		}
	      else
		{
		  curr_scope->inner_last->next = new_scope;
		  curr_scope->inner_last = new_scope;
		}
	      curr_scope = curr_scope->inner_last;
	    }
	  else if (NOTE_LINE_NUMBER (x) == NOTE_INSN_BLOCK_END)
	    {
	      if (NOTE_BLOCK (x) != curr_scope->block)
		abort ();
	      level--;
	      curr_scope = curr_scope->outer;
	    }
	} /* if note */

      if (curr_bb && curr_bb->end == x)
	{
	  curr_bb = NULL;
	  bbi++;
	}

    } /* for */
  return root;
}

/* Remove all the NOTE_INSN_BLOCK_BEG and NOTE_INSN_BLOCK_END notes from
   the insn chain.  */

static void
remove_scope_notes ()
{
  rtx x, next;
  basic_block currbb = NULL;

  for (x = get_insns (); x; x = next)
    {
      next = NEXT_INSN (x);
      if (NOTE_INSN_BASIC_BLOCK_P (x))
	currbb = NOTE_BASIC_BLOCK (x);

      if (GET_CODE (x) == NOTE
	  && (NOTE_LINE_NUMBER (x) == NOTE_INSN_BLOCK_BEG
	      || NOTE_LINE_NUMBER (x) == NOTE_INSN_BLOCK_END))
	{
	  /* Check if the scope note happens to be the end of a bb.  */
	  if (currbb && x == currbb->end)
	    currbb->end = PREV_INSN (x);
	  if (currbb && x == currbb->head)
	    abort ();

	  if (PREV_INSN (x))
	    {
	      NEXT_INSN (PREV_INSN (x)) = next;
	      PREV_INSN (next) = PREV_INSN (x);

              NEXT_INSN (x) = NULL;
              PREV_INSN (x) = NULL;
	    }
	  else
	    abort ();
	}
    }
}

/* Emit BASIC_BLOCK notes needed to change scope from S1 to S2.  */

void
change_scope (orig_insn, s1, s2)
     scope s1, s2;
     rtx orig_insn;
{
  rtx insn = orig_insn;
  scope com = NULL;
  scope s;
  scope ts1 = s1, ts2 = s2;

  while (ts1 != ts2)
    {
      if (!(ts1 && ts2))
	abort ();
      if (ts1->level > ts2->level)
	ts1 = ts1->outer;
      else if (ts2->level > ts1->level)
	ts2 = ts2->outer;
      else
	{
	  ts1 = ts1->outer;
	  ts2 = ts2->outer;
	}
    }
  com = ts1;

  /* Close scopes.  */
  s = s1;
  while (s != com)
    {
      if (s->block)
	{
	  rtx note = emit_note_before (NOTE_INSN_BLOCK_END, insn);
	  NOTE_BLOCK (note) = s->block;
	}
      s = s->outer;
    }

  /* Open scopes.  */
  s = s2;
  while (s != com)
    {
      if (s->block)
	{
	  insn = emit_note_before (NOTE_INSN_BLOCK_BEG, insn);
	  NOTE_BLOCK (insn) = s->block;
	}
      s = s->outer;
    }
}

/* Rebuild all the NOTE_INSN_BLOCK_BEG and NOTE_INSN_BLOCK_END notes based
   on the scope tree and the newly reordered basic blocks.  */

static void
rebuild_scope_notes (root)
    scope root;
{
  scope scope = root;
  rtx insn = get_insns ();
  rtx note;

  for (insn = get_insns (); insn; insn = NEXT_INSN (insn))
    {
      if (active_insn_p (insn)
	  && INSN_UID (insn) < insn_scope->num_elements
	  && GET_CODE (PATTERN (insn)) != ADDR_VEC
	  && GET_CODE (PATTERN (insn)) != ADDR_DIFF_VEC
	  && INSN_SCOPE (insn)
	  && INSN_SCOPE (insn) != scope)
	{
	  change_scope (insn, scope, INSN_SCOPE (insn));
	  scope = INSN_SCOPE (insn);
	}
    }
  /* change_scope emits before the insn, not after.  */
  note = emit_note (NULL, NOTE_INSN_DELETED);
  change_scope (get_last_insn (), scope, root);
  delete_insn (note);
}

/* Free the storage associated with the scope tree at S.  */

static void
free_scope_tree (s)
    scope s;
{
  scope p, next;

  for (p = s->inner; p; p = next)
    {
      next = p->next;
      free_scope_tree (p);
    }

  free (s);
}

/* Visualize the scope tree.  */

void
dump_scope_tree (file, root)
    FILE *file;
    scope root;
{
  fprintf (file, "\n< Scope tree >\n");
  dump_scope_tree_1 (file, root, 0);
}

/* Recursive portion of dump_scope_tree.  */

static void
dump_scope_tree_1 (file, s, indent)
     FILE *file;
     scope s;
     int indent;
{
  scope p;

  fprintf (file, "%*s", indent, "");
  if (s->level >= 0)
    fprintf (file, "level %d (block %p)\n", s->level,
	     (PTR) s->block);
  
  for (p = s->inner; p; p = p->next)
    dump_scope_tree_1 (file, p, indent + 2);
}

/* Given a reorder chain, rearrange the code to match.  */

static void
fixup_reorder_chain ()
{
  basic_block bb, last_bb;
  int index;
  rtx insn;
  int old_n_basic_blocks = n_basic_blocks;

  /* First do the bulk reordering -- rechain the blocks without regard to
     the needed changes to jumps and labels.  */

  last_bb = BASIC_BLOCK (0);
  bb = RBI (last_bb)->next;
  index = 1;
  while (bb)
    {
      rtx last_e = RBI (last_bb)->eff_end;
      rtx curr_h = RBI (bb)->eff_head;

      NEXT_INSN (last_e) = curr_h;
      PREV_INSN (curr_h) = last_e;

      last_bb = bb;
      bb = RBI (bb)->next;
      index++;
    }

  if (index != n_basic_blocks)
    abort ();

  insn = RBI (last_bb)->eff_end;

  NEXT_INSN (insn) = function_tail_eff_head;
  if (function_tail_eff_head)
    PREV_INSN (function_tail_eff_head) = insn;

  while (NEXT_INSN (insn))
    insn = NEXT_INSN (insn);
  set_last_insn (insn);
#ifdef ENABLE_CHECKING
  verify_insn_chain ();
#endif

  /* Now add jumps and labels as needed to match the blocks new
     outgoing edges.  */

  for (bb = BASIC_BLOCK (0); bb ; bb = RBI (bb)->next)
    {
      edge e_fall, e_taken, e;
      rtx bb_end_insn;
      basic_block nb;

      if (bb->succ == NULL)
	continue;

      /* Find the old fallthru edge, and another non-EH edge for
	 a taken jump.  */
      e_taken = e_fall = NULL;
      for (e = bb->succ; e ; e = e->succ_next)
	if (e->flags & EDGE_FALLTHRU)
	  e_fall = e;
	else if (! (e->flags & EDGE_EH))
	  e_taken = e;

      bb_end_insn = bb->end;
      if (GET_CODE (bb_end_insn) == JUMP_INSN)
	{
	  if (any_condjump_p (bb_end_insn))
	    {
	      /* If the old fallthru is still next, nothing to do.  */
	      if (RBI (bb)->next == e_fall->dest
	          || (!RBI (bb)->next
		      && e_fall->dest == EXIT_BLOCK_PTR))
		continue;

	      /* There is one special case: if *neither* block is next,
		 such as happens at the very end of a function, then we'll
		 need to add a new unconditional jump.  Choose the taken
		 edge based on known or assumed probability.  */
	      if (RBI (bb)->next != e_taken->dest)
		{
		  rtx note = find_reg_note (bb_end_insn, REG_BR_PROB, 0);
		  if (note
		      && INTVAL (XEXP (note, 0)) < REG_BR_PROB_BASE / 2
		      && invert_jump (bb_end_insn,
				      label_for_bb (e_fall->dest), 0))
		    {
		      e_fall->flags &= ~EDGE_FALLTHRU;
		      e_taken->flags |= EDGE_FALLTHRU;
		      e = e_fall, e_fall = e_taken, e_taken = e;
		    }
		}

	      /* Otherwise we can try to invert the jump.  This will 
		 basically never fail, however, keep up the pretense.  */
	      else if (invert_jump (bb_end_insn,
				    label_for_bb (e_fall->dest), 0))
		{
		  e_fall->flags &= ~EDGE_FALLTHRU;
		  e_taken->flags |= EDGE_FALLTHRU;
		  continue;
		}
	    }
	  else if (returnjump_p (bb_end_insn))
	    continue;
	  else
	    {
	      /* Otherwise we have some switch or computed jump.  In the
		 99% case, there should not have been a fallthru edge.  */
	      if (! e_fall)
		continue;
#ifdef CASE_DROPS_THROUGH
	      /* Except for VAX.  Since we didn't have predication for the
		 tablejump, the fallthru block should not have moved.  */
	      if (RBI (bb)->next == e_fall->dest)
		continue;
	      bb_end_insn = skip_insns_after_block (bb);
#else
	      abort ();
#endif
	    }
	}
      else
	{
	  /* No fallthru implies a noreturn function with EH edges, or
	     something similarly bizarre.  In any case, we don't need to
	     do anything.  */
	  if (! e_fall)
	    continue;

	  /* If the fallthru block is still next, nothing to do.  */
	  if (RBI (bb)->next == e_fall->dest)
	    continue;

	  /* An fallthru to exit block.  */
	  if (!RBI (bb)->next && e_fall->dest == EXIT_BLOCK_PTR)
	    continue;
	}

      /* We got here if we need to add a new jump insn.  */

      nb = force_nonfallthru (e_fall);

      if (nb)
	{
	  alloc_aux_for_block (nb, sizeof (struct reorder_block_def));
	  RBI (nb)->eff_head = nb->head;
	  RBI (nb)->eff_end = NEXT_INSN (nb->end);
	  RBI (nb)->scope = RBI (bb)->scope;
	  RBI (nb)->visited = 1;
	  RBI (nb)->next = RBI (bb)->next;
	  RBI (bb)->next = nb;
	  /* Don't process this new block.  */
	  bb = nb;
	}
    }

  /* Put basic_block_info in the new order.  */
  bb = BASIC_BLOCK (0);
  index = 0;

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "Reordered sequence:\n");
  while (bb)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, " %i %sbb %i freq %i\n", index,
		 bb->index >= old_n_basic_blocks ? "compensation " : "",
		 bb->index,
	   	 bb->frequency);
      bb->index = index;
      BASIC_BLOCK (index) = bb;

      bb = RBI (bb)->next;
      index++;
    }
}

/* Perform sanity checks on the insn chain.
   1. Check that next/prev pointers are consistent in both the forward and
      reverse direction.
   2. Count insns in chain, going both directions, and check if equal.
   3. Check that get_last_insn () returns the actual end of chain.  */

void
verify_insn_chain ()
{
  rtx x,
      prevx,
      nextx;
  int insn_cnt1,
      insn_cnt2;

  prevx = NULL;
  insn_cnt1 = 1;
  for (x = get_insns (); x; x = NEXT_INSN (x))
    {
      if (PREV_INSN (x) != prevx)
	{
	  fprintf (stderr, "Forward traversal: insn chain corrupt.\n");
	  fprintf (stderr, "previous insn:\n");
	  debug_rtx (prevx);
	  fprintf (stderr, "current insn:\n");
	  debug_rtx (x);
	  abort ();
	}
      ++insn_cnt1;
      prevx = x;
    }

  if (prevx != get_last_insn ())
    {
      fprintf (stderr, "last_insn corrupt.\n");
      abort ();
    }

  nextx = NULL;
  insn_cnt2 = 1;
  for (x = get_last_insn (); x; x = PREV_INSN (x))
    {
      if (NEXT_INSN (x) != nextx)
	{
	  fprintf (stderr, "Reverse traversal: insn chain corrupt.\n");
	  fprintf (stderr, "current insn:\n");
	  debug_rtx (x);
	  fprintf (stderr, "next insn:\n");
	  debug_rtx (nextx);
	  abort ();
	}
      ++insn_cnt2;
      nextx = x;
    }

  if (insn_cnt1 != insn_cnt2)
    {
      fprintf (stderr, "insn_cnt1 (%d) not equal to insn_cnt2 (%d).\n",
	       insn_cnt1, insn_cnt2);
      abort ();
    }
}

/* Remove any unconditional jumps and forwarder block creating fallthru
   edges instead.  During BB reordering fallthru edges are not required
   to target next basic block in the linear CFG layout, so the unconditional
   jumps are not needed.  */

static void
cleanup_unconditional_jumps ()
{
  int i;
  for (i = 0; i < n_basic_blocks; i++)
    {
      basic_block bb = BASIC_BLOCK (i);

      if (!bb->succ)
	continue;
      if (bb->succ->flags & EDGE_FALLTHRU)
	continue;
      if (!bb->succ->succ_next)
	{
	  rtx insn;
	  if (GET_CODE (bb->head) != CODE_LABEL && forwarder_block_p (bb) && i)
	    {
	      basic_block prev = BASIC_BLOCK (--i);

	      if (rtl_dump_file)
		fprintf (rtl_dump_file, "Removing forwarder BB %i\n",
			 bb->index);

	      redirect_edge_succ (bb->pred, bb->succ->dest);
	      flow_delete_block (bb);
	      bb = prev;
	    }
	  else if (simplejump_p (bb->end))
	    {
	      rtx jump = bb->end;

	      if (rtl_dump_file)
		fprintf (rtl_dump_file, "Removing jump %i in BB %i\n",
			 INSN_UID (jump), bb->index);
	      delete_insn (jump);
	      bb->succ->flags |= EDGE_FALLTHRU;
	    }
	  else
	    continue;

	  /* Cleanup barriers and delete ADDR_VECs in a way as they are belonging
             to removed tablejump anyway.  */
	  insn = NEXT_INSN (bb->end);
	  while (insn
		 && (GET_CODE (insn) != NOTE
		     || NOTE_LINE_NUMBER (insn) != NOTE_INSN_BASIC_BLOCK))
	    {
	      rtx next = NEXT_INSN (insn);

	      if (GET_CODE (insn) == BARRIER)
		delete_barrier (insn);
	      else if (GET_CODE (insn) == JUMP_INSN)
		delete_insn_chain (PREV_INSN (insn), insn);
	      else if (GET_CODE (insn) == CODE_LABEL)
		;
	      else if (GET_CODE (insn) != NOTE)
		abort ();

	      insn = next;
	    }
	}
    }
}

/* The block falling trought to exit must be last in the reordered
   chain.  Make it happen so.  */
static void
fixup_fallthru_exit_predecesor ()
{
  edge e;
  basic_block bb = NULL;

  for (e = EXIT_BLOCK_PTR->pred; e; e = e->pred_next)
    if (e->flags & EDGE_FALLTHRU)
      bb = e->src;
  if (bb && RBI (bb)->next)
    {
      basic_block c = BASIC_BLOCK (0);
      while (RBI (c)->next != bb)
	c = RBI (c)->next;
      RBI (c)->next = RBI (bb)->next;
      while (RBI (c)->next)
	c = RBI (c)->next;
      RBI (c)->next = bb;
      RBI (bb)->next = NULL;
    }
}

/* Return true in case it is possible to duplicate the basic block BB.  */

bool
cfg_layout_can_duplicate_bb_p (bb)
     basic_block bb;
{
  rtx next;
  edge s;

  if (bb == EXIT_BLOCK_PTR || bb == ENTRY_BLOCK_PTR)
    return false;

  /* Duplicating fallthru block to exit would require adding an jump
     and splitting the real last BB.  */
  for (s = bb->succ; s; s = s->succ_next)
    if (s->dest == EXIT_BLOCK_PTR && s->flags & EDGE_FALLTHRU)
       return false;

  /* Do not attempt to duplicate tablejumps, as we need to unshare
     the dispatch table.  This is dificult to do, as the instructions
     computing jump destination may be hoisted outside the basic block.  */
  if (GET_CODE (bb->end) == JUMP_INSN && JUMP_LABEL (bb->end)
      && (next = next_nonnote_insn (JUMP_LABEL (bb->end)))
      && GET_CODE (next) == JUMP_INSN
      && (GET_CODE (PATTERN (next)) == ADDR_VEC
	  || GET_CODE (PATTERN (next)) == ADDR_DIFF_VEC))
    return false;
  return true;
}

/* Create an duplicate of the basic block BB and redirect edge E into it.  */

basic_block
cfg_layout_duplicate_bb (bb, e)
     basic_block bb;
     edge e;
{
  rtx last = get_last_insn ();
  rtx insn, new = NULL_RTX;
  rtx note1, note2;
  rtx pre_head = NULL_RTX, end = NULL_RTX;
  edge s, n;
  basic_block new_bb;

  if (!bb->pred || !bb->pred->pred_next)
    abort ();
#ifdef ENABLE_CHECKING
  if (!cfg_layout_can_duplicate_bb_p (bb))
    abort ();
#endif

  /* Create copy at the end of INSN chain.  The chain will
     be reordered later.  */
  for (insn = RBI (bb)->eff_head; insn != NEXT_INSN (RBI (bb)->eff_end);
       insn = NEXT_INSN (insn))
    {
      new = NULL;
      switch (GET_CODE (insn))
	{
	case INSN:
	  new = emit_insn (copy_insn (PATTERN (insn)));
	  VARRAY_GROW (insn_scope, INSN_UID (new) + 1);
	  INSN_SCOPE (new) = INSN_SCOPE (insn);
	  if (REG_NOTES (insn))
	    REG_NOTES (new) = copy_insn (REG_NOTES (insn));
	  break;

	case JUMP_INSN:
	  /* Avoid copying of dispatch tables.  We never duplicate
	     tablejumps, so this can hit only in case the table got
	     moved far from original jump.  */
	  if (GET_CODE (PATTERN (insn)) == ADDR_VEC
	      || GET_CODE (PATTERN (insn)) == ADDR_DIFF_VEC)
	    abort ();
	  new = emit_jump_insn (copy_insn (PATTERN (insn)));
	  VARRAY_GROW (insn_scope, INSN_UID (new) + 1);
	  INSN_SCOPE (new) = INSN_SCOPE (insn);
	  if (REG_NOTES (insn))
	    REG_NOTES (new) = copy_insn (REG_NOTES (insn));
	  mark_jump_label (PATTERN (new), new, 0);
	  if (JUMP_LABEL (new))
	    LABEL_NUSES (JUMP_LABEL (new))++;
	  break;

	case CALL_INSN:
	  new = emit_call_insn (copy_insn (PATTERN (insn)));
	  VARRAY_GROW (insn_scope, INSN_UID (new) + 1);
	  INSN_SCOPE (new) = INSN_SCOPE (insn);
	  if (REG_NOTES (insn))
	    REG_NOTES (new) = copy_insn (REG_NOTES (insn));
	  if (CALL_INSN_FUNCTION_USAGE (insn))
	    CALL_INSN_FUNCTION_USAGE (new)
	      = copy_insn (CALL_INSN_FUNCTION_USAGE (insn));
	  SIBLING_CALL_P (new) = SIBLING_CALL_P (insn);
	  CONST_OR_PURE_CALL_P (new) = CONST_OR_PURE_CALL_P (insn);
	  break;

	case CODE_LABEL:
	  break;

	case BARRIER:
	  new = emit_barrier ();
	  break;

	case NOTE:
	  switch (NOTE_LINE_NUMBER (insn))
	    {
	    case NOTE_INSN_LOOP_VTOP:
	    case NOTE_INSN_LOOP_CONT:
	    case NOTE_INSN_LOOP_BEG:
	    case NOTE_INSN_LOOP_END:
	      /* Strip down the loop notes - we don't really want to keep
	         them consistent in loop copies.  */
	    case NOTE_INSN_DELETED:
	    case NOTE_INSN_DELETED_LABEL:
	      /* No problem to strip these.  */
	    case NOTE_INSN_EPILOGUE_BEG:
	    case NOTE_INSN_FUNCTION_END:
	      /* Debug code expect these notes to exist just once.
	         Keep them in the master copy.
	         ??? It probably makes more sense to duplicate them for each
	         epilogue copy.  */
	    case NOTE_INSN_FUNCTION_BEG:
	      /* There is always just single entry to function.  */
	      break;

	    case NOTE_INSN_BASIC_BLOCK:
	      pre_head = get_last_insn ();
	      break;

	    case NOTE_INSN_PROLOGUE_END:
	      /* There is no purpose to duplicate prologue.  */
	    case NOTE_INSN_BLOCK_BEG:
	    case NOTE_INSN_BLOCK_END:
	      /* The BLOCK_BEG/BLOCK_END notes should be eliminated when BB
	         reordering is in the progress.  */
	    case NOTE_INSN_EH_REGION_BEG:
	    case NOTE_INSN_EH_REGION_END:
	    case NOTE_INSN_RANGE_BEG:
	    case NOTE_INSN_RANGE_END:
	      /* Should never exist at BB duplication time.  */
	      abort ();
	      break;

	    default:
	      if (NOTE_LINE_NUMBER (insn) < 0)
		abort ();
	      /* It is possible that no_line_number is set and the note
		 won't be emitted.  */
	      {
		rtx x = emit_note (NOTE_SOURCE_FILE (insn),
				   NOTE_LINE_NUMBER (insn));
		if (x)
		  new = x;
	      }
	    }
	  break;
	default:
	  abort ();
	}
      /* Fix the libcall sequences.  */
      if (new && INSN_P (new)
	  && (note1 = find_reg_note (new, REG_RETVAL, NULL_RTX)) != NULL)
	{
	  rtx p = new;
	  while ((note2 = find_reg_note (p, REG_LIBCALL, NULL_RTX)) == NULL)
	    {
	      p = PREV_INSN (p);
	      if (p == pre_head)
		abort ();
	    }
	  XEXP (note1, 0) = p;
	  XEXP (note2, 0) = new;
	}
      if (bb->end == insn)
	end = get_last_insn ();
    }

  new_bb = create_basic_block (n_basic_blocks, NEXT_INSN (pre_head), end);

  if (bb->global_live_at_start)
    {
      new_bb->global_live_at_start = OBSTACK_ALLOC_REG_SET (&flow_obstack);
      new_bb->global_live_at_end = OBSTACK_ALLOC_REG_SET (&flow_obstack);
      COPY_REG_SET (new_bb->global_live_at_start, bb->global_live_at_start);
      COPY_REG_SET (new_bb->global_live_at_end, bb->global_live_at_end);
    }

  new_bb->loop_depth = bb->loop_depth;
  new_bb->flags = bb->flags;
  for (s = bb->succ; s; s = s->succ_next)
    {
      n = make_edge (new_bb, s->dest, s->flags);
      n->probability = s->probability;
      if (bb->count)
	n->count = s->count * e->count / bb->count;
      else
	n->count = 0;
    }

  new_bb->count = e->count;
  new_bb->frequency = EDGE_FREQUENCY (e);
  bb->count -= e->count;
  bb->frequency -= EDGE_FREQUENCY (e);
  if (bb->count < 0)
    bb->count = 0;
  if (bb->frequency < 0)
    bb->frequency = 0;

  /* Avoid redirect_edge_and_branch from overactive optimizing.  */
  new_bb->index = n_basic_blocks + 1;
  if (e->flags & EDGE_FALLTHRU)
    redirect_edge_succ (e, new_bb);
  else
    redirect_edge_and_branch (e, new_bb);
  new_bb->index = n_basic_blocks - 1;

  alloc_aux_for_block (new_bb, sizeof (struct reorder_block_def));
  RBI (new_bb)->eff_head = NEXT_INSN (last);
  RBI (new_bb)->eff_end = get_last_insn ();
  RBI (new_bb)->scope = RBI (bb)->scope;
  return new_bb;
}

/* Main entry point to this module - initialize the datastructures for
   CFG layout changes.  */

void
cfg_layout_initialize ()
{
  VARRAY_GENERIC_PTR_INIT (insn_scope, get_max_uid (), "insn scopes");
  scope_tree = build_scope_tree ();
  if (rtl_dump_file)
    dump_scope_tree (rtl_dump_file, scope_tree);
  remove_scope_notes ();

  alloc_aux_for_blocks (sizeof (struct reorder_block_def));
  cleanup_unconditional_jumps ();
  record_effective_endpoints ();
}

/* Finalize the changes - reorder insn list according to the sequence,
   enter compensation code.  */

void
cfg_layout_finalize ()
{
  fixup_fallthru_exit_predecesor ();
  fixup_reorder_chain ();
#ifdef ENABLE_CHECKING
  verify_insn_chain ();
#endif

  free_aux_for_blocks ();

#ifdef ENABLE_CHECKING
  verify_flow_info ();
#endif

  rebuild_scope_notes (scope_tree);
  free_scope_tree (scope_tree);
  reorder_blocks ();
  /* Dump the newly formed tree - this makes visible the changes
     done by reorder_blocks - it should contain duplicated regions when
     needed.  */
  VARRAY_FREE (insn_scope);
  if (rtl_dump_file)
    {
      VARRAY_GENERIC_PTR_INIT (insn_scope, get_max_uid (), "insn scopes");
      scope_tree = build_scope_tree ();
      dump_scope_tree (rtl_dump_file, scope_tree);
      free_scope_tree (scope_tree);
      VARRAY_FREE (insn_scope);
    }
}
