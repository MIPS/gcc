/* Struct-reorg optimizations.
   Copyright (C) 2016-2017 Free Software Foundation, Inc.
   Contributed by Andrew Pinski  <apinski@cavium.com>

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

/* This pass implements the structure reorganization organization (struct-reorg).
   Right now it handles just splitting off the hottest fields for a struct of 2 fields:
   struct s {
     type1 field1; // Hot field
     type2 field2;
   };
   s *v;
   into:
   struct s_hot {
     type1 field1;
   };
   struct c_cold {
     type2 field2;
   };
   s_hot *v_hot;
   s_cold *v_cold;
  
   TODO: This pass can be extended to more fields, and other alogrothims like reordering.

   This pass operate in four stages:
    1. All of the field accesses, declarations (struct types and pointers to that type)
       and struct types are scanned and recorded.  This includes global declarations.
       Also record all allocation and freeing sites; this is needed for the rewriting
       phase.

       FIXME: If there is a top-level inline-asm, the pass immediately returns.

    2. Prune out the types which are considered escaping.
       Examples of types which are considered escaping:
       1. A declaration has been marked as having the attribute used or has user defined
	  alignment (type too).
       2. Accesses are via a BIT_FIELD_REF. FIXME: Handle VECTOR_TYPE for this case.
       3. The "allocation" site is not a known builtin function.
       4. Casting to/from an integer.

    3. Analyze the types for which optimization to do.
       a. Split the fields into two different structs.
	  (FIXME: two field case handled only)
	  Look at all structs which contain two fields, if one of the fields is hotter
	  then split it and put it on the rewritting for accesses.
	  Allocations and freeing are marked to split into two functions; all uses of
	  that type will now be considered as two.
       b. Reorder fields hottest to the coldest.  TODO: Implement.

    4. Rewrite each access and allocation and free which is marked as rewriting.

 */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-pass.h"
#include "cgraph.h"
#include "diagnostic-core.h"
#include "function.h"
#include "basic-block.h"
#include "gimple.h"
#include "vec.h"
#include "tree-pretty-print.h"
#include "gimple-pretty-print.h"
#include "gimple-iterator.h"
#include "cfg.h"
#include "ssa.h"
#include "tree-dfa.h"
#include "fold-const.h"
#include "tree-inline.h"
#include "stor-layout.h"
#include "tree-into-ssa.h"
#include "tree-cfg.h"
#include "symbol-summary.h"
#include "alloc-pool.h"
#include "ipa-prop.h"
#include "ipa-struct-reorg.h"
#include "tree-eh.h"
#include "bitmap.h"
#include "tree-ssa-live.h"  /* For remove_unused_locals.  */

#define VOID_POINTER_P(type) (POINTER_TYPE_P (type) && VOID_TYPE_P (TREE_TYPE (type)))

namespace {

using namespace struct_reorg;

/* Return true iff TYPE is stdarg va_list type.  */

static inline bool
is_va_list_type (tree type)
{
  return TYPE_MAIN_VARIANT (type) == TYPE_MAIN_VARIANT (va_list_type_node);
}


/* Return the inner most type for arrays and pointers of TYPE.  */

tree
inner_type (tree type)
{
  while (POINTER_TYPE_P (type)
	 || TREE_CODE (type) == ARRAY_TYPE)
    type = TREE_TYPE (type);
  return type;
}

/*  Return true if TYPE is a type which struct reorg should handled.  */

bool
handled_type (tree type)
{
  type = inner_type (type);
  if (TREE_CODE (type) == RECORD_TYPE)
    return !is_va_list_type (type);
  return false;
}

} // anon namespace

namespace struct_reorg {

/* Constructor of srfunction. */

srfunction::srfunction (cgraph_node *n)
  : node (n),
    old (NULL),
    newnode (NULL),
    newf (NULL)
{
}

/* Add an ARG to the list of arguments for the function. */

void
srfunction::add_arg(srdecl *arg)
{
  args.safe_push(arg);
}

/* Dump the SRFUNCTION to the file FILE.  */

void
srfunction::dump (FILE *file)
{
  if (node)
    {
      fprintf (file, "function : ");
      print_generic_expr (file, node->decl, 0);
      fprintf (file, " with arguments: ");
      for (unsigned i = 0; i < args.length (); i++)
	{
	  if (i == 0)
	    fprintf (file, "\n  ");
	  else
	    fprintf (file, "\n,  ");
	  args[i]->dump (file);
	}

      fprintf (file, "\nuses globals: ");
      for(unsigned i = 0; i < globals.length (); i++)
	{
	  fprintf (file, "\n  ");
          globals[i]->dump (file);
        }

      fprintf (file, "\ndecls: ");
    }
  else
    fprintf (file, "globals : ");

  for(unsigned i = 0; i < decls.length (); i++)
    {
      fprintf (file, "\n  ");
      decls[i]->dump (file);
    }
}

/* Simple dump the SRFUNCTION to the file FILE; used so it is not recusive.  */

void
srfunction::simple_dump (FILE *file)
{
  print_generic_expr (file, node->decl, 0);
}


/* Constructor of FIELD. */

srfield::srfield (tree field, srtype *base)
  : offset(int_byte_position (field)),
    fieldtype (TREE_TYPE (field)),
    fielddecl (field),
    base(base),
    type(NULL),
    clusternum(0)
{
  for(int i = 0;i < max_split; i++)
    newfield[i] = NULL_TREE;
}

/* Constructor of TYPE. */

srtype::srtype (tree type)
  : type (type),
    chain_type (false),
    escapes (does_not_escape),
    visited (false)
{
  for (int i = 0; i < max_split; i++)
    newtype[i] = NULL_TREE;

  for (tree field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
    {
      if (TREE_CODE (field) == FIELD_DECL)
	{
	  if (DECL_BIT_FIELD (field))
	    {
	      escapes = escape_bitfields;
	      continue;
	    }
	  else if (!DECL_SIZE (field)
	           || TREE_CODE (DECL_SIZE (field)) != INTEGER_CST)
	    {
	      escapes = escape_variable_sized_array;
	      break;
	    }
	  srfield *t = new srfield (field, this);
	  fields.safe_push(t);
	}
    }
}

/* Mark the type as escaping type E at statement STMT. */

void
srtype::mark_escape (escape_type e, gimple *stmt)
{
  /* Once the type has escaped, it should never
     change back to non escaping. */
  gcc_assert (e != does_not_escape);
  if (has_escaped ())
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\nO type: ");
	  simple_dump (dump_file);
	  fprintf (dump_file, " has already escaped.");
          fprintf (dump_file, " old = \"%s\" ", escape_type_string[escapes - 1]);
          fprintf (dump_file, " new = \"%s\"\n", escape_type_string[e - 1]);
	  if (stmt)
	    print_gimple_stmt (dump_file, stmt, 0, 0);
	  fprintf (dump_file, "\n");
	}
      return;
    }
  escapes = e;
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nN type: ");
      simple_dump (dump_file);
      fprintf (dump_file, " new = \"%s\"\n", escape_reason ());
      if (stmt)
        print_gimple_stmt (dump_file, stmt, 0, 0);
      fprintf (dump_file, "\n");
    }
}

/* Add FIELD to the list of fields that use this type.  */

void
srtype::add_field_site (srfield *field)
{
  field_sites.safe_push(field);
}


/* Constructor of DECL. */

srdecl::srdecl (srtype *tp, tree decl, int argnum)
  : type (tp),
    decl (decl),
    func (NULL_TREE),
    argumentnum (argnum),
    visited (false)
{
  if (TREE_CODE (decl) == SSA_NAME)
    func = current_function_decl;
  else if (!is_global_var (decl))
    func = DECL_CONTEXT (decl);
  for(int i = 0;i < max_split; i++)
    newdecl[i] = NULL_TREE;
}

/* Find DECL in the function. */

srdecl *
srfunction::find_decl (tree decl)
{
  for (unsigned i = 0; i < decls.length (); i++)
    if (decls[i]->decl == decl)
      return decls[i];
  return NULL;
}

/* Record DECL of the TYPE with argument num ARG. */

srdecl *
srfunction::record_decl (srtype *type, tree decl, int arg)
{
  // Search for the decl to see if it is already there.
  srdecl *decl1 = find_decl (decl);

  if (decl1)
    return decl1;

  gcc_assert (type);

  decl1 = new srdecl (type, decl, arg);
  decls.safe_push(decl1);
  return decl1;
}

/* Find the field at OFF offset.  */

srfield *
srtype::find_field (unsigned HOST_WIDE_INT off)
{
  unsigned int i;
  srfield *field;

  /* FIXME: handle array/struct field inside the current struct. */
  /* NOTE This does not need to be fixed to handle libquatumn */
  FOR_EACH_VEC_ELT (fields, i, field)
    {
      if (off == field->offset)
	return field;
    }
  return NULL;
}

/* Add the function FN to the list of functions if it
   is there not already. */

void
srtype::add_function (srfunction *fn)
{
  unsigned decluid;
  unsigned i;
  decluid = DECL_UID (fn->node->decl);

  srfunction *fn1;
  // Search for the decl to see if it is already there.
  FOR_EACH_VEC_ELT (functions, i, fn1)
    {
      if (DECL_UID (fn1->node->decl) == decluid)
        return;
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Recording new function: %u.\n", decluid);

  functions.safe_push(fn);
}

/* Dump out the type structure to FILE. */

void
srtype::dump (FILE *f)
{
  unsigned int i;
  srfield *field;
  srfunction *fn;
  sraccess *access;

  if (chain_type)
    fprintf (f, "chain decl ");

  fprintf (f, "type : ");
  print_generic_expr (f, type, 0);
  fprintf (f, "(%d) { ", TYPE_UID (type));
  if (escapes != does_not_escape)
    fprintf (f, " escapes = \"%s\"\n", escape_reason ());
  fprintf (f, " fields = { ");
  FOR_EACH_VEC_ELT (fields, i, field)
    {
      if (i == 0)
	fprintf (f, "\n  ");
      else
	fprintf (f, "\n,  ");
      field->dump (f);
    }
  fprintf (f, " }\n ");
  fprintf (f, "\n accesses = {");
  FOR_EACH_VEC_ELT (accesses, i, access)
    {
      fprintf (f, "\n");
      access->dump (f);
    }
  fprintf (f, " }\n ");
  fprintf (f, "\n functions = {");
  FOR_EACH_VEC_ELT (functions, i, fn)
    {
      fprintf (f, "  \n");
      fn->simple_dump (f);
    }
  fprintf (f, "\n }\n");
  fprintf (f, "\n field_sites = {");
  FOR_EACH_VEC_ELT (field_sites, i, field)
    {
      fprintf (f, "  \n");
      field->simple_dump (f);
    }
  fprintf (f, "\n }\n");
  fprintf (f, "}\n");
}

/* A simplified dump out the type structure to FILE. */

void
srtype::simple_dump (FILE *f)
{
  print_generic_expr (f, type, 0);
}

/* Analyze the type and decide what to be done with it. */

void
srtype::analyze (void)
{
  /* Chain decl types can't be split
     so don't try. */
  if (chain_type)
    return;

  /* If there is only one field then there is nothing
     to be done. */
  if (fields.length () == 1)
    return;

  /*  For now we unconditionally split only structures with 2 fields
      into 2 different structures.  In future we intend to add profile
      info and/or static heuristics to differentiate splitting process.  */
  if (fields.length () == 2)
    fields[1]->clusternum = 1;

  /* REMOVEME: FIXME: this is here for testing more testcases. */
  if (fields.length () >= 3)
    {
      fields[1]->clusternum = 1;
    }
}

/* Create the new fields for this field. */

void
srfield::create_new_fields (tree newtype[max_split],
			    tree newfields[max_split],
			    tree newlast[max_split])
{
  tree nt[max_split];

  for (unsigned i = 0; i < max_split; i++)
    nt[i] = NULL;

  if (type == NULL)
    nt[0] = fieldtype;
  else
    memcpy (nt, type->newtype, sizeof(type->newtype));

  for (unsigned i = 0; i < max_split && nt[i] != NULL; i++)
    {
      tree field = make_node (FIELD_DECL);
      if (nt[1] != NULL && DECL_NAME (fielddecl))
	{
	  const char *tname = IDENTIFIER_POINTER (DECL_NAME (fielddecl));
	  char id[10];
	  char *name;

	  sprintf(id, "%d", i);
	  name = concat (tname, ".reorg.", id, NULL);
	  DECL_NAME (field) = get_identifier (name);
	  free (name);
	}
      else
	DECL_NAME (field) = DECL_NAME (fielddecl);

      TREE_TYPE (field) = reconstruct_complex_type (TREE_TYPE (fielddecl), nt[i]);
      DECL_SOURCE_LOCATION (field) = DECL_SOURCE_LOCATION (fielddecl);
      SET_DECL_ALIGN (field, DECL_ALIGN (fielddecl));
      DECL_USER_ALIGN (field) = DECL_USER_ALIGN (fielddecl);
      TREE_ADDRESSABLE (field) = TREE_ADDRESSABLE (fielddecl);
      DECL_NONADDRESSABLE_P (field) = !TREE_ADDRESSABLE (fielddecl);
      TREE_THIS_VOLATILE (field) = TREE_THIS_VOLATILE (fielddecl);
      DECL_CONTEXT (field) = newtype[clusternum];

      if (newfields[clusternum] == NULL)
	newfields[clusternum] = newlast[clusternum] = field;
      else
	{
	  DECL_CHAIN (newlast[clusternum]) = field;
	  newlast[clusternum] = field;
        }
      newfield[i] = field;
    }

}

/* Create the new TYPE corresponding to THIS type. */

bool
srtype::create_new_type (void)
{
  /* If the type has been visited,
     then return if a new type was
     created or not. */
  if (visited)
    return has_new_type ();

  visited = true;

  if (escapes != does_not_escape)
    {
      newtype[0] = type;
      return false;
    }

  bool createnewtype = false;
  unsigned maxclusters = 0;

  /* Create a new type for each field. */
  for (unsigned i = 0; i < fields.length (); i++)
    {
      srfield *field = fields[i];
      if (field->type)
	createnewtype |= field->type->create_new_type ();
      if (field->clusternum > maxclusters)
	maxclusters = field->clusternum;
    }

  /* If the fields' types did have a change or
     we are not splitting the struct into two clusters,
     then just return false and don't change the type. */
  if (!createnewtype && maxclusters == 0)
    {
      newtype[0] = type;
      return false;
    }

  /* Should have at most max_split clusters.  */
  gcc_assert (maxclusters < max_split);

  tree newfields[max_split];
  tree newlast[max_split];

  maxclusters++;

  const char *tname = NULL;

  if (TYPE_NAME (type) != NULL)
    {
      if (TREE_CODE (TYPE_NAME (type)) == IDENTIFIER_NODE)
        tname = IDENTIFIER_POINTER (TYPE_NAME (type));
      else if (DECL_NAME (TYPE_NAME (type)) != NULL)
        tname = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (type)));
    }

  for (unsigned i = 0; i < maxclusters; i++)
    {
      newfields[i] = NULL_TREE;
      newlast[i] = NULL_TREE;
      newtype[i] = make_node (RECORD_TYPE);

      char *name = NULL;
      char id[10];
      sprintf(id, "%d", i);
      if (tname) 
	{
          name = concat (tname, ".reorg.", id, NULL);
          TYPE_NAME (newtype[i]) = get_identifier (name);
          free (name);
        }
    }

  for (unsigned i = 0; i < fields.length (); i++)
    {
      srfield *f = fields[i];
      f->create_new_fields (newtype, newfields, newlast);
    }


  /* No reason to warn about these structs since the warning would
     have happened already.  */
  int save_warn_padded = warn_padded;
  warn_padded = 0;

  for (unsigned i = 0; i < maxclusters; i++)
    {
      TYPE_FIELDS (newtype[i]) = newfields[i];
      layout_type (newtype[i]);
    }

  warn_padded = save_warn_padded;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Created %d types:\n", maxclusters);
      for (unsigned i = 0; i < maxclusters; i++)
	{
	  print_generic_expr (dump_file, newtype[i], 0);
	  fprintf (dump_file, "\n");
	}
    }

  return true;
}

