/* Callgraph handling code.
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
#include "tree.h"
#include "tree-inline.h"
#include "langhooks.h"
#include "hashtab.h"
#include "toplev.h"
#include "flags.h"
#include "ggc.h"
#include "debug.h"
#include "target.h"
#include "varray.h"

/* The declarations we know about must not get garbage collected.
   We do not want callgraph datastructure to be saved via PCH code
   since it would make it dificult to extend it into untramodule
   optimizer later, so we store the references into the array to avoid
   garbage collector from doing it's job.  */
GTY(()) varray_type known_fns;

/* The cgraph data strutcture.
   Each function decl has assigned cgraph_node listing calees and callers.  */

struct cgraph_node
{
  tree decl;
  struct cgraph_edge *callees;
  struct cgraph_edge *callers;
  struct cgraph_node *next;
  /* For nested functions points to function the node is nested in.  */
  struct cgraph_node *origin;
  void *aux;

  /* Set when function must be output - it is externally visible
     or it's address is taken.  */
  bool needed;
  /* Set when function is reachable by call from other function
     that is eighter reachable or needed.  */
  bool reachable;
  /* Set when the frontend has been asked to lower representation of this
     function into trees.  Callees lists are not available when lowered
     is not set.  */
  bool lowered;
  /* Set when function is scheduled to be assembled.  */
  bool output;

  /* Set when function is inlinable at all and when it is profitable to inline
     it many times.  */
  bool inline_single;
  bool inline_many;
};

struct cgraph_edge
{
  struct cgraph_node *caller, *callee;
  struct cgraph_edge *next_caller;
  struct cgraph_edge *next_callee;
};

/* Hash table used to convert declarations into nodes.  */
static htab_t cgraph_hash = 0;

/* The linked list of cgraph nodes.  */
static struct cgraph_node *cgraph_nodes;

/* Number of nodes in existence.  */
static int cgraph_n_nodes;

static struct cgraph_node *cgraph_node PARAMS ((tree decl));
static struct cgraph_edge *create_edge PARAMS ((struct cgraph_node *,
						struct cgraph_node *));
static void remove_edge PARAMS ((struct cgraph_node *, struct cgraph_node *));
static struct cgraph_edge *record_call PARAMS ((tree, tree));
static tree record_call_1 PARAMS ((tree *, int *, void *));
static hashval_t hash_node PARAMS ((const PTR));
static int eq_node PARAMS ((const PTR, const PTR));
static struct cgraph_node *cgraph_node PARAMS ((tree));
static tree fixup_calls PARAMS ((tree *, int *, void *));
static void cgraph_expand_functions PARAMS ((void));

/* Returns a hash code for P.  */

static hashval_t
hash_node (p)
     const PTR p;
{
  return (hashval_t)
    htab_hash_pointer (DECL_ASSEMBLER_NAME
		       (((struct cgraph_node *) p)->decl));
}

/* Returns non-zero if P1 and P2 are equal.  */

static int
eq_node (p1, p2)
     const PTR p1;
     const PTR p2;
{
  return ((DECL_ASSEMBLER_NAME (((struct cgraph_node *) p1)->decl)) ==
	  DECL_ASSEMBLER_NAME ((tree) p2));
}

/* Return cgraph node assigned to DECL.  Create new one when needed.  */
static struct cgraph_node *
cgraph_node (decl)
     tree decl;
{
  struct cgraph_node *node;
  struct cgraph_node **slot;

  if (TREE_CODE (decl) != FUNCTION_DECL)
    abort ();

  if (!cgraph_hash)
    {
      cgraph_hash = htab_create (10, hash_node, eq_node, NULL);
      VARRAY_TREE_INIT (known_fns, 32, "known_fns");
    }

  slot =
    (struct cgraph_node **) htab_find_slot_with_hash (cgraph_hash, decl,
						      htab_hash_pointer
						      (DECL_ASSEMBLER_NAME
						       (decl)), 1);
  if (*slot)
    return *slot;
  node = xcalloc (sizeof (*node), 1);
  node->decl = decl;
  node->next = cgraph_nodes;
  cgraph_nodes = node;
  cgraph_n_nodes++;
  *slot = node;
  if (DECL_CONTEXT (decl))
    node->origin = cgraph_node (DECL_CONTEXT (decl));
  VARRAY_PUSH_TREE (known_fns, decl);
  return node;
}

