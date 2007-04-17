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
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.	If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA. */

#include "congraph.h"
#include "hashtab.h"
#include "sbitmap.h"

#include "tree.h"

extern FILE* dump_file;

/* Quick hacks to avoid warnings */
bool is_global (tree);
static int get_uid (tree);

/* -------------------------------------------------------------*
 *			memory and hashtables			*
 * -------------------------------------------------------------*/
static hashval_t
con_node_id_hash (const void *p)
{
  const con_node n = (con_node) p;
  return htab_hash_pointer (n->id);
}

static int
htab_con_node_eq (const void *p1, const void *p2)
{
  const con_node n1 = (con_node) p1;
  const con_node n2 = (con_node) p2;
  gcc_assert (n1->graph == n2->graph);

  return ((n1->id == n2->id || n1->id == NULL || n2->id == NULL)
	  && n1->uid == n2->uid 
	  && n1->call_id == n2->call_id);
}

/* -------------------------------------------------------------*
 *			graph					*
 * -------------------------------------------------------------*/

/* I used to identify nodes in the dumps by using their address. This is
 * unreliable and doesnt lend itself well to regression. Instead, use the
 * order in which they are created, which appears to be fairly constant */
static bool
is_actual_type (con_node node)
{
  return node->type == CALLER_ACTUAL_NODE 
    || node->type == CALLEE_ACTUAL_NODE
    || node->type == RETURN_NODE;
}

static char*
get_cg_filename (tree function, int bb_index, int count)
{
  /* Make strings for the two digits */
  char bb_digits[10];
  sprintf (bb_digits, "%d", bb_index);

  char digit[] = {'0', '\0'};
  digit[0] += count;

  char* filename = concat ("graphs/",
		     (IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME
					  (function))),
		     bb_digits, "_", digit, NULL);

  if (strnlen (filename, 230) >= 230) 
    filename[230] = '\0';
  return filename;
}


/* allocates a new, cleared and inited con_graph */
con_graph
new_con_graph (tree function, int bb_index, int count)
{
  con_graph result = xcalloc (1, sizeof (struct _con_graph));

  result->filename = get_cg_filename (function, bb_index, count);
  result->function = function;
  result->index = bb_index;
  result->nodes = htab_create (20, con_node_id_hash, htab_con_node_eq,
			       free);
  return result;
}


/* Finds the node from CG which is the same as NODE. */
static con_node
get_corresponding_node (con_graph cg, con_node node)
{
  con_node result;
  gcc_assert (node->graph != cg);
  if (node->type == FIELD_NODE)
    result = get_existing_field_node_with_call_id (cg, node->id,
						   node->uid, node->owner,
						   node->call_id);
  else
    result = get_existing_node_with_call_id (cg, node->id, node->uid,
					     node->call_id);

  if (result)
    gcc_assert (result->type == node->type);
  return result;
}



/* Checks if two graphs are identical */
bool
graphs_equal (con_graph cg1,
	      con_graph cg2)
{
  con_node node;
  /* for every node, there must be a correspoinding node */
  for (node = cg1->root; node; node = node->next)
    {
      con_node corr = get_corresponding_node (cg2, node);
      if (corr == NULL)
	{
	  df (node);
	  fprintf (dump_file, "Node not found in graph %d\n", cg2->index);
	  return false;
	}
      else if (corr->escape != node->escape)
	{
	  df (node);
	  fprintf (dump_file, "different escape states\n");
	  return false;
	}
    }

  /* and vice verse */
  for (node = cg2->root; node; node = node->next)
    {
      con_node corr = get_corresponding_node (cg1, node);
      if (corr == NULL)
	{
	  df (node);
	  fprintf (dump_file, "Node not found in graph %d\n", cg1->index);
	  return false;
	}
      else if (corr->escape != node->escape)
	{
	  df (node);
	  fprintf (dump_file, "different escape states\n");
	  return false;
	}
    }
      
  /* for every edge, there must be a corresponding edge */
  for (node = cg1->root; node; node = node->next)
    {
      con_node corr = get_corresponding_node (cg2, node);
      con_edge out;
      for (out = node->out; out; out = out->next_out)
	{
	  con_node corr2 = get_corresponding_node (cg2, out->target);
	  if (get_edge (corr, corr2) == NULL)
	    {
	      fprintf (dump_file, "Edge missing from ");
	      df (corr);
	      fprintf (dump_file, "to ");
	      df (corr2);
	      nl ();
	      return false;
	    }
	}
    }

  /* and vice versa */
  for (node = cg2->root; node; node = node->next)
    {
      con_node corr = get_corresponding_node (cg1, node);
      con_edge out;
      for (out = node->out; out; out = out->next_out)
	{
	  con_node corr2 = get_corresponding_node (cg1, out->target);
	  if (get_edge (corr, corr2) == NULL)
	    return false;
	}
    }

  return true;
}


/* Creates an edge in DEST for every edge in SRC. */
static void
copy_edges (con_graph dest, con_graph src)
{
  con_node node = src->root;
  while (node)
    {
      con_edge out = node->out;
      while (out)
	{
	  con_node source = get_corresponding_node (dest, out->source);
	  con_node target = get_corresponding_node (dest, out->target);
	  gcc_assert (source && target);
	  if (get_edge (source, target) == NULL)
	    {
	      /* HACK: add_edge adds the owner to target */
	      if (target->type == FIELD_NODE 
		  && source->type == OBJECT_NODE)
		{
		  gcc_assert (target->owner == source->id);
		  target->owner = NULL;
		}
	      add_edge (source, target);
	    }

	  out = out->next_out;
	}

      node = node->next;
    }
}

