/* Basic block reordering routines for the GNU compiler.
   Copyright (C) 2000, 2002, 2003 Free Software Foundation, Inc.

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

/* This (greedy) algorithm constructs traces in several rounds.
   The construction starts from "seeds".  The seed for the first round
   is the entry point of function.  When there are more than one seed
   that one is selected first that has the lowest key in the heap
   (see function bb_to_key).  Then the algorithm repeatedly adds the most
   probable successor to the end of a trace.  Finally it connects the traces.

   There are two parameters: Branch Threshold and Exec Threshold.
   If the edge to a successor of the actual basic block is lower than
   Branch Threshold or the frequency of the successor is lower than
   Exec Threshold the successor will be the seed in one of the next rounds.
   Each round has these parameters lower than the previous one.
   The last round has to have these parameters set to zero
   so that the remaining blocks are picked up.

   The algorithm selects the most probable successor from all unvisited
   successors and successors that have been added to this trace.
   The other successors (that has not been "sent" to the next round) will be
   other seeds for this round and the secondary traces will start in them.
   If the successor has not been visited in this trace it is added to the trace
   (however, there is some heuristic for simple branches).
   If the successor has been visited in this trace the loop has been found.
   If the loop has many iterations the loop is rotated so that the
   source block of the most probable edge going out from the loop
   is the last block of the trace.
   If the loop has few iterations and there is no edge from the last block of
   the loop going out from loop the loop header is duplicated.
   Finally, the construction of the trace is terminated.

   When connecting traces it first checks whether there is an edge from the
   last block of one trace to the first block of another trace.
   When there are still some unconnected traces it checks whether there exists
   a basic block BB such that BB is a successor of the last bb of one trace
   and BB is a predecessor of the first block of another trace. In this case,
   BB is duplicated and the traces are connected through this duplicate.
   The rest of traces are simply connected so there will be a jump to the
   beginning of the rest of trace.


   References:

   "Software Trace Cache"
   A. Ramirez, J. Larriba-Pey, C. Navarro, J. Torrellas and M. Valero; 1999
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
#include "target.h"
#include "profile.h"
#include "params.h"

/* The number of rounds.  */
#define N_ROUNDS 4

/* Branch thresholds in thousandths (per mille) of the REG_BR_PROB_BASE.  */
static int branch_threshold[N_ROUNDS] = {280, 280, 280, 280};

/* Exec thresholds in thousandths (per mille) of the frequency of bb 0.  */
static int exec_threshold[N_ROUNDS] = {0, 0, 0, 0};

/* If edge frequency is lower than DUPLICATION_THRESHOLD per mille of entry
   block the edge destination is not duplicated while connecting traces.  */
#define DUPLICATION_THRESHOLD 100

/* Cost of an unconditional jump.  */
#define JUMP_COST 100

/* The cost of correctly predicted branch that is taken.  */
#define HIT_TAKEN_COST (PARAM_VALUE (PARAM_HIT_TAKEN_COST))

/* The cost of incorrectly predicted branch that is taken.  */
#define MISS_TAKEN_COST (PARAM_VALUE (PARAM_MISS_TAKEN_COST))

/* The cost of incorrectly predicted branch that is not taken.  */
#define MISS_FALLTHRU_COST (PARAM_VALUE (PARAM_MISS_FALLTHRU_COST))

/* Length of unconditional jump instruction.  */
static int uncond_jump_length;

/* The current size of the following dynamic arrays.  */
static int array_size;

/* To avoid frequent reallocation the size of arrays is greater than needed,
   the number of elements is (not less than) 1.25 * size_wanted.  */
#define GET_ARRAY_SIZE(X) ((((X) / 4) + 1) * 5)

/* Which trace is the bb start of (-1 means it is not a start of a trace).  */
static int *start_of_trace;
static int *end_of_trace;

/* Which heap and node is BB in?  */
static fibheap_t *bb_heap;
static fibnode_t *bb_node;

/* Free the memory and set the pointer to NULL.  */
#define FREE(P) \
  do { if (P) { free (P); P = 0; } else { abort (); } } while (0)

/* Structure for holding information about a trace.  */
struct trace
{
  /* First and last basic block of the trace.  */
  basic_block first, last;

  /* The round of the STC creation which this trace was found in.  */
  int round;

  /* The length (i.e. the number of basic blocks) of the trace.  */
  int length;
};

/* Maximum frequency and count of one of the entry blocks.  */
int max_entry_frequency;
gcov_type max_entry_count;

/* Local function prototypes.  */
static void find_traces			PARAMS ((int *, struct trace *));
static basic_block rotate_loop		PARAMS ((edge, struct trace *, int));
static void mark_bb_visited		PARAMS ((basic_block, int));
static void find_traces_1_round		PARAMS ((int, int, gcov_type,
						 struct trace *, int *, int,
						 fibheap_t *));
static basic_block copy_bb		PARAMS ((basic_block, edge,
						 basic_block, int));
static fibheapkey_t bb_to_key		PARAMS ((basic_block));
static bool better_edge_p		PARAMS ((basic_block, edge, int, int,
						 int, int));
