/* This pass is made of two different optimization steps which try to
   remove redundant local variables which have been generated during the
   GIMPLE-to-CIL expansion.

   Copyright (C) 2006-2009 Free Software Foundation, Inc.

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
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
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
 * Globals                                                                    *
 ******************************************************************************/

/* Holds all the local variables and arguments whose argument was taken.  */
static struct pointer_set_t *address_taken_vars;

/* Dummy variable used as a marker when flagging visited basic blocks. */
static int marker = 0;

/******************************************************************************
 * Local functions prototypes                                                 *
 ******************************************************************************/

static void visit_bb (basic_block);
static bool visited_p (basic_block);

static void record_address_taken_vars (void);
static cil_stmt build_conversion (cil_type_t, cil_type_t);
static bool remove_matching_ldloc (cil_stmt_iterator, cil_stack, tree, bool);
static void stloc_ldloc_to_dup (void);
static bool compatible_vars_p (tree, tree);
static void propagate_copy_1 (basic_block, tree, tree, bool);
static void propagate_copy (cil_stmt, tree, tree, bool);
static void simple_copy_propagation (void);
static bool dead_store_1 (basic_block, tree);
static bool dead_store_p (cil_stmt);
static void remove_dead_stores (void);
static unsigned int remove_temps (void);
static bool remove_temps_gate (void);

/******************************************************************************
 * Pass implementation                                                        *
 ******************************************************************************/

/* Flag a basic block as visited.  */

static void
visit_bb (basic_block bb)
{
  bb->aux = (void *) &marker;
}

/* Return TRUE if the basic block BB has already been visited, FALSE otherwise.
 */

static bool
visited_p (basic_block bb)
{
  return (bb->aux != NULL);
}

/* Records in a pointer set all the variables and arguments whose address was
   taken.  */

static void
record_address_taken_vars (void)
{
  cil_stmt_iterator csi;
  cil_stmt stmt;
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      for (csi = csi_start_bb (bb); !csi_end_p (csi); csi_next (&csi))
	{
	  stmt = csi_stmt (csi);

	  switch (cil_opcode (stmt))
	    {
	    case CIL_LDLOCA:
	    case CIL_LDARGA:
	      pointer_set_insert (address_taken_vars, cil_var (stmt));
	      break;

	    default:
	      ;
	    }
	}
    }
}

/* Creates a conversion from type SRC to type DST if it is needed, return the
   conversion or NULL if it is not needed.  This function works only on 32-bit
   or smaller integer types, conversions between other types are not considered
   as there are no assignment with implicit conversions between variables of
   those types.  */

static cil_stmt
build_conversion (cil_type_t dst, cil_type_t src)
{
  enum cil_opcode opcode;

  switch (dst)
    {
    case CIL_INT8:
      switch (src)
	{
	case CIL_INT16:
	case CIL_INT32:
	case CIL_UNSIGNED_INT8:
	case CIL_UNSIGNED_INT16:
	case CIL_UNSIGNED_INT32:
	  opcode = CIL_CONV_I1;
	  break;

	default:
	  return NULL;
	}

      break;

    case CIL_INT16:
      switch (src)
	{
	case CIL_INT32:
	case CIL_UNSIGNED_INT8:
	case CIL_UNSIGNED_INT16:
	case CIL_UNSIGNED_INT32:
	  opcode = CIL_CONV_I2;
	  break;

	default:
	  return NULL;
	}

      break;

    case CIL_UNSIGNED_INT8:
      switch (src)
	{
	case CIL_INT8:
	case CIL_INT16:
	case CIL_INT32:
	case CIL_UNSIGNED_INT16:
	case CIL_UNSIGNED_INT32:
	  opcode = CIL_CONV_U1;
	  break;

	default:
	  return NULL;
	}

      break;

    case CIL_UNSIGNED_INT16:
      switch (src)
	{
	case CIL_INT8:
	case CIL_INT16:
	case CIL_INT32:
	case CIL_UNSIGNED_INT32:
	  opcode = CIL_CONV_U2;
	  break;

	default:
	  return NULL;
	}

      break;

    default:
      /* Nothing to do */
      return NULL;
    }

  return cil_build_stmt (opcode);
}

