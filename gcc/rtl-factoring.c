/* RTL factoring (code hoisting/sinking and sequence abstraction).
   Copyright (C) 2004 Free Software Foundation, Inc.

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
#include "obstack.h"
#include "basic-block.h"
#include "resource.h"
#include "flags.h"
#include "ggc.h"
#include "regs.h"
#include "params.h"
#include "expr.h"
#include "tm_p.h"
#include "fact-common.h"
#include "df.h"
#include "diagnostic.h"
#include "tree-pass.h"
#include "timevar.h"

/* TODO:
   Sequence abstraction:
   - Use length attribute of insns to calculate gain. (Count insns only if
     length is not available.)
   - Use REG_ALLOC_ORDER when choosing link register.
   - Handle JUMP_INSNs. Also handle volatile function calls (handle them
     simmilar to unconditional jumps.)
   - Test command line option -fpic.
*/

/* Predicate yielding nonzero iff X is an abstractable insn.  Non-jump insns are
   abstractable.  */
#define ABSTRACTABLE_INSN_P(X) (INSN_P (X) && !JUMP_P (X))

/* Predicate yielding nonzero iff X is a call insn.  */
#define CALL_P(X) (GET_CODE (X) == CALL_INSN)

#ifndef REGNO_MODE_OK_FOR_BASE_P
/* Using REGNO_OK_FOR_BASE_P if target machine does not define this macro.  */
#define REGNO_MODE_OK_FOR_BASE_P(REGNO, MODE) REGNO_OK_FOR_BASE_P (REGNO)
#endif

/* Cost of a call sequence for seq.abstr.
   FIXME: Should be 2 but on ARM reorg transforms a symbol_ref into a load from
   the constant pool so compensate it here.  */
#define CALL_COST 3

/* Cost of a return insn.  */
#define RETURN_COST 1

/* Gain conter for code hoisting/sinking.  */
static int gain;

/* A sequence matching a pattern sequence.  */
typedef struct matching_seq_def
{
  /* The last insn in the matching sequence.  */
  rtx insn;

  /* The number of insns matching in this sequence and the pattern sequence.
   */
  int matching_length;

  /* The number of insns selected to abstract from this sequence. Less than
     or equal to MATCHING_LENGTH.  */
  int abstracted_length;

  /* The next sequence in the chain matching the same pattern.  */
  struct matching_seq_def *next_matching_seq;
} *matching_seq;


/* A pattern instruction sequence.  */
typedef struct pattern_seq_def
{
  /* The last insn in the pattern sequence.  */
  rtx insn;

  /* The gain of transforming the pattern sequence into a pseudo-function and
     the matching sequences into pseudo-calls.  */
  int gain;

  /* The maximum of the ABSTRACTED_LENGTH of the matching sequences.  */
  int abstracted_length;

  /* The register used to hold the return address during the pseudo-call.  */
  rtx link_reg;

  /* The sequences matching this pattern.  */
  matching_seq matching_seqs;

  /* The next pattern sequence in the chain.  */
  struct pattern_seq_def *next_pattern_seq;
} *pattern_seq;


/* A block of a pattern sequence.  */
typedef struct seq_block_def
{
  /* The number of insns in the block.  */
  int length;

  /* The code_label of the block.  */
  rtx label;

  /* The sequences entering the pattern sequence at LABEL.  */
  matching_seq matching_seqs;

  /* The next block in the chain. The blocks are sorted by LENGTH in
     ascending order.  */
  struct seq_block_def *next_seq_block;
} *seq_block;


/* The pattern sequences collected from the current functions.  */
static pattern_seq pattern_seqs;

/* The blocks of the current pattern sequence.  */
static seq_block seq_blocks;

/* Auxiliary routine for insn_dependent_p called through note_stores.  */

static void
insn_dependent_p_1 (rtx x, rtx pat ATTRIBUTE_UNUSED, void *data)
{
  rtx *pinsn = (rtx *) data;

  if (*pinsn && reg_mentioned_p (x, *pinsn))
    *pinsn = NULL_RTX;
}

/* Return true if anything in insn X is (anti,output,true) dependent on
   anything in insn Y.  */

static bool
insn_dependent_p (rtx x, rtx y)
{
  rtx tmp;

  if (CALL_P (x) || CALL_P (y))
    return true;

  gcc_assert (INSN_P (x) && INSN_P (y));

  tmp = PATTERN (y);
  note_stores (PATTERN (x), insn_dependent_p_1, &tmp);
  if (tmp == NULL_RTX)
    return true;

  tmp = PATTERN (x);
  note_stores (PATTERN (y), insn_dependent_p_1, &tmp);
  if (tmp == NULL_RTX)
    return true;

  return false;
}

/* Deletion of the 'analogous' relations from FIRST.  */

static void
delete_analogous (bb_decorator first)
{
  bb_decorator last = NULL;
  bb_decorator di;
  for (di = first; di; di = di->next_analogous)
    {
      if (last)
        last->next_analogous = NULL;
      last = di;
      di->prev_analogous = NULL;
    }
}

/* Insert INSN into BB. If BB's last instruction is jump, INSN will be before
   the jump instruction.  */

static void
insert_insn_end_of_basic_block (rtx insn, basic_block bb)
{
  rtx last = BB_END (bb);

  if (JUMP_P (last))
    {
      if (reg_set_p (cc0_rtx, PREV_INSN (last)))
        add_insn_before (insn, PREV_INSN (last));
      else
        add_insn_before (insn, last);
    }
  else
    add_insn_after (insn, last);
}

/* It returns with 1, if the INSN is moveable from the block while hoisting the
   common code. Otherwise returns with 0.  */

static int
moveable_from_begin (rtx insn)
{
  basic_block bb = BLOCK_FOR_INSN (insn);
  rtx head = BB_HEAD (bb);
  rtx prev;

  if (insn == head)
    return 1;

  prev = PREV_INSN (insn);

  while (prev)
    {
      if (INSN_P (prev))
        if (insn_dependent_p (insn, prev))
          return 0;
      if (prev == head)
        prev = NULL;
      else
        prev = PREV_INSN (prev);
    }
  return 1;
}

/* Return with the first not examined insn in the DECORATOR's current block,
   which would be good to move into the pred bb. If we find a jump or call
   insn return with NULL_RTX, and there is no more investigation in this
   block to try moving an other insn. The MAX_DEPTH describes the maximum
   searching distance from the begining of the block.  */

static rtx
find_first_from_begin (bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  rtx insn;
  int depth = 0;
  decorator->moveable_insn = NULL_RTX;
  for (insn = BB_HEAD (bb); insn != BB_END (bb); insn = NEXT_INSN (insn))
    {
      if (!INSN_P (insn))
        continue;
      if (depth < decorator->last_depth)
        {
          depth++;
          continue;
        }

      if (GET_CODE (insn) == CALL_INSN || volatile_refs_p (PATTERN (insn)))
        return NULL_RTX;
      if (reg_set_p (cc0_rtx, insn))
        {
          depth++;
          decorator->last_depth = depth;
          continue;
        }
      if (moveable_from_begin (insn))
        {
          decorator->moveable_insn = insn;
          return (insn);
        }
      depth++;
      decorator->last_depth = depth;


      if (depth == max_depth - 1)
        break;
    }
  return NULL_RTX;
}