static void connect_traces		PARAMS ((int, struct trace *));
static bool copy_bb_p			PARAMS ((basic_block, int));
static int get_uncond_jump_length	PARAMS ((void));
static void reorder_using_tsp (void);
static int cost_of_uncond_jump (edge);
static int cost_of_cond_jump (edge);
static int cost_of_cond_branch (basic_block);
static int cost_b_after_a (basic_block, basic_block);
static void record_edges (basic_block, struct vertex *, int last);

/* Find the traces for Software Trace Cache.  Chain each trace through
   RBI()->next.  Store the number of traces to N_TRACES and description of
   traces to TRACES.  */

static void
find_traces (n_traces, traces)
     int *n_traces;
     struct trace *traces;
{
  int i;
  edge e;
  fibheap_t heap;

  /* We need to know some information for each basic block.  */
  array_size = GET_ARRAY_SIZE (last_basic_block);
  start_of_trace = xmalloc (array_size * sizeof (int));
  end_of_trace = xmalloc (array_size * sizeof (int));
  bb_heap = xmalloc (array_size * sizeof (fibheap_t));
  bb_node = xmalloc (array_size * sizeof (fibnode_t));
  for (i = 0; i < array_size; i++)
    {
      start_of_trace[i] = -1;
      end_of_trace[i] = -1;
      bb_heap[i] = NULL;
      bb_node[i] = NULL;
    }

  /* Insert entry points of function into heap.  */
  heap = fibheap_new ();
  max_entry_frequency = 0;
  max_entry_count = 0;
  for (e = ENTRY_BLOCK_PTR->succ; e; e = e->succ_next)
    {
      int bb_index = e->dest->index;
      bb_heap[bb_index] = heap;
      bb_node[bb_index] = fibheap_insert (heap, bb_to_key (e->dest), e->dest);
      if (e->dest->frequency > max_entry_frequency)
	max_entry_frequency = e->dest->frequency;
      if (e->dest->count > max_entry_count)
	max_entry_count = e->dest->count;
    }

  /* Find the traces.  */
  for (i = 0; i < N_ROUNDS; i++)
    {
      gcov_type count_threshold;

      if (rtl_dump_file)
	fprintf (rtl_dump_file, "STC - round %d\n", i + 1);

      if (max_entry_count < INT_MAX / 1000)
	count_threshold = max_entry_count * exec_threshold[i] / 1000;
      else
	count_threshold = max_entry_count / 1000 * exec_threshold[i];

      find_traces_1_round (REG_BR_PROB_BASE * branch_threshold[i] / 1000,
			   max_entry_frequency * exec_threshold[i] / 1000,
			   count_threshold, traces, n_traces, i, &heap);
    }
  fibheap_delete (heap);
  FREE (bb_node);
  FREE (bb_heap);

  if (rtl_dump_file)
    {
      for (i = 0; i < *n_traces; i++)
	{
	  basic_block bb;
	  fprintf (rtl_dump_file, "Trace %d (round %d):  ", i + 1,
		   traces[i].round + 1);
	  for (bb = traces[i].first; bb != traces[i].last; bb = RBI (bb)->next)
	    fprintf (rtl_dump_file, "%d [%d] ", bb->index, bb->frequency);
	  fprintf (rtl_dump_file, "%d [%d]\n", bb->index, bb->frequency);
	}
      fflush (rtl_dump_file);
    }
}

/* Rotate loop whose back edge is BACK_EDGE in the tail of trace TRACE
   (with sequential number TRACE_N).  */

