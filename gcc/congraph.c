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

/* -------------------------------------------------------------*
 *			memory and hashtables			*
 * -------------------------------------------------------------*/
static htab_t con_node_hashtable;

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
  return (n1->id == n2->id 
	  && n1->graph == n2->graph
	  && n1->call_id == n2->call_id
	  && n1->index == n2->index);
}

void
init_con_node_hashtable (void)
{
  /* 9 was the average for the data I had on hand */
  con_node_hashtable = htab_create (20, con_node_id_hash, htab_con_node_eq, free);
}

static htab_t statement_type_hashtable;

static hashval_t
statement_type_hash (const void *p)
{
  const stmt_type_container container = (stmt_type_container) p;
  return htab_hash_pointer (container->stmt);
}

static int
htab_statement_type_eq (const void *p1, const void *p2)
{
  const stmt_type_container container1 = (stmt_type_container) p1;
  const stmt_type_container container2 = (stmt_type_container) p2;
  return (container1->graph == container2->graph
	  && container1->stmt == container2->stmt);
}

void
init_statement_type_hashtable (void)
{
  /* 9 was the average for the data I had on hand */
  statement_type_hashtable = htab_create (20, statement_type_hash, htab_statement_type_eq, free);
}


/* -------------------------------------------------------------*
 *			graph					*
 * -------------------------------------------------------------*/

/* I used to identify nodes in the dumps by using their address. This is
 * unreliable and doesnt lend itself well to regression. Instead, use the
 * order in which they are created, which appears to be fairly constant */
static int dump_index = 1;

static bool
is_actual_type (con_node node)
{
  return node->type == CALLER_ACTUAL_NODE 
    || node->type == CALLEE_ACTUAL_NODE;
}

/* allocates a new, cleared and inited con_graph */
con_graph
new_con_graph (const char *filename, tree function, con_graph next)
{
  con_graph result = xcalloc (1, sizeof (struct _con_graph));
  gcc_assert (result);

  result->filename = filename;
  result->function = function;
  result->next = next;

  return result;
}

/*  is int so you can do: gcc_assert (cond || con_graph_dump (cg) */
int				
con_graph_dump (con_graph cg)
{

  con_node node;
  con_edge edge;
  FILE *out;

  gcc_assert (cg);
  gcc_assert (cg->function);
  gcc_assert (cg->filename);

  /* occasionally want to dump graphs that arent complete */

  out = fopen (cg->filename, "w");
  if (out == NULL)
      fprintf (stderr, "%s\n", strerror (errno));
  gcc_assert (out);

  /* step 1 - print the nodes and attributes */
  for (node = cg->root; node != NULL; node = node->next)
    {
      con_graph_dump_node (node, out);
    }

  /* step 2 - put them in groups */
  /* do the localgraph */
  fprintf (out, " ( LocalGraph: \n");
  for (node = cg->root; node != NULL; node = node->next)
    {
      if (node->escape == EA_NO_ESCAPE)
	{
	  fprintf (out, " [ %d ]\n", node->dump_index);
	}
    }
  fprintf (out, ") { background: yellow; }\n\n");

  /* do the nonlocalgraph */
  fprintf (out, "( NonLocalGraph: \n");
  for (node = cg->root; node != NULL; node = node->next)
    {
      if (node->escape != EA_NO_ESCAPE)
	{
	  fprintf (out, " [ %d ]\n", node->dump_index);
	}
    }
  fprintf (out, ") { background: lightblue; }\n\n");

  /* step 3 - add the edges */
  for (node = cg->root; node != NULL; node = node->next)
    {
      for (edge = node->out; edge != NULL; edge = edge->next_out)
	{
	  con_graph_dump_edge (edge, out);
	}
    }

  fclose (out);
  return 0;
}