/* Looks for a LDLOC instruction operating on variable VAR using the CSI
   iterator and tries to remove it.  STACK is a copy of the stack
   representation right after the matching STLOC was executed.  A matching
   LDLOC instruction can be removed if the stack depth never shrinks below the
   initial depth of STACK and the depth before executing the LDLOC is the same
   as STACK's and VAR was not altered in the meantime.  If ADDRESS_TAKEN is
   true the address of VAR was taken, in this case a LDLOC will be removed only
   if it follows immediately the corresponding STLOC.  */

static bool
remove_matching_ldloc (cil_stmt_iterator csi, cil_stack stack, tree var,
		       bool address_taken)
{
  size_t min_depth = cil_stack_depth (stack);
  size_t nargs;
  cil_stmt stmt;

  for (csi_next (&csi); !csi_end_p (csi); csi_next (&csi))
    {
      stmt = csi_stmt (csi);

      if ((cil_opcode (stmt) == CIL_STLOC) && (cil_var (stmt) == var))
	break;
      else if ((cil_opcode (stmt) == CIL_LDLOC)
	       && (cil_var (stmt) == var)
	       && (cil_stack_depth (stack) == min_depth))
	{
	  csi_remove (&csi);
	  return true;
	}
      else if (address_taken)
	return false;

      switch (cil_opcode (stmt))
	{
	case CIL_ADD:
	case CIL_AND:
	case CIL_CEQ:
	case CIL_CGT:
	case CIL_CGT_UN:
	case CIL_CLT:
	case CIL_CLT_UN:
	case CIL_DIV:
	case CIL_DIV_UN:
	case CIL_MUL:
	case CIL_OR:
	case CIL_REM:
	case CIL_REM_UN:
	case CIL_SHL:
	case CIL_SHR:
	case CIL_SHR_UN:
	case CIL_SUB:
	case CIL_XOR:
	  if (cil_stack_depth (stack) - 2 < min_depth)
	    return false;

	  break;

	case CIL_ARGLIST:
	case CIL_CKFINITE:
	case CIL_CONV_I1:
	case CIL_CONV_I2:
	case CIL_CONV_I4:
	case CIL_CONV_I8:
	case CIL_CONV_R4:
	case CIL_CONV_R8:
	case CIL_CONV_U1:
	case CIL_CONV_U2:
	case CIL_CONV_U4:
	case CIL_CONV_U8:
	case CIL_CONV_I:
	case CIL_CONV_U:
	case CIL_CONV_R_UN:
	case CIL_DUP:
	case CIL_LDFLD:
	case CIL_LDFLDA:
	case CIL_LDIND_I1:
	case CIL_LDIND_I2:
	case CIL_LDIND_I4:
	case CIL_LDIND_I8:
	case CIL_LDIND_U1:
	case CIL_LDIND_U2:
	case CIL_LDIND_U4:
	case CIL_LDIND_U8:
	case CIL_LDIND_R4:
	case CIL_LDIND_R8:
	case CIL_LDIND_I:
	case CIL_LDOBJ:
	case CIL_NEG:
	case CIL_NOT:
	  if (cil_stack_depth (stack) == min_depth)
	    return false;

	  break;

	case CIL_CALL:
	case CIL_JMP:
	case CIL_CALLI:
	  nargs = cil_call_nargs (stmt);

	  if (cil_stack_depth (stack) - nargs < min_depth)
	    return false;

	  break;

	case CIL_ASM:
	case CIL_LOCALLOC:
	  /* localloc instructions must be executed with an empty stack.  */
	  return false;

	default:
	  ;
	}

      cil_stack_after_stmt (stack, stmt);

      if (cil_stack_depth (stack) < min_depth)
	return false;
    }

  return false;
}

/* Replaces STLOC/LDLOC instruction couples accessing the same variable with
   DUP/STLOC couples.  */

