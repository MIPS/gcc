/* Callgraph based intraprocedural optimizations.
   Copyright (C) 2003 Free Software Foundation, Inc.
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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-inline.h"
#include "langhooks.h"
#include "hashtab.h"
#include "toplev.h"
#include "flags.h"
#include "ggc.h"
#include "debug.h"
#include "target.h"
#include "cgraph.h"
#include "diagnostic.h"
#include "timevar.h"
#include "params.h"
#include "fibheap.h"
#include "c-common.h"

#define INSNS_PER_CALL 10

static void cgraph_expand_all_functions (void);
static void cgraph_mark_functions_to_output (void);
static void cgraph_expand_function (struct cgraph_node *);
static tree record_call_1 (tree *, int *, void *);
static void cgraph_mark_local_functions (void);
static bool cgraph_default_inline_p (struct cgraph_node *n);
static void cgraph_analyze_function (struct cgraph_node *node);
static void cgraph_decide_inlining_incrementally (struct cgraph_node *);

/* Statistics we collect about inlining algorithm.  */
static int ncalls_inlined;
static int nfunctions_inlined;
static int initial_insns;
static int overall_insns;

/* Records tree nodes seen in cgraph_create_edges.  Simply using
   walk_tree_without_duplicates doesn't guarantee each node is visited
   once because it gets a new htab upon each recursive call from
   record_calls_1.  */
static htab_t visited_nodes;

/* Determine if function DECL is needed.  That is, visible to something
   either outside this translation unit, something magic in the system
   configury, or (if not doing unit-at-a-time) to something we havn't
   seen yet.  */

static bool
decide_is_function_needed (struct cgraph_node *node, tree decl)
{
  /* If we decided it was needed before, but at the time we didn't have
     the body of the function available, then it's still needed.  We have
     to go back and re-check its dependencies now.  */
  if (node->needed)
    return true;

  /* Externally visible functions must be output.  The exception is
     COMDAT functions that must be output only when they are needed.  */
  if (TREE_PUBLIC (decl) && !DECL_COMDAT (decl) && !DECL_EXTERNAL (decl))
    return true;

  /* Constructors and destructors are reachable from the runtime by
     some mechanism.  */
  if (DECL_STATIC_CONSTRUCTOR (decl) || DECL_STATIC_DESTRUCTOR (decl))
    return true;

  /* If the user told us it is used, then it must be so.  */
  if (lookup_attribute ("used", DECL_ATTRIBUTES (decl)))
    return true;

  /* ??? If the assembler name is set by hand, it is possible to assemble
     the name later after finalizing the function and the fact is noticed
     in assemble_name then.  This is arguably a bug.  */
  if (DECL_ASSEMBLER_NAME_SET_P (decl)
      && TREE_SYMBOL_REFERENCED (DECL_ASSEMBLER_NAME (decl)))
    return true;

  if (flag_unit_at_a_time)
    return false;

  /* If not doing unit at a time, then we'll only defer this function
     if its marked for inlining.  Otherwise we want to emit it now.  */

  /* "extern inline" functions are never output locally.  */
  if (DECL_EXTERNAL (decl))
    return false;
  /* We want to emit COMDAT functions only when absolutely necessary.  */
  if (DECL_COMDAT (decl))
    return false;
  if (!DECL_INLINE (decl)
      || (!node->local.disregard_inline_limits
	  /* When declared inline, defer even the uninlinable functions.
	     This allows them to be eliminated when unused.  */
	  && !DECL_DECLARED_INLINE_P (decl) 
	  && (!node->local.inlinable || !cgraph_default_inline_p (node))))
    return true;

  return false;
}

/* When not doing unit-at-a-time, output all functions enqueued.
   Return true when such a functions were found.  */

bool
cgraph_assemble_pending_functions (void)
{
  bool output = false;

  if (flag_unit_at_a_time)
    return false;

  while (cgraph_nodes_queue)
    {
      struct cgraph_node *n = cgraph_nodes_queue;

      cgraph_nodes_queue = cgraph_nodes_queue->next_needed;
      n->next_needed = NULL;
      if (!n->origin && !n->global.inlined_to && !DECL_EXTERNAL (n->decl))
	{
	  cgraph_expand_function (n);
	  output = true;
	}
    }

  return output;
}

/* DECL has been parsed.  Take it, queue it, compile it at the whim of the
   logic in effect.  If NESTED is true, then our caller cannot stand to have
   the garbage collector run at the moment.  We would need to either create
   a new GC context, or just not compile right now.  */

