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
#include "opts.h"

/* Not used yet.  Commented it out so we can bootstrap.  */
#if 0 
static htab_t visited_nodes;
#endif

struct struct_list {
  struct data_structure *struct_data;
  struct struct_list *next;
};

/* Returns the identifier pointer (char*) of the original 
   type of DECL.  */
static const char *
get_original_type_decl_name (tree decl)
{
  if (! decl)
    return NULL;
  if (TREE_CODE (decl) == IDENTIFIER_NODE)
    return IDENTIFIER_POINTER (decl);

  if (TREE_CODE (decl) != TYPE_DECL)
    decl = TYPE_NAME (decl);
  if (! decl) 
    return NULL;

  if (TREE_CODE (decl) == IDENTIFIER_NODE)
    return IDENTIFIER_POINTER (decl);

  decl =  DECL_ORIGINAL_TYPE (decl) ? DECL_ORIGINAL_TYPE (decl) : decl;

  if (TREE_CODE (decl) == IDENTIFIER_NODE)
    return IDENTIFIER_POINTER (decl);
  else if (TREE_CODE (decl) == TYPE_DECL) 
    return IDENTIFIER_POINTER (DECL_NAME (decl));

  decl = TYPE_NAME (decl);
  if (!decl)
    return NULL;
  if (TREE_CODE (decl) == IDENTIFIER_NODE)
    return IDENTIFIER_POINTER (decl);

  return IDENTIFIER_POINTER (DECL_NAME (decl));
}

/* Returns true if the two declations have similar types, 
   and fasle if not.  */
static bool
similar_struct_decls_p (tree decl1, tree decl2)
{
  const char *name1, *name2;

  if (! decl1 || ! decl2)
    return false;

  if (decl1 == decl2)
    return true;
  name1 =  get_original_type_decl_name (decl1); 
  name2 =  get_original_type_decl_name (decl2); 
  if (!name1 || !name2)
    return false; 
  if (! strcmp (name1, name2))
    return true; 
  return false;
}

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
	list[idx].count = 0;
      }

  return list;
}

void
verify_data_structure (struct data_structure *ds)
{
  int i;

  for (i = 0; i < ds->num_fields; i++)
    if (ds->fields[i].index != i || ! ds->fields[i].decl 
	|| TREE_CODE (ds->fields[i].decl) != FIELD_DECL
        || ((ds->fields[i].count > 0) && ! ds->fields[i].acc_sites))
      abort ();
}

/* Record STMT as the access site of the filed with INDEX of DS data
   structure.  OP is the COMPONENT_REF is access to the field in STMT.  */
static void
add_field_access_site (struct data_structure *ds, HOST_WIDE_INT bit_pos,
		       HOST_WIDE_INT bit_size, tree stmt, tree op, 
		       sbitmap fields)
{
  int i;
  struct access_site *as;

  for (i = 0; i < ds->num_fields; i++)
    {
      HOST_WIDE_INT f_bit_pos;
      tree f_bit_pos_t = bit_position (ds->fields[i].decl);

      if (! host_integerp (f_bit_pos_t, 0))
	continue;

      f_bit_pos = tree_low_cst (f_bit_pos_t, 0);
      if (f_bit_pos >= bit_pos && f_bit_pos < (bit_pos + bit_size))
	{
	  as = (struct access_site *)xcalloc (1, sizeof (struct access_site));
	  as->stmt = stmt;
	  as->field_access = op;
	  as->next = ds->fields[i].acc_sites;
	  ds->fields[i].acc_sites = as;
	  SET_BIT (fields, i);
	}
    }
}

#if 0
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
#endif

