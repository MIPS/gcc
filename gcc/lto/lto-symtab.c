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
#include "lto-tree.h"

/* Returns true iff TYPE_1 and TYPE_2 are the same type.  */
static bool
lto_same_type_p (tree type_1, tree type_2)
{
  /* ??? For now, assume that type equality is pointer equality.  */
  return type_1 == type_2;
}

/* Returns true iff the union of ATTRIBUTES_1 and ATTRIBUTES_2 can be
   applied to DECL.  */
static bool
lto_compatible_attributes_p (tree decl ATTRIBUTE_UNUSED, 
			     tree attributes_1, 
			     tree attributes_2)
{
  /* ??? For now, assume two attribute sets are compatible only if they
     are both empty.  */
  return !attributes_1 && !attributes_2;
}

tree 
lto_symtab_merge_var (tree new_var) 
{ 
  tree old_var;
  tree name;

  gcc_assert (TREE_CODE (new_var) == VAR_DECL);
  /* Variables with internal linkage do not need to be merged.  */
  if (!TREE_PUBLIC (new_var))
    return new_var;
  /* Check that variables reaching this function do not have
     properties inconsistent with having external linkage.  If any of
     these asertions fail, then the object file reader has failed to
     detect these cases and issue appropriate error messages.  */
  gcc_assert (TREE_STATIC (new_var));
  gcc_assert (!DECL_NONLOCAL (new_var));
  gcc_assert (!DECL_REGISTER (new_var));
  gcc_assert (!(DECL_EXTERNAL (new_var) && DECL_INITIAL (new_var)));
  /* Retrieve the previous declaration.  */
  name = DECL_ASSEMBLER_NAME (new_var);
  old_var = LTO_IDENTIFIER_DECL (name);
  /* If there was no previous declaration, then there is nothing to
     merge.  */
  if (!old_var)
    {
      LTO_IDENTIFIER_DECL (name) = new_var;
      return new_var;
    }
  /* Check for inconsistencies.  */
  if (TREE_CODE (old_var) != VAR_DECL)
    {
      gcc_assert (TREE_CODE (old_var) == FUNCTION_DECL);
      error ("function %qD redeclared as variable", old_var);
      return error_mark_node;
    }
  if (!lto_same_type_p (TREE_TYPE (old_var), TREE_TYPE (new_var)))
    {
      /* ??? We should allow an array type with unspecified bounds to
	 be merged with an array type whose bounds are specified, so
	 as to allow "extern int i[];" in one file to be combined with
	 "int i[3];" in another.  */ 
      error ("type of %qD does not match original declaration",
	     new_var);
      return error_mark_node;
    }
  if (DECL_UNSIGNED (old_var) != DECL_UNSIGNED (new_var))
    {
      error ("signedness of %qD does not match original declaration",
	     new_var);
      return error_mark_node;
    }
  if (!tree_int_cst_equal (DECL_SIZE (old_var),
			   DECL_SIZE (new_var))
      || !tree_int_cst_equal (DECL_SIZE_UNIT (old_var),
			      DECL_SIZE_UNIT (new_var)))
    {
      error ("size of %qD does not match original declaration", 
	     new_var);
      return error_mark_node;
    }
  if (DECL_ALIGN (old_var) != DECL_ALIGN (new_var))
    {
      error ("alignment of %qD does not match original declaration",
	     new_var);
      return error_mark_node;
    }
  if (DECL_MODE (old_var) != DECL_MODE (new_var))
    {
      error ("machine mode of %qD does not match original declaration",
	     new_var);
      return error_mark_node;
    }
  if (!DECL_EXTERNAL (old_var) && !DECL_EXTERNAL (new_var))
    {
      /* ??? We should handle COMDAT here.  */
      error ("%qD has already been defined", new_var);
      return error_mark_node;
    }
  if (!lto_compatible_attributes_p (old_var,
				    DECL_ATTRIBUTES (old_var),
				    DECL_ATTRIBUTES (new_var)))
    {
      error ("attributes applied to %qD are incompatible with original "
	     "declaration", new_var);
      return error_mark_node;
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
       
     Therefore, at this point we have decided to merge the variables.  */
  TREE_SIDE_EFFECTS (old_var) |= TREE_SIDE_EFFECTS (new_var);
  TREE_CONSTANT (old_var) |= TREE_CONSTANT (new_var);
  TREE_ADDRESSABLE (old_var) |= TREE_ADDRESSABLE (new_var);
  TREE_THIS_VOLATILE (old_var) |= TREE_THIS_VOLATILE (new_var);
  TREE_READONLY (old_var) |= TREE_READONLY (new_var);
  TREE_INVARIANT (old_var) |= TREE_INVARIANT (new_var);
  DECL_EXTERNAL (old_var) &= DECL_EXTERNAL (new_var);
  DECL_PRESERVE_P (old_var) |= DECL_PRESERVE_P (new_var);
  if (DECL_INITIAL (new_var))
    {
      gcc_assert (!DECL_INITIAL (old_var));
      DECL_INITIAL (old_var) = DECL_INITIAL (new_var);
    }
  /* Callers should use the value returned.  So, we no longer need
     NEW_VAR.  */
  ggc_free (new_var);

  return old_var;
}
