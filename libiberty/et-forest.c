/* ET-trees datastructure implementation.
   Contributed by Pavel Nejedly
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Cygnus Solutions.

This file is part of the libiberty library.
Libiberty is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

Libiberty is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with libiberty; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "libiberty.h"
#include "et-forest.h"


struct et_forest_occurrence;
typedef struct et_forest_occurrence* et_forest_occurrence_t;

struct et_forest_node;
typedef struct et_forest_node* et_forest_node_t;

/* Single occurrence of vertex in ET-forest.  
   A single node may have multiple occurrences.
 */
struct et_forest_occurrence
{
  /* Parent in the splay-tree.  */
  et_forest_occurrence_t parent;

  /* Children in the splay-tree.  */
  et_forest_occurrence_t left, right;

  /* Counts of vertices in the two splay-subtrees.  */
  int count_left, count_right;

  /* Next occurrence of this node in the sequence.  */
  et_forest_occurrence_t next;

  /* The node, which this occurrence is of.  */
  et_forest_node_t node;
};


/* ET-forest node.  */
struct et_forest_node
{
  /* The value of the node.  */
  et_forest_value value;
  
  /* In order to use hashtable, we need to know hash_f, eq_f and del_f.  */
  et_forest_t forest;

  /* First and last occurrence of this node in the sequence.  */
  et_forest_occurrence_t first, last;
};


static hashval_t _hash_func PARAMS ((const void*));
static int _compare_func PARAMS ((const void*, const void*));
static void _delete_func PARAMS ((void*));

static et_forest_occurrence_t splay PARAMS ((et_forest_occurrence_t));
static void remove_all_occurrences PARAMS ((et_forest_node_t));
static et_forest_occurrence_t find_leftmost_node 
                               PARAMS ((et_forest_occurrence_t));
static et_forest_occurrence_t find_rightmost_node 
                               PARAMS ((et_forest_occurrence_t));
static int calculate_value PARAMS ((et_forest_occurrence_t));


static
hashval_t
_hash_func (_node)
     const void *_node;
{
  et_forest_node_t node = (et_forest_node_t) _node;

  return node->forest->hash_f (node->value);
}


static
int
_compare_func (_node1, _node2)
     const void *_node1;
     const void *_node2;
{
  et_forest_node_t node1 = (et_forest_node_t) _node1;
  et_forest_node_t node2 = (et_forest_node_t) _node2;

  return node1->forest->eq_f (node1->value, node2->value);
}


static
void
_delete_func (_node)
     void *_node;
{
  et_forest_node_t node = (et_forest_node_t) _node;

  if (node->forest->del_f)
    node->forest->del_f (node->value);

  remove_all_occurrences (node);

  free (node);
}

