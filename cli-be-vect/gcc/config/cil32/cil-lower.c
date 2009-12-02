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
   Andrea C. Ornstein
   Erven Rohou
   Gabriele Svelto

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

/******************************************************************************
 * Globals                                                                    *
 ******************************************************************************/

enum simd_backend simd_type = UNDEF_SIMD;

/******************************************************************************
 * Local function prototypes                                                  *
 ******************************************************************************/

static bool lower_cil_gate (void);
static unsigned int lower_cil (void);

static cil_stmt lower_cil_vector_ctor (const_cil_stmt);
static cil_stmt lower_cil_ldvec (const_cil_stmt);
static cil_stmt lower_cil_stvec (const_cil_stmt);
static cil_stmt lower_cil_vector_add (cil_type_t);
static cil_stmt lower_cil_vector_sub (cil_type_t);
static cil_stmt lower_cil_vector_and (cil_type_t);
static cil_stmt lower_cil_vector_or (cil_type_t);
static cil_stmt lower_cil_vector_xor (cil_type_t);

/******************************************************************************
 * Vector lowering functions                                                  *
 ******************************************************************************/

/* Lower a generic CIL vector constructor CTOR to a type-specific vector
   constructor depending on the vector type.  Return the replacement
   statement.  */

static cil_stmt
lower_cil_vector_ctor (const_cil_stmt ctor)
{
  enum cil32_builtin builtin;
  tree type = cil_type (ctor);
  cil_type_t cil_type = vector_to_cil (type);

  /* Depending on the chosen back end, a VEC_CTOR node translates to different
     constructs. In the case of GCC_SIMD, it is a call to the static method,
     in the case of MONO_SIMD, it is a newobj statement.  */

  if (simd_type == GCC_SIMD)
    {
      if (cil_short_ctor (ctor))
        {
          switch (cil_type)
            {
              /* 16-byte */
            case CIL_V2DI: builtin = CIL32_GCC_V2DI_CTOR_U; break;
            case CIL_V4SI: builtin = CIL32_GCC_V4SI_CTOR_U; break;
            case CIL_V8HI: builtin = CIL32_GCC_V8HI_CTOR_U; break;
            case CIL_V16QI: builtin = CIL32_GCC_V16QI_CTOR_U; break;
            case CIL_V2DF: builtin = CIL32_GCC_V2DF_CTOR_U; break;
            case CIL_V4SF: builtin = CIL32_GCC_V4SF_CTOR_U; break;
              /* 8-byte */
            case CIL_V2SI: builtin = CIL32_GCC_V2SI_CTOR_U; break;
            case CIL_V4HI: builtin = CIL32_GCC_V4HI_CTOR_U; break;
            case CIL_V8QI: builtin = CIL32_GCC_V8QI_CTOR_U; break;
            case CIL_V2SF: builtin = CIL32_GCC_V2SF_CTOR_U; break;
            default:
              internal_error ("Vector type expected, seen %d", cil_type);
            }
        }
      else
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
              internal_error ("Vector type expected, seen %d", cil_type);
            }
        }
      return cil_build_call (cil32_builtins[builtin]);
    }
  else  /* simd_type == MONO_SIMD */
    {
      if (cil_short_ctor (ctor))
        {
          switch (cil_type)
            {
              /* No 8-byte support in Mono.Simd. Error/warning?  */

            case CIL_V2DI: builtin = CIL32_MONO_V2DI_CTOR_U; break;
            case CIL_V4SI: builtin = CIL32_MONO_V4SI_CTOR_U; break;
            case CIL_V8HI: builtin = CIL32_MONO_V8HI_CTOR_U; break;
            case CIL_V16QI: builtin = CIL32_MONO_V16QI_CTOR_U; break;
            case CIL_V2DF: builtin = CIL32_MONO_V2DF_CTOR_U; break;
            case CIL_V4SF: builtin = CIL32_MONO_V4SF_CTOR_U; break;
            default:
              internal_error ("Vector type expected, seen %d", cil_type);
            }
        }
      else
        {
          switch (cil_type)
            {
              /* No 8-byte support in Mono.Simd. Error/warning?  */

            case CIL_V2DI: builtin = CIL32_MONO_V2DI_CTOR; break;
            case CIL_V4SI: builtin = CIL32_MONO_V4SI_CTOR; break;
            case CIL_V8HI: builtin = CIL32_MONO_V8HI_CTOR; break;
            case CIL_V16QI: builtin = CIL32_MONO_V16QI_CTOR; break;
            case CIL_V2DF: builtin = CIL32_MONO_V2DF_CTOR; break;
            case CIL_V4SF: builtin = CIL32_MONO_V4SF_CTOR; break;
            default:
              internal_error ("Vector type expected, seen %d", cil_type);
            }
        }
      return cil_build_newobj (cil32_builtins[builtin]);
    }
}

