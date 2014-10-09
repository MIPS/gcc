/* Struct-reorg optimizations.
   Copyright (C) 2007-2014 Free Software Foundation, Inc.
   Contributed by Olga Golovanevsky <golovanevsky.olga@gmail.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "flags.h"
#include "tree-pass.h"
#include "print-tree.h" // for debug_tree 
#include "diagnostic.h"
#include "ipa-struct-reorg.h"
#include "tree-pretty-print.h"
#include "cgraph.h"
#include "internal-fn.h"
#include "gimple-expr.h"
#include "tree-ssa-alias.h"
#include "gimple.h"
#include "gimple-iterator.h"
#include "lto-streamer.h"
#include "data-streamer.h"
#include "tree-streamer.h"
#include "sbitmap.h"
#include "gimplify.h"
#include "attribs.h"
#include "stor-layout.h"
#include "stringpool.h"

/* Structure types that appear in symtab_node: 

   (1) if symtab_node is a cgraph_node, then structure 
       type might be a type of one or more of its formal 
       or actual parameters; 
   (2) if symtab_node is a varpool_node, then the node itself 
       or one of its sub-components might be of structure type.

   This structure is used to define visibility of structure types. */

struct struct_symbols_d {
  tree struct_decl;

  bool analized;
  vec<symtab_node *> symbols; 

  /* Number of fields in the structure.  */
  int num_fields;

  /* An array of the structure fields, indexed by field ID.  */
  struct field_entry *fields;

  /* A data structure representing a reorganization decision.  */
  struct field_cluster *struct_clustering;

  /* New types to replace the original structure type.  */
  vec<tree> new_types;
};

typedef struct struct_symbols_d * struct_symbols;

/* Symbols with structure types that are candidate for transformation.
   Based on their visibility, determined 
   during WPA, we decide if structure type can be trunsformed or not.  */

static vec<struct_symbols> struct_symbols_vec;

/* Given a type TYPE, this function returns the name of the type.  */

static const char *
get_type_name (tree type)
{
  if (! TYPE_NAME (type))
    return NULL;

  if (TREE_CODE (TYPE_NAME (type)) == IDENTIFIER_NODE)
    return IDENTIFIER_POINTER (TYPE_NAME (type));
  else if (TREE_CODE (TYPE_NAME (type)) == TYPE_DECL
	   && DECL_NAME (TYPE_NAME (type)))
    return IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (type)));
  else
    return NULL;
}

/* This function prints the SHIFT number of spaces to the DUMP_FILE.  */

static inline void
print_shift (unsigned HOST_WIDE_INT shift)
{
  unsigned HOST_WIDE_INT sh = shift;

  while (sh--)
    fprintf (dump_file, " ");
}

/* Print structure TYPE, its name, if it exists, and body.
   INDENT defines the level of indentation (similar
   to the option -i of indent command). SHIFT parameter
   defines a number of spaces by which a structure will
   be shifted right.  */

static void
dump_struct_type (tree type, unsigned HOST_WIDE_INT indent,
		   unsigned HOST_WIDE_INT shift)
{
  const char *struct_name;
  tree field;

  if (!type || !dump_file)
    return;

  if (TREE_CODE (type) != RECORD_TYPE)
    {
      print_generic_expr (dump_file, type, 0);
      return;
    }

  print_shift (shift);
  struct_name = get_type_name (type);
  fprintf (dump_file, "struct ");
  if (struct_name)
    fprintf (dump_file, "%s\n",struct_name);
  print_shift (shift);
  fprintf (dump_file, "{\n");

  for (field = TYPE_FIELDS (type); field;
       field = TREE_CHAIN (field))
    {
      unsigned HOST_WIDE_INT s = indent;
      tree f_type = TREE_TYPE (field);

      print_shift (shift);
      while (s--)
	fprintf (dump_file, " ");
      dump_struct_type (f_type, indent, shift + indent);
      fprintf(dump_file, " ");
      print_generic_expr (dump_file, field, 0);
      fprintf(dump_file, ";\n");
    }
  print_shift (shift);
  fprintf (dump_file, "}\n");
}

/* Function to print out struct_symbols_vec.  */

