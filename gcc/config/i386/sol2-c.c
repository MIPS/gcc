/* Solaris support needed only by C/C++ frontends.
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
#include "tm.h"
#include "cpplib.h"
#include "tree.h"
#include "c-pragma.h"
#include "c-tree.h"
#include "c-incpath.h"
#include "toplev.h"
#include "tm_p.h"

static GTY(()) tree pending_aligns, pending_inits, pending_finis;

/* Handle #pragma align ALIGNMENT (VAR [, VAR]...)  */

void
solaris_pragma_align (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  tree t, x;
  enum cpp_ttype ttype;
  HOST_WIDE_INT low;

  if (c_lex (&x) != CPP_NUMBER
      || c_lex (&t) != CPP_OPEN_PAREN)
    {
      warning ("malformed %<#pragma align%>, ignoring");
      return;
    }

  low = TREE_INT_CST_LOW (x);
  if (TREE_INT_CST_HIGH (x) != 0
      || (low != 1 && low != 2 && low != 4 && low != 8 && low != 16
	  && low != 32 && low != 64 && low != 128))
    {
      warning ("invalid alignment for %<#pragma align%>, ignoring");
      return;
    }

  ttype = c_lex (&t);
  if (ttype != CPP_NAME)
    {
      warning ("malformed %<#pragma align%>, ignoring");
      return;
    }

  while (1)
    {
      tree decl = identifier_global_value (t);
      if (decl && TREE_CODE_CLASS (TREE_CODE (decl)) == 'd')
	warning ("%<#pragma align%> must appear before the declaration of "
		 "%D, ignoring", decl);
      else
	pending_aligns = tree_cons (t, build_tree_list (NULL, x),
				    pending_aligns);

      ttype = c_lex (&t);
      if (ttype == CPP_COMMA)
	{
	  ttype = c_lex (&t);
	  if (ttype != CPP_NAME)
	    {
	      warning ("malformed %<#pragma align%>");
	      return;
	    }
	}
      else if (ttype == CPP_CLOSE_PAREN)
	{
	  if (c_lex (&t) != CPP_EOF)
	    warning ("junk at end of %<#pragma align%>");
	  return;
	}
      else
	{
	  warning ("malformed %<#pragma align%>");
	  return;
	}
    }
}

/* Handle #pragma init (function [, function]...)  */

void
solaris_pragma_init (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  tree t;
  enum cpp_ttype ttype;

  if (c_lex (&t) != CPP_OPEN_PAREN)
    {
      warning ("malformed %<#pragma init%>, ignoring");
      return;
    }

  ttype = c_lex (&t);
  if (ttype != CPP_NAME)
    {
      warning ("malformed %<#pragma init%>, ignoring");
      return;
    }

  while (1)
    {
      tree decl = identifier_global_value (t);
      if (decl && TREE_CODE_CLASS (TREE_CODE (decl)) == 'd')
	{
	  tree init_list = build_tree_list (get_identifier ("init"),
					    NULL);
	  tree attrs = tree_cons (get_identifier ("used"), NULL, init_list);
	  decl_attributes (&decl, attrs, 0);
	}
      else
	pending_inits = tree_cons (t, NULL, pending_inits);

      ttype = c_lex (&t);
      if (ttype == CPP_COMMA)
	{
	  ttype = c_lex (&t);
	  if (ttype != CPP_NAME)
	    {
	      warning ("malformed %<#pragma init%>");
	      return;
	    }
	}
      else if (ttype == CPP_CLOSE_PAREN)
	{
	  if (c_lex (&t) != CPP_EOF)
	    warning ("junk at end of %<#pragma init%>");
	  return;
	}
      else
	{
	  warning ("malformed %<#pragma init%>");
	  return;
	}
    }
}

/* Handle #pragma fini (function [, function]...)  */

