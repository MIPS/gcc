/* Alias analysis for trees.
   Copyright (C) 2004 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

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
#include "tree-simple.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "tree-alias-common.h"
#include "tree-pass.h"
#include "convert.h"
#include "params.h"


/* Alias information used by compute_may_aliases and its helpers.  */
struct alias_info
{
  /* SSA names visited while collecting points-to information.  If bit I
     is set, it means that SSA variable with version I has already been
     visited.  */
  bitmap ssa_names_visited;

  /* Array of SSA_NAME pointers processed by the points-to collector.  */
  varray_type processed_ptrs;

  /* Variables whose address is still needed.  */
  bitmap addresses_needed;

  /* ADDRESSABLE_VARS contains all the global variables and locals that
     have had their address taken.  */
  varray_type addressable_vars;

  /* POINTERS contains all the _DECL pointers that have been referenced in
     the program.  */
  varray_type pointers;

  /* Number of function calls found in the program.  */
  size_t num_calls_found;
};


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
  unsigned int pta_queries;
  unsigned int pta_resolved;
};


/* Tuple to map a variable to its alias set.  Used to cache the results of
   calls to get_alias_set().  */
struct alias_map_d
{
  tree var;
  HOST_WIDE_INT set;
  size_t num_aliases;
};


/* Local variables.  */
static struct alias_stats_d alias_stats;

/* Local functions.  */
static void compute_flow_insensitive_aliasing (struct alias_info *);
static void dump_alias_stats (FILE *);
static bool may_alias_p (tree, HOST_WIDE_INT, tree, HOST_WIDE_INT);
static tree create_memory_tag (tree type, bool is_type_tag);
static tree get_tmt_for (tree, struct alias_info *);
static tree get_nmt_for (tree);
static void add_may_alias (tree, tree);
static struct alias_info *init_alias_info (void);
static void delete_alias_info (struct alias_info *);
static void compute_points_to_and_addr_escape (struct alias_info *);
static void compute_flow_sensitive_aliasing (struct alias_info *);
static void setup_pointers_and_addressables (struct alias_info *);
static void collect_points_to_info_r (tree, tree, void *);
static bool is_escape_site (tree, size_t *);
static void add_pointed_to_var (struct alias_info *, tree, tree);
static void add_pointed_to_expr (tree, tree);
static void create_global_var (void);
static void collect_points_to_info_for (struct alias_info *, tree);
static bool ptr_is_dereferenced_by (tree, tree);
static void maybe_create_global_var (struct alias_info *ai);

/* Global declarations.  */

/* Call clobbered variables in the function.  If bit I is set, then
   REFERENCED_VARS (I) is call-clobbered.  */
bitmap call_clobbered_vars;

/* 'true' after aliases have been computed (see compute_may_aliases).  This
   is used by get_stmt_operands and its helpers to determine what to do
   when scanning an operand for a variable that may be aliased.  If
   may-alias information is still not available, the statement is marked as
   having volatile operands.  */
bool aliases_computed_p;

/* When the program has too many call-clobbered variables and call-sites,
   this variable is used to represent the clobbering effects of function
   calls.  In these cases, all the call clobbered variables in the program
   are forced to alias this variable.  This reduces compile times by not
   having to keep track of too many VDEF expressions at call sites.  */
tree global_var;


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
   function arguments and returning a pointer are all escape sites.

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
   called "type memory tag" (TMT).  The other class are those associated
   with SSA_NAMEs, called "name memory tag" (NMT). The basic idea is that
   when adding operands for an INDIRECT_REF *P_i, we will first check
   whether P_i has a name tag, if it does we use it, because that will have
   more precise aliasing information.  Otherwise, we use the standard type
   tag.

   In this phase, we go through all the pointers we found in points-to
   analysis and create alias sets for the name memory tags associated with
   each pointer P_i.  If P_i escapes, we mark call-clobbered the variables
   it points to and its tag.


   3- Compute flow-insensitive aliases

   This pass will compare the alias set of every type memory tag and every
   addressable variable found in the program.  Given a type memory tag TMT
   and an addressable variable V.  If the alias sets of TMT and V conflict
   (as computed by may_alias_p), then V is marked as an alias tag and added
   to the alias set of TMT.

   For instance, consider the following function:

	    foo (int i)
	    {
	      int *p, *q, a, b;
	    
	      if (i > 10)
	        p = &a;
	      else
	        q = &b;
	    
	      *p = 3;
	      *q = 5;
	      a = b + 2;
	      return *p;
	    }

   After aliasing analysis has finished, the type memory tag for pointer
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

   When the set of may aliases for a pointer grows beyond a certain number of
   elements (configurable with --param max-alias-set-size), instead of adding
   new variables to the may-alias set, the new variables are made to share the
   same alias set as the original pointer.  For instance, suppose that pointer
   'p' may point to variables 'a', 'b', 'c', 'd', 'e', 'f' and 'g'. After alias
   analysis, the alias sets will be as follows:

	may-alias(p) = { a, b, c, d, e }
	may-alias(f) = { a, b, c, d, e }
	may-alias(g) = { a, b, c, d, e }

   Notice that this grouping causes variables 'f' and 'g' to be aliased to
   variables they can't possibly alias to.  This causes loss of precision,
   but prevents excessive compile times by reducing the number of virtual
   operands.  */

