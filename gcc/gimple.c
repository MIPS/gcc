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
#include "gimple.h"
#include "diagnostic.h"

#define DEFGSCODE(SYM, NAME)	NAME,
const char *const gimple_code_name[] = {
#include "gimple.def"
};
#undef DEFGSCODE

/* Pointer map to store the sequence of GIMPLE statements
   corresponding to each function.  For every FUNCTION_DECL FN, the
   sequence of GIMPLE statements corresponding to FN are stored in
   gimple_body (FN).  */
static struct pointer_map_t *gimple_bodies = NULL;

/* Gimple tuple constructors.  */

/* Note: Any constructor taking a ``gimple_seq'' as a parameter, can be passed
   a NULL to start with an empty sequence.  */

/* Construct a GIMPLE_RETURN statement.

   RESULT_DECL_P is non-zero if using RESULT_DECL.
   RETVAL is the return value.  */

gimple
gimple_build_return (bool result_decl_p, tree retval)
{
  gimple p = ggc_alloc_cleared (sizeof (struct gimple_statement_return));

  GIMPLE_CODE (p) = GIMPLE_RETURN;
  GIMPLE_SUBCODE_FLAGS (p) = (int) result_decl_p;
  gimple_return_set_retval (p, retval);
  return p;
}

/* Helper for gimple_build_call and gimple_build_call_vec.  Build the basic
   components of a GIMPLE_CALL statement to function FN with NARGS
   arguments.  */

static inline gimple
gimple_build_call_1 (tree fn, size_t nargs)
{
  gimple gs = ggc_alloc_cleared (sizeof (struct gimple_statement_call)
                                 + sizeof (tree) * (nargs - 1));

  GIMPLE_CODE (gs) = GIMPLE_CALL;
  GIMPLE_SUBCODE_FLAGS (gs) = 0;
  gs->gimple_call.nargs = nargs;
  gs->gimple_call.fn = fn;

  return gs;
}


/* Build a GIMPLE_CALL statement to function FN with the arguments
   specified in vector ARGS.  */

gimple
gimple_build_call_vec (tree fn, VEC(tree, gc) *args)
{
  size_t i;
  size_t nargs = VEC_length (tree, args);
  gimple call = gimple_build_call_1 (fn, nargs);

  for (i = 0; i < nargs; i++)
    gimple_call_set_arg (call, i, VEC_index (tree, args, i));

  return call;
}


/* Build a GIMPLE_CALL statement to function FN.  NARGS is the number of
   arguments.  The ... are the arguments.  */

gimple
gimple_build_call (tree fn, size_t nargs, ...)
{
  va_list ap;
  gimple call;
  size_t i;

  call = gimple_build_call_1 (fn, nargs);

  va_start (ap, nargs);
  for (i = 0; i < nargs; i++)
    gimple_call_set_arg (call, i, va_arg (ap, tree));
  va_end (ap);

  return call;
}

/* Construct a GIMPLE_ASSIGN statement.

   LHS of the assignment.
   RHS of the assignment which can be unary or binary.  */

gimple
gimple_build_assign (tree lhs, tree rhs)
{
  gimple p;
  enum gimple_statement_structure_enum gss;

  gss = gss_for_assign (TREE_CODE (rhs));
  switch (gss)
    {
    case GSS_ASSIGN_BINARY:
      p = ggc_alloc_cleared (sizeof (struct gimple_statement_assign_binary));
      GIMPLE_CODE (p) = GIMPLE_ASSIGN;
      GIMPLE_SUBCODE_FLAGS (p) = TREE_CODE (rhs);
      gimple_assign_set_lhs (p, lhs);
      gimple_assign_binary_set_rhs1 (p, TREE_OPERAND (rhs, 0));
      gimple_assign_binary_set_rhs2 (p, TREE_OPERAND (rhs, 1));
      break;

    case GSS_ASSIGN_UNARY_REG:
      p = ggc_alloc_cleared (sizeof (struct gimple_statement_assign_unary_reg));
      GIMPLE_CODE (p) = GIMPLE_ASSIGN;
      GIMPLE_SUBCODE_FLAGS (p) = TREE_CODE (rhs);
      gimple_assign_set_lhs (p, lhs);
      if (IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (TREE_CODE (rhs))))
	gimple_assign_unary_set_rhs (p, TREE_OPERAND (rhs, 0));
      else
	gimple_assign_unary_set_rhs (p, rhs);
      break;

    case GSS_ASSIGN_UNARY_MEM:
      p = ggc_alloc_cleared (sizeof (struct gimple_statement_assign_unary_mem));
      GIMPLE_CODE (p) = GIMPLE_ASSIGN;
      GIMPLE_SUBCODE_FLAGS (p) = TREE_CODE (rhs);
      gimple_assign_set_lhs (p, lhs);
      if (IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (TREE_CODE (rhs))))
        gimple_assign_unary_set_rhs (p, TREE_OPERAND (rhs, 0));
      else
        gimple_assign_unary_set_rhs (p, rhs);

      break;

    default:
      gcc_unreachable ();
    }

  return p;
}


