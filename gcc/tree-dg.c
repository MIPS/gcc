/* Dependence Graph 
   Copyright (C) 2004 Free Software Foundation, Inc.
   Contributed by Devang Patel <dpatel@apple.com>

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

/* This pass build data dependence graph based on the information
   collected by scalar evolution analyzer.

   A short description of data dependence graph:

   Each node in the graph represents one GIMPLE statement.

   Nodes are connected using dependence edge that describes data
   dependence relation between two nodes.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "errors.h"
#include "ggc.h"
#include "tree.h"
#include "flags.h"
#include "timevar.h"
#include "varray.h"
#include "rtl.h"
#include "basic-block.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "cfgloop.h"
#include "tree-fold-const.h"
#include "tree-chrec.h"
#include "tree-data-ref.h"
#include "tree-scalar-evolution.h"
#include "tree-pass.h"
#include "tree-dg.h"

/* local function prototypes */
static void dg_init_graph (void);
static void set_dg_node_for_stmt (tree, dependence_node);
static dependence_node dg_get_node_for_stmt (tree, bool);
static dependence_node alloc_dependence_node (void);
static dependence_edge alloc_dependence_edge (void);
static dependence_node dg_create_node (tree);
static dependence_edge dg_find_edge (dependence_node, dependence_node, bool);
static bool gate_ddg (void);
static void dump_dg (FILE *, int);

/* Initial dependence graph capacity.  */
static int dependence_graph_size = 25;

/* The dependence graph.  */
static GTY (()) varray_type dependence_graph;
static GTY (()) varray_type datarefs;
static GTY (()) varray_type dependence_relations;

/* Total dependence node count.  */
static int n_dependence_node = 0;

#define DEPENDENCE_GRAPH(N) (VARRAY_DG (dependence_graph, (N)))

/* Initialize data dependence graph.  */
static
void dg_init_graph (void)
{
  VARRAY_DG_INIT (dependence_graph, dependence_graph_size, "dependence_graph");
}

/* Create dependency graph.  */
void dg_create_graph (void)
{
  unsigned int i;
  
  /* When computing the data references, this is the  maximum number 
     of nodes that we want to compute.  */
  int alldd_max_size = 100;
  
  VARRAY_GENERIC_PTR_INIT (datarefs, alldd_max_size, "datarefs");
  VARRAY_GENERIC_PTR_INIT (dependence_relations, 
			   alldd_max_size * alldd_max_size,
			   "dependence_relations");

  /* Analyze data references and dependence relations using scev.  */
  
  find_data_references (datarefs);
  compute_all_dependences (datarefs, dependence_relations);
  
  /* Initialize.  */
  dg_init_graph ();

  /* Using data refernces, populate graph.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (dependence_relations); i++)
    {
      dependence_edge connecting_edge;

      struct data_reference *first_dr, *second_dr;
      struct data_dependence_relation *ddr;
      tree first_stmt, second_stmt;

      ddr = VARRAY_GENERIC_PTR (dependence_relations, i);

      /* If there is no dependence than do not create an edge.  */
      if (DDR_ARE_DEPENDENT (ddr) == chrec_bot)
	continue;

      /* Get dependence references */
      first_dr = DDR_A (ddr);
      second_dr = DDR_B (ddr);

      /* Get statements */
      first_stmt = DR_STMT (first_dr);
      second_stmt = DR_STMT(second_dr);

      /* Find connecting edge.  */
      connecting_edge = dg_find_edge (dg_get_node_for_stmt (first_stmt, true),
				      dg_get_node_for_stmt (second_stmt, true),
				      true);

      /* Record data dependence relation.  */
      connecting_edge->ddr = ddr;
    }

  if (tree_dump_file)
    {
      dump_dg (tree_dump_file, tree_dump_flags);
    }
  
}

/*---------------------------------------------------------------------------
			Dependence node creation
---------------------------------------------------------------------------*/

/* Allocate memory for dependence_node.  */

static dependence_node
alloc_dependence_node (void)
{
  dependence_node dg_node;
  dg_node = ggc_alloc_cleared (sizeof (*dg_node));
  return dg_node;
}

/* Create new dependency_node.  */

static dependence_node 
dg_create_node (tree stmt)
{
  dependence_node dg_node;
  if (!stmt)
    return NULL;

  /* Allocate */
  dg_node = alloc_dependence_node ();

  /* Assign id.  */
  dg_node->node_id = n_dependence_node;

  VARRAY_PUSH_DG (dependence_graph, dg_node);

  /* Increment count.  */
  n_dependence_node++;

  /* Connect dg_node and stmt with each other.  */
  dg_node->stmt = stmt;
  set_dg_node_for_stmt (stmt, dg_node);

  return dg_node;
}

/*---------------------------------------------------------------------------
			Dependence edge creation
---------------------------------------------------------------------------*/

/* Allocate memory for dependence_edge.  */

