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

/* ---------------------------------------------------
 *			graph
 * --------------------------------------------------- */

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

  result->return_node = add_return_node (result);
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
  assert_all_next_link_free (cg);

  out = fopen (cg->filename, "w");
  gcc_assert (out);

  /* step 1 - print the nodes and attributes */
  fprintf (out, " ( LocalGraph: \n");
  for (node = cg->root; node != NULL; node = node->next)
    {
      con_graph_dump_node (node, out);
    }

  /* step 2 - put them in groups */
  /* do the localgraph */
  for (node = cg->root; node != NULL; node = node->next)
    {
      if (node->escape == EA_NO_ESCAPE)
	{
	  fprintf (out, " [ %p ]\n", (void *) node);
	}
    }
  fprintf (out, ") { background: yellow; }\n\n");

  /* do the nonlocalgraph */
  fprintf (out, "( NonLocalGraph: \n");
  for (node = cg->root; node != NULL; node = node->next)
    {
      if (node->escape != EA_NO_ESCAPE)
	{
	  fprintf (out, " [ %p ]\n", (void *) node);
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
  gcc_assert (node->type == OBJECT_NODE ? node->class_id != NULL : true);
  gcc_assert (node->graph);
  /* print the nodes first, so that we can annotate them */

  /* guarantees that there is a node printed for each node in
   * the graph */
  fprintf (out, "[ %p ] ", (void *) node);
  fprintf (out, "{ ");
  fprintf (out, "size: 1,1; ");

  /* print the label */
  fprintf (out, "label: ");
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
      print_generic_expr (out, node->id, 0);
      break;

    case RETURN_NODE:
      fprintf (out, "RETURN");
      break;

    default:
      gcc_unreachable ();
      break;
    }
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
  if (node->phantom)
    {
      fprintf (out, "border-width: 3px; ");
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
  fprintf (out, "[ %p ] ", (void *) source);

  /* draw the edge */
  fprintf (out, " -> ");
  switch (edge->type)
    {
    case POINTS_TO_EDGE:
      fprintf (out, " { label: P; }");
      break;

    case DEFERRED_EDGE:
      fprintf (out, " { label: D; style: dashed; }");
      break;

    case FIELD_EDGE:
      fprintf (out, " { label: F; }");
      break;

    default:
      gcc_unreachable ();
    }

  /* print the target */
  fprintf (out, "[ %p ] ", (void *) target);

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
  gcc_assert (cg);
  gcc_assert (id);
  gcc_assert (class_id);

  node = new_con_node ();
  cg->num_objects++;
  node->id = id;
  node->class_id = class_id;
  node->type = OBJECT_NODE;
  add_node (cg, node);
  return node;
}

con_node
add_callee_actual_node (con_graph cg, tree id, int index, tree stmt)
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
  node->call_id = stmt;
  add_node (cg, node);
  return node;
}

con_node
add_caller_actual_node (con_graph cg, tree id, int index,
			tree call_id)
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
add_return_node (con_graph cg)
{
  con_node node;
  gcc_assert (cg);

  node = new_con_node ();
  /* HACK TODO this is very bold. fix it */
  /* so that it wont be the same as another id */
  node->id = (tree) cg;
  node->type = RETURN_NODE;
  node->escape = EA_ARG_ESCAPE;
  node->phantom = true;
  add_node (cg, node);
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
  gcc_assert (cg);
  gcc_assert (node);
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
  con_node node;
  con_node result = NULL;
  gcc_assert (cg);
  gcc_assert (id);
  for (node = cg->root; node != NULL; node = node->next)
    {
      /* an actual node has the same id, but a different type */
      if (node->id == id && node->call_id == call_id
	  && !is_actual_type (node))
	{
	  /* For field nodes use get_existing_field_nodes */
	  gcc_assert (node->type != FIELD_NODE);

	  /* check for dups */
	  gcc_assert (result == NULL);
	  result = node;
	}
    }

  return result;
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
  con_edge edge = NULL;
  con_edge temp;

  for (temp = source->out; temp != NULL; temp = temp->next_out)
    {
      if (temp->target == target)
	{
	  /* this means we've found two such edges */
	  gcc_assert (edge == NULL);

	  edge = temp;
	}
    }

  if (edge)
    {
      return edge;
    }

  for (temp = target->in; temp != NULL; temp = temp->next_in)
    {
      /* SANITY CHECK - we should have found this edge in the
       * first loop, but didnt */
      gcc_assert (temp->source != source);
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

con_node
get_points_to (con_node node)
{
  con_edge edge;
  con_node result = NULL;

  gcc_assert (node);
  gcc_assert (node->type != OBJECT_NODE);

  for (edge = node->out; edge != NULL; edge = edge->next_out)
    {
      con_node new_result = NULL;

      if (edge->target->type == OBJECT_NODE)
	{
	  /* its not already in the list */
	  if (edge->target->next_link == NULL)
	    {
	      new_result = edge->target;
	    }
	}
      else
	{
	  /* recurse */
	  new_result = get_points_to (edge->target);
	}

      /* new list or append to list */
      if (result)
	{
	  result->next_link = new_result;
	}
      else
	{
	  result = new_result;
	}
    }
  return result;
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

  if (!node->phantom)
    gcc_unreachable ();

  return NULL;
}

con_node
get_field_nodes (con_node node, tree field_id)
{
  con_node next_node;
  con_edge edge;
  con_node result = NULL;

  gcc_assert (node);
  gcc_assert (field_id);

  /* iterate through the nodes using next_link */
  while (node)
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
	      if (result == NULL)
		{
		  result = edge->target;
		}
	      else
		{
		  result->next_link = edge->target;
		}

	      /* no need to check the other fields */
	      field_found = true;
	      break;
	    }
	}

      /* there must be a field of this type (unless its a phantom) */
      gcc_assert (field_found || node->phantom);

      /* clear the next_link */
      next_node = node->next_link;
      node->next_link = NULL;
      node = next_node;
    }
  return result;
}