static 
et_forest_occurrence_t
splay (node)
     et_forest_occurrence_t node;
{
  et_forest_occurrence_t parent;
  et_forest_occurrence_t grandparent;

  while (1)
    {
      parent = node->parent;

      if (! parent)
	return node;  /* node == root.  */

      grandparent = parent->parent;

      if (! grandparent)
	break;

      /* Now there are four possible combinations:  */

      if (node == parent->left)
	{
	  if (parent == grandparent->left)
	    {
	      et_forest_occurrence_t node1, node2;
	      int count1, count2;

	      node1 = node->right;
	      count1 = node->count_right;
	      node2 = parent->right;
	      count2 = parent->count_right;

	      grandparent->left = node2;
	      grandparent->count_left = count2;
	      if (node2)
		node2->parent = grandparent;
	      parent->left = node1;
	      parent->count_left = count1;
	      if (node1)
		node1->parent = parent;
	      parent->right = grandparent;
	      parent->count_right = count2 + grandparent->count_right + 1;
	      node->right = parent;
	      node->count_right = count1 + parent->count_right + 1;

	      node->parent = grandparent->parent;
	      parent->parent = node;
	      grandparent->parent = parent;

	      if (node->parent)
		{
		  if (node->parent->left == grandparent)
		    node->parent->left = node;
		  else
		    node->parent->right = node;
		}
	    }
	  else
	    {
	      /* parent == grandparent->right && node == parent->left*/
	      et_forest_occurrence_t node1, node2;
	      int count1, count2;

	      node1 = node->left;
	      count1 = node->count_left;
	      node2 = node->right;
	      count2 = node->count_right;

	      grandparent->right = node1;
	      grandparent->count_right = count1;
	      if (node1)
		node1->parent = grandparent;
	      parent->left = node2;
	      parent->count_left = count2;
	      if (node2)
		node2->parent = parent;
	      node->left = grandparent;
	      node->count_left = grandparent->count_left + count1 + 1;
	      node->right = parent;
	      node->count_right = parent->count_right + count2 + 1;

	      node->parent = grandparent->parent;
	      parent->parent = node;
	      grandparent->parent = node;

	      if (node->parent)
		{
		  if (node->parent->left == grandparent)
		    node->parent->left = node;
		  else
		    node->parent->right = node;
		}
	    }
	}
      else
	{
	  /* node == parent->right.  */
	  if (parent == grandparent->left)
	    {
	      et_forest_occurrence_t node1, node2;
	      int count1, count2;

	      node1 = node->left;
	      count1 = node->count_left;
	      node2 = node->right;
	      count2 = node->count_right;

	      parent->right = node1;
	      parent->count_right = count1;
	      if (node1)
		node1->parent = parent;
	      grandparent->left = node2;
	      grandparent->count_left = count2;
	      if (node2)
		node2->parent = grandparent;
	      node->left = parent;
	      node->count_left = parent->count_left + count1 + 1;
	      node->right = grandparent;
	      node->count_right = grandparent->count_right + count2 + 1;

	      node->parent = grandparent->parent;
	      parent->parent = node;
	      grandparent->parent = node;

	      if (node->parent)
		{
		  if (node->parent->left == grandparent)
		    node->parent->left = node;
		  else
		    node->parent->right = node;
		}
	    }
	  else
	    {
	      /* parent == grandparent->right && node == parent->right*/
	      et_forest_occurrence_t node1, node2;
	      int count1, count2;

	      node1 = node->left;
	      count1 = node->count_left;
	      node2 = parent->left;
	      count2 = parent->count_left;

	      grandparent->right = node2;
	      grandparent->count_right = count2;
	      if (node2)
		node2->parent = grandparent;
	      parent->right = node1;
	      parent->count_right = count1;
	      if (node1)
		node1->parent = parent;
	      parent->left = grandparent;
	      parent->count_left = count2 + grandparent->count_left + 1;
	      node->left = parent;
	      node->count_left = count1 + parent->count_left + 1;

	      node->parent = grandparent->parent;
	      parent->parent = node;
	      grandparent->parent = parent;

	      if (node->parent)
		{
		  if (node->parent->left == grandparent)
		    node->parent->left = node;
		  else
		    node->parent->right = node;
		}
	    }
	}
	  
    }

  /* parent == root.  */
  /* There are two possible combinations:  */

  if (node == parent->left)
    {
      et_forest_occurrence_t node1;
      int count1;
      
      node1 = node->right;
      count1 = node->count_right;

      parent->left = node1;
      parent->count_left = count1;
      if (node1)
	node1->parent = parent;
      node->right = parent;
      node->count_right = parent->count_right + 1 + count1;
      node->parent = parent->parent;  /* the same as = 0;  */
      parent->parent = node;

      if (node->parent)
	{
	  if (node->parent->left == parent)
	    node->parent->left = node;
	  else
	    node->parent->right = node;
	}
    } 
  else 
    {
      /* node == parent->right.  */
      et_forest_occurrence_t node1;
      int count1;
      
      node1 = node->left;
      count1 = node->count_left;

      parent->right = node1;
      parent->count_right = count1;
      if (node1)
	node1->parent = parent;
      node->left = parent;
      node->count_left = parent->count_left + 1 + count1;
      node->parent = parent->parent;  /* the same as = 0;  */
      parent->parent = node;

      if (node->parent)
	{
	  if (node->parent->left == parent)
	    node->parent->left = node;
	  else
	    node->parent->right = node;
	}
    }

  return node;
}


