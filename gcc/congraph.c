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
  return node->type == CALLER_ACTUAL || node->type == CALLEE_ACTUAL;
}

/* allocates a new, cleared and inited con_graph */
con_graph
new_con_graph (const char *filename, int num_basic_blocks,
	       tree function, con_graph next)
{
  con_graph result = xcalloc (1, sizeof (struct _con_graph));
  gcc_assert (result);

  result->filename = filename;

  /* used for flow-sensitive */
  result->block_iteration_count =
    xcalloc (num_basic_blocks, sizeof (int));
  gcc_assert (result->block_iteration_count);

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
      if (node->escape == NO_ESCAPE)
	{
	  fprintf (out, " [ %p ]\n", (void *) node);
	}
    }
  fprintf (out, ") { background: yellow; }\n\n");

  /* do the nonlocalgraph */
  fprintf (out, "( NonLocalGraph: \n");
  for (node = cg->root; node != NULL; node = node->next)
    {
      if (node->escape != NO_ESCAPE)
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
  gcc_assert (node->type == FIELD ? node->owner != NULL : true);
  gcc_assert (node->type == OBJECT ? node->class_id != NULL : true);
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
    case CALLEE_ACTUAL:
      fprintf (out, "a%d_ee (", node->index);
      print_generic_expr (out, node->id, 0);
      fprintf (out, ")");
      break;

    case CALLER_ACTUAL:
      fprintf (out, "a%d_er (", node->index);
      print_generic_expr (out, node->id, 0);
      fprintf (out, ")");
      break;

    case FIELD:
    case OBJECT:
    case LOCAL:
    case GLOBAL:
      print_generic_expr (out, node->id, 0);
      break;

    case RETURN:
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
    case NO_ESCAPE:
      fprintf (out, "border: limegreen; ");
      break;
    case ARG_ESCAPE:
      fprintf (out, "border: blue; ");
      break;
    case GLOBAL_ESCAPE:
      fprintf (out, "border: red; ");
      break;
    default:
      gcc_unreachable ();
      break;
    }

  /* design for the color and shape node depends on its type */
  switch (node->type)
    {
    case FIELD:
      fprintf (out, "fill: #cfffcf; ");
      fprintf (out, "shape: ellipse; ");
      break;

    case OBJECT:
      fprintf (out, "shape: rect; ");
      break;

    case LOCAL:
      fprintf (out, "shape: ellipse; ");
      break;

    case RETURN:
    case CALLEE_ACTUAL:
    case CALLER_ACTUAL:
      fprintf (out, "fill: #ffcfcf; ");
      fprintf (out, "shape: ellipse; ");
      break;

    case GLOBAL:
      fprintf (out, "shape: ellipse; ");
      fprintf (out, "fill: silver; ");
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
    case FIELD:
      fprintf (out, " { label: F; }");
      break;
    case POINTS_TO:
      fprintf (out, " { label: P; }");
      break;
    case DEFERRED:
      fprintf (out, " { label: D; style: dashed; }");
      break;

    default:
      gcc_unreachable ();
    }

  /* print the target */
  fprintf (out, "[ %p ] ", (void *) target);

  fprintf (out, "\n");
}

/* TODO should each bb have a dirty bit */
void
reset_dirty (con_graph cg)
{
  gcc_assert (cg);
  cg->dirty = false;
}

bool
is_dirty (con_graph cg)
{
  gcc_assert (cg);
  return cg->dirty;
}

void
increment_iteration_count (con_graph cg, int block_index)
{
  gcc_assert (cg);
  gcc_assert (cg->block_iteration_count);
  cg->block_iteration_count[block_index]++;
}

int
get_iteration_count (con_graph cg, int block_index)
{
  gcc_assert (cg);
  gcc_assert (cg->block_iteration_count);
  return cg->block_iteration_count[block_index];
}

void
reset_iteration_count (con_graph cg, int block_index)
{
  gcc_assert (cg);
  gcc_assert (cg->block_iteration_count);
  cg->block_iteration_count[block_index] = 0;
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
  node->type = OBJECT;
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
  node->type = CALLEE_ACTUAL;
  node->escape = ARG_ESCAPE;
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
  node->type = CALLER_ACTUAL;
  node->escape = ARG_ESCAPE;
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
  node->type = FIELD;
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
  node->type = RETURN;
  node->escape = ARG_ESCAPE;
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
  node->type = GLOBAL;
  node->escape = GLOBAL_ESCAPE;
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
  node->type = LOCAL;
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
  if (node->type != FIELD && !is_actual_type (node))
    gcc_assert (get_existing_node_with_call_id
		(cg, node->id, node->call_id) == NULL);

  /* insert the node at the front of the list */
  node->next = cg->root;
  cg->root = node;

  /* remove this later, this is just for debugging */
  node->graph = cg;

  cg->dirty = true;
}


