/* This pass applies a few simple peephole optimizations.

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
#include "tree.h"
#include "tree-pass.h"
#include "cil-stmt.h"
#include "cil-types.h"

/******************************************************************************
 * Local functions prototypes                                                 *
 ******************************************************************************/

static unsigned int cil_peephole (void);
static bool cil_peephole_gate (void);

/******************************************************************************
 * Pass implementation                                                        *
 ******************************************************************************/

/* Main function of this pass, do some peephole optimizations:
   LDLOCA/LDIND -> LDLOC
   LDARGA/LDIND -> LDARG
   LDLOCA/<instr>/STIND -> <instr>/STLOC
   LDARGA/<instr>/STIND -> <instr>/STLOC */

static unsigned int
cil_peephole (void)
{
  basic_block bb;
  cil_stmt_iterator csi;
  cil_stmt stmt;
  enum cil_opcode opcode;
  tree var;
  bool loc;

  FOR_EACH_BB (bb)
    {
      csi = csi_start_bb (bb);

      while (!csi_end_p (csi))
	{
	  stmt = csi_stmt (csi);
	  opcode = cil_opcode (stmt);

	  if (((opcode == CIL_LDLOCA) || (opcode == CIL_LDARGA))
	      && !csi_one_before_end_p (csi))
	    {
	      var = cil_var (stmt);
	      loc = (opcode == CIL_LDLOCA);
	      csi_next (&csi);
	      stmt = csi_stmt (csi);

	      if (cil_ldind_p (stmt) && !cil_prefix_volatile (stmt))
		{
		  csi_prev (&csi);
		  csi_remove (&csi);
		  opcode = loc ? CIL_LDLOC : CIL_LDARG;
		  csi_replace (&csi, cil_build_stmt_arg (opcode, var));
		}
	      else if (cil_push_p (stmt) && !csi_one_before_end_p (csi))
		{
		  csi_next (&csi);
		  stmt = csi_stmt (csi);

		  if (cil_stind_p (stmt) && !cil_prefix_volatile (stmt))
		    {
		      csi_prev (&csi);
		      csi_prev (&csi);
		      csi_remove (&csi);
		      csi_next (&csi);
		      opcode = loc ? CIL_STLOC : CIL_STARG;
		      csi_replace (&csi, cil_build_stmt_arg (opcode, var));
		    }
		}
	    }
	  else
	    csi_next (&csi);
	}
    }

  return 0;
}

/* Gate function of the peephole-optimizations pass.  */

static bool
cil_peephole_gate (void)
{
  return true;
}

/* Define the parameters of the peephole-optimizations pass.  */

struct tree_opt_pass pass_cil_peephole =
{
  "cil_peephole",                       /* name */
  cil_peephole_gate,                    /* gate */
  cil_peephole,                         /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_CIL_PEEPHOLE,                      /* tv_id */
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
