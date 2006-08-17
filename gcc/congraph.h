/* Functions to create and manipulate connection graphs for escape
   analysis
   Copyright (C) 2006 Free Software Foundation, Inc.
   Contributed by Paul Biggar <paul.biggar@gmail.com>

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
02110-1301, USA. */

#ifndef _CON_GRAPH_H
#define _CON_GRAPH_H 1

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "timevar.h"
#include "expr.h"
#include "ggc.h"
#include "langhooks.h"
#include "flags.h"
#include "function.h"
#include "diagnostic.h"
#include "tree-dump.h"
#include "tree-gimple.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "tree-pass.h"
#include "tree-ssa-structalias.h"
#include "convert.h"
#include "params.h"
#include "ipa-type-escape.h"
#include "vec.h"
#include "bitmap.h"


/* nodes */
#define LOCAL	1
#define OBJECT 2
#define FIELD	3
#define GLOBAL 4
#define RETURN 5
#define CALLEE_ACTUAL	6
#define CALLER_ACTUAL	7

/* edges */
#define POINTS_TO 1
#define DEFERRED	2
/* #define FIELD		3	// shared with node definition */

/* escaping */
#define NO_ESCAPE 10
#define ARG_ESCAPE 11
#define GLOBAL_ESCAPE 12

#define CALLER 23
#define CALLEE 24

extern bool flow_sensisitive;
extern bool interprocedural;


struct _con_graph;
typedef struct _con_graph *con_graph;

struct _con_node;
typedef struct _con_node *con_node;

struct _con_edge;
typedef struct _con_edge *con_edge;

struct _con_graph
{
  con_node root;
  const char *filename;

  int *block_iteration_count;
  bool dirty;			/* has the graph been updated */

  tree function;

  /* for the inter-procedural, we use a return node, and draw an edge
   * between it and each return node */
  con_node return_node;

  /* in order to find the function, you need to cycle through the
   * other available functions. This is a cyclic list, so if you find
   * _function_, or null, the function you need isnt available */
  con_graph next;

  /* I use this as a maximum size for MapsTo vectors */
  int num_objects;

  /* for deubgging */
  int role;
};



struct _con_node
{
  /* the id used for comparison - done using a node to have a
   * '1-limited naming scheme */
  tree id;

  /* for field nodes, this is the owner */
  con_node owner;

  /* type; can be LOCAL, FIELD, ACTUAL, GLOBAL, OBJECT */
  int type;

  /* for object nodes, the class this node is an object of */
  /* Not allowed call things class, due to GNU coding stds */
  tree class_id;

  /* a phantom node is one which is referred to in the function,
   * but doesnt otherwise exist there (ie, a field of a passed
   * reference)*/
  bool phantom;

  /* index for actual nodes */
  int index;

  /* the linked list of incoming edges */
  con_edge in;

  /* the linked list of outgoing edges */
  con_edge out;

  /* the next in this linked list of nodes. Note that this could
   * be any node in the entire graph, and does not relate to the
   * graph's structure */
  con_node next;

  /* the next in the linked list of points_to nodes of a
   * particular node.  This list contains object nodes only. This
   * is used in the get_points_to traversal, and nothing can be
   * infered from this otherwise. */
  con_node next_link;

  /* how far the node escapes. Can be UNINIT, NO_ESCAPE,
   * ARG_ESCAPE or GLOBAl_ESCAPE */
  int escape;

  /* this is here for debugging. It allows me dump the con_graph
   * at any time. Also allows me update the dirty flag, though I
   * may have to refactor this */
  con_graph graph;

  /* the list of maps_to_obj nodes */
  con_node *maps_to_obj;
  int num_maps_to_obj;

  /* the id used by the caller/callee */
  tree call_id;

};

/*typedef struct _con_node *con_node; */



struct _con_edge
{
  /* the source of this edge */
  con_node source;

  /* target of this edge */
  con_node target;

  /* the next in the linked list of edges which come from the
   * source node */
  con_edge next_out;


  /* the next in the linked list of edges which end at the target
   * node */
  con_edge next_in;

  /* type, can be FIELD, DEFERRED, or POINTS_TO */
  int type;
};


/* print the name of the node for (Graph::Easy) */
void print_node_name (FILE * out, con_node node);

/* increment the count for basic block _index_. This is for
 * flow-sensitive analysis, to make sure we dont keep iterating
 * forever when a solution wont converge. When it gets >= a magic
 * number (probably 10), we're going to stop, and set all vars to
 * GlobalEscape. */
void increment_iteration_count (con_graph cg, int block_index);

int get_iteration_count (con_graph cg, int block_index);

void reset_iteration_count (con_graph cg, int block_index);

/* reset the dirty flag. Do this when you're starting again */
void reset_dirty (con_graph cg);


/* ---------------------------------------------------
 *							graph
 * --------------------------------------------------- */

/* allocates a new, cleared and inited con_graph */
con_graph new_con_graph (const char *filename, int num_basic_blocks,
			 tree function, con_graph next);

/* print the connection graph to file (in Graph::Easy format) */
int con_graph_dump (con_graph cg);
void con_graph_dump_node (con_node node, FILE * out);
void con_graph_dump_edge (con_edge edge, FILE * out);