/* Finding of an instruction for code hoisting in the current block of
   DECORATOR which is equal with INSN and which is outsourcable from the
   block.  */

static int
find_insn_from_begin (rtx insn, bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  rtx tmp;
  int depth = 0;

  for (tmp = BB_HEAD (bb); tmp != BB_END (bb); tmp = NEXT_INSN (tmp))
    {
      if (!INSN_P (tmp))
        continue;
      if (rtx_equal_p (PATTERN (tmp), PATTERN (insn)))
        {
          decorator->moveable_insn = tmp;
          return 1;
        }
      depth++;
      if (depth == max_depth)
        return 0;

      if (insn_dependent_p (insn, tmp))
        return 0;
    }
  return 0;
}

/* Main function of local factoring hoisting. It returns 1 if there was any
   hoisted insn. Otherwise it returns 0.  */

static int
rtl_hoisting (int depth, bb_decorator decorator)
{
  bb_decorator di = decorator;
  int change = 0;

  while (di)
    {
      di->last_depth = 0;
      di = di->next_decorator;
    }
  di = decorator;
  while (di)
    {
      if ((!di->prev_sibling) && (di->next_sibling))
        {
          bb_decorator di2;
          rtx insn = NULL;
          int not_hoistable = 0;
          edge e;
          edge_iterator ei;
          insn = find_first_from_begin (di, depth);

          if (!insn)
            {
              di = di->next_decorator;
              continue;
            }

          /* Finding equal instruction in the concatenated basic blocks.  */
          for (di2 = di->next_sibling; di2; di2 = di2->next_sibling)
            if (!find_insn_from_begin (insn, di2, depth))
              {
                not_hoistable = 1;
                di->last_depth++;
                break;
              }

          if (not_hoistable)
            continue;

          /* Verifying the insn if it is insertable after the end of pred
             functions.  */
          FOR_EACH_EDGE (e, ei, (di->curr)->preds)
          {
            if (!JUMP_P (BB_END (e->src)))
              continue;
            if (insn_dependent_p (insn, BB_END (e->src))
                || (reg_set_p (cc0_rtx, PREV_INSN (BB_END (e->src)))
                    && insn_dependent_p (insn, PREV_INSN (BB_END (e->src)))))
              {
                not_hoistable = 1;
                break;
              }
          }

          if (not_hoistable)
            {
              di->last_depth++;
              continue;
            }

          if (!not_hoistable)
            {
              /* Moving the insn.  */
              edge e;
              edge_iterator ei;

              FOR_EACH_EDGE (e, ei, (di->curr)->preds)
              {
                rtx new_insn = make_insn_raw (PATTERN (di->moveable_insn));
                insert_insn_end_of_basic_block (new_insn, e->src);
                gain--;

              }

              for (di2 = di; di2; di2 = di2->next_sibling)
                {
                  rtx rinsn = di2->moveable_insn;
                  di2->moveable_insn = NULL;
                  remove_insn (rinsn);
                  gain++;
                }
              change = 1;
            }
        }
      else
        di = di->next_decorator;
    }
  return change;
}

/* Insert INSN into BB to the first possible place.  */

static void
insert_insn_begin_of_basic_block (rtx insn, basic_block bb)
{
  rtx first = BB_HEAD (bb);
  while (!INSN_P (first))
    first = NEXT_INSN (first);

  add_insn_before (insn, first);
}

/* The function returns with 1, if the INSN is moveable from the block while
   sinking the common code. Otherwise it returns 0.  */

static int
moveable_from_end (rtx insn)
{
  basic_block bb = BLOCK_FOR_INSN (insn);
  rtx end = BB_END (bb);
  rtx next;

  if (insn == end)
    return 1;

  next = NEXT_INSN (insn);

  while (next)
    {
      if (INSN_P (next) && (insn_dependent_p (next, insn)))
        return 0;
      if (next == end)
        next = NULL;
      else
        next = NEXT_INSN (next);
    }
  return 1;
}

/* Return with the first not examined insn in the DECORATOR's current block,
   which would be good to move into the succ bb. If we find a jump or call
   insn return with NULL_RTX, and there is no more investigation in this
   block to try moving an other insn. The MAX_DEPTH describes the maximum
   searching distance from the begining of the block.  */

static rtx
find_first_from_end (bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  rtx insn;
  int depth = 0;
  decorator->moveable_insn = NULL_RTX;
  for (insn = BB_END (bb); insn != BB_HEAD (bb); insn = PREV_INSN (insn))
    {
      if (!INSN_P (insn) || JUMP_P (insn))
        continue;

      if (depth < decorator->last_depth)
        {
          depth++;
          continue;
        }

      if ((GET_CODE (insn) == CALL_INSN) || volatile_refs_p (PATTERN (insn)))
        return NULL_RTX;

      if (moveable_from_end (insn))
        {
          decorator->moveable_insn = insn;
          return (insn);
        }
      depth++;
      decorator->last_depth++;
      if (depth == max_depth)
        break;
    }
  return NULL_RTX;
}

/* Finding of an instruction for code sinking in the current block of
   DECORATOR which is equal with INSN and which is outsourcable from the
   block.  */

static int
find_insn_from_end (rtx insn, bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  rtx tmp;
  int depth = 0;
  for (tmp = BB_END (bb); tmp != BB_HEAD (bb); tmp = PREV_INSN (tmp))
    {
      if (!INSN_P (tmp) || JUMP_P (tmp))
        continue;
      if (GET_CODE (tmp) == CALL_INSN)
        return 0;
      if (rtx_equal_p (PATTERN (tmp), PATTERN (insn)))
        {
          decorator->moveable_insn = tmp;
          return (1);
        }
      depth++;
      if (depth == max_depth)
        return 0;
      if (insn_dependent_p (insn, tmp))
        return 0;
    }
  return 0;
}

/* This function do the statement sinking for the FIRST bb_decorator list.  */

static void
sinking_rtl_stmt (bb_decorator first)
{
  edge e;
  edge_iterator ei;
  bb_decorator di;

  FOR_EACH_EDGE (e, ei, (first->curr)->succs)
  {
    rtx new_insn = make_insn_raw (PATTERN (first->moveable_insn));
    insert_insn_begin_of_basic_block (new_insn, e->dest);
    gain--;
  }

  for (di = first; di; di = di->next_analogous)
    {
      rtx rinsn = di->moveable_insn;
      di->moveable_insn = NULL;
      remove_insn (rinsn);
      gain++;
    }

  delete_analogous (first);
}

/* If we insert a new basic block (NEW_BB), the relations between the basic
   blocks (FIRST) will be changed, so we must modifying some concatenation.  */

