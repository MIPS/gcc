/* Translate EQUIVALENCE lists
   Copyright (C) 2000, 2001, 2002, 2003 Free Software Foundation, Inc.
   Contributed by Canqun Yang <canqun@yahoo.com.cn>.

This file is part of GNU G95.

GNU G95 is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU G95 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU G95; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* EQUIVALENCE.c -- Handling of EQUIVALENCE lists. */

/* Implementation notes:

   There are two steps to implement EQUIVALENCE:

   1. Build the EQUIVALENCE ring. The main work is to evaulate the
      reference index for each EQUIVALENCE object and merge the EQUIVALENCE
      rings when the same variable is in two EQUIVALENCE rings. Refer to
      'gfc_add_to_equiv_ring'.

   2. Storage layout for EQUIVALENCE object.

      For local EQUIVALENCE (none of the EQUIVALENCE object in an EQUIVALENCE
      ring is a COMMON variable), we make a master area to hold the
      EQUIVALENCE object. Refer to 'gfc_layout_local_equiv'.

      For global EQUIVALENCE, storage layout is done when the master area is
      made for COMMON block. Refer to 'gfc_layout_global_equiv' in
      'trans-common.c'.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "toplev.h"
#include "tm.h"
#include "target.h"
#include <assert.h>
#include "gfortran.h"
#include "trans.h"
#include "trans-types.h"
#include "trans-const.h"


/* Get the reference index for ARRAY_TYPE variable.  */

static int
gfc_get_array_ref_index (gfc_array_ref ar, mpz_t * result)
{
  mpz_t dimen_size, tmp;
  gfc_array_spec *as;
  gfc_expr *indice;
  gfc_expr *lower;
  gfc_expr *upper;
  int n;

  as = ar.as;
  if (as == NULL || as->type != AS_EXPLICIT)
    return FAILURE;

  mpz_init (dimen_size);

  for (n = 0; n < ar.dimen ; n++)
    {
      mpz_init (tmp);
      indice = ar.start[n];
      lower = as->lower[n];
      upper = as->upper[n];

      assert (indice != NULL);
      assert (lower != NULL);
      assert (upper != NULL);

      if (lower->expr_type != EXPR_CONSTANT
          || upper->expr_type != EXPR_CONSTANT)
        {
          return FAILURE;
        }

      mpz_sub (tmp, indice->value.integer, lower->value.integer);
      if (n>0)
         mpz_mul (tmp, tmp, dimen_size);
      mpz_add (*result, *result, tmp);

      mpz_sub (dimen_size, upper->value.integer, lower->value.integer);
      mpz_add_ui (dimen_size, dimen_size, 1);
    }

  return SUCCESS;
}


/* Get the reference index for non-scalar variable.  */

static int
gfc_evaluate_ref_index (gfc_expr * expr, mpz_t * result)
{
  if (expr->ref == NULL)
    return SUCCESS;

  switch (expr->ref->type)
    {
      case REF_ARRAY:
        if (gfc_get_array_ref_index (expr->ref->u.ar, result) == FAILURE)
          {
            gfc_error ("Can't get the index for EQUIVALENCE object "
                       "at %L", &expr->where);
            return FAILURE;
          }
          break;
      case REF_SUBSTRING:
        gfc_todo_error ("SUBSTRING reference as EQUIVALENCE object");

      default:
        gfc_error ("illegal reference type at %L as EQUIVALENCE object",
                   &expr->where);
        return FAILURE;
    }
  return SUCCESS;
}


/* Get the element size of ARRAY_TYPE variable, etc.  */

static HOST_WIDE_INT
gfc_get_element_size (gfc_symbol * sym)
{
  tree sym_type;

  sym_type = gfc_sym_type (sym);
  if (sym->as)
     sym_type = gfc_get_element_type (sym_type);

  return TREE_INT_CST_LOW (TYPE_SIZE_UNIT (sym_type));
}


/* Build the EQUIVALENCE ring.
   Equivalence ring merging operation is performed when needed.  */

