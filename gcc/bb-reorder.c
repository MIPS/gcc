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

/* References:

   "Software Trace Cache"
   Ramirez, Larriba-Pey, Navarro, Torrellas and Valero; 1999
   http://citeseer.nj.nec.com/15361.html

*/

#include "config.h"
#include "system.h"
#include "tree.h"
#include "rtl.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "flags.h"
#include "output.h"
#include "cfglayout.h"
#include "fibheap.h"

/* The number of rounds.  */
#define N_ROUNDS 4

/* The number of rounds which the code can grow in.  */
#define N_CODEGROWING_ROUNDS 3

/* Branch thresholds in thousandths (per milles) of the REG_BR_PROB_BASE.  */
static int branch_threshold[N_ROUNDS] = {400, 200, 100, 0};

/* Exec thresholds in thousandths (per milles) of the frequency of bb 0.  */
static int exec_threshold[N_ROUNDS] = {500, 200, 50, 0};

/* Length of unconditional jump instruction.  */
static int uncond_jump_length;

/* Array of flags that the bb was visited in copy_bb_p to avoid infinite
   loop.  */
static unsigned int *copy_bb_p_visited;

/* The size of the previous array.  */
static int copy_bb_p_visited_size;

struct trace
{
  /* First and last basic block of the trace.  */
  basic_block first, last;

  /* The round of the STC creation which this trace was found in.  */
  int round;
};

/* Local function prototypes.  */
static void find_traces			PARAMS ((void));
static void find_traces_1_round		PARAMS ((int, int, struct trace *,
						 int *, int, fibheap_t *,
						 bool));
static int bb_to_key			PARAMS ((basic_block));
static bool copy_bb_p			PARAMS ((basic_block, int, bool));
static bool better_edge_p		PARAMS ((basic_block, edge, int, int,
						 int *, int *, int *, int *));
static int get_uncond_jump_length	PARAMS ((void));

/* Find the traces for Software Trace Cache.  Record the ordering
   in RBI()->index and chain it through RBI()->next.  */

static void
find_traces ()
{
  int i;
  int n_traces;
  struct trace *traces;
  basic_block bb0;
  fibheap_t heap;
  int *start_of_trace;

  /* There will be at most N_BASIC_BLOCKS traces because trace can start only
     in an original (not duplicated) basic block.  */
  traces = xmalloc (n_basic_blocks * sizeof(struct trace));
  n_traces = 0;

  /* Find the traces.  */
  bb0 = BASIC_BLOCK (0);
  heap = fibheap_new();
  fibheap_insert (heap, bb_to_key (bb0), bb0);
  for (i = 0; i < N_ROUNDS; i++)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file, "STC - round %d\n", i);

      find_traces_1_round (branch_threshold[i] * REG_BR_PROB_BASE / 1000,
			   exec_threshold[i] * bb0->frequency / 1000,
			   traces, &n_traces, i, &heap,
			   !optimize_size && i < N_CODEGROWING_ROUNDS);
    }
  fibheap_delete (heap);

  if (rtl_dump_file)
    {
      for (i = 0; i < n_traces; i++)
	{
	  basic_block bb;
	  fprintf(rtl_dump_file, "Trace %d (round %d):  ", i, traces[i].round);
	  for (bb = traces[i].first; bb != traces[i].last; bb = RBI (bb)->next)
	    fprintf(rtl_dump_file, "%d [%d] ", bb->index, bb->frequency);
	  fprintf(rtl_dump_file, "%d [%d]\n", bb->index, bb->frequency);
	}
      fflush (rtl_dump_file);
    }

  /* Mark each bb which is a start of a trace.  */
  start_of_trace = xmalloc (n_basic_blocks * sizeof(int));
  for (i = 0; i < n_basic_blocks; i++)
    start_of_trace[i] = -1;
  for (i = 0; i < n_traces; i++)
    start_of_trace[traces[i].first->index] = i;

  /* Connect traces.  */
  for (i = 0; i < n_traces - 1; i++)
    {
      edge e, best = NULL;

      for (e = traces[i].last->succ; e; e = e->succ_next)
	if (e->dest != EXIT_BLOCK_PTR
	    && (e->flags & EDGE_CAN_FALLTHRU)
	    && start_of_trace[e->dest->index] >= i + 1
	    && (!best || e->probability > best->probability))
	  best = e;

      if (best && start_of_trace[best->dest->index] != i + 1)
	{
	  int t = start_of_trace[best->dest->index];
	  struct trace temp;

	  temp = traces[t];
	  traces[t] = traces[i + 1];
	  traces[i + 1] = temp;
	  start_of_trace[traces[i + 1].first->index] = i + 1;
	  start_of_trace[traces[t].first->index] = t;
	}

      RBI (traces[i].last)->next = traces[i+1].first;
    }

  free (traces);
  free (start_of_trace);
}