static void
print_struct_symbol_vec ()
{
  unsigned int i, j, k;
  symtab_node *sbl;
  struct_symbols symbols, str;
  tree new_type;

  if (!dump_file) 
    return;

  if (!struct_symbols_vec. exists ())
    fprintf (dump_file, "\nstruct_symbols_vec does not exist.");

  FOR_EACH_VEC_ELT (struct_symbols_vec, i, symbols)
    {
      str = struct_symbols_vec[i];
      fprintf (dump_file, "\nThe structure type is\n");
      dump_struct_type (str->struct_decl, 2, 0);
      fprintf (dump_file, "\n ");
      
      if (symbols->symbols.exists ())
	fprintf (dump_file, "\nSymbols are:\n");

      FOR_EACH_VEC_ELT (symbols->symbols, j, sbl)
	{
	  fprintf (dump_file, "%s  ", sbl->name ());
	  fprintf (dump_file, "with resolution %s  ",
		   ld_plugin_symbol_resolution_names[(int)sbl->resolution]);
	  fprintf (dump_file, "\n");
	}

      fprintf (dump_file, "\nNumber of fields: %d\n", str->num_fields);  
      for (int i = 0; i < str->num_fields; i++)
	{
	  print_generic_expr (dump_file, 
			      str->fields[i].decl, 0);
	  fprintf (dump_file, "\n");
	}
      if (symbols->new_types.exists ())
	  fprintf (dump_file, "\nNew types generated by this optimization are:\n");
	
      FOR_EACH_VEC_ELT (symbols->new_types, k, new_type)
	{
	  dump_struct_type (new_type, 2, 0);
	  fprintf (dump_file, "\n");
	}
    }
}

/* Forward declarations.  */
static bool is_equal_types (tree type1, tree type2);

/* This function looks for the entry with the type TYPE in 
   struct_symbols_vec. It returns an index of the entry, if it's found, 
   and -1 otherwise.  */

static inline int
is_in_struct_symbols_vec (tree type)
{
  unsigned int i;
  struct_symbols symbol; 
  if (struct_symbols_vec.exists ())
    {
      FOR_EACH_VEC_ELT (struct_symbols_vec, i, symbol)
	{
	  if (is_equal_types (symbol->struct_decl, type))
	      return (int)i;
	}
	  
    }
  return -1;
}

/* Add a symbol SYMBOL to the symbols of the I-th element of 
   struct_symbols_vec, if it's not already there.  */

static void
add_symbol_to_struct_symbols_vec (int i, symtab_node *symbol)
{
  gcc_assert ((i != -1));
  struct_symbols symbols = struct_symbols_vec[i];
  if (symbols->symbols.exists ())
    {
      unsigned int j;
      symtab_node *sbl;
      /* The simbol might be already in vector of symbols.  */
      FOR_EACH_VEC_ELT (symbols->symbols, j, sbl)
	{
	  if (sbl == symbol)
	    {
	      if (dump_file)
		{
		  fprintf (dump_file, "\nSymbol %s", sbl->name ());
		  fprintf (dump_file, " for type ");
		  print_generic_expr (dump_file, symbols->struct_decl, 0);		  
		  fprintf (dump_file, "\" already in symbols.");
		}
	      return;
	    }
	}
      symbols->symbols.safe_push (symbol);
    }
  else 
      symbols->symbols.safe_push (symbol);
}

/* Strip structure TYPE from pointers and arrays.  */

static inline tree
strip_type (tree type)
{
  gcc_assert (TYPE_P (type));

  while (POINTER_TYPE_P (type)
	 || TREE_CODE (type) == ARRAY_TYPE)
    type = TREE_TYPE (type);

  return  type;
}


/* Check whether the type of VAR is potential candidate for peeling.
   Returns true if yes, false otherwise.  If yes, TYPE_P will contain
   candidate type.  */

static bool
type_is_candidate (tree type, tree *type_p)
{
  *type_p = NULL;

  if (!type)
    return false;

  if (type)
    {
      type = TYPE_MAIN_VARIANT (strip_type (type));
      if (TREE_CODE (type) != RECORD_TYPE)
	  return false;
      else
	{
	  *type_p = type;
	  return true;
      }
    }
  else
    return false;
}

/* This function creates struct_symbols_d structure with the type TYPE 
   and inserts it into struct_symbols_vec.  */

static unsigned int 
add_struct_to_struct_symbols_vec (tree type)
{
  int i;
  struct_symbols symbs;

  type = TYPE_MAIN_VARIANT (type);
  i = is_in_struct_symbols_vec (type);

  if (i != -1)
    return i;

  symbs = XNEW (struct struct_symbols_d);
  symbs->struct_decl = type;
  symbs->symbols.create (0);
  symbs->new_types.create (0);
  symbs->num_fields = 0;
  symbs->struct_clustering = 0;
  symbs->fields = 0;
  symbs->analized = false;
  if (!struct_symbols_vec.exists ())
    struct_symbols_vec.create (0);
  struct_symbols_vec.safe_push (symbs);
  return struct_symbols_vec.length () - 1;
}

static void 
remove_type_from_struct_symbols_vec (struct_symbols symbols)
{
  if (symbols->symbols.exists ())
    symbols->symbols.release ();
  free (symbols);
}