static dependence_edge
alloc_dependence_edge (void)
{
  dependence_edge dg_edge;
  dg_edge = ggc_alloc_cleared (sizeof (*dg_edge));
  return dg_edge;
}

/* Find edge in the dependence graph that connects two nodes. 
 If required, create new edge.  */

static dependence_edge 
dg_find_edge (dependence_node n1, dependence_node n2, bool create)
{
  tree stmt1, stmt2;
  dependence_edge e;

  if (!n1 || !n2)
    abort ();

  stmt1 = DN_STMT (n1);
  stmt2 = DN_STMT (n2);

  if (!stmt1 || !stmt2)
    abort ();

  /* Browse succ edges and see if dst of any edge is stmt2.
     If there is one then return that edge.  */
  for (e = n1->succ; e; e = e->succ_next)
    {
      if (DN_STMT (e->dst) == stmt2)
	return e;
    }

  /* Browse pred edges and see if src of any edge is stmt2.
     If there is one then return that edge.  */
  for (e = n1->pred; e; e = e->pred_next)
    {
      if (DN_STMT (e->src) == stmt2)
	return e;
    }

  if (!create)
    return NULL;

  /* OK, time to create new edge to connect these two nodes.  */
  e = alloc_dependence_edge ();

  /* Set source and destination nodes.  */
  e->src = n1;
  e->dst = n2;

  /* Set succ and pred */
  if (n1->succ)
    e->succ_next = n1->succ;
  n1->succ = e;

  if (n2->pred)
    e->pred_next = n2->pred;
  n2->pred = e;

  /* Return newly created edge.  */
  return e;
}

/*---------------------------------------------------------------------------
			stmt_ann manipulation for dg_node
---------------------------------------------------------------------------*/

/* Find dependence_node for the given input tree. If there is not one,
   create new one.  */

static 
dependence_node  dg_get_node_for_stmt (tree t, bool create)
{
  dependence_node dg_node = dg_node_for_stmt (t);

  /* If there is none, create one.  */
  if (!dg_node && create)
      dg_node = dg_create_node (t);

  return dg_node;
}

/* Set the dg_node for the input tree.  */
static void 
set_dg_node_for_stmt (tree t, dependence_node dg_node)
{
  stmt_ann_t ann;

  if (!t)
    abort (); 

  ann = get_stmt_ann (t);
  if (!ann)
    abort ();

  ann->dg_node = dg_node;
}

/*---------------------------------------------------------------------------
                         Pass management
---------------------------------------------------------------------------*/

static bool
gate_ddg (void)
{
  return flag_ddg && flag_scalar_evolutions != 0;
}

struct tree_opt_pass pass_ddg =
{
  "ddg",				/* name */
  gate_ddg,			        /* gate */
  dg_create_graph,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_DEP_GRAPH,			        /* tv_id */
  PROP_scev,      			/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0					/* todo_flags_finish */
};

/*---------------------------------------------------------------------------
			 Printing and debugging
---------------------------------------------------------------------------*/

/* Print dependency graph in the dump file.  */
static void 
dump_dg (FILE *file, int flags ATTRIBUTE_UNUSED)
{
  unsigned int i, j;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (dependence_graph); i++)
    {
      dependence_edge e;
      dependence_node dg_node = DEPENDENCE_GRAPH (i);

      if (!dg_node)
	abort ();

      fprintf (file, "# Dependence Node %d\n", dg_node->node_id);

      /* Print Predecssors */
      fprintf (file, "# Pred :");
      for (e = dg_node->pred; e; e = e->pred_next)
	if (e->dst == dg_node)
	  fprintf (file, "%d ", DN_ID(e->src));
      fprintf (file, "\n");

      /* Print Successors */
      fprintf (file, "# Succ :");
      for (e = dg_node->succ; e; e = e->succ_next)
	if (e->src == dg_node)
	  fprintf (file, "%d ", DN_ID(e->dst));
      fprintf (file, "\n");

      fprintf (file, "# Statement :");
      print_generic_stmt (file, DN_STMT (dg_node), 0);
      
      fprintf (file, "# From\tTo\tDirection Vector\n");
      for (e = dg_node->succ; e; e = e->succ_next)
	{

	  fprintf (file,"  %d\t", DN_ID(e->src));
	  fprintf (file,"%d\t", DN_ID(e->dst));

	  if (DDR_ARE_DEPENDENT (e->ddr) == chrec_top)
	    fprintf (file, "don't know\n");

	  for (j = 0; j < DDR_NUM_SUBSCRIPTS (e->ddr); j++)
	    {
	      struct subscript *sub = DDR_SUBSCRIPT (e->ddr, j);
	      
	      dump_data_dependence_direction (file, SUB_DIRECTION (sub));
	      fprintf (file, " ");
	    }
	  fprintf (file,"\n");
	}

      /* Add one blank line at the end of this node.  */
      fprintf (file, "\n");
    }
}

void 
debug_dg (int flags)
{
  dump_dg (stderr, flags);
}
