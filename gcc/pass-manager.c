/* High-level pass management for GCC plugins.
   Copyright (C) 2009 Free Software Foundation, Inc.
   
   Contributed by Inria.

   Authors: Grigori Fursin <grigori.fursin@inria.fr>, Cupertino Miranda
   <cupertinomiranda@gmail.com>, Zbigniew Chamski <zbigniew.chamski@gmail.com>.

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
#undef FLOAT /* This is for hpux. They should change hpux.  */
#undef FFS  /* Some systems define this in param.h.  */
#include "system.h"
#include "coretypes.h"
#include "toplev.h"
#include "hashtab.h"
#include "tree-pass.h"
#include "highlev-plugin-internal.h"
#include "feature-internal.h"
#include "pass-manager.h"

static htab_t passes_hash = NULL; 

/* Hash table dependent functions. */
static hashval_t
passes_htab_hash (const void *x)
{
  const struct opt_pass *p = (const struct opt_pass *) x;

  if (!p)
    internal_error ("passes_htab_hash: called with NULL pass pointer!");

  if (p->name != NULL)
    return htab_hash_string(p->name);
  else
    internal_error ("passes_htab_hash: NULL-named pass!\n");
}

static int
passes_htab_eq (const void *x, const void *y)
{
  const struct opt_pass *p1 = (const struct opt_pass *) x;
  const struct opt_pass *p2 = (const struct opt_pass *) y;

  return !strcmp (p1->name, p2->name);
}


/* Accumulate pass names.  */
static int add_pass_name (void **slot, void *data)
{
  const char ***tmp = (const char ***) data;
  const char **list = *tmp;
  struct opt_pass *pass = *(struct opt_pass **) slot;

  *list = pass->name;

  *tmp = *tmp + 1;
  
  return 1;
}

/* Build the array of names of all registered passes. */
const char **list_passes (void)
{
  size_t size = htab_elements (passes_hash);
  const char **list = 
    (const char **) xmalloc (sizeof (const char *) * (size + 1));
  const char ***tmp = &list;
  const char **ret = list;

  list[size] = NULL;
  htab_traverse_noresize (passes_hash, add_pass_name, tmp);

  return ret;
}


/* Insert a pass mapped by is pass_name in hash table.
 * Allocate hash table when used for the first time.  */
void register_pass (struct opt_pass *pass)
{
  void **slot;

  /* Bail out if the pass has no name or an empty name */
  if (pass->name == NULL)
    internal_error ("Cannot register a pass with no name");

  if (!strcmp (pass->name, ""))
    internal_error ("Cannot register a pass with an empty name");

  if (passes_hash == NULL)
    passes_hash = htab_create_alloc (150, passes_htab_hash, passes_htab_eq, 
				     NULL, xcalloc, free);

  if (passes_hash == NULL)
    error ("Could not allocate the hastable for passes");

  slot = htab_find_slot (passes_hash, pass, INSERT);
  *slot = pass;
}


/* Executes a pass, from any plugin, by its pass_name. */
void run_pass (char *pass_name)
{
  struct opt_pass tmp_pass;
  struct opt_pass *pass;

  tmp_pass.name = pass_name;

  pass = (struct opt_pass *) htab_find (passes_hash, &tmp_pass);

  execute_one_pass (pass);
}
