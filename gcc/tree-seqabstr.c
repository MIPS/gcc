/* Sequence abstraction on tree.
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
#include "ggc.h"
#include "flags.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "langhooks.h"
#include "tree-pass.h"
#include "tree-ssa-operands.h"
#include "fact-common.h"
#include "expr.h"

/* The MIN_LENGTH is the minimum length of abstractable sequences.  */
#define MIN_LENGTH	12

/* The MIN_COST is the minimum number of tree's leafs.  */
#define MIN_COST	60

/* The cost of the jump instruction. */
#define SEQ_COST	2

/* Fixed cost value which is used by the gain computation.  */
#define FIXED_COST	1

/* Set OBJ to the first element of the container list.  */
#define REWIND(obj)	for(;obj && obj->prev;obj = obj->prev)

/* Enum for the overlaping sequences.  */
enum remove_type
{
  NONE,
  REMOVE_SELECTED,
  REMOVE_FIRST,
  REMOVE_SECOND
};

/* The sequence structure.  */
typedef struct defSequence
{
  /* The end of sequence.  */
  block_stmt_iterator ending;

  /* The distance of the end of sequence from basic_block ending.  */
  int index;

  /* The length of sequence.  */
  int length;

  /* The cost of sequence which is computed from the insns leafs.  */
  int cost;

  /* The new entry point of sequence.  */
  tree t_entry;

  /* The old starting point of sequence.  */
  tree t_starting;

  /* The old ending point of sequence.  */
  tree t_ending;

  /* The return position of the new sequence.  */
  tree t_return;

  /* The return address saving insn.  */
  tree t_storing;

  /* Chain iterators.  */
  struct defSequence *next;
  struct defSequence *prev;
} *pSequence;

/* The sequences list structure. */
typedef struct defSeqList
{
  /* The element of sequence list.  */
  pSequence seq;

  /* Chain iterators.  */
  struct defSeqList *next;
  struct defSeqList *prev;
} *pSeqList;

/* The container of the abstractable sequences.  */
typedef struct defAllAbstrSeq
{
  /* The element of all sequence list.  */
  pSeqList abstrseq;

  /* The gain of sequence abstraction.  */
  int gain;

  /* Non zero if the element is not going to abstract.  */
  int removed;

  /* Chain iterators.  */
  struct defAllAbstrSeq *next;
  struct defAllAbstrSeq *prev;
} *pAllAbstrSeq;

typedef pSeqList *pBlockList;

static pSeqList add_sequence (block_stmt_iterator, int, int);

/*  Return true if {s,e]i0 and {s,e]i1 overlap each other.  */
#define OVER(si0,ei0,si1,ei1)     \
  ((si0 <= si1 && si1 <= ei0) ||  \
   (si1 <= si0 && si0 <= ei1))

/* The all_sequence conatins all possibly sequences for abstraction.  */
pSequence all_sequence;

/* The all_abstr_seq conatins the list of sequences list for abstraction.  */
pAllAbstrSeq all_abstr_seq;

/* The has_gain is 1 if there was any sequence abstrtaction.  */
int has_gain;

/* Initalize the lists.  */
static void
init_seqabstr (void)
{
  all_sequence = NULL;
  all_abstr_seq = NULL;
}

/* Free the local lists.  */
static void
cleanup_seqabstr (void)
{
  pAllAbstrSeq all;
  pSeqList abstr;
  pSequence seq;

  while (all_sequence)
    {
      seq = all_sequence;
      all_sequence = seq->next;
      free (seq);
    }

  while (all_abstr_seq)
    {
      all = all_abstr_seq;
      all_abstr_seq = all->next;
      while (all->abstrseq)
        {
          abstr = all->abstrseq;
          all->abstrseq = abstr->next;
          free (abstr);
        }
      free (all);
    }
}

/* Add a sequence which start from BSI into LIST list. The length of sequence
   is LEN and the cost of it is COST.  It returns the generated pSequence
   structure of the seuence.  */
