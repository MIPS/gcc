/* Inlining decision heuristics.
   Copyright (C) 2003, 2004 Free Software Foundation, Inc.
   Contributed by Jan Hubicka

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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/*  Inlining decision heuristics

    We separate inlining decisions from the inliner itself and store it
    inside callgraph as so called inline plan.  Refer to cgraph.c
    documentation about particular representation of inline plans in the
    callgraph.

    There are three major parts of this file:

    cgraph_mark_inline implementation

      This function allow to mark given call inline and performs neccesary
      modifications of cgraph (production of the clones and updating overall
      statistics)

    inlining heuristics limits

      These functions allow to check that particular inlining is allowed
      by the limits specified by user (allowed function growth, overall unit
      growth and so on).

    inlining heuristics

      This is implementation of IPA pass aiming to get as much of benefit
      from inlining obeying the limits checked above.

      The implementation of particular heuristics is separated from
      the rest of code to make it easier to replace it with more complicated
      implementation in the future.  The rest of inlining code acts as a
      library aimed to modify the callgraph and verify that the parameters
      on code size growth fits.

      To mark given call inline, use cgraph_mark_inline function, the
      verification is performed by cgraph_default_inline_p and
      cgraph_check_inline_limits.

      The heuristics implements simple knapsack style algorithm ordering
      all functions by their "profitability" (estimated by code size growth)
      and inlining them in priority order.

      cgraph_decide_inlining implements heuristics taking whole callgraph
      into account, while cgraph_decide_inlining_incrementally considers
      only one function at a time and is used in non-unit-at-a-time mode.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-inline.h"
#include "langhooks.h"
#include "flags.h"
#include "cgraph.h"
#include "diagnostic.h"
#include "timevar.h"
#include "params.h"
#include "fibheap.h"
#include "intl.h"
#include "tree-pass.h"
#include "coverage.h"

#define INSNS_PER_CALL 10

/* Statistics we collect about inlining algorithm.  */
static int ncalls_inlined;
static int nfunctions_inlined;
static int initial_insns;
static int overall_insns;
static HOST_WIDEST_INT max_insns;

static struct cgraph_node *already_cloned;

/* Estimate size of the function after inlining WHAT into TO.  */

static int
cgraph_estimate_size_after_inlining (int times, struct cgraph_node *to,
				     struct cgraph_node *what)
{
  int size;
  size = (what->global.insns - INSNS_PER_CALL) * times + to->global.insns;
  gcc_assert (size >= 0);
  return size;
}

/* E is expected to be an edge being inlined.  Clone destination node of
   the edge and redirect it to the new clone.
   DUPLICATE is used for bookkeeping on whether we are actually creating new
   clones or re-using node originally representing out-of-line function call.
   */
static void
cgraph_clone_inlined_nodes_1 (struct cgraph_edge *e, bool duplicate)
{
  struct cgraph_node *n = e->callee;
  struct cgraph_edge *step_edge;

  if (e->callee->aux)
    abort();

  /* We may eliminate the need for out-of-line copy to be output.  In that
     case just go ahead and re-use it.  */
  if (!e->callee->callers->next_caller
      && (!e->callee->needed || DECL_EXTERNAL (e->callee->decl))
      && duplicate
      && flag_unit_at_a_time)
    {
      gcc_assert (!e->callee->global.inlined_to);
      if (!DECL_EXTERNAL (e->callee->decl))
        overall_insns -= e->callee->global.insns, nfunctions_inlined++;
      duplicate = 0;
    }
   else if (duplicate)
    {
      e->callee->aux = already_cloned;
      already_cloned = e->callee;
      n = cgraph_clone_node (e->callee);
      cgraph_redirect_edge_callee (e, n);
    }

  if (e->caller->global.inlined_to)
    n->global.inlined_to = e->caller->global.inlined_to;
  else
    n->global.inlined_to = e->caller;

  /* Recursively clone all bodies.  Non-zero aux means we've handled
     this edge already; skip it to avoid confusing ourselves.  */
  for (step_edge = n->callees; step_edge; step_edge = step_edge->next_callee)
    if (!step_edge->inline_failed && !step_edge->callee->aux)
      cgraph_clone_inlined_nodes_1 (step_edge, duplicate);
}

