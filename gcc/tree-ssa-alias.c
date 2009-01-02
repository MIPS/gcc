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

/* Broad overview of how aliasing works:
   
   First we compute points-to sets, which is done in
   tree-ssa-structalias.c
      
   During points-to set constraint finding, a bunch of little bits of
   information is collected.
   This is not done because it is necessary for points-to, but because
   points-to has to walk every statement anyway.  The function performing
   this collecting is update_alias_info.

   Bits update_alias_info collects include:
   1. Directly escaping variables and variables whose value escapes
   (using is_escape_site).  This is the set of variables and values that
   escape prior to transitive closure of the clobbers.
   2.  The set of variables dereferenced on the LHS (into
   dereferenced_ptr_stores) 
   3. The set of variables dereferenced on the RHS (into
   dereferenced_ptr_loads) 
   4. The set of all pointers we saw.
   5. The number of loads and stores for each variable
   6. The number of statements touching memory
   7. The set of address taken variables.
   
   
   #1 is computed by a combination of is_escape_site, and counting the
   number of uses/deref operators.  This function properly accounts for
   situations like &ptr->field, which is *not* a dereference.
   
   After points-to sets are computed, the sets themselves still
   contain points-to specific variables, such as a variable that says
   the pointer points to anything, a variable that says the pointer
   points to readonly memory, etc.

   These are eliminated in a later phase, as we will see.

   The rest of the phases are located in tree-ssa-alias.c

   The next phase after points-to set computation is called
   "setup_pointers_and_addressables"

   This pass does 3 main things:
   
   1. All variables that can have TREE_ADDRESSABLE removed safely (IE
   non-globals whose address is not taken), have TREE_ADDRESSABLE
   removed.
   2. All variables that may be aliased (which is the set of addressable
   variables and globals) at all, are marked for renaming, and have
   symbol memory tags created for them.
   3. All variables which are stored into have their SMT's added to
   written vars. 


   After this function is run, all variables that will ever have an
   SMT, have one, though its aliases are not filled in.

   The next phase is to compute flow-insensitive aliasing, which in
   our case, is a misnomer.  it is really computing aliasing that
   requires no transitive closure to be correct.  In particular, it
   uses stack vs non-stack, TBAA, etc, to determine whether two
   symbols could *ever* alias .  This phase works by going through all
   the pointers we collected during update_alias_info, and for every
   addressable variable in the program, seeing if they alias.  If so,
   the addressable variable is added to the symbol memory tag for the
   pointer.

   As part of this, we handle symbol memory tags that conflict but
   have no aliases in common, by forcing them to have a symbol in
   common (through unioning alias sets or adding one as an alias of
   the other), or by adding one as an alias of another.  The case of
   conflicts with no aliases in common occurs mainly due to aliasing
   we cannot see.  In particular, it generally means we have a load
   through a pointer whose value came from outside the function.
   Without an addressable symbol to point to, they would get the wrong
   answer.

   After flow insensitive aliasing is computed, we compute name tags
   (called compute_flow_sensitive_info).  We walk each pointer we
   collected and see if it has a usable points-to set.  If so, we
   generate a name tag using that pointer, and make an alias bitmap for
   it.  Name tags are shared between all things with the same alias
   bitmap.  The alias bitmap will be translated from what points-to
   computed.  In particular, the "anything" variable in points-to will be
   transformed into a pruned set of SMT's and their aliases that
   compute_flow_insensitive_aliasing computed.
   Note that since 4.3, every pointer that points-to computed a solution for
   will get a name tag (whereas before 4.3, only those whose set did
   *not* include the anything variable would).  At the point where name
   tags are all assigned, symbol memory tags are dead, and could be
   deleted, *except* on global variables.  Global variables still use
   symbol memory tags as of right now.

   After name tags are computed, the set of clobbered variables is
   transitively closed.  In particular, we compute the set of clobbered
   variables based on the initial set of clobbers, plus the aliases of
   pointers which either escape, or have their value escape.

   After this, maybe_create_global_var is run, which handles a corner
   case where we have no call clobbered variables, but have pure and
   non-pure functions.
   
   Staring at this function, I now remember it is a hack for the fact
   that we do not mark all globals in the program as call clobbered for a
   function unless they are actually used in that function.  Instead,  we
   only mark the set that is actually clobbered.  As a result, you can
   end up with situations where you have no call clobbered vars set.
   
   After maybe_create_global_var, we set pointers with the REF_ALL flag
   to have alias sets that include all clobbered
   memory tags and variables.
   
   After this, memory partitioning is computed (by the function
   compute_memory_partitions) and alias sets are reworked accordingly.

   Lastly, we delete partitions with no symbols, and clean up after
   ourselves.  */