/* Helper function to copy some attributes from ORIG_DECL to the NEW_DECL. */

static inline void
copy_var_attributes (tree new_decl, tree orig_decl)
{
  DECL_ARTIFICIAL (new_decl) = 1;
  DECL_EXTERNAL (new_decl) = DECL_EXTERNAL (orig_decl);
  TREE_STATIC (new_decl) = TREE_STATIC (orig_decl);
  TREE_PUBLIC (new_decl) = TREE_PUBLIC (orig_decl);
  TREE_USED (new_decl) = TREE_USED (orig_decl);
  DECL_CONTEXT (new_decl) = DECL_CONTEXT (orig_decl);
  TREE_THIS_VOLATILE (new_decl) = TREE_THIS_VOLATILE (orig_decl);
  TREE_ADDRESSABLE (new_decl) = TREE_ADDRESSABLE (orig_decl);
  TREE_READONLY (new_decl) = TREE_READONLY (orig_decl);
  if (is_global_var (orig_decl))
    set_decl_tls_model (new_decl, DECL_TLS_MODEL (orig_decl));
}

/* Create all of the new decls (SSA_NAMES included) for THIS function. */

void
srfunction::create_new_decls (void)
{
  /* If this function has been cloned, we don't need to
     create the new decls. */
  if (newnode)
    return;

  if (node)
    set_cfun (DECL_STRUCT_FUNCTION (node->decl));

  for (unsigned i = 0; i < decls.length (); i++)
    {
      srdecl *decl = decls[i];
      srtype *type = decl->type;
      /* If the type of the decl does not change,
	 then don't create a new decl. */
      if (!type->has_new_type ())
	{
	  decl->newdecl[0] = decl->decl;
	  continue;
	}

      /* Handle SSA_NAMEs. */
      if (TREE_CODE (decl->decl) == SSA_NAME)
	{
	  tree newtype1[max_split];
	  tree inner = SSA_NAME_VAR (decl->decl);
	  tree newinner[max_split];
	  memset (newinner, 0, sizeof(newinner));
	  for (unsigned j = 0; j < max_split && type->newtype[j]; j++)
	    newtype1[j] = reconstruct_complex_type (TREE_TYPE (decls[i]->decl), type->newtype[j]);
	  if (inner)
	    {
	      srdecl *in = find_decl (inner);
	      gcc_assert (in);
	      memcpy (newinner, in->newdecl, sizeof(newinner));
	    }
	  tree od = decls[i]->decl;
	  /* Create the new ssa names and copy some attributes from the old one.  */
	  for (unsigned j = 0; j < max_split && type->newtype[j]; j++)
	    {
	      tree nd = make_ssa_name (newinner[j] ? newinner[j] : newtype1[j]);
	      decl->newdecl[j] = nd;
	      /* If the old decl was a default defition, handle it specially. */
	      if (SSA_NAME_IS_DEFAULT_DEF (od))
		{
	          SSA_NAME_IS_DEFAULT_DEF (nd) = true;
		  SSA_NAME_DEF_STMT (nd) = gimple_build_nop ();

		  /* Set the default definition for the ssaname if needed. */
		  if (inner)
		    {
		      gcc_assert (newinner[j]);
		      set_ssa_default_def (cfun, newinner[j], nd);
		    }
		}
	      SSA_NAME_OCCURS_IN_ABNORMAL_PHI (nd)
		= SSA_NAME_OCCURS_IN_ABNORMAL_PHI (od);
	      statistics_counter_event (cfun, "Create new ssa_name", 1);
	    }
	}
      else if (TREE_CODE (decls[i]->decl) == VAR_DECL)
	{
	 tree orig_var = decl->decl;
	 const char *tname = NULL;
	 if (DECL_NAME (orig_var))
	   tname = IDENTIFIER_POINTER (DECL_NAME (orig_var));
	 for (unsigned j = 0; j < max_split && type->newtype[j]; j++)
	   {
	      tree new_name = NULL;
	      char *name = NULL;
	      char id[10];
	      sprintf(id, "%d", j);
	      if (tname)
	        {
	          name = concat (tname, ".reorg.", id, NULL);
		  new_name = get_identifier (name);
		  free (name);
		}
	      tree newtype1 = reconstruct_complex_type (TREE_TYPE (orig_var), type->newtype[j]);
	      decl->newdecl[j] = build_decl (DECL_SOURCE_LOCATION (orig_var),
					     VAR_DECL, new_name, newtype1);
	      copy_var_attributes (decl->newdecl[j], orig_var);
	      if (!is_global_var (orig_var))
		add_local_decl (cfun, decl->newdecl[j]);
	      else
		varpool_node::add (decl->newdecl[j]);
	      statistics_counter_event (cfun, "Create new var decl", 1);
	    }
        }
      /* Paramater decls are already handled in create_new_functions. */
      else if (TREE_CODE (decls[i]->decl) == PARM_DECL)
	;
      else
	internal_error ("Unhandled decl type stored");

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Created New decls for decl:\n");
	  fprintf (dump_file, "\n");
	  decls[i]->dump (dump_file);
	  fprintf (dump_file, "\n");
	  for (unsigned j = 0; j < max_split && decls[i]->newdecl[j]; j++)
	    {
	      print_generic_expr (dump_file, decls[i]->newdecl[j], 0);
	      fprintf (dump_file, "\n");
	    }
	  fprintf (dump_file, "\n");
	}
    }

  set_cfun (NULL);

}

/* Dump out the field structure to FILE. */

void
srfield::dump (FILE *f)
{
  fprintf (f, "field (%d) { ", DECL_UID (fielddecl));
  fprintf (f, "base = ");
  base->simple_dump (f);
  fprintf (f, ", offset = " HOST_WIDE_INT_PRINT_DEC, offset);
  fprintf (f, ", type = ");
  print_generic_expr (f, fieldtype, 0);
  if (type)
    {
      fprintf (f, "( srtype = ");
      type->simple_dump (f);
      fprintf (f, ")");
    }
  fprintf (f, "\n}\n");
}


/* A simplified dump out the field structure to FILE. */

void
srfield::simple_dump (FILE *f)
{
  fprintf (f, "field (%d)", DECL_UID (fielddecl));
}

/* Dump out the access structure to FILE. */

void
sraccess::dump (FILE *f)
{
  fprintf (f, "access { ");
  fprintf (f, "type = '(");
  type->simple_dump (f);
  fprintf (f, ")'");
  if (field)
    {
      fprintf (f, ", field = '(");
      field->simple_dump (f);
      fprintf (f, ")'");
    }
  else
    fprintf (f, ", whole type");
  fprintf (f, " in function: %s/%d", node->name (), node->order);
  fprintf (f, ", stmt:\n");
  print_gimple_stmt (f, stmt, 0, 0);
  fprintf (f, "\n }\n");
  
}

/* Dump out the decl structure to FILE. */

void
srdecl::dump (FILE *file)
{
  if (!func)
    fprintf (file, "global ");
  if (argumentnum != -1)
    fprintf (file, "argument(%d) ", argumentnum);
  fprintf (file, "decl: ");
  print_generic_expr (file, decl, 0);
  fprintf (file, " type: ");
  type->simple_dump (file);
}

} // namespace struct_reorg

namespace {

struct ipa_struct_reorg
{
  // Constructors
  ipa_struct_reorg(void)
    : current_function (NULL),
      done_recording(false)
  {
  }

  // public methods
  unsigned execute(void);
  void mark_type_as_escape (tree type, escape_type, gimple *stmt = NULL);
private:
  // fields
  auto_vec_del<srtype> types;
  auto_vec_del<srfunction> functions;
  srglobal globals;
  srfunction *current_function;

  bool done_recording;

  // private methods
  void dump_types (FILE *f);
  void dump_types_escaped (FILE *f);
  void dump_functions (FILE *f);
  void record_accesses (void);
  void detect_cycles (void);
  bool walk_field_for_cycles (srtype*);
  void prune_escaped_types (void);
  void propagate_escape (void);
  void analyze_types (void);
  void clear_visited (void);
  bool create_new_types (void);
  void create_new_decls (void);
  srdecl *find_decl (tree);
  void create_new_functions (void);
  void create_new_args (cgraph_node *new_node);
  unsigned rewrite_functions (void);
  srdecl *record_var (tree decl, escape_type escapes = does_not_escape, int arg = -1);
  srfunction *record_function (cgraph_node *node);
  srfunction *find_function (cgraph_node *node);
  srtype *record_type (tree type);
  void process_union (tree type);
  srtype *find_type (tree type);
  void maybe_record_stmt (cgraph_node *, gimple *);
  void maybe_record_assign (cgraph_node *, gassign *);
  void maybe_record_call (cgraph_node *, gcall *);
  void maybe_record_allocation_site (cgraph_node *, gimple *);
  void record_stmt_expr (tree expr, cgraph_node *node, gimple *stmt);
  void mark_expr_escape(tree, escape_type, gimple *stmt);
  tree allocate_size (srtype *t, gimple *stmt);

  void mark_decls_in_as_not_needed (tree fn);

  bool rewrite_stmt (gimple*, gimple_stmt_iterator *);
  bool rewrite_assign (gassign *, gimple_stmt_iterator *);
  bool rewrite_call (gcall *, gimple_stmt_iterator *);
  bool rewrite_cond (gcond *, gimple_stmt_iterator *);
  bool rewrite_debug (gimple *, gimple_stmt_iterator *);
  bool rewrite_phi (gphi *);
  bool rewrite_expr (tree expr, tree newexpr[max_split], bool ignore_missing_decl = false);
  bool rewrite_lhs_rhs (tree lhs, tree rhs, tree newlhs[max_split], tree newrhs[max_split]);
  bool get_type_field (tree expr, tree &base, bool &indirect, srtype *&type, srfield *&field, bool &realpart, bool &imagpart, bool &address, bool should_create = false, bool can_escape = false);
  bool wholeaccess (tree expr, tree base, tree accesstype, srtype *t);

  void check_definition (srdecl *decl, vec<srdecl*>&);
  void check_uses (srdecl *decl, vec<srdecl*>&);
  void check_use (srdecl *decl, gimple *stmt, vec<srdecl*>&);
  void check_type_and_push (tree newdecl, srtype *type, vec<srdecl*> &worklist, gimple *stmt);
  void check_other_side (srdecl *decl, tree other, gimple *stmt, vec<srdecl*> &worklist);

  void find_vars (gimple *stmt);
  void find_var (tree expr, gimple *stmt);
  void mark_types_asm (gasm *astmt);