/* E is expected to be an edge being inlined.  Clone destination node of
   the edge and redirect it to the new clone.
   DUPLICATE is used for bookeeping on whether we are actually creating new
   clones or re-using node originally representing out-of-line function call.
   */
void
cgraph_clone_inlined_nodes (struct cgraph_edge *e, bool duplicate)
{
  struct cgraph_node *next_node;
  struct cgraph_node *step_node;
  struct cgraph_node end_node;        /* A non-NULL end-of-chain marker.  */
  
  end_node.aux = (struct cgraph_node *)0;
  already_cloned = &end_node;
  cgraph_clone_inlined_nodes_1 (e, duplicate);
  for (step_node = already_cloned; step_node != &end_node; step_node = next_node)
    {
      next_node = step_node->aux;
      step_node->aux = (struct cgraph_node *)0;
    }
  already_cloned = (struct cgraph_node *)0;
}

/* Mark edge E as inlined and update callgraph accordingly.  */

void
cgraph_mark_inline_edge (struct cgraph_edge *e)
{
  int old_insns = 0, new_insns = 0;
  struct cgraph_node *to = NULL, *what;
  bool simple_recursion = (e->callee == e->caller);

  gcc_assert (e->inline_failed);
  e->inline_failed = NULL;

  if (!e->callee->global.inlined && flag_unit_at_a_time)
    DECL_POSSIBLY_INLINED (e->callee->decl) = true;
  e->callee->global.inlined = true;

  cgraph_clone_inlined_nodes (e, true);

  what = e->callee;
  if (simple_recursion)
    {
      struct cgraph_edge* e2;
      /* New edge from original to copy should be marked !inline_failed, but
	 edge(s) back from copy to original (which we just created by
	 cloning the original->copy edge) should not be.  Indirect
	 recursion doesn't have the problem. */
      for (e2 = what->callees; e2; e2 = e2->next_callee)
	if (e2->callee == e->caller)
	  e2->inline_failed = N_("function not considered for inlining");
    }

  /* Install the just-cloned cgraph_node on the list.  */

  /* Now update size of caller and all functions caller is inlined into.  */
  for (;e && !e->inline_failed; e = e->caller->callers)
    {
      old_insns = e->caller->global.insns;
      new_insns = cgraph_estimate_size_after_inlining (1, e->caller,
						       what);
      gcc_assert (new_insns >= 0);
      to = e->caller;
      to->global.insns = new_insns;
    }
  gcc_assert (what->global.inlined_to == to);
  overall_insns += new_insns - old_insns;
  ncalls_inlined++;
}

/* Mark all calls of EDGE->CALLEE inlined into EDGE->CALLER.
   Return following unredirected edge in the list of callers
   of EDGE->CALLEE  */

static struct cgraph_edge *
cgraph_mark_inline (struct cgraph_edge *edge)
{
  struct cgraph_node *to = edge->caller;
  struct cgraph_node *what = edge->callee;
  struct cgraph_edge *e, *next;
  int times = 0;

  /* Look for all calls, mark them inline and clone recursively
     all inlined functions.  */
  for (e = what->callers; e; e = next)
    {
      next = e->next_caller;
      if (e->caller == to && e->inline_failed)
	{
          cgraph_mark_inline_edge (e);
	  if (e == edge)
	    edge = next;
	  times++;
	}
    }
  gcc_assert (times);
  return edge;
}


/* Estimate the growth caused by inlining NODE into all callees.  */

static int
cgraph_estimate_growth (struct cgraph_node *node)
{
  int growth = 0;
  struct cgraph_edge *e;

  for (e = node->callers; e; e = e->next_caller)
    if (e->inline_failed)
      growth += (cgraph_estimate_size_after_inlining (1, e->caller, node)
		 - e->caller->global.insns);

  /* ??? Wrong for self recursive functions or cases where we decide to not
     inline for different reasons, but it is not big deal as in that case
     we will keep the body around, but we will also avoid some inlining.  */
  if (!node->needed && !DECL_EXTERNAL (node->decl))
    growth -= node->global.insns;

  return growth;
}