/* Given a CODE for the RHS of a GIMPLE_ASSIGN, return the GSS enum for it.  */

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

/* Construct a GIMPLE_COND statement.

   PRED is the condition used to compare LHS and the RHS.
   T_LABEL is the label to jump to if the condition is true.
   F_LABEL is teh label to jump to otherwise.  */

gimple
gimple_build_cond (enum gimple_cond pred, tree lhs, tree rhs,
		   tree t_label, tree f_label)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_cond));
  GIMPLE_CODE (p) = GIMPLE_COND;
  GIMPLE_SUBCODE_FLAGS (p) = pred;
  gimple_cond_set_lhs (p, lhs);
  gimple_cond_set_rhs (p, rhs);
  gimple_cond_set_true_label (p, t_label);
  gimple_cond_set_false_label (p, f_label);

  return p;
}

/* Invert the condition of a GIMPLE_COND by swapping its labels.  */

void
gimple_cond_invert (gimple g)
{
  tree tmp;

  tmp = gimple_cond_true_label (g);
  gimple_cond_set_true_label (g, gimple_cond_false_label (g));
  gimple_cond_set_false_label (g, tmp);
}

/* Construct a GIMPLE_LABEL statement for LABEL.  */

gimple
gimple_build_label (tree label)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_label));
  GIMPLE_CODE (p) = GIMPLE_LABEL;
  gimple_label_set_label (p, label);

  return p;
}

/* Construct a GIMPLE_GOTO statement to DEST.  */

gimple
gimple_build_goto (tree dest)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_goto));
  GIMPLE_CODE (p) = GIMPLE_GOTO;
  gimple_goto_set_dest (p, dest);

  return p;
}

/* Construct a GIMPLE_NOP statement.  */

gimple 
gimple_build_nop (void)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_base));
  GIMPLE_CODE (p) = GIMPLE_NOP;

  return p;
}

/* Construct a GIMPLE_BIND statement.

   VARS are the variables in BODY.  */

gimple
gimple_build_bind (tree vars, gimple_seq body)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_bind));
  GIMPLE_CODE (p) = GIMPLE_BIND;
  gimple_bind_set_vars (p, vars);
  if (body)
    gimple_bind_set_body (p, body);

  return p;
}

/* Construct a GIMPLE_ASM statement.

   STRING is the assembly code.
   NINPUT is the number of register inputs.
   NOUTPUT is the number of register outputs.
   NCLOBBERED is the number of clobbered registers.
   ... are trees for each input, output and clobbered register.  */

gimple
gimple_build_asm (const char *string, unsigned ninputs, unsigned noutputs, 
              unsigned nclobbered, ...)
{
  gimple p;
  unsigned i;
  va_list ap;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_asm)
                         + sizeof (tree) * (ninputs + noutputs + nclobbered - 1));
  GIMPLE_CODE (p) = GIMPLE_ASM;
  gimple_asm_set_ninputs (p, ninputs);
  gimple_asm_set_noutputs (p, noutputs);
  gimple_asm_set_nclobbered (p, nclobbered);
  gimple_asm_set_string (p, string);
  
  va_start (ap, nclobbered);
  for (i = 0; i < ninputs; i++)
    gimple_asm_set_input_op (p, i, va_arg (ap, tree));
  for (i = 0; i < noutputs; i++)
    gimple_asm_set_output_op (p, i, va_arg (ap, tree));
  for (i = 0; i < nclobbered; i++)
    gimple_asm_set_clobber_op (p, i, va_arg (ap, tree));
  va_end (ap);
  
  return p;
}