static pSequence
addto_sequence (pSequence * list, block_stmt_iterator bsi, int len, int cost)
{
  block_stmt_iterator it;
  pSequence *seq;
  pSequence last;
  int i;

  seq = list;
  last = NULL;
  while (*seq)
    {
      last = *seq;
      seq = &((*seq)->next);
    }

  (*seq) = (pSequence) xcalloc (1, sizeof (struct defSequence));
  (*seq)->next = NULL;
  (*seq)->prev = last;
  (*seq)->ending = bsi;
  (*seq)->length = len;
  (*seq)->cost = cost;
  (*seq)->index = -1;
  (*seq)->t_entry = NULL;
  (*seq)->t_return = NULL;
  (*seq)->t_storing = NULL;
  (*seq)->t_ending = bsi_stmt (bsi);
  it = bsi;
  for (i = 0; i < len - 1; i++)
    bsi_prev (&it);
  (*seq)->t_starting = bsi_stmt (it);

  return *seq;
}

/* It returns a sequence which start from BSI with LEN length and COST cost if
   all_sequence contains it. Otherwise It creates a new sequence and returns
   with it.  */
static pSequence
access_sequence (block_stmt_iterator bsi, int len, int cost)
{
  pSequence seq;

  for (seq = all_sequence;
       seq && (bsi_stmt (bsi) != bsi_stmt (seq->ending)
               || len != seq->length); seq = seq->next);

  if (!seq)
    seq = addto_sequence (&all_sequence, bsi, len, cost);

  return seq;
}

/* The addto_seqlist inserts SEQ sequence into LIST list and returns with the
   sequence list.  */
static pSeqList
addto_seqlist (pSeqList * list, pSequence seq)
{
  pSeqList *seqlist;
  pSeqList last;

  seqlist = list;
  last = NULL;
  while (*seqlist)
    {
      last = *seqlist;
      seqlist = &((*seqlist)->next);
    }

  (*seqlist) = (pSeqList) xcalloc (1, sizeof (struct defSeqList));
  (*seqlist)->next = NULL;
  (*seqlist)->prev = last;
  (*seqlist)->seq = seq;

  return *seqlist;
}

/* The add_seq_pair insert sequence  which start from BSI with LEN length and
   COST cost into LIST list. The LIST contains the sequences which have same
   suffix. The function returns with the modified LIST.  */
static pSeqList
add_seq_pair (pSeqList * list, block_stmt_iterator bsi, int len, int cost)
{
  pSequence seq;

  if (!(*list))
    return add_sequence (bsi, len, cost);

  seq = access_sequence (bsi, len, cost);
  addto_seqlist (list, seq);

  return *list;
}

/* The addto_allabstrseq inserts SEQLIST sequence list into the global container
   of the abstractable sequences. It returns the modified list container.  */
static pAllAbstrSeq
addto_allabstrseq (pAllAbstrSeq * list, pSeqList seqlist)
{
  pAllAbstrSeq *abstrlist;
  pAllAbstrSeq last;

  abstrlist = list;
  last = NULL;
  while (*abstrlist)
    {
      last = *abstrlist;
      abstrlist = &((*abstrlist)->next);
    }

  (*abstrlist) = (pAllAbstrSeq) xcalloc (1, sizeof (struct defAllAbstrSeq));
  (*abstrlist)->next = NULL;
  (*abstrlist)->prev = last;
  (*abstrlist)->abstrseq = seqlist;
  (*abstrlist)->gain = 0;
  (*abstrlist)->removed = 0;

  return *abstrlist;
}

/* Create a new sequence list for the sequence which start from BSI with LEN
   length and COST cost.  It returns the new sequence list.  */
static pSeqList
add_sequence (block_stmt_iterator bsi, int len, int cost)
{
  pSeqList seqlist;
  pSequence seq;
  seqlist = NULL;
  seq = access_sequence (bsi, len, cost);
  addto_seqlist (&seqlist, seq);
  addto_allabstrseq (&all_abstr_seq, seqlist);

  return seqlist;
}