static void
con_graph_dump_node (con_node node, FILE * out)
{
  gcc_assert (node->id);
  gcc_assert (node->type == FIELD_NODE ? node->owner != NULL : true);
  /* dont check the object's class_id */
  gcc_assert (node->graph);
  /* print the nodes first, so that we can annotate them */

  /* guarantees that there is a node printed for each node in
   * the graph */
  fprintf (out, "[ %d ] ", node->dump_index);
  fprintf (out, "{ ");
  fprintf (out, "size: 1,1; ");

  /* print the label */
  fprintf (out, "label: \"");

  if (node->phantom)
    fprintf (out, "[");

  switch (node->type)
    {
    case CALLEE_ACTUAL_NODE:
      fprintf (out, "callee (");
      print_generic_expr (out, node->id, 0);
      fprintf (out, ")");
      break;

    case CALLER_ACTUAL_NODE:
      fprintf (out, "caller (");
      print_generic_expr (out, node->id, 0);
      fprintf (out, ")");
      break;

    case FIELD_NODE:
    case OBJECT_NODE:
    case LOCAL_NODE:
    case GLOBAL_NODE:
    case RETURN_NODE:
      print_generic_expr (out, node->id, 0);
      break;

    default:
      gcc_unreachable ();
      break;
    }
  if (node->phantom)
    fprintf (out, "]");
  fprintf (out, "\"; ");

  /* color the border and assign groups based on the nodes escape-ness
   * */
  switch (node->escape)
    {
    case EA_NO_ESCAPE:
      fprintf (out, "border: limegreen; ");
      break;
    case EA_ARG_ESCAPE:
      fprintf (out, "border: blue; ");
      break;
    case EA_GLOBAL_ESCAPE:
      fprintf (out, "border: red; ");
      break;
    default:
      gcc_unreachable ();
      break;
    }

  /* design for the color and shape node depends on its type */
  switch (node->type)
    {

    case LOCAL_NODE:
      fprintf (out, "shape: ellipse; ");
      break;

    case OBJECT_NODE:
      fprintf (out, "shape: rect; ");
      break;

    case FIELD_NODE:
      fprintf (out, "fill: #cfffcf; ");
      fprintf (out, "shape: ellipse; ");
      break;

    case GLOBAL_NODE:
      fprintf (out, "shape: ellipse; ");
      fprintf (out, "fill: silver; ");
      break;

    case RETURN_NODE:
    case CALLEE_ACTUAL_NODE:
    case CALLER_ACTUAL_NODE:
      fprintf (out, "fill: #ffcfcf; ");
      fprintf (out, "shape: ellipse; ");
      break;

    default:
      gcc_unreachable ();
    }

  fprintf (out, "}\n");


}

static void
con_graph_dump_edge (con_edge edge, FILE * out)
{
  con_node source = edge->source;
  con_node target = edge->target;

  gcc_assert (source);
  gcc_assert (target);

  /* print the source */
  fprintf (out, "[ %d ] ", source->dump_index);

  /* draw the edge */
  fprintf (out, " -> ");
  switch (edge->type)
    {
    case POINTS_TO_EDGE:
      fprintf (out, " { label: P; }");
      gcc_assert (is_reference_node (source));
      gcc_assert (target->type == OBJECT_NODE);
      break;

    case DEFERRED_EDGE:
      fprintf (out, " { label: D; style: dashed; }");
      gcc_assert (is_reference_node (source));
      gcc_assert (is_reference_node (target));
      break;

    case FIELD_EDGE:
      fprintf (out, " { label: F; }");
      gcc_assert (source->type == OBJECT_NODE);
      gcc_assert (target->type == FIELD_NODE);
      break;

    default:
      gcc_unreachable ();
    }

  /* print the target */
  fprintf (out, "[ %d ] ", target->dump_index);

  fprintf (out, "\n");
}


void
con_graph_dump (con_graph cg)
{
  con_node node;
  con_edge edge;
  FILE *out;

  gcc_assert (cg);
  gcc_assert (cg->function);
  gcc_assert (cg->filename);

  out = fopen (concat (cg->filename, ".graph", NULL), "w");
  if (out == NULL)
      fprintf (stderr, "%s\n", strerror (errno));
  gcc_assert (out);

  /* this needs to be done first */
  /* step 1 - put them in groups */
  /* do the localgraph */
  fprintf (out, "( LocalGraph\n"); /* an empty node to avoid a bug */
  for (node = cg->root; node != NULL; node = node->next)
    {
      if (node->escape == EA_NO_ESCAPE)
	{
	  con_graph_dump_node (node, out);
	}
    }
  fprintf (out, ") { fill: skyblue; }\n\n");

  /* do the nonlocalgraph */
  fprintf (out, "( NonLocalGraph\n");
  for (node = cg->root; node != NULL; node = node->next)
    {
      if (node->escape != EA_NO_ESCAPE)
	{
      con_graph_dump_node (node, out);
	}
    }
  fprintf (out, ") { fill: lightblue; }\n\n");

  /* step 2 - add the edges */
  for (node = cg->root; node != NULL; node = node->next)
    {
      for (edge = node->out; edge != NULL; edge = edge->next_out)
	{
	  con_graph_dump_edge (edge, out);
	}
    }

  fclose (out);
}

/* Put all the connection graphs in a hashtable. This hashtable contains
 * graphs created in this compilation unit, and other serialized from
 * outside. Therefore, we use strings as keys, wrapped in a tree fetched
 * with get_identifier (). */
htab_t graphs;


static hashval_t
htab_con_graph_hash (const void *p)
{
  const con_graph cg = (con_graph) p;
  return htab_hash_pointer (cg->function_name);
}

static int
htab_con_graph_eq (const void *p1, const void *p2)
{
  const con_graph cg1 = (con_graph) p1;
  const con_graph cg2 = (con_graph) p2;
  return cg1->function_name == cg2->function_name;
}

void
init_graphs (void)
{
  graphs = htab_create (500, htab_con_graph_hash, htab_con_graph_eq, free);
}

static void
mark_and_recurse (con_node node, sbitmap bm)
{
  if (TEST_BIT (bm, node->dump_index))
    return; // we've been here before

  SET_BIT (bm, node->dump_index);
  con_edge edge;
  for (edge = node->out; edge != NULL; edge = edge->next_out)
    {
      mark_and_recurse (edge->target, bm);
    }
}

void
clean_up_con_graph (con_graph cg)
{
  sbitmap bm = sbitmap_alloc (10000);
  sbitmap_zero (bm);
  con_node node;

  // starting at the parameters, do a mark and sweep. 
  tree parameters = DECL_ARGUMENTS (cg->function);
  while (parameters)
    {
      node = get_existing_node (cg, parameters);
      mark_and_recurse (node, bm);
      parameters = TREE_CHAIN (parameters);
    }

  for (node = cg->root; node; node = node->next)
    {
      if (!TEST_BIT (bm, node->dump_index))
	remove_con_node (node, false);
    }

  sbitmap_free (bm);
}

void
add_con_graph (con_graph cg)
{
  struct _con_graph finder;
  void** slot;

  /* This wont have been added before */
  cg->function_name = DECL_ASSEMBLER_NAME (cg->function);
  fprintf (dump_file, "Adding congraph: ");
  tf (cg->function_name, true);

  finder.function = cg->function_name;
  slot = htab_find_slot (graphs, &finder, INSERT);
  *slot = (void*) cg;
}


/* search through the list of connection graphs FUNCTION */
con_graph
get_cg_for_function (tree function)
{
  struct _con_graph finder;
  finder.function_name = DECL_ASSEMBLER_NAME (function);
  fprintf (dump_file, "Finding congraph: ");
  tf (finder.function_name, true);
//  con_graph cg = htab_find (graphs, &finder);
  //if (cg) return cg;

  con_graph cg = deserialize_con_graph (function);
//  if (cg)
//    add_con_graph (cg);
  return cg;
}


/* ---------------------------------------------------
 *			nodes
 * --------------------------------------------------- */
/* Allocate a new, cleared and inited con_node */
static con_node
new_con_node (void)
{
  con_node result = xcalloc (1, sizeof (struct _con_node));
  gcc_assert (result);
  result->escape = EA_NO_ESCAPE;
  return result;
}

