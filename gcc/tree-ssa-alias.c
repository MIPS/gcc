/* Alias analysis for trees.
   Copyright (C) 2004, 2005, 2006, 2007, 2008 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "timevar.h"
#include "expr.h"
#include "ggc.h"
#include "langhooks.h"
#include "flags.h"
#include "function.h"
#include "diagnostic.h"
#include "tree-dump.h"
#include "gimple.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "tree-pass.h"
#include "tree-ssa-structalias.h"
#include "convert.h"
#include "params.h"
#include "ipa-type-escape.h"
#include "vec.h"
#include "bitmap.h"
#include "vecprim.h"
#include "pointer-set.h"
#include "alloc-pool.h"
#include "tree-ssa-alias.h"

/* Broad overview of how alias analysis on gimple works:

   Statements clobbering or using memory are linked through the
   virtual operand factored use-def chain.  The virtual operand
   is unique per function, its symbol is accessible via gimple_vop (cfun).
   Virtual operands are used for efficiently walking memory statements
   in the gimple IL and are useful for things like value-numbering as
   a generation count for memory references.

   SSA_NAME pointers may have associated points-to information
   accessible via the SSA_NAME_PTR_INFO macro.  Flow-insensitive
   points-to information is (re-)computed by the TODO_rebuild_alias
   pass manager todo.  Points-to information is also used for more
   precise tracking of call-clobbered and call-used variables and
   related disambiguations.

   This file contains functions for disambiguating memory references
   and tools for walking of the gimple IL.  */


/* Return true, if PTR may point to a global variable.  */

bool
may_point_to_global_var (tree ptr)
{
  struct ptr_info_def *pi;

  /* If we end up with a pointer constant here that may point
     to global memory.  */
  if (TREE_CODE (ptr) != SSA_NAME)
    return true;

  pi = SSA_NAME_PTR_INFO (ptr);

  /* If we do not have points-to information for this variable,
     we have to punt.  */
  if (!pi)
    return true;

  return pi->pt.anything || pi->pt.nonlocal || pi->pt.vars_contains_global;
}

/* Return true if PTR may point to DECL.  */

bool
may_point_to_decl (tree ptr, tree decl)
{
  struct ptr_info_def *pi;

  /* ???  During SCCVN/PRE we can end up with *&x during valueizing
     operands.  Likewise we can end up with dereferencing constant
     pointers.  Just bail out in these cases for now.  */
  if (TREE_CODE (ptr) == ADDR_EXPR
      || TREE_CODE (ptr) == INTEGER_CST)
    return true;

  gcc_assert (TREE_CODE (ptr) == SSA_NAME
	      && (TREE_CODE (decl) == VAR_DECL
		  || TREE_CODE (decl) == PARM_DECL
		  || TREE_CODE (decl) == RESULT_DECL));

  /* Non-aliased variables can not be pointed to.  */
  if (!may_be_aliased (decl))
    return false;

  /* If we do not have useful points-to information for this pointer
     we cannot disambiguate anything else.  */
  pi = SSA_NAME_PTR_INFO (ptr);
  if (!pi
      || pi->pt.anything)
    return true;

  /* If the points-to set includes the variable we are done.  */
  if (bitmap_bit_p (pi->pt.vars, DECL_UID (decl)))
    return true;

  /* pt_nonlocal includes any global variable.  */
  if (is_global_var (decl)
      && pi->pt.nonlocal)
    return true;

  return false;
}


/* Return true if STMT is an "escape" site from the current function.  Escape
   sites those statements which might expose the address of a variable
   outside the current function.  STMT is an escape site iff:

   	1- STMT is a function call, or
	2- STMT is an __asm__ expression, or
	3- STMT is an assignment to a non-local variable, or
	4- STMT is a return statement.

   Return the type of escape site found, if we found one, or NO_ESCAPE
   if none.  */