/* Construct a GIMPLE_CATCH statement.

  TYPES are the catch types.
  HANDLER is the exception handler.  */

gimple
gimple_build_catch (tree types, gimple_seq handler)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_catch));
  GIMPLE_CODE (p) = GIMPLE_CATCH;
  gimple_catch_set_types (p, types);
  if (handler)
    gimple_catch_set_handler (p, handler);

  return p;
}

/* Construct a GIMPLE_EH_FILTER statement.

   TYPES are the filter's types.
   FAILURE is the filter's failure action.  */

gimple
gimple_build_eh_filter (tree types, gimple_seq failure)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_eh_filter));
  GIMPLE_CODE (p) = GIMPLE_EH_FILTER;
  gimple_eh_filter_set_types (p, types);
  if (failure)
    gimple_eh_filter_set_failure (p, failure);

  return p;
}

/* Construct a GIMPLE_TRY statement.

   EVAL is the expression to evaluate.
   CLEANUP is the cleanup expression.
   CATCH_FINALLY is either GIMPLE_TRY_CATCH or GIMPLE_TRY_FINALLY depending on
   whether this is a try/catch or a try/finally respectively.  */

gimple
gimple_build_try (gimple_seq eval, gimple_seq cleanup,
    		  unsigned int catch_finally)
{
  gimple p;

  gcc_assert (catch_finally == GIMPLE_TRY_CATCH
	      || catch_finally == GIMPLE_TRY_FINALLY);
  p = ggc_alloc_cleared (sizeof (struct gimple_statement_try));
  GIMPLE_CODE (p) = GIMPLE_TRY;
  if (eval)
    gimple_try_set_eval (p, eval);
  if (cleanup)
    gimple_try_set_cleanup (p, cleanup);
  GIMPLE_SUBCODE_FLAGS (p) = catch_finally;

  return p;
}

/* Construct a GIMPLE_PHI statement.

   CAPACITY is the max number of args this node can have (for later reuse).
   RESULT the 
   NARGS is the number of phi_arg_d's in ..., representing the number of
   incomming edges in this phi node.
   ... phi_arg_d* for the incomming edges to this node.  */

gimple
gimple_build_phi (unsigned capacity, unsigned nargs, tree result, ...)
{
  gimple p;
  unsigned int i;
  va_list va;
  p = ggc_alloc_cleared (sizeof (struct gimple_statement_phi)
      + (sizeof (struct phi_arg_d) * (nargs - 1)) );
  
  GIMPLE_CODE (p) = GIMPLE_PHI;
  gimple_phi_set_capacity (p, capacity);
  gimple_phi_set_nargs (p, nargs);
  gimple_phi_set_result (p, result);
  
  va_start (va, result);
  for (i = 0; i < nargs; i++)
    {
      struct phi_arg_d* phid = va_arg (va, struct phi_arg_d*);
      gimple_phi_set_arg (p, i, phid);
    }
  va_end (va);
  
  return p;
}

/* Construct a GIMPLE_RESX statement.

   REGION is the region number from which this resx causes control flow to 
   leave.  */

gimple
gimple_build_resx (int region)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_resx));
  GIMPLE_CODE (p) = GIMPLE_RESX;
  gimple_resx_set_region (p, region);
  return p;
}


/* The helper for constructing a gimple switch statement.
   INDEX is the switch's index.
   NLABELS is the number of labels in the switch excluding the default.
   DEFAULT_LABEL is the default label for the switch statement.  */

static inline gimple 
gimple_build_switch_1 (unsigned int nlabels, tree index, tree default_label)
{
  gimple p;
  unsigned int n_all_labels;
  /*  We count the default in the number of labels.  */
  n_all_labels = nlabels + 1;
  /* total labels - 1 extra from struct.  */
  p = ggc_alloc_cleared (sizeof (struct gimple_statement_switch)
                         + sizeof (tree) * (n_all_labels - 1));
  GIMPLE_CODE (p) = GIMPLE_SWITCH;

  gimple_switch_set_nlabels (p, n_all_labels);
  gimple_switch_set_index (p, index);
  gimple_switch_set_default_label (p, default_label);
  
  return p;
}


/* Construct a GIMPLE_SWITCH statement.

   INDEX is the switch's index.
   NLABELS is the number of labels in the switch excluding the DEFAULT_LABEL. 
   ... are the labels excluding the default.  */

