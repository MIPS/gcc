/* Data flow functions for trees.
   Copyright (C) 2001, 2002, 2003 Free Software Foundation, Inc.
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
#include "hashtab.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "errors.h"
#include "timevar.h"
#include "expr.h"
#include "ggc.h"
#include "langhooks.h"
#include "flags.h"
#include "function.h"
#include "diagnostic.h"
#include "tree-simple.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "tree-alias-common.h"
#include "convert.h"

/* Build and maintain data flow information for trees.  */

/* Counters used to display DFA and SSA statistics.  */
struct dfa_stats_d
{
  long num_stmt_anns;
  long num_var_anns;
  long num_defs;
  long num_uses;
  long num_phis;
  long num_phi_args;
  int max_num_phi_args;
  long num_vdefs;
  long num_vuses;
};

struct clobber_data_d
{
  tree stmt;
  voperands_t prev_vops;
};

/* Tuple to map a variable to its alias set.  Used to cache the results of
   calls to get_alias_set().  */
struct alias_map_d
{
  tree var;
  HOST_WIDE_INT set;
};


/* ADDRESSABLE_VARS contains all the global variables and locals that have
   had their address taken.  POINTERS contains all the pointers that have been
   referenced in the program.  Alias analysis will determine, for every two
   elements from each array whether they may alias each other or not.  */
static GTY(()) varray_type addressable_vars;
static GTY(()) varray_type pointers;

/* State information for find_vars_r.  */
struct walk_state
{
  /* Hash table used to avoid adding the same variable more than once.  */
  htab_t vars_found;

  /* Nonzero if the variables found under the current tree are written to.  */
  int is_store;

  /* Nonzero if the walker is inside an INDIRECT_REF node.  */
  int is_indirect_ref;
};


/* Flags to describe operand properties in get_stmt_operands and helpers.  */

/* By default, operands are loaded.  */
static const int opf_none	= 0;

/* Operand is the target of an assignment expression.  */
static const int opf_is_def 	= 1 << 0;

/* Consider the operand virtual, regardlessof aliasing information.  */
static const int opf_force_vop	= 1 << 1;


/* Data and functions shared with tree-ssa.c.  */
struct dfa_stats_d dfa_stats;
extern FILE *tree_ssa_dump_file;
extern int tree_ssa_dump_flags;


/* Local functions.  */
static void get_expr_operands (tree, tree *, int, voperands_t);
static void collect_dfa_stats (struct dfa_stats_d *);
static tree collect_dfa_stats_r (tree *, int *, void *);
static tree clobber_vars_r (tree *, int *, void *);
static void compute_alias_sets (void);
static bool may_alias_p (tree, HOST_WIDE_INT, tree, HOST_WIDE_INT);
static bool may_access_global_mem_p (tree);
static void set_def (tree *, tree);
static void add_use (tree *, tree);
static void add_vdef (tree, tree, voperands_t);
static void add_stmt_operand (tree *, tree, int, voperands_t);
static void add_immediate_use (tree, tree);
static tree find_vars_r (tree *, int *, void *);
static void add_referenced_var (tree, struct walk_state *);
static tree get_memory_tag_for (tree);
static void compute_immediate_uses_for (tree, int);
static void add_may_alias (tree, tree);
static bool call_may_clobber (tree);
static void find_hidden_use_vars (tree);
static tree find_hidden_use_vars_r (tree *, int *, void *);


/* Global declarations.  */

/* Array of all variables referenced in the function.  */
varray_type referenced_vars;

/* Arrays for all the call clobbered variables in the function.  */
varray_type call_clobbered_vars;

/* Artificial variable used to model the effects of function calls on every
   variable that they may use and define.  Calls to non-const and non-pure
   functions are assumed to use and clobber this variable.

   Loads and stores to call clobbered variables will be considered aliased
   with this variable.  */
tree global_var;

/* Get the operands of statement STMT.  Note that repeated calls to
   get_stmt_operands for the same statement will do nothing until the
   statement is marked modified by a call to modify_stmt().  */

void
get_stmt_operands (tree stmt)
{
  enum tree_code code;
  stmt_ann_t ann;
  voperands_t prev_vops = NULL;

#if defined ENABLE_CHECKING
  /* The optimizers cannot handle statements that are nothing but a
     _DECL.  This indicates a bug in the gimplifier.  */
  if (SSA_VAR_P (stmt))
    abort ();
#endif

  if (IS_EMPTY_STMT (stmt) || stmt == error_mark_node)
    return;

  /* If the statement has not been modified, the operands are still valid.  */
  if (!stmt_modified_p (stmt))
    return;

  ann = get_stmt_ann (stmt);

  /* Remove any existing operands as they will be scanned again.  */
  ann->ops = NULL;

  /* We cannot remove existing virtual operands because we would lose their
     SSA versions.  Instead, we save them on PREV_VOPS.  When add_vdef and
     add_vuse are called, they will do nothing if PREV_VOPS is set.
     FIXME: This means that passes that modify virtual operands must
            make the changes on their own.  */
  if (ann->vops)
    prev_vops = ann->vops;

  code = TREE_CODE (stmt);
  switch (code)
    {
    case MODIFY_EXPR:
      get_expr_operands (stmt, &TREE_OPERAND (stmt, 1), opf_none, prev_vops);
      get_expr_operands (stmt, &TREE_OPERAND (stmt, 0), opf_is_def, prev_vops);
      break;

    case COND_EXPR:
      get_expr_operands (stmt, &COND_EXPR_COND (stmt), opf_none, prev_vops);
      break;

    case SWITCH_EXPR:
      get_expr_operands (stmt, &SWITCH_COND (stmt), opf_none, prev_vops);
      break;

    case ASM_EXPR:
      get_expr_operands (stmt, &ASM_INPUTS (stmt), opf_none, prev_vops);
      get_expr_operands (stmt, &ASM_OUTPUTS (stmt), opf_is_def, prev_vops);
      get_expr_operands (stmt, &ASM_CLOBBERS (stmt), opf_is_def, prev_vops);
      break;

    case RETURN_EXPR:
      get_expr_operands (stmt, &TREE_OPERAND (stmt, 0), opf_none, prev_vops);
      break;

    case GOTO_EXPR:
      get_expr_operands (stmt, &GOTO_DESTINATION (stmt), opf_none, prev_vops);
      break;

    case LABEL_EXPR:
      get_expr_operands (stmt, &LABEL_EXPR_LABEL (stmt), opf_none, prev_vops);
      break;

      /* These nodes contain no variable references.  */
    case LOOP_EXPR:
    case BIND_EXPR:
    case CASE_LABEL_EXPR:
    case TRY_CATCH_EXPR:
    case TRY_FINALLY_EXPR:
    case EH_FILTER_EXPR:
    case CATCH_EXPR:
      break;

    default:
      /* Notice that if get_expr_operands tries to use &STMT as the operand
	 pointer (which may only happen for USE operands), we will abort in
	 add_use.  This default will handle statements like CALL_EXPRs or
	 VA_ARG_EXPRs that may appear on the RHS of a statement or as
	 statements themselves.  */
      get_expr_operands (stmt, &stmt, opf_none, prev_vops);
      break;
    }

  /* Resize the operand arrays.  */
  if (ann->ops && ann->ops->use_ops)
    VARRAY_GROW (ann->ops->use_ops, VARRAY_ACTIVE_SIZE (ann->ops->use_ops));

  if (ann->vops)
    {
      if (ann->vops->vdef_ops)
	VARRAY_GROW (ann->vops->vdef_ops,
		     VARRAY_ACTIVE_SIZE (ann->vops->vdef_ops));

      if (ann->vops->vuse_ops)
	VARRAY_GROW (ann->vops->vuse_ops,
		     VARRAY_ACTIVE_SIZE (ann->vops->vuse_ops));
    }

  /* Clear the modified bit for STMT.  Subsequent calls to
     get_stmt_operands for this statement will do nothing until the
     statement is marked modified by a call to modify_stmt().  */
  ann->modified = 0;
}


/* Recursively scan the expression pointed by EXPR_P in statement STMT.
   FLAGS is one of the OPF_* constants modifying how to interpret the
   operands found.  PREV_VOPS is as in add_vdef and add_vuse.  */

