/* Initialization of partially used subregs. 
   Copyright (C) 1987, 1988, 1992, 1993, 1994, 1995, 1996, 1997, 1998,
   1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006 Free Software Foundation,
   Inc.

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
02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "regs.h"
#include "expr.h"
#include "tree-pass.h"
#include "basic-block.h"
#include "flags.h"
#include "df.h"

/* Add a store of 0 before any subreg load that my reference undefined
   bits.  This makes combine happy.  */

/* This structure is used to pass parameters to and from the
   the function find_regno_partial(). It is used to pass in the
   register number we are looking, as well as to return any rtx
   we find.  */

static int find_regno_partial (rtx *, void *);

typedef struct {
  unsigned regno_to_find;
  rtx retval;
} find_regno_partial_param;


/* Find the rtx for the reg numbers specified in 'data' if it is
   part of an expression which only uses part of the register.  Return
   it in the structure passed in.  */
static int
find_regno_partial (rtx *ptr, void *data)
{
  find_regno_partial_param *param = (find_regno_partial_param *)data;
  unsigned reg = param->regno_to_find;
  param->retval = NULL_RTX;

  if (*ptr == NULL_RTX)
    return 0;

  switch (GET_CODE (*ptr))
    {
    case ZERO_EXTRACT:
    case SIGN_EXTRACT:
    case STRICT_LOW_PART:
      if (REG_P (XEXP (*ptr, 0)) && REGNO (XEXP (*ptr, 0)) == reg)
	{
	  param->retval = XEXP (*ptr, 0);
	  return 1;
	}
      break;

    case SUBREG:
      if (REG_P (SUBREG_REG (*ptr))
	  && REGNO (SUBREG_REG (*ptr)) == reg)
	{
	  param->retval = SUBREG_REG (*ptr);
	  return 1;
	}
      break;

    default:
      break;
    }

  return 0;
}

/* Process all immediate successors of the entry block looking for pseudo
   registers which are live on entry. Find all of those whose first
   instance is a partial register reference of some kind, and initialize
   them to 0 after the entry block.  This will prevent bit sets within
   registers whose value is unknown, and may contain some kind of sticky
   bits we don't want.  */

static void
initialize_uninitialized_subregs (void)
{
  rtx insn;
  edge e;
  unsigned reg;
  bool did_something = false;
  find_regno_partial_param param;
  edge_iterator ei;
  df_analyze ();

  FOR_EACH_EDGE (e, ei, ENTRY_BLOCK_PTR->succs)
    {
      basic_block bb = e->dest;
      regset map = DF_LIVE_IN (bb);
      reg_set_iterator rsi;

      EXECUTE_IF_SET_IN_REG_SET (map, FIRST_PSEUDO_REGISTER, reg, rsi)
	{
	  int uid = REGNO_FIRST_UID (reg);
	  rtx i;

	  /* Find an insn which mentions the register we are looking for.
	     Its preferable to have an instance of the register's rtl since
	     there may be various flags set which we need to duplicate.
	     If we can't find it, its probably an automatic whose initial
	     value doesn't matter, or hopefully something we don't care about.  */
	  for (i = get_insns (); i && INSN_UID (i) != uid; i = NEXT_INSN (i))
	    ;
	  if (i != NULL_RTX)
	    {
	      /* Found the insn, now get the REG rtx, if we can.  */
	      param.regno_to_find = reg;
	      for_each_rtx (&i, find_regno_partial, &param);
	      if (param.retval != NULL_RTX)
		{
		  start_sequence ();
		  emit_move_insn (param.retval,
				  CONST0_RTX (GET_MODE (param.retval)));
		  insn = get_insns ();
		  end_sequence ();
		  insert_insn_on_edge (insn, e);
		  did_something = true;
		}
	    }
	}
    }

  if (did_something)
    {
      commit_edge_insertions ();
      allocate_reg_life_data ();
    }
}

static bool
gate_initialize_subregs (void)
{
  return optimize > 0;
}

static unsigned int
rest_of_handle_initialize_subregs (void)
{
  no_new_pseudos = 0;
  initialize_uninitialized_subregs ();
  no_new_pseudos = 1;
  return 0;
}

struct tree_opt_pass pass_initialize_subregs =
{
  "init-subreg",                        /* name */
  gate_initialize_subregs,              /* gate */
  rest_of_handle_initialize_subregs,    /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  0,                                    /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  0,                                    /* todo_flags_finish */
  0                                     /* letter */
};
