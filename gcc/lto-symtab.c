/* LTO symbol table.
   Copyright 2009 Free Software Foundation, Inc.
   Contributed by CodeSourcery, Inc.

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
#include "toplev.h"
#include "tree.h"
#include "gimple.h"
#include "ggc.h"	/* lambda.h needs this */
#include "lambda.h"	/* gcd */
#include "hashtab.h"
#include "plugin-api.h"
#include "lto-streamer.h"

/* Vector to keep track of external variables we've seen so far.  */
VEC(tree,gc) *lto_global_var_decls;

/* Base type for resolution map. It maps NODE to resolution.  */

struct GTY(()) lto_symtab_base_def
{
  /* Key is either an IDENTIFIER or a DECL.  */
  tree node;
};
typedef struct lto_symtab_base_def *lto_symtab_base_t;

struct GTY(()) lto_symtab_identifier_def
{
  struct lto_symtab_base_def base;
  tree decl;
};
typedef struct lto_symtab_identifier_def *lto_symtab_identifier_t;

struct GTY(()) lto_symtab_decl_def
{
  struct lto_symtab_base_def base;
  enum ld_plugin_symbol_resolution resolution;
  struct lto_file_decl_data * GTY((skip (""))) file_data;
};
typedef struct lto_symtab_decl_def *lto_symtab_decl_t;

/* A poor man's symbol table. This hashes identifier to prevailing DECL
   if there is one. */

static GTY ((if_marked ("lto_symtab_identifier_marked_p"),
	     param_is (struct lto_symtab_identifier_def)))
  htab_t lto_symtab_identifiers;

static GTY ((if_marked ("lto_symtab_decl_marked_p"),
	     param_is (struct lto_symtab_decl_def)))
  htab_t lto_symtab_decls;

/* Return the hash value of an lto_symtab_base_t object pointed to by P.  */

static hashval_t
lto_symtab_base_hash (const void *p)
{
  const struct lto_symtab_base_def *base =
    (const struct lto_symtab_base_def*) p;
  return htab_hash_pointer (base->node);
}

/* Return non-zero if P1 and P2 points to lto_symtab_base_def structs
   corresponding to the same tree node.  */

static int
lto_symtab_base_eq (const void *p1, const void *p2)
{
  const struct lto_symtab_base_def *base1 =
     (const struct lto_symtab_base_def *) p1;
  const struct lto_symtab_base_def *base2 =
     (const struct lto_symtab_base_def *) p2;
  return (base1->node == base2->node);
}

/* Returns non-zero if P points to an lto_symtab_base_def struct that needs
   to be marked for GC.  */ 

static int
lto_symtab_base_marked_p (const void *p)
{
  const struct lto_symtab_base_def *base =
     (const struct lto_symtab_base_def *) p;

  /* Keep this only if the key node is marked.  */
  return ggc_marked_p (base->node);
}

/* Returns non-zero if P points to an lto_symtab_identifier_def struct that
   needs to be marked for GC.  */ 

static int
lto_symtab_identifier_marked_p (const void *p)
{
  return lto_symtab_base_marked_p (p);
}

/* Returns non-zero if P points to an lto_symtab_decl_def struct that needs
   to be marked for GC.  */ 

static int
lto_symtab_decl_marked_p (const void *p)
{
  return lto_symtab_base_marked_p (p);
}

#define lto_symtab_identifier_eq	lto_symtab_base_eq
#define lto_symtab_identifier_hash	lto_symtab_base_hash
#define lto_symtab_decl_eq		lto_symtab_base_eq
#define lto_symtab_decl_hash		lto_symtab_base_hash

/* Lazily initialize resolution hash tables.  */

static void
lto_symtab_maybe_init_hash_tables (void)
{
  if (!lto_symtab_identifiers)
    {
      lto_symtab_identifiers =
	htab_create_ggc (1021, lto_symtab_identifier_hash,
			 lto_symtab_identifier_eq, NULL);
      lto_symtab_decls =
	htab_create_ggc (1021, lto_symtab_decl_hash,
			 lto_symtab_decl_eq, NULL);
    }
}

/* Transfer qualifiers between TYPE_1 and TYPE_2 so that qualifiers
   for both types are conservatively correct with respect to
   optimization done before the merge.  */