/* accesor - remove later */
bool is_dirty (con_graph cg);

/* search through the list of connection graphs for the one
 * representing _function_ */
con_graph get_cg_for_function (con_graph cg, tree function);

/* ---------------------------------------------------
 *							nodes
* --------------------------------------------------- */


/* add a new object node, identified by id, of class class, and
 * return the node */
con_node add_object_node (con_graph cg, tree id, tree class);


/* add a new field node representing a field id */
con_node add_field_node (con_graph cg, tree id);

/* add a new actual node representing a parameter _id_. It represents be the
 * _i_th paramater in _function_. */
con_node add_callee_actual_node (con_graph cg,
				 tree id, int index, tree function);


/* add a new actual node representing an argument _id_. It should be the
 * _i_th paramater in the function called by _stmt_ */
con_node add_caller_actual_node (con_graph cg,
				 tree id, int index, tree stmt);

/* add a new global node, identified by id, and return the node */
con_node add_global_node (con_graph cg, tree id);

/* add a new local node, identified by id, and return the node */
con_node add_local_node (con_graph cg, tree id);

/* add a new return node, identified by id, and return the node */
con_node add_return_node (con_graph cg);

/* add the given node to the connection graph */
void add_node (con_graph cg, con_node node);


/* create and return a fresh node */
con_node new_con_node (void);


/* Get the node with the specified id from cg */
con_node get_existing_node (con_graph cg, tree id);

/* As above, but also with it's call_id set */
con_node get_existing_node_with_call_id (con_graph cg,
					 tree id, tree call_id);



/* Get the field node with specified id, from cg*/
con_node get_existing_field_node (con_graph cg,
				  tree id, con_node owner);

/* As above, but also with it's call_id set */
con_node get_existing_field_node_with_call_id (con_graph cg,
					       tree id,
					       con_node owner,
					       tree call_id);


/* When not in use a node's next_link field should be free */
void assert_all_next_link_free (con_graph cg);


/* Returns true if the node is a not an object */
bool is_reference_node (con_node node);

/* Returns a list of object nodes, pointed to by node. They are
 * chained using the next_link field of the node returned
 *
 * Precondition:	All object nodes pointed to by node will have
 * empty next_link fields
 */
con_node get_points_to (con_node node);

/* Returns a list of reference nodes, pointed to by node, which are
 * each the last node in a path which does not end in an object
 * node. They are chained using the next_link field of the node
 * returned, which is the first in the list.
 *
 * Precondition:	All nodes pointed to by node will have empty
 * next_link fields
 */
con_node get_terminal_nodes (con_node node);


/* Returns a list of field nodes of the passed object nodes, with
 * the same id as field_id. 
 *
 * Precondition: All passed nodes will be object nodes (checked)
 * Precondition: All field nodes will have empty next_link fields
 * (checked)
 *
 * Postcondition: The passed nodes will have their next_link fields
 * reset to NULL
*/
con_node get_field_nodes (con_node node, tree field_id);


/* bypasses the node by removing all its incoming and outgoing
 * edges, and adding corresponding direct edges between the source
 * of each of the incoming nodes and the target of each of the
 * outgoing nodes. If there were i incoming edges and o outgoing
 * edges, these are replaced by i*o new edges 
 *
 * NOTE it should be OK to use this to resolve all deferred nodes
 * at the end of the function analysis. It will leave hanging
 * nodes, but its not the end of the world: they were hanging
 * anyway */
void bypass_node (con_node node);

/* Get the node in the caller correspoding to the _i_th parameter of the function called by statement _call_id_ */
con_node get_caller_actual_node (con_graph cg,
				 int index, tree call_id);

/* Get the node representing the _i_th parameter of the function _call_id_*/
con_node get_callee_actual_node (con_graph cg,
				 int index, tree call_id);

/* ---------------------------------------------------
 *							edges
 * --------------------------------------------------- */
/* Returns the existing edge from source to target, or NULL if none
 * exists */
con_edge get_edge (con_node source, con_node target);

/* returns a list of nodes which are reachable from _source_ */
con_node get_nodes_reachable_from (con_node source);


/* create and return a fresh edge */
con_edge new_con_edge (void);

/* adds the specified edge to the connection graph, between source
 * and target. Note that the graph is implicit, and is the graph
 * which contains the source and target. If the edge already
 * exists, it's returned
 */
con_edge add_edge (con_node source, con_node target);

/* removes an edge from the graph */
void remove_con_edge (con_edge edge);

/* analysis */
void update_nodes (con_node fee, con_node maps_to_f);


con_node get_single_named_field_node (con_node node, tree field_id);


con_node *get_field_nodes_vec (con_node node);

bool in_maps_to_obj (con_node source, con_node target);
void add_to_maps_to_obj (con_node source, con_node target);
void update_escape_state (con_node source, con_node target);

void d (con_node node);
void t (tree id);

/* Copy the constructor graph into cg, using call_id to resolve actual
 * nodes correctly */
void inline_constructor_graph (con_graph cg,
			       con_graph constructor, tree call_id);

con_node get_matching_node_in_caller (con_graph cg,
				      con_node node, tree call_id);


#endif /* _CON_GRAPH_H */