static void
modifying_concatenation (bb_decorator first, bb_decorator new_bb)
{
  bb_decorator last_sibling = first->prev_sibling;
  bb_decorator first_sibling = NULL;
  bb_decorator last_analogous = first;
  bb_decorator di;

  if (first->prev_sibling)
    {
      (first->prev_sibling)->next_sibling = NULL;
      first->prev_sibling = NULL;
    }
  for (di = first->next_sibling; di; di = di->next_sibling)
    {
      if (di->prev_sibling == last_analogous)
        {
          if (last_analogous->next_analogous != di)
            {
              di->prev_sibling->next_sibling = NULL;
              di->prev_sibling = last_sibling;
              if (last_sibling)
                {
                  if (last_sibling->next_sibling)
                    last_sibling->next_sibling->prev_sibling = NULL;
                  last_sibling->next_sibling = di;
                }
              last_sibling = di;
              if (!first_sibling)
                first_sibling = di;
            }
          else
            last_analogous = di;
        }
      else if (di->prev_sibling == last_sibling)
        {
          if (last_analogous->next_analogous != di)
            {
              last_sibling = di;
              if (!first_sibling)
                first_sibling = di;
            }
          else
            last_analogous = di;
        }
    }
  if (last_sibling)
    {
      last_sibling->next_sibling = new_bb;
      new_bb->prev_sibling = last_sibling;
    }

  delete_siblings (first);
  for (di = first; di; di = di->next_analogous)
    {
      di->prev_sibling = di->prev_analogous;
      di->next_sibling = di->next_analogous;
    }
  delete_analogous (first);
}

/* Modification of the edges and LABEL jump labels of basic blocks (FIRST) if
   it's necessary after the insertion of new NEW_BB basic block.  */

static void
modify_jump_labels (basic_block new_bb, bb_decorator first, rtx label)
{
  bb_decorator di;
  for (di = first; di; di = di->next_analogous)
    {
      rtx jump = BB_END (di->curr);
      if (JUMP_P (jump))
        {
          redirect_jump (jump, label, 0);
          redirect_edge_succ_nodup (EDGE_SUCC (di->curr, 0), new_bb);
        }
      else
        redirect_edge_succ_nodup (EDGE_SUCC (di->curr, 0), new_bb);
    }
}


/* It is not safe to modify the destination of edge of a bb if this edge
   is ABNORMAL. This function returns 1 if there is minimum two succ edges
   which have got ABNORMAL flag. Otherwise return 0.  */

static int
find_abnormal_edge (bb_decorator first)
{
  int num_of_not_abnormal_succ = 0;
  bb_decorator di;
  for (di = first; di; di = di->next_analogous)
    {
      if (EDGE_SUCC (di->curr, 0)->flags & EDGE_ABNORMAL)
        {
          if (di->prev_analogous)
            di->prev_analogous->next_analogous = di->next_analogous;
          if (di->next_analogous)
            di->next_analogous->prev_analogous = di->prev_analogous;
          di->prev_analogous = di->next_analogous = NULL;
        }
      else
        num_of_not_abnormal_succ++;
    }
  return (num_of_not_abnormal_succ > 1 ? 1 : 0);
}

/* Creation of new basic block for partial matching. Return with the new basic
   block.  */

static basic_block
create_basic_block_by_factoring (bb_decorator first)
{
  bb_decorator di;
  basic_block next_bb = EDGE_SUCC (first->curr, 0)->dest;
  basic_block prev = next_bb->prev_bb;
  basic_block new_bb;
  rtx next;
  rtx new_insn;
  rtx label = gen_label_rtx ();
  edge f, new_edge;
  edge_iterator ei;
  rtx insn = first->moveable_insn;
  int find = 0;
  edge e = NULL;
  int find2 = 0;
  rtx last = get_last_insn ();

  next = BB_END (next_bb->prev_bb);
  while (BARRIER_P (NEXT_INSN (next)))
    next = NEXT_INSN (next);

  new_bb = create_basic_block (NULL, NULL, prev);
  PREV_INSN (BB_HEAD (new_bb)) = next;
  NEXT_INSN (BB_HEAD (new_bb)) = NEXT_INSN (next);
  PREV_INSN (NEXT_INSN (next)) = BB_HEAD (new_bb);
  NEXT_INSN (next) = BB_HEAD (new_bb);
  NEXT_INSN (last) = NULL;
  set_last_insn (last);

  /* Creation of the new instructions for the new bb.  */
  new_insn = make_insn_raw (PATTERN (insn));
  gain--;
  PUT_MODE (new_insn, GET_MODE (insn));
  INSN_LOCATOR (new_insn) = INSN_LOCATOR (insn);

  emit_label_before (label, BB_HEAD (new_bb));
  BB_HEAD (new_bb) = label;
  set_block_for_insn (label, new_bb);
  add_insn_after (new_insn, BB_END (new_bb));
  set_block_for_insn (new_insn, new_bb);
  BB_END (new_bb) = new_insn;

  /* Creation of the edge of the new bb.  */
  new_edge = make_edge (new_bb, next_bb, EDGE_FALLTHRU);
  if (new_edge)
    new_edge->probability = REG_BR_PROB_BASE;

  FOR_EACH_EDGE (f, ei, prev->succs)
  {
    if (f->dest == next_bb)
      {
        e = f;
        for (di = first; di; di = di->next_analogous)
          if (prev == di->curr)
            {
              find = 1;
              break;
            }
        find2 = 1;
        break;
      }
  }

  /* Modifications in the environment.  */
  if (JUMP_P (BB_END (prev)) && !find && find2 && e)
    {
      e->flags &= ~EDGE_FALLTHRU;
      emit_barrier_after (BB_END (prev));
    }
  if (!JUMP_P (BB_END (prev)) && !find && find2 && e)
    {
      edge new_edge_;
      remove_edge (e);
      emit_jump_insn_after (gen_jump (BB_HEAD (next_bb)), BB_END (prev));
      gain--;
      emit_barrier_after (BB_END (prev));
      new_edge_ = make_edge (prev, next_bb, 0);
      if (new_edge_)
        new_edge_->probability = REG_BR_PROB_BASE;
      JUMP_LABEL (BB_END (prev)) = block_label (next_bb);
      LABEL_NUSES (block_label (next_bb))++;
    }

  for (di = first; di; di = di->next_analogous)
    {
      rtx rinsn = di->moveable_insn;
      di->moveable_insn = NULL;
      remove_insn (rinsn);
      gain++;
    }

  modify_jump_labels (new_bb, first, label);
  return (new_bb);
}

/* If we create a new NEW_BB basic block we must complement it with a
   DECORATOR bbDecorator.  Return with the new bb_decorator.  */

