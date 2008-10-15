/* LTO symbol table.
   Copyright 2006 Free Software Foundation, Inc.
   Contributed by CodeSourcery, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "toplev.h"
#include "tree.h"
#include "lto.h"
#include "gimple.h"
#include "lto-tree.h"
#include "lto-tree-in.h"
#include "ggc.h"	/* lambda.h needs this */
#include "lambda.h"	/* gcd */

/* Vector to keep track of external variables we've seen so far.  */
VEC(tree,gc) *lto_global_var_decls;

/* Returns true iff TYPE_1 and TYPE_2 are the same type.  */
static bool
lto_same_type_p (tree type_1, tree type_2)
{
  unsigned int code;

  /* Check first for the obvious case of pointer identity.  */
  if (type_1 == type_2)
    return true;

  /* Can't be the same type if the types don't have the same code.  */
  code = TREE_CODE (type_1);
  if (code != TREE_CODE (type_2))
    return false;

  /* Can't be the same type if they have different CV qualifiers.  */
  if (TYPE_QUALS (type_1) != TYPE_QUALS (type_2))
    return false;

  /* "If GNU attributes are present, types which could be the same be it not
     for their GNU attributes may in fact be different due to the use of GNU
     attributes."  Hmmm.  Punt on this for now and assume they're different
     if we see attributes on either type.  */
  if (TYPE_ATTRIBUTES (type_1) || TYPE_ATTRIBUTES (type_2))
    return false;

  switch (code)
    {
    case VOID_TYPE:
      /* Void types are the same in all translation units.  */
      return true;

    case INTEGER_TYPE:
    case BOOLEAN_TYPE:
      /* Corresponding integral types are the same.  */
      return (TYPE_PRECISION (type_1) == TYPE_PRECISION (type_2)
	      && TYPE_UNSIGNED (type_1) == TYPE_UNSIGNED (type_2)
	      && tree_int_cst_equal (TYPE_SIZE (type_1), TYPE_SIZE (type_2))
	      && TYPE_ALIGN (type_1) == TYPE_ALIGN (type_2)
	      && TYPE_STRING_FLAG (type_1) == TYPE_STRING_FLAG (type_2));
      
    case REAL_TYPE:
      /* Corresponding float types are the same.  */
      return (TYPE_PRECISION (type_1) == TYPE_PRECISION (type_2)
	      && tree_int_cst_equal (TYPE_SIZE (type_1), TYPE_SIZE (type_2))
	      && TYPE_ALIGN (type_1) == TYPE_ALIGN (type_2));

    case ARRAY_TYPE:
      /* Array types are the same if the element types are the same and
	 the number of elements are the same.  */
      if (!lto_same_type_p (TREE_TYPE (type_1), TREE_TYPE (type_2))
	  || TYPE_STRING_FLAG (type_1) != TYPE_STRING_FLAG (type_2))
	return false;
      else
	{
	  tree index_1 = TYPE_DOMAIN (type_1);
	  tree index_2 = TYPE_DOMAIN (type_2);
	  if (!index_1 || !index_2)
	    return false;
	  else
	    {
	      tree min_1 = TYPE_MIN_VALUE (index_1);
	      tree min_2 = TYPE_MIN_VALUE (index_2);
	      tree max_1 = TYPE_MAX_VALUE (index_1);
	      tree max_2 = TYPE_MAX_VALUE (index_2);
	      /* If the array types both have unspecified bounds, then
		 MAX_{1,2} will be NULL_TREE.  */
	      if (min_1 && min_2 && !max_1 && !max_2)
		return (integer_zerop (min_1)
			&& integer_zerop (min_2));
	      /* Otherwise, we need the bounds to be fully
		 specified.  */
	      if (!min_1 || !min_2 || !max_1 || !max_2)
		return false;
	      if (TREE_CODE (min_1) != INTEGER_CST
		  || TREE_CODE (min_2) != INTEGER_CST
		  || TREE_CODE (max_1) != INTEGER_CST
		  || TREE_CODE (max_2) != INTEGER_CST)
		return false;
	      if (tree_int_cst_equal (min_1, min_2))
		return tree_int_cst_equal (max_1, max_2);
	      else
		{
		  tree nelts_1 = array_type_nelts (type_1);
		  tree nelts_2 = array_type_nelts (type_2);
		  if (! nelts_1 || ! nelts_2)
		    return false;
		  if (TREE_CODE (nelts_1) != INTEGER_CST
		      || TREE_CODE (nelts_2) != INTEGER_CST)
		    return false;
		  return tree_int_cst_equal (nelts_1, nelts_2);
		}
	    }
	}

    case FUNCTION_TYPE:
      /* Function types are the same if the return type and arguments types
	 are the same.  */
      if (!lto_same_type_p (TREE_TYPE (type_1), TREE_TYPE (type_2)))
	return false;
      else
	{
	  tree parms_1 = TYPE_ARG_TYPES (type_1);
	  tree parms_2 = TYPE_ARG_TYPES (type_2);
	  if (parms_1 == parms_2)
	    return true;
	  else
	    {
	      while (parms_1 && parms_2)
		{
		  if (!lto_same_type_p (TREE_VALUE (parms_1),
					TREE_VALUE (parms_2)))
		    return false;
		  parms_1 = TREE_CHAIN (parms_1);
		  parms_2 = TREE_CHAIN (parms_2);
		}
	      return !parms_1 && !parms_2;
	    }
	}

    case POINTER_TYPE:
    case REFERENCE_TYPE:
      /* Pointer and reference types are the same if the pointed-to types are
	 the same.  */
      return lto_same_type_p (TREE_TYPE (type_1), TREE_TYPE (type_2));

    case ENUMERAL_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      /* Enumeration and class types are the same if they have the same
	 name.  */
      {
	tree name_1 = TYPE_NAME (type_1);
	tree name_2 = TYPE_NAME (type_2);
	if (!name_1 || !name_2)
	  /* Presumably, anonymous types are all unique.  */
	  return false;

	if (TREE_CODE (name_1) == TYPE_DECL)
	  {
	    name_1 = DECL_NAME (name_1);
	    if (! name_1)
	      return false;
	  }
	gcc_assert (TREE_CODE (name_1) == IDENTIFIER_NODE);

	if (TREE_CODE (name_2) == TYPE_DECL)
	  {
	    name_2 = DECL_NAME (name_2);
	    if (! name_2)
	      return false;
	  }
	gcc_assert (TREE_CODE (name_2) == IDENTIFIER_NODE);

	/* Identifiers can be compared with pointer equality rather
	   than a string comparison.  */
	return name_1 == name_2;
      }

      /* FIXME:  add pointer to member types.  */
    default:
      return false;
    }
}