void
cgraph_finalize_function (tree decl, bool nested)
{
  struct cgraph_node *node = cgraph_node (decl);

  if (node->local.finalized)
    {
      /* As an GCC extension we allow redefinition of the function.  The
	 semantics when both copies of bodies differ is not well defined.
	 We replace the old body with new body so in unit at a time mode
	 we always use new body, while in normal mode we may end up with
	 old body inlined into some functions and new body expanded and
	 inlined in others.
	 
	 ??? It may make more sense to use one body for inlining and other
	 body for expanding the function but this is difficult to do.  */

      /* If node->output is set, then this is a unit-at-a-time compilation
	 and we have already begun whole-unit analysis.  This is *not*
	 testing for whether we've already emitted the function.  That
	 case can be sort-of legitimately seen with real function 
	 redefinition errors.  I would argue that the front end should
	 never present us with such a case, but don't enforce that for now.  */
      if (node->output)
	abort ();

      /* Reset our datastructures so we can analyze the function again.  */
      memset (&node->local, 0, sizeof (node->local));
      memset (&node->global, 0, sizeof (node->global));
      memset (&node->rtl, 0, sizeof (node->rtl));
      node->analyzed = false;
      while (node->callees)
	cgraph_remove_edge (node->callees);

      /* We may need to re-queue the node for assembling in case
         we already proceeded it and ignored as not needed.  */
      if (node->reachable && !flag_unit_at_a_time)
	{
	  struct cgraph_node *n;

	  for (n = cgraph_nodes_queue; n; n = n->next_needed)
	    if (n == node)
	      break;
	  if (!n)
	    node->reachable = 0;
	}
    }

  notice_global_symbol (decl);
  node->decl = decl;
  node->local.finalized = true;

  /* If not unit at a time, then we need to create the call graph
     now, so that called functions can be queued and emitted now.  */
  if (!flag_unit_at_a_time)
    {
      cgraph_analyze_function (node);
      cgraph_decide_inlining_incrementally (node);
    }

  if (decide_is_function_needed (node, decl))
    cgraph_mark_needed_node (node);

  /* If not unit at a time, go ahead and emit everything we've found
     to be reachable at this time.  */
  if (!nested)
    cgraph_assemble_pending_functions ();

  /* If we've not yet emitted decl, tell the debug info about it.  */
  if (!TREE_ASM_WRITTEN (decl))
    (*debug_hooks->deferred_inline_function) (decl);
}

/* Walk tree and record all calls.  Called via walk_tree.  */
static tree
record_call_1 (tree *tp, int *walk_subtrees, void *data)
{
  tree t = *tp;

  switch (TREE_CODE (t))
    {
    case VAR_DECL:
      /* ??? Really, we should mark this decl as *potentially* referenced
	 by this function and re-examine whether the decl is actually used
	 after rtl has been generated.  */
      if (TREE_STATIC (t))
        cgraph_varpool_mark_needed_node (cgraph_varpool_node (t));
      break;

    case ADDR_EXPR:
      if (flag_unit_at_a_time)
	{
	  /* Record dereferences to the functions.  This makes the
	     functions reachable unconditionally.  */
	  tree decl = TREE_OPERAND (*tp, 0);
	  if (TREE_CODE (decl) == FUNCTION_DECL)
	    cgraph_mark_needed_node (cgraph_node (decl));
	}
      break;

    case CALL_EXPR:
      {
	tree decl = get_callee_fndecl (*tp);
	if (decl && TREE_CODE (decl) == FUNCTION_DECL)
	  {
	    cgraph_create_edge (data, cgraph_node (decl), *tp);

	    /* When we see a function call, we don't want to look at the
	       function reference in the ADDR_EXPR that is hanging from
	       the CALL_EXPR we're examining here, because we would
	       conclude incorrectly that the function's address could be
	       taken by something that is not a function call.  So only
	       walk the function parameter list, skip the other subtrees.  */

	    walk_tree (&TREE_OPERAND (*tp, 1), record_call_1, data,
		       visited_nodes);
	    *walk_subtrees = 0;
	  }
	break;
      }

    default:
      /* Save some cycles by not walking types and declaration as we
	 won't find anything useful there anyway.  */
      if (DECL_P (*tp) || TYPE_P (*tp))
	{
	  *walk_subtrees = 0;
	  break;
	}

      if ((unsigned int) TREE_CODE (t) >= LAST_AND_UNUSED_TREE_CODE)
	abort ();
      break;
    }

  return NULL;
}

/* Create cgraph edges for function calls inside BODY from NODE.  */

void
cgraph_create_edges (struct cgraph_node *node, tree body)
{
  /* The nodes we're interested in are never shared, so walk
     the tree ignoring duplicates.  */
  visited_nodes = htab_create (37, htab_hash_pointer,
				    htab_eq_pointer, NULL);
  walk_tree (&body, record_call_1, node, visited_nodes);
  htab_delete (visited_nodes);
  visited_nodes = NULL;
}

static bool error_found;

/* Callbrack of verify_cgraph_node.  Check that all call_exprs have cgraph nodes.  */
static tree
verify_cgraph_node_1 (tree *tp, int *walk_subtrees, void *data)
{
  tree t = *tp;
  tree decl;

  if (TREE_CODE (t) == CALL_EXPR && (decl = get_callee_fndecl (t)))
    {
      struct cgraph_edge *e = cgraph_edge (data, t);
      if (e)
	{
	  if (e->aux)
	    {
	      error ("Shared call_expr:");
	      debug_tree (t);
	      error_found = true;
	    }
	  if (e->callee->decl != cgraph_node (decl)->decl)
	    {
	      error ("Edge points to wrong declaration:");
	      debug_tree (e->callee->decl);
	      fprintf (stderr," Instead of:");
	      debug_tree (decl);
	    }
	  e->aux = (void *)1;
	}
      else
	{
	  error ("Missing callgraph edge for call expr:");
	  debug_tree (t);
	  error_found = true;
	}
    }
  /* Save some cycles by not walking types and declaration as we
     won't find anything useful there anyway.  */
  if (DECL_P (*tp) || TYPE_P (*tp))
    {
      *walk_subtrees = 0;
    }
  return NULL_TREE;
}