/* This function returns true if two fields FIELD1 and FIELD2 are 
   semantically equal, and false otherwise.  */

static bool
compare_fields (tree field1, tree field2)
{
  if (DECL_NAME (field1) && DECL_NAME (field2))
    {
      const char *name1 = IDENTIFIER_POINTER (DECL_NAME (field1));
      const char *name2 = IDENTIFIER_POINTER (DECL_NAME (field2));

      gcc_assert (name1 && name2);

      if (strcmp (name1, name2))
	return false;
	
    }
  else if (DECL_NAME (field1) || DECL_NAME (field2))
    return false;

  if (!is_equal_types (TREE_TYPE (field1), TREE_TYPE (field2)))
    return false;

  return true;
}

/* This function is a temporary hack to overcome the types problem.
   When several compilation units are compiled together
   with -combine, the TYPE_MAIN_VARIANT of the same type
   can appear differently in different compilation units.
   Therefore this function first compares type names.
   If there are no names, structure bodies are recursively
   compared.  */

static bool
is_equal_types (tree type1, tree type2)
{
  const char * name1,* name2;

  if ((!type1 && type2)
      ||(!type2 && type1))
    return false;

  if (!type1 && !type2)
    return true;

  if (TREE_CODE (type1) != TREE_CODE (type2))
    return false;

  if (type1 == type2)
      return true;

  if (TYPE_MAIN_VARIANT (type1) == TYPE_MAIN_VARIANT (type2))
      return true;

  name1 = get_type_name (type1);
  name2 = get_type_name (type2);

  if (name1 && name2)
    return strcmp (name1, name2) == 0;

  switch (TREE_CODE (type1))
    {
    case POINTER_TYPE:
    case REFERENCE_TYPE:
      {
	return is_equal_types (TREE_TYPE (type1), TREE_TYPE (type2));
      }
      break;

    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
    case ENUMERAL_TYPE:
      {
	tree field1, field2;

	/* Compare fields of structure.  */
	for (field1 = TYPE_FIELDS (type1), field2 = TYPE_FIELDS (type2);
	     field1 && field2;
	     field1 = TREE_CHAIN (field1), field2 = TREE_CHAIN (field2))
	  {
	    if (!compare_fields (field1, field2))
	      return false;
	  }
	if (field1 || field2)
	  return false;
	else
	  return true;
      }
      break;

    case INTEGER_TYPE:
      {
	if (TYPE_UNSIGNED (type1) == TYPE_UNSIGNED (type2)
	    && TYPE_PRECISION (type1) == TYPE_PRECISION (type2))
	  return true;
      }
      break;

    case ARRAY_TYPE:
      {
	tree d1, d2;
	tree max1, min1, max2, min2;

	if (!is_equal_types (TREE_TYPE (type1), TREE_TYPE (type2)))
	  return false;

	d1 = TYPE_DOMAIN (type1);
	d2 = TYPE_DOMAIN (type2);

	if (!d1 || !d2)
	  return false;

	max1 = TYPE_MAX_VALUE (d1);
	max2 = TYPE_MAX_VALUE (d2);
	min1 = TYPE_MIN_VALUE (d1);
	min2 = TYPE_MIN_VALUE (d2);

	if (max1 && max2 && min1 && min2
	    && TREE_CODE (max1) == TREE_CODE (max2)
	    && TREE_CODE (max1) == INTEGER_CST
	    && TREE_CODE (min1) == TREE_CODE (min2)
	    && TREE_CODE (min1) == INTEGER_CST
	    && tree_int_cst_equal (max1, max2)
	    && tree_int_cst_equal (min1, min2))
	  return true;
      }
      break;

    default:
	gcc_unreachable();
    }

  return false;
}

/* Given a structure declaration STRUCT_DECL, and number of fields
   in the structure NUM_FIELDS, this function creates and returns
   corresponding field_entry's.  */

static struct field_entry *
get_fields (tree struct_decl, int num_fields)
{
  struct field_entry *list;
  tree t = TYPE_FIELDS (struct_decl);
  int idx = 0;

  list = XNEWVEC (struct field_entry, num_fields);

  for (idx = 0 ; t; t = TREE_CHAIN (t), idx++)
    if (TREE_CODE (t) == FIELD_DECL)
      {
	list[idx].index = idx;
	list[idx].decl = t;
	list[idx].acc_sites = 0; //htab_create (32, field_acc_hash, field_acc_eq, NULL);
	list[idx].count = 0;
	list[idx].field_mapping = NULL_TREE;
      }

  return list;
} 


/* This function adds a structure TYPE to the vector of structures,
   if it's not already there.  */

static void
analyze_structure (struct_symbols str)
{
  str->num_fields = fields_length (str->struct_decl);
  str->fields = get_fields (str->struct_decl, str->num_fields);
}