/* Return true if 'p1' and 'p2' is equal for abstraction.  */
static int
stmt_equal (tree p1, tree p2)
{
  if (p1 == NULL || p2 == NULL)
    return 0;
  if (TREE_CODE (p1) != TREE_CODE (p2)
      || TREE_TYPE (p1) != TREE_TYPE (p2)
      || TREE_CODE (p1) == LABEL_EXPR
      || TREE_CODE (p1) == GOTO_EXPR)
    return 0;

  if (stmt_ends_bb_p (p1) || stmt_ends_bb_p (p2))
    return 0;

  return simple_cst_equal (p1, p2);
}

/* This function computes the leafs of the P sub-tree. It is only
   counting the leafs of expression nodes.  */
static int
compute_node_length (tree p)
{
  int cost;
  if (!p)
    return 0;

  cost = 0;
  if (EXPR_P (p))
    {
      int i, op_cost;
      cost = 1;
      for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (p)); i++)
        {
          op_cost = compute_node_length (TREE_OPERAND (p, i));
          if (!op_cost)
            op_cost = 1;
          cost += op_cost;
        }
    }

  return cost;
}

/* This function determines that two sequence which start from SI0, and SI1
   are possible candidates for abstraction.  If the SEQLIST list is not NULL
   the algorithm will use SI1 and the last element of the SEQLIST for searching
   for candidate. The SEQ_LENGTH sequence length and the SEQ_COST sequence
   cost are the length and cost of the sequence which is start from SI1.  */
static void
matching_in_seqlist (pSeqList seqlist, block_stmt_iterator si0,
                     block_stmt_iterator si1, int *seq_length, int *seq_cost)
{
  int length;
  int cost;
  block_stmt_iterator bsi0;
  block_stmt_iterator bsi1;

  length = 0;
  cost = 0;
  if (!seqlist)
    {
      /* When no candidate.  */
      bsi0 = si0;
      bsi1 = si1;

      /* Compute the sequence length, and measure the cost of it.  */
      while (!bsi_end_p (bsi0)
             && !bsi_end_p (bsi1)
             && stmt_equal (bsi_stmt (bsi0), bsi_stmt (bsi1)))
        {
          cost += compute_node_length (bsi_stmt (bsi0));
          bsi_prev (&bsi0);
          bsi_prev (&bsi1);
          length++;
        }
    }
  else
    {
      /* When there are candidates also.  */
      int len, co;
      pSeqList sl;

      for (sl = seqlist; sl; sl = sl->next)
        {
          bsi0 = sl->seq->ending;
          bsi1 = si1;
          len = 0;
          co = 0;

          /* Compute the sequence length, and measure the cost of it.  */
          while (!bsi_end_p (bsi0)
                 && !bsi_end_p (bsi1)
                 && stmt_equal (bsi_stmt (bsi0), bsi_stmt (bsi1)))
            {
              len++;
              co += compute_node_length (bsi_stmt (bsi0));
              bsi_prev (&bsi0);
              bsi_prev (&bsi1);
            }

          /* Expand the length of last element of the SEQLIST if the current
             candidate is longer than the last determined.  */
          if (sl->seq->length < len)
            {
              block_stmt_iterator it;
              int i;
              it = sl->seq->ending;
              for (i = 0; i < len - 1; i++)
                bsi_prev (&it);
              sl->seq->t_starting = bsi_stmt (it);
              sl->seq->length = len;
              sl->seq->cost = co;
            }

          if (length < len)
            {
              length = len;
              cost = co;
            }
        }
    }

  /* Returned values.  */
  *seq_length = length;
  *seq_cost = cost;
}