/* One round of finding traces. Find traces for BRANCH_TH and EXEC_TH i.e. do
   not include basic blocks their probability is lower than BRANCH_TH or threir
   frequency is lower than EXEC_TH into traces.  It stores the new traces into
   TRACES and modifies the number of traces *N_TRACES. Sets the round (which
   the trace belongs to) to ROUND. It expects that the starting basic blocks
   are in the *HEAP and at the end it deletes *HEAP and stores the starting
   points for the next round into *HEAP.  SIZE_CAN_GROW is the flag whether
   the code is permited to grow.  */

static void
find_traces_1_round (branch_th, exec_th, traces, n_traces, round, heap,
		     size_can_grow)
     int branch_th;
     int exec_th;
     struct trace *traces;
     int *n_traces;
     int round;
     fibheap_t *heap;
     bool size_can_grow;
{
  /* Heap for discarded basic blocks which are possible starting points for
     the next round.  */
  fibheap_t new_heap = fibheap_new ();

  while (!fibheap_empty (*heap))
    {
      basic_block bb;
      struct trace *trace;
      edge best_edge, e;

      bb = fibheap_extract_min (*heap);
      if (rtl_dump_file)
	fprintf (rtl_dump_file, "Getting bb %d%s\n", bb->index,
		 RBI (bb)->visited ? " (visited)" : "");
      if (RBI (bb)->visited)
	continue;

      /* If the BB's frequency is too low send BB to the next round.  */
      if (bb->frequency < exec_th)
	{
	  int key = bb_to_key (bb);
	  fibheap_insert (new_heap, key, bb);

	  if (rtl_dump_file)
	    fprintf (rtl_dump_file,
		     "  Possible start point of next round: %d (key: %d)\n",
		     bb->index, key);
	  continue;
	}

      trace = traces + *n_traces;
      trace->first = bb;
      trace->last = bb;
      trace->round = round;
      (*n_traces)++;

      do
	{
	  /* If the probability of an edge is between prob_lower and
	     prob_higher the edge is considered to be as probable as the
	     temporary best edge.  Similar for frequencies of successors.  */
	  int prob_lower = INT_MIN, prob_higher = INT_MIN;
	  int freq_lower = INT_MIN, freq_higher = INT_MIN;

	  best_edge = NULL;
	  RBI (bb)->visited = *n_traces;
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, "Basic block %d was visited in trace %d\n",
		     bb->index, *n_traces - 1);

	  /* Select the successor that will be placed after BB.  */
	  for (e = bb->succ; e; e = e->succ_next)
	    {
	      int prob, freq;

	      if (e->flags & EDGE_FAKE)
		abort();
	      if (e->dest == EXIT_BLOCK_PTR)
		continue;

	      prob = e->probability;
	      if (RBI (e->dest)->visited
		  && RBI (e->dest)->visited != *n_traces)
		{
		  if (!copy_bb_p (e->dest, *n_traces, size_can_grow))
		    continue;
		  freq = EDGE_FREQUENCY (e);
		}
	      else
	        {
		  freq = e->dest->frequency;
		}

	      /* Edge that cannot be fallthru
		 or improbable or infrequent successor.  */
	      if (!(e->flags & EDGE_CAN_FALLTHRU) || (e->flags & EDGE_COMPLEX)
		  || prob < branch_th || freq < exec_th)
		{
		  if (!RBI (e->dest)->visited)
		    /* This bb is not in any trace yet.  */
		    {
		      int key = bb_to_key (e->dest);
		      if (round == N_ROUNDS - 1)
			/* There are no more rounds so the successor is another
			   possible starting point of this round of trace
			   creation.  */
			fibheap_insert (*heap, key, e->dest);
		      else
		        /* This successor is possible starting point for next
			   round of trace creation.  */
		        fibheap_insert (new_heap, key, e->dest);

		      if (rtl_dump_file)
			fprintf (rtl_dump_file,
				 "  Possible start point of %s round: %d"
				 " (key: %d)\n",
				 (round == N_ROUNDS - 1) ? "this" : "next",
				 e->dest->index, key);
		    }
		  continue;
		}

	      if (better_edge_p (bb, e, prob, freq, &prob_lower, &prob_higher,
				 &freq_lower, &freq_higher))
		{
		  if (best_edge && !RBI (best_edge->dest)->visited)
		    {
		      /* Start a secondary trace from the old temporary best
			 successor.  */
		      int key = bb_to_key (best_edge->dest);
		      fibheap_insert (*heap, key, best_edge->dest);

		      if (rtl_dump_file)
			fprintf (rtl_dump_file,
				 "  Possible start of secondary trace: %d"
				 " (key: %d)\n", best_edge->dest->index, key);
		    }

		  best_edge = e;
		}
	      else if (!RBI (e->dest)->visited)
		{
		  /* Start a secondary trace from this successor.  */
		  int key = bb_to_key (e->dest);
		  fibheap_insert (*heap, key, e->dest);

		  if (rtl_dump_file)
		    fprintf (rtl_dump_file,
			     "  Possible start of secondary trace: %d"
			     " (key: %d)\n", e->dest->index, key);
		}
	    }

	  if (best_edge)
	    /* Found suitable successor.  */
	    {
	      if (RBI (best_edge->dest)->visited == *n_traces)
		{
		  if (bb != best_edge->dest)
		    {
		      edge e;

		      /* Check whether the loop has not been rotated yet.  */
		      for (e = best_edge->dest->succ; e; e = e->succ_next)
			if (e->dest == RBI (best_edge->dest)->next)
			  break;
		      if (e && (EDGE_FREQUENCY (best_edge)
				> 2 * best_edge->dest->frequency / 3))
			/* The loop has not been rotated yet
			   && has at least 2 iterations.  */
			{
			  /* Rotate the loop.  */

			  if (rtl_dump_file)
			    fprintf (rtl_dump_file, "Rotating loop %d - %d\n",
				     best_edge->dest->index, bb->index);

			  if (best_edge->dest == trace->first)
			    {
			      RBI (bb)->next = trace->first;
			      trace->first = RBI (trace->first)->next;
			      RBI (best_edge->dest)->next = NULL;
			      bb = best_edge->dest;
			    }
			  else
			    {
			      basic_block temp;

			      for (temp = trace->first;
				   RBI (temp)->next != best_edge->dest;
				   temp = RBI (temp)->next);
			      RBI (temp)->next = RBI (best_edge->dest)->next;
			      RBI (bb)->next = best_edge->dest;
			      RBI (best_edge->dest)->next = NULL;
			      bb = best_edge->dest;
			    }
			}
		    }

		  /* Terminate the trace.  */
		  trace->last = bb;
		  break;
		}
	      else if (RBI (best_edge->dest)->visited)
		{
		  basic_block new_bb, old_bb;

		  old_bb = best_edge->dest;
		  new_bb = cfg_layout_duplicate_bb (best_edge->dest, best_edge);
		  if (best_edge->dest != new_bb)
		    abort ();
		  if (RBI (best_edge->dest)->visited)
		    abort ();
		  if (rtl_dump_file)
		    fprintf (rtl_dump_file, "Duplicated bb %d (created bb %d)\n",
			     old_bb->index, new_bb->index);
		  RBI (old_bb)->visited = *n_traces;
		  RBI (old_bb)->duplicated = *n_traces;
		  RBI (bb)->next = new_bb;
		  RBI (bb)->visited = *n_traces;
		  bb = new_bb;

		  if (n_basic_blocks > copy_bb_p_visited_size)
		    {
		      /* Realloc the COPY_BB_P_VISITED array, copying of data
		         is not necessary.  */
		      free (copy_bb_p_visited);
  		      copy_bb_p_visited_size = 4 * n_basic_blocks / 3;
		      copy_bb_p_visited = xcalloc (copy_bb_p_visited_size,
			  			   sizeof (unsigned int));
		      if (!copy_bb_p_visited)
			abort ();
		    }
		}
	      else
		{
		  RBI (bb)->next = best_edge->dest;
		  bb = best_edge->dest;
		}
	      trace->last = bb;
	    }
	}
      while (best_edge);
    }

  fibheap_delete (*heap);

  /* "Return" the new heap.  */
  *heap = new_heap;
}