static void
compute_may_aliases (void)
{
  struct alias_info *ai;
  
  memset (&alias_stats, 0, sizeof (alias_stats));

  /* Initialize aliasing information.  */
  ai = init_alias_info ();

  /* For each pointer P_i, determine the sets of variables that P_i may
     point-to.  For every addressable variable V, determine whether the
     address of V escapes the current function, making V call-clobbered
     (i.e., whether &V is stored in a global variable or if its passed as a
     function call argument).  */
  compute_points_to_and_addr_escape (ai);

  /* Collect all pointers and addressable variables, compute alias sets,
     create memory tags for pointers and promote variables whose address is
     not needed anymore.  */
  setup_pointers_and_addressables (ai);

  /* Compute flow-sensitive, points-to based aliasing for all the name
     memory tags.  */
  compute_flow_sensitive_aliasing (ai);

  /* Compute type-based flow-insensitive aliasing for all the type memory
     tags.  */
  compute_flow_insensitive_aliasing (ai);

  /* If the program has too many call-clobbered variables and/or function
     calls, create .GLOBAL_VAR and use it to model call-clobbering
     semantics at call sites.  This reduces the number of virtual operands
     considerably, improving compile times at the expense of lost
     aliasing precision.  */
  maybe_create_global_var (ai);

  /* Debugging dumps.  */
  if (tree_dump_file)
    {
      if (tree_dump_flags & TDF_STATS)
	dump_alias_stats (tree_dump_file);
      dump_points_to_info (tree_dump_file);
      dump_alias_info (tree_dump_file);
    }

  /* Deallocate memory used by aliasing data structures.  */
  delete_alias_info (ai);

  /* Indicate that may-alias information is now available.  */
  aliases_computed_p = true;
}

struct tree_opt_pass pass_may_alias = 
{
  "alias",				/* name */
  NULL,					/* gate */
  compute_may_aliases,			/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_TREE_MAY_ALIAS,			/* tv_id */
  PROP_cfg | PROP_ssa | PROP_pta,	/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func | TODO_rename_vars
    | TODO_ggc_collect | TODO_verify_ssa  /* todo_flags_finish */
};


/* Initialize the data structures used for alias analysis.  */

static struct alias_info *
init_alias_info (void)
{
  struct alias_info *ai;

  ai = xcalloc (1, sizeof (struct alias_info));
  ai->ssa_names_visited = BITMAP_XMALLOC ();
  bitmap_clear (ai->ssa_names_visited);
  VARRAY_TREE_INIT (ai->processed_ptrs, 20, "processed_ptrs");
  ai->addresses_needed = BITMAP_XMALLOC ();
  bitmap_clear (ai->addresses_needed);
  VARRAY_GENERIC_PTR_INIT (ai->addressable_vars, 20, "addressable_vars");
  VARRAY_GENERIC_PTR_INIT (ai->pointers, 20, "pointers");

  return ai;
}


/* Deallocate memory used by alias analysis.  */

static void
delete_alias_info (struct alias_info *ai)
{
  BITMAP_FREE (ai->ssa_names_visited);
  ai->processed_ptrs = NULL;
  BITMAP_FREE (ai->addresses_needed);
  ai->addressable_vars = NULL;
  ai->pointers = NULL;
  free (ai);
}


/* Walk use-def chains for pointer PTR to determine what variables is PTR
   pointing to.  */

static void
collect_points_to_info_for (struct alias_info *ai, tree ptr)
{
#if defined ENABLE_CHECKING
  if (!POINTER_TYPE_P (TREE_TYPE (ptr)))
    abort ();
#endif

  if (!bitmap_bit_p (ai->ssa_names_visited, SSA_NAME_VERSION (ptr)))
    {
      ssa_name_ann_t ann;

      bitmap_set_bit (ai->ssa_names_visited, SSA_NAME_VERSION (ptr));
      walk_use_def_chains (ptr, collect_points_to_info_r, ai);

      VARRAY_PUSH_TREE (ai->processed_ptrs, ptr);

      /* If we could not determine where PTR was pointing to, clear all the
	 other points-to information.  */
      ann = ssa_name_ann (ptr);
      if (ann->pt_anything)
	{
	  ann->pt_malloc = 0;
	  ann->pt_vars = NULL;
	}
    }
}


/* Return true if STMT contains INDIRECT_REF <PTR>.  */

static bool
ptr_is_dereferenced_by (tree ptr, tree stmt)
{
  if (TREE_CODE (stmt) == MODIFY_EXPR
      || (TREE_CODE (stmt) == RETURN_EXPR
	&& TREE_CODE (TREE_OPERAND (stmt, 0)) == MODIFY_EXPR))
    {
      tree e, lhs, rhs;

      e = (TREE_CODE (stmt) == RETURN_EXPR) ? TREE_OPERAND (stmt, 0) : stmt;
      lhs = TREE_OPERAND (e, 0);
      rhs = TREE_OPERAND (e, 1);

      return ((TREE_CODE_CLASS (TREE_CODE (lhs)) == 'r'
	       && get_base_var (lhs) == ptr)
	      || (TREE_CODE_CLASS (TREE_CODE (rhs)) == 'r'
	   	  && get_base_var (rhs) == ptr));
    }

  return false;
}


/* Traverse use-def links for all the pointers in the program to collect
   address escape and points-to information.  */

static void
compute_points_to_and_addr_escape (struct alias_info *ai)
{
  basic_block bb;
  size_t i;

  timevar_push (TV_TREE_PTA);

  FOR_EACH_BB (bb)
    {
      bb_ann_t block_ann = bb_ann (bb);
      block_stmt_iterator si;

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  use_optype uses;
	  stmt_ann_t ann;
	  bitmap addr_taken;
	  tree stmt = bsi_stmt (si);
	  bool stmt_escapes_p = is_escape_site (stmt, &ai->num_calls_found);

	  /* Mark all the variables whose address are taken by the
	     statement.  Note that this will miss all the addresses taken
	     in PHI nodes (those are discovered while following the use-def
	     chains).  */
	  get_stmt_operands (stmt);
	  addr_taken = addresses_taken (stmt);
	  if (addr_taken)
	    EXECUTE_IF_SET_IN_BITMAP (addr_taken, 0, i,
		{
		  tree var = referenced_var (i);
		  bitmap_set_bit (ai->addresses_needed, var_ann (var)->uid);
		  if (stmt_escapes_p)
		    mark_call_clobbered (var);
		});

	  if (stmt_escapes_p)
	    block_ann->has_escape_site = 1;

	  ann = stmt_ann (stmt);
	  uses = USE_OPS (ann);
	  for (i = 0; i < NUM_USES (uses); i++)
	    {
	      tree op = USE_OP (uses, i);
	      ssa_name_ann_t ptr_ann;

	      if (!POINTER_TYPE_P (TREE_TYPE (op)))
		continue;

	      collect_points_to_info_for (ai, op);

	      ptr_ann = ssa_name_ann (op);
	      if (ptr_is_dereferenced_by (op, stmt))
		{
		  /* If we found OP to point to a set of variables or
		     malloc, then create a name memory tag for it.  This
		     gives more precise aliasing information, which helps
		     the optimizers.

		     FIXME: Cycles in the SSA web and the lack of SSA 
		     information for structures will prevent the creation
		     of name tags.  Find ways around this limitation.  */
		  if (ptr_ann->pt_malloc || ptr_ann->pt_vars)
		    ptr_ann->name_mem_tag = get_nmt_for (op);
		}
	      else if (stmt_escapes_p)
		{
		  /* Note that even if STMT is an escape point, pointer OP
		     will not escape if it is being dereferenced.  That's
		     why we only check for escape points if OP is not
		     dereferenced by STMT.  */
		  ptr_ann->value_escapes_p = 1;
		}
	    }

	  /* After promoting variables and computing aliasing we will
	     need to re-scan most statements.  FIXME: Try to minimize the
	     number of statements re-scanned.  It's not really necessary to
	     re-scan *all* statements.  */
	  modify_stmt (stmt);
	}
    }

  timevar_pop (TV_TREE_PTA);
}