static void
get_expr_operands (tree stmt, tree *expr_p, int flags, voperands_t prev_vops)
{
  enum tree_code code;
  char class;
  tree expr = *expr_p;

  if (expr == NULL || expr == error_mark_node)
    return;

  code = TREE_CODE (expr);
  class = TREE_CODE_CLASS (code);

  /* Expressions that make no memory references.  */
  if (class == 'c'
      || class == 't'
      || class == 'b'
      || code == FUNCTION_DECL
      || code == EXC_PTR_EXPR
      || code == LABEL_DECL)
    return;

  /* We could have the address of a component, array member, etc which
     has interesting variable references.  */
  if (code == ADDR_EXPR)
    {
      enum tree_code subcode = TREE_CODE (TREE_OPERAND (expr, 0));

      /* Only a few specific types of ADDR_EXPR expressions are
       	 of interest.  */
      if (subcode != COMPONENT_REF
	  && subcode != INDIRECT_REF
	  && subcode != ARRAY_REF
	  && subcode != PARM_DECL
	  && subcode != VAR_DECL)
	return;

      /* Avoid recursion.  */
      flags |= opf_force_vop;
      code = subcode;
      class = TREE_CODE_CLASS (code);
      expr_p = &TREE_OPERAND (expr, 0);
      expr = *expr_p;
    }

  /* If this reference is associated with a non GIMPLE expression, then we
     mark the statement non GIMPLE and recursively clobber every
     variable referenced by STMT.  FIXME: TREE_NOT_GIMPLE must die.  */
  if (stmt && TREE_NOT_GIMPLE (expr))
    {
      struct clobber_data_d cd;
      mark_not_gimple (&stmt);
      cd.stmt = stmt;
      cd.prev_vops = prev_vops;
      walk_tree (&stmt, clobber_vars_r, (void *) &cd, NULL);
      return;
    }

  /* If the parent statement is marked not-gimple, don't do anything.  This
     means that in a previous iteration we encountered a non-gimple
     sub-expression which already clobbered all the variables in the
     statement.  FIXME: TREE_NOT_GIMPLE must die.  */
  if (stmt && TREE_NOT_GIMPLE (stmt))
    return;

  /* If we found a variable, add it to DEFS or USES depending on the
     operand flags.  */
  if (SSA_VAR_P (expr))
    {
      add_stmt_operand (expr_p, stmt, flags, prev_vops);
      return;
    }

  /* Pointer dereferences always represent a use of the base pointer.  */
  if (code == INDIRECT_REF)
    {
      var_ann_t ann;
      tree ptr = TREE_OPERAND (expr, 0);

#if defined ENABLE_CHECKING
      if (!SSA_VAR_P (ptr) && !TREE_CONSTANT (ptr))
	abort ();
#endif

      if (SSA_VAR_P (ptr))
	{
	  ann = var_ann (ptr);
	  if (ann->mem_tag)
	    add_stmt_operand (&ann->mem_tag, stmt, flags|opf_force_vop,
			      prev_vops);
	}

      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), opf_none, prev_vops);
      return;
    }

  /* Treat array references as references to the virtual variable
     representing the array.  The virtual variable for an ARRAY_REF is the
     VAR_DECL for the array.  */
  if (code == ARRAY_REF)
    {
      /* Add the virtual variable for the ARRAY_REF to VDEFS or VUSES
	 according to the value of IS_DEF.  Recurse if the LHS of the
	 ARRAY_REF node is not a regular variable.  */
      if (SSA_VAR_P (TREE_OPERAND (expr, 0)))
	add_stmt_operand (expr_p, stmt, flags, prev_vops);
      else
	get_expr_operands (stmt, &TREE_OPERAND (expr, 0), flags, prev_vops);

      get_expr_operands (stmt, &TREE_OPERAND (expr, 1), opf_none, prev_vops);
      return;
    }

  /* Similarly to arrays, references to compound variables (complex types
     and structures/unions) are globbed.

     FIXME: This means that

     			a.x = 6;
			a.y = 7;
			foo (a.x, a.y);

	   will not be constant propagated because the two partial
	   definitions to 'a' will kill each other.  */
  if (code == IMAGPART_EXPR || code == REALPART_EXPR || code == COMPONENT_REF)
    {
      /* If the LHS of the compound reference is not a regular variable,
	 recurse to keep looking for more operands in the subexpression.  */
      if (SSA_VAR_P (TREE_OPERAND (expr, 0)))
	add_stmt_operand (expr_p, stmt, flags, prev_vops);
      else
	get_expr_operands (stmt, &TREE_OPERAND (expr, 0), flags, prev_vops);

      return;
    }

  /* Function calls.  Add every argument to USES.  If the callee is
     neither pure nor const, create a VDEF reference for GLOBAL_VAR
     (See find_vars_r).  */
  if (code == CALL_EXPR)
    {
      tree op;
      bool may_clobber = call_may_clobber (expr);

      /* Find uses in the called function.  */
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), opf_none, prev_vops);

      for (op = TREE_OPERAND (expr, 1); op; op = TREE_CHAIN (op))
	add_stmt_operand (&TREE_VALUE (op), stmt, opf_none, prev_vops);

      /* If the called function is neither pure nor const and there are
	 call clobbered variables, create a definition of GLOBAL_VAR and
	 mark the statement as a clobbering statement.  */
      if (may_clobber && num_call_clobbered_vars > 0)
	{
	  stmt_ann (stmt)->makes_clobbering_call = may_clobber;
	  add_stmt_operand (&global_var, stmt, opf_is_def|opf_force_vop,
			    prev_vops);
	}

      return;
    }

  /* Lists.  */
  if (code == TREE_LIST)
    {
      tree op;

      for (op = expr; op; op = TREE_CHAIN (op))
        get_expr_operands (stmt, &TREE_VALUE (op), flags, prev_vops);

      return;
    }

  /* Assignments.  */
  if (code == MODIFY_EXPR)
    {
      get_expr_operands (stmt, &TREE_OPERAND (expr, 1), opf_none, prev_vops);
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), opf_is_def, prev_vops);
      return;
    }

  /* VA_ARG_EXPR nodes read and modify the argument pointer.  Add it to
     VOPS to avoid optimizations messing it up.  */
  if (code == VA_ARG_EXPR)
    {
      add_stmt_operand (&TREE_OPERAND (expr, 0), stmt, opf_is_def|opf_force_vop,
			prev_vops);
      return;
    }

  /* Unary expressions.  */
  if (class == '1'
      || code == BIT_FIELD_REF
      || code == CONSTRUCTOR)
    {
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), flags, prev_vops);
      return;
    }

  /* Binary expressions.  */
  if (class == '2'
      || class == '<'
      || code == TRUTH_AND_EXPR
      || code == TRUTH_OR_EXPR
      || code == TRUTH_XOR_EXPR
      || code == COMPOUND_EXPR)
    {
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), flags, prev_vops);
      get_expr_operands (stmt, &TREE_OPERAND (expr, 1), flags, prev_vops);
      return;
    }

  /* If we get here, something has gone wrong.  */
  fprintf (stderr, "unhandled expression in get_expr_operands():\n");
  debug_tree (expr);
  fputs ("\n", stderr);
  abort ();
}


/* Add *VAR_P to the appropriate operand array of STMT.  FLAGS is as in
   get_expr_operands.  The following are the rules used to decide
   whether an operand belongs in OPS or VOPS:

   1- Non-aliased scalar and pointer variables are real operands.

   2- If a variable is aliased, all its aliases are added to the virtual
      operands.

   3- For non-scalar variables (arrays, structures, unions and complex
      types), their virtual variable (see get_virtual_var) is added to the
      virtual operands.

   PREV_VOPS is used when adding virtual operands to statements that
      already had them (See add_vdef and add_vuse).  */