gimple 
gimple_build_switch (unsigned int nlabels, tree index, tree default_label, ...)
{
  va_list al;
  unsigned int i;

  gimple p = gimple_build_switch_1 (nlabels, index, default_label);
  /*  Put labels in labels[1 - (nlables + 1)].
     Default label is in labels[0].  */
  va_start (al, default_label);
  for (i = 1; i <= nlabels; i++)
    gimple_switch_set_label (p, i, va_arg (al, tree));
  va_end (al);

  return p;
}


/* Construct a GIMPLE_SWITCH statement.

   INDEX is the switch's index.
   NLABELS is the number of labels in the switch excluding the default. 
   ARGS is a vector of labels excluding the default.  */

gimple
gimple_build_switch_vec (tree index, tree default_label, VEC(tree, heap) *args)
{
  size_t i;
  size_t nlabels = VEC_length (tree, args);
  gimple p = gimple_build_switch_1 (nlabels, index, default_label);

  /*  Put labels in labels[1 - (nlables + 1)].
     Default label is in labels[0].  */
  for (i = 1; i <= nlabels; i++)
    gimple_switch_set_label (p, i, VEC_index (tree, args, i - 1));

  return p;
}


/* Construct a GIMPLE_OMP_CRITICAL statement.

   BODY is the sequence of statements for which only one thread can execute.
   NAME is optional identifier for this critical block.  */

gimple 
gimple_omp_build_critical (gimple_seq body, tree name)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp_critical));
  GIMPLE_CODE (p) = GIMPLE_OMP_CRITICAL;
  gimple_omp_critical_set_name (p, name);
  if (body)
    gimple_omp_set_body (p, body);

  return p;
}

/* Construct a GIMPLE_OMP_FOR statement.

   BODY is sequence of statements inside the for loop.
   CLAUSES, are any of the OMP loop construct's clauses: private, firstprivate, 
   lastprivate, reductions, ordered, schedule, and nowait.
   PRE_BODY is the sequence of statements that are loop invariant.
   INDEX is the index variable.
   INITIAL is the initial value of INDEX.
   FINAL is final value of INDEX.
   OMP_FOR_COND  the predicate used to compare INDEX and FINAL.
   INCR is the increment expression.  */

gimple
gimple_omp_build_for (gimple_seq body, tree clauses, tree index, 
                  tree initial, tree final, tree incr, 
                  gimple_seq pre_body, enum gimple_cond omp_for_cond)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp_for));
  GIMPLE_CODE (p) = GIMPLE_OMP_FOR;
  if (body)
    gimple_omp_set_body (p, body);
  gimple_omp_for_set_clauses (p, clauses);
  gimple_omp_for_set_index (p, index);
  gimple_omp_for_set_initial (p, initial);
  gimple_omp_for_set_final (p, final);
  gimple_omp_for_set_incr (p, incr);
  if (pre_body)
    gimple_omp_for_set_pre_body (p, pre_body);
  gimple_assign_omp_for_cond (p, omp_for_cond);

  return p;
}

/* Construct a GIMPLE_OMP_PARALLEL statement.

   BODY is sequence of statements which are executed in parallel.
   CLAUSES, are the OMP parallel construct's clauses.
   CHILD_FN is the function created for the parallel threads to execute.
   DATA_ARG are the shared data argument(s).  */

gimple 
gimple_omp_build_parallel (gimple_seq body, tree clauses, tree child_fn, 
                       tree data_arg)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp_parallel));
  GIMPLE_CODE (p) = GIMPLE_OMP_PARALLEL;
  if (body)
    gimple_omp_set_body (p, body);
  gimple_omp_parallel_set_clauses (p, clauses);
  gimple_omp_parallel_set_child_fn (p, child_fn);
  gimple_omp_parallel_set_data_arg (p, data_arg);

  return p;
}

/* Construct a GIMPLE_OMP_SECTION statement for a sections statement.

   BODY is the sequence of statements in the section.  */

gimple
gimple_omp_build_section (gimple_seq body)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp));
  GIMPLE_CODE (p) = GIMPLE_OMP_SECTION;
  if (body)
    gimple_omp_set_body (p, body);

  return p;
}
/* Construct a GIMPLE_OMP_MASTER statement.

   BODY is the sequence of statements to be executed by just the master.  */