void
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
  fprintf (out, "label: ");

  if (node->phantom)
    fprintf (out, "[");

  switch (node->type)
    {
    case CALLEE_ACTUAL_NODE:
      fprintf (out, "a%d_ee (", node->index);
      print_generic_expr (out, node->id, 0);
      fprintf (out, ")");
      break;

    case CALLER_ACTUAL_NODE:
      fprintf (out, "a%d_er (", node->index);
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
  fprintf (out, "; ");

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

void
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


/* search through the list of connection graphs FUNCTION */
con_graph
get_cg_for_function (con_graph cg, tree function)
{
  con_graph result = cg;
  gcc_assert (cg);
  gcc_assert (function);

  /* this is a cyclic list */
  for (result = cg->next; result != cg; result = result->next)
    {
      if (result->function == function)
	return result;
    }

  /* we've gone the whole circle, and not found it */
  return NULL;
}


/* ---------------------------------------------------
 *			nodes
 * --------------------------------------------------- */


/* Create a new object node representing an allocation of type
 * class, named using id */
con_node
add_object_node (con_graph cg, tree id, tree class_id)
{
  con_node node;
  gcc_assert (cg && id);

  node = new_con_node ();
  cg->num_objects++;
  node->id = id;
  node->class_id = class_id;
  node->type = OBJECT_NODE;
  add_node (cg, node);
  return node;
}

con_node
add_callee_actual_node (con_graph cg, tree id, int index)
{
  con_node node;
  gcc_assert (cg);
  gcc_assert (id);

  node = new_con_node ();
  node->id = id;
  node->index = index;
  node->phantom = true;
  node->type = CALLEE_ACTUAL_NODE;
  node->escape = EA_ARG_ESCAPE;
  add_node (cg, node);
  return node;
}

con_node
add_caller_actual_node (con_graph cg, tree id, int index, tree call_id)
{
  con_node node;
  gcc_assert (cg);
  gcc_assert (id);

  node = new_con_node ();
  node->id = id;
  node->index = index;
  node->phantom = true;
  node->type = CALLER_ACTUAL_NODE;
  node->escape = EA_ARG_ESCAPE;
  node->call_id = call_id;
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
add_return_node (con_graph cg, tree id)
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

con_node
add_global_node (con_graph cg, tree id)
{
  con_node node;
  gcc_assert (cg);
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
  gcc_assert (cg);
  gcc_assert (id);

  node = new_con_node ();
  node->id = id;
  node->type = LOCAL_NODE;
  add_node (cg, node);
  return node;
}

void
add_node (con_graph cg, con_node node)
{
  struct _con_node finder;
  void** slot;
  gcc_assert (cg && node);
  gcc_assert (node->next == NULL);

  /* we search for id, which is duplicated in the case of field and
   * actual nodes */
  if (node->type != FIELD_NODE && !is_actual_type (node))
    gcc_assert (get_existing_node_with_call_id
		(cg, node->id, node->call_id) == NULL);

  /* insert the node at the front of the list */
  node->next = cg->root;
  cg->root = node;

  /* remove this later, this is just for debugging */
  node->graph = cg;

  node->dump_index = dump_index;
  dump_index++;

  /* put the node into the hashtable */
  finder.graph = node->graph;
  finder.id = node->id;
  finder.call_id = node->call_id;
  finder.index = node->index;
  slot = htab_find_slot (con_node_hashtable, &finder, INSERT);
  *slot = (void*) node;
  
}


/* allocates a new, cleared and inited con_node */
con_node
new_con_node (void)
{
  con_node result = xcalloc (1, sizeof (struct _con_node));
  gcc_assert (result);
  result->escape = EA_NO_ESCAPE;
  return result;
}

con_node
get_existing_node (con_graph cg, tree id)
{
  return get_existing_node_with_call_id (cg, id, NULL);
}

con_node
get_existing_node_with_call_id (con_graph cg, tree id, tree call_id)
{
  return find_node (cg, id, call_id, 0);
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
find_node (con_graph cg, tree id, tree call_id, int index)
{
  struct _con_node finder;
  gcc_assert (cg);
  gcc_assert (id);
  finder.graph = cg;
  finder.id = id;
  finder.call_id = call_id;
  finder.index = index;

  return htab_find (con_node_hashtable, &finder);
}

con_node
get_existing_field_node (con_graph cg, tree id, con_node owner)
{
  return get_existing_field_node_with_call_id (cg, id, owner, NULL);
}

con_node
get_existing_field_node_with_call_id (con_graph graph,
				      tree id, con_node owner,
				      tree call_id)
{
  con_node node;
  gcc_assert (graph);
  gcc_assert (id);
  gcc_assert (owner);
  for (node = graph->root; node != NULL; node = node->next)
    {
      if (node->id == id && node->owner == owner
	  && node->call_id == call_id)
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
  gcc_assert (source);
  gcc_assert (target);
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
      target->owner = source;

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

  return edge;
}

void
remove_con_node (con_node node)
{
  con_graph cg;
  void** slot;
  struct _con_node finder;
  /* this is only really designed to remove caller nodes */
  gcc_assert (node);
  gcc_assert (node->in == NULL);
  gcc_assert (node->out);
  gcc_assert (node->out->next_out == NULL);
  gcc_assert (node->type == CALLER_ACTUAL_NODE);

  remove_con_edge (node->out);

  /* remove it from the hashtable and the list of nodes */
  cg = node->graph;
  if (cg->root == node)
    cg->root = node->next;
  else
    {
      con_node prev_node = cg->root;
      while (prev_node->next != node)
	{
	  prev_node = prev_node->next;
	}
      prev_node->next = node->next;
    }
  finder.id = node->id;
  finder.call_id = node->call_id;
  finder.index = node->index;
  finder.graph = node->graph;
  slot = htab_find_slot (con_node_hashtable, &finder, NO_INSERT);
  gcc_assert (*slot == node);
  htab_clear_slot (con_node_hashtable, slot);
}

void
remove_con_edge (con_edge edge)
{
  /* edges dont have a previos_edge field, so we need to iterate
   * through them the find the edge which points at this one */

  con_node node;
  con_edge temp;
  gcc_assert (edge);

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

con_node 
get_field_nodes (con_node_vec nodes, tree field_id)
{
  con_edge edge;
  con_node result = NULL;
  con_node node;
  int i;

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
	  gcc_assert (get_existing_field_node (node->graph, field_id, node) == NULL);
	  field = add_field_node (node->graph, field_id);

	  if (node->phantom) field->phantom = true;
	  add_edge (node, field);

	  /* link it on the front */
	  field->next_link = result;
	  result = field;
	}
    }
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


void
bypass_node (con_node node)
{
  /* TODO look at this again */
  /* what to do if the node has no output node. It's about to be
   * overriden, but the nodes pointing to it have to point to
   * something. Or so it seems. In reality, they were hanging loose,
   * and they still are. No biggie */
  con_edge in;
  con_edge out;

  gcc_assert (node);
  gcc_assert (node->type != OBJECT_NODE);

  /* you dont want to bypass nodes at the start or the end */
  if (node->in == NULL || node->out == NULL)
    {
      return;
    }

  for (in = node->in; in != NULL; in = in->next_in)
    {
      if (in->type == DEFERRED_EDGE)
	{
	  for (out = node->out; out != NULL; out = out->next_out)
	    {
	      gcc_assert (in->type != FIELD_EDGE);

	      add_edge (in->source, out->target);
	    }
	  remove_con_edge (in);
	}
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

con_node
get_caller_actual_node (con_graph cg, int index, tree stmt)
{
  con_node node;
  gcc_assert (cg);
  gcc_assert (stmt);
  for (node = cg->root; node; node = node->next)
    {
      if (node->type == CALLER_ACTUAL_NODE && node->index == index
	  && node->call_id == stmt)
	{
	  return node;
	}
    }
  return NULL;

}

con_node
get_callee_actual_node (con_graph cg, int index, tree id)
{
  con_node node;
  gcc_assert (cg);
  gcc_assert (id);
  for (node = cg->root; node; node = node->next)
    {
      if (node->type == CALLEE_ACTUAL_NODE && node->index == index)
	{
	  gcc_assert (id == node->id);
	  return node;
	}
    }

  return NULL;
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
t (tree id)
{
  print_generic_stmt (stderr, id, 0);
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

void 
l (con_graph cg)
{
  gcc_assert (cg);
  con_node node;
  fprintf (stderr, "DI    T  ESC P NL Name                 (Extra)\n");
  for (node = cg->root; node != NULL; node = node->next)
    {
      /* guarantees that there is a node printed for each node in
       * the graph */
      fprintf (stderr, "%-4d ", node->dump_index);

      switch (node->type)
	{
	case CALLEE_ACTUAL_NODE: fprintf (stderr, "AE%-3d ", node->index); break;
	case CALLER_ACTUAL_NODE: fprintf (stderr, "AR%-3d ", node->index); break;
	case FIELD_NODE:	fprintf (stderr, " F    "); break;
	case OBJECT_NODE:	fprintf (stderr, " O    "); break;
	case LOCAL_NODE:	fprintf (stderr, " L    "); break;
	case GLOBAL_NODE:	fprintf (stderr, " G    "); break;
	case RETURN_NODE:	fprintf (stderr, " RX   "); break;
	default: gcc_unreachable (); break;
	}

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
	  fprintf (stderr, "  (owner -- %d)", node->owner->dump_index);
	  break;
	case OBJECT_NODE:
	  fprintf (stderr, "  (class -- ");
	  print_generic_expr (stderr, node->class_id, 0);
	  fprintf (stderr, ")");
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
inline_constructor_graph (con_graph cg,
			  con_graph constructor_graph, tree call_id)
{
  con_node node = NULL;
  gcc_assert (constructor_graph);
  gcc_assert (cg);
  gcc_assert (call_id);

  /* field nodes must have their owner already present in the caller. So
   * add the objects first */
  for (node = constructor_graph->root; node; node = node->next)
    {
      if (node->type == OBJECT_NODE)
	{
	  con_node result = add_object_node (cg, node->id, node->class_id);
	  result->call_id = call_id;
	  result->phantom = node->phantom;
	}
    }

  /* now add everything except the objects */
  for (node = constructor_graph->root; node; node = node->next)
    {
      con_node result = NULL;
      con_node owner;
      switch (node->type)
	{
	case LOCAL_NODE:
	  result = add_local_node (cg, node->id);
	  result->call_id = call_id;
	  break;

	case FIELD_NODE:
	  /* find this node's owner, without which we cant find it later */
	  owner = get_matching_node_in_caller (cg, node->owner,
							call_id);
	  gcc_assert (owner);
	  result = add_field_node (cg, node->id);
	  result->call_id = call_id;
	  add_edge (owner, result);
	  break;

	case GLOBAL_NODE:
	  /* dont copy global's. They'll be there anyway */
	  break;

	case RETURN_NODE:
	  /* constructors dont have caller results */
	  /* TODO make it so these arent added */
	  gcc_assert (node->out == NULL);
	  gcc_assert (node->in == NULL);
	  gcc_unreachable (); /* i think */
	  break;

	case CALLEE_ACTUAL_NODE:
	  /* no need for this anymore */
	  break;


	case CALLER_ACTUAL_NODE:
	  result = add_caller_actual_node (cg, node->id, node->index,
					   node->call_id);
	  break;

	case OBJECT_NODE:
	  /* handled above */
	  break;

	default:
	  gcc_unreachable ();
	}

      if (result)
	result->phantom = node->phantom;
    }

  /* match the callee nodes in the caller with the caller nodes in the
   * callee */
  for (node = constructor_graph->root; node; node = node->next)
    {
      con_edge edge;
      for (edge = node->out; edge; edge = edge->next_out)
	{
	  /* field nodes will already have their edges with thier owners,
	   * which were added above */
	  con_node source, target;
	  source = get_matching_node_in_caller (cg, edge->source,
						call_id);
	  gcc_assert (source);
	  target = get_matching_node_in_caller (cg, edge->target,
						call_id);
	  gcc_assert (target);
	  if (edge->target->type == CALLEE_ACTUAL_NODE)
	    {
	      /* we override the old state */
	      /* TODO what if it was global? */
	      gcc_assert (target->escape != EA_GLOBAL_ESCAPE);
	      target->escape = EA_NO_ESCAPE;
	    }
	  if (source->type == OBJECT_NODE && target->type == FIELD_NODE)
	    {
	      gcc_assert (get_edge (source, target));
	    }
	    else
	    {
	      add_edge (source, target);
	    }
	}
    }
}


con_node
get_matching_node_in_caller (con_graph cg,
			     con_node node, tree call_id)
{
  gcc_assert (cg);
  gcc_assert (node);
  gcc_assert (node->graph != cg);
  switch (node->type)
    {
      con_node field_owner;

    case FIELD_NODE:
      field_owner = get_matching_node_in_caller (cg, node->owner,
						 call_id);
      gcc_assert (field_owner);
      return get_existing_field_node_with_call_id (cg, node->id,
						   field_owner,
						   call_id);

    case LOCAL_NODE:
    case OBJECT_NODE:
      return get_existing_node_with_call_id (cg, node->id, call_id);

    case GLOBAL_NODE:
      /* globals dont use a call_id */
      return get_existing_node (cg, node->id);

    case CALLEE_ACTUAL_NODE:
      /* Note: not a typo, case CALLEE: return get_caller.. */
      /*a callee matches with a caller, but the callee doesnt have the
       * same id, which is why this function has a call_id parameter*/
      return get_caller_actual_node (cg, node->index, call_id);

    case CALLER_ACTUAL_NODE:
      /* a caller node isnt affected by the inlining, so get the
       * original id */
      return get_caller_actual_node (cg, node->index, node->call_id);

    default:
      gcc_unreachable ();

    }
}

void
set_statement_type (con_graph cg, tree stmt, enum statement_type type)
{
  enum statement_type st = get_statement_type (cg, stmt);
  void** slot;
  gcc_assert (cg && stmt && type);
  if (!st)
    {
      stmt_type_container finder = xcalloc (1, sizeof (struct
						       _stmt_type_container));
      finder->graph= cg;
      finder->stmt = stmt;
      finder->type = type;
      slot = htab_find_slot (statement_type_hashtable, finder, INSERT);
      *slot = (void*)(finder) ;
    }
}

enum statement_type
get_statement_type (con_graph cg, tree stmt)
{
  struct _stmt_type_container finder;
  stmt_type_container result;
  finder.graph = cg;
  finder.stmt = stmt;
  result = htab_find (statement_type_hashtable, &finder);
  if (result)
    return result->type;
  else
    return 0;
}


void 
print_stmt_type (con_graph cg, FILE* file, tree stmt)
{
  enum statement_type data;
  gcc_assert (cg);
  gcc_assert (file);
  gcc_assert (stmt);

  data = get_statement_type (cg, stmt);

  fprintf (file, " (");

  if (data)
    {
      gcc_assert (data >= FUNCTION_CALL);
      switch (data)
	{
#define HANDLE(A) case A: fprintf (file, #A); break;
	  HANDLE(FUNCTION_CALL);
	  HANDLE(FUNCTION_CALL_WITH_RETURN);
	  HANDLE(CONSTRUCTOR_STMT);
	  HANDLE(INDIRECT_FUNCTION_CALL);
	  HANDLE(INDIRECT_FUNCTION_CALL_WITH_RETURN);
	  HANDLE(REFERENCE_COPY);
	  HANDLE(CAST);
	  HANDLE(ASSIGNMENT_FROM_FIELD);
	  HANDLE(ASSIGNMENT_TO_FIELD);
	  HANDLE(ASSIGNMENT_FROM_VTABLE);
	  HANDLE(ASSIGNMENT_FROM_EXCEPTION);
	  HANDLE(ASSIGNMENT_TO_EXCEPTION);
	  HANDLE(RETURN);
	  HANDLE(INDIRECT_GOTO);
	  HANDLE(IGNORED_FUNCTION_POINTER);
	  HANDLE(IGNORED_RETURNING_VOID);
	  HANDLE(IGNORED_NOT_A_POINTER);
	  HANDLE(IGNORED_LABEL_EXPR);
	  HANDLE(IGNORED_COND_EXPR);
	  HANDLE(IGNORED_UNKNOWN);
	  HANDLE(ASSIGNMENT_FROM_ARRAY);
	  HANDLE(ASSIGNMENT_FROM_DATA_ARRAY);
	  HANDLE(POINTER_ARITHMETIC);
	  HANDLE(POINTER_DEREFERENCE);
	  HANDLE(IGNORED_ASSIGNMENT_TO_NULL);
	  HANDLE(ASSIGNMENT_TO_INDIRECT_ARRAY_REF);
	  HANDLE(IGNORED_NULL_POINTER_EXCEPTION);
	  HANDLE(OBJECT_ALLOCATION);
	  HANDLE(OBJECT_ARRAY_ALLOCATION);
	  HANDLE(PRIM_ARRAY_ALLOCATION);
	  HANDLE(MULTI_ARRAY_ALLOCATION);
	}
    }
  else
    {
      fprintf (file, "Unknown");
    }
  fprintf (file, ")");
#undef HANDLE
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

con_node_vec
get_points_to_and_terminals (con_graph cg, con_node source, tree stmt_id, tree type)
{
  con_node phantom;
  con_node terminal; /* used to iterate through the terminal vector */
  int i;
  con_node_vec u;
  con_node_vec terminals;
  gcc_assert (cg && source && stmt_id);
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
      phantom = get_existing_node (cg, stmt_id); /* 1-limited */
      if (phantom == NULL)
	{
	  /* This id will only belong to this node, and no other one,
	   * because gimple doesnt have statements in the form 
	   * x.y = new X();
	   * which would allow a phantom and an object node to be created
	   * with the same stmt_id; */
	  phantom = add_object_node (cg, stmt_id, type);
	  phantom->phantom = true;
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
	      || (is_actual_type (f_bar) && is_actual_type (f)));

  add_to_maps_to_obj (f, f_bar);
  update_escape_state (f, f_bar);

  /* get the nodes pointed to by each variable */
  points_to_f = get_points_to (f, VEC_alloc (con_node, heap, 10));
  clear_end_markers (points_to_f);

  /* fetch out the information in case a node needs to be made */
  points_to_f_bar = get_points_to_and_terminals (f_bar->graph, f_bar, call_id, NULL);

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
				  (o_bar->graph, g->id, o_bar, call_id) ==
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


