/* Callgraph based intraprocedural optimizations.
   Copyright (C) 2004 Free Software Foundation, Inc.
   Contributed by Caroline Tice

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
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tree-inline.h"
#include "tree-flow.h"
#include "tree-flow-inline.h"
#include "langhooks.h"
#include "hashtab.h"
#include "toplev.h"
#include "flags.h"
#include "ggc.h"
#include "debug.h"
#include "target.h"
#include "cgraph.h"
#include "diagnostic.h"
#include "timevar.h"
#include "params.h"
#include "fibheap.h"
#include "c-common.h"
#include "intl.h"
#include "function.h"
#include "basic-block.h"
#include "tree-iterator.h"
#include "tree-pass.h"
#include "struct-reorg.h"

static void peel_structs (void);

static bool
gate_peel_structs (void)
{
  return flag_peel_structs != 0;
}

static void
do_peel_structs (void)
{
  peel_structs ();
}

struct tree_opt_pass pass_struct_reorg =
{
  "struct_reorg",                       /* name */
  gate_peel_structs,			/* gate */
  do_peel_structs,                      /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_PEEL_STRUCTS,                      /* tv_id */
  PROP_gimple_leh | PROP_cfg,           /* properties_required */
  PROP_gimple_leh | PROP_cfg,           /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_verify_stmts                     /* todo_flags_finish */
};

static htab_t visited_nodes;

struct struct_list {
  struct data_structure *struct_data;
  struct struct_list *next;
};

static struct data_field_entry *
get_fields (tree struct_decl, int num_fields)
{
  struct data_field_entry *list;
  tree t = TYPE_FIELDS (struct_decl);
  int idx = 0;

  list = (struct data_field_entry * ) xmalloc (num_fields
					       * sizeof
					              (struct data_field_entry));

  for (idx = 0 ; t; t = TREE_CHAIN (t), idx++)
    if (TREE_CODE (t) == FIELD_DECL)
      {
	list[idx].index = idx;
	list[idx].decl = t;
	list[idx].acc_sites = NULL;
      }

  return list;
}

/* Calculates the list of accessed fields (of the given DS) in the given
   statement STMT.  */
static sbitmap 
get_stmt_accessed_fields (tree stmt, struct data_structure *ds);

/* Calculates the access distance that the given statement (STMT)
   imposes.  This is a function of the memory accessed in STMT
   (memory other than accesses to fields of DS.  */
static int
get_stmt_access_distance (tree stmt, struct data_structure *ds);

/* return the last access to a field in a given basic block, if there
   is no field accesses the a dummy element (with -1 as field index)
   is returned.  */
struct bb_field_access *
get_last_field_access (struct data_structure *ds, basic_block bb)
{
  struct bb_field_access *crr = ds->bbs_f_acc_lists[bb->index];

  for (; crr->next; crr = crr->next);

  return crr;
}

/* Build the access list of the fields of the data structure DS that
   happen in bb.  */
static struct bb_field_access *
build_f_acc_list_for_bb (struct data_structure *ds, basic_block bb)
{
  struct bb_field_access *head, *crr;
  block_stmt_iterator bsi;

  /* Create a new dummy item to hold the disntance to the first field
     in the basic block (or hold the distance accross the basic block
     if there is no fields accessed in it.  */
  crr = xcalloc (1, sizeof (struct bb_field_access));
  crr->f_indx = -1;
  head = crr;

  /* Go over the basic block statements and create a linked list of
     the accessed fields.  */
  for (bsi = bsi_start (bb); bsi_stmt (bsi); bsi_next (&bsi))
    {
      tree stmt = bsi_stmt (bsi);
      sbitmap fields;

      fields = get_stmt_accessed_fields (stmt, ds);
      if ( sbitmap_first_set_bit (fields) < 0)
        crr->distance_to_next += get_stmt_access_distance (stmt, ds);
      else
        {
          int i;

          /* A statement may access more than one field; in such case the
             distance is zero between fields in the same statement.  */
          for (i = 0; i < ds->num_fields; i++)
            {
              if ( ! TEST_BIT (fields, i))
                continue;
              /* The disntance to the next is zeroed here, if the next
	         access is in the same statement it will not be advanced
		 at all, and we will get a distance of zero.  */
              crr->next = xcalloc (1, sizeof (struct bb_field_access));
              crr->next->f_indx = i;
              crr = crr->next;
              /* Update the access count for a single field.  */
              ds->fields[i].count += bb->count;
            }
        }
    }
  return head;
}

/* Build the accesses list of the fields of the given data structure
   in each one of the basic blocks.  */