static bb_decorator
insert_new_bb_decorator (bb_decorator decorator, basic_block new_bb)
{
  bb_decorator last_di;
  bb_decorator new_di = NULL;

  last_di = decorator;
  while (last_di->next_decorator)
    last_di = last_di->next_decorator;

  new_di = xcalloc (1, sizeof (struct bb_decorator_def));
  new_di->num_pred_bb = EDGE_COUNT (new_bb->preds);
  new_di->num_succ_bb = EDGE_COUNT (new_bb->succs);
  new_di->curr = new_bb;
  new_di->prev_decorator = last_di;
  last_di->next_decorator = new_di;
  return new_di;
}

/* Main function of local factoring sinking. It returns 1 if there was any
   sinked insn. Otherwise it returns 0.  */

static int
rtl_sinking (int depth, bb_decorator decorator)
{
  int change = 0;
  bb_decorator di = decorator;
  while (di)
    {
      di->last_depth = 0;
      di = di->next_decorator;
    }

  di = decorator;
  while (di)
    {
      if (di->next_sibling)
        {
          bb_decorator di2;
          rtx insn = NULL;
          int insert_bb = 0;
          unsigned long num_of_next_bb = di->num_succ_bb;
          unsigned long num_of_good_bb = 0;
          bb_decorator last_sibling;

          if (di->prev_sibling)
            insert_bb = 1;
          insn = find_first_from_end (di, depth);

          if (!insn)
            {
              di = di->next_sibling;
              continue;
            }

          last_sibling = di;

          /* Finding equal instruction in the concatenated basic blocks.  */
          for (di2 = di->next_sibling; di2; di2 = di2->next_sibling)
            {
              if (find_insn_from_end (insn, di2, depth))
                {
                  last_sibling->next_analogous = di2;
                  di2->prev_analogous = last_sibling;
                  last_sibling = di2;
                  num_of_good_bb++;
                }
              else
                insert_bb = 1;
            }
          /* Cost analizing.  */
          if (num_of_good_bb <= num_of_next_bb)
            {
              di->last_depth++;
              delete_analogous (di);
              continue;
            }

          if (!insert_bb && EDGE_SUCC (di->curr, 0))
            {
              if (num_of_good_bb <
                  EDGE_COUNT ((EDGE_SUCC (di->curr, 0))->dest->preds))
                insert_bb = 1;
            }

          if (!insert_bb)
            {
              sinking_rtl_stmt (di);
              change = 1;
              continue;
            }
          else if (di->num_succ_bb == 1
                   && (EDGE_SUCC (di->curr, 0)->dest != EXIT_BLOCK_PTR))
            {
              basic_block new_bb;
              bb_decorator new_bb_decorator;


              if (optimize_size
                  && find_abnormal_edge (di)
                  && !any_condjump_p (
                    BB_END (EDGE_SUCC (di->curr, 0)->dest->prev_bb)))
                {
                  new_bb = create_basic_block_by_factoring (di);
                  new_bb_decorator =
                    insert_new_bb_decorator (decorator, new_bb);
                  modifying_concatenation (di, new_bb_decorator);
                  change = 1;
                }
            }
          else
            di->last_depth++;

          delete_analogous (di);
        }
      else
        di = di->next_sibling;
    }
  return change;
}

/* The main entry point of loacl factoring algorithms (code hoisting/sinking).
   It returns 1 if there was any code motion. Otherwise it returns 0.
   DEPTH is used to limit the searching depth of the algorithms. ALG describes
   the running algorithm (code hoisting or sinking).  */

int
rtl_lfact (int depth, enum lfact_direction alg)
{
  bb_decorator decorator = NULL;
  int result = 0;
  gain = 0;

  decorator = init_factoring (decorator);

  switch (alg)
    {
    case LFD_HOISTING:
      collect_full_sibling (decorator, alg);
      cost_analyzer (decorator, alg);
      while (rtl_hoisting (depth, decorator))
        result = 1;
      break;
    case LFD_SINKING:
    default:
      collect_sibling (decorator, alg);
      cost_analyzer (decorator, alg);
      while (rtl_sinking (depth, decorator))
        result = 1;
      break;
    }

  free_bb_decorator_list (decorator);

  return (result);
}

/* Returns the first insn preceding INSN for which INSN_P is true and belongs to
   the same basic block. Returns NULL_RTX if no such insn can be found.  */

static rtx
prev_insn_in_block (rtx insn)
{
  while (insn != BB_HEAD (BLOCK_FOR_INSN (insn)))
    {
      insn = PREV_INSN (insn);
      if (INSN_P (insn))
        return insn;
    }
  return NULL_RTX;
}

/* Determines the number of common insns in the sequences ending in INSN1 and
   INSN2.  */

static int
matching_length (rtx insn1, rtx insn2)
{
  rtx x1;
  rtx x2;
  int len;

  x1 = insn1;
  x2 = insn2;
  len = 0;
  while (x1 && x2 && (x1 != insn2) && (x2 != insn1)
         && rtx_equal_p (PATTERN (x1), PATTERN (x2)))
    {
      len++;

      x1 = prev_insn_in_block (x1);
      x2 = prev_insn_in_block (x2);
    }

  return len;
}

/* Adds PINSN as a pattern sequence to PATTERN_SEQS with MINSN as a matching
   sequence.  */

static void
match_seqs (rtx pinsn, rtx minsn)
{
  int len;
  matching_seq mseq;

  /* Determines the number of common insns and return without doing anything
     if it is too small to produce any gain.  */
  len = matching_length (pinsn, minsn);
  if (len <= CALL_COST)
    return;

  /* Prepend a new PATTERN_SEQ to PATTERN_SEQS if the last pattern sequence
     does not end in PINSN. This assumes that once the value of PINSN changes
     the old value will never appear again.  */
  if (!pattern_seqs || pattern_seqs->insn != pinsn)
    {
      pattern_seq pseq =
        (pattern_seq) xmalloc (sizeof (struct pattern_seq_def));
      pseq->insn = pinsn;
      pseq->gain = 0;		/* Set to zero to force recomputing.  */
      pseq->abstracted_length = 0;
      pseq->link_reg = NULL_RTX;
      pseq->matching_seqs = 0;
      pseq->next_pattern_seq = pattern_seqs;
      pattern_seqs = pseq;
    }

  /* Add a new matching sequence to the first pattern sequence. We know that
     it ends in PINSN.  */
  mseq = (matching_seq) xmalloc (sizeof (struct matching_seq_def));
  mseq->insn = minsn;
  mseq->matching_length = len;
  mseq->abstracted_length = 0;
  mseq->next_matching_seq = pattern_seqs->matching_seqs;
  pattern_seqs->matching_seqs = mseq;
}

/* Collects all pattern sequences and their matching sequences and puts them
   into PATTERN_SEQS.  */