static gfc_symbol*
gfc_add_to_equiv_ring (gfc_symbol * p, HOST_WIDE_INT * base, gfc_expr * expr,
                       gfc_symbol * sym)
{
  gfc_symbol *q1;
  gfc_symbol *q2;
  mpz_t mpz_index;
  tree tree_index;

  HOST_WIDE_INT esize;
  HOST_WIDE_INT index;
  HOST_WIDE_INT offset;
  HOST_WIDE_INT diff;

  mpz_init (mpz_index);
  mpz_set_ui (mpz_index, 0);

  /* Don't add this symbol.  */
  if (gfc_evaluate_ref_index (expr, &mpz_index) == FAILURE)
    return p;

  tree_index = gfc_conv_mpz_to_tree (mpz_index, gfc_index_integer_kind);
  index = TREE_INT_CST_LOW (tree_index);

  esize = gfc_get_element_size (sym);
  offset = *base - index * esize;

  /* Add SYM to the EQUIVALENCE ring.  */
  if (sym->equiv_ring == NULL)
    {
      if (p == NULL) p = sym;
      else  sym->equiv_ring = p->equiv_ring;
      p->equiv_ring = sym;
      sym->equiv_offset = offset;
    }

  /* Merge the current EQUIVALENCE ring and the original one.  */
  else
    {
      diff = sym->equiv_offset - offset;
      *base += diff;

      if (p == NULL) p = sym;
      else
        {
          q1 = p;
          do /* Adjust the offset in the current EQUIVALENCE ring.  */
            {
              if (q1 == sym)
                {
                  if (diff)
                    gfc_error ("EQUIVALENCE conflict for object '%s' at %L",
                               sym->name, &expr->where);
                  else goto skip;
                }
              q1->equiv_offset = q1->equiv_offset + diff;
              q2 = q1;
              q1 = q1->equiv_ring;
            }
          while (q1 != p);

          /* Merging.  */
          q2->equiv_ring = sym->equiv_ring;
          sym->equiv_ring = p;
         }
     }
skip:
  return p;
}


/* Layout local EQUIVALENCE. Local EQUIVALENCE means none of the EQUIVALENCE
   object in an EQUIVALENCE ring is a COMMON variable).  */

static void
gfc_layout_local_equiv (gfc_symbol * sym)
{
  gfc_symbol *var;
  HOST_WIDE_INT min_offset;
  HOST_WIDE_INT max_size;
  HOST_WIDE_INT offset;
  HOST_WIDE_INT var_size;
  tree equiv_decl;
  tree range;
  tree array_type;
  char equiv_name [GFC_MAX_SYMBOL_LEN*2 + 1];

  /* The symbol is already layed out.  */
  if ( sym->addr_base )
    return;

  /* The symbol is not an EQUIVALENCE object.  */
  if ( !sym->equiv_ring )
    return;

  var = sym;
  /* Contains COMMON variables ? If so, return.  */
  do
    {
      /* Global EQUIVALENCE.
         Layout each EQUIVALENCE object in the master area of that COMMON
         block. Refer to trans-common.c.  */
      if (var->attr.in_common)
        return;
      var = var->equiv_ring;
    }
  while (var != sym);

  /* Find the minimize offset.  */
  var = sym;
  min_offset = 0;
  do
    {
      min_offset = min_offset < var->equiv_offset ?
                   min_offset : var->equiv_offset;
      var = var->equiv_ring;
    }
  while (var != sym);

  /* Assign the address offset to each EQUIVALENCE object and calculate the
     maximum storage size to hold these EQUIVALENCE objects.  */
  var = sym;
  max_size = 0;
  do
    {
      offset = var->equiv_offset - min_offset;
      var->addr_offset = build_int_2 (offset, 0);

      var_size = TREE_INT_CST_LOW (TYPE_SIZE_UNIT (gfc_sym_type (var)))
                 + offset;

      if (max_size < var_size)
        max_size = var_size;

      var = var->equiv_ring;
    }
  while (var != sym);

  /* Make the storage object for this EQUIVALENCE ring.  */
  range = build_int_2 (max_size - 1, 0);
  range = build_range_type (gfc_array_index_type, integer_zero_node, range);
  array_type = build_array_type (gfc_character1_type_node, range);
  sprintf(equiv_name, "__gfc_equiv_%s", sym->name);
  equiv_decl = build_decl (VAR_DECL,
                           get_identifier (equiv_name), array_type);
  DECL_COMMON (equiv_decl) = 1;
  TREE_ADDRESSABLE (equiv_decl) = 1;
  TREE_USED (equiv_decl) = 1;
  gfc_add_decl_to_function (equiv_decl);

  /* Assign the base address to each EQUIVALENCE object.  */
  var = sym;
  do
    {
      var->addr_base = equiv_decl;
      var = var->equiv_ring;
    }
  while (var != sym);
}


/* Main entry point for translating EQUIVALENCE lists.  */

void
gfc_trans_equivalence (gfc_namespace * ns)
{
  gfc_equiv  *list;
  gfc_equiv  *element;
  gfc_symbol *p;
  gfc_expr   *expr;

  HOST_WIDE_INT base;

  list = ns->equiv;
  if (list == NULL)
    return;

  /* Build EQUIVALENCE rings.  */
  while (list)
    {
      p = NULL;
      base = 0;

      expr = list->expr;
      p = gfc_add_to_equiv_ring (p, &base, expr, expr->symtree->n.sym);
      for (element = list->eq; element; element = element->eq)
        {
          expr = element->expr;
          p = gfc_add_to_equiv_ring (p, &base, expr, expr->symtree->n.sym);
        }

      list = list->next;
    }

  /* Layout local EQUIVALENCE (none of the EQUIVALENCE object in an EQUIVALENCE
     ring is a COMMON variable).  */
  gfc_traverse_ns (ns, gfc_layout_local_equiv);
}