static basic_block
rotate_loop (back_edge, trace, trace_n)
     edge back_edge;
     struct trace *trace;
     int trace_n;
{
  basic_block bb;

  /* Information about the best end (end after rotation) of the loop.  */
  basic_block best_bb = NULL;
  edge best_edge = NULL;
  int best_freq = -1;
  gcov_type best_count = -1;
  /* The best edge is preferred when its destination is not visited yet
     or is a start block of some trace.  */
  bool is_preferred = false;

  /* Find the most frequent edge that goes out from current trace.  */
  bb = back_edge->dest;
  do
    {
      edge e;
      for (e = bb->succ; e; e = e->succ_next)
	if (e->dest != EXIT_BLOCK_PTR
	    && RBI (e->dest)->visited != trace_n
	    && (e->flags & EDGE_CAN_FALLTHRU)
	    && !(e->flags & EDGE_COMPLEX))
	{
	  if (is_preferred)
	    {
	      /* The best edge is preferred.  */
	      if (!RBI (e->dest)->visited
		  || start_of_trace[e->dest->index] >= 0)
		{
		  /* The current edge E is also preferred.  */
		  int freq = EDGE_FREQUENCY (e);
		  if (freq > best_freq || e->count > best_count)
		    {
		      best_freq = freq;
		      best_count = e->count;
		      best_edge = e;
		      best_bb = bb;
		    }
		}
	    }
	  else
	    {
	      if (!RBI (e->dest)->visited
		  || start_of_trace[e->dest->index] >= 0)
		{
		  /* The current edge E is preferred.  */
		  is_preferred = true;
		  best_freq = EDGE_FREQUENCY (e);
		  best_count = e->count;
		  best_edge = e;
		  best_bb = bb;
		}
	      else
		{
		  int freq = EDGE_FREQUENCY (e);
		  if (!best_edge || freq > best_freq || e->count > best_count)
		    {
		      best_freq = freq;
		      best_count = e->count;
		      best_edge = e;
		      best_bb = bb;
		    }
		}
	    }
	}
      bb = RBI (bb)->next;
    }
  while (bb != back_edge->dest);

  if (best_bb)
    {
      /* Rotate the loop so that the BEST_EDGE goes out from the last block of
	 the trace.  */
      if (back_edge->dest == trace->first)
	{
	  trace->first = RBI (best_bb)->next;
	}
      else
	{
	  basic_block prev_bb;
	  
	  for (prev_bb = trace->first;
	       RBI (prev_bb)->next != back_edge->dest;
	       prev_bb = RBI (prev_bb)->next)
	    ;
	  RBI (prev_bb)->next = RBI (best_bb)->next;

	  /* Try to get rid of uncond jump to cond jump.  */
	  if (prev_bb->succ && !prev_bb->succ->succ_next)
	    {
	      basic_block header = prev_bb->succ->dest;

	      /* Duplicate HEADER if it is a small block containing condjump
		 in the end.  */
	      if (any_condjump_p (header->end) && copy_bb_p (header, false))
		{
		  copy_bb (header, prev_bb->succ, prev_bb, trace_n);
		}
	    }
	}
    }
  else
    {
      /* We have not found suitable loop tail so do no rotation.  */
      best_bb = back_edge->src;
    }
  RBI (best_bb)->next = NULL;
  return best_bb;
}

/* This function marks BB that it was visited in trace number TRACE.  */

static void
mark_bb_visited (bb, trace)
     basic_block bb;
     int trace;
{
  RBI (bb)->visited = trace;
  if (bb_heap[bb->index])
    {
      fibheap_delete_node (bb_heap[bb->index], bb_node[bb->index]);
      bb_heap[bb->index] = NULL;
      bb_node[bb->index] = NULL;
    }
}

/* One round of finding traces. Find traces for BRANCH_TH and EXEC_TH i.e. do
   not include basic blocks their probability is lower than BRANCH_TH or their
   frequency is lower than EXEC_TH into traces (or count is lower than
   COUNT_TH).  It stores the new traces into TRACES and modifies the number of
   traces *N_TRACES. Sets the round (which the trace belongs to) to ROUND. It
   expects that starting basic blocks are in *HEAP and at the end it deletes
   *HEAP and stores starting points for the next round into new *HEAP.  */