/* Lower a generic CIL vector load STMT to a type-specific vector load
   depending on the vector type.  Return the replacement statement.  */

static cil_stmt
lower_cil_ldvec (const_cil_stmt stmt)
{
  enum cil32_builtin builtin;
  tree type = cil_type (stmt);
  cil_type_t cil_type = vector_to_cil (type);

  /* The MONO_SIMD back end has support for aligned accesses.  */
  if ((simd_type == MONO_SIMD) && (cil_prefix_unaligned (stmt) == 0))
    {
      switch (cil_type)
	{
	case CIL_V2DF: builtin = CIL32_MONO_V2DF_LOAD_ALIGNED; break;
	case CIL_V4SF: builtin = CIL32_MONO_V4SF_LOAD_ALIGNED; break;
	case CIL_V2DI: builtin = CIL32_MONO_V2DI_LOAD_ALIGNED; break;
	case CIL_V4SI: builtin = CIL32_MONO_V4SI_LOAD_ALIGNED; break;
	case CIL_V8HI: builtin = CIL32_MONO_V8HI_LOAD_ALIGNED; break;
	case CIL_V16QI: builtin = CIL32_MONO_V16QI_LOAD_ALIGNED; break;
	default:
	  gcc_unreachable ();
	}

      return cil_build_call (cil32_builtins[builtin]);
    }
  else
    return cil_build_stmt_arg (CIL_LDOBJ, type);
}

/* Lower a generic CIL vector store STMT to a type-specific vector store
   depending on the vector type.  Return the replacement statement.  */

static cil_stmt
lower_cil_stvec (const_cil_stmt stmt)
{
  enum cil32_builtin builtin;
  tree type = cil_type (stmt);
  cil_type_t cil_type = vector_to_cil (type);

  /* The MONO_SIMD back end has support for aligned accesses.  */
  if ((simd_type == MONO_SIMD) && (cil_prefix_unaligned (stmt) == 0))
    {
      switch (cil_type)
	{
	case CIL_V2DF: builtin = CIL32_MONO_V2DF_STORE_ALIGNED; break;
	case CIL_V4SF: builtin = CIL32_MONO_V4SF_STORE_ALIGNED; break;
	case CIL_V2DI: builtin = CIL32_MONO_V2DI_STORE_ALIGNED; break;
	case CIL_V4SI: builtin = CIL32_MONO_V4SI_STORE_ALIGNED; break;
	case CIL_V8HI: builtin = CIL32_MONO_V8HI_STORE_ALIGNED; break;
	case CIL_V16QI: builtin = CIL32_MONO_V16QI_STORE_ALIGNED; break;
	default:
	  gcc_unreachable ();
	}

      return cil_build_call (cil32_builtins[builtin]);
    }
  else
    return cil_build_stmt_arg (CIL_STOBJ, type);
}

/* Lower a generic CIL vector addition on the vector type represented by TYPE
   to a type-specific vector addition depending on the vector type.  Return
   the replacement statement.  */