enum escape_type
is_escape_site (gimple stmt)
{
  if (is_gimple_call (stmt))
    {
      if (gimple_call_flags (stmt) & (ECF_PURE | ECF_CONST))
	return ESCAPE_TO_PURE_CONST;

      return ESCAPE_TO_CALL;
    }
  else if (gimple_code (stmt) == GIMPLE_ASM)
    return ESCAPE_TO_ASM;
  else if (is_gimple_assign (stmt))
    {
      tree lhs = gimple_assign_lhs (stmt);

      /* Get to the base of _REF nodes.  */
      if (TREE_CODE (lhs) != SSA_NAME)
	lhs = get_base_address (lhs);

      /* If we couldn't recognize the LHS of the assignment, assume that it
	 is a non-local store.  */
      if (lhs == NULL_TREE)
	return ESCAPE_UNKNOWN;

      if (gimple_assign_cast_p (stmt))
	{
	  tree from = TREE_TYPE (gimple_assign_rhs1 (stmt));
	  tree to = TREE_TYPE (lhs);

	  /* If the RHS is a conversion between a pointer and an integer, the
	     pointer escapes since we can't track the integer.  */
	  if (POINTER_TYPE_P (from) && !POINTER_TYPE_P (to))
	    return ESCAPE_BAD_CAST;
	}

      /* If the LHS is an SSA name, it can't possibly represent a non-local
	 memory store.  */
      if (TREE_CODE (lhs) == SSA_NAME)
	return NO_ESCAPE;

      /* If the LHS is a non-global decl, it isn't a non-local memory store.
	 If the LHS escapes, the RHS escape is dealt with in the PTA solver.  */
      if (DECL_P (lhs)
	  && !is_global_var (lhs))
	return NO_ESCAPE;

      /* FIXME: LHS is not an SSA_NAME.  Even if it's an assignment to a
	 local variables we cannot be sure if it will escape, because we
	 don't have information about objects not in SSA form.  Need to
	 implement something along the lines of

	 J.-D. Choi, M. Gupta, M. J. Serrano, V. C. Sreedhar, and S. P.
	 Midkiff, ``Escape analysis for java,'' in Proceedings of the
	 Conference on Object-Oriented Programming Systems, Languages, and
	 Applications (OOPSLA), pp. 1-19, 1999.  */
      return ESCAPE_STORED_IN_GLOBAL;
    }
  else if (gimple_code (stmt) == GIMPLE_RETURN)
    return ESCAPE_TO_RETURN;

  return NO_ESCAPE;
}


/* Dump alias information on FILE.  */

void
dump_alias_info (FILE *file)
{
  size_t i;
  const char *funcname
    = lang_hooks.decl_printable_name (current_function_decl, 2);
  referenced_var_iterator rvi;
  tree var;

  fprintf (file, "\n\nAlias information for %s\n\n", funcname);

  fprintf (file, "Aliased symbols\n\n");
  
  FOR_EACH_REFERENCED_VAR (var, rvi)
    {
      if (may_be_aliased (var))
	dump_variable (file, var);
    }

  fprintf (file, "\n\nFlow-insensitive points-to information for %s\n\n", funcname);

  for (i = 1; i < num_ssa_names; i++)
    {
      tree ptr = ssa_name (i);
      struct ptr_info_def *pi;
      
      if (ptr == NULL_TREE
	  || SSA_NAME_IN_FREE_LIST (ptr))
	continue;

      pi = SSA_NAME_PTR_INFO (ptr);
      if (pi)
	dump_points_to_info_for (file, ptr);
    }

  fprintf (file, "\n");
}


/* Dump alias information on stderr.  */

void
debug_alias_info (void)
{
  dump_alias_info (stderr);
}


/* Return the alias information associated with pointer T.  It creates a
   new instance if none existed.  */

struct ptr_info_def *
get_ptr_info (tree t)
{
  struct ptr_info_def *pi;

  gcc_assert (POINTER_TYPE_P (TREE_TYPE (t)));

  pi = SSA_NAME_PTR_INFO (t);
  if (pi == NULL)
    {
      pi = GGC_CNEW (struct ptr_info_def);
      pi->pt.anything = 1;
      SSA_NAME_PTR_INFO (t) = pi;
    }

  return pi;
}

/* Dump points-to information for SSA_NAME PTR into FILE.  */

void
dump_points_to_info_for (FILE *file, tree ptr)
{
  struct ptr_info_def *pi = SSA_NAME_PTR_INFO (ptr);

  print_generic_expr (file, ptr, dump_flags);

  if (pi)
    {
      if (pi->is_dereferenced)
	fprintf (file, ", is dereferenced");

      if (pi->pt.anything)
	fprintf (file, ", points-to anything");

      if (pi->pt.nonlocal)
	fprintf (file, ", points-to non-local");

      if (pi->pt.escaped)
	fprintf (file, ", points-to escaped");

      if (pi->pt.null)
	fprintf (file, ", points-to NULL");

      if (pi->pt.vars)
	{
	  fprintf (file, ", points-to vars: ");
	  dump_decl_set (file, pi->pt.vars);
	  if (pi->pt.vars_contains_global)
	    fprintf (file, " (includes global vars)");
	}
    }

  fprintf (file, "\n");
}


/* Dump points-to information for VAR into stderr.  */

void
debug_points_to_info_for (tree var)
{
  dump_points_to_info_for (stderr, var);
}


/* If the call CALL may use the memory reference REF return true,
   otherwise return false.  */

static bool
ref_may_used_by_call_p (gimple call ATTRIBUTE_UNUSED, tree ref)
{
  tree base = get_base_address (ref);
  unsigned i;

  /* If the base variable is call-used then it may be used.  */
  if (base
      && DECL_P (base)
      && is_call_used (base))
    return true;

  /* Inspect call arguments for passed-by-value aliases.  */
  for (i = 0; i < gimple_call_num_args (call); ++i)
    {
      tree op = gimple_call_arg (call, i);

      if (TREE_CODE (op) == EXC_PTR_EXPR
	  || TREE_CODE (op) == FILTER_EXPR)
	continue;

      if (TREE_CODE (op) == WITH_SIZE_EXPR)
	op = TREE_OPERAND (op, 0);

      if (TREE_CODE (op) != SSA_NAME
	  && !is_gimple_min_invariant (op)
	  && refs_may_alias_p (op, ref))
	return true;
    }

  return false;
}