/* Adds NODE to the callgraph CG */
static void
add_node (con_graph cg, con_node node)
{
  struct _con_node finder;
  void** slot;
  gcc_assert (cg && node);
  gcc_assert (node->next == NULL);

  /* add uid before we search */
  if (node->id != NULL && TREE_CODE (node->id) != IDENTIFIER_NODE)
    node->uid = get_uid (node->id);

  /* we search for id, which is duplicated in the case of field nodes */
  if (node->type != FIELD_NODE)
    gcc_assert (get_existing_node_with_call_id
		(cg, node->id, node->uid, node->call_id) == NULL);

  /* insert the node at the front of the list */
  node->next = cg->root;
  cg->root = node;

  /* remove this later, this is just for debugging */
  node->graph = cg;

  node->dump_index = cg->dump_index;
  cg->dump_index++;

  /* put the node into the hashtable */
  finder.graph = node->graph;
  finder.id = node->id;
  finder.uid = node->uid;
  finder.call_id = node->call_id;
  slot = htab_find_slot (cg->nodes, &finder, INSERT);
  *slot = (void*) node;
  
}

static void
copy_nodes (con_graph dest, con_graph src)
{
  con_node node;
  for (node = src->root; node; node = node->next)
    {
      /* nodes can obviously be in multiple graphs */
      con_node other;
      if ((other = get_corresponding_node (dest, node)))
	{
	  if (node->escape > other->escape)
	    other->escape = node->escape;
	  continue;
	}

      con_node new_node = new_con_node ();

      /* Keep its allocated index when overwriting */
      int index = new_node->dump_index;
      memcpy (new_node, node, sizeof (struct _con_node));
      new_node->dump_index = index;

      new_node->graph = dest;
      new_node->in = NULL;
      new_node->out = NULL;
      new_node->next = NULL;

      gcc_assert (new_node->maps_to_obj == NULL);
      gcc_assert (new_node->next_link == NULL);

      add_node (dest, new_node);
      if (new_node->type == RETURN_NODE)
	dest->return_node = new_node;
    }
}

/* Create a new object node representing an allocation of type
 * class, named using id */
con_node
add_object_node (con_graph cg, tree id)
{
  con_node node;
  gcc_assert (cg && id);

  node = new_con_node ();
  cg->num_objects++;
  node->id = id;
  node->type = OBJECT_NODE;
  add_node (cg, node);
  return node;
}

/* create a new field node representing a field id */
con_node
add_field_node (con_graph cg, tree id)
{
  con_node node;
  gcc_assert (cg);
  gcc_assert (id);

  node = new_con_node ();
  node->id = id;
  node->type = FIELD_NODE;
  add_node (cg, node);
  return node;
}

/* create this function's return node, used to merge other return
 * values */
con_node
add_result_node (con_graph cg, tree id)
{
  con_node node;
  gcc_assert (cg);
  gcc_assert (cg->return_node == NULL);

  node = new_con_node ();
  node->id = id;
  node->type = RETURN_NODE;
  node->escape = EA_ARG_ESCAPE;
  node->phantom = true;
  add_node (cg, node);
  cg->return_node = node;
  return node;
}

/* Add any node which can be global */
con_node
add_global_node (con_graph cg, tree id)
{
  con_node node;
  gcc_assert (id);

  node = new_con_node ();
  node->id = id;
  node->type = GLOBAL_NODE;
  node->escape = EA_GLOBAL_ESCAPE;
  add_node (cg, node);
  return node;
}

con_node
add_local_node (con_graph cg, tree id)
{
  con_node node;
  gcc_assert (id);

  /* Check global vars */
  if (is_global (id))
    return add_global_node (cg, id);

  node = new_con_node ();
  node->id = id;
  node->type = LOCAL_NODE;
  add_node (cg, node);
  return node;
}

con_node
add_callee_parameter (con_graph cg, tree id)
{
  gcc_assert (id);
  con_node node = new_con_node ();
  node->id = id;
  node->type = CALLEE_ACTUAL_NODE;
  node->escape = EA_ARG_ESCAPE;
  add_node (cg, node);
  return node;
}

static con_node
find_node (con_graph cg, tree id, int uid, tree call_id)
{
  struct _con_node finder;
  gcc_assert (cg);

  if (cg->root == NULL)
    return NULL;

  /* In deserialized graphs, uid nodes have NULL ids */
  if (uid && cg->deserialized)
    id = NULL;


  finder.id = id;
  finder.uid = uid;
  finder.graph = cg;
  finder.call_id = call_id;

  return htab_find (cg->nodes, &finder);
}

/*static con_node
get_existing_node_with_identifier (con_graph cg, tree id)
{
  // this is only here to check for dups. I think that the only ids with
  // dups are <<<exception>>>, so it should be ok to read these.
  con_node node;
  con_node result = NULL;
  tree identifier = DECL_NAME (id);
  for (node = cg->root; node ; node = node->next)
    {
      if (node->id == identifier)
	{
	  gcc_assert (result == NULL);
	  result = node;
	}
    }
  gcc_assert (result != NULL);
  return result;
}*/

static int
get_uid (tree id)
{
  if (id == NULL) 
    return 0;
  if (SSA_VAR_P (id) && TREE_CODE (id) != SSA_NAME)
    return DECL_UID (id);
  return 0;
}

con_node
get_existing_node (con_graph cg, tree id)
{
  // congraphs which have been deserialized
  return get_existing_node_with_call_id (cg, id, get_uid (id), NULL);
}

con_node
get_existing_node_with_call_id (con_graph cg, tree id, int uid, tree call_id)
{
  return find_node (cg, id, uid, call_id);
}

con_node
existing_local_node (con_graph cg, tree id)
{
  con_node result = get_existing_node (cg, id);
  if (result == NULL)
    {
      result = add_local_node (cg, id);
    }
  gcc_assert (result);
  /* the node doesnt have to be null if it exists already */
  return result;
}



con_node
get_existing_field_node (con_graph cg, tree id, int uid, tree owner)
{
  return get_existing_field_node_with_call_id (cg, id, uid, owner, NULL);
}

con_node
get_existing_field_node_with_call_id (con_graph graph,
				      tree id, int uid, tree owner,
				      tree call_id)
{
  con_node node;
  gcc_assert (graph);
  gcc_assert (id);
  gcc_assert (owner);
  for (node = graph->root; node != NULL; node = node->next)
    {
      if (node->id == id 
	  && node->owner == owner
	  && node->uid == uid 
	  && node->call_id == call_id )
	{
	  /* For other types use get_existing_node */
	  gcc_assert (node->type == FIELD_NODE);
	  return node;
	}
    }

  return NULL;
}

/* ---------------------------------------------------
 *		edges
 * --------------------------------------------------- */

/* allocates a new, cleared con_node */
con_edge
new_con_edge (void)
{
  con_edge result = xcalloc (1, sizeof (struct _con_edge));
  gcc_assert (result);
  return result;
}