/* Return false when inlining WHAT into TO is not good idea
   as it would cause too large growth of function bodies.  */

static bool
cgraph_check_inline_limits (struct cgraph_node *to, struct cgraph_node *what,
			    const char **reason)
{
  int times = 0;
  struct cgraph_edge *e;
  int newsize;
  int limit;

  if (to->global.inlined_to)
    to = to->global.inlined_to;

  for (e = to->callees; e; e = e->next_callee)
    if (e->callee == what)
      times++;

  /* When inlining large function body called once into small function,
     take the inlined function as base for limiting the growth.  */
  if (to->local.self_insns > what->local.self_insns)
    limit = to->local.self_insns;
  else
    limit = what->local.self_insns;

  limit += limit * PARAM_VALUE (PARAM_LARGE_FUNCTION_GROWTH) / 100;

  newsize = cgraph_estimate_size_after_inlining (times, to, what);
  if (newsize > PARAM_VALUE (PARAM_LARGE_FUNCTION_INSNS)
      && newsize > limit)
    {
      if (reason)
        *reason = N_("--param large-function-growth limit reached");
      return false;
    }
  return true;
}

/* Return true when function N is small enough to be inlined.  */

bool
cgraph_default_inline_p (struct cgraph_node *n)
{
  if (!DECL_INLINE (n->decl) || !DECL_SAVED_TREE (n->decl))
    return false;
  if (DECL_DECLARED_INLINE_P (n->decl))
    return n->global.insns < MAX_INLINE_INSNS_SINGLE;
  else
    return n->global.insns < MAX_INLINE_INSNS_AUTO;
}

/* Return true when inlining WHAT would create recursive inlining.
   We call recursive inlining all cases where same function appears more than
   once in the single recursion nest path in the inline graph.  */

static bool
cgraph_recursive_inlining_p (struct cgraph_node *to,
			     struct cgraph_node *what,
			     const char **reason)
{
  bool recursive;
  if (to->global.inlined_to)
    recursive = what->decl == to->global.inlined_to->decl;
  else
    recursive = what->decl == to->decl;
  /* Marking recursive function inline has sane semantic and thus we should
     not warn on it.  */
  if (recursive && reason)
    *reason = (what->local.disregard_inline_limits
	       ? N_("recursive inlining") : "");
  return recursive;
}

/* Recompute heap nodes for each of callees.  */

static void
update_callee_keys (fibheap_t heap, struct fibnode **heap_node,
		    struct cgraph_node *node)
{
  struct cgraph_edge *e;

  for (e = node->callees; e; e = e->next_callee)
    if (e->inline_failed && heap_node[e->callee->uid])
      fibheap_replace_key (heap, heap_node[e->callee->uid],
			   cgraph_estimate_growth (e->callee));
    else if (!e->inline_failed)
      update_callee_keys (heap, heap_node, e->callee);
}

/* Enqueue all recursive calls from NODE into queue linked via aux pointers
   in between FIRST and LAST.  WHERE is used for bookkeeping while looking
   int calls inlined within NODE.  */

static void
lookup_recursive_calls (struct cgraph_node *node, struct cgraph_node *where,
			struct cgraph_edge **first, struct cgraph_edge **last)
{
  struct cgraph_edge *e;
  for (e = where->callees; e; e = e->next_callee)
    if (e->callee == node)
      {
	if (!*first)
	  *first = e;
	else
	  (*last)->aux = e;
	*last = e;
      }
  for (e = where->callees; e; e = e->next_callee)
    if (!e->inline_failed)
      lookup_recursive_calls (node, e->callee, first, last);
}

/* Decide on recursive inlining: in the case function has recursive calls,
   inline until body size reaches given argument.  */