static void
lto_merge_qualifiers (tree type_1, tree type_2)
{
  /* If one is volatile, the other should also be.  */
  if (TYPE_VOLATILE (type_2))
    TYPE_VOLATILE (type_1) = 1;
  else if (TYPE_VOLATILE (type_1))
    TYPE_VOLATILE (type_2) = 1;

  /* If one type is writable, the other should also be.  */
  if (!TYPE_READONLY (type_2))
    TYPE_READONLY (type_1) = 0;
  else if (!TYPE_READONLY (type_1))
    TYPE_READONLY (type_2) = 0;

  /* If one type does not have the restrict qualifier, the other
     should not have it either.  */
  if (!TYPE_RESTRICT (type_2))
    TYPE_RESTRICT (type_1) = 0;
  else if (!TYPE_RESTRICT (type_1))
    TYPE_RESTRICT (type_2) = 0;
}

/* If TYPE_1 and TYPE_2 can be merged to form a common type, do it.
   Specifically, if they are both array types that have the same element
   type and one of them is a complete array type and the other isn't,
   return the complete array type.  Otherwise return NULL_TREE. */

static tree
lto_merge_types (tree type_1, tree type_2)
{
  if (TREE_CODE (type_1) == ARRAY_TYPE
      && TREE_CODE (type_2) == ARRAY_TYPE
      && !TYPE_ATTRIBUTES (type_1)
      && !TYPE_ATTRIBUTES (type_2)
      && gimple_types_compatible_p (TREE_TYPE (type_1), TREE_TYPE (type_2)))
    {
      lto_merge_qualifiers (type_1, type_2);

      if (COMPLETE_TYPE_P (type_1) && !COMPLETE_TYPE_P (type_2))
	return type_1;
      else if (COMPLETE_TYPE_P (type_2) && !COMPLETE_TYPE_P (type_1))
	return type_2;
      else
	return type_1;
    }

  return NULL_TREE;
}

/* Returns true iff the union of ATTRIBUTES_1 and ATTRIBUTES_2 can be
   applied to DECL.  */
static bool
lto_compatible_attributes_p (tree decl ATTRIBUTE_UNUSED, 
			     tree attributes_1, 
			     tree attributes_2)
{
#if 0
  /* ??? For now, assume two attribute sets are compatible only if they
     are both empty.  */
  return !attributes_1 && !attributes_2;
#else
  /* FIXME.  For the moment, live dangerously, and assume the user knows
     what he's doing. I don't think the linker would distinguish these cases.  */
  return true || (!attributes_1 && !attributes_2);
#endif
}

/* Helper for lto_symtab_compatible. Return TRUE if DECL is an external
   variable declaration of an aggregate type. */

static bool
external_aggregate_decl_p (tree decl)
{
  return (TREE_CODE (decl) == VAR_DECL
	  && DECL_EXTERNAL (decl)
	  && AGGREGATE_TYPE_P (TREE_TYPE (decl)));
}

/* Check if OLD_DECL and NEW_DECL are compatible. */

