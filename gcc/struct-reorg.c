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

/* Not used yet.  Commented it out so we can bootstrap.  */
#if 0 
static htab_t visited_nodes;
#endif

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

/* Record STMT as the access site of the filed with INDEX of DS data
   structure.  OP is the COMPONENT_REF is access to the field in STMT.  */
static void
add_field_access_site (struct data_structure *ds, int index,
		       tree stmt, tree op)
{
   struct access_site *as;

   if (index < 0 )
     abort ();

   as = (struct access_site *)xcalloc (1, sizeof (struct access_site));
   as->stmt = stmt;
   as->field_access = op;
   as->next = ds->fields[index].acc_sites;
   ds->fields[index].acc_sites = as;
}

/* Given the field declaration tree (F_DECL) return the 
   appropraite field index in DS.  */
static int 
get_field_index (struct data_structure *ds, tree f_decl)
{
  int i;
  
  for (i = 0; i < ds->num_fields; i++)
    if (ds->fields[i].decl == f_decl)
      return i;

  return -1;  
}

/* A helper function for get_stmt_accessed_fields.  */
static int
get_stmt_accessed_fields_1 (tree stmt, tree op, struct data_structure *ds)
{
  int i, code, total_dist = 0;

  code = TREE_CODE (op);
  switch (code)
    {
      case COMPONENT_REF:
        {
          /* This is a reference to a field check if this is to a relevant
	     field.  */
          tree struct_var = TREE_OPERAND (op, 0);
          tree field_decl = TREE_OPERAND (op, 1);
          tree field_decl_type = TREE_TYPE (field_decl);
	  tree struct_type = NULL_TREE;
          int index;

	  if (TREE_CODE (struct_var) == VAR_DECL)
	    struct_type = TREE_TYPE (struct_var);
	  else if (TREE_CODE (struct_var) == INDIRECT_REF)
	    struct_type = TREE_TYPE (TREE_TYPE (TREE_OPERAND (struct_var, 0)));

          if (ds->decl != struct_type )
            {
              if (TREE_CODE (TYPE_SIZE (field_decl_type)) == INTEGER_CST)
                return - int_cst_value (TYPE_SIZE (field_decl_type));
              else
                return -1;
            }
          if ((index = get_field_index (ds, field_decl)) < 0)
            abort ();

          /* Add a field access site here.  */
          add_field_access_site (ds, index, stmt, op);

          /* We return the field index + 1 becuase field IDs starts
	     at 0 and the 0 return value has a special meaning.  */
          return index + 1;
          break;
        }
      case BIT_FIELD_REF:
        {
          tree struct_type = TREE_TYPE (TREE_OPERAND (op, 0));
          tree n_bits = TREE_OPERAND (op, 1);

          /* Mark the structure as not appropriate for optimization.  */
          if (ds->decl == struct_type)
            ds->unresolved_field_access = true;
          if (TREE_CODE (n_bits) == INTEGER_CST)
            return - (int_cst_value (n_bits)/BITS_PER_UNIT);
          else
            return -1;
        }
      case ADDR_EXPR:
        {
          /* Mark the structure as not appropriate for optimization.  */
          return 0;
        }
      default:
  	for (i = 0; i < TREE_CODE_LENGTH (code); i++)
    	  {
            int dist;

	    tree opi = TREE_OPERAND (op, i);
           
            if (! opi)
	      continue;
      	    dist = get_stmt_accessed_fields_1 (stmt, opi, ds);
            if (dist > 0)
              return dist;
            else
              total_dist += dist;
    	  }
    }

  return total_dist;
}

/* Analyze the statement STMT and search for field accesses of the given
   data structure DS.  Returns the "ID + 1" of the accessed field if there
   is any, otherwise it returns the accesses distance produced by this
   instruction as a negative number or zero (if this is not a memory
   access).  We assume this is a GIMPLE statement meaning that a memory
   access must be in a MODIFY_EXPR, and the memory can be in one side
   of the MODIFY_EXPR.  That means that only one field could be
   accessed within this GIMPLE statement.  */
