/* Callgraph handling code.
   Copyright (C) 2002 Free Software Foundation, Inc.
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
#include "coretypes.h"
#include "tm.h"
#include "system.h"
#include "tree.h"
#include <hashtab.h>
#include "tree-inline.h"

/* The cgraph data strutcture.
   Each function decl has assigned cgraph_node listing calees and callers.  */

struct cgraph_node
{
  tree decl;
  struct cgraph_edge *callees;
  struct cgraph_edge *callers;
  struct cgraph_node *next;
  void *aux;
};

struct cgraph_edge
{
  struct cgraph_node *caller, *callee;
  struct cgraph_edge *next_caller;
  struct cgraph_edge *next_callee;
};

static htab_t cgraph_hash = 0;
static struct cgraph_node *cgraph_nodes;

static struct cgraph_node *cgraph_node PARAMS ((tree decl));
static struct cgraph_edge *create_edge PARAMS ((struct cgraph_node *, 
						struct cgraph_node *));
static void remove_edge PARAMS ((struct cgraph_node *, 
				 struct cgraph_node *));
static struct cgraph_edge *record_call PARAMS ((tree, tree));
static tree record_call_1 PARAMS ((tree *, int *, void *));
static hashval_t hash_node PARAMS ((const PTR));
static int eq_node PARAMS ((const PTR, const PTR));
static struct cgraph_node *cgraph_node PARAMS ((tree));
static tree fixup_calls PARAMS ((tree *, int *, void *));

/* Returns a hash code for P.  */

static hashval_t
hash_node (p)
     const PTR p;
{
  return (hashval_t)
    htab_hash_pointer (DECL_NAME (((struct cgraph_node *) p)->decl));
}

/* Returns non-zero if P1 and P2 are equal.  */

static int
eq_node (p1, p2)
     const PTR p1;
     const PTR p2;
{
  return (DECL_NAME (((struct cgraph_node *) p1)->decl)) ==
    DECL_NAME ((tree) p2);
}

/* Return cgraph node assigned to DECL.  Create new one when needed.  */
static struct cgraph_node *
cgraph_node (decl)
     tree decl;
{
  struct cgraph_node *node;
  struct cgraph_node **slot;
  if (!cgraph_hash)
    {
      cgraph_hash = htab_create (10, hash_node, eq_node, NULL);
    }

  slot =
    (struct cgraph_node **) htab_find_slot_with_hash (cgraph_hash, decl,
							 htab_hash_pointer
							 (DECL_NAME (decl)),
							 1);
  if (*slot)
    return *slot;
  node = xmalloc (sizeof (*node));
  node->decl = decl;
  node->callees = NULL;
  node->callers = NULL;
  node->next = cgraph_nodes;
  cgraph_nodes = node;
  *slot = node;
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
  if (TREE_CODE (*tp) == CALL_EXPR)
    {
      tree decl = TREE_OPERAND (*tp, 0);
      if (TREE_CODE (decl) == ADDR_EXPR)
	decl = TREE_OPERAND (decl, 0);
      if (TREE_CODE (decl) == FUNCTION_DECL)
	record_call (data, decl);
    }
  return NULL;
}

/* Create cgraph edges for function calles via BODY.  */
void
create_cgraph_edges (decl, body)
     tree decl;
     tree body;
{
  struct cgraph_node *node = cgraph_node (decl);
  node->decl = decl;
  walk_tree (&body, record_call_1, decl, NULL);
}

/* Dump the callgraph.  */
void
dump_cgraph (f)
     FILE *f;
{
  struct cgraph_node *node;
  fprintf (f, "Callgraph:\n\n");
  for (node = cgraph_nodes; node; node = node->next)
    {
      struct cgraph_edge *edge;
      fprintf (f, "%s\n  called by :",
	       IDENTIFIER_POINTER (DECL_NAME (node->decl)));
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

/* Replace forward declaration by real ones allowing reverse inlining.  */
static tree
fixup_calls (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees ATTRIBUTE_UNUSED;
     void *data ATTRIBUTE_UNUSED;
{
  if (TREE_CODE (*tp) == CALL_EXPR)
    {
      tree *decl = &TREE_OPERAND (*tp, 0);
      if (TREE_CODE (*decl) == ADDR_EXPR)
	decl = &TREE_OPERAND (*decl, 0);
      if (TREE_CODE (*decl) == FUNCTION_DECL)
	*decl = cgraph_node (*decl)->decl;
    }
  return NULL;
}

/* Perform simple optimizations based on callgraph.  */
void
cgraph_optimize ()
{
  struct cgraph_node *node;
  bool marked = 0;

  /* First kill forward declaration so reverse inling works properly.  */
  for (node = cgraph_nodes; node; node = node->next)
    {
      tree decl = node->decl;
      if (DECL_SAVED_TREE (decl))
	walk_tree (&DECL_SAVED_TREE (decl), fixup_calls, NULL, NULL);
    }
  /* Now inline functions marked for inlining.  */
  for (node = cgraph_nodes; node; node = node->next)
    {
      tree decl = node->decl;
      if (DECL_SAVED_TREE (decl)
	  && (TREE_PUBLIC (decl)
	      || !tree_inlinable_function_p (decl))
	  && !TREE_ASM_WRITTEN (decl))
	optimize_inline_calls (decl);
    }
  /* Now look for function called only once and mark them to inline.  From this
     point number of calls to given function won't grow.  */
  for (node = cgraph_nodes; node; node = node->next)
    {
      tree decl = node->decl;
      if (node->callers && !node->callers->next_caller && !TREE_PUBLIC (decl))
	{
	  DECL_ATTRIBUTES (decl) =
	    chainon (DECL_ATTRIBUTES (decl),
		     build_tree_list (get_identifier ("always_inline"),
				      NULL_TREE));
	  DECL_INLINE (decl) = 1;
	  DECL_UNINLINABLE (decl) = 0;
	  node->aux = (void *) 1;
	  marked = 1;
	}
      else
	node->aux = NULL;
    }
  /* Inline function we marked.  */
  if (marked)
    for (node = cgraph_nodes; node; node = node->next)
      {
	tree decl = node->decl;
	if (DECL_SAVED_TREE (decl)
	    && (TREE_PUBLIC (decl)
		|| !tree_inlinable_function_p (decl))
	    && !TREE_ASM_WRITTEN (decl))
	  {
	    struct cgraph_edge *e;
	    for (e = node->callees; e && !e->callee->aux ; e = e->next_callee)
	      ;
	    if (e)
	      optimize_inline_calls (decl);
	  }
      }
#if 0
  /* Output everything.  */
  for (node = cgraph_nodes; node; node = node->next)
    {
      tree decl = node->decl;
      if (DECL_SAVED_TREE (decl))
	{
	  if (!TREE_ASM_WRITTEN (decl))
	    {
	      expand_deferred_function (decl);
	      current_function_decl = NULL;
	    }
	}
    }
#endif
}