bool
is_reference_node (con_node node)
{
  return (node->type != OBJECT_NODE);
}

con_edge
get_edge (con_node source, con_node target)
{
  con_edge edge;
  for (edge = source->out; edge != NULL; edge = edge->next_out)
    {
      if (edge->target == target)
	{
	  return edge;
	}
    }

  return NULL;
}

con_edge
add_edge (con_node source, con_node target)
{
  con_edge edge;
/*  fprintf (dump_file, "Adding edge from ");
  df (source);
  fprintf (dump_file, " to ");
  df (target);
  nl ();*/

  gcc_assert (source != target);

  /* check that this edge doesnt already occur */
  gcc_assert (get_edge (source, target) == NULL);

  edge = new_con_edge ();
  edge->source = source;
  edge->target = target;

  /* set the type */
  if (is_reference_node (source) && target->type == OBJECT_NODE)
    edge->type = POINTS_TO_EDGE;

  else if (source->type == OBJECT_NODE && target->type == FIELD_NODE)
    {
      con_edge temp;
      edge->type = FIELD_EDGE;

      /* there can only be 1 owner of a field node */
      gcc_assert (target->owner == NULL);
      gcc_assert (source->owner == NULL);
      target->owner = source->id;

      /* TODO this may be allowable for a phantom field node */
      /* check that no other edges originate from on object */
      for (temp = target->in; temp != NULL; temp = temp->next_in)
	{
	  gcc_assert (temp->type != POINTS_TO_EDGE);
	}

    }
  else if (is_reference_node (source) && is_reference_node (target))
    edge->type = DEFERRED_EDGE;

  else
    gcc_unreachable ();


  /* add outgoing edge */
  edge->next_out = source->out;
  source->out = edge;

  /* add incoming edge */
  edge->next_in = target->in;
  target->in = edge;

  /* update the escape state */
  if (source->escape > target->escape)
    target->escape = source->escape;

  return edge;
}

void
remove_con_node (con_node node, bool check)
{
  con_graph cg;
  void** slot;
  struct _con_node finder;

  if (check)
    {
      /* this is only really designed to remove caller nodes */
      gcc_assert (node);
      gcc_assert (node->in == NULL);
      gcc_assert (node->out);
      gcc_assert (node->out->next_out == NULL);
      gcc_assert (node->type == CALLER_ACTUAL_NODE);
    }

  if (check || node->out)
    remove_con_edge (node->out); 

  /* remove it from the hashtable and the list of nodes */
  cg = node->graph;
  if (cg->root == node)
    cg->root = node->next;
  else
    {
      con_node prev_node = cg->root;
      while (prev_node->next != node)
	prev_node = prev_node->next;

      prev_node->next = node->next;
    }
  finder.id = node->id;
  finder.uid = node->uid;
  finder.call_id = node->call_id;
  finder.graph = node->graph;
  slot = htab_find_slot (cg->nodes, &finder, NO_INSERT);
  gcc_assert (*slot == node);
  htab_clear_slot (cg->nodes, slot);
}

void
remove_con_edge (con_edge edge)
{
  /* edges dont have a previos_edge field, so we need to iterate
   * through them the find the edge which points at this one */

  con_node node;
  con_edge temp;
  gcc_assert (edge);
  fprintf (dump_file, "Removing edge from ");
  df (edge->source);
  fprintf (dump_file, " to ");
  df (edge->target);
  nl ();

  /* do the incoming first */
  node = edge->target;

  if (node->in == edge)
    {
      /* special case cause its the start of the list */
      node->in = edge->next_in;
    }
  else
    {
      /* iterate through the list */
      bool found_edge = false;
      for (temp = node->in; temp->next_in != NULL;
	   temp = temp->next_in)
	{
	  if (temp->next_in == edge)
	    {
	      /* have the previous node link to the next one */
	      temp->next_in = edge->next_in;
	      found_edge = true;
	      break;
	    }
	}
      gcc_assert (found_edge);
    }

  /* now do the outgoing */
  node = edge->source;

  if (node->out == edge)
    {
      /* start of list special case */
      node->out = edge->next_out;
    }
  else
    {
      /* iterate through the list */
      bool found_edge = false;
      for (temp = node->out; temp->next_out != NULL;
	   temp = temp->next_out)
	{
	  if (temp->next_out == edge)
	    {
	      /* have the previous node link to the next one */
	      temp->next_out = edge->next_out;
	      found_edge = true;
	      break;
	    }
	}
      gcc_assert (found_edge);
    }
}

/* Bypasses SRC and adds an edge from SRC to DEST, returning true if this
 * changes the connection graph */
bool
add_killing_edge (con_node src, con_node dest)
{
  /* if the only edge to/from SRC is the edge to DEST, return false
   * straight away, and do nothing. */
  if (src->in == NULL
      && src->out 
      && src->out->next_out == NULL
      && src->out->target == dest)
    {
      return false;
    }

  if (src->in != NULL)
    {
      gcc_assert (src->out != NULL);
      bypass_node (src);
    }

  add_edge (src, dest);

  return true;
}

static void
assert_no_duplicate_indices (con_graph cg)
{
  sbitmap bm = sbitmap_alloc (10000);
  sbitmap_zero (bm);
  con_node node;
  for (node = cg->root; node; node = node->next)
    {
      gcc_assert (!TEST_BIT (bm, node->dump_index));
      SET_BIT (bm, node->dump_index);
    }
  sbitmap_free (bm);
}

/* Copy the nodes and edges from SRC into DEST */
void
merge_into (con_graph dest, con_graph src)
{
  copy_nodes (dest, src);
  copy_edges (dest, src);
  assert_no_duplicate_indices (dest);
}


/* When searching through a graph, its useful to have markers to note
 * where we've gone before. We just allocate nodes for these, as the we
 * wont have any unlucky memory conflicts */
static con_node END_MARKER;
static con_node NIL_MARKER;

void
init_markers (void)
{
  END_MARKER = new_con_node ();
  NIL_MARKER = new_con_node ();
}

/* Because vector's can reallocate themselves, there's no guarantee that
 * the addres in use at the start of the function and at the end of the
 * function is the same, so we return the new address */
con_node_vec
get_points_to (con_node source, con_node_vec vector)
{
  con_edge edge;
  gcc_assert (is_reference_node (source));

  source->next_link = NIL_MARKER;

  for (edge = source->out; edge != NULL; edge = edge->next_out)
    {
      con_node next_node = edge->target;

      /* recurse more */
      if (is_reference_node (next_node))
	{
	  /* traversed nodes have their next_link set to NIL_MARKER */
	  /* If a terminal node has two incoming edges, this assertion
	   * should fail, but it hasnt yet. odd. */
	  if (next_node->next_link == NULL)
	    vector = get_points_to (next_node, vector);
	}
      else
	{
	  /* already returned nodes have their next_link set to END_MARKER
	   * */
	  gcc_assert (next_node->next_link != NIL_MARKER);
	    
	  if (next_node->next_link != END_MARKER)
	    {
	      VEC_safe_push (con_node, heap, vector, next_node);
	      next_node->next_link = END_MARKER;
	    }
	}
    }
  gcc_assert (source->next_link == NIL_MARKER);
  source->next_link = NULL;
  return vector;
}