static void
build_access_list_for_struct (struct data_structure *ds)
{
  basic_block bb;

  ds->bbs_f_acc_lists = 
    (struct bb_field_access **) xcalloc (last_basic_block,
                                sizeof (struct bb_field_access *));
  /* Build the access list in each one of the basic blocks.  */
  FOR_EACH_BB (bb)
    {
      ds->bbs_f_acc_lists[bb->index] = build_f_acc_list_for_bb (ds, bb);
      /* If there are accesses to fields of DS in BB update the access
         count of the data structure DS.  */
      if (ds->bbs_f_acc_lists[bb->index]->next)
        ds->count += bb->count;
    }
}

static void
do_peel (ATTRIBUTE_UNUSED struct data_structure *struct_data)
{
}


static tree
tree_contains_struct_access (ATTRIBUTE_UNUSED tree t,ATTRIBUTE_UNUSED  void *data)
{
  return NULL_TREE;
}

static tree
find_struct_data (ATTRIBUTE_UNUSED tree *tp, ATTRIBUTE_UNUSED int *walk_subtrees, 
		  ATTRIBUTE_UNUSED void *data)
{
  return NULL_TREE;
}

static void
verify_library_parameters (ATTRIBUTE_UNUSED tree fdecl, 
			   ATTRIBUTE_UNUSED struct struct_list *data_struct_list)
{
}

/* Perform data structure peeling.  */
static void
peel_structs (void)
{

  struct struct_list *data_struct_list = NULL;
  struct struct_list *current_struct;
  struct cgraph_varpool_node *current_varpool;
  struct cgraph_node *c_node;
  bool reordering_only = false;
  bool success;

  /* Stage 1:  Find all potential structs for peeling. Perform analysis
     to decide if transformations are safe/legal.  */

  /* Make sure compiler invocation is of type to use this opt. Not Done yet.  */


  /* For every global variable in the program:
   *     - Check to see if it's of a candidate type; if so stuff type into
   *       data structure.
   */

  for (current_varpool = cgraph_varpool_nodes_queue; current_varpool;
       current_varpool = current_varpool->next_needed)
    {
      tree var_decl = current_varpool->decl;
      if (!var_decl || TREE_CODE (var_decl) != VAR_DECL)
	continue;

      if ((TREE_CODE (TREE_TYPE (var_decl)) == POINTER_TYPE
	   || TREE_CODE (TREE_TYPE (var_decl)) == ARRAY_TYPE)
	  && TREE_CODE (TREE_TYPE (TREE_TYPE (var_decl))) == RECORD_TYPE)
	{
	  tree struct_decl;
	  struct struct_list *temp_node;
	  struct data_structure *d_node;
	  int num_fields;

	  struct_decl = TREE_TYPE (TREE_TYPE (var_decl));

	  /* Exclude "FILE" records from consideration.  */
	  if (TREE_CODE_CLASS (TREE_CODE (struct_decl)) == 't'
	      && TYPE_NAME (struct_decl)
	      && TREE_CODE (TYPE_NAME (struct_decl)) == TYPE_DECL
	      && DECL_NAME (TYPE_NAME (struct_decl))
	      && strcmp (IDENTIFIER_POINTER (DECL_NAME
					     (TYPE_NAME (struct_decl))),
			 "FILE") == 0)
	    continue;

	  d_node = (struct data_structure *) xmalloc (sizeof
						      (struct data_structure));
	  num_fields = fields_length (struct_decl);
	  d_node->decl = struct_decl;
	  d_node->num_fields = num_fields;
	  d_node->fields = get_fields (struct_decl, num_fields);
	  d_node->alloc_sites = NULL;
	  d_node->struct_clustering = NULL;

	  temp_node = (struct struct_list *) xmalloc (sizeof(struct struct_list));
	  temp_node->struct_data = d_node;
	  temp_node->next = data_struct_list;
	  data_struct_list = temp_node;
	  build_access_list_for_struct (d_node);
	}
    }

  /* Walk through entire program tree looking for:
   *     - Any declaration that uses struct type; record variable
   *     - Any use of any variable that uses struct type; record use
   *    (- Verify that all uses are "legal".)
   */

  for (c_node = cgraph_nodes; c_node; c_node = c_node->next)
    {
    }

  /* Stage 2:  Determine what, if anything, we want to transform and how.  */

  for (current_struct = data_struct_list; current_struct;
       current_struct = current_struct->next)
    {
      success = cache_aware_data_reorganization (current_struct->struct_data, reordering_only);

      /* Stage 3:  Do the actual transformation decided on in stage 2.  */

      if (success)
        do_peel (current_struct->struct_data);
    }
}