static
void
remove_all_occurrences (forest_node)
     et_forest_node_t forest_node;
{
  et_forest_occurrence_t first = forest_node->first;
  et_forest_occurrence_t last = forest_node->last;
  et_forest_occurrence_t node;

  splay (first);

  if (first->left)
    first->left->parent = 0;
  if (first->right)
    first->right->parent = 0;   

  if (last != first)
    {
      splay (last);

      if (last->left)
	last->left->parent = 0;
      if (last->right)
	last->right->parent = 0;
    }

  if (last->right && first->left) /* actually, first->left would suffice.  */
    {
      /* Need to join them.  */
      et_forest_occurrence_t prev_node, next_node;

      prev_node = splay (find_rightmost_node (first->left));
      next_node = splay (find_leftmost_node (last->right));
      /* prev_node and next_node are consecutive occurencies
	 of the same node.  */
      if (prev_node->next != next_node)
	abort ();

      prev_node->right = next_node->right;
      prev_node->count_right = next_node->count_right;
      prev_node->next = next_node->next;
      if (prev_node->right)
	prev_node->right->parent = prev_node;

      if (prev_node->node->last == next_node)
	prev_node->node->last = prev_node;

      free (next_node);
    }

  if (first != last)
    {
      node = first->next;

      while (node != last)
	{
	  et_forest_occurrence_t next_node;

	  splay (node);

	  if (node->left)
	    node->left->parent = 0;
	  if (node->right)
	    node->right->parent = 0;

	  next_node = node->next;
	  free (node);
	  node = next_node;
	}
    }

  free (first);
  if (first != last)
    free (last);
}


static
et_forest_occurrence_t
find_leftmost_node (tree)
     et_forest_occurrence_t tree;
{
  while (tree->left)
    tree = tree->left;

  return tree;
}


static
et_forest_occurrence_t
find_rightmost_node (tree)
     et_forest_occurrence_t tree;
{
  while (tree->right)
    tree = tree->right;
  return tree;
}


int
calculate_value (node)
     et_forest_occurrence_t node;
{
  int value = node->count_left;

  while (node->parent)
    {
      if (node == node->parent->right)
	value += node->parent->count_left + 1;

      node = node->parent;
    }

  return value;
}




/* Create ET-forest structure.  */
et_forest_t
et_forest_create (initial_htab_size, hash_func, compare_func, delete_func)
     size_t initial_htab_size;
     et_forest_hash hash_func;
     et_forest_eq compare_func;
     et_forest_del delete_func;
{

  et_forest_t forest = xmalloc (sizeof (struct et_forest));
  
  forest->table = htab_create (initial_htab_size, _hash_func, _compare_func,
			       _delete_func);

  forest->hash_f = hash_func;
  forest->eq_f = compare_func;
  forest->del_f = delete_func;

  return forest;
}



/* Deallocate the structure.  */
void 
et_forest_delete (forest)
     et_forest_t forest;
{

  /* This removes nodes and occurrences as well.  */
  htab_delete (forest->table);
  
  free (forest);
}


void
et_forest_add_vertex (forest, vertex)
     et_forest_t forest;
     et_forest_value vertex;
{
  /* Create node with one occurrence.  */
  et_forest_node_t node;
  et_forest_occurrence_t occ;
  void **slot;

  node = xmalloc (sizeof (struct et_forest_node));
  occ = xmalloc (sizeof (struct et_forest_occurrence));

  node->value = vertex;
  node->forest = forest;
  node->first = node->last = occ;

  slot = htab_find_slot (forest->table, node, 1);
  if (!slot)
    abort ();

  if (*slot)
    abort ();    /* Already inserted.  */

  *slot = node;

  occ->node = node;
  occ->left = occ->right = occ->parent = 0;
  occ->next = 0;
  occ->count_left = occ->count_right = 0;
}

void
et_forest_add_edge (forest, parent, child)
     et_forest_t forest;
     et_forest_value parent;
     et_forest_value child;
{
  struct et_forest_node fake_node;
  et_forest_node_t parent_node;
  et_forest_node_t child_node;
  et_forest_occurrence_t new_occ, parent_occ, child_occ;

  fake_node.forest = forest;
  fake_node.value = parent;
  parent_node = htab_find (forest->table, &fake_node);
  fake_node.value = child;
  child_node = htab_find (forest->table, &fake_node);

  if (! parent_node || ! child_node)
    abort ();

  parent_occ = parent_node->first;
  child_occ = child_node->first;

  splay (parent_occ);
  splay (child_occ);

  if (parent_occ->parent)
    abort ();  /* Both child and parent are in the same tree.  */

  if (child_occ->left)
    abort ();  /* child must be root of its containing tree.  */
  
  new_occ = xmalloc (sizeof (struct et_forest_occurrence));

  new_occ->node = parent_node;
  new_occ->left = child_occ;
  new_occ->count_left = child_occ->count_right + 1; /* count_left is 0.  */
  new_occ->right = parent_occ->right;
  new_occ->count_right = parent_occ->count_right;
  new_occ->parent = parent_occ;
  new_occ->next = parent_occ->next;
  child_occ->parent = new_occ;
  parent_occ->right = new_occ;
  parent_occ->count_right = new_occ->count_left + new_occ->count_right + 1;
  parent_occ->next = new_occ;
  if (new_occ->right)
    new_occ->right->parent = new_occ;

  if (parent_node->last == parent_occ)
    parent_node->last = new_occ;
}


