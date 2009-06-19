/* CIL IR lowering.

   Copyright (C) 2009 Free Software Foundation, Inc.

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

Author:
   Erven Rohou

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
*/

#include <stdio.h>

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "timevar.h"
#include "errors.h"
#include "real.h"
#include "tree.h"
#include "tree-pass.h"

#include "cil-builtins.h"
#include "cil-refs.h"
#include "cil-stack.h"
#include "cil-stmt.h"
#include "cil-types.h"

static bool lower_cil_gate (void);
static unsigned int lower_cil (void);


enum simd_backend {
  GCC_SIMD,
  MONO_SIMD
};

/* Entry point of the CIL lowering pass.  */


/* This function scans the IR and looks for specific opcodes related to vector
   types that need to be lowered. These opcodes are the constructor, loads and
   stores, and arithmetic.  */
static unsigned int
lower_cil (void)
{
  enum simd_backend backend = GCC_SIMD;
  basic_block bb;
  cil_stmt_iterator csi;
  cil_bb_stacks bbs;
  cil_stack stack;

  if (simd_backend_str)
    {
      if (strcmp (simd_backend_str, "mono") == 0)
	{
	  backend = MONO_SIMD;
	  add_referenced_assembly ("Mono.Simd");
	}
      else if (strcmp (simd_backend_str, "gcc") != 0)
	{
	  fprintf (stderr, "Unknown SIMD backend '%s', using GCC\n",
		   simd_backend_str);
	}
    }

  /* We need to compute the stack to be able to catch arithmetic nodes
     operating on vector types.  */
  bbs = cil_bb_stacks_create ();

  FOR_EACH_BB (bb)
    {
      stack = cil_stack_for_bb (bbs, bb);
      csi = csi_start_bb (bb);

      while (!csi_end_p (csi))
	{
	  enum cil32_builtin builtin;
	  cil_stmt stmt = csi_stmt (csi);
          enum cil_opcode opcode = cil_opcode (stmt);

	  switch (opcode)
	    {
	    case CIL_VEC_CTOR:
	      {
		/* Depending on the chosen back end, a VEC_CTOR node translates
		   to different constructs. In the case of GCC_SIMD, it is a
		   call to the static method, in the case of MONO_SIMD, it is a
		   newobj statement.  */
		tree vec_type = cil_type (stmt);
		cil_type_t cil_type = scalar_to_cil (vec_type);
		if (backend == GCC_SIMD)
		  {
		    switch (cil_type)
		      {
			/* 16-byte */		       
		      case CIL_V2DI: builtin = CIL32_GCC_V2DI_CTOR; break;
		      case CIL_V4SI: builtin = CIL32_GCC_V4SI_CTOR; break;
		      case CIL_V8HI: builtin = CIL32_GCC_V8HI_CTOR; break;
		      case CIL_V16QI: builtin = CIL32_GCC_V16QI_CTOR; break;
		      case CIL_V2DF: builtin = CIL32_GCC_V2DF_CTOR; break;
		      case CIL_V4SF: builtin = CIL32_GCC_V4SF_CTOR; break;
			/* 8-byte */
		      case CIL_V2SI: builtin = CIL32_GCC_V2SI_CTOR; break;
		      case CIL_V4HI: builtin = CIL32_GCC_V4HI_CTOR; break;
		      case CIL_V8QI: builtin = CIL32_GCC_V8QI_CTOR; break;
		      case CIL_V2SF: builtin = CIL32_GCC_V2SF_CTOR; break;
		      default:
			internal_error ("Vector type expected, seen %d",
					cil_type);
		      }
		    stmt = cil_build_call (cil32_builtins[builtin]);
		  }
		else
		  {
		    switch (cil_type)
		      {
		      case CIL_V2DI: builtin = CIL32_MONO_V2DI_CTOR; break;
		      case CIL_V4SI: builtin = CIL32_MONO_V4SI_CTOR; break;
		      case CIL_V8HI: builtin = CIL32_MONO_V8HI_CTOR; break;
		      case CIL_V16QI: builtin = CIL32_MONO_V16QI_CTOR; break;
		      case CIL_V2DF: builtin = CIL32_MONO_V2DF_CTOR; break;
		      case CIL_V4SF: builtin = CIL32_MONO_V4SF_CTOR; break;
		      default:
			internal_error ("Vector type expected, seen %d",
					cil_type);
		      }
		    stmt = cil_build_newobj (cil32_builtins[builtin]);
		  }

		csi_remove (&csi);
		csi_insert_before (&csi, stmt, CSI_CONTINUE_LINKING);
		stmt = csi_stmt (csi);
		cil_stack_after_stmt (stack, stmt);
	      }
	      break;

	    case CIL_LDVEC:
	      /* Process the stack now. After expansion, the return type is
		 going to be an opaque valuetype.  */
	      cil_stack_after_stmt (stack, stmt);

	      /* The MONO_SIMD back end has support for aligned accesses.  */
	      if ((backend == MONO_SIMD) && (cil_prefix_unaligned (stmt) == 0))
		{
		  tree vec_type = cil_type (stmt);
		  cil_type_t cil_type = scalar_to_cil (vec_type);
		  switch (cil_type)
		    {
		    case CIL_V2DF:
		      builtin = CIL32_MONO_V2DF_LOAD_ALIGNED;
		      break;
		    case CIL_V4SF:
		      builtin = CIL32_MONO_V4SF_LOAD_ALIGNED;
		      break;
		    case CIL_V2DI:
		      builtin = CIL32_MONO_V2DI_LOAD_ALIGNED;
		      break;
		    case CIL_V4SI:
		      builtin = CIL32_MONO_V4SI_LOAD_ALIGNED;
		      break;
		    case CIL_V8HI:
		      builtin = CIL32_MONO_V8HI_LOAD_ALIGNED;
		      break;
		    case CIL_V16QI:
		      builtin = CIL32_MONO_V16QI_LOAD_ALIGNED;
		      break;
		    default:
		      gcc_unreachable ();
		    }
		  stmt = cil_build_call (cil32_builtins[builtin]);
		}
	      else
		{
		  tree type = cil_type (stmt);
		  stmt = cil_build_stmt_arg (CIL_LDOBJ, type);
		}

	      csi_remove (&csi);
	      csi_insert_before (&csi, stmt, CSI_CONTINUE_LINKING);
	      stmt = csi_stmt (csi);
	      break;

	    case CIL_STVEC:
	      /* The MONO_SIMD back end has support for aligned accesses.  */
	      if ((backend == MONO_SIMD) && (cil_prefix_unaligned (stmt) == 0))
		{
		  tree vec_type = cil_type (stmt);
		  cil_type_t cil_type = scalar_to_cil (vec_type);
		  switch (cil_type)
		    {
		    case CIL_V2DF:
		      builtin = CIL32_MONO_V2DF_STORE_ALIGNED;
		      break;
		    case CIL_V4SF:
		      builtin = CIL32_MONO_V4SF_STORE_ALIGNED;
		      break;
		    case CIL_V2DI:
		      builtin = CIL32_MONO_V2DI_STORE_ALIGNED;
		      break;
		    case CIL_V4SI:
		      builtin = CIL32_MONO_V4SI_STORE_ALIGNED;
		      break;
		    case CIL_V8HI:
		      builtin = CIL32_MONO_V8HI_STORE_ALIGNED;
		      break;
		    case CIL_V16QI:
		      builtin = CIL32_MONO_V16QI_STORE_ALIGNED;
		      break;
		    default:
		      gcc_unreachable ();
		    }
		  stmt = cil_build_call (cil32_builtins[builtin]);
		}
	      else
		{
		  tree type = cil_type (stmt);
		  stmt = cil_build_stmt_arg (CIL_STOBJ, type);
		}

	      csi_remove (&csi);
	      csi_insert_before (&csi, stmt, CSI_CONTINUE_LINKING);
	      stmt = csi_stmt (csi);
	      cil_stack_after_stmt (stack, stmt);
	      break;

	    case CIL_ADD:
	      {
		cil_type_t top = cil_stack_top (stack);
		cil_stack_after_stmt (stack, stmt);

		if (cil_vector_p (top))
		  {
		    switch (top)
		      {
			/* No 8-byte support in Mono.Simd. Error/warning?  */
		      case CIL_V2SF: builtin = CIL32_GCC_V2SF_ADD; break;
		      case CIL_V2SI: builtin = CIL32_GCC_V2SI_ADD; break;
		      case CIL_V4HI: builtin = CIL32_GCC_V4HI_ADD; break;
		      case CIL_V8QI: builtin = CIL32_GCC_V8QI_ADD; break;

		      case CIL_V2DF:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V2DF_ADD
				                       : CIL32_MONO_V2DF_ADD);
			break;
		      case CIL_V4SF:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V4SF_ADD
				                       : CIL32_MONO_V4SF_ADD);
			break;
		      case CIL_V2DI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V2DI_ADD
				                       : CIL32_MONO_V2DI_ADD);
			break;
		      case CIL_V4SI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V4SI_ADD
				                       : CIL32_MONO_V4SI_ADD);
			break;
		      case CIL_V8HI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V8HI_ADD
				                       : CIL32_MONO_V8HI_ADD);
			break;
		      case CIL_V16QI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V16QI_ADD
				                       : CIL32_MONO_V16QI_ADD);
			break;
		      default:
			gcc_unreachable ();
		      }
		    stmt = cil_build_call (cil32_builtins[builtin]);
		    csi_remove (&csi);
		    csi_insert_before (&csi, stmt, CSI_CONTINUE_LINKING);
		    stmt = csi_stmt (csi);
		  }
	      }
	      break;

	    case CIL_SUB:
	      {
		cil_type_t top = cil_stack_top (stack);
		cil_stack_after_stmt (stack, stmt);

		if (cil_vector_p (top))
		  {
		    switch (top)
		      {
		      /* No 8-byte support in Mono.Simd. Error/warning?  */
		      case CIL_V2SF: builtin = CIL32_GCC_V2SF_SUB; break;
		      case CIL_V2SI: builtin = CIL32_GCC_V2SI_SUB; break;
		      case CIL_V4HI: builtin = CIL32_GCC_V4HI_SUB; break;
		      case CIL_V8QI: builtin = CIL32_GCC_V8QI_SUB; break;

		      case CIL_V2DF:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V2DF_SUB
				                       : CIL32_MONO_V2DF_SUB);
			break;
		      case CIL_V4SF:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V4SF_SUB
				                       : CIL32_GCC_V4SF_SUB);
			break;
		      case CIL_V2DI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V2DI_SUB
				                       : CIL32_MONO_V2DI_SUB);
			break;
		      case CIL_V4SI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V4SI_SUB
				                       : CIL32_MONO_V4SI_SUB);
			break;
		      case CIL_V8HI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V8HI_SUB
				                       : CIL32_MONO_V8HI_SUB);
			break;
		      case CIL_V16QI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V16QI_SUB
				                       : CIL32_MONO_V16QI_SUB);
			break;
		      default:
			gcc_unreachable ();
		      }
		    stmt = cil_build_call (cil32_builtins[builtin]);
		    csi_remove (&csi);
		    csi_insert_before (&csi, stmt, CSI_CONTINUE_LINKING);
		    stmt = csi_stmt (csi);
		  }
	      }
	      break;

	    case CIL_AND:
	      {
		cil_type_t top = cil_stack_top (stack);
		cil_stack_after_stmt (stack, stmt);

		if (cil_vector_p (top))
		  {
		    switch (top)
		      {
		      /* No 8-byte support in Mono.Simd. Error/warning?  */
		      case CIL_V2SI: builtin = CIL32_GCC_V2SI_AND; break;
		      case CIL_V4HI: builtin = CIL32_GCC_V4HI_AND; break;
		      case CIL_V8QI: builtin = CIL32_GCC_V8QI_AND; break;

		      case CIL_V2DI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V2DI_AND
				                       : CIL32_MONO_V2DI_AND);
			break;
		      case CIL_V4SI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V4SI_AND
				                       : CIL32_MONO_V4SI_AND);
			break;
		      case CIL_V8HI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V8HI_AND
				                       : CIL32_MONO_V8HI_AND);
			break;
		      case CIL_V16QI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V16QI_AND
				                       : CIL32_MONO_V16QI_AND);
			break;
		      default:
			gcc_unreachable ();
		      }
		    stmt = cil_build_call (cil32_builtins[builtin]);
		    csi_remove (&csi);
		    csi_insert_before (&csi, stmt, CSI_CONTINUE_LINKING);
		    stmt = csi_stmt (csi);
		  }
	      }
	      break;

	    case CIL_OR:
	      {
		cil_type_t top = cil_stack_top (stack);
		cil_stack_after_stmt (stack, stmt);

		if (cil_vector_p (top))
		  {
		    switch (top)
		      {
		      /* No 8-byte support in Mono.Simd. Error/warning?  */
		      case CIL_V2SI: builtin = CIL32_GCC_V2SI_OR; break;
		      case CIL_V4HI: builtin = CIL32_GCC_V4HI_OR; break;
		      case CIL_V8QI: builtin = CIL32_GCC_V8QI_OR; break;

		      case CIL_V2DI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V2DI_OR
				                       : CIL32_MONO_V2DI_OR);
			break;
		      case CIL_V4SI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V4SI_OR
				                       : CIL32_MONO_V4SI_OR);
			break;
		      case CIL_V8HI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V8HI_OR
				                       : CIL32_MONO_V8HI_OR);
			break;
		      case CIL_V16QI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V16QI_OR
				                       : CIL32_MONO_V16QI_OR);
			break;
		      default:
			gcc_unreachable ();
		      }
		    stmt = cil_build_call (cil32_builtins[builtin]);
		    csi_remove (&csi);
		    csi_insert_before (&csi, stmt, CSI_CONTINUE_LINKING);
		    stmt = csi_stmt (csi);
		  }
	      }
	      break;

	    case CIL_XOR:
	      {
		cil_type_t top = cil_stack_top (stack);
		cil_stack_after_stmt (stack, stmt);

		if (cil_vector_p (top))
		  {
		    switch (top)
		      {
		      /* No 8-byte support in Mono.Simd. Error/warning?  */
		      case CIL_V2SI: builtin = CIL32_GCC_V2SI_XOR; break;
		      case CIL_V4HI: builtin = CIL32_GCC_V4HI_XOR; break;
		      case CIL_V8QI: builtin = CIL32_GCC_V8QI_XOR; break;

		      case CIL_V2DI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V2DI_XOR
				                       : CIL32_MONO_V2DI_XOR);
			break;
		      case CIL_V4SI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V4SI_XOR
				                       : CIL32_MONO_V4SI_XOR);
			break;
		      case CIL_V8HI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V8HI_XOR
				                       : CIL32_MONO_V8HI_XOR);
			break;
		      case CIL_V16QI:
			builtin = (backend == GCC_SIMD ? CIL32_GCC_V16QI_XOR
				                       : CIL32_MONO_V16QI_XOR);
			break;
		      default:
			gcc_unreachable ();
		      }
		    stmt = cil_build_call (cil32_builtins[builtin]);
		    csi_remove (&csi);
		    csi_insert_before (&csi, stmt, CSI_CONTINUE_LINKING);
		    stmt = csi_stmt (csi);
		  }
	      }
	      break;

	    default:
	      cil_stack_after_stmt (stack, stmt);
	      break;
	    }

	  csi_next (&csi);
	}
    }
  return 0;
}


/* Gate function of the CIL lowering pass.  */

static bool
lower_cil_gate (void)
{
  /* FIXME: should lower only if vector types are handled in this funtion. */
  return true;
}


/* Define the parameters of the CIL lowering pass.  */

struct tree_opt_pass pass_lower_cil =
{
  "lowercil",                           /* name */
  lower_cil_gate,                       /* gate */
  lower_cil,                            /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_LOWER_CIL,                         /* tv_id */
  PROP_cfg,                             /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,
  0,                                    /* todo_flags_finish */
  0                                     /* letter */
};