/* For every pointer P_i in AI->PROCESSED_PTRS, create may-alias sets for
   the name memory tag (NMT) associated with P_i.  If P_i escapes, then its
   name tag and the variables it points-to are call-clobbered.  Finally, if
   P_i escapes and we could not determine where it points to, then all the
   variables in the same alias set as *P_i are marked call-clobbered.  This
   is necessary because we must assume that P_i may take the address of any
   variable in the same alias set.  */

static void
compute_flow_sensitive_aliasing (struct alias_info *ai)
{
  size_t i;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (ai->processed_ptrs); i++)
    {
      size_t j;
      tree ptr = VARRAY_TREE (ai->processed_ptrs, i);
      ssa_name_ann_t ann = ssa_name_ann (ptr);

      if (ann->value_escapes_p || ann->pt_anything)
	{
	  var_ann_t v_ann = var_ann (SSA_NAME_VAR (ptr));

	  /* If PTR escapes or may point to anything, then its associated
	     memory tags are call-clobbered.  */
	  if (ann->name_mem_tag)
	    mark_call_clobbered (ann->name_mem_tag);

	  if (v_ann->type_mem_tag)
	    mark_call_clobbered (v_ann->type_mem_tag);

	  /* If PTR may point to anything, mark call-clobbered all the
	     addressables with the same alias set as the type pointed-to by
	     PTR.  */
	  if (ann->pt_anything)
	    {
	      HOST_WIDE_INT ptr_set;
	      ptr_set = get_alias_set (TREE_TYPE (TREE_TYPE (ptr)));
	      for (j = 0; j < VARRAY_ACTIVE_SIZE (ai->addressable_vars); j++)
		{
		  struct alias_map_d *alias_map;
		  alias_map = VARRAY_GENERIC_PTR (ai->addressable_vars, j);
		  if (alias_map->set == ptr_set)
		    mark_call_clobbered (alias_map->var);
		}
	    }

	  /* If PTR's value may escape and PTR is never dereferenced, we
	     need to mark all the variables PTR points-to as
	     call-clobbered.  Note that we only need do this it PTR is
	     never dereferenced.  If PTR is dereferenced, it will have a
	     name memory tag, which will have been marked call-clobbered.
	     This will in turn mark the pointed-to variables as
	     call-clobbered when we call add_may_alias below.  */
	  if (ann->value_escapes_p && !ann->name_mem_tag && ann->pt_vars)
	    EXECUTE_IF_SET_IN_BITMAP (ann->pt_vars, 0, j,
		mark_call_clobbered (referenced_var (j)));
	}

      /* Set up aliasing information for PTR's name memory tag (if it has
	 one).  Note that only pointers that have been dereferenced will
	 have a memory tag.  */
      if (ann->name_mem_tag && ann->pt_vars)
	EXECUTE_IF_SET_IN_BITMAP (ann->pt_vars, 0, j,
	    add_may_alias (ann->name_mem_tag, referenced_var (j)));
    }
}


/* Compute type-based alias sets.  Traverse all the pointers and
   addressable variables found in setup_pointers_and_addressables.  For
   every pointer P in AI->POINTERS and addressable variable V in
   AI->ADDRESSABLE_VARS, add V to the may-alias sets of P's type memory tag
   (TMT) if their alias sets conflict.  */

static void
compute_flow_insensitive_aliasing (struct alias_info *ai)
{
  size_t i;

  /* For every pointer P, determine which addressable variables may alias
     with P's type memory tag.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ai->pointers); i++)
    {
      size_t j;
      struct alias_map_d *p_map = VARRAY_GENERIC_PTR (ai->pointers, i);
      tree mem = var_ann (p_map->var)->type_mem_tag;
      var_ann_t mem_ann = var_ann (mem);

      for (j = 0; j < VARRAY_ACTIVE_SIZE (ai->addressable_vars); j++)
	{
	  struct alias_map_d *v_map;
	  var_ann_t v_ann;
	  
	  v_map = VARRAY_GENERIC_PTR (ai->addressable_vars, j);
	  v_ann = var_ann (v_map->var);

	  /* Skip memory tags and variables that have never been written to.  */
	  if (!mem_ann->is_stored && !v_ann->is_stored)
	    continue;
	     
	  /* Skip memory tags which are written if the variable is readonly.  */
	  if (mem_ann->is_stored && TREE_READONLY (v_map->var))
	    continue;

	  if (may_alias_p (p_map->var, p_map->set, v_map->var, v_map->set))
	    {
	      /* If MEM has less than MAX_ALIAS_SET_SIZE aliases in its
		 alias set, add V_MAP->VAR to the list of aliases for MEM.
		 Otherwise, set the may-alias set for V_MAP->VAR to be the
		 same alias set as MEM.  This is to avoid the problem of
		 having large may-alias sets.  Large may-alias sets
		 translate into lots of virtual operands which can slow
		 down the SSA pass tremendously.  */
	      if (mem_ann->may_aliases
		  && p_map->num_aliases >= (size_t) MAX_ALIAS_SET_SIZE)
		{
		  VARRAY_TREE_INIT (v_ann->may_aliases,
				    VARRAY_SIZE (mem_ann->may_aliases),
				    "aliases");
		  varray_copy (v_ann->may_aliases, mem_ann->may_aliases);
		}
	      else
		{
		  add_may_alias (mem, v_map->var);
		  p_map->num_aliases++;
		}
	    }
	}
    }
}


