/* Gimple IR support functions.

   Copyright 2007 Free Software Foundation, Inc.
   Contributed by Aldy Hernandez <aldyh@redhat.com>

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
#include "tree.h"
#include "ggc.h"
#include "errors.h"
#include "tree-gimple.h"
#include "gimple-ir.h"

#define DEFGSCODE(SYM, NAME)	NAME,
static const char *gs_code_name[] = {
#include "gs.def"
};
#undef DEFGSCODE

/* Gimple tuple constructors.  */

/* Construct a GS_RETURN statement.  */

gimple
gs_build_return (bool result_decl_p, tree retval)
{
  gimple p = ggc_alloc_cleared (sizeof (struct gimple_statement_return));

  GS_CODE (p) = GS_RETURN;
  GS_SUBCODE_FLAGS (p) = (int) result_decl_p;
  GS_RETURN_OPERAND_RETVAL (p) = retval;
  return p;
}

/* Return which gimple structure is used by T.  The enums here are defined
   in gsstruct.def.  */

enum gimple_statement_structure_enum
gimple_statement_structure (gimple gs)
{
  unsigned int code = GS_CODE (gs);
  unsigned int subcode = GS_SUBCODE_FLAGS (gs);

  switch (code)
    {
    case GS_ASSIGN:
      {
	enum tree_code_class class = TREE_CODE_CLASS (subcode);

	if (class == tcc_binary
	    || class == tcc_comparison)
	  return GSS_ASSIGN_BINARY;
	else 
	  {
	    /* There can be 3 types of unary operations:

		 SYM = <constant>	<== GSS_ASSIGN_UNARY_REG
		 SYM = SSA_NAME		<== GSS_ASSIGN_UNARY_REG
	         SYM = SYM2		<== GSS_ASSIGN_UNARY_MEM
		 SYM = UNARY_OP SYM2	<== GSS_ASSIGN_UNARY_MEM
	    */
	    if (class == tcc_constant || subcode == SSA_NAME)
	      return GSS_ASSIGN_UNARY_REG;

	    /* Must be class == tcc_unary.  */
	    return GSS_ASSIGN_UNARY_MEM;
	  }
      }
    case GS_ASM:		return GSS_ASM;
    case GS_BIND:		return GSS_BIND;
    case GS_CALL:		return GSS_CALL;
    case GS_CATCH:		return GSS_CATCH;
    case GS_COND:		return GSS_COND;
    case GS_EH_FILTER:		return GSS_EH_FILTER;
    case GS_GOTO:		return GSS_GOTO;
    case GS_LABEL:		return GSS_LABEL;
    case GS_NOP:		return GSS_BASE;
    case GS_PHI:		return GSS_PHI;
    case GS_RESX:		return GSS_RESX;
    case GS_RETURN:		return GSS_RETURN;
    case GS_SWITCH:		return GSS_SWITCH;
    case GS_TRY:		return GSS_TRY;
    case GS_OMP_CRITICAL:	return GSS_OMP_CRITICAL;
    case GS_OMP_FOR:		return GSS_OMP_FOR;
    case GS_OMP_CONTINUE:
    case GS_OMP_MASTER:		
    case GS_OMP_ORDERED:
    case GS_OMP_RETURN:
    case GS_OMP_SECTION:
				return GSS_OMP;
    case GS_OMP_PARALLEL:	return GSS_OMP_PARALLEL;
    case GS_OMP_SECTIONS:	return GSS_OMP_SECTIONS;
    case GS_OMP_SINGLE:		return GSS_OMP_SINGLE;
    default: ;
    }
  gcc_unreachable ();
  return GSS_BASE;
}

#if defined ENABLE_TREE_CHECKING && (GCC_VERSION >= 2007)
/* Complain of a gimple type mismatch and die.  */

void
gs_check_failed (const gimple gs, const char *file, int line,
	         const char *function, unsigned int code,
		 unsigned int subcode)
{
  internal_error ("gimple check: expected %s(%s), have %s(%s) in %s, at %s:%d",
      		  gs_code_name[code],
		  tree_code_name[subcode],
		  gs_code_name[GS_CODE (gs)],
		  tree_code_name[GS_SUBCODE_FLAGS (gs)],
		  function, trim_filename (file), line);
}
#endif /* ENABLE_TREE_CHECKING */

/* Link a gimple statement(s) to the end of the sequence SEQ.  */

void
gs_add (gimple gs, gs_seq seq)
{
  /* Make sure this stmt is not part of another chain.  */
  gcc_assert (GS_PREV (gs) == NULL);

  if (GS_SEQ_FIRST (seq) == NULL)
    {
      GS_SEQ_FIRST (seq) = gs;
      GS_SEQ_LAST (seq) = gs;
    }
  else
    {
      GS_PREV (gs) = GS_SEQ_LAST (seq);
      GS_NEXT (GS_SEQ_LAST (seq)) = gs;
      GS_SEQ_LAST (seq) = gs;
    }
}
