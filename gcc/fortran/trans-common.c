/* Translate COMMON blocks
   Copyright (C) 2000, 2001, 2002, 2003 Free Software Foundation, Inc.
   Contributed by Canqun Yang <canqun@yahoo.com.cn>


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

/* trans-common.c -- Make the master area for each COMMON block and layout
   all COMMON variables */

/* Notes for COMMON implementation:

   The strategy of COMMON implementation is to tell the GCC back end about the
   COMMON area as a block of the master type of that area (e.g. character,
   real*4, integer*8, etc.). The master type is determined by gfc_get_common_
   master_type, and is set by gfc_set_common_master_type. If initial values are
   provided, remember not to allow further initialization. If no initialization
   happens at all, tell the GCC back end the largest size that has been seen
   declared so the space does get reserved. As named COMMON blocks shall be of
   the same size in all scoping units of a program in which they appear, a
   warning is issued when this kind of violation is encountered.

   For COMMON variables, we just register which COMMON block it is in and its
   offset from the starting address of the COMMON block by using the fields
   extended in gfc_symbol (addr_base, addr_offset). References like
   *((float *) (addr_base + addr_offset) is created each time a COMMON variable
   is referenced.  */


#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "toplev.h"
#include "tm.h"
#include "gfortran.h"
#include "trans.h"
#include "trans-types.h"

/* COMMON block type: blank COMMON block, and named COMMON block.  */
typedef enum
{
  BLANK_COMMON,
  NAMED_COMMON
}
gfc_common_type;

/* Assembly name for blank COMMON block   */
#define BLANK_COMMON_NAME "__BLNK__"

/* The namespace used to store COMMON block symbols */
static gfc_namespace *gfc_common_ns = NULL;


/* Determine the master type of the COMMON block area. There are three cases
   for COMMON variables in a COMMON block:
     1. all COMMON variables are of the same typespec,
        just choose the typspec.
     2. all COMMON variables are of the same type with different kind,
        choose the typespec with the smallest kind.
     3. COMMON varaibles are not of the same type, choose BT_CHARACTER
        typespec.
   This can lead the initial values of COMMON blocks be output properly.  */

static void
gfc_get_common_master_type (gfc_typespec * ts, gfc_symbol * common_var)
{
  bt type;
  int kind;

  type = common_var->ts.type;
  kind = common_var->ts.kind;

  if (ts->type == BT_UNKNOWN)
    {
      ts->type = type;
      ts->kind = kind;
    }

  if (ts->type == BT_CHARACTER)
    return;

  if (ts->type == type)
    {
      if (ts->kind > kind)
        ts->kind = kind;
    }
  else
    {
      ts->type = BT_CHARACTER;
      ts->kind = 1;
    }
}


/* Set the master type of the COMMON block area.  */

static void
gfc_set_common_master_type (gfc_typespec ts, tree * common_decl)
{
  tree type_node;

  switch (ts.type)
    {
    case BT_UNKNOWN:
    case BT_CHARACTER:
    case BT_DERIVED:
    case BT_PROCEDURE:
      type_node = gfc_character1_type_node;
      break;
    case BT_INTEGER:
      type_node = gfc_get_int_type (ts.kind);
      break;
    case BT_REAL:
      type_node = gfc_get_real_type (ts.kind);
      break;
    case BT_LOGICAL:
      type_node = gfc_get_logical_type (ts.kind);
      break;
    case BT_COMPLEX:
      type_node = gfc_get_complex_type (ts.kind);
      break;
    default:
      type_node = gfc_character1_type_node;
    }

   TREE_TYPE (TREE_TYPE (*common_decl)) = type_node;
}


/* Construct mangled common block name from symbol name.  */

static tree
gfc_sym_mangled_common_id (gfc_symbol * sym)
{
  int has_underscore;
  char name[GFC_MAX_MANGLED_SYMBOL_LEN + 1];

  if (strcmp (sym->name, "__BLNK__") == 0)
    return get_identifier (sym->name);
  if (gfc_option.flag_underscoring)
    {
      has_underscore = strchr (sym->name, '_') != 0;
      if (gfc_option.flag_second_underscore && has_underscore)
        snprintf (name, sizeof name, "%s__", sym->name);
      else
        snprintf (name, sizeof name, "%s_", sym->name);
      return get_identifier (name);
    }
  else
    return get_identifier (sym->name);
}


/* Make the master area for a COMMON block. Initial values for this COMMON
   block will be collected as needed.  */