static void
find_traces_1_round (branch_th, exec_th, count_th, traces, n_traces, round,
		     heap)
     int branch_th;
     int exec_th;
     gcov_type count_th;
     struct trace *traces;
     int *n_traces;
     int round;
     fibheap_t *heap;
{
  /* Heap for discarded basic blocks which are possible starting points for
     the next round.  */
  fibheap_t new_heap = fibheap_new ();

  while (!fibheap_empty (*heap))
    {
      basic_block bb;
      struct trace *trace;
      edge best_edge, e;
      int bb_index;
      fibheapkey_t key;

      bb = fibheap_extract_min (*heap);
      bb_heap[bb->index] = NULL;
      bb_node[bb->index] = NULL;

      if (rtl_dump_file)
	fprintf (rtl_dump_file, "Getting bb %d\n", bb->index);

      /* If the BB's frequency is too low send BB to the next round.  */
      if (round < N_ROUNDS - 1
	  && (bb->frequency < exec_th || bb->count < count_th
	      || probably_never_executed_bb_p (bb)))
	{
	  int key = bb_to_key (bb);
	  bb_heap[bb->index] = new_heap;
	  bb_node[bb->index] = fibheap_insert (new_heap, key, bb);

	  if (rtl_dump_file)
	    fprintf (rtl_dump_file,
		     "  Possible start point of next round: %d (key: %d)\n",
		     bb->index, key);
	  continue;
	}

      trace = traces + *n_traces;
      trace->first = bb;
      trace->round = round;
      trace->length = 0;
      (*n_traces)++;

      do
	{
	  int prob, freq;

	  /* The probability and frequency of the best edge.  */
	  int best_prob = INT_MIN / 2;
	  int best_freq = INT_MIN / 2;

	  best_edge = NULL;
	  mark_bb_visited (bb, *n_traces);
	  trace->length++;

	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, "Basic block %d was visited in trace %d\n",
		     bb->index, *n_traces - 1);

	  /* Select the successor that will be placed after BB.  */
	  for (e = bb->succ; e; e = e->succ_next)
	    {
	      if (e->flags & EDGE_FAKE)
		abort ();

	      if (e->dest == EXIT_BLOCK_PTR)
		continue;

	      if (RBI (e->dest)->visited
		  && RBI (e->dest)->visited != *n_traces)
		continue;

	      prob = e->probability;
	      freq = EDGE_FREQUENCY (e);

	      /* Edge that cannot be fallthru or improbable or infrequent
		 successor (ie. it is unsuitable successor).  */
	      if (!(e->flags & EDGE_CAN_FALLTHRU) || (e->flags & EDGE_COMPLEX)
		  || prob < branch_th || freq < exec_th || e->count < count_th)
		continue;

	      if (better_edge_p (bb, e, prob, freq, best_prob, best_freq))
		{
		  best_edge = e;
		  best_prob = prob;
		  best_freq = freq;
		}
	    }

	  /* Add all non-selected successors to the heaps.  */
	  for (e = bb->succ; e; e = e->succ_next)
	    {
	      if (e == best_edge
		  || e->dest == EXIT_BLOCK_PTR
		  || RBI (e->dest)->visited)
		continue;

	      key = bb_to_key (e->dest);
	      bb_index = e->dest->index;

	      if (bb_heap[bb_index])
		{
		  if (key != bb_node[bb_index]->key)
		    {
		      if (rtl_dump_file)
			{
			  fprintf (rtl_dump_file,
				   "Changing key for bb %d from %ld to %ld.\n",
				   bb_index, (long) bb_node[bb_index]->key,
				   key);
			}
		      fibheap_replace_key (bb_heap[bb_index],
					   bb_node[bb_index], key);
		    }
		}
	      else
		{
		  fibheap_t which_heap = *heap;

		  prob = e->probability;
		  freq = EDGE_FREQUENCY (e);

		  if (!(e->flags & EDGE_CAN_FALLTHRU)
		      || (e->flags & EDGE_COMPLEX)
		      || prob < branch_th || freq < exec_th
		      || e->count < count_th)
		    {
		      if (round < N_ROUNDS - 1)
			which_heap = new_heap;
		    }

		  bb_heap[bb_index] = which_heap;
		  bb_node[bb_index] = fibheap_insert (which_heap, key, e->dest);

		  if (rtl_dump_file)
		    {
		      fprintf (rtl_dump_file,
			       "  Possible start of %s round: %d (key: %ld)\n",
			       (which_heap == new_heap) ? "next" : "this",
			       bb_index, (long) key);
		    }

		}
	    }

	  if (best_edge) /* Suitable successor was found.  */
	    {
	      if (RBI (best_edge->dest)->visited == *n_traces)
		{
		  /* We do nothing with one basic block loops.  */
		  if (best_edge->dest != bb)
		    {
		      if (EDGE_FREQUENCY (best_edge)
			  > 4 * best_edge->dest->frequency / 5)
			{
			  /* The loop has at least 4 iterations.  If the loop
			     header is not the first block of the function
			     we can rotate the loop.  */

			  if (best_edge->dest != ENTRY_BLOCK_PTR->next_bb)
			    {
			      if (rtl_dump_file)
				{
				  fprintf (rtl_dump_file, 
					   "Rotating loop %d - %d\n",
					   best_edge->dest->index, bb->index);
				}
			      RBI (bb)->next = best_edge->dest;
			      bb = rotate_loop (best_edge, trace, *n_traces);
			    }
			}
		      else
			{
			  /* The loop has less than 4 iterations.  */

			  /* Check whether there is another edge from BB.  */
			  edge another_edge;
			  for (another_edge = bb->succ;
			       another_edge;
			       another_edge = another_edge->succ_next)
			    if (another_edge != best_edge)
			      break;
				
			  if (!another_edge && copy_bb_p (best_edge->dest,
							  !optimize_size))
			    {
			      bb = copy_bb (best_edge->dest, best_edge, bb,
					    *n_traces);
			    }
			}
		    }

		  /* Terminate the trace.  */
		  break;
		}
	      else
		{
		  /* Check for a situation

		    A
		   /|
		  B |
		   \|
		    C

		  where
		  EDGE_FREQUENCY (AB) + EDGE_FREQUENCY (BC)
		    >= EDGE_FREQUENCY (AC).
		  (i.e. 2 * B->frequency >= EDGE_FREQUENCY (AC) )
		  Best ordering is then A B C.

		  This situation is created for example by:

		  if (A) B;
		  C;

		  */

		  for (e = bb->succ; e; e = e->succ_next)
		    if (e != best_edge
			&& (e->flags & EDGE_CAN_FALLTHRU)
			&& !(e->flags & EDGE_COMPLEX)
			&& !RBI (e->dest)->visited
			&& !e->dest->pred->pred_next
			&& e->dest->succ
			&& (e->dest->succ->flags & EDGE_CAN_FALLTHRU)
			&& !(e->dest->succ->flags & EDGE_COMPLEX)
			&& !e->dest->succ->succ_next
			&& e->dest->succ->dest == best_edge->dest
			&& 2 * e->dest->frequency >= EDGE_FREQUENCY (best_edge))
		      {
			best_edge = e;
			if (rtl_dump_file)
			  fprintf (rtl_dump_file, "Selecting BB %d\n",
				   best_edge->dest->index);
			break;
		      }

		  RBI (bb)->next = best_edge->dest;
		  bb = best_edge->dest;
		}
	    }
	}
      while (best_edge);
      trace->last = bb;
      start_of_trace[trace->first->index] = *n_traces - 1;
      end_of_trace[trace->last->index] = *n_traces - 1;

      /* The trace is terminated so we have to recount the keys in heap
	 (some block can have a lower key because now one of its predecessors
	 is an end of the trace).  */
      for (e = bb->succ; e; e = e->succ_next)
	{
	  if (e->dest == EXIT_BLOCK_PTR
	      || RBI (e->dest)->visited)
	    continue;
	  
	  bb_index = e->dest->index;
	  if (bb_heap[bb_index])
	    {
	      key = bb_to_key (e->dest);
	      if (key != bb_node[bb_index]->key)
		{
		  if (rtl_dump_file)
		    {
		      fprintf (rtl_dump_file,
			       "Changing key for bb %d from %ld to %ld.\n",
			       bb_index, (long) bb_node[bb_index]->key, key);
		    }
		  fibheap_replace_key (bb_heap[bb_index], bb_node[bb_index],
				       key);
		}
	    }
	}
    }

  fibheap_delete (*heap);

  /* "Return" the new heap.  */
  *heap = new_heap;
}