/* Verify cgraph nodes of given cgraph node.  */
void
verify_cgraph_node (struct cgraph_node *node)
{
  struct cgraph_edge *e;
  struct cgraph_node *main_clone;

  timevar_push (TV_CGRAPH_VERIFY);
  error_found = false;
  for (e = node->callees; e; e = e->next_callee)
    if (e->aux)
      {
	error ("Aux field set for edge %s->%s",
	       cgraph_node_name (e->caller), cgraph_node_name (e->callee));
	error_found = true;
      }
  for (e = node->callers; e; e = e->next_caller)
    {
      if (e->inline_call)
	{
	  if (node->global.inlined_to
	      != (e->caller->global.inlined_to
		  ? e->caller->global.inlined_to : e->caller))
	    {
	      error ("Inlined_to pointer is wrong");
	      error_found = true;
	    }
	  if (node->callers->next_caller)
	    {
	      error ("Multiple inline callers");
	      error_found = true;
	    }
	}
      else
	if (node->global.inlined_to)
	  {
	    error ("Inlined_to pointer set for noninline callers");
	    error_found = true;
	  }
    }
  if (!node->callers && node->global.inlined_to)
    {
      error ("Inlined_to pointer is set but no predecesors found");
      error_found = true;
    }
  if (node->global.inlined_to == node)
    {
      error ("Inlined_to pointer reffers to itself");
      error_found = true;
    }

  for (main_clone = cgraph_node (node->decl); main_clone;
       main_clone = main_clone->next_clone)
    if (main_clone == node)
      break;
  if (!node)
    {
      error ("Node not found in DECL_ASSEMBLER_NAME hash");
      error_found = true;
    }
  
  if (node->analyzed
      && DECL_SAVED_TREE (node->decl) && !TREE_ASM_WRITTEN (node->decl))
    {
      walk_tree_without_duplicates (&DECL_SAVED_TREE (node->decl),
				    verify_cgraph_node_1, node);
      for (e = node->callees; e; e = e->next_callee)
	{
	  if (!e->aux)
	    {
	      error ("Edge %s->%s has no corresponding call_expr",
		     cgraph_node_name (e->caller),
		     cgraph_node_name (e->callee));
	      error_found = true;
	    }
	  e->aux = 0;
	}
    }
  if (error_found)
    {
      dump_cgraph_node (stderr, node);
      internal_error ("verify_cgraph_node failed.");
    }
  timevar_pop (TV_CGRAPH_VERIFY);
}

/* Verify whole cgraph structure.  */
void
verify_cgraph (void)
{
  struct cgraph_node *node;

  for (node = cgraph_nodes; node; node = node->next)
    verify_cgraph_node (node);
}

/* Analyze the function scheduled to be output.  */
static void
cgraph_analyze_function (struct cgraph_node *node)
{
  tree decl = node->decl;

  current_function_decl = decl;

  /* First kill forward declaration so reverse inlining works properly.  */
  cgraph_create_edges (node, DECL_SAVED_TREE (decl));

  node->local.inlinable = tree_inlinable_function_p (decl);
  if (!DECL_ESTIMATED_INSNS (decl))
    DECL_ESTIMATED_INSNS (decl) = estimate_num_insns (DECL_SAVED_TREE (decl));
  node->local.self_insns = DECL_ESTIMATED_INSNS (decl);
  if (node->local.inlinable)
    node->local.disregard_inline_limits
      = (*lang_hooks.tree_inlining.disregard_inline_limits) (decl);

  /* Inlining characteristics are maintained by the cgraph_mark_inline.  */
  node->global.insns = node->local.self_insns;

  node->analyzed = true;
  current_function_decl = NULL;
}

/* Analyze the whole compilation unit once it is parsed completely.  */

