/* No-plt symbol class for the GNU compiler.
   Copyright (C) 2015 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "options.h"
#include "diagnostic.h"
#include "tree.h"
#include "target.h"
#include "noplt-symbols.h"

static noplt_symbol_table_type *noplt_symbols;
static char *symbols;

void
noplt_symbols_initialize (void)
{
  if (!flag_plt || !noplt_string)
    return;

  struct stat st;
  char *symbol;
  const char *delim;
  if (stat (noplt_string, &st) < 0)
    {
      symbols = xstrdup (noplt_string);
      delim = ",";
    }
  else
    {
      if (!S_ISREG (st.st_mode))
	{
	  error ("%s: invalid no-plt symbol file", noplt_string);
	  return;
	}

      size_t filesize = st.st_size;
      FILE *fp = fopen (noplt_string, "rb");
      if (fp == NULL)
	{
	  error ("%s: could not open no-plt symbol file", noplt_string);
	  return;
	}
      symbols = (char *) xmalloc (filesize + 1);
      if (fread (symbols, 1, filesize, fp) != filesize)
	{
	  error ("%s: could not read no-plt symbol file", noplt_string);
	  fclose (fp);
	  return;
	}
      fclose (fp);
      symbols[filesize] = '\0';
      delim = "\r\n";
    }
  noplt_symbols = new noplt_symbol_table_type (10);
  symbol = strtok (symbols, delim);
  while (symbol)
    {
      const char **slot = noplt_symbols->find_slot (symbol, INSERT);
      *slot = symbol;
      symbol = strtok (NULL, delim);
    }
}

void
noplt_symbols_finish (void)
{
  if (noplt_symbols)
    {
      delete noplt_symbols;
      noplt_symbols = NULL;
    }
  if (symbols)
    {
      free (symbols);
      symbols = NULL;
    }
}

bool
noplt_symbol_p (const char *name)
{
  if (noplt_symbols)
    {
      name = targetm.strip_name_encoding (name);
      return noplt_symbols->find_slot (name, NO_INSERT) != NULL;
    }
  return false;
}

bool
noplt_decl_p (tree decl)
{
  if (!flag_plt)
    return true;

  if (decl == NULL_TREE)
    return false;

  if (lookup_attribute ("noplt", DECL_ATTRIBUTES (decl)))
    return true;

  if (noplt_symbols)
    {
      const char *name = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (decl));
      name = targetm.strip_name_encoding (name);
      if (noplt_symbols->find_slot (name, NO_INSERT) != NULL)
	return true;

      if (lookup_attribute ("weakref", DECL_ATTRIBUTES (decl)))
	{
	  tree alias = lookup_attribute ("alias", DECL_ATTRIBUTES (decl));
	  name = TREE_STRING_POINTER (TREE_VALUE (TREE_VALUE (alias)));
	  return noplt_symbols->find_slot (name, NO_INSERT) != NULL;
	}
    }

  return false;
}