con_node *
get_field_nodes_vec (con_node node)
{
  con_edge edge;
  con_node *result = XCNEWVEC (con_node, 100);
  int count = 0;

  gcc_assert (node);
  gcc_assert (node->type == OBJECT_NODE);

  /* get the field nodes from the target of the field edges, and build
   * a list of them */
  for (edge = node->out; edge != NULL; edge = edge->next_out)
    {
      gcc_assert (edge->type == FIELD_EDGE);
      gcc_assert (edge->target->type == FIELD_NODE);

      /* add it to the result list */
      result[count++] = edge->target;
    }

  result = XRESIZEVEC (con_node, result, count + 1);
  return result;
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
  gcc_assert (node->type != OBJECT_NODE);
  gcc_assert (node->next_link == NULL);

  for (edge = node->out; edge != NULL; edge = edge->next_out)
    {
      con_node new_result = NULL;

      if (is_reference_node (edge->target))
	{
	  if (edge->target->out == NULL)
	    {
	      /* end of the line */
	      new_result = edge->target;
	    }
	  else
	    {
	      /* recurse */
	      new_result = get_terminal_nodes (edge->target);
	    }
	}
      else
	{
	  /* object node - do nothing */
	}

      /* TODO it may be that new_result == node. Then what? */
      if (new_result)
	{
	  /* new list or append to list */
	  new_result->next_link = result;
	  result = new_result;
	}
    }

  /* this can return null */
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
get_callee_actual_node (con_graph cg, int index, tree call_id)
{
  con_node node;
  gcc_assert (cg);
  gcc_assert (call_id);
  for (node = cg->root; node; node = node->next)
    {
      if (node->type == CALLEE_ACTUAL_NODE && node->index == index
	  && node->call_id == call_id)
	{
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
  print_generic_stmt (stderr, node->id, 0);
}


void
inline_constructor_graph (con_graph cg,
			  con_graph constructor_graph, tree call_id)
{
  con_node node = NULL;
  gcc_assert (constructor_graph);
  gcc_assert (cg);
  gcc_assert (call_id);
  for (node = constructor_graph->root; node; node = node->next)
    {
      con_node result = NULL;

      switch (node->type)
	{
	case LOCAL_NODE:
	  result = add_local_node (cg, node->id);
	  result->call_id = call_id;
	  break;

	case OBJECT_NODE:
	  result = add_object_node (cg, node->id, node->class_id);
	  result->call_id = call_id;
	  break;

	case FIELD_NODE:
	  result = add_field_node (cg, node->id);
	  result->call_id = call_id;
	  break;

	case GLOBAL_NODE:
	  /* dont copy global's. They'll be there anyway */
	  break;

	case RETURN_NODE:
	  /* constructors dont have caller results */
	  /* TODO make it so these arent added */
	  gcc_assert (node->out == NULL);
	  gcc_assert (node->in == NULL);
	  break;

	case CALLEE_ACTUAL_NODE:
	  /* no need for this anymore */
	  break;


	case CALLER_ACTUAL_NODE:
	  result =
	    add_caller_actual_node (cg, node->id, node->index,
				    node->call_id);
	  break;

	}

      if (result && node->phantom)
	{
	  result->phantom = true;
	}
    }

  for (node = constructor_graph->root; node; node = node->next)
    {
      con_edge edge;
      for (edge = node->out; edge; edge = edge->next_out)
	{
	  con_node source =
	    get_matching_node_in_caller (cg, edge->source, call_id);
	  con_node target =
	    get_matching_node_in_caller (cg, edge->target, call_id);
	  gcc_assert (source);
	  gcc_assert (target);
	  if (edge->target->type == CALLEE_ACTUAL_NODE)
	    {
	      target->escape = EA_NO_ESCAPE;
	    }
	  add_edge (source, target);
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
      field_owner =
	get_matching_node_in_caller (cg, node->owner, call_id);
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