void
cgraph_finalize_compilation_unit (void)
{
  struct cgraph_node *node;

  if (!flag_unit_at_a_time)
    {
      cgraph_assemble_pending_functions ();
      return;
    }

  cgraph_varpool_assemble_pending_decls ();
  if (!quiet_flag)
    fprintf (stderr, "\nAnalyzing compilation unit\n");

  timevar_push (TV_CGRAPH);
  if (cgraph_dump_file)
    {
      fprintf (cgraph_dump_file, "Initial entry points:");
      for (node = cgraph_nodes; node; node = node->next)
	if (node->needed && DECL_SAVED_TREE (node->decl))
	  fprintf (cgraph_dump_file, " %s", cgraph_node_name (node));
      fprintf (cgraph_dump_file, "\n");
    }

  /* Propagate reachability flag and lower representation of all reachable
     functions.  In the future, lowering will introduce new functions and
     new entry points on the way (by template instantiation and virtual
     method table generation for instance).  */
  while (cgraph_nodes_queue)
    {
      struct cgraph_edge *edge;
      tree decl = cgraph_nodes_queue->decl;

      node = cgraph_nodes_queue;
      cgraph_nodes_queue = cgraph_nodes_queue->next_needed;
      node->next_needed = NULL;

      /* ??? It is possible to create extern inline function and later using
	 weak alas attribute to kill it's body. See
	 gcc.c-torture/compile/20011119-1.c  */
      if (!DECL_SAVED_TREE (decl))
	continue;

      if (node->analyzed || !node->reachable || !DECL_SAVED_TREE (decl))
	abort ();

      cgraph_analyze_function (node);

      for (edge = node->callees; edge; edge = edge->next_callee)
	if (!edge->callee->reachable)
	  cgraph_mark_reachable_node (edge->callee);

      cgraph_varpool_assemble_pending_decls ();
    }

  /* Collect entry points to the unit.  */

  if (cgraph_dump_file)
    {
      fprintf (cgraph_dump_file, "Unit entry points:");
      for (node = cgraph_nodes; node; node = node->next)
	if (node->needed && DECL_SAVED_TREE (node->decl))
	  fprintf (cgraph_dump_file, " %s", cgraph_node_name (node));
      fprintf (cgraph_dump_file, "\n\nInitial ");
      dump_cgraph (cgraph_dump_file);
    }

  if (cgraph_dump_file)
    fprintf (cgraph_dump_file, "\nReclaiming functions:");

  for (node = cgraph_nodes; node; node = node->next)
    {
      tree decl = node->decl;

      if (!node->reachable && DECL_SAVED_TREE (decl))
	{
	  cgraph_remove_node (node);
	  if (cgraph_dump_file)
	    fprintf (cgraph_dump_file, " %s", cgraph_node_name (node));
	}
    }
  if (cgraph_dump_file)
    {
      fprintf (cgraph_dump_file, "\n\nReclaimed ");
      dump_cgraph (cgraph_dump_file);
    }
  ggc_collect ();
  timevar_pop (TV_CGRAPH);
}

/* Figure out what functions we want to assemble.  */

static void
cgraph_mark_functions_to_output (void)
{
  struct cgraph_node *node;

  for (node = cgraph_nodes; node; node = node->next)
    {
      tree decl = node->decl;
      struct cgraph_edge *e;
      if (node->output)
	abort ();

      for (e = node->callers; e; e = e->next_caller)
	if (!e->inline_call)
	  break;

      /* We need to output all local functions that are used and not
	 always inlined, as well as those that are reachable from
	 outside the current compilation unit.  */
      if (DECL_SAVED_TREE (decl)
	  && !node->global.inlined_to
	  && (node->needed
	      || (e && node->reachable))
	  && !TREE_ASM_WRITTEN (decl) && !node->origin
	  && !DECL_EXTERNAL (decl))
	node->output = 1;
      /* We should've reclaimed all functions that are not needed.  */
      else if (!node->global.inlined_to && DECL_SAVED_TREE (decl)
	       && !node->origin)
	{
	  dump_cgraph_node (stderr, node);
	  abort ();
	}
    }
}

/* Expand function specified by NODE.  */

static void
cgraph_expand_function (struct cgraph_node *node)
{
  tree decl = node->decl;

  /* We ought to not compile any inline clones.  */
  if (node->global.inlined_to)
    abort ();

  if (flag_unit_at_a_time)
    announce_function (decl);

  /* Generate RTL for the body of DECL.  Nested functions are expanded
     via lang_expand_decl_stmt.  */
  (*lang_hooks.callgraph.expand_function) (decl);

  /* Make sure that BE didn't gave up on compiling.  */
  if (!TREE_ASM_WRITTEN (node->decl))
    abort ();

  current_function_decl = NULL;
}

/* Fill array order with all nodes with output flag set in the reverse
   topological order.  */
static int
cgraph_postorder (struct cgraph_node **order)
{
  struct cgraph_node *node, *node2;
  int stack_size = 0;
  int order_pos = 0;
  struct cgraph_edge *edge, last;

  struct cgraph_node **stack =
    xcalloc (cgraph_n_nodes, sizeof (struct cgraph_node *));

  /* We have to deal with cycles nicely, so use a depth first traversal
     output algorithm.  Ignore the fact that some functions won't need
     to be output and put them into order as well, so we get dependencies
     right through intline functions.  */
  for (node = cgraph_nodes; node; node = node->next)
    node->aux = NULL;
  for (node = cgraph_nodes; node; node = node->next)
    if (!node->aux)
      {
	node2 = node;
	if (!node->callers)
	  node->aux = &last;
	else
	  node->aux = node->callers;
	while (node2)
	  {
	    while (node2->aux != &last)
	      {
		edge = node2->aux;
		if (edge->next_caller)
		  node2->aux = edge->next_caller;
		else
		  node2->aux = &last;
		if (!edge->caller->aux)
		  {
		    if (!edge->caller->callers)
		      edge->caller->aux = &last;
		    else
		      edge->caller->aux = edge->caller->callers;
		    stack[stack_size++] = node2;
		    node2 = edge->caller;
		    break;
		  }
	      }
	    if (node2->aux == &last)
	      {
		order[order_pos++] = node2;
		if (stack_size)
		  node2 = stack[--stack_size];
		else
		  node2 = NULL;
	      }
	  }
      }
  free (stack);
  return order_pos;
}