static tree
gfc_build_common_decl (gfc_common_type common_type, gfc_symbol * common_sym,
                       gfc_symbol * common_var, gfc_typespec ts, tree total_size)
{
  tree array_type;
  tree common_decl;
  gfc_symbol *sym;
  gfc_symbol *var;
  bool is_init = FALSE;
  tree range;

  /* Create a namespace to store symbols for COMMON blocks.  */
  if (gfc_common_ns == NULL)
     gfc_common_ns = gfc_get_namespace (NULL);

  /* Store the COMMON block symbol in gfc_common_ns.  */
  gfc_get_symbol (common_sym->name, gfc_common_ns, &sym);

  /* Backend declaration for this COMMON block.  */
  common_decl = sym->backend_decl;

  /* Build the type of the master area for the COMMON block.
     The default basic type is gfc_character1_type_node which may be altered by
     gfc_set_common_master_type.  */
  range = fold (build (MINUS_EXPR, gfc_array_index_type, total_size,
		       integer_one_node));
  range = build_range_type (gfc_array_index_type, integer_zero_node, range);
  array_type = build_array_type (gfc_character1_type_node, range);

  /* Update the size of this COMMON block as needed.  */
  if (common_decl != NULL_TREE)
    {
      if (tree_int_cst_lt (DECL_SIZE_UNIT (common_decl), total_size))
        {
          /* Named COMMON blocks of the same name shall be of the same size
             in all scoping units of a program in which they appear, but
             blank COMMON blocks may be of different sizes.  */
          if (common_type == NAMED_COMMON)
            {
              gfc_warning ("Named COMMON block '%s' at %L shall be of the same "
                           "size", common_sym->name, &common_sym->declared_at);
            }

          TREE_TYPE (common_decl) = array_type;
          DECL_SIZE_UNIT (common_decl) = total_size;
        }
    }

  /* Are there any initial values for this COMMON block? if so, set is_init.  */
  var = common_var;
  while (var)
    {
#if 0  /* Not support yet.  */
      if (var->attr.data == 1)
        {
          is_init = TRUE;
          break;
        }
#endif
      var = var->common_next;
    }

  /* If this COMMON block has been declared in a previous program unit,
     and either it is already initialized or there is no new initialization
     for it, just return.  */
  if ((common_decl != NULL_TREE)
      && (!is_init || DECL_INITIAL (common_decl)))
    return common_decl;

  /* If there is no backend declaration for the COMMON block, build it.  */
  if (common_decl == NULL_TREE)
    {
      common_decl = build_decl (VAR_DECL, get_identifier (common_sym->name),
                                array_type);
      SET_DECL_ASSEMBLER_NAME (common_decl,
                               gfc_sym_mangled_common_id (common_sym));
      TREE_PUBLIC (common_decl) = 1;
      TREE_STATIC (common_decl) = 1;
    }

  DECL_ALIGN (common_decl) = BIGGEST_ALIGNMENT;
  DECL_USER_ALIGN (common_decl) = 0;

  if (!is_init)
    {
      DECL_INITIAL (common_decl) = NULL_TREE;
      DECL_COMMON (common_decl) = 1;
      DECL_DEFER_OUTPUT (common_decl) = 1;

      /* Place the back end declaration for this COMMON block in
         GLOBAL_BINDING_LEVEL.  */
      sym->backend_decl = pushdecl_top_level (common_decl);
    }
  else /* is_init */
    {
      DECL_INITIAL (common_decl) = error_mark_node;
      DECL_COMMON (common_decl) = 0;
      DECL_DEFER_OUTPUT (common_decl) = 0;
      sym->backend_decl = common_decl;

      /* Set the master type of the COMMON block area.  */
      gfc_set_common_master_type (ts, &common_decl);

      gfc_todo_error ("Initial values for COMMON variables.");
    }

  return common_decl;
}


/* If there is at least one COMMON variable in an EQUIVALENCE ring, all
   variables in this ring will be put in the master area of the COMMON
   block.  The EQUIVALENCE ring is build by gfc_trans_equivalence.  */

static void
gfc_layout_global_equiv (gfc_symbol * common_sym, gfc_symbol *common_var,
                         tree * common_size, tree offset)
{
  gfc_symbol *equiv_var;
  tree equiv_offset;
  tree var_offset;
  tree var_size;
  tree extra_size;
  HOST_WIDE_INT diff;

  /* Get the equivalence ring.  */
  equiv_var = common_var->equiv_ring;

  /* The offset (from the starting address of the master area for the COMMON
     block) is calculated for each object in the EQUIVALENCE ring. The size
     of the master area is updated when needed.  */
  while (equiv_var && equiv_var != common_var)
    {
      diff = equiv_var->equiv_offset - common_var->equiv_offset;
      equiv_offset = build_int_2 (diff, diff<0? -1: 0);

      /* The offset for this EQUIVALENCE object.  */
      var_offset = fold (build (PLUS_EXPR, gfc_array_index_type,
                                offset, equiv_offset));

      /* var_offset<0 means an underflow error for the master area.  */
      if (tree_int_cst_sgn (var_offset) == -1)
         gfc_fatal_error ("COMMON '%s' at %L underflow due to EQUIVALENCE.",
                          common_sym->name, &common_sym->declared_at);

      var_size = TYPE_SIZE_UNIT (gfc_sym_type (equiv_var));

      extra_size = fold (build (PLUS_EXPR, gfc_array_index_type,
                                var_size, var_offset));
      extra_size = fold (build (MINUS_EXPR, gfc_array_index_type,
                                extra_size, *common_size));

      /* When extra_size>0, update the size of the master area.  */
      if (tree_int_cst_sgn (extra_size) == 1)
         *common_size = fold (build (PLUS_EXPR, gfc_array_index_type,
                                     *common_size, extra_size));

      equiv_var->attr.in_common = 1;
      equiv_var->addr_offset = var_offset;

      /* To handle the next object in this EQUIVALENCE ring.  */
      equiv_var = equiv_var ->equiv_ring;
    }
}