/* This function returns type of VAR.  */

static inline tree
get_type_of_var (tree var)
{
  if (!var)
    return NULL;

  if (TREE_CODE (var) == PARM_DECL)
      return DECL_ARG_TYPE (var);
  else
    return TREE_TYPE (var);
}

/* Check whether the type of VAR is potential candidate for peeling.
   Returns true if yes, false otherwise.  If yes, TYPE_P will contain
   candidate type.  */

static bool
var_is_candidate (tree var, tree *type_p)
{
  tree type;

  *type_p = NULL;

  if (!var)
    return false;

  type = get_type_of_var (var);

  return type_is_candidate (type, type_p);
}

/* Add a symbol SYMBOL to the vector of symbols corresponding 
   to the type TYPE in struct_symbols_vec.  */

/* static void
add_symbol (tree type, symtab_node *symbol)
{

  int i = is_in_struct_symbols_vec (type);
  if (i != -1)
  {
    add_symbol_to_struct_symbols_vec (i, symbol);
  } else {
    i = add_struct_to_struct_symbols_vec (type);
    add_symbol_to_struct_symbols_vec (i, symbol);
  }
  } */

/* This function looks for structure types instantiated in the program.
   The candidate types are added to the structures vector.  */

static void
build_data_structure (void)
{
  tree var, type, p;
  struct varpool_node *current_var;
  struct cgraph_node *c_node;
  //struct cgraph_node *callee;
  //struct cgraph_edge *edge;

  /* Check global and static (global and local) variables.  */
  FOR_EACH_VARIABLE (current_var) 
    {
      var = current_var->decl;

      if (dump_file) {
	fprintf (dump_file, "\nConsidering var: ");
	print_generic_expr (dump_file, var, 0);
	fprintf (dump_file, "\n");
      }

      if (var_is_candidate (var, &type)) {
	if (dump_file) {
	  fprintf (dump_file, " Type ");
	  print_generic_expr (dump_file, type, 0);
	  fprintf (dump_file,  " is candidate. Adding to list of candidates...\n");

	}
	add_symbol_to_struct_symbols_vec (
					  add_struct_to_struct_symbols_vec (type), 
					  current_var);
      } else {
	if (dump_file) {
	  fprintf (dump_file, " Type ");
	  print_generic_expr (dump_file, type, 0);
	  fprintf (dump_file,  "is not candidate. Skipping...\n");
	}
      }
    }

  /* Now add structures that are types of function parameters and
     local variables.  */
  FOR_EACH_FUNCTION(c_node)
    {
      struct function *fn = DECL_STRUCT_FUNCTION (c_node->decl);
      unsigned ix; 


      if (dump_file) {
	fprintf (dump_file, "\nConsidering c_node: ");
	dump_cgraph_node (dump_file, c_node);
	fprintf (dump_file, "\n");
      }


      if (c_node->decl) {
	if (dump_file) {
	  fprintf (dump_file, "\nFunc:");
	  print_generic_expr (dump_file, c_node->decl, 0);
	}
      }
      
      
      for (p = TYPE_ARG_TYPES (TREE_TYPE(c_node->decl)); p; p = TREE_CHAIN (p))
	{
	  tree arg_type = TREE_VALUE (p);
	  fprintf (dump_file, "\n\tArg:");
	  print_generic_expr (dump_file, arg_type, 0);	  
	  //debug_tree (arg_type);
	  if (type_is_candidate (arg_type, &type)) {
	    if (dump_file) {
	      fprintf (dump_file, " Type ");
	      print_generic_expr (dump_file, type, 0);
	      fprintf (dump_file,  " is candidate. Adding to list of candidates...\n");	      
	    }
	    add_symbol_to_struct_symbols_vec (
					  add_struct_to_struct_symbols_vec (type), 
					  c_node);

	  } else {
	    if (dump_file) {
	      fprintf (dump_file, " Type ");
	      print_generic_expr (dump_file, type, 0);
	      fprintf (dump_file,  "is not candidate. Skipping...\n");
	    }	
	  }
	}

      if (fn) 
	{
	  /* Check function local variables.  */
	  FOR_EACH_LOCAL_DECL (fn, ix, var)
	    {
	      if (dump_file) {
		fprintf (dump_file, "\nConsidering var: ");
		print_generic_expr (dump_file, var, 0);
		fprintf (dump_file, "\n");
	      }

	      if (var_is_candidate (var, &type)) {
		if (dump_file) {
		  fprintf (dump_file, " Type ");
		  print_generic_expr (dump_file, type, 0);
		  fprintf (dump_file,  " is candidate. Adding to list of candidates...\n");
		}
		add_struct_to_struct_symbols_vec (type);
	      } else {
		if (dump_file) {
		  fprintf (dump_file, " Type ");
		  print_generic_expr (dump_file, type, 0);
		  fprintf (dump_file,  "is not candidate. Skipping...\n");
		}
	      }
	    }
	}
    }
  /* Scan parameters of functions that have no forward declarations.  */
  FOR_EACH_DEFINED_FUNCTION(c_node)
  {
    /* struct function *fn = DECL_STRUCT_FUNCTION (c_node->decl);
    basic_block bb;
      gimple_stmt_iterator gsi; */
    
    struct cgraph_edge *cs;
    for (cs = c_node->callees; cs; cs = cs->next_callee)
      {
      
	gimple stmt = cs->call_stmt;

	if (is_gimple_call(stmt))
	  {
	    tree target = gimple_call_fndecl (stmt);
	    tree op;
	    unsigned int i;
	    
	    if (dump_file) {
	      fprintf (dump_file, "\nTarget:");
	      print_generic_expr (dump_file, target, 0);
	    }

	    p = TYPE_ARG_TYPES (TREE_TYPE(target));
	    if (p)
	      {
		if (dump_file) 
		  fprintf (dump_file, " Continuing...");
		continue;
	      }

	    for (i = 0; i < gimple_num_ops (stmt); i++) 
	      {
		op = gimple_op (stmt, i);

		if (dump_file) 
		  {
		    fprintf (dump_file, "\nOp %d:", i);
		    print_generic_expr (dump_file, op, 0);
		    //debug_tree(op);
		  }

		if (var_is_candidate (op, &type)) {
		  if (dump_file)
		    {
		      fprintf (dump_file, " Type ");
		      print_generic_expr (dump_file, type, 0);
		      fprintf (dump_file,  " is candidate. Adding to list of candidates...\n");
		    }
		  add_symbol_to_struct_symbols_vec (
					  add_struct_to_struct_symbols_vec (type), 
					  cs->callee);
		  // add_structure (type);
		  // add_symbol (type, cs->callee);
		} else {
		  if (dump_file)
		    {
		      fprintf (dump_file, " Type ");
		      print_generic_expr (dump_file, type, 0);
		      fprintf (dump_file,  "is not candidate. Skipping...\n");
		    }
		}
	      } 
	  }
      }
  }
  print_struct_symbol_vec ();
}