gimple 
gimple_omp_build_master (gimple_seq body)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp));
  GIMPLE_CODE (p) = GIMPLE_OMP_MASTER;
  if (body)
    gimple_omp_set_body (p, body);

  return p;
}
/* Construct a GIMPLE_OMP_CONTINUE statement.
   FIXME tuples: BODY.  */

gimple 
gimple_omp_build_continue (gimple_seq body)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp));
  GIMPLE_CODE (p) = GIMPLE_OMP_CONTINUE;
  if (body)
    gimple_omp_set_body (p, body);

  return p;
}

/* Construct a GIMPLE_OMP_ORDERED statement.

   BODY is the sequence of statements inside a loop that will executed in
   sequence.  */

gimple 
gimple_omp_build_ordered (gimple_seq body)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp));
  GIMPLE_CODE (p) = GIMPLE_OMP_ORDERED;
  if (body)
    gimple_omp_set_body (p, body);

  return p;
}

/* Construct a GIMPLE_OMP_RETURN statement.
   WAIT_P is true if this is a non-waiting return.  */

gimple 
gimple_omp_build_return (bool wait_p)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp));
  GIMPLE_CODE (p) = GIMPLE_OMP_RETURN;
  if (wait_p)
    GIMPLE_SUBCODE_FLAGS(p) = OMP_RETURN_NOWAIT_FLAG;

  return p;
}

/* Construct a GIMPLE_OMP_SECTIONS statement.

   BODY is a sequence of section statements.
   CLAUSES are any of the OMP sections contsruct's clauses: private,
   firstprivate, lastprivate, reduction, and nowait.  */

gimple 
gimple_omp_build_sections (gimple_seq body, tree clauses)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp_sections));
  GIMPLE_CODE (p) = GIMPLE_OMP_SECTIONS;
  if (body)
    gimple_omp_set_body (p, body);
  gimple_omp_sections_set_clauses (p, clauses);

  return p;
}

/* Construct a GIMPLE_OMP_SINGLE statement.

   BODY is the sequence of statements that will be executed once.
   CLAUSES are any of the OMP single construct's clauses: private, firstprivate,
   copyprivate, nowait.  */

gimple 
gimple_omp_build_single (gimple_seq body, tree clauses)
{
  gimple p;

  p = ggc_alloc_cleared (sizeof (struct gimple_statement_omp_single));
  GIMPLE_CODE (p) = GIMPLE_OMP_SINGLE;
  if (body)
    gimple_omp_set_body (p, body);
  gimple_omp_single_set_clauses (p, clauses);

  return p;
}

/* Return which gimple structure is used by T.  The enums here are defined
   in gsstruct.def.  */

enum gimple_statement_structure_enum
gimple_statement_structure (gimple gs)
{
  unsigned int code = GIMPLE_CODE (gs);
  unsigned int subcode = GIMPLE_SUBCODE_FLAGS (gs);

  switch (code)
    {
    case GIMPLE_ASSIGN:		return gss_for_assign (subcode);
    case GIMPLE_ASM:		return GSS_ASM;
    case GIMPLE_BIND:		return GSS_BIND;
    case GIMPLE_CALL:		return GSS_CALL;
    case GIMPLE_CATCH:		return GSS_CATCH;
    case GIMPLE_COND:		return GSS_COND;
    case GIMPLE_EH_FILTER:		return GSS_EH_FILTER;
    case GIMPLE_GOTO:		return GSS_GOTO;
    case GIMPLE_LABEL:		return GSS_LABEL;
    case GIMPLE_NOP:		return GSS_BASE;
    case GIMPLE_PHI:		return GSS_PHI;
    case GIMPLE_RESX:		return GSS_RESX;
    case GIMPLE_RETURN:		return GSS_RETURN;
    case GIMPLE_SWITCH:		return GSS_SWITCH;
    case GIMPLE_TRY:		return GSS_TRY;
    case GIMPLE_OMP_CRITICAL:	return GSS_OMP_CRITICAL;
    case GIMPLE_OMP_FOR:		return GSS_OMP_FOR;
    case GIMPLE_OMP_CONTINUE:
    case GIMPLE_OMP_MASTER:		
    case GIMPLE_OMP_ORDERED:
    case GIMPLE_OMP_RETURN:
    case GIMPLE_OMP_SECTION:
				return GSS_OMP;
    case GIMPLE_OMP_PARALLEL:	return GSS_OMP_PARALLEL;
    case GIMPLE_OMP_SECTIONS:	return GSS_OMP_SECTIONS;
    case GIMPLE_OMP_SINGLE:		return GSS_OMP_SINGLE;
    default:
      gcc_unreachable ();
    }
}

