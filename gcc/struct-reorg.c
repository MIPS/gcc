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
#include "tree-gimple.h"
#include "tree-inline.h"
#include "tree-flow.h"
#include "tree-flow-inline.h"
#include "langhooks.h"
#include "pointer-set.h"
#include "hashtab.h"
#include "c-tree.h"
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
#include "intl.h"
#include "function.h"
#include "basic-block.h"
#include "tree-iterator.h"
#include "tree-pass.h"
#include "struct-reorg.h"
#include "opts.h"
#include "ipa-static.h"

/*  ************** Static Global Data Structures  ********************  */

/* A linked list of information about program statemetns that use structs
   in which we are interested.  Data is first collected into a list of
   these nodes, then the list is traversed to make the approrpriate changes
   to the program.  Each record corresponds to an assignment stmt in the
   gimple.  */

struct use_struct {
  bool lhs_use;                       /* Indicates if lhs uses struct/field */
  tree lhs;                           /* Lhs of the assignment              */
  tree offset;                        /* IFF stmt converts an integer to an
	                                 array offset, contains rhs of stmt 
	                                 otherwise is null.                 */
  struct struct_tree_list *rhs_vars; /* IFF stmt is not an offset conversion,
	                                this field contains a list of
	                                variables in the rhs of the stmt
	                                that are of struct types we care 
	                                about.                              */
  tree stmt;                         /* The complete assignment stmt.       */
  basic_block bb;	             /* The basic block containing the 
	                                assignment stmt.                    */
  tree function;                     /* The function containing the assignment
	                                statement.                          */
  struct use_struct *next;
};


/* A linked list of struct types which we are interested in optimizing.  */

struct struct_list {
  struct data_structure *struct_data;
  struct struct_list *next;
};


/* Data for new (sub) types we create when we split or peel a struct.  */

struct new_type_node {
  bool sub_type;            /* Indicates whether we're peeling or splitting */
  int num_fields;           /* Number of fields in the new type             */
  tree field_list;          /* Tree chain of the new fields                 */
  char *name;               /* Name of the new type                         */
  struct new_type_node *next;
};


/* Data about the new program variables created (for the new peeled types).  
   When doing struct peeling, each variable of the original struct type will
   need to be replaced with a set of new variables, one new variable for each
   new type created from the original type.  */

struct new_var_data {
  tree orig_var;                      /* Var decl for original struct type  */
  struct struct_tree_list *new_vars;  /* List of new vars to replace the 
					 original var; one new var for each 
					 new type peeled off the original 
					 type.  */
  struct new_var_data *next;
};


/* When peeling structs, we need to change the original struct malloc 
   statement, and add new malloc statements for each new sub-type.  To do 
   this properly we need to collect a lot of data from the original malloc, 
   particularly as the gimplifier has converted:

   orig_var = (struct_type *) malloc (x * sizeof (struct_type *));

   into

   T3 = <constant> ;  ** <constant> is amount to malloc; precomputed **
   T4 = malloc (T3);
   T5 = (struct_type *) T4;
   orig_var = T5;

   The following struct fields allow us to collect all the necessary data from
   the gimplified program.  The comments in the struct below are all based
   on the gimple example above.  */

struct malloc_call_data 
{
  tree call_stmt;        /* Tree for "T4 = malloc (T3);"                    */
  tree size_var;         /* Var decl for T3.                                */
  tree result_var;       /* Var decl for T4.                                */
  tree final_lhs;        /* Var decl for T5.                                */
  tree malloc_size;      /* Tree for "<constant>",  the rhs assigned to T3. */
  tree malloc_type;      /* Decl for "(struct_type *)" from cast statement. */
  tree num_elts;         /* Needed for "calloc" calls.                      */
  tree ptr_var;          /* Needed for "realloc" calls.                     */
};

/* List of data for all malloc calls in the program.  */

struct malloc_struct
{
  tree context;                       /* Function decl call is made from    */
  int num_calls;                      /* Number of malloc calls in context  */
  struct malloc_call_data *malloc_list;  /* List of call site data for each
					    call to malloc from the current
					    function (context).             */
  struct malloc_struct *next;         /* Next item in linked list.          */
};



static struct pointer_set_t * visited_nodes;
struct struct_tree_list *struct_reorg_malloc_list = NULL;
struct malloc_struct *malloc_data_list = NULL;

static void debug_gimple (tree);
static bool similar_struct_decls_p (tree, tree);

static bool
replace_old_field_types (struct struct_tree_list *, tree, tree,
			 struct struct_tree_list **);

static bool
found_in_list (tree struct_type, struct struct_tree_list *list)
{
  struct struct_tree_list *current;
  
  for (current = list; current; current = current->next)
    if (current->data == struct_type)
      return true;

  return false;
}

/* The following variable is for testing/comparison purposes only.  It 
   indicates whether we should use the data generated from Stage2, or 
   automatically split all the fields apart (for the f1_neuron struct, in
   the 'art' benchmark, only.  */

bool split_all_fields = false;
#if 0
/* The following function is for testing/comparison purposes only, it 
   creates a field_cluster record that would be generated by stage 2,
   if stage2 decided to split all the fields of f1_neuron (from 'art')
   into separate sub-structs.   This complete splitting gets the
   best performance from 'art' that I have seen so far, so I want to
   keep this function around to compare against.  */

static struct field_cluster *
build_f1neuron_cluster_info (void)
{
  struct field_cluster *p;
  struct field_cluster *u;
  struct field_cluster *w;
  struct field_cluster *r;
  struct field_cluster *x;
  struct field_cluster *q;
  struct field_cluster *v;
  struct field_cluster *i;

  i = (struct field_cluster *) xmalloc (sizeof (struct field_cluster));
  i->fields_order = NULL;
  i->fields_in_cluster = sbitmap_alloc (8);
  sbitmap_zero (i->fields_in_cluster);
  SET_BIT (i->fields_in_cluster, 0);
  i->direct_access = true;
  i->parent = NULL;
  i->children = NULL;
  i->sibling = NULL;
  
  w = (struct field_cluster *) xmalloc (sizeof (struct field_cluster));
  w->fields_order = NULL;
  w->fields_in_cluster = sbitmap_alloc (8);
  sbitmap_zero (w->fields_in_cluster);
  SET_BIT (w->fields_in_cluster, 1);
  w->direct_access = true;
  w->parent = NULL;
  w->children = NULL;
  w->sibling = i;

  x = (struct field_cluster *) xmalloc (sizeof (struct field_cluster));
  x->fields_order = NULL;
  x->fields_in_cluster = sbitmap_alloc (8);
  sbitmap_zero (x->fields_in_cluster);
  SET_BIT (x->fields_in_cluster, 2);
  x->direct_access = true;
  x->parent = NULL;
  x->children = NULL;
  x->sibling = w;

  v = (struct field_cluster *) xmalloc (sizeof (struct field_cluster));
  v->fields_order = NULL;
  v->fields_in_cluster = sbitmap_alloc (8);
  sbitmap_zero (v->fields_in_cluster);
  SET_BIT (v->fields_in_cluster, 3);
  v->direct_access = true;
  v->parent = NULL;
  v->children = NULL;
  v->sibling = x;

  u = (struct field_cluster *) xmalloc (sizeof (struct field_cluster));
  u->fields_order = NULL;
  u->fields_in_cluster = sbitmap_alloc (8);
  sbitmap_zero (u->fields_in_cluster);
  SET_BIT (u->fields_in_cluster, 4);
  u->direct_access = true;
  u->parent = NULL;
  u->children = NULL;
  u->sibling = v;

  p = (struct field_cluster *) xmalloc (sizeof (struct field_cluster));
  p->fields_order = NULL;
  p->fields_in_cluster = sbitmap_alloc (8);
  sbitmap_zero (p->fields_in_cluster);
  SET_BIT (p->fields_in_cluster, 5);
  p->direct_access = true;
  p->parent = NULL;
  p->children = NULL;
  p->sibling = u;

  q = (struct field_cluster *) xmalloc (sizeof (struct field_cluster));
  q->fields_order = NULL;
  q->fields_in_cluster = sbitmap_alloc (8);
  sbitmap_zero (q->fields_in_cluster);
  SET_BIT (q->fields_in_cluster, 6);
  q->direct_access = true;
  q->parent = NULL;
  q->children = NULL;
  q->sibling = p;
  
  r = (struct field_cluster *) xmalloc (sizeof (struct field_cluster));
  r->fields_order = NULL;
  r->fields_in_cluster = sbitmap_alloc (8);
  sbitmap_zero (r->fields_in_cluster);
  SET_BIT (r->fields_in_cluster, 7);
  r->direct_access = true;
  r->parent = NULL;
  r->children = NULL;
  r->sibling = q;
  
  return r;
}
#endif

/* Given a tree representing a type, this function returns the 
   name of the type, as a string.  */

static char *
get_type_name (tree type_node)
{
  if (! TYPE_NAME (type_node))
    return NULL;

  if (TREE_CODE (TYPE_NAME (type_node)) == IDENTIFIER_NODE)
    return (char *) IDENTIFIER_POINTER (TYPE_NAME (type_node));
  else if (TREE_CODE (TYPE_NAME (type_node)) == TYPE_DECL
	   && DECL_NAME (TYPE_NAME (type_node)))
    return (char *) IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (type_node)));
  else
    return NULL;
}

static void
debug_gimple (tree t)
{
  print_generic_stmt (stdout, t, TDF_SLIM | TDF_BLOCKS | TDF_VOPS);
}


static void
print_new_vars (struct new_var_data *new_vars)
{
  struct new_var_data *current;
  struct struct_tree_list *cur_var;
  tree var_type;
  char *type_name;
  bool global_context;

  for (current = new_vars; current; current = current->next)
    {
      var_type = TREE_TYPE (current->orig_var);
      while (POINTER_TYPE_P (var_type)
	     || TREE_CODE (var_type) == ARRAY_TYPE)
	var_type = TREE_TYPE (var_type);
      type_name = get_type_name (var_type);

      fprintf (stdout, "Orig var: ");
      if (DECL_NAME (current->orig_var)
	  && IDENTIFIER_POINTER (DECL_NAME (current->orig_var)))
	fprintf (stdout, "%s\n", 
		 (char *) IDENTIFIER_POINTER (DECL_NAME (current->orig_var)));
      else
	fprintf (stdout, " <tmp var.%d> of type %s\n",
		 current->orig_var->decl.uid,
		 type_name);

      for (cur_var = current->new_vars; cur_var; cur_var = cur_var->next)
	{
	  tree var_decl = cur_var->data;
	  char *var_name;

	  if (DECL_NAME (var_decl)
	      && IDENTIFIER_POINTER (DECL_NAME (var_decl)))
	    var_name = (char *) IDENTIFIER_POINTER (DECL_NAME (var_decl));
	  else
	    var_name = NULL;

	  var_type = TREE_TYPE (var_decl);
	  
	  while (POINTER_TYPE_P (var_type))
	    var_type = TREE_TYPE (var_type);
	  
	  type_name = get_type_name (var_type);
	  if (! (DECL_CONTEXT (var_decl))
	      || TREE_CODE (DECL_CONTEXT (var_decl)) == TRANSLATION_UNIT_DECL)
	    global_context = true;
	  else
	    global_context = false;
	  
	  if (var_name)
	    fprintf (stdout, "     %s declared in function %s of type %s\n",
		     var_name,
		     (! global_context ?
		      IDENTIFIER_POINTER (DECL_NAME (DECL_CONTEXT (var_decl))) :
		      "global"),
		     type_name ? type_name : "");
	  else
	    fprintf (stdout, 
		    "     <tmp var.%d>  declared in function '%s' of type '%s'\n",
		     var_decl->decl.uid,
		     (! global_context ?
		      IDENTIFIER_POINTER (DECL_NAME (DECL_CONTEXT (var_decl))) :
		      "global"),
		     type_name ? type_name : "");
	}
    }
}

static void
print_use_list (struct use_struct *uses_list)
{
  tree old_fn_decl = NULL_TREE;
  basic_block old_bb = NULL;
  struct use_struct *current;

  for (current = uses_list; current; current = current->next)
    {
      if (current->function != old_fn_decl)
	{
	  old_fn_decl = current->function;
	  fprintf (stdout, "\nFunction %s: \n",
		   (char *) IDENTIFIER_POINTER (DECL_NAME (old_fn_decl)));
	}
      if (old_bb != current->bb)
	{
	  old_bb = current->bb;
	  fprintf (stdout, "  <bb %d>\n", old_bb->index);
	}

      fprintf (stdout, "    <%s>", current->offset ? "offset" : "other");
      print_generic_stmt (stdout, current->stmt, 
			  TDF_SLIM | TDF_BLOCKS | TDF_VOPS);
    }

}

/* Print out an individual new type (generated by this optimization).  
   Helper function for print_new_types.  */

static void
print_type (tree new_type, char * indent, struct struct_tree_list **done,
	    struct struct_tree_list **worklist)
{
  struct struct_tree_list *current;
  struct struct_tree_list *prev;
  struct struct_tree_list *tmp_node;
  char * new_indent;
  char * struct_name;
  tree cur_field;
  int len;
  int i;

  if (!new_type)
    return;

  if (TREE_CODE (new_type) != RECORD_TYPE)
    return;

  struct_name = get_type_name (new_type);

  if (struct_name)
    {
      fprintf (stdout, "%s%s {\n", indent, struct_name);
      len = strlen (struct_name) + strlen (indent) + 3;
    }
  else
    {
      fprintf (stdout, "%s{\n", indent);
      len = strlen (indent) + 2;
    }
      
  new_indent = (char *) xmalloc (len * sizeof (char));
  memset (new_indent, ' ', len);
  new_indent[len] = '\0';
 
  for (cur_field = TYPE_FIELDS (new_type); cur_field; 
       cur_field = TREE_CHAIN (cur_field))
    {
      tree field_type;
      int ptr_count = 0;
      field_type = TREE_TYPE (cur_field);
      while (POINTER_TYPE_P (field_type))
	{
	  ptr_count++;
	  field_type = TREE_TYPE (field_type);
	}

      fprintf (stdout, "%s%s ", new_indent, get_type_name (field_type));
      for (i = 0; i < ptr_count; i++)
	fprintf (stdout, "*");
      fprintf (stdout, " %s;\n", IDENTIFIER_POINTER (DECL_NAME (cur_field)));
      
      if (TREE_CODE (field_type) == RECORD_TYPE)
	{
	  /* Add field_type to worklist */
	  tmp_node = (struct struct_tree_list *)  xmalloc (sizeof (struct struct_tree_list));
	  tmp_node->data = field_type;
	  tmp_node->next = *worklist;
	  
	  *worklist = tmp_node;
	}

    }

  fprintf (stdout, "%s}\n", indent);

  /* Remove new_type from worklist and put new_type on done list */

  prev = NULL;
  current = *worklist;
  tmp_node = NULL;
  
  while (current)
    {
      if (current->data == new_type)
	{
	  tmp_node = current;
	  if (!prev)
	    {
	      current = current->next;
	      *worklist = current;
	    }
	  else
	    {
	      prev->next = current->next;
	      current = current->next;
	    }
	}
      else
	{
	  prev = current;
	  current = current->next;
	}
    }

  if (tmp_node)
    tmp_node->next = *done;
  else
    {
      tmp_node = (struct struct_tree_list *) xmalloc (sizeof 
						     (struct struct_tree_list));
      tmp_node->data = new_type;
      tmp_node->next = *done;
    }
  
  *done = tmp_node;
	

  /* Make sure all types on worklist are NOT in done list.  */

  for (prev = NULL, current = *worklist; current; current = current->next)
    if (found_in_list (current->data, *done))
      {
	if (!prev)
	  *worklist = current->next;
	else
	  prev->next = current->next;
      }
    else
      {
	prev = current;
      }
}

/* Print out the list of new types generated by this optimization.  */

static void
print_new_types (struct struct_tree_list *new_type)
{
  char indent[3] = "  ";
  struct struct_tree_list *worklist = NULL;
  struct struct_tree_list *done = NULL;

  while (new_type)
    {
      print_type (new_type->data, indent, &done, &worklist);
      new_type = new_type->next;
    }

  while (worklist)
    print_type (worklist->data, indent, &done, &worklist);
}

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
	list[idx].new_mapping = NULL;
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

static tree
find_field_offset (tree exp)
{
  tree result = NULL_TREE;

  while (1)
    {
      switch (TREE_CODE (exp))
	{
	case BIT_FIELD_REF: 
	case ARRAY_REF:
	case ARRAY_RANGE_REF:
	case REALPART_EXPR:
	case IMAGPART_EXPR:
	  break;
	case COMPONENT_REF:
	  result = component_ref_field_offset (exp);
	  break;
	default:
	  goto done;
	}
      exp = TREE_OPERAND (exp, 0);
    }
  
 done:
  return result;
}

/* Record STMT as the access site of the filed with INDEX of DS data
   structure.  OP is the COMPONENT_REF is access to the field in STMT.  */
static void
add_field_access_site (struct data_structure *ds, HOST_WIDE_INT bit_pos,
		       HOST_WIDE_INT bit_size, tree stmt, tree op, 
		       sbitmap fields, basic_block bb, 
		       struct function *context, tree offsetr)
{
  int i;
  struct access_site *as;
  tree field_offset_t = NULL_TREE;
  HOST_WIDE_INT field_offset = 0;

  if (offsetr)
    {
      field_offset_t = find_field_offset (op);
      if (field_offset_t)
	field_offset = tree_low_cst (field_offset_t, 0);
    }