static bool
lto_symtab_compatible (tree old_decl, tree new_decl)
{
  tree merged_type = NULL_TREE;
  tree merged_result = NULL_TREE;

  if (TREE_CODE (old_decl) != TREE_CODE (new_decl))
    {
      switch (TREE_CODE (new_decl))
	{
	case VAR_DECL:
	  gcc_assert (TREE_CODE (old_decl) == FUNCTION_DECL);
	  error_at (DECL_SOURCE_LOCATION (new_decl),
		    "function %qD redeclared as variable", new_decl);
	  inform (DECL_SOURCE_LOCATION (old_decl),
		  "previously declared here");
	  return false;

	case FUNCTION_DECL:
	  gcc_assert (TREE_CODE (old_decl) == VAR_DECL);
	  error_at (DECL_SOURCE_LOCATION (new_decl),
		    "variable %qD redeclared as function", new_decl);
	  inform (DECL_SOURCE_LOCATION (old_decl),
		  "previously declared here");
	  return false;

	default:
	  gcc_unreachable ();
	}
    }

  if (!gimple_types_compatible_p (TREE_TYPE (old_decl), TREE_TYPE (new_decl)))
    {
      /* Allow an array type with unspecified bounds to
	 be merged with an array type whose bounds are specified, so
	 as to allow "extern int i[];" in one file to be combined with
	 "int i[3];" in another.  */
      if (TREE_CODE (new_decl) == VAR_DECL)
	merged_type = lto_merge_types (TREE_TYPE (old_decl),
				       TREE_TYPE (new_decl));
      else if (TREE_CODE (new_decl) == FUNCTION_DECL)
	{
	  if (!merged_type
	      /* We want either of the types to have argument types,
		 but not both.  */
	      && ((TYPE_ARG_TYPES (TREE_TYPE (old_decl)) != NULL)
		  ^ (TYPE_ARG_TYPES (TREE_TYPE (new_decl)) != NULL)))
	    {
	      /* The situation here is that (in C) somebody was smart
		 enough to use proper declarations in a header file, but
		 the actual definition of the function uses
		 non-ANSI-style argument lists.  Or we have a situation
		 where declarations weren't used anywhere and we're
		 merging the actual definition with a use.  One of the
		 decls will then have a complete function type, whereas
		 the other will only have a result type.  Assume that
		 the more complete type is the right one and don't
		 complain.  */
	      if (TYPE_ARG_TYPES (TREE_TYPE (old_decl)))
		{
		  merged_type = TREE_TYPE (old_decl);
		  merged_result = DECL_RESULT (old_decl);
		}
	      else
		{
		  merged_type = TREE_TYPE (new_decl);
		  merged_result = DECL_RESULT (new_decl);
		}
	    }

	  /* If we don't have a merged type yet...sigh.  The linker
	     wouldn't complain if the types were mismatched, so we
	     probably shouldn't either.  Just use the type from
	     whichever decl appears to be associated with the
	     definition.  If for some odd reason neither decl is, the
	     older one wins.  */
	  if (!merged_type)
	    {
	      if (!DECL_EXTERNAL (new_decl))
		{
		  merged_type = TREE_TYPE (new_decl);
		  merged_result = DECL_RESULT (new_decl);
		}
	      else
		{
		  merged_type = TREE_TYPE (old_decl);
		  merged_result = DECL_RESULT (old_decl);
		}
	    }
	}

      if (!merged_type)
	{
	  error_at (DECL_SOURCE_LOCATION (new_decl),
		    "type of %qD does not match original declaration",
		    new_decl);
	  inform (DECL_SOURCE_LOCATION (old_decl),
		  "previously declared here");
	  return false;
	}
    }

  if (DECL_UNSIGNED (old_decl) != DECL_UNSIGNED (new_decl))
    {
      error_at (DECL_SOURCE_LOCATION (new_decl),
		"signedness of %qD does not match original declaration",
		new_decl);
      inform (DECL_SOURCE_LOCATION (old_decl), "previously declared here");
      return false;
    }

  if (!tree_int_cst_equal (DECL_SIZE (old_decl),
			   DECL_SIZE (new_decl))
      || !tree_int_cst_equal (DECL_SIZE_UNIT (old_decl),
			      DECL_SIZE_UNIT (new_decl)))
    {
      /* Permit cases where we are declaring aggregates and at least one
	 of the decls is external and one of the decls has a size whereas
	 the other one does not.  This is perfectly legal in C:

         struct s;
	 extern struct s x;

	 void*
	 f (void)
	 {
	   return &x;
	 }

	 There is no way a compiler can tell the size of x.  So we cannot
	 assume that external aggreates have complete types.  */

      if (!((TREE_CODE (TREE_TYPE (old_decl))
	     == TREE_CODE (TREE_TYPE (new_decl)))
	    && ((external_aggregate_decl_p (old_decl)
		 && DECL_SIZE (old_decl) == NULL_TREE)
		|| (external_aggregate_decl_p (new_decl)
		    && DECL_SIZE (new_decl) == NULL_TREE))))
	{
	  error_at (DECL_SOURCE_LOCATION (new_decl),
		    "size of %qD does not match original declaration",
		    new_decl);
	  inform (DECL_SOURCE_LOCATION (old_decl),
		  "previously declared here");
	  return false;
	}
    }

  /* Report an error if user-specified alignments do not match.  */
  if ((DECL_USER_ALIGN (old_decl) && DECL_USER_ALIGN (new_decl))
      && DECL_ALIGN (old_decl) != DECL_ALIGN (new_decl))
    {
      error_at (DECL_SOURCE_LOCATION (new_decl),
		"alignment of %qD does not match original declaration",
		new_decl);
      inform (DECL_SOURCE_LOCATION (old_decl), "previously declared here");
      return false;
    }

  if (DECL_MODE (old_decl) != DECL_MODE (new_decl))
    {
      /* We can arrive here when we are merging 'extern char foo[]' and
	 'char foo[SMALLNUM]'; the former is probably BLKmode and the
	 latter is not.  In such a case, we should have merged the types
	 already; detect it and don't complain.  We also need to handle
	 external aggregate declaration specially.  */
      if ((TREE_CODE (TREE_TYPE (old_decl))
	   == TREE_CODE (TREE_TYPE (new_decl)))
	  && (((TREE_CODE (TREE_TYPE (old_decl)) != ARRAY_TYPE)
	       && ((external_aggregate_decl_p (old_decl)
		    && DECL_MODE (old_decl) == VOIDmode)
		   || (external_aggregate_decl_p (new_decl)
		       && DECL_MODE (new_decl) == VOIDmode)))
	      || ((TREE_CODE (TREE_TYPE (old_decl)) == ARRAY_TYPE)
		  && merged_type)))
	;
      else
	{
	  error_at (DECL_SOURCE_LOCATION (new_decl),
		    "machine mode of %qD does not match original declaration",
		    new_decl);
	  inform (DECL_SOURCE_LOCATION (old_decl),
		  "previously declared here");
	  return false;
	}
    }

  if (!lto_compatible_attributes_p (old_decl,
				    DECL_ATTRIBUTES (old_decl),
				    DECL_ATTRIBUTES (new_decl)))
    {
      error_at (DECL_SOURCE_LOCATION (new_decl),
		"attributes applied to %qD are incompatible with original "
		"declaration", new_decl);
      inform (DECL_SOURCE_LOCATION (old_decl), "previously declared here");
      return false;
    }

  /* We do not require matches for:

     - DECL_NAME

       Only the name used in object files matters.

     - DECL_CONTEXT  

       An entity might be declared in a C++ namespace in one file and
       with a C identifier in another file.  

     - TREE_PRIVATE, TREE_PROTECTED

       Access control is the problem of the front end that created the
       object file.  
       
     Therefore, at this point we have decided to merge the declarations.  */
  return true;
}