static void
cgraph_decide_recursive_inlining (struct cgraph_node *node)
{
  int limit = PARAM_VALUE (PARAM_MAX_INLINE_INSNS_RECURSIVE_AUTO);
  int max_depth = PARAM_VALUE (PARAM_MAX_INLINE_RECURSIVE_DEPTH_AUTO);
  struct cgraph_edge *first_call = NULL, *last_call = NULL;
  struct cgraph_edge *last_in_current_depth;
  struct cgraph_edge *e;
  struct cgraph_node *master_clone;
  int depth = 0;
  int n = 0;

  if (DECL_DECLARED_INLINE_P (node->decl))
    {
      limit = PARAM_VALUE (PARAM_MAX_INLINE_INSNS_RECURSIVE);
      max_depth = PARAM_VALUE (PARAM_MAX_INLINE_RECURSIVE_DEPTH);
    }

  /* Make sure that function is small enough to be considered for inlining.  */
  if (!max_depth
      || node->global.insns < INSNS_PER_CALL
      || cgraph_estimate_size_after_inlining (1, node, node)  >= limit)
    return;
  lookup_recursive_calls (node, node, &first_call, &last_call);
  if (!first_call)
    return;

  if (dump_file)
    fprintf (dump_file, 
	     "\nPerforming recursive inlining on %s\n",
	     cgraph_node_name (node));

  /* We need original clone to copy around.  */
  master_clone = cgraph_clone_node (node);
  master_clone->needed = true;
  for (e = master_clone->callees; e; e = e->next_callee)
    if (!e->inline_failed)
      cgraph_clone_inlined_nodes (e, true);

  /* Do the inlining and update list of recursive call during process.  */
  last_in_current_depth = last_call;
  while (first_call
	 && cgraph_estimate_size_after_inlining (1, node, master_clone) <= limit)
    {
      struct cgraph_edge *curr = first_call;

      first_call = first_call->aux;
      curr->aux = NULL;

      cgraph_redirect_edge_callee (curr, master_clone);
      cgraph_mark_inline_edge (curr);
      lookup_recursive_calls (node, curr->callee, &first_call, &last_call);

      if (last_in_current_depth
	  && ++depth >= max_depth)
	break;
      n++;
    }

  /* Cleanup queue pointers.  */
  while (first_call)
    {
      struct cgraph_edge *next = first_call->aux;
      first_call->aux = NULL;
      first_call = next;
    }
  if (dump_file)
    fprintf (dump_file, 
	     "\n   Inlined %i times, body grown from %i to %i insns\n", n,
	     master_clone->global.insns, node->global.insns);

  /* Remove master clone we used for inlining.  We rely that clones inlined
     into master clone gets queued just before master clone so we don't
     need recursion.  */
  for (node = cgraph_nodes; node != master_clone;
       node = node->next)
    if (node->global.inlined_to == master_clone)
      cgraph_remove_node (node);
  cgraph_remove_node (master_clone);
}

/* Set inline_failed for all callers of given function to REASON.  */

static void
cgraph_set_inline_failed (struct cgraph_node *node, const char *reason)
{
  struct cgraph_edge *e;

  if (dump_file)
    fprintf (dump_file, "Inlining failed: %s\n", reason);
  for (e = node->callers; e; e = e->next_caller)
    if (e->inline_failed)
      e->inline_failed = reason;
}

/* Return true if the call can be hot.  */
static bool
cgraph_maybe_hot_edge_p (struct cgraph_edge *edge)
{
  if (profile_info && flag_branch_probabilities
      && (edge->count
	  < profile_info->sum_max / PARAM_VALUE (HOT_BB_COUNT_FRACTION)))
    return false;
  return true;
}

/* Given a call graph edge EDGE, return the "desirability" for inlining
   the callee into the caller.
   The desirability of a function body or a CALL_EXPR is a unit-less
   measure of how beneficial it would be to inline.  Generally, smaller
   bodies are more desirable, and CALL_EXPRs with higher block-counts are
   more desirable.  This function computes the "desirability" of a
   function body.  */