/* Compute and return the key (for the heap) of the basic block BB.  */

static int
bb_to_key (bb)
     basic_block bb;
{
  edge e;

  for (e = bb->pred; e; e = e->pred_next)
    if (!(e->flags & EDGE_DFS_BACK) && !RBI (e->src)->visited)
      return -bb->frequency;

  /* All edges to predecessors of BB are DFS back edges or the predecessors
     of BB are visited.  I want such basic blocks first.  */
  return -100 * BB_FREQ_MAX - bb->frequency;
}

/* Return true when the edge E from basic block BB is better than the temporary
   best edge (details are in function).  The (scaled) probability of edge E is
   PROB. The frequency of the successor is FREQ. The *PROB_LOWER and
   *PROB_HIGHER are the lower and higher bounds of interval which the PROB must
   be in to be "equivalent" to the probability of the temporary best edge.
   Similarly the *FREQ_LOWER and *FREQ_HIGHER.
   If the edge is considered to be better it changes the values of *PROB_LOWER,
   *PROB_HIGHER, *FREQ_LOWER and *FREQ_HIGHER to appropriate values.  */

static bool
better_edge_p (bb, e, prob, freq, prob_lower, prob_higher, freq_lower,
	       freq_higher)
     basic_block bb;
     edge e;
     int prob;
     int freq;
     int *prob_lower;
     int *prob_higher;
     int *freq_lower;
     int *freq_higher;
{
  bool is_better_edge;

  if (prob > *prob_higher)
    /* The edge has higher probability than the temporary best edge.  */
    is_better_edge = true;
  else if (prob < *prob_lower)
    /* The edge has lower probability than the temporary best edge.  */
    is_better_edge = false;
  else if (freq < *freq_lower)
    /* The edge and the temporary best edge  have almost equivalent
       probabilities.  The higher frequency of a successor now means
       that there is another edge going into that successor.
       This successor has lower frequency so it is better.  */
    is_better_edge = true;
  else if (freq > *freq_higher)
    /* This successor has higher frequency so it is worse.  */
    is_better_edge = false;
  else if (e->dest->index == bb->index + 1)
    /* The edges have equivalent probabilities and the successors
       have equivalent frequencies.  Select the previous successor.  */
    is_better_edge = true;
  else
    is_better_edge = false;

  if (is_better_edge)
    {
      int prob_diff, freq_diff;

      prob_diff = prob / 10;
      *prob_higher = prob + prob_diff;
      *prob_lower = prob - prob_diff;
      freq_diff = freq / 10;
      *freq_higher = freq + freq_diff;
      *freq_lower = freq - freq_diff;
    }
  return is_better_edge;
}