/* Remove overlaping sequences from SL sequence list.  */
static void
clean_sequence_list (pSeqList sl)
{
  pSeqList it0, it1;
  int no;
  no = 0;
  /* Compare all candidate against each other.  */
  for (it0 = sl; it0; it0 = it0->next)
    {
      if (!it0->seq)
        continue;
      for (it1 = it0->next; it1; it1 = it1->next)
        {
          if (!it1->seq)
            continue;
          /* If IT0 or IT1 is overlaping sequence it will be removed from
             the seuqnce list.  */
          if (it0->seq->ending.bb == it1->seq->ending.bb
              && OVER (it0->seq->index, it0->seq->index + it0->seq->length,
                       it1->seq->index, it1->seq->index + it1->seq->length))
            {
              if (it0->seq->length < it1->seq->length)
                it0->seq = NULL;
              else
                it1->seq = NULL;
            }
          if (!it0->seq)
            break;
        }
    }
  /* Search for removed sequences.  */
  for (it0 = sl; it0;)
    {
      /* Unlink it.  */
      if (!it0->seq)
        {
          it1 = it0;
          if (it0->next)
            it0->next->prev = it0->prev;

          if (it0->prev)
            {
              it0->prev->next = it0->next;
              it0 = it0->next;
            }
          else
            {
              /* Redirect global all_abstr_seq.  */
              pAllAbstrSeq all;
              for (all = all_abstr_seq; all; all = all->next)
                {
                  if (all->abstrseq == sl)
                    {
                      all->abstrseq = all->abstrseq->next;
                      sl = all->abstrseq;
                      it0 = all->abstrseq;
                      break;
                    }
                }
            }
          free (it1);
        }
      else
        it0 = it0->next;
    }

  /*  Mark it with REMOVED flag if SL sequence list only has one element.  */
  if (sl && !sl->next)
    {
      pAllAbstrSeq all;
      for (all = all_abstr_seq; all; all = all->next)
        {
          if (all->abstrseq == sl)
            {
              all->removed = 1;
              break;
            }
        }
    }
}

/* The find_sequences collects all abstractable sequences. */
static void
find_sequences (void)
{
  basic_block bb0, bb1;
  block_stmt_iterator si0, si1;
  block_stmt_iterator si0_t, si1_t;
  int length, cost;
  pSeqList seqlist;

  int bb0_i, bb1_i;

  FOR_EACH_BB (bb0)
  {
    bb0_i = 0;
    /* The algorithm searches for identicall suffixes of sequences.  */
    for (si0 = bsi_last (bb0); !bsi_end_p (si0); bsi_prev (&si0))
      {
        seqlist = NULL;
        FOR_EACH_BB (bb1)
        {
          bb1_i = 0;
          for (si1 = bsi_last (bb1); !bsi_end_p (si1); bsi_prev (&si1))
            {
              if (bsi_stmt (si0) == bsi_stmt (si1))
                continue;
              si0_t = si0;
              si1_t = si1;
              /* Determine if there is a sequence pair at the position of
                 SI0 and SI1.  */
              matching_in_seqlist (seqlist, si0_t, si1_t, &length, &cost);

              /* Cost evaulation method (currently this works in a very simple
                 way):
                  - There is a minimum length restriction (MIN_LENGTH)
                    The LENGTH is the number of common insn in the sequences.
                  - There is a minimum cost restriction (MIN_COST)
                    The COST is the expression leafs of the sequences. */
              if (cost >= MIN_COST && length >= MIN_LENGTH)
                {
                  pSequence s;
                  if (seqlist == NULL)
                    {
                      /* If there isn't any common sequence which is started
                         from this position add it into sequences list.  */
                      seqlist = add_sequence (si0, length, cost);
                      s = access_sequence (si0, length, cost);
                      s->index = bb0_i;
                    }
                  /* Add common sequence pair into global list.  */
                  add_seq_pair (&seqlist, si1, length, cost);
                  s = access_sequence (si1, length, cost);
                  s->index = bb1_i;
                }
              bb1_i++;
            }
        }
        clean_sequence_list (seqlist);
        bb0_i++;
      }
  }
}

/* The compute_gain computes some type of gain for ABSTRSEQ sequence which is
   used for comparing.  */