void
solaris_pragma_fini (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  tree t;
  enum cpp_ttype ttype;

  if (c_lex (&t) != CPP_OPEN_PAREN)
    {
      warning ("malformed %<#pragma fini%>, ignoring");
      return;
    }

  ttype = c_lex (&t);
  if (ttype != CPP_NAME)
    {
      warning ("malformed %<#pragma fini%>, ignoring");
      return;
    }

  while (1)
    {
      tree decl = identifier_global_value (t);
      if (decl && TREE_CODE_CLASS (TREE_CODE (decl)) == 'd')
	{
	  tree fini_list = build_tree_list (get_identifier ("fini"),
					    NULL);
	  tree attrs = tree_cons (get_identifier ("used"), NULL, fini_list);
	  decl_attributes (&decl, attrs, 0);
	}
      else
	pending_finis = tree_cons (t, NULL, pending_finis);

      ttype = c_lex (&t);
      if (ttype == CPP_COMMA)
	{
	  ttype = c_lex (&t);
	  if (ttype != CPP_NAME)
	    {
	      warning ("malformed %<#pragma fini%>");
	      return;
	    }
	}
      else if (ttype == CPP_CLOSE_PAREN)
	{
	  if (c_lex (&t) != CPP_EOF)
	    warning ("junk at end of %<#pragma fini%>");
	  return;
	}
      else
	{
	  warning ("malformed %<#pragma fini%>");
	  return;
	}
    }
}

/* Attach any pending attributes for DECL to the list in *ATTRIBUTES.  */

void
solaris_insert_attributes (tree decl, tree *attributes)
{
  tree *x, next;

  if (pending_aligns != NULL && TREE_CODE (decl) == VAR_DECL)
    for (x = &pending_aligns; *x; x = &TREE_CHAIN (pending_aligns))
      {
	tree name = TREE_PURPOSE (*x);
	tree value = TREE_VALUE (*x);
	if (DECL_NAME (decl) == name)
	  {
	    if (lookup_attribute ("aligned", DECL_ATTRIBUTES (decl))
		|| lookup_attribute ("aligned", *attributes))
	      warning ("%Jignoring %<#pragma align%> for explicitly "
		       "aligned %<%D%>", decl, decl);
	    else
	      *attributes = tree_cons (get_identifier ("aligned"), value,
				       *attributes);
	    next = TREE_CHAIN (*x);
	    ggc_free (*x);
	    *x = next;
	    break;
	  }
      }

  if (pending_inits != NULL && TREE_CODE (decl) == FUNCTION_DECL)
    for (x = &pending_inits; *x; x = &TREE_CHAIN (pending_inits))
      {
	tree name = TREE_PURPOSE (*x);
	if (DECL_NAME (decl) == name)
	  {
	    *attributes = tree_cons (get_identifier ("init"), NULL,
				     *attributes);
	    *attributes = tree_cons (get_identifier ("used"), NULL,
				     *attributes);
	    next = TREE_CHAIN (*x);
	    ggc_free (*x);
	    *x = next;
	    break;
	  }
      }

  if (pending_finis != NULL && TREE_CODE (decl) == FUNCTION_DECL)
    for (x = &pending_finis; *x; x = &TREE_CHAIN (pending_finis))
      {
	tree name = TREE_PURPOSE (*x);
	if (DECL_NAME (decl) == name)
	  {
	    *attributes = tree_cons (get_identifier ("fini"), NULL,
				     *attributes);
	    *attributes = tree_cons (get_identifier ("used"), NULL,
				     *attributes);
	    next = TREE_CHAIN (*x);
	    ggc_free (*x);
	    *x = next;
	    break;
	  }
      }
}

/* Output initializer or finalizer entries for DECL to FILE.  */

void
solaris_output_init_fini (FILE *file, tree decl)
{
  if (lookup_attribute ("init", DECL_ATTRIBUTES (decl)))
    {
      fprintf (file, "\t.section\t.init\n");
      fprintf (file, "\tcall\t%s\n", IDENTIFIER_POINTER (DECL_NAME (decl)));
      fprintf (file, "\t.popsection\n");
    }

  if (lookup_attribute ("fini", DECL_ATTRIBUTES (decl)))
    {
      fprintf (file, "\t.section\t.fini\n");
      fprintf (file, "\tcall\t%s\n", IDENTIFIER_POINTER (DECL_NAME (decl)));
      fprintf (file, "\t.popsection\n");
    }
}