/* Create a duplicate of the basic block OLD_BB and redirect edge E to it, add
   it to trace after BB, mark OLD_BB visited and update pass' data structures
   (TRACE is a number of trace which OLD_BB is duplicated to).  */

static basic_block
copy_bb (old_bb, e, bb, trace)
     basic_block old_bb;
     edge e;
     basic_block bb;
     int trace;
{
  basic_block new_bb;

  new_bb = cfg_layout_duplicate_bb (old_bb, e);
  if (e->dest != new_bb)
    abort ();
  if (RBI (e->dest)->visited)
    abort ();
  if (rtl_dump_file)
    fprintf (rtl_dump_file,
	     "Duplicated bb %d (created bb %d)\n",
	     old_bb->index, new_bb->index);
  RBI (new_bb)->visited = trace;
  RBI (new_bb)->next = RBI (bb)->next;
  RBI (bb)->next = new_bb;

  if (new_bb->index >= array_size || last_basic_block > array_size)
    {
      int i;
      int new_size;
      
      new_size = MAX (last_basic_block, new_bb->index + 1);
      new_size = GET_ARRAY_SIZE (new_size);
      
      start_of_trace = xrealloc (start_of_trace, new_size * sizeof (int));
      end_of_trace = xrealloc (end_of_trace, new_size * sizeof (int));
      for (i = array_size; i < new_size; i++)
	{
	  start_of_trace[i] = -1;
	  end_of_trace[i] = -1;
	}
      if (bb_heap)
	{
	  bb_heap = xrealloc (bb_heap, new_size * sizeof (fibheap_t));
	  bb_node = xrealloc (bb_node, new_size * sizeof (fibnode_t));
	  for (i = array_size; i < new_size; i++)
	    {
	      bb_heap[i] = NULL;
	      bb_node[i] = NULL;
	    }
	}
      array_size = new_size;
      
      if (rtl_dump_file)
	{
	  fprintf (rtl_dump_file,
		   "Growing the dynamic arrays to %d elements.\n",
		   array_size);
	}
    }

  return new_bb;
}

/* Compute and return the key (for the heap) of the basic block BB.  */

static fibheapkey_t
bb_to_key (bb)
     basic_block bb;
{
  edge e;

  int priority = 2;

  /* Do not start in probably never executed blocks.  */
  if (probably_never_executed_bb_p (bb))
    return BB_FREQ_MAX;

  /* Decrease the priority if there is an unvisited predecessor.  */
  for (e = bb->pred; e; e = e->pred_next)
    if (!(e->flags & EDGE_DFS_BACK) && !RBI (e->src)->visited)
      {
	priority = 0;
	break;
      }

  /* Increase the priority if there is a predecessor which is an end of some
     trace.  */
  for (e = bb->pred; e; e = e->pred_next)
    {
      if (e->src != ENTRY_BLOCK_PTR && end_of_trace[e->src->index] >= 0)
	{
	  priority++;
	  break;
	}
    }

  return -100 * BB_FREQ_MAX * priority - bb->frequency;
}

/* Return true when the edge E from basic block BB is better than the temporary
   best edge (details are in function).  The probability of edge E is PROB. The
   frequency of the successor is FREQ.  The current best probability is
   BEST_PROB, the best frequency is BEST_FREQ.
   The edge is considered to be equivalent when PROB does not differ much from
   BEST_PROB; similarly for frequency.  */

static bool
better_edge_p (bb, e, prob, freq, best_prob, best_freq)
     basic_block bb;
     edge e;
     int prob;
     int freq;
     int best_prob;
     int best_freq;
{
  bool is_better_edge;

  /* The BEST_* values do not have to be best, but can be a bit smaller than
     maximum values.  */
  int diff_prob = best_prob / 10;
  int diff_freq = best_freq / 10;

  if (prob > best_prob + diff_prob)
    /* The edge has higher probability than the temporary best edge.  */
    is_better_edge = true;
  else if (prob < best_prob - diff_prob)
    /* The edge has lower probability than the temporary best edge.  */
    is_better_edge = false;
  else if (freq < best_freq - diff_freq)
    /* The edge and the temporary best edge  have almost equivalent
       probabilities.  The higher frequency of a successor now means
       that there is another edge going into that successor.
       This successor has lower frequency so it is better.  */
    is_better_edge = true;
  else if (freq > best_freq + diff_freq)
    /* This successor has higher frequency so it is worse.  */
    is_better_edge = false;
  else if (e->dest->prev_bb == bb)
    /* The edges have equivalent probabilities and the successors
       have equivalent frequencies.  Select the previous successor.  */
    is_better_edge = true;
  else
    is_better_edge = false;

  return is_better_edge;
}