/* Create memory tags for all the dereferenced pointers and build the
   ADDRESSABLE_VARS and POINTERS arrays used for building the may-alias
   sets.  Based on the address escape and points-to information collected
   earlier, this pass will also clear the TREE_ADDRESSABLE flag from those
   variables whose address is not needed anymore.  */

static void
setup_pointers_and_addressables (struct alias_info *ai)
{
  size_t i, n_vars;

  /* Since we will be creating type memory tags within this loop, cache the
     value of NUM_REFERENCED_VARS to avoid processing the additional tags
     unnecessarily.  */
  n_vars = num_referenced_vars;

  for (i = 0; i < n_vars; i++)
    {
      tree var = referenced_var (i);
      var_ann_t v_ann = var_ann (var);

      /* Name memory tags already have flow-sensitive aliasing information, so
	 they need not be processed by compute_may_aliases.  Similarly,
	 type memory tags are already accounted for when we process their
	 associated pointer.  */
      if (v_ann->mem_tag_kind != NOT_A_TAG)
	continue;

      /* Remove the ADDRESSABLE flag from every addressable variable whose
         address is not needed anymore.  This is caused by the propagation
         of ADDR_EXPR constants into INDIRECT_REF expressions and the
         removal of dead pointer assignments done by the early scalar
         cleanup passes.  */
      if (TREE_ADDRESSABLE (var))
	{
	  if (!bitmap_bit_p (ai->addresses_needed, v_ann->uid)
	      && !v_ann->has_hidden_use
	      && v_ann->mem_tag_kind == NOT_A_TAG
	      && !needs_to_live_in_memory (var))
	    {
	      /* The address of VAR is not needed, remove the addressable bit,
	         so that it can be optimized as a regular variable.  */
	      mark_non_addressable (var);

	      /* Since VAR is now a regular GIMPLE register, we will need
		 to rename VAR into SSA afterwards.  */
	      bitmap_set_bit (vars_to_rename, v_ann->uid);
	    }
	}

      /* Global variables and addressable locals may be aliased.  Create an
         entry in ADDRESSABLE_VARS for VAR.  */
      if (may_be_aliased (var))
	{
	  /* Create a new alias set entry for VAR.  */
	  struct alias_map_d *alias_map;
	  alias_map = ggc_alloc (sizeof (*alias_map));
	  memset (alias_map, 0, sizeof (*alias_map));
	  alias_map->var = var;

	  if (TREE_CODE (TREE_TYPE (var)) == ARRAY_TYPE)
	    alias_map->set = get_alias_set (TREE_TYPE (TREE_TYPE (var)));
	  else
	    alias_map->set = get_alias_set (var);
	  VARRAY_PUSH_GENERIC_PTR (ai->addressable_vars, alias_map);
	  bitmap_set_bit (vars_to_rename, var_ann (var)->uid);
	}

      /* Add pointer variables that have been dereferenced to the POINTERS
         array and create a type memory tag for them.  */
      if (POINTER_TYPE_P (TREE_TYPE (var))
	  && (v_ann->is_dereferenced_store
	      || v_ann->is_dereferenced_load))
	{
	  tree tag = v_ann->type_mem_tag;
	  var_ann_t t_ann;

	  /* If pointer VAR still doesn't have a memory tag associated with it,
	     create it now or re-use an existing one.  */
	  if (tag == NULL_TREE)
	    tag = get_tmt_for (var, ai);
	  t_ann = var_ann (tag);

	  /* Associate the tag with pointer VAR.  */
	  v_ann->type_mem_tag = tag;

	  /* If pointer VAR has been used in a store operation, then its
	     memory tag must be marked as stored.  */
	  if (v_ann->is_dereferenced_store)
	    t_ann->is_stored = 1;

	  /* If pointer VAR is a global variable or a PARM_DECL, then its
	     memory tag should be considered a global variable.  */
	  if (TREE_CODE (var) == PARM_DECL || needs_to_live_in_memory (var))
	    mark_call_clobbered (tag);
	}
    }
}


/* Determine whether to use .GLOBAL_VAR to model call clobbering semantics.  At
   every call site, we need to emit VDEF expressions to represent the
   clobbering effects of the call for variables whose address escapes the
   current function.

   One approach is to group all call-clobbered variables into a single
   representative that is used as an alias of every call-clobbered variable
   (.GLOBAL_VAR).  This works well, but it ties the optimizer hands because
   references to any call clobbered variable is a reference to .GLOBAL_VAR.

   The second approach is to emit a clobbering VDEF for every call-clobbered
   variable at call sites.  This is the preferred way in terms of optimization
   opportunities but it may create too many VDEF operands if there are many
   call clobbered variables and function calls in the function.

   To decide whether or not to use .GLOBAL_VAR we multiply the number of
   function calls found by the number of call-clobbered variables.  If that
   product is beyond a certain threshold, as determined by the parameterized
   values shown below, we use .GLOBAL_VAR.

   FIXME.  This heuristic should be improved.  One idea is to use several
   .GLOBAL_VARs of different types instead of a single one.  The thresholds
   have been derived from a typical bootstrap cycle, including all target
   libraries. Compile times were found increase by ~1% compared to using
   .GLOBAL_VAR.  */

static void
maybe_create_global_var (struct alias_info *ai)
{
  size_t i, n_clobbered;
  
  /* Count all the call-clobbered variables.  */
  n_clobbered = 0;
  EXECUTE_IF_SET_IN_BITMAP (call_clobbered_vars, 0, i, n_clobbered++);

  if (ai->num_calls_found * n_clobbered >= (size_t) GLOBAL_VAR_THRESHOLD)
    create_global_var ();

  /* If the function has calls to clobbering functions and .GLOBAL_VAR has
     been created, make it an alias for all call-clobbered variables.  */
  if (global_var)
    EXECUTE_IF_SET_IN_BITMAP (call_clobbered_vars, 0, i,
      {
	tree var = referenced_var (i);
	if (var != global_var)
	  {
	     add_may_alias (var, global_var);
	     bitmap_set_bit (vars_to_rename, var_ann (var)->uid);
	  }
      });
}


