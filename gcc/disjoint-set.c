/* Tree based linear points-to analysis
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.
   Contributed by Daniel Berlin <dberlin@dberlin.org>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "ggc.h"
#include "disjoint-set.h"

disjoint_set
disjoint_set_new ()
{
  disjoint_set ret = ggc_alloc (sizeof (struct disjoint_set_def));
  ret->parent = NULL;
  ret->rank = 0;
  ret->elements = NULL;
  return ret;

}

disjoint_set
disjoint_set_union (x, y)
     disjoint_set x;
     disjoint_set y;
{
  disjoint_set elemx = disjoint_set_find (x);
  disjoint_set elemy = disjoint_set_find (y);

  if (elemx == elemy)
    return elemx;
  if (elemx->rank > elemy->rank)
    {
      elemy->parent = elemx;
      disjoint_set_merge_elems (elemx, elemy);
      return elemx;
    }
  elemx->parent = elemy;
  if (elemx->rank == elemy->rank)
    elemy->rank += 1;
  disjoint_set_merge_elems (elemy, elemx);
  return elemy;
}

disjoint_set
disjoint_set_find (x)
     disjoint_set x;
{
  if (x->parent == NULL)
    return x;
  return disjoint_set_find (x->parent);
}

bool
disjoint_set_is_rep (x)
     disjoint_set x;
{
  return x->parent == NULL;
}

int
disjoint_set_size (x)
     disjoint_set x;
{
  disjoint_set rep = disjoint_set_find (x);
  if (rep->elements == NULL)
    return 1;
  return VARRAY_ACTIVE_SIZE (rep->elements);
}

varray_type
disjoint_set_elems (x)
     disjoint_set x;
{
  /* XXX: This sucks, since we have to copy the varray. */
  varray_type ret;
  size_t i;
  disjoint_set rep = disjoint_set_find (x);
  VARRAY_GENERIC_PTR_INIT (ret, 1, "Disjoint set elements");
  if (rep->elements == NULL)
    {
      VARRAY_PUSH_GENERIC_PTR (ret, x);
      return ret;
    }
  for (i = 0; i < VARRAY_ACTIVE_SIZE (rep->elements); i++)
    {
      VARRAY_PUSH_GENERIC_PTR (ret, VARRAY_GENERIC_PTR (rep->elements, i));
    }
  return ret;
}

bool
disjoint_set_equiv (x, y)
     disjoint_set x;
     disjoint_set y;
{
  if (x == y)
    return true;
  if (disjoint_set_find (x) == disjoint_set_find (y))
    return true;
  return false;
}

void
disjoint_set_merge_elems (x, y)
     disjoint_set x;
     disjoint_set y;
{
  size_t ysize;
  size_t i;
  if (x == y)
    return;
  if (x->elements == NULL)
    {
      VARRAY_GENERIC_PTR_INIT (x->elements, 1, "Disjoint set elements");
      VARRAY_PUSH_GENERIC_PTR (x->elements, x);
    }
  if (y->elements == NULL)
    {

      VARRAY_PUSH_GENERIC_PTR (x->elements, y);
      return;
    }
  ysize = VARRAY_ACTIVE_SIZE (y->elements);
  for (i = 0; i < ysize; i++)
    VARRAY_PUSH_GENERIC_PTR (x->elements,
			     VARRAY_GENERIC_PTR (y->elements, i));
}