static gcov_type
cgraph_desirability (struct cgraph_edge *edge)
{
  HOST_WIDEST_INT desire;
  HOST_WIDEST_INT denominator;
  struct cgraph_node *callee = edge->callee;
  
  if (!callee->local.inlinable)
    return 0;
  
  /* FIXME: Ideally we'd replace this with a more sophisticated
     "temperature" sort of metric.  */
  denominator = edge->callee->global.insns - INSNS_PER_CALL;
  if (denominator > 0)
    desire = cgraph_maybe_hot_edge_p (edge) ? edge->count / denominator : 0;
  else
    desire = profile_info->sum_max;
  return desire;
}

/* Given a caller represented by NODE, find the edge that points to the
   callee that is the most desirable to inline in the caller.  */

static struct cgraph_edge *
cgraph_pick_most_desirable_edge (struct cgraph_node *node)
{
  struct cgraph_edge *step_edge;
  struct cgraph_edge *most_desirable_edge;
  HOST_WIDEST_INT highest_desire;
  
  highest_desire = 0;
  most_desirable_edge = (struct cgraph_edge *)0;
  for (step_edge = node->callees;
       step_edge;
       step_edge = step_edge->next_callee)
    {
      if (step_edge->inline_failed && !step_edge->undesirable
	  && (step_edge->desirability > highest_desire))
	{
	  highest_desire = step_edge->desirability;
	  most_desirable_edge = step_edge;
	}
    }
  return most_desirable_edge;
}

/* Decide what to inline using profile information.
   ???  Needs more explaining, htf does this work?!
   ???  This algorithm i *way* too expensive, can be done
	much cheaper, no doubt.  */

static void
cgraph_profile_driven_inlining (void)
{
  struct cgraph_node *step_node;
  struct cgraph_edge *step_edge;
  HOST_WIDEST_INT highest_desire;
  struct cgraph_edge *most_desirable_edge;
  struct cgraph_node *most_desirable_node;
  
  /* Pass 1: compute desirability of all CALL_EXPRs.  */
  for (step_node = cgraph_nodes;
       step_node;
       step_node = step_node->next)
    {
      highest_desire = 0;
      most_desirable_edge = (struct cgraph_edge *)0;
      for (step_edge = step_node->callees;
	   step_edge;
	   step_edge = step_edge->next_callee)
	if (step_edge->inline_failed)
	  {
	    step_edge->desirability = cgraph_desirability (step_edge);
	    if (step_edge->desirability > highest_desire)
	      {
		highest_desire = step_edge->desirability;
		most_desirable_edge = step_edge;
	      }
	  }
      step_node->most_desirable = most_desirable_edge;
    }
  /* Pass 2: pick edges to inline.  */
  while (overall_insns <= max_insns)
    {
      highest_desire = 0;
      most_desirable_node = (struct cgraph_node *)0;
      for (step_node = cgraph_nodes;
	   step_node;
	   step_node = step_node->next)
	{
	  if (step_node->most_desirable
	      && (step_node->most_desirable->desirability > highest_desire))
	    {
	      highest_desire = step_node->most_desirable->desirability;
	      most_desirable_node = step_node;
	    }
	}
      /* If we found a suitable CALL_EXPR, record our choice, else
	 abandon the search.  */
      if (most_desirable_node)
	{
	  struct cgraph_edge *e = most_desirable_node->most_desirable;
	  if (e->inline_failed
	      && cgraph_check_inline_limits (e->caller, e->callee,
			  		     &e->inline_failed))
	    cgraph_mark_inline_edge (e);
	  else
	    e->undesirable = true;
	  most_desirable_node->most_desirable = cgraph_pick_most_desirable_edge (most_desirable_node);
	}
      else
        break;
    }
}

/* We use greedy algorithm for inlining of small functions:
   All inline candidates are put into prioritized heap based on estimated
   growth of the overall number of instructions and then update the estimates.

   INLINED and INLINED_CALEES are just pointers to arrays large enough
   to be passed to cgraph_inlined_into and cgraph_inlined_callees.  */