/* Counters used to display statistics on alias analysis.  */
struct alias_stats_d
{
  unsigned int alias_queries;
  unsigned int alias_mayalias;
  unsigned int alias_noalias;
  unsigned int simple_queries;
  unsigned int simple_resolved;
  unsigned int tbaa_queries;
  unsigned int tbaa_resolved;
  unsigned int structnoaddress_queries;
  unsigned int structnoaddress_resolved;
};


/* Local variables.  */
static struct alias_stats_d alias_stats;

/* Local functions.  */
static void dump_alias_stats (FILE *);
static void reset_alias_info (void);


/* Transfer the call-clobber solutions from the points-to solution
   to the call-clobber state of the variables.  */

static void
compute_call_clobbered (void)
{
  referenced_var_iterator rvi;
  tree var;
  bool any_pt_anything = false;
  enum escape_type pt_anything_mask = 0;

  timevar_push (TV_CALL_CLOBBER);

  FOR_EACH_REFERENCED_VAR (var, rvi)
    {
      if (is_global_var (var))
	{
	  if (!unmodifiable_var_p (var))
	    mark_call_clobbered (var, ESCAPE_IS_GLOBAL);
	}
      else if (TREE_CODE (var) == PARM_DECL
	       && gimple_default_def (cfun, var)
	       && POINTER_TYPE_P (TREE_TYPE (var)))
	{
	  tree def = gimple_default_def (cfun, var);
	  get_ptr_info (def)->value_escapes_p = 1;
	  get_ptr_info (def)->escape_mask |= ESCAPE_IS_PARM;	  
	}
    }

  if (!clobber_what_escaped ())
    {
      any_pt_anything = true;
      pt_anything_mask |= ESCAPE_TO_CALL;
    }

  compute_call_used_vars ();

  /* If a pt_anything pointer escaped we need to mark all addressable
     variables call clobbered.  */
  if (any_pt_anything)
    {
      bitmap_iterator bi;
      unsigned int j;

      EXECUTE_IF_SET_IN_BITMAP (gimple_addressable_vars (cfun), 0, j, bi)
	{
	  tree var = referenced_var (j);
	  if (!unmodifiable_var_p (var))
	    mark_call_clobbered (var, pt_anything_mask);
	}
    }

  timevar_pop (TV_CALL_CLOBBER);
}