  bool has_rewritten_type (srfunction*);
  void maybe_mark_or_record_other_side (tree side, tree other, gimple *stmt);
};

/* Dump all of the recorded types to file F. */

void
ipa_struct_reorg::dump_types (FILE *f)
{
  unsigned i;
  srtype *type;
  FOR_EACH_VEC_ELT (types, i, type)
    {
      type->dump(f);
    }
  fprintf (f, "\n");
}

/* Dump all of the recorded types to file F. */

void
ipa_struct_reorg::dump_types_escaped (FILE *f)
{
  unsigned i;
  srtype *type;
  FOR_EACH_VEC_ELT (types, i, type)
    {
      if (type->has_escaped ())
	{
	  type->simple_dump (f);
	  fprintf (f, " has escaped: \"%s\"\n", type->escape_reason());
	}
    }
  fprintf (f, "\n");
}


/* Dump all of the record functions to file F. */

void
ipa_struct_reorg::dump_functions (FILE *f)
{
  unsigned i;
  srfunction *fn;

  fprintf (f, "\n\n");
  globals.dump (f);
  fprintf (f, "\n\n");
  FOR_EACH_VEC_ELT (functions, i, fn)
    {
      fn->dump(f);
      fprintf (f, "\n");
    }
  fprintf (f, "\n\n");
}

/* Find the recorded srtype corresponding to TYPE.  */

srtype *
ipa_struct_reorg::find_type (tree type)
{
  unsigned i;
  /* Get the main variant as we are going
     to find that type only. */
  type = TYPE_MAIN_VARIANT (type);

  srtype *type1;
  // Search for the type to see if it is already there.
  FOR_EACH_VEC_ELT (types, i, type1)
    {
      if (types_compatible_p (type1->type, type))
	return type1;
    }
  return NULL;
}

/* Is TYPE a volatile type or one which points
   to a volatile type. */

bool isvolatile_type (tree type)
{
  if (TYPE_VOLATILE (type))
    return true;
  while (POINTER_TYPE_P (type) || TREE_CODE (type) == ARRAY_TYPE)
    {
      type = TREE_TYPE (type);
      if (TYPE_VOLATILE (type))
        return true;
    }
  return false;
}

/* Is TYPE an array type or points to an array type. */

bool isarraytype (tree type)
{
  if (TREE_CODE (type) == ARRAY_TYPE)
    return true;
  while (POINTER_TYPE_P (type))
    {
      type = TREE_TYPE (type);
      if (TREE_CODE (type) == ARRAY_TYPE)
        return true;
    }
  return false;
}

/*  Is TYPE a pointer to another pointer. */

bool isptrptr (tree type)
{
  bool firstptr = false;
  while (POINTER_TYPE_P (type) || TREE_CODE (type) == ARRAY_TYPE)
    {
      if (POINTER_TYPE_P (type))
	{
	  if (firstptr)
	    return true;
	  firstptr = true;
	}
      type = TREE_TYPE (type);
    }
  return false;
}

/* Return the escape type which corresponds to if
   this is an volatile type, an array type or a pointer
   to a pointer type.  */

escape_type escape_type_volatile_array_or_ptrptr (tree type)
{
  if (isvolatile_type (type))
    return escape_volatile;
  if (isarraytype (type))
    return escape_array;
  if (isptrptr (type))
    return escape_ptr_ptr;
  return does_not_escape;
}

/* Record TYPE if not already recorded. */

srtype *
ipa_struct_reorg::record_type (tree type)
{
  unsigned typeuid;

  /* Get the main variant as we are going
     to record that type only. */
  type = TYPE_MAIN_VARIANT (type);
  typeuid = TYPE_UID (type);

  srtype *type1;

  type1 = find_type (type);
  if (type1)
    return type1;

  /* If already done recording just return NULL. */
  if (done_recording)
    return NULL;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Recording new type: %u.\n", typeuid);

  type1 = new srtype (type);
  types.safe_push(type1);

  /* If the type has an user alignment set,
     that means the user most likely already setup the type. */
  if (TYPE_USER_ALIGN (type))
    type1->mark_escape (escape_user_alignment, NULL);

  for (tree field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
    {
      if (TREE_CODE (field) == FIELD_DECL)
        {
	  tree t = TREE_TYPE (field);
	  process_union (t);
	  if (isvolatile_type (t))
	    type1->mark_escape (escape_volatile, NULL);
	  escape_type e = escape_type_volatile_array_or_ptrptr (t);
	  if (e != does_not_escape)
	    type1->mark_escape (e, NULL);
	  if (handled_type (t))
	    {
	      srtype *t1 = record_type (inner_type (t));
	      srfield *f = type1->find_field (int_byte_position (field));
	      /* We might have an variable sized type which we don't set the handle. */
	      if (f)
		{
		  f->type = t1;
		  t1->add_field_site (f);
		}
	      if (t1 == type1)
		type1->mark_escape (escape_rescusive_type, NULL);
	    }
        }
    }

  return type1;
}

/* Mark TYPE as escaping with ESCAPES as the reason.  */

void
ipa_struct_reorg::mark_type_as_escape (tree type, escape_type escapes, gimple *stmt)
{
  if (handled_type (type))
    {
      srtype *stype = record_type (inner_type (type));

      if (!stype)
	return;

      stype->mark_escape (escapes, stmt);
    }
}

/* Maybe process the union of type TYPE, such that marking all of the fields'
   types as being escaping. */

void
ipa_struct_reorg::process_union (tree type)
{
  static hash_set<tree> unions_recorded;

  type = inner_type (type);
  if (TREE_CODE (type) != UNION_TYPE
      && TREE_CODE (type) != QUAL_UNION_TYPE)
    return;

  type = TYPE_MAIN_VARIANT (type);

  /* We already processed this type. */
  if (unions_recorded.add (type))
    return;

  for (tree field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
    {
      if (TREE_CODE (field) == FIELD_DECL)
	{
	  mark_type_as_escape (TREE_TYPE (field), escape_union);
	  process_union (TREE_TYPE (field));
	}
    }
}

/*  Used by record_var function as a callback to walk_tree.
    Mark the type as escaping if it has expressions which
    cannot be converted for global initializations. */

static tree
record_init_types (tree *tp, int *walk_subtrees, void *data)
{
  ipa_struct_reorg *c = (ipa_struct_reorg *)data;
  switch (TREE_CODE (*tp))
    {
      CASE_CONVERT:
      case COMPONENT_REF:
      case VIEW_CONVERT_EXPR:
      case ARRAY_REF:
	{
	  tree typeouter = TREE_TYPE (*tp);
	  tree typeinner = TREE_TYPE (TREE_OPERAND (*tp, 0));
	  c->mark_type_as_escape (typeouter, escape_via_global_init);
	  c->mark_type_as_escape (typeinner, escape_via_global_init);
	  break;
	}
      case INTEGER_CST:
	if (!integer_zerop (*tp))
	  c->mark_type_as_escape (TREE_TYPE (*tp), escape_via_global_init);
	break;
     case VAR_DECL:
     case PARM_DECL:
     case FIELD_DECL:
	c->mark_type_as_escape (TREE_TYPE (*tp), escape_via_global_init);
	*walk_subtrees = false;
	break;
     default:
	*walk_subtrees = true;
	break;
    }
  return NULL_TREE;
}

/* Record var DECL; optionally specify the escape reason and the argument
   number in a function. */

srdecl *
ipa_struct_reorg::record_var (tree decl, escape_type escapes, int arg)
{
  srtype *type;
  srdecl *sd = NULL;

  process_union (TREE_TYPE (decl));

  /* */
  if (handled_type (TREE_TYPE (decl)))
    {
      type = record_type (inner_type (TREE_TYPE (decl)));
      escape_type e;

      if (done_recording && !type)
	return NULL;

      gcc_assert (type);
      if (TREE_CODE (decl) == VAR_DECL && is_global_var (decl))
	sd = globals.record_decl (type, decl, arg);
      else
	{
	  gcc_assert (current_function);
          sd = current_function->record_decl (type, decl, arg);
	}

      /* If the variable has the "used" attribute, then treat the type as escaping. */
      if (escapes != does_not_escape)
	e = escapes;
      else if (TREE_CODE (decl) != SSA_NAME && DECL_PRESERVE_P (decl))
	e = escape_marked_as_used;
      else if (TREE_THIS_VOLATILE (decl))
	e = escape_volatile;
      else if (TREE_CODE (decl) != SSA_NAME && DECL_USER_ALIGN (decl))
	e = escape_user_alignment;
      else if (TREE_CODE (decl) != SSA_NAME && TREE_STATIC (decl) && TREE_PUBLIC (decl))
	e = escape_via_global_var;
      /* We don't have an initlizer. */
      else if (TREE_CODE (decl) != SSA_NAME && DECL_INITIAL (decl) == error_mark_node)
	e = escape_via_global_var;
      else
	e = escape_type_volatile_array_or_ptrptr (TREE_TYPE (decl));

      if (e != does_not_escape)
	type->mark_escape (e, NULL);
    }

  /* Record the initial usage of variables as types escapes.  */
  if (TREE_CODE (decl) != SSA_NAME && TREE_STATIC (decl) && DECL_INITIAL (decl))
    {
      walk_tree_without_duplicates (&DECL_INITIAL (decl), record_init_types, this);
      if (!integer_zerop (DECL_INITIAL (decl))
	  && DECL_INITIAL (decl) != error_mark_node)
	mark_type_as_escape (TREE_TYPE (decl), escape_via_global_init);
    }
  return sd;
}

/* Find void* ssa_names which are used inside MEM[] or if we have &a.c,
   mark the type as escaping. */

void
ipa_struct_reorg::find_var (tree expr, gimple *stmt)
{
  /* If we have VCE<a> mark the outer type as escaping and the inner one
     Also mark the inner most operand.  */
  if (TREE_CODE (expr) == VIEW_CONVERT_EXPR)
    {
      mark_type_as_escape (TREE_TYPE (expr), escape_vce, stmt);
      mark_type_as_escape (TREE_TYPE (TREE_OPERAND (expr, 0)),
			   escape_vce, stmt);
    }

  /* If we have &b.c then we need to mark the type of b
     as escaping as tracking a will be hard.  */
  if (TREE_CODE (expr) == ADDR_EXPR
      || TREE_CODE (expr) == VIEW_CONVERT_EXPR)
    {
      tree r = TREE_OPERAND (expr, 0);
      if (handled_component_p (r)
          || TREE_CODE (r) == MEM_REF)
        {
          while (handled_component_p (r)
		 || TREE_CODE (r) == MEM_REF)
	    {
	      if (TREE_CODE (r) == VIEW_CONVERT_EXPR)
		{
		  mark_type_as_escape (TREE_TYPE (r), escape_vce, stmt);
		  mark_type_as_escape (TREE_TYPE (TREE_OPERAND (r, 0)),
				       escape_vce, stmt);
		}
	      if (TREE_CODE (r) == MEM_REF)
		mark_type_as_escape (TREE_TYPE (TREE_OPERAND (r, 1)),
				     escape_addr, stmt);
              r = TREE_OPERAND (r, 0);
	    }
          mark_expr_escape (r, escape_addr, stmt);
        }
    }

  tree base;
  bool indirect;
  srtype *type;
  srfield *field;
  bool realpart, imagpart, address;
  get_type_field (expr, base, indirect, type, field,
		  realpart, imagpart, address, true, true);
}


void
ipa_struct_reorg::find_vars (gimple *stmt)
{
  gasm *astmt;
  switch (gimple_code (stmt))
    {
    case GIMPLE_ASSIGN:
      if (gimple_assign_rhs_class (stmt) == GIMPLE_SINGLE_RHS
          || gimple_assign_rhs_code (stmt) == POINTER_PLUS_EXPR)
	{
	  tree lhs = gimple_assign_lhs (stmt);
	  tree rhs = gimple_assign_rhs1 (stmt);
	  find_var (gimple_assign_lhs (stmt), stmt);
	  find_var (gimple_assign_rhs1 (stmt), stmt);
	  if (TREE_CODE (lhs) == SSA_NAME
	      && VOID_POINTER_P (TREE_TYPE (lhs))
	      && handled_type (TREE_TYPE (rhs)))
	    {
	      srtype *t = find_type (inner_type (TREE_TYPE (rhs)));
	      srdecl *d = find_decl (lhs);
	      if (!d && t)
		current_function->record_decl (t, lhs, -1);
	    }
	  if (TREE_CODE (rhs) == SSA_NAME
	      && VOID_POINTER_P (TREE_TYPE (rhs))
	      && handled_type (TREE_TYPE (lhs)))
	    {
	      srtype *t = find_type (inner_type (TREE_TYPE (lhs)));
	      srdecl *d = find_decl (rhs);
	      if (!d && t)
		current_function->record_decl (t, rhs, -1);
	    }
	}
      break;

    case GIMPLE_CALL:
      if (gimple_call_lhs (stmt))
	find_var (gimple_call_lhs (stmt), stmt);

      if (gimple_call_chain (stmt))
	find_var (gimple_call_chain (stmt), stmt);

      for (unsigned i = 0; i < gimple_call_num_args (stmt); i++)
	find_var (gimple_call_arg (stmt, i), stmt);
      break;

    case GIMPLE_ASM:
      astmt = as_a <gasm*>(stmt);
      for (unsigned i = 0; i < gimple_asm_ninputs (astmt); i++)
	find_var (TREE_VALUE (gimple_asm_input_op (astmt, i)), stmt);
      for (unsigned i = 0; i < gimple_asm_noutputs (astmt); i++)
	find_var (TREE_VALUE (gimple_asm_output_op (astmt, i)), stmt);
      mark_types_asm (astmt);
      break;

    case GIMPLE_RETURN:
      {
	tree expr = gimple_return_retval (as_a<greturn*>(stmt));
	if (expr)
          find_var (expr, stmt);
	/* return &a; should mark the type of a as escaping through a return. */
	if (expr && TREE_CODE (expr) == ADDR_EXPR)
	  {
	    expr = TREE_OPERAND (expr, 0);
	    srdecl *d = find_decl (expr);
	    if (d)
	      d->type->mark_escape (escape_return, stmt);
	  }
      }
      break;

    default:
      break;
    }
}

/* Maybe record access of statement for further analaysis. */

void
ipa_struct_reorg::maybe_record_stmt (cgraph_node *node, gimple *stmt)
{
  switch (gimple_code (stmt))
    {
    case GIMPLE_ASSIGN:
      maybe_record_assign (node, as_a <gassign *> (stmt));
      break;
    case GIMPLE_CALL:
      maybe_record_call (node, as_a <gcall *> (stmt));
      break;
    case GIMPLE_DEBUG:
      break;
    case GIMPLE_GOTO:
    case GIMPLE_SWITCH:
      break;
    default:
      break;
    }
}

/* This function checks whether ARG is a result of multiplication
   of some number by STRUCT_SIZE. If yes, the function returns true
   and this number is filled into NUM.  */

static bool
is_result_of_mult (tree arg, tree *num, tree struct_size)
{
  if (!struct_size
      || TREE_CODE (struct_size) != INTEGER_CST
      || integer_zerop (struct_size))
    return false;

  /* If we have a integer, just check if it is a multiply of STRUCT_SIZE. */
  if (TREE_CODE (arg) == INTEGER_CST)
    {
      if (integer_zerop (size_binop (FLOOR_MOD_EXPR, arg, struct_size)))
	{
	  *num = size_binop (FLOOR_DIV_EXPR, arg, struct_size);
	  return true;
	}
      return false;
    }
  gimple *size_def_stmt = SSA_NAME_DEF_STMT (arg);

  /* If the allocation statement was of the form
     D.2229_10 = <alloc_func> (D.2228_9);
     then size_def_stmt can be D.2228_9 = num.3_8 * 8;  */    

  while (size_def_stmt && is_gimple_assign (size_def_stmt))
    {
      tree lhs = gimple_assign_lhs (size_def_stmt);

      /* We expect temporary here.  */
      if (!is_gimple_reg (lhs))
	return false;

      // FIXME: this should handle SHIFT also.
      if (gimple_assign_rhs_code (size_def_stmt) == PLUS_EXPR)
	{
	  tree num1, num2;
	  tree arg0 = gimple_assign_rhs1 (size_def_stmt);
	  tree arg1 = gimple_assign_rhs2 (size_def_stmt);
	  if (!is_result_of_mult (arg0, &num1, struct_size))
	    return false;
	  if (!is_result_of_mult (arg1, &num2, struct_size))
	    return false;
	  *num = size_binop (PLUS_EXPR, num1, num2);
	  return true;
	}
      if (gimple_assign_rhs_code (size_def_stmt) == MULT_EXPR)
	{
	  tree arg0 = gimple_assign_rhs1 (size_def_stmt);
	  tree arg1 = gimple_assign_rhs2 (size_def_stmt);
	  tree num1;

	  if (is_result_of_mult (arg0, &num1, struct_size))
	    {
	      *num = size_binop (MULT_EXPR, arg1, num1);
	      return true;
	    }
	  if (is_result_of_mult (arg1, &num1, struct_size))
	    {
	      *num = size_binop (MULT_EXPR, arg0, num1);
	      return true;
	    }

	  *num = NULL_TREE;
	  return false;
	}
      else if (gimple_assign_rhs_code (size_def_stmt) == SSA_NAME)
	{
	  arg = gimple_assign_rhs1 (size_def_stmt);
	  size_def_stmt = SSA_NAME_DEF_STMT (arg);
	}
      else
	{
	  *num = NULL_TREE;
	  return false;
	}	
    }

  *num = NULL_TREE;
  return false;
}

/* Return TRUE if STMT is an allocation statement that is handled. */

static bool
handled_allocation_stmt (gimple *stmt)
{
  if (gimple_call_builtin_p (stmt, BUILT_IN_REALLOC)
      || gimple_call_builtin_p (stmt, BUILT_IN_MALLOC)
      || gimple_call_builtin_p (stmt, BUILT_IN_CALLOC)
      || gimple_call_builtin_p (stmt, BUILT_IN_ALIGNED_ALLOC)
      || gimple_call_builtin_p (stmt, BUILT_IN_ALLOCA)
      || gimple_call_builtin_p (stmt, BUILT_IN_ALLOCA_WITH_ALIGN))
    return true;
  return false;
}


/* Returns the allocated size / T size for STMT.  That is the number of
   elements in the array allocated.   */

tree
ipa_struct_reorg::allocate_size (srtype *type, gimple *stmt)
{
  if (!stmt
      || gimple_code (stmt) != GIMPLE_CALL
      || !handled_allocation_stmt (stmt))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\nNot a allocate statment:\n");
	  print_gimple_stmt (dump_file, stmt, 0, 0);
	  fprintf (dump_file, "\n");
	}
      return NULL;
    }

  if (type->has_escaped ())
    return NULL;

  tree struct_size = TYPE_SIZE_UNIT (type->type);

  tree size = gimple_call_arg (stmt, 0);

  if (gimple_call_builtin_p (stmt, BUILT_IN_REALLOC)
      || gimple_call_builtin_p (stmt, BUILT_IN_ALIGNED_ALLOC))
    size = gimple_call_arg (stmt, 1);
  else if (gimple_call_builtin_p (stmt, BUILT_IN_CALLOC))
    {
      tree arg1;
      arg1 = gimple_call_arg (stmt, 1);
      /* Check that second argument is a constant equal to the size of structure.  */
      if (operand_equal_p (arg1, struct_size, 0))
	return size;
      /* Check that first argument is a constant equal to the size of structure.  */
      if (operand_equal_p (size, struct_size, 0))
	return arg1;
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\ncalloc the correct size:\n");
	  print_gimple_stmt (dump_file, stmt, 0, 0);
	  fprintf (dump_file, "\n");
	}
      return NULL;
    }