/* Marks decl DECL as having resolution RESOLUTION. */

static void
lto_symtab_set_resolution_and_file_data (tree decl,
					 ld_plugin_symbol_resolution_t
					 resolution,
					 struct lto_file_decl_data *file_data)
{
  lto_symtab_decl_t new_entry;
  void **slot;

  gcc_assert (decl);

  gcc_assert (TREE_PUBLIC (decl));
  gcc_assert (TREE_CODE (decl) != FUNCTION_DECL || !DECL_ABSTRACT (decl));

  new_entry = GGC_CNEW (struct lto_symtab_decl_def);
  new_entry->base.node = decl;
  new_entry->resolution = resolution;
  new_entry->file_data = file_data;
  
  lto_symtab_maybe_init_hash_tables ();
  slot = htab_find_slot (lto_symtab_decls, new_entry, INSERT);
  gcc_assert (!*slot);
  *slot = new_entry;
}

/* Get the lto_symtab_identifier_def struct associated with ID
   if there is one.  If there is none and INSERT_P is true, create
   a new one.  */

static lto_symtab_identifier_t
lto_symtab_get_identifier (tree id, bool insert_p)
{
  struct lto_symtab_identifier_def temp;
  lto_symtab_identifier_t symtab_id;
  void **slot;

  lto_symtab_maybe_init_hash_tables ();
  temp.base.node = id;
  slot = htab_find_slot (lto_symtab_identifiers, &temp,
			 insert_p ? INSERT : NO_INSERT);
  if (insert_p)
    {
      if (*slot)
	return (lto_symtab_identifier_t) *slot;
      else
	{
	  symtab_id = GGC_CNEW (struct lto_symtab_identifier_def);
	  symtab_id->base.node = id;
	  *slot = symtab_id;
	  return symtab_id;
	}
    }
  else
    return slot ? (lto_symtab_identifier_t) *slot : NULL;
}

