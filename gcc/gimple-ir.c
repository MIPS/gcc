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
const char *const gs_code_name[] = {
#include "gimple.def"
};
#undef DEFGSCODE

/* Gimple tuple constructors.  */

/* Construct a GS_RETURN statement.
   RESULT_DECL_P is non-zero if using RESULT_DECL.
   RETVAL is the return value.  */

gimple
gs_build_return (bool result_decl_p, tree retval)
{
  gimple p = ggc_alloc_cleared (sizeof (struct gimple_statement_return));

  GS_CODE (p) = GS_RETURN;
  GS_SUBCODE_FLAGS (p) = (int) result_decl_p;
  gs_return_set_retval (p, retval);
  return p;
}

/* Construct a GS_CALL statement.

   FUNC is the function decl.
   NARGS is the number of arguments.
   The ... are the arguments.  */

gimple
gs_build_call (tree func, int nargs, ...)
{
  va_list ap;
  gimple p;
  int i;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_call)
                         + sizeof (tree) * (nargs - 1));

  GS_CODE (p) = GS_CALL;
  GS_SUBCODE_FLAGS (p) = 0;
  gs_call_set_nargs (p, nargs);
  gs_call_set_fn (p, func);

  va_start (ap, nargs);
  for (i = 0; i < nargs; ++i)
    gs_call_set_arg (p, i, va_arg (ap, tree));
  va_end (ap);

  return p;
}

/* Construct a GS_ASSIGN statement.
   LHS of the assignment.
   RHS of the assignment which can be unary or binary.  */

gimple
gs_build_assign (tree lhs, tree rhs)
{
  gimple p;
  enum gimple_statement_structure_enum gss;

  gss = gss_for_assign (TREE_CODE (rhs));
  switch (gss)
    {
    case GSS_ASSIGN_BINARY:
      p = ggc_alloc_cleared (sizeof (struct gimple_statement_assign_binary));
      GS_CODE (p) = GS_ASSIGN;
      GS_SUBCODE_FLAGS (p) = TREE_CODE (rhs);
      gs_assign_set_lhs (p, lhs);
      gs_assign_binary_set_rhs1 (p, TREE_OPERAND (rhs, 0));
      gs_assign_binary_set_rhs2 (p, TREE_OPERAND (rhs, 1));
      break;

    case GSS_ASSIGN_UNARY_REG:
      p = ggc_alloc_cleared (sizeof (struct gimple_statement_assign_unary_reg));
      GS_CODE (p) = GS_ASSIGN;
      GS_SUBCODE_FLAGS (p) = TREE_CODE (rhs);
      gs_assign_set_lhs (p, lhs);
      if (IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (TREE_CODE (rhs))))
	gs_assign_unary_set_rhs (p, TREE_OPERAND (rhs, 0));
      else
	gs_assign_unary_set_rhs (p, rhs);
      break;

    case GSS_ASSIGN_UNARY_MEM:
      p = ggc_alloc_cleared (sizeof (struct gimple_statement_assign_unary_mem));
      GS_CODE (p) = GS_ASSIGN;
      GS_SUBCODE_FLAGS (p) = TREE_CODE (rhs);
      gs_assign_set_lhs (p, lhs);
      if (IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (TREE_CODE (rhs))))
        gs_assign_unary_set_rhs (p, TREE_OPERAND (rhs, 0));
      else
        gs_assign_unary_set_rhs (p, rhs);

      break;

    default:
      gcc_unreachable ();
    }

  return p;
}


/* Given a CODE for the RHS of a GS_ASSIGN, return the GSS enum for it.  */

enum gimple_statement_structure_enum
gss_for_assign (enum tree_code code)
{
  enum tree_code_class class = TREE_CODE_CLASS (code);

  if (class == tcc_binary || class == tcc_comparison)
    return GSS_ASSIGN_BINARY;

  /* There can be 3 types of unary operations:

     SYM = <constant>		<== GSS_ASSIGN_UNARY_REG
     SYM = SSA_NAME		<== GSS_ASSIGN_UNARY_REG
     SYM = SYM2			<== GSS_ASSIGN_UNARY_MEM
     SYM = UNARY_OP SYM2	<== GSS_ASSIGN_UNARY_MEM
  */

  if (class == tcc_constant || code == SSA_NAME)
    return GSS_ASSIGN_UNARY_REG;

  /* Must be class == tcc_unary.  */
  return GSS_ASSIGN_UNARY_MEM;
}

/* Construct a GS_COND statement.  Compare using PRED the LHS and the RHS,
   if true goto T_LABEL, else goto F_LABEL.  */

gimple
gs_build_cond (tree lhs, enum gs_cond pred, tree rhs, struct gimple_statement_label* t_label,
               struct gimple_statement_label* f_label)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_cond));
  GS_CODE (p) = GS_COND;
  GS_SUBCODE_FLAGS(p) = pred;
  gs_cond_set_lhs (p, lhs);
  gs_cond_set_rhs (p, rhs);
  gs_cond_set_true_label (p, t_label);
  gs_cond_set_false_label (p, f_label);

  return p;
}