  tree num;
  if (!is_result_of_mult (size, &num, struct_size))
    return NULL;

  return num;

}


void
ipa_struct_reorg::maybe_mark_or_record_other_side (tree side, tree other, gimple *stmt)
{
  gcc_assert (TREE_CODE (side) == SSA_NAME || TREE_CODE (side) == ADDR_EXPR);
  srtype *type = NULL;
  if (handled_type (TREE_TYPE (other)))
    type = record_type (inner_type (TREE_TYPE (other)));
  if (TREE_CODE (side) == ADDR_EXPR)
    side = TREE_OPERAND (side, 0);
  srdecl *d = find_decl (side);
  if (!type)
    {
      if (!d)
	return;
      if (TREE_CODE (side) == SSA_NAME
	  && VOID_POINTER_P (TREE_TYPE (side)))
	return;
      d->type->mark_escape (escape_cast_another_ptr, stmt);
      return;
    }

  if (!d)
    {
      if (VOID_POINTER_P (TREE_TYPE (side))
	  && TREE_CODE (side) == SSA_NAME)
	current_function->record_decl (type, side, -1);
      else
	type->mark_escape (escape_cast_another_ptr, stmt);
    }
  else if (type != d->type)
    {
      type->mark_escape (escape_cast_another_ptr, stmt);
      d->type->mark_escape (escape_cast_another_ptr, stmt);
    }
}

/* Record accesses in an assignment statement STMT.  */

void
ipa_struct_reorg::maybe_record_assign (cgraph_node *node, gassign *stmt)
{

  /*  */

  if (gimple_clobber_p (stmt))
    {
      record_stmt_expr (gimple_assign_lhs (stmt), node, stmt);
      return;
    }

  if (gimple_assign_rhs_code (stmt) == POINTER_PLUS_EXPR)
    {
      tree lhs = gimple_assign_lhs (stmt);
      tree rhs1 = gimple_assign_rhs1 (stmt);
      tree rhs2 = gimple_assign_rhs2 (stmt);
      tree num;
      if (!handled_type (TREE_TYPE (lhs)))
	return;
      /* Check if rhs2 is a multiplication of the size of the type. */
      if (is_result_of_mult (rhs2, &num, TYPE_SIZE_UNIT (TREE_TYPE (TREE_TYPE (lhs)))))
	{
	  record_stmt_expr (lhs, node, stmt);
	  record_stmt_expr (rhs1, node, stmt);
	}
      else
	{
	  mark_expr_escape (lhs, escape_non_multiply_size, stmt);
	  mark_expr_escape (rhs1, escape_non_multiply_size, stmt);
	}
      return;
    }
  /* Copies, References, Taking addresses. */
  if (gimple_assign_rhs_class (stmt) == GIMPLE_SINGLE_RHS)
    {
      tree lhs = gimple_assign_lhs (stmt);
      tree rhs = gimple_assign_rhs1 (stmt);
      /* If we have a = &b.c then we need to mark the type of b
         as escaping as tracking a will be hard.  */
      if (TREE_CODE (rhs) == ADDR_EXPR)
	{
	  tree r = TREE_OPERAND (rhs, 0);
	  if (handled_component_p (r))
	    {
	      while (handled_component_p (r))
		r = TREE_OPERAND (r, 0);
	      mark_expr_escape (r, escape_addr, stmt);
	      return;
	    }
	}
      if ((TREE_CODE (rhs) == SSA_NAME || TREE_CODE (rhs) == ADDR_EXPR))
	maybe_mark_or_record_other_side (rhs, lhs, stmt);
      if (TREE_CODE (lhs) == SSA_NAME)
	maybe_mark_or_record_other_side (lhs, rhs, stmt);
    }
}

tree
get_ref_base_and_offset (tree &e, HOST_WIDE_INT &offset, bool &realpart, bool &imagpart, tree &accesstype)
{
  offset = 0;
  realpart = false;
  imagpart = false;
  accesstype = NULL_TREE;
  if (TREE_CODE (e) == REALPART_EXPR)
    {
      e = TREE_OPERAND (e, 0);
      realpart = true;
    }
  if (TREE_CODE (e) == IMAGPART_EXPR)
    {
      e = TREE_OPERAND (e, 0);
      imagpart = true;
    }
  tree expr = e;
  while (true)
    {
      switch (TREE_CODE (expr))
	{
	  case COMPONENT_REF:
	  {
	    tree field = TREE_OPERAND (expr, 1);
	    tree field_off = byte_position (field);
	    if (TREE_CODE (field_off) != INTEGER_CST)
	      return NULL;
	    offset += tree_to_shwi (field_off);
	    expr = TREE_OPERAND (expr, 0);
	    accesstype = NULL;
	    break;
	  }
	  case MEM_REF:
	  {
	    tree field_off = TREE_OPERAND (expr, 1);
	    gcc_assert (TREE_CODE (field_off) == INTEGER_CST);
	    /* So we can mark the types as escaping if different. */
	    accesstype = TREE_TYPE (field_off);
	    offset += tree_to_uhwi (field_off);
	    return TREE_OPERAND (expr, 0);
	  }
	  default:
	    return expr;
	}
    }
}

/* Return true if EXPR was accessing the whole type T.  */

bool
ipa_struct_reorg::wholeaccess (tree expr, tree base, tree accesstype, srtype *t)
{
  if (expr == base)
    return true;

  if (TREE_CODE (expr) == ADDR_EXPR && TREE_OPERAND (expr, 0) == base)
    return true;

  if (!accesstype)
    return false;

  if (!types_compatible_p (TREE_TYPE (expr), TREE_TYPE (accesstype)))
    return false;

  if (!handled_type (TREE_TYPE (expr)))
    return false;

  srtype *other_type = find_type (inner_type (TREE_TYPE (expr)));

  if (t == other_type)
    return true;

  return false;
}

bool
ipa_struct_reorg::get_type_field (tree expr, tree &base, bool &indirect, srtype *&type, srfield *&field, bool &realpart, bool &imagpart, bool &address, bool should_create, bool can_escape)
{
  HOST_WIDE_INT offset;
  tree accesstype;
  address = false;
  bool mark_as_bit_field = false;

  if (TREE_CODE (expr) == BIT_FIELD_REF)
    {
      expr = TREE_OPERAND (expr, 0);
      mark_as_bit_field = true;
    }

  base = get_ref_base_and_offset (expr, offset, realpart, imagpart, accesstype);

  /* Variable access, unkown type. */
  if (base == NULL)
    return false;

  if (TREE_CODE (base) == ADDR_EXPR)
    {
      address = true;
      base = TREE_OPERAND (base, 0);
    }

  if (offset != 0 && accesstype)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Non zero offset (%d) with MEM.\n", (int)offset);
	  print_generic_expr (dump_file, expr, 0);
	  fprintf (dump_file, "\n");
	  print_generic_expr (dump_file, base, 0);
	  fprintf (dump_file, "\n");
	}
    }

  srdecl *d = find_decl (base);
  srtype *t;

  if (integer_zerop (base))
    {
      gcc_assert (!d);
      if (!accesstype)
	return false;
      t = find_type (inner_type (inner_type (accesstype)));
      if (!t && should_create && handled_type (accesstype))
	t = record_type (inner_type (accesstype));
      if (!t)
	return false;
    }
  else if (!d && accesstype)
    {
      if (!should_create)
	return false;
      if (!handled_type (accesstype))
	return false;
      t = find_type (inner_type (inner_type (accesstype)));
      if (!t)
	t = record_type (inner_type (accesstype));
      if (!t || t->has_escaped ())
	return false;
      /* If base is not void* mark the type as escaping. */
      if (!VOID_POINTER_P (TREE_TYPE (base)))
	{
          gcc_assert (can_escape);
	  t->mark_escape (escape_cast_another_ptr, NULL);
	  return false;
	}
      if (TREE_CODE (base) == SSA_NAME)
	current_function->record_decl (t, base, -1);
    }
  else if (!d)
    return false;
  else
    t = d->type;

  if (t->has_escaped ())
    return false;

  if (mark_as_bit_field)
    {
      gcc_assert (can_escape);
      t->mark_escape (escape_bitfields, NULL);
      return false;
    }

  if (wholeaccess (expr, base, accesstype, t))
    {
      field = NULL;
      type = t;
      indirect = accesstype != NULL;
      return true;
    }

  srfield *f = t->find_field (offset);
  if (!f)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\nunkown field\n");
	  print_generic_expr (dump_file, expr, 0);
	  fprintf (dump_file, "\n");
	  print_generic_expr (dump_file, base, 0);
	  fprintf (dump_file, "\n");
	}
      gcc_assert (can_escape);
      t->mark_escape (escape_unkown_field, NULL);
      return false;
    }
  if (!types_compatible_p (f->fieldtype, TREE_TYPE (expr)))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\nfieldtype = ");
	  print_generic_expr (dump_file, f->fieldtype, 0);
	  fprintf (dump_file, "\naccess type = ");
	  print_generic_expr (dump_file, TREE_TYPE (expr), 0);
	  fprintf (dump_file, "original expr = ");
	  print_generic_expr (dump_file, expr, 0);
	  fprintf (dump_file, "\n");
	}
      gcc_assert (can_escape);
      t->mark_escape (escape_unkown_field, NULL);
      return false;
    }
  field = f;
  type = t;
  indirect = accesstype != NULL;
  return true;
}

/* Mark the type used in EXPR as escaping. */

void
ipa_struct_reorg::mark_expr_escape (tree expr, escape_type escapes, gimple *stmt)
{
  tree base;
  bool indirect;
  srtype *type;
  srfield *field;
  bool realpart, imagpart, address;
  if (!get_type_field (expr, base, indirect, type, field, realpart, imagpart, address))
    return;

  type->mark_escape (escapes, stmt);
}

/* Record accesses in a call statement STMT.  */

void
ipa_struct_reorg::maybe_record_call (cgraph_node *node, gcall *stmt)
{
  tree argtype;
  tree fndecl;
  escape_type escapes = does_not_escape;
  bool free_or_realloc = gimple_call_builtin_p (stmt, BUILT_IN_FREE)
			 || gimple_call_builtin_p (stmt, BUILT_IN_REALLOC);

  /* We check allocation sites in a different location. */
  if (handled_allocation_stmt (stmt))
    return;


  /* A few cases here:
     1) assigned from the lhs
     2) Used in argument
     If a function being called is global (or indirect)
      then we reject the types as being escaping. */

  if (tree chain = gimple_call_chain (stmt))
    record_stmt_expr (chain, node, stmt); 

  /* Assigned from LHS.  */
  if (tree lhs = gimple_call_lhs (stmt))
    {
      /* FIXME: handle return types.. */
      mark_type_as_escape (TREE_TYPE (lhs), escape_return);
    }

  /* If we have an internal call, just record the stmt. */
  if (gimple_call_internal_p (stmt))
    {
      for (unsigned i = 0; i < gimple_call_num_args (stmt); i++)
	record_stmt_expr (gimple_call_arg (stmt, i), node, stmt);
      return;
    }

  fndecl = gimple_call_fndecl (stmt);

  /* If we have an indrect call, just mark the types as escape. */
  if (!fndecl)
    escapes = escape_pointer_function;
  /* Non local functions cause escape except for calls to free
     and realloc.
     FIXME: should support function annotations too.  */
  else if (!free_or_realloc
	   && !cgraph_node::local_info (fndecl)->local)
    escapes = escape_external_function;
  else if (!free_or_realloc
	   && !cgraph_node::local_info (fndecl)->can_change_signature)
    escapes = escape_cannot_change_signature;
  /* FIXME: we should be able to handle functions in other partitions.  */
  else if (symtab_node::get(fndecl)->in_other_partition)
    escapes = escape_external_function;

  if (escapes != does_not_escape)
    {
      for (unsigned i = 0; i < gimple_call_num_args (stmt); i++)
	mark_type_as_escape (TREE_TYPE (gimple_call_arg (stmt, i)),
			     escapes);
      return;
    }

  argtype = TYPE_ARG_TYPES (gimple_call_fntype (stmt));
  for (unsigned i = 0; i < gimple_call_num_args (stmt); i++)
    {
      tree arg = gimple_call_arg (stmt, i);
      if (argtype)
	{
	  tree argtypet = TREE_VALUE (argtype);
	  if (!free_or_realloc
	      && VOID_POINTER_P (argtypet))
	    mark_type_as_escape (TREE_TYPE (arg), escape_cast_void);
	  else
	    record_stmt_expr (arg, node, stmt);
	}
      else
	mark_type_as_escape (TREE_TYPE (arg), escape_var_arg_function);

      argtype = argtype ? TREE_CHAIN (argtype) : NULL_TREE;
    }

}