static void
add_stmt_operand (tree *var_p, tree stmt, int flags, voperands_t prev_vops)
{
  bool is_scalar;
  tree var;
  varray_type aliases;
  size_t i;
  stmt_ann_t s_ann;
  var_ann_t v_ann;

  var = *var_p;
  STRIP_NOPS (var);

  /* If the original variable is not a scalar, it will be added to the list
     of virtual operands.  In that case, use its base symbol as the virtual
     variable representing it.  */
  is_scalar = (SSA_VAR_P (var) && !AGGREGATE_TYPE_P (TREE_TYPE (var)));
  if (!is_scalar && !DECL_P (var))
    var = get_virtual_var (var);

  /* If VAR is not a variable, do nothing.  */
  if (var == NULL_TREE || !SSA_VAR_P (var))
    return;

  s_ann = stmt_ann (stmt);
  v_ann = var_ann (var);

  /* FIXME: Currently, global and local static variables are always treated as
     virtual operands.  Otherwise, we would have to insert copy-in/copy-out
     operations at escape points in the function (e.g., at call sites and
     return points). The additional overhead of inserting these copies may
     negate the optimizations enabled by renaming globals.  */
  if (decl_function_context (!DECL_P (var) ? get_base_symbol (var) : var) == 0
      || TREE_STATIC ((!DECL_P (var) ? get_base_symbol (var) : var)))
    {
      flags |= opf_force_vop;
      s_ann->has_volatile_ops = 1;
    }

  aliases = v_ann->may_aliases;
  if (aliases == NULL)
    {
      /* The variable is not aliased.  If it's a scalar that is not used as
	 an alias tag for other variables, process it as a real operand.
	 Otherwise, add it to the virtual operands.  Note that we never
	 consider ASM_EXPR operands as real.  They are always added to
	 virtual operands so that optimizations don't try to optimize them.

	 FIXME: This is true for CCP.  It tries to propagate constants in
		some __asm__ operands causing ICEs during RTL expansion
		(execute/20020107-1.c).  Do we really need to be this
		drastic?  Or should each optimization take care when
		dealing with ASM_EXPRs?  */
      if (flags & opf_is_def)
	{
	  if (is_scalar
	      && !(flags & opf_force_vop)
	      && TREE_CODE (stmt) == MODIFY_EXPR)
	    set_def (var_p, stmt);
	  else
	    add_vdef (var, stmt, prev_vops);

	  /* If the variable is an alias tag, mark the statement.  */
	  if (v_ann->is_alias_tag)
	    s_ann->makes_aliased_stores = 1;
	}
      else
	{
	  if (is_scalar
	      && !(flags & opf_force_vop)
	      && TREE_CODE (stmt) != ASM_EXPR)
	    add_use (var_p, stmt);
	  else
	    add_vuse (var, stmt, prev_vops);

	  /* If the variable is an alias tag, mark the statement.  */
	  if (v_ann->is_alias_tag)
	    s_ann->makes_aliased_stores = 1;
	}

      /* If the variable is volatile, inform the statement that it makes
	 volatile storage references.  */
      if (TREE_THIS_VOLATILE (var))
	s_ann->has_volatile_ops = 1;
    }
  else
    {
      /* The variable is aliased.  Add its aliases to the virtual operands.  */
      if (flags & opf_is_def)
	{
	  /* If the variable is also an alias tag, add a virtual operand
	     for it, otherwise we will miss representing references to the
	     members of the variable's alias set.  This fixes the bug in
	     gcc.c-torture/execute/20020503-1.c.  */
	  if (v_ann->is_alias_tag)
	    add_vdef (var, stmt, prev_vops);

	  for (i = 0; i < VARRAY_ACTIVE_SIZE (aliases); i++)
	    add_vdef (VARRAY_TREE (aliases, i), stmt, prev_vops);

	  s_ann->makes_aliased_stores = 1;
	}
      else
	{
	  if (v_ann->is_alias_tag)
	    add_vuse (var, stmt, prev_vops);

	  for (i = 0; i < VARRAY_ACTIVE_SIZE (aliases); i++)
	    add_vuse (VARRAY_TREE (aliases, i), stmt, prev_vops);

	  s_ann->makes_aliased_loads = 1;
	}
    }
}


/* Set DEF_P to be the pointer to the operand defined by STMT.  */

static void
set_def (tree *def_p, tree stmt)
{
  stmt_ann_t ann = stmt_ann (stmt);

#if defined ENABLE_CHECKING
  /* There should only be a single real definition per
     assignment.  */
  if (ann->ops && ann->ops->def_op)
    abort ();
#endif

  if (ann->ops == NULL)
    {
      ann->ops = ggc_alloc (sizeof (struct operands_d));
      memset ((void *) ann->ops, 0, sizeof (*(ann->ops)));
    }

  ann->ops->def_op = def_p;
  get_var_ann (*def_p)->has_real_refs = 1;
}


/* Add USE_P to the list of pointers to operands used by STMT.  */

static void
add_use (tree *use_p, tree stmt)
{
  stmt_ann_t ann = stmt_ann (stmt);

#if defined ENABLE_CHECKING
  /* If the pointer to the operand is the statement itself, something is
     wrong.  It means that we are pointing to a local variable (the initial
     call to get_stmt_operands does not pass a pointer to a statement).  */
  if (*use_p == stmt)
    abort ();
#endif

  if (ann->ops == NULL)
    {
      ann->ops = ggc_alloc (sizeof (struct operands_d));
      memset ((void *) ann->ops, 0, sizeof (*(ann->ops)));
    }

  if (ann->ops->use_ops == NULL)
    VARRAY_GENERIC_PTR_INIT (ann->ops->use_ops, 3, "use_ops");

  VARRAY_PUSH_GENERIC_PTR (ann->ops->use_ops, use_p);
  get_var_ann (*use_p)->has_real_refs = 1;
}


/* Add a new VDEF_EXPR for variable VAR to statement STMT.  If PREV_VOPS
   is not NULL, the existing entries are preserved and no new entries are
   added here.  This is done to preserve the SSA numbering of virtual
   operands.  */

