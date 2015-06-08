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
#include "hashtab.h"
#include "varasm.h"
#include "tree-ssa-operands.h"
#include "tree-into-ssa.h"
#include "gimple-ssa.h"
#include "tree-phinodes.h"
#include "ssa-iterators.h"
#include "gimple-pretty-print.h"
#include "tree-cfg.h"
#include "tree-ssanames.h"
#include "gimple-walk.h"
#include "tree-cfgcleanup.h"
#include "cfgloop.h"

typedef enum escape_type {
  NONESCAPE = 0,
  CUSTOM_ALLOC_SITE,
  NUM_ESCAPE_TYPES /* Not used, must be last.  */
} escape_t;

const char *escape_type_names[NUM_ESCAPE_TYPES] =
{
  "nonescape",
  "custom_alloc_site"
};

typedef enum struct_reorg_give_up_reason
{
  STRUCT_REORG_DO_NOT_GIVE_UP = 0,
  STRUCT_REORG_GIVE_UP_ASM_STMT,
  STRUCT_REORG_NUM_OF_GIVE_UP_REASONS /* Not used, must be last.  */
} struct_reorg_give_up_type;

const char *struct_reorg_give_up_names[STRUCT_REORG_NUM_OF_GIVE_UP_REASONS] =
{
  "struct_reorg_do_not_give_up",
  "struct_reorg_give_up_asm_stmt"
};

static unsigned 
struct_reorg_give_up = (1 << STRUCT_REORG_DO_NOT_GIVE_UP);

/* Structure types that appear in symtab_node: 

   (1) if symtab_node is a cgraph_node, then structure 
       type might be a type of one or more of its formal 
       or actual parameters; 
   (2) if symtab_node is a varpool_node, then the node itself 
       or one of its sub-components might be of structure type.

   This structure is used to define visibility of structure types. */

struct struct_symbols_d {

  /* The following fields are part of serialization.  */

  tree struct_decl;
  unsigned int  escape;
  vec<symtab_node *> symbols; 

  /* The fields below do not participate in serialization.  */

  /* Number of fields in the structure.  */
  int num_fields;

  /* An array of the structure fields, indexed by field ID.  */
  struct field_entry *fields;

  /* A data structure representing a reorganization decision.  */
  struct field_cluster *struct_clustering;

  /* Non-field accesses of the structure.  */
  htab_t accs;

  /* New types to replace the original structure type.  */
  vec<tree> new_types;
};

typedef struct struct_symbols_d * struct_symbols;

/* Symbols with structure types that are candidate for transformation.
   Based on their visibility, determined 
   during WPA, we decide if structure type can be trunsformed or not.  */

static vec<struct_symbols> struct_symbols_vec;

static inline void 
add_escape (struct_symbols str, unsigned int escape)
{
  str->escape = str->escape | escape;
}

static void
print_escape (struct_symbols str)
{
  unsigned int escape = str->escape;
  int j = 0;
  
  if (!dump_file)
    return;

  for ( unsigned int i = 0; i < NUM_ESCAPE_TYPES; i++)
    {
      if (escape & (1 << i))
	{
	  if (j)
	    fprintf (dump_file, ", %s", escape_type_names[i]);
	  else
	    fprintf (dump_file, "\nEscape: %s", escape_type_names[i]);
	  j++;
	}
    }
}

static void
print_give_up_reason (void)
{
  int j = 0;
  
  if (!dump_file)
    return;

  if (struct_reorg_give_up == (1 << STRUCT_REORG_DO_NOT_GIVE_UP))
    {
      fprintf (dump_file, "\nNo reason to give up yet...\n");
    }
  else
    {
      /* Skipping 0 index since we won't give up in this case.  */
      for ( unsigned int i = 1; i < STRUCT_REORG_NUM_OF_GIVE_UP_REASONS; i++)
	{
	  if (struct_reorg_give_up & (1 << i))
	    {
	      if (j)
		fprintf (dump_file, ", %s", struct_reorg_give_up_names[i]);
	      else
		fprintf (dump_file, "\nGive up reasons: %s", struct_reorg_give_up_names[i]);
	      j++;
	    }
	}
      fprintf (dump_file, "\n");      
    }
}

/* New variables created by this optimization.
   When are doing struct peeling, each variable of
   the original struct type will be replaced by
   the set of new variables corresponding to
   the new structure types.  */
struct new_var_data {
  tree orig_var;
  /* Vector of new variables.  */
  vec<tree> new_vars;
};

typedef struct new_var_data *new_var;
typedef const struct new_var_data *const_new_var;

/* New global variables.  */
htab_t new_global_vars = NULL;

/* New local variables. Generated per-function.  */
htab_t new_local_vars;

/* New  temporaries.  */
htab_t new_tmp_vars;

#define IS_TMP(X) (TREE_CODE (X) == SSA_NAME && !SSA_NAME_VAR (X))

/* Hash value for new_var.  */

static hashval_t
new_var_hash (const void *x)
{
  tree orig_var = ((const_new_var)x)->orig_var;

  if (IS_TMP (orig_var))
    return SSA_NAME_VERSION (orig_var);
  else if (DECL_P (orig_var))
    return DECL_UID (orig_var);
  else
    return 0;
}

/* This function returns nonzero if orig_var of new_var X 
   and tree Y have equal UIDs.  */

static int
new_var_eq (const void *x, const void *y)
{
  const_tree yy = (const_tree) y;
  tree xx = ((const_new_var) x)->orig_var;

  if (IS_TMP (yy) && IS_TMP (xx))
    return SSA_NAME_VERSION (xx) == SSA_NAME_VERSION (yy);
  else if (DECL_P (yy) && DECL_P (xx))
    return DECL_UID (xx) == DECL_UID (yy);
  else
    return 0;
}

/* This function returns new_var node, the orig_var of which is DECL.
   It looks for new_var's in NEW_VARS_HTAB. If not found,
   the function returns NULL.  */

static new_var
is_in_new_vars_htab (tree var, htab_t new_vars_htab)
{
  if (IS_TMP (var))
    return (new_var) htab_find_with_hash (new_vars_htab, var,
					  SSA_NAME_VERSION (var));
  else
    return (new_var) htab_find_with_hash (new_vars_htab, var,
					  DECL_UID (var));
}


/* Non-field accesses of the structures.  */
htab_t accs;


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

/* This function looks for the variable of NEW_TYPE type, stored in VAR.
   It returns it, if found, and NULL_TREE otherwise.  */

static tree
find_var_in_new_vars_vec (new_var var, tree new_type)
{
  tree n_var;
  unsigned i;

  FOR_EACH_VEC_ELT (var->new_vars, i, n_var)
    {
      tree type = strip_type(get_type_of_var (n_var));
      gcc_assert (type);

      if (type == new_type)
	return n_var;
    }

  return NULL_TREE;
}

/* Given original variable ORIG_VAR, this function returns
   new variable corresponding to it of NEW_TYPE type. */

static tree
find_new_var_of_type (tree orig_var, tree new_type)
{
  new_var var = NULL;
  tree var1 = orig_var;
  tree tmp = NULL_TREE;

  gcc_assert (orig_var && new_type);

  if (TREE_CODE (orig_var) == SSA_NAME)
     var1 = SSA_NAME_VAR (orig_var);

  if (var1 == NULL_TREE) /* orig_var is temporary  */
    {
      var = is_in_new_vars_htab (orig_var, new_tmp_vars);
      if (!var)
	fprintf (dump_file, "\nVar is not in new_local_vars.");      
    }
  else
    {

      var = is_in_new_vars_htab (var1, new_global_vars);
      if (!var)
	{
	  fprintf (dump_file, "\nVar is not in new_global_vars.");
	  var = is_in_new_vars_htab (var1, new_local_vars);
	  if (!var)
	    fprintf (dump_file, "\nVar is not in new_local_vars.");
	}
    }

  if (!var)
    debug_tree (orig_var);
  gcc_assert (var);
  if (dump_file)
    {
      fprintf (dump_file, "\nvar is ");
      print_generic_expr (dump_file, var->orig_var, 0);
      //debug_tree (var->orig_var);
    }

  tmp = find_var_in_new_vars_vec (var, new_type); 
  if (tmp)
    {
      fprintf (dump_file, "\nfound var is ");
      print_generic_expr (dump_file, tmp, 0);
      //debug_tree (tmp);
    }

  return tmp;
}

/* This structure is used to represent array of
   wrappers of structure type. For example, if type1 
   is structure type, then for type1 ** we generate 
   two type_wrapper structures with wrap = 0 each one.
   It's used to unwind the original type up to
   structure type, replace it with the new structure type
   and wrap it back in the opposite order.  */

typedef struct type_wrapper
{
  /* 0 stand for pointer wrapper, and 1 for array wrapper.  */
  bool wrap;

  /* Relevant for arrays as domain or index.  */
  tree domain;
}type_wrapper_t;

/* A formal of struct type to be replaced by transformation 
   by formal(s) of new types.  */

struct struct_formal
{
  unsigned op_n; /* Operand number.  */
  tree type;     /* Type of operand.  */
};

typedef struct struct_formal *formal;

/* Function call that contain formal of tructure type(s).  */
struct func_call
{
  gimple call_stmt;
  vec<formal> formals; /* Formals of struct types to be transformed.  */
};

typedef struct func_call *call_with_structs;
static vec<call_with_structs> calls_with_structs;

/* */
static inline int
is_in_formals (vec<formal> formals, unsigned op_n)
{
  unsigned i;
  formal f;
  if (formals.exists ())
    {
      FOR_EACH_VEC_ELT (formals, i, f)
	{
	  if (f->op_n == op_n)
	    return (int)i;
	}
    }
  return -1;
}

/* This function looks for the entry with the call statment CALL in 
   calls_with_structs vector. It returns an index of the entry, 
   if it's found, and -1 otherwise.  */

static inline int
is_in_calls_with_structs (gimple call)
{
  unsigned int i;
  call_with_structs str_call; 
  if (calls_with_structs.exists ())
    {
      FOR_EACH_VEC_ELT (calls_with_structs, i, str_call)
	{
	  if (call == str_call->call_stmt)
	    return (int)i;
	}	  
    }
  return -1;
}

/* This function adds an operands number OP_N and its structure 
   type STR_DECL to its call statement allocated 
   in calls_with_stucts vector.  */

static void
add_formal_to_call_with_struct (gimple call, tree str_decl, unsigned op_n)
{
  call_with_structs fcall;
  formal form;
  int i;
  
  i = is_in_calls_with_structs (call);
  if (i == -1)
    {
      fcall = XNEW (struct func_call);
      fcall->call_stmt = call;
      fcall->formals.create (0);
      if (!calls_with_structs.exists ())
	calls_with_structs.create (0);
      calls_with_structs.safe_push (fcall);      
    }
  else
      fcall = calls_with_structs[i];

  i = is_in_formals (fcall->formals, op_n);
  if (dump_file)
      fprintf (dump_file, "\ni = %d ", i);

  if (i != -1)
    {
      gcc_assert (fcall->formals[i]->type == str_decl);
      if (dump_file)
	{
	  fprintf (dump_file, "\nOperand %d of struct type ", op_n);
	  print_generic_expr (dump_file, str_decl, TDF_VOPS);
	  fprintf (dump_file, " of the call ");      
	  print_gimple_stmt (dump_file, call, 0, TDF_VOPS);
	  fprintf (dump_file, " already in calls_with_structs vector.");
	}
    }
  else
    {
      form = XNEW (struct struct_formal);
      form->op_n = op_n;
      form->type = str_decl;
      fcall->formals.safe_push (form);
          
      if (dump_file)
	{
	  fprintf (dump_file, "\nAdding operand %d of struct type ", op_n);
	  print_generic_expr (dump_file, str_decl, TDF_VOPS);
	  fprintf (dump_file, " to the call ");      
	  print_gimple_stmt (dump_file, call, 0, TDF_VOPS);
	  fprintf (dump_file, " to calls_with_structs vector.");
	}
    }
} 

/* */

static void
free_calls_with_structs (void)
{
  call_with_structs fcall;
  formal form;
  unsigned int i, j;

  if (calls_with_structs.exists ())
    {
      FOR_EACH_VEC_ELT (calls_with_structs, i, fcall)
	{
	  if (fcall->formals.exists ())
	    {
	      FOR_EACH_VEC_ELT (fcall->formals, j, form)
		{
		  free (form);
		}
	      fcall->formals.release ();
	    }      
	  free (fcall);
	}
      calls_with_structs.release ();
    }
}

/* This structure represents allocation site of the structure.  */
typedef struct alloc_site
{
  gimple stmt;
  struct_symbols str;
} alloc_site_t;

typedef alloc_site_t * alloc_site_p;
static vec<alloc_site_p> alloc_sites; 


/* This function looks for the entry with the allocation statement STMT in 
   alloc_sites vector. It returns an index of the entry, if it's found, 
   and -1 otherwise.  */

static inline int
is_in_alloc_site_vec (gimple stmt, struct_symbols str)
{
  unsigned int i;
  alloc_site_p asite; 
  if (alloc_sites.exists ())
    {
      FOR_EACH_VEC_ELT (alloc_sites, i, asite)
	{
	  if (stmt == asite->stmt && asite->str == str)
	    return (int)i;
	}	  
    }
  return -1;
} 

/* This function adds an allocation site to alloc_sites vector.
   The allocation site appears in STMT and
   allocates the structure represented by STR.  */

static void
add_alloc_site (struct cgraph_node *node, gimple stmt, struct_symbols str)
{
  alloc_site_p alloc;

  if (is_in_alloc_site_vec (stmt, str) == -1)
    {
      alloc = XNEW (alloc_site_t);
      alloc->stmt = stmt;
      alloc->str = str;
      
      if (!alloc_sites.exists ())
	alloc_sites.create (0);
      alloc_sites.safe_push (alloc);
    }

  if (dump_file)
    {
      fprintf (dump_file, "\nAdding stmt ");
      print_gimple_stmt (dump_file, stmt, 0, TDF_VOPS);
      fprintf (dump_file, " to vector of allocation site of function %s.", 
	       node->name ());
    }
} 

/* */