/* Construct a GS_LABEL statement for LABEL.  */

gimple
gs_build_label (tree label)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_label));
  GS_CODE (p) = GS_LABEL;
  gs_label_set_label (p, label);

  return p;
}

/* Construct a GS_GOTO statement to DEST.  */

gimple
gs_build_goto (tree dest)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_goto));
  GS_CODE (p) = GS_GOTO;
  gs_goto_set_dest (p, dest);

  return p;
}

/* Construct a GS_NOP statement.  */

gimple 
gs_build_nop (void)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_base));
  GS_CODE (p) = GS_NOP;

  return p;
}

/* Construct a GS_BIND statement, where VARS are the variables, in BODY.  */

gimple
gs_build_bind (tree vars, gs_seq body)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_bind));
  GS_CODE (p) = GS_BIND;
  gs_bind_set_vars (p, vars);
  gs_bind_set_body (p, body);

  return p;
}

/* Construct a GS_ASM statement.

   STRING is the assembly code.
   NI is the number of register inputs.
   NO is the number of register outputs.
   NC is the number of clobbered registers.
   ...s are trees for each input, output and clobber.  */

gimple
gs_build_asm (const char* string, unsigned ni, unsigned no, unsigned nc,...)
{
  gimple p;
  unsigned i;
  va_list ap;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_asm)
                         + sizeof (tree) * (ni + no + nc) - 1);
  GS_CODE (p) = GS_ASM;
  gs_asm_set_ni (p, ni);
  gs_asm_set_no (p, no);
  gs_asm_set_nc (p, nc);
  gs_asm_set_string (p, string);
  
  va_start (ap, nc);
  for (i = 0; i < ni; i++)
    gs_asm_set_in_op (p, i, va_arg (ap, tree));
  for (i = 0; i < no; i++)
    gs_asm_set_out_op (p, i, va_arg (ap, tree));
  for (i = 0; i < nc; i++)
    gs_asm_set_clobber_op (p, i, va_arg (ap, tree));
  va_end (ap);
  
  return p;
}

/* Construct a GS_CATCH statement.

  TYPES are the catch types.
  HANDLER is the exception handler.  */

gimple
gs_build_catch (tree types, gimple handler)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_catch));
  GS_CODE (p) = GS_CATCH;
  gs_catch_set_types (p, types);
  gs_catch_set_handler (p, handler);

  return p;
}

/* Construct a GS_EH_FILTER statement.
 
  TYPES is the filter's types.
  FAILURE is the filters failure action.  */

gimple
gs_build_eh_filter (tree types, gimple failure)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_eh_filter));
  GS_CODE (p) = GS_EH_FILTER;
  gs_eh_filter_set_types (p, types);
  gs_eh_filter_set_failure (p, failure);

  return p;
}

/* Construct a GS_TRY statement.
   
   EVAL is the expression to evaluate.
   CLEANUP is the cleanup expression.
   CATCH, does this try catch?
   FINALLY, does this try have a finally?  */

gimple
gs_build_try (gimple eval, gimple cleanup, bool catch, bool finally)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_try));
  GS_CODE (p) = GS_TRY;
  gs_try_set_eval (p, eval);
  gs_try_set_cleanup (p, cleanup);
  if(catch)
    GS_SUBCODE_FLAGS(p) |= GS_TRY_CATCH;
  if(finally)
    GS_SUBCODE_FLAGS(p) |= GS_TRY_FINALLY;

  return p;
}

/* Construct a GS_PHI statement, with a CAPACITY, a RESULT, and NARGS
   phi_arg_d * arguments from ...  */

gimple
gs_build_phi (unsigned capacity, unsigned nargs, tree result, ...)
{
  gimple p;
  unsigned int i;
  va_list va;
  p = ggc_alloc_cleared (sizeof (struct gimple_statement_phi)
      + (sizeof(struct phi_arg_d) * (nargs - 1)) );
  
  GS_CODE (p) = GS_PHI;
  gs_phi_set_capacity (p, capacity);
  gs_phi_set_nargs (p, nargs);
  gs_phi_set_result (p, result);
  
  va_start (va, result);
  for (i = 0; i < nargs; i++)
    {
      struct phi_arg_d * phid = va_arg (va, struct phi_arg_d *);
      gs_phi_set_arg (p, i, phid);
    }
  va_end (va);
  
  return p;
}

/* Construct a GS_RESX statement, with a REGION.  */

gimple
gs_build_resx (int region)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_resx));
  GS_CODE (p) = GS_RESX;
  gs_resx_set_region (p, region);
  return p;
}

/* Construct a GS_SWITCH statement: for INDEX with the NLABLES labels 
   in ..., excluding the DEFAULT_LABEL.  */

gimple 
gs_build_switch (unsigned int nlabels, tree index, tree default_label, ...)
{
  gimple p;
  unsigned int i;
  va_list al;
  
  /* nlables + 1 default - 1 extra from struct */ 
  p = ggc_alloc_cleared ( sizeof (struct gimple_statement_switch)
                          + (sizeof (tree) * (nlabels) ) );
  GS_CODE (p) = GS_SWITCH;

  gs_switch_set_nlabels (p, nlabels);
  gs_switch_set_index (p, index);
  gs_switch_set_default_label (p, default_label);
  va_start (al, default_label);
  for (i = 0; i < nlabels; i++)
    gs_switch_set_label (p, i, va_arg (al, tree));
  va_end (al);
  return p;
}