  for (i = 0; i < ds->num_fields; i++)
    {
      HOST_WIDE_INT f_bit_pos;
      tree f_bit_pos_t = bit_position (ds->fields[i].decl);
      bool found_field;

      if (! host_integerp (f_bit_pos_t, 0))
	continue;

      f_bit_pos = tree_low_cst (f_bit_pos_t, 0);

      if (!offsetr 
	  && (f_bit_pos >= bit_pos && f_bit_pos < (bit_pos + bit_size)))
	found_field = true;
      else if (offsetr
	       && (f_bit_pos == (8 * field_offset) + bit_pos))
	found_field = true;
      else
	found_field = false;

      if (found_field)
	{
	  as = (struct access_site *)xcalloc (1, sizeof (struct access_site));
	  as->stmt = stmt;
	  as->bb = bb;
	  as->context = context;
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
			    sbitmap fields, basic_block bb, 
			    struct function *context)
{
  HOST_WIDE_INT bitsize = 0;
  HOST_WIDE_INT bitpos; 
  tree offsetr, struct_var;
  tree struct_type = NULL_TREE;
  enum machine_mode mode; 
  int unsignedp, volatilep;

#if 1
  struct_var = get_inner_reference (op, &bitsize, &bitpos, &offsetr, &mode, 
				    &unsignedp, &volatilep, false);
  if (! struct_var
      || (struct_var == op))
    return 0;

  if (TREE_CODE (struct_var) == VAR_DECL)
    struct_type = TREE_TYPE (struct_var);
  else if (TREE_CODE (struct_var) == INDIRECT_REF
	   || TREE_CODE (struct_var) == MEM_REF)
    struct_type = TREE_TYPE (TREE_TYPE (TREE_OPERAND (struct_var, 0)));

  if (struct_type && (TREE_CODE (struct_type) == ARRAY_TYPE))
    struct_type = TREE_TYPE (struct_type);

  if (! struct_type || ! similar_struct_decls_p (ds->decl, struct_type))
    {
      
      if ( bitsize < 0 )
        return  - bitsize / BITS_PER_UNIT;
      else
	return bitsize / BITS_PER_UNIT;
    }

  /* In case of a varaible size field access, invalidate the structure
     for the optimization*/
  /*
  if ( offsetr)
    {
      ds->unresolved_field_access = true;
      if ( bitsize < 0 )
        return - bitsize / BITS_PER_UNIT;
      else
        return bitsize / BITS_PER_UNIT;
    }  
  */

  /* Add a field access site here.  */
  add_field_access_site (ds, bitpos, bitsize, stmt, op, fields, bb, context,
			 offsetr);

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
          add_field_access_site (ds, index, stmt, op, bb, context);

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
      	    dist = get_stmt_accessed_fields_1 (stmt, opi, ds, bb, context);
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
get_stmt_accessed_fields (tree stmt, struct data_structure *ds, sbitmap fields,
			  basic_block bb, struct function *context)
{
  int code, dist0, dist1;
    

  code = TREE_CODE (stmt);
  if (code != MODIFY_EXPR)
    return 0;

  /* Analyze the left side of the MODIFY.  If dist is not zero it means
     that the left side (OP0) is a memory access that means that the
     right side cannot be a memory access, so just return DIST.  */
  dist0 = get_stmt_accessed_fields_1 (stmt, TREE_OPERAND (stmt, 0), ds, fields, 
				      bb, context);
  dist1 = get_stmt_accessed_fields_1 (stmt, TREE_OPERAND (stmt, 1), ds, fields, 
				      bb, context);

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
build_f_acc_list_for_bb (struct data_structure *ds, basic_block bb, 
			 struct function *context)
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
      crr->distance_to_next += get_stmt_accessed_fields (stmt, ds, fields, bb,
							 context);

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
      ds->bbs_f_acc_lists[bb->index] = build_f_acc_list_for_bb (ds, bb, f);
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

/* The following function takes information about a new struct type to
   be created (NEW_TYPE and NAME), as well as information about the
   original struct that is being reorganized, split or peeled (ORIG_STRUCT)
   and creates a new type, as specified.  It returns the tree for the
   new type decl.  Pieces of this function were copied from the functions
   start_struct and finish_struct.  */

tree
build_basic_struct (void *new_type_info, char * name, tree orig_struct)
{
  /* Takes a struct new_type_node and builds (and returns) a type
     tree.  */

  struct new_type_node *new_type = (struct new_type_node *) new_type_info;
  tree attributes = NULL_TREE;
  tree ref = 0;
  tree x;

  /* From start_struct ()  */

  if (TYPE_ATTRIBUTES (orig_struct))
    attributes = copy_node (TYPE_ATTRIBUTES (orig_struct));
  ref = make_node (RECORD_TYPE);
  C_TYPE_BEING_DEFINED (ref) = 1;
  TYPE_PACKED (ref) = flag_pack_struct;
  
  /* From finish_struct (ref, tree fieldlist, tree attributes);  */

  TYPE_SIZE (ref) = 0;
  lang_hooks.optimize.decl_attributes (&ref, attributes, 
				       (int) ATTR_FLAG_TYPE_IN_PLACE);
  TYPE_PACKED (ref) = TYPE_PACKED (orig_struct);
  C_TYPE_FIELDS_READONLY (ref) = C_TYPE_FIELDS_READONLY (orig_struct);
  C_TYPE_FIELDS_VOLATILE (ref) = C_TYPE_FIELDS_VOLATILE (orig_struct);
  C_TYPE_VARIABLE_SIZE (ref) = C_TYPE_VARIABLE_SIZE (orig_struct);
  for (x = new_type->field_list; x; x = TREE_CHAIN (x))
    {
      DECL_CONTEXT (x) = ref;
      DECL_PACKED (x) |= TYPE_PACKED (ref);
    }
  TYPE_FIELDS (ref) = new_type->field_list;
  layout_type (ref);
  
  TYPE_NAME (ref) = get_identifier (name);

  return ref;
}


/* When we are splitting a struct (but NOT peeling it), some of the fields
   from the original struct are grouped together in a new struct.  However
   we need to create a new field in the original struct to point to the
   new sub-struct.  This function creates the field decl for the new
   field.

   The following example should clarify what we are doing:

   struct orig {
       T1 f1;
       T2 f2;
       T3 f3;
       T4 f4;
       struct orig *next:
   }

   might become

   struct orig {
       T1 f1;
       struct orig_sub_1 *f_orig_sub_1;
       struct orig *next;
   }

   struct orig_sub_1 {
        T2 f2;
        T3 f3;
        T4 f4;
   }

   From the above example, the following function would create and return the
   tree for the new field "struct orig_sub_1 *f_orig_sub_1".  */

static tree
make_new_field_decl (tree sub_struct)
{
  tree new_field_decl;
  tree name;

  /* The type for the new field will be "pointer to the new sub-struct".  
     Create such a type here.  */

  tree ptr_type = build_pointer_type (sub_struct);
  char * name_str;
  char *struct_name;

  /* We will give the new field a name based on the new type it will be
     pointing to.  Get the new type name, and prepend "f_" to it.  */

  struct_name = get_type_name (sub_struct);

  name_str = (char *) xmalloc ((strlen (struct_name) + 3) * sizeof (char));

  sprintf (name_str, "f_%s", struct_name);

  /* Make sure there isn't anything else that already has this name.  */

  while (maybe_get_identifier (name_str))
    {
      int len = strlen (name_str) + 3;
      char * tmp_string = (char *) xmalloc (len * sizeof (char));
      sprintf (tmp_string, "%s.0",  name_str);
      name_str = tmp_string;
    }

  /* Now create and return the new field decl tree.  */

  name = get_identifier (name_str);
  new_field_decl = make_node_stat (FIELD_DECL PASS_MEM_STAT);
  DECL_NAME (new_field_decl) = name;
  TREE_TYPE (new_field_decl) = ptr_type;
  DECL_VISIBILITY (new_field_decl) = default_visibility;

  return new_field_decl;
}

/*  As we move and re-arrange fields within the new struct hierarchy, keep
    track of which new type contains the field, and which other new
    types contain/point to that type, etc. so that we know how to 
    appropriately update the field accesses in the program once we are done 
    creating the new types. 

    The field map is a doubly-linked list indicating the super-struct/
    sub-struct relations. "T1.contains == T2" means T2 is a sub-struct
    type, and T1 has a field f_T2 that points to T2. This implies that
    T2.containing_type == T1.  By following this hierarchy, we can
    work out the correct way to access a field in the new type
    hierarchy.

    When we do struct peeling, there should only be one level of
    hierarchy, so each field should have a new_mapping entry that
    looks like this:

         new_mapping->decl = new peeled type
	 new_mapping->containing_type = NULL;
	 new_mapping->contains = NULL
*/

static bool
struct_contains_field_p (tree the_struct, tree field)
{
  tree cur_field;


  for (cur_field = TYPE_FIELDS (the_struct); cur_field; 
       cur_field = TREE_CHAIN (cur_field))
    if ((strcmp (IDENTIFIER_POINTER (DECL_NAME (cur_field)),
		 IDENTIFIER_POINTER (DECL_NAME (field))) == 0)
	&& (TREE_CODE (TREE_TYPE (cur_field)) == TREE_CODE (TREE_TYPE (field))))
      return true;

  return false;
}

static bool
struct_contains_substruct_p (tree superstruct, tree substruct)
{
  tree cur_field;
  tree f_type;

  for (cur_field = TYPE_FIELDS (superstruct); cur_field;
       cur_field = TREE_CHAIN (cur_field))
    {
      f_type = TREE_TYPE (cur_field);
      while (POINTER_TYPE_P (f_type))
	f_type = TREE_TYPE (f_type);

      if (similar_struct_decls_p (f_type, substruct))
	return true;
    }

  return false;
}

static void
update_field_mappings (tree old_struct, tree new_type,
		       struct data_structure *struct_data)
{
  int i;
  char *struct_name;
  struct field_map *new_container;
  struct field_map *current;

  struct_name = get_type_name (old_struct);

  for (i = 0; i < struct_data->num_fields; i++)
    if (struct_data->fields[i].new_mapping
	&& struct_contains_substruct_p (new_type,
					struct_data->fields[i].new_mapping->decl))
      {
	for (current = struct_data->fields[i].new_mapping;
	     current && current->containing_type;
	     current = current->containing_type);
	
	if (!current)
	  continue;
	
	new_container = (struct field_map *) xmalloc (sizeof 
						      (struct field_map));
	new_container->decl = new_type;
	new_container->containing_type = NULL;
	new_container->contains = current;
	
	current->containing_type = new_container;
	struct_data->fields[i].new_mapping = new_container;
      }
}

/* Takes a tree of a type decl (or a list of such trees), and a list of such 
   trees, and adds the new tree(s) to the list.  */

static void
add_to_types_list (struct struct_tree_list *new_list, 
		   struct struct_tree_list **old_list)
{
  struct struct_tree_list *current;

  if (!(*old_list))
    *old_list = new_list;
  else
    {
      for (current = *old_list; current && current->next;
	   current = current->next);
      current->next = new_list;
    }
}

/* This function is the main function for creating the new struct
   hierarchy from the cluster info recommendations.  It takes information
   about an original struct type (STRUCT_DATA) and field cluster information
   (FIELD_INFO) and builds the recommended new struct hiearchy.  */

static struct struct_tree_list *
create_and_assemble_new_types (struct data_structure *struct_data,
			       struct field_cluster *field_info,
			       int *count, int depth)
{
  struct new_type_node *new_types = NULL;
  struct struct_tree_list *child_types = NULL;
  struct struct_tree_list *sib_types = NULL;
  struct struct_tree_list *return_list = NULL;
  struct struct_tree_list *cur_child;
  struct struct_tree_list *tmp_node;
  struct field_map *tmp_map;
  int i;
  int k;
  tree last_field = NULL;
  tree new_decl;
  char *new_name;
  char *old_name;
  tree new_type;

  old_name = get_type_name (struct_data->decl);

  if (field_info->children)
    child_types = create_and_assemble_new_types (struct_data, 
						 field_info->children,
						 count, depth++);

  if (field_info->sibling)
    sib_types = create_and_assemble_new_types (struct_data,
					       field_info->sibling,
					       count, depth++);

  if (sib_types)
    add_to_types_list (sib_types, &return_list);

  k = *count;
  new_name = (char *) xmalloc ((strlen (old_name) + 11) * sizeof (char));
  sprintf (new_name, "%s_sub_%d", old_name, k);
  
  /* Make sure there isn't anything else that already has that name.  */

  while (maybe_get_identifier (new_name))
    {
      int len = strlen (new_name) + 3;
      char * tmp_name = (char *) xmalloc (len * sizeof (char));
      sprintf (tmp_name, "%s.0", new_name);
      new_name = tmp_name;
    }

  k++;
  *count = k;

  new_types = (struct new_type_node *) xmalloc (sizeof 
						      (struct new_type_node));

  new_types->sub_type = !field_info->direct_access;
  new_types->num_fields = 0;
  new_types->field_list = NULL_TREE;
  new_types->name = NULL;
  new_types->next = NULL;

  for (i = 0; i < struct_data->num_fields; i++)
    if (TEST_BIT (field_info->fields_in_cluster, i))
      {
	new_decl = copy_node (struct_data->fields[i].decl);
	new_decl->decl.rtl = NULL;
	
	struct_data->fields[i].new_mapping = NULL;
	new_types->num_fields++;

	if (!new_types->field_list)
	  new_types->field_list = new_decl;
	else
	  TREE_CHAIN (last_field) = new_decl;
	last_field = new_decl;
      }

  for (cur_child = child_types; cur_child; cur_child = cur_child->next)
    {
      tree tmp_node = make_new_field_decl (cur_child->data);
      TREE_CHAIN (last_field) = tmp_node;
      last_field = tmp_node;
      new_types->num_fields++;
    }

  TREE_CHAIN (last_field) = NULL_TREE;

  new_types->next = NULL;

  new_type = lang_hooks.optimize.build_data_struct ((void *) new_types, new_name, 
						    struct_data->decl);

  for (cur_child = child_types; cur_child; cur_child = cur_child->next)
    update_field_mappings (cur_child->data, new_type, struct_data);

  for (i = 0; i < struct_data->num_fields; i++)
    if (TEST_BIT (field_info->fields_in_cluster, i))
      {
	struct field_map *node;

	for (tmp_map = struct_data->fields[i].new_mapping;
	     tmp_map && tmp_map->containing_type;
	     tmp_map = tmp_map->containing_type);

	node = (struct field_map *) xmalloc (sizeof (struct field_map));
	node->decl = new_type;
	node->containing_type = NULL;
	node->contains = tmp_map;
	if (tmp_map)
	  tmp_map->containing_type = node;
	else
	  struct_data->fields[i].new_mapping = node;
      }

  tmp_node = (struct struct_tree_list *) xmalloc (sizeof 
						  (struct struct_tree_list));

  tmp_node->data = new_type;
  tmp_node->next = NULL;

  add_to_types_list (tmp_node, &return_list);

  return return_list;
}

/* This function takes a list of newly created variables (NEW_VARS) and
   a tree_list of existing variables, and adds the new variables to the
   list.  */

static tree
insert_into_var_list (struct struct_tree_list *new_vars, tree var_list,
		      tree current_var_decl ATTRIBUTE_UNUSED)
{
  tree tmp;
  tree new_tree;
  tree var_decl;
  struct struct_tree_list *current;

  tmp = TREE_CHAIN (var_list);
  for (current = new_vars; current; current = current->next)
    {
      var_decl = current->data;
      new_tree = copy_node (var_list);
      TREE_VALUE (new_tree) = var_decl;
      TREE_CHAIN (var_list) = new_tree;
      var_list = new_tree;
    }
  TREE_CHAIN (var_list) = tmp;

  return var_list;
}

/* This function takes a list of newly created variables (NEW_VARS) and
   a tree_list of existing variables, and adds the new variables to the
   list.  */

static struct struct_tree_list *
make_new_vars_1 (tree orig_decl, struct data_structure *struct_data)
{
  struct struct_tree_list *new_vars = NULL;
  struct struct_tree_list *tmp_node;
  struct struct_tree_list *new_types = struct_data->new_types;
  struct struct_tree_list *current;
  bool pointer_type_p = false;
  bool array_type_p = false;
  tree pointer_type;
  tree array_type;
  int i;

  if (POINTER_TYPE_P (TREE_TYPE (orig_decl)))
    pointer_type_p = true;
  else if (TREE_CODE (TREE_TYPE (orig_decl)) == ARRAY_TYPE)
    array_type_p = true;

  /* For each new type...  */

  for (current = new_types, i = 0; current; current = current->next, i++)
    {
      tree new_type = current->data;
      tree new_decl;
      tree id_node;
      char *new_name;
      char *old_name;
      int len;

      /* Make a copy of the original variable decl.  */

      new_decl = copy_node (orig_decl);
      new_decl->decl.rtl = NULL;
      DECL_CONTEXT (new_decl) = DECL_CONTEXT (orig_decl);

      /* IFF the original variable decl has a name, create an appropriate new
	 name for the new decl and attach it to the new decl.  */

      if (DECL_NAME (orig_decl)
	  && IDENTIFIER_POINTER (DECL_NAME (orig_decl)))
	{
	  int counter = 0;
	  int new_len;
	  char *tmp_name;
	  old_name = (char *) IDENTIFIER_POINTER (DECL_NAME (orig_decl));
	  len = strlen (old_name) + 6;
	  new_name = (char *) xmalloc (len * sizeof (char));
	  sprintf (new_name, "%s_%d", old_name, i);
	  
	  /* Make sure there isn't anything else that already has that 
	     name.  */
      
	  new_len = strlen (new_name) + 5;
	  tmp_name = (char *) xmalloc (new_len * sizeof (char));
	  sprintf (tmp_name, "%s", new_name);

	  while (maybe_get_identifier (tmp_name))
	    sprintf (tmp_name, "%s.%d", new_name, counter++);
	  new_name = tmp_name;

	  id_node = get_identifier (new_name);
	  DECL_NAME (new_decl) = id_node;
	}
      else
	DECL_NAME (new_decl) = NULL;

      /* Create the appropriate type for the new variable decl, and attach
         it.  */

      if (pointer_type_p)
	{
	  pointer_type = build_pointer_type (new_type);
	  TREE_TYPE (new_decl) = pointer_type;
	}
      else if (array_type_p)
	{
	  array_type = build_array_type (new_type, 
					 TYPE_DOMAIN (TREE_TYPE (orig_decl)));
	  TREE_TYPE (new_decl) = array_type;
	}
      else
	TREE_TYPE (new_decl) = new_type;
      
      /* Stuff the new var decl into a struct_tree_list node, and append it
	 to the front of the list created so far.  */
      
      tmp_node = (struct struct_tree_list *) xmalloc (sizeof 
						   (struct struct_tree_list));
      tmp_node->data = new_decl;
      tmp_node->next = new_vars;
      new_vars = tmp_node;
    }

  /* Return the list of new var decls.  */

  return new_vars;
}

/* This function takes two lists of new_var_data (newly created variable
   decls), prepends NEW_NODE to OLD_LIST, then updates OLD_LIST to be
   the newly combined list.  */

static void
add_to_vars_list (struct new_var_data *new_node,
		  struct new_var_data **old_list)
{
  struct new_var_data *current;

  if (!new_node)
    return;
  else if (! (*old_list))
    *old_list = new_node;
  else
    {
      for (current = new_node; current && current->next; 
	   current = current->next);
      if (current && !current->next)
	{
	  current->next = *old_list;
	  *old_list = new_node;
	}
    }
}

static tree
build_new_type (struct struct_tree_list *type_nest, tree new_record)
{
  tree new_type;
  tree tmp_type;
  struct struct_tree_list *current;

  new_type = new_record;
  for (current = type_nest; current; current = current->next)
    {
      tmp_type = new_type;
      if (TREE_CODE (current->data) == POINTER_TYPE)
	new_type = build_pointer_type (tmp_type);
      else if (TREE_CODE (current->data) == REFERENCE_TYPE)
	new_type = build_reference_type (tmp_type);
      else if (TREE_CODE (current->data) == ARRAY_TYPE)
	{
	  tree index_type = TYPE_DOMAIN (current->data);
	  new_type = build_array_type (tmp_type, index_type);
	}
      else
	abort ();
    }
  
  return new_type;
}

static void
replace_var_types (tree old_record, tree new_record)
{
  struct cgraph_varpool_node *current_varpool;
  struct cgraph_varpool_node *first_new_var = NULL;
  struct cgraph_node *c_node;
  tree var_decl;
  tree var_type;
  tree var_list;
  tree new_type;
  struct struct_tree_list *type_nest;
  struct struct_tree_list *tmp_node;
  struct struct_tree_list *donelist = NULL;

  /* Go through global variables... */

  for (current_varpool = cgraph_varpool_nodes_queue;
       current_varpool && (current_varpool != first_new_var);
       current_varpool = current_varpool->next)
    {
      var_decl = current_varpool->decl;
      if (!var_decl || TREE_CODE (var_decl) != VAR_DECL)
	continue;

      var_type = TREE_TYPE (var_decl);

      type_nest = NULL;
      while (POINTER_TYPE_P (var_type)
	     || TREE_CODE (var_type) == ARRAY_TYPE)
	{
	  tmp_node = (struct struct_tree_list *) xmalloc 
	                                          (sizeof 
						   (struct struct_tree_list));
	  tmp_node->data = var_type;
	  tmp_node->next = type_nest;
	  type_nest = tmp_node;
	  var_type = TREE_TYPE (var_type);
	}

      if (similar_struct_decls_p (var_type, old_record))
	{
	  new_type = build_new_type (type_nest, new_record);
	  TREE_TYPE (var_decl) = new_type;
	}
      else if (TREE_CODE (var_type) == RECORD_TYPE)
	{
	  
	  tmp_node = (struct struct_tree_list *) xmalloc 
	                                          (sizeof 
						   (struct struct_tree_list));
	  tmp_node->data = var_type;
	  tmp_node->next = NULL;
	  replace_old_field_types (tmp_node, new_record, old_record,
				   &donelist);
	}
    }

  /* For each function in the call graph... */

  for (c_node = cgraph_nodes; c_node; c_node = c_node->next)
    {
      struct function *func = DECL_STRUCT_FUNCTION (c_node->decl);

      /* Check the type of the function */

      var_type = TREE_TYPE (TREE_TYPE (c_node->decl));

      type_nest = NULL;
      while (POINTER_TYPE_P (var_type)
	     || TREE_CODE (var_type) == ARRAY_TYPE)
	{
	  tmp_node = (struct struct_tree_list *) xmalloc 
		                                  (sizeof 
						   (struct struct_tree_list));
	  tmp_node->data = var_type;
	  tmp_node->next = type_nest;
	  type_nest = tmp_node;
	  var_type = TREE_TYPE (var_type);
	}
	  
      if (similar_struct_decls_p (var_type, old_record))
	{
	  new_type = build_new_type (type_nest, new_record);
	  TREE_TYPE (TREE_TYPE (c_node->decl)) = new_type;
	}

      /* Check the return type of the function */

      if (DECL_RESULT (c_node->decl))
	{
	  var_type = TREE_TYPE (DECL_RESULT (c_node->decl));

	  type_nest = NULL;
	  while (POINTER_TYPE_P (var_type)
		 || TREE_CODE (var_type) == ARRAY_TYPE)
	    {
	      tmp_node = (struct struct_tree_list *) xmalloc 
		                                      (sizeof 
						       (struct struct_tree_list));
	      tmp_node->data = var_type;
	      tmp_node->next = type_nest;
	      type_nest = tmp_node;
	      var_type = TREE_TYPE (var_type);
	    }
	  
	  if (similar_struct_decls_p (var_type, old_record))
	    {
	      new_type = build_new_type (type_nest, new_record);
	      TREE_TYPE (DECL_RESULT (c_node->decl)) = new_type;
	    }
	}
      
      
      /* Go through the arguments for each function...  */

      for (var_decl = DECL_ARGUMENTS (c_node->decl); var_decl;
	   var_decl = TREE_CHAIN (var_decl))
	{
	  var_type = TREE_TYPE (var_decl);

	  type_nest = NULL;
	  while (POINTER_TYPE_P (var_type)
		 || TREE_CODE (var_type) == ARRAY_TYPE)
	    {
	      tmp_node = (struct struct_tree_list *) xmalloc 
	                                          (sizeof 
						   (struct struct_tree_list));
	      tmp_node->data = var_type;
	      tmp_node->next = type_nest;
	      type_nest = tmp_node;
	      var_type = TREE_TYPE (var_type);
	    }

	  if (similar_struct_decls_p (var_type, old_record))
	    {
	      new_type = build_new_type (type_nest, new_record);
	      TREE_TYPE (var_decl) = new_type;
	    }
	  else if (TREE_CODE (var_type) == RECORD_TYPE)
	    {
	      tmp_node = (struct struct_tree_list *) xmalloc 
	                                          (sizeof 
						   (struct struct_tree_list));
	      tmp_node->data = var_type;
	      tmp_node->next = NULL;
	      replace_old_field_types (tmp_node, new_record, old_record,
				       &donelist);
	    }
	}

      if (! func)
	continue;

      /* If the function has a body, go through the local variables for
	 the function...  */

      for (var_list = func->unexpanded_var_list; var_list;
	   var_list = TREE_CHAIN (var_list))
	{
	  var_decl = TREE_VALUE (var_list);
	  if (TREE_CODE (var_decl) == VAR_DECL)
	    {
	      var_type = TREE_TYPE (var_decl);

	      type_nest = NULL;
	      while (POINTER_TYPE_P (var_type)
		     || TREE_CODE (var_type) == ARRAY_TYPE)
		{
		  tmp_node = (struct struct_tree_list *) xmalloc 
	                                          (sizeof 
						   (struct struct_tree_list));
		  tmp_node->data = var_type;
		  tmp_node->next = type_nest;
		  type_nest = tmp_node;
		  var_type = TREE_TYPE (var_type);
		}

	      if (similar_struct_decls_p (var_type, old_record))
		{
		  new_type = build_new_type (type_nest, new_record);
		  TREE_TYPE (var_decl) = new_type;
		}
	      else if (TREE_CODE (var_type) == RECORD_TYPE)
		{
		  tmp_node = (struct struct_tree_list *) xmalloc 
	                                          (sizeof 
						   (struct struct_tree_list));
		  tmp_node->data = var_type;
		  tmp_node->next = NULL;
		  replace_old_field_types (tmp_node, new_record, old_record,
					   &donelist);
		}
	    }
	}
    }
}

/* This function finds every global or local variable or parameter of the
   type described in STRUCT_DATA (the struct type we are peeling), and 
   for each original variable found, creates a set of new variables, one for
   each newly peeled struct type.  It also puts the newly created variables
   into the appropriate list (global variables vs. local variables vs. local 
   parameters).  */

static struct new_var_data *
create_new_var_decls (struct data_structure *struct_data)
{
  struct struct_tree_list *current;
  struct struct_tree_list *new_vars_list;
  struct new_var_data *return_list = NULL;
  struct new_var_data *tmp_node;
  struct cgraph_varpool_node *current_varpool;
  struct cgraph_varpool_node *first_new_var = NULL;
  struct cgraph_node *c_node;
  tree var_decl;
  tree var_type;
  tree var_list;
  tree tmp;
  struct struct_tree_list *donelist = NULL;

  /* Go through global variables,,,  */

  for (current_varpool = cgraph_varpool_nodes_queue; 
       current_varpool && (current_varpool != first_new_var);
       current_varpool = current_varpool->next_needed)
    {
      var_decl = current_varpool->decl;
      if (!var_decl || TREE_CODE (var_decl) != VAR_DECL)
	continue;

      var_type = TREE_TYPE (var_decl);

      while (POINTER_TYPE_P (var_type)
	     || TREE_CODE (var_type) == ARRAY_TYPE)
	  var_type = TREE_TYPE (var_type);
      
      if (similar_struct_decls_p (var_type, struct_data->decl))
	{
	  struct cgraph_varpool_node *new_node;
	  struct struct_tree_list *current;

	  new_vars_list = make_new_vars_1 (var_decl, struct_data);
	  tmp_node = (struct new_var_data *) xmalloc (sizeof 
						      (struct new_var_data));
	  tmp_node->orig_var = var_decl;
	  tmp_node->new_vars = new_vars_list;
	  tmp_node->next = NULL;
	  add_to_vars_list (tmp_node, &return_list);

	  /* Add the newly created global variahles (NEW_VARS_LIST)
	     to the cgraph varpool list at its end.  */
	  for (current = new_vars_list; current; current = current->next)
	    {
	      tree v_decl = current->data;

	      new_node = cgraph_varpool_node (v_decl);
	      notice_global_symbol (v_decl);
	      cgraph_varpool_mark_needed_node (new_node);
	      cgraph_varpool_finalize_decl (v_decl);
	      if (! first_new_var)
		first_new_var = new_node;
	    }
	}
      else if (TREE_CODE (var_type) == RECORD_TYPE)
	{
	  struct struct_tree_list *tmp_node;
	  tmp_node = (struct struct_tree_list *) xmalloc (sizeof 
	                                               (struct struct_tree_list));
	  tmp_node->data = var_type;
	  tmp_node->next = NULL;
	  replace_old_field_types (tmp_node, struct_data->new_types->data,
				   struct_data->decl, &donelist);
	}
    }

  /* For each function in the call graph...  */

  for (c_node = cgraph_nodes; c_node; c_node = c_node->next)
    {
      struct function *func = DECL_STRUCT_FUNCTION (c_node->decl);

      /* Go through the arguments for each function... */

      for (var_decl = DECL_ARGUMENTS (c_node->decl); var_decl;
	   var_decl = TREE_CHAIN (var_decl))
	{
	  var_type = TREE_TYPE (var_decl);
	  
	  while (POINTER_TYPE_P (var_type)
		 || TREE_CODE (var_type) == ARRAY_TYPE)
	    var_type = TREE_TYPE (var_type);
	  
	  if (similar_struct_decls_p (var_type, struct_data->decl))
	    {
	      new_vars_list = make_new_vars_1 (var_decl, struct_data);
	      tmp_node = (struct new_var_data *) xmalloc (sizeof 
						       (struct new_var_data));
	      tmp_node->orig_var = var_decl;
	      tmp_node->new_vars = new_vars_list;
	      tmp_node->next = NULL;
	      add_to_vars_list (tmp_node, &return_list);
	      tmp = TREE_CHAIN (var_decl);
	      for (current = new_vars_list; current; current = current->next)
		{
		  TREE_CHAIN (var_decl) = current->data;
		  var_decl = current->data;
		}
	      TREE_CHAIN (var_decl) = tmp;
	    }
	  else if (TREE_CODE (var_type) == RECORD_TYPE)
	    {
	      struct struct_tree_list * tmp_node;
	      tmp_node = (struct struct_tree_list *) xmalloc (sizeof 
                                                       (struct struct_tree_list));
	      tmp_node->data = var_type;
	      tmp_node->next = NULL;
	      replace_old_field_types (tmp_node, struct_data->new_types->data,
				       struct_data->decl, &donelist);
	    }
	}

      if (! func)
	continue;

      /* If the function has a body, go through the local variables for
	 the function... */

      for (var_list = func->unexpanded_var_list; var_list;
	   var_list = TREE_CHAIN (var_list))
	{
	  var_decl = TREE_VALUE (var_list);
	  if (TREE_CODE (var_decl) == VAR_DECL)
	    {
	      var_type = TREE_TYPE (var_decl);

	      while (POINTER_TYPE_P (var_type)
		     || TREE_CODE (var_type) == ARRAY_TYPE)
		var_type = TREE_TYPE (var_type);
      
	      if (similar_struct_decls_p (var_type, struct_data->decl))
		{
		  new_vars_list = make_new_vars_1 (var_decl, struct_data);
		  tmp_node = (struct new_var_data *) xmalloc (sizeof 
						       (struct new_var_data));
		  tmp_node->orig_var = var_decl;
		  tmp_node->new_vars = new_vars_list;
		  tmp_node->next = NULL;
		  add_to_vars_list (tmp_node, &return_list);
		  var_list = insert_into_var_list (new_vars_list, var_list, 
						   var_decl);
		}
	      else if (TREE_CODE (var_type) == RECORD_TYPE)
		{
		  struct struct_tree_list *tmp_node;
		  tmp_node = (struct struct_tree_list *) xmalloc (sizeof 
                                                       (struct struct_tree_list));
		  tmp_node->data = var_type;
		  tmp_node->next = NULL;
		  replace_old_field_types (tmp_node, 
					   struct_data->new_types->data,
					   struct_data->decl, &donelist);
		}
	    }
	}
    }
  
  /* Return a list of all newly created variables.  */
  
  return return_list;
}

/* The front end of the compiler, when parsing statements of the form:

   var = (type_cast) malloc (sizeof (type));
   
   always converts this single statement into the following statements
   (GIMPLE form):

   T.1 = sizeof (type);
   T.2 = malloc (T.1);
   T.3 = (type_cast) T.2;
   var = T.3;

   Since we need to create new malloc statements and modify the original
   statements somewhat, we need to find all four of the above statements.
   Currently record_call_1 (called for building cgraph edges) finds and
   records the statements containing the actual call to malloc, but we
   need to find the rest of the variables/statements on our own.  That
   is what the following function does.  */

static void
collect_malloc_data (void)
{
  struct struct_tree_list *current;
  struct malloc_struct *malloc_node;
  struct malloc_struct *cur_malloc;
  int max_mallocs = 0;;

  for (current = struct_reorg_malloc_list; current; current = current->next)
    max_mallocs++;

  /* struct_reorg_malloc_list is a list of every assignment statement in
     the program that has a call to malloc, calloc or xmalloc on the 
     right-hand-side.  We need to go through these assignment statements,
     organize them by containing function, and for each function make a list
     of variables we need to look for.  Then we will need to walk through 
     the code in each function... */

  for (current = struct_reorg_malloc_list; current;  current = current->next)
    {
      tree stmt = current->data;
      tree size_var = NULL_TREE;
      tree result_var;
      tree fn_decl;
      tree malloc_fn_decl;
      tree tmp;
      tree arg1;
      tree arg2;
      tree num_elts = NULL_TREE;
      tree ptr_var = NULL_TREE;
      bool is_malloc = false;
      bool is_calloc = false;
      bool is_realloc = false;

      if (TREE_CODE (stmt) != MODIFY_EXPR)
	fatal_error ("Call to malloc not part of an assignment.");

      result_var = TREE_OPERAND (stmt, 0);

      /* The assumption below is that the result of malloc is always 
	 assigned to a (temporary) local variable (of type void *). 
	 This is what the front end of the compiler does when parsing
	 malloc calls and putting out gimple.  */

      if (!DECL_CONTEXT (result_var))
	fatal_error ("Call to malloc outside of any function.");

      fn_decl = DECL_CONTEXT (result_var);

      tmp = TREE_OPERAND (stmt, 1);

      malloc_fn_decl = TREE_OPERAND (tmp, 0);
      if (TREE_CODE (malloc_fn_decl) != ADDR_EXPR)
	fatal_error ("Expected ADDR_EXPR in function call; found something else.");

      malloc_fn_decl = TREE_OPERAND (malloc_fn_decl, 0);

      if (TREE_CODE (malloc_fn_decl) != FUNCTION_DECL)
	fatal_error ("Expected FUNCTION_DECL, found something else.");

      if ((strcmp (IDENTIFIER_POINTER (DECL_NAME (malloc_fn_decl)),
		  "malloc") == 0)
	  || (strcmp (IDENTIFIER_POINTER (DECL_NAME (malloc_fn_decl)),
		      "xmalloc") == 0))
	is_malloc = true;
      else if (strcmp (IDENTIFIER_POINTER (DECL_NAME (malloc_fn_decl)),
		       "calloc") == 0)
	is_calloc = true;
      else if (strcmp (IDENTIFIER_POINTER (DECL_NAME (malloc_fn_decl)),
		       "realloc") == 0)
	is_realloc = true;

      if (!is_malloc && !is_calloc && !is_realloc)
	fatal_error ("Unidentified version of MALLOC used.");
      
      if (TREE_CODE (tmp) == CALL_EXPR)
	{
	  tmp = TREE_OPERAND (tmp, 1);  /* Get the argument. */
	  arg1 = TREE_VALUE (tmp);  /* Arguments are in a TREE_LIST.  */
	  if (is_malloc)
	    size_var = arg1;
	  else if (is_calloc)
	    {
	      num_elts = arg1;
	      tmp = TREE_CHAIN (tmp);
	      arg2 = TREE_VALUE (tmp);
	      size_var = arg2;
	    }
	  else if (is_realloc)
	    {
	      ptr_var = arg1;
	      tmp = TREE_CHAIN (tmp);
	      arg2 = TREE_VALUE (tmp);
	      size_var = arg2;
	    }
	}
      else
	fatal_error ("Was expecting a function call; found something else.");

      for (cur_malloc = malloc_data_list; cur_malloc; 
	   cur_malloc = cur_malloc->next)
	if (cur_malloc->context == fn_decl)
	  break;

      if (cur_malloc)
	{
	  int idx = cur_malloc->num_calls;
	  cur_malloc->malloc_list[idx].call_stmt = stmt;
	  cur_malloc->malloc_list[idx].size_var = size_var;
	  cur_malloc->malloc_list[idx].result_var = result_var;
	  if (TREE_CODE (size_var) != VAR_DECL)
	    cur_malloc->malloc_list[idx].malloc_size = size_var;
	  else
	    cur_malloc->malloc_list[idx].malloc_size = NULL_TREE;
	  cur_malloc->malloc_list[idx].malloc_type = NULL_TREE;
	  cur_malloc->malloc_list[idx].final_lhs = NULL_TREE;
	  cur_malloc->malloc_list[idx].num_elts = num_elts;
	  cur_malloc->malloc_list[idx].ptr_var = ptr_var;
	  cur_malloc->num_calls++;
	}
      else
	{
	  malloc_node = (struct malloc_struct *) xmalloc (sizeof 
						      (struct malloc_struct));
	  malloc_node->context = fn_decl;
	  malloc_node->num_calls = 1;

	  malloc_node->malloc_list = (struct malloc_call_data *) 
	                 xmalloc (max_mallocs 
				  * sizeof (struct malloc_call_data));
	  malloc_node->malloc_list[0].call_stmt = stmt;
	  malloc_node->malloc_list[0].size_var = size_var;
	  malloc_node->malloc_list[0].result_var = result_var;
	  if (TREE_CODE (size_var) != VAR_DECL)
	    malloc_node->malloc_list[0].malloc_size = size_var;
	  else
	    malloc_node->malloc_list[0].malloc_size = NULL_TREE;
	  malloc_node->malloc_list[0].malloc_type = NULL_TREE;
	  malloc_node->malloc_list[0].final_lhs = NULL_TREE;
	  malloc_node->malloc_list[0].num_elts = num_elts;
	  malloc_node->malloc_list[0].ptr_var = ptr_var;

	  malloc_node->next = malloc_data_list;
	  malloc_data_list = malloc_node;
	}

    }
}

/* The following function is called from walk_tree.  Once we have filled in
   the basic information directly obtainable for the "T4 = malloc (T3)"
   statements, this function goes through the rest of the code filling in
   the necessary blanks in the malloc_struct records, in malloc_data_list.
   This function assumes that collect_malloc_data has already been called
   to initialize malloc_data_list and fill in some of the  blanks.  */

static tree
record_malloc_1 (tree *tp, int *walk_subtrees, void *data)
{
  int i;
  tree t = *tp;
  struct malloc_struct *current;
  struct cgraph_node *c_node = (struct cgraph_node *) data;

  switch (TREE_CODE (t))
    {
    case MODIFY_EXPR:
      {
	tree lhs = TREE_OPERAND (t, 0);
	tree rhs = TREE_OPERAND (t, 1);

	if (TREE_CODE (rhs) == NOP_EXPR)
	  rhs = TREE_OPERAND (rhs, 0);

	for (current = malloc_data_list; current; current = current->next)
	  {
	    if (current->context == c_node->decl)
	      {
		for (i = 0; i < current->num_calls; i++)
		  {
		    if (current->malloc_list[i].size_var == lhs)
		      current->malloc_list[i].malloc_size = rhs;

		    if (current->malloc_list[i].result_var == rhs)
		      {
			current->malloc_list[i].malloc_type = TREE_TYPE (lhs);
			if (POINTER_TYPE_P 
			                (current->malloc_list[i].malloc_type))
			  current->malloc_list[i].malloc_type = 
			      TREE_TYPE (current->malloc_list[i].malloc_type);
			current->malloc_list[i].final_lhs = lhs;
		      }
		  }
	      }
	  }
	*walk_subtrees = 0;
	break;
      }
    case STATEMENT_LIST:
      {
	tree_stmt_iterator tsi;
	for (tsi = tsi_start (*tp); !tsi_end_p (tsi); tsi_next (&tsi))
	  walk_tree (tsi_stmt_ptr (tsi), record_malloc_1, data, 
		     visited_nodes);
	break;
      }
    default:
      if (DECL_P (*tp) || TYPE_P (*tp))
	{
	  *walk_subtrees = 0;
	  break;
	}

      if ((unsigned int) TREE_CODE (t) >= LAST_AND_UNUSED_TREE_CODE)
	return lang_hooks.callgraph.analyze_expr (tp, walk_subtrees, data);
      break;
    }
  
  return NULL;
}

/* Given a statement list (STMT_LIST), and a particular basic block into
   which the statement list is going to be inserted (BB), it updates the
   statement/bb bookkeeping appropriately.  */

static void
add_bb_info (basic_block bb, tree stmt_list)
{
  if (TREE_CODE (stmt_list) == STATEMENT_LIST)
    {
      tree_stmt_iterator tsi;
      for (tsi = tsi_start (stmt_list); !tsi_end_p (tsi); tsi_next (&tsi))
	{
	  tree stmt = tsi_stmt (tsi);

	  set_bb_for_stmt (stmt, bb);
	}
    }
}

/* This function takes a VAR_DECL for an original program variables, and
   looks through the list of newly created variables (NEW_VARS) to see if 
   we created any new variables based on it.  If so, return the record
   for these new variables.  */

static struct new_var_data *
find_var (tree var_decl, struct new_var_data *new_vars)
{
  struct new_var_data *current;
  
  for (current = new_vars; current; current = current->next)
    if (current->orig_var == var_decl)
      return current;

  return NULL;
}

/* Given a tree representation of an integer constant, return a boolean
   indicating if the constant value is zero or not.  */

static bool
is_constant_0 (tree t)
{
  if (TREE_CODE (t) == INTEGER_CST
      && TREE_INT_CST_HIGH (t) == 0
      && TREE_INT_CST_LOW (t) == 0)
    return true;
  else
    return false;
}

static void
add_field_mallocs (tree cur_lhs,
		   tree cur_struct,
		   struct data_structure *struct_data,
		   tree *new_mallocs_list,
		   struct malloc_struct *cur_malloc,
		   tree malloc_fn_decl)
{
  tree cur_field;
  tree new_struct_size;
  tree arg_list;
  tree call_expr;
  tree new_lhs;
  struct cgraph_node *c_node;
  struct cgraph_node *c_node2;

  for (cur_field = TYPE_FIELDS (cur_struct); cur_field; 
       cur_field = TREE_CHAIN (cur_field))
    {
      tree field_type = TREE_TYPE (cur_field);
      tree save_type = field_type;
      tree field_identifier = DECL_NAME (cur_field);

      while (POINTER_TYPE_P (field_type))
	field_type = TREE_TYPE (field_type);

      if (! struct_contains_field_p (struct_data->decl, cur_field))
	{
	  tree tmp_var1;
	  tree tmp_var2;
	  tree tmp_var3;
	  tree new_rhs;
	  tree call_stmt;
	  tree convert_stmt;
	  tree new_malloc_stmt;
	  tree assign_stmt;
	  tree void_pointer_type;

	  new_struct_size = lang_hooks.optimize.sizeof_type (field_type,
						      true, 0);
	  arg_list = build_tree_list (NULL_TREE, new_struct_size);
	  call_expr = build3 (CALL_EXPR, TREE_TYPE (TREE_TYPE (malloc_fn_decl)),
			      build1 (ADDR_EXPR,
				      build_pointer_type (TREE_TYPE (malloc_fn_decl)),
				      malloc_fn_decl),
			      arg_list,
			      NULL_TREE);

	  void_pointer_type = build_pointer_type (void_type_node);
	  tmp_var1 = create_tmp_var_raw (void_pointer_type, NULL);
	  gimple_add_tmp_var (tmp_var1);
	  call_stmt = build (MODIFY_EXPR, void_pointer_type,
			     tmp_var1,
			     call_expr);
	  append_to_statement_list (call_stmt, new_mallocs_list);

	  tmp_var2 = create_tmp_var_raw (save_type, NULL);
	  gimple_add_tmp_var (tmp_var2);
	  convert_stmt = build (MODIFY_EXPR, save_type,
				tmp_var2,
				build1 (CONVERT_EXPR, save_type, tmp_var1));

	  append_to_statement_list (convert_stmt, new_mallocs_list);

	  new_lhs = lang_hooks.optimize.build_field_reference (lang_hooks.optimize.build_pointer_ref (cur_lhs, ""),
					   field_identifier);
	  new_malloc_stmt = build (MODIFY_EXPR, TREE_TYPE (new_lhs),
				   new_lhs,
				   tmp_var2);

	  append_to_statement_list (new_malloc_stmt, new_mallocs_list);

	  new_rhs = lang_hooks.optimize.build_field_reference (lang_hooks.optimize.build_pointer_ref (cur_lhs, ""),
					   field_identifier);

	  tmp_var3 = create_tmp_var_raw (TREE_TYPE (new_rhs), NULL);
	  gimple_add_tmp_var (tmp_var3);
	  assign_stmt = build (MODIFY_EXPR, TREE_TYPE (new_rhs),
			       tmp_var3,
			       new_rhs);
	  append_to_statement_list (assign_stmt, new_mallocs_list);

	  c_node = cgraph_node (cur_malloc->context);
	  c_node2 = cgraph_node (malloc_fn_decl);
	  cgraph_create_edge (c_node, c_node2, assign_stmt, /*FKZ HACK*/0, 0);

	  add_field_mallocs (tmp_var3, field_type, struct_data, new_mallocs_list,
			     cur_malloc, malloc_fn_decl);
	  
	}
    }
}

static void
create_cascading_mallocs (struct malloc_struct *cur_malloc,
			  struct data_structure *struct_data,
			  struct new_var_data *cur_var,
			  tree malloc_fn_decl,
			  tree *new_mallocs_list)
{
  tree new_struct_type;
  tree new_struct_size;
  tree arg_list;
  tree call_expr;
  tree new_malloc_tree;
  struct cgraph_node *c_node;
  struct cgraph_node *c_node2;
  struct function *save_cfun;
  tree save_fn_decl;
  
  save_cfun = cfun;
  save_fn_decl = current_function_decl;

  if (!cur_var->new_vars
      || cur_var->new_vars->next)
    fatal_error ("Problem with cur_var.");

  current_function_decl = cur_malloc->context;
  cfun = DECL_STRUCT_FUNCTION (cur_malloc->context);

  /* Start with type of cur_var: "cur_new_type = TREE_TYPE (TREE_TYPE (cur_var))"

     Create a malloc for cur_new_type:
     Assign result of malloc to cur_new_var: 
                T1 = malloc (sizeof (cur_new_type));
		T2 = (struct cur_new_type *) T1;
		cur_var = T2; (???)
     For each field F in cur_new_type:
                if (TREE_TYPE (f) is in new_types list)
		       create_FIELD_malloc;
		cur_var->F = malloc_result.
  */

  new_struct_type = TREE_TYPE (cur_var->new_vars->data);
  while (POINTER_TYPE_P (new_struct_type))
    new_struct_type = TREE_TYPE (new_struct_type);

  new_struct_size = lang_hooks.optimize.sizeof_type (new_struct_type, 
						     true, 0);

  arg_list = build_tree_list (NULL_TREE, new_struct_size);
  call_expr = build3 (CALL_EXPR, TREE_TYPE (TREE_TYPE (malloc_fn_decl)),
		      build1 (ADDR_EXPR, 
			      build_pointer_type (TREE_TYPE (malloc_fn_decl)),
			      malloc_fn_decl),
		      arg_list,
		      NULL_TREE);

  new_malloc_tree = build (MODIFY_EXPR, TREE_TYPE (cur_var->new_vars->data),
			   cur_var->new_vars->data,
			   build1 (NOP_EXPR, TREE_TYPE (cur_var->new_vars->data), 
				   call_expr));

  append_to_statement_list (new_malloc_tree, new_mallocs_list);

  c_node = cgraph_node (cur_malloc->context);
  c_node2 = cgraph_node (malloc_fn_decl);
  cgraph_create_edge (c_node, c_node2, new_malloc_tree, /*FKZ HACK*/0, 0);

  add_field_mallocs (cur_var->new_vars->data, new_struct_type, struct_data,
		     new_mallocs_list, cur_malloc, malloc_fn_decl);

  cfun = save_cfun;
  current_function_decl = save_fn_decl;
}

/* This function replaces each malloc of a struct we are peeling with
   a series of appropriately changed mallocs for the newly peeled
   substructs, assigning the new malloc results to the appropriate newly
   created variables.  */

static void
create_new_mallocs (struct struct_list *data_struct_list,
		    struct new_var_data *new_vars)
{
  struct malloc_struct *cur_malloc;
  struct struct_list *cur_struct;
  struct struct_tree_list *cur_tree;
  struct new_var_data *cur_var;
  tree malloc_fn_decl = NULL_TREE;
  tree new_mallocs_list = NULL_TREE;
  tree tmp_tree;
  tree rhs;
  struct cgraph_node *c_node;
  struct cgraph_node *c_node_2;
  tree call_expr;

  /* Loop through each function (context) that contains one or more calls
     to malloc.  */
      
  for (cur_malloc = malloc_data_list; cur_malloc; 
       cur_malloc = cur_malloc->next)
    {
      int i;

      /* Create a context within the current function for gimplification.  */

      push_gimplify_context ();

      /* Look at each malloc site within the current context, one at at
	 time.  */

      for (i = 0; i < cur_malloc->num_calls; i++)
	{
	  tree old_var = cur_malloc->malloc_list[i].final_lhs;

	  /* Find the function decl for malloc.  Pull it out of the
	     call statement.  This assumes the call statement has a
	     very particular structure; the code tests this assumption
	     and fails if it is false.  */
      
	  tmp_tree = cur_malloc->malloc_list[i].call_stmt;
      
	  if (TREE_CODE (tmp_tree) != MODIFY_EXPR)
	    fatal_error ("Expected MODIFY_EXPR");
	  
	  rhs = TREE_OPERAND (tmp_tree, 1);
	  while (TREE_CODE (rhs) == NOP_EXPR)
	    rhs = TREE_OPERAND (rhs, 0);
	  
	  if (TREE_CODE (rhs) != CALL_EXPR)
	    fatal_error ("Expected CALL_EXPR");
	  else
	    tmp_tree = TREE_OPERAND (rhs, 0);
	  
	  if (TREE_CODE (tmp_tree) != ADDR_EXPR)
	    fatal_error ("Expected ADDR_EXPR");
	  else
	    tmp_tree = TREE_OPERAND (tmp_tree, 0);
	  
	  if (TREE_CODE (tmp_tree) != FUNCTION_DECL)
	    fatal_error ("Expected FUNCTION_DECL");
	  else
	    {
	      char *fn_name = (char *) IDENTIFIER_POINTER (DECL_NAME (tmp_tree));
	      
	      if (strcmp (fn_name,  "malloc") != 0
		  && strcmp (fn_name, "calloc") != 0
		  && strcmp (fn_name, "realloc") != 0
		  && strcmp (fn_name, "xmalloc") != 0)
		fatal_error ("Expected malloc or calloc function");
	      
	      malloc_fn_decl = tmp_tree;
	    }

	  /* Check newly created variables against the lhs of the malloc
	     assignment to see if we need to create a new malloc for the
	     variable (if they match).  */

	  for (cur_var = new_vars; cur_var; cur_var = cur_var->next)
	    if (cur_var->orig_var == old_var)
	      {
		for (cur_tree = cur_var->new_vars; cur_tree; 
		     cur_tree = cur_tree->next)
		  {
		    tree new_struct_type;
		    tree orig_struct_size;
		    tree new_struct_size;
		    tree old_malloc_size;
		    tree arg_list;
		    tree old_type;
		    tree new_var_decl = cur_tree->data;

		    if (cur_malloc->context != DECL_CONTEXT 
			                                  (cur_var->orig_var))
		      continue;

		    old_type = TREE_TYPE (cur_var->orig_var);
		    while (POINTER_TYPE_P (old_type))
		      old_type = TREE_TYPE (old_type);
		    
		    for (cur_struct = data_struct_list; cur_struct;
			 cur_struct = cur_struct->next)
		      {
			if (old_type == cur_struct->struct_data->decl)
			  {
			    struct data_structure *struct_data;
			    struct_data = cur_struct->struct_data;

			    if (struct_data->struct_clustering->direct_access)
			      {
				tree new_malloc_factor_var;
				tree new_malloc_size_var;
				tree new_malloc_result_var;
				tree div_stmt;
				tree mult_stmt;
				tree call_stmt;
				tree final_assignment_stmt;
				tree void_pointer_type = build_pointer_type 
                                                               (void_type_node);
				
				/* Create necessary new temporary variables.  */

				new_malloc_factor_var = create_tmp_var_raw (integer_type_node, NULL);
				new_malloc_size_var = create_tmp_var_raw (integer_type_node, NULL);
				new_malloc_result_var = create_tmp_var_raw (void_pointer_type, NULL);

			        gimple_add_tmp_var (new_malloc_factor_var);
				gimple_add_tmp_var (new_malloc_size_var);
				gimple_add_tmp_var (new_malloc_result_var);

				/* Get size for new malloc first:
			       
				    new_size = (orig_malloc_size 
				                 / orig_struct_size) 
						            * new_struct_size;
				  */
		    
				new_struct_type = TREE_TYPE (new_var_decl);
				while (POINTER_TYPE_P (new_struct_type))
				  new_struct_type = TREE_TYPE 
				                            (new_struct_type);
				
				orig_struct_size = lang_hooks.optimize.sizeof_type
		                                        (struct_data->decl,
							 true,
							 0);
				new_struct_size = lang_hooks.optimize.sizeof_type
			                                (new_struct_type,
							 true, 
							 0);
				old_malloc_size = 
				          cur_malloc->malloc_list[i].size_var;


				if (! new_mallocs_list)
				  new_mallocs_list = alloc_stmt_list ();
				
				div_stmt = build (MODIFY_EXPR, integer_type_node,
						  new_malloc_factor_var,
						  build (TRUNC_DIV_EXPR, 
							 integer_type_node,
							 old_malloc_size,
							 orig_struct_size));
				append_to_statement_list (div_stmt, 
							  &new_mallocs_list);
						 
				mult_stmt = build (MODIFY_EXPR, integer_type_node,
						   new_malloc_size_var,
						   build (MULT_EXPR, 
							  integer_type_node,
							  new_malloc_factor_var,
							  new_struct_size));

				append_to_statement_list (mult_stmt, 
							  &new_mallocs_list);

				/* Wrap the new malloc size expr up into a 
				   TREE_LIST (function arguments are always 
				   in TREE_LIST form).  */

				arg_list = build_tree_list (NULL_TREE, 
							    new_malloc_size_var);

				if (cur_malloc->malloc_list[i].num_elts)
				  arg_list = tree_cons (NULL_TREE, 
					      cur_malloc->malloc_list[i].num_elts,
							arg_list);
				else if (cur_malloc->malloc_list[i].ptr_var)
				  {
				    struct new_var_data *tmp_var;
				    struct struct_tree_list *tmp_var_2;
				    tree new_ptr_var = NULL_TREE;
				    for (tmp_var = new_vars; tmp_var; 
					 tmp_var = tmp_var->next)
				      if (tmp_var->orig_var == 
					  cur_malloc->malloc_list[i].ptr_var)
					{
					  for (tmp_var_2 = tmp_var->new_vars;
					       tmp_var_2 && !new_ptr_var; 
					       tmp_var_2 = tmp_var_2->next)
					    {
					      tree t = tmp_var_2->data;
					      tree tmp_type = TREE_TYPE (t);
					      while (POINTER_TYPE_P (tmp_type))
						tmp_type = TREE_TYPE (tmp_type);
					      if (similar_struct_decls_p (
								tmp_type,
								new_struct_type))
						new_ptr_var = t;
					    }
					}
				    arg_list = tree_cons (NULL_TREE,
							  new_ptr_var,
							  arg_list);
				  }

				call_expr =  build3 (CALL_EXPR,
						     TREE_TYPE 
						         (TREE_TYPE 
							    (malloc_fn_decl)),
						     build1 (ADDR_EXPR,
							   build_pointer_type 
							   (TREE_TYPE 
							    (malloc_fn_decl)),
							     malloc_fn_decl), 
						     arg_list,
						     NULL_TREE);
				
				call_stmt = build (MODIFY_EXPR,
						   TREE_TYPE (TREE_TYPE (malloc_fn_decl)),
						   new_malloc_result_var,
						   call_expr);
				append_to_statement_list (call_stmt, 
							  &new_mallocs_list);

				final_assignment_stmt = build (MODIFY_EXPR, 
							       TREE_TYPE 
							       (new_var_decl),
							       new_var_decl,
							       build1 (NOP_EXPR, 
							 TREE_TYPE (new_var_decl),
							  new_malloc_result_var));
				
				append_to_statement_list (final_assignment_stmt, 
							  &new_mallocs_list);
				
				c_node = cgraph_node (cur_malloc->context);
				c_node_2 = cgraph_node (malloc_fn_decl);
				cgraph_create_edge (c_node, c_node_2, 
						    call_stmt /*FKZ HACK*/, 
						    0, 0);
			      }
			    else
			      {
				/* NOT direct_access; need to malloc
				   sub-struct fields.  */		

				create_cascading_mallocs (cur_malloc,
							  cur_struct->struct_data,
							  cur_var,
							  malloc_fn_decl,
							  &new_mallocs_list);
			      }
			  }
		      }
		    
		  } /* end for each new variable we've created */

	      } /* end if cur_var == malloc final lhs var */

	  if (new_mallocs_list)
	    {
	      struct function *save_cfun = cfun;
	      tree save_fn_decl = current_function_decl;
	      basic_block bb;
	      block_stmt_iterator bsi;
	      tree_stmt_iterator *insertion_point;
	      
	      /* gimplify_to_stmt_list (&new_mallocs_list); */
	      
	      /* insert after current malloc stmt. */
	      
	      /* Find original malloc stmt (for insertion).  */
	      cfun = DECL_STRUCT_FUNCTION (cur_malloc->context);
	      current_function_decl = cur_malloc->context;

	      FOR_EACH_BB_FN (bb, cfun)
		{
		  bool malloc_found = false;
		  for (bsi= bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
		    {
		      if (bsi_stmt (bsi) 
			  == cur_malloc->malloc_list[i].call_stmt)
			{
			  malloc_found = true;
			  insertion_point = &(bsi.tsi);
			  add_bb_info (bb, new_mallocs_list);
			  tsi_link_after (insertion_point,
					  new_mallocs_list, 
					  TSI_CONTINUE_LINKING);
			}
		    }

		  /* Check to see if basic block ends with a test of malloc
		     result against NULL.  Add tests comparing new malloc
		     results against NULL (and fix up the cfg 
		     appropriately).  */

		  if (malloc_found)
		    {
		      tree last_stmt;
		      struct struct_tree_list *current;

		      bsi = bsi_last (bb);
		      last_stmt = bsi_stmt (bsi);
		      if (TREE_CODE (last_stmt) == COND_EXPR)
			{
			  tree condition = COND_EXPR_COND (last_stmt);
			  tree stmt_list = NULL_TREE;
			  bool first_stmt = true;
			  bool swapped = false;
			  edge_iterator ei;
			  edge succ1, succ2, e;
			  basic_block old_bb, new_bb;
			  tree new_label;
			  tree new_goto;
			  tree old_goto;
			  edge false_edge;
			  edge true_edge;
			  edge new_e1;
			  edge new_e2;

			  if (TREE_CODE (condition) == EQ_EXPR
			      || TREE_CODE (condition) == NE_EXPR)
			    {
			      struct new_var_data *cur_var0 = NULL;
			      struct new_var_data *cur_var1 = NULL;
			      tree arg0 = TREE_OPERAND (condition, 0);
			      tree arg1 = TREE_OPERAND (condition, 1);
			      bool null_test_p;

			      if (TREE_CODE (arg0) == VAR_DECL
				  || TREE_CODE (arg0) == PARM_DECL)
				cur_var0 = find_var (arg0, new_vars);

			      if (TREE_CODE (arg1) == VAR_DECL
				  || TREE_CODE (arg1) == PARM_DECL)
				cur_var1 = find_var (arg1, new_vars);

			      if (!cur_var0 && cur_var1)
				{
				  cur_var0 = cur_var1;
				  cur_var1 = NULL;
				  swapped = true;
				  null_test_p = is_constant_0 (arg0);
				}
			      else
				null_test_p = is_constant_0 (arg1);

			      if (cur_var0
				  && null_test_p)
				for (current = cur_var0->new_vars; current;
				     current = current->next)
				  {
				    tree new_cond;
				    tree new_if;
				    tree new_else;
				    tree new_stmt;

				    new_if = copy_node (COND_EXPR_THEN 
							         (last_stmt));
				    new_else = copy_node (COND_EXPR_ELSE 
							         (last_stmt));
				    new_cond = copy_node (COND_EXPR_COND 
							         (last_stmt));

				    if (swapped)
				      TREE_OPERAND (new_cond, 1) = 
					                        current->data;
				    else
				      TREE_OPERAND (new_cond, 0) = 
					                        current->data;
				      
				    new_stmt = build3 (COND_EXPR,
						       TREE_TYPE (last_stmt),
						       new_cond,
						       new_if,
						       new_else);

				    if (first_stmt)
				      {
					tree *tp;

					/* REPLACE current cond expr with new
					   stmt.  */
					tp = bsi_stmt_ptr (bsi);
					*tp = new_stmt;
					set_bb_for_stmt (new_stmt, bb);
					last_stmt = bsi_stmt (bsi);
					first_stmt = false;
					
				      }
				    else
				      {
					/* Create a new basic block after 
					   old_bb.  */
					succ1 = NULL;
					succ2 = NULL;
					old_bb = bb;
					new_bb = create_basic_block (NULL,
								     NULL,
								     old_bb);

					/* Create new label stmt.  */

					new_label = build1 (LABEL_EXPR,
							    void_type_node,
							    NULL_TREE);
					stmt_list = new_bb->stmt_list;
					append_to_statement_list (new_label,
								  &stmt_list);
					new_goto = build_and_jump 
					  (&LABEL_EXPR_LABEL (new_label));

					if (TREE_CODE (condition) == NE_EXPR)
					  {
					    old_goto = COND_EXPR_THEN 
					                          (last_stmt);
					    COND_EXPR_THEN (last_stmt) = 
					                             new_goto;
					    COND_EXPR_THEN (new_stmt) = 
					                             old_goto;
					  }
					else
					  {
					    old_goto = COND_EXPR_ELSE 
					                          (last_stmt);
					    COND_EXPR_ELSE (last_stmt) = 
					                             new_goto;
					    COND_EXPR_ELSE (new_stmt) = 
					                             old_goto;
					  }

					append_to_statement_list (new_stmt,
								  &stmt_list);
					add_bb_info (new_bb, stmt_list);

					/* Fix up the edges of the newly
					   inserted basic block:  */

					/* Give new_bb the same successor
					   edges old_bb had.  */

					succ1 = NULL;
					succ2 = NULL;

					FOR_EACH_EDGE (e, ei, old_bb->succs)
					  if (!succ1)
					    succ1 = e;
					  else
					    succ2 = e;

					if (succ1->flags & EDGE_FALSE_VALUE)
					  {
					    false_edge = succ1;
					    true_edge = succ2;
					  }
					else if (succ2->flags 
						 & EDGE_FALSE_VALUE)
					  {
					    false_edge = succ2;
					    true_edge = succ1;
					  }
					else
					  fatal_error 
					    ("cond_expr missing false edge.");

					new_e1 = make_edge (new_bb,
							    succ1->dest,
							    succ1->flags);

					new_e2 = make_edge (new_bb,
							    succ2->dest,
							    succ2->flags);
					
					if (TREE_CODE (condition) == EQ_EXPR)
					  redirect_edge_succ (false_edge, 
							      new_bb);
					else
					  redirect_edge_succ (true_edge, 
							      new_bb);
					
				      }

				  }
			    }
			}
		    }
		}
	      
	      new_mallocs_list = NULL_TREE;
	      cfun = save_cfun;
	      current_function_decl = save_fn_decl;
	    }
	  
	} /* end for each malloc call site */
      
      if (!cfun)
	{
	  cfun = DECL_STRUCT_FUNCTION (cur_malloc->context);
	  pop_gimplify_context (NULL);
	  cfun = NULL;
	}
      else
	pop_gimplify_context (NULL);
      
    } /* end for each function containing a call to malloc */
  
} /* end function create_new_mallocs */

/* This function checks to see if tree RHS contains any of the original 
   variables of the a type listed in  DATA_STRUCT_LIST, for which we created 
   new variables. If so, it returns a list of each such original variable 
   found.  */

static struct struct_tree_list *
search_rhs_for_struct_vars (tree rhs, struct struct_list *data_struct_list,
			    struct new_var_data *new_vars)
{
  struct struct_tree_list *return_list = NULL;
  struct struct_tree_list *tmp_node;
  struct struct_tree_list *tmp_list;

  switch (TREE_CODE (rhs))
    {
    case PARM_DECL:
    case VAR_DECL:
      {
	struct new_var_data *current;
	bool found = false;

	for (current = new_vars; current && !found; current = current->next)
	  if (rhs == current->orig_var)
	    found = true;

	if (found)
	  {
	    tmp_node = (struct struct_tree_list *) xmalloc (sizeof 
						   (struct struct_tree_list));
	    tmp_node->data = rhs;
	    tmp_node->next = return_list;
	    return_list = tmp_node;
	  }

	break;
      }
    case COMPONENT_REF:
    case INDIRECT_REF:
    case FIELD_DECL:
      /*  These cases are taken care of separately, by 
	  update_field_accesses.  */
      break;
    default:
      {
	int i;

	for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (rhs)); i++)
	  {
	    tree t = TREE_OPERAND (rhs, i);
	    if (t)
	      {
		tmp_list = search_rhs_for_struct_vars (t, data_struct_list, 
						       new_vars);
		if (tmp_list)
		  add_to_types_list (tmp_list, &return_list);
	      }
	  }
	break;
      }
    }

  return return_list;
}

static bool
equivalent_types_p (tree type1, tree type2)
{
  bool ret_value = false;
  tree tt1;
  tree tt2;

  if (type1 == type2)
    ret_value = true;
  else
    {
      tt1 = type1;
      tt2 = type2;
      while (POINTER_TYPE_P (tt1)
	     || TREE_CODE (tt1) == ARRAY_TYPE)
	tt1 = TREE_TYPE (tt1);
      while (POINTER_TYPE_P (tt2)
	     || TREE_CODE (tt2) == ARRAY_TYPE)
	tt2 = TREE_TYPE (tt2);
      if (tt1 == tt2)
	ret_value = true;
      else
	ret_value = similar_struct_decls_p (tt1, tt2);
    }
  
  return ret_value;
}

/* Given and OLD_RHS of an OLD_TYPE,  start with a NEW_RHS that is
   a copy of the OLD_RHS, but then find the original var in for which
   we created new vars, and which corresponds to the OLD_RHS; replace 
   the old var with the appropriate newly created var (one whose type
   is NEW_TYPE).  Call self recursively on all tree_operands of NEW_RHS,
   to make sure we replace all variables of original tyep with appropriate
   new variables of appropriate new type.  */

static void
find_and_substitute_types_and_vars (tree old_type, tree new_type,
				    struct new_var_data *new_vars,
				    tree *new_rhs, tree old_rhs)
{
  int i;
  struct struct_tree_list *current;
  struct new_var_data *cur_var;
  bool found;
  tree tmp_type;

  if (! (*new_rhs))
    return;

  for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (*new_rhs)); i++)
    find_and_substitute_types_and_vars (old_type, new_type, new_vars,
					&TREE_OPERAND (*new_rhs, i), old_rhs);

  tmp_type = new_type;
  while (POINTER_TYPE_P (tmp_type)
	 || TREE_CODE (tmp_type) == ARRAY_TYPE)
    tmp_type = TREE_TYPE (tmp_type);

  switch (TREE_CODE (*new_rhs))
    {
    case CONVERT_EXPR:
    case NOP_EXPR:
      {
	if (TREE_TYPE (*new_rhs) == old_type)
	  TREE_TYPE (*new_rhs) = new_type;
	break;
      }
    case VAR_DECL:
    case PARM_DECL:
      {
	if (equivalent_types_p (TREE_TYPE (*new_rhs), old_type))
	  {
	    found  = false;
	    for (cur_var = new_vars; cur_var && !found; 
		 cur_var = cur_var->next)
	      if (cur_var->orig_var == *new_rhs
		  || cur_var->orig_var == old_rhs)
		{
		  for (current = cur_var->new_vars; current && !found;
		       current = current->next)
		    if (equivalent_types_p (TREE_TYPE (current->data), new_type)
			|| similar_struct_decls_p (TREE_TYPE (current->data),
						   tmp_type))
		      {
			*new_rhs = current->data;
			found = true;
		      }
		}
	  }
	break;
      }
    default:
      break;
    }
}

static tree
recursively_copy_node (tree old_node)
{
  tree new_node;
  int i = 0;

  new_node = copy_node (old_node);
  if (DECL_P (new_node))
    new_node->decl.rtl = NULL;

  for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (old_node)); i++)
    {
      tree new_op = NULL_TREE;
      tree op = TREE_OPERAND (old_node, i);
      if (op
	  && TREE_CODE_LENGTH (TREE_CODE (op)) > 0)
	{
	  new_op = recursively_copy_node (op);
	  TREE_OPERAND (new_node, i) = new_op;
	}
    }
  return new_node;
}