static void
free_alloc_sites (void)
{
  alloc_site_p asite;
  unsigned int i;

  FOR_EACH_VEC_ELT (alloc_sites, i, asite)
    free (asite);
  if (alloc_sites.exists ())
    alloc_sites.release ();
} 

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

      
      print_escape (str);
      
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

      if (str->struct_clustering)
	{
	  fprintf (dump_file, "\nStructure decomposition decision:\n");
	  struct field_cluster *cluster = str->struct_clustering;
	  int i = 0;

	  while (cluster)
	    {
	      sbitmap bits = cluster->fields_in_cluster;

	      i++;
	      fprintf (dump_file, "\nFields in %d structure:\n", i);
	      for (int j = 0; j < str->num_fields; j++)
		if (bitmap_bit_p (bits, j))
		  {
		    print_generic_expr (dump_file, 
					str->fields[j].decl, 0);	      
		    fprintf (dump_file, "\n");
		  }
	      cluster = cluster->sibling;
	    }
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
  symbs->accs = NULL;
  symbs->escape = (1 << NONESCAPE);
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
	list[idx].acc_sites = 0; 
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

/* This function looks for structure types instantiated in the program.
   The candidate types are added to the structures vector.  */

static void
build_data_structure (void)
{
  tree var, type, p;
  struct varpool_node *current_var;
  struct cgraph_node *c_node;

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
		  {
		    fprintf (dump_file, " TREE_TYPE(target): ");
		    print_generic_expr (dump_file, TREE_TYPE (target), 0);
		    fprintf (dump_file, " TYPE_ARG_TYPES (TREE_TYPE (target)): ");		    
		    print_generic_expr (dump_file, TYPE_ARG_TYPES (TREE_TYPE (target)), 0);
		  }
	      }
	    else
	      continue;

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

/* This function check if STMT is a cast stmt 
   to the structure type p_8 = (struct str_t *) D.2225_7;
   If it is, function returns true and index of struct in i_p.
   If it's not, function returns false, and i_p is filled with -1.  */

static bool
is_cast_to_struct (gimple stmt, int *i_p)
{
  tree lhs;
  tree type; 

  *i_p = -1;

  if (!stmt)
    {
      if (dump_file)
	{
	  fprintf (dump_file, "\nuse stmt is 0.");
	}
      return false;
    } 

  /* final_stmt should be of the form:
     T.3 = (struct_type *) T.2; */

  if (gimple_code (stmt) != GIMPLE_ASSIGN)
    {
      if (dump_file)
	fprintf (dump_file, "\nfinal_stmt is not gimple_assign.");
      return false;
    }

  lhs = gimple_assign_lhs (stmt);

  type = get_type_of_var (lhs);

  if (!type)
    {
      if (dump_file)
	fprintf (dump_file, "\ntype is 0.");
      return false;
    }

  if (!POINTER_TYPE_P (type)
      || TREE_CODE (strip_type (type)) != RECORD_TYPE)
    {
      if (dump_file)
	fprintf (dump_file, "\ntype does not fit.");

      return false;
    }

  *i_p = is_in_struct_symbols_vec (strip_type (type));

  if ((*i_p) == -1)
    return false;
  else
    return true;
}


/* This function returns true if the result of STMT, that contains a call
   to an allocation function, is cast to one of the structure types.
   STMT should be of the form:    T.2 = <alloc_func> (T.1);
   If true, I_P contains an index of an allocated structure.
   Otherwise I_P contains the length of the vector of structures.  */

static bool
is_alloc_of_struct (gimple alloc_stmt, int *i_p)
{
  gimple use_stmt;
  tree alloc_res;
  imm_use_iterator imm_iter;
  *i_p = -1;
  bool res;
  int ii;
  tree type;

  if (!alloc_stmt)
    return false;

  if (!is_gimple_call (alloc_stmt))
    return false;

  alloc_res = gimple_get_lhs (alloc_stmt);

  if (TREE_CODE (alloc_res) != SSA_NAME)
    {
      if (dump_file)
	fprintf (dump_file, "\nalloc_res is not ssa_name.");
      return false;
    }

  /* Check whether result of function call is assigned directly into 
     struct variable, as in case of builtin alloca.  */
  type = get_type_of_var (alloc_res);

  if (!type)
    {
      if (dump_file)
	fprintf (dump_file, "\ntype is 0.");
      return false;
    }

  if (POINTER_TYPE_P (type)
      && TREE_CODE (strip_type (type)) == RECORD_TYPE)
    {
      ii = is_in_struct_symbols_vec (strip_type (type));

      if (ii != -1)
	{
	  *i_p = ii;
	  return true;
	}
    }

  /* Visit all uses of alloc_res, and check whether 
     at least one of them is cast to struct type.  */
  res = false;
  ii = -1;
  FOR_EACH_IMM_USE_STMT (use_stmt, imm_iter, alloc_res)
    {
      if (is_cast_to_struct (use_stmt, &ii))
	{
	  if (!res)
	    {
	      res = true;
	      *i_p = ii;
	    }
	  else
	  /* Result of malloc is cast to two different structures.  */
	    gcc_assert (ii == *i_p);	    
	}
    }

  return res;
}


/* This function looks for structure allocation sites which cannot
   be handled and defines their escape type as CUSTOM_SITE_MALLOC.  

   In this function we assume that an allocation statement

   var = (type_cast) malloc (size);

   is converted into the following set of statements:

   T.1 = size;
   T.2 = malloc (T.1);
   T.3 = (type_cast) T.2;
   var = T.3;

   In this function we collect into alloc_sites the allocation
   sites of variables of structure types that are present
   in structures vector.  */

static void
exclude_custom_malloc (void)
{
  struct cgraph_node *node;
  struct cgraph_edge *cs;

  FOR_EACH_DEFINED_FUNCTION(node)
    if (node->analyzed && node->decl)
      {
	if (dump_file)
	  dump_function_to_file (node->decl, dump_file, 0);

	for (cs = node->callees; cs; cs = cs->next_callee)
	  {
	    gimple stmt = cs->call_stmt;

	    if (stmt)
	      {
		tree decl;

		if (is_gimple_call (stmt)
		    && (decl = gimple_call_fndecl (stmt))
		    && gimple_call_lhs (stmt))
		  {
		    int i;

		    if (dump_file)
		      {
			fprintf (dump_file, "\nCall stmt is ");
			print_gimple_stmt (dump_file, stmt, 0, 0);
		      }

		    if (is_alloc_of_struct (stmt, &i))
		      {
			if (dump_file)
			  print_generic_expr (dump_file, struct_symbols_vec[i]->struct_decl, 0);

			/* All struct types are already collected.  */
			gcc_assert (i != -1);

			/* We support only malloc now.  */
			if (DECL_FUNCTION_CODE (decl) != BUILT_IN_MALLOC
			    && DECL_FUNCTION_CODE (decl) != BUILT_IN_ALLOCA
			    && DECL_FUNCTION_CODE (decl) != BUILT_IN_ALLOCA_WITH_ALIGN)
			  {
			    if (dump_file)
			      {
				fprintf (dump_file,
					 "\nUnsupported allocation function ");
				print_gimple_stmt (dump_file, stmt, 0, 0);
			      }
			    add_escape (struct_symbols_vec[i], 1 << CUSTOM_ALLOC_SITE);
			  }
		      }
		  }
	      }
	  }
      }
}

static void
has_inline_asm (void)
{
  basic_block bb;
  struct cgraph_node *c_node;

  FOR_EACH_FUNCTION(c_node)
  {
    enum availability avail = cgraph_function_body_availability (c_node);

    if (avail == AVAIL_LOCAL || avail == AVAIL_AVAILABLE)
      {
	struct function *func = DECL_STRUCT_FUNCTION (c_node->decl);
	gimple_stmt_iterator bsi;

	FOR_EACH_BB_FN (bb, func)
	  {
	    for (bsi = gsi_start_bb (bb); !gsi_end_p (bsi); gsi_next (&bsi))
	      {
		gimple stmt = gsi_stmt (bsi);

		/* In asm stmt we cannot always track the arguments,
		   so we just give up.  */
		if (gimple_code (stmt) == GIMPLE_ASM)
		  {
		    struct_reorg_give_up |= (1 << STRUCT_REORG_GIVE_UP_ASM_STMT);		      
		    goto GIVE_UP_LABEL;
		  }
	      }
	  }
      }
  }
 GIVE_UP_LABEL:
  return;
}

/* This function collects structures potential
   for peeling transformation, and inserts
   them into structures hashtable.  */

static void
collect_structures (void)
{  
  /* Collect all structures in compilation unit into struct_symbols_vec.  */
  build_data_structure ();
  exclude_custom_malloc ();
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

/* This function copies type qualities from ORIG_TYPE to NEW_TYPE.  */

static void
copy_type_quals (tree new_type, tree orig_type)
{
      /* Copy original type qualities.  */
      TYPE_VOLATILE (new_type) = TYPE_VOLATILE (orig_type);
      TYPE_ATOMIC (new_type) = TYPE_ATOMIC (orig_type);
      TYPE_RESTRICT (new_type) = TYPE_RESTRICT (orig_type);
      TYPE_READONLY (new_type) = TYPE_READONLY (orig_type);
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
      copy_type_quals (new_type, str->struct_decl);
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

static bool
is_alloc_or_free (symtab_node *sbl)
{
  cgraph_node *node = dyn_cast <cgraph_node> (sbl);
  if (node 
      && (DECL_FUNCTION_CODE (node->decl) == BUILT_IN_ALLOCA
	  || DECL_FUNCTION_CODE (node->decl) == BUILT_IN_ALLOCA_WITH_ALIGN
	  || DECL_FUNCTION_CODE (node->decl) == BUILT_IN_FREE))
    return true;
  else
    return false;
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

  if (struct_reorg_give_up != (1 << STRUCT_REORG_DO_NOT_GIVE_UP))
    {
      if (dump_file)
	fprintf (dump_file, "\nWe are giving up...");

      /* Free struct_symbols_vec.  */
      for (i = 0; struct_symbols_vec.iterate (i, &symbols);)
	{
	  remove_type_from_struct_symbols_vec (symbols);
	  struct_symbols_vec.ordered_remove (i);
	}
      struct_symbols_vec.release ();
    }

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
		  fprintf (dump_file, " might escapes.");
		}

	      /* If symbol is allocation or free function, it is not real escape.  */
	      if (is_alloc_or_free (sbl))
		{
		  if (dump_file)
		    fprintf (dump_file, " But it is not.");		    
		}
	      else
		{
		  if (dump_file)
		    fprintf (dump_file, " In fact, it is.");		    
		  escape = true;
		  break;
		}
	    }
	}
      /* We remove all externally visible structures and 
	 all non-suitable (based on previously detected reasons) structures.  */
      if (escape || (symbols->escape > (1 << NONESCAPE)))
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

static struct symtab_node *
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

static void
read_struct_clusters (struct lto_input_block *ib_main_p, struct_symbols str)
{
  unsigned int num_clusters = streamer_read_uhwi (ib_main_p);
  
  if (!num_clusters)
    return;

  for (unsigned int i = 0; i < num_clusters; i++)
    {
      sbitmap bits = sbitmap_alloc ((unsigned int)str->num_fields);

      for (unsigned int j = 0; j < bits->size; j++)
	bits->elms[j] = streamer_read_uhwfi (ib_main_p);
      gen_cluster (bits, str);
    }  
}

static void
read_struct_new_types (struct lto_input_block *ib_main_p, 
		       struct data_in *data_in, struct_symbols str)
{
  unsigned int num_new_types = streamer_read_uhwi (ib_main_p);

  if (!num_new_types)
    return;
  
  for (unsigned int i = 0; i < num_new_types; i++)
    (str->new_types).safe_push (stream_read_tree (ib_main_p, data_in));
}

static void
read_struct_fields (struct lto_input_block *ib_main_p, 
		    struct data_in *data_in, struct_symbols str)
{
  str->fields = get_fields (str->struct_decl, str->num_fields);

  /* Only field mappings should be read in.  */
  for (int i = 0; i < str->num_fields; i++)
      str->fields[i].field_mapping = stream_read_tree (ib_main_p, data_in);
}

static void
read_struct_symbols (struct lto_input_block *ib_main_p, 
		     lto_symtab_encoder_t encoder, int j)
{
  unsigned int n_symbols;
  unsigned int index;
  struct symtab_node *symbol;

  n_symbols = streamer_read_uhwi (ib_main_p);

  while (n_symbols)
    {	      
      index = streamer_read_uhwi (ib_main_p);
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
  unsigned int count, escape;
  lto_symtab_encoder_t encoder;

  LTO_INIT_INPUT_BLOCK (ib_main, (const char *) data + main_offset, 0,
			header->main_size);

  data_in =
    lto_data_in_create (file_data, (const char *) data + string_offset,
			header->string_size, vNULL);
  struct_reorg_give_up |= streamer_read_uhwi (&ib_main);
  count = streamer_read_uhwi (&ib_main);
  encoder = file_data->symtab_node_encoder;

  for (i = 0; i < count; i++)
    {
      tree struct_decl;
      int j;

      /* Deserialize structure declaration.  */
      struct_decl = stream_read_tree (&ib_main, data_in);
      escape = streamer_read_uhwi (&ib_main);
      j = is_in_struct_symbols_vec (struct_decl);
      if (j == -1)	
	j = add_struct_to_struct_symbols_vec (struct_decl);

      /* Deserialize symbols.  */
      read_struct_symbols (&ib_main, encoder, j);
      add_escape (struct_symbols_vec[j], escape);
    }

  lto_free_section_data (file_data, LTO_section_ipa_struct_reorg, NULL, data,
			 len);
  lto_data_in_delete (data_in);
}

/* Read section in file FILE_DATA of length LEN with data DATA.  */

static void
struct_reorg_read_section_after_decision (struct lto_file_decl_data *file_data, 
					  const char *data, size_t len)
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
      int j;
      struct_symbols str;

      struct_decl = stream_read_tree (&ib_main, data_in);
      j = is_in_struct_symbols_vec (struct_decl);
      if (j == -1)	
	j = add_struct_to_struct_symbols_vec (struct_decl);
      str = struct_symbols_vec[j];
      
      /* Deserialize symbols.  */
      read_struct_symbols (&ib_main, encoder, j);
      
      /* Deserialize number of fields.  */
      str->num_fields = streamer_read_uhwi (&ib_main);

      /* Deserialize fields.  */
      read_struct_fields (&ib_main, data_in, str);

      /* Deserialize clusters.  */
      read_struct_clusters (&ib_main, str);

      /* Deserialize new types.  */
      read_struct_new_types (&ib_main, data_in, str);
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

/* This function serializes structure symbols.  */

static void
write_struct_symbols (struct output_block *ob, vec<symtab_node *> symbols,
		      lto_symtab_encoder_t encoder)
{
  unsigned int i;
  symtab_node *sbl;


  if (!symbols.exists () || struct_symbols_vec.is_empty ())
    {
      streamer_write_zero (ob);
      if (dump_file)
	fprintf (dump_file, "\nNo symbols found...");
    }
  else
    {
      streamer_write_uhwi (ob, symbols.length ());
      if (dump_file)
	fprintf (dump_file, "\nNumber of symbols for this type is %d\n", symbols.length ());
      FOR_EACH_VEC_ELT (symbols, i, sbl)
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

static void
write_struct_new_types (struct output_block *ob, vec<tree> new_types)
{
  unsigned int k;
  tree new_type;

  if (!new_types.exists () || new_types.is_empty ())
    {
      k = 0;
      streamer_write_uhwi (ob, k);
      return;
    }

  streamer_write_uhwi (ob, new_types.length ());
  FOR_EACH_VEC_ELT (new_types, k, new_type)
    stream_write_tree (ob, new_type, true);
}

static void
write_struct_clusters (struct output_block *ob, 
		       struct field_cluster *clusters)
{
  struct field_cluster *cluster = clusters;
  int num_clusters = 0;
  
 
  /* Calculate and write number of clusters.  */
  while (cluster)
    {
      num_clusters++;
      cluster = cluster->sibling;
    }
  streamer_write_uhwi (ob, num_clusters);

  if (!num_clusters)
    return;

  cluster = clusters;
  for (int i = 0; i < num_clusters; i++)
    {
      sbitmap bits = cluster->fields_in_cluster;

      
      /* Number of bits in bitmap is equal to number of fields.

	 We also suppose that writing and reading optimization summary
	 is executed on the same machine, so that HOST_WIDEST_FAST_INT 
	 value is the same. If it's not correct, we should serrialize 
	 also bits->size.  */
      
      for (unsigned int j = 0; j < bits->size; j++)
	streamer_write_uhwfi (ob, bits->elms[j]);

      cluster = cluster->sibling;
    }
}

/* Serialize the ipa info for lto.  */

static void
struct_reorg_write_summary (void)
{
  struct output_block *ob = create_output_block (LTO_section_ipa_struct_reorg);
  lto_symtab_encoder_t encoder;
  unsigned int i;
  struct_symbols symbols;

  streamer_write_uhwi (ob, struct_reorg_give_up);

  encoder = ob->decl_state->symtab_node_encoder;
  ob->cgraph_node = NULL;
  streamer_write_uhwi (ob, struct_symbols_vec.length ());

  if (dump_file)
    {
      fprintf (dump_file, "\n\nStruct-reorg write summary starts...");
      fprintf (dump_file, "\nNumber of structure types is %d\n", struct_symbols_vec.length ());
    }

  if (struct_symbols_vec.exists () && !struct_symbols_vec.is_empty ())
    {
      FOR_EACH_VEC_ELT (struct_symbols_vec, i, symbols)
	{
	  /* Serialize structure declaration.  */
	  gcc_assert (struct_symbols_vec[i]->struct_decl);
	  stream_write_tree (ob, struct_symbols_vec[i]->struct_decl, true);
	  streamer_write_uhwi (ob, struct_symbols_vec[i]->escape);

	  if (dump_file)
	    dump_struct_type (struct_symbols_vec[i]->struct_decl, 2, 0);
      
	  /* Serialize structure symbols.  */
	  write_struct_symbols (ob, symbols->symbols, encoder);      
	}
    }

  streamer_write_char_stream (ob->main_stream, 0);
  produce_asm (ob, NULL);
  destroy_output_block (ob);
} 

static void
struct_reorg_read_opt_summary (void)
{
  struct lto_file_decl_data **file_data_vec = lto_get_file_decl_data ();
  struct lto_file_decl_data *file_data;
  unsigned int j = 0; 

  if (dump_file)
      fprintf (dump_file, "\n\nStruct-reorg read optimization summary starts...");

  while ((file_data = file_data_vec[j++]))
    {
      size_t len;
      const char *data = lto_get_section_data (file_data, LTO_section_ipa_struct_reorg, NULL, &len);

      if (data) {
        struct_reorg_read_section_after_decision (file_data, data, len);
      }
    }

  print_struct_symbol_vec ();
}

/* Serialize structure fields.  */

static void
write_struct_fields (struct output_block *ob, struct field_entry *fields, 
		     int num_fields)
{

  for (int i = 0; i < num_fields; i++)
    {
      /* No need to save field decl or field index, since it can be reconstructed 
	 from struct decl.  */
      stream_write_tree (ob, fields[i].field_mapping, true);
    }
}

/* Serialize the ipa info after propagation.  */

static void
struct_reorg_write_opt_summary (void)
{   
  struct output_block *ob = create_output_block (LTO_section_ipa_struct_reorg);
  lto_symtab_encoder_t encoder;
  unsigned int i;
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
      fprintf (dump_file, "\n\nStruct-reorg write optimization summary starts...");
      fprintf (dump_file, "\nNumber of structure types is %d\n", struct_symbols_vec.length ());
    }

  FOR_EACH_VEC_ELT (struct_symbols_vec, i, symbols)
    {
      /* Serialize structure declaration.  */
      gcc_assert (struct_symbols_vec[i]->struct_decl);
      stream_write_tree (ob, struct_symbols_vec[i]->struct_decl, true);

      if (dump_file)
	dump_struct_type (struct_symbols_vec[i]->struct_decl, 2, 0);
      
      /* Serialize structure symbols.  */
      write_struct_symbols (ob, symbols->symbols, encoder);

      /* Serialize number of fields.  */
      streamer_write_uhwi (ob, struct_symbols_vec[i]->num_fields);

      /* Serialize fields.  */
      write_struct_fields (ob, symbols->fields, symbols->num_fields);

      /* Serialize clusters.  */
      write_struct_clusters (ob, symbols->struct_clustering);

      /* Serialize new types.  */
      write_struct_new_types (ob, symbols->new_types);
    }

  streamer_write_char_stream (ob->main_stream, 0);
  produce_asm (ob, NULL);
  destroy_output_block (ob);
}

/* This function creates and returns new_var_data node
   with empty new_vars and orig_var equal to VAR.  */

static new_var
create_new_var_node (tree var, struct_symbols str)
{
  new_var node;

  node = XNEW (struct new_var_data);
  node->orig_var = var;
  node->new_vars.create (str->new_types.length ());
  return node;
}

/* This function generates and returns new variable name based on
   ORIG_DECL name, combined with index I.
   The form of the new name is <orig_name>.<I> .  */

static tree
gen_var_name (tree orig_var, unsigned HOST_WIDE_INT i)
{
  const char *old_name;
  char *prefix;
  char *new_name;

  if (!DECL_NAME (orig_var) 
      || !IDENTIFIER_POINTER (DECL_NAME (orig_var)))
     return NULL;

  /* If the original variable has a name, create an
     appropriate new name for the new variable.  */

  old_name = IDENTIFIER_POINTER (DECL_NAME (orig_var));
  prefix = XALLOCAVEC (char, strlen (old_name) + 1);
  strcpy (prefix, old_name);
  ASM_FORMAT_PRIVATE_NAME (new_name, prefix, i);
  return get_identifier (new_name);
}

/* This function wraps NEW_STR_TYPE in pointers or arrays wrapper
   the same way as a structure type is wrapped in DECL.
   It returns the generated type.  */

static inline tree
gen_struct_type (tree decl, tree new_str_type)
{
  tree type_orig = get_type_of_var (decl);
  tree new_type = new_str_type;
  vec<type_wrapper_t> wrapper;
  type_wrapper_t wr;

  wrapper.create (10); 
  while (POINTER_TYPE_P (type_orig)
	 || TREE_CODE (type_orig) == ARRAY_TYPE)
    {
      if (POINTER_TYPE_P (type_orig))
	{
	  wr.wrap = 0;
	  wr.domain = NULL_TREE;
	}
      else
	{
	  gcc_assert (TREE_CODE (type_orig) == ARRAY_TYPE);
	  wr.wrap = 1;
	  wr.domain = TYPE_DOMAIN (type_orig);
	}
      wrapper.safe_push (wr);
	// VEC_safe_push (type_wrapper_t, heap, wrapper, &wr);
      type_orig = TREE_TYPE (type_orig);
    }

  while (!wrapper.is_empty ())
    {
      wr = wrapper.last ();

      if (wr.wrap) /* Array.  */
	new_type = build_array_type (new_type, wr.domain);
      else /* Pointer.  */
	new_type = build_pointer_type (new_type);

      wrapper.pop ();
    }

  wrapper.release ();
  return new_type;
}

/* This function copies attributes form SSA_NAME to NEW_SSA.  */

static inline void
copy_ssa_name_attributes (tree new_ssa, tree orig_ssa)
{

  TREE_STATIC (new_ssa) = TREE_STATIC (orig_ssa);
  TREE_PUBLIC (new_ssa) = TREE_PUBLIC (orig_ssa);
  TREE_USED (new_ssa) = TREE_USED (orig_ssa);
  TREE_THIS_VOLATILE (new_ssa) = TREE_THIS_VOLATILE (orig_ssa);
  TREE_ADDRESSABLE (new_ssa) = TREE_ADDRESSABLE (orig_ssa);

}

/* This function copies attributes form ORIG_DECL to NEW_DECL.  */

static inline void
copy_decl_attributes (tree new_decl, tree orig_decl)
{

  DECL_ARTIFICIAL (new_decl) = 1;
  DECL_EXTERNAL (new_decl) = DECL_EXTERNAL (orig_decl);
  TREE_STATIC (new_decl) = TREE_STATIC (orig_decl);
  TREE_PUBLIC (new_decl) = TREE_PUBLIC (orig_decl);
  TREE_USED (new_decl) = TREE_USED (orig_decl);
  DECL_CONTEXT (new_decl) = DECL_CONTEXT (orig_decl);
  TREE_THIS_VOLATILE (new_decl) = TREE_THIS_VOLATILE (orig_decl);
  TREE_ADDRESSABLE (new_decl) = TREE_ADDRESSABLE (orig_decl);

  if (TREE_CODE (orig_decl) == VAR_DECL)
    {
      TREE_READONLY (new_decl) = TREE_READONLY (orig_decl);
      DECL_TLS_MODEL (new_decl) = DECL_TLS_MODEL (orig_decl);
    }
}

/* Given an original variable ORIG_DECL of structure type STR,
   this function generates new variables of the types defined
   by STR->new_type. Generated types are saved in new_var node NODE.
   ORIG_DECL should has VAR_DECL tree_code.  */

static void
create_new_var_1 (tree orig_var, struct_symbols str, new_var node)
{
  unsigned i;
  tree type;

  FOR_EACH_VEC_ELT (str->new_types, i, type)
    {
      tree new_var = NULL;
      tree new_name;

      type = gen_struct_type (orig_var, type);
      if (IS_TMP (orig_var))
	{
	  /* When we'll use new_var, we have to fix its definition stmt.  */ 
	  new_var = make_ssa_name (type, SSA_NAME_DEF_STMT (orig_var));
	  copy_ssa_name_attributes (new_var, orig_var);
	}
      else
	{
	  new_name = gen_var_name (orig_var, i);

	  if (is_global_var (orig_var))
	    new_var = build_decl (DECL_SOURCE_LOCATION (orig_var),
			       VAR_DECL, new_name, type);
	  else
	    {
	      const char *name = new_name ? IDENTIFIER_POINTER (new_name) : NULL;
	      new_var = create_tmp_var (type, name);
	    }
	  
	  copy_decl_attributes (new_var, orig_var);
	}

      /* */
      copy_type_quals (TREE_TYPE (new_var), TREE_TYPE (orig_var));

      if (dump_file)
	{
	  fprintf (dump_file, "\nnew_var is ");
	  print_generic_expr (dump_file, new_var, 0);
	  fprintf (dump_file, "\n");	  
	}
      node->new_vars.safe_push (new_var);
    }
}

/* This function adds NEW_NODE to hashtable of new_var's NEW_VARS_HTAB. */

static void
add_to_new_vars_htab (new_var new_node, htab_t new_vars_htab)
{
  void **slot;
  tree orig_var = new_node->orig_var;

  if (IS_TMP (orig_var))
    slot = htab_find_slot_with_hash (new_vars_htab, orig_var,
				   SSA_NAME_VERSION (orig_var),
				   INSERT);
  else
    slot = htab_find_slot_with_hash (new_vars_htab, new_node->orig_var,
				   DECL_UID (new_node->orig_var),
				   INSERT);
  *slot = new_node;
}

/* This function inserts new variables from new_var into varpool.  */

static void
update_varpool_with_new_var (new_var node)
{
  tree var;
  unsigned i;

  if (!node)
    return;

  FOR_EACH_VEC_ELT (node->new_vars, i, var)
    {      
      /* FIXME: if not preserved, var will be remove 
	 after first function transform as non-referenced.  */
      DECL_PRESERVE_P (var) = 1;
      varpool_finalize_decl (var);
    }
}

/* This function creates new variables to
   substitute the original variable VAR_DECL and adds
   them to the new_var's hashtable NEW_VARS_HTAB.  */

static new_var
create_new_var (tree var, htab_t new_vars_htab)
{
  new_var node;
  struct_symbols str;
  tree type;
  int i;

  if (!var || is_in_new_vars_htab (var, new_vars_htab))
    return NULL;

  if (!var_is_candidate (var, &type))
    return NULL;

  i = is_in_struct_symbols_vec (type);
  if (i == -1)
    return NULL;

  str = struct_symbols_vec[i];
  node = create_new_var_node (var, str);
  create_new_var_1 (var, str, node);
  add_to_new_vars_htab (node, new_vars_htab);

  return node;
}

/* This function is a callback for traversal on new_var's hashtable.
   SLOT is a pointer to new_var. This function prints to dump_file
   an original variable and all new variables from the new_var
   pointed by *SLOT.  */

static int
dump_new_var (void **slot, void *data ATTRIBUTE_UNUSED)
{
  new_var n_var = *(new_var *) slot;
  tree var_type;
  tree var;
  unsigned i;

  var_type = get_type_of_var (n_var->orig_var);

  fprintf (dump_file, "\nOrig var: ");
  print_generic_expr (dump_file, n_var->orig_var, 0);
  fprintf (dump_file, " of type ");
  print_generic_expr (dump_file, var_type, 0);
  fprintf (dump_file, "\n");

  FOR_EACH_VEC_ELT (n_var->new_vars, i, var)
    {
      var_type = get_type_of_var (var);

      fprintf (dump_file, "      ");
      print_generic_expr (dump_file, var, 0);
      fprintf (dump_file, " of type ");
      print_generic_expr (dump_file, var_type, 0);
      fprintf (dump_file, "\n");
    }
  return 1;
}

/* This function prints new variables from hashtable
   NEW_VARS_HTAB to dump_file.  */

static void
dump_new_vars (htab_t new_vars_htab)
{
  if (!dump_file)
    return;

  if (new_vars_htab)
    htab_traverse (new_vars_htab, dump_new_var, NULL);
}

/* Implementation of variable_transform function for struct-reorg.  */

void
struct_reorg_var_transform (varpool_node *vnode)
{
  tree var_decl;

  if (dump_file)
    fprintf (dump_file, "\nStruct_reorg is transforming %s variable.", vnode->name ());

  if (!new_global_vars)
    new_global_vars = htab_create (30, new_var_hash, new_var_eq, NULL);

  var_decl = vnode->decl;

  if (!var_decl || TREE_CODE (var_decl) != VAR_DECL)
    return;

  update_varpool_with_new_var (create_new_var (var_decl, new_global_vars));
  dump_new_vars (new_global_vars);  
}

/* For each local variable of structure type from the vector of structures
   this function generates new variable(s) to replace it.  */

static void
create_new_local_vars (void)
{
  tree decl;
  unsigned ix;

  new_local_vars = htab_create (vec_safe_length (cfun->local_decls),
				new_var_hash, new_var_eq, NULL);

  if (dump_file)
    fprintf (dump_file, "\nChecking for local variables...");
  FOR_EACH_LOCAL_DECL (cfun, ix, decl)
    {
      /* Local static varibales are in symbol table,
	 and were treated as part of variable_transform.  */
      if (!TREE_STATIC (decl))
	{
	  if (dump_file)
	    {
	      fprintf (dump_file, "\nLocal Var is: ");
	      print_generic_expr (dump_file, decl, 0);
	      fprintf (dump_file, "\n");
	    }
	  create_new_var (decl, new_local_vars);
	}
    }

  dump_new_vars (new_local_vars);
}

static void
create_new_tmp_vars (void)
{
  unsigned ix;
  /* We are adding ssa name vars, so we need to 
     count only original ones.  */
  unsigned init_num_ssa_names = num_ssa_names;

  if (dump_file)
    fprintf (dump_file, "\nPrinting temporaries ...\n");

  new_tmp_vars = htab_create (init_num_ssa_names,
				new_var_hash, new_var_eq, NULL);

  if (gimple_in_ssa_p (cfun))
    for (ix = 1; ix < init_num_ssa_names; ++ix)
      {
	tree name = ssa_name (ix);
	if (name && !SSA_NAME_VAR (name))
	  {
	    if (dump_file)
	      {
		fprintf (dump_file, "  ");
		print_generic_expr (dump_file, TREE_TYPE (name), 0);
		fprintf (dump_file, " ");
		print_generic_expr (dump_file, name, 0);
		fprintf (dump_file, ";\n");
	      }
	    create_new_var (name, new_tmp_vars);
	  }
      }

  dump_new_vars (new_tmp_vars);
}

/* This function is a callback for traversal over new_var's hashtable.
   SLOT is a pointer to new_var. This function frees memory allocated
   for new_var and pointed by *SLOT.  */

static int
free_new_var (void **slot, void *data ATTRIBUTE_UNUSED)
{
  new_var n_var = *(new_var *) slot;

  /* Free vector of new_vars.  */
  (n_var->new_vars).release ();
  free (n_var);
  return 1;
}

/* Free new_vars hashtable NEW_VARS_HTAB.  */

static void
free_new_vars_htab (htab_t new_vars_htab)
{
  if (new_vars_htab)
    {
      htab_traverse (new_vars_htab, free_new_var, NULL);
      htab_delete (new_vars_htab);
    }
  new_vars_htab = NULL;
}

/* In this function we assume that an allocation statement

   var = (type_cast) malloc (size);

   is converted into the following set of statements:

   T.1 = size;
   T.2 = malloc (T.1);
   T.3 = (type_cast) T.2;
   var = T.3;

   In this function we collect into alloc_sites the allocation
   sites of variables of structure types that are present
   in structures vector.  */

static void
collect_alloc_sites (struct cgraph_node *node)
{
  struct cgraph_edge *cs;
  
  if (!node->analyzed || !node->decl)
    return;

  for (cs = node->callees; cs; cs = cs->next_callee)
    {
      gimple stmt = cs->call_stmt;

      if (stmt)
	{
	  tree decl;

	  if (is_gimple_call (stmt)
	      && (decl = gimple_call_fndecl (stmt))
	      && gimple_call_lhs (stmt))
	    {
	      int i;
	      
	      if (is_alloc_of_struct (stmt, &i))
		add_alloc_site (node, stmt, struct_symbols_vec[i]); 
	    }
	}
    }
}

/* */
static void
check_func_args (gimple stmt)
{
  tree arg, t;
  unsigned i;
  int j;
  
  for (i = 0; i < gimple_call_num_args (stmt); i++) 
    {
      arg = gimple_call_arg (stmt, i);
      if (!arg)
	continue;

      t = strip_type (get_type_of_var (arg));
      if (dump_file) 
	{
	  fprintf (dump_file, "\narg %d:", i);
	  print_generic_expr (dump_file, arg, 0);
	  //debug_tree(arg);
	}

      j = is_in_struct_symbols_vec (t);
      if (j != -1)
	{
	  if (dump_file)
	    {
	      fprintf (dump_file, " Type ");
	      print_generic_expr (dump_file, t, 0);
	      fprintf (dump_file,  " is in symbols_vec. \n");
	    }
	  add_formal_to_call_with_struct (stmt, struct_symbols_vec[j]->struct_decl, i);
	} 
      else 
	{
	  if (dump_file)
	    {
	      fprintf (dump_file, " Type ");
	      print_generic_expr (dump_file, t, 0);
	      fprintf (dump_file,  " is not in symbols_vec. \n");
	    }
	}
    }
  return;
}

/* This function collects function calls of current function 
   that have parameters of struct types. We intend to transform these 
   function calls to have parameters of new struct types. Function calls
   that are allocation or free sites are omitted here. They are 
   treated as part of allocation or general access sites.  */

static void
collect_func_calls (struct cgraph_node *node)
{
  struct cgraph_edge *cs;
  
  if (!node->analyzed || !node->decl)
    return;

  for (cs = node->callees; cs; cs = cs->next_callee)
    {
      gimple stmt = cs->call_stmt;

      if (stmt)
	{
	  tree decl;

	  if (is_gimple_call (stmt)
	      && (decl = gimple_call_fndecl (stmt)))
	    {
	      
	      if (dump_file)
		{
		  fprintf (dump_file, "\ncollect function calls, func call: ");
		  print_generic_expr (dump_file, decl, 0);
		}
	      check_func_args (stmt);
	    }
	}
    }
}

/* This function checks whether ARG is a result of multiplication
   of some number by STRUCT_SIZE. If yes, the function returns true
   and this number is filled into NUM.  */

static bool
is_result_of_mult (tree arg, tree *num, tree struct_size)
{
  gimple size_def_stmt = SSA_NAME_DEF_STMT (arg);

  /* If the allocation statement was of the form
     D.2229_10 = <alloc_func> (D.2228_9);
     then size_def_stmt can be D.2228_9 = num.3_8 * 8;  */

  if (size_def_stmt && is_gimple_assign (size_def_stmt))
    {
      tree lhs = gimple_assign_lhs (size_def_stmt);

      /* We expect temporary here.  */
      if (!is_gimple_reg (lhs))
	return false;

      if (gimple_assign_rhs_code (size_def_stmt) == MULT_EXPR)
	{
	  tree arg0 = gimple_assign_rhs1 (size_def_stmt);
	  tree arg1 = gimple_assign_rhs2 (size_def_stmt);

	  if (operand_equal_p (arg0, struct_size, OEP_ONLY_CONST))
	    {
	      *num = arg1;
	      return true;
	    }

	  if (operand_equal_p (arg1, struct_size, OEP_ONLY_CONST))
	    {
	      *num = arg0;
	      return true;
	    }
	}
    }

  *num = NULL_TREE;
  return false;
}

/* This function returns a tree representing
   the number of instances of structure STR_DECL allocated
   by allocation STMT. If new statements are generated,
   they are filled into NEW_STMTS_P.  */

static tree
gen_num_of_structs_in_malloc (gimple stmt, tree str_decl,
			      gimple_seq *new_stmts_p)
{
  tree arg;
  tree struct_size;
  HOST_WIDE_INT struct_size_int;

  if (!stmt)
    return NULL_TREE;

  /* Get malloc argument.  */
  if (!is_gimple_call (stmt))
    return NULL_TREE;

  arg = gimple_call_arg (stmt, 0);
  //debug_tree (arg);

  if (dump_file)
    {
      fprintf (dump_file, "\narg is ");
      print_generic_expr (dump_file, arg, 0);
    }
  
  if (TREE_CODE (arg) != SSA_NAME
      && !TREE_CONSTANT (arg))
    {
      if (dump_file)
	fprintf (dump_file, "\nReturning NULL from gen_num.");
      return NULL_TREE;
    }

  struct_size = TYPE_SIZE_UNIT (str_decl);
  struct_size_int = TREE_INT_CST_LOW (struct_size);

  gcc_assert (struct_size);

  if (dump_file)
    fprintf (dump_file, "\nstruct_size_int is %ld",
	     struct_size_int);

  if (TREE_CODE (arg) == SSA_NAME)
    {
      tree num;
      gimple div_stmt;

      if (is_result_of_mult (arg, &num, struct_size))
	  return num;

      num = create_tmp_var (TREE_TYPE(arg)/* integer_type_node */ , NULL);

      /* if (num)
	 add_referenced_var (num); */

      if (exact_log2 (struct_size_int) == -1)
	div_stmt = gimple_build_assign_with_ops (TRUNC_DIV_EXPR, num, arg,
						 struct_size);
      else
	{
	  tree C =  build_int_cst (integer_type_node,
				   exact_log2 (struct_size_int));

	  div_stmt = gimple_build_assign_with_ops (RSHIFT_EXPR, num, arg, C);
	}
      gimple_seq_add_stmt (new_stmts_p, div_stmt);
      update_stmt (div_stmt);
      fprintf (dump_file, "\n We are in ssa_name at the end.");

      if (*new_stmts_p)
	fprintf (dump_file, "\n new_stmts_p is not 0.");
	
      
      { /* Print new_stmts seq.  */
	gimple_stmt_iterator i;

	for (i = gsi_start (*new_stmts_p); !gsi_end_p (i); gsi_next (&i))
	  {
	    gimple gs = gsi_stmt (i);
	    print_gimple_stmt (dump_file, gs, 0, TDF_SLIM);
	  }
      }
      return num;
    }

  if (CONSTANT_CLASS_P (arg)
      && multiple_of_p (TREE_TYPE (struct_size), arg, struct_size))
    return int_const_binop (TRUNC_DIV_EXPR, arg, struct_size);
  

  return NULL_TREE;
}

/* Insert NEW_STMTS after STMT.  */

static void
insert_seq_after_stmt (gimple stmt, gimple_seq new_stmts)
{
  gimple_stmt_iterator bsi;

  if (!stmt || !new_stmts)
    return;

  bsi = gsi_for_stmt (stmt);
  gsi_insert_seq_after (&bsi, new_stmts, GSI_SAME_STMT);
}

/* Insert NEW_STMT after STMT.  */

static void
insert_after_stmt (gimple stmt, gimple new_stmt)
{
  gimple_stmt_iterator bsi;

  if (!stmt || !new_stmt)
    return;

  bsi = gsi_for_stmt (stmt);
  gsi_insert_after (&bsi, new_stmt, GSI_SAME_STMT);
}

/* Update call graph with new edge generated by new MALLOC_STMT.
   The edge origin is CONTEXT function.  */

static void
update_cgraph_with_malloc_call (gimple malloc_stmt, tree context)
{
  struct cgraph_node *src, *dest;
  tree malloc_fn_decl;

  if (!malloc_stmt)
    return;

  malloc_fn_decl = gimple_call_fndecl (malloc_stmt);

  src = cgraph_get_node (context);
  dest = cgraph_get_node (malloc_fn_decl);
  cgraph_create_edge (src, dest, malloc_stmt,
		      gimple_bb (malloc_stmt)->count,
		      compute_call_stmt_bb_frequency
		        (context, gimple_bb (malloc_stmt)));
} 

/* This function generates stmt:
   res = NUM * sizeof(TYPE) and returns it.
   res is filled into RES.  */

static gimple
gen_size (tree num, tree type, tree *res)
{
  tree struct_size = TYPE_SIZE_UNIT (type);
  HOST_WIDE_INT struct_size_int = TREE_INT_CST_LOW (struct_size);
  gimple new_stmt;

  *res = create_tmp_var (TREE_TYPE (num), NULL);

  /* if (*res)
     add_referenced_var (*res); */

  if (exact_log2 (struct_size_int) == -1)
    {
      tree size = build_int_cst (TREE_TYPE (num), struct_size_int);
      new_stmt = gimple_build_assign (*res, fold_build2 (MULT_EXPR,
							 TREE_TYPE (num),
							 num, size));
    }
  else
    {
      tree C = build_int_cst (TREE_TYPE (num), exact_log2 (struct_size_int));

      new_stmt = gimple_build_assign (*res, fold_build2 (LSHIFT_EXPR,
							 TREE_TYPE (num),
							 num, C));
    }

  update_stmt (new_stmt);
  return new_stmt;
}

/* This function generates and returns a statement, that cast variable
   BEFORE_CAST to NEW_TYPE. The cast result variable is stored
   into RES_P. ORIG_CAST_STMT is the original cast statement.  */

static gimple
gen_cast_stmt (tree before_cast, tree new_type, gimple orig_cast_stmt,
	       tree *res_p)
{
  tree lhs, new_lhs = NULL_TREE;
  gimple new_stmt = NULL;

  lhs = gimple_assign_lhs (orig_cast_stmt);
  new_lhs = find_new_var_of_type (lhs, new_type);
    
  gcc_assert (new_lhs);
    
  // new_stmt = gimple_build_assign_with_ops (NOP_EXPR, new_lhs, before_cast, 0);
  new_stmt = gimple_build_assign (new_lhs, before_cast);
  if (IS_TMP (lhs))
    SSA_NAME_DEF_STMT (new_lhs) = new_stmt;
  update_stmt (new_stmt);
  *res_p = new_lhs;

  return new_stmt;
}

/* This function generates set of statements required
   to allocate number NUM of structures of type NEW_TYPE.
   The statements are stored in NEW_STMTS. The statement that contain
   call to malloc is returned. MALLOC_STMT is an original call to malloc.  */

static gimple
create_new_malloc_stmt (gimple malloc_stmt, tree new_type, gimple_seq *new_stmts,
		   tree num)
{
  tree new_malloc_size;
  tree malloc_fn_decl;
  gimple new_stmt;
  tree malloc_res;
  gimple call_stmt;
  unsigned args_num;
  tree lhs, lhs_type;

  gcc_assert (num && malloc_stmt && new_type);
  gcc_assert (*new_stmts == NULL); 

  /* Generate argument to malloc as multiplication of num
     and size of new_type.  */
  new_stmt = gen_size (num, new_type, &new_malloc_size);
  gimple_seq_add_stmt (new_stmts, new_stmt);

  /* Generate new call for malloc.  */
  
  lhs = gimple_call_lhs (malloc_stmt);
  lhs_type = get_type_of_var (lhs);
  if (lhs_type && POINTER_TYPE_P (lhs_type)
      && TREE_CODE (strip_type (lhs_type)) == RECORD_TYPE
      && (is_in_struct_symbols_vec (strip_type (lhs_type)) != -1))
    malloc_res = find_new_var_of_type (lhs, new_type);
  else
    {
      if (TREE_CODE (gimple_call_lhs (malloc_stmt)) == SSA_NAME)    
	malloc_res = make_ssa_name (ptr_type_node, NULL);
      else if (TREE_CODE (gimple_call_lhs (malloc_stmt)) == VAR_DECL)
	malloc_res = create_tmp_var (ptr_type_node, NULL);
      else
	gcc_assert (0);
    }

  malloc_fn_decl = gimple_call_fndecl (malloc_stmt);
  args_num = gimple_call_num_args (malloc_stmt);
  if (dump_file)
    fprintf (dump_file, "\nNum of alloc args is %u", args_num);
  if (args_num == 1)
    /* This is a malloc case.  */
    call_stmt = gimple_build_call (malloc_fn_decl, 1, new_malloc_size);  
  else if (args_num == 2)
    /* This is a case of builtin_alloc*/
    call_stmt = gimple_build_call (malloc_fn_decl, 2, new_malloc_size,
				   gimple_call_arg (malloc_stmt, 1));
  else
    /* We do not support other cases now.  */
    gcc_assert (0);
        
  gimple_call_set_lhs (call_stmt, malloc_res);
  gimple_seq_add_stmt (new_stmts, call_stmt);
  update_stmt (call_stmt);

  if (dump_file) {
    fprintf (dump_file, "\nNew malloc is ");
    print_gimple_stmt (dump_file, call_stmt, 0, TDF_VOPS);
  }

  /*final_stmt = get_final_alloc_stmt (malloc_stmt);
  gcc_assert (final_stmt);
  gimple_seq_add_stmt (new_stmts, new_stmt);*/

  return call_stmt;
}

/* This function adds allocation sites for peeled structures.
   M_DATA is vector of allocation sites of function CONTEXT.  */

static void
create_new_alloc_sites_1 (tree context)
{
  unsigned int j;
  alloc_site_p call; 

  FOR_EACH_VEC_ELT (alloc_sites, j, call)
    {
      gimple stmt = call->stmt;
      struct_symbols str = call->str;
      tree num;
      gimple_seq new_stmts = NULL;
      gimple last_stmt = stmt; /* = get_final_alloc_stmt (stmt); */
      unsigned i;
      tree type;

      if (dump_file)
	fprintf (dump_file, "\nAllocation site number %d", j);

      num = gen_num_of_structs_in_malloc (stmt, str->struct_decl, &new_stmts);
      if (new_stmts)
	{	  
	  gimple last_stmt_tmp = gimple_seq_last_stmt (new_stmts);
	  insert_seq_after_stmt (stmt, new_stmts);
	  last_stmt = last_stmt_tmp;
	  if (dump_file)
	    print_gimple_stmt (dump_file, last_stmt, 0, TDF_SLIM);
	}

      /* Generate an allocation statements for each new structure type.  */
      FOR_EACH_VEC_ELT (str->new_types, i, type)
	{
	  gimple new_malloc_stmt = NULL;
	  gimple use_stmt;
	  tree alloc_res;
	  tree malloc_res;	  
	  tree cast_res; 
	  imm_use_iterator imm_iter;
	  gimple new_stmt;
	  tree vuse = NULL_TREE, vdef = NULL_TREE;

	  if (dump_file)
	    fprintf (dump_file, "\nNew type number %d", i);

	  new_stmts = NULL;
	  if (dump_file)
	    {
	      vuse = gimple_vuse (stmt);
	      if (vuse) {
		fprintf (dump_file, "\nVuse of malloc stmt is ");
		print_generic_expr (dump_file, vuse, 0); }
	      vdef = gimple_vdef (stmt);
	      if (vdef) {
		fprintf (dump_file, "\nVdef of malloc stmt is ");
		print_generic_expr (dump_file, vdef, 0); }
	    }
	  
	  new_malloc_stmt = create_new_malloc_stmt (stmt, type, &new_stmts, num);
	  insert_seq_after_stmt (last_stmt, new_stmts);
	  update_cgraph_with_malloc_call (new_malloc_stmt, context);

	  /* Create new cast statements. */

	  malloc_res = gimple_get_lhs (new_malloc_stmt);
	  /* If result of malloc is already a variable of desired structure type 
	     (and not pointer to void) we need not fix its uses.  */
	  if (strip_type (get_type_of_var (malloc_res)) == type)
	    continue;

	  alloc_res = gimple_get_lhs (stmt);
	  gcc_assert (TREE_CODE (alloc_res) == SSA_NAME);
	  
	  FOR_EACH_IMM_USE_STMT (use_stmt, imm_iter, alloc_res)
	    {
	      new_stmt = gen_cast_stmt (malloc_res, type, use_stmt, &cast_res);
	      fprintf (dump_file, "\nUse stmt is ");
	      print_gimple_stmt (dump_file, use_stmt, 0, TDF_VOPS);
	      if (new_stmt)
		{
		  insert_after_stmt (use_stmt, new_stmt);
		}
	    }
	}
    }
}

/* Create new allocation sites for the function represented by NODE.  */

static void
create_new_alloc_sites (struct cgraph_node *node)
{
  if (!alloc_sites.exists () || !alloc_sites.length ())
    {
      if (dump_file)
	fprintf (dump_file, "Allocation sites do not exist...");
      return;
    }

  create_new_alloc_sites_1 (node->decl);
}

/* Hash value for field_access_site.  */

static hashval_t
field_acc_hash (const void *x)
{
  return htab_hash_pointer (((const struct field_access_site *)x)->stmt);
}

/* This function returns nonzero if stmt of field_access_site X
   is equal to Y.  */

static int
field_acc_eq (const void *x, const void *y)
{
  return ((const struct field_access_site *)x)->stmt == (const_gimple)y;
}

/* Hash value for access_site.  */

static hashval_t
acc_hash (const void *x)
{
  return htab_hash_pointer (((const struct access_site *)x)->stmt);
}

/* Return nonzero if stmt of access_site X is equal to Y.  */

static int
acc_eq (const void *x, const void *y)
{
  return ((const struct access_site *)x)->stmt == (const_gimple)y;
}

/* Given structure type SRT_TYPE and field FIELD,
   this function is looking for a field with the same name
   and type as FIELD in STR_TYPE. It returns it if found,
   or NULL_TREE otherwise.  */

static tree
find_field_in_struct_1 (tree str_type, tree field)
{
  tree str_field;

  if (!DECL_NAME (field))
    return NULL;

  for (str_field = TYPE_FIELDS (str_type); str_field;
       str_field = TREE_CHAIN (str_field))
    {

      if (!DECL_NAME (str_field))
	continue;

      if (compare_fields (field, str_field))
	return str_field;
    }

  return NULL_TREE;
}

/* Given a field declaration FIELD_DECL, this function
   returns corresponding field entry in structure STR.  */

static struct field_entry *
find_field_in_struct (struct_symbols str, tree field_decl)
{
  int i;

  tree field = find_field_in_struct_1 (str->struct_decl, field_decl);

  for (i = 0; i < str->num_fields; i++)
    if (str->fields[i].decl == field)
      return &(str->fields[i]);

  return NULL;
}

/* This function returns the structure field access, defined by STMT,
   if it is already in hashtable of accesses F_ACCS.  */

static struct field_access_site *
is_in_field_accs (gimple stmt, htab_t f_accs)
{
  return (struct field_access_site *)
    htab_find_with_hash (f_accs, stmt, htab_hash_pointer (stmt));
}

/* This function adds an access ACC to the hashtable
   of FIELD access sites.  */

static void
add_field_acc_to_acc_sites (struct field_access_site *acc,
			    struct field_entry *field)
{
  void **slot;

  if (!field->acc_sites)
    field->acc_sites = htab_create (32, field_acc_hash, field_acc_eq, NULL);

  gcc_assert (!is_in_field_accs (acc->stmt, field->acc_sites));
  slot = htab_find_slot_with_hash (field->acc_sites, acc->stmt,
				   htab_hash_pointer (acc->stmt),
				   INSERT);
  *slot = acc;
}

/* This function adds the VAR to vector of variables of
   an access site defined by statement STMT. If access entry
   with statement STMT does not exist in hashtable of
   accesses ACCS, this function creates it.  */

static void
add_access_to_acc_sites (gimple stmt, tree var, struct_symbols str)
{
   struct access_site *acc;

   if (!str->accs)
     str->accs = htab_create (32, acc_hash, acc_eq, NULL);
   
   acc = (struct access_site *)
     htab_find_with_hash (str->accs, stmt, htab_hash_pointer (stmt));

   if (!acc)
     {
       void **slot;

       acc = XNEW (struct access_site);
       acc->stmt = stmt;
       if (!is_gimple_debug (stmt))
	 (acc->vars).create (0);
       slot = htab_find_slot_with_hash (str->accs, stmt,
					htab_hash_pointer (stmt), INSERT);
       *slot = acc;
     }
   if (!is_gimple_debug (stmt))
     (acc->vars).safe_push (var);
}

/* This function creates empty field_access_site node.  */

static inline struct field_access_site *
make_field_acc_node (void)
{
  return XCNEW (struct field_access_site);
}

/* Check definition of base variable.  */

/* 
static bool
check_ref_def_stmt (gimple ref_def_stmt, tree ref_var)
{
  if (!(ref_def_stmt)
      || (gimple_code (ref_def_stmt) != GIMPLE_ASSIGN))
    {
      if (dump_file)
	fprintf (dump_file, "  ref_def_stmt is not GIMPLE_ASSIGN ");
      gcc_assert (SSA_NAME_VAR (ref_var));
      if (is_global_var (SSA_NAME_VAR (ref_var)))
	{
	  if (dump_file)
	    fprintf (dump_file, "  is global ");
	  return true;
	}
      else if (TREE_CODE (SSA_NAME_VAR (ref_var)) == PARM_DECL)
	{
	  if (dump_file)
	    fprintf (dump_file, "  is PARM_DECL ");
	  return true;
	}
      else
	{
	  if (dump_file)
	    fprintf (dump_file, "  is neither global nor PARM_DECL");
	  return false;
	}
    }
  else
      if (dump_file)
	{
	  fprintf (dump_file, "  ref_var_def_stmt = ");
	  print_gimple_stmt (dump_file, acc->ref_def_stmt, 0, 0);      
	}

  return true;
} */

/* This function returns true if access ACC corresponds to the pattern
   generated by compiler when an address of element i of an array
   of structures STR_DECL (pointed by p) is calculated (p[i]). If this
   pattern is recognized correctly, this function returns true
   and fills missing fields in ACC. Otherwise it returns false.  */

static bool
decompose_indirect_ref_acc (tree str_decl, struct field_access_site *acc)
{
  tree ref_var, ref_offset;
  tree struct_size, op0, op1;
  /* tree before_cast; */
  enum tree_code rhs_code;
  tree op0type, op1type;
  gimple ref_def_stmt;

  ref_var = TREE_OPERAND (acc->ref, 0);
  ref_offset = TREE_OPERAND (acc->ref, 1);

  if (dump_file)
    {
      fprintf (dump_file, "  ref_var = ");
      print_generic_expr (dump_file, ref_var, 0);      
      fprintf (dump_file, " is %s tree code ", 
	       get_tree_code_name (TREE_CODE (ref_var)));

      fprintf (dump_file, "  ref_offset = ");
      print_generic_expr (dump_file, ref_offset, 0);      
      fprintf (dump_file, " is %s tree code ", 
	       get_tree_code_name (TREE_CODE (ref_offset)));
    }

  /* Check offset if it's present inside reference.  */
  if (TREE_CODE (ref_offset) == INTEGER_CST) 
    {
      if (TREE_INT_CST_LOW (ref_offset) % 
	  TREE_INT_CST_LOW (TYPE_SIZE_UNIT (str_decl)))
	{
	  if (dump_file)
	    {
	      fprintf (dump_file, "   ref_offset =  %lu", 
		   TREE_INT_CST_LOW(ref_offset));
	      fprintf (dump_file, "   struct_size =  %lu", 
		       TREE_INT_CST_LOW(TYPE_SIZE_UNIT(str_decl)));
	      fprintf (dump_file, "   ref_offset mod struct_size =  %lu", 
		       (TREE_INT_CST_LOW (ref_offset) 
			% TREE_INT_CST_LOW (TYPE_SIZE_UNIT (str_decl))));	  
	      fprintf (dump_file, "   offset ");
	      print_generic_expr (dump_file, ref_offset, 0);      
	      fprintf (dump_file, "   does not fit.");
	    }
	  return false;
	}
      else
	{
	  /* Memory access is base plus constant: 
	     MEM[(struct my_struct *)_32 + 8B].a */
	  acc->insn_base = ref_var;
	  acc->insn_offset = ref_offset;
	  acc->insn_num = build_int_cst (TREE_TYPE (acc->insn_offset), TREE_INT_CST_LOW (ref_offset) /
				    TREE_INT_CST_LOW (TYPE_SIZE_UNIT (str_decl)));
	  //return true;

	}
    }

  if (TREE_CODE (ref_var) != SSA_NAME)
    {
      if (dump_file)
	fprintf (dump_file, "  rev_var is not SSA_NAME ");

      return false;
    }

  ref_def_stmt = SSA_NAME_DEF_STMT (ref_var);

  while (ref_def_stmt && ref_var)
    {

      if (gimple_code (ref_def_stmt) != GIMPLE_ASSIGN)
	return true;

      rhs_code = gimple_assign_rhs_code (ref_def_stmt);
      
      if (rhs_code == SSA_NAME)
	{
	  ref_var = gimple_assign_rhs1(ref_def_stmt);
	  ref_def_stmt = SSA_NAME_DEF_STMT (ref_var);
	  continue;
	}
      else if (rhs_code == PLUS_EXPR
	  || rhs_code == MINUS_EXPR
	  || rhs_code == POINTER_PLUS_EXPR)
	{
	  if (dump_file)
	    {
	      fprintf (dump_file, "  Tree cpde of rhs of def is %s ", 
		       get_tree_code_name (rhs_code));
	    }

	  op0 = gimple_assign_rhs1 (ref_def_stmt);
	  op1 = gimple_assign_rhs2 (ref_def_stmt);
  
	  if (dump_file)
	    {
	      fprintf (dump_file, "  Tree code of op0 is  %s ", 
		       get_tree_code_name (TREE_CODE (op0)));
	      fprintf (dump_file, "  Tree code of op1 is %s ", 
		       get_tree_code_name (TREE_CODE (op1)));
	    }

	  op0type = TYPE_MAIN_VARIANT (TREE_TYPE (op0));
	  op1type = TYPE_MAIN_VARIANT (TREE_TYPE (op1));

	  /* One of op0 and op1 is of pointer type and the other is numerical.  */
	  if (POINTER_TYPE_P (op0type) && NUMERICAL_TYPE_CHECK (op1type))
	    {
	      acc->base = op0;
	      acc->offset = op1;
	      if (dump_file)
		fprintf (dump_file, "\nop1 is base and op2 is offset.\n");
	    }
	  else if (POINTER_TYPE_P (op1type) && NUMERICAL_TYPE_CHECK (op0type))
	    {
	      acc->base = op1;
	      acc->offset = op0;
	      if (dump_file)
		fprintf (dump_file, "\nop0 is base and op1 is offset.\n");
	    }
	  else 
	    {
	      if (dump_file)
		fprintf (dump_file, "\nop1 and op2 are not pointer plus number.\n");
	      return false;	      
	    }

	  /* We care about def stmt ouly when it is a calculation of offset.
	     Otherwise it should be NULL. */
	  acc->ref_def_stmt = SSA_NAME_DEF_STMT (ref_var);
      
	  struct_size = TYPE_SIZE_UNIT (str_decl);
	  if (!is_result_of_mult (acc->offset, &acc->num, struct_size))
	    return false;
	  else
	    return true;
	}
      else
	return true;
    }
    
  return true;
} 

/* This function checks whether the access ACC of structure type STR
   is of the form suitable for transformation. If yes, it returns true.
   False otherwise.  */

static bool
decompose_access (tree str_decl, struct field_access_site *acc)
{
  gcc_assert (acc->ref);

  if (TREE_CODE (acc->ref) == MEM_REF)
    {
      if (dump_file)
	fprintf (dump_file, "  MEM_REF\n");
      //debug_tree(acc->comp_ref);
      return decompose_indirect_ref_acc (str_decl, acc);
    }
  else if (TREE_CODE (acc->ref) == ARRAY_REF)
    {
      if (dump_file)
	fprintf (dump_file, "  ARRAY_REF\n");
      return true;
    }
  else if (TREE_CODE (acc->ref) == VAR_DECL)
    {
      if (dump_file)
	fprintf (dump_file, "  VAR_DECL\n");
      return true;
    }


  if (dump_file)
    fprintf (dump_file, "  NON_OF_DEFINED\n");
  return false;
}

/* Callback function for walk_tree called from collect_accesses_in_bb
   function. DATA is the statement which is analyzed.  */

static tree
get_stmt_accesses (tree *tp, int *walk_subtrees, void *data)
{
  struct walk_stmt_info *wi = (struct walk_stmt_info *) data;
  gimple stmt = (gimple) wi->info;
  tree t = *tp;

  if (!t)
    return NULL;
  else
    {
      if (dump_file)
	{	  
	  fprintf (dump_file, "\nTREE_CODE (");
	  print_generic_expr (dump_file, t, 0);
	  fprintf (dump_file, " ) is %s in stmt ", get_tree_code_name (TREE_CODE (t)));
	  print_gimple_stmt (dump_file, stmt, 0, 0);
	}
      *walk_subtrees = 1;
    }

  switch (TREE_CODE (t))
    {
      /* case BIT_FIELD_REF:
      {
	tree var = TREE_OPERAND(t, 0);
	tree type = TYPE_MAIN_VARIANT (strip_type (get_type_of_var (var)));
	unsigned i = find_structure (type);

	if (i != VEC_length (structure, structures))
	  {
	    if (is_gimple_debug (stmt))
	      {
		d_str str;

		str = VEC_index (structure, structures, i);
		add_access_to_acc_sites (stmt, NULL, str->accs);
		*walk_subtrees = 0;
		break;
	      }
	    if (dump_file)
	      {
		fprintf (dump_file, "\nThe type ");
		print_generic_expr (dump_file, type, 0);
		fprintf (dump_file, " has bitfield.");
	      }
	    remove_structure (i);
	  }
      }
      break; */

    case COMPONENT_REF:
      {
	tree ref = TREE_OPERAND (t, 0);
	tree field_decl = TREE_OPERAND (t, 1);


	if ((TREE_CODE (ref) == MEM_REF
	     || TREE_CODE (ref) == ARRAY_REF
	     || TREE_CODE (ref) == VAR_DECL)
	    && TREE_CODE (field_decl) == FIELD_DECL)
	  {
	    tree type = TYPE_MAIN_VARIANT (TREE_TYPE (ref));
	    int i = is_in_struct_symbols_vec (type);
	    struct_symbols str;

	    if (i != -1)
	      {
		str = struct_symbols_vec[i];

		struct field_entry * field =
		  find_field_in_struct (str, field_decl);

		if (is_gimple_debug (stmt))
		  {
		    add_access_to_acc_sites (stmt, NULL, str);
		    *walk_subtrees = 0;
		    break;
		  }

		if (field)
		  {
		    struct field_access_site *acc = make_field_acc_node ();

		    gcc_assert (acc);

		    acc->stmt = stmt;
		    acc->comp_ref = t;
		    acc->ref = ref;
		    acc->field_decl = field_decl;
		    acc->cast_stmt = NULL;
		    acc->num = NULL_TREE;
		    acc->base = NULL_TREE;
		    acc->offset = NULL_TREE;
		    acc->insn_base = NULL_TREE;
		    acc->insn_offset = NULL_TREE;
		    acc->insn_num = NULL_TREE;
		    acc->ref_def_stmt = NULL;

		    /* Check whether the access is of the form
		       we can deal with.  */
		    if (!decompose_access (str->struct_decl, acc))
		      {
			if (dump_file)
			  {
			    fprintf (dump_file, "\nThe type ");
			    print_generic_expr (dump_file, type, 0);
			    fprintf (dump_file,
				     " has complicate access in statement ");
			    print_gimple_stmt (dump_file, stmt, 0, 0);
			  }

			//remove_structure (i);
			free (acc);
			gcc_assert (0);
		      }
		    else
		      {
			/* Increase count of field.  */
			 basic_block bb = gimple_bb (stmt);
			 field->count += bb->count; 

			/* Add stmt to the acc_sites of field.  */
			add_field_acc_to_acc_sites (acc, field);
		      }
		    *walk_subtrees = 0;
		  }
	      }
	  }
      }
      break;

    case COND_EXPR:
      {
	tree cond = COND_EXPR_COND (t);
	int i;
	for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (cond)); i++)
	  {
	    tree t = TREE_OPERAND (cond, i);

	    *walk_subtrees = 1;
	    walk_tree (&t, get_stmt_accesses, data, NULL);
	  }
	*walk_subtrees = 0;
      }
      break;

    case VAR_DECL:
    case SSA_NAME:
      {
	int i;

	if (TREE_CODE (t) == SSA_NAME)
	  if (SSA_NAME_VAR (t))	    
	    t = SSA_NAME_VAR (t);

	i = is_in_struct_symbols_vec (strip_type (get_type_of_var (t)));

	if (i != -1)
	  {
	    struct_symbols str;

	    str = struct_symbols_vec[i];
	    add_access_to_acc_sites (stmt, t, str);
	  }
	*walk_subtrees = 0;
      }
      break;

    default:
      return NULL;
      } 

  return NULL;
}

/* Collect accesses to the structure types that appear in basic block BB.  */

static void
collect_accesses_in_bb (basic_block bb)
{
  gimple_stmt_iterator bsi;
  struct walk_stmt_info wi;

  memset (&wi, 0, sizeof (wi));

  for (bsi = gsi_start_bb (bb); !gsi_end_p (bsi); gsi_next (&bsi))
    {
      gimple stmt = gsi_stmt (bsi);

      wi.info = (void *) stmt;
      walk_gimple_op (stmt, get_stmt_accesses, &wi);
    }
}

/* This function prints a field access site, defined by SLOT.  */

static int
dump_field_acc (void **slot, void *data ATTRIBUTE_UNUSED)
{
  struct field_access_site *f_acc =
    *(struct field_access_site **) slot;

  if (f_acc->stmt)
    {
      fprintf(dump_file, "\nstmt: ");
      print_gimple_stmt (dump_file, f_acc->stmt, 0, 0);
    }
  if (f_acc->ref_def_stmt)
    {
      fprintf(dump_file, "\nref_def_stmt: ");      
      print_gimple_stmt (dump_file, f_acc->ref_def_stmt, 0, 0);
    }
  if (f_acc->cast_stmt)
    {
      fprintf(dump_file, "\ncast_stmt: ");      
      print_gimple_stmt (dump_file, f_acc->cast_stmt, 0, 0);
    }
  if (f_acc->comp_ref)
    {
      fprintf(dump_file, "\ncomp_ref: ");      
      print_generic_expr (dump_file, f_acc->comp_ref, 0);
    }
  if (f_acc->field_decl)
    {
      fprintf(dump_file, "\nfield_decl: ");      
      print_generic_expr (dump_file, f_acc->field_decl, 0);
    }
  if (f_acc->ref)
    {
      fprintf(dump_file, "\nref: ");      
      print_generic_expr (dump_file, f_acc->ref, 0);
    }
  if (f_acc->num)
    {
      fprintf(dump_file, "\nnum: ");      
      print_generic_expr (dump_file, f_acc->num, 0);
    }
  if (f_acc->offset)
    {
      fprintf(dump_file, "\noffset: ");      
      print_generic_expr (dump_file, f_acc->offset, 0);
    }
  if (f_acc->base)
    {
      fprintf(dump_file, "\nbase: ");      
      print_generic_expr (dump_file, f_acc->base, 0);
    }
  if (f_acc->insn_offset)
    {
      fprintf(dump_file, "\ninsn_offset: ");      
      print_generic_expr (dump_file, f_acc->insn_offset, 0);
    }
  if (f_acc->insn_base)
    {
      fprintf(dump_file, "\ninsn_base: ");      
      print_generic_expr (dump_file, f_acc->insn_base, 0);
    }

  return 1;
}

/* Print field accesses from hashtable F_ACCS.  */

static void
dump_field_acc_sites (htab_t f_accs)
{
  if (!dump_file)
    return;

  if (f_accs)
    htab_traverse (f_accs, dump_field_acc, NULL);
}

/* This function prints an access site, defined by SLOT.  */

static int
dump_acc (void **slot, void *data ATTRIBUTE_UNUSED)
{
  struct access_site *acc = *(struct access_site **) slot;
  tree var;
  unsigned i;

  fprintf(dump_file, "\n");
  if (acc->stmt)
    print_gimple_stmt (dump_file, acc->stmt, 0, 0);
  fprintf(dump_file, " : ");

  if (acc->vars.exists ())
    {
      FOR_EACH_VEC_ELT (acc->vars, i, var)
	{
	  print_generic_expr (dump_file, var, 0);
	  fprintf(dump_file, ", ");
	}
    }
  return 1;
}

/* Print non-field accesses from hashtable ACCS of structure.  */

static void
dump_access_sites (htab_t accs)
{
  if (!dump_file)
    return;

  if (accs)
    htab_traverse (accs, dump_acc, NULL);
}


/* This function prints non-field and field accesses
   of the structure STR.  */

static void
dump_accs (struct_symbols str)
{
  int i;

  fprintf (dump_file, "\nAccess sites of struct ");
  print_generic_expr (dump_file, str->struct_decl, 0);

  for (i = 0; i < str->num_fields; i++)
    {
      fprintf (dump_file, "\nAccess site of field ");
      print_generic_expr (dump_file, str->fields[i].decl, 0);
      dump_field_acc_sites (str->fields[i].acc_sites);
      fprintf (dump_file, ":\n");
    }
  fprintf (dump_file, "\nGeneral access sites\n");
  dump_access_sites (str->accs);
}

/* Print collected accesses.  */

static void
dump_accesses (void)
{
  struct_symbols str;
  unsigned i;

  if (!dump_file)
    return;

  if (struct_symbols_vec.exists ())
    {   
      FOR_EACH_VEC_ELT (struct_symbols_vec, i, str)
	dump_accs (str);
    }
}
/* Auxiliary structure for a lookup over field accesses. */
struct find_stmt_data
{
  bool found;
  gimple stmt;
};

/* This function looks for DATA->stmt among
   the statements involved in the field access,
   defined by SLOT. It stops when it's found. */

static int
find_in_field_accs (void **slot, void *data)
{
  struct field_access_site *f_acc = *(struct field_access_site **) slot;
  gimple stmt = ((struct find_stmt_data *)data)->stmt;

  if (f_acc->stmt == stmt
      || f_acc->ref_def_stmt == stmt
      || f_acc->cast_stmt == stmt)
    {
      ((struct find_stmt_data *)data)->found = true;
      return 0;
    }
  else
    return 1;
}

/* This function checks whether STMT is part of field
   accesses of structure STR. It returns true, if found,
   and false otherwise.  */

static bool
is_part_of_field_access (gimple stmt, struct_symbols str)
{
  int i;

  for (i = 0; i < str->num_fields; i++)
    {
      struct find_stmt_data data;
      data.found = false;
      data.stmt = stmt;

      if (str->fields[i].acc_sites)
	htab_traverse (str->fields[i].acc_sites, find_in_field_accs, &data);

      if (data.found)
	return true;
    }

  return false;
}

/* If ALLOC_STMT is D.2225_7 = <alloc_func> (D.2224_6);
   and it is a part of allocation of a structure,
   then it is usually followed by a cast stmt
   p_8 = (struct str_t *) D.2225_7, or simply 
   p_8 = D.2225_7; 
   We check whether STMT is of this kind, and return 
   true if it is.  */

static bool
is_final_alloc_stmt (gimple alloc_stmt, gimple stmt)
{
  tree alloc_res;
  imm_use_iterator imm_iter;
  gimple use_stmt;
  bool res = false;


  if (!alloc_stmt)
    return NULL;

  if (!is_gimple_call (alloc_stmt))
    return NULL;

  alloc_res = gimple_get_lhs (alloc_stmt);

  if (TREE_CODE (alloc_res) != SSA_NAME)
    return NULL;

  FOR_EACH_IMM_USE_STMT (use_stmt, imm_iter, alloc_res)
    {
      if (use_stmt == stmt)
	res=true;
    }

  return res;
}

/* This function returns true if STMT is one of allocation
   sites. It returns false otherwise.  */

static bool
is_part_of_calls_with_structs (gimple stmt)
{
  if (calls_with_structs.exists ())
    {
      call_with_structs call;
      unsigned i;

      FOR_EACH_VEC_ELT (calls_with_structs, i, call)
	{
	  if (call->call_stmt == stmt)
	    return true;
	}
    }
  return false;
}

/* This function returns true if STMT is one of allocation
   sites. It returns false otherwise.  */

static bool
is_part_of_malloc (gimple stmt)
{
  if (alloc_sites.exists ())
    {
      alloc_site_p call;
      unsigned i;

      FOR_EACH_VEC_ELT (alloc_sites, i, call)
	{
	  if (call->stmt == stmt
	      || is_final_alloc_stmt (call->stmt, stmt))
	    return true;
	}
    }
  return false;
}

/* This function excludes statements, that are
   part of allocation sites or field accesses, from the
   hashtable of general accesses. SLOT represents general
   access that will be checked. DATA is en entry 
   in struct_symbols vector. */

static int
exclude_from_accs (void **slot, void *data)
{
  struct access_site *acc = *(struct access_site **) slot;
  struct_symbols str = (struct_symbols)data;

  if (is_part_of_malloc (acc->stmt)
      || is_part_of_field_access (acc->stmt, str)
      || is_part_of_calls_with_structs (acc->stmt))
    {
      (acc->vars).release ();
      free (acc);
      htab_clear_slot (str->accs, slot);
    }
  return 1;
}

/* This function excludes statements that are part of allocation sites or
   field accesses from the hashtable of general accesses of the structure
   type STR.  */

static void
exclude_acc_dupls_1 (struct_symbols str)
{
  struct_symbols dt = str;

  if (str->accs)
    htab_traverse (str->accs, exclude_from_accs, dt);
}

/* We exclude from non-field accesses of the structure
   all statements that will be treated as part of the structure
   allocation sites or field accesses.  */

static void
exclude_acc_dupls (void)
{
  struct_symbols str;
  unsigned i;

  if (struct_symbols_vec.exists ())
    {   
      FOR_EACH_VEC_ELT (struct_symbols_vec, i, str)
	exclude_acc_dupls_1 (str);
    }
}

/* Currently we support only EQ_EXPR or NE_EXPR conditions.
   COND_STMT is a condition statement to check.  */

static bool
is_safe_cond_expr (gimple cond_stmt)
{
  tree arg0, arg1;
  int str0, str1;
  bool s0, s1;

  if (gimple_cond_code (cond_stmt) != EQ_EXPR
      && gimple_cond_code (cond_stmt) != NE_EXPR)
    return false;

  arg0 = gimple_cond_lhs (cond_stmt);
  arg1 = gimple_cond_rhs (cond_stmt);

  str0 = is_in_struct_symbols_vec (strip_type (get_type_of_var (arg0)));
  str1 = is_in_struct_symbols_vec (strip_type (get_type_of_var (arg1)));

  s0 = (str0 != -1) ? true : false;
  s1 = (str1 != -1) ? true : false;

  if (!s0 && !s1)
    return false;

  /* For now we allow only comparison with 0 or NULL.  */
  if (!integer_zerop (arg0) && !integer_zerop (arg1))
    return false;

  return true;
}

/* This function checks whether an access statement, pointed by SLOT,
   is a condition we are capable to transform.  It returns false if not,
   setting bool *DATA to false.  */

static int
safe_cond_expr_check (void **slot, void *data)
{
  struct access_site *acc = *(struct access_site **) slot;

  if (gimple_code (acc->stmt) == GIMPLE_COND
      && !is_safe_cond_expr (acc->stmt))
    {
      if (dump_file)
	{
	  fprintf (dump_file, "\nUnsafe conditional statement ");
	  print_gimple_stmt (dump_file, acc->stmt, 0, 0);
	}
      *(bool *) data = false;
      return 0;
    }
  return 1;
}

/* This function checks whether the accesses of structures in conditional
   expressions are of the kind we are capable to transform.
   If not, such structures are removed from the vector of structures.  */

static void
check_cond_exprs (void)
{
  struct_symbols str;
  unsigned i;

  if (struct_symbols_vec.exists ())
    {   
      bool safe_p = true;

      FOR_EACH_VEC_ELT (struct_symbols_vec, i, str)
	{
	  if (str->accs)
	    htab_traverse (str->accs, safe_cond_expr_check, &safe_p);
	  /* Conditional stmt is of the form we cannot tranform.  */
	  if (!safe_p)	    
	    gcc_assert (0);
	}
    }
}

/* This function collects accesses of structures in current function.  */

static void
collect_data_accesses (void)
{
  basic_block bb;

  if (!cfun)
    return;

  /* Collect accesses for each basic blocks separately.  */
  FOR_EACH_BB_FN (bb, cfun)
    collect_accesses_in_bb (bb);
  exclude_acc_dupls ();
  check_cond_exprs ();
  dump_accesses ();
}

/* This function is a callback for traversal over a structure accesses.
   It frees an access represented by SLOT.  */

static int
free_accs (void **slot, void *data ATTRIBUTE_UNUSED)
{
  struct access_site * acc = *(struct access_site **) slot;

  (acc->vars).release ();
  free (acc);
  return 1;
}

/* This is a callback function for traversal over field accesses.
   It frees a field access represented by SLOT.  */

static int
free_field_accs (void **slot, void *data ATTRIBUTE_UNUSED)
{
  struct field_access_site *f_acc = *(struct field_access_site **) slot;

  free (f_acc);
  return 1;
}

/* This function free non-field accesses from hashtable ACCS.  */

static void
free_accesses (htab_t accs)
{
  if (accs)
    {
      htab_traverse (accs, free_accs, NULL);
      htab_delete (accs);
    }
}

/* This function free field accesses hashtable F_ACCS.  */

static void
free_field_accesses (htab_t f_accs)
{
  if (f_accs)
    {
      htab_traverse (f_accs, free_field_accs, NULL);
      htab_delete (f_accs);
    }
}

/* This function free non-field and field access of all structures, 
   allocated for current function.  */

static void
free_data_accesses (void)
{

  struct_symbols str;
  unsigned i;
  int j;

  if (struct_symbols_vec.exists () && !struct_symbols_vec.is_empty ())
    {
      FOR_EACH_VEC_ELT (struct_symbols_vec, i, str)
	{

	  if (str->fields)
	    {
	      for (j = 0; j < str->num_fields; j++)
		{
		  free_field_accesses (str->fields[j].acc_sites);
		  str->fields[j].acc_sites = 0;
		}
	    }

	  if (str->accs)
	    {
	      free_accesses (str->accs);
	      str->accs = NULL;
	    }	  
	}
    }
}

/* This struct represent data used for walk_tree
   called from function find_pos_in_stmt.
   - ref is a tree to be found,
   - and pos is a pointer that points to ref in stmt.  */
struct ref_pos
{
  tree *pos;
  tree ref;
  tree container;
};

/* This is a callback function for walk_tree, called from
   collect_accesses_in_bb function. DATA is a pointer to ref_pos structure.
   When *TP is equal to DATA->ref, the walk_tree stops,
   and found position, equal to TP, is assigned to DATA->pos.  */

static tree
find_pos_in_stmt_1 (tree *tp, int *walk_subtrees, void * data)
{
  struct walk_stmt_info *wi = (struct walk_stmt_info *) data;
  struct ref_pos *r_pos = (struct ref_pos *) wi->info;
  tree ref = r_pos->ref;
  tree t = *tp;

  if (t == ref || (TREE_CODE (t) == SSA_NAME && SSA_NAME_VAR (t) == ref))
    {
      r_pos->pos = tp;
      return t;
    }

  r_pos->container = t;
  *walk_subtrees = 1;
  return NULL_TREE;
}


/* This function looks for the pointer of REF in STMT,
   It returns it, if found, and NULL otherwise.  */

static tree *
find_pos_in_stmt (gimple stmt, tree ref, struct ref_pos * r_pos)
{
  struct walk_stmt_info wi;

  r_pos->ref = ref;
  r_pos->pos = NULL;
  r_pos->container = NULL_TREE;
  memset (&wi, 0, sizeof (wi));
  wi.info = r_pos;
  walk_gimple_op (stmt, find_pos_in_stmt_1, &wi);

  return r_pos->pos;
}

/* This function returns component_ref with the BASE and
   field named FIELD_ID from structure TYPE.  */

static inline tree
build_comp_ref (tree base, tree field_id, tree type, 
		struct field_access_site *acc)
{
  tree field;
  bool found = false;

  if (dump_file)
    {
      fprintf (dump_file, "\nnew_type is ");
      print_generic_expr (dump_file, type, 0);
    }

  /* Find field of structure type with the same name as field_id. */
  for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
    {
      if (DECL_NAME (field) == field_id)
	{
	  found = true;
	  break;
	}
    }


  gcc_assert (found);

  if (acc->insn_offset && TREE_CODE (acc->insn_offset) == INTEGER_CST && acc->insn_num)
    {
      tree mem_ref;
      tree new_offset;
      HOST_WIDE_INT new_off;

      new_off = TREE_INT_CST_LOW (TYPE_SIZE_UNIT (type)) * 
	TREE_INT_CST_LOW (acc->insn_num);
      
      new_offset = build_int_cst (TREE_TYPE (base), new_off);
      if (dump_file)
	{
	  fprintf (dump_file, "\nnew_type is");
	  print_generic_expr (dump_file, type, 0);
	  fprintf (dump_file, "\nbase type is ");
	  print_generic_expr (dump_file, TREE_TYPE (base), 0);
	  fprintf (dump_file, "\nbase  is ");
	  print_generic_expr (dump_file, base, 0);
	  fprintf (dump_file, "\nnew_offset type is ");
	  print_generic_expr (dump_file, TREE_TYPE (new_offset), 0);
	}
      mem_ref = fold_build2 (MEM_REF, /*TREE_TYPE (base)*/type, base, new_offset);
      return build3 (COMPONENT_REF, TREE_TYPE (field), mem_ref, field, NULL_TREE);
    }
  else 
    return build3 (COMPONENT_REF, TREE_TYPE (field), base, field, NULL_TREE);
}

/* This function replace field access ACC by the new
   field access of structure type NEW_TYPE.  */

static void
replace_field_acc (struct field_access_site *acc, tree new_type)
{
  tree ref_var /* = acc->ref */;
  tree new_ref;
  tree lhs, rhs;
  tree *pos;
  tree new_acc;
  tree field_id = DECL_NAME (acc->field_decl);
  vec<type_wrapper_t> wrapper;
  struct ref_pos r_pos;

  wrapper.create (10);

  ref_var = (acc->insn_base)? acc->insn_base : acc->ref;

  if (dump_file)
    {
      fprintf (dump_file, "\nref_ref is before stripping");
      print_generic_expr (dump_file, ref_var, 0);
    }

  while (TREE_CODE (ref_var) == MEM_REF 
	    || TREE_CODE (ref_var) == ARRAY_REF)
    {
      type_wrapper_t wr;

      if (TREE_CODE (ref_var) == MEM_REF)
	{
	  wr.wrap = 0;
	  wr.domain = 0;
	}
      else
	{
	  wr.wrap = 1;
	  wr.domain = TREE_OPERAND (ref_var, 1);
	}
      wrapper.safe_push (wr);
      ref_var = TREE_OPERAND (ref_var, 0);
      if (dump_file)
	{
	  fprintf (dump_file, "\nref_var is after stripping");
	  print_generic_expr (dump_file, ref_var, 0);
	}
    }

  new_ref = find_new_var_of_type (ref_var, new_type);
  //finalize_global_creation (new_ref);
  if (dump_file)
    {
      fprintf (dump_file, "\nnew_ref is before wraping");
      print_generic_expr (dump_file, new_ref, 0);
    }

  while (!wrapper.is_empty ())
    {
      tree type = TREE_TYPE (TREE_TYPE (new_ref));
      type_wrapper_t wr = wrapper.last ();

      if (wr.wrap) 
	new_ref = build4 (ARRAY_REF, type, new_ref,
			  wr.domain, NULL_TREE, NULL_TREE);
      else 
	new_ref = build_simple_mem_ref (new_ref);
      wrapper.pop ();
    } 

  if (dump_file)
    {
      fprintf (dump_file, "\nnew_ref is after wrap");
      print_generic_expr (dump_file, new_ref, 0);
    }

  new_acc = build_comp_ref (new_ref, field_id, new_type, acc);
  wrapper.release ();
  
  if (dump_file)
    {
      fprintf (dump_file, "\nnew_acc is ");
      print_generic_expr (dump_file, new_acc, 0);
    }

  if (is_gimple_assign (acc->stmt))
    {
      lhs = gimple_assign_lhs (acc->stmt);
      rhs = gimple_assign_rhs1 (acc->stmt);

      if (lhs == acc->comp_ref)
	gimple_assign_set_lhs (acc->stmt, new_acc);
      else if (rhs == acc->comp_ref)
	gimple_assign_set_rhs1 (acc->stmt, new_acc);
      else
	{
	  pos = find_pos_in_stmt (acc->stmt, acc->comp_ref, &r_pos);
	  gcc_assert (pos);
	  *pos = new_acc;
	}
    }
  else
    {
      pos = find_pos_in_stmt (acc->stmt, acc->comp_ref, &r_pos);
      gcc_assert (pos);
      *pos = new_acc;
    }

  update_stmt (acc->stmt);
}

/* This function replace field access ACC by a new field access
   of structure type NEW_TYPE.  */

static void
replace_field_access_stmt (struct field_access_site *acc, tree new_type)
{

  if (TREE_CODE (acc->ref) == MEM_REF
      ||TREE_CODE (acc->ref) == ARRAY_REF
      ||TREE_CODE (acc->ref) == VAR_DECL)
    replace_field_acc (acc, new_type);
  else
    gcc_unreachable ();
}

/* Auxiliary data for creation of accesses.  */

struct create_acc_data
{
  basic_block bb;
  struct_symbols str;
  int field_index;
};

/* In this function we create new statements that have the same
   form as ORIG_STMT, but of type NEW_TYPE. The statements
   treated by this function are simple assignments,
   like assignments:  p.8_7 = p; or statements with rhs of
   tree codes PLUS_EXPR and MINUS_EXPR.  */

static gimple
create_base_plus_offset (gimple orig_stmt, tree new_type, tree offset)
{
  tree lhs;
  tree new_lhs;
  gimple new_stmt;
  tree new_op0 = NULL_TREE, new_op1 = NULL_TREE;

  lhs = gimple_assign_lhs (orig_stmt);

  gcc_assert (TREE_CODE (lhs) == VAR_DECL
	      || TREE_CODE (lhs) == SSA_NAME);

  new_lhs = find_new_var_of_type (lhs, new_type);
  gcc_assert (new_lhs);
  //finalize_var_creation (new_lhs);

  switch (gimple_assign_rhs_code (orig_stmt))
    {
    case PLUS_EXPR:
    case MINUS_EXPR:
    case POINTER_PLUS_EXPR:
      {
	tree op0 = gimple_assign_rhs1 (orig_stmt);
	tree op1 = gimple_assign_rhs2 (orig_stmt);
	int str0, str1;


	str0 = is_in_struct_symbols_vec (strip_type (get_type_of_var (op0)));
	str1 = is_in_struct_symbols_vec (strip_type (get_type_of_var (op1)));
	gcc_assert ((str0 != -1) || (str1 != -1));

	if (str0 != -1)
	  new_op0 = find_new_var_of_type (op0, new_type);
	if (str1 != -1)
	  new_op1 = find_new_var_of_type (op1, new_type);

	if (!new_op0)
	  new_op0 = offset;
	if (!new_op1)
	  new_op1 = offset;
      }
      break;

    default:
      gcc_unreachable();
    }

  new_stmt = gimple_build_assign_with_ops (gimple_assign_rhs_code (orig_stmt),
                                           new_lhs, new_op0, new_op1);

  update_stmt (new_stmt);

  return new_stmt;
}

/* This function inserts NEW_STMT before STMT.  */

static void
insert_before_stmt (gimple stmt, gimple new_stmt)
{
  gimple_stmt_iterator bsi;

  if (!stmt || !new_stmt)
    return;

  bsi = gsi_for_stmt (stmt);
  gsi_insert_before (&bsi, new_stmt, GSI_SAME_STMT);
}

/* Given a field access F_ACC of the FIELD, this function
   replaces it by the new field access.  */

static void
create_new_field_access (struct field_access_site *f_acc,
			 struct field_entry field)
{
  tree new_type = field.field_mapping;
  gimple new_stmt;
  tree size_res;
  gimple mult_stmt;
  gimple cast_stmt;
  tree cast_res = NULL;

  if (f_acc->num)
    {
      mult_stmt = gen_size (f_acc->num, new_type, &size_res);
      insert_before_stmt (f_acc->ref_def_stmt, mult_stmt);
    }

  if (f_acc->cast_stmt)
    {
      cast_stmt = gen_cast_stmt (size_res, new_type,
				 f_acc->cast_stmt, &cast_res);
      insert_after_stmt (f_acc->cast_stmt, cast_stmt);
    }

  if (f_acc->ref_def_stmt)
    {
      tree offset;
      if (cast_res)
	offset = cast_res;
      else
	offset = size_res;

      new_stmt = create_base_plus_offset (f_acc->ref_def_stmt,
					  new_type, offset);
      insert_after_stmt (f_acc->ref_def_stmt, new_stmt);
    }

  /* In stmt D.2163_19 = D.2162_18->b; we replace variable
   D.2162_18 by an appropriate variable of new_type type.  */
  replace_field_access_stmt (f_acc, new_type);
}

/* This function creates a new field access, defined by SLOT.
   DATA is a pointer to create_acc_data structure.  */

static int
create_new_field_acc (void **slot, void *data)
{
  struct field_access_site *acc = *(struct field_access_site **) slot;
  basic_block bb = ((struct create_acc_data *)data)->bb;
  struct_symbols str = ((struct create_acc_data *)data)->str;
  int i = ((struct create_acc_data *)data)->field_index;

  if (gimple_bb (acc->stmt) == bb)
    create_new_field_access (acc, str->fields[i]);
  return 1;
}

/* This function builds an edge between BB and E->dest and updates
   phi nodes of E->dest. It returns newly created edge.  */

static edge
make_edge_and_fix_phis_of_dest (basic_block bb, edge e)
{
  edge new_e;
  tree arg;
  gimple_stmt_iterator si;

  new_e = make_edge (bb, e->dest, e->flags);

  for (si = gsi_start_phis (new_e->dest); !gsi_end_p (si); gsi_next (&si))
    {
      gimple phi = gsi_stmt (si);
      arg = PHI_ARG_DEF_FROM_EDGE (phi, e);
      add_phi_arg (phi, arg, new_e, gimple_phi_arg_location_from_edge (phi, e));
    }

  return new_e;
}

/* This function creates a new condition statement
   corresponding to the original COND_STMT, adds new basic block
   and redirects condition edges. NEW_VAR is a new condition
   variable located in the condition statement at the position POS.  */

static void
create_new_stmts_for_cond_expr_1 (tree new_var, gimple cond_stmt, unsigned pos)
{
  gimple new_stmt;
  edge true_e = NULL, false_e = NULL;
  basic_block new_bb, bb;
  gimple_stmt_iterator si;  

  bb = gimple_bb (cond_stmt);
  extract_true_false_edges_from_block (gimple_bb (cond_stmt),
				       &true_e, &false_e);

  new_stmt = gimple_build_cond (gimple_cond_code (cond_stmt),
			       pos == 0 ? new_var : gimple_cond_lhs (cond_stmt),
			       pos == 1 ? new_var : gimple_cond_rhs (cond_stmt),
			       NULL_TREE,
			       NULL_TREE);

  update_stmt (new_stmt); 

  /* Create new basic block after bb.  */
  new_bb = create_empty_bb (gimple_bb (cond_stmt));
  new_bb->loop_father = bb->loop_father;

  /* Add new condition stmt to the new_bb.  */
  si = gsi_start_bb (new_bb);
  gsi_insert_after (&si, new_stmt, GSI_NEW_STMT); 

  /* Create false and true edges from new_bb.  */
  make_edge_and_fix_phis_of_dest (new_bb, true_e);
  make_edge_and_fix_phis_of_dest (new_bb, false_e);

  /* Redirect one of original edges to point to new_bb.  */
  if (gimple_cond_code (cond_stmt) == NE_EXPR)
    redirect_edge_succ (true_e, new_bb);
  else
    redirect_edge_succ (false_e, new_bb);
} 

/* This function creates new condition statements corresponding
   to original condition STMT, one for each new type, and
   recursively redirect edges to newly generated basic blocks.  */

static void
create_new_stmts_for_cond_expr (gimple stmt)
{
  tree arg0, arg1, arg;
  int str0, str1;
  bool s0, s1;
  struct_symbols str;
  tree type;
  unsigned pos;
  int i;

  gcc_assert (gimple_cond_code (stmt) == EQ_EXPR
	      || gimple_cond_code (stmt) == NE_EXPR);

  arg0 = gimple_cond_lhs (stmt);
  arg1 = gimple_cond_rhs (stmt);

  str0 = is_in_struct_symbols_vec (strip_type (get_type_of_var (arg0)));
  str1 = is_in_struct_symbols_vec (strip_type (get_type_of_var (arg1)));

  s0 = (str0 != -1) ? true : false;
  s1 = (str1 != -1) ? true : false;

  gcc_assert (s0 || s1); 
  /* For now we allow only comparison with 0 or NULL.  */
  gcc_assert (integer_zerop (arg0) || integer_zerop (arg1));

  str = integer_zerop (arg0) ?
    struct_symbols_vec[str1]:
    struct_symbols_vec[str0];
  arg = integer_zerop (arg0) ? arg1 : arg0;
  pos = integer_zerop (arg0) ? 1 : 0;

  FOR_EACH_VEC_ELT (str->new_types, i, type)
    {
      tree new_arg;

      new_arg = find_new_var_of_type (arg, type);
      create_new_stmts_for_cond_expr_1 (new_arg, stmt, pos);
    }
} 

/* This function looks for VAR in STMT, and replace it with NEW_VAR.
   If needed, it wraps NEW_VAR in pointers and indirect references
   before insertion.  */

static void
insert_new_var_in_stmt (gimple stmt, tree var, tree new_var)
{
  struct ref_pos r_pos;
  tree *pos;

  pos = find_pos_in_stmt (stmt, var, &r_pos);
  gcc_assert (pos);

  while (r_pos.container && (TREE_CODE(r_pos.container) == MEM_REF
			     || TREE_CODE(r_pos.container) == ADDR_EXPR))
    {
      if (TREE_CODE(r_pos.container) == MEM_REF)
	new_var = build_simple_mem_ref (new_var);
      else
	new_var = build_fold_addr_expr (new_var);
      pos = find_pos_in_stmt (stmt, r_pos.container, &r_pos);
    }

  *pos = new_var;
}


/* Create a new general access to replace original access ACC
   for structure type NEW_TYPE.  */

static gimple
create_general_new_stmt (struct access_site *acc, tree new_type)
{
  gimple old_stmt = acc->stmt;
  tree var;
  gimple new_stmt = gimple_copy (old_stmt);
  unsigned i;
  tree vuse = NULL_TREE, vdef = NULL_TREE;
  bool volatilep = false;
  tree tmp;

  if (dump_file)
    {
      print_gimple_stmt (dump_file, old_stmt, 0, TDF_VOPS);
      vuse = gimple_vuse (old_stmt);
      if (vuse) {
	fprintf (dump_file, "\nVuse of general access is ");
	print_generic_expr (dump_file, vuse, 0); }
      vdef = gimple_vdef (old_stmt);
      if (vdef) {
	fprintf (dump_file, "\nVdef of malloc stmt is ");
	print_generic_expr (dump_file, vdef, 0); }
    }

	      
  /* We are really building a new stmt, clear the virtual operands.  */
   if (gimple_has_mem_ops (new_stmt))
    {
      gimple_set_vuse (new_stmt, NULL_TREE);
      gimple_set_vdef (new_stmt, NULL_TREE);
    } 

  FOR_EACH_VEC_ELT (acc->vars, i, var)
    {
      tree new_var = find_new_var_of_type (var, new_type);
      tree lhs, rhs = NULL_TREE;

      gcc_assert (new_var);
      //finalize_var_creation (new_var);

      if (is_gimple_assign (new_stmt))
	{
	  lhs = gimple_assign_lhs (new_stmt);

	  if (TREE_CODE (lhs) == SSA_NAME)
	    if (SSA_NAME_VAR (lhs))
	      lhs = SSA_NAME_VAR (lhs);
	  
	  rhs = gimple_assign_rhs1 (new_stmt);
	  if (gimple_assign_rhs_code (new_stmt) == SSA_NAME)
	    if (SSA_NAME_VAR (gimple_assign_rhs1 (new_stmt)))
	      rhs = SSA_NAME_VAR (gimple_assign_rhs1 (new_stmt));

	  
	  /* It can happen that rhs is a constructor.
	   Then we have to replace it to be of new_type.  */
	  if (gimple_assign_rhs_code (new_stmt) == CONSTRUCTOR)
	     {
	      /* Dealing only with empty constructors right now.  */
	       gcc_assert (vec_safe_is_empty(CONSTRUCTOR_ELTS (rhs)));
	       unsigned HOST_WIDE_INT idx;
	       tree value;
	       	       
	       if (TREE_THIS_VOLATILE (rhs))
		 volatilep = true;

	       FOR_EACH_CONSTRUCTOR_VALUE (CONSTRUCTOR_ELTS (rhs), idx, value)
		 {
		   if (dump_file)
		     {
		       fprintf (dump_file, "\nConstructure element %lu: ", idx);
		       print_generic_expr (dump_file, value, 0); 
		     }
		 }

	       rhs = build_constructor (TREE_TYPE (new_var), 0);
	       if (volatilep)
		 TREE_THIS_VOLATILE (rhs) = 1;
	       gimple_assign_set_rhs1 (new_stmt, rhs);
	     } 
	  
	  if (lhs == var)
	    gimple_assign_set_lhs (new_stmt, new_var);
	  else if (rhs == var)
	    gimple_assign_set_rhs1 (new_stmt, new_var);
	  else
	    insert_new_var_in_stmt (new_stmt, var, new_var);
	}
      else
	insert_new_var_in_stmt (new_stmt, var, new_var);
    }

  if (is_gimple_assign (new_stmt) && 
    gimple_assign_rhs_code (new_stmt) == CONSTRUCTOR)
    {
      if (dump_file)
	    fprintf (dump_file, "\nChecking volatile ");
      tmp = gimple_assign_rhs1 (new_stmt);
      if (TREE_THIS_VOLATILE (tmp))
	if (dump_file)
	  {
	    fprintf (dump_file, "\nVolatile is set succesfully on stmt ");
	    print_gimple_stmt (dump_file, new_stmt, 0, TDF_VOPS);
	  }
    }
  update_stmt (new_stmt);
  return new_stmt;
}

/* For each new type in STR this function creates new general accesses
   corresponding to the original access ACC.  */

static void
create_new_stmts_for_general_acc (struct access_site *acc, struct_symbols str)
{
  tree type;
  gimple stmt = acc->stmt;
  unsigned i;

  FOR_EACH_VEC_ELT (str->new_types, i, type)
    {
      gimple new_stmt;

      new_stmt = create_general_new_stmt (acc, type);
      insert_after_stmt (stmt, new_stmt);
    }
}

/* This function creates a new general access of structure STR
   to replace the access ACC.  */

static void
create_new_general_access (struct access_site *acc, struct_symbols str)
{
  gimple stmt = acc->stmt;
  switch (gimple_code (stmt))
    {
    case GIMPLE_COND:
      create_new_stmts_for_cond_expr (stmt);
      break;

    case GIMPLE_DEBUG:
      /* It is very hard to maintain usable debug info after struct peeling,
	 for now just reset all debug stmts referencing objects that have
	 been peeled.  */
      gimple_debug_bind_reset_value (stmt);
      update_stmt (stmt);
      break;

    default:
      create_new_stmts_for_general_acc (acc, str);
    }
}

/* This function creates a new general access, defined by SLOT.
   DATA is a pointer to create_acc_data structure.  */

static int
create_new_acc (void **slot, void *data)
{
  struct access_site *acc = *(struct access_site **) slot;
  basic_block bb = ((struct create_acc_data *)data)->bb;
  struct_symbols str = ((struct create_acc_data *)data)->str;

  if (gimple_bb (acc->stmt) == bb)
    create_new_general_access (acc, str);
  return 1;
}

/* This function creates new accesses for the structure
   type STR in basic block BB.  */

static void
create_new_accs_for_struct (struct_symbols str, basic_block bb)
{
  int i;
  struct create_acc_data dt;

  dt.str = str;
  dt.bb = bb;
  dt.field_index = -1;

  for (i = 0; i < str->num_fields; i++)
    {
      dt.field_index = i;

      if (str->fields[i].acc_sites)
	htab_traverse (str->fields[i].acc_sites,
		       create_new_field_acc, &dt);
    }
  if (str->accs)
     htab_traverse (str->accs, create_new_acc, &dt);
}

/* This function creates new general and field 
   accesses in BB.  */

static void
create_new_accesses_in_bb (basic_block bb)
{
  struct_symbols str;
  unsigned i;

  if (struct_symbols_vec.exists ())
    {   
      FOR_EACH_VEC_ELT (struct_symbols_vec, i, str)
	create_new_accs_for_struct (str, bb);
    }
}

/* This function creates new general and field accesses 
   that appear in cfun.  */

static void
create_new_accesses (void)
{
  basic_block bb;

  FOR_EACH_BB_FN (bb, cfun)
    create_new_accesses_in_bb (bb);
}

static void
print_loop_fathers (void)
{
  basic_block bb;

  FOR_EACH_BB_FN (bb, cfun)
    if (dump_file)
      {
	fprintf (dump_file, "\nbb %i ", bb->index);
	if (bb->loop_father)
	  fprintf (dump_file, " has loop father number %d", 
	       bb->loop_father->num);
	else
	  fprintf (dump_file, " has loop_father NULL ptr"); 
      }

}

/* Do struct-reorg transformation for individual function
   represented by NODE. All structure types relevant
   for this function are transformed.  */

static void
do_reorg_for_func (struct cgraph_node *node)
{
  create_new_local_vars ();
  create_new_tmp_vars ();
  collect_alloc_sites (node);
  collect_func_calls (node);
  create_new_alloc_sites (node);
  collect_data_accesses ();
  if (dump_file)
    {
      fprintf (dump_file, "\nPrinting loop_father before transform.");
      print_loop_fathers ();
    }
  create_new_accesses ();
  update_ssa (TODO_update_ssa);
  if (dump_file)
    {
      fprintf (dump_file, "\nPrinting loop_father after transform.");
      print_loop_fathers ();
    }
  cleanup_tree_cfg ();
  cgraph_rebuild_references (); 

  /* Free auxiliary data.  */
  free_alloc_sites ();
  free_new_vars_htab (new_local_vars);
  free_new_vars_htab (new_tmp_vars);
  free_data_accesses ();
  free_calls_with_structs ();
}

/* Implementation of function_transform function for struct-reorg.  */

unsigned int
struct_reorg_func_transform (struct cgraph_node *node)
{
  if (dump_file)
    {
      fprintf (dump_file, "\nStruct_reorg is transforming %s function.\n", node->name ());
    }

  if (dump_file)
    dump_symtab (dump_file); 

  gcc_checking_assert (cfun);
  gcc_checking_assert (current_function_decl);

  print_struct_symbol_vec ();

  do_reorg_for_func (node);
  return 0;

  // DO not forget to free free_new_vars_htab (new_global_vars); !!!
}

static void
check_prog_eligibility (void)
{
  has_inline_asm ();

  /* TBD: Add other reasons here to skip struct-reorg optimization.  */

  return;
}

/* Analyze program. */

static void
struct_reorg_generate_summary (void)
{
  check_prog_eligibility ();
  if (struct_reorg_give_up != (1 << STRUCT_REORG_DO_NOT_GIVE_UP))
    {
      print_give_up_reason ();
      return;
    }
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
		      struct_reorg_write_opt_summary, 
		      /* write_optimization_summary */
		      struct_reorg_read_opt_summary, 
		      /* read_optimization_summary */
		      NULL, /* stmt_fixup */
		      0, /* function_transform_todo_flags_start */
		      struct_reorg_func_transform, /* function_transform */
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