static void
collect_pattern_seqs (void)
{
  basic_block pbb, mbb;
#ifdef STACK_REGS

  basic_block bb;
  bitmap_head stack_reg_live;

  /* Extra initialization step to ensure that no stack registers (if present)
     are live across abnormal edges. Set a flag in STACK_REG_LIVE for an insn
     if a stack register is live after the insn.  */
  bitmap_initialize (&stack_reg_live, NULL);

  FOR_EACH_BB (bb)
  {
    regset_head live;
    struct propagate_block_info *pbi;
    rtx insn;

    /* Initialize liveness propagation.  */
    INIT_REG_SET (&live);
    COPY_REG_SET (&live, bb->il.rtl->global_live_at_end);
    pbi = init_propagate_block_info (bb, &live, NULL, NULL, 0);

    /* Propagate liveness info and mark insns where a stack reg is live.  */
    insn = BB_END (bb);
    while (1)
      {
        int reg;
        for (reg = FIRST_STACK_REG; reg <= LAST_STACK_REG; reg++)
          {
            if (REGNO_REG_SET_P (&live, reg))
              {
                bitmap_set_bit (&stack_reg_live, INSN_UID (insn));
                break;
              }
          }

        if (insn == BB_HEAD (bb))
          break;
        insn = propagate_one_insn (pbi, insn);
      }

    /* Free unused data.  */
    CLEAR_REG_SET (&live);
    free_propagate_block_info (pbi);
  }
#endif

  /* Initialize PATTERN_SEQS to empty.  */
  pattern_seqs = 0;

  /* Try to match every abstractable insn in every block with every other
     insn.  */
  FOR_EACH_BB (pbb)
  {
    rtx pinsn;

    /* Skip that bb when its first succ. bb has more incoming edge than
       PARAM_MAX_CROSSJUMP_EDGES. The same checks as in crossjump.  */
    if (EDGE_COUNT (pbb->succs)
        && (EDGE_SUCC (pbb, 0))->dest
        && EDGE_COUNT ((EDGE_SUCC (pbb, 0))->dest->preds) >
        (unsigned long) PARAM_VALUE (PARAM_MAX_CROSSJUMP_EDGES))
      continue;

    for (pinsn = BB_HEAD (pbb);; pinsn = NEXT_INSN (pinsn))
      {
        if (ABSTRACTABLE_INSN_P (pinsn)
#ifdef STACK_REGS
            && !bitmap_bit_p (&stack_reg_live, INSN_UID (pinsn))
#endif
           )
          {

            FOR_EACH_BB (mbb)
            {
              rtx minsn;

              for (minsn = BB_END (mbb);; minsn = PREV_INSN (minsn))
                {
                  if (ABSTRACTABLE_INSN_P (minsn)
#ifdef STACK_REGS
                      && !bitmap_bit_p (&stack_reg_live, INSN_UID (minsn))
#endif
                     )
                    match_seqs (pinsn, minsn);

                  if (minsn == BB_HEAD (mbb))
                    break;
                }
            }

          }

        if (pinsn == BB_END (pbb))
          break;
      }
  }

#ifdef STACK_REGS
  /* Free unused data.  */
  bitmap_clear (&stack_reg_live);
#endif
}

/* Transforms a regset to a HARD_REG_SET. Every hard register in REGS is added
   to hregs. Additionally, the hard counterpart of every renumbered pseudo
   register is also added.  */

static void
renumbered_reg_set_to_hard_reg_set (HARD_REG_SET * hregs, regset regs)
{
  int r;

  REG_SET_TO_HARD_REG_SET (*hregs, regs);
  for (r = FIRST_PSEUDO_REGISTER; r < max_regno; r++)
    if (REGNO_REG_SET_P (regs, r) && reg_renumber[r] >= 0)
      SET_HARD_REG_BIT (*hregs, reg_renumber[r]);
}

/* Clears the bits in REGS for all registers, which are live in the sequence
   give by its last INSN and its LENGTH.  */

static void
clear_regs_live_in_seq (HARD_REG_SET * regs, rtx insn, int length)
{
  basic_block bb;
  regset_head live;
  HARD_REG_SET hlive;
  struct propagate_block_info *pbi;
  rtx x;
  int i;

  /* Initialize liveness propagation.  */
  bb = BLOCK_FOR_INSN (insn);
  INIT_REG_SET (&live);
  COPY_REG_SET (&live, bb->il.rtl->global_live_at_end);
  pbi = init_propagate_block_info (bb, &live, NULL, NULL, 0);

  /* Propagate until INSN if found.  */
  for (x = BB_END (bb); x != insn;)
    x = propagate_one_insn (pbi, x);

  /* Clear registers live after INSN.  */
  renumbered_reg_set_to_hard_reg_set (&hlive, &live);
  AND_COMPL_HARD_REG_SET (*regs, hlive);

  /* Clear registers live in and before the sequence.  */
  for (i = 0; i < length;)
    {
      rtx prev = propagate_one_insn (pbi, x);

      if (INSN_P (x))
        {
          renumbered_reg_set_to_hard_reg_set (&hlive, &live);
          AND_COMPL_HARD_REG_SET (*regs, hlive);
          i++;
        }

      x = prev;
    }

  /* Free unused data.  */
  free_propagate_block_info (pbi);
  CLEAR_REG_SET (&live);
}

/* Computes the gain of turning PSEQ into a pseudo-function and its matching
   sequences into pseudo-calls. Also computes and caches the number of insns to
   abstract from  the matching sequences.
   FIXME: The implementation is overly conservative. Should be improved.  */