con_node
get_single_named_field_node (con_node node, tree field_id)
{
  con_edge edge;

  gcc_assert (node);
  gcc_assert (field_id);
  gcc_assert (node->type == OBJECT_NODE);


  /* get the field nodes from the target of the field edges, and build
   * a list of them */
  for (edge = node->out; edge != NULL; edge = edge->next_out)
    {
      gcc_assert (edge->type == FIELD_EDGE);
      gcc_assert (edge->target->type == FIELD_NODE);
      gcc_assert (edge->target->next_link == NULL);

      /* add it to the result list */
      if (edge->target->id == field_id)
	return edge->target;
    }

  return NULL;
}

void
prune_con_graph (con_graph cg)
{
  con_node node;
  for (node = cg->root; node; node = node->next)
    {
      if (node->type == LOCAL_NODE
	  && TREE_CODE (node->id) == SSA_NAME)
	remove_con_node (node, true);
    }
}

con_node 
get_field_nodes (con_node_vec nodes, tree field_id)
{
  con_edge edge;
  con_node result = NULL;
  con_node node;
  int i;

  fprintf (dump_file, "Finding field node ");
  tf (field_id, false);
  nl ();

  gcc_assert (nodes && field_id);

  for (i = 0; VEC_iterate (con_node, nodes, i, node); i++)
    {
      bool field_found = false;
      gcc_assert (node->type == OBJECT_NODE);

      /* get the field nodes from the target of the field edges, and
       * build a list of them */
      for (edge = node->out; edge != NULL; edge = edge->next_out)
	{
	  gcc_assert (edge->type == FIELD_EDGE);
	  gcc_assert (edge->target->type == FIELD_NODE);
	  gcc_assert (edge->target->next_link == NULL);

	  /* add it to the result list */
	  if (edge->target->id == field_id)
	    {
	      fprintf (dump_file, "found it in ");
	      df (edge->source);
	      /* TODO does this leave a possability of a circular list, or
	       * should we check the entire list */
	      if (result && result != edge->target)
		{
		  edge->target->next_link = result;
		}
	      result = edge->target;

	      /* no need to check the other fields */
	      field_found = true;
	      break;
	    }
	}

      /* lazily add the field */
      if (!field_found)
	{
	  con_node field;
	  gcc_assert (get_existing_field_node (node->graph, field_id, node->uid, node->id) == NULL);
	  field = add_field_node (node->graph, field_id);
	  fprintf (dump_file, "adding lazily to ");
	  df (node);
	  nl ();

	  if (node->phantom) field->phantom = true;
	  add_edge (node, field);

	  /* link it on the front */
	  field->next_link = result;
	  result = field;
	}
    }
  nl ();
  return result;
}


/* This returns the address because it can change when pushed onto */
con_node_vec
get_field_nodes_vec (con_node object, con_node_vec fields)
{
  con_edge edge;

  gcc_assert (object);
  gcc_assert (object->type == OBJECT_NODE);

  /* get the field objects from the target of the field edges, and build
   * a list of them */
  for (edge = object->out; edge != NULL; edge = edge->next_out)
    {
      gcc_assert (edge->type == FIELD_EDGE);
      gcc_assert (edge->target->type == FIELD_NODE);

      /* add it to the result list */
      VEC_safe_push (con_node, heap, fields, edge->target);
    }

  return fields;
}

/* Redirects incoming deferred edges of NODE, so that the edge is removed,
 * and an edge between every incoming edge and every outgoing edge of
 * NODE. Returns false if all these edges already existed. */
/* Note that bypassing a node doesnt mark the graph as changed. The node
 * isnt conceptually changed, as all nodes point to the same thing they
 * did before. */
bool
bypass_node (con_node node)
{
  con_edge in;
  con_edge out;

  /* Dont bypass static fields */
  if (TREE_CODE (node->id) == VAR_DECL)
    {
      fprintf (dump_file, "Static field, dont bypass\n");
      return false;
    }

  fprintf (dump_file, "Bypassing ");
  df (node);
  nl ();
  gcc_assert (node->type != OBJECT_NODE);
  /* its time to admit that we can have end nodes */
//  gcc_assert (node->out != NULL); /* this shouldnt be an end node */

  bool new_edge = false;

  for (in = node->in; in != NULL; in = in->next_in)
    {
      gcc_assert (in->type != POINTS_TO_EDGE);
      if (in->type == DEFERRED_EDGE)
	{
	  for (out = node->out; out != NULL; out = out->next_out)
	    {
	      if (!get_edge (in->source, out->target))
		{
		  add_edge (in->source, out->target);
		  new_edge = true;
		}
	    }
	  remove_con_edge (in);
	}
      else
	gcc_assert (in->type == FIELD_NODE);
    }

  while (node->out)
    {
      remove_con_edge (node->out);
    }

  return new_edge;
}

void
bypass_every_node (con_graph cg)
{
  con_node node;
  for (node = cg->root; node; node = node->next)
    {
      con_edge in;

      for (in = node->in; in != NULL; in = in->next_in)
	{
	  /* only replace deferred and points to edges */
	  if (in->type == DEFERRED_EDGE)
	    {
	      con_edge out;
	      for (out = node->out; out != NULL; out = out->next_out)
		if (!get_edge (in->source, out->target))
		  add_edge (in->source, out->target);

	      remove_con_edge (in);
	    }
	}
      /* dont remove outgoing edges */
    }
}

/* finds the last node in a chain, and returns a list of them. Objects
 * are not returned or traversed. */
con_node
get_terminal_nodes (con_node node)
{
  con_edge edge;
  con_node result = NULL;

  gcc_assert (node);
  gcc_assert (is_reference_node (node));
  gcc_assert (node->next_link == NULL);

  /* if there are outward nodes, but they point to objects or are
   * circular, then we should return null. Note that circular references
   * ONLY occur when there is an object present. */

  if (node->out == NULL)
    {
      node->next_link = END_MARKER;
      return node;
    }

  node->next_link = NIL_MARKER;

  for (edge = node->out; edge != NULL; edge = edge->next_out)
    {
      con_node next_node = edge->target;
      if (is_reference_node (next_node) && next_node->next_link == NULL)
	{
	  con_node new_result = get_terminal_nodes (next_node);
	  result = merge_next_lists (result, new_result);
	}
    }
  gcc_assert (node->next_link == NIL_MARKER);
  node->next_link = NULL;
  gcc_assert (node != result);
  return result;
}