/* Estimate size of the function after inlining WHAT into TO.  */

static int
cgraph_estimate_size_after_inlining (int times, struct cgraph_node *to,
				     struct cgraph_node *what)
{
  return (what->global.insns - INSNS_PER_CALL) * times + to->global.insns;
}

/* Estimate the growth caused by inlining NODE into all callees.  */

static int
cgraph_estimate_growth (struct cgraph_node *node)
{
  int growth = 0;
  struct cgraph_edge *e;

  for (e = node->callers; e; e = e->next_caller)
    if (!e->inline_call)
      growth += (cgraph_estimate_size_after_inlining (1, e->caller, node)
		 - e->caller->global.insns);

  /* ??? Wrong for self recursive functions or cases where we decide to not
     inline for different reasons, but it is not big deal as in that case
     we will keep the body around, but we will also avoid some inlining.  */
  if (!node->needed && !node->origin && !DECL_EXTERNAL (node->decl))
    growth -= node->global.insns;

  return growth;
}

/* E is expected to be an edge being inlined.  Clone destination node of
   the edge and redirect it to the new clone.
   DUPLICATE is used for bookeeping on whether we are actually creating new
   clones or re-using node originally representing out-of-line function call.
   */
static void
cgraph_clone_inlined_nodes (struct cgraph_edge *e, bool duplicate)
{
  struct cgraph_node *n;

  /* We may elliminate the need for out-of-line copy to be output.  In that
     case just go ahead and re-use it.  */
  if (!e->callee->callers->next_caller
      && (!e->callee->needed || DECL_EXTERNAL (e->callee->decl))
      && !e->callee->origin
      && duplicate
      && flag_unit_at_a_time)
    {
      if (e->callee->global.inlined_to)
	abort ();
      if (!DECL_EXTERNAL (e->callee->decl))
        overall_insns -= e->callee->global.insns, nfunctions_inlined++;
      duplicate = 0;
    }
   else if (duplicate)
    {
      n = cgraph_clone_node (e->callee);
      cgraph_redirect_edge_callee (e, n);
    }

  if (e->caller->global.inlined_to)
    e->callee->global.inlined_to = e->caller->global.inlined_to;
  else
    e->callee->global.inlined_to = e->caller;

  /* Recursivly clone all bodies.  */
  for (e = e->callee->callees; e; e = e->next_callee)
    if (e->inline_call)
      cgraph_clone_inlined_nodes (e, duplicate);
}

/* Mark edge E as inlined and update callgraph accordingly.  */

void
cgraph_mark_inline_edge (struct cgraph_edge *e)
{
  int old_insns = 0, new_insns = 0;
  struct cgraph_node *to = NULL, *what;

  if (e->inline_call)
    abort ();
  e->inline_call = true;

  if (!e->callee->global.inlined && flag_unit_at_a_time)
    {
      if (!cgraph_inline_hash)
        cgraph_inline_hash = htab_create_ggc (42, htab_hash_pointer,
					      htab_eq_pointer, NULL);
      htab_find_slot (cgraph_inline_hash,
		      DECL_ASSEMBLER_NAME (e->callee->decl), INSERT);
    }
  e->callee->global.inlined = true;

  cgraph_clone_inlined_nodes (e, true);

  what = e->callee;

  /* Now update size of caller and all functions caller is inlined into. */
  for (;e && e->inline_call; e = e->caller->callers)
    {
      old_insns = e->caller->global.insns;
      new_insns = cgraph_estimate_size_after_inlining (1, e->caller,
						       what);
      to = e->caller;
      to->global.insns = new_insns;
    }
  if (what->global.inlined_to != to)
    abort ();
  overall_insns += new_insns - old_insns;
  ncalls_inlined++;
}

/* Mark all calls of WHAT inlined into TO.  */

static void
cgraph_mark_inline (struct cgraph_node *to, struct cgraph_node *what)
{
  struct cgraph_edge *e, *next;
  int times = 0;

  /* Look for all calls, mark them inline and clone recursivly
     all inlined functions.  */
  for (e = what->callers; e; e = next)
    {
      next = e->next_caller;
      if (e->caller == to && !e->inline_call)
	{
          cgraph_mark_inline_edge (e);
	  times ++;
	}
    }
  if (!times)
    abort ();
}

/* Return false when inlining WHAT into TO is not good idea
   as it would cause too large growth of function bodies.  */

static bool
cgraph_check_inline_limits (struct cgraph_node *to, struct cgraph_node *what)
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
    return false;
  return true;
}

/* Return true when function N is small enough to be inlined.  */