static void
recompute_gain_for_pattern_seq (pattern_seq pseq)
{
  matching_seq mseq;
  rtx prev, x;
  int i;
  int hascall;
  HARD_REG_SET linkregs;

  /* Initialize data.  */
  SET_HARD_REG_SET (linkregs);
  pseq->link_reg = NULL_RTX;
  pseq->abstracted_length = 0;

  /* FIXME: Substract 1 from CALL_COST since no call-jump required in the
     pattern sequence.  */
  pseq->gain = -(CALL_COST - 1 + RETURN_COST);

  /* Determine ABSTRACTED_LENGTH for matching sequences of PSEQ.
     ABSTRACTED_LENGTH may be less than MATCHING_LENGTH if sequences in the
     same block overlap.

     PREV holds the last insn of the previous matching sequence. To be useful
     this assumes that matching sequences from the same block are in reverse
     insn order.  */
  prev = NULL_RTX;
  for (mseq = pseq->matching_seqs; mseq; mseq = mseq->next_matching_seq)
    {
      /* Determine ABSTRACTED_LENGTH.  */
      x = mseq->insn;
      for (i = 0; (i < mseq->matching_length) && (x != prev); i++)
        x = prev_insn_in_block (x);
      mseq->abstracted_length = i;

      /* If ABSTRACTED_LENGTH is big enough registers live in this matching
         sequence should not be used as a link register. Also set
         ABSTRACTED_LENGTH of PSEQ.  */
      if (mseq->abstracted_length > CALL_COST)
        {
          clear_regs_live_in_seq (&linkregs, mseq->insn,
                                  mseq->abstracted_length);
          if (mseq->abstracted_length > pseq->abstracted_length)
            pseq->abstracted_length = mseq->abstracted_length;

          prev = mseq->insn;
        }
    }

  /* Modify ABSTRACTED_LENGTH of PSEQ if pattern sequence overlaps with one
     of the matching sequences.  */
  for (mseq = pseq->matching_seqs; mseq; mseq = mseq->next_matching_seq)
    {
      x = pseq->insn;
      for (i = 0; (i < pseq->abstracted_length) && (x != mseq->insn); i++)
        x = prev_insn_in_block (x);
      pseq->abstracted_length = i;
    }
  /* No gain if ABSTRACTED_LENGTH is too small.  */
  if (pseq->abstracted_length <= CALL_COST)
    return;

  /* Ensure that no matching sequence is longer than the pattern sequence.  */
  for (mseq = pseq->matching_seqs; mseq; mseq = mseq->next_matching_seq)
    {
      if (mseq->abstracted_length > pseq->abstracted_length)
        mseq->abstracted_length = pseq->abstracted_length;

      /* Once the length is stabilizing the gain can be calculated.  */
      if (mseq->abstracted_length > CALL_COST)
        pseq->gain += mseq->abstracted_length - CALL_COST;
    }
  /* No need to do further work if there is no gain.  */
  if (pseq->gain <= 0)
    return;

  /* Should not use registers live in the pattern sequence as link register.
   */
  clear_regs_live_in_seq (&linkregs, pseq->insn, pseq->abstracted_length);

  /* Determine whether pattern sequence contains a call_insn.  */
  hascall = 0;
  x = pseq->insn;
  for (i = 0; i < pseq->abstracted_length; i++)
    {
      if (CALL_P (x))
        {
          hascall = 1;
          break;
        }
      x = prev_insn_in_block (x);
    }

  /* Should not use a register as a link register if - it is a fixed
     register, or - the sequence contains a call insn and the register is a
     call used register, or - the register needs to be saved if used in a
     function but was not used before (since saving it can invalidate already
     computed frame pointer offsets), or - the register cannot be used as a
     base register.  */

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (fixed_regs[i]
        || (hascall && call_used_regs[i])
        || (!call_used_regs[i] && !regs_ever_live[i])
        || !REGNO_MODE_OK_FOR_BASE_P (i, Pmode)
        || (!reg_class_subset_p (REGNO_REG_CLASS (i), BASE_REG_CLASS)))
      CLEAR_HARD_REG_BIT (linkregs, i);

  /* Find an appropriate register to be used as the link register.  */
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (TEST_HARD_REG_BIT (linkregs, i))
        {
          pseq->link_reg = gen_rtx_REG (Pmode, i);
          break;
        }
    }

  /* Abstraction is not possible if no link register is available, so set
     gain to 0.  */
  if (!pseq->link_reg)
    pseq->gain = 0;
}

/* Deallocates memory occupied by PSEQ and its matching seqs.  */

static void
free_pattern_seq (pattern_seq pseq)
{
  while (pseq->matching_seqs)
    {
      matching_seq mseq = pseq->matching_seqs;
      pseq->matching_seqs = mseq->next_matching_seq;
      free (mseq);
    }
  free (pseq);
}


/* Computes the gain for pattern sequences. Pattern sequences producing no gain
   are deleted. The pattern sequence with the biggest gain is moved to the first
   place of PATTERN_SEQS.  */

static void
recompute_gain (void)
{
  pattern_seq *pseq;
  int maxgain;

  maxgain = 0;
  for (pseq = &pattern_seqs; *pseq;)
    {
      if ((*pseq)->gain <= 0)
        recompute_gain_for_pattern_seq (*pseq);

      if ((*pseq)->gain > 0)
        {
          if ((*pseq)->gain > maxgain)
            {
              pattern_seq temp = *pseq;
              (*pseq) = temp->next_pattern_seq;
              temp->next_pattern_seq = pattern_seqs;
              pattern_seqs = temp;
              maxgain = pattern_seqs->gain;
            }
          else
            {
              pseq = &(*pseq)->next_pattern_seq;
            }
        }
      else
        {
          pattern_seq temp = *pseq;
          *pseq = temp->next_pattern_seq;
          free_pattern_seq (temp);
        }
    }
}

/* Updated those pattern sequences and matching sequences, which overlap with
   the sequence given by INSN and LEN. Deletes sequences shrinking below a
   limit.
   FIXME: Should be improved.  */

static void
erase_from_pattern_seqs (rtx insn, int len)
{
  pattern_seq *pseq;
  matching_seq *mseq;
  rtx x;
  int plen, mlen;

  while (len > 0)
    {
      for (pseq = &pattern_seqs; *pseq;)
        {
          plen = 0;
          for (x = (*pseq)->insn; x && (x != insn);
               x = prev_insn_in_block (x))
            plen++;

          if (plen <= CALL_COST)
            {
              pattern_seq temp = *pseq;
              *pseq = temp->next_pattern_seq;
              free_pattern_seq (temp);
            }
          else
            {
              for (mseq = &(*pseq)->matching_seqs; *mseq;)
                {
                  mlen = 0;
                  for (x = (*mseq)->insn;
                       x && (x != insn) && (mlen < plen)
                       && (mlen < (*mseq)->matching_length);
                       x = prev_insn_in_block (x))
                    mlen++;

                  if (mlen <= CALL_COST)
                    {
                      matching_seq temp = *mseq;
                      *mseq = temp->next_matching_seq;
                      free (temp);
                      /* Set to 0 to force gain recomputation.  */
                      (*pseq)->gain = 0;
                    }
                  else
                    {
                      if (mlen < (*mseq)->matching_length)
                        {
                          (*mseq)->matching_length = mlen;
                          /* Set to 0 to force gain recomputation.  */
                          (*pseq)->gain = 0;
                        }
                      mseq = &(*mseq)->next_matching_seq;
                    }
                }

              pseq = &(*pseq)->next_pattern_seq;
            }
        }

      len--;
      insn = prev_insn_in_block (insn);
    }
}

/* Updates those pattern sequences and matching sequences, which overlap with
   the pattern sequence with the biggest gain and its matching sequences.  */

static void
update_pattern_seqs (void)
{
  pattern_seq bestpseq;
  matching_seq mseq;

  bestpseq = pattern_seqs;
  pattern_seqs = bestpseq->next_pattern_seq;

  for (mseq = bestpseq->matching_seqs; mseq; mseq = mseq->next_matching_seq)
    if (mseq->abstracted_length > CALL_COST)
      erase_from_pattern_seqs (mseq->insn, mseq->abstracted_length);
  erase_from_pattern_seqs (bestpseq->insn, bestpseq->abstracted_length);

  bestpseq->next_pattern_seq = pattern_seqs;
  pattern_seqs = bestpseq;
}

/* Groups together those matching sequences of the best pattern sequence, which
   have the same ABSTRACTED_LENGTH and puts these groups in ascending order.
   SEQ_BLOCKS contains the result.  */