static int
compute_gain (pAllAbstrSeq * abstrseq)
{
  int gain, maxlen;
  pSeqList sl;

  /* Do not recount gain.  */
  if ((*abstrseq)->gain)
    return (*abstrseq)->gain;

  sl = (*abstrseq)->abstrseq;
  maxlen = 0;
  gain = 0;

  /* The gain is computed in the following way:
      - each sequence which is different from the longest sequence
        is a gain
      - for every sequence the cost of the abtraction is constant
      - and there is a global cost of abtraction

     So the formula is something like that:
     GAIN = (cost of all sequences) - (cost of longest sequence)
            - (number of sequences) * (SEQ_COST)
            - (FIXED_COST).  */
  REWIND (sl);
  for (; sl; sl = sl->next)
    {
      if (maxlen < sl->seq->cost)
        maxlen = sl->seq->cost;
      gain += sl->seq->cost;
      gain -= SEQ_COST;
    }
  gain -= maxlen;
  gain -= FIXED_COST;
  (*abstrseq)->gain = gain;

  return gain;
}

/* Remove overlaping sequences from ALL_ABSTR_SEQ.  */
static void
clean_overlaping_seq (void)
{
  pAllAbstrSeq first;
  pAllAbstrSeq a0;
  pAllAbstrSeq a1;
  pSeqList sl0;
  pSeqList sl1;
  enum remove_type remove;

  first = all_abstr_seq;
  REWIND (first);

  /* Compare each sequence list against each other.  */
  for (a0 = first; a0; a0 = a0->next)
    {
      if (a0->removed)
        continue;
      for (a1 = first; a1 && !a0->removed; a1 = a1->next)
        {
          if (a1->removed || a0 == a1)
            continue;
          remove = NONE;

          sl0 = a0->abstrseq;
          REWIND (sl0);

          /* Search for overlaping sequences in SL0 and SL1 sequence lists.  */
          for (; sl0 && !remove; sl0 = sl0->next)
            {
              sl1 = a1->abstrseq;
              REWIND (sl1);
              for (; sl1 && !remove; sl1 = sl1->next)
                {
                  int si0, si1, ei0, ei1;
                  if (sl0->seq == sl1->seq)
                    /* We need some gain computation in order to remove one
                       of them.  */
                    remove = REMOVE_SELECTED;
                  else if (sl0->seq->ending.bb == sl1->seq->ending.bb)
                    {
                      si0 = sl0->seq->index;
                      ei0 = si0 + sl0->seq->length;
                      si1 = sl1->seq->index;
                      ei1 = si1 + sl1->seq->length;

                      if (si1 <= si0 && ei1 >= ei0)
                        /* Remove the smaller SL0 sequence list.  */
                        remove = REMOVE_FIRST;
                      else if (si0 <= si1 && ei0 >= ei1)
                        /* Remove the smaller SL1 sequence list.  */
                        remove = REMOVE_SECOND;
                      else if ((si0 < si1 && si1 < ei0)
                               || (si1 < si0 && si0 < ei1))
                        /* We need some gain computation in order to remove one
                           of them.  */
                        remove = REMOVE_SELECTED;
                    }
                }
            }

          /* Mark the overlaping sequence list with REMOVED flag.  */
          switch (remove)
            {
            case REMOVE_SELECTED:
              if (compute_gain (&a0) < compute_gain (&a1))
                a0->removed = 1;
              else
                a1->removed = 1;
              break;
            case REMOVE_FIRST:
              a0->removed = 1;
              break;
            case REMOVE_SECOND:
              a1->removed = 1;
              break;
            default:
              break;
            }
        }
    }
}