/* Construct a GS_OMP_CRITICAL statement at BODY named NAME.  */

gimple 
gs_omp_build_critical (struct gs_sequence body, tree name)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp_critical));
  GS_CODE (p) = GS_OMP_CRITICAL;
  gs_omp_critical_set_name (p, name);
  gs_omp_set_body (p, body);

  return p;
}

/* Construct a GS_OMP_FOR statement with a BODY and PRE_BODY, with a predicate
   OMP_FOR_COND, CLAUSES, index variable INDEX, initial value INITIAL, final 
   value FINAL, and increment of INCR.   */

gimple 
gs_omp_build_for (struct gs_sequence body, tree clauses, tree index, 
                  tree initial, tree final, tree incr, 
                  struct gs_sequence pre_body, 
                  enum gs_cond omp_for_cond)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp_for));
  GS_CODE (p) = GS_OMP_FOR;
  gs_omp_set_body (p, body);
  gs_omp_for_set_clauses (p, clauses);
  gs_omp_for_set_index (p, index);
  gs_omp_for_set_initial (p, initial);
  gs_omp_for_set_final (p, final);
  gs_omp_for_set_incr (p, incr);
  gs_omp_for_set_pre_body (p, pre_body);
  gs_assign_omp_for_cond (p, omp_for_cond);

  return p;
}

/* Construct a GS_OMP_PARRELLE statement for BODY, with CLAUSES, a child 
   function CHILD_FN, and shared data argument DATA_ARG.  */

gimple 
gs_omp_build_parallel (struct gs_sequence body, tree clauses, tree child_fn, 
                       tree data_arg)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp_parallel));
  GS_CODE (p) = GS_OMP_PARALLEL;
  gs_omp_set_body (p, body);
  gs_omp_parallel_set_clauses (p, clauses);
  gs_omp_parallel_set_child_fn (p, child_fn);
  gs_omp_parallel_set_data_arg (p, data_arg);

  return p;
}

/* Construct a GS_OMP_SECTION statement for BODY.  */

gimple 
gs_omp_build_section (struct gs_sequence body)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp));
  GS_CODE (p) = GS_OMP_SECTION;
  gs_omp_set_body (p, body);

  return p;
}
/* Construct a GS_OMP_MASTER statement for BODY.  */

gimple 
gs_omp_build_master (struct gs_sequence body)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp));
  GS_CODE (p) = GS_OMP_MASTER;
  gs_omp_set_body (p, body);

  return p;
}
/* Construct a GS_OMP_CONTINUE statement for BODY.  */

gimple 
gs_omp_build_continue (struct gs_sequence body)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp));
  GS_CODE (p) = GS_OMP_CONTINUE;
  gs_omp_set_body (p, body);

  return p;
}
/* Construct a GS_OMP_ORDERED statement for BODY.  */

gimple 
gs_omp_build_ordered (struct gs_sequence body)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp));
  GS_CODE (p) = GS_OMP_ORDERED;
  gs_omp_set_body (p, body);

  return p;
}

/* Construct a GS_OMP_RETURN statement for BODY.  */

gimple 
gs_omp_build_return (struct gs_sequence body)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp));
  GS_CODE (p) = GS_OMP_RETURN;
  gs_omp_set_body (p, body);

  return p;
}

/* Construct a GS_OMP_SECTIONS statement for BODY with CLAUSES.  */

gimple 
gs_omp_build_sections (struct gs_sequence body, tree clauses)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp_sections));
  GS_CODE (p) = GS_OMP_SECTIONS;
  gs_omp_set_body (p, body);
  gs_omp_sections_set_clauses (p, clauses);

  return p;
}

/* Construct a GS_OMP_SINGLE statement for BODY with CLAUSES.  */

gimple 
gs_omp_build_single (struct gs_sequence body, tree clauses)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp_single));
  GS_CODE (p) = GS_OMP_SINGLE;
  gs_omp_set_body (p, body);
  gs_omp_single_set_clauses (p, clauses);

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
    case GS_ASSIGN:		return gss_for_assign (subcode);
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
    default:
      gcc_unreachable ();
      return GSS_BASE;
    }
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
  gimple last;

  /* Make sure this stmt is not part of another chain.  */
  gcc_assert (GS_PREV (gs) == NULL);

  for (last = gs; GS_NEXT (last) != NULL; last = GS_NEXT (last))
    ;

  if (GS_SEQ_FIRST (seq) == NULL)
    {
      GS_SEQ_FIRST (seq) = gs;
      GS_SEQ_LAST (seq) = last;
    }
  else
    {
      GS_PREV (gs) = GS_SEQ_LAST (seq);
      GS_NEXT (GS_SEQ_LAST (seq)) = gs;
      GS_SEQ_LAST (seq) = last;
    }
}