/* Return TRUE if pointer PTR may point to variable VAR.
   
   MEM_ALIAS_SET is the alias set for the memory location pointed-to by PTR
	This is needed because when checking for type conflicts we are
	interested in the alias set of the memory location pointed-to by
	PTR.  The alias set of PTR itself is irrelevant.
   
   VAR_ALIAS_SET is the alias set for VAR.  */

static bool
may_alias_p (tree ptr, HOST_WIDE_INT mem_alias_set,
	     tree var, HOST_WIDE_INT var_alias_set)
{
  tree mem;
  var_ann_t v_ann, m_ann;

  alias_stats.alias_queries++;
  alias_stats.simple_queries++;

  /* By convention, a variable cannot alias itself.  */
  mem = var_ann (ptr)->type_mem_tag;
  if (mem == var)
    {
      alias_stats.alias_noalias++;
      alias_stats.simple_resolved++;
      return false;
    }

  v_ann = var_ann (var);
  m_ann = var_ann (mem);

#if defined ENABLE_CHECKING
  if (m_ann->mem_tag_kind != TYPE_TAG)
    abort ();
#endif

  alias_stats.tbaa_queries++;

  /* If VAR is a pointer with the same alias set as PTR, then dereferencing
     PTR can't possibly affect VAR.  Note, that we are specifically testing
     for PTR's alias set here, not its pointed-to type.  We also can't
     do this check with relaxed aliasing enabled.  */
  if (POINTER_TYPE_P (TREE_TYPE (var))
      && var_alias_set != 0)
    {
      HOST_WIDE_INT ptr_alias_set = get_alias_set (ptr);
      if (ptr_alias_set == var_alias_set)
	{
	  alias_stats.alias_noalias++;
	  alias_stats.tbaa_resolved++;
	  return false;
	}
    }

  /* If the alias sets don't conflict then MEM cannot alias VAR.  */
  if (!alias_sets_conflict_p (mem_alias_set, var_alias_set))
    {
      /* Handle aliases to structure fields.  If either VAR or MEM are
	 aggregate types, they may not have conflicting types, but one of
	 the structures could contain a pointer to the other one.

	 For instance, given

		MEM -> struct P *p;
		VAR -> struct Q *q;

	 It may happen that '*p' and '*q' can't alias because 'struct P'
	 and 'struct Q' have non-conflicting alias sets.  However, it could
	 happen that one of the fields in 'struct P' is a 'struct Q *' or
	 vice-versa.

	 Therefore, we also need to check if 'struct P' aliases 'struct Q *'
	 or 'struct Q' aliases 'struct P *'.  Notice, that since GIMPLE
	 does not have more than one-level pointers, we don't need to
	 recurse into the structures.  */
      if (AGGREGATE_TYPE_P (TREE_TYPE (mem))
	  || AGGREGATE_TYPE_P (TREE_TYPE (var)))
	{
	  tree ptr_to_var = TYPE_POINTER_TO (TREE_TYPE (var));

	  /* If no pointer-to VAR exists, then MEM can't alias VAR.  */
	  if (ptr_to_var == NULL_TREE)
	    {
	      alias_stats.alias_noalias++;
	      alias_stats.tbaa_resolved++;
	      return false;
	    }

	  /* If MEM doesn't alias a pointer to VAR and VAR doesn't alias
	     PTR, then PTR can't alias VAR.  */
	  if (!alias_sets_conflict_p (mem_alias_set, get_alias_set (ptr_to_var))
	      && !alias_sets_conflict_p (var_alias_set, get_alias_set (ptr)))
	    {
	      alias_stats.alias_noalias++;
	      alias_stats.tbaa_resolved++;
	      return false;
	    }
	}
      else
	{
	  alias_stats.alias_noalias++;
	  alias_stats.tbaa_resolved++;
	  return false;
	}
    }

  if (flag_tree_points_to != PTA_NONE)
      alias_stats.pta_queries++;

  /* If -ftree-points-to is given, check if PTR may point to VAR.  */
  if (flag_tree_points_to == PTA_ANDERSEN
      && !ptr_may_alias_var (ptr, var))
    {
      alias_stats.alias_noalias++;
      alias_stats.pta_resolved++;
      return false;
    }

  alias_stats.alias_mayalias++;
  return true;
}


/* Add ALIAS to the set of variables that may alias VAR.  */

static void
add_may_alias (tree var, tree alias)
{
  size_t i;
  var_ann_t v_ann = get_var_ann (var);
  var_ann_t a_ann = get_var_ann (alias);

#if defined ENABLE_CHECKING
  if (var == alias)
    abort ();
#endif

  if (v_ann->may_aliases == NULL)
    VARRAY_TREE_INIT (v_ann->may_aliases, 2, "aliases");

  /* Avoid adding duplicates.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (v_ann->may_aliases); i++)
    if (alias == VARRAY_TREE (v_ann->may_aliases, i))
      return;

  /* If VAR is a call-clobbered variable, so is its new ALIAS.  */
  if (is_call_clobbered (var))
    mark_call_clobbered (alias);

  /* Likewise.  If ALIAS is call-clobbered, so is VAR.  */
  else if (is_call_clobbered (alias))
    mark_call_clobbered (var);

  VARRAY_PUSH_TREE (v_ann->may_aliases, alias);
  a_ann->is_alias_tag = 1;
}


/* Given two pointers DEST and ORIG.  Merge the points-to information in
   ORIG into DEST.  AI is as in collect_points_to_info.  */