static void
cgraph_decide_inlining_of_small_functions (void)
{
  struct cgraph_node *node;
  fibheap_t heap = fibheap_new ();
  struct fibnode **heap_node =
    xcalloc (cgraph_max_uid, sizeof (struct fibnode *));

  /* Put all inline candidates into the heap.  */

  for (node = cgraph_nodes; node; node = node->next)
    {
      if (!node->local.inlinable || !node->callers
	  || node->local.disregard_inline_limits)
	continue;

      if (!cgraph_default_inline_p (node))
	{
	  cgraph_set_inline_failed (node,
	    N_("--param max-inline-insns-single limit reached"));
	  continue;
	}
      heap_node[node->uid] =
	fibheap_insert (heap, cgraph_estimate_growth (node), node);
    }

  if (dump_file)
    fprintf (dump_file, "\nDeciding on smaller functions:\n");
  while (overall_insns <= max_insns && (node = fibheap_extract_min (heap)))
    {
      struct cgraph_edge *e, *next;
      int old_insns = overall_insns;

      heap_node[node->uid] = NULL;
      if (dump_file)
	fprintf (dump_file, 
		 "\nConsidering %s with %i insns\n"
		 " Estimated growth is %+i insns.\n",
		 cgraph_node_name (node), node->global.insns,
		 cgraph_estimate_growth (node));
      if (!cgraph_default_inline_p (node))
	{
	  cgraph_set_inline_failed (node,
	    N_("--param max-inline-insns-single limit reached after inlining into the callee"));
	  continue;
	}
      for (e = node->callers; e; e = next)
	{
	  next = e->next_caller;
	  if (e->inline_failed)
	    {
	      struct cgraph_node *where;

	      if (cgraph_recursive_inlining_p (e->caller, e->callee,
				      	       &e->inline_failed)
		  || !cgraph_check_inline_limits (e->caller, e->callee,
			  			  &e->inline_failed))
		{
		  if (dump_file)
		    fprintf (dump_file, " Not inlining into %s:%s.\n",
			     cgraph_node_name (e->caller), e->inline_failed);
		  continue;
		}
	      next = cgraph_mark_inline (e);
	      where = e->caller;
	      if (where->global.inlined_to)
		where = where->global.inlined_to;

	      if (heap_node[where->uid])
		fibheap_replace_key (heap, heap_node[where->uid],
				     cgraph_estimate_growth (where));

	      if (dump_file)
		fprintf (dump_file, 
			 " Inlined into %s which now has %i insns.\n",
			 cgraph_node_name (e->caller),
			 e->caller->global.insns);
	    }
	}

      cgraph_decide_recursive_inlining (node);

      /* Similarly all functions called by the function we just inlined
         are now called more times; update keys.  */
      update_callee_keys (heap, heap_node, node);

      if (dump_file)
	fprintf (dump_file, 
		 " Inlined for a net change of %+i insns.\n",
		 overall_insns - old_insns);
    }
  while ((node = fibheap_extract_min (heap)) != NULL)
    if (!node->local.disregard_inline_limits)
      cgraph_set_inline_failed (node, N_("--param inline-unit-growth limit reached"));
  fibheap_delete (heap);
  free (heap_node);
}

/* Decide on the inlining.  We do so in the topological order to avoid
   expenses on updating data structures.  */

