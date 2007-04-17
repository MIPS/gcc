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
#include "diagnostic.h"
#include "vec.h"
#include "hashtab.h"
#include "statistics.h"

enum con_node_type
{
  LOCAL_NODE = 101,
  OBJECT_NODE,
  FIELD_NODE,
  GLOBAL_NODE,
  RETURN_NODE,
  CALLEE_ACTUAL_NODE, /* node in the callee, which matches a node in the caller */
  CALLER_ACTUAL_NODE /* node in the caller, which matches a node in the callee */
};

enum con_edge_type
{
  POINTS_TO_EDGE = 111,
  DEFERRED_EDGE,
  FIELD_EDGE
};

/* This should be combined in the future with structure aliasing, but
 * for now it needs a prefix */
enum ea_escape_state
{
  EA_NO_ESCAPE = 121,
  EA_ARG_ESCAPE,
  EA_GLOBAL_ESCAPE
};

/* Declare types */
struct _con_graph;
typedef struct _con_graph *con_graph;

struct _con_node;
typedef struct _con_node *con_node;

struct _con_edge;
typedef struct _con_edge *con_edge;

/* set up vectors */
DEF_VEC_P(con_node);
DEF_VEC_ALLOC_P(con_node,heap);
typedef VEC(con_node,heap) *con_node_vec;
DEF_VEC_P(con_graph);
DEF_VEC_ALLOC_P(con_graph,heap);
typedef VEC(con_graph,heap) *con_graph_vec;

struct _con_graph
{
  /* In order to iterate across, we keep a linked list of nodes */
  con_node root;

  /* A hashtable of the nodes in the graph, accessed by add_node,
   * get_existing_node and get_existing_field_node */
  htab_t nodes;

  /* For con_graph_dump */
  const char *filename;

  tree function;

  bool deserialized;

  /* Some congraphs represent functions outside the current compilation
   * unit. This allows them to be named */
  tree function_name;

  /* for the inter-procedural, we use a return node, and draw an edge
   * between it and each return node */
  con_node return_node;

  /* I use this as a maximum size for MapsTo vectors */
  int num_objects;

  /* The basic block that this graph represents */
  int index;

  /* The dump_index of the next node */
  int dump_index;
};



struct _con_node
{
  /* the id used for comparison - done using a node to have a
   * '1-limited naming scheme' */
  tree id;

  /* VAR_DECLs, PARM_DECLs and FIELD_DECLs seem to have consistent uids
   * across comilation units. If this is not zero, it should be used as
   * id, and ID should be NULL. */
  int uid;

  /* For field nodes, the object to which this belongs */
  tree owner; 

  /* type */
  enum con_node_type type;

  /* a phantom node is one which represents the existing value, when
   * no information about that value is provided. Currently this is
   * the object and fields of passed references, other references they
   * access, and the values of caller and callee parameters */
  bool phantom;

  /* the linked list of incoming edges */
  con_edge in;

  /* the linked list of outgoing edges */
  con_edge out;

  /* the next in this linked list of nodes. Note that this could
   * be any node in the entire graph, and does not relate to the
   * graph's structure */
  con_node next;

  /* Used to return lists of nodes from functions */
  con_node next_link;

  enum ea_escape_state escape;

  /* this is here for debugging. It allows me dump the con_graph
   * at any time. */
  con_graph graph;

  /* the id used by the caller/callee */
  tree call_id;

  int dump_index;

  con_node_vec maps_to_obj;
};



struct _con_edge
{
  con_node source;
  con_node target;

  /* next in the linked list of the source's outgoing edges */
  con_edge next_out;


  /* the next in the linked list of the target's incoming edges */
  con_edge next_in;

  enum con_edge_type type;
};


/* ---------------------------------------------------
 *                      graph
 * --------------------------------------------------- */

/* allocates a new, cleared and inited con_graph */
con_graph new_con_graph (tree function, int bb_index, int count);

/* print the connection graph to file (in Graph::Easy format) */
void con_graph_dump (con_graph cg);
void prune_con_graph (con_graph cg);

/* search through the list of connection graphs for the one
 * representing _function_ */
con_graph get_cg_for_function (tree function);

void add_con_graph (con_graph cg);
void init_graphs (void);

/* Combine the graphs into DEST */
void merge_into (con_graph dest, con_graph src);

bool graphs_equal (con_graph, con_graph);

/* ---------------------------------------------------
 *			nodes
* --------------------------------------------------- */


/* add a new object node, identified by id, of class class, and
 * return the node */
con_node add_object_node (con_graph cg, tree id);


/* add a new field node representing a field id */
con_node add_field_node (con_graph cg, tree id);

/* add a new actual node representing a parameter _id_. It represents be
 * the _i_th parameter in the function. */
con_node add_callee_parameter (con_graph cg, tree id);