static void
stloc_ldloc_to_dup (void)
{
  cil_stmt_iterator csi;
  cil_stmt stmt, conv, dup;
  basic_block bb;
  edge_iterator ei;
  edge e;
  tree var;
  cil_stack stack, tstack;
  cil_bb_stacks bbs;
  cil_type_t type;
  bool addr_taken;

  bbs = cil_bb_stacks_create ();

  FOR_EACH_BB (bb)
    {
      stack = cil_stack_for_bb (bbs, bb);

      for (csi = csi_start_bb (bb); !csi_end_p (csi); csi_next (&csi))
	{
	  stmt = csi_stmt (csi);

	  if (cil_opcode (stmt) == CIL_STLOC)
	    {
	      var = cil_var (stmt);
	      addr_taken = pointer_set_contains (address_taken_vars, var);
	      tstack = cil_stack_copy (stack);
	      cil_stack_after_stmt (tstack, stmt);

	      if (remove_matching_ldloc (csi, tstack, var, addr_taken))
		{
		  if (INTEGRAL_TYPE_P (TREE_TYPE (var)))
		    {
		      /* Writing an integral type from the stack to a variable
			 of smaller size may cause an implicit conversion,
			 reproduce this conversion on the stack if needed. This
			 a win anyway as a CONV/DUP couple is in the worst case
			 as large as the STLOC/LDLOC couple it replaces but
			 allows for the variable to be removed.  */
		      type = scalar_to_cil (TREE_TYPE (var));
		      conv = build_conversion (type, cil_stack_top (stack));

		      if (conv)
			{
			  csi_insert_before (&csi, conv, CSI_SAME_STMT);
			  cil_stack_after_stmt (stack, conv);
			}
		    }

		  dup = cil_build_stmt (CIL_DUP);
		  csi_insert_before (&csi, dup, CSI_SAME_STMT);
		  cil_stack_after_stmt (stack, dup);
		}

	      cil_stack_free (tstack);
	    }

	  cil_stack_after_stmt (stack, stmt);
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
}

/* Return true if DST and SRC have compatible types for replacement.  */

static bool
compatible_vars_p (tree dst, tree src)
{
  tree dst_type = TYPE_MAIN_VARIANT (TREE_TYPE (dst));
  tree src_type = TYPE_MAIN_VARIANT (TREE_TYPE (src));
  cil_type_t dst_cil, src_cil;

  if (dst_type == src_type)
    return true;
  else
    {
      if (INTEGRAL_TYPE_P (dst_type) && INTEGRAL_TYPE_P (src_type))
	{
	  dst_cil = scalar_to_cil (dst_type);
	  src_cil = scalar_to_cil (src_type);

	  if (src_cil == dst_cil)
	    return true;

	  switch (dst_cil)
	    {
	    case CIL_INT16:
	    case CIL_UNSIGNED_INT16:
	      if ((src_cil == CIL_INT8) || (src_cil == CIL_UNSIGNED_INT8))
		return true;

	      break;

	    case CIL_INT32:
	    case CIL_UNSIGNED_INT32:
	      return true; /* No loss of precision from smaller types.  */

	    case CIL_INT64:
	    case CIL_UNSIGNED_INT64:
	      return true; /* Always explicitely converted.  */

	      break;

	    default:
	      return false;
	    }
	}
    }

  return false;
}

/* Helper function used for implementing the recursive call of the
   propagate_copy() function.  */

static void
propagate_copy_1 (basic_block bb, tree dst, tree src, bool arg)
{
  cil_stmt_iterator csi;
  cil_stmt stmt;
  enum cil_opcode opcode;
  edge e;
  edge_iterator ei;

  for (csi = csi_start_bb (bb); !csi_end_p (csi); csi_next (&csi))
    {
      stmt = csi_stmt (csi);
      opcode = cil_opcode (stmt);

      if ((opcode == CIL_LDLOC) && (cil_var (stmt) == dst))
	{
	  stmt = cil_build_stmt_arg (arg ? CIL_LDARG : CIL_LDLOC, src);
	  csi_replace (&csi, stmt);
	}
      else if (((opcode == CIL_STLOC) || (opcode == CIL_STARG))
	       && ((cil_var (stmt) == src) || (cil_var (stmt) == dst)))
	{
	  return;
	}
      else if (opcode == CIL_RET || opcode == CIL_RET_VAL)
	return;
    }

  visit_bb (bb);

  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      if (!visited_p (e->dest))
	{
	  visit_bb (e->dest);

	  if (single_pred_p (e->dest))
	    propagate_copy_1 (e->dest, dst, src, arg);
	}
    }
}

/* Propagate the copy of variable SRC into variable DST starting from the
   statement STMT.  If ARG is true then SRC is an argument, if it is false
   then SRC is a local variable.  */

static void
propagate_copy (cil_stmt store, tree dst, tree src, bool arg)
{
  cil_stmt_iterator csi;
  cil_stmt stmt;
  enum cil_opcode opcode;
  basic_block bb = cil_bb (store);
  edge e;
  edge_iterator ei;

  csi = csi_for_stmt (store);
  csi_next (&csi);

  while (!csi_end_p (csi))
    {
      stmt = csi_stmt (csi);
      opcode = cil_opcode (stmt);

      if ((opcode == CIL_LDLOC) && (cil_var (stmt) == dst))
	{
	  stmt = cil_build_stmt_arg (arg ? CIL_LDARG : CIL_LDLOC, src);
	  csi_replace (&csi, stmt);
	}
      else if (((opcode == CIL_STLOC) || (opcode == CIL_STARG))
	       && ((cil_var (stmt) == src) || (cil_var (stmt) == dst)))
	{
	  return;
	}
      else if (opcode == CIL_RET || opcode == CIL_RET_VAL)
	return;

      csi_next (&csi);
    }

  visit_bb (bb);

  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      if (!visited_p (e->dest))
	{
	  visit_bb (e->dest);

	  if (single_pred_p (e->dest))
	    propagate_copy_1 (e->dest, dst, src, arg);
	}
    }

  clear_aux_for_blocks ();
}