#if defined ENABLE_TREE_CHECKING && (GCC_VERSION >= 2007)
/* Complain of a gimple type mismatch and die.  */

void
gimple_check_failed (const gimple gs, const char *file, int line,
	         const char *function, unsigned int code,
		 unsigned int subcode)
{
  internal_error ("gimple check: expected %s(%s), have %s(%s) in %s, at %s:%d",
      		  gimple_code_name[code],
		  tree_code_name[subcode],
		  gimple_code_name[GIMPLE_CODE (gs)],
		  tree_code_name[GIMPLE_SUBCODE_FLAGS (gs)],
		  function, trim_filename (file), line);
}
#endif /* ENABLE_TREE_CHECKING */


/* Link a gimple statement(s) to the end of the sequence SEQ.  */

void
gimple_add (gimple_seq seq, gimple gs)
{
  gimple last;

  /* Make sure this stmt is not part of another chain.  */
  gcc_assert (GIMPLE_PREV (gs) == NULL);

  for (last = gs; GIMPLE_NEXT (last) != NULL; last = GIMPLE_NEXT (last))
    ;

  if (gimple_seq_first (seq) == NULL)
    {
      gimple_seq_set_first (seq, gs);
      gimple_seq_set_last (seq, last);
    }
  else
    {
      GIMPLE_PREV (gs) = gimple_seq_last (seq);
      GIMPLE_NEXT (gimple_seq_last (seq)) = gs;
      gimple_seq_set_last (seq, last);
    }
}


/* Visit all the tuples in sequence SEQ, and apply FUNC to all the tree leaves
   in the tuples.  The trees in the tuples encountered will be walked with
   walk_tree().  FUNC, DATA, and PSET are as in walk_tree.

   You cannot use this function to rewrite trees, as the address of
   thre trees passed to walk_tree are local to this function.
   Besides, you shouldn't be rewriting trees this late in the
   game.  */

void
walk_seq_ops (gimple_seq seq, walk_tree_fn func, void *data,
	      struct pointer_set_t *pset)
{
  gimple_stmt_iterator gsi;

  for (gsi = gsi_start (seq); !gsi_end_p (gsi); gsi_next (&gsi))
    walk_tuple_ops (gsi_stmt (gsi), func, data, pset);
}


/* Helper function of walk_seq_ops.  Walks one tuple's trees.  The
   arguments are as in walk_seq_ops, except GS is the tuple to
   walk.  */
#define WALKIT(__this) leaf = (__this), walk_tree (&leaf, func, data, pset)
void
walk_tuple_ops (gimple gs, walk_tree_fn func, void *data,
		struct pointer_set_t *pset)
{
  unsigned int i;
  tree leaf;