/* This function, given an LHS and a tree representing a program stmt
   (STMT), will create a series of new assignment statements.  The rhs
   of the new assignment statements will be copies of the rhs of STMT,
   but with appropriate newly created variables substituted for any
   variables in the rhs that were of the original struct type.  */

static tree
build_new_stmts (tree lhs, struct new_var_data *new_vars, tree stmt,
		 tree fn_decl, struct struct_list *data_struct_list)
{
  tree new_stmt_list = NULL_TREE;
  tree tmp_lhs;
  struct struct_tree_list *cur_lhs;
  struct new_var_data *cur_var;
  struct function *save_cfun;
  tree save_fn_decl;

  save_cfun = cfun;
  save_fn_decl = current_function_decl;

  current_function_decl = fn_decl;
  cfun = DECL_STRUCT_FUNCTION (fn_decl);

  if (TREE_CODE (lhs) == COMPONENT_REF)
    {
      tmp_lhs = TREE_OPERAND (lhs, 0);
      if (TREE_CODE (tmp_lhs) != VAR_DECL
	  && TREE_CODE (tmp_lhs) != PARM_DECL)
	tmp_lhs = TREE_OPERAND (tmp_lhs, 0);
    }
  else if (TREE_CODE (lhs) == INDIRECT_REF
	   || TREE_CODE (lhs) == ARRAY_REF)
    tmp_lhs = TREE_OPERAND (lhs, 0);
  else
    tmp_lhs = lhs;

  /* Find the lhs var in the new_vars list */

  for (cur_var = new_vars; cur_var && cur_var->orig_var != tmp_lhs;
       cur_var = cur_var->next);
  
  if (!cur_var)
    {
      struct struct_tree_list *rhs_vars;
      struct struct_tree_list *current;
      struct struct_tree_list *current2;
      struct new_var_data *cur_rhs_var;
      tree old_var;
      tree old_rhs = TREE_OPERAND (stmt, 1);

      rhs_vars = search_rhs_for_struct_vars (old_rhs, data_struct_list,
					     new_vars);

      if (!rhs_vars)
	fatal_error ("Can't find correct number of structs in rhs.");
			
      for (current2 = rhs_vars; current2; current2 = current2->next)
	{
	  for (cur_rhs_var = new_vars; 
	       cur_rhs_var && cur_rhs_var->orig_var != current2->data;
	       cur_rhs_var = cur_rhs_var->next);

	  if (!cur_rhs_var)
	    fatal_error ("Bad stmt passed to build_new_stmts.");

	  old_var = cur_rhs_var->orig_var;
	  for (current = cur_rhs_var->new_vars; current; current = current->next)
	    {
	      tree new_stmt;
	      tree new_lhs;
	      tree new_rhs;
	      tree new_var = current->data;
	      tree old_type = TREE_TYPE (old_var);
	      tree new_type = TREE_TYPE (new_var);
	      
	      new_lhs = create_tmp_var_raw (new_type, NULL);
	      gimple_add_tmp_var (new_lhs);
	      new_rhs = copy_node (old_rhs);
	      if (DECL_P (new_rhs))
		new_rhs->decl.rtl = NULL;
	      
	      find_and_substitute_types_and_vars (old_type, new_type, new_vars,
						  &new_rhs, old_rhs);
	      new_stmt = build (MODIFY_EXPR, TREE_TYPE (new_lhs),
				new_lhs, new_rhs);
	      if (!new_stmt_list)
		new_stmt_list = alloc_stmt_list ();
	      append_to_statement_list (new_stmt, &new_stmt_list);
	    }
	}
    }
  else
    for (cur_lhs = cur_var->new_vars; cur_lhs; cur_lhs = cur_lhs->next)
      {
	tree new_stmt;
	tree new_var_decl = cur_lhs->data;
	tree new_type = TREE_TYPE (new_var_decl);
	tree old_type = TREE_TYPE (cur_var->orig_var);
	tree old_rhs = TREE_OPERAND (stmt, 1);
	tree new_rhs = recursively_copy_node (old_rhs);
	tree new_lhs;
	
	if (lhs == tmp_lhs)
	  new_lhs = new_var_decl;
	else if (TREE_CODE (lhs) == INDIRECT_REF)
	  new_lhs = lang_hooks.optimize.build_pointer_ref (cur_lhs->data, "");
	else if (TREE_CODE (lhs) == ARRAY_REF)
	  {
	    new_lhs = copy_node (lhs);

	    if (DECL_P (new_lhs))
	      new_lhs->decl.rtl = NULL;

	    find_and_substitute_types_and_vars (old_type, new_type, new_vars,
						&new_lhs, lhs);
	  }
	else
	  {
	    tree field_identifier = TREE_OPERAND (lhs, 1);
	    
	    if (TREE_CODE (field_identifier) == INDIRECT_REF)
	      field_identifier = TREE_OPERAND (field_identifier, 0);
	    
	    if (TREE_CODE (field_identifier) != FIELD_DECL)
	      fatal_error ("Expected FIELD_DECL; found something else.");
	    
	    field_identifier = DECL_NAME (field_identifier);

	    if (lang_hooks.optimize.lookup_field (TREE_TYPE (cur_lhs->data), field_identifier))
	      new_lhs = lang_hooks.optimize.build_field_reference (lang_hooks.optimize.build_pointer_ref (cur_lhs->data, 
								   ""),
					       field_identifier);
	    else
	      {
		new_lhs = lhs;
	      }
	  }
	
	if (DECL_P (new_rhs))
	  new_rhs->decl.rtl = NULL;
	
	find_and_substitute_types_and_vars (old_type, new_type, new_vars, 
					    &new_rhs, old_rhs);
	
	new_stmt = build (MODIFY_EXPR, TREE_TYPE (new_lhs),
			  new_lhs, new_rhs);
	
	if (!new_stmt_list)
	  new_stmt_list = alloc_stmt_list ();
	append_to_statement_list (new_stmt, &new_stmt_list);
      }

  cfun = save_cfun;
  current_function_decl = save_fn_decl;

  return new_stmt_list;
}