/* Make the master area for a COMMON block and layout all COMMON variables
   defined in it.  */

static void
gfc_trans_one_common (gfc_common_type common_type, gfc_symbol * common_sym,
                      gfc_symbol * common_var)
{
  gfc_symbol *var;
  gfc_typespec ts;
  tree common_decl;
  tree common_size;
  tree offset;
  tree s;

  gfc_clear_ts (&ts);
  /* No COMMON variables in the COMMON block, do nothing.  */
  if (common_var == NULL)
    return;

  /* Caculate the offset for each COMMON variable.  */
  offset = integer_zero_node;
  common_size = integer_zero_node;
  var = common_var;
  while (var)
    {
      tree align, pad;
      tree var_type, var_size;

      var_type = gfc_sym_type (var);

      /* Size in bytes for this COMMON varialbe.  */
      var_size = TYPE_SIZE_UNIT (var_type);

      if (!INTEGER_CST_P (var_size))
        gfc_error ("Size of common variable '%s' at %L must be constant",
                   var->name, &var->declared_at);

      /* Determine the master type of the COMMON block area.  */
      gfc_get_common_master_type (&ts, var);

      /* Align the COMMON variable.  */
      align = build_int_2 (TYPE_ALIGN_UNIT (var_type), 0);
      pad = fold (build (FLOOR_MOD_EXPR, gfc_array_index_type, offset, align));

      /* If the pad isn't equal to zero, add it to the offset.
         Let the current COMMON variable has a proper alignment.  */
      if (tree_int_cst_sgn (pad))
        {
          pad = fold (build (MINUS_EXPR, gfc_array_index_type, align, pad));
          common_size = fold (build (PLUS_EXPR,
                                     gfc_array_index_type, common_size, pad));
          offset = fold (build (PLUS_EXPR, gfc_array_index_type, offset, pad));
        }

      /* Set the offset of this COMMON variable.  */
      var->addr_offset = offset;

      /* Current COMMON area size.  */
      s = fold (build (PLUS_EXPR, gfc_array_index_type, offset, var_size));
      if (tree_int_cst_lt (common_size, s))
        common_size = s;

      /* Layout global EQUIVALENCE objects.  */
      if (var->equiv_ring)
        gfc_layout_global_equiv (common_sym, var, &common_size, offset);

      /* Update the offset for the next COMMON variable.  */
      offset = fold (build (PLUS_EXPR, gfc_array_index_type, offset, var_size));

      var = var->common_next;
    }

  /* Make the master area for the COMMON block.  */
  common_decl = gfc_build_common_decl (common_type,
                                       common_sym, common_var, ts, common_size);
  /* Register the base address for each COMMON variable. */
  for (var = common_var; var; var = var->common_next)
    {
      gfc_symbol *equiv_var;

      equiv_var = var;
      do
        {
          equiv_var->addr_base = common_decl;
          equiv_var = equiv_var->equiv_ring;
        }
      while (equiv_var && equiv_var != var);
    }
}


/* Translate a named COMMON block.  */

static void
gfc_trans_named_common (gfc_symbol * sym)
{
  gfc_symbol *common_var;

  /* This symbol is not a COMMON block.  */
  if (!sym->attr.common)
    return;

  common_var = sym->common_head;

  /* Translate this COMMON block.  */
  gfc_trans_one_common (NAMED_COMMON, sym, common_var);
}


/* Translate the blank COMMON block.  */

static void
gfc_trans_blank_common (gfc_namespace * ns)
{
  gfc_symbol *common_sym;
  gfc_symbol *common_var;

  /* No blank COMMON block in this namespace */
  if (ns->blank_common == NULL)
    return;

  gfc_get_symbol ((char *)BLANK_COMMON_NAME, ns, &common_sym);
  common_var = ns->blank_common;

  /* Translate this COMMON block.   */
  gfc_trans_one_common (BLANK_COMMON, common_sym, common_var);
}


/* Main entry point for translating COMMON blocks.  */

void
gfc_trans_common (gfc_namespace * ns)
{
  /* Translate the blank COMMON block.  */
  gfc_trans_blank_common (ns);

  /* Translate named COMMON blocks.  */
  gfc_traverse_ns (ns, gfc_trans_named_common);

  /* Commit the newly created symbols for COMMON blocks.  */
  gfc_commit_symbols ();
}