/* This function collects structures potential
   for peeling transformation, and inserts
   them into structures hashtable.  */

static void
collect_structures (void)
{  
  /* Build data structures hashtable of all data structures
     in the program.  */
  build_data_structure ();
}
/* This function generates cluster substructure that contains FIELDS.
   The cluster added to the set of clusters of the structure STR.  */

static void
gen_cluster (sbitmap fields, struct_symbols str)
{
  struct field_cluster *crr_cluster = XCNEW (struct field_cluster);

  crr_cluster->sibling = str->struct_clustering;
  str->struct_clustering = crr_cluster;
  crr_cluster->fields_in_cluster = fields;
}

/* This function peels a field with the index I from the structure DS.  */

static void
peel_field (int i, struct_symbols ds)
{
  struct field_cluster *crr_cluster = XCNEW (struct field_cluster);

  crr_cluster->sibling = ds->struct_clustering;
  ds->struct_clustering = crr_cluster;
  crr_cluster->fields_in_cluster =
    sbitmap_alloc ((unsigned int) ds->num_fields);
  bitmap_clear (crr_cluster->fields_in_cluster);
  bitmap_set_bit (crr_cluster->fields_in_cluster, i);
}

/* For now we unconditionally peel structure into separate fields. 
   In future we intend to add profile info and/or static heuristics 
   to differentiate peeling process. */

static void
peel_fields (struct_symbols str)
{
  sbitmap fields_left = sbitmap_alloc (str->num_fields);
  int i;

  bitmap_ones (fields_left);

  str->struct_clustering = NULL;

  for (i = 0; i < str->num_fields; i++)
    {
      bitmap_clear_bit (fields_left, i);
      peel_field (i, str);
    }

  i = bitmap_first_set_bit (fields_left);
  if (i != -1)
    gen_cluster (fields_left, str);
  else
    sbitmap_free (fields_left);
}

/* This function decomposes original structure into substructures,
   i.e.clusters.  */

static void
peel_structs (void)
{
  unsigned int i;
  struct_symbols str;

  if (!struct_symbols_vec.exists ()) 
    return;

  FOR_EACH_VEC_ELT (struct_symbols_vec, i, str)
    peel_fields (struct_symbols_vec[i]);
}

/* This function updates field_mapping of FIELDS in CLUSTER with NEW_TYPE.  */

static inline void
update_fields_mapping (struct field_cluster *cluster, tree new_type,
		       struct field_entry * fields, int num_fields)
{
  int i;