/* Compute may-alias information for every variable referenced in function
   FNDECL.

   Alias analysis proceeds in 3 main phases:

   1- Points-to and escape analysis.

   This phase walks the use-def chains in the SSA web looking for three
   things:

	* Assignments of the form P_i = &VAR
	* Assignments of the form P_i = malloc()
	* Pointers and ADDR_EXPR that escape the current function.

   The concept of 'escaping' is the same one used in the Java world.  When
   a pointer or an ADDR_EXPR escapes, it means that it has been exposed
   outside of the current function.  So, assignment to global variables,
   function arguments and returning a pointer are all escape sites, as are
   conversions between pointers and integers.

   This is where we are currently limited.  Since not everything is renamed
   into SSA, we lose track of escape properties when a pointer is stashed
   inside a field in a structure, for instance.  In those cases, we are
   assuming that the pointer does escape.

   We use escape analysis to determine whether a variable is
   call-clobbered.  Simply put, if an ADDR_EXPR escapes, then the variable
   is call-clobbered.  If a pointer P_i escapes, then all the variables
   pointed-to by P_i (and its memory tag) also escape.

   2- Compute flow-sensitive aliases

   We have two classes of memory tags.  Memory tags associated with the
   pointed-to data type of the pointers in the program.  These tags are
   called "symbol memory tag" (SMT).  The other class are those associated
   with SSA_NAMEs, called "name memory tag" (NMT). The basic idea is that
   when adding operands for an INDIRECT_REF *P_i, we will first check
   whether P_i has a name tag, if it does we use it, because that will have
   more precise aliasing information.  Otherwise, we use the standard symbol
   tag.

   In this phase, we go through all the pointers we found in points-to
   analysis and create alias sets for the name memory tags associated with
   each pointer P_i.  If P_i escapes, we mark call-clobbered the variables
   it points to and its tag.


   3- Compute flow-insensitive aliases

   This pass will compare the alias set of every symbol memory tag and
   every addressable variable found in the program.  Given a symbol
   memory tag SMT and an addressable variable V.  If the alias sets of
   SMT and V conflict (as computed by may_alias_p), then V is marked
   as an alias tag and added to the alias set of SMT.

   For instance, consider the following function:

	    foo (int i)
	    {
	      int *p, a, b;
	    
	      if (i > 10)
	        p = &a;
	      else
	        p = &b;
	    
	      *p = 3;
	      a = b + 2;
	      return *p;
	    }

   After aliasing analysis has finished, the symbol memory tag for pointer
   'p' will have two aliases, namely variables 'a' and 'b'.  Every time
   pointer 'p' is dereferenced, we want to mark the operation as a
   potential reference to 'a' and 'b'.

	    foo (int i)
	    {
	      int *p, a, b;

	      if (i_2 > 10)
		p_4 = &a;
	      else
		p_6 = &b;
	      # p_1 = PHI <p_4(1), p_6(2)>;

	      # a_7 = VDEF <a_3>;
	      # b_8 = VDEF <b_5>;
	      *p_1 = 3;

	      # a_9 = VDEF <a_7>
	      # VUSE <b_8>
	      a_9 = b_8 + 2;

	      # VUSE <a_9>;
	      # VUSE <b_8>;
	      return *p_1;
	    }

   In certain cases, the list of may aliases for a pointer may grow too
   large.  This may cause an explosion in the number of virtual operands
   inserted in the code.  Resulting in increased memory consumption and
   compilation time.

   When the number of virtual operands needed to represent aliased
   loads and stores grows too large (configurable with option --param
   max-aliased-vops and --param avg-aliased-vops), alias sets are
   grouped to avoid severe compile-time slow downs and memory
   consumption. See compute_memory_partitions.  */

unsigned int
compute_may_aliases (void)
{
  timevar_push (TV_TREE_MAY_ALIAS);
  
  memset (&alias_stats, 0, sizeof (alias_stats));

  /* Reset aliasing information.  */
  reset_alias_info ();

  /* For each pointer P_i, determine the sets of variables that P_i may
     point-to.  For every addressable variable V, determine whether the
     address of V escapes the current function, making V call-clobbered
     (i.e., whether &V is stored in a global variable or if its passed as a
     function call argument).  */
  compute_points_to_sets ();
  
  /* Compute call clobbering information.  */
  compute_call_clobbered ();

  /* Debugging dumps.  */
  if (dump_file)
    {
      dump_alias_info (dump_file);

      if (dump_flags & TDF_STATS)
	dump_alias_stats (dump_file);

      if (dump_flags & TDF_DETAILS)
	dump_referenced_vars (dump_file);
    }

  /* Deallocate memory used by aliasing data structures.  */
  delete_points_to_sets ();

  gcc_assert (!need_ssa_update_p ());

  timevar_pop (TV_TREE_MAY_ALIAS);
  
  return 0;
}

/* Data structure used to count the number of dereferences to PTR
   inside an expression.  */
struct count_ptr_d
{
  tree ptr;
  unsigned num_stores;
  unsigned num_loads;
};


/* Helper for count_uses_and_derefs.  Called by walk_tree to look for
   (ALIGN/MISALIGNED_)INDIRECT_REF nodes for the pointer passed in DATA.  */