static void
determine_seq_blocks (void)
{
  seq_block sb;
  matching_seq *mseq;
  matching_seq m;

  /* Initialize SEQ_BLOCKS to empty.  */
  seq_blocks = 0;

  /* Process all matching sequences.  */
  for (mseq = &pattern_seqs->matching_seqs; *mseq;)
    {
      /* Deal only with matching sequences being long enough. */
      if ((*mseq)->abstracted_length <= CALL_COST)
        {
          mseq = &(*mseq)->next_matching_seq;
          continue;
        }

      /* Ensure that SB contains a seq_block with the appropriate length.
         Insert a new seq_block if neccessary.  */
      if (!seq_blocks || ((*mseq)->abstracted_length < seq_blocks->length))
        {
          sb = (seq_block) xmalloc (sizeof (struct seq_block_def));
          sb->length = (*mseq)->abstracted_length;
          sb->label = NULL_RTX;
          sb->matching_seqs = 0;
          sb->next_seq_block = seq_blocks;
          seq_blocks = sb;
        }
      else
        {
          for (sb = seq_blocks; sb; sb = sb->next_seq_block)
            {
              if ((*mseq)->abstracted_length == sb->length)
                break;
              if (!sb->next_seq_block
                  || ((*mseq)->abstracted_length <
                      sb->next_seq_block->length))
                {
                  seq_block temp =
                    (seq_block) xmalloc (sizeof (struct seq_block_def));
                  temp->length = (*mseq)->abstracted_length;
                  temp->label = NULL_RTX;
                  temp->matching_seqs = 0;
                  temp->next_seq_block = sb->next_seq_block;
                  sb->next_seq_block = temp;
                }
            }
        }

      /* Remove the matching sequence from the linked list of the pattern
         sequence and link it to SB.  */
      m = *mseq;
      *mseq = m->next_matching_seq;
      m->next_matching_seq = sb->matching_seqs;
      sb->matching_seqs = m;
    }
}

/* Builds a symbol_ref for LABEL.  */

static rtx
gen_symbol_ref_rtx_for_label (rtx label)
{
  char name[20];
  rtx sym;

  ASM_GENERATE_INTERNAL_LABEL (name, "L", CODE_LABEL_NUMBER (label));
  sym = gen_rtx_SYMBOL_REF (Pmode, ggc_strdup (name));
  SYMBOL_REF_FLAGS (sym) = SYMBOL_FLAG_LOCAL;
  return sym;
}

/* Ensures that INSN is the last insn in its block and returns the block label
   of the next block.  */

static rtx
block_label_after (rtx insn)
{
  basic_block bb = BLOCK_FOR_INSN (insn);
  if ((insn == BB_END (bb)) && (bb->next_bb != EXIT_BLOCK_PTR))
    return block_label (bb->next_bb);
  else
    return block_label (split_block (bb, insn)->dest);
}

/* Ensures that the last insns of the best pattern and its matching sequences
   are the last insns in their block. Additionally, extends the live set at the
   end of the pattern sequence with the live sets at the end of the matching
   sequences.  */

static void
split_blocks_after_seqs (void)
{
  seq_block sb;
  matching_seq mseq;

  block_label_after (pattern_seqs->insn);
  for (sb = seq_blocks; sb; sb = sb->next_seq_block)
    {
      for (mseq = sb->matching_seqs; mseq; mseq = mseq->next_matching_seq)
        {
          block_label_after (mseq->insn);
          IOR_REG_SET (BLOCK_FOR_INSN (pattern_seqs->insn)->
                       il.rtl->global_live_at_end,
                       BLOCK_FOR_INSN (mseq->insn)->il.rtl->global_live_at_end);
        }
    }
}

/* Splits the best pattern sequence accoring to SEQ_BLOCKS. Emits pseudo-call
   and -return insns before and after the sequence.  */

static void
split_pattern_seq (void)
{
  rtx insn;
  basic_block bb;
  rtx retlabel, retjmp, saveinsn;
  int i;
  seq_block sb;

  insn = pattern_seqs->insn;
  bb = BLOCK_FOR_INSN (insn);

  /* Get the label after the sequence. This will be the return address. The
     label will be referenced using a symbol_ref so protect it from
     deleting.  */
  retlabel = block_label_after (insn);
  LABEL_PRESERVE_P (retlabel) = 1;

  /* Emit an indirect jump via the link register after the sequence acting
     as the return insn.  Also emit a barrier and update the basic block.  */
  retjmp = emit_jump_insn_after (gen_indirect_jump (pattern_seqs->link_reg),
                                 BB_END (bb));
  emit_barrier_after (BB_END (bb));

  /* Replace all outgoing edges with a new one to the block of RETLABEL.  */
  while (EDGE_COUNT (bb->succs) != 0)
    remove_edge (EDGE_SUCC (bb, 0));
  make_edge (bb, BLOCK_FOR_INSN (retlabel), EDGE_ABNORMAL);

  /* Split the sequence according to SEQ_BLOCKS and cache the label of the
     resulting basic blocks.  */
  i = 0;
  for (sb = seq_blocks; sb; sb = sb->next_seq_block)
    {
      for (; i < sb->length; i++)
        insn = prev_insn_in_block (insn);

      sb->label = block_label (split_block (bb, insn)->dest);
    }

  /* Emit an insn saving the return address to the link register before the
     sequence.  */
  saveinsn = emit_insn_after (gen_move_insn (pattern_seqs->link_reg,
                              gen_symbol_ref_rtx_for_label
                              (retlabel)), BB_END (bb));
  /* Update liveness info.  */
  SET_REGNO_REG_SET (bb->il.rtl->global_live_at_end,
                     REGNO (pattern_seqs->link_reg));
}

/* Deletes the insns of the matching sequences of the best pattern sequence and
   replaces them with pseudo-calls to the pattern sequence.  */

static void
erase_matching_seqs (void)
{
  seq_block sb;
  matching_seq mseq;
  rtx insn;
  basic_block bb;
  rtx retlabel, saveinsn, callinsn;
  int i;

  for (sb = seq_blocks; sb; sb = sb->next_seq_block)
    {
      for (mseq = sb->matching_seqs; mseq; mseq = mseq->next_matching_seq)
        {
          insn = mseq->insn;
          bb = BLOCK_FOR_INSN (insn);

          /* Get the label after the sequence. This will be the return
             address. The label will be referenced using a symbol_ref so
             protect it from deleting.  */
          retlabel = block_label_after (insn);
          LABEL_PRESERVE_P (retlabel) = 1;

          /* Delete the insns of the sequence.  */
          for (i = 0; i < sb->length; i++)
            insn = prev_insn_in_block (insn);
          delete_basic_block (split_block (bb, insn)->dest);

          /* Emit an insn saving the return address to the link register
             before the deleted sequence.  */
          saveinsn = emit_insn_after (gen_move_insn (pattern_seqs->link_reg,
                                      gen_symbol_ref_rtx_for_label
                                      (retlabel)),
                                      BB_END (bb));
          BLOCK_FOR_INSN (saveinsn) = bb;

          /* Emit a jump to the appropriate part of the pattern sequence
             after the save insn. Also update the basic block.  */
          callinsn = emit_jump_insn_after (gen_jump (sb->label), saveinsn);
          JUMP_LABEL (callinsn) = sb->label;
          LABEL_NUSES (sb->label)++;
          BLOCK_FOR_INSN (callinsn) = bb;
          BB_END (bb) = callinsn;

          /* Maintain control flow and liveness information.  */
          SET_REGNO_REG_SET (bb->il.rtl->global_live_at_end,
                             REGNO (pattern_seqs->link_reg));
          emit_barrier_after (BB_END (bb));
          make_single_succ_edge (bb, BLOCK_FOR_INSN (sb->label), 0);
          IOR_REG_SET (bb->il.rtl->global_live_at_end,
            BLOCK_FOR_INSN (sb->label)->il.rtl->global_live_at_start);

          make_edge (BLOCK_FOR_INSN (seq_blocks->label),
                     BLOCK_FOR_INSN (retlabel), EDGE_ABNORMAL);
        }
    }
}