con_node
get_nodes_reachable_from (con_node source)
{
  /* This is temporary. As we find more reachable node, we add them on
   * to the end of the list, then traverse their edges. Note that this
   * will only work for flow-insensitive, as there are no back edge.
   * */
  /* TODO can there be back edges? yes there can. We shall need a
   * traversed bit. But we'll need that anyway, later */
  con_node result = source;
  con_node tail = source;
  con_node node;
  con_edge edge;

  /* we could do this recursively, but we shant, as we'd have to
   * append the list, then traverse it to find the new end, and this
   * way is simpler */
  for (node = source; node; node = node->next_link)
    {
      for (edge = node->out; edge; edge = edge->next_out)
	{
	  /* check both that the new node isnt in the list (if it were
	   * last it wouldnt have a next_link, so compare pointers */
	  if (edge->target->next_link == NULL && edge->target != tail)
	    {
	      /* append the edge's target for further analysis */
	      tail->next_link = edge->target;
	      tail = tail->next_link;
	    }
	  else
	    {
	      /* it's already in the list */
	    }
	}
    }

  /* result includes source, at the start of the list. We dont want
   * that */
  result = result->next_link;
  source->next_link = NULL;
  return result;

}

void
assert_all_next_link_free (con_graph cg)
{
  con_node node;
  for (node = cg->root; node; node = node->next)
    {
      gcc_assert (node->next_link == NULL);
    }
}

void
update_escape_state (con_node source, con_node target)
{
  gcc_assert (source);
  gcc_assert (target);

  if (source->escape == EA_GLOBAL_ESCAPE)
    target->escape = EA_GLOBAL_ESCAPE;
}

void
d (con_node node)
{
  if (node == NIL_MARKER)
    {
      fprintf (stderr, "NIL_MARKER\n");
    }
  else if (node == END_MARKER)
    {
      fprintf (stderr, "END_MARKER\n");
    }
  else if (node == NULL)
    {
      fprintf (stderr, "0x0\n");
    }
  else
    {
      print_generic_stmt (stderr, node->id, 0);
    }
}

static char* 
get_serialized_filename (tree function)
{
  return concat ("serialized/", 
		 IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (function)),
		 NULL);
}

void
serialize_con_graph (con_graph cg)
{
  FILE* file = fopen (get_serialized_filename (cg->function), "w");
  con_node node;
  /* renumber the nodes from 1 - do this now else the owner might not be
   * renumbered when we dump the field */
  int i = 1;
  for (node = cg->root; node != NULL; node = node->next)
    node->dump_index = i++;

  int count = i-1;

  /* # number :: type (0-6) :: escape (0,1,2) :: 
	   phantom (true/false) :: ID (string) :: 
	   owner (string if field, blank otherwise) :: 
	   call_id (string)\n*/
  fprintf (file, "%d\n", count);
  for (node = cg->root; node != NULL; node = node->next)
    {
      fprintf (file, "%d::%d::%d::%d::%d::",
	       node->dump_index, node->type, node->escape, node->phantom,
	       node->uid);

      print_generic_expr (file, node->id, 0);

      fprintf (file, "::");
      if (node->type == FIELD_NODE)
	{
	  con_node owner = get_existing_node (cg, node->owner);
	  gcc_assert (owner);
	  print_generic_expr (file, owner->call_id, 0);
	}
      else
	fprintf (file, " ");
      fprintf (file, "::");

      if (node->call_id)
	print_generic_expr (file, node->call_id, 0);
      else
	fprintf (file, " ");
      fprintf (file, "::\n");
    }

  for (node = cg->root; node != NULL; node = node->next)
    {
      con_edge out;
      for(out = node->out; out; out = out->next_out)
	{
	  fprintf (file, "%d||%d||%d\n", out->source->dump_index,
		   out->target->dump_index, out->type);
	}
    }

  /* now edges */
  fclose (file);
}

static con_node
get_node_numbered (con_graph cg, int number)
{
  con_node node;
  for (node = cg->root; node; node = node->next)
    {
      if (node->dump_index == number)
	return node;
    }
  gcc_unreachable ();
}

con_graph
deserialize_con_graph (tree function)
{
  char* filename = get_serialized_filename (function);
  FILE* file = fopen (filename, "r");
  if (!file) return NULL;

  char buffer[1000];
  con_graph cg = new_con_graph (function, 1 /*EXIT_BLOCK*/, 0);
  cg->deserialized = true;

  /* Find the number of nodes in the graph */
  buffer[999] = '\0'; /* set last char as NULL */
  char* result = fgets (buffer, 999, file);
  gcc_assert (result);
  int num_nodes = atoi (result);

  int i;
  int saved_dump_index = cg->dump_index;
  for (i = 0; i < num_nodes; i++)
    {
      con_node node = new_con_node ();

      /* line */
      buffer[999] = '\0'; /* set last char as NULL */
      char* result = fgets (buffer, 999, file);
      if (result == NULL) break;
      gcc_assert (buffer[999] == '\0');

      /* number */
      char* number = strtok (buffer, "::");
      gcc_assert (number != NULL);
/*      node->dump_index = atoi (number);*/

      /* type */
      char* type = strtok (NULL, "::");
      gcc_assert (type != NULL);
      node->type = atoi (type);

      /* escape */
      char* escape = strtok (NULL, "::");
      gcc_assert (escape != NULL);
      node->escape = atoi (escape);
      
      /* phantom */
      char* phantom = strtok (NULL, "::");
      gcc_assert (phantom != NULL);
      node->phantom = atoi (phantom);

      /* uid */
      char* uid = strtok (NULL, "::");
      gcc_assert (uid != NULL);
      node->uid = atoi (uid);

      /* id (string) */
      char* id = strtok (NULL, "::");
      gcc_assert (id != NULL);
      node->id = NULL;
      if (node->uid == 0)
	node->id = get_identifier (id);

      /* owner (string) - this works cause an owner is a tree,
       * representing the identifier name. Since we use get_identifier,
       * these will use the same  */
      char* owner = strtok (NULL, "::");
      gcc_assert (owner != NULL);
      if (strcmp (owner, " ") != 0)
	node->owner = get_identifier (owner);

      /* call_id (string) - as above */
      char* call_id = strtok (NULL, "::");
      gcc_assert (call_id != NULL);
      if (strcmp (call_id, " ") != 0)
	node->call_id = get_identifier (call_id);

      add_node (cg, node);
    }

  while (file && !feof (file))
    {
      char buffer[1000];
      buffer[999] = '\0'; /* set last char as NULL */
      char* result = fgets (buffer, 999, file);
      if (result == NULL) break;
      gcc_assert (buffer[999] == '\0');

      /* source (string) - as above */
      char* source = strtok (buffer, "||");
      gcc_assert (source != NULL);
      con_node src = get_node_numbered (cg, 
					saved_dump_index + atoi (source) - 1);
      gcc_assert (src);

      /* target (string) - as above */
      char* target = strtok (NULL, "||");
      gcc_assert (target != NULL);
      con_node dest = get_node_numbered (cg, 
					 saved_dump_index + atoi (target) - 1);
      gcc_assert (dest);

      /* target (string) - as above */
      char* type_string = strtok (NULL, "||");
      gcc_assert (type_string != NULL);
      unsigned int type = atoi (type_string);

      con_edge edge = add_edge (src, dest);
      gcc_assert (edge->type == type);
    }

  fclose (file);
  return cg;
}