static bool
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
   once in the single recusion nest path in the inline graph.  */

static bool
cgraph_recursive_inlining_p (struct cgraph_node *to,
			     struct cgraph_node *what)
{
  struct cgraph_node *node;

  /* Walk TO and all functions TO is inlined in.  */
  while (1)
    {
      /* We create recursive inlining either by inlining WHAT into something
	 already inlined in possibly different clone of WHAT.  */
      if (what->decl == to->decl)
	return true;
      /* Or by inlining WHAT into something that is already inlined in WHAT.  */
      for (node = cgraph_node (to->decl); node; node = node->next_clone)
	if (node->global.inlined_to == what)
	  return true;
      if (!to->callers || !to->callers->inline_call)
	return false;
      to = to->callers->caller;
    }
}

/* Recompute heap nodes for each of callees.  */
static void
update_callee_keys (fibheap_t heap, struct fibnode **heap_node,
		    struct cgraph_node *node)
{
  struct cgraph_edge *e;

  for (e = node->callees; e; e = e->next_callee)
    if (!e->inline_call && heap_node[e->callee->uid])
      fibheap_replace_key (heap, heap_node[e->callee->uid],
			   cgraph_estimate_growth (e->callee));
    else if (e->inline_call)
      update_callee_keys (heap, heap_node, e->callee);
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
  int max_insns = ((HOST_WIDEST_INT) initial_insns
		   * (100 + PARAM_VALUE (PARAM_INLINE_UNIT_GROWTH)) / 100);

  /* Put all inline candidates into the heap.  */

  for (node = cgraph_nodes; node; node = node->next)
    {
      struct cgraph_edge *e;

      if (!node->local.inlinable || !node->callers
	  || !cgraph_default_inline_p (node))
	continue;

      /* Rule out always_inline functions we dealt with earlier.  */
      for (e = node->callers; e; e = e->next_caller)
	if (e->inline_call)
	  break;
      if (e)
	continue;
      heap_node[node->uid] =
	fibheap_insert (heap, cgraph_estimate_growth (node), node);
    }

  if (cgraph_dump_file)
    fprintf (cgraph_dump_file, "\nDeciding on smaller functions:\n");
  while ((node = fibheap_extract_min (heap)) && overall_insns <= max_insns)
    {
      struct cgraph_edge *e, *next;
      int old_insns = overall_insns;

      heap_node[node->uid] = NULL;
      if (cgraph_dump_file)
	fprintf (cgraph_dump_file, 
		 "\nConsidering %s with %i insns\n"
		 " Estimated growth is %+i insns.\n",
		 cgraph_node_name (node), node->global.insns,
		 cgraph_estimate_growth (node));
      if (!cgraph_default_inline_p (node))
	{
	  if (cgraph_dump_file)
	    fprintf (cgraph_dump_file, " Function too large.\n");
	  continue;
	}
      for (e = node->callers; e; e = next)
	{
	  next = e->next_caller;
	  if (!e->inline_call && e->caller->decl != node->decl)
	    {
	      struct cgraph_node *where;

	      if (cgraph_recursive_inlining_p (e->caller, e->callee)
		  || !cgraph_check_inline_limits (e->caller, e->callee))
		{
		  if (cgraph_dump_file)
		    fprintf (cgraph_dump_file, " Not inlining into %s.\n",
			     cgraph_node_name (e->caller));
		  continue;
		}
	      cgraph_mark_inline (e->caller, e->callee);
	      where = e->caller;
	      if (where->global.inlined_to)
		where = where->global.inlined_to;

	      if (heap_node[where->uid])
		fibheap_replace_key (heap, heap_node[where->uid],
				     cgraph_estimate_growth (where));

	      if (cgraph_dump_file)
		fprintf (cgraph_dump_file, 
			 " Inlined into %s which now has %i insns.\n",
			 cgraph_node_name (e->caller),
			 e->caller->global.insns);
	      next = node->callers;
	    }
	}

      /* Similarly all functions called by the function we just inlined
         are now called more times; update keys.  */
      update_callee_keys (heap, heap_node, node);

      if (cgraph_dump_file)
	fprintf (cgraph_dump_file, 
		 " Inlined for a net change of %+i insns.\n",
		 overall_insns - old_insns);
    }
  if (cgraph_dump_file && !fibheap_empty (heap))
    fprintf (cgraph_dump_file, "\nReached the inline-unit-growth limit.\n");
  fibheap_delete (heap);
  free (heap_node);
}