/* If TYPE_1 and TYPE_2 can be merged to form a common type, do it.
   Specifically, if they are both array types that have the same element
   type and one of them is a complete array type and the other isn't,
   return the complete array type.  Otherwise return NULL_TREE. */
static tree
lto_merge_types (tree type_1, tree type_2)
{
  if (TREE_CODE (type_1) == ARRAY_TYPE
      && (TREE_CODE (type_2) == ARRAY_TYPE)
      && (TYPE_QUALS (type_1) == TYPE_QUALS (type_2))
      && !TYPE_ATTRIBUTES (type_1) && ! TYPE_ATTRIBUTES (type_2)
      && (lto_same_type_p (TREE_TYPE (type_1), TREE_TYPE (type_2))))
    {
      if (COMPLETE_TYPE_P (type_1) && !COMPLETE_TYPE_P (type_2))
	return type_1;
      else if (COMPLETE_TYPE_P (type_2) && !COMPLETE_TYPE_P (type_1))
	return type_2;
      else
	return NULL_TREE;
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

/* Compute the least common multiple of A and B.  */

static inline unsigned
lto_least_common_multiple (unsigned a, unsigned b)
{
  return (a * b) / gcd (a, b);
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
	  error ("function %qD redeclared as variable", old_decl);
	  return false;
	case FUNCTION_DECL:
	  gcc_assert (TREE_CODE (old_decl) == VAR_DECL);
	  error ("variable %qD redeclared as function", old_decl);
	  return false;
	default:
	  gcc_unreachable ();
	}
    }
  if (!lto_same_type_p (TREE_TYPE (old_decl), TREE_TYPE (new_decl)))
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
	  if (DECL_IS_BUILTIN (old_decl) || DECL_IS_BUILTIN (new_decl))
	    {
	      tree candidate = match_builtin_function_types (TREE_TYPE (new_decl),
							     TREE_TYPE (old_decl));
	      /* We don't really have source location information at this
		 point, so the above matching was a bit of a gamble.  */
	      if (candidate)
		merged_type = candidate;
	    }

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
	  error ("type of %qD does not match original declaration",
		 new_decl);
	  return false;
	}
    }
  if (DECL_UNSIGNED (old_decl) != DECL_UNSIGNED (new_decl))
    {
      error ("signedness of %qD does not match original declaration",
	     new_decl);
      return false;
    }
  if (!tree_int_cst_equal (DECL_SIZE (old_decl),
			   DECL_SIZE (new_decl))
      || !tree_int_cst_equal (DECL_SIZE_UNIT (old_decl),
			      DECL_SIZE_UNIT (new_decl)))
    {
      /* Permit cases where we are declaring arrays and at least one of
	 the decls is external and one of the decls has a size whereas
	 the other one does not.  */
      if (!((DECL_EXTERNAL (old_decl) || DECL_EXTERNAL (new_decl))
	    && ((DECL_SIZE (old_decl) == NULL_TREE
		 && DECL_SIZE (new_decl) != NULL_TREE)
		|| (DECL_SIZE (new_decl) == NULL_TREE
		    && DECL_SIZE (old_decl) != NULL_TREE))
	    && TREE_CODE (TREE_TYPE (old_decl)) == ARRAY_TYPE
	    && TREE_CODE (TREE_TYPE (new_decl)) == ARRAY_TYPE))
	{
	  error ("size of %qD does not match original declaration", 
		 new_decl);
	  return false;
	}
    }
  /* Report an error if user-specified alignments do not match.  */
  if ((DECL_USER_ALIGN (old_decl) && DECL_USER_ALIGN (new_decl))
      && DECL_ALIGN (old_decl) != DECL_ALIGN (new_decl))
    {
      error ("alignment of %qD does not match original declaration",
	     new_decl);
      return false;
    }
  if (DECL_MODE (old_decl) != DECL_MODE (new_decl))
    {
      /* We can arrive here when we are merging 'extern char foo[]' and
	 'char foo[SMALLNUM]'; the former is probably BLKmode and the
	 latter is not.  In such a case, we should have merged the types
	 already; detect it and don't complain.  */
      if (TREE_CODE (old_decl) == VAR_DECL
	  && TREE_CODE (TREE_TYPE (old_decl)) == ARRAY_TYPE
	  && TREE_CODE (TREE_TYPE (new_decl)) == ARRAY_TYPE
	  && merged_type)
	;
      else
	{
	  error ("machine mode of %qD does not match original declaration",
		 new_decl);
	  return false;
	}
    }
  if (!lto_compatible_attributes_p (old_decl,
				    DECL_ATTRIBUTES (old_decl),
				    DECL_ATTRIBUTES (new_decl)))
    {
      error ("attributes applied to %qD are incompatible with original "
	     "declaration", new_decl);
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
lto_symtab_set_resolution (tree decl, ld_plugin_symbol_resolution_t resolution)
{
  gcc_assert (decl);

  gcc_assert (TREE_PUBLIC (decl));
  gcc_assert (TREE_CODE (decl) != FUNCTION_DECL || !DECL_ABSTRACT (decl));

  gcc_assert (!DECL_LANG_SPECIFIC (decl));
  DECL_LANG_SPECIFIC (decl) = GGC_NEW (struct lang_decl);
  LTO_DECL_RESOLUTION (decl) = resolution;
}

/* Common helper function for merging variable and function declarations.
   NEW_DECL is the newly found decl. RESOLUTION is the decl's resolution
   provided by the linker. */

static void
lto_symtab_merge_decl (tree new_decl,
		       enum ld_plugin_symbol_resolution resolution)
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
  /* FIXME lto: The assertion below may fail incorrectly on a static
     class member.  The problem seems to be the (documented) fact
     that DECL_NONLOCAL may be set for class instance variables as
     well as for variables referenced from inner functions.  */
  /*gcc_assert (!DECL_NONLOCAL (new_decl));*/
  if (TREE_CODE (new_decl) == VAR_DECL)
    {
      gcc_assert (!DECL_REGISTER (new_decl));
      gcc_assert (!(DECL_EXTERNAL (new_decl) && DECL_INITIAL (new_decl)));
    }

  /* Remember the resolution of this symbol. */
  lto_symtab_set_resolution (new_decl, resolution);

  /* Retrieve the previous declaration.  */
  name = DECL_ASSEMBLER_NAME (new_decl);
  old_decl = LTO_IDENTIFIER_DECL (name);

  /* If there was no previous declaration, then there is nothing to
     merge.  */
  if (!old_decl)
    {
      LTO_IDENTIFIER_DECL (name) = new_decl;
      VEC_safe_push (tree, gc, lto_global_var_decls, new_decl);
      return;
    }

  /* The linker may ask us to combine two incompatible symbols. */
  if (!lto_symtab_compatible (old_decl, new_decl))
    return;

  old_resolution = lto_symtab_get_resolution (old_decl);
  gcc_assert (resolution != LDPR_UNKNOWN
	      && resolution != LDPR_UNDEF
	      && old_resolution != LDPR_UNKNOWN
	      && old_resolution != LDPR_UNDEF);

  if (resolution == LDPR_PREVAILING_DEF
      || resolution == LDPR_PREVAILING_DEF_IRONLY)
    {
      if (old_resolution == LDPR_PREVAILING_DEF
	  || old_resolution == LDPR_PREVAILING_DEF_IRONLY)
	{
	  error ("%qD has already been defined", new_decl);
	  return;
	}
      gcc_assert (old_resolution == LDPR_PREEMPTED_IR
		  || old_resolution ==  LDPR_RESOLVED_IR);
      LTO_IDENTIFIER_DECL (name) = new_decl;
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
  lto_symtab_merge_decl (new_var, resolution);
}

/* Merge the FUNCTION_DECL NEW_FN with resolution RESOLUTION with any previous
   declaration with the same name. */

void
lto_symtab_merge_fn (tree new_fn, enum ld_plugin_symbol_resolution resolution)
{
  lto_symtab_merge_decl (new_fn, resolution);
}

/* Given the decl DECL, return the prevailing decl with the same name. */

tree
lto_symtab_prevailing_decl (tree decl)
{
  tree ret;
  gcc_assert (decl);

  if (!TREE_PUBLIC (decl))
    return decl;

  /* FIXME lto. There should be no DECL_ABSTRACT in the middle end. */
  if (TREE_CODE (decl) == FUNCTION_DECL && DECL_ABSTRACT (decl))
    return decl;

  ret = LTO_IDENTIFIER_DECL (DECL_ASSEMBLER_NAME (decl));
  gcc_assert (ret);
  return ret;
}

/* Return the resolution of DECL. */

enum ld_plugin_symbol_resolution
lto_symtab_get_resolution (tree decl)
{
  gcc_assert (decl);

  if (!TREE_PUBLIC (decl))
    return LDPR_PREVAILING_DEF_IRONLY;

  /* LTO FIXME: There should be no DECL_ABSTRACT in the middle end. */
 if (TREE_CODE (decl) == FUNCTION_DECL && DECL_ABSTRACT (decl))
    return LDPR_PREVAILING_DEF_IRONLY;

 return LTO_DECL_RESOLUTION (decl);
}