static cil_stmt
lower_cil_vector_add (cil_type_t type)
{
  enum cil32_builtin builtin;

  if (simd_type == GCC_SIMD)
    switch (type)
      {
      case CIL_V2SF: builtin = CIL32_GCC_V2SF_ADD; break;
      case CIL_V2SI: builtin = CIL32_GCC_V2SI_ADD; break;
      case CIL_V4HI: builtin = CIL32_GCC_V4HI_ADD; break;
      case CIL_V8QI: builtin = CIL32_GCC_V8QI_ADD; break;

      case CIL_V2DF: builtin = CIL32_GCC_V2DF_ADD; break;
      case CIL_V4SF: builtin = CIL32_GCC_V4SF_ADD; break;
      case CIL_V2DI: builtin = CIL32_GCC_V2DI_ADD; break;
      case CIL_V4SI: builtin = CIL32_GCC_V4SI_ADD; break;
      case CIL_V8HI: builtin = CIL32_GCC_V8HI_ADD; break;
      case CIL_V16QI: builtin = CIL32_GCC_V16QI_ADD; break;
      default:
        gcc_unreachable ();
      }
  else
    switch (type)
      {
        /* No 8-byte support in Mono.Simd. Error/warning?  */

      case CIL_V2DF: builtin = CIL32_MONO_V2DF_ADD; break;
      case CIL_V4SF: builtin = CIL32_MONO_V4SF_ADD; break;
      case CIL_V2DI: builtin = CIL32_MONO_V2DI_ADD; break;
      case CIL_V4SI: builtin = CIL32_MONO_V4SI_ADD; break;
      case CIL_V8HI: builtin = CIL32_MONO_V8HI_ADD; break;
      case CIL_V16QI: builtin = CIL32_MONO_V16QI_ADD; break;
      default:
        gcc_unreachable ();
      }

  return cil_build_call (cil32_builtins[builtin]);
}

/* Lower a generic CIL vector substraction on the vector type represented by
   TYPE to a type-specific vector substraction depending on the vector type.
   Return the replacement statement.  */

static cil_stmt
lower_cil_vector_sub (cil_type_t type)
{
  enum cil32_builtin builtin;

  if (simd_type == GCC_SIMD)
    switch (type)
      {
      case CIL_V2SF: builtin = CIL32_GCC_V2SF_SUB; break;
      case CIL_V2SI: builtin = CIL32_GCC_V2SI_SUB; break;
      case CIL_V4HI: builtin = CIL32_GCC_V4HI_SUB; break;
      case CIL_V8QI: builtin = CIL32_GCC_V8QI_SUB; break;

      case CIL_V2DF: builtin = CIL32_GCC_V2DF_SUB; break;
      case CIL_V4SF: builtin = CIL32_GCC_V4SF_SUB; break;
      case CIL_V2DI: builtin = CIL32_GCC_V2DI_SUB; break;
      case CIL_V4SI: builtin = CIL32_GCC_V4SI_SUB; break;
      case CIL_V8HI: builtin = CIL32_GCC_V8HI_SUB; break;
      case CIL_V16QI: builtin = CIL32_GCC_V16QI_SUB; break;
      default:
        gcc_unreachable ();
      }
  else
    switch (type)
      {
        /* No 8-byte support in Mono.Simd. Error/warning?  */

      case CIL_V2DF: builtin = CIL32_MONO_V2DF_SUB; break;
      case CIL_V4SF: builtin = CIL32_MONO_V4SF_SUB; break;
      case CIL_V2DI: builtin = CIL32_MONO_V2DI_SUB; break;
      case CIL_V4SI: builtin = CIL32_MONO_V4SI_SUB; break;
      case CIL_V8HI: builtin = CIL32_MONO_V8HI_SUB; break;
      case CIL_V16QI: builtin = CIL32_MONO_V16QI_SUB; break;
      default:
        gcc_unreachable ();
      }

  return cil_build_call (cil32_builtins[builtin]);
}

/* Lower a generic CIL vector logical AND on the vector type represented by
   TYPE to a type-specific vector logical AND depending on the vector type.
   Return the replacement statement.  */