  for (i = 0; i < num_fields; i++)
    if (bitmap_bit_p (cluster->fields_in_cluster, i))
	fields[i].field_mapping = new_type;
}

/* This functions builds structure with FIELDS,
   NAME and attributes similar to ORIG_STRUCT.
   It returns the newly created structure.  */

static tree
build_basic_struct (tree fields, tree name, tree orig_struct)
{
  tree attributes = NULL_TREE;
  tree ref = 0;
  tree x;

  if (TYPE_ATTRIBUTES (orig_struct))
    attributes = unshare_expr (TYPE_ATTRIBUTES (orig_struct));
  ref = make_node (RECORD_TYPE);
  TYPE_SIZE (ref) = 0;
  decl_attributes (&ref, attributes, (int) ATTR_FLAG_TYPE_IN_PLACE);
  TYPE_PACKED (ref) = TYPE_PACKED (orig_struct);
  for (x = fields; x; x = TREE_CHAIN (x))
    {
      DECL_CONTEXT (x) = ref;
      DECL_PACKED (x) |= TYPE_PACKED (ref);
    }
  TYPE_FIELDS (ref) = fields;
  layout_type (ref);
  TYPE_NAME (ref) = name;

  return ref;
}

/* This function copies FIELDS from CLUSTER into TREE_CHAIN as part
   of preparation for new structure building. NUM_FIELDS is a total
   number of fields in the structure. The function returns newly
   generated fields.  */

static tree
create_fields (struct field_cluster * cluster,
	       struct field_entry * fields, int num_fields)
{
  int i;
  tree new_types = NULL_TREE;
  tree last = NULL_TREE;

  for (i = 0; i < num_fields; i++)
    if (bitmap_bit_p (cluster->fields_in_cluster, i))
      {
	tree new_decl = NULL_TREE;
		
	new_decl = copy_node (fields[i].decl);
	
	if (!new_types)
	  new_types = new_decl;
	else
	  TREE_CHAIN (last) = new_decl;
	last = new_decl;
      }

  TREE_CHAIN (last) = NULL_TREE;

  return new_types;

}

/* This function creates a cluster name. The name is based on
   the original structure name, if it is present. It has a form:

   <original_struct_name>_sub.<CLUST_NUM>

   The original structure name is taken from the type of DECL.
   If an original structure name is not present, it's generated to be:

   struct.<STR_NUM>

   The function returns identifier of the new cluster name.  */

static inline tree
gen_cluster_name (tree decl, int clust_num, int str_num)
{
  const char * orig_name = get_type_name (decl);
  char * tmp_name = NULL;
  char * prefix;
  char * new_name;
  size_t len;

  if (!orig_name)
    ASM_FORMAT_PRIVATE_NAME(tmp_name, "struct", str_num);

  len = strlen (tmp_name ? tmp_name : orig_name) + strlen ("_sub");
  prefix = XALLOCAVEC (char, len + 1);
  memcpy (prefix, tmp_name ? tmp_name : orig_name,
	  strlen (tmp_name ? tmp_name : orig_name));
  strcpy (prefix + strlen (tmp_name ? tmp_name : orig_name), "_sub");

  ASM_FORMAT_PRIVATE_NAME (new_name, prefix, clust_num);
  return get_identifier (new_name);
}

/* This function creates new structure types to replace original type,
   indicated by STR->decl. The names of the new structure types are
   derived from the original structure type. If the original structure
   type has no name, we assume that its name is 'struct.<STR_NUM>'.  */

static void
create_new_type (struct_symbols str, int *str_num)
{
  int cluster_num = 0;

  struct field_cluster *cluster = str->struct_clustering;
  while (cluster)
    {
      tree  name;
      tree fields;
      tree new_type;
      cluster_num++;
      
      name = gen_cluster_name (str->struct_decl, cluster_num,
				     *str_num);      
      fields = create_fields (cluster, str->fields,
			      str->num_fields);
      new_type = build_basic_struct (fields, name, str->struct_decl);
      update_fields_mapping (cluster, new_type,
			     str->fields, str->num_fields);
      (str->new_types).safe_push (new_type);
      cluster = cluster->sibling;
    }
  (*str_num)++;
}

/* This function creates new types to replace old structure types.  */

static void
create_new_types (void)
{
  struct_symbols str;
  unsigned int i;
  int str_num = 0;

  FOR_EACH_VEC_ELT (struct_symbols_vec, i, str)
    create_new_type (str, &str_num);
}

static void
analyze_structs (void)
{
  unsigned int i;
  struct_symbols str;

  FOR_EACH_VEC_ELT (struct_symbols_vec, i, str)
    analyze_structure (struct_symbols_vec[i]);
}