static tree
count_ptr_derefs (tree *tp, int *walk_subtrees, void *data)
{
  struct walk_stmt_info *wi_p = (struct walk_stmt_info *) data;
  struct count_ptr_d *count_p = (struct count_ptr_d *) wi_p->info;

  /* Do not walk inside ADDR_EXPR nodes.  In the expression &ptr->fld,
     pointer 'ptr' is *not* dereferenced, it is simply used to compute
     the address of 'fld' as 'ptr + offsetof(fld)'.  */
  if (TREE_CODE (*tp) == ADDR_EXPR)
    {
      *walk_subtrees = 0;
      return NULL_TREE;
    }

  if (INDIRECT_REF_P (*tp) && TREE_OPERAND (*tp, 0) == count_p->ptr)
    {
      if (wi_p->is_lhs)
	count_p->num_stores++;
      else
	count_p->num_loads++;
    }

  return NULL_TREE;
}


/* Count the number of direct and indirect uses for pointer PTR in
   statement STMT.  The number of direct uses is stored in
   *NUM_USES_P.  Indirect references are counted separately depending
   on whether they are store or load operations.  The counts are
   stored in *NUM_STORES_P and *NUM_LOADS_P.  */

void
count_uses_and_derefs (tree ptr, gimple stmt, unsigned *num_uses_p,
		       unsigned *num_loads_p, unsigned *num_stores_p)
{
  ssa_op_iter i;
  tree use;

  *num_uses_p = 0;
  *num_loads_p = 0;
  *num_stores_p = 0;

  /* Find out the total number of uses of PTR in STMT.  */
  FOR_EACH_SSA_TREE_OPERAND (use, stmt, i, SSA_OP_USE)
    if (use == ptr)
      (*num_uses_p)++;

  /* Now count the number of indirect references to PTR.  This is
     truly awful, but we don't have much choice.  There are no parent
     pointers inside INDIRECT_REFs, so an expression like
     '*x_1 = foo (x_1, *x_1)' needs to be traversed piece by piece to
     find all the indirect and direct uses of x_1 inside.  The only
     shortcut we can take is the fact that GIMPLE only allows
     INDIRECT_REFs inside the expressions below.  */
  if (is_gimple_assign (stmt)
      || gimple_code (stmt) == GIMPLE_RETURN
      || gimple_code (stmt) == GIMPLE_ASM
      || is_gimple_call (stmt))
    {
      struct walk_stmt_info wi;
      struct count_ptr_d count;

      count.ptr = ptr;
      count.num_stores = 0;
      count.num_loads = 0;

      memset (&wi, 0, sizeof (wi));
      wi.info = &count;
      walk_gimple_op (stmt, count_ptr_derefs, &wi);

      *num_stores_p = count.num_stores;
      *num_loads_p = count.num_loads;
    }

  gcc_assert (*num_uses_p >= *num_loads_p + *num_stores_p);
}

/* Helper for init_alias_info.  Reset existing aliasing information.  */

static void
reset_alias_info (void)
{
  referenced_var_iterator rvi;
  tree var;
  unsigned i;

  /* Since we are about to re-discover call-clobbered
     variables, clear all the call-clobbered flags.  */
  FOR_EACH_REFERENCED_VAR (var, rvi)
    {
      if (is_gimple_reg (var)
	  || is_global_var (var))
	continue;

      clear_call_clobbered (var);
    }

  /* There should be no call-clobbered variable left.  */
  gcc_assert (bitmap_empty_p (gimple_call_clobbered_vars (cfun)));

  /* Clear the call-used variables.  */
  bitmap_clear (gimple_call_used_vars (cfun));

  /* Clear points-to information from each SSA name.  */
  for (i = 1; i < num_ssa_names; i++)
    {
      tree name = ssa_name (i);

      if (!name || !POINTER_TYPE_P (TREE_TYPE (name)))
	continue;

      if (SSA_NAME_PTR_INFO (name))
	{
	  struct ptr_info_def *pi = SSA_NAME_PTR_INFO (name);

	  /* Clear all the flags and reset the points-to solution.  */
	  pi->value_escapes_p = 0;
	  pi->is_dereferenced = 0;
	  pi->pt.anything = 1;
	  pi->pt.nonlocal = 0;
	  pi->pt.escaped = 0;
	  pi->pt.null = 0;
	  pi->pt.vars_contains_global = 0;
	  if (pi->pt.vars)
	    {
	      bitmap_clear (pi->pt.vars);
	      pi->pt.vars = NULL;
	    }
	}
    }
}