void
ipa_struct_reorg::record_stmt_expr (tree expr, cgraph_node *node, gimple *stmt)
{
  tree base;
  bool indirect;
  srtype *type;
  srfield *field;
  bool realpart, imagpart, address;
  if (!get_type_field (expr, base, indirect, type, field, realpart, imagpart, address))
    return;

  if (!opt_for_fn (current_function_decl, flag_ipa_struct_reorg))
    type->mark_escape (escape_non_optimize, stmt);

  /* Record it. */
  type->add_access (new sraccess (stmt, node, type, field));
}

/* Find function corresponding to NODE.  */

srfunction *
ipa_struct_reorg::find_function (cgraph_node *node)
{
  for (unsigned i = 0; i < functions.length (); i++)
    if (functions[i]->node == node)
	return functions[i];
  return NULL;
}

void
ipa_struct_reorg::check_type_and_push (tree newdecl, srtype *type, vec<srdecl*> &worklist, gimple *stmt)
{
  if (integer_zerop (newdecl))
    return;

  if (TREE_CODE (newdecl) == ADDR_EXPR)
    {
      srdecl *d = find_decl (TREE_OPERAND (newdecl, 0));
      if (!d)
	{
          type->mark_escape (escape_cast_another_ptr, stmt);
	  return;
	}
      if (d->type == type)
        return;

      srtype *type1 = d->type;
      type->mark_escape (escape_cast_another_ptr, stmt);
      type1->mark_escape (escape_cast_another_ptr, stmt);
      return;
    }

  srdecl *d = find_decl (newdecl);
  if (!d)
    {
      if (TREE_CODE (newdecl) == INTEGER_CST)
	{
          type->mark_escape (escape_int_const, stmt);
	  return;
	}
      /* If we have a non void* or a decl (which is hard to track),
         then mark the type as escaping.  */
      if (!VOID_POINTER_P (TREE_TYPE (newdecl))
	  || DECL_P (newdecl))
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "\nunkown decl: ");
	      print_generic_expr (dump_file, newdecl, 0);
	      fprintf (dump_file, " in type:\n");
	      print_generic_expr (dump_file, TREE_TYPE (newdecl), 0);
	      fprintf (dump_file, "\n");
	    }
          type->mark_escape (escape_cast_another_ptr, stmt);
	  return;
	}
      /* At this point there should only be unkown void* ssa names. */
      gcc_assert (TREE_CODE (newdecl) == SSA_NAME);
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\nrecording unkown decl: ");
	  print_generic_expr (dump_file, newdecl, 0);
	  fprintf (dump_file, " as type:\n");
	  type->simple_dump (dump_file);
	  fprintf (dump_file, "\n");
	}
      d = current_function->record_decl (type, newdecl, -1);
      worklist.safe_push (d);
      return;
    }

  /* Only add to the worklist if the decl is a SSA_NAME.  */
  if (TREE_CODE (newdecl) == SSA_NAME)
    worklist.safe_push (d);
  if (d->type == type)
    return;

  srtype *type1 = d->type;
  type->mark_escape (escape_cast_another_ptr, stmt);
  type1->mark_escape (escape_cast_another_ptr, stmt);

}

/*
  2) Check SSA_NAMEs for non type usages (source or use) (worlist of srdecl)
     a) if the SSA_NAME is sourced from a pointer plus, record the pointer and
	check to make sure the addition was a multiple of the size.
	check the pointer type too.
     b) If the name is sourced from an allocation check the allocation
	i) Add SSA_NAME (void*) to the worklist if allocated from realloc
     c) if the name is from a param, make sure the param type was of the original type
     d) if the name is from a cast/assignment, make sure it is used as that type or void*
	i) If void* then push the ssa_name into worklist
*/
void
ipa_struct_reorg::check_definition (srdecl *decl, vec<srdecl*> &worklist)
{
  tree ssa_name = decl->decl;
  srtype *type = decl->type;

  /* c) if the name is from a param, make sure the param type was
     of the original type */
  if (SSA_NAME_IS_DEFAULT_DEF (ssa_name))
    {
      tree var = SSA_NAME_VAR (ssa_name);
      if (var
	  && TREE_CODE (var) == PARM_DECL
	  && VOID_POINTER_P (TREE_TYPE (ssa_name)))
        type->mark_escape (escape_cast_void, NULL);
      return;
    }
  gimple *stmt = SSA_NAME_DEF_STMT (ssa_name);

  /*
     b) If the name is sourced from an allocation check the allocation
	i) Add SSA_NAME (void*) to the worklist if allocated from realloc
  */
  if (gimple_code (stmt) == GIMPLE_CALL)
    {
      /* For realloc, check the type of the argument. */
      if (gimple_call_builtin_p (stmt, BUILT_IN_REALLOC))
        check_type_and_push (gimple_call_arg (stmt, 0), type, worklist, stmt);

      if (!handled_allocation_stmt (stmt)
          || !allocate_size (type, stmt))
        type->mark_escape (escape_return, stmt);
      return;
    }
  /* If the SSA_NAME is sourced from an inline-asm, just mark the type as escaping.  */
  if (gimple_code (stmt) == GIMPLE_ASM)
    {
      type->mark_escape (escape_inline_asm, stmt);
      return;
    }

  /* If the SSA_NAME is sourced from a PHI check add each name to the worklist and
     check to make sure they are used correctly.  */
  if (gimple_code (stmt) == GIMPLE_PHI)
    {
      for (unsigned i = 0; i < gimple_phi_num_args (stmt); i++)
	check_type_and_push (gimple_phi_arg_def (stmt, i), type, worklist, stmt);
      return;
    }

  gcc_assert (gimple_code (stmt) == GIMPLE_ASSIGN);
  /*
     a) if the SSA_NAME is sourced from a pointer plus, record the pointer and
	check to make sure the addition was a multiple of the size.
	check the pointer type too.
  */

  tree rhs = gimple_assign_rhs1 (stmt);
  if (gimple_assign_rhs_code (stmt) == POINTER_PLUS_EXPR)
    {
      tree rhs2 = gimple_assign_rhs2 (stmt);
      tree num;
      if (!is_result_of_mult (rhs2, &num, TYPE_SIZE_UNIT (type->type)))
        type->mark_escape (escape_non_multiply_size, stmt);

      if (TREE_CODE (rhs) == SSA_NAME)
        check_type_and_push (rhs, type, worklist, stmt);
      return;
    }

  /* Casts between pointers and integer are escaping.  */
  if (gimple_assign_cast_p (stmt))
    {
      type->mark_escape (escape_cast_int, stmt);
      return;
    }

  /*
     d) if the name is from a cast/assignment, make sure it is used as that type or void*
	i) If void* then push the ssa_name into worklist
  */
  gcc_assert (gimple_assign_single_p (stmt));
  check_other_side (decl, rhs, stmt, worklist);
}

/* Mark the types used by the inline-asm as escaping.  It is unkown what happens inside
   an inline-asm. */

void
ipa_struct_reorg::mark_types_asm (gasm *astmt)
{
  for (unsigned i = 0; i < gimple_asm_ninputs (astmt); i++)
    {
      tree v = TREE_VALUE (gimple_asm_input_op (astmt, i));
      /* If we have &b, just strip the & here. */
      if (TREE_CODE (v) == ADDR_EXPR)
	v = TREE_OPERAND (v, 0);
      mark_expr_escape (v, escape_inline_asm, astmt);
    }
  for (unsigned i = 0; i < gimple_asm_noutputs (astmt); i++)
    {
      tree v = TREE_VALUE (gimple_asm_output_op (astmt, i));
      /* If we have &b, just strip the & here. */
      if (TREE_CODE (v) == ADDR_EXPR)
	v = TREE_OPERAND (v, 0);
      mark_expr_escape (v, escape_inline_asm, astmt);
    }
}

void
ipa_struct_reorg::check_other_side (srdecl *decl, tree other, gimple *stmt, vec<srdecl*> &worklist)
{
  srtype *type = decl->type;

  if (TREE_CODE (other) == SSA_NAME
      || DECL_P (other)
      || TREE_CODE (other) == INTEGER_CST)
    {
      check_type_and_push (other, type, worklist, stmt);
      return;
    }

  tree t = TREE_TYPE (other);
  if (!handled_type (t))
    {
      type->mark_escape (escape_cast_another_ptr, stmt);
      return;
    }

  srtype *t1 = find_type (inner_type (t));
  if (t1 == type)
    {
      tree base;
      bool indirect;
      srtype *type1;
      srfield *field;
      bool realpart, imagpart, address;
      if (!get_type_field (other, base, indirect, type1, field, realpart, imagpart, address))
        type->mark_escape (escape_cast_another_ptr, stmt);

      return;
    }

  if (t1)
    t1->mark_escape (escape_cast_another_ptr, stmt);

  type->mark_escape (escape_cast_another_ptr, stmt);
}


void
ipa_struct_reorg::check_use (srdecl *decl, gimple *stmt, vec<srdecl*> &worklist)
{
  srtype *type = decl->type;

  if (gimple_code (stmt) == GIMPLE_RETURN)
    {
      type->mark_escape (escape_return, stmt);
      return;
    }
  /* If the SSA_NAME PHI check and add the src to the worklist and
     check to make sure they are used correctly.  */
  if (gimple_code (stmt) == GIMPLE_PHI)
    {
      check_type_and_push (gimple_phi_result (stmt), type, worklist, stmt);
      return;
    }

  if (gimple_code (stmt) == GIMPLE_ASM)
    {
      mark_types_asm (as_a <gasm*>(stmt));
      return;
    }

  if (gimple_code (stmt) == GIMPLE_COND)
    {
      tree rhs1 = gimple_cond_lhs (stmt);
      tree rhs2 = gimple_cond_rhs (stmt);
      tree orhs = rhs1;
      if (gimple_cond_code (stmt) != EQ_EXPR
          && gimple_cond_code (stmt) != NE_EXPR)
	{
	  mark_expr_escape (rhs1, escape_non_eq, stmt);
	  mark_expr_escape (rhs2, escape_non_eq, stmt);
	}
      if (rhs1 == decl->decl)
	orhs = rhs2;
      if (integer_zerop (orhs))
	return;
      if (TREE_CODE (orhs) != SSA_NAME)
	mark_expr_escape (rhs1, escape_non_eq, stmt);
      check_type_and_push (orhs, type, worklist, stmt);
      return;
    }


  /* Casts between pointers and integer are escaping.  */
  if (gimple_assign_cast_p (stmt))
    {
      type->mark_escape (escape_cast_int, stmt);
      return;
    }

  /* We might have a_1 = ptr_2 == ptr_3; */
  if (is_gimple_assign (stmt)
      && TREE_CODE_CLASS (gimple_assign_rhs_code (stmt)) == tcc_comparison)
    {
      tree rhs1 = gimple_assign_rhs1 (stmt);
      tree rhs2 = gimple_assign_rhs2 (stmt);
      tree orhs = rhs1;
      if (gimple_assign_rhs_code (stmt) != EQ_EXPR
          && gimple_assign_rhs_code (stmt) != NE_EXPR)
	{
	  mark_expr_escape (rhs1, escape_non_eq, stmt);
	  mark_expr_escape (rhs2, escape_non_eq, stmt);
	}
      if (rhs1 == decl->decl)
	orhs = rhs2;
      if (integer_zerop (orhs))
	return;
      if (TREE_CODE (orhs) != SSA_NAME)
	mark_expr_escape (rhs1, escape_non_eq, stmt);
      check_type_and_push (orhs, type, worklist, stmt);
      return;
    }

  if (gimple_assign_single_p (stmt))
    {
      tree lhs = gimple_assign_lhs (stmt);
      tree rhs = gimple_assign_rhs1 (stmt);
      /* Check if we have a_1 = b_2; that a_1 is in the correct type. */
      if (decl->decl == rhs)
	{
	  check_other_side (decl, lhs, stmt, worklist);
	  return;
	}
    }

  if (is_gimple_assign (stmt)
      && gimple_assign_rhs_code (stmt) == POINTER_PLUS_EXPR)
    {
      tree rhs2 = gimple_assign_rhs2 (stmt);
      tree lhs = gimple_assign_lhs (stmt);
      tree num;
      check_other_side (decl, lhs, stmt, worklist);
      if (!is_result_of_mult (rhs2, &num, TYPE_SIZE_UNIT (type->type)))
        type->mark_escape (escape_non_multiply_size, stmt);
    }

}

/*
   2) Check SSA_NAMEs for non type usages (source or use) (worlist of srdecl)
	d) if the name is used in a cast/assignment, make sure it is used as that type or void*
	  i) If void* then push the ssa_name into worklist
	e) if used in conditional check the other side
	  i) If the conditional is non NE/EQ then mark the type as non rejecting
	f) Check if the use in a Pointer PLUS EXPR Is used by mulitplication of its size
  */
void
ipa_struct_reorg::check_uses (srdecl *decl, vec<srdecl*> &worklist)
{
  tree ssa_name = decl->decl;
  imm_use_iterator imm_iter;
  use_operand_p use_p;

  FOR_EACH_IMM_USE_FAST (use_p, imm_iter, ssa_name)
    {
      gimple *stmt = USE_STMT (use_p);

      if (is_gimple_debug (stmt))
	continue;

      check_use (decl, stmt, worklist);
    }
}