static void
generate_new_types (void)
{
  if (!struct_symbols_vec. exists ()) 
    return;

  analyze_structs ();
  peel_structs ();
  create_new_types ();
}

static unsigned int
propagate (void)
{
  unsigned int i, j;
  symtab_node *sbl;
  struct_symbols symbols;
  bool escape;

  if (dump_file)
    fprintf (dump_file, "\n\nPropagating LTO info...");
    
  if (!struct_symbols_vec. exists ()) 
    {
      if (dump_file)
	fprintf (dump_file, "\nstruct_symbols_vec does not exist.");
    }

  if (dump_file)
    fprintf (dump_file, "\nnumber of types is %d", struct_symbols_vec.length ());

  print_struct_symbol_vec ();  

  for (i = 0; struct_symbols_vec.iterate (i, &symbols);)
    {
      if (dump_file)
	{
	  fprintf (dump_file, "\nConsidering symbols for type ");
	  print_generic_expr (dump_file, struct_symbols_vec[i]->struct_decl, 0);
	}
      
      escape = false;
      FOR_EACH_VEC_ELT (symbols->symbols, j, sbl)
	{
	  if (sbl->resolution != LDPR_PREVAILING_DEF_IRONLY) 
	    {
	      if (dump_file)
		{
		  fprintf (dump_file, "\nSymbol %s", sbl->name ());
		  fprintf (dump_file, " has resolution %s.\nType ",
			   ld_plugin_symbol_resolution_names[(int)sbl->resolution]);
		  print_generic_expr (dump_file, struct_symbols_vec[i]->struct_decl, 0);
		  fprintf (dump_file, "escapes.\n");
		}
	      escape = true;
	      break;
	    }
	}
      if (escape)
	{
	  /* Delete i's type from struct_symbols_vec.  */
	  remove_type_from_struct_symbols_vec (symbols);
	  struct_symbols_vec.ordered_remove (i);
	}
      else
	i++;
    } 

  if (dump_file)
    fprintf (dump_file, "\nAfter propagation:\n");

  if (!struct_symbols_vec. exists ()) 
    {
      if (dump_file)
	fprintf (dump_file, "\nstruct_symbols_vec does not exist.");
    }

  if (dump_file)
    fprintf (dump_file, "\nnumber of types is %d", struct_symbols_vec.length ());

  generate_new_types ();
  print_struct_symbol_vec ();

  return 0;
}

struct symtab_node *
get_prevailing_symbol (struct symtab_node * node)
{
  struct symtab_node *e;
  struct symtab_node *next;
  for (e = node; e; e = next)
    {
      next = e->previous_sharing_asm_name;
      if (!next)
	return e;
    }

  return NULL;
}

/* Read section in file FILE_DATA of length LEN with data DATA.  */

static void
struct_reorg_read_section (struct lto_file_decl_data *file_data, const char *data,
		       size_t len)
{
  const struct lto_function_header *header =
    (const struct lto_function_header *) data;
  const int cfg_offset = sizeof (struct lto_function_header);
  const int main_offset = cfg_offset + header->cfg_size;
  const int string_offset = main_offset + header->main_size;
  struct data_in *data_in;
  struct lto_input_block ib_main;
  unsigned int i;
  unsigned int count;
  lto_symtab_encoder_t encoder;

  LTO_INIT_INPUT_BLOCK (ib_main, (const char *) data + main_offset, 0,
			header->main_size);

  data_in =
    lto_data_in_create (file_data, (const char *) data + string_offset,
			header->string_size, vNULL);
  count = streamer_read_uhwi (&ib_main);
  encoder = file_data->symtab_node_encoder;

  for (i = 0; i < count; i++)
    {
      tree struct_decl;
      unsigned int n_symbols;
      unsigned int index;
      struct symtab_node *symbol;
      int j;

      struct_decl = stream_read_tree (&ib_main, data_in);
      j = is_in_struct_symbols_vec (struct_decl);
      if (j == -1)	
	j = add_struct_to_struct_symbols_vec (struct_decl);
      n_symbols = streamer_read_uhwi (&ib_main);

      while (n_symbols)
	{	      
	  index = streamer_read_uhwi (&ib_main);
	  symbol = lto_symtab_encoder_deref (encoder, index);

	  /* For each symbol we keep its prevailing symbol, 
             since non-previaling symbols are replaced by 
	     prevailing ones just after lto passes read in 
	     their summaries. So we won't be able to see 
	     non-prevailing symbols in propagation.  */
	  add_symbol_to_struct_symbols_vec (j, 
					    get_prevailing_symbol (symbol));
	  n_symbols--;
	}
    }

  lto_free_section_data (file_data, LTO_section_ipa_struct_reorg, NULL, data,
			 len);
  lto_data_in_delete (data_in);
} 