static tree
find_base (tree arg0, tree arg1, block_stmt_iterator bsi)
{
  for ( ; !bsi_end_p (bsi); bsi_prev (&bsi))
    {
      tree stmt = bsi_stmt (bsi);
      
      if (TREE_CODE (stmt) == MODIFY_EXPR)
	{
	  tree lhs = TREE_OPERAND (stmt, 0);
	  tree rhs = TREE_OPERAND (stmt, 1);

	  if (lhs == arg0)
	    {
	      if (TREE_CODE (rhs) == VAR_DECL
		  ||TREE_CODE (rhs) == PARM_DECL)
		return arg0;
	      else if ((TREE_CODE (rhs) == NOP_EXPR
			|| TREE_CODE (rhs) == CONVERT_EXPR)
		       && INTEGRAL_TYPE_P (TREE_TYPE (TREE_OPERAND (rhs, 0))))
		return find_base (arg1, NULL, bsi);
	      else if (TREE_CODE (rhs) == PLUS_EXPR)
		return find_base (TREE_OPERAND (rhs, 0), 
				  TREE_OPERAND (rhs, 1),
				  bsi);
	    }
	  else if (lhs == arg1)
	    {
	      if (TREE_CODE (rhs) == VAR_DECL
		  || TREE_CODE (rhs) == PARM_DECL)
		return arg1;
	      else if ((TREE_CODE (rhs) == NOP_EXPR
			|| TREE_CODE (rhs) == CONVERT_EXPR)
		       && INTEGRAL_TYPE_P (TREE_TYPE (TREE_OPERAND (rhs, 0))))
		return find_base (arg0, NULL, bsi);
	      else if (TREE_CODE (rhs) == PLUS_EXPR)
		return find_base (TREE_OPERAND (rhs, 0), 
				  TREE_OPERAND (rhs, 1),
				  bsi);
	    }
	}
    }

  return NULL_TREE;
}