/* Return the DECL associated with an IDENTIFIER ID or return NULL_TREE
   if there is none.  */

static tree
lto_symtab_get_identifier_decl (tree id)
{
  lto_symtab_identifier_t symtab_id = lto_symtab_get_identifier (id, false);
  return symtab_id ? symtab_id->decl : NULL_TREE;
}

/* SET the associated DECL of an IDENTIFIER ID to be DECL.  */

static void
lto_symtab_set_identifier_decl (tree id, tree decl)
{
  lto_symtab_identifier_t symtab_id = lto_symtab_get_identifier (id, true);
  symtab_id->decl = decl;
}

/* Common helper function for merging variable and function declarations.
   NEW_DECL is the newly found decl. RESOLUTION is the decl's resolution
   provided by the linker. */

static void
lto_symtab_merge_decl (tree new_decl,
		       enum ld_plugin_symbol_resolution resolution,
		       struct lto_file_decl_data *file_data)
{
  tree old_decl;
  tree name;
  ld_plugin_symbol_resolution_t old_resolution;

  gcc_assert (TREE_CODE (new_decl) == VAR_DECL
	      || TREE_CODE (new_decl) == FUNCTION_DECL);

  gcc_assert (TREE_PUBLIC (new_decl));

  /* Check that declarations reaching this function do not have
     properties inconsistent with having external linkage.  If any of
     these asertions fail, then the object file reader has failed to
     detect these cases and issue appropriate error messages.  */
  if (TREE_CODE (new_decl) == VAR_DECL)
    gcc_assert (!(DECL_EXTERNAL (new_decl) && DECL_INITIAL (new_decl)));

  /* Remember the resolution of this symbol. */
  lto_symtab_set_resolution_and_file_data (new_decl, resolution, file_data);

  /* Ensure DECL_ASSEMBLER_NAME will not set assembler name.  */
  gcc_assert (DECL_ASSEMBLER_NAME_SET_P (new_decl));

  /* Retrieve the previous declaration.  */
  name = DECL_ASSEMBLER_NAME (new_decl);
  old_decl = lto_symtab_get_identifier_decl (name);

  /* If there was no previous declaration, then there is nothing to
     merge.  */
  if (!old_decl)
    {
      lto_symtab_set_identifier_decl (name, new_decl);
      VEC_safe_push (tree, gc, lto_global_var_decls, new_decl);
      return;
    }

  /* Give ODR violation errors.  */
  old_resolution = lto_symtab_get_resolution (old_decl);
  if (resolution == LDPR_PREVAILING_DEF
      || resolution == LDPR_PREVAILING_DEF_IRONLY)
    {
      if ((old_resolution == LDPR_PREVAILING_DEF
	   || old_resolution == LDPR_PREVAILING_DEF_IRONLY)
	  && (old_resolution != resolution || flag_no_common))
	{
	  error_at (DECL_SOURCE_LOCATION (new_decl),
		    "%qD has already been defined", new_decl);
	  inform (DECL_SOURCE_LOCATION (old_decl),
		  "previously defined here");
	  return;
	}
    }

  /* The linker may ask us to combine two incompatible symbols. */
  if (!lto_symtab_compatible (old_decl, new_decl))
    return;

  /* Merge decl state in both directions, we may still end up using
     the new decl.  */
  TREE_ADDRESSABLE (old_decl) |= TREE_ADDRESSABLE (new_decl);
  TREE_ADDRESSABLE (new_decl) |= TREE_ADDRESSABLE (old_decl);

  gcc_assert (resolution != LDPR_UNKNOWN
	      && resolution != LDPR_UNDEF
	      && old_resolution != LDPR_UNKNOWN
	      && old_resolution != LDPR_UNDEF);

  if (resolution == LDPR_PREVAILING_DEF
      || resolution == LDPR_PREVAILING_DEF_IRONLY)
    {
      gcc_assert (old_resolution == LDPR_PREEMPTED_IR
		  || old_resolution ==  LDPR_RESOLVED_IR
		  || (old_resolution == resolution && !flag_no_common));
      lto_symtab_set_identifier_decl (name, new_decl);
      return;
    }