static void
add_vdef (tree var, tree stmt, voperands_t prev_vops)
{
  tree vdef;
  stmt_ann_t ann;
  size_t i;

  if (prev_vops && prev_vops->vdef_ops)
    return;
  else
    vdef = build_vdef_expr (var);

  ann = stmt_ann (stmt);
  if (ann->vops == NULL)
    {
      ann->vops = ggc_alloc (sizeof (struct voperands_d));
      memset ((void *) ann->vops, 0, sizeof (*(ann->vops)));
    }

  if (ann->vops->vdef_ops == NULL)
    VARRAY_TREE_INIT (ann->vops->vdef_ops, 5, "vdef_ops");

  /* Don't allow duplicate entries.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ann->vops->vdef_ops); i++)
    if (var == VDEF_RESULT (VARRAY_TREE (ann->vops->vdef_ops, i)))
      return;

  VARRAY_PUSH_TREE (ann->vops->vdef_ops, vdef);
}


/* Add VAR to the list of virtual uses for STMT.  If PREV_VOPS
   is not NULL, the existing entries are preserved and no new entries are
   added here.  This is done to preserve the SSA numbering of virtual
   operands.  */

void
add_vuse (tree var, tree stmt, voperands_t prev_vops)
{
  stmt_ann_t ann;
  size_t i;

  if (prev_vops && prev_vops->vuse_ops)
    return;

  ann = stmt_ann (stmt);
  if (ann->vops == NULL)
    {
      ann->vops = ggc_alloc (sizeof (struct voperands_d));
      memset ((void *) ann->vops, 0, sizeof (*(ann->vops)));
    }

  if (ann->vops->vuse_ops == NULL)
    VARRAY_TREE_INIT (ann->vops->vuse_ops, 5, "vuse_ops");

  /* Don't allow duplicate entries.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ann->vops->vuse_ops); i++)
    if (var == VARRAY_TREE (ann->vops->vuse_ops, i))
      return;

  VARRAY_PUSH_TREE (ann->vops->vuse_ops, var);
}


/* Create a new PHI node for variable VAR at basic block BB.  */

tree
create_phi_node (tree var, basic_block bb)
{
  tree phi;
  bb_ann_t ann;
  int len;
  edge e;

  for (len = 0, e = bb->pred; e; e = e->pred_next)
    len++;

  phi = make_phi_node (var, len);

  /* Add the new PHI node to the list of PHI nodes for block BB.  */
  ann = bb_ann (bb);
  if (ann->phi_nodes == NULL)
    ann->phi_nodes = phi;
  else
    chainon (ann->phi_nodes, phi);

  /* Associate BB to the PHI node.  */
  set_bb_for_stmt (phi, bb);

  return phi;
}


/* Add a new argument to PHI node PHI.  DEF is the incoming reaching
   definition and E is the edge through which DEF reaches PHI.  The new
   argument is added at the end of the argument list.  */

void
add_phi_arg (tree phi, tree def, edge e)
{
  int i = PHI_NUM_ARGS (phi);

#if defined ENABLE_CHECKING
  if (i >= PHI_ARG_CAPACITY (phi))
    abort ();
#endif

  /* Copy propagation needs to know what object occur in abnormal
     PHI nodes.  This is a convenient place to record such information.  */
  if (e->flags & EDGE_ABNORMAL)
    {
      var_ann (def)->occurs_in_abnormal_phi = 1;
      var_ann (PHI_RESULT (phi))->occurs_in_abnormal_phi = 1;
    }

  PHI_ARG_DEF (phi, i) = def;
  PHI_ARG_EDGE (phi, i) = e;
  PHI_NUM_ARGS (phi)++;
}


/* Remove a PHI argument from PHI.  BLOCK is the predecessor block where
   the PHI argument is coming from.  */

void
remove_phi_arg (tree phi, basic_block block)
{
  int i, num_elem = PHI_NUM_ARGS (phi);

  for (i = 0; i < num_elem; i++)
    {
      basic_block src_bb;

      src_bb = PHI_ARG_EDGE (phi, i)->src;

      if (src_bb == block)
	{
	  remove_phi_arg_num (phi, i);
	  return;
	}
    }
}


/* Remove the Ith argument from PHI's argument list.  This routine assumes
   ordering of alternatives in the vector is not important and implements
   removal by swapping the last alternative with the alternative we want to
   delete, then shrinking the vector.  */

void
remove_phi_arg_num (tree phi, int i)
{
  int num_elem = PHI_NUM_ARGS (phi);

  /* If we are not at the last element, switch the last element
     with the element we want to delete.  */
  if (i != num_elem - 1)
    {
      PHI_ARG_DEF (phi, i) = PHI_ARG_DEF (phi, num_elem - 1);
      PHI_ARG_EDGE (phi, i) = PHI_ARG_EDGE (phi, num_elem - 1);
    }

  /* Shrink the vector and return.  */
  PHI_ARG_DEF (phi, num_elem - 1) = NULL_TREE;
  PHI_ARG_EDGE (phi, num_elem - 1) = NULL;
  PHI_NUM_ARGS (phi)--;
}


/* Remove PHI node PHI from basic block BB.  If PREV is non-NULL, it is
   used as the node immediately before PHI in the linked list.  */

void
remove_phi_node (tree phi, tree prev, basic_block bb)
{
  if (prev)
    {
      /* Rewire the list if we are given a PREV pointer.  */
      TREE_CHAIN (prev) = TREE_CHAIN (phi);
    }
  else if (phi == phi_nodes (bb))
    {
      /* Update the list head if removing the first element.  */
      bb_ann_t ann = bb_ann (bb);
      ann->phi_nodes = TREE_CHAIN (phi);
    }
  else
    {
      /* Traverse the list looking for the node to remove.  */
      tree prev, t;
      prev = NULL_TREE;
      for (t = phi_nodes (bb); t && t != phi; t = TREE_CHAIN (t))
	prev = t;
      if (t)
	remove_phi_node (t, prev, bb);
    }
}


/*---------------------------------------------------------------------------
			Dataflow analysis (DFA) routines
---------------------------------------------------------------------------*/
/* Compute immediate uses.  */

void
compute_immediate_uses (int flags)
{
  basic_block bb;
  block_stmt_iterator si;

  FOR_EACH_BB (bb)
    {
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	compute_immediate_uses_for (phi, flags);

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	compute_immediate_uses_for (bsi_stmt (si), flags);
    }
}

/* Helper for compute_immediate_uses.  Check all the USE and/or VUSE
   operands in STMT and add a def-use edge between their defining statement
   and STMT.  */

static void
compute_immediate_uses_for (tree stmt, int flags)
{
  size_t i;
  varray_type ops;

  /* PHI nodes are a special case.  We only need to look at its arguments.  */
  if (TREE_CODE (stmt) == PHI_NODE)
    {
      int i;

      for (i = 0; i < PHI_NUM_ARGS (stmt); i++)
	{
	  tree imm_rdef_stmt = SSA_NAME_DEF_STMT (PHI_ARG_DEF (stmt, i));
	  if (!IS_EMPTY_STMT (imm_rdef_stmt))
	    add_immediate_use (imm_rdef_stmt, stmt);
	}
      return;
    }

  /* Otherwise, we look at USE_OPS or VUSE_OPS according to FLAGS.  */
  get_stmt_operands (stmt);

  if ((flags & TDFA_USE_OPS) && use_ops (stmt))
    {
      ops = use_ops (stmt);
      for (i = 0; i < VARRAY_ACTIVE_SIZE (ops); i++)
	{
	  tree *use_p = VARRAY_GENERIC_PTR (ops, i);
	  tree imm_rdef_stmt = SSA_NAME_DEF_STMT (*use_p);
	  if (!IS_EMPTY_STMT (imm_rdef_stmt))
	    add_immediate_use (imm_rdef_stmt, stmt);
	}
    }

  if ((flags & TDFA_USE_VOPS) && vuse_ops (stmt))
    {
      ops = vuse_ops (stmt);
      for (i = 0; i < VARRAY_ACTIVE_SIZE (ops); i++)
	{
	  tree vuse = VARRAY_TREE (ops, i);
	  tree imm_rdef_stmt = SSA_NAME_DEF_STMT (vuse);
	  if (!IS_EMPTY_STMT (imm_rdef_stmt))
	    add_immediate_use (imm_rdef_stmt, stmt);
	}
    }
}


/* Compute reached uses.  */

void
compute_reached_uses (int flags ATTRIBUTE_UNUSED)
{
  abort ();
}


/* Compute reaching definitions.  */

void
compute_reaching_defs (int flags ATTRIBUTE_UNUSED)
{
  abort ();
}



/* Add statement USE_STMT to the list of statements that use definitions
    made by STMT.  */

static void
add_immediate_use (tree stmt, tree use_stmt)
{
  stmt_ann_t ann = get_stmt_ann (stmt);

  if (ann->df == NULL)
    {
      ann->df = ggc_alloc (sizeof (struct dataflow_d));
      memset ((void *) ann->df, 0, sizeof (*(ann->df)));
    }

  if (ann->df->immediate_uses == NULL)
    VARRAY_GENERIC_PTR_INIT (ann->df->immediate_uses, 10, "immediate_uses");

  VARRAY_PUSH_TREE (ann->df->immediate_uses, use_stmt);
}


/*---------------------------------------------------------------------------
			    Manage annotations
---------------------------------------------------------------------------*/
/* Create a new annotation for a _DECL node T.  */

var_ann_t
create_var_ann (tree t)
{
  var_ann_t ann;

#if defined ENABLE_CHECKING
  if (t == NULL_TREE || !DECL_P (t))
    abort ();
#endif

  ann = ggc_alloc (sizeof (*ann));
  memset ((void *) ann, 0, sizeof (*ann));

  ann->common.type = VAR_ANN;

  t->common.ann = (tree_ann) ann;

  return ann;
}


/* Create a new annotation for a statement node T.  */

stmt_ann_t
create_stmt_ann (tree t)
{
  stmt_ann_t ann;

#if defined ENABLE_CHECKING
  if (t == NULL_TREE
      || TREE_CODE_CLASS (TREE_CODE (t)) == 'c'
      || TREE_CODE_CLASS (TREE_CODE (t)) == 't')
    abort ();
#endif

  ann = ggc_alloc (sizeof (*ann));
  memset ((void *) ann, 0, sizeof (*ann));

  ann->common.type = STMT_ANN;

  /* Since we just created the annotation, mark the statement modified.  */
  ann->modified = true;

  t->common.ann = (tree_ann) ann;
  ann->common.stmt = t; 

  /* SSA-PRE currently needs to be able to get to the parent node of an
     arbitrary RHS.  This should eventually be fixed as it makes removal
     of annotations more complicated than it should be (consider 
     propagation of a constant that appeared on the RHS of a MODIFY_EXPR
     and eventual removal of the MODIFY_EXPR). 

     If additional annotations are created, then the removal code for
     annotations needs to be updated.  Such code appears in 
     remove_annotation_r and remove_stmt.  */
  if (TREE_CODE (t) == MODIFY_EXPR)
    {
      tree op = TREE_OPERAND (t, 1);
      if (op->common.ann != NULL)
        op->common.ann->common.stmt = t;
      else
        {
          op->common.ann = ggc_alloc (sizeof (struct tree_ann_common_d));
          op->common.ann->common.type = TREE_ANN_COMMON;
	  op->common.ann->common.stmt = t;
        }
    }

  return ann;
}


/*---------------------------------------------------------------------------
			      Debugging functions
---------------------------------------------------------------------------*/
/* Dump the list of all the referenced variables in the current function to
   FILE.  */

void
dump_referenced_vars (FILE *file)
{
  size_t i;

  fprintf (file, "\nReferenced variables in %s: %u\n\n", 
	   get_name (current_function_decl), (unsigned) num_referenced_vars);

  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      fprintf (file, "Variable: ");
      dump_variable (file, var);
      fprintf (file, "\n");
    }
}


/* Dump the list of all the referenced variables to stderr.  */

void
debug_referenced_vars (void)
{
  dump_referenced_vars (stderr);
}


/* Dump variable VAR and its may-aliases to FILE.  */

void
dump_variable (FILE *file, tree var)
{
  var_ann_t ann;

  if (var == NULL_TREE)
    {
      fprintf (file, "<nil>");
      return;
    }

  print_generic_expr (file, var, 0);
  
  ann = var_ann (var);

  if (ann->mem_tag)
    {
      fprintf (file, ", memory tag: ");
      print_generic_expr (file, ann->mem_tag, 0);
    }

  if (ann->is_alias_tag)
    fprintf (file, ", is an alias tag");

  if (ann->may_alias_global_mem)
    fprintf (file, ", may alias global memory");

  if (ann->has_hidden_use)
    fprintf (file, ", has a hidden use");

  if (ann->may_point_to_global_mem)
    fprintf (file, ", may point to global memory");

  if (ann->is_call_clobbered)
    fprintf (file, ", call clobbered");

  if (ann->occurs_in_abnormal_phi)
    fprintf (file, ", occurs in an abnormal PHI node");

  if (ann->is_stored)
    fprintf (file, ", is stored");

  if (ann->may_aliases)
    {
      fprintf (file, ", may aliases: ");
      dump_may_aliases_for (file, var);
    }

  fprintf (file, "\n");
}


/* Dump variable VAR and its may-aliases to stderr.  */

void
debug_variable (tree var)
{
  dump_variable (stderr, var);
}


/* Dump to FILE the list of variables that may be aliasing VAR.  */

void
dump_may_aliases_for (FILE *file, tree var)
{
  varray_type aliases = var_ann (var)->may_aliases;

  if (aliases)
    {
      size_t i, num_aliases = VARRAY_ACTIVE_SIZE (aliases);

      fprintf (file, "{ ");
      for (i = 0; i < num_aliases; i++)
	{
	  print_generic_expr (file, VARRAY_TREE (aliases, i), 0);
	  fprintf (file, " ");
	}
      fprintf (file, "}");
    }

  fprintf (file, "\n");
}


/* Dump to stderr the list of variables that may be aliasing VAR.  */

void
debug_may_aliases_for (tree var)
{
  dump_may_aliases_for (stderr, var);
}


/* Dump def-use edges on FILE.  */

void
dump_immediate_uses (FILE *file)
{
  basic_block bb;
  block_stmt_iterator si;
  const char *funcname
    = (*lang_hooks.decl_printable_name) (current_function_decl, 2);

  fprintf (file, "\nDef-use edges for function %s\n", funcname);

  FOR_EACH_BB (bb)
    {
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	dump_immediate_uses_for (file, phi);

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	dump_immediate_uses_for (file, bsi_stmt (si));
    }

  fprintf (file, "\n");
}


/* Dump def-use edges on stderr.  */

void
debug_immediate_uses (void)
{
  dump_immediate_uses (stderr);
}


/* Dump all immediate uses for STMT on FILE.  */

void
dump_immediate_uses_for (FILE *file, tree stmt)
{
  varray_type imm_uses = immediate_uses (stmt);

  if (imm_uses)
    {
      size_t i;

      fprintf (file, "-> ");
      print_generic_stmt (file, stmt, TDF_SLIM);
      fprintf (file, "\n");

      for (i = 0; i < VARRAY_ACTIVE_SIZE (imm_uses); i++)
	{
	  fprintf (file, "\t");
	  print_generic_stmt (file, VARRAY_TREE (imm_uses, i), TDF_SLIM);
	  fprintf (file, "\n");
	}

      fprintf (file, "\n");
    }
}


/* Dump immediate uses for STMT on stderr.  */

void
debug_immediate_uses_for (tree stmt)
{
  dump_immediate_uses_for (stderr, stmt);
}


/* Dump various DFA statistics to FILE.  */

void
dump_dfa_stats (FILE *file)
{
  struct dfa_stats_d dfa_stats;

  unsigned long size, total = 0;
  const char * const fmt_str   = "%-30s%-13s%12s\n";
  const char * const fmt_str_1 = "%-30s%13lu%11lu%c\n";
  const char * const fmt_str_3 = "%-43s%11lu%c\n";
  const char *funcname
    = (*lang_hooks.decl_printable_name) (current_function_decl, 2);

  collect_dfa_stats (&dfa_stats);

  fprintf (file, "\nDFA Statistics for %s\n\n", funcname);

  fprintf (file, "---------------------------------------------------------\n");
  fprintf (file, fmt_str, "", "  Number of  ", "Memory");
  fprintf (file, fmt_str, "", "  instances  ", "used ");
  fprintf (file, "---------------------------------------------------------\n");

  size = num_referenced_vars * sizeof (tree);
  total += size;
  fprintf (file, fmt_str_1, "Referenced variables", num_referenced_vars, 
	   SCALE (size), LABEL (size));

  size = num_call_clobbered_vars * sizeof (tree);
  total += size;
  fprintf (file, fmt_str_1, "Call clobbered variables", num_call_clobbered_vars,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_stmt_anns * sizeof (struct stmt_ann_d);
  total += size;
  fprintf (file, fmt_str_1, "Statements annotated", dfa_stats.num_stmt_anns,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_var_anns * sizeof (struct var_ann_d);
  total += size;
  fprintf (file, fmt_str_1, "Variables annotated", dfa_stats.num_var_anns,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_uses * sizeof (tree *);
  total += size;
  fprintf (file, fmt_str_1, "USE operands", dfa_stats.num_uses,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_defs * sizeof (tree *);
  total += size;
  fprintf (file, fmt_str_1, "DEF operands", dfa_stats.num_defs,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_vuses * sizeof (tree *);
  total += size;
  fprintf (file, fmt_str_1, "VUSE operands", dfa_stats.num_vuses,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_vdefs * sizeof (tree *);
  total += size;
  fprintf (file, fmt_str_1, "VDEF operands", dfa_stats.num_vdefs,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_phis * sizeof (struct tree_phi_node);
  total += size;
  fprintf (file, fmt_str_1, "PHI nodes", dfa_stats.num_phis,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_phi_args * sizeof (struct phi_arg_d);
  total += size;
  fprintf (file, fmt_str_1, "PHI arguments", dfa_stats.num_phi_args,
 	   SCALE (size), LABEL (size));

  fprintf (file, "---------------------------------------------------------\n");
  fprintf (file, fmt_str_3, "Total memory used by DFA/SSA data", SCALE (total),
	   LABEL (total));
  fprintf (file, "---------------------------------------------------------\n");
  fprintf (file, "\n");

  if (dfa_stats.num_phis)
    fprintf (file, "Average number of arguments per PHI node: %.1f (max: %d)\n",
	     (float) dfa_stats.num_phi_args / (float) dfa_stats.num_phis,
	     dfa_stats.max_num_phi_args);

  fprintf (file, "\n");
}


/* Dump DFA statistics on stderr.  */

void
debug_dfa_stats (void)
{
  dump_dfa_stats (stderr);
}


/* Collect DFA statistics and store them in the structure pointed by
   DFA_STATS_P.  */

static void
collect_dfa_stats (struct dfa_stats_d *dfa_stats_p)
{
  htab_t htab;
  basic_block bb;
  block_stmt_iterator i;

  if (dfa_stats_p == NULL)
    abort ();

  memset ((void *)dfa_stats_p, 0, sizeof (struct dfa_stats_d));

  /* Walk all the trees in the function counting references.  Start at
     basic block 0, but don't stop at block boundaries.  */
  htab = htab_create (30, htab_hash_pointer, htab_eq_pointer, NULL);

  for (i = bsi_start (BASIC_BLOCK (0)); !bsi_end_p (i); bsi_next (&i))
    walk_tree (bsi_stmt_ptr (i), collect_dfa_stats_r, (void *) dfa_stats_p,
	       (void *) htab);

  htab_delete (htab);

  FOR_EACH_BB (bb)
    {
      tree phi;
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	{
	  dfa_stats_p->num_phis++;
	  dfa_stats_p->num_phi_args += PHI_NUM_ARGS (phi);
	  if (PHI_NUM_ARGS (phi) > dfa_stats_p->max_num_phi_args)
	    dfa_stats_p->max_num_phi_args = PHI_NUM_ARGS (phi);
	}
    }
}


/* Callback for walk_tree to collect DFA statistics for a tree and its
   children.  */

static tree
collect_dfa_stats_r (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED,
		     void *data)
{
  tree t = *tp;
  struct dfa_stats_d *dfa_stats_p = (struct dfa_stats_d *)data;

  if (t->common.ann)
    {
      switch (ann_type (t->common.ann))
	{
	case STMT_ANN:
	  {
	    stmt_ann_t ann = (stmt_ann_t) t->common.ann;
	    dfa_stats_p->num_stmt_anns++;
	    if (ann->ops)
	      {
		if (ann->ops->def_op)
		  dfa_stats_p->num_defs++;
		if (ann->ops->use_ops)
		  dfa_stats_p->num_uses += VARRAY_ACTIVE_SIZE (ann->ops->use_ops);
	      }

	    if (ann->vops)
	      {
		voperands_t vops = ann->vops;

		if (vops->vdef_ops)
		  dfa_stats_p->num_vdefs += VARRAY_ACTIVE_SIZE (vops->vdef_ops);

		if (vops->vuse_ops)
		  dfa_stats_p->num_vuses += VARRAY_ACTIVE_SIZE (vops->vuse_ops);
	      }
	    break;
	  }

	case VAR_ANN:
	  dfa_stats_p->num_var_anns++;
	  break;

	default:
	  break;
	}
    }

  return NULL;
}


/* Callback for walk_tree, create may-def/may-use references for every
   declaration node and compound reference found under a given tree node
   TP.  */

static tree
clobber_vars_r (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED,
		void *data)
{
  enum tree_code code = TREE_CODE (*tp);

  /* Add every *_DECL node to VDEFS and VUSES.  */
  if (code == VAR_DECL || code == PARM_DECL || code == RESULT_DECL)
    {
      struct clobber_data_d *cd = (struct clobber_data_d *) data;
      add_vuse (*tp, cd->stmt, cd->prev_vops);
      add_vdef (*tp, cd->stmt, cd->prev_vops);
    }

  return NULL;
}


/*---------------------------------------------------------------------------
				    Aliasing
---------------------------------------------------------------------------*/
/* Compute may-alias information for every variable referenced in the
   program.  Note that in the absence of points-to analysis
   (-ftree-points-to), this may compute a much bigger set than necessary.  */

void
compute_may_aliases (tree fndecl)
{
  static htab_t vars_found;
  basic_block bb;
  block_stmt_iterator si;
  struct walk_state walk_state;
  tree block;

  timevar_push (TV_TREE_MAY_ALIAS);

  /* Walk the lexical blocks in the function looking for variables that may
     have been used to declare VLAs and for nested functions.  Both
     constructs create hidden uses of variables. 

     Note that at this point we may have multiple blocks hung off
     DECL_INITIAL chained through the BLOCK_CHAIN field due to
     how inlining works.  Egad.  */
  block = DECL_INITIAL (fndecl);
  while (block)
    {
      find_hidden_use_vars (block);
      block = BLOCK_CHAIN (block);
    }

  VARRAY_GENERIC_PTR_INIT (addressable_vars, 20, "addressable_vars");
  VARRAY_GENERIC_PTR_INIT (pointers, 20, "pointers");

  /* Hash table of all the objects the SSA builder needs to be aware of.  */
  vars_found = htab_create (50, htab_hash_pointer, htab_eq_pointer, NULL);

  if (flag_tree_points_to != PTA_NONE)
    {
      timevar_push (TV_TREE_PTA);
      create_alias_vars (fndecl);
      timevar_pop (TV_TREE_PTA);
    }

  walk_state.vars_found = vars_found;
  walk_state.is_store = 0;
  walk_state.is_indirect_ref = 0;

  /* Find all the variables referenced in the function.  */
  FOR_EACH_BB (bb)
    for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
      walk_tree (bsi_stmt_ptr (si), find_vars_r, &walk_state, NULL);

  htab_delete (vars_found);

  compute_alias_sets ();
  
  if (flag_tree_points_to != PTA_NONE)
    {
      timevar_push (TV_TREE_PTA);
      delete_alias_vars ();
      timevar_pop (TV_TREE_PTA);
    }

  /* Deallocate memory used by aliasing data structures.  */
  addressable_vars = NULL;
  pointers = NULL;

  timevar_pop (TV_TREE_MAY_ALIAS);
}


/* Compute alias sets.  Aliasing information is computed in two stages:

   1- Artificial variables called "memory tags" are created for each
      pointer used in the program.  Each memory tag (MT) represents the
      memory location pointed by its associated pointer.  Since pointers
      may point to each other, two or more pointers that may point to each
      other will be assigned the same memory tag.  These unique memory tags
      are computed by get_memory_tag_for and their associated pointers are
      added to the POINTERS array.

   2- All the addressable variables in ADDRESABLE_VARS are compared against
      the pointers collected in step 1.  If a pointer P may point to
      variable V, then V is added to the list of may-aliases for P.

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

   After aliasing analysis has finished, the memory tag for pointer 'p'
   will have two aliases, namely variables 'a' and 'b'.  Every time pointer
   'p' is dereferenced, we want to mark the operation as a potential
   reference to 'a' and 'b'.  This is marked with virtual operands.
   Resulting in the following renamed program:

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
	      a_9 = b_8 + 2;

	      # VUSE <a_9>;
	      # VUSE <b_8>;
	      return *p_1;
	    }

   This method allows the compiler to optimize aliased variables when
   they're use directly and prevent optimizations when they are being
   accessed via aliased pointers.

   In certain cases, the list of may aliases for a pointer may grow too
   large.  This may cause an explosion in the number of virtual operands
   inserted in the code.  Resulting in increased memory consumption and
   compilation time.

   When the set of may aliases for a pointer grows beyond 5 elements
   (FIXME, this is currently an arbitrary limit), instead of adding new
   variables to the may-alias set, the new variables are made to share the
   same alias set as the original pointer.  For instance, suppose that
   pointer 'p' may point to variables 'a', 'b', 'c', 'd', 'e', 'f' and 'g'.
   After alias analysis, the alias sets will be as follows:

	may-alias(p) = { a, b, c, d, e }
	may-alias(f) = { a, b, c, d, e }
	may-alias(g) = { a, b, c, d, e }

   Notice that this grouping causes variables 'f' and 'g' to be aliased to
   variables they can't possibly alias to.  */

static void
compute_alias_sets (void)
{
  size_t i;

  /* For every pointer P, determine which addressable variables may alias
     with P.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (pointers); i++)
    {
      size_t j;
      struct alias_map_d *ptr = VARRAY_GENERIC_PTR (pointers, i);
      tree mem = var_ann (ptr->var)->mem_tag;
      var_ann_t mem_ann = var_ann (mem);

      for (j = 0; j < VARRAY_ACTIVE_SIZE (addressable_vars); j++)
	{
	  struct alias_map_d *var = VARRAY_GENERIC_PTR (addressable_vars, j);
	  var_ann_t v_ann = var_ann (var->var);

	  /* Skip memory tags and variables that have never been written to.  */
	  if (!mem_ann->is_stored && !v_ann->is_stored)
	    continue;
	     
	  if (may_alias_p (ptr->var, ptr->set, var->var, var->set))
	    {
	      /* If MEM has less than 5 aliases in its alias set, add
		 VAR->VAR to the list of aliases for MEM.  Otherwise,
		 set the may-alias set for VAR->VAR to be the same alias
		 set as MEM.  This is to avoid the problem of having
		 large may-alias sets.  Large may-alias sets translate into
		 lots of virtual operands which can slow down the SSA pass
		 tremendously.  */
	      if (mem_ann->may_aliases
		  && VARRAY_ACTIVE_SIZE (mem_ann->may_aliases) >= 5)
		v_ann->may_aliases = mem_ann->may_aliases;
	      else
		add_may_alias (mem, var->var);
	    }
	}
    }

  /* If the function has calls to clobbering functions, make GLOBAL_VAR
     an alias for all call-clobbered variables.  */
  if (global_var)
    for (i = 0; i < num_call_clobbered_vars; i++)
      {
	tree var = call_clobbered_var (i);
	add_may_alias (var, global_var);
      }

  /* Debugging dumps.  */
  if (tree_ssa_dump_file && tree_ssa_dump_flags & TDF_ALIAS)
    {
      dump_alias_info (tree_ssa_dump_file);
      dump_referenced_vars (tree_ssa_dump_file);
    }
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

  mem = var_ann (ptr)->mem_tag;

  /* By convention, a variable cannot alias itself.  */
  if (mem == var)
    return false;

  v_ann = var_ann (var);
  m_ann = var_ann (mem);

#if defined ENABLE_CHECKING
  if (!m_ann->is_mem_tag)
    abort ();
#endif

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
	    return false;

	  /* If MEM doesn't alias a pointer to VAR and VAR doesn't alias
	     PTR, then PTR can't alias VAR.  */
	  if (!alias_sets_conflict_p (mem_alias_set, get_alias_set (ptr_to_var))
	      && !alias_sets_conflict_p (var_alias_set, get_alias_set (ptr)))
	    return false;
	}
      else
	return false;
    }

  /* If -ftree-points-to is given, check if PTR may point to VAR.  */
  if (flag_tree_points_to != PTA_NONE
      && !ptr_may_alias_var (ptr, var))
    return false;


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

  /* If either VAR or ALIAS may access global memory, then mark the other
     one as a global memory alias.  */
  if (may_access_global_mem_p (var))
    a_ann->may_alias_global_mem = 1;

  if (may_access_global_mem_p (alias))
    v_ann->may_alias_global_mem = 1;

  VARRAY_PUSH_TREE (v_ann->may_aliases, alias);
  a_ann->is_alias_tag = 1;
}


/* Dump alias information on FILE.  */

void
dump_alias_info (FILE *file)
{
  size_t i;
  const char *funcname
    = (*lang_hooks.decl_printable_name) (current_function_decl, 2);

  if (addressable_vars == NULL)
    return;

  fprintf (file, "\nAlias information for %s\n\n", funcname);
  fprintf (file, "%u addressable variables\n",
           (unsigned) VARRAY_ACTIVE_SIZE (addressable_vars));
  fprintf (file, "%u memory tags\n\n",
           (unsigned) VARRAY_ACTIVE_SIZE (pointers));

  for (i = 0; i < VARRAY_ACTIVE_SIZE (pointers); i++)
    {
      struct alias_map_d *map = VARRAY_GENERIC_PTR (pointers, i);
      tree mem = var_ann (map->var)->mem_tag;
      varray_type aliases = may_aliases (mem);
      if (aliases)
	{
	  fprintf (file, "Memory tag ");
	  print_generic_expr (file, mem, 0);
	  fprintf (file, " aliases ");
	  dump_may_aliases_for (file, mem);
	}
    }

  for (i = 0; i < VARRAY_ACTIVE_SIZE (addressable_vars); i++)
    {
      struct alias_map_d *map = VARRAY_GENERIC_PTR (addressable_vars, i);
      varray_type aliases = may_aliases (map->var);
      if (aliases)
	{
	  fprintf (file, "Addressable var ");
	  print_generic_expr (file, map->var, 0);
	  fprintf (file, " aliases ");
	  dump_may_aliases_for (file, map->var);
	}
    }

  fprintf (file, "\n");
}


/* Dump alias information on stderr.  */

void
debug_alias_info (void)
{
  dump_alias_info (stderr);
}



/*---------------------------------------------------------------------------
			     Miscellaneous helpers
---------------------------------------------------------------------------*/
/* Return TRUE if expression EXPR may reference memory outside the current
   function scope.  */

static bool
may_access_global_mem_p (tree expr)
{
  char class;

  if (expr == NULL_TREE)
    return false;

  /* Function arguments and global variables may reference global memory.  */
  if (DECL_P (expr)
      && (TREE_CODE (expr) == PARM_DECL
	  || decl_function_context (expr) == NULL_TREE))
    return true;

  /* If the expression is a variable that may point to or alias global memory,
     return true.  */
  if (SSA_VAR_P (expr))
    {
      var_ann_t ann = var_ann (expr);
      if (ann->may_point_to_global_mem || ann->may_alias_global_mem)
	return true;
    }

  /* Otherwise, the expression must be of pointer type.  */
  if (TREE_TYPE (expr) == NULL_TREE
      || !POINTER_TYPE_P (TREE_TYPE (expr)))
    return false;

  /* Call expressions that return pointers may point to global memory.  */
  if (TREE_CODE (expr) == CALL_EXPR)
    return true;

  /* Recursively check the expression's operands.  */
  class = TREE_CODE_CLASS (TREE_CODE (expr));
  if (IS_EXPR_CODE_CLASS (class) || class == 'r')
    {
      unsigned char i;

      for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (expr)); i++)
	if (may_access_global_mem_p (TREE_OPERAND (expr, i)))
	  return true;
    }

  return false;
}


/* Remove variable DECL from the block that declares it.  */

void
remove_decl (tree decl, tree block)
{
  tree *loc;
  
  loc = find_decl_location (decl, block);
  if (loc)
    *loc = TREE_CHAIN (decl);
}


/* Find the location for declaration DECL in lexical block BLOCK.  All the
   subblocks of BLOCK are searched as well if BLOCK does not declare DECL.
   Return an address LOC such that *LOC == DECL or NULL if DECL couldn't be
   located.  */

tree *
find_decl_location (tree decl, tree block)
{
  tree d, sub;

  /* Special case.  If DECL is the first symbol in the block, return its
     location directly.  */
  if (BLOCK_VARS (block) == decl)
    return &(BLOCK_VARS (block));

  for (d = BLOCK_VARS (block); d; d = TREE_CHAIN (d))
    if (TREE_CHAIN (d) == decl)
      return &(TREE_CHAIN (d));

  for (sub = BLOCK_SUBBLOCKS (block); sub; sub = TREE_CHAIN (sub))
    {
      tree *loc = find_decl_location (decl, sub);
      if (loc)
	return loc;
    }

  return NULL;
}


/* Callback for walk_tree.  Used to collect variables referenced in
   the function.  */

static tree
find_vars_r (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED,
	     void *data)
{
  tree t = *tp;
  struct walk_state *walk_state = (struct walk_state *)data;
  int saved_is_store = walk_state->is_store;

  /* Type and constant nodes have no interesting children.  Ignore them.  */
  if (TYPE_P (t) || TREE_CODE_CLASS (TREE_CODE (t)) == 'c')
    {
      *walk_subtrees = 0;
      return NULL_TREE;
    }

  /* DECL nodes have no interesting children.  */
  if (DECL_P (t))
    {
      *walk_subtrees = 0;

      /* If this _DECL node is not interesting to the SSA builder,
         then we can just return now.  */
      if (! SSA_VAR_P (t)) 
	return NULL_TREE;
    }

  if (TREE_CODE (t) == MODIFY_EXPR)
    {
      walk_state->is_store = 1;
      walk_tree (&TREE_OPERAND (t, 0), find_vars_r, data, NULL);
      walk_state->is_store = 0;
      walk_tree (&TREE_OPERAND (t, 1), find_vars_r, data, NULL);
      walk_state->is_store = saved_is_store;

      /* If this is an assignment to a pointer and the RHS may point to
	 global memory, mark the pointer on the LHS.  FIXME: This causes a
	 second traversal of the RHS of the assignment, we should set a bit
	 in WALK_STATE as we walk the RHS.  */
      if (SSA_VAR_P (TREE_OPERAND (t, 0))
	  && POINTER_TYPE_P (TREE_TYPE (TREE_OPERAND (t, 0)))
	  && may_access_global_mem_p (TREE_OPERAND (t, 1)))
	set_may_point_to_global_mem (TREE_OPERAND (t, 0));

      return t;
    }
  else if (TREE_CODE (t) == ASM_EXPR)
    {
      walk_state->is_store = 1;
      walk_tree (&ASM_OUTPUTS (t), find_vars_r, data, NULL);
      walk_tree (&ASM_CLOBBERS (t), find_vars_r, data, NULL);
      walk_state->is_store = 0;
      walk_tree (&ASM_INPUTS (t), find_vars_r, data, NULL);
      walk_state->is_store = saved_is_store;
      return t;
    }
  else if (TREE_CODE (t) == INDIRECT_REF)
    {
      walk_state->is_indirect_ref = 1;
      walk_tree (&TREE_OPERAND (t, 0), find_vars_r, data, NULL);
      /* INDIRECT_REF nodes cannot be nested in GIMPLE, so there is no need
	 of saving/restoring the state.  */
      walk_state->is_indirect_ref = 0;
      return NULL_TREE;
    }

  if (SSA_VAR_P (*tp))
    {
      add_referenced_var (*tp, walk_state);
      return NULL_TREE;
    }

  /* A function call that receives pointer arguments may dereference them.
     For every pointer 'p' in the argument to the function call, add a
     reference to '*p'.  */
  if (TREE_CODE (*tp) == CALL_EXPR)
    {
      tree op;
      bool may_clobber = call_may_clobber (*tp);

      for (op = TREE_OPERAND (*tp, 1); op; op = TREE_CHAIN (op))
	{
	  tree arg = TREE_VALUE (op);
	  if (SSA_VAR_P (arg) && POINTER_TYPE_P (TREE_TYPE (arg)))
	    {
	      walk_state->is_indirect_ref = 1;
	      add_referenced_var (arg, walk_state);
	      walk_state->is_indirect_ref = 0;
	    }
	}

      /* If the function may clobber globals and addressable locals, add a
	 reference to GLOBAL_VAR.  */
      if (may_clobber)
	{
	  walk_state->is_store = 1;
	  if (global_var == NULL_TREE)
	    create_global_var ();
	  add_referenced_var (global_var, walk_state);
	}

      walk_state->is_store = saved_is_store;
    }

  return NULL_TREE;
}


/* Add VAR to the list of dereferenced variables.  If VAR is a candidate
   for aliasing, add it to the ADDRESSABLE_VAR array.  If VAR is a memory
   tag, add it to the POINTERS array.  These two arrays are used for
   alias analysis (See compute_alias_sets).

   WALK_STATE is an array with a hash table used to avoid adding the
   same variable more than once to its corresponding set as well as flags
   indicating if we're processing a load or store.  Note that this function
   assumes that VAR is a valid SSA variable.  */

static void
add_referenced_var (tree var, struct walk_state *walk_state)
{
  void **slot;
  htab_t vars_found = walk_state->vars_found;
  var_ann_t v_ann;

  v_ann = get_var_ann (var);

  slot = htab_find_slot (vars_found, (void *) var, INSERT);
  if (*slot == NULL)
    {
      bool is_addressable;

      /* This is the first time we find this variable, add it to the
         REFERENCED_VARS array and annotate it with attributes that are
	 intrinsic to the variable.  */
      *slot = (void *) var;
      v_ann->uid = num_referenced_vars;
      VARRAY_PUSH_TREE (referenced_vars, var);

      /* Arguments or global variable pointers may point to memory outside
	 the current function.  */
      if (POINTER_TYPE_P (TREE_TYPE (var))
	  && (TREE_CODE (var) == PARM_DECL
	      || decl_function_context (var) == NULL_TREE))
	v_ann->may_point_to_global_mem = 1;

      /* By default, assume that the variable has no real references.  If
	 the variable is used as a real operand to a statement (i.e.,
	 add_use and set_def), this field will be set to 1.  */
      v_ann->has_real_refs = 0;

      is_addressable = TREE_ADDRESSABLE (var)
		       || decl_function_context (var) == NULL;

      /* Global variables and addressable locals may be aliased.  Create an
	 entry in ADDRESSABLE_VARS for VAR.  */
      if (is_addressable)
	{
	  /* Create a new alias set entry for VAR.  */
	  struct alias_map_d *alias_map;
	  alias_map = ggc_alloc (sizeof (*alias_map));
	  alias_map->var = var;
	  alias_map->set = get_alias_set (var);
	  VARRAY_PUSH_GENERIC_PTR (addressable_vars, (void *) alias_map);
	}

      /* If the variable is a writeable memory tag, a global or an
	 addressable local, it may be clobbered by function calls.  */
      if (!TREE_READONLY (var)
	  && (is_addressable || v_ann->is_mem_tag))
	{
	  add_call_clobbered_var (var);
	  v_ann->is_call_clobbered = 1;
	}
    }

  /* Now, set attributes that depend on WALK_STATE.  */

  /* Remember if the variable has been written to.  This is important for
     alias analysis.  If a variable and its aliases are never modified, it
     is not interesting for the optimizers because there are no aliased
     stores to keep track of.  */
  if (walk_state->is_store)
    v_ann->is_stored = 1;

  /* If VAR is a pointer referenced in an INDIRECT_REF node, create (or
     re-use) a memory tag to represent the location pointed-to by VAR.  */
  if (walk_state->is_indirect_ref)
    {
      /* If pointer VAR still doesn't have a memory tag associated with it,
	 create it now or re-use an existing one.  A memory tag for some
	 other pointer P will be reused if P and VAR may point to each
	 other.  */
      tree tag = v_ann->mem_tag;
      if (tag == NULL_TREE)
	tag = get_memory_tag_for (var);

      /* Associate the tag with pointer VAR.  */
      v_ann->mem_tag = tag;

      /* Add the memory tag to the list of referenced variables.  Note that
	 this needs to be done every time because there are attributes for
	 the memory tag that depend on WALK_STATE (e.g., whether this
	 variable is being stored-to).  */
      walk_state->is_indirect_ref = 0;
      add_referenced_var (tag, walk_state);

      /* If pointer VAR may point to global mem, then TAG may alias
	 global memory.  */
      if (v_ann->may_point_to_global_mem)
	var_ann (tag)->may_alias_global_mem = 1;
    }
}


/* Return the memory tag associated to pointer P.  */

static tree
get_memory_tag_for (tree ptr)
{
  size_t i;
  tree tag;
  tree tag_type = TREE_TYPE (TREE_TYPE (ptr));
  HOST_WIDE_INT tag_set = get_alias_set (tag_type);

  /* See if PTR may alias any of the existing pointers.  Note that we can't
     use may_alias_p here because we have not created a memory tag for PTR
     yet.  */
  for (i = 0, tag = NULL_TREE; i < VARRAY_ACTIVE_SIZE (pointers); i++)
    {
      struct alias_map_d *curr = VARRAY_GENERIC_PTR (pointers, i);
      if (alias_sets_conflict_p (curr->set, tag_set)
	  && (flag_tree_points_to == PTA_NONE
	      || ptr_may_alias_var (ptr, curr->var)))
	{
	  tag = var_ann (curr->var)->mem_tag;
	  break;
	}
    }

  /* If VAR cannot alias with any of the existing memory tags, create a new
     tag for PTR and add it to the POINTERS array.  */
  if (tag == NULL_TREE)
    {
      struct alias_map_d *alias_map;
      var_ann_t tag_ann;

      /* Create a new MT.* artificial variable representing the memory
	 location pointed-to by PTR.  */
      tag = create_tmp_alias_var (tag_type, "MT");
      tag_ann = get_var_ann (tag);
      tag_ann->is_mem_tag = 1;
      tag_ann->mem_tag = NULL_TREE;

      /* Add PTR to the POINTERS array.  Note that we are not interested in
	 PTR's alias set.  Instead, we cache the alias set for the memory that
	 PTR points to.  */
      alias_map = ggc_alloc (sizeof (*alias_map));
      alias_map->var = ptr;
      alias_map->set = tag_set;
      VARRAY_PUSH_GENERIC_PTR (pointers, (void *) alias_map);
    }

  return tag;
}


/* Return the virtual variable associated to the non-scalar variable VAR.  */

tree
get_virtual_var (tree var)
{
  enum tree_code code;

  STRIP_NOPS (var);

  if (TREE_CODE (var) == SSA_NAME)
    var = SSA_NAME_VAR (var);

  code = TREE_CODE (var);

  while (code == ARRAY_REF
         || code == COMPONENT_REF
	 || code == REALPART_EXPR
	 || code == IMAGPART_EXPR)
    {
      var = TREE_OPERAND (var, 0);
      code = TREE_CODE (var);
    }

  return var;
}


/* Return true if EXPR is a CALL_EXPR tree to a function that may clobber
   globals and local addressable variables.  */

static bool
call_may_clobber (tree expr)
{
  tree callee;
  int flags;

  if (TREE_CODE (expr) != CALL_EXPR)
    return false;

  callee = get_callee_fndecl (expr);
  flags = (callee) ? flags_from_decl_or_type (callee) : 0;
  return (! (flags & (ECF_CONST | ECF_PURE | ECF_NORETURN)));
}


/* Mark variables that have hidden uses.

   A hidden use can occur due to VLA declarations or nested functions.   */

static void
find_hidden_use_vars (tree block)
{
  tree sub, decl;

  /* Check all the arrays declared in the block for VLAs.

     While scanning the block's variables, also see if there is
     a nested function at this scope.  */
  for (decl = BLOCK_VARS (block); decl; decl = TREE_CHAIN (decl))
    {
      int inside_vla = 0;

      /* The front-ends nicely set DECL_NONLOCAL for us to mark 
	 variables which are referenced inside nested functions.  */
      if ((TREE_CODE (decl) == VAR_DECL || TREE_CODE (decl) == PARM_DECL)
	  &&  DECL_NONLOCAL (decl))
	set_has_hidden_use (decl);

      walk_tree (&decl, find_hidden_use_vars_r, &inside_vla, NULL);
    }

  /* Now repeat the search in any sub-blocks.  */
  for (sub = BLOCK_SUBBLOCKS (block); sub; sub = TREE_CHAIN (sub))
    find_hidden_use_vars (sub);
}

/* Callback for walk_tree used by find_hidden_use_vars to analyze each 
   variable in a lexical block.  If the variable's size has a variable
   size, then mark all objects needed to compute the variable's size
   as having hidden uses.  */

static tree
find_hidden_use_vars_r (tree *tp, int *walk_subtrees ATTRIBUTE_UNUSED,
			void *data ATTRIBUTE_UNUSED)
{
  int *inside_vla = (int *) data;

  /* We need to look for hidden uses due to VLAs in variable
     definitions.  We originally used to look for these hidden
     uses in the variable's type, but that's unreliable if the
     type's size contains a SAVE_EXPR for a different function
     context than the variable is used within.  */
  if (SSA_VAR_P (*tp)
      && ((DECL_SIZE (*tp)
	   && ! really_constant_p (DECL_SIZE (*tp)))
	  || (DECL_SIZE_UNIT (*tp)
	      && ! really_constant_p (DECL_SIZE_UNIT (*tp)))))
    {
      int save = *inside_vla;

      *inside_vla = 1;
      walk_tree (&DECL_SIZE (*tp), find_hidden_use_vars_r, inside_vla, NULL);
      walk_tree (&DECL_SIZE_UNIT (*tp), find_hidden_use_vars_r,
		 inside_vla, NULL);
      *inside_vla = save;
    }
  else if (*inside_vla && SSA_VAR_P (*tp))
    set_has_hidden_use (*tp);

  return NULL_TREE;
}


/* Create GLOBAL_VAR, an artificial global variable to act as a
   representative of all the variables that may be clobbered by function
   calls.  Also create GLOBAL_CLOBBER_EXPR, an artificial expression that
   is used as the originating definition of all clobbered SSA variables in
   the program.  */

void
create_global_var (void)
{
  global_var = build_decl (VAR_DECL, get_identifier (".GLOBAL_VAR"),
                           size_type_node);
  DECL_ARTIFICIAL (global_var) = 1;
  TREE_READONLY (global_var) = 1;
  DECL_EXTERNAL (global_var) = 0;
  TREE_STATIC (global_var) = 1;
  TREE_USED (global_var) = 1;
  DECL_CONTEXT (global_var) = current_function_decl;
  TREE_THIS_VOLATILE (global_var) = 1;
  TREE_ADDRESSABLE (global_var) = 0;
}