static void
merge_pointed_to_info (struct alias_info *ai, tree dest, tree orig)
{
  ssa_name_ann_t dest_ann, orig_ann;

  /* Make sure we have points-to information for ORIG.  */
  collect_points_to_info_for (ai, orig);

  dest_ann = get_ssa_name_ann (dest);
  orig_ann = ssa_name_ann (orig);

  if (orig_ann)
    {
      dest_ann->pt_anything |= orig_ann->pt_anything;
      dest_ann->pt_malloc |= orig_ann->pt_malloc;

      if (orig_ann->pt_vars)
	{
	  if (dest_ann->pt_vars == NULL)
	    {
	      dest_ann->pt_vars = BITMAP_GGC_ALLOC ();
	      bitmap_copy (dest_ann->pt_vars, orig_ann->pt_vars);
	    }
	  else
	    bitmap_a_or_b (dest_ann->pt_vars,
			   dest_ann->pt_vars,
			   orig_ann->pt_vars);
	}
    }
}


/* Add VALUE to the list of expressions pointed-to by PTR.  */

static void
add_pointed_to_expr (tree ptr, tree value)
{
  ssa_name_ann_t ann;

#if defined ENABLE_CHECKING
  /* Pointer variables should have been handled by merge_pointed_to_info.  */
  if (TREE_CODE (value) == SSA_NAME
      && POINTER_TYPE_P (TREE_TYPE (value)))
    abort ();
#endif

  ann = get_ssa_name_ann (ptr);

  /* If VALUE is the result of a malloc-like call, then the area pointed to
     PTR is guaranteed to not alias with anything else.  */
  if (TREE_CODE (value) == CALL_EXPR
      && (call_expr_flags (value) & (ECF_MALLOC | ECF_MAY_BE_ALLOCA)))
    ann->pt_malloc = 1;
  else
    ann->pt_anything = 1;

  if (tree_dump_file)
    {
      fprintf (tree_dump_file, "Pointer ");
      print_generic_expr (tree_dump_file, ptr, 0);
      fprintf (tree_dump_file, " points to ");
      if (ann->pt_malloc)
	fprintf (tree_dump_file, "malloc space: ");
      else
	fprintf (tree_dump_file, "an arbitrary address: ");
      print_generic_expr (tree_dump_file, value, 0);
      fprintf (tree_dump_file, "\n");
    }
}


/* If VALUE is of the form &DECL, add DECL to the set of variables
   pointed-to by PTR.  Otherwise, add VALUE as a pointed-to expression by
   PTR.  AI is as in collect_points_to_info.  */

static void
add_pointed_to_var (struct alias_info *ai, tree ptr, tree value)
{
  if (TREE_CODE (value) == ADDR_EXPR)
    {
      tree pt_var;
      ssa_name_ann_t ann;
      size_t uid;

      pt_var = TREE_OPERAND (value, 0);
      if (TREE_CODE_CLASS (TREE_CODE (pt_var)) == 'r')
	pt_var = get_base_decl (pt_var);

      if (pt_var && SSA_VAR_P (pt_var))
	{
	  ann = get_ssa_name_ann (ptr);
	  uid = var_ann (pt_var)->uid;
	  if (ann->pt_vars == NULL)
	    ann->pt_vars = BITMAP_GGC_ALLOC ();
	  bitmap_set_bit (ann->pt_vars, uid);
	  bitmap_set_bit (ai->addresses_needed, uid);
	}
      else
	add_pointed_to_expr (ptr, value);
    }
  else
    add_pointed_to_expr (ptr, value);
}


/* Callback for walk_use_def_chains to gather points-to information from the
   SSA web.
   
   VAR is an SSA variable or a GIMPLE expression.
   
   STMT is the statement that generates the SSA variable or, if STMT is a
      PHI_NODE, VAR is one of the PHI arguments.

   DATA is a pointer to a structure of type ALIAS_INFO.  */

static void
collect_points_to_info_r (tree var, tree stmt, void *data)
{
  struct alias_info *ai = (struct alias_info *) data;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "Visiting use-def links for ");
      print_generic_expr (tree_dump_file, var, 0);
      fprintf (tree_dump_file, "\n");
    }

  if (TREE_CODE (stmt) == MODIFY_EXPR)
    {
      tree rhs = TREE_OPERAND (stmt, 1);
      STRIP_NOPS (rhs);

      /* Found P_i = CONST.  */
      if (is_gimple_min_invariant (rhs))
	add_pointed_to_var (ai, var, rhs);

      /* Found P_i = Q_j.  */
      else if (TREE_CODE (rhs) == SSA_NAME
	       && POINTER_TYPE_P (TREE_TYPE (rhs)))
	merge_pointed_to_info (ai, var, rhs);

      /* Found P_i = PLUS_EXPR or P_i = MINUS_EXPR  */
      else if (TREE_CODE (rhs) == PLUS_EXPR
	       || TREE_CODE (rhs) == MINUS_EXPR)
	{
	  tree op0 = TREE_OPERAND (rhs, 0);
	  tree op1 = TREE_OPERAND (rhs, 1);

	  if (TREE_CODE (op0) == SSA_NAME
	      && POINTER_TYPE_P (TREE_TYPE (op0)))
	    merge_pointed_to_info (ai, var, op0);
	  else if (TREE_CODE (op1) == SSA_NAME
		   && POINTER_TYPE_P (TREE_TYPE (op1)))
	    merge_pointed_to_info (ai, var, op1);
	  else if (is_gimple_min_invariant (op0))
	    add_pointed_to_var (ai, var, op0);
	  else if (is_gimple_min_invariant (op1))
	    add_pointed_to_var (ai, var, op1);
	  else
	    add_pointed_to_expr (var, rhs);
	}

      /* Something else.  */
      else
	add_pointed_to_expr (var, rhs);
    }
  else if (TREE_CODE (stmt) == ASM_EXPR)
    {
      /* Pointers defined by __asm__ statements can point anywhere.  */
      ssa_name_ann_t ann = get_ssa_name_ann (var);
      ann->pt_anything = 1;
    }
  else if (IS_EMPTY_STMT (stmt))
    {
      tree decl = SSA_NAME_VAR (var);

      if (TREE_CODE (decl) == PARM_DECL)
	add_pointed_to_expr (var, decl);
      else if (DECL_INITIAL (decl))
	add_pointed_to_var (ai, var, DECL_INITIAL (decl));
      else
	add_pointed_to_expr (var, decl);
    }
  else if (TREE_CODE (stmt) == PHI_NODE)
    {
      tree lhs = PHI_RESULT (stmt);

      if (is_gimple_min_invariant (var))
	add_pointed_to_var (ai, lhs, var);
      else if (TREE_CODE (var) == SSA_NAME)
	merge_pointed_to_info (ai, lhs, var);
      else
	abort ();
    }
  else
    abort ();
}