/* Record function corresponding to NODE. */

srfunction *
ipa_struct_reorg::record_function (cgraph_node *node)
{
  function *fn;
  tree parm, var;
  unsigned int i;
  srfunction *sfn;
  escape_type escapes = does_not_escape;

  sfn = new srfunction (node);
  functions.safe_push (sfn);

  if (dump_file  && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nRecording accesses and types from function: %s/%u\n",
             node->name (), node->order);

  /* Nodes without a body are not interesting.  Especially do not
     visit clones at this point for now - we get duplicate decls
     there for inline clones at least.  */
  if (!node->has_gimple_body_p () || node->global.inlined_to)
    return sfn;

  node->get_body ();
  fn = DECL_STRUCT_FUNCTION (node->decl);

  if (!fn)
    return sfn;

  current_function = sfn;

  if (DECL_PRESERVE_P (node->decl))
    escapes = escape_marked_as_used;
  else if (!node->local.local)
    escapes = escape_visible_function;
  else if (!node->local.can_change_signature)
    escapes = escape_cannot_change_signature;
  else if (!tree_versionable_function_p (node->decl))
    escapes = escape_noclonable_function;
  else if (!opt_for_fn (node->decl, flag_ipa_struct_reorg))
    escapes = escape_non_optimize;

  basic_block bb;
  gimple_stmt_iterator si;

  /* Record the static chain decl.  */
  if (fn->static_chain_decl)
   {
     srdecl *sd = record_var (fn->static_chain_decl,
                              escapes,
                              -2);
      if (sd)
        {
	  /* Specify that this type is used by the static
	     chain so it cannot be split. */
	  sd->type->chain_type = true;
          sfn->add_arg (sd);
          sd->type->add_function (sfn);
        }
    }

  /* Record the arguments. */
  for (parm = DECL_ARGUMENTS (node->decl), i = 0;
       parm;
       parm = DECL_CHAIN (parm), i++)
   {
      srdecl *sd = record_var (parm, escapes, i);
      if (sd)
	{
	  sfn->add_arg (sd);
	  sd->type->add_function (sfn);
	}
    }

  /* Mark the return type as escaping */
  {
    tree return_type = TREE_TYPE (TREE_TYPE (node->decl));
    mark_type_as_escape (return_type, escape_return, NULL);
  }

  /* If the cfg does not exist for the function, don't process the function.  */
  if (!fn->cfg)
    {
      current_function = NULL;
      return sfn;
    }

  /* The following order is done for recording stage:
     0) Record all variables/SSA_NAMES that are of struct type
     1) Record MEM_REF/COMPONENT_REFs
	a) Record SSA_NAMEs (void*) and record that as the accessed type.
  */

  push_cfun (fn);

  FOR_EACH_LOCAL_DECL (cfun, i, var)
    {
      if (TREE_CODE (var) != VAR_DECL)
        continue;

      record_var (var);
    }

  for (i = 1; i < num_ssa_names; ++i)
    {
      tree name = ssa_name (i);
      if (!name
          || has_zero_uses (name)
          || virtual_operand_p (name))
        continue;

      record_var (name);
    }

  /* Find the variables which are used via MEM_REF and are void* types. */
  FOR_EACH_BB_FN (bb, cfun)
    {
      for (si = gsi_start_bb (bb); !gsi_end_p (si); gsi_next (&si))
	{
	  gimple *stmt = gsi_stmt (si);
	  find_vars (stmt);
	}
    }

  auto_vec<srdecl *> worklist;
  for (unsigned i = 0; i < current_function->decls.length (); i++)
    {
      srdecl *decl = current_function->decls[i];
      if (TREE_CODE (decl->decl) == SSA_NAME)
	{
	  decl->visited = false;
	  worklist.safe_push (decl);
	}
    }

  /*
     2) Check SSA_NAMEs for non type usages (source or use) (worlist of srdecl)
	a) if the SSA_NAME is sourced from a pointer plus, record the pointer and
	   check to make sure the addition was a multiple of the size.
	   check the pointer type too.
	b) If the name is sourced from an allocation check the allocation
	  i) Add SSA_NAME (void*) to the worklist if allocated from realloc
	c) if the name is from a param, make sure the param type was of the original type
	d) if the name is used in a cast/assignment, make sure it is used as that type or void*
	  i) If void* then push the ssa_name into worklist
	e) if used in conditional check the other side
	  i) If the conditional is non NE/EQ then mark the type as non rejecting
	f) Check if the use in a POinter PLUS EXPR Is used by mulitplication of its size
  */

  while (!worklist.is_empty ())
    {
      srdecl *decl = worklist.pop ();
      if (decl->visited)
	continue;
      decl->visited = true;
      check_definition (decl, worklist);
      check_uses (decl, worklist);
    }

  FOR_EACH_BB_FN (bb, cfun)
    {
      for (si = gsi_start_bb (bb); !gsi_end_p (si); gsi_next (&si))
	{
	  gimple *stmt = gsi_stmt (si);
	  maybe_record_stmt (node, stmt);
	}
    }

  pop_cfun ();
  current_function = NULL;
  return sfn;
}


/* Record all accesses for all types including global variables. */

void
ipa_struct_reorg::record_accesses (void)
{
  varpool_node *var;
  cgraph_node *cnode;

  /* Record global (non-auto) variables first. */
  FOR_EACH_VARIABLE (var)
    {
      if (!var->real_symbol_p ())
	continue;

      /* Record all variables including the accesses inside a variable. */
      escape_type escapes = does_not_escape;
      if (var->externally_visible || !var->definition)
	escapes = escape_via_global_var;
      if (var->in_other_partition)
	escapes = escape_via_global_var;
      if (!var->externally_visible && var->definition)
	var->get_constructor ();
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Recording global variable: ");
	  print_generic_expr (dump_file, var->decl, 0);
	  fprintf (dump_file, "\n");
	}
      record_var (var->decl, escapes);
    }

  FOR_EACH_FUNCTION (cnode)
    {
      if (!cnode->real_symbol_p ())
        continue;

      /* Record accesses inside a function. */
      if(cnode->definition)
	record_function (cnode);
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "all types (before pruning):\n");
      dump_types (dump_file);
      fprintf (dump_file, "all functions (before pruning):\n");
      dump_functions (dump_file);
    }
  done_recording = true;
}

/* A helper function to detect cycles (recusive) types.
   Return TRUE if TYPE was a rescusive type.  */

bool
ipa_struct_reorg::walk_field_for_cycles (srtype *type)
{
  unsigned i;
  srfield *field;

  type->visited = true;
  if (type->escaped_rescusive ())
    return true;

  if (type->has_escaped ())
    return false;

  FOR_EACH_VEC_ELT (type->fields, i, field)
    {
      if (!field->type)
	;
      else if (field->type->visited
	       || walk_field_for_cycles (field->type))
	{
	  type->mark_escape (escape_rescusive_type, NULL);
	  return true;
	}
    }

  return false;
}

/* Clear visited on all types.  */

void
ipa_struct_reorg::clear_visited (void)
{
  for (unsigned i = 0; i < types.length (); i++)
    types[i]->visited = false;
}

/* Detect recusive types and mark them as escaping.  */

void
ipa_struct_reorg::detect_cycles (void)
{
  for (unsigned i = 0; i <  types.length (); i++)
    {
      if (types[i]->has_escaped ())
	continue;

      clear_visited ();
      walk_field_for_cycles (types[i]);
    }
}

/* Propagate escaping to depdenent types.  */

void
ipa_struct_reorg::propagate_escape (void)
{

  unsigned i;
  srtype *type;
  bool changed = false;

  do
    {
      changed = false;
      FOR_EACH_VEC_ELT (types, i, type)
	{
	  for (tree field = TYPE_FIELDS (type->type);
	       field;
	       field = DECL_CHAIN (field))
	    {
	      if (TREE_CODE (field) == FIELD_DECL
		  && handled_type (TREE_TYPE (field)))
		{
		  tree t = inner_type (TREE_TYPE (field));
		  srtype *type1 = find_type (t);
	          if (!type1)
		    continue;
		  if (type1->has_escaped ()
		      && !type->has_escaped ())
		    {
		      type->mark_escape (escape_dependent_type_escapes, NULL);
		      changed = true;
		    }
		  if (type->has_escaped ()
		      && !type1->has_escaped ())
		    {
		      type1->mark_escape (escape_dependent_type_escapes, NULL);
		      changed = true;
		    }
		}
	    }
	}
    } while (changed);
}

/* Prune the escaped types and their decls from what was recorded.  */

void
ipa_struct_reorg::prune_escaped_types (void)
{
  detect_cycles ();
  propagate_escape ();

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "all types (after prop but before pruning):\n");
      dump_types (dump_file);
      fprintf (dump_file, "all functions (after prop but before pruning):\n");
      dump_functions (dump_file);
    }

  if (dump_file)
    dump_types_escaped (dump_file);


  /* Prune the function arguments which escape
     and functions which have no types as arguments. */
  for (unsigned i = 0; i < functions.length (); )
    {
      srfunction *function = functions[i];

      /* Prune function arguments of types that escape. */
      for (unsigned j = 0; j < function->args.length ();)
	{
	  if (function->args[j]->type->has_escaped ())
	    function->args.ordered_remove (j);
	  else
	    j++;
	}

      /* Prune global variables that the function uses of types that escape. */
      for (unsigned j = 0; j < function->globals.length ();)
	{
	  if (function->globals[j]->type->has_escaped ())
	    function->globals.ordered_remove (j);
	  else
	    j++;
	}

      /* Prune variables that the function uses of types that escape. */
      for (unsigned j = 0; j < function->decls.length ();)
	{
	  srdecl *decl = function->decls[j];
	  if (decl->type->has_escaped ())
	    {
	      function->decls.ordered_remove (j);
	      delete decl;
	    }
	  else
	    j++;
	}

      /* Prune functions which don't refer to any variables any more.  */
      if (function->args.is_empty ()
	  && function->decls.is_empty ()
	  && function->globals.is_empty ())
	{
	  delete function;
	  functions.ordered_remove (i);
	}
      else
	i++;
    }

  /* Prune globals of types that escape, all references to those decls
     will have been removed in the first loop.  */
  for (unsigned j = 0; j < globals.decls.length ();)
    {
      srdecl *decl = globals.decls[j];
      if (decl->type->has_escaped ())
        {
          globals.decls.ordered_remove (j);
          delete decl;
        }
      else
        j++;
    }

  /* Prune types that escape, all references to those types
     will have been removed in the above loops.  */
  for (unsigned i = 0; i < types.length (); )
    {
      srtype *type = types[i];
      if (type->has_escaped ())
	{
	  /* All references to this type should have been removed now. */
	  delete type;
	  types.ordered_remove (i);
	}
      else
	i++;
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "all types (after pruning):\n");
      dump_types (dump_file);
      fprintf (dump_file, "all functions (after pruning):\n");
      dump_functions (dump_file);
    }
}

/* Analyze all of the types. */

void
ipa_struct_reorg::analyze_types (void)
{
  for (unsigned i = 0; i < types.length (); i++)
    {
      if (!types[i]->has_escaped ())
        types[i]->analyze();
    }
}

/* Create all new types we want to create. */

bool
ipa_struct_reorg::create_new_types (void)
{
  int newtypes = 0;
  clear_visited ();
  for (unsigned i = 0; i < types.length (); i++)
    newtypes += types[i]->create_new_type ();

  if (dump_file)
    {
      if (newtypes)
	fprintf (dump_file, "\nNumber of structures to transform is %d\n", newtypes);
      else
	fprintf (dump_file, "\nNo structures to transform.\n");
    }

  return newtypes != 0;
}

/* Create all the new decls except for the new arguments
   which create_new_functions would have created. */

void
ipa_struct_reorg::create_new_decls (void)
{
  globals.create_new_decls ();
  for (unsigned i = 0; i < functions.length (); i++)
    functions[i]->create_new_decls ();
}

/* Create the new arguments for the function corresponding to NODE. */

void
ipa_struct_reorg::create_new_args (cgraph_node *new_node)
{
  tree decl = new_node->decl;
  vec<tree> params = ipa_get_vector_of_formal_parms (decl);
  ipa_parm_adjustment_vec adjs;
  adjs.create (params.length ());
  for (unsigned i = 0; i < params.length (); i++)
    {
      struct ipa_parm_adjustment adj;
      tree parm = params[i];
      memset (&adj, 0, sizeof (adj));
      adj.base_index = i;
      adj.base = parm;
      srtype *t = find_type (inner_type (TREE_TYPE (parm)));
      if (!t
	  || t->has_escaped ()
	  || !t->has_new_type ())
	{
	  adj.op = IPA_PARM_OP_COPY;
	  adjs.safe_push (adj);
	  continue;
	}
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Creating a new argument for: ");
	  print_generic_expr (dump_file, params[i], 0);
	  fprintf (dump_file, " in function: ");
	  print_generic_expr (dump_file, decl, 0);
	  fprintf (dump_file, "\n");
	}
      adj.arg_prefix = "struct_reorg";
      adj.op = IPA_PARM_OP_NONE;
      for (unsigned j = 0; j < max_split && t->newtype[j]; j++)
	{
	  adj.type = reconstruct_complex_type (TREE_TYPE (parm), t->newtype[j]);
	  adjs.safe_push (adj);
	}
    }
  ipa_modify_formal_parameters (decl, adjs);
  params.release ();
  for (unsigned i = 0; i < adjs.length (); i++)
    {
      if (adjs[i].op != IPA_PARM_OP_NONE)
	continue;
      tree decl = adjs[i].base;
      srdecl *d = find_decl (decl);
      if (!d)
	continue;
      unsigned j = 0;
      while (j < max_split && d->newdecl[j])
	j++;
      d->newdecl[j] = adjs[i].new_decl;
    }
  adjs.release ();

  function *fn = DECL_STRUCT_FUNCTION (decl);

  if (!fn->static_chain_decl)
    return;
  srdecl *chain = find_decl (fn->static_chain_decl);
  if (!chain)
    return;

  srtype *type = chain->type;
  tree orig_var = chain->decl;
  const char *tname = NULL;
  if (DECL_NAME (orig_var))
    tname = IDENTIFIER_POINTER (DECL_NAME (orig_var));
  gcc_assert (!type->newtype[1]);
  tree new_name = NULL;
  char *name = NULL;
  if (tname)
    {
      name = concat (tname, ".reorg.0", NULL);
      new_name = get_identifier (name);
      free (name);
    }
  tree newtype1 = reconstruct_complex_type (TREE_TYPE (orig_var), type->newtype[0]);
  chain->newdecl[0] = build_decl (DECL_SOURCE_LOCATION (orig_var),
				  PARM_DECL, new_name, newtype1);
  copy_var_attributes (chain->newdecl[0], orig_var);
  fn->static_chain_decl = chain->newdecl[0];

}