/* Given an original var decl (ORIG_VAR) and a newly created type (NEW_TYPE), 
   find the newly created var decl that corresponds to the original var decl
   but is of the new type.  */

static tree
find_new_var (tree orig_var, tree new_type,
	      struct new_var_data *new_vars)
{
  struct new_var_data *current;
  struct struct_tree_list *cur_var;
  bool found = false;
  tree return_var = NULL_TREE;

  for (current = new_vars; current && !found; current = current->next)
    {
      if (current->orig_var == orig_var)
	{
	  for (cur_var = current->new_vars; cur_var && !found; 
	       cur_var = cur_var->next)
	    {
	      if (TREE_TYPE (cur_var->data) == new_type)
		{
		  found = true;
		  return_var = cur_var->data;
		}
	    }
	}
    }

  return return_var;
}

static void
update_struct_offset (struct struct_list *data_struct_list,
		      struct new_var_data *new_vars, 
		      struct use_struct *use,
		      tree_stmt_iterator *insertion_point)
{
  tree index_var;
  tree orig_struct;
  tree new_stmt;
  tree orig_struct_size;
  struct struct_tree_list *tmp_node;
  struct data_structure *struct_data;
  struct struct_list *cur_struct;
  struct function *save_cfun;
  tree save_fn_decl;

  tree lhs = use->lhs;
  tree new_stmt_list = alloc_stmt_list ();
  tree convert = TREE_OPERAND (use->stmt, 1);  /* stmt rhs */
  tree arg0 = TREE_OPERAND (convert, 0);       /* int being converted */
  tree new_int_type = TREE_TYPE (arg0);
  bool found = false;
			  
  save_cfun = cfun;
  save_fn_decl = current_function_decl;

  cfun = DECL_STRUCT_FUNCTION (use->function);
  current_function_decl = use->function;

  orig_struct = TREE_TYPE (convert);

  while (POINTER_TYPE_P (orig_struct))
    orig_struct = TREE_TYPE (orig_struct);
  
  for (cur_struct = data_struct_list;
       cur_struct && !found;
       cur_struct = cur_struct->next)
    if (cur_struct->struct_data->decl == orig_struct)
      {
	found = true;
	break;
      }
			  
  if (!found)
    fatal_error ("Unable to find correct struct type.");
  else
    struct_data = cur_struct->struct_data;
			  
  orig_struct_size = lang_hooks.optimize.sizeof_type (struct_data->decl,
						      true, 0);

  index_var = create_tmp_var_raw (new_int_type, NULL);
  
  gimple_add_tmp_var (index_var);
  
  new_stmt = build (MODIFY_EXPR, new_int_type,
		    index_var,
		    build (TRUNC_DIV_EXPR, 
			   new_int_type,
			   arg0, orig_struct_size));

  append_to_statement_list (new_stmt, &new_stmt_list);

  for (tmp_node = struct_data->new_types;
       tmp_node; 
       tmp_node = tmp_node->next)
    {
      tree new_stmt1;
      tree new_stmt2;
      tree size_var;
      tree typecast_var;
      tree ptr_type;
      tree new_struct_type = tmp_node->data;
      tree new_struct_size;
      
      size_var = create_tmp_var_raw (new_int_type,
				     NULL);
      gimple_add_tmp_var (size_var);
      
      new_struct_size = lang_hooks.optimize.sizeof_type (new_struct_type, 
							 true, 0);
      
      new_stmt1 = build (MODIFY_EXPR, new_int_type,
			 size_var,
			 build (MULT_EXPR, 
				new_int_type,
				index_var, 
				new_struct_size));
      
      append_to_statement_list (new_stmt1, 
				&new_stmt_list);
      
      ptr_type = build_pointer_type (new_struct_type);
      
      typecast_var = find_new_var (lhs, ptr_type,
				   new_vars);
      
      if (!typecast_var)
	fatal_error ("Unable to find newly created var.");
      
      new_stmt2 = build (MODIFY_EXPR, ptr_type,
			 typecast_var, 
			 build1 (CONVERT_EXPR, ptr_type,
				 size_var));
      
      append_to_statement_list (new_stmt2, 
				&new_stmt_list);
    }

  add_bb_info (use->bb, new_stmt_list);
  tsi_link_after (insertion_point, new_stmt_list,
		  TSI_CONTINUE_LINKING);

  cfun = save_cfun;
  current_function_decl = save_fn_decl;
}

