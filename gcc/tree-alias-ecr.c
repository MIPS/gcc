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
#include "varray.h"
#include "tree-alias-ecr.h"
#include "splay-tree.h"
#include "bitmap.h"

static unsigned int ECR_num = 0;
/*static  GTY((param_is (struct ECR_def)))*/ 
static splay_tree ECR_mapping = NULL;

static void ECR_add_pending PARAMS ((ECR, ECR));


ECR
ECR_new ()
{
  return ECR_new_with_type (alias_bottom, NULL);
}

ECR
ECR_new_with_type (type, var)
     alias_type type;
     alias_typevar var;
{
  ECR ret = ggc_alloc (sizeof (struct ECR_def));
  ret->set.parent = NULL;
  ret->set.rank = 0;
  ret->set.elements = NULL;
  ret->type = type;
  ret->color = 0;
  ret->var = var;
  ret->pending = NULL;
  ret->id = ECR_num++;
  if (!ECR_mapping)
    {
      ECR_mapping = splay_tree_new (splay_tree_compare_ints, NULL, NULL);
    }
  splay_tree_insert (ECR_mapping, (splay_tree_key) ECR_num - 1, (splay_tree_value) ret);
  return ret;
}

alias_type
ECR_get_type (ecr)
     ECR ecr;
{
  ECR rep = ECR_find (ecr);
  return rep->type;
}

int
ECR_get_setid (ecr)
     ECR ecr;
{
  ECR rep = ECR_find (ecr);
  return rep->id;
}

void
ECR_union_pending_sets (e1, e2, e3)
     ECR e1;
     ECR e2;
     ECR e3;
{
  if ((e1->pending != NULL) && (e2->pending != NULL))
    {
      if (e3->pending == NULL)
	{
	  e3->pending = BITMAP_GGC_ALLOC ();
	  bitmap_clear (e3->pending);
	}
      bitmap_a_or_b (e3->pending, e2->pending, e1->pending);
    }
  else if (e2->pending != NULL)
    {
      if (e3->pending == NULL)
	{
	  e3->pending = BITMAP_GGC_ALLOC ();
	  bitmap_clear (e3->pending);
	}
      bitmap_copy (e3->pending, e2->pending);
    }
  else if (e1->pending != NULL)
    {
      if (e3->pending == NULL)
	{
	  e3->pending = BITMAP_GGC_ALLOC ();
	  bitmap_clear (e3->pending);
	}
      bitmap_copy (e3->pending, e1->pending);
    }

  if (e3 != e1)
    {
      if (e1->pending)
	bitmap_clear (e1->pending);
      e1->pending = NULL;
    }
  if (e3 != e2)
    {
      if (e2->pending)
	bitmap_clear (e2->pending);
      e2->pending = NULL;
    }
}

static void
ECR_add_pending (e1, e2)
     ECR e1;
     ECR e2;
{
  ECR rep = ECR_find (e1);
  if (rep->pending == NULL)
    {
      rep->pending = BITMAP_GGC_ALLOC ();
      bitmap_clear (rep->pending);
    }
  bitmap_set_bit (rep->pending, ECR_get_id (e2));
}

void
ECR_cjoin (e1, e2)
     ECR e1;
     ECR e2;
{
  if (ECR_get_type (e2) == alias_bottom)
    ECR_add_pending (e2, e1);
  else
    ECR_join (e1, e2);
}

void
ECR_join (e1, e2)
     ECR e1;
     ECR e2;
{
  alias_type t1 = ECR_get_type (e1);
  alias_type t2 = ECR_get_type (e2);

  ECR u = ECR_union (e1, e2);

  if (t1 == alias_bottom)
    {
      u->type = t2;
      if (t2 == alias_bottom)
	{
	  ECR_union_pending_sets (u, e1, e2);
	}
      else
	{
	  if (e1->pending != NULL)
	    {
	      int i;
	      EXECUTE_IF_SET_IN_BITMAP (e1->pending, 0, i,
	      {
		ECR_join (u, (ECR) splay_tree_lookup (ECR_mapping, i)->value);
	      });
	      bitmap_clear (e1->pending);
	      e1->pending = NULL;
	    }
	}
    }
  else
    {
      u->type = t1;
      if (t2 == alias_bottom)
	{
	  if (e2->pending != NULL)
	    {
	      int i;
	      EXECUTE_IF_SET_IN_BITMAP (e2->pending, 0, i,
	      {
		ECR_join (u, (ECR) splay_tree_lookup (ECR_mapping, i)->value);
	      });
	      bitmap_clear (e2->pending);
	      e2->pending = NULL;
	    }

	}
      else
	{
	  ALIAS_TYPE_UNIFY (t1, t2);
	}
    }
}

void
ECR_set_type (e1, t)
     ECR e1;
     alias_type t;
{
  ECR rep = ECR_find (e1);
  rep->type = t;
  if (e1->pending != NULL)
    {
      int i;
      EXECUTE_IF_SET_IN_BITMAP (e1->pending, 0, i,
      {
	ECR_join (e1, (ECR) splay_tree_lookup (ECR_mapping, i)->value);
      });
      bitmap_clear (e1->pending);
      e1->pending = NULL;
    }
}