void 
l (con_graph cg)
{
  gcc_assert (cg);
  con_node node;
  fprintf (stderr, "\nDI    T     UID S ESC P NL Name                 (Extra)\n");
  for (node = cg->root; node != NULL; node = node->next)
    {
      /* guarantees that there is a node printed for each node in
       * the graph */
      fprintf (stderr, "%-4d ", node->dump_index);

      switch (node->type)
	{
	case CALLEE_ACTUAL_NODE: fprintf (stderr, "AE%-3d ",
					  node->dump_index); break;
	case CALLER_ACTUAL_NODE: fprintf (stderr, "AR%-3d ",
					  node->dump_index); break;
	case FIELD_NODE:	fprintf (stderr, " F    "); break;
	case OBJECT_NODE:	fprintf (stderr, " O    "); break;
	case LOCAL_NODE:	fprintf (stderr, " L    "); break;
	case GLOBAL_NODE:	fprintf (stderr, " G    "); break;
	case RETURN_NODE:	fprintf (stderr, " RX   "); break;
	default: gcc_unreachable (); break;
	}

      fprintf (stderr, "%-4d ", node->uid);
      fprintf (stderr, "%s   ", 
	       TREE_CODE (node->id) == IDENTIFIER_NODE ?  "S" : "P");
      /* color the border and assign groups based on the nodes escape-ness
       * */
      switch (node->escape)
	{
	case EA_NO_ESCAPE:	fprintf (stderr, "N "); break;
	case EA_ARG_ESCAPE:	fprintf (stderr, "A "); break;
	case EA_GLOBAL_ESCAPE:	fprintf (stderr, "G "); break;
	default: gcc_unreachable (); break;
	}

      fprintf (stderr, "%d ", node->phantom);
      fprintf (stderr, "%-4d", node->next_link ?
	       node->next_link->dump_index : 0);
      print_generic_expr (stderr, node->id, 0);


      switch (node->type)
	{
	case CALLEE_ACTUAL_NODE:
	  fprintf (stderr, "  (call_id -- ");
	  print_generic_expr (stderr, node->call_id, 0);
	  fprintf (stderr, ")");
	  break;
	case CALLER_ACTUAL_NODE:
	  fprintf (stderr, "  (call_id -- ");
	  print_generic_expr (stderr, node->call_id, 0);
	  fprintf (stderr, ")");
	  break;
	case FIELD_NODE:
	  fprintf (stderr, "  (owner -- ");
	  print_generic_expr (stderr, node->owner, 0);
	  fprintf (stderr, ")");
	  break;
	case OBJECT_NODE:
	  fprintf (stderr, "  (class -- REMOVED)");
	  break;
	case LOCAL_NODE:
	case GLOBAL_NODE:
	case RETURN_NODE:
	  break;
	default: gcc_unreachable (); break;
	}

      fprintf (stderr, "\n");
    }
}

void 
set_escape_state (con_node node, enum ea_escape_state state)
{
  if (node->escape < state)
    {
      node->escape = state;
    }
}

int 
link_length (con_node node)
{
  int result = 0;
  gcc_assert (node);
  while (node)
    {
      result++;
      NEXT_LINK (node);
    }
  gcc_assert (result > 0);
  return result;
}

void 
clear_links (con_node node)
{
  con_node current = node;
  gcc_assert (current);
  while (current)
    {
      con_node next = current->next_link;
      current->next_link = NULL;
      current = next;
    }
}

bool
in_link_list (con_node list, con_node subject)
{
  while (list)
    {
      if (list == subject) return true;
      NEXT_LINK (list);
    }
  return false;
}

con_node
merge_next_lists (con_node list1, con_node list2)
{
  con_node result = list1;
  if (list1 == NULL && list2 == NULL)
    return NULL;

  if (list1 == NULL)
    {
      gcc_assert (last_link (list2) == END_MARKER);
      return list2;
    }
  if (list2 == NULL)
    {
      gcc_assert (last_link (list1) == END_MARKER);
      return list1;
    }
  gcc_assert (last_link (list1) == END_MARKER);
  gcc_assert (last_link (list2) == END_MARKER);

  while (list1->next_link 
	 && list1->next_link != END_MARKER 
	 && list1->next_link != NIL_MARKER)
    {
      NEXT_LINK (list1);
    }
  list1->next_link = list2;
  return result;
}

con_node
last_link (con_node node)
{
  while (node->next_link)
    {
      NEXT_LINK (node);
    }
  return node;
}


static void
clear_end_markers (con_node_vec vector)
{
  int i;
  con_node node;
  gcc_assert (vector);

  for (i = 0; VEC_iterate (con_node, vector, i, node); i++)
    {
      gcc_assert (node->next_link == END_MARKER);
      node->next_link = NULL;
    }
}

static bool
VEC_contains (con_node_vec vector, con_node node)
{
  int i;
  con_node vnode;
  gcc_assert (vector && node);

  for (i = 0; VEC_iterate (con_node, vector, i, vnode); i++)
    {
      if (vnode == node)
	{
	  return true;
	}
    }

  return false;
}

void
VEC_print (con_node_vec vector)
{
  int i;
  con_node node;

  for (i = 0; VEC_iterate (con_node, vector, i, node); i++)
    {
      d (node);
    }
}

/* keep these here so they can be resized */
static con_node_vec ps;
static con_node_vec ts;


static void
get_ps_and_ts (con_node source)
{
  con_edge edge;
  gcc_assert (source && ps && ts);
  gcc_assert (is_reference_node (source));
  gcc_assert (source->next_link == NULL);

  /* if there are outward nodes, but they point to objects or are
   * circular, then we should return null */
  source->next_link = NIL_MARKER;

  if (source->out == NULL)
    {
      source->next_link = END_MARKER;
      VEC_safe_push (con_node, heap, ts, source);
      return;
    }

  for (edge = source->out; edge != NULL; edge = edge->next_out)
    {
      con_node next_node = edge->target;

      /* recurse more */
      if (is_reference_node (next_node))
	{
	  /* traversed nodes have their next_link set to NIL_MARKER */
	  /* If a terminal node has two incoming edges, this assertion
	   * should fail, but it hasnt yet. odd. */
	  if (next_node->next_link == NULL)
	    get_ps_and_ts (next_node);
	}
      else
	{
	  /* already returned nodes have their next_link set to END_MARKER
	   * */
	  gcc_assert (next_node->next_link != NIL_MARKER);
	    
	  if (next_node->next_link != END_MARKER)
	    {
	      unsigned count = VEC_length (con_node, ps);
	      next_node->next_link = END_MARKER;
	      VEC_safe_push (con_node, heap, ps, next_node);
	      gcc_assert (VEC_length (con_node, ps) == count + 1);
	    }
	}
    }
  gcc_assert (source->next_link == NIL_MARKER);
  source->next_link = NULL;
}