  if (resolution == LDPR_PREEMPTED_REG
      || resolution == LDPR_RESOLVED_EXEC
      || resolution == LDPR_RESOLVED_DYN)
    gcc_assert (old_resolution == LDPR_PREEMPTED_REG
		|| old_resolution == LDPR_RESOLVED_EXEC
		|| old_resolution == LDPR_RESOLVED_DYN);

  if (resolution == LDPR_PREEMPTED_IR
      || resolution == LDPR_RESOLVED_IR)
    gcc_assert (old_resolution == LDPR_PREVAILING_DEF
		|| old_resolution == LDPR_PREVAILING_DEF_IRONLY
		|| old_resolution == LDPR_PREEMPTED_IR
		|| old_resolution == LDPR_RESOLVED_IR);

  return;
}


/* Merge the VAR_DECL NEW_VAR with resolution RESOLUTION with any previous
   declaration with the same name. */

void
lto_symtab_merge_var (tree new_var, enum ld_plugin_symbol_resolution resolution)
{
  lto_symtab_merge_decl (new_var, resolution, NULL);
}

/* Merge the FUNCTION_DECL NEW_FN with resolution RESOLUTION with any previous
   declaration with the same name. */

void
lto_symtab_merge_fn (tree new_fn, enum ld_plugin_symbol_resolution resolution,
		     struct lto_file_decl_data *file_data)
{
  lto_symtab_merge_decl (new_fn, resolution, file_data);
}

/* Given the decl DECL, return the prevailing decl with the same name. */

tree
lto_symtab_prevailing_decl (tree decl)
{
  tree ret;
  gcc_assert (decl);

  /* Builtins and local symbols are their own prevailing decl.  */
  if (!TREE_PUBLIC (decl) || DECL_IS_BUILTIN (decl))
    return decl;

  /* FIXME lto. There should be no DECL_ABSTRACT in the middle end. */
  if (TREE_CODE (decl) == FUNCTION_DECL && DECL_ABSTRACT (decl))
    return decl;

  /* Ensure DECL_ASSEMBLER_NAME will not set assembler name.  */
  gcc_assert (DECL_ASSEMBLER_NAME_SET_P (decl));

  ret = lto_symtab_get_identifier_decl (DECL_ASSEMBLER_NAME (decl));

  return ret;
}

/* Return the hash table entry of DECL. */

static struct lto_symtab_decl_def *
lto_symtab_get_symtab_def (tree decl)
{
  struct lto_symtab_decl_def temp, *symtab_decl;
  void **slot;

  gcc_assert (decl);

  lto_symtab_maybe_init_hash_tables ();
  temp.base.node = decl;
  slot = htab_find_slot (lto_symtab_decls, &temp, NO_INSERT);
  gcc_assert (slot && *slot);
  symtab_decl = (struct lto_symtab_decl_def*) *slot;
  return symtab_decl;
}

/* Return the resolution of DECL. */

enum ld_plugin_symbol_resolution
lto_symtab_get_resolution (tree decl)
{
  gcc_assert (decl);

  if (!TREE_PUBLIC (decl))
    return LDPR_PREVAILING_DEF_IRONLY;

  /* FIXME lto: There should be no DECL_ABSTRACT in the middle end. */
  if (TREE_CODE (decl) == FUNCTION_DECL && DECL_ABSTRACT (decl))
    return LDPR_PREVAILING_DEF_IRONLY;

  return lto_symtab_get_symtab_def (decl)->resolution;
}

/* Return the file of DECL. */

struct lto_file_decl_data *
lto_symtab_get_file_data (tree decl)
{
  return lto_symtab_get_symtab_def (decl)->file_data;
}

/* Remove any storage used to store resolution of DECL.  */

void
lto_symtab_clear_resolution (tree decl)
{
  struct lto_symtab_decl_def temp;
  gcc_assert (decl);

  if (!TREE_PUBLIC (decl))
    return;

  /* LTO FIXME: There should be no DECL_ABSTRACT in the middle end. */
  if (TREE_CODE (decl) == FUNCTION_DECL && DECL_ABSTRACT (decl))
    return;

  lto_symtab_maybe_init_hash_tables ();
  temp.base.node = decl;
  htab_remove_elt (lto_symtab_decls, &temp);
}

#include "gt-lto-symtab.h"