/* Does a simple copy propagation trying to replace copies of a local variable
   or argument with the original definition.  Stores to the temporary copy are
   left in the code as they will be removed - if they are dead - by a later
   pass.  */

static void
simple_copy_propagation (void)
{
  cil_stmt_iterator csi;
  cil_stmt stmt;
  enum cil_opcode opcode;
  basic_block bb;
  tree src;

  FOR_EACH_BB (bb)
    {
      csi = csi_start_bb (bb);

      while (!csi_end_p (csi))
	{
	  stmt = csi_stmt (csi);
	  opcode = cil_opcode (stmt);

	  if (((opcode == CIL_LDLOC) || (opcode == CIL_LDARG))
	      && !pointer_set_contains (address_taken_vars, cil_var (stmt))
	      && !csi_one_before_end_p (csi))
	    {
	      src = cil_var (stmt);
	      csi_next (&csi);
	      stmt = csi_stmt (csi);

	      if (cil_opcode (stmt) == CIL_STLOC
		  && compatible_vars_p (cil_var (stmt), src))
		{
		  propagate_copy (stmt, cil_var (stmt), src,
				  opcode == CIL_LDARG);
		}
	    }
	  else
	    csi_next (&csi);
	}
    }
}

/* Helper function used for implementing dead_store_p(). Scans the basic block
   BB and its non visited successors for loads from the variable VAR.  */

static bool
dead_store_1 (basic_block bb, tree var)
{
  cil_stmt_iterator csi;
  cil_stmt stmt;
  enum cil_opcode opcode;
  edge e;
  edge_iterator ei;

  for (csi = csi_start_bb (bb); !csi_end_p (csi); csi_next (&csi))
    {
      stmt = csi_stmt (csi);
      opcode = cil_opcode (stmt);

      if ((opcode == CIL_LDLOC) && (cil_var (stmt) == var))
	return false;
      else if ((opcode == CIL_STLOC) && (cil_var (stmt) == var))
	return true;
      else if (opcode == CIL_RET || opcode == CIL_RET_VAL)
	return true;
    }

  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      if (!visited_p (e->dest))
	{
	  visit_bb (e->dest);

	  if (!dead_store_1 (e->dest, var))
	    return false;
	}
    }

  return true;
}

/* Returns TRUE if the target local variable of the store specified by STORE is
   never read from, FALSE otherwise.  */