/* Connect traces in array TRACES, N_TRACES is the count of traces.  */

static void
connect_traces (n_traces, traces)
     int n_traces;
     struct trace *traces;
{
  int i;
  bool *connected;
  int last_trace;
  int freq_threshold;
  gcov_type count_threshold;

  freq_threshold = max_entry_frequency * DUPLICATION_THRESHOLD / 1000;
  if (max_entry_count < INT_MAX / 1000)
    count_threshold = max_entry_count * DUPLICATION_THRESHOLD / 1000;
  else
    count_threshold = max_entry_count / 1000 * DUPLICATION_THRESHOLD;

  connected = xcalloc (n_traces, sizeof (bool));
  last_trace = -1;
  for (i = 0; i < n_traces; i++)
    {
      int t = i;
      int t2;
      edge e, best;
      int best_len;

      if (connected[t])
	continue;

      connected[t] = true;

      /* Find the predecessor traces.  */
      for (t2 = t; t2 > 0;)
	{
	  best = NULL;
	  best_len = 0;
	  for (e = traces[t2].first->pred; e; e = e->pred_next)
	    {
	      int si = e->src->index;

	      if (e->src != ENTRY_BLOCK_PTR
		  && (e->flags & EDGE_CAN_FALLTHRU)
		  && !(e->flags & EDGE_COMPLEX)
		  && end_of_trace[si] >= 0
		  && !connected[end_of_trace[si]]
		  && (!best 
		      || e->probability > best->probability
		      || (e->probability == best->probability
			  && traces[end_of_trace[si]].length > best_len)))
		{
		  best = e;
		  best_len = traces[end_of_trace[si]].length;
		}
	    }
	  if (best)
	    {
	      RBI (best->src)->next = best->dest;
	      t2 = end_of_trace[best->src->index];
	      connected[t2] = true;
	      if (rtl_dump_file)
		{
		  fprintf (rtl_dump_file, "Connection: %d %d\n",
			   best->src->index, best->dest->index);
		}
	    }
	  else
	    break;
	}

      if (last_trace >= 0)
	RBI (traces[last_trace].last)->next = traces[t2].first;
      last_trace = t;

      /* Find the successor traces.  */
      while (1)
	{
	  /* Find the continuation of the chain.  */
	  best = NULL;
	  best_len = 0;
	  for (e = traces[t].last->succ; e; e = e->succ_next)
	    {
	      int di = e->dest->index;

	      if (e->dest != EXIT_BLOCK_PTR
		  && (e->flags & EDGE_CAN_FALLTHRU)
		  && !(e->flags & EDGE_COMPLEX)
		  && start_of_trace[di] >= 0
		  && !connected[start_of_trace[di]]
		  && (!best
		      || e->probability > best->probability
		      || (e->probability == best->probability
			  && traces[start_of_trace[di]].length > best_len)))
		{
		  best = e;
		  best_len = traces[start_of_trace[di]].length;
		}
	    }

	  if (best)
	    {
	      if (rtl_dump_file)
		{
		  fprintf (rtl_dump_file, "Connection: %d %d\n",
			   best->src->index, best->dest->index);
		}
	      t = start_of_trace[best->dest->index];
	      RBI (traces[last_trace].last)->next = traces[t].first;
	      connected[t] = true;
	      last_trace = t;
	    }
	  else
	    {
	      /* Try to connect the traces by duplication of 1 block.  */
	      edge e2;
	      basic_block next_bb = NULL;

	      for (e = traces[t].last->succ; e; e = e->succ_next)
		if (e->dest != EXIT_BLOCK_PTR
		    && (e->flags & EDGE_CAN_FALLTHRU)
		    && !(e->flags & EDGE_COMPLEX)
		    && (EDGE_FREQUENCY (e) >= freq_threshold)
		    && (e->count >= count_threshold)
		    && (!best 
			|| e->probability > best->probability))
		  {
		    edge best2 = NULL;
		    int best2_len = 0;

		    for (e2 = e->dest->succ; e2; e2 = e2->succ_next)
		      {
			int di = e2->dest->index;

			if (e2->dest == EXIT_BLOCK_PTR
			    || ((e2->flags & EDGE_CAN_FALLTHRU)
				&& !(e2->flags & EDGE_COMPLEX)
				&& start_of_trace[di] >= 0
				&& !connected[start_of_trace[di]]
				&& (EDGE_FREQUENCY (e2) >= freq_threshold)
				&& (e2->count >= count_threshold)
				&& (!best2
				    || e2->probability > best2->probability
				    || (e2->probability == best2->probability
					&& traces[start_of_trace[di]].length
					   > best2_len))))
			  {
			    best = e;
			    best2 = e2;
			    if (e2->dest != EXIT_BLOCK_PTR)
			      best2_len = traces[start_of_trace[di]].length;
			    else
			      best2_len = INT_MAX;
			    next_bb = e2->dest;
			  }
		      }
		  }
	      if (best && next_bb && copy_bb_p (best->dest, !optimize_size))
		{
		  basic_block new_bb;

		  if (rtl_dump_file)
		    {
		      fprintf (rtl_dump_file, "Connection: %d %d ",
			       traces[t].last->index, best->dest->index);
		      if (next_bb == EXIT_BLOCK_PTR)
			fprintf (rtl_dump_file, "exit\n");
		      else
			fprintf (rtl_dump_file, "%d\n", next_bb->index);
		    }

		  new_bb = copy_bb (best->dest, best, traces[t].last, t);
		  traces[t].last = new_bb;
		  if (next_bb != EXIT_BLOCK_PTR)
		    {
		      t = start_of_trace[next_bb->index];
		      RBI (traces[last_trace].last)->next = traces[t].first;
		      connected[t] = true;
		      last_trace = t;
		    }
		  else
		    break;	/* Stop finding the successor traces.  */
		}
	      else
		break;	/* Stop finding the successor traces.  */
	    }
	}
    }