  switch (GIMPLE_CODE (gs))
    {
    case GIMPLE_ASM:
      for (i = 0; i < gimple_asm_ninputs (gs); ++i)
	WALKIT (gimple_asm_input_op (gs, i));

      for (i = 0; i < gimple_asm_noutputs (gs); ++i)
	WALKIT (gimple_asm_output_op (gs, i));

      for (i = 0; i < gimple_asm_nclobbered (gs); ++i)
	WALKIT (gimple_asm_clobber_op (gs, i));
      break;

    case GIMPLE_ASSIGN:
      WALKIT (gimple_assign_operand (gs, 0));
      WALKIT (gimple_assign_operand (gs, 1));

      if (gss_for_assign (GIMPLE_SUBCODE_FLAGS (gs)) == GSS_ASSIGN_BINARY)
	WALKIT (gimple_assign_operand (gs, 2));
      break;

    case GIMPLE_BIND:
      WALKIT (gimple_bind_vars (gs));
      walk_seq_ops (gimple_bind_body (gs), func, data, pset);
      break;

    case GIMPLE_CALL:
      WALKIT (gimple_call_lhs (gs));
      WALKIT (gimple_call_fn (gs));
      WALKIT (gimple_call_chain (gs));
      for (i = 0; i < gimple_call_nargs (gs); ++i)
        WALKIT (gimple_call_arg (gs, i));
      break;

    case GIMPLE_CATCH:
      WALKIT (gimple_catch_types (gs));
      walk_seq_ops (gimple_catch_handler (gs), func, data, pset);
      break;

    case GIMPLE_COND:
      WALKIT (gimple_cond_lhs (gs));
      WALKIT (gimple_cond_rhs (gs));
      WALKIT (gimple_cond_true_label (gs));
      WALKIT (gimple_cond_false_label (gs));
      break;

    case GIMPLE_EH_FILTER:
      WALKIT (gimple_eh_filter_types (gs));
      walk_seq_ops (gimple_eh_filter_failure (gs), func, data, pset);
      break;

    case GIMPLE_GOTO:
      WALKIT (gimple_goto_dest (gs));
      break;

    case GIMPLE_LABEL:
      WALKIT (gimple_label_label (gs));
      break;

    case GIMPLE_PHI:
      WALKIT (gimple_phi_result (gs));
      break;

    case GIMPLE_RETURN:
      WALKIT (gimple_return_retval (gs));
      break;

    case GIMPLE_SWITCH:
      WALKIT (gimple_switch_index (gs));
      for (i = 0; i < gimple_switch_nlabels (gs); ++i)
	WALKIT (gimple_switch_label (gs, i));
      break;

    case GIMPLE_TRY:
      walk_seq_ops (gimple_try_eval (gs), func, data, pset);
      walk_seq_ops (gimple_try_cleanup (gs), func, data, pset);
      break;

    case GIMPLE_OMP_CRITICAL:
      walk_seq_ops (gimple_omp_body (gs), func, data, pset);
      WALKIT (gimple_omp_critical_name (gs));
      break;

      /* Just a body.  */
    case GIMPLE_OMP_CONTINUE:
    case GIMPLE_OMP_MASTER:
    case GIMPLE_OMP_ORDERED:
    case GIMPLE_OMP_SECTION:
      walk_seq_ops (gimple_omp_body (gs), func, data, pset);
      break;

    case GIMPLE_OMP_FOR:
      walk_seq_ops (gimple_omp_body (gs), func, data, pset);
      WALKIT (gimple_omp_for_clauses (gs));
      WALKIT (gimple_omp_for_index (gs));
      WALKIT (gimple_omp_for_initial (gs));
      WALKIT (gimple_omp_for_final (gs));
      WALKIT (gimple_omp_for_incr (gs));
      walk_seq_ops (gimple_omp_for_pre_body (gs), func, data, pset);
      break;

    case GIMPLE_OMP_PARALLEL:
      walk_seq_ops (gimple_omp_body (gs), func, data, pset);
      WALKIT (gimple_omp_parallel_clauses (gs));
      WALKIT (gimple_omp_parallel_child_fn (gs));
      WALKIT (gimple_omp_parallel_data_arg (gs));
      break;

    case GIMPLE_OMP_SECTIONS:
      walk_seq_ops (gimple_omp_body (gs), func, data, pset);
      WALKIT (gimple_omp_sections_clauses (gs));
      break;

    case GIMPLE_OMP_SINGLE:
      walk_seq_ops (gimple_omp_body (gs), func, data, pset);
      WALKIT (gimple_omp_single_clauses (gs));
      break;

      /* Tuples that do not have trees.  */
    case GIMPLE_NOP:
    case GIMPLE_RESX:
    case GIMPLE_OMP_RETURN:
      break;

    default:
      debug_gimple_stmt (gs);
      gcc_unreachable ();
      break;
    }
}


/* Set sequence SEQ to be the GIMPLE body for function FN.  */

void
set_gimple_body (tree fn, gimple_seq seq)
{
  void **slot;

  if (gimple_bodies == NULL)
    gimple_bodies = pointer_map_create ();

  slot = pointer_map_insert (gimple_bodies, fn);
  *slot = (void *) seq;
}
  

/* Return the body of GIMPLE statements for function FN.  */

gimple_seq
gimple_body (tree fn)
{
  void **slot;
  
  if (gimple_bodies && (slot = pointer_map_contains (gimple_bodies, fn)))
    return (gimple_seq) *slot;
  
  return NULL;
}
