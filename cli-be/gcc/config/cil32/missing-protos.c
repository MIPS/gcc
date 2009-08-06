/* Function without prototype patching pass.

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
#include "emit-cil.h"

/******************************************************************************
 * Globals                                                                    *
 ******************************************************************************/

/* Pointer-map used to map parameters to their equivalent local variable.  */
static struct pointer_map_t *args_to_locals;

/******************************************************************************
 * Local functions prototypes                                                 *
 ******************************************************************************/

static void create_replacement_args (void);
static void replace_ldargs (void);
static void replace_args (void);
static unsigned int missing_protos (void);
static bool missing_protos_gate (void);

/******************************************************************************
 * Pass implementation                                                        *
 ******************************************************************************/

/* Replaces the function arguments with local variables of the corresponding
   type. Code is then injected for copying the arguments in the replacement
   local variables.  */

static void
create_replacement_args (void)
{
  tree args;
  tree var;
  void **slot;

  args_to_locals = pointer_map_create ();

  for (args = DECL_ARGUMENTS (current_function_decl);
       args != NULL_TREE;
       args = TREE_CHAIN (args))
    {
      tree type = TREE_TYPE (args);
      tree promoted = DECL_ARG_TYPE (args);

      if ((type != promoted) || cil_pointer_type_p (type))
	{
	  var = create_tmp_var (type, IDENTIFIER_POINTER (DECL_NAME (args)));
	  slot = pointer_map_insert (args_to_locals, args);
	  gcc_assert (*slot == NULL);
	  *slot = var;
	}
    }
}

/* Replaces every instance of LDARG/LDARGA with a a LDLOC/LDLOCA to the
   replacement argument.  */

static void
replace_ldargs (void)
{
  cil_stmt_iterator csi;
  cil_stmt stmt;
  basic_block bb;
  enum cil_opcode opcode;
  void **slot;

  FOR_EACH_BB (bb)
    {
      for (csi = csi_start_bb (bb); !csi_end_p (csi); csi_next (&csi))
	{
	  stmt = csi_stmt (csi);
	  opcode = cil_opcode (stmt);

	  if (opcode == CIL_LDARG
	      || opcode == CIL_LDARGA
	      || opcode == CIL_STARG)
	    {
	      slot = pointer_map_contains (args_to_locals, cil_var (stmt));

	      if (slot)
		{
		  switch (opcode)
		    {
		    case CIL_LDARG:  opcode = CIL_LDLOC;  break;
		    case CIL_LDARGA: opcode = CIL_LDLOCA; break;
		    case CIL_STARG:  opcode = CIL_STLOC;  break;
		    default:
		      gcc_unreachable ();
		    }

		  stmt = cil_build_stmt_arg (opcode, (tree) *slot);
		  csi_replace (&csi, stmt); /* Also updates the locus.  */
		}
	    }
	}
    }
}

/* Initialize the replacement variables.  */

static void
replace_args (void)
{
  cil_seq seq = cil_seq_alloc ();
  cil_stmt_iterator csi = csi_start (seq);
  cil_stmt_iterator bb_csi = csi_start_bb (single_succ (ENTRY_BLOCK_PTR));
  cil_stmt stmt;
  tree args;
  void **slot;

  for (args = DECL_ARGUMENTS (current_function_decl);
       args != NULL_TREE;
       args = TREE_CHAIN (args))
    {
      slot = pointer_map_contains (args_to_locals, args);

      if (slot != NULL)
	{
	  stmt = cil_build_stmt_arg (CIL_LDARG, args);
	  csi_insert_after (&csi, stmt, CSI_CONTINUE_LINKING);
	  stmt = cil_build_stmt_arg (CIL_STLOC, (tree) *slot);
	  csi_insert_after (&csi, stmt, CSI_CONTINUE_LINKING);
	}
    }

  csi_insert_seq_before (&bb_csi, seq, CSI_SAME_STMT);
}


/* Main function of this pass. Looks for functions w/o a prototype and replaces
   their arguments with local variables of the correct types. All instances of
   LDARG are replaced with LDLOCs except for the ones used for initializing the
   replacement locals. Once the prototypeless function reaches the emission
   point it's prototype is then printed using the 'promoted' argument types.  */

static unsigned int
missing_protos (void)
{
  if (TYPE_ARG_TYPES (TREE_TYPE (current_function_decl)) == NULL_TREE)
    {
      create_replacement_args ();
      replace_ldargs ();
      replace_args ();
      pointer_map_destroy (args_to_locals);
    }

  return 0;
}

/* Gate function of the missing prototype functions patch pass.  */

static bool
missing_protos_gate (void)
{
  return 1;
}

/* Define the parameters of the missing-protos pass.  */

struct gimple_opt_pass pass_missing_protos =
{
 {
  GIMPLE_PASS,                          /* type */
  "missing_protos",                     /* name */
  missing_protos_gate,                  /* gate */
  missing_protos,                       /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_MISSING_PROTOS,                    /* tv_id */
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