static void
cgraph_decide_inlining (void)
{
  struct cgraph_node *node;
  int nnodes;
  struct cgraph_node **order =
    xcalloc (cgraph_n_nodes, sizeof (struct cgraph_node *));
  int old_insns = 0;
  int i;

  for (node = cgraph_nodes; node; node = node->next)
    initial_insns += node->local.self_insns;
  overall_insns = initial_insns;

  max_insns = ((HOST_WIDEST_INT) overall_insns
	       * (100 + PARAM_VALUE (PARAM_INLINE_UNIT_GROWTH)) / 100);

  nnodes = cgraph_postorder (order);

  if (dump_file)
    fprintf (dump_file,
	     "\nDeciding on inlining.  Starting with %i insns.\n",
	     initial_insns);

  for (node = cgraph_nodes; node; node = node->next)
    node->aux = 0;

  if (dump_file)
    fprintf (dump_file, "\nInlining always_inline functions:\n");

  /* In the first pass mark all always_inline edges.  Do this with a priority
     so none of our later choices will make this impossible.  */
  for (i = nnodes - 1; i >= 0; i--)
    {
      struct cgraph_edge *e, *next;

      node = order[i];

      if (!node->local.disregard_inline_limits)
	continue;
      if (dump_file)
	fprintf (dump_file,
		 "\nConsidering %s %i insns (always inline)\n",
		 cgraph_node_name (node), node->global.insns);
      old_insns = overall_insns;
      for (e = node->callers; e; e = next)
	{
	  next = e->next_caller;
	  if (!e->inline_failed)
	    continue;
	  if (cgraph_recursive_inlining_p (e->caller, e->callee,
				  	   &e->inline_failed))
	    continue;
	  cgraph_mark_inline_edge (e);
	  if (dump_file)
	    fprintf (dump_file, 
		     " Inlined into %s which now has %i insns.\n",
		     cgraph_node_name (e->caller),
		     e->caller->global.insns);
	}
      if (dump_file)
	fprintf (dump_file, 
		 " Inlined for a net change of %+i insns.\n",
		 overall_insns - old_insns);
    }

  if (!flag_really_no_inline)
    {
      /* If we have profiling information, use it to choose inlining candidates.  */
      if (profile_info)
	cgraph_profile_driven_inlining ();
      else
	cgraph_decide_inlining_of_small_functions ();

      if (dump_file)
	fprintf (dump_file, "\nDeciding on functions called once:\n");

      /* And finally decide what functions are called once.  */

      for (i = nnodes - 1; i >= 0; i--)
	{
	  node = order[i];

	  if (node->callers && !node->callers->next_caller && !node->needed
	      && node->local.inlinable && node->callers->inline_failed
	      && !DECL_EXTERNAL (node->decl) && !DECL_COMDAT (node->decl))
	    {
	      bool ok = true;
	      struct cgraph_node *node1;

	      /* Verify that we won't duplicate the caller.  */
	      for (node1 = node->callers->caller;
		   node1->callers && !node1->callers->inline_failed
		   && ok; node1 = node1->callers->caller)
		if (node1->callers->next_caller || node1->needed)
		  ok = false;
	      if (ok)
		{
		  if (dump_file)
		    fprintf (dump_file,
			     "\nConsidering %s %i insns.\n"
			     " Called once from %s %i insns.\n",
			     cgraph_node_name (node), node->global.insns,
			     cgraph_node_name (node->callers->caller),
			     node->callers->caller->global.insns);

		  old_insns = overall_insns;

		  if (cgraph_check_inline_limits (node->callers->caller, node,
					  	  NULL))
		    {
		      cgraph_mark_inline (node->callers);
		      if (dump_file)
			fprintf (dump_file,
				 " Inlined into %s which now has %i insns"
				 " for a net change of %+i insns.\n",
				 cgraph_node_name (node->callers->caller),
				 node->callers->caller->global.insns,
				 overall_insns - old_insns);
		    }
		  else
		    {
		      if (dump_file)
			fprintf (dump_file,
				 " Inline limit reached, not inlined.\n");
		    }
		}
	    }
	}
    }

  /* We will never output extern functions we didn't inline. 
     ??? Perhaps we can prevent accounting of growth of external
     inline functions.  */

  /* FIXME: at the moment we don't want to remove nodes here
     or we confuse aliasing code.  This needs to be dealt with
     better later by reorganizing the order of analysis.  

     Uncommenting this should make same call in cgraph_optimize unnecesary.
  cgraph_remove_unreachable_nodes (false, dump_file);  */

  if (dump_file)
    fprintf (dump_file,
	     "\nInlined %i calls, eliminated %i functions, "
	     "%i insns turned to %i insns.\n\n",
	     ncalls_inlined, nfunctions_inlined, initial_insns,
	     overall_insns);
  free (order);
}