/* A helper function for get_stmt_accessed_fields.  */
static int
get_stmt_accessed_fields_1 (tree stmt, tree op, struct data_structure *ds, 
			    sbitmap fields)
{
  HOST_WIDE_INT bitsize = 0;
  HOST_WIDE_INT bitpos; 
  tree offsetr, struct_var;
  tree struct_type = NULL_TREE;
  enum machine_mode mode; 
  int unsignedp, volatilep;

#if 1
  struct_var = get_inner_reference (op, &bitsize, &bitpos, &offsetr, &mode, 
				    &unsignedp, &volatilep);
  if (! struct_var)
    return 0;


  if (TREE_CODE (struct_var) == VAR_DECL)
    struct_type = TREE_TYPE (struct_var);
  else if (TREE_CODE (struct_var) == INDIRECT_REF)
    struct_type = TREE_TYPE (TREE_TYPE (TREE_OPERAND (struct_var, 0)));

  if (! struct_type || ! similar_struct_decls_p (ds->decl, struct_type))
    {
      
      if ( bitsize < 0 )
        return  - bitsize / BITS_PER_UNIT;
      else
	return bitsize / BITS_PER_UNIT;
    }

  /* In case of a varaible size field access, invalidate the structure
     for the optimization*/
  if ( offsetr)
    {
      ds->unresolved_field_access = true;
      if ( bitsize < 0 )
        return - bitsize / BITS_PER_UNIT;
      else
        return bitsize / BITS_PER_UNIT;
    }  


  /* Add a field access site here.  */
  add_field_access_site (ds, bitpos, bitsize, stmt, op, fields);

  /* Access to fields of the relevant struct so the distance is 0.  */ 
  return 0; 

 
#else
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
#endif
}

/* Analyze the statement STMT and search for field accesses of the given
   data structure DS, returns zero if there are any.  Otherwise it returns 
   the accesses distance produced by this instruction.  We assume this is a 
   GIMPLE statement meaning that a memory access must be in a MODIFY_EXPR.  */