/* Dump function for sequence abstraction.  */
static void
dump_all_abstr_seq (void)
{
  pAllAbstrSeq abstrseq;
  pSeqList seqlist;
  int gain;

  if (!dump_file || !all_abstr_seq)
    return;

  gain = 0;
  if (all_abstr_seq)
    fprintf (dump_file, "Dump all abstractable sequence.\n");
  for (abstrseq = all_abstr_seq; abstrseq; abstrseq = abstrseq->next)
    {
      int seqgain;
      if (abstrseq->removed)
        continue;
      for (seqlist = abstrseq->abstrseq; seqlist; seqlist = seqlist->next)
        {
          fprintf (dump_file, "Sequence (bb:%d, pos:%d, len:%d) ",
                   seqlist->seq->ending.bb->index,
                   seqlist->seq->index, seqlist->seq->length);
        }
      fprintf (dump_file, "\n");

      seqgain = compute_gain (&abstrseq);
      if (seqgain)
        {
          fprintf (dump_file, "Gain of sequences' abstraction: %d\n",
                   seqgain);
          gain += seqgain;
        }
    }
  if (gain)
    fprintf (dump_file, "Gain of all sequences: %d\n", gain);
}

/* Sort the abstractable sequences in descending length order.  */
static void
sort_abstract_sequences (void)
{
  pAllAbstrSeq abstrseq;
  pSeqList seqlist1, seqlist2;
  pSequence tmp;

  for (abstrseq = all_abstr_seq; abstrseq; abstrseq = abstrseq->next)
    {
      if (abstrseq->removed)
        continue;
      for (seqlist1 = abstrseq->abstrseq; seqlist1; seqlist1 = seqlist1->next)
        {
          for (seqlist2 = seqlist1->next; seqlist2; seqlist2 = seqlist2->next)
            {
              if (seqlist1 == seqlist2)
                continue;
              if (seqlist1->seq->length < seqlist2->seq->length)
                {
                  /* Swap the  body of sequences.  */
                  tmp = seqlist1->seq;
                  seqlist1->seq = seqlist2->seq;
                  seqlist2->seq = tmp;
                }
            }
        }
    }
}

/* Determine BSI block_stmt_iterator fro T tree node. Return true if BSI is
   valid.  */
static int
tree_to_bsi (block_stmt_iterator * bsi, tree t)
{
  basic_block bb;

  bb = bb_for_stmt (t);
  for (*bsi = bsi_last (bb); !bsi_end_p (*bsi) && t != bsi_stmt (*bsi);
       bsi_prev (bsi));
  return !bsi_end_p (*bsi);
}


/* Add reference variable for storing return address and return with it. */
static tree
abstr_add_var (void)
{
  tree temp_var;

  temp_var = create_tmp_var (ptr_type_node, "seqabstr");
  add_referenced_tmp_var (temp_var);

  return temp_var;
}

/* Add a label for REP sequence. This label describes the entry point of SEQ
   sequence after the abstraction.  */
static void
abstr_add_entrylabel (pSequence rep, pSequence seq)
{
  block_stmt_iterator it;
  basic_block bb;
  edge e;
  int i;

  /* Block of repository.  */
  bb = bb_for_stmt (rep->t_ending);

  /* Find the last common insn in the repository.  */
  for (it = bsi_last (bb); !bsi_end_p (it) && rep->t_ending != bsi_stmt (it);)
    bsi_prev (&it);

  /* Find the first common insn in the repository with sequence.  */
  for (i = 0; !bsi_end_p (it) && i < seq->length; i++)
    bsi_prev (&it);

  gcc_assert (i >= seq->length);

  if (!bsi_end_p (it))
    {
      /* Find or create an entry label for sequence.  */
      e = split_block (bb, bsi_stmt (it));
      it = bsi_start (e->dest);
    }
  else
    it = bsi_start (bb);
  seq->t_entry = bsi_stmt (it);
}

/* Add a label for SEQ sequence for storing return address.  */
static void
abstr_add_endlabels (pSequence rep, pSequence seq)
{
  block_stmt_iterator it;
  basic_block bb;
  edge e;

  /* Block of the sequence.  */
  bb = bb_for_stmt (seq->t_ending);

  /* Find the last common insn of the sequence.  */
  for (it = bsi_last (bb); !bsi_end_p (it) && seq->t_ending != bsi_stmt (it);
       bsi_prev (&it));

  gcc_assert (!bsi_end_p (it));

  /* Detach the sequence from the original basic block.  */
  e = split_block (bb, bsi_stmt (it));
  seq->t_return = tree_block_label (e->dest);

  if (rep == seq)
    while (EDGE_COUNT (bb->succs))
      remove_edge (EDGE_SUCC (bb, 0));
}