static int
get_stmt_accessed_fields (tree stmt, struct data_structure *ds)
{
  int code, dist;

  code = TREE_CODE (stmt);
  if (code != MODIFY_EXPR)
    return 0;

  /* Analyze the left side of the MODIFY.  If dist is not zero it means
     that the left side (OP0) is a memory access that means that the
     right side cannot be a memory access, so just return DIST.  */
  dist = get_stmt_accessed_fields_1 (stmt, TREE_OPERAND (stmt, 0), ds);
  if (dist)
    return dist;
  return get_stmt_accessed_fields_1 (stmt, TREE_OPERAND (stmt, 1), ds);
}

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
  for (bsi = bsi_start (bb); ! bsi_end_p (bsi); bsi_next (&bsi))
    {
      tree stmt = bsi_stmt (bsi);
      int index;

      index = get_stmt_accessed_fields (stmt, ds);
      if ( index <= 0)
        crr->distance_to_next += (- index);
      else
        {
          /* The disntance to the next is zeroed here, if the next
	     access is in the same statement it will not be advanced
	     at all, and we will get a distance of zero.  */
          crr->next = xcalloc (1, sizeof (struct bb_field_access));
          crr->next->f_indx = index - 1;
          crr = crr->next;
          /* Update the access count for a single field.  */
          ds->fields[crr->f_indx].count += bb->count;
        }
    }
  return head;
}

/* Build the accesses list of the fields of the given data structure
   in each one of the basic blocks.  */
static void
build_bb_access_list_for_struct (struct data_structure *ds, struct function *f)
{
  basic_block bb;

  if (! f)
    return;

  ds->bbs_f_acc_lists =
    (struct bb_field_access **) xcalloc (n_basic_blocks_for_function (f),
                                sizeof (struct bb_field_access *));
  /* Build the access list in each one of the basic blocks.  */
  FOR_EACH_BB_FN (bb, f)
    {
      ds->bbs_f_acc_lists[bb->index] = build_f_acc_list_for_bb (ds, bb);
      /* If there are accesses to fields of DS in BB update the access
         count of the data structure DS.  */
      if (ds->bbs_f_acc_lists[bb->index]->next)
        ds->count += bb->count;
    }
}

/* Free the BB field access list for the gievn function.  */
static void
free_bb_access_list_for_struct (struct data_structure *ds, struct function *f)
{
  int i;

  if (! f)
    return;
  
  for (i = 0; i < n_basic_blocks_for_function (f); i++)
    {
       
      struct bb_field_access *crr =  ds->bbs_f_acc_lists[i]; 
      struct bb_field_access *next;

      while (crr != NULL)
        {
	  next = crr->next; 
	  free (crr);
	  crr = next;
        }
     } 
  free (ds->bbs_f_acc_lists);
  ds->bbs_f_acc_lists = NULL;
}

static void
do_peel (ATTRIBUTE_UNUSED struct data_structure *struct_data)
{
}

/* The following aren't imeplemnted yet, so we comment them out
   for now to be able to bootstrap.  */
#if 0
/* ??? This is should be the same as get_stmt_field_accesses.  */
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
#endif 

/* Perform data structure peeling.  */
void
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
     - Check to see if it's of a candidate type; if so stuff type into
     data structure.
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

	  d_node = (struct data_structure *) xcalloc (1, 
						      sizeof (struct data_structure));
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
	}
    }

  /* Walk through entire program tree looking for:
       - Any declaration that uses struct type; record variable
       - Any use of any variable that uses struct type; record use
       TODO: (- Verify that all uses are "legal".)
     We currenlty traverse each function separately several times 
     one for each data structure of our interest, this is a gready
     algorithm, we should optimize this in the future. 
   */

  for (c_node = cgraph_nodes; c_node; c_node = c_node->next)
    {
      struct struct_list *ds_node;

      for (ds_node = data_struct_list; ds_node; ds_node = ds_node->next)
	{
          struct function *func = DECL_STRUCT_FUNCTION (c_node->decl);

          /* Build the access sites list for fields and also the field
	     access lists for basic blocks.  */
	  build_bb_access_list_for_struct (ds_node->struct_data, func);
          update_cpg_for_structure (ds_node->struct_data, func);
	  free_bb_access_list_for_struct (ds_node->struct_data, func);
	}
    }

  /* Stage 2:  Determine what, if anything, we want to transform and how.  */

  for (current_struct = data_struct_list; current_struct;
       current_struct = current_struct->next)
    {
      struct data_structure *crr_ds = current_struct->struct_data;
      success = cache_aware_data_reorganization (crr_ds, reordering_only);
      dump_cpg (stdout, crr_ds->cpg);
      free_cpg (crr_ds->cpg);
      crr_ds->cpg = NULL;

      /* Stage 3:  Do the actual transformation decided on in stage 2.  */

      if (success)
        do_peel (crr_ds);
    }
}