/* Create edge from CALLER to CALLEE in the cgraph.  */

static struct cgraph_edge *
create_edge (caller, callee)
     struct cgraph_node *caller, *callee;
{
  struct cgraph_edge *edge = xmalloc (sizeof (struct cgraph_edge));

  edge->caller = caller;
  edge->callee = callee;
  edge->next_caller = callee->callers;
  edge->next_callee = caller->callees;
  caller->callees = edge;
  callee->callers = edge;
  return edge;
}

/* Create edge from CALLER to CALLEE in the cgraph.  */

static void
remove_edge (caller, callee)
     struct cgraph_node *caller, *callee;
{
  struct cgraph_edge **edge, **edge2;

  for (edge = &callee->callers; *edge && (*edge)->caller != caller;
       edge = &((*edge)->next_caller))
    ;
  if (!*edge)
    abort ();
  *edge = (*edge)->next_caller;
  for (edge2 = &caller->callees; *edge2 && (*edge2)->callee != callee;
       edge2 = &(*edge2)->next_callee)
    ;
  if (!*edge2)
    abort ();
  *edge2 = (*edge2)->next_callee;
}

/* Record call from CALLER to CALLEE  */

static struct cgraph_edge *
record_call (caller, callee)
     tree caller, callee;
{
  return create_edge (cgraph_node (caller), cgraph_node (callee));
}

void
cgraph_remove_call (caller, callee)
     tree caller, callee;
{
  remove_edge (cgraph_node (caller), cgraph_node (callee));
}

/* Return true when CALLER_DECL calls CALLEE_DECL.  */

bool
cgraph_calls_p (caller_decl, callee_decl)
     tree caller_decl, callee_decl;
{
  struct cgraph_node *caller = cgraph_node (caller_decl);
  struct cgraph_node *callee = cgraph_node (callee_decl);
  struct cgraph_edge *edge;

  for (edge = callee->callers; edge && (edge)->caller != caller;
       edge = (edge->next_caller))
    ;
  return edge != NULL;
}