/* Find the refered DECL in the current function or globals.
   If this is a global decl, record that as being used
   in the current function.  */

srdecl *
ipa_struct_reorg::find_decl (tree decl)
{
  srdecl *d;
  d = globals.find_decl (decl);
  if (d)
    {
      /* Record the global usage in the current function.  */
      if (!done_recording && current_function)
	{
	  bool add = true;
	  /* No reason to add it to the current function if it is
	     already recorded as such. */
	  for (unsigned i = 0; i < current_function->globals.length (); i++)
	    {
	      if (current_function->globals[i] == d)
		{
		  add = false;
		  break;
		}
	    }
	  if (add)
	    current_function->globals.safe_push (d);
	}
      return d;
    }
  if (current_function)
    return current_function->find_decl (decl);
  return NULL;
}

/* Create new function clones for the cases where the arguments
   need to be changed.  */

void
ipa_struct_reorg::create_new_functions (void)
{
  for (unsigned i = 0; i < functions.length (); i++)
    {
      srfunction *f = functions[i];
      bool anyargchanges = false;
      cgraph_node *new_node;
      cgraph_node *node = f->node;
      int newargs = 0;
      if (f->old)
	continue;

      if (f->args.length () == 0)
	continue;

      for (unsigned j = 0; j < f->args.length (); j++)
	{
	  srdecl *d = f->args[j];
	  srtype *t = d->type;
	  if (t->has_new_type ())
	    {
	      newargs += t->newtype[1] != NULL;
	      anyargchanges = true;
	    }
	}
      if (!anyargchanges)
	continue;

      if (dump_file)
	{
	  fprintf (dump_file, "Creating a clone of function: ");
	  f->simple_dump (dump_file);
	  fprintf (dump_file, "\n");
	}
      statistics_counter_event (NULL, "Create new function", 1);
      new_node = node->create_version_clone_with_body (vNULL, NULL,
						       NULL, false, NULL, NULL,
						      "struct_reorg");
      new_node->make_local ();
      f->newnode = new_node;
      srfunction *n = record_function (new_node);
      current_function = n;
      n->old = f;
      f->newf = n;
      /* Create New arguments. */
      create_new_args (new_node);
      current_function = NULL;
    }
}

bool
ipa_struct_reorg::rewrite_lhs_rhs (tree lhs, tree rhs, tree newlhs[max_split], tree newrhs[max_split])
{
  bool l = rewrite_expr (lhs, newlhs);
  bool r = rewrite_expr (rhs, newrhs);

  /* Handle NULL pointer specially. */
  if (l && !r && integer_zerop (rhs))
    {
      r = true;
      for (unsigned i = 0; i < max_split && newlhs[i]; i++)
	newrhs[i] = fold_convert (TREE_TYPE (newlhs[i]), rhs);
    }

  return l || r;
}

bool
ipa_struct_reorg::rewrite_expr (tree expr, tree newexpr[max_split], bool ignore_missing_decl)
{
  tree base;
  bool indirect;
  srtype *t;
  srfield *f;
  bool realpart, imagpart;
  bool address;

  tree newbase[max_split];
  memset (newexpr, 0, sizeof(tree[max_split]));

  if (TREE_CODE (expr) == CONSTRUCTOR)
    {
      srtype *t = find_type (TREE_TYPE (expr));
      if (!t)
	return false;
      gcc_assert (CONSTRUCTOR_NELTS (expr) == 0);
      if (!t->has_new_type ())
	return false;
      for (unsigned i = 0; i < max_split && t->newtype[i]; i++)
	newexpr[i] = build_constructor (t->newtype[i], NULL);
      return true;
    }

  if (!get_type_field (expr, base, indirect, t, f, realpart, imagpart, address))
    return false;

  /* If the type is not changed, then just return false. */
  if (!t->has_new_type ())
    return false;

  /*  NULL pointer handling is "special".  */
  if (integer_zerop (base))
    {
      gcc_assert (indirect && !address);
      for (unsigned i = 0; i < max_split && t->newtype[i]; i++)
	{
	  tree newtype1 = reconstruct_complex_type (TREE_TYPE (base), t->newtype[i]);
	  newbase[i] = fold_convert (newtype1, base);
	}
    }
  else
    {
      srdecl *d = find_decl (base);

      if (!d && dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Can't find decl:\n");
	  print_generic_expr (dump_file, base, 0);
	  fprintf (dump_file, "\ntype:\n");
	  t->dump (dump_file);
	}
      if (!d && ignore_missing_decl)
	return true;
      gcc_assert (d);
      memcpy (newbase, d->newdecl, sizeof(d->newdecl));
    }

  if (f == NULL)
    {
      memcpy (newexpr, newbase, sizeof(newbase));
      for (unsigned i = 0; i < max_split && newexpr[i]; i++)
	{
	  if (address)
	    newexpr[i] = build_fold_addr_expr (newexpr[i]);
	  if (indirect)
	    newexpr[i] = build_simple_mem_ref (newexpr[i]);
	  if (imagpart)
	    newexpr[i] = build1 (IMAGPART_EXPR, TREE_TYPE (TREE_TYPE (newexpr[i])), newexpr[i]);
	  if (realpart)
	    newexpr[i] = build1 (REALPART_EXPR, TREE_TYPE (TREE_TYPE (newexpr[i])), newexpr[i]);
	}
      return true;
    }

  tree newdecl = newbase[f->clusternum];
  for (unsigned i = 0; i < max_split && f->newfield[i]; i++)
    {
      tree newbase1 = newdecl;
      if (address)
        newbase1 = build_fold_addr_expr (newbase1);
      if (indirect)
        newbase1 = build_simple_mem_ref (newbase1);
      newexpr[i] = build3 (COMPONENT_REF, TREE_TYPE (f->newfield[i]),
			   newbase1, f->newfield[i], NULL_TREE);
      if (imagpart)
	newexpr[i] = build1 (IMAGPART_EXPR, TREE_TYPE (TREE_TYPE (newexpr[i])), newexpr[i]);
      if (realpart)
	newexpr[i] = build1 (REALPART_EXPR, TREE_TYPE (TREE_TYPE (newexpr[i])), newexpr[i]);
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "cluster: %d. decl = ", (int)f->clusternum);
	  print_generic_expr (dump_file, newbase1, 0);
	  fprintf (dump_file, "\nnewexpr = ");
	  print_generic_expr (dump_file, newexpr[i], 0);
	  fprintf (dump_file, "\n");
	}
    }
  return true;
}

bool
ipa_struct_reorg::rewrite_assign (gassign *stmt, gimple_stmt_iterator *gsi)
{
  bool remove = false;
  if (gimple_clobber_p (stmt))
    {
      tree lhs = gimple_assign_lhs (stmt);
      tree newlhs[max_split];
      if (!rewrite_expr (lhs, newlhs))
	return false;
      for (unsigned i = 0; i < max_split && newlhs[i]; i++)
	{
	  tree clobber = build_constructor (TREE_TYPE (newlhs[i]), NULL);
	  TREE_THIS_VOLATILE (clobber) = true;
	  gimple *newstmt = gimple_build_assign (newlhs[i], clobber);
	  gsi_insert_before (gsi, newstmt, GSI_SAME_STMT);
	  remove = true;
	}
      return remove;
    }

  if (gimple_assign_rhs_code (stmt) == EQ_EXPR
      || gimple_assign_rhs_code (stmt) == NE_EXPR)
    {
      tree rhs1 = gimple_assign_rhs1 (stmt);
      tree rhs2 = gimple_assign_rhs2 (stmt);
      tree newrhs1[max_split];
      tree newrhs2[max_split];
      tree_code rhs_code = gimple_assign_rhs_code (stmt);
      tree_code code = rhs_code == EQ_EXPR ? BIT_AND_EXPR : BIT_IOR_EXPR;
      if (!rewrite_lhs_rhs (rhs1, rhs2, newrhs1, newrhs2))
	return false;
      tree newexpr = NULL_TREE;
      for (unsigned i = 0; i < max_split && newrhs1[i]; i++)
	{
	  tree expr = gimplify_build2 (gsi, rhs_code, boolean_type_node, newrhs1[i], newrhs2[i]);
          if (!newexpr)
	    newexpr = expr;
	  else
	    newexpr = gimplify_build2 (gsi, code, boolean_type_node, newexpr, expr);
	}

      if (newexpr)
	{
	  newexpr = fold_convert (TREE_TYPE (gimple_assign_lhs (stmt)), newexpr);
	  gimple_assign_set_rhs_from_tree (gsi, newexpr);
	  update_stmt (stmt);
	}
      return false;
    }

  if (gimple_assign_rhs_code (stmt) == POINTER_PLUS_EXPR)
    {
      tree lhs = gimple_assign_lhs (stmt);
      tree rhs1 = gimple_assign_rhs1 (stmt);
      tree rhs2 = gimple_assign_rhs2 (stmt);
      tree newlhs[max_split];
      tree newrhs[max_split];

      if (!rewrite_lhs_rhs (lhs, rhs1, newlhs, newrhs))
	return false;
      tree size = TYPE_SIZE_UNIT (TREE_TYPE (TREE_TYPE (lhs)));
      tree num;
      /* Check if rhs2 is a multiplication of the size of the type. */
      if (!is_result_of_mult (rhs2, &num, size))
	internal_error ("the rhs of pointer was not a multiplicate and it slipped through.");

      num = gimplify_build1 (gsi, NOP_EXPR, sizetype, num);
      for (unsigned i = 0; i < max_split && newlhs[i]; i++)
	{
	  gimple *new_stmt;

          tree newsize = TYPE_SIZE_UNIT (TREE_TYPE (TREE_TYPE (newlhs[i])));
	  newsize = gimplify_build2 (gsi, MULT_EXPR, sizetype, num, newsize);
	  new_stmt = gimple_build_assign (newlhs[i], POINTER_PLUS_EXPR, newrhs[i], newsize);
	  gsi_insert_before (gsi, new_stmt, GSI_SAME_STMT);
	  remove = true;
	}
      return remove;
    }
  if (gimple_assign_rhs_class (stmt) == GIMPLE_SINGLE_RHS)
    {
      tree lhs = gimple_assign_lhs (stmt);
      tree rhs = gimple_assign_rhs1 (stmt);

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "rewriting stamtenet:\n");
	  print_gimple_stmt (dump_file, stmt, 0, 0);
	  fprintf (dump_file, "\n");
	}
      tree newlhs[max_split];
      tree newrhs[max_split];
      if (!rewrite_lhs_rhs (lhs, rhs, newlhs, newrhs))
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "\nDid nothing to statement.\n");
	  return false;
	}

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "\nreplaced with:\n");
      for (unsigned i = 0; i < max_split && (newlhs[i] || newrhs[i]); i++)
	{
	  gimple *newstmt = gimple_build_assign (newlhs[i] ? newlhs[i] : lhs, newrhs[i] ? newrhs[i] : rhs);
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      print_gimple_stmt (dump_file, newstmt, 0, 0);
	      fprintf (dump_file, "\n");
	    }
	  gsi_insert_before (gsi, newstmt, GSI_SAME_STMT);
	  remove = true;
	}
      return remove;
    }

  return remove;
}

/* Rewrite function call statement STMT.  Return TRUE if the statement
   is to be removed. */