/* This function stores return address into VAR variable and inserts a jump to
   the repository.  */
static void
abstr_add_jumps (pSequence seq, tree var)
{
  basic_block rep_bb, seq_bb, ret_bb;
  block_stmt_iterator it;
  tree store;
  edge e;

  /* Find the starting point of the sequence.  */
  if (!tree_to_bsi (&it, seq->t_starting))
    return;

  /* Create insn for storing the address of the following insn.  */

  store = build (MODIFY_EXPR, ptr_type_node, var,
                 build (ADDR_EXPR, ptr_type_node, seq->t_return));

  TREE_SIDE_EFFECTS (store) = 1;

  bsi_insert_before (&it, store, BSI_NEW_STMT);

  /* Detach the address storing from the block.  */
  e = split_block (it.bb, store);

  seq->t_storing = store;

  rep_bb = bb_for_stmt (seq->t_entry);
  seq_bb = e->src;
  ret_bb = label_to_block (seq->t_return);

  /* Jump to the repository after address storing.  */
  redirect_edge_succ (e, rep_bb);

  /* Add an abnormal outgoing edge from the repository to the sequence
     ending.  */
  e = find_edge (rep_bb, ret_bb);
  if (!e)
    e = make_edge (rep_bb, ret_bb, EDGE_ABNORMAL);
}

/* Remove empty sequence block.  */
static void
remove_empty_sequence (pSequence rep, pSequence seq)
{
  block_stmt_iterator rep_it, seq_it;
  basic_block bb;

  if (rep == seq)
    return;

  /* Find the last common insn in the repository and the sequence also.  */
  if (!tree_to_bsi (&rep_it, seq->t_entry)
      || !tree_to_bsi (&seq_it, seq->t_starting))
    return;

  bb = seq_it.bb;

  /* Remove empty sequence block.  */
  delete_basic_block (bb);
}

/* Insert jump with VAR address into REP repository. */
static void
abstr_add_return (pSequence rep, tree var)
{
  block_stmt_iterator it;
  tree jump;

  it = bsi_last (bb_for_stmt (rep->t_ending));

  /* Create copmuted goto insn.  */
  jump = build1 (GOTO_EXPR, void_type_node, var);
  bsi_insert_after (&it, jump, BSI_NEW_STMT);
}