/* Decide on the inlining.  We do so in the topological order to avoid
   expenses on updating datastructures.  */

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

  nnodes = cgraph_postorder (order);

  if (cgraph_dump_file)
    fprintf (cgraph_dump_file,
	     "\nDeciding on inlining.  Starting with %i insns.\n",
	     initial_insns);

  for (node = cgraph_nodes; node; node = node->next)
    node->aux = 0;

  if (cgraph_dump_file)
    fprintf (cgraph_dump_file, "\nInlining always_inline functions:\n");

  /* In the first pass mark all always_inline edges.  Do this with a priority
     so none of our later choices will make this impossible.  */
  for (i = nnodes - 1; i >= 0; i--)
    {
      struct cgraph_edge *e;

      node = order[i];

      for (e = node->callees; e; e = e->next_callee)
	if (e->callee->local.disregard_inline_limits)
	  break;
      if (!e)
	continue;
      if (cgraph_dump_file)
	fprintf (cgraph_dump_file,
		 "\nConsidering %s %i insns (always inline)\n",
		 cgraph_node_name (e->callee), e->callee->global.insns);
      for (; e; e = e->next_callee)
	{
	  old_insns = overall_insns;
	  if (e->inline_call || !e->callee->local.disregard_inline_limits)
	    continue;
	  if (cgraph_recursive_inlining_p (order[i], e->callee))
	    continue;
	  cgraph_mark_inline (order[i], e->callee);
	  if (cgraph_dump_file)
	    fprintf (cgraph_dump_file, 
		     " Inlined into %s which now has %i insns.\n",
		     cgraph_node_name (node->callees->caller),
	             node->callees->caller->global.insns);
	}
	if (cgraph_dump_file)
	  fprintf (cgraph_dump_file, 
		   " Inlined for a net change of %+i insns.\n",
		   overall_insns - old_insns);
    }

  cgraph_decide_inlining_of_small_functions ();

  if (cgraph_dump_file)
    fprintf (cgraph_dump_file, "\nDeciding on functions called once:\n");

  /* And finally decide what functions are called once.  */

  for (i = nnodes - 1; i >= 0; i--)
    {
      node = order[i];

      if (node->callers && !node->callers->next_caller && !node->needed
	  && node->local.inlinable && !node->callers->inline_call
	  && !DECL_EXTERNAL (node->decl) && !DECL_COMDAT (node->decl))
	{
	  bool ok = true;
	  struct cgraph_node *node1;

	  /* Verify that we won't duplicate the caller.  */
	  for (node1 = node->callers->caller;
	       node1->callers && node1->callers->inline_call
	       && ok; node1 = node1->callers->caller)
	    if (node1->callers->next_caller || node1->needed)
	      ok = false;
	  if (ok)
	    {
	      if (cgraph_dump_file)
		fprintf (cgraph_dump_file,
			 "\nConsidering %s %i insns.\n"
			 " Called once from %s %i insns.\n",
			 cgraph_node_name (node), node->global.insns,
			 cgraph_node_name (node->callers->caller),
			 node->callers->caller->global.insns);

	      old_insns = overall_insns;

	      if (cgraph_check_inline_limits (node->callers->caller, node))
		{
		  cgraph_mark_inline (node->callers->caller, node);
		  if (cgraph_dump_file)
		    fprintf (cgraph_dump_file,
			     " Inlined into %s which now has %i insns"
			     " for a net change of %+i insns.\n",
			     cgraph_node_name (node->callers->caller),
			     node->callers->caller->global.insns,
			     overall_insns - old_insns);
		}
	      else
                {
		  if (cgraph_dump_file)
		    fprintf (cgraph_dump_file,
	                     " Inline limit reached, not inlined.\n");
	        }
	    }
	}
    }

  /* We will never output extern functions we didn't inline. 
     ??? Perhaps we can prevent accounting of growth of external
     inline functions.  */
  for (node = cgraph_nodes; node; node = node->next)
    {
      if (node->global.inlined_to
	  && DECL_EXTERNAL (node->global.inlined_to->decl))
        cgraph_remove_node (node);
      if (!node->global.inlined_to && DECL_EXTERNAL (node->decl)
	  && !dump_enabled_p (TDI_all))
	DECL_SAVED_TREE (node->decl) = NULL_TREE;
    }

  if (cgraph_dump_file)
    fprintf (cgraph_dump_file,
	     "\nInlined %i calls, eliminated %i functions, "
	     "%i insns turned to %i insns.\n\n",
	     ncalls_inlined, nfunctions_inlined, initial_insns,
	     overall_insns);
  free (order);
}

/* Decide on the inlining.  We do so in the topological order to avoid
   expenses on updating datastructures.  */

static void
cgraph_decide_inlining_incrementally (struct cgraph_node *node)
{
  struct cgraph_edge *e;

  /* First of all look for always inline functions.  */
  for (e = node->callees; e; e = e->next_callee)
    if (e->callee->local.disregard_inline_limits
	&& !e->inline_call
        && !cgraph_recursive_inlining_p (node, e->callee)
	/* ??? It is possible that renaming variable removed the function body
	   in duplicate_decls. See gcc.c-torture/compile/20011119-2.c  */
	&& DECL_SAVED_TREE (e->callee->decl))
      cgraph_mark_inline (node, e->callee);

  /* Now do the automatic inlining.  */
  for (e = node->callees; e; e = e->next_callee)
    if (e->callee->local.inlinable
	&& !e->inline_call
        && !cgraph_recursive_inlining_p (node, e->callee)
        && cgraph_default_inline_p (e->callee)
	&& cgraph_check_inline_limits (node, e->callee)
	&& DECL_SAVED_TREE (e->callee->decl))
      cgraph_mark_inline (node, e->callee);
}