static cil_stmt
lower_cil_vector_and (cil_type_t type)
{
  enum cil32_builtin builtin;

  if (simd_type == GCC_SIMD)
    switch (type)
      {
      case CIL_V2SI: builtin = CIL32_GCC_V2SI_AND; break;
      case CIL_V4HI: builtin = CIL32_GCC_V4HI_AND; break;
      case CIL_V8QI: builtin = CIL32_GCC_V8QI_AND; break;

      case CIL_V2DI: builtin = CIL32_GCC_V2DI_AND; break;
      case CIL_V4SI: builtin = CIL32_GCC_V4SI_AND; break;
      case CIL_V8HI: builtin = CIL32_GCC_V8HI_AND; break;
      case CIL_V16QI: builtin = CIL32_GCC_V16QI_AND; break;
      default:
        gcc_unreachable ();
      }
  else
    switch (type)
      {
        /* No 8-byte support in Mono.Simd. Error/warning?  */

      case CIL_V2DI: builtin = CIL32_MONO_V2DI_AND; break;
      case CIL_V4SI: builtin = CIL32_MONO_V4SI_AND; break;
      case CIL_V8HI: builtin = CIL32_MONO_V8HI_AND; break;
      case CIL_V16QI: builtin = CIL32_MONO_V16QI_AND; break;
      default:
        gcc_unreachable ();
      }

  return cil_build_call (cil32_builtins[builtin]);
}

/* Lower a generic CIL vector logical OR on the vector type represented by
   TYPE to a type-specific vector logical OR depending on the vector type.
   Return the replacement statement.  */

static cil_stmt
lower_cil_vector_or (cil_type_t type)
{
  enum cil32_builtin builtin;

  if (simd_type == GCC_SIMD)
    switch (type)
      {
      case CIL_V2SI: builtin = CIL32_GCC_V2SI_OR; break;
      case CIL_V4HI: builtin = CIL32_GCC_V4HI_OR; break;
      case CIL_V8QI: builtin = CIL32_GCC_V8QI_OR; break;

      case CIL_V2DI: builtin = CIL32_GCC_V2DI_OR; break;
      case CIL_V4SI: builtin = CIL32_GCC_V4SI_OR; break;
      case CIL_V8HI: builtin = CIL32_GCC_V8HI_OR; break;
      case CIL_V16QI: builtin = CIL32_GCC_V16QI_OR; break;
      default:
        gcc_unreachable ();
      }
  else
    switch (type)
      {
        /* No 8-byte support in Mono.Simd. Error/warning?  */

      case CIL_V2DI: builtin = CIL32_MONO_V2DI_OR; break;
      case CIL_V4SI: builtin = CIL32_MONO_V4SI_OR; break;
      case CIL_V8HI: builtin = CIL32_MONO_V8HI_OR; break;
      case CIL_V16QI: builtin = CIL32_MONO_V16QI_OR; break;
      default:
        gcc_unreachable ();
      }

  return cil_build_call (cil32_builtins[builtin]);
}

/* Lower a generic CIL vector logical XOR on the vector type represented by
   TYPE to a type-specific vector logical XOR depending on the vector type.
   Return the replacement statement.  */

static cil_stmt
lower_cil_vector_xor (cil_type_t type)
{
  enum cil32_builtin builtin;

  if (simd_type == GCC_SIMD)
    switch (type)
      {
      case CIL_V2SI: builtin = CIL32_GCC_V2SI_XOR; break;
      case CIL_V4HI: builtin = CIL32_GCC_V4HI_XOR; break;
      case CIL_V8QI: builtin = CIL32_GCC_V8QI_XOR; break;

      case CIL_V2DI: builtin = CIL32_GCC_V2DI_XOR; break;
      case CIL_V4SI: builtin = CIL32_GCC_V4SI_XOR; break;
      case CIL_V8HI: builtin = CIL32_GCC_V8HI_XOR; break;
      case CIL_V16QI: builtin = CIL32_GCC_V16QI_XOR; break;
      default:
        gcc_unreachable ();
      }
  else
    switch (type)
      {
        /* No 8-byte support in Mono.Simd. Error/warning?  */

      case CIL_V2DI: builtin = CIL32_MONO_V2DI_XOR; break;
      case CIL_V4SI: builtin = CIL32_MONO_V4SI_XOR; break;
      case CIL_V8HI: builtin = CIL32_MONO_V8HI_XOR; break;
      case CIL_V16QI: builtin = CIL32_MONO_V16QI_XOR; break;
      default:
        gcc_unreachable ();
      }

  return cil_build_call (cil32_builtins[builtin]);
}