/* This function takes two lists of type new_var_data, prepends NEW_LIST to
   RETURN_LIST, and modifies RETURN_LIST to be the newly combined list.  */

static void
append_to_var_list (struct new_var_data *new_list,
		    struct new_var_data **return_list)
{
  struct new_var_data *current;

  if (!new_list)
    return;
  else if (! *return_list)
    *return_list = new_list;
  else
    {
      for (current = *return_list; current && current->next;
	   current = current->next);
      current->next = new_list;
    }
}

/* This function goes to  every place where a field in the original struct
   (STRUCT_DATA) is accessed, and updates the field access code to 
   correctly reflect the new struct hierarchy, types, variables, etc.   */

static void
update_field_accesses (struct struct_list *data_struct_list,
		       struct new_var_data *new_vars)
{
  int i;
  struct function *save_cfun;
  struct struct_list *cur_struct;
  struct data_field_entry cur_field;
  struct new_var_data *cur_var;
  tree new_stmt_list = NULL_TREE;
  tree save_fn_decl;

  save_cfun = cfun;
  save_fn_decl = current_function_decl;

  for (cur_struct = data_struct_list; cur_struct; 
       cur_struct = cur_struct->next)
    if (cur_struct->struct_data->new_types)
      {
	for (i = 0; i < cur_struct->struct_data->num_fields; i++)
	  {
	    struct access_site *cur_access;
	    struct field_map *new_mapping;
	    struct field_map *cur_map_pos;
	    
	    cur_field = cur_struct->struct_data->fields[i];
	    new_mapping = cur_field.new_mapping;
	    
	    for (cur_access = cur_field.acc_sites; cur_access; 
		 cur_access = cur_access->next)
	      {
		tree access = cur_access->field_access;
		tree arg0;
		tree arg1;
		tree old_var;
		tree new_access;
		tree field_identifier;
		tree outer_tree;
		bool found = false;
		tree_stmt_iterator *insertion_point = NULL;
		block_stmt_iterator bsi;
		bool is_indirect_ref = false;
		bool is_mem_ref = false;
		bool is_array_ref = false;
		bool field_access_found = false;
		tree mem_ref_index = NULL_TREE;
		tree array_ref_index = NULL_TREE;
		int arg_position;
		
		new_stmt_list = NULL_TREE;
		
		if (TREE_CODE (access) == VAR_DECL
		    || TREE_CODE (access) == PARM_DECL
		    || TREE_CODE (access) == INDIRECT_REF)
		  continue;
		
		outer_tree = cur_access->stmt;
		if (TREE_OPERAND (outer_tree, 0) == access)
		  arg_position = 0;
		else if (TREE_OPERAND (outer_tree, 1) == access)
		  arg_position = 1;
		else
		  fatal_error ("Field access is not direct child of statement.");
		
		if (TREE_CODE (access) == ARRAY_REF)
		  {
		    outer_tree = access;
		    arg_position = 0;
		    access = TREE_OPERAND (access, 0);
		  }
		
		arg0 = TREE_OPERAND (access, 0);
		arg1 = TREE_OPERAND (access, 1);
		
		while (! field_access_found)
		  {
		    if (DECL_P (arg1)
			&& DECL_NAME (arg1)
			&& (strcmp (IDENTIFIER_POINTER (DECL_NAME (arg1)),
				    IDENTIFIER_POINTER (DECL_NAME (cur_field.decl)))
			    == 0))
		      field_access_found = true;
		    else
		      {
			if (TREE_CODE (arg0) != COMPONENT_REF
			    && TREE_CODE (arg0) != ARRAY_REF)
			  fatal_error ("Cannot find field access within statement.");
			else
			  {
			    outer_tree = access;
			    arg_position = 0;
			    access = arg0;
			    arg0 = TREE_OPERAND (access, 0);
			    arg1 = TREE_OPERAND (access, 1);
			  }
		      }
		  }
		
		/* Find the access stmt within the basic block.  */
		
		for (bsi = bsi_start (cur_access->bb); !bsi_end_p (bsi);
		     bsi_next (&bsi))
		  if (bsi_stmt (bsi) == cur_access->stmt)
		    {
		      insertion_point = &(bsi.tsi);
		      break;
		    }
		
		cfun = cur_access->context;
		current_function_decl = cfun->decl;
		
		/* find <indirect_ref> whose arg1 is field decl for cur_field */
		
		if (TREE_CODE (arg0) == INDIRECT_REF)
		  is_indirect_ref = true;
		else if (TREE_CODE (arg0) == MEM_REF)
		  {
		    is_mem_ref = true;
		    mem_ref_index = TREE_OPERAND (arg0, 1);
		  }
		else if (TREE_CODE (arg0) == ARRAY_REF)
		  {
		    is_array_ref = true;
		    array_ref_index = TREE_OPERAND (arg0, 1);
		  }
		
		if ((strcmp (IDENTIFIER_POINTER (DECL_NAME (arg1)),
			     IDENTIFIER_POINTER (DECL_NAME (cur_field.decl))) != 0)
		    || (TREE_CODE (TREE_TYPE (arg1)) != TREE_CODE (TREE_TYPE (cur_field.decl))))
		  fatal_error ("Field decls should match but don't.");
		
		if (TREE_CODE (arg0) == VAR_DECL
		    || TREE_CODE (arg0) == PARM_DECL)
		  old_var = arg0;
		else
		  old_var = TREE_OPERAND (arg0, 0);
		
		for (cur_var = new_vars; cur_var && !found; 
		     cur_var = cur_var->next)
		  {
		    if (old_var == cur_var->orig_var)
		      {
			struct struct_tree_list *current;
			found = true;
			for (current = cur_var->new_vars; current; 
			     current = current->next)
			  {
			    tree var_type = TREE_TYPE (current->data);
			    
			    while (POINTER_TYPE_P (var_type))
			      var_type = TREE_TYPE (var_type);

			    if (TREE_CODE (var_type) == ARRAY_TYPE)
			      var_type = TREE_TYPE (var_type);
			    
			    if (similar_struct_decls_p (var_type, new_mapping->decl))
			      {
				tree tmp_field;
				tree tmp_type;
				tree save_type;
				tree old_var;
				tree new_var;
				tree new_stmt;
				new_access = NULL_TREE;
				old_var = current->data;
				for (cur_map_pos = new_mapping; cur_map_pos;
				     cur_map_pos = cur_map_pos->contains)
				  {
				    if (cur_map_pos->contains)
				      {
					field_identifier = NULL_TREE;
					for (tmp_field = TYPE_FIELDS (cur_map_pos->decl);
					     tmp_field && !field_identifier;
					     tmp_field = TREE_CHAIN (tmp_field))
					  {
					    tmp_type = TREE_TYPE (tmp_field);
					    save_type = tmp_type;
					    while (POINTER_TYPE_P (tmp_type))
					      tmp_type = TREE_TYPE (tmp_type);
					    if (similar_struct_decls_p (tmp_type,
									cur_map_pos->contains->decl))
					      field_identifier = DECL_NAME (tmp_field);
					  }
					if (!field_identifier)
					  fatal_error ("Could not find field for substruct in superstruct.");
					
					if (! new_stmt_list)
					  new_stmt_list = alloc_stmt_list ();
					
					new_var = create_tmp_var_raw (save_type,
								      NULL);
					gimple_add_tmp_var (new_var);
					
					if (is_indirect_ref)
					  new_stmt = build (MODIFY_EXPR,
							    save_type,
							    new_var,
							    lang_hooks.optimize.build_field_reference 
							    (lang_hooks.optimize.build_pointer_ref 
							     (old_var, ""),
							     field_identifier));
					else if (is_mem_ref)
					  new_stmt = build (MODIFY_EXPR,
							    save_type,
							    new_var,
							    lang_hooks.optimize.build_field_reference 
							    (lang_hooks.optimize.build_array_ref 
							     (old_var, mem_ref_index),
							     field_identifier));
					else if (is_array_ref)
					  new_stmt = build (MODIFY_EXPR,
							    save_type,
							    new_var,
							    lang_hooks.optimize.build_field_reference
							    (lang_hooks.optimize.build_array_ref
							     (old_var, array_ref_index),
							     field_identifier));
					else
					  new_stmt = build (MODIFY_EXPR,
							    save_type,
							    new_var,
							    lang_hooks.optimize.build_field_reference
							    (old_var,
							     field_identifier));
					
					append_to_statement_list (new_stmt, 
								  &new_stmt_list);
					old_var = new_var;
				      }
				    else
				      {
					field_identifier = DECL_NAME (arg1);
					if (is_indirect_ref)
					  new_access = lang_hooks.optimize.build_field_reference
					    (lang_hooks.optimize.build_pointer_ref (old_var, ""),
					     field_identifier);
					else if (is_mem_ref)
					  new_access = lang_hooks.optimize.build_field_reference
					    (lang_hooks.optimize.build_array_ref (old_var, mem_ref_index),
					     field_identifier);
					else if (is_array_ref)
					  new_access = lang_hooks.optimize.build_field_reference

					    (lang_hooks.optimize.build_array_ref (old_var, array_ref_index),
					     field_identifier);
					else
					  new_access = lang_hooks.optimize.build_field_reference
					    (old_var, field_identifier);
				      }
				  }
				
				if (!new_access)
				  fatal_error ("Couldn't find correct field access.");
				
				/*
				  if (access == TREE_OPERAND (cur_access->stmt, 0))
				  TREE_OPERAND (cur_access->stmt, 0) = new_access;
				  else if (access == TREE_OPERAND (cur_access->stmt,
				  1))
				  TREE_OPERAND (cur_access->stmt, 1) = new_access;
				  else
				  fatal_error 
				  ("Cannot locate field access within statement.");
				*/
				
				TREE_OPERAND (outer_tree, arg_position) = new_access;
				
				if (new_stmt_list)
				  {
				    /* Insert new stmts before access stmt */
				    add_bb_info (cur_access->bb, new_stmt_list);
				    tsi_link_before (insertion_point,
						     new_stmt_list,
						     TSI_CONTINUE_LINKING);
				  }
			      }
			  }
		      }
		  }
	      }
	  }
      }
  
  cfun = save_cfun;
  current_function_decl = save_fn_decl;
}

static void
collect_struct_use_data (struct struct_list *data_struct_list,
			 struct new_var_data *new_vars_list,
			 struct use_struct **uses_list)
{
  struct cgraph_node *c_node;
  basic_block this_bb;
  struct use_struct *last_use = NULL;
  struct use_struct *first_use_in_fn;
  struct use_struct *prev_use, *cur_use;
  struct function *func;
  struct malloc_struct *cur_malloc;
  struct function *save_cfun;
  tree fn_decl;
  tree save_fn_decl;
  bool function_contains_malloc;
  block_stmt_iterator bsi;
  tree cur_stmt;
  struct struct_tree_list *plus_modifies;
  struct struct_tree_list *base_modifies;
  struct struct_tree_list *tmp_node_1;
  struct struct_tree_list *prev, *cur;

  save_cfun = cfun;
  save_fn_decl = current_function_decl;