/* If a phantom node needs to be created, use PHANTOM_ID */
con_node_vec
get_points_to_and_terminals (con_graph cg, con_node source, tree phantom_id)
{
  con_node phantom;
  con_node terminal; /* used to iterate through the terminal vector */
  int i;
  con_node_vec u;
  con_node_vec terminals;
  gcc_assert (cg && source && phantom_id);
  gcc_assert (is_reference_node (source));
  gcc_assert (cg == source->graph);

  /* This comes from the end of section 3 in Choi 99, though we dont
   * exactly follow the recommendations. In particular, the paper alleges
   * that you only use phantom nodes if there are no points to nodes, but
   * in reality, there can be both at the same time.  */

  gcc_assert (ps == NULL);
  gcc_assert (ts == NULL);
  ps = VEC_alloc (con_node, heap, 10);
  ts = VEC_alloc (con_node, heap, 10);

  /* get the objects pointed */
  get_ps_and_ts (source);
  u = ps;
  terminals = ts;

  ps = NULL;
  ts = NULL;


  /* there may also be nodes which don't end up as an object. These need
   * to point to phantom objects. Note that the final node in this list
   * points to END_MARKER */

  /* this is a lonely node */
  if (VEC_empty (con_node, terminals) && VEC_empty (con_node, u))
    {
      gcc_assert (source->out == NULL);
      source->next_link = END_MARKER;
      VEC_quick_push (con_node, terminals, source);
    }

  /* get the phantom node ready, if needed */
  if (!VEC_empty (con_node, terminals))
    {
      /* TODO there should be a dependecny here */
      phantom = get_existing_node (cg, phantom_id); /* 1-limited */
      if (phantom == NULL)
	{
	  /* This id will only belong to this node, and no other one,
	   * because gimple doesnt have statements in the form 
	   * x.y = new X();
	   * which would allow a phantom and an object node to be created
	   * with the same phantom_id; */
	  phantom = add_object_node (cg, phantom_id);
	  tf (phantom_id, false);
	  fprintf (dump_file, "Adding a new phantom\n");
	  phantom->phantom = true;
	}
      else
	{
	  tf (phantom_id, false);
	  fprintf (dump_file, "Reusing an existing phantom\n");
	}
      gcc_assert (phantom->phantom);

      /* add the phantom the the list of u, and make space for the
       * terminals list, which will be added soon */
      if (!VEC_contains (u, phantom))
	{
	  phantom->next_link = END_MARKER;
	  VEC_safe_push (con_node, heap, u, phantom);
	}
      gcc_assert (phantom->next_link == END_MARKER);

      /* make the terminal nodes point to the node */
      for (i = 0; VEC_iterate (con_node, terminals, i, terminal); i++)
	{
	  gcc_assert (terminal != phantom);
	  add_edge (terminal, phantom);
	}
    }

  clear_end_markers (terminals);
  clear_end_markers (u);

  VEC_free (con_node, heap, terminals);
  
  return u;
}


/* This function, and the symbols in it come Toplas03, Figure 7
 * (Choi99 has the same code, but different symbol names). I call the
 * objects 'o' instead of 'n_o', as this is more intuitive (o is for
 * object) */
/* The algorithm in Choi 99 and Toplas03 indicates a vector for the second
 * parameter, but the vector is never added to, and only contains 1 item,
 * so we're using a scalar here. */
/* _bar nodes belong to the caller, and the other nodes belong to
 * the callee */
void
update_nodes (con_node f, con_node f_bar, tree call_id)
{
  int i, j, k;
  con_node o, g, o_bar;
  con_node_vec points_to_f;
  con_node_vec points_to_f_bar;

  gcc_assert ((f_bar->type == f->type && f->type == FIELD_NODE)
	      || ((f_bar->type == LOCAL_NODE) && is_actual_type (f)));

  add_to_maps_to_obj (f, f_bar);
  update_escape_state (f, f_bar);

  /* get the nodes pointed to by each variable */
  points_to_f = get_points_to (f, VEC_alloc (con_node, heap, 10));
  clear_end_markers (points_to_f);

  /* fetch out the information in case a node needs to be made */
  points_to_f_bar = get_points_to_and_terminals (f_bar->graph, f_bar, call_id);

  for (i = 0; VEC_iterate (con_node, points_to_f, i, o); i++)
    {
      gcc_assert (o->type == OBJECT_NODE);
      gcc_assert (o->graph->function == f->graph->function);
      gcc_assert (o->escape != EA_NO_ESCAPE);

      for (j = 0; VEC_iterate (con_node, points_to_f_bar, j, o_bar); j++)
	{
	  gcc_assert (o_bar->graph->function != f->graph->function);
	  gcc_assert (o_bar->graph->function == f_bar->graph->function);
	  gcc_assert (o_bar->type == OBJECT_NODE);
	  if ( !in_maps_to_obj (o, o_bar))
	    {
	      con_node_vec fields;

	      add_to_maps_to_obj (o, o_bar);
	      update_escape_state (o, o_bar);
	      fields = get_field_nodes_vec (o, VEC_alloc (con_node, heap,
							  1000));
	      for (k = 0; VEC_iterate (con_node, fields, k, g); k++)
		{
		  con_node g_bar = get_single_named_field_node (o_bar,
								g->id);
		  /* this can be null because we add field nodes lazily.
		   * so if the callee uses a field, but the caller doesnt,
		   * then it needs to be added to the caller graph */

		  if (g_bar == NULL)
		    { 
		      gcc_assert (get_existing_field_node_with_call_id
				  (o_bar->graph, g->id, g->uid, o_bar->id, call_id) ==
				  NULL);

		      /* No call_id here, since the field belongs to an
		       * object */
		      g_bar = add_field_node (o_bar->graph, g->id);
		      if (o_bar->phantom) g_bar->phantom = true;

		      add_edge (o_bar, g_bar);
		    }
		  gcc_assert (g_bar);
		  update_nodes (g, g_bar, call_id);
		}
	    }
	}
    }
}

void
add_to_maps_to_obj (con_node node, con_node mapped_to)
{
  gcc_assert (node && mapped_to);
  if (node->maps_to_obj == NULL)
    node->maps_to_obj = VEC_alloc (con_node, heap, 10);

  if (!VEC_contains (node->maps_to_obj, mapped_to))
    {
      VEC_safe_push (con_node, heap, node->maps_to_obj, mapped_to);
    }
}

bool
in_maps_to_obj (con_node node, con_node mapped_to)
{
  gcc_assert (node && mapped_to);
  if (node->maps_to_obj == NULL)
    return false;

  return VEC_contains (node->maps_to_obj, mapped_to);
}


