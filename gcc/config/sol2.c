/* General Solaris system support.
   Copyright (C) 2004  Free Software Foundation, Inc.
   Contributed by CodeSourcery, LLC.

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
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "tm.h"
#include "tm_p.h"
#include "toplev.h"
#include "ggc.h"

tree solaris_pending_aligns, solaris_pending_inits, solaris_pending_finis;

/* Attach any pending attributes for DECL to the list in *ATTRIBUTES.
   Pending attributes come from #pragma or _Pragma, so this code is
   only useful in the C family front ends, but it is included in
   all languages to avoid changing the target machine initializer
   depending on the language.  */

attribute_count
solaris_add_attributes (tree decl, attribute_list attributes,
			const struct one_attribute **to_add)
{
  static struct one_attribute a[4];
  attribute_count ac = 0;
  tree *x, next;

  *to_add = a;

  if (solaris_pending_aligns != NULL && TREE_CODE (decl) == VAR_DECL)
    for (x = &solaris_pending_aligns; *x; x = &TREE_CHAIN (*x))
      {
	tree name = TREE_PURPOSE (*x);
	tree value = TREE_VALUE (*x);
	if (DECL_NAME (decl) == name)
	  {
	    if (has_attribute_p ("aligned", DECL_ATTRIBUTES (decl))
		|| has_attribute_p ("aligned", attributes))
	      warning ("%Jignoring %<#pragma align%> for explicitly "
		       "aligned %<%D%>", decl, decl);
	    else
	      {
		a[ac].name = get_identifier ("aligned");
		a[ac++].value = value;
	      }
	    next = TREE_CHAIN (*x);
	    ggc_free (*x);
	    *x = next;
	    break;
	  }
      }

  if (solaris_pending_inits != NULL && TREE_CODE (decl) == FUNCTION_DECL)
    for (x = &solaris_pending_inits; *x; x = &TREE_CHAIN (*x))
      {
	tree name = TREE_PURPOSE (*x);
	if (DECL_NAME (decl) == name)
	  {
	    a[ac].name = get_identifier ("init");
	    a[ac++].value = NULL_TREE;
	    a[ac].name = get_identifier ("used");
	    a[ac++].value = NULL_TREE;
	    next = TREE_CHAIN (*x);
	    ggc_free (*x);
	    *x = next;
	    break;
	  }
      }

  if (solaris_pending_finis != NULL && TREE_CODE (decl) == FUNCTION_DECL)
    for (x = &solaris_pending_finis; *x; x = &TREE_CHAIN (*x))
      {
	tree name = TREE_PURPOSE (*x);
	if (DECL_NAME (decl) == name)
	  {
	    a[ac].name = get_identifier ("fini");
	    a[ac++].value = NULL_TREE;
	    a[ac].name = get_identifier ("used");
	    a[ac++].value = NULL_TREE;
	    next = TREE_CHAIN (*x);
	    ggc_free (*x);
	    *x = next;
	    break;
	  }
      }
  return ac;
}

/* Output initializer or finalizer entries for DECL to FILE.  */

void
solaris_output_init_fini (FILE *file, tree decl)
{
  if (has_attribute_p ("init", DECL_ATTRIBUTES (decl)))
    {
      fprintf (file, "\t.pushsection\t\".init\"\n");
      ASM_OUTPUT_CALL (file, IDENTIFIER_POINTER (DECL_NAME (decl)));
      fprintf (file, "\t.popsection\n");
    }

  if (has_attribute_p ("fini", DECL_ATTRIBUTES (decl)))
    {
      fprintf (file, "\t.pushsection\t\".fini\"\n");
      ASM_OUTPUT_CALL (file, IDENTIFIER_POINTER (DECL_NAME (decl)));
      fprintf (file, "\t.popsection\n");
    }
}