/* Walk tree and record all calls.  Called via walk_tree.  */
static tree
record_call_1 (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees ATTRIBUTE_UNUSED;
     void *data;
{
  /* Record dereferences to the functions.  This makes the functions
     reachable unconditionally.  */
  if (TREE_CODE (*tp) == ADDR_EXPR)
    {
      tree decl = TREE_OPERAND (*tp, 0);
      if (TREE_CODE (decl) == FUNCTION_DECL)
	{
	  struct cgraph_node *n = cgraph_node (decl);
	  n->needed = n->reachable = true;
	}
    }
  else if (TREE_CODE (*tp) == CALL_EXPR)
    {
      tree decl = TREE_OPERAND (*tp, 0);
      if (TREE_CODE (decl) == ADDR_EXPR)
	decl = TREE_OPERAND (decl, 0);
      if (TREE_CODE (decl) == FUNCTION_DECL)
	{
	  if (DECL_BUILT_IN (decl))
	    return NULL;
	  record_call (data, decl);
	  walk_tree (&TREE_OPERAND (*tp, 1), record_call_1, data, NULL);
	  *walk_subtrees = 0;
	}
    }
  return NULL;
}

/* Create cgraph edges for function calles via BODY.  */

void
cgraph_create_edges (decl, body)
     tree decl;
     tree body;
{
  walk_tree (&body, record_call_1, decl, NULL);
}

/* Analyze function once it is parsed.  Set up the local information
   available - create cgraph edges for function calles via BODY.  */

void
cgraph_finalize_function (decl, body)
     tree decl;
     tree body ATTRIBUTE_UNUSED;
{
  struct cgraph_node *node = cgraph_node (decl);

  node->decl = decl;

  node->inline_single = tree_inlinable_function_p (decl, 1);
  node->inline_many = tree_inlinable_function_p (decl, 0);

  (*debug_hooks->deferred_inline_function) (decl);
}

/* Dump the callgraph.  */

void
dump_cgraph (f)
     FILE *f;
{
  struct cgraph_node *node;

  fprintf (f, "\nCallgraph:\n\n");
  for (node = cgraph_nodes; node; node = node->next)
    {
      struct cgraph_edge *edge;
      fprintf (f, "%s", IDENTIFIER_POINTER (DECL_NAME (node->decl)));
      if (node->origin)
	fprintf (f, " nested in: %s",
		 IDENTIFIER_POINTER (DECL_NAME (node->origin->decl)));
      if (node->needed)
	fprintf (f, " needed");
      else if (node->reachable)
	fprintf (f, " reachable");
      if (node->inline_single)
	fprintf (f, " inlinable");
      if (node->inline_many)
	fprintf (f, " small");
      if (DECL_SAVED_TREE (node->decl))
	fprintf (f, " tree");

      fprintf (f, "\n  called by :");
      for (edge = node->callers; edge; edge = edge->next_caller)
	fprintf (f, "%s ",
		 IDENTIFIER_POINTER (DECL_NAME (edge->caller->decl)));

      fprintf (f, "\n  calls: ");
      for (edge = node->callees; edge; edge = edge->next_callee)
	fprintf (f, "%s ",
		 IDENTIFIER_POINTER (DECL_NAME (edge->callee->decl)));
      fprintf (f, "\n");
    }
}

/* Replace forward declaration by real ones allowing reverse inlining.
   Also replace types by new ones to allow optimizers to modify function
   declaration (such as add attributes to it)  */

static tree
fixup_calls (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees ATTRIBUTE_UNUSED;
     void *data ATTRIBUTE_UNUSED;
{
  if (TREE_CODE (*tp) == CALL_EXPR)
    {
      tree *addr = &TREE_OPERAND (*tp, 0);
      tree *decl = &TREE_OPERAND (*tp, 0);
      if (TREE_CODE (*decl) == ADDR_EXPR)
	decl = &TREE_OPERAND (*decl, 0);
      if (TREE_CODE (*decl) == FUNCTION_DECL)
	{
	  if (DECL_BUILT_IN (*decl))
	    return NULL;
	  *decl = cgraph_node (*decl)->decl;
	  if (TREE_CODE (*addr) == ADDR_EXPR)
	    {
	      TREE_TYPE (*addr) = copy_node (TREE_TYPE (*addr));
	      TREE_TYPE (TREE_TYPE (*addr)) = TREE_TYPE (*decl);
	    }
	}
    }
  return NULL;
}

/* Analyze whole compilation unit once it is parsed completely.  */

void
cgraph_finalize_compilation_unit ()
{
  struct cgraph_node *node;
  struct cgraph_edge *edge;
  bool changed = true;

  if (!quiet_flag)
    fprintf (stderr, "\n\nUnit entry points:");
  /*  Lower representation of all reachable functions.  */
  while (changed)
    {
      changed = false;
      for (node = cgraph_nodes; node; node = node->next)
	{
	  tree decl = node->decl;

	  if (node->lowered || !DECL_SAVED_TREE (decl))
	    continue;
	  if ((TREE_PUBLIC (decl) && !DECL_COMDAT (decl) && !DECL_EXTERNAL (decl))
	      || (DECL_ASSEMBLER_NAME_SET_P (decl)
		  && TREE_SYMBOL_REFERENCED (DECL_ASSEMBLER_NAME (decl))))
	    {
	      announce_function (decl);
	      node->needed = node->reachable = true;
	    }
	  /* At the moment frontend automatically emits all nested functions.  */
	  if (node->origin && node->origin->reachable)
	    node->reachable = 1;
	  if (!node->reachable)
	    continue;

	  if (lang_hooks.callgraph.lower_function)
	    (*lang_hooks.callgraph.lower_function) (decl);
	  /* First kill forward declaration so reverse inling works properly.  */
	  walk_tree (&DECL_SAVED_TREE (decl), fixup_calls, NULL, NULL);
	  cgraph_create_edges (decl, DECL_SAVED_TREE (decl));

	  for (edge = node->callees; edge; edge = edge->next_callee)
	    edge->callee->reachable = true;
	  node->lowered = true;
	  changed = true;
	}
    }
  if (!quiet_flag)
    {
      /*dump_cgraph (stderr);*/
      fprintf (stderr, "\n\nReclaiming functions:");
    }

  for (node = cgraph_nodes; node; node = node->next)
    {
      tree decl = node->decl;

      if (!node->reachable && DECL_SAVED_TREE (decl))
	{
	  DECL_SAVED_TREE (decl) = NULL;
	  announce_function (decl);
	}
    }
  ggc_collect ();
  /* Now inline functions marked for inlining.  */
  for (node = cgraph_nodes; node; node = node->next)
    {
      tree decl = node->decl;
      if (DECL_SAVED_TREE (decl)
	  && node->needed
	  && (TREE_PUBLIC (decl)
	      || !node->inline_single) && !TREE_ASM_WRITTEN (decl))
	optimize_inline_calls (decl);
    }
}

/* Expand all functions that must be output.  */
#if 0
static void
cgraph_expand_functions ()
{
  struct cgraph_node *node;
  for (node = cgraph_nodes; node; node = node->next)
    {
      tree decl = node->decl;

      if (DECL_SAVED_TREE (decl)
	  && (node->needed
	      || (DECL_UNINLINABLE (decl) && node->reachable)
	      || TREE_SYMBOL_REFERENCED (DECL_ASSEMBLER_NAME (decl)))
	  && !TREE_ASM_WRITTEN (decl) && !node->origin
	  && !DECL_EXTERNAL (decl))
	{
	  announce_function (decl);
	  optimize_inline_calls (decl);
	  walk_tree (&DECL_SAVED_TREE (decl), fixup_calls, NULL, NULL);
	  if (!node->reachable)
	    abort ();
	  (*lang_hooks.callgraph.expand_function) (decl);
	  if (DECL_UNINLINABLE (decl))
	    DECL_SAVED_TREE (decl) = NULL;
	  current_function_decl = NULL;
	}
    }
}
#endif

#define NPREDECESORS(node) (size_t)((node)->aux)
#define SET_NPREDECESORS(node,n) (node)->aux = (void *) (n);

/* Expand all functions that must be output.  */

static void
cgraph_expand_functions ()
{
  struct cgraph_node *node;
  struct cgraph_node **stack =
    xcalloc (sizeof (struct cgraph_node *), cgraph_n_nodes);
  int stack_size = 0;
  struct cgraph_edge *edge;

  /* Figure out functions we want to assemble.  */
  for (node = cgraph_nodes; node; node = node->next)
    {
      tree decl = node->decl;

      if (DECL_SAVED_TREE (decl)
	  && (node->needed
	      || (DECL_UNINLINABLE (decl) && node->reachable)
	      || TREE_SYMBOL_REFERENCED (DECL_ASSEMBLER_NAME (decl)))
	  && !TREE_ASM_WRITTEN (decl) && !node->origin
	  && !DECL_EXTERNAL (decl))
	node->output = 1;
    }

  /* Attempt to topologically sort the nodes so function is output when
     all called functions are already assembled to allow data to be propagated
     accross the callgraph.  Use stack to get smaller distance between function
     and it's callees (later we may use more sophisticated algorithm for
     function reordering, we will likely want to use subsections to make output
     functions to appear in top-down order, not bottom-up they are assembled).
     */

  for (node = cgraph_nodes; node; node = node->next)
    if (node->output)
      {
	int n = 0;
	for (edge = node->callees; edge; edge = edge->next_callee)
	  if (edge->callee->output)
	    n++;
	SET_NPREDECESORS (node, n);
	if (n == 0)
	  stack[stack_size++] = node;
      }
  while (1)
    {
      struct cgraph_node *minnode;
      while (stack_size)
	{
	  tree decl;

	  node = stack[--stack_size];
	  decl = node->decl;
	  announce_function (decl);
	  node->output = 0;

	  for (edge = node->callers; edge; edge = edge->next_caller)
	    if (edge->caller->output)
	      {
	        SET_NPREDECESORS (edge->caller,
		    		  NPREDECESORS (edge->caller) - 1);
		if (!NPREDECESORS (edge->caller))
		  stack[stack_size++] = edge->caller;
	      }
	  optimize_inline_calls (decl);
	  walk_tree (&DECL_SAVED_TREE (decl), fixup_calls, NULL, NULL);
	  if (!node->reachable)
	    abort ();
	  (*lang_hooks.callgraph.expand_function) (decl);
	  if (DECL_UNINLINABLE (decl))
	    DECL_SAVED_TREE (decl) = NULL;
	  current_function_decl = NULL;
	}
      minnode = NULL;
      /* We found cycle.  Break it and try again.  */
      for (node = cgraph_nodes; node; node = node->next)
	if (node->output
	    && (!minnode
	        || NPREDECESORS (minnode) > NPREDECESORS (node)))
	  minnode = node;
      if (!minnode)
	return;
      stack[stack_size++] = minnode;
    }
}

/* Perform simple optimizations based on callgraph.  */

void
cgraph_optimize ()
{
  struct cgraph_node *node;
  bool changed = true;
  struct cgraph_edge *edge;

  if (!quiet_flag)
    fprintf (stderr, "\n\nMarking as always_inline:");
  /* Now look for function called only once and mark them to inline.  From this
     point number of calls to given function won't grow.  */
  for (node = cgraph_nodes; node; node = node->next)
    {
      tree decl = node->decl;

      if (node->callers && !node->callers->next_caller && !TREE_PUBLIC (decl)
	  && node->inline_single)
	{
	  DECL_ATTRIBUTES (decl) =
	    chainon (DECL_ATTRIBUTES (decl),
		     build_tree_list (get_identifier ("always_inline"),
				      NULL_TREE));
	  DECL_INLINE (decl) = true;
	  DECL_UNINLINABLE (decl) = 0;
	  announce_function (decl);
	}
    }

  if (targetm.cgraph.optimize_local_function)
    {
      if (!quiet_flag)
	fprintf (stderr, "\n\nMarking local functions:");
      for (node = cgraph_nodes; node; node = node->next)
	{
	  tree decl = node->decl;

	  if (!node->needed && !TREE_PUBLIC (decl)
	      && DECL_SAVED_TREE (decl)
	      && (!DECL_ASSEMBLER_NAME_SET_P (decl)
		  || !TREE_SYMBOL_REFERENCED (DECL_ASSEMBLER_NAME (decl))))
	    {
	      (*targetm.cgraph.optimize_local_function) (decl);
	      announce_function (decl);
	    }
	}
    }
  if (!quiet_flag)
    fprintf (stderr, "\n\nAssembling functions:");

  /* Output everything.  
     ??? Our inline heuristic may decide to not inline functions previously
     marked as inlinable thus adding new function bodies that must be output.
     Later we should move all inlining decisions to callgraph code to make
     this impossible.  */
  cgraph_expand_functions ();
  while (changed)
    {
      changed = false;
      for (node = cgraph_nodes; node; node = node->next)
	{
	  if (!node->needed)
	    continue;

	  for (edge = node->callees; edge; edge = edge->next_callee)
	    if (!edge->callee->needed)
	      changed = edge->callee->needed = true;
	}
    }
  cgraph_expand_functions ();
}