/* add a new global node, identified by id, and return the node */
con_node add_global_node (con_graph cg, tree id);

/* add a new local node, identified by id, and return the node */
con_node add_local_node (con_graph cg, tree id);
con_node add_ref_node (con_graph cg, tree id);

/* add a new return node, identified by id, and return the node */
con_node add_result_node (con_graph cg, tree id);


/* Get the node with the specified id from cg */
con_node get_existing_node (con_graph cg, tree id);

/* Get the node with the specified id from cg, or add it if it doesnt
 * exist */
con_node existing_local_node (con_graph cg, tree id);

/* As above, but also with it's call_id set */
con_node get_existing_node_with_call_id (con_graph cg, tree id, int uid,
					 tree call_id);

/* Get the field node with specified id, from cg */
con_node get_existing_field_node (con_graph cg,
				  tree id, int uid, tree owner);

/* As above, but also with it's call_id set */
con_node get_existing_field_node_with_call_id (con_graph cg,
					       tree id,
					       int uid,
					       tree owner,
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
con_node_vec get_points_to (con_node node, con_node_vec vector);

/* Returns a list of reference nodes, pointed to by node, which are
 * each the last node in a path which does not end in an object
 * node. They are chained using the next_link field of the node
 * returned, which is the first in the list.
 *
 * Precondition:	All nodes pointed to by node will have empty
 * next_link fields
 */
con_node get_terminal_nodes (con_node node);


/* Returns a list of field nodes of the passed object node, with
 * the same id as field_id. 
 *
 * Precondition: All field nodes will have empty next_link fields
 * (checked)
 *
 * Postcondition: The passed nodes will have their next_link fields
 * reset to NULL
*/
con_node get_field_nodes (con_node_vec nodes, tree field_id);


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
bool bypass_node (con_node node);

/* Removes all incoming edges, and replaces them with an edge to each
 * from the source of the first edge, to the target of each successor
 * edge. Repeats this for each node so that the net effect is the removal
 * of nearly all deferred nodes */
void bypass_every_node (con_graph cg);

void set_escape_state (con_node node, enum ea_escape_state);
/* ---------------------------------------------------
 *			edges
 * --------------------------------------------------- */
/* Returns the existing edge from source to target, or NULL if none
 * exists */
con_edge get_edge (con_node source, con_node target);

/* returns a list of nodes which are reachable from _source_ */
con_node get_nodes_reachable_from (con_node source);


/* create and return a fresh edge */
con_edge new_con_edge (void);

void tf (tree, bool);
void nl (void);
void df (con_node);

/* adds the specified edge to the connection graph, between source
 * and target. Note that the graph is implicit, and is the graph
 * which contains the source and target. */
con_edge add_edge (con_node source, con_node target);
bool add_killing_edge (con_node source, con_node target);

/* removes an edge from the graph */
void remove_con_edge (con_edge edge);
void remove_con_node (con_node node, bool check);
void clean_up_con_graph (con_graph);

/* This could be called multiple times with the same caller and callee, in
 * which case, the call_id will differentiate between the nodes creates in
 * each instance */
void update_nodes (con_node f, con_node mapped_field, tree call_id);


/* Get the field node of the passed object, with the specified field_id.
 * Return NULL if not found. */
/* TODO look at this again */
con_node get_single_named_field_node (con_node node, tree field_id);

/* Pushes the object's field node into the passed vector, and returns the
 * vector. */
con_node_vec get_field_nodes_vec (con_node object, con_node_vec fields);

bool in_maps_to_obj (con_node source, con_node target);
void add_to_maps_to_obj (con_node source, con_node target);
void update_escape_state (con_node source, con_node target);
void serialize_con_graph (con_graph);
con_graph deserialize_con_graph (tree);

void d (con_node node);
void l (con_graph cg);
void t (tree id);
void tt (tree id);

con_node get_matching_node_in_caller (con_graph cg,
				      con_node node, tree call_id);

/* NEXT_LINK_CLEAR clears a node's next_link field, then replaces it with
 * it's next field */
#define NEXT_LINK_CLEAR(A) do { con_node next = (A)->next_link; (A)->next_link = NULL; (A) = next; } while (0)
#define NEXT_LINK(A) do { (A) = (A)->next_link; } while (0)

/* return the number of linked nodes from node, including node */
int link_length (con_node node);

/* clear the next_link field for all linked nodes in the list */
void clear_links (con_node node);

bool in_link_list (con_node list, con_node subject);
con_node last_link (con_node node);
con_node merge_next_lists (con_node list1, con_node list2);

con_node_vec get_points_to_and_terminals (con_graph cg, con_node source,
					  tree stmt_id);

void init_markers (void);
void init_con_node_hashtable (void);
void init_statement_type_hashtable (void);

void VEC_print (con_node_vec);

#endif /* _CON_GRAPH_H */