  for (c_node = cgraph_nodes; c_node; c_node = c_node->next)
    {
      if (!c_node->next_clone)
	{
	  func = DECL_STRUCT_FUNCTION (c_node->decl);
	  fn_decl = c_node->decl;

	  if (func)
	    {
	      /* Check to see if we need to do any malloc work in this 
		 function.  */
	      
	      function_contains_malloc = false;
	      for (cur_malloc = malloc_data_list; 
		   cur_malloc && !function_contains_malloc; 
		   cur_malloc = cur_malloc->next)
		if (cur_malloc->context == fn_decl)
		  {
		    function_contains_malloc = true;
		    visited_nodes = pointer_set_create ();
		  }
	      
	      plus_modifies = NULL;
	      base_modifies = NULL;
	      first_use_in_fn = NULL;
	      prev_use = last_use;
	      
	      FOR_EACH_BB_FN (this_bb, func)
		{
		  /* Collect malloc data, if appropriate.  */
		  
		  if (function_contains_malloc)
		    {
		      c_node->current_basic_block = this_bb;
		      walk_tree (&this_bb->stmt_list, record_malloc_1, c_node,
				 visited_nodes);
		    }
		  
		  /* Go through all the statements in the function, looking for
		     those we might need to modify.  */
		  
		  for (bsi = bsi_start (this_bb); !bsi_end_p (bsi); 
		       bsi_next (&bsi))
		    {
		      cur_stmt = bsi_stmt (bsi);
		      if (TREE_CODE (cur_stmt) == MODIFY_EXPR)
			{
			  struct data_structure *struct_data = NULL;
			  tree lhs = TREE_OPERAND (cur_stmt, 0);
			  tree rhs = TREE_OPERAND (cur_stmt, 1);
			  bool is_part_of_malloc = false;
			  
			  /* If stmt is part of a set of malloc statements,
			     skip it; it will be taken care of when we fix up
			     the mallocs.  */
			  
			  if (function_contains_malloc)
			    for (cur_malloc = malloc_data_list; 
				 cur_malloc && !is_part_of_malloc;
				 cur_malloc = cur_malloc->next)
			      if (cur_malloc->context == fn_decl)
				{
				  int idx;
				  for (idx = 0; idx < cur_malloc->num_calls; 
				       idx++)
				    {
				      if (lhs == cur_malloc->malloc_list[idx].size_var
					  || lhs == cur_malloc->malloc_list[idx].result_var
					  || lhs == cur_malloc->malloc_list[idx].final_lhs)
					is_part_of_malloc = true;
				    }
				}
			  
			  if (!is_part_of_malloc)
			    {
			      tree lhs_type;
			      int pointer_levels_count = 0;
			      
			      /* Check to see if lhs is of an interesting 
				 type.  */
			      
			      lhs_type = TREE_TYPE (lhs);
			      
			      while (POINTER_TYPE_P (lhs_type))
				{
				  pointer_levels_count++;
				  lhs_type = TREE_TYPE (lhs_type);
				}
			      
			      if (TREE_CODE (lhs_type) == RECORD_TYPE)
				{
				  struct struct_list *cur_struct;
				  
				  for (cur_struct = data_struct_list;
				       cur_struct && !struct_data; 
				       cur_struct = cur_struct->next)
				    if (cur_struct->struct_data->decl == lhs_type
					&& cur_struct->struct_data->struct_clustering
					&& cur_struct->struct_data->struct_clustering->direct_access)
				      struct_data = cur_struct->struct_data;
				  
				}
			      
			      if (struct_data
				  && (TREE_CODE (rhs) == VAR_DECL
				      || TREE_CODE (rhs) == PARM_DECL))
				{
				  tmp_node_1 =  (struct struct_tree_list *) 
				    xmalloc (sizeof (struct struct_tree_list));
				  tmp_node_1->data = cur_stmt;
				  tmp_node_1->next = base_modifies;
				  base_modifies = tmp_node_1;
				}
			      else if (struct_data
				       && TREE_CODE (rhs) == PLUS_EXPR)
				{
				  tmp_node_1 = (struct struct_tree_list *) 
				    xmalloc (sizeof (struct struct_tree_list));
				  tmp_node_1->data = cur_stmt;
				  tmp_node_1->next = plus_modifies;
				  plus_modifies = tmp_node_1;
				}
			      
			      if (struct_data
				  && (TREE_CODE (rhs) == CONVERT_EXPR
				      || TREE_CODE (rhs) == NOP_EXPR)
				  && INTEGRAL_TYPE_P (TREE_TYPE 
						      (TREE_OPERAND (rhs, 0))))
				{
				  /* We've found an offset conversion  */
				  /* OFFSET == INDEX * ELEMENT_SIZE    */
				  struct use_struct *offset;
				  
				  offset = (struct use_struct *) xmalloc 
				    (sizeof (struct use_struct));
				  
				  offset->lhs_use = false;
				  offset->lhs = lhs;
				  offset->rhs_vars = NULL;
				  offset->offset = rhs;
				  offset->stmt = cur_stmt;
				  offset->bb = this_bb;
				  offset->function = fn_decl;
				  offset->next = NULL;
				  
				  if (!last_use)
				    (*uses_list) = offset;
				  else
				    last_use->next = offset;
				  last_use = offset;
				  
				  if (!first_use_in_fn)
				    first_use_in_fn = offset;
				}
			      else /* if (struct_data) */
				{
				  struct struct_tree_list *rhs_vars;
				  
				  rhs_vars = search_rhs_for_struct_vars (rhs,
									 data_struct_list,
									 new_vars_list);
				  
				  if (rhs_vars || struct_data)
				    {
				      /* We've found a "use" that needs to be
					 "updated. */
				      
				      struct use_struct *use;
				      
				      use = (struct use_struct *) xmalloc 
					(sizeof (struct use_struct));
				      
				      if (struct_data)
					use->lhs_use = true;
				      else
					use->lhs_use = false;
				      use->lhs = lhs;
				      use->rhs_vars = rhs_vars;
				      use->offset = NULL_TREE;
				      use->stmt = cur_stmt;
				      use->bb = this_bb;
				      use->function = fn_decl;
				      use->next = NULL;
				      
				      if (!last_use)
					(*uses_list) = use;
				      else
					last_use->next = use;
				      last_use = use;
				      
				      if (!first_use_in_fn)
					first_use_in_fn = use;
				      
				    }
				}
			    }
			}
		    }
		  
		}  /* FOR_EACH_BB_FN */
	      
	      /* Verify that every "offset" use is really an offset, i.e. actually
		 gets use in a base-plus-offset calculation.  */
	      
	      /* First, go through the plus_modifies stmt list and find the ones
		 where one argument is a 'base' variable, i.e. the argument is an
		 lhs in the base_modifies list.  Remove other plus_expr stmts from
		 the list.  */
	      
	      prev = NULL;
	      for (cur = plus_modifies; cur; cur = cur->next)
		{
		  struct struct_list *cur_struct;
		  tree plus_stmt = cur->data;
		  tree plus_expr = TREE_OPERAND (plus_stmt, 1);
		  tree arg0 = TREE_OPERAND (plus_expr, 0);
		  tree arg1 = TREE_OPERAND (plus_expr, 1);
		  struct struct_tree_list *tmp_ptr;
		  bool found = false;
		  
		  for (tmp_ptr = base_modifies; tmp_ptr && !found;
		       tmp_ptr = tmp_ptr->next)
		    if (TREE_OPERAND (tmp_ptr->data, 0) == arg0
			|| TREE_OPERAND (tmp_ptr->data, 0) == arg1)
		      found = 1;
		  
		  if (!found)
		    {
		      tree type0 = TREE_TYPE (arg0);
		      tree type1 = TREE_TYPE (arg1);
		      
		      while (POINTER_TYPE_P (type0)
			     || TREE_CODE (type0) == ARRAY_TYPE)
			type0 = TREE_TYPE (type0);
		      
		      while (POINTER_TYPE_P (type1)
			     || TREE_CODE (type1) == ARRAY_TYPE)
			type1 = TREE_TYPE (type1);
		      
		      if ((TREE_CODE (type0) == RECORD_TYPE)
			  || (TREE_CODE (type1) == RECORD_TYPE))
			for (cur_struct = data_struct_list; cur_struct && !found;
			     cur_struct = cur_struct->next)
			  {
			    if (similar_struct_decls_p (cur_struct->struct_data->decl,
							type0)
				|| similar_struct_decls_p (cur_struct->struct_data->decl,
							   type1))
			      found = true;
			  }
		    }
		  
		  if (!found)
		    {
		      if (!prev)
			plus_modifies = cur->next;
		      else
			prev->next = cur->next;
		    }
		  else
		    prev = cur;
		}
	      
	      /* Now every statement in the plus_modifies list is a base plus
		 offset calculation.  Next, go through the convert uses found in
		 the current function, and remove all of the ones where the 
		 'offset' being converted is not used in a base-plus-offset 
		 calculation.  */
	      
	      for (cur_use = first_use_in_fn; cur_use; cur_use = cur_use->next)
		{
		  tree convert_stmt = cur_use->stmt;
		  tree convert_lhs = TREE_OPERAND (convert_stmt, 0);
		  struct struct_tree_list *tmp_ptr;
		  bool found = false;
		  
		  if (!cur_use->offset)
		    {
		      prev_use = cur_use;
		      continue;
		    }
		  
		  for (tmp_ptr = plus_modifies; tmp_ptr && !found;
		       tmp_ptr = tmp_ptr->next)
		    if ((TREE_OPERAND (TREE_OPERAND (tmp_ptr->data, 1), 0) 
			 == convert_lhs)
			|| (TREE_OPERAND (TREE_OPERAND (tmp_ptr->data, 1), 1) 
			    == convert_lhs))
		      found = true;
		  
		  if (!found)
		    {
		      if (!prev_use)
			*uses_list = cur_use->next;
		      else
			prev_use->next = cur_use->next;
		    }
		  else
		    prev_use = cur_use;
		}
	      
	      /* Clean up malloc data collection stuff.  */
	      
	      if (function_contains_malloc)
		{
		  pointer_set_destroy (visited_nodes);
		  visited_nodes = NULL;
		}
	      
	    } /* if (func) */
	}
    } /* for (c_node = cgraph_nodes... */

  cfun = save_cfun;
  current_function_decl = save_fn_decl;

} /* function update_code_in_functions */

static void
update_struct_use_stmts (struct struct_list *data_struct_list,
			 struct new_var_data *new_vars_list,
			 struct use_struct *uses_list)
{
  block_stmt_iterator bsi;
  struct use_struct *current;
  
  for (current = uses_list; current; current = current->next)
    {
      for (bsi = bsi_start (current->bb); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  tree stmt = bsi_stmt (bsi);
	  
	  if (stmt == current->stmt)
	    {
	      tree_stmt_iterator *insertion_point = &(bsi.tsi);
	      if (current->offset)
		{
		  update_struct_offset (data_struct_list, new_vars_list,
					current, insertion_point);
		}
	      else
		{
		  tree new_stmt_list;

		  new_stmt_list = build_new_stmts (current->lhs, 
						   new_vars_list, 
						   current->stmt,
						   current->function,
						   data_struct_list);

		  if (new_stmt_list)
		    {
		      add_bb_info (current->bb, new_stmt_list);
		      tsi_link_after (insertion_point, new_stmt_list,
				      TSI_CONTINUE_LINKING);
		    }
		}
	    }
	}
    }
}

static void
add_type_to_worklist (tree new_type, struct struct_tree_list **worklist,
		      struct struct_tree_list **donelist)
{
  struct struct_tree_list *tmp_node;

  if (found_in_list (new_type, *worklist))
    return;

  if (found_in_list (new_type, *donelist))
    return;

  tmp_node = (struct struct_tree_list *) xmalloc (sizeof 
						  (struct struct_tree_list));
  tmp_node->data = new_type;
  tmp_node->next = *worklist;

  *worklist = tmp_node;
}

static void
add_type_to_donelist (tree new_type, struct struct_tree_list **donelist)
{
  struct struct_tree_list *tmp_node;

  if (found_in_list (new_type, *donelist))
    return;

  tmp_node = (struct struct_tree_list *) xmalloc (sizeof (struct struct_tree_list));
  tmp_node->data = new_type;
  tmp_node->next = *donelist;

  *donelist = tmp_node;
}

static void
remove_type_from_list (tree new_type, struct struct_tree_list **worklist)
{
  struct struct_tree_list *prev;
  struct struct_tree_list *current;

  for (prev = NULL, current = *worklist; current; current = current->next)
    {
      if (current->data == new_type)
	{
	  if (!prev)
	    *worklist = current->next;
	  else
	    prev->next = current->next;
	}
      else
	{
	  prev = current;
	}
    }
}

static bool
replace_old_field_types (struct struct_tree_list *new_type_list,
			 tree new_type_decl, tree old_type_decl,
			 struct struct_tree_list **donelist)
{
  bool recursive = false;
  struct struct_tree_list *current;
  struct struct_tree_list *worklist = NULL;
  tree cur_field;
  tree f_type;
  int pointer_count;

  for (current = new_type_list; current; current = current->next)
    {
      tree new_struct = current->data;
      for (cur_field = TYPE_FIELDS (new_struct); cur_field; 
	   cur_field = TREE_CHAIN (cur_field))
	{
	  pointer_count = 0;
	  f_type = TREE_TYPE (cur_field);
	  while (POINTER_TYPE_P (f_type))
	    {
	      pointer_count++;
	      f_type = TREE_TYPE (f_type);
	    }

	  if (TREE_CODE (f_type) == RECORD_TYPE)
	    {
	      if (similar_struct_decls_p (f_type, new_type_decl))
		continue;
	      else if (similar_struct_decls_p (f_type, old_type_decl))
		{
		  tree new_type = new_type_decl;
		  int i;
		  
		  recursive = true;
		  for (i = 0; i < pointer_count; i++)
		    new_type = build_pointer_type (new_type);
		  
		  TREE_TYPE (cur_field) = new_type;
		}
	      else
		{
		  add_type_to_worklist (f_type, &worklist, donelist);
		}
	    }
	}
      add_type_to_donelist (new_struct, donelist);
      if (found_in_list (new_struct, worklist))
	remove_type_from_list (new_struct, &worklist);
    }

  if (worklist)
    {
      bool tmp;
      tmp = replace_old_field_types (worklist, new_type_decl,
				     old_type_decl,
				     donelist);

      recursive = recursive || tmp;
    }

  return recursive;
}

static bool
search_for_bit_field_ref (tree t)
{
  int i;
  bool ret_val = false;
  
  if (TREE_CODE (t) == BIT_FIELD_REF)
    ret_val = true;
  else
    {
      for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (t)); i++)
	{
	  tree operand = TREE_OPERAND (t, i);
	  if (operand)
	    {
	      ret_val = ret_val || search_for_bit_field_ref (operand);
	    }
	}
    }

  return ret_val;
}

static bool
contains_bit_field_accesses_p (struct data_structure *struct_data)
{
  bool ret_val = false;
  struct access_site *cur_access;
  struct data_field_entry cur_field;
  int i;

  for (i = 0; i < struct_data->num_fields; i++)
    {
      cur_field = struct_data->fields[i];
      for (cur_access = cur_field.acc_sites; cur_access; 
	   cur_access = cur_access->next)
	{
	  tree cur_stmt = cur_access->stmt;
	  ret_val = search_for_bit_field_ref (cur_stmt);
	  if (ret_val)
	    return true;
	}
    }

  return ret_val;
}

static bool
check_field_order (struct data_structure *ds)
{
  bool different_order = false;
  int i;
  HOST_WIDE_INT prev_f_bit_pos;
  tree f_bit_pos_t;

  if (!ds->struct_clustering->fields_order)
    return false;

  f_bit_pos_t = bit_position (ds->fields[0].decl);
  if (! host_integerp (f_bit_pos_t, 0))
    abort ();
  prev_f_bit_pos = tree_low_cst (f_bit_pos_t, 0);

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
  return different_order;
}

static struct struct_tree_list *
reorder_fields (struct data_structure *ds)
{
  char *old_name;
  char *new_name;
  int i;
  int idx;
  int k = 0;
  tree new_type;
  tree new_decl;
  tree last_field = NULL;
  struct new_type_node *new_types = NULL;
  struct struct_tree_list *tmp_node;

  old_name = get_type_name (ds->decl);
  new_name = (char *) xmalloc (strlen (old_name) + 7);
  sprintf (new_name, "%s_%d", old_name, k++);
  
  while (maybe_get_identifier (new_name))
    sprintf (new_name, "%s_%d", old_name, k++);
  
  new_types = (struct new_type_node *) xmalloc (sizeof (struct new_type_node));

  new_types->sub_type = false;
  new_types->num_fields = ds->num_fields;
  new_types->field_list = NULL;
  new_types->name = NULL;
  new_types->next = NULL;


  for (i = 0; i < ds->num_fields; i++)
    {
      idx = ds->struct_clustering->fields_order[i];
      new_decl = copy_node (ds->fields[idx].decl);
      new_decl->decl.rtl = NULL;
      if (!new_types->field_list)
	new_types->field_list = new_decl;
      else
	TREE_CHAIN (last_field) = new_decl;
      last_field = new_decl;
    }

  TREE_CHAIN (last_field) = NULL;
  new_types->next = NULL;

  new_type = lang_hooks.optimize.build_data_struct ((void *) new_types,
						    new_name,
						    ds->decl);

  if (TYPE_MAIN_VARIANT (ds->decl) != ds->decl)
    TYPE_MAIN_VARIANT (new_type) = TYPE_MAIN_VARIANT (ds->decl);

  tmp_node = (struct struct_tree_list *) xmalloc (sizeof 
						  (struct struct_tree_list));
  tmp_node->data = new_type;
  tmp_node->next = NULL;

  return tmp_node;
}

static bool
struct_passed_to_external_function (tree decl)
{
  struct cgraph_node *c_node;

  for (c_node = cgraph_nodes; c_node; c_node = c_node->next)
    {
      /* Check for structs being passed as argument to builtin functions */

      if (DECL_BUILT_IN (c_node->decl)
	  && (! DECL_ARGUMENTS (c_node->decl))
	  && (strcmp (IDENTIFIER_POINTER (DECL_NAME (c_node->decl)),
		      "malloc") != 0)
	  && (strcmp (IDENTIFIER_POINTER (DECL_NAME (c_node->decl)),
		      "xmalloc") != 0)
	  && (strcmp (IDENTIFIER_POINTER (DECL_NAME (c_node->decl)),
		      "realloc") != 0)
	  && (strcmp (IDENTIFIER_POINTER (DECL_NAME (c_node->decl)),
		      "calloc") != 0))
	{
	  tree call_stmt;
	  tree call_site;
	  tree arg_list;
	  tree arg;
	  struct cgraph_edge *caller;

	  for (caller = c_node->callers; caller; caller = caller->next_caller)
	    {
	      call_stmt = caller->call_stmt;
	      if (TREE_CODE (call_stmt) == CALL_EXPR)
		call_site = call_stmt;
	      else if (TREE_CODE (call_stmt) == MODIFY_EXPR
		       && TREE_CODE (TREE_OPERAND (call_stmt, 1)) == CALL_EXPR)
		call_site = TREE_OPERAND (call_stmt, 1);
	      else
		abort ();
	      for (arg_list = TREE_OPERAND (call_site, 1); arg_list;
		   arg_list = TREE_CHAIN (arg_list))
		{
		  tree arg_type;
		  arg = TREE_VALUE (arg_list);

		  arg_type = TREE_TYPE (arg);
		  while (POINTER_TYPE_P (arg_type)
			 || TREE_CODE (arg_type) == ARRAY_TYPE)
		    arg_type = TREE_TYPE (arg_type);
		  
		  if (similar_struct_decls_p (arg_type, decl))
		      return true;
		}
	    }
	}
    }

  return false;
}