void
et_forest_remove_vertex (forest, vertex)
     et_forest_t forest;
     et_forest_value vertex;
{
  struct et_forest_node fake_node;

  fake_node.forest = forest;
  fake_node.value = vertex;
  
  htab_remove_elt (forest->table, &fake_node);
}


void
et_forest_remove_edge (forest, parent, child)
     et_forest_t forest;
     et_forest_value parent;
     et_forest_value child;
{
  struct et_forest_node fake_node;
  et_forest_node_t parent_node;
  et_forest_node_t child_node;
  et_forest_occurrence_t parent_pre_occ, parent_post_occ;

  fake_node.forest = forest;
  fake_node.value = parent;
  parent_node = htab_find (forest->table, &fake_node);
  fake_node.value = child;
  child_node = htab_find (forest->table, &fake_node);

  if (! child_node || ! parent_node)
    abort ();

  splay (child_node->first);

  if (! child_node->first->left)
    abort (); /* must have parent.  */

  parent_pre_occ = find_rightmost_node (child_node->first->left);
  if (parent_pre_occ->node != parent_node)
    abort ();

  splay (parent_pre_occ);
  parent_pre_occ->right->parent = 0;
  
  parent_post_occ = parent_pre_occ->next;
  splay (parent_post_occ);

  parent_post_occ->left->parent = 0;

  parent_pre_occ->right = parent_post_occ->right;
  parent_pre_occ->count_right = parent_post_occ->count_right;
  if (parent_post_occ->right)
    parent_post_occ->right->parent = parent_pre_occ;

  parent_pre_occ->next = parent_post_occ->next;

  if (parent_post_occ == parent_node->last)
    parent_node->last = parent_pre_occ;

  free (parent_post_occ);
}


et_forest_value
et_forest_parent (forest, vertex)
     et_forest_t forest;
     et_forest_value vertex;
{
  et_forest_node_t node;
  struct et_forest_node fake_node;

  fake_node.forest = forest;
  fake_node.value = vertex;
  node = htab_find (forest->table, &fake_node);

  if (! node)
    abort ();  /* Node not found.  */

  splay (node->first);

  if (node->first->left)
    return find_rightmost_node (node->first->left)->node->value;
  else
    return 0;
}


et_forest_value
et_forest_common_ancestor (forest, vertex1, vertex2)
     et_forest_t forest;
     et_forest_value vertex1;
     et_forest_value vertex2;
{
  et_forest_node_t node1;
  et_forest_node_t node2;
  struct et_forest_node fake_node;
  int value1, value2, max_value;
  et_forest_node_t ancestor;

  fake_node.forest = forest;
  fake_node.value = vertex1;
  node1 = htab_find (forest->table, &fake_node);
  fake_node.value = vertex2;
  node2 = htab_find (forest->table, &fake_node);

  if (vertex1 == vertex2)
    return vertex1;
  
  if (! node1 || ! node2)
    abort ();

  splay (node1->first);
  splay (node2->first);

  if (! node1->first->parent)  /* The two vertices are in different trees.  */
    return 0;

  value2 = calculate_value (node2->first);
  value1 = calculate_value (node1->first);

  if (value1 < value2)
    {
      ancestor = node1;
      max_value = value2;
    }
  else
    {
      ancestor = node2;
      max_value = value1;
    }
  
  while (calculate_value (ancestor->last) < max_value)
    {
      /* Find parent node.  */
      splay (ancestor->first);
      ancestor = find_rightmost_node (ancestor->first->left) ->node;
    }

  return ancestor->value;
}


int
et_forest_contains_vertex (forest, vertex)
     et_forest_t forest;
     et_forest_value vertex;
{
  struct et_forest_node fake_node;

  fake_node.forest = forest;
  fake_node.value = vertex;
  
  return htab_find (forest->table, &fake_node) != 0;
}
