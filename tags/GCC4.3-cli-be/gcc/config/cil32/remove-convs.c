/* This pass tries to remove redundant conversions which may have been
   introduced during GIMPLE-to-CIL expansion.  This pass is currently
   implemented as a somewhat elaborate peephole optimizer, to make it
   optimal however it should be reimplemented using proper DFA.

   Copyright (C) 2006-2008 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

Authors:
   Andrea Ornstein
   Erven Rohou
   Gabriele Svelto

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Erven Rohou             <erven.rohou@st.com>
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "flags.h"
#include "timevar.h"
#include "errors.h"
#include "tree.h"
#include "tree-pass.h"
#include "pointer-set.h"
#include "cil-refs.h"
#include "cil-stack.h"
#include "cil-stmt.h"
#include "cil-types.h"

/******************************************************************************
 * Local functions prototypes                                                 *
 ******************************************************************************/

static bool cil_store_p (const_cil_stmt);
static bool fold_store_conversion (const_cil_stmt, enum cil_opcode, cil_type_t);
static bool fold_conversion (enum cil_opcode, cil_type_t);
static unsigned int remove_convs (void);
static bool remove_convs_gate (void);

/******************************************************************************
 * Pass implementation                                                        *
 ******************************************************************************/

/* Return TRUE if OPCODE is a store (either indirect, to a field or to a
   variable), FALSE otherwise.  */

static bool
cil_store_p (const_cil_stmt stmt)
{
  switch (cil_opcode (stmt))
    {
    case CIL_STARG:
    case CIL_STFLD:
    case CIL_STIND_I1:
    case CIL_STIND_I2:
    case CIL_STIND_I4:
    case CIL_STIND_I8:
    case CIL_STIND_R4:
    case CIL_STIND_R8:
    case CIL_STIND_I:
    case CIL_STLOC:
    case CIL_STOBJ:
    case CIL_STSFLD:
      return true;

    default:
      return false;
    }
}

/* Return TRUE if the conversion from type SRC by CONV before executing the
   store STORE is redundant, false otherwise.  */

static bool
fold_store_conversion (const_cil_stmt store, enum cil_opcode conv,
		       cil_type_t src)
{
  cil_type_t dst;
  tree type;

  if (!cil_int_or_smaller_p (src))
    return false;

  switch (cil_opcode (store))
    {
    case CIL_STARG:
    case CIL_STLOC:
      type = TREE_TYPE (cil_var (store));

      if (INTEGRAL_TYPE_P (type))
	dst = scalar_to_cil (type);
      else
	return false;

      break;

    case CIL_STFLD:
    case CIL_STSFLD:
      type = TREE_TYPE (cil_field (store));

      if (INTEGRAL_TYPE_P (type))
	dst = scalar_to_cil (type);
      else
	return false;

      break;

    case CIL_STIND_I1:
      dst = CIL_INT8;
      break;

    case CIL_STIND_I2:
      dst = CIL_INT16;
      break;

    default:
      return false;
    }

  switch (dst)
    {
    case CIL_INT8:
    case CIL_UNSIGNED_INT8:
      return true;

    case CIL_INT16:
    case CIL_UNSIGNED_INT16:
      if ((conv == CIL_CONV_I1) || (conv == CIL_CONV_U1))
	return false;
      else
	return true;

    default:
      return false;
    }
}

/* Fold the conversion specified by OP with an input of type TYPE.  If the
   converion can be folded return TRUE, otherwise return FALSE.  */

static bool
fold_conversion (enum cil_opcode op, cil_type_t type)
{
  switch (op)
    {
    case CIL_CONV_U1:
      if (type == CIL_UNSIGNED_INT8)
	return true;

      break;

    case CIL_CONV_I2:
      if ((type == CIL_INT8) || (type == CIL_UNSIGNED_INT8)
	  || (type == CIL_INT16))
	{
	  return true;
	}

      break;

    case CIL_CONV_U2:
      if ((type == CIL_UNSIGNED_INT8) || (type == CIL_UNSIGNED_INT16))
	return true;

      break;

    case CIL_CONV_I4:
    case CIL_CONV_U4:
      if ((type == CIL_INT8) || (type == CIL_UNSIGNED_INT8)
	  || (type == CIL_INT16) || (type == CIL_UNSIGNED_INT16)
	  || (type == CIL_INT32) || (type == CIL_UNSIGNED_INT32))
	{
	  return true;
	}

      break;

    case CIL_CONV_I8:
    case CIL_CONV_U8:
      if ((type == CIL_INT64) || (type == CIL_UNSIGNED_INT64))
	return true;

      break;

    case CIL_CONV_R4:
      if (type == CIL_FLOAT32)
	return true;

      break;

    case CIL_CONV_R8:
      if (type == CIL_FLOAT64)
	return true;

      break;

    case CIL_CONV_I:
    case CIL_CONV_U:
      if (cil_native_integer_p (type))
	return true;

      break;

    default:
      ;
    }

  return false;
}

/* Main function of this pass.  */

static unsigned int
remove_convs (void)
{
  basic_block bb;
  cil_stmt_iterator csi;
  cil_stmt stmt;
  enum cil_opcode opcode;
  edge_iterator ei;
  edge e;
  cil_bb_stacks bbs;
  cil_stack stack;
  cil_type_t type;

  bbs = cil_bb_stacks_create ();

  FOR_EACH_BB (bb)
    {
      stack = cil_stack_for_bb (bbs, bb);
      csi = csi_start_bb (bb);

      while (!csi_end_p (csi))
	{
	  stmt = csi_stmt (csi);
          opcode = cil_opcode (stmt);

	  if (cil_conversion_p (stmt))
	    {
	      type = cil_stack_top (stack);

	      if (fold_conversion (opcode, type))
		{
		  csi_remove (&csi);
		  continue;
		}
	      else if (!csi_one_before_end_p (csi))
		{
		  csi_next (&csi);

		  if (cil_store_p (csi_stmt (csi))
		      && fold_store_conversion (csi_stmt (csi), opcode, type))
		    {
		      csi_prev (&csi);
		      csi_remove (&csi);
		      continue;
		    }
		  else
		    csi_prev (&csi);
		}
	    }

	  cil_stack_after_stmt (stack, stmt);
	  csi_next (&csi);
	}

      /* We're done with this basic block.  Copy the stack at the end of the
         block into the successors so that they will be able to access it and
	 then free the current stack as we're done with it.  */

      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  cil_set_stack_for_bb (bbs, e->dest, stack);
	}
    }

  cil_bb_stacks_destroy (bbs);
  return 0;
}

/* Gate function of the remove-convs pass.  */

static bool
remove_convs_gate (void)
{
  return true;
}

/* Define the parameters of the remove-temps pass.  */

struct tree_opt_pass pass_remove_convs =
{
  "remove_convs",                       /* name */
  remove_convs_gate,                    /* gate */
  remove_convs,                         /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_REMOVE_CONVS,                      /* tv_id */
  PROP_cfg,                             /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,
  TODO_ggc_collect,                     /* todo_flags_finish */
  0                                     /* letter */
};

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