/* If the statement STMT may use the memory reference REF return
   true, otherwise return false.  */

bool
ref_may_used_by_stmt_p (gimple stmt, tree ref)
{
  if (is_gimple_assign (stmt))
    {
      tree rhs;

      /* All memory assign statements are single.  */
      if (!gimple_assign_single_p (stmt))
	return false;

      rhs = gimple_assign_rhs1 (stmt);
      if (is_gimple_reg (rhs)
	  || is_gimple_min_invariant (rhs)
	  || gimple_assign_rhs_code (stmt) == CONSTRUCTOR)
	return false;

      return refs_may_alias_p (rhs, ref);
    }
  else if (is_gimple_call (stmt))
    return ref_may_used_by_call_p (stmt, ref);

  return true;
}

/* If the call in statement CALL may clobber the memory reference REF
   return true, otherwise return false.  */

static bool
call_may_clobber_ref_p (gimple call, tree ref)
{
  tree base;

  /* If the call is pure or const it cannot clobber anything.  */
  if (gimple_call_flags (call)
      & (ECF_PURE|ECF_CONST|ECF_LOOPING_CONST_OR_PURE|ECF_NOVOPS))
    return false;

  base = get_base_address (ref);
  if (!base)
    return true;

  if (TREE_CODE (base) == SSA_NAME
      || CONSTANT_CLASS_P (base))
    return false;

  if (DECL_P (base))
    return is_call_clobbered (base);

  return true;
}

/* If the statement STMT may clobber the memory reference REF return true,
   otherwise return false.  */

bool
stmt_may_clobber_ref_p (gimple stmt, tree ref)
{
  if (is_gimple_call (stmt))
    {
      tree lhs = gimple_call_lhs (stmt);
      if (lhs
	  && !is_gimple_reg (lhs)
	  && refs_may_alias_p (ref, lhs))
	return true;

      return call_may_clobber_ref_p (stmt, ref);
    }
  else if (is_gimple_assign (stmt))
    return refs_may_alias_p (ref, gimple_assign_lhs (stmt));
  else if (gimple_code (stmt) == GIMPLE_ASM)
    return true;

  return false;
}

/* For a phi-node PHI for the virtual operand get the argument that
   represents the single incoming edge into a loop if REF is invariant
   in that loop.  */

static tree
get_single_incoming_phi_arg_for_maybe_loop_invariant_ref (gimple phi, tree ref)
{
  tree def_arg = NULL_TREE;
  unsigned i;

  for (i = 0; i < gimple_phi_num_args (phi); ++i)
    {
      tree arg = PHI_ARG_DEF (phi, i);
      gimple def_stmt;

      if (!(gimple_phi_arg_edge (phi, i)->flags & EDGE_DFS_BACK))
	{
	  /* Multiple non-back edges?  Do not try to handle this.  */
	  if (def_arg)
	    return NULL_TREE;
	  def_arg = arg;
	  continue;
	}

      /* Follow the definitions back to the original PHI node.  Bail
	 out once a definition is found that may clobber REF.  */
      def_stmt = SSA_NAME_DEF_STMT (arg);
      do
	{
	  if (gimple_code (def_stmt) == GIMPLE_PHI
	      || gimple_nop_p (def_stmt)
	      || stmt_may_clobber_ref_p (def_stmt, ref))
	    return NULL;
	  def_stmt = SSA_NAME_DEF_STMT (gimple_vuse (def_stmt));
	}
      while (def_stmt != phi);
    }

  return def_arg;
}

/* Based on the memory reference REF and its virtual use VUSE call
   WALKER for each virtual use that is equivalent to VUSE, including VUSE
   itself.  That is, for each virtual use for which its defining statement
   does not clobber REF.

   WALKER is called with REF, the current virtual use and DATA.  If
   WALKER returns non-NULL the walk stops and its result is returned.
   At the end of a non-successful walk NULL is returned.

   TODO: Cache the vector of equivalent vuses per ref, vuse pair.  */

void *
walk_non_aliased_vuses (tree ref, tree vuse,
			void *(*walker)(tree, tree, void *), void *data)
{
  do
    {
      gimple def_stmt;
      void *res;

      res = (*walker) (ref, vuse, data);
      if (res)
	return res;

      def_stmt = SSA_NAME_DEF_STMT (vuse);
      if (gimple_nop_p (def_stmt))
	return NULL;
      else if (gimple_code (def_stmt) == GIMPLE_PHI)
	vuse = get_single_incoming_phi_arg_for_maybe_loop_invariant_ref
	         (def_stmt, ref);
      else
	{
	  if (stmt_may_clobber_ref_p (def_stmt, ref))
	    return NULL;
	  vuse = gimple_vuse (def_stmt);
	}
    }
  while (vuse);

  return NULL;
}