/* Return true if STMT is an "escape" site from the current function.  Escape
   sites those statements which might expose the address of a variable
   outside the current function.  STMT is an escape site iff:

   	1- STMT is a function call, or
	2- STMT is an __asm__ expression, or
	3- STMT is an assignment to a non-local variable, or
	4- STMT is a return statement.

   If NUM_CALLS_P is not NULL, the counter is incremented if STMT contains
   a function call.  */

static bool
is_escape_site (tree stmt, size_t *num_calls_p)
{
  if (get_call_expr_in (stmt) != NULL_TREE)
    {
      if (num_calls_p)
	(*num_calls_p)++;

      return true;
    }
  else if (TREE_CODE (stmt) == ASM_EXPR)
    return true;
  else if (TREE_CODE (stmt) == MODIFY_EXPR)
    {
      tree lhs = TREE_OPERAND (stmt, 0);

      /* Get to the base of _REF nodes.  */
      if (TREE_CODE (lhs) != SSA_NAME)
	lhs = get_base_var (lhs);

      /* If we couldn't recognize the LHS of the assignment, assume that it
	 is a non-local store.  */
      if (lhs == NULL_TREE)
	return true;

      /* If the LHS is an SSA name, it can't possibly represent a non-local
	 memory store.  */
      if (TREE_CODE (lhs) == SSA_NAME)
	return false;

      if (!DECL_P (lhs))
	abort ();

      /* FIXME: LHS is not an SSA_NAME.  Even if it's an assignment to a
	 local variables we cannot be sure if it will escape, because we
	 don't have information about objects not in SSA form.  Need to
	 implement something along the lines of

	 J.-D. Choi, M. Gupta, M. J. Serrano, V. C. Sreedhar, and S. P.
	 Midkiff, ``Escape analysis for java,'' in Proceedings of the
	 Conference on Object-Oriented Programming Systems, Languages, and
	 Applications (OOPSLA), pp. 1-19, 1999.  */
      return true;
    }
  else if (TREE_CODE (stmt) == RETURN_EXPR)
    return true;

  return false;
}


/* Create a new memory tag of type TYPE.  If IS_TYPE_TAG is true, the tag
   is considered to represent all the pointers whose pointed-to types are
   in the same alias set class.  Otherwise, the tag represents a single
   SSA_NAME pointer variable.  */

static tree
create_memory_tag (tree type, bool is_type_tag)
{
  var_ann_t ann;
  tree tag = create_tmp_var_raw (type, (is_type_tag) ? "TMT" : "NMT");

  /* By default, memory tags are local variables.  Alias analysis will
     determine whether they should be considered globals.  */
  DECL_CONTEXT (tag) = current_function_decl;

  /* If the pointed-to type is volatile, so is the tag.  */
  TREE_THIS_VOLATILE (tag) = TREE_THIS_VOLATILE (type);

  /* Memory tags are by definition addressable.  This also prevents
     is_gimple_ref frome confusing memory tags with optimizable
     variables.  */
  TREE_ADDRESSABLE (tag) = 1;

  ann = get_var_ann (tag);
  ann->mem_tag_kind = (is_type_tag) ? TYPE_TAG : NAME_TAG;
  ann->type_mem_tag = NULL_TREE;

  /* Add the tag to the symbol table and mark it for renaming.  */
  add_referenced_tmp_var (tag);
  bitmap_set_bit (vars_to_rename, ann->uid);

  return tag;
}


/* Create a name memory tag to represent a specific SSA_NAME pointer P_i.
   This is used if P_i has been found to point to a specific set of
   variables or to a non-aliased memory location like the address returned
   by malloc functions.  */

static tree
get_nmt_for (tree ptr)
{
  ssa_name_ann_t ptr_ann = ssa_name_ann (ptr);
  tree tag = ptr_ann->name_mem_tag;

  if (tag == NULL_TREE)
    {
      tag = create_memory_tag (TREE_TYPE (TREE_TYPE (ptr)), false);

      /* If PTR is a PARM_DECL, its memory tag should be considered a
	 global variable.  */
      if (TREE_CODE (SSA_NAME_VAR (ptr)) == PARM_DECL)
	mark_call_clobbered (tag);

      /* Similarly, if PTR points to malloc, then TAG is a global.  */
      if (ptr_ann->pt_malloc)
	mark_call_clobbered (tag);
    }

  return tag;
}


/* Return the type memory tag associated to pointer PTR.  A memory tag is an
   artificial variable that represents the memory location pointed-to by
   PTR.  It is used to model the effects of pointer de-references on
   addressable variables.
   
   AI points to the data gathered during alias analysis.  This function
   populates the array AI->POINTERS.  */

static tree
get_tmt_for (tree ptr, struct alias_info *ai)
{
  size_t i;
  tree tag;
  tree tag_type = TREE_TYPE (TREE_TYPE (ptr));
  HOST_WIDE_INT tag_set = get_alias_set (tag_type);

  /* To avoid creating unnecessary memory tags, only create one memory tag
     per alias set class.  */
  for (i = 0, tag = NULL_TREE; i < VARRAY_ACTIVE_SIZE (ai->pointers); i++)
    {
      struct alias_map_d *curr = VARRAY_GENERIC_PTR (ai->pointers, i);
      if (tag_set == curr->set 
	  && (flag_tree_points_to == PTA_NONE 
	      || same_points_to_set (curr->var, ptr)))
	{
	  tag = var_ann (curr->var)->type_mem_tag;
	  break;
	}
    }

  /* If VAR cannot alias with any of the existing memory tags, create a new
     tag for PTR and add it to the POINTERS array.  */
  if (tag == NULL_TREE)
    {
      struct alias_map_d *alias_map;

      /* Create a new MT.* artificial variable representing the memory
	 location pointed-to by PTR.  */
      tag = create_memory_tag (tag_type, true);

      /* Add PTR to the POINTERS array.  Note that we are not interested in
	 PTR's alias set.  Instead, we cache the alias set for the memory that
	 PTR points to.  */
      alias_map = ggc_alloc (sizeof (*alias_map));
      memset (alias_map, 0, sizeof (*alias_map));
      alias_map->var = ptr;
      alias_map->set = tag_set;
      VARRAY_PUSH_GENERIC_PTR (ai->pointers, alias_map);
    }

  return tag;
}