/* Return TRUE if pointer PTR may point to variable VAR.
   
   MEM_ALIAS_SET is the alias set for the memory location pointed-to by PTR
	This is needed because when checking for type conflicts we are
	interested in the alias set of the memory location pointed-to by
	PTR.  The alias set of PTR itself is irrelevant.
   
   VAR_ALIAS_SET is the alias set for VAR.  */

bool
may_alias_p (tree ptr, alias_set_type mem_alias_set,
	     tree var, alias_set_type var_alias_set,
	     bool alias_set_only)
{
  alias_stats.alias_queries++;
  alias_stats.simple_queries++;

  /* If -fargument-noalias-global is > 2, pointer arguments may
     not point to anything else.  */
  if (flag_argument_noalias > 2 && TREE_CODE (ptr) == PARM_DECL)
    {
      alias_stats.alias_noalias++;
      alias_stats.simple_resolved++;
      return false;
    }

  /* If -fargument-noalias-global is > 1, pointer arguments may
     not point to global variables.  */
  if (flag_argument_noalias > 1 && is_global_var (var)
      && TREE_CODE (ptr) == PARM_DECL)
    {
      alias_stats.alias_noalias++;
      alias_stats.simple_resolved++;
      return false;
    }

  /* If the pointed to memory has alias set zero, or the pointer
     is ref-all, or the pointer decl is marked that no TBAA is to
     be applied, the MEM can alias VAR.  */
  if (mem_alias_set == 0
      || DECL_POINTER_ALIAS_SET (ptr) == 0
      || TYPE_REF_CAN_ALIAS_ALL (TREE_TYPE (ptr))
      || DECL_NO_TBAA_P (ptr))
    {
      alias_stats.alias_mayalias++;
      alias_stats.simple_resolved++;
      return true;
    }

  alias_stats.tbaa_queries++;

  /* If the alias sets don't conflict then MEM cannot alias VAR.  */
  if (mem_alias_set != var_alias_set
      && !alias_set_subset_of (mem_alias_set, var_alias_set))
    {
      alias_stats.alias_noalias++;
      alias_stats.tbaa_resolved++;
      return false;
    }

  /* If VAR is a record or union type, PTR cannot point into VAR
     unless there is some explicit address operation in the
     program that can reference a field of the type pointed-to by
     PTR.  This also assumes that the types of both VAR and PTR
     are contained within the compilation unit, and that there is
     no fancy addressing arithmetic associated with any of the
     types involved.  */
  if (mem_alias_set != 0 && var_alias_set != 0)
    {
      tree ptr_type = TREE_TYPE (ptr);
      tree var_type = TREE_TYPE (var);
      
      /* The star count is -1 if the type at the end of the
	 pointer_to chain is not a record or union type. */ 
      if (!alias_set_only && 
	  0 /* FIXME tuples ipa_type_escape_star_count_of_interesting_type (var_type) >= 0*/)
	{
	  int ptr_star_count = 0;

	  /* ipa_type_escape_star_count_of_interesting_type is a
	     little too restrictive for the pointer type, need to
	     allow pointers to primitive types as long as those
	     types cannot be pointers to everything.  */
	  while (POINTER_TYPE_P (ptr_type))
	    {
	      /* Strip the *s off.  */ 
	      ptr_type = TREE_TYPE (ptr_type);
	      ptr_star_count++;
	    }
	  
	  /* There does not appear to be a better test to see if
	     the pointer type was one of the pointer to everything
	     types.  */
	  if (ptr_star_count > 0)
	    {
	      alias_stats.structnoaddress_queries++;
	      if (ipa_type_escape_field_does_not_clobber_p (var_type, 
							    TREE_TYPE (ptr)))
		{
		  alias_stats.structnoaddress_resolved++;
		  alias_stats.alias_noalias++;
		  return false;
		}
	    }
	  else if (ptr_star_count == 0)
	    {
	      /* If PTR_TYPE was not really a pointer to type, it cannot 
		 alias.  */ 
	      alias_stats.structnoaddress_queries++;
	      alias_stats.structnoaddress_resolved++;
	      alias_stats.alias_noalias++;
	      return false;
	    }
	}
    }

  alias_stats.alias_mayalias++;
  return true;
}

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

  /* Local variables that do not have their address taken
     can not be pointed to.  */
  if (!TREE_ADDRESSABLE (decl)
      && !is_global_var (decl))
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