  if (rtl_dump_file)
    {
      basic_block bb;

      fprintf (rtl_dump_file, "Final order:\n");
      for (bb = traces[0].first; bb; bb = RBI (bb)->next)
	fprintf (rtl_dump_file, "%d ", bb->index);
      fprintf (rtl_dump_file, "\n");
      fflush (rtl_dump_file);
    }

  FREE (connected);
  FREE (end_of_trace);
  FREE (start_of_trace);
}

/* Return true when BB can and should be copied. CODE_MAY_GROW is true
   when code size is allowed to grow by duplication.  */

static bool
copy_bb_p (bb, code_may_grow)
     basic_block bb;
     int code_may_grow;
{
  int size = 0;
  int max_size = uncond_jump_length;
  rtx insn;

  if (!bb->frequency)
    return false;
  if (!bb->pred || !bb->pred->pred_next)
    return false;
  if (!cfg_layout_can_duplicate_bb_p (bb))
    return false;

  if (code_may_grow && maybe_hot_bb_p (bb))
    max_size *= 8;

  for (insn = bb->head; insn != NEXT_INSN (bb->end);
       insn = NEXT_INSN (insn))
    {
      if (INSN_P (insn))
	size += get_attr_length (insn);
    }

  if (size <= max_size)
    return true;

  if (rtl_dump_file)
    {
      fprintf (rtl_dump_file,
	       "Block %d can't be copied because its size = %d.\n",
	       bb->index, size);
    }

  return false;
}

/* Return the maximum length of unconditional jump instruction.  */

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

/* Returns a cost of unconditional jump corresponding to edge E.  */

static int
cost_of_uncond_jump (edge e)
{
  if (e->dest == EXIT_BLOCK_PTR)
    return 0;

  if (optimize_size)
    return 1;

  /* If the area is cold, do flat optimization for size, with low
     priority.  */
  if (probably_cold_bb_p (e->src))
    return JUMP_COST;

  return JUMP_COST * e->src->frequency;
}

/* Returns a cost of conditional jump corresponding to edge E.  */

static int
cost_of_cond_jump (edge e)
{
  int cost = 0;

  if (optimize_size
      || probably_cold_bb_p (e->src))
    return 0;

  /* We assume the more probable branch is predicted.  */
  if (2 * e->probability > REG_BR_PROB_BASE)
    {
      /* E is predicted to be taken.  */
      cost += EDGE_FREQUENCY (e) * HIT_TAKEN_COST;
      cost += (e->src->frequency - EDGE_FREQUENCY (e)) * MISS_FALLTHRU_COST;
    }
  else
    {
      /* E is predicted not to be taken.  */
      cost += EDGE_FREQUENCY (e) * MISS_TAKEN_COST;

      /* HIT_FALLTHRU has no cost.  */
    }

  return cost;
}

/* Return a cost of jumps from basic block BB to two locations not adjacent
   to it.  */
static int
cost_of_cond_branch (basic_block bb)
{
  int more_freq, less_freq, tmp, cost = 0;

  if (optimize_size)
    return 1;

  /* If the area is cold, do flat optimization for size, with low
     priority.  */
  if (probably_cold_bb_p (bb))
    return JUMP_COST;

  /* It is assumed to be transformed into

     bb  -- helper
      \           \
       \           \
        more        less  */

  more_freq = EDGE_FREQUENCY (bb->succ);
  less_freq = EDGE_FREQUENCY (bb->succ->succ_next);

  if (less_freq > more_freq)
    {
      tmp = less_freq;
      less_freq = more_freq;
      more_freq = tmp;
    }

  cost += more_freq * HIT_TAKEN_COST;
  cost += less_freq * (MISS_FALLTHRU_COST + JUMP_COST);

  return cost;
}