/* Return true when BB can and should be copied.  The trace with number TRACE
   is now being built.  SIZE_CAN_GROW is the flag whether the code is permited
   to grow.  */

static bool
copy_bb_p (bb, trace, size_can_grow)
     basic_block bb;
     int trace;
     bool size_can_grow;
{
  int size = 0;
  rtx insn;
  static unsigned int id;

  if (id == UINT_MAX)
    {
      int i;

      id = 1;
      /* We are starting again with ID 1 so clear the flags.  */
      for (i = 0; i < copy_bb_p_visited_size; i++)
	copy_bb_p_visited[i] = 0;
    }
  else
    id++;
    
  
  if (!bb->frequency)
    return false;
  if (!bb->pred || !bb->pred->pred_next)
    return false;
  while (1)
    {
      edge e, best_edge;
      int prob_lower, prob_higher;
      int freq_lower, freq_higher;

      copy_bb_p_visited[bb->index] = id;

      if (RBI (bb)->visited == trace)
	return false;
      if (!cfg_layout_can_duplicate_bb_p (bb))
	return false;
      for (insn = bb->head; insn != NEXT_INSN (bb->end);
	   insn = NEXT_INSN (insn))
	{
	  if (INSN_P (insn))
	    size += get_attr_length (insn);
	}
      if (size > 8 * uncond_jump_length)
	return false;

      /* Select the successor.  */
      best_edge = NULL;
      prob_lower = INT_MIN;
      freq_lower = INT_MIN;
      prob_higher = INT_MIN;
      freq_higher = INT_MIN;
      for (e = bb->succ; e; e = e->succ_next)
	{
	  if (!(e->flags & EDGE_CAN_FALLTHRU))
	    continue;
	  if (e->flags & EDGE_COMPLEX)      /* Is this code redundant?  */
	    continue;
	  if (e->dest == EXIT_BLOCK_PTR)
	    continue;
	  if (better_edge_p (bb, e, e->probability, e->dest->frequency,
			     &prob_lower, &prob_higher, &freq_lower,
			     &freq_higher))
	    best_edge = e;
	}
      if (!best_edge || !RBI (best_edge->dest)->visited
	  || RBI (best_edge->dest)->visited == trace
	  || copy_bb_p_visited[best_edge->dest->index] == id)
	/* All edges point to EXIT_BLOCK or to the same trace
	   OR the selected successor has not been visited yet
	   OR we have found a loop (so the trace will be terminated)
	   OR we have reached the bb that was already visited
	      in this run of copy_bb_p.  */
	{
	  if (size_can_grow || size <= uncond_jump_length)
	    return true;
	  else
	    return false;
	}
      bb = best_edge->dest;
    }
  return false;
}

/* Return the maximum length of unconditional jump.  */

static int
get_uncond_jump_length ()
{
  rtx label, jump;
  int length;

  label = emit_label_before (gen_label_rtx (), get_insns ());
  jump = emit_jump_insn (gen_jump (label));

  length = get_attr_length (jump);

  delete_insn (jump);
  delete_insn (label);
  return length;
}

/* Reorder basic blocks.  The main entry point to this file.  */

void
reorder_basic_blocks ()
{
  if (n_basic_blocks <= 1)
    return;

  cfg_layout_initialize ();

  copy_bb_p_visited_size = MAX (4 * n_basic_blocks / 3, 10);
  copy_bb_p_visited = xcalloc (copy_bb_p_visited_size, sizeof (unsigned int));
  if (!copy_bb_p_visited)
    abort ();
  
  set_edge_can_fallthru_flag ();
  uncond_jump_length = get_uncond_jump_length ();
  find_traces ();

  free (copy_bb_p_visited);

  if (rtl_dump_file)
    dump_flow_info (rtl_dump_file);

  cfg_layout_finalize ();
}