/* Dump alias statistics on FILE.  */

static void 
dump_alias_stats (FILE *file)
{
  const char *funcname
    = lang_hooks.decl_printable_name (current_function_decl, 2);
  fprintf (file, "\nAlias statistics for %s\n\n", funcname);
  fprintf (file, "Total alias queries:\t%u\n", alias_stats.alias_queries);
  fprintf (file, "Total alias mayalias results:\t%u\n", 
	   alias_stats.alias_mayalias);
  fprintf (file, "Total alias noalias results:\t%u\n",
	   alias_stats.alias_noalias);
  fprintf (file, "Total simple queries:\t%u\n",
	   alias_stats.simple_queries);
  fprintf (file, "Total simple resolved:\t%u\n",
	   alias_stats.simple_resolved);
  fprintf (file, "Total TBAA queries:\t%u\n",
	   alias_stats.tbaa_queries);
  fprintf (file, "Total TBAA resolved:\t%u\n",
	   alias_stats.tbaa_resolved);
  fprintf (file, "Total non-addressable structure type queries:\t%u\n",
	   alias_stats.structnoaddress_queries);
  fprintf (file, "Total non-addressable structure type resolved:\t%u\n",
	   alias_stats.structnoaddress_resolved);
}


/* Return true if VAR may be aliased.  */

static bool
may_be_aliased (tree var)
{
  /* Obviously.  */
  if (TREE_ADDRESSABLE (var))
    return true;

  /* ???  Why does this differ from is_global_var ()?  */
  if (DECL_EXTERNAL (var) || TREE_PUBLIC (var))
    return true;

  /* Automatic variables can't have their addresses escape any other
     way.  This must be after the check for global variables, as
     extern declarations do not have TREE_STATIC set.  */
  if (!TREE_STATIC (var))
    return false;

  return false;
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

      if (pi->value_escapes_p)
	fprintf (file, ", its value escapes");

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


/* ???  Remove me.
   For now just a quick verification run on sane initial properties.  */

static unsigned int
reset_cc_flags (void)
{
  tree var;
  referenced_var_iterator rvi;

  FOR_EACH_REFERENCED_VAR (var, rvi)
    if (is_global_var (var)
	|| TREE_ADDRESSABLE (var))
      gcc_assert (is_call_clobbered (var));
    else
      gcc_assert (!is_call_clobbered (var));

  return 0;
}

struct gimple_opt_pass pass_reset_cc_flags =
{
 {
  GIMPLE_PASS,
  NULL,		 /* name */
  NULL,  	 /* gate */
  reset_cc_flags, /* execute */
  NULL,			 /* sub */
  NULL,			 /* next */
  0,			 /* static_pass_number */
  0,			 /* tv_id */
  PROP_referenced_vars |PROP_cfg, /* properties_required */
  0,			 /* properties_provided */
  0,			 /* properties_destroyed */
  0,			 /* todo_flags_start */
  0         	         /* todo_flags_finish */
 }
};


/* A dummy pass to cause aliases to be computed via TODO_rebuild_alias.  */

struct gimple_opt_pass pass_build_alias =
{
 {
  GIMPLE_PASS,
  "alias",		    /* name */
  NULL,			    /* gate */
  NULL,                     /* execute */
  NULL,                     /* sub */
  NULL,                     /* next */
  0,                        /* static_pass_number */
  0,                        /* tv_id */
  PROP_cfg | PROP_ssa,      /* properties_required */
  PROP_alias,               /* properties_provided */
  0,                        /* properties_destroyed */
  0,                        /* todo_flags_start */
  TODO_rebuild_alias | TODO_dump_func  /* todo_flags_finish */
 }
};


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
