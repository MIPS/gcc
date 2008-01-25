/* Handle concepts in GNU C++ 
   Copyright (C) 2005, 2006, 2007, 2008  Free Software Foundation, Inc.
   Written by Douglas Gregor (doug.gregor@gmail.com) while at Indiana University. 
This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "cp-tree.h"
#include "toplev.h"

static bool check_refinement (tree, tree);

/* Begin the definition of a new concept with the given NAME (an
   IDENTIFIER_NODE) and the currently-active template parameter list.
   IMPLICIT_P is true if this is an implicit concept (denoted via the
   `auto' keyword).  Returns the new concept node.  */
tree
begin_concept_definition (tree name, bool implicit_p)
{
  tree the_concept = xref_tag (concept_type, name, ts_current, 
                               /*template_header_p=*/true);

  if (the_concept == error_mark_node)
    return error_mark_node;

  /* Mark this as a concept.  */
  CLASSTYPE_USE_CONCEPT (the_concept) 
    = implicit_p? ck_implicit_concept : ck_explicit_concept;

  /* Start the definition of this concept.  */
  return begin_class_definition (the_concept, /*attributes=*/NULL_TREE);
}

/* Verify that the concept-id REFINED can be a refined by THE_CONCEPT,
   which is the concept currently being defined.  Returns TRUE when
   the refinement is valid, FALSE otherwise.  */
bool 
check_refinement (tree the_concept, tree refined)
{
  /* A requirement-specifier shall refer to a defined concept
     [concept.refinement].  The only way this can fail is if the
     concept refers to itself in a refinement.  */
  if (CLASSTYPE_TI_TEMPLATE (refined) == the_concept)
    {
      error ("refinement %<%T%> cannot be recursive", refined);
      return false;
    }

  /* The template-argument-list of a refinement-specifier's concept-id
     shall refer to at least one of the concept parameters
     [concept.refinement].  */
  if (!uses_template_parms (refined))
    {
      error ("refinement %<%T%> does not refer to any concept parameters", 
             refined);
      return false;
    }

  /* TODO: A refinement-specifier in the refinement clause shall not
     refer to associated types. */
  return true;
}

/* Attach the list of refined concepts to THE_CONCEPT (a
   RECORD_TYPE). REFINEMENTS is a TREE_LIST whose TREE_VALUE fields
   contain concept-ids.

   This routine verifies the correctness of each refinement,
   complaining about and removing any erroneous refinements from the
   list.  The resulting set of refinements is attached to the concept
   as base classes.  */
void
xref_refinements (tree the_concept, tree refinements)
{
  tree refined = refinements;
  tree prev_refined = NULL_TREE;
  tree concept_tmpl = CLASSTYPE_TI_TEMPLATE (the_concept);

  while (refined) 
    {
      /* Verify that this refinement is well-formed.  */
      if (!check_refinement (concept_tmpl, TREE_VALUE (refined)))
        {
          /* The refinement is broken, so remove it from
             consideration.  */
          if (prev_refined)
            {
              TREE_CHAIN (prev_refined) = TREE_CHAIN (refined);
              refined = TREE_CHAIN (prev_refined);
            }
          else
            {
              refinements = TREE_CHAIN (refinements);
              refined = refinements;
            }

          continue;
        }

      /* Refinements are stored as virtual, public inheritance.  */
      TREE_PURPOSE (refined) = access_public_node; /* public */
      TREE_TYPE (refined) = void_type_node;        /* virtual */
      prev_refined = refined;
      refined = TREE_CHAIN (refined);
    }

  if (refinements)
    /* Attach the remaining refinements to the concept.  We use the same
       representation as base classes.  */
    xref_basetypes (the_concept, refinements);
}

/* Finish the definition of a new concept.  */
tree
finish_concept_definition (tree the_concept)
{
  return finish_struct (the_concept, NULL_TREE);
}