static int
get_stmt_accessed_fields (tree stmt, struct data_structure *ds, sbitmap fields)
{
  int code, dist0, dist1;
    

  code = TREE_CODE (stmt);
  if (code != MODIFY_EXPR)
    return 0;

  /* Analyze the left side of the MODIFY.  If dist is not zero it means
     that the left side (OP0) is a memory access that means that the
     right side cannot be a memory access, so just return DIST.  */
  dist0 = get_stmt_accessed_fields_1 (stmt, TREE_OPERAND (stmt, 0), ds, fields);
  dist1 = get_stmt_accessed_fields_1 (stmt, TREE_OPERAND (stmt, 1), ds, fields);

  if (! dist0 || ! dist1)
    return 0;
  else
    return dist0 + dist1;

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
  sbitmap fields = sbitmap_alloc (ds->num_fields);

  /* Create a new dummy item to hold the disntance to the first field
     in the basic block (or hold the distance accross the basic block
     if there is no fields accessed in it.  */
  crr = xcalloc (1, sizeof (struct bb_field_access));
  crr->f_index = -1;
  head = crr;

  /* Go over the basic block statements and create a linked list of
     the accessed fields.  */
  for (bsi = bsi_start (bb); ! bsi_end_p (bsi); bsi_next (&bsi))
    {
      tree stmt = bsi_stmt (bsi);
      int i;

      sbitmap_zero (fields);
      crr->distance_to_next += get_stmt_accessed_fields (stmt, ds, fields);

      EXECUTE_IF_SET_IN_SBITMAP (fields, 0, i, 
	/* The disntance to the next is zeroed here, if the next
	   access is in the same statement it will not be advanced
	   at all, and we will get a distance of zero.  */
	crr->next = xcalloc (1, sizeof (struct bb_field_access));
	crr->next->f_index = i;
	crr = crr->next;
	/* Update the access count for a single field.  */
	ds->fields[crr->f_index].count += bb->count;
      );
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

/* Search the data structure list DS_LIST for a data structure with 
   SDECL declaration return it if found and return NULL otherwise.  */
static struct data_structure *
get_data_struct_by_decl (struct struct_list *ds_list, tree sdecl)
{
  for (; ds_list ; ds_list = ds_list->next)
    if (similar_struct_decls_p (ds_list->struct_data->decl, sdecl))
      return ds_list->struct_data;

  return NULL;
}

/* This should free up all the memory allocated for the information 
   about the data structure DS.  */

static
void free_data_struct (ATTRIBUTE_UNUSED struct data_structure *ds)
{
  return;
}

/* Given a VAR_DECL it checks its type and create an appropriate
   STRUCT_LIST node if it is of a record type.  */

static struct struct_list *
make_data_struct_node (struct struct_list *s_list, tree var_decl)
{
  struct struct_list *new_node = NULL;
  struct data_structure *d_node;
  tree struct_type = NULL_TREE;
  tree var_type;
  int num_fields;

  if (TREE_CODE (var_decl) == PARM_DECL)
    var_type = DECL_ARG_TYPE (var_decl);
  else if (TREE_CODE (var_decl) == VAR_DECL)
    var_type = TREE_TYPE (var_decl);
  else
    return NULL;

  if ((TREE_CODE (var_type) == POINTER_TYPE
       || TREE_CODE (var_type) == ARRAY_TYPE)
      && TREE_CODE (TREE_TYPE (var_type)) == RECORD_TYPE)
    struct_type = TREE_TYPE (var_type);
  else if ( TREE_CODE (var_type) == RECORD_TYPE)
    struct_type = var_type;

  /* Check to see if this structure is already in there.  */
  if (! struct_type || get_data_struct_by_decl (s_list, struct_type))
    return NULL;

  d_node = (struct data_structure *) 
	   xcalloc (1, sizeof (struct data_structure));
  num_fields = fields_length (struct_type);
  d_node->decl = struct_type;
  d_node->num_fields = num_fields;
  d_node->fields = get_fields (struct_type, num_fields);
  d_node->alloc_sites = NULL;
  d_node->struct_clustering = NULL;

  new_node = (struct struct_list *) xmalloc (sizeof(struct struct_list));
  new_node->struct_data = d_node;
  new_node->next = s_list;

  if ((struct_type = TYPE_NAME (struct_type)))
    {
      if (TREE_CODE (struct_type) == TYPE_DECL)
        struct_type = DECL_NAME (struct_type);

      fprintf (stderr, "A new data structure to analyze: %s\n",
	       IDENTIFIER_POINTER (struct_type));
    }

  return new_node;
} 

/* Stage 1:  Find all potential structs for peeling. Perform analysis
   to decide if transformations are safe/legal.  */

static struct struct_list *
build_data_structure_list (void)
{
  tree var_decl;
  tree var_list;
  struct struct_list *tmp; 
  struct struct_list *data_struct_list = NULL;
  struct cgraph_varpool_node *current_varpool;
  struct cgraph_node *c_node;

  /* For every global variable in the program:
     - Check to see if it's of a candidate type; if so stuff type into
     data structure.
  */ 

  for (current_varpool = cgraph_varpool_nodes_queue; current_varpool;
       current_varpool = current_varpool->next_needed)
    {

      var_decl = current_varpool->decl;
      if (!var_decl || TREE_CODE (var_decl) != VAR_DECL)
	continue;
      
      if ((tmp = make_data_struct_node (data_struct_list, var_decl)))
	data_struct_list = tmp;
    }

  /* Now add data structures of function parameters and local variables */
  for (c_node = cgraph_nodes; c_node; c_node = c_node->next)
    {
      struct function *func = DECL_STRUCT_FUNCTION (c_node->decl);

      /* Function arguments.  */
      for (var_decl = DECL_ARGUMENTS (c_node->decl); 
	   var_decl; var_decl = TREE_CHAIN (var_decl)) 
	if ((tmp = make_data_struct_node (data_struct_list, 
					  var_decl)))
	  {
	    data_struct_list = tmp;
	    if (! func)
	      tmp->struct_data->in_extern_definition = true;
	  }
	
      if (! func)
	continue;

      /* Function local variables.  */
      for (var_list = func->unexpanded_var_list; var_list; 
	   var_list = TREE_CHAIN (var_list))
	{
	  var_decl = TREE_VALUE (var_list);
	  if (TREE_CODE (var_decl) == VAR_DECL)
	    if ((tmp = make_data_struct_node (data_struct_list, var_decl)))
	      data_struct_list = tmp;
	}
    }
  return data_struct_list;
}

static void
printout_structure_splitting_hints (struct data_structure *ds, FILE *outfile)
{
  int i, j;
  tree struct_id;
  struct field_cluster *crr; 

  if (!ds->struct_clustering->sibling)
    return;

  /* Print out the recomended ordering of the fields.  */
  if ( ! (struct_id = TYPE_NAME (ds->decl)))
    return;

  if (TREE_CODE (struct_id) == TYPE_DECL)
    struct_id = DECL_NAME (struct_id);

  fprintf (outfile, "Following are recommendations for structure splitting.\n");
  for (j=0, crr = ds->struct_clustering; crr; j++, crr = crr->sibling)
    {
      fprintf (outfile, "%s_%d \n{\n", IDENTIFIER_POINTER (struct_id), j);
      for (i = 0 ; i < ds->num_fields; i++)
	{  
	  tree field_type = TREE_TYPE (ds->fields[i].decl);
	  enum tree_code field_type_code = TREE_CODE (field_type);
	  const char *type_name; 
      
	  if (! TEST_BIT (crr->fields_in_cluster, i))
	    continue;

	  if (field_type_code == POINTER_TYPE 
	      || field_type_code == ARRAY_TYPE)
	    field_type = TREE_TYPE (field_type);
      
	  if (TREE_CODE (field_type) != TYPE_DECL)
	    field_type = TYPE_NAME (field_type);
	  type_name = get_original_type_decl_name (field_type);

	  switch (field_type_code)
	    {
	      case POINTER_TYPE :
	        fprintf (outfile, "\t%s\t*%s;\n", type_name,
		         IDENTIFIER_POINTER (DECL_NAME (ds->fields[i].decl)));
	        break;

	      case ARRAY_TYPE :
	        fprintf (outfile, "\t%s\t%s[]\n", type_name,
		         IDENTIFIER_POINTER (DECL_NAME (ds->fields[i].decl)));
	        break;
	      default :
	        fprintf (outfile, "\t%s\t%s;\n", type_name,
		         IDENTIFIER_POINTER (DECL_NAME (ds->fields[i].decl)));
	        break;
	    }
	}	       
      fprintf (outfile, "}\n");
    }
}
static void
printout_field_reordering_hints (struct data_structure *ds, FILE *outfile)
{
  int i;
  HOST_WIDE_INT prev_f_bit_pos;
  tree struct_id;
  bool different_order = false; 
  tree f_bit_pos_t; 

  if (!ds->struct_clustering->fields_order)
    return;

  f_bit_pos_t = bit_position (ds->fields[0].decl);
  if (! host_integerp (f_bit_pos_t, 0))
    abort ();
  prev_f_bit_pos = tree_low_cst (f_bit_pos_t, 0);

  /* Check if the suggested reordering is different that the original.  */
  for (i = 1; i < ds->num_fields; i++)
    {
      HOST_WIDE_INT f_bit_pos;
      int f;

      f = ds->struct_clustering->fields_order[i];
      f_bit_pos_t = bit_position (ds->fields[f].decl);
      if (! host_integerp (f_bit_pos_t, 0))
	abort ();
      f_bit_pos = tree_low_cst (f_bit_pos_t, 0);
      if (f_bit_pos < prev_f_bit_pos)
	{
  	  different_order = true;    
	  break;
	}
      prev_f_bit_pos = f_bit_pos;
    }
  if (! different_order )
    return;

  /* Print out the recomended ordering of the fields.  */
  if ( ! (struct_id = TYPE_NAME (ds->decl)))
    return;

  if (TREE_CODE (struct_id) == TYPE_DECL)
    struct_id = DECL_NAME (struct_id);

  fprintf (outfile, "Following are recommendations for field reordering.\n");
  fprintf (outfile, "%s \n{\n", IDENTIFIER_POINTER (struct_id));
  for (i = 0 ; i < ds->num_fields; i++)
    {
      int f = ds->struct_clustering->fields_order[i];
      tree field_type = TREE_TYPE (ds->fields[f].decl);
      enum tree_code field_type_code = TREE_CODE (field_type);
      const char *type_name; 
      
      if (field_type_code == POINTER_TYPE 
	  || field_type_code == ARRAY_TYPE)
	field_type = TREE_TYPE (field_type);
      type_name = get_original_type_decl_name (field_type);      

      switch (field_type_code)
	{
	  case POINTER_TYPE :
	    fprintf (outfile, "\t%s\t*%s;\n", type_name,
		     IDENTIFIER_POINTER (DECL_NAME (ds->fields[f].decl)));
	    break;

	  case ARRAY_TYPE :
	    fprintf (outfile, "\t%s\t%s[]\n", type_name,
		     IDENTIFIER_POINTER (DECL_NAME (ds->fields[f].decl)));
	    break;
	  default :
	    fprintf (outfile, "\t%s\t%s;\n", type_name,
		     IDENTIFIER_POINTER (DECL_NAME (ds->fields[f].decl)));
	    break;
	}
	       
    }
  fprintf (outfile, "}\n");
}

/* Perform data structure peeling.  */
void
peel_structs (void)
{
  struct struct_list *data_struct_list = NULL;
  struct struct_list *current_struct;
  struct cgraph_node *c_node;
  bool reordering_only = false;
  bool success;
  gcov_type hotest_struct_count = 0;
  FILE *vcg_dump = NULL;
   
  vcg_dump = fopen (concat (dump_base_name, ".struct-reorg.vcg", NULL), "w");

  /* Verify that this compiler invocation was passed *all* the user-written
     code for this program.  */
  
  if (! flag_whole_program)
    {
      inform 
	("Whole program not passed to compiler: Can't perform struct peeling.");
      return;
    }

  /* Stage 1: Build DATA_STRUCTURE list of the data structures that are valid
     for the transformation.  */
  data_struct_list = build_data_structure_list ();

  /* Walk through entire program tree looking for:
       - Any declaration that uses struct type; record variable
       - Any use of any variable that uses struct type; record use
       TODO: (- Verify that all uses are "legal".)
     We currenlty traverse each function separately several times 
     one for each data structure of our interest, this is a gready
     algorithm, we should optimize this in the future. 
   */
  if (vcg_dump)
    fprintf (vcg_dump, "graph: {\n");
  for (current_struct = data_struct_list; current_struct; 
       current_struct = current_struct->next)
    {
      struct data_structure *crr_ds = current_struct->struct_data;

      /* Stage 2:  Determine what, if anything, we want to transform and how.  */
      for (c_node = cgraph_nodes; c_node; c_node = c_node->next)
        {
          struct function *func = DECL_STRUCT_FUNCTION (c_node->decl);

	  /* There's no point in re-analyzing the same function definition
	     more than once... */

	  if (!c_node->next_clone)
	    {	      
	      /* Build the access sites list for fields and also the field
		 access lists for basic blocks.  */
	      build_bb_access_list_for_struct (crr_ds, func);
	      update_cpg_for_structure (vcg_dump, crr_ds, func);
	      free_bb_access_list_for_struct (crr_ds, func);
	    }
	}
      if (crr_ds->count > hotest_struct_count)
	hotest_struct_count = crr_ds->count;
    }
  for (current_struct = data_struct_list; current_struct; 
       current_struct = current_struct->next)
    {
      struct data_structure *crr_ds = current_struct->struct_data;

      if (crr_ds->count < (hotest_struct_count / COLD_STRUCTURE_RATIO))
	continue;
      success = cache_aware_data_reorganization (crr_ds, reordering_only);
      dump_cpg (stdout, crr_ds->cpg);
      printout_field_reordering_hints (crr_ds, stdout);
      printout_structure_splitting_hints (crr_ds, stdout);
      free_cpg (crr_ds->cpg);
      crr_ds->cpg = NULL;
      /* Stage 3:  Do the actual transformation decided on in stage 2.  */

      if (success)
        do_peel (crr_ds);
      /* Free up the memory allocated for the CRR_DS.  */
      free_data_struct (crr_ds);
    }
    if (vcg_dump)
      {
	fprintf (vcg_dump, "}\n");
	fclose (vcg_dump);
      }
}