static bool
contained_in_other_struct (struct data_structure *struct_data, 
			   struct struct_list *data_struct_list)
{
  struct struct_list *cur_struct;
  struct data_structure *ds;
  bool ret_value = false;
  int i;
  tree cur_field;
  tree cur_type;

  for (cur_struct = data_struct_list; cur_struct && !ret_value; 
       cur_struct = cur_struct->next)
    {
      ds = cur_struct->struct_data;
      if (ds == struct_data)
	continue;
      if (!ds->struct_clustering 
	  || ds->count <= 0)
	continue;

      for (i = 0; i < ds->num_fields; i++)
	{
	  cur_field = ds->fields[i].decl;
	  cur_type = TREE_TYPE (cur_field);
	  while (POINTER_TYPE_P (cur_type)
		 || TREE_CODE (cur_type) == ARRAY_TYPE)
	    cur_type = TREE_TYPE (cur_type);
	  if (similar_struct_decls_p (cur_type, struct_data->decl))
	    {
	      ret_value = true;
	      break;
	    }
	}
    }
  
  return ret_value;
}

static bool
is_recursive_struct (struct data_structure *struct_data)
{
  int i;
  bool ret_value = false;
  for (i = 0; i < struct_data->num_fields && !ret_value; i++)
    {
      tree cur_field = struct_data->fields[i].decl;
      tree cur_type = TREE_TYPE (cur_field);

      if (DECL_P (cur_field)
	  && (strcmp (IDENTIFIER_POINTER (DECL_NAME (cur_field)), "next") == 0)
	  && POINTER_TYPE_P (cur_type))
	ret_value = true;

      while (POINTER_TYPE_P (cur_type)
	     || TREE_CODE (cur_type) == ARRAY_TYPE)
	cur_type = TREE_TYPE (cur_type);
      if (similar_struct_decls_p (cur_type, struct_data->decl))
	ret_value = true;
    }

  return ret_value;
}

static tree
replace_all_types_1 (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED, void *data)
{
  tree t = *tp;
  struct data_structure *struct_data = (struct data_structure *) data;

  switch (TREE_CODE (t))
    {
    case STATEMENT_LIST :
      {
	tree_stmt_iterator tsi;
	for (tsi = tsi_start (*tp); !tsi_end_p (tsi); tsi_next (&tsi))
	  walk_tree (tsi_stmt_ptr (tsi), replace_all_types_1, data,
		     visited_nodes);
	break;
      default:
	{
	  struct struct_tree_list *type_nest;
	  struct struct_tree_list *tmp_node;
	  tree node_type;
	  tree new_type;
	  tree operand;
	  int i;

	  node_type = TREE_TYPE (t);
	  type_nest = NULL;
	  while (node_type
		 && (POINTER_TYPE_P (node_type)
		     || TREE_CODE (node_type) == ARRAY_TYPE))
	    {
	      tmp_node = (struct struct_tree_list *) xmalloc (sizeof
							      (struct struct_tree_list));
	      tmp_node->data = node_type;
	      tmp_node->next = type_nest;
	      type_nest = tmp_node;
	      node_type = TREE_TYPE (node_type);
	    }

	  if (similar_struct_decls_p (node_type, struct_data->decl))
	    {
	      new_type = build_new_type (type_nest, struct_data->new_types->data);
	      TREE_TYPE (t) = new_type;
	    }

	  for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (t)); i++)
	    {
	      operand = TREE_OPERAND (t, i);
	      if (operand)
		walk_tree (&operand, replace_all_types_1, data,
			   visited_nodes);
	    }
	  break;
	}
      }
    }
  return NULL; 
}

static void
replace_all_types (struct data_structure *struct_data)
{
  struct cgraph_node *c_node;
  struct function *func;
  struct function *save_cfun;
  tree fn_decl;
  tree save_fn_decl;
  basic_block this_bb;

  save_cfun = cfun;
  save_fn_decl = current_function_decl;

  for (c_node = cgraph_nodes; c_node; c_node = c_node->next)
    {
      func = DECL_STRUCT_FUNCTION (c_node->decl);
      fn_decl = c_node->decl;

      if (func)
	{
	  visited_nodes = pointer_set_create ();
	  FOR_EACH_BB_FN (this_bb, func)
	    walk_tree (&this_bb->stmt_list, replace_all_types_1, struct_data,
		       visited_nodes);
	  pointer_set_destroy (visited_nodes);
	  visited_nodes = NULL;
	}
    }

  current_function_decl = save_fn_decl;
  cfun = save_cfun;
}

static void
do_reorg (struct struct_list *data_struct_list, bitmap passed_types,
	 bitmap escaped_types)
{
  int sub_type_count = 0;
  struct struct_list *cur_struct;
  struct new_var_data *total_new_vars = NULL;
  bool debug_vars = false;
  bool debug_new_types = false;
  bool debug_uses = true;

  /* 
     1).  Create a new struct/record type (or hierarchy of types)
          according to the information in struct_clustering field of
          struct_data.  Look at the following functions for help:
          start_struct and end_struct (in c-decl.c) and layout_type (in
          stor-layout.c).

     1b). Possibly there will be multiple new types (full peeling rather
          than just splitting into sub-structs...).  Verify that we have
	  enough information to decide if this is what we are doing or not.

     2). Create a new pointer type for each new type created in step 1, 
         pointing to the new struct/record types. (See build_pointer_type).
	
     3).  For each variable that is declared to be of original type:
                create new sub-variable for each fully peeled sub-struct.
		declare new sub-variable to be of appropriate sub-struct type.
	        declare main variable to be of appriopriate type.

     4).  Find locations where mallocs of types are occurring & fix 
          apropriately.
              - Add new mallocs for newly created/split sub-types & newly
	        created vars.

     5).  Find and fix all field accesses appropriately.

     6).  If we are doing full peeling,  find any functions to which full
          struct is being passed, add new parameters & arguments as
	  appropriate.  For first pass we should probably disallow peeling
	  of such structs in phase 1.
  */

  for (cur_struct = data_struct_list; cur_struct;
       cur_struct = cur_struct->next)
    {
      struct data_structure *struct_data = cur_struct->struct_data;

      if (struct_data->count > 0
	  && (TYPE_NAME (struct_data->decl))
	  && struct_data->struct_clustering)
	{
	  if (bitmap_bit_p (escaped_types, (struct_data->decl)->type.uid)
	      || bitmap_bit_p (passed_types, (struct_data->decl)->type.uid)
	      || contains_bit_field_accesses_p (struct_data))
	    struct_data->reorder_only = true;

	  if (!struct_data->reorder_only
	      && struct_passed_to_external_function (struct_data->decl))
	    struct_data->reorder_only = true;

	  if (!struct_data->reorder_only
	      && is_recursive_struct (struct_data))
	    {
	      if (!struct_data->struct_clustering->children)
		{
		  struct_data->struct_clustering->children = 
		                        struct_data->struct_clustering->sibling;
		  struct_data->struct_clustering->sibling = NULL;
		}
	      else
		struct_data->reorder_only = true;
	    }

	  if (struct_data->reorder_only)
	    {
	      bool different_order;
	      bool recursive_struct = false;
	      struct struct_tree_list *donelist = NULL;
	      
	      different_order = check_field_order (struct_data);
	      if (different_order)
		{
		  struct_data->new_types = reorder_fields (struct_data);
		  recursive_struct = replace_old_field_types 
		    (struct_data->new_types,
		     struct_data->new_types->data,
		     struct_data->decl,
		     &donelist);
		  replace_var_types (struct_data->decl,
				     struct_data->new_types->data);
		  replace_all_types (struct_data);
		}
	    }
	  else if ((struct_data->struct_clustering->sibling
		    || struct_data->struct_clustering->children))
	    {
	      struct struct_tree_list *donelist = NULL;
	      struct new_var_data *new_vars;
	      bool recursive_struct = false;
	      
	      if (!struct_data->struct_clustering->children)
		struct_data->struct_clustering->direct_access = true;
	      sub_type_count = 0;
	      
	      struct_data->new_types =  create_and_assemble_new_types 
		(struct_data,
		 struct_data->struct_clustering,
		 &sub_type_count,
		 1);
	      recursive_struct = 
		replace_old_field_types (struct_data->new_types,
					 struct_data->new_types->data,
					 struct_data->decl,
					 &donelist);

	      if (contained_in_other_struct (struct_data, data_struct_list))
		{
		  if (!struct_data->struct_clustering->direct_access)
		    replace_all_types (struct_data);
		  else
		    struct_data->new_types = NULL;
		}
	      
	      new_vars = create_new_var_decls (struct_data);
	      append_to_var_list (new_vars, &total_new_vars);
	      if (debug_vars)
		print_new_vars (new_vars);
	    }
	  else
	    struct_data->new_types = NULL; 
	  
	  /* if (struct_data->new_types) */
	  if (debug_new_types)
	    {
	      fprintf (stdout, 
		       "\nThe following are the new types generated by"
		       " this optimization.\n");
	      print_new_types (struct_data->new_types);
	    }
	}
    }

  if (total_new_vars)
    {
      struct use_struct *uses_list = NULL;
      
      collect_struct_use_data (data_struct_list, total_new_vars, &uses_list);
      create_new_mallocs (data_struct_list, total_new_vars);
      if (debug_uses)
	print_use_list (uses_list);
      update_struct_use_stmts (data_struct_list, total_new_vars,
			       uses_list);
      update_field_accesses (data_struct_list, total_new_vars);
    }
}

/* The following aren't imeplemnted yet, so we comment them out
   for now to be able to bootstrap.  */
#if 0
/* ??? This is should be the same as get_stmt_field_accesses.  */
static tree
tree_contains_struct_access (ATTRIBUTE_UNUSED tree t,
			     ATTRIBUTE_UNUSED  void *data)
{
  return NULL_TREE;
}

static tree
find_struct_data (ATTRIBUTE_UNUSED tree *tp, 
		  ATTRIBUTE_UNUSED int *walk_subtrees,
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

  /* Check to see if it is legal to rearrange this structure.  */

  /*
    if (!ipa_static_type_contained_p (struct_type))
    return NULL;
  */

  /* At this time we don't want to mess with pre-initialized
     arrays, so if it's got a hard-coded initialization, don't
     reorganize it.  */

  if (DECL_INITIAL (var_decl))
    return NULL;

  d_node = (struct data_structure *) 
	   xcalloc (1, sizeof (struct data_structure));
  num_fields = fields_length (struct_type);
  d_node->decl = struct_type;
  d_node->num_fields = num_fields;
  d_node->fields = get_fields (struct_type, num_fields);
  d_node->alloc_sites = NULL;
  d_node->struct_clustering = NULL;
  d_node->reorder_only = false;

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

/* This performs a very coarse-grained escape analysis:  It checks all the
   arguments of all non-static functions (i.e. functions not defined in the
   current compilation) called and marks all such arguments and their
   types as "escaping".  

   Eventually we want to refine this analysis it two ways:  If the 
   non-static function is a library print function we may want to say 
   the call is okay, and if the type of the argument being passed is
   a simple type we may want to say the call is okay (because we can modify
   the argument as we wish before calling the function).

   It may be that we actually want to do the escape analysis after we
   build the data struct list.  */

static void
perform_escape_analysis (bitmap escaped_vars, bitmap escaped_types,
			  bitmap passed_types, bool type_check_p)
{
  struct cgraph_node *c_node;
  struct cgraph_edge *caller;
  tree var_decl;
  tree var_type;

  for (c_node = cgraph_nodes; c_node; c_node = c_node->next)
    {
      tree fn_decl = c_node->decl;

      if (strcmp (IDENTIFIER_POINTER (DECL_NAME (fn_decl)), "realloc")
	  == 0)
	continue;

      /* If function is non-static, check arguments at each place it is
	 called.  */

      if (! (TREE_STATIC (fn_decl))
	  || (TREE_PUBLIC (fn_decl)))
	{
	  for (caller = c_node->callers; caller; caller = caller->next_caller)
	    {
	      tree call_stmt = caller->call_stmt;
	      tree call_expr; /* = caller->call_stmt; */
	      tree arg_list; /* = TREE_OPERAND (call_expr, 1); */

	      if (TREE_CODE (call_stmt) == CALL_EXPR)
		call_expr = call_stmt;
	      else if (TREE_CODE (call_stmt) == MODIFY_EXPR
		       && TREE_CODE (TREE_OPERAND (call_stmt, 1)) == CALL_EXPR)
		call_expr = TREE_OPERAND (call_stmt, 1);
	      else
		abort ();

	      arg_list = TREE_OPERAND (call_expr, 1);
	      
	      for ( ; arg_list ; arg_list = TREE_CHAIN (arg_list))
		{
		  tree arg = TREE_VALUE (arg_list);
		  tree arg_type = TREE_TYPE (arg);

		  while (POINTER_TYPE_P (arg_type))
		    arg_type = TREE_TYPE (arg_type);

		  if (DECL_P (arg))
		    {
		      bitmap_set_bit (escaped_vars, arg->decl.uid);
		      if (type_check_p
			  && (AGGREGATE_TYPE_P (arg_type))
			  && !bitmap_bit_p (escaped_types, 
					    arg_type->type.uid))
			bitmap_set_bit (escaped_types, 
					arg_type->type.uid);
		    }
		}
	    }
	}
      else 
	{

	  for (var_decl = DECL_ARGUMENTS (c_node->decl); var_decl;
	       var_decl = TREE_CHAIN (var_decl))
	    {
	      var_type = TREE_TYPE (var_decl);
	      bitmap_set_bit (passed_types, var_type->type.uid);

	      while (POINTER_TYPE_P (var_type))
		{
		  var_type = TREE_TYPE (var_type);
		  bitmap_set_bit (passed_types, var_type->type.uid);
		}
	    }

	  /* For functions that are local to the current compilation, 
	     check arguments passed to any indirect function call.  */
	  for (caller = INDIRECT_CALLS (c_node);  caller;
	       caller = NEXT_INDIRECT_CALL (caller))
	    {
	      tree call_stmt = caller->call_stmt;
	      tree call_expr; /*  = caller->call_stmt; */
	      tree arg_list;
	      
	      if (TREE_CODE (call_stmt) == CALL_EXPR)
		call_expr = call_stmt;
	      else if (TREE_CODE (call_stmt) == MODIFY_EXPR
		       && TREE_CODE (TREE_OPERAND (call_stmt, 1)) == CALL_EXPR)
		call_expr = TREE_OPERAND (call_stmt, 1);
	      else
		abort ();

	      if (0)
		debug_gimple (call_expr);

	      if (call_expr)
		for (arg_list = TREE_OPERAND (call_expr, 1); arg_list;
		     arg_list = TREE_CHAIN (arg_list))
		  {
		    tree arg = TREE_VALUE (arg_list);
		    tree arg_type = TREE_TYPE (arg);

		    while (POINTER_TYPE_P (arg_type))
		      arg_type = TREE_TYPE (arg_type);

		    if (DECL_P (arg))
		      {
			bitmap_set_bit (escaped_vars, arg->decl.uid);
			if (type_check_p
			    && (AGGREGATE_TYPE_P (arg_type))
			    && !bitmap_bit_p (escaped_types, 
					      arg_type->type.uid))
			  bitmap_set_bit (escaped_types, 
					  arg_type->type.uid);
		      }
		  }
	      
	    }
	}
    }
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

static bool
program_redefines_malloc_p (void)
{
  bool redefines = false;
  struct cgraph_node *c_node;
  struct cgraph_node *c_node2;
  struct cgraph_edge *c_edge;
  tree fndecl;
  tree fndecl2;
  
  for (c_node = cgraph_nodes; c_node && !redefines; c_node = c_node->next)
    {
      fndecl = c_node->decl;
      
      if (strcmp (IDENTIFIER_POINTER (DECL_NAME (fndecl)), "malloc") == 0)
	{
	  for (c_edge = c_node->callers; c_edge && !redefines; 
	       c_edge = c_edge->next_caller)
	    {
	      c_node2 = c_edge->caller;
	      fndecl2 = c_node2->decl;
	      if (strstr (IDENTIFIER_POINTER (DECL_NAME (fndecl2)), "_malloc") != 0)
		redefines = true;
	    }
	}

      if (strcmp (IDENTIFIER_POINTER (DECL_NAME (fndecl)), "calloc") == 0)
	{
	  for (c_edge = c_node->callers; c_edge && !redefines; 
	       c_edge = c_edge->next_caller)
	    {
	      c_node2 = c_edge->caller;
	      fndecl2 = c_node2->decl;
	      if (strstr (IDENTIFIER_POINTER (DECL_NAME (fndecl2)), "_calloc") != 0)
		redefines = true;
	    }
	}

      if(strcmp (IDENTIFIER_POINTER (DECL_NAME (fndecl)), "realloc") == 0)
	{
	  for (c_edge = c_node->callers; c_edge && !redefines; 
	       c_edge = c_edge->next_caller)
	    {
	      c_node2 = c_edge->caller;
	      fndecl2 = c_node2->decl;
	      if (strstr (IDENTIFIER_POINTER (DECL_NAME (fndecl2)), "_realloc") != 0)
		redefines = true;
	    }
	}
    }
  
  return redefines;
}

/* Perform data structure peeling.  */
void
reorg_structs (void)
{
  struct struct_list *data_struct_list = NULL;
  struct struct_list *current_struct;
  struct cgraph_node *c_node;
  bool reordering_only = false;
  bool success;
  bool type_check_p;
  bitmap escaped_vars;
  bitmap escaped_types;
  bitmap passed_types;
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

  if (program_redefines_malloc_p ())
    return;

  type_check_p = true;
  escaped_vars = BITMAP_GGC_ALLOC ();
  escaped_types = BITMAP_GGC_ALLOC ();
  passed_types = BITMAP_GGC_ALLOC ();
  perform_escape_analysis (escaped_vars, escaped_types, passed_types, 
			   type_check_p);

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

      /* Stage 2:  Determine what, if anything, we want to transform and 
	 how.  */
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
    }

  /* Stage 3:  Do the actual transformation decided on in stage 2.  */
  
  collect_malloc_data ();

  do_reorg (data_struct_list, passed_types, escaped_types);

  /* Free up the memory allocated for the CRR_DS.  */

  for (current_struct = data_struct_list; current_struct;
       current_struct = current_struct->next)
    free_data_struct (current_struct->struct_data);

  if (vcg_dump)
    {
      fprintf (vcg_dump, "}\n");
      fclose (vcg_dump);
    }
}

void
add_call_to_malloc_list (tree fn_call)
{
  struct struct_tree_list *tmp;

  tmp = (struct struct_tree_list *) xmalloc (sizeof (struct struct_tree_list));
  tmp->data = fn_call;
  tmp->next = struct_reorg_malloc_list;

  struct_reorg_malloc_list = tmp;
}