/* Returns cost for placing B immediately after A.  */
static int
cost_b_after_a (basic_block a, basic_block b)
{
  if (!a->succ)
    return 0;

  if (!a->succ->succ_next)
    return a->succ->dest == b ? 0 : cost_of_uncond_jump (a->succ);

  if (!any_condjump_p (a->end))
    return 0;

  if (a->succ->succ_next->succ_next)
    abort ();

  if (a->succ->dest == b)
    return cost_of_cond_jump (a->succ->succ_next);
  
  if (a->succ->succ_next->dest == b)
    return cost_of_cond_jump (a->succ);

  return cost_of_cond_branch (a);
}

/* Record costs for edges coming out of FROM to VERTEX; LAST is true
   if it is the tail of the considered segment.  */
static void
record_edges (basic_block from, struct vertex *vertex, int last)
{
  edge e;
  basic_block tgt;

  vertex->n_spec = 0;

  if (last
      || !from->succ
      || (from->succ->succ_next
	  && !any_condjump_p (from->end)))
    {
      vertex->def_cost = 0;
      return;
    }

  for (e = from->succ; e; e = e->succ_next)
    {
      tgt = e->dest;
      if (tgt == EXIT_BLOCK_PTR)
	{
	  if (vertex->n_spec != 0)
	    abort ();
	  vertex->def_cost = 0;
	  return;
	}

      /* We indeed want <= here, as edges to the head of the current segment
	 are irrelevant.  */
      if (RBI (tgt)->visited <= 0)
	continue;

      if (tgt == from)
	continue;

      vertex->spec_tgt[vertex->n_spec] = RBI (tgt)->visited;
      vertex->spec_cost[vertex->n_spec] = cost_b_after_a (from, tgt);
      vertex->n_spec++;
    }
  if (vertex->n_spec > 2)
    abort ();

  vertex->def_cost = (from->succ->succ_next
		      ? cost_of_cond_branch (from)
		      : cost_of_uncond_jump (from->succ));
}

/* Reorder blocks using tsp solver.  */

static void
reorder_using_tsp ()
{
  basic_block block[MAX_TSP_SIZE + 2];
  struct vertex graph[MAX_TSP_SIZE + 2];
  int tour[MAX_TSP_SIZE + 2];
  basic_block start, old_start;
  int n, i, a;
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      RBI (bb)->visited = -1;
    }
  if (n_basic_blocks <= 1)
    return;

#define NEXT_BLOCK(BB)			\
  ((BB) == EXIT_BLOCK_PTR		\
   ? NULL				\
   : (RBI (BB)->next == NULL		\
      ? EXIT_BLOCK_PTR			\
      : RBI (BB)->next))
  start = ENTRY_BLOCK_PTR->next_bb;
  while (1)
    {
      old_start = start;

      if (rtl_dump_file)
	fprintf (rtl_dump_file, "Old order:");
      for (n = 0;
	   start && n < MAX_TSP_SIZE + 2;
	   n++, start = NEXT_BLOCK (start))
	{
	  block[n] = start;
	  RBI (start)->visited = n;
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, " %d", start->index);
	}
      if (rtl_dump_file)
	fprintf (rtl_dump_file, "\n");

      for (i = 0; i < n; i++)
	{
	  record_edges (block[i], graph + i, i == n - 1);
	  tour[i] = i + 1;
	}
      solve_tsp (n, tour, graph, optimize >= 3);
      
      if (block[n - 1] == EXIT_BLOCK_PTR)
	block[n - 1] = NULL;

      if (rtl_dump_file)
	fprintf (rtl_dump_file, "New order:");
      for (i = 0, a = 0; i < n - 1; i++, a = tour[a])
	{
	  RBI (block[a])->visited = -1;
	  RBI (block[a])->next = block[tour[a]];

	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, " %d", block[a]->index);
	}
      if (block[a])
	{
	  RBI (block[a])->visited = -1;
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file, " %d\n", block[a]->index);
	}
      else if (rtl_dump_file)
	fprintf (rtl_dump_file, " EXIT\n");

      if (!start)
	break;

      /* Ensure some overlap between the instances.  */
      start = old_start;
      for (; n > 0; n -= 2)
	start = NEXT_BLOCK (start);
    }
#undef NEXT_BLOCK
}

/* Reorder basic blocks.  The main entry point to this file.  */

void
reorder_basic_blocks ()
{
  int n_traces;
  struct trace *traces;

  if (n_basic_blocks <= 1)
    return;

  if ((* targetm.cannot_modify_jumps_p) ())
    return;

  cfg_layout_initialize (NULL);

  set_edge_can_fallthru_flag ();
  mark_dfs_back_edges ();
  uncond_jump_length = get_uncond_jump_length ();

  traces = xmalloc (n_basic_blocks * sizeof (struct trace));
  n_traces = 0;
  find_traces (&n_traces, traces);
  connect_traces (n_traces, traces);
  FREE (traces);

  if (rtl_dump_file)
    dump_flow_info (rtl_dump_file);

  if (flag_tsp_ordering)
    reorder_using_tsp ();

  cfg_layout_finalize ();
}