/* allocates a new, cleared and inited con_node */
con_node
new_con_node (void)
{
  con_node result = xcalloc (1, sizeof (struct _con_node));
  gcc_assert (result);
  result->escape = NO_ESCAPE;
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
	  gcc_assert (node->type != FIELD);

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
	  gcc_assert (node->type == FIELD);
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
  return (node->type != OBJECT);
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

  /* mark graph as dirty */
  source->graph->dirty = true;

  edge = new_con_edge ();
  edge->source = source;
  edge->target = target;

  /* set the type */
  if (is_reference_node (source) && target->type == OBJECT)
    edge->type = POINTS_TO;

  else if (source->type == OBJECT && target->type == FIELD)
    {
      con_edge temp;
      edge->type = FIELD;

      /* there can only be 1 owner of a field node */
      gcc_assert (target->owner == NULL);
      gcc_assert (source->owner == NULL);
      target->owner = source;

      /* TODO this may be allowable for a phantom field node */
      /* check that no other edges originate from on object */
      for (temp = target->in; temp != NULL; temp = temp->next_in)
	{
	  gcc_assert (temp->type != POINTS_TO);
	}

    }
  else if (is_reference_node (source) && is_reference_node (target))
    edge->type = DEFERRED;

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

  /* guaranteed to be dirty */
  edge->source->graph->dirty = true;

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
  gcc_assert (node->type != OBJECT);

  for (edge = node->out; edge != NULL; edge = edge->next_out)
    {
      con_node new_result = NULL;

      if (edge->target->type == OBJECT)
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
  gcc_assert (node->type == OBJECT);


  /* get the field nodes from the target of the field edges, and build
   * a list of them */
  for (edge = node->out; edge != NULL; edge = edge->next_out)
    {
      gcc_assert (edge->type == FIELD);
      gcc_assert (edge->target->type == FIELD);
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
      gcc_assert (node->type == OBJECT);

      /* get the field nodes from the target of the field edges, and
       * build a list of them */
      for (edge = node->out; edge != NULL; edge = edge->next_out)
	{
	  gcc_assert (edge->type == FIELD);
	  gcc_assert (edge->target->type == FIELD);
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
  gcc_assert (node->type == OBJECT);

  /* get the field nodes from the target of the field edges, and build
   * a list of them */
  for (edge = node->out; edge != NULL; edge = edge->next_out)
    {
      gcc_assert (edge->type == FIELD);
      gcc_assert (edge->target->type == FIELD);

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
  gcc_assert (node->type != OBJECT);

  /* you dont want to bypass nodes at the start or the end */
  if (node->in == NULL || node->out == NULL)
    {
      return;
    }

  for (in = node->in; in != NULL; in = in->next_in)
    {
      if (in->type == DEFERRED)
	{
	  for (out = node->out; out != NULL; out = out->next_out)
	    {
	      gcc_assert (in->type != FIELD);

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
  gcc_assert (node->type != OBJECT);
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
      if (node->type == CALLER_ACTUAL && node->index == index
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
      if (node->type == CALLEE_ACTUAL && node->index == index
	  && node->call_id == call_id)
	{
	  return node;
	}
    }

  return NULL;

}

/* This function, and the symbols in it come Toplas03, Figure 7
 * (Choi99 has the same code, but different symbol names). I call the
 * objects 'o' instead of 'n_o', as this is more intuitive (o is for
 * object) */
void
update_nodes (con_node f, con_node mapped_fields)
{
  con_node temp;
  con_node o = get_points_to (f);

  /* _bar nodes belong to the caller, and the other nodes belong to
   * the callee */
  fprintf (stderr, "update_nodes ");
  fprintf (stderr, "f: '");
  print_generic_stmt (stderr, f->id, 0);
  fprintf (stderr, "' mapped: '");
  print_generic_stmt (stderr, mapped_fields->id, 0);

  /* this is my theory */
  gcc_assert (mapped_fields->next_link == NULL);

  gcc_assert (f->type == FIELD || is_actual_type (f));
  gcc_assert (f->graph->role == CALLEE);
  gcc_assert (mapped_fields->graph->role == CALLER);

  /* The mapsTo is an n-k relationship, which I'll model one-way,
   * using a hashtable of arrays, and not at all the other way. To
   * make calculating the size easier, I'll just use the number of
   * objects in the function.  */


  /* There are 4 distinct set of linked lists going on here. f, f_bar,
   * o, and o_bar. f and o are from the same method, as are f_bar and
   * o_bar, so there should be no overlap of lists. f and f_bar are
   * field nodes, and o and o_bar are object nodes, so there should be
   * no overlap in those lists either.  */


  while (o)
    {
      con_node f_bar;
      gcc_assert (o->type == OBJECT);
      gcc_assert (o->graph->function == f->graph->function);
      gcc_assert (o->escape != NO_ESCAPE);

      f_bar = mapped_fields;
      while (f_bar)
	{
	  con_node o_bar;
	  gcc_assert (is_actual_type (f_bar) || f_bar->type == FIELD);

	  o_bar = get_points_to (f_bar);
	  if (o_bar == NULL)
	    {
	      gcc_unreachable ();
	      /* TODO
	         create_target_node(f_bar); */
	      /* create/insert a new node as the target of f, make it
	       * NoEscape */
	    }
	  while (o_bar)
	    {
	      gcc_assert (o_bar->graph->function !=
			  f->graph->function);
	      gcc_assert (o_bar->graph->function ==
			  f_bar->graph->function);
	      gcc_assert (o_bar->type == OBJECT);

	      if (!in_maps_to_obj (o, o_bar))
		{
		  con_node *vec;

		  add_to_maps_to_obj (o, o_bar);
		  update_escape_state (o, o_bar);

		  vec = get_field_nodes_vec (o);
		  while (*vec)
		    {
		      con_node g = *vec;
		      con_node tmp_mapped_fields =
			get_single_named_field_node (o_bar, g->id);
		      gcc_assert (tmp_mapped_fields);
		      /* not to say it will be, but more of a TODO */
		      update_nodes (g, tmp_mapped_fields);
		      vec++;
		    }
		}

	      /* iterate through o_bar */
	      temp = o_bar->next_link;
	      o_bar->next_link = NULL;
	      o_bar = temp;
	    }

	  /* iterate through maps_to_f */
	  temp = f_bar->next_link;
	  f_bar->next_link = NULL;
	  f_bar = temp;
	}

      /* iterate through get_points_to(fee) */
      temp = o->next_link;
      o->next_link = NULL;
      o = temp;
    }
}

/* whether source maps_to target */
bool
in_maps_to_obj (con_node source, con_node target)
{
  int i;
  gcc_assert (source);
  gcc_assert (target);

  gcc_assert (source->type == OBJECT);
  gcc_assert (target->type == OBJECT);
  gcc_assert (source->graph->function != target->graph->function);

  if (source->maps_to_obj == NULL)
    return false;

  for (i = 0; i < target->graph->num_objects; i++)
    if (source->maps_to_obj[i] == target)
      return true;

  return false;
}

void
add_to_maps_to_obj (con_node source, con_node target)
{
  int i;
  /* quick sanity check - this takes care of all preconditions aswell
   * */
  gcc_assert (!in_maps_to_obj (source, target));

  if (source->maps_to_obj == NULL)
    {
      source->maps_to_obj =
	XCNEWVEC (con_node, target->graph->num_objects);
      source->maps_to_obj[0] = target;
      return;
    }

  for (i = 0; i < target->graph->num_objects; i++)
    {
      if (source->maps_to_obj[i] == NULL)
	{
	  source->maps_to_obj[i] = target;
	  break;
	}
    }

}

void
update_escape_state (con_node source, con_node target)
{
  gcc_assert (source);
  gcc_assert (target);

  if (source->escape == GLOBAL_ESCAPE)
    target->escape = GLOBAL_ESCAPE;
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
	case LOCAL:
	  result = add_local_node (cg, node->id);
	  result->call_id = call_id;
	  break;

	case OBJECT:
	  result = add_object_node (cg, node->id, node->class_id);
	  result->call_id = call_id;
	  break;

	case FIELD:
	  result = add_field_node (cg, node->id);
	  result->call_id = call_id;
	  break;

	case GLOBAL:
	  /* dont copy global's. They'll be there anyway */
	  break;

	case RETURN:
	  /* constructors dont have caller results */
	  /* TODO make it so these arent added */
	  gcc_assert (node->out == NULL);
	  gcc_assert (node->in == NULL);
	  break;

	case CALLEE_ACTUAL:
	  /* no need for this anymore */
	  break;


	case CALLER_ACTUAL:
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
	  if (edge->target->type == CALLEE_ACTUAL)
	    {
	      target->escape = NO_ESCAPE;
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

    case FIELD:
      field_owner =
	get_matching_node_in_caller (cg, node->owner, call_id);
      gcc_assert (field_owner);
      return get_existing_field_node_with_call_id (cg, node->id,
						   field_owner,
						   call_id);

    case LOCAL:
    case OBJECT:
      return get_existing_node_with_call_id (cg, node->id, call_id);

    case GLOBAL:
      /* globals dont use a call_id */
      return get_existing_node (cg, node->id);

    case CALLEE_ACTUAL:
      /* Note: not a typo, case CALLEE: return get_caller.. */
      /*a callee matches with a caller, but the callee doesnt have the
       * same id, which is why this function has a call_id parameter*/
      return get_caller_actual_node (cg, node->index, call_id);

    case CALLER_ACTUAL:
      /* a caller node isnt affected by the inlining, so get the
       * original id */
      return get_caller_actual_node (cg, node->index, node->call_id);

    default:
      gcc_unreachable ();

    }
}