/* Create GLOBAL_VAR, an artificial global variable to act as a
   representative of all the variables that may be clobbered by function
   calls.  */

static void
create_global_var (void)
{
  global_var = build_decl (VAR_DECL, get_identifier (".GLOBAL_VAR"),
                           size_type_node);
  DECL_ARTIFICIAL (global_var) = 1;
  TREE_READONLY (global_var) = 0;
  DECL_EXTERNAL (global_var) = 0;
  TREE_STATIC (global_var) = 1;
  TREE_USED (global_var) = 1;
  DECL_CONTEXT (global_var) = NULL_TREE;
  TREE_THIS_VOLATILE (global_var) = 0;
  TREE_ADDRESSABLE (global_var) = 0;

  add_referenced_tmp_var (global_var);
}


/* Dump alias statistics on FILE.  */

static void 
dump_alias_stats (FILE *file)
{
  const char *funcname
    = (*lang_hooks.decl_printable_name) (current_function_decl, 2);
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
  fprintf (file, "Total PTA queries:\t%u\n",
	   alias_stats.pta_queries);
  fprintf (file, "Total PTA resolved:\t%u\n",
	   alias_stats.pta_resolved);
}
  

/* Dump alias information on FILE.  */

void
dump_alias_info (FILE *file)
{
  size_t i;
  const char *funcname
    = (*lang_hooks.decl_printable_name) (current_function_decl, 2);

  fprintf (file, "\nAlias information for %s\n\n", funcname);

  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      var_ann_t ann = var_ann (var);
      if (ann->may_aliases
	  || ann->type_mem_tag
	  || ann->is_alias_tag
	  || ann->mem_tag_kind != NOT_A_TAG)
	dump_variable (file, var);
    }

  fprintf (file, "\n");
}


/* Dump alias information on stderr.  */

void
debug_alias_info (void)
{
  dump_alias_info (stderr);
}


/* Dump points-to information for SSA_NAME PTR into FILE.  */

static void
dump_points_to_info_for (FILE *file, tree ptr)
{
  ssa_name_ann_t ann = ssa_name_ann (ptr);

  fprintf (file, "Pointer ");
  print_generic_expr (file, ptr, 0);

  if (ann == NULL)
    return;

  if (ann->name_mem_tag)
    {
      fprintf (file, ", name memory tag: ");
      print_generic_expr (file, ann->name_mem_tag, 0);
    }

  if (ann->value_escapes_p)
    fprintf (file, ", its value escapes");

  if (ann->pt_anything)
    fprintf (file, ", points-to anything");

  if (ann->pt_malloc)
    fprintf (file, ", points-to malloc");

  if (ann->pt_vars)
    {
      unsigned ix;

      fprintf (file, ", points-to vars: { ");
      EXECUTE_IF_SET_IN_BITMAP (ann->pt_vars, 0, ix,
	  {
	    print_generic_expr (file, referenced_var (ix), 0);
	    fprintf (file, " ");
	  });
      fprintf (file, "}");
    }

  fprintf (file, "\n");
}


/* Dump points-to information into FILE.  NOTE: This function is slow, as
   it needs to traverse the whole CFG looking for pointer SSA_NAMEs.  */

void
dump_points_to_info (FILE *file)
{
  basic_block bb;
  block_stmt_iterator si;
  size_t i;
  const char *fname =
    (*lang_hooks.decl_printable_name) (current_function_decl, 2);

  fprintf (file, "\n\nPointed-to sets for pointers in %s\n\n", fname);

  /* First dump points-to information for the default definitions of
     pointer variables.  This is necessary because default definitions are
     not part of the code.  */
  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      if (POINTER_TYPE_P (TREE_TYPE (var)))
	{
	  var_ann_t ann = var_ann (var);
	  if (ann->default_def)
	    dump_points_to_info_for (file, ann->default_def);
	}
    }

  /* Dump points-to information for every pointer defined in the program.  */
  FOR_EACH_BB (bb)
    {
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	{
	  tree ptr = PHI_RESULT (phi);
	  if (POINTER_TYPE_P (TREE_TYPE (ptr)))
	    dump_points_to_info_for (file, ptr);
	}

	for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	  {
	    stmt_ann_t ann = stmt_ann (bsi_stmt (si));
	    def_optype defs = DEF_OPS (ann);
	    if (defs)
	      for (i = 0; i < NUM_DEFS (defs); i++)
		if (POINTER_TYPE_P (TREE_TYPE (DEF_OP (defs, i))))
		  dump_points_to_info_for (file, DEF_OP (defs, i));
	  }
    }

  fprintf (file, "\n");
}


/* Dump points-to info pointed by PTO into STDERR.  */

void
debug_points_to_info (void)
{
  dump_points_to_info (stderr);
}

/* Dump to FILE the list of variables that may be aliasing VAR.  */

void
dump_may_aliases_for (FILE *file, tree var)
{
  varray_type aliases;
  
  if (TREE_CODE (var) == SSA_NAME)
    var = SSA_NAME_VAR (var);

  aliases = var_ann (var)->may_aliases;
  if (aliases)
    {
      size_t i;
      fprintf (file, "{ ");
      for (i = 0; i < VARRAY_ACTIVE_SIZE (aliases); i++)
	{
	  print_generic_expr (file, VARRAY_TREE (aliases, i), 0);
	  fprintf (file, " ");
	}
      fprintf (file, "}");
    }
}


/* Dump to stderr the list of variables that may be aliasing VAR.  */

void
debug_may_aliases_for (tree var)
{
  dump_may_aliases_for (stderr, var);
}
