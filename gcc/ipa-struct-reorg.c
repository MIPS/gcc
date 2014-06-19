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

/* Structure types that appear in symtab_node: 

   (1) if symtab_node is a cgraph_node, then structure 
       type might be a type of one or more of its formal 
       or actual parameters; 
   (2) if symtab_node is a varpool_node, then the node itself 
       or one of its sub-components might be of structure type.

   This structure is used to define visibility of structure types. */

struct struct_symbols_d {
  tree struct_decl;
  vec<symtab_node *> symbols; 
};

typedef struct struct_symbols_d * struct_symbols;

/* Symbols with structure types that are candidate for ftansformation.
   Based on their visibility, determined 
   during WPA, we decide if structure type can be trunsformed or not.  */

static vec<struct_symbols> struct_symbols_vec;

/* Function to print out struct_symbols_vec.  */

static void
print_struct_symbol_vec ()
{
  unsigned int i, j;
  symtab_node *sbl;
  struct_symbols symbols;
  if (!dump_file) 
    return;

  if (!struct_symbols_vec. exists ())
    fprintf (dump_file, "\nstruct_symbols_vec does not exist.");

  FOR_EACH_VEC_ELT (struct_symbols_vec, i, symbols)
    {
      if (!symbols->symbols.exists ())
	{
	  fprintf (dump_file, "\nThere is no symbols for type");
	  print_generic_expr (dump_file, struct_symbols_vec[i]->struct_decl, 0);
	}
      else
	{
	  fprintf (dump_file, "\nSymbols for type ");
	  print_generic_expr (dump_file, struct_symbols_vec[i]->struct_decl, 0);
	  fprintf (dump_file, " are:\n ");

	  FOR_EACH_VEC_ELT (symbols->symbols, j, sbl)
	    {
	      fprintf (dump_file, "%s  ", sbl->name ());
	    }
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
add_symbol_to_struct_symbols_vec (unsigned int i, symtab_node *symbol)
{
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
  gcc_assert (is_in_struct_symbols_vec (type) == -1);
  struct_symbols symbs = XNEW (struct struct_symbols_d);
  symbs->struct_decl = type;
  symbs->symbols.create (0);
  if (!struct_symbols_vec.exists ())
    struct_symbols_vec.create (0);
  struct_symbols_vec.safe_push (symbs);
  return struct_symbols_vec.length () - 1;
}

/* Vector of structures to be transformed.  */
typedef struct data_structure structure;
vec<structure, va_heap, vl_ptr> *structures;

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

/* This function looks for d_str, represented by TYPE, in the structures
   vector. If found, it returns an index of found structure. Otherwise
   it returns a length of the structures vector.  */

static unsigned
find_structure (tree type)
{
  d_str str;
  unsigned i;

  type = TYPE_MAIN_VARIANT (type);

  FOR_EACH_VEC_ELT ((*structures), i, str)
    if (is_equal_types (str->decl, type))
      return i;

  return structures->length ();
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
add_structure (tree type)
{
  structure node;
  unsigned i;
  int num_fields;

  type = TYPE_MAIN_VARIANT (type);

  i = find_structure (type);

  if (i != structures->length ()) {
    if (dump_file)
      {
	fprintf (dump_file, "\nType \"");
	print_generic_expr (dump_file, type, 0);
	fprintf (dump_file, "\" already in structures.");
      }
    return;
  }

  num_fields = fields_length (type);
  node.decl = type;
  node.num_fields = num_fields;
  node.fields = get_fields (type, num_fields);
  node.struct_clustering = NULL;
  node.accs = 0; //htab_create (32, acc_hash, acc_eq, NULL);
  node.new_types = 0; // node.new_types.create (num_fields);
  node.count = 0;

  structures->safe_push (node);

  if (dump_file)
    {
      fprintf (dump_file, "\nAdding data structure \"");
      print_generic_expr (dump_file, type, 0);
      fprintf (dump_file, "\" to data_struct_list.");
    }
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

static void 
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
}

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
	add_structure (type);
	add_symbol (type, current_var);
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
	    add_structure (type);
	    add_symbol (type, c_node);
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
		add_structure (type);
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
		  add_structure (type);
		  add_symbol (type, cs->callee);
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
  vec_alloc (structures, 32);
  
  /* Build data structures hashtable of all data structures
     in the program.  */
  build_data_structure ();
}

static unsigned int
propagate (void)
{
  return 0;
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
	  add_symbol_to_struct_symbols_vec (j, symbol);
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
		      NULL) /* variable_transform */
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