static bool
dead_store_p (cil_stmt store)
{
  cil_stmt_iterator csi;
  cil_stmt stmt;
  enum cil_opcode opcode;
  tree var = cil_var (store);
  basic_block bb = cil_bb (store);
  edge e;
  edge_iterator ei;

  csi = csi_for_stmt (store);
  csi_next (&csi);

  while (!csi_end_p (csi))
    {
      stmt = csi_stmt (csi);
      opcode = cil_opcode (stmt);

      if ((opcode == CIL_LDLOC) && (cil_var (stmt) == var))
	return false;
      else if ((opcode == CIL_STLOC) && (cil_var (stmt) == var))
	return true;
      else if (opcode == CIL_RET || opcode == CIL_RET_VAL)
	return true;

      csi_next (&csi);
    }

  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      if (!visited_p (e->dest))
	{
	  visit_bb (e->dest);

	  if (!dead_store_1 (e->dest, var))
	    {
	      clear_aux_for_blocks ();
	      return false;
	    }
	}
    }

  clear_aux_for_blocks ();
  return true;
}

/* Looks for stores to variables which are never read, remove those stores and
   mark the related variables as written to assembly, this will prevent the
   compiler from emitting them later.  */

static void
remove_dead_stores (void)
{
  cil_stmt_iterator csi;
  cil_stmt stmt;
  basic_block bb;

  /* Remove all stores to variables which haven't been read back. The
     instructions used to load the value written on the stack should also be
     removed but doing it under complex conditions might be tricky so we only
     catch simple cases for now.  The algorythm may be improved in the future
     by using DFA.  */

  FOR_EACH_BB (bb)
    {
      csi = csi_start_bb (bb);

      while (!csi_end_p (csi))
	{
	  stmt = csi_stmt (csi);

	  if ((cil_opcode (stmt) == CIL_STLOC)
	      && !pointer_set_contains (address_taken_vars, cil_var (stmt))
	      && dead_store_p (stmt))
	    {
	      /* We found a dead store, we'll remove or replace it.  */

	      if (!csi_first_p (csi))
		{
		  csi_prev (&csi);
		  stmt = csi_stmt (csi);

		  if (cil_push_p (stmt))
		    {
		      /* We remove the store and the instruction feeding it.  */
		      csi_remove (&csi);
		      csi_remove (&csi);
		      continue;
		    }
		  else if (cil_conversion_p (stmt) && !csi_first_p (csi))
		    {
		      csi_prev (&csi);
		      stmt = csi_stmt (csi);

		      if (cil_push_p (stmt))
			{
			  /* We remove the store, the conversion and the
			     instruction feeding it. */
			  csi_remove (&csi);
			  csi_remove (&csi);
			  csi_remove (&csi);
			  continue;
			}
		      else
			csi_next (&csi);
		    }

		  /* We cannot remove the instruction feeding this store,
		     instead we replace the store with a pop.  */
		  csi_next (&csi);
		  csi_replace (&csi, cil_build_stmt (CIL_POP));
		  csi_next (&csi);
		}
	      else
		{
		  /* This is the first instruction in the block, we transform
		     it into a pop.  */
		  csi_replace (&csi, cil_build_stmt (CIL_POP));
		  csi_next (&csi);
		}
	    }
	  else
	    csi_next (&csi);
	}
    }
}

/* Main function of this pass.  */

static unsigned int
remove_temps (void)
{
  /* Create pass-specific data.  */
  address_taken_vars = pointer_set_create ();

  /* Pass processing.  */
  record_address_taken_vars ();
  stloc_ldloc_to_dup ();
  simple_copy_propagation ();
  remove_dead_stores ();

  /* Dispose of pass-specific data.  */
  pointer_set_destroy (address_taken_vars);

  return 0;
}

/* Gate function of the remove-temps pass.  */

static bool
remove_temps_gate (void)
{
  return true;
}

/* Define the parameters of the remove-temps pass.  */

struct gimple_opt_pass pass_remove_temps =
{
 {
  GIMPLE_PASS,                          /* type */
  "remove_temps",                       /* name */
  remove_temps_gate,                    /* gate */
  remove_temps,                         /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_REMOVE_TEMPS,                      /* tv_id */
  PROP_cfg,                             /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_ggc_collect                      /* todo_flags_finish */
 }
};

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