/* Do the sequence abstraction.  */
static void
abstract (void)
{
  pAllAbstrSeq abstrseq;
  pSeqList seqlist;
  tree var;

  /* First of all do the sorting in order to the longest sequence will be at
    begining of the sequence list.  */
  sort_abstract_sequences ();
  for (abstrseq = all_abstr_seq; abstrseq; abstrseq = abstrseq->next)
    {
      /* Skip the removed sequences. This may be abtracted in the next run of
        this function.  */
      if (abstrseq->removed)
        continue;

      /* Add a variable which will hold the return address.  */
      var = abstr_add_var ();

      /* Do the following for each sequence.  */
      for (seqlist = abstrseq->abstrseq; seqlist; seqlist = seqlist->next)
        {
          /* Add an entry label in the reference sequence.  */
          abstr_add_entrylabel (abstrseq->abstrseq->seq, seqlist->seq);
          /* Place a label which will be the return address.  */
          abstr_add_endlabels (abstrseq->abstrseq->seq, seqlist->seq);
          /* Insert jumps into reference sequence.  */
          abstr_add_jumps (seqlist->seq, var);
          /* Remove unused sequence.  */
          remove_empty_sequence (abstrseq->abstrseq->seq, seqlist->seq);
        }
      /* Place the return insn.  */
      abstr_add_return (abstrseq->abstrseq->seq, var);
      has_gain = 1;

      for (seqlist = abstrseq->abstrseq; seqlist; seqlist = seqlist->next)
        {
          basic_block bb;
          block_stmt_iterator bsi;
          tree first, label;
          int found;

          bb = bb_for_stmt (seqlist->seq->t_entry);
          bsi = bsi_start (bb);
          first = bsi_stmt (bsi);

          /* If the begining of the BB basic block is not LABEL_EXPR insert a
            new one.  */
          if (TREE_CODE (first) != LABEL_EXPR)
            {
              tree label_decl, label_expr;
              label_decl = create_artificial_label ();
              label_expr = build1 (LABEL_EXPR, void_type_node, label_decl);
              bsi_insert_before (&bsi, label_expr, BSI_NEW_STMT);
            }

          label = TREE_OPERAND (TREE_OPERAND (seqlist->seq->t_storing, 1), 0);
          found = 0;
          /* Redirect those return addresses of the abstraction which is
            point on any empty block.  */
          FOR_EACH_BB (bb)
          {
            if (!EDGE_COUNT (bb->succs))
              continue;
            if (tree_block_label (bb) == label)
              {
                bsi = bsi_start (bb);
                while (!bsi_end_p (bsi)
                      && TREE_CODE (bsi_stmt (bsi)) == LABEL_EXPR)
                  bsi_next (&bsi);
                /* If it is an empty block redirect the edge. */
                if (bsi_end_p (bsi))
                  {
                    tree next_label;
                    basic_block next_bb;

                    next_bb = EDGE_SUCC (bb, 0)->dest;
                    /* The new label will be the label of following basick
                      block.  */
                    next_label = tree_block_label (next_bb);
                    if (!TREE_TYPE (next_label))
                      TREE_TYPE (next_label) = void_type_node;

                    TREE_OPERAND (TREE_OPERAND (seqlist->seq->t_storing, 1),
                                  0) = next_label;

                    FORCED_LABEL (next_label) = 1;
                    TREE_USED (next_label) = 1;

                    /* Delete the empty block and redirect the edges.  */
                    if (!find_edge (EDGE_PRED (bb, 0)->src, next_bb))
                      redirect_edge_succ (EDGE_PRED (bb, 0), next_bb);

                    delete_basic_block (bb);

                    found = 1;
                    break;
                  }
              }
          }

          /* The new label is neccesary.  */
          if (!found)
            {
              FORCED_LABEL (label) = 1;
              TREE_USED (label) = 1;
              if (!TREE_TYPE (label))
                TREE_TYPE (label) = void_type_node;
            }
        }
    }
}

/* Main function of sequence abstraction.  */
void
tree_seqabstr (void)
{
  has_gain = 1;

  /* Do abtraction until there is any gain of it.  */
  while (has_gain)
    {
      basic_block bb;
      has_gain = 0;

      /* Start sequence abtraction.  */
      init_seqabstr ();
      find_sequences ();
      clean_overlaping_seq ();
      dump_all_abstr_seq ();
      abstract ();
      cleanup_seqabstr ();

      /* Cleanup.  */
      if (has_gain)
        {
          free_dominance_info (CDI_DOMINATORS);
          cleanup_tree_cfg ();
          calculate_dominance_info (CDI_DOMINATORS);
          FOR_EACH_BB (bb)
          {
            basic_block dom;
            dom = recount_dominator (CDI_DOMINATORS, bb);
            set_immediate_dominator (CDI_DOMINATORS, bb, dom);
          }
        }
    }
}

/* The gate of sequence abstraction.  */
bool
is_seqabstr (void)
{
  return (flag_tree_seqabstr == 1);
}

struct tree_opt_pass pass_tree_seqabstr = {
  "seqabstr",			/* name */
  is_seqabstr,			/* gate */
  tree_seqabstr,		/* execute */
  NULL,				/* sub */
  NULL,				/* next */
  0,				/* static_pass_number */
  0,				/* tv_id */
  0,				/* properties_required */
  0,				/* properties_provided */
  0,				/* properties_destroyed */
  0,				/* todo_flags_start */
  TODO_dump_func,		/* todo_flags_finish */
  0				/* letter */
};