/* Deallocates SEQ_BLOCKS and all the matching sequences.  */

static void
free_seq_blocks (void)
{
  while (seq_blocks)
    {
      seq_block sb = seq_blocks;
      while (sb->matching_seqs)
        {
          matching_seq mseq = sb->matching_seqs;
          sb->matching_seqs = mseq->next_matching_seq;
          free (mseq);
        }
      seq_blocks = sb->next_seq_block;
      free (sb);
    }
}

/* Transforms the best pattern sequence into a pseudo-function and its matching
   sequences to pseudo-calls. Afterwards the best pattern sequence is removed
   from PATTERN_SEQS.  */

static void
abstract_best_seq (void)
{
  pattern_seq bestpseq;

  /* Do the abstraction.  */
  determine_seq_blocks ();
  split_blocks_after_seqs ();
  split_pattern_seq ();
  erase_matching_seqs ();
  free_seq_blocks ();

  /* Record the usage of the link register.  */
  regs_ever_live[REGNO (pattern_seqs->link_reg)] = 1;

  /* Remove the best pattern sequence.  */
  bestpseq = pattern_seqs;
  pattern_seqs = bestpseq->next_pattern_seq;
  free_pattern_seq (bestpseq);
}

/* Prints info on the pattern sequences to the dump file.  */

static void
dump_pattern_seqs (void)
{
  pattern_seq pseq;
  matching_seq mseq;

  if (!dump_file)
    return;

  fprintf (dump_file, ";; Pattern sequences\n");
  for (pseq = pattern_seqs; pseq; pseq = pseq->next_pattern_seq)
    {
      fprintf (dump_file, "Pattern sequence at insn %d matches sequences at",
               INSN_UID (pseq->insn));
      for (mseq = pseq->matching_seqs; mseq; mseq = mseq->next_matching_seq)
        {
          fprintf (dump_file, " insn %d (length %d)", INSN_UID (mseq->insn),
                   mseq->matching_length);
          if (mseq->next_matching_seq)
            fprintf (dump_file, ",");
        }
      fprintf (dump_file, ".\n");
    }
  fprintf (dump_file, "\n");
}

/* Prints info on the best pattern sequence transformed in the ITER-th
   iteration to the dump file.  */

static void
dump_best_pattern_seq (int iter)
{
  matching_seq mseq;

  if (!dump_file)
    return;

  fprintf (dump_file, ";; Iteration %d\n", iter);
  fprintf (dump_file,
           "Best pattern sequence with %d gain is at insn %d (length %d).\n",
           pattern_seqs->gain, INSN_UID (pattern_seqs->insn),
           pattern_seqs->abstracted_length);
  fprintf (dump_file, "Matching sequences are at");
  for (mseq = pattern_seqs->matching_seqs; mseq;
       mseq = mseq->next_matching_seq)
    {
      fprintf (dump_file, " insn %d (length %d)", INSN_UID (mseq->insn),
               mseq->abstracted_length);
      if (mseq->next_matching_seq)
        fprintf (dump_file, ",");
    }
  fprintf (dump_file, ".\n");
  fprintf (dump_file, "Using reg %d as link register.\n\n",
           REGNO (pattern_seqs->link_reg));
}

/* Finds equivalent insn sequences in the current function and retains only one
   instance of them which is turned into a pseudo-function. The additional
   copies are erased and replaced by pseudo-calls to the retained sequence.  */

void
rtl_seqabstr (void)
{
  int iter;

  /* Build an initial set of pattern sequences from the current function.  */
  collect_pattern_seqs ();
  dump_pattern_seqs ();

  /* Iterate until there are sequences to abstract.  */
  for (iter = 1;; iter++)
    {
      /* Recompute gain for sequences if neccessary and select sequence with
         biggest gain.  */
      recompute_gain ();
      if (!pattern_seqs)
        break;
      dump_best_pattern_seq (iter);
      /* Update the cached info of the other sequences and force gain
         recomputation where needed.  */
      update_pattern_seqs ();
      /* Turn best sequences into pseudo-functions and -calls.  */
      abstract_best_seq ();
    }
}

static bool
gate_rtl_lfact (void)
{
  return flag_rtl_lfact;
}

static void
rest_of_rtl_lfact (void)
{
  rtl_lfact (0, LFD_HOISTING);
  rtl_lfact (0, LFD_SINKING);

  cleanup_cfg (CLEANUP_EXPENSIVE);
}

struct tree_opt_pass pass_rtl_lfact = {
  "lfact",                              /* name */
  gate_rtl_lfact,                       /* gate */
  rest_of_rtl_lfact,                    /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_FACTORING,                         /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
  TODO_ggc_collect,                     /* todo_flags_finish */
  'q'                                   /* letter */
};

static bool
gate_rtl_seqabstr (void)
{
  return flag_rtl_seqabstr;
}

static void
rest_of_rtl_seqabstr (void)
{
  life_analysis (dump_file, PROP_DEATH_NOTES |
                            PROP_SCAN_DEAD_CODE |
                            PROP_KILL_DEAD_CODE);

  cleanup_cfg (CLEANUP_EXPENSIVE |
               CLEANUP_UPDATE_LIFE |
               (flag_crossjumping ? CLEANUP_CROSSJUMP : 0));

  /* Abstract out common insn sequences. */
  rtl_seqabstr ();

  cleanup_cfg (CLEANUP_EXPENSIVE |
               CLEANUP_UPDATE_LIFE |
               (flag_crossjumping ? CLEANUP_CROSSJUMP : 0));
}

struct tree_opt_pass pass_rtl_seqabstr = {
  "seqabstr",                           /* name */
  gate_rtl_seqabstr,                    /* gate */
  rest_of_rtl_seqabstr,                 /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_SEQABSTR,                          /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
  TODO_ggc_collect,                     /* todo_flags_finish */
  'Q'                                   /* letter */
};