/* Deserialize the ipa info for lto.  */

static void
struct_reorg_read_summary (void)
{
  struct lto_file_decl_data **file_data_vec = lto_get_file_decl_data ();
  struct lto_file_decl_data *file_data;
  unsigned int j = 0; 

  while ((file_data = file_data_vec[j++]))
    {
      size_t len;
      const char *data = lto_get_section_data (file_data, LTO_section_ipa_struct_reorg, NULL, &len);

      if (data) {
        struct_reorg_read_section (file_data, data, len);
      }
    }

  print_struct_symbol_vec ();  
}

/* Serialize the ipa info for lto.  */

static void
struct_reorg_write_summary (void)
{
  struct output_block *ob = create_output_block (LTO_section_ipa_struct_reorg);
  lto_symtab_encoder_t encoder;
  unsigned int i, j;
  symtab_node *sbl;
  struct_symbols symbols;


  if (!struct_symbols_vec.exists () || struct_symbols_vec.is_empty ())
    {
      destroy_output_block (ob);
      return;
    } 

  encoder = ob->decl_state->symtab_node_encoder;
  ob->cgraph_node = NULL;
  streamer_write_uhwi (ob, struct_symbols_vec.length ());

  if (dump_file)
    {
      fprintf (dump_file, "\n\nStruct-reorg write summary starts...");
      fprintf (dump_file, "\nNumber of structure types is %d\n", struct_symbols_vec.length ());
    }

  FOR_EACH_VEC_ELT (struct_symbols_vec, i, symbols)
    {
      gcc_assert (struct_symbols_vec[i]->struct_decl);
      stream_write_tree (ob, struct_symbols_vec[i]->struct_decl, true);

      if (dump_file)
	  print_generic_expr (dump_file, struct_symbols_vec[i]->struct_decl, 0);
      
      if (!symbols->symbols.exists () || struct_symbols_vec.is_empty ())
	{
	  streamer_write_zero (ob);
	  if (dump_file)
	    fprintf (dump_file, "\nNo symbols found...");
	}
      else
	{
	  streamer_write_uhwi (ob, symbols->symbols.length ());
	  if (dump_file)
	      fprintf (dump_file, "\nNumber of symbols for this type is %d\n", symbols->symbols.length ());
	  FOR_EACH_VEC_ELT (symbols->symbols, j, sbl)
	    {
	      unsigned int node_ref = lto_symtab_encoder_encode (encoder, sbl);
	      streamer_write_uhwi (ob, node_ref);
	      if (dump_file)
		{
		  fprintf (dump_file, "\nnode_ref for symbol ");
		  fprintf (dump_file, "%s  ", sbl->name ());
		  fprintf (dump_file, "is %d", node_ref);
		}

	    }
	}
    }

  streamer_write_char_stream (ob->main_stream, 0);
  produce_asm (ob, NULL);
  destroy_output_block (ob);
  
} 

/* */

void
struct_reorg_var_transform (varpool_node *vnode)
{
  if (dump_file)
    fprintf (dump_file, "\nStruct_reorg is transforming %s variable.", vnode->name ());      
}

/* Analyze each function in the cgraph. */

static void
struct_reorg_generate_summary (void)
{

  collect_structures ();

}

static bool
gate_struct_reorg (void)
{
  return (flag_ipa_struct_reorg
	  /* Don't bother doing anything if the program has errors.  */
	  && !seen_error ());
}

namespace {

const pass_data pass_data_ipa_struct_reorg =
{
  IPA_PASS, /* type */
  "struct-reorg", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  true, /* has_gate */
  true, /* has_execute */
  TV_IPA_STRUCT_REORG, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_ipa_struct_reorg : public ipa_opt_pass_d
{
public:
  pass_ipa_struct_reorg (gcc::context *ctxt)
    : ipa_opt_pass_d (pass_data_ipa_struct_reorg, ctxt,
		      struct_reorg_generate_summary, /* generate_summary */
		      struct_reorg_write_summary, /* write_summary */
		      struct_reorg_read_summary, /* read_summary */
		      NULL, /* write_optimization_summary */
		      NULL, /* read_optimization_summary */
		      NULL, /* stmt_fixup */
		      0, /* function_transform_todo_flags_start */
		      NULL, /* function_transform */
		      struct_reorg_var_transform) /* variable_transform */
  {}

  /* opt_pass methods: */
  bool gate () { return gate_struct_reorg (); }
  unsigned int execute () { return propagate (); }

}; // class pass_ipa_struct_reorg

} // anon namespace

ipa_opt_pass_d *
make_pass_ipa_struct_reorg (gcc::context *ctxt)
{
  return new pass_ipa_struct_reorg (ctxt);
}