/* Decide on the inlining.  We do so in the topological order to avoid
   expenses on updating data structures.  */

void
cgraph_decide_inlining_incrementally (struct cgraph_node *node)
{
  struct cgraph_edge *e;

  /* First of all look for always inline functions.  */
  for (e = node->callees; e; e = e->next_callee)
    if (e->callee->local.disregard_inline_limits
	&& e->inline_failed
        && !cgraph_recursive_inlining_p (node, e->callee, &e->inline_failed)
	/* ??? It is possible that renaming variable removed the function body
	   in duplicate_decls. See gcc.c-torture/compile/20011119-2.c  */
	&& DECL_SAVED_TREE (e->callee->decl))
      cgraph_mark_inline (e);

  /* Now do the automatic inlining.  */
  if (!flag_really_no_inline)
    for (e = node->callees; e; e = e->next_callee)
      if (e->callee->local.inlinable
	  && e->inline_failed
	  && !e->callee->local.disregard_inline_limits
	  && !cgraph_recursive_inlining_p (node, e->callee, &e->inline_failed)
	  && cgraph_check_inline_limits (node, e->callee, &e->inline_failed)
	  && DECL_SAVED_TREE (e->callee->decl))
	{
	  if (cgraph_default_inline_p (e->callee))
	    cgraph_mark_inline (e);
	  else
	    e->inline_failed
	      = N_("--param max-inline-insns-single limit reached");
	}
}

/* Compute function size and other parameters used by inlining arguments.  */

void
cgraph_analyze_function_inlinability (struct cgraph_node *node)
{
  tree decl = node->decl;
  struct cgraph_edge *e;

  node->local.inlinable = tree_inlinable_function_p (decl);
  node->local.self_insns = estimate_num_insns (decl);
  if (node->local.inlinable)
    node->local.disregard_inline_limits
      = lang_hooks.tree_inlining.disregard_inline_limits (decl);
  for (e = node->callers; e; e = e->next_caller)
    {
      if (node->local.redefined_extern_inline)
	e->inline_failed = N_("redefined extern inline functions are not "
			      "considered for inlining");
      else if (!node->local.inlinable)
	e->inline_failed = N_("function not inlinable");
      else
	e->inline_failed = N_("function not considered for inlining");
    }
  if (flag_really_no_inline && !node->local.disregard_inline_limits)
    node->local.inlinable = 0;
  /* Inlining characteristics are maintained by the cgraph_mark_inline.  */
  node->global.insns = node->local.self_insns;
}

static void
cgraph_apply_inline (struct cgraph_node *node)
{
  if (flag_inline_trees)
    {
      struct cgraph_edge *e;
      for (e = node->callees; e; e = e->next_callee)
	if (!e->inline_failed || warn_inline)
	  break;
      if (e)
	optimize_inline_calls (current_function_decl);
    }

  /* We are not going to maintain the cgraph edges up to date.
     Kill it so it won't confuse us.  */
  while (node->callees)
    {
      /* In non-unit-at-a-time we must mark all referenced functions as needed.
         */
      if (node->callees->callee->analyzed && !flag_unit_at_a_time)
        cgraph_mark_needed_node (node->callees->callee);
      cgraph_remove_edge (node->callees);
    }
}

static bool
cgraph_gate_inlining (void)
{
  return flag_inline_trees;
}

struct tree_opt_pass pass_ipa_inline = 
{
  "inline",				/* name */
  cgraph_gate_inlining,			/* gate */
  cgraph_analyze_function_inlinability,	/* function analysis */
  NULL,					/* variable analysis */
  cgraph_decide_inlining,		/* execute */
  cgraph_apply_inline,			/* function modification */
  NULL,					/* variable modification */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_INTEGRATION,			/* tv_id */
  0,	                                /* properties_required */
  PROP_trees,				/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_cgraph | TODO_dump_func,	/* todo_flags_finish */
  0					/* letter */
};