/* Return true when CALLER_DECL should be inlined into CALLEE_DECL.  */

bool
cgraph_inline_p (struct cgraph_edge *e)
{
  return e->inline_call;
}
/* Expand all functions that must be output.

   Attempt to topologically sort the nodes so function is output when
   all called functions are already assembled to allow data to be
   propagated across the callgraph.  Use a stack to get smaller distance
   between a function and it's callees (later we may choose to use a more
   sophisticated algorithm for function reordering; we will likely want
   to use subsections to make the output functions appear in top-down
   order).  */

static void
cgraph_expand_all_functions (void)
{
  struct cgraph_node *node;
  struct cgraph_node **order =
    xcalloc (cgraph_n_nodes, sizeof (struct cgraph_node *));
  int order_pos = 0, new_order_pos = 0;
  int i;

  cgraph_mark_functions_to_output ();

  order_pos = cgraph_postorder (order);
  if (order_pos != cgraph_n_nodes)
    abort ();

  /* Garbage collector may remove inline clones we elliminate during
     optimization.  So we must be sure to not reference them.  */
  for (i = 0; i < order_pos; i++)
    if (order[i]->output)
      order[new_order_pos++] = order[i];

  for (i = new_order_pos - 1; i >= 0; i--)
    {
      node = order[i];
      if (node->output)
	{
	  if (!node->reachable)
	    abort ();
	  node->output = 0;
	  cgraph_expand_function (node);
	}
    }
  free (order);
}

/* Mark all local functions.

   A local function is one whose calls can occur only in the
   current compilation unit, so we change its calling convention.
   We simply mark all static functions whose address is not taken
   as local.  */

static void
cgraph_mark_local_functions (void)
{
  struct cgraph_node *node;

  if (cgraph_dump_file)
    fprintf (cgraph_dump_file, "\nMarking local functions:");

  /* Figure out functions we want to assemble.  */
  for (node = cgraph_nodes; node; node = node->next)
    {
      node->local.local = (!node->needed
		           && DECL_SAVED_TREE (node->decl)
		           && !TREE_PUBLIC (node->decl));
      if (cgraph_dump_file && node->local.local)
	fprintf (cgraph_dump_file, " %s", cgraph_node_name (node));
    }
  if (cgraph_dump_file)
    fprintf (cgraph_dump_file, "\n\n");
}

/* Return true when function body of DECL still needs to be kept around
   for later re-use.  */
bool
cgraph_preserve_function_body_p (tree decl)
{
  struct cgraph_node *node;
  /* Keep the body; we're going to dump it.  */
  if (dump_enabled_p (TDI_all))
    return true;
  if (!cgraph_global_info_ready)
    return (DECL_INLINE (decl) && !flag_really_no_inline);
  /* Look if there is any clone around.  */
  for (node = cgraph_node (decl); node; node = node->next_clone)
    if (node->global.inlined_to)
      return true;
  return false;
}

/* Perform simple optimizations based on callgraph.  */

void
cgraph_optimize (void)
{
#ifdef ENABLE_CHECKING
  verify_cgraph ();
#endif
  if (!flag_unit_at_a_time)
    return;
  timevar_push (TV_CGRAPHOPT);
  if (!quiet_flag)
    fprintf (stderr, "Performing intraprocedural optimizations\n");

  cgraph_mark_local_functions ();
  if (cgraph_dump_file)
    {
      fprintf (cgraph_dump_file, "Marked ");
      dump_cgraph (cgraph_dump_file);
    }

  if (flag_inline_trees)
    cgraph_decide_inlining ();
  cgraph_global_info_ready = true;
  if (cgraph_dump_file)
    {
      fprintf (cgraph_dump_file, "Optimized ");
      dump_cgraph (cgraph_dump_file);
    }
  timevar_pop (TV_CGRAPHOPT);

  /* Output everything.  */
  if (!quiet_flag)
    fprintf (stderr, "Assembling functions:\n");
#ifdef ENABLE_CHECKING
  verify_cgraph ();
#endif
  cgraph_expand_all_functions ();
  if (cgraph_dump_file)
    {
      fprintf (cgraph_dump_file, "\nFinal ");
      dump_cgraph (cgraph_dump_file);
    }
#ifdef ENABLE_CHECKING
  verify_cgraph ();
  /* Double check that all inline clones are gone and that all function bodies
     has been released from memory.  */
  if (flag_unit_at_a_time && !dump_enabled_p (TDI_all))
    {
      struct cgraph_node *node;
      bool error_found = false;

      for (node = cgraph_nodes; node; node = node->next)
	if (node->analyzed
	    /* ??? We don't handle memory management of nested functions quite
	       right.  It probably does not worth the effort as these functions
	       should go away soon.  */
	    && !node->origin
	    && (node->global.inlined_to
	        || DECL_SAVED_TREE (node->decl)))
	  {
	    error_found = true;
	    dump_cgraph_node (stderr, node);
 	  }
      if (error_found)
	internal_error ("Nodes with no released memory found.");
    }
#endif
}