/******************************************************************************
 * Helper functions used in other passes                                      *
 ******************************************************************************/

void
cil_lower_init (cil_seq init_seq)
{
  cil_stmt_iterator csi = csi_start (init_seq);

  while (!csi_end_p (csi))
    {
      cil_stmt stmt = csi_stmt (csi);
      enum cil_opcode opcode = cil_opcode (stmt);

      switch (opcode)
	{
	case CIL_VEC_CTOR:
	  stmt = lower_cil_vector_ctor (stmt);
	  csi_replace (&csi, stmt);
	  break;

	case CIL_LDVEC:
	  stmt = lower_cil_ldvec (stmt);
	  csi_replace (&csi, stmt);
	  break;

	case CIL_STVEC:
	  stmt = lower_cil_stvec (stmt);
	  csi_replace (&csi, stmt);
	  break;

	default:
	  ;
	}

      csi_next (&csi);
    }
}

/******************************************************************************
 * Entry point of the CIL lowering pass.                                      *
 ******************************************************************************/

/* This function scans the IR and looks for specific opcodes related to vector
   types that need to be lowered. These opcodes are the constructor, loads and
   stores, and arithmetic.  */
static unsigned int
lower_cil (void)
{
  basic_block bb;
  cil_stmt_iterator csi;
  cil_bb_stacks bbs;
  cil_stack stack;
  cil_type_t top;

  /* We need to compute the stack to be able to catch arithmetic nodes
     operating on vector types.  */
  bbs = cil_bb_stacks_create ();

  FOR_EACH_BB (bb)
    {
      stack = cil_stack_for_bb (bbs, bb);
      csi = csi_start_bb (bb);

      while (!csi_end_p (csi))
	{
	  cil_stmt stmt = csi_stmt (csi);
	  enum cil_opcode opcode = cil_opcode (stmt);

	  switch (opcode)
	    {
	    case CIL_VEC_CTOR:
	      stmt = lower_cil_vector_ctor (stmt);
	      csi_replace (&csi, stmt);
	      cil_stack_after_stmt (stack, stmt);
	      break;

	    case CIL_LDVEC:
	      /* Process the stack now. After expansion, the return type is
		 going to be an opaque valuetype.  */
	      cil_stack_after_stmt (stack, stmt);
	      stmt = lower_cil_ldvec (stmt);
	      csi_replace (&csi, stmt);
	      break;

	    case CIL_STVEC:
	      stmt = lower_cil_stvec (stmt);
	      csi_replace (&csi, stmt);
	      cil_stack_after_stmt (stack, stmt);
	      break;

	    case CIL_ADD:
	      top = cil_stack_top (stack);
	      cil_stack_after_stmt (stack, stmt);

	      if (cil_vector_p (top))
		csi_replace (&csi, lower_cil_vector_add (top));

	      break;

	    case CIL_SUB:
	      top = cil_stack_top (stack);
	      cil_stack_after_stmt (stack, stmt);

	      if (cil_vector_p (top))
		csi_replace (&csi, lower_cil_vector_sub (top));

	      break;

	    case CIL_AND:
	      top = cil_stack_top (stack);
	      cil_stack_after_stmt (stack, stmt);

	      if (cil_vector_p (top))
		csi_replace (&csi, lower_cil_vector_and (top));

	      break;

	    case CIL_OR:
	      top = cil_stack_top (stack);
	      cil_stack_after_stmt (stack, stmt);

	      if (cil_vector_p (top))
		csi_replace (&csi, lower_cil_vector_or (top));

	      break;

	    case CIL_XOR:
	      top = cil_stack_top (stack);
	      cil_stack_after_stmt (stack, stmt);

	      if (cil_vector_p (top))
		csi_replace (&csi, lower_cil_vector_xor (top));

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

struct gimple_opt_pass pass_lower_cil =
{
 {
  GIMPLE_PASS,                          /* type */
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
  0,                                    /* todo_flags_start */
  0                                     /* todo_flags_finish */
 }
};

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