bool
ipa_struct_reorg::rewrite_call (gcall *stmt, gimple_stmt_iterator *gsi)
{
  /* Handled allocation calls are handled seperately from normal
     function calls. */
  if (handled_allocation_stmt (stmt))
    {
      tree lhs = gimple_call_lhs (stmt);
      tree newrhs1[max_split];
      srdecl *decl = find_decl (lhs);
      if (!decl || !decl->type)
	return false;
      srtype *type = decl->type;
      tree num = allocate_size (type, stmt);
      gcc_assert (num);
      memset (newrhs1, 0, sizeof(newrhs1));

      /* The realloc call needs to have its first argument rewritten. */
      if (gimple_call_builtin_p (stmt, BUILT_IN_REALLOC))
	{
	  tree rhs1 = gimple_call_arg (stmt, 0);
	  if (integer_zerop (rhs1))
	    {
	      for (unsigned i = 0; i < max_split; i++)
		newrhs1[i] = rhs1;
	    }
	  else if (!rewrite_expr (rhs1, newrhs1))
	    internal_error ("rewrite failed for realloc");
	}

      /* Go through each new lhs.  */
      for (unsigned i = 0; i < max_split && decl->newdecl[i]; i++)
	{
	  tree newsize = TYPE_SIZE_UNIT (type->type);
	  gimple *g;
	  /* Every allocation except for calloc needs the size multiplied out. */
	  if (!gimple_call_builtin_p (stmt, BUILT_IN_CALLOC))
	    newsize = gimplify_build2 (gsi, MULT_EXPR, sizetype, num, newsize);

	  if (gimple_call_builtin_p (stmt, BUILT_IN_MALLOC)
	      || gimple_call_builtin_p (stmt, BUILT_IN_ALLOCA))
	    g = gimple_build_call (gimple_call_fndecl (stmt),
				   1, newsize);
	  else if (gimple_call_builtin_p (stmt, BUILT_IN_CALLOC))
	    g = gimple_build_call (gimple_call_fndecl (stmt),
				   2, num, newsize);
	  else if (gimple_call_builtin_p (stmt, BUILT_IN_REALLOC))
	    g = gimple_build_call (gimple_call_fndecl (stmt),
				   2, newrhs1[i], newsize);
	  else
	    gcc_assert (false);
	  gimple_call_set_lhs (g, decl->newdecl[i]);
	  gsi_insert_before (gsi, g, GSI_SAME_STMT);
	}
      return true;
    }

  /* The function call free needs to be handled special. */
  if (gimple_call_builtin_p (stmt, BUILT_IN_FREE))
    {
      tree expr = gimple_call_arg (stmt, 0);
      tree newexpr[max_split];
      if (!rewrite_expr (expr, newexpr))
	return false;

      if (newexpr[1] == NULL)
	{
	  gimple_call_set_arg (stmt, 0, newexpr[0]);
	  update_stmt (stmt);
	  return false;
	}

      for (unsigned i = 0; i < max_split && newexpr[i]; i++)
	{
	  gimple *g = gimple_build_call (gimple_call_fndecl (stmt),
					 1, newexpr[i]);
	  gsi_insert_before (gsi, g, GSI_SAME_STMT);
	}
      return true;
    }

  /* Otherwise, look up the function to see if we have cloned it
     and rewrite the arguments. */
  tree fndecl = gimple_call_fndecl (stmt);

  /* Indirect calls are already marked as escaping so ignore.  */
  if (!fndecl)
    return false;

  cgraph_node *node = cgraph_node::get (fndecl);
  gcc_assert (node);
  srfunction *f = find_function (node);

  /* Did not find the function or had not cloned it return saying don't
     change the function call. */
  if (!f || !f->newf)
    return false;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Changing arguments for function call :\n");
      print_gimple_expr (dump_file, stmt, 0, 0);
      fprintf (dump_file, "\n");
    }

  /* Move over to the new function. */
  f = f->newf;

  tree chain = gimple_call_chain (stmt);
  unsigned nargs = gimple_call_num_args (stmt);
  auto_vec<tree> vargs (nargs);

  if (chain)
    {
      tree newchains[max_split];
      if (rewrite_expr (chain, newchains))
	{
	  /* Chain decl's type cannot be split and but it can change. */
	  gcc_assert (newchains[1] == NULL);
	  chain = newchains[0];
	}
    }

  for (unsigned i = 0; i < nargs; i++)
    vargs.quick_push (gimple_call_arg (stmt, i));

  int extraargs = 0;

  for (unsigned i = 0; i < f->args.length (); i++)
    {
      srdecl *d = f->args[i];
      if (d->argumentnum == -2)
	continue;
      gcc_assert (d->argumentnum != -1);
      tree arg = vargs[d->argumentnum + extraargs];
      tree newargs[max_split];
      if (!rewrite_expr (arg, newargs))
	continue;

      /* If this ARG has a replacement handle the replacement.  */
      for (unsigned j = 0; j < max_split && d->newdecl[j]; j++)
	{
	  gcc_assert (newargs[j]);
	  /* If this is the first replacement of the arugment,
	     then just replace it.  */
	  if (j == 0)
	    vargs[d->argumentnum + extraargs] = newargs[j];
	  else
	    {
	      /* More than one replacement, we need to insert into the array. */
	      extraargs++;
	      vargs.safe_insert(d->argumentnum + extraargs, newargs[j]);
	    }
	}
    }

  gcall *new_stmt;

  new_stmt = gimple_build_call_vec (f->node->decl, vargs);

  if (gimple_call_lhs (stmt))
    gimple_call_set_lhs (new_stmt, gimple_call_lhs (stmt));

  gimple_set_vuse (new_stmt, gimple_vuse (stmt));
  gimple_set_vdef (new_stmt, gimple_vdef (stmt));

  if (gimple_has_location (stmt))
    gimple_set_location (new_stmt, gimple_location (stmt));
  gimple_call_copy_flags (new_stmt, stmt);
  gimple_call_set_chain (new_stmt, chain);

  gimple_set_modified (new_stmt, true);

  if (gimple_vdef (new_stmt)
      && TREE_CODE (gimple_vdef (new_stmt)) == SSA_NAME)
    SSA_NAME_DEF_STMT (gimple_vdef (new_stmt)) = new_stmt;

  gsi_replace (gsi, new_stmt, false);

  /* We need to defer cleaning EH info on the new statement to
     fixup-cfg.  We may not have dominator information at this point
     and thus would end up with unreachable blocks and have no way
     to communicate that we need to run CFG cleanup then.  */
  int lp_nr = lookup_stmt_eh_lp (stmt);
  if (lp_nr != 0)
    {
      remove_stmt_from_eh_lp (stmt);
      add_stmt_to_eh_lp (new_stmt, lp_nr);
    }


  return false;
}

/* Rewrite the conditional statement STMT.  Return TRUE if the
   old statement is to be removed. */

bool
ipa_struct_reorg::rewrite_cond (gcond *stmt, gimple_stmt_iterator *gsi)
{
  tree_code rhs_code = gimple_cond_code (stmt);

  /* Handle only equals or not equals conditionals. */
  if (rhs_code != EQ_EXPR
      && rhs_code != NE_EXPR)
    return false;
  tree rhs1 = gimple_cond_lhs (stmt);
  tree rhs2 = gimple_cond_rhs (stmt);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "COND: Rewriting\n");
      print_gimple_stmt (dump_file, stmt, 0, 0);
      fprintf (dump_file, "\n");
      print_generic_expr (dump_file, rhs1, 0);
      fprintf (dump_file, "\n");
      print_generic_expr (dump_file, rhs2, 0);
      fprintf (dump_file, "\n");
    }

  tree newrhs1[max_split];
  tree newrhs2[max_split];
  tree_code code = rhs_code == EQ_EXPR ? BIT_AND_EXPR : BIT_IOR_EXPR;
  if (!rewrite_lhs_rhs (rhs1, rhs2, newrhs1, newrhs2))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
        fprintf (dump_file, "\nDid nothing to statement.\n");
      return false;
    }

  tree newexpr = NULL_TREE;
  for (unsigned i = 0; i < max_split && newrhs1[i]; i++)
    {
      tree expr = gimplify_build2 (gsi, rhs_code, boolean_type_node, newrhs1[i], newrhs2[i]);
      if (!newexpr)
	newexpr = expr;
      else
	newexpr = gimplify_build2 (gsi, code, boolean_type_node, newexpr, expr);
     }

  if (newexpr)
    {
      gimple_cond_set_lhs (stmt, newexpr);
      gimple_cond_set_rhs (stmt, boolean_true_node);
      update_stmt (stmt);
    }
  return false;
}

/* Rewrite debug statments if possible.  Return TRUE if the statement
   should be removed. */

bool
ipa_struct_reorg::rewrite_debug (gimple *stmt, gimple_stmt_iterator *)
{
  bool remove = false;
  if (gimple_debug_bind_p (stmt))
    {
      tree var = gimple_debug_bind_get_var (stmt);
      tree newvar[max_split];
      if (rewrite_expr (var, newvar, true))
	remove = true;
      if (gimple_debug_bind_has_value_p (stmt))
	{
          var = gimple_debug_bind_get_value (stmt);
	  if (TREE_CODE (var) == POINTER_PLUS_EXPR)
	    var = TREE_OPERAND (var, 0);
	  if (rewrite_expr (var, newvar, true))
	    remove = true;
	}
    }
  else if (gimple_debug_source_bind_p (stmt))
    {
      tree var = gimple_debug_source_bind_get_var (stmt);
      tree newvar[max_split];
      if (rewrite_expr (var, newvar, true))
	remove = true;
      var = gimple_debug_source_bind_get_value (stmt);
      if (TREE_CODE (var) == POINTER_PLUS_EXPR)
	var = TREE_OPERAND (var, 0);
      if (rewrite_expr (var, newvar, true))
	remove = true;
    }

  return remove;
}

/* Rewrite PHI nodes, return true if the PHI was replaced. */

bool
ipa_struct_reorg::rewrite_phi (gphi *phi)
{
  tree newlhs[max_split];
  gphi *newphi[max_split];
  tree result = gimple_phi_result (phi);
  gphi_iterator gsi;

  memset(newphi, 0, sizeof(newphi));

  if (!rewrite_expr (result, newlhs))
    return false;

  if (newlhs[0] == NULL)
    return false;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nrewriting PHI:");
      print_gimple_stmt (dump_file, phi, 0, 0);
    }

  for (unsigned i = 0; i < max_split && newlhs[i]; i++)
    newphi[i] = create_phi_node (newlhs[i], gimple_bb (phi));

  for(unsigned i = 0; i  < gimple_phi_num_args (phi); i++)
    {
      tree newrhs[max_split];
      phi_arg_d rhs = *gimple_phi_arg (phi, i);
      rewrite_expr (rhs.def, newrhs);
      for (unsigned j = 0; j < max_split && newlhs[j]; j++)
	{
	  SET_PHI_ARG_DEF (newphi[j], i, newrhs[j]);
	  gimple_phi_arg_set_location (newphi[j], i, rhs.locus);
	  update_stmt (newphi[j]);
	}
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\ninto\n:");
      for (unsigned i = 0; i < max_split && newlhs[i]; i++)
	{
	  print_gimple_stmt (dump_file, newphi[i], 0, 0);
	  fprintf (dump_file, "\n");
	}
    }

  gsi = gsi_for_phi (phi);
  remove_phi_node (&gsi, false);

  return true;
}

/* Rewrite gimple statement STMT, return true if the STATEMENT
   is to be removed. */

bool
ipa_struct_reorg::rewrite_stmt (gimple *stmt, gimple_stmt_iterator *gsi)
{
  switch (gimple_code (stmt))
    {
    case GIMPLE_ASSIGN:
      return rewrite_assign (as_a <gassign *> (stmt), gsi);
    case GIMPLE_CALL:
      return rewrite_call (as_a <gcall *> (stmt), gsi);
    case GIMPLE_COND:
      return rewrite_cond (as_a <gcond *> (stmt), gsi);
      break;
    case GIMPLE_GOTO:
    case GIMPLE_SWITCH:
      break;
    case GIMPLE_DEBUG:
    case GIMPLE_ASM:
      break;
    default:
      break;
    }
  return false;
}

/* Does the function F uses any decl which has changed. */

bool
ipa_struct_reorg::has_rewritten_type (srfunction *f)
{
  for (unsigned i = 0; i < f->decls.length (); i++)
    {
      srdecl *d = f->decls[i];
      if (d->newdecl[0] != d->decl)
	return true;
    }

  for (unsigned i = 0; i < f->globals.length (); i++)
    {
      srdecl *d = f->globals[i];
      if (d->newdecl[0] != d->decl)
	return true;
    }
  return false;

}

/* Rewrite the functions if needed, return
   the TODOs requested.  */

unsigned
ipa_struct_reorg::rewrite_functions (void)
{
  unsigned retval = 0;


  /* Create new types, if we did not create any new types,
     then don't rewrite any accesses. */
  if (!create_new_types ())
    return 0;

  if (functions.length ())
    {
      retval = TODO_remove_functions;
      create_new_functions ();
    }

  create_new_decls ();

  for (unsigned i = 0; i < functions.length (); i++)
    {
      srfunction *f = functions[i];
      if (f->newnode)
	continue;

      /* Function uses no rewriten types so don't cause a rewrite. */
      if (!has_rewritten_type (f))
	continue;

      cgraph_node *node = f->node;
      basic_block bb;

      push_cfun (DECL_STRUCT_FUNCTION (node->decl));
      current_function = f;

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\nBefore rewrite:\n");
          dump_function_to_file (current_function_decl, dump_file, dump_flags | TDF_VOPS);
	}
      FOR_EACH_BB_FN (bb, cfun)
	{
	  for (gphi_iterator si = gsi_start_phis (bb); !gsi_end_p (si); )
	    {
	      if (rewrite_phi (si.phi ()))
		si = gsi_start_phis (bb);
	      else
		gsi_next (&si);
	    }

	  for (gimple_stmt_iterator si = gsi_start_bb (bb); !gsi_end_p (si); )
	    {
	      gimple *stmt = gsi_stmt (si);
	      if (rewrite_stmt (stmt, &si))
		gsi_remove (&si, true);
	      else
		gsi_next (&si);
	    }
        }

      /* Debug statements need to happen after all other statements
	 have changed. */
      FOR_EACH_BB_FN (bb, cfun)
	{
	  for (gimple_stmt_iterator si = gsi_start_bb (bb); !gsi_end_p (si); )
	    {
	      gimple *stmt = gsi_stmt (si);
	      if (gimple_code (stmt) == GIMPLE_DEBUG
		  && rewrite_debug (stmt, &si))
		gsi_remove (&si, true);
	      else
		gsi_next (&si);
	    }
	}

      /* Release the old SSA_NAMES for old arguments.  */
      if (f->old)
	{
	  for (unsigned i = 0; i < f->args.length (); i++)
	    {
	      srdecl *d = f->args[i];
	      if (d->newdecl[0] != d->decl)
		{
		  tree ssa_name = ssa_default_def (cfun, d->decl);
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    {
		      fprintf (dump_file, "Found ");
		      print_generic_expr (dump_file, ssa_name, 0);
		      fprintf (dump_file, " to be released.\n");
		    }
		  release_ssa_name (ssa_name);
		}
	    }
	}

      update_ssa (TODO_update_ssa_only_virtuals);

      if (flag_tree_pta)
	compute_may_aliases ();

      remove_unused_locals ();

      cgraph_edge::rebuild_edges ();

      free_dominance_info (CDI_DOMINATORS);

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\nAfter rewrite:\n");
          dump_function_to_file (current_function_decl, dump_file, dump_flags | TDF_VOPS);
	}

      pop_cfun ();
      current_function = NULL;
    }

  return retval | TODO_verify_all;
}

unsigned int
ipa_struct_reorg::execute (void)
{
  /* FIXME: If there is a top-level inline-asm, the pass immediately returns. */
  if (symtab->first_asm_symbol ())
    return 0;
  record_accesses ();
  prune_escaped_types ();
  analyze_types ();

  return rewrite_functions ();
}

const pass_data pass_data_ipa_struct_reorg =
{
  SIMPLE_IPA_PASS, /* type */
  "struct_reorg", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_IPA_STRUCT_REORG, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_ipa_struct_reorg : public simple_ipa_opt_pass
{
public:
  pass_ipa_struct_reorg (gcc::context *ctxt)
    : simple_ipa_opt_pass (pass_data_ipa_struct_reorg, ctxt)
  {}

  /* opt_pass methods: */
  virtual bool gate (function *);
  virtual unsigned int execute (function *) { return ipa_struct_reorg ().execute(); }

}; // class pass_ipa_struct_reorg

bool
pass_ipa_struct_reorg::gate (function *)
{
  return (optimize
	  && flag_ipa_struct_reorg
	  /* Don't bother doing anything if the program has errors.  */
	  && !seen_error ());
}

} // anon namespace

simple_ipa_opt_pass *
make_pass_ipa_struct_reorg (gcc::context *ctxt)
{
  return new pass_ipa_struct_reorg (ctxt);
}
