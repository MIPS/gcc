/* Data flow functions for trees.
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
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
#include "flags.h"
#include "function.h"
#include "diagnostic.h"
#include "tree-simple.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "tree-alias-common.h"
#include "convert.h"

/* Build and maintain data flow information for trees.  */

/* Local declarations.  */
static tree *alias_tags;
static unsigned long num_alias_tags;

/* Counters used to display DFA and SSA statistics.  */
struct dfa_stats_d
{
  long num_tree_refs;
  long size_tree_refs;
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

/* Data and functions shared with tree-ssa.c.  */
struct dfa_stats_d dfa_stats;
extern FILE *tree_ssa_dump_file;
extern int tree_ssa_dump_flags;


/* Local functions.  */
static void get_expr_operands		PARAMS ((tree, tree *, int,
      						 voperands_t));
static void collect_dfa_stats		PARAMS ((struct dfa_stats_d *));
static tree collect_dfa_stats_r		PARAMS ((tree *, int *, void *));
static tree clobber_vars_r		PARAMS ((tree *, int *, void *));
static void find_may_aliases_for	PARAMS ((int));
static tree find_alias_tag		PARAMS ((tree, tree));
static bool may_access_global_mem 	PARAMS ((tree));
static void set_def			PARAMS ((tree *, tree));
static void add_use			PARAMS ((tree *, tree));
static void add_vdef			PARAMS ((tree, tree, voperands_t));
static void add_vuse			PARAMS ((tree, tree, voperands_t));
static void add_stmt_operand		PARAMS ((tree *, tree, int, int,
      						 voperands_t));
static void add_immediate_use		PARAMS ((tree, tree));
static tree find_vars_r			PARAMS ((tree *, int *, void *));
static void compute_immediate_uses_for	PARAMS ((tree, int));
static void add_may_alias		PARAMS ((tree, tree));


/* Global declarations.  */

/* The total number of referenced variables in the function.  */
unsigned long num_referenced_vars;

/* Array of all variables referenced in the function.  */
varray_type referenced_vars;

/* The total number of unique INDIRECT_REFs in the function.  */
static unsigned long num_indirect_refs;

/* Array of all the unique INDIRECT_REFs in the function.  */
static varray_type indirect_refs;

/* The total number of unique addressable vars in the function.  */
static unsigned long num_addressable_vars;

/* Array of all the unique addressable vars in the function.  */
static varray_type addressable_vars;

/* Artificial variable used to model the effects of function calls on every
   variable that they may use and define.  Calls to non-const and non-pure
   functions are assumed to use and clobber this variable.  The SSA builder
   will then consider this variable to be an alias for every global
   variable and every local that has had its address taken.  */
tree global_var;

/* Get the operands of statement STMT.  Note that repeated calls to
   get_stmt_operands for the same statement will do nothing until the
   statement is marked modified by a call to modify_stmt().  */

void
get_stmt_operands (stmt)
     tree stmt;
{
  enum tree_code code;
  stmt_ann_t ann;
  voperands_t prev_vops = NULL;

  if (stmt == error_mark_node || stmt == empty_stmt_node)
    return;

  STRIP_NOPS (stmt);

  /* If the statement has not been modified, the operands are still valid.  */
  if (!stmt_modified_p (stmt))
    return;

  ann = stmt_ann (stmt);
  if (ann == NULL)
    ann = create_stmt_ann (stmt);

  /* Remove any existing operands as they will be scanned again.  */
  ann->ops = NULL;

  /* We cannot remove existing virtual operands because we would lose their
     SSA versions.  Instead, we save them on PREV_VOPS.  When add_vdef and
     add_vuse are called, they will search for the operand in PREV_VOPS
     first.  */
  if (ann->vops)
    prev_vops = ann->vops;
  ann->vops = NULL;

  code = TREE_CODE (stmt);
  switch (code)
    {
    case INIT_EXPR:
    case MODIFY_EXPR:
      get_expr_operands (stmt, &TREE_OPERAND (stmt, 1), false, prev_vops);
      get_expr_operands (stmt, &TREE_OPERAND (stmt, 0), true, prev_vops);
      break;

    case COND_EXPR:
      get_expr_operands (stmt, &COND_EXPR_COND (stmt), false, prev_vops);
      break;

    case SWITCH_EXPR:
      get_expr_operands (stmt, &SWITCH_COND (stmt), false, prev_vops);
      break;

    case ASM_EXPR:
      get_expr_operands (stmt, &ASM_INPUTS (stmt), false, prev_vops);
      get_expr_operands (stmt, &ASM_OUTPUTS (stmt), true, prev_vops);
      get_expr_operands (stmt, &ASM_CLOBBERS (stmt), true, prev_vops);
      break;

    case RETURN_EXPR:
      get_expr_operands (stmt, &TREE_OPERAND (stmt, 0), false, prev_vops);
      break;

    case GOTO_EXPR:
      get_expr_operands (stmt, &GOTO_DESTINATION (stmt), false, prev_vops);
      break;

    case LABEL_EXPR:
      get_expr_operands (stmt, &LABEL_EXPR_LABEL (stmt), false, prev_vops);
      break;

      /* These nodes contain no variable references.  */
    case LOOP_EXPR:
    case BIND_EXPR:
    case CASE_LABEL_EXPR:
      break;

    default:
      /* Notice that if get_expr_operands tries to use &STMT as the operand
	 pointer (which may only happen for USE operands), we will abort in
	 add_use.  This default will handle statements like CALL_EXPRs or
	 VA_ARG_EXPRs that may appear on the RHS of a statement or as
	 statements themselves.  */
      get_expr_operands (stmt, &stmt, false, prev_vops);
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
   IS_DEF is nonzero if the expression is expected to define the
   referenced variables.  PREV_VOPS is as in add_vdef and add_vuse.  */

static void
get_expr_operands (stmt, expr_p, is_def, prev_vops)
     tree stmt;
     tree *expr_p;
     int is_def;
     voperands_t prev_vops;
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
      || code == ADDR_EXPR
      || code == RESULT_DECL
      || code == FUNCTION_DECL
      || code == LABEL_DECL)
    return;

  /* If this reference is associated with a non SIMPLE expression, then we
     mark the statement non GIMPLE and recursively clobber every
     variable referenced by STMT.  FIXME: TREE_NOT_GIMPLE must die.  */
  if (stmt && TREE_NOT_GIMPLE (expr))
    {
      struct clobber_data_d cd;
      mark_not_simple (&stmt);
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
     IS_DEF flag.  */
  if (SSA_VAR_P (expr))
    {
      add_stmt_operand (expr_p, stmt, is_def, false, prev_vops);

      /* Pointer dereferences represent a VUSE of their base pointer.  */
      if (TREE_CODE (expr) == INDIRECT_REF)
	add_stmt_operand (&TREE_OPERAND (expr, 0), stmt, false, true,
			  prev_vops);

      return;
    }

  /* Treat array references as references to the virtual variable
     representing the array.  The virtual variable for an ARRAY_REF is the
     VAR_DECL for the array.  */
  if (code == ARRAY_REF)
    {
      /* Add the virtual variable for the ARRAY_REF to VDEFS or VUSES
	 according to the value of IS_DEF.  */
      add_stmt_operand (expr_p, stmt, is_def, false, prev_vops);
      
      /* Look for variable uses in every dimension of the array.  */
      do
	{
	  add_stmt_operand (&TREE_OPERAND (expr, 1), stmt, false, false,
			    prev_vops);

	  expr = (TREE_CODE (expr) == ARRAY_REF)
		 ? TREE_OPERAND (expr, 0)
	         : NULL_TREE;
	}
      while (expr && TREE_CODE (expr) == ARRAY_REF);

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
      if (SSA_VAR_P (TREE_OPERAND (expr, 0)))
	add_stmt_operand (expr_p, stmt, is_def, false, prev_vops);
      else
	get_expr_operands (stmt, &TREE_OPERAND (expr, 0), is_def, prev_vops);

      return;
    }

  /* Function calls.  Add every argument to USES.  If the callee is
     neither pure nor const, create a use and clobbering definition of
     *GLOBAL_VAR (See find_vars_r).  */
  if (code == CALL_EXPR)
    {
      tree callee;
      int flags;

      /* Find uses in the called function.  */
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), false, prev_vops);

      /* If the called function is neither pure nor const, we create a
	  clobbering definition of *GLOBAL_VAR.  */
      callee = get_callee_fndecl (expr);
      flags = (callee) ? flags_from_decl_or_type (callee) : 0;
      if (! (flags & (ECF_CONST | ECF_PURE)))
	{
	  tree v = build1 (INDIRECT_REF, TREE_TYPE (TREE_TYPE (global_var)),
		      	   global_var);
	  add_stmt_operand (&v, stmt, true, true, prev_vops);
	}

      /* Find uses for all the arguments to the function.  */
      get_expr_operands (stmt, &TREE_OPERAND (expr, 1), 0, prev_vops);

      return;
    }

  /* Lists.  */
  if (code == TREE_LIST)
    {
      tree op;

      for (op = expr; op; op = TREE_CHAIN (op))
	add_stmt_operand (&TREE_VALUE (op), stmt, is_def, false, prev_vops);

      return;
    }

  /* Assignments.  */
  if (code == INIT_EXPR || code == MODIFY_EXPR)
    {
      get_expr_operands (stmt, &TREE_OPERAND (expr, 1), false, prev_vops);
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), true, prev_vops);
      return;
    }

  /* VA_ARG_EXPR nodes read and modify the argument pointer.  Add it to
     VOPS to avoid optimizations messing it up.  */
  if (code == VA_ARG_EXPR)
    {
      add_stmt_operand (&TREE_OPERAND (expr, 0), stmt, true, true, prev_vops);
      return;
    }

  /* Unary expressions.  */
  if (class == '1'
      || code == BIT_FIELD_REF)
    {
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), is_def, prev_vops);
      return;
    }

  /* Binary expressions.  */
  if (class == '2'
      || class == '<'
      || code == TRUTH_AND_EXPR
      || code == TRUTH_OR_EXPR
      || code == TRUTH_XOR_EXPR
      || code == COMPOUND_EXPR
      || code == CONSTRUCTOR)
    {
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), is_def, prev_vops);
      get_expr_operands (stmt, &TREE_OPERAND (expr, 1), is_def, prev_vops);
      return;
    }

  /* If we get here, something has gone wrong.  */
  fprintf (stderr, "unhandled expression in get_expr_operands():\n");
  debug_tree (expr);
  fputs ("\n", stderr);
  abort ();
}


/* Add *VAR_P to the appropriate operand array of STMT.  IS_DEF is nonzero
   if *VAR_P is being defined.  The following are the rules used to decide
   whether an operand belongs in OPS or VOPS:

   1- Non-aliased scalar and pointer variables are real operands.

   2- If a variable is aliased, all its aliases are added to the virtual
      operands.

   3- For non-scalar variables (arrays, structures, unions and complex
      types), their virtual variable (see get_virtual_var) is added to the
      virtual operands.

   The caller may force a variable to be added as a virtual operand by
   setting the FORCE_VOP flag.

   PREV_VOPS is used when adding virtual operands to statements that
      already had them (See add_vdef and add_vuse).  */

static void
add_stmt_operand (var_p, stmt, is_def, force_vop, prev_vops)
     tree *var_p;
     tree stmt;
     int is_def;
     int force_vop;
     voperands_t prev_vops;
{
  bool is_scalar;
  tree var;

  var = *var_p;
  STRIP_NOPS (var);

  /* If the original variable is not a scalar, it will be added to the list
     of virtual operands.  In that case, use its base symbol as the virtual
     variable representing it.  */
  is_scalar = SSA_VAR_P (var);
  if (!is_scalar)
    var = get_virtual_var (var);

  /* If VAR is not a variable, do nothing.  */
  if (var == NULL_TREE || !SSA_VAR_P (var))
    return;

  if (!is_aliased (var))
    {
      /* The variable is not aliased.  If it's a scalar, process it as a
	 real operand.  Otherwise, add it to the virtual operands.  Note
	 that we never consider ASM_EXPR operands as real.  They are always
	 added to virtual operands so that optimizations don't try to
	 optimize them.

	 FIXME: This is true for CCP.  It tries to propagate constants in
		some __asm__ operands causing ICEs during RTL expansion
		(execute/20020107-1.c).  Do we really need to be this
		drastic?  Or should each optimization take care when
		dealing with ASM_EXPRs?  */
      if (is_def)
	{
	  if (is_scalar && !force_vop && TREE_CODE (stmt) == MODIFY_EXPR)
	    set_def (var_p, stmt);
	  else
	    add_vdef (var, stmt, prev_vops);
	}
      else
	{
	  if (is_scalar && !force_vop && TREE_CODE (stmt) != ASM_EXPR)
	    add_use (var_p, stmt);
	  else
	    add_vuse (var, stmt, prev_vops);
	}
    }
  else
    {
      /* The variable is aliased.  Add its aliases to the virtual operands.  */
      size_t i;
      varray_type aliases = may_aliases (var);

      if (is_def)
	{
	  for (i = 0; i < VARRAY_ACTIVE_SIZE (aliases); i++)
	    add_vdef (VARRAY_TREE (aliases, i), stmt, prev_vops);
	}
      else
	{
	  for (i = 0; i < VARRAY_ACTIVE_SIZE (aliases); i++)
	    add_vuse (VARRAY_TREE (aliases, i), stmt, prev_vops);
	}
    }


  /* A definition of a pointer variable 'p' clobbers its associated
     indirect variable '*p', because now 'p' is pointing to a different
     memory location.  */
  if (is_def
      && SSA_DECL_P (var)
      && POINTER_TYPE_P (TREE_TYPE (var))
      && is_dereferenced (var))
    {
      tree deref = indirect_ref (var);
      add_stmt_operand (&deref, stmt, true, true, prev_vops);

      /* If the relocation of 'p' is due to an expression that may
	 point to global memory, then mark '*p' as an alias for
	 global memory.  */
      if (TREE_CODE (stmt) == MODIFY_EXPR
	  && may_access_global_mem (TREE_OPERAND (stmt, 1)))
	set_may_alias_global_mem (deref);
    }
}


/* Set DEF_P to be the pointer to the operand defined by STMT.  */

static void
set_def (def_p, stmt)
     tree *def_p;
     tree stmt;
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
}


/* Add USE_P to the list of pointers to operands used by STMT.  */

static void
add_use (use_p, stmt)
     tree *use_p;
     tree stmt;
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
}


/* Add a new VDEF_EXPR for variable VAR to statement STMT.  If PREV_VOPS
   is not NULL, it will be searched for an existing VDEF_EXPR to VAR.  If
   found, the existing VDEF_EXPR will be added to STMT.  This is done to
   preserve the SSA numbering of virtual operands.  */

static void
add_vdef (var, stmt, prev_vops)
     tree var;
     tree stmt;
     voperands_t prev_vops;
{
  tree vdef;
  stmt_ann_t ann;
  size_t i;

  if (prev_vops && prev_vops->vdef_ops)
    {
      vdef = NULL;
      for (i = 0; i < VARRAY_ACTIVE_SIZE (prev_vops->vdef_ops); i++)
	{
	  tree d = VARRAY_TREE (prev_vops->vdef_ops, i);
	  if (same_var_p (var, SSA_NAME_DECL (VDEF_RESULT (d))))
	    {
	      vdef = d;
	      break;
	    }
	}

      /* FIXME.  If we didn't find an existing VDEF for VAR, it means that
	 we would have to rewrite this statement into SSA form again.
	 Which means that we would have to update other statements, insert
	 PHI nodes, etc.  We don't handle this situation.  */
      if (vdef == NULL)
	abort ();
    }
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
    if (same_var_p (var, VDEF_RESULT (VARRAY_TREE (ann->vops->vdef_ops, i))))
      return;

  VARRAY_PUSH_TREE (ann->vops->vdef_ops, vdef);
}


/* Add VAR to the list of virtual uses for STMT.  If PREV_VOPS is not NULL,
   it will be searched for an existing VUSE of VAR.  If found, the
   existing VUSE will be added to STMT.  This is done to preserve the
   SSA numbering of virtual operands.  */

static void
add_vuse (var, stmt, prev_vops)
     tree var;
     tree stmt;
     voperands_t prev_vops;
{
  stmt_ann_t ann;
  size_t i;

  if (prev_vops && prev_vops->vuse_ops)
    {
      tree vuse = NULL_TREE;
      for (i = 0; i < VARRAY_ACTIVE_SIZE (prev_vops->vuse_ops); i++)
	{
	  tree u = VARRAY_TREE (prev_vops->vuse_ops, i);
	  if (same_var_p (var, SSA_NAME_DECL (u)))
	    {
	      vuse = u;
	      break;
	    }
	}

      if (vuse)
	var = vuse;
      else
	{
	  /* FIXME.  If we didn't find an existing VUSE of VAR, it means
	     that we would have to rewrite this statement into SSA form
	     again.  Which means that we would have to find its current
	     reaching definition.  We don't handle this situation.  */
	  abort ();
	}
    }

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
    if (same_var_p (var, VARRAY_TREE (ann->vops->vuse_ops, i)))
      return;

  VARRAY_PUSH_TREE (ann->vops->vuse_ops, var);
}


/* Create a new PHI node for variable VAR at basic block BB.  */

tree
create_phi_node (var, bb)
     tree var;
     basic_block bb;
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
    definition and E is the edge through which DEF reaches PHI.  */

void
add_phi_arg (phi, def, e)
     tree phi;
     tree def;
     edge e;
{
  int i = PHI_NUM_ARGS (phi);

#if defined ENABLE_CHECKING
  if (i >= PHI_ARG_CAPACITY (phi))
    abort ();
#endif
  PHI_ARG_DEF (phi, i) = def;
  PHI_ARG_EDGE (phi, i) = e;
  PHI_NUM_ARGS (phi)++;
}


/*---------------------------------------------------------------------------
			     Code replacement
---------------------------------------------------------------------------*/
/* Return a copy of statement ORIG.  Note that the original statement
   annotations are not copied.  */

tree
copy_stmt (orig)
     tree orig;
{
  tree copy;

  STRIP_NOPS (orig);
  copy = orig;
  walk_tree (&copy, copy_tree_r, NULL, NULL);
  copy->common.ann = NULL;

  return copy;
}


/*---------------------------------------------------------------------------
			Dataflow analysis (DFA) routines
---------------------------------------------------------------------------*/
/* Compute immediate uses.  */

void
compute_immediate_uses (flags)
     int flags;
{
  basic_block bb;
  gimple_stmt_iterator si;

  FOR_EACH_BB (bb)
    {
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	compute_immediate_uses_for (phi, flags);

      for (si = gsi_start_bb (bb); !gsi_end_bb_p (si); gsi_step_bb (&si))
	compute_immediate_uses_for (gsi_stmt (si), flags);
    }
}

/* Helper for compute_immediate_uses.  Check all the USE and/or VUSE
   operands in STMT and add a def-use edge between their defining statement
   and STMT.  */

static void
compute_immediate_uses_for (stmt, flags)
     tree stmt;
     int flags;
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
	  if (imm_rdef_stmt != empty_stmt_node)
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
	  if (imm_rdef_stmt != empty_stmt_node)
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
	  if (imm_rdef_stmt != empty_stmt_node)
	    add_immediate_use (imm_rdef_stmt, stmt);
	}
    }
}


/* Compute reached uses.  */

void
compute_reached_uses (flags)
     int flags ATTRIBUTE_UNUSED;
{
  abort ();
}


/* Compute reaching definitions.  */

void
compute_reaching_defs (flags)
    int flags ATTRIBUTE_UNUSED;
{
  abort ();
}



/* Add statement USE_STMT to the list of statements that use definitions
    made by STMT.  */

static void
add_immediate_use (stmt, use_stmt)
     tree stmt;
     tree use_stmt;
{
  stmt_ann_t ann = stmt_ann (stmt);

  if (ann == NULL)
    ann = create_stmt_ann (stmt);

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
create_var_ann (t)
     tree t;
{
  var_ann_t ann;

#if defined ENABLE_CHECKING
  if (t == NULL_TREE || !SSA_VAR_P (t))
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
create_stmt_ann (t)
     tree t;
{
  stmt_ann_t ann;

#if defined ENABLE_CHECKING
  if (t == empty_stmt_node
      || t == NULL_TREE
      || TREE_CODE_CLASS (TREE_CODE (t)) == 'c'
      || TREE_CODE_CLASS (TREE_CODE (t)) == 't')
    abort ();
#endif

  ann = ggc_alloc (sizeof (*ann));
  memset ((void *) ann, 0, sizeof (*ann));

  ann->common.type = STMT_ANN;

  /* Since we just created the annotation, mark the statement modified.  */
  ann->modified = true;

  STRIP_NOPS (t);
  t->common.ann = (tree_ann) ann;

  return ann;
}


/*---------------------------------------------------------------------------
			      Debugging functions
---------------------------------------------------------------------------*/
/* Dump the list of all the referenced variables in the current function to
   FILE.  */

void
dump_referenced_vars (file)
     FILE *file;
{
  unsigned long i;

  fprintf (file, "\nReferenced variables in %s: %lu\n\n", 
	   get_name (current_function_decl), num_referenced_vars);

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
debug_referenced_vars ()
{
  dump_referenced_vars (stderr);
}


/* Dump variable VAR and its may-aliases to FILE.  */

void
dump_variable (file, var)
     FILE *file;
     tree var;
{
  if (var == NULL_TREE)
    {
      fprintf (file, "<nil>");
      return;
    }

  print_generic_expr (file, var, 0);
  
  if (is_aliased (var))
    {
      varray_type aliases = may_aliases (var);
      size_t i, num_aliases = VARRAY_ACTIVE_SIZE (aliases);
      fprintf (file, ", may aliases: ");
      for (i = 0; i < num_aliases; i++)
	{
	  print_generic_expr (file, VARRAY_TREE (aliases, i), 0);
	  if (i < num_aliases - 1)
	    fprintf (file, ", ");
	}
    }

  if (may_alias_global_mem_p (var))
    fprintf (file, ", may alias global memory");

  fprintf (file, "\n");
}


/* Dump variable VAR and its may-aliases to stderr.  */

void
debug_variable (var)
     tree var;
{
  dump_variable (stderr, var);
}


/* Dump def-use edges on FILE.  */

void
dump_immediate_uses (file)
     FILE *file;
{
  basic_block bb;
  gimple_stmt_iterator si;

  fprintf (file, "\nDef-use edges for function %s\n", current_function_name);

  FOR_EACH_BB (bb)
    {
      tree phi;

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	dump_immediate_uses_for (file, phi);

      for (si = gsi_start_bb (bb); !gsi_end_bb_p (si); gsi_step_bb (&si))
	dump_immediate_uses_for (file, gsi_stmt (si));
    }

  fprintf (file, "\n");
}


/* Dump def-use edges on stderr.  */

void
debug_immediate_uses ()
{
  dump_immediate_uses (stderr);
}


/* Dump all immediate uses for STMT on FILE.  */

void
dump_immediate_uses_for (file, stmt)
     FILE *file;
     tree stmt;
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
debug_immediate_uses_for (stmt)
     tree stmt;
{
  dump_immediate_uses_for (stderr, stmt);
}

/* Dump various DFA statistics to FILE.  */

void
dump_dfa_stats (file)
     FILE *file;
{
  struct dfa_stats_d dfa_stats;

  unsigned long size, total = 0;
  const char * const fmt_str   = "%-30s%-13s%12s\n";
  const char * const fmt_str_1 = "%-30s%13lu%11lu%c\n";
  const char * const fmt_str_3 = "%-43s%11lu%c\n";

  collect_dfa_stats (&dfa_stats);

  fprintf (file, "\nDFA Statistics for %s\n\n", current_function_name);

  fprintf (file, "---------------------------------------------------------\n");
  fprintf (file, fmt_str, "", "  Number of  ", "Memory");
  fprintf (file, fmt_str, "", "  instances  ", "used ");
  fprintf (file, "---------------------------------------------------------\n");

  size = num_referenced_vars * sizeof (tree);
  total += size;
  fprintf (file, fmt_str_1, "Referenced variables", num_referenced_vars, 
	   SCALE (size), LABEL (size));

  size = num_indirect_refs * sizeof (tree);
  total += size;
  fprintf (file, fmt_str_1, "INDIRECT_REFs", num_indirect_refs, 
	   SCALE (size), LABEL (size));

  size = num_addressable_vars * sizeof (tree);
  total += size;
  fprintf (file, fmt_str_1, "Addressable variables", num_addressable_vars, 
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

  size = dfa_stats.num_phis * sizeof (struct tree_phi_node);
  total += size;
  fprintf (file, fmt_str_1, "PHI nodes", dfa_stats.num_phis,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_phi_args * sizeof (struct phi_arg_d);
  total += size;
  fprintf (file, fmt_str_1, "PHI arguments", dfa_stats.num_phi_args,
 	   SCALE (size), LABEL (size));

  size = dfa_stats.size_tree_refs;
  total += size;
  fprintf (file, fmt_str_1, "Virtual references", dfa_stats.num_tree_refs,
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
debug_dfa_stats ()
{
  dump_dfa_stats (stderr);
}


/* Collect DFA statistics and store them in the structure pointed by
   DFA_STATS_P.  */

static void
collect_dfa_stats (dfa_stats_p)
     struct dfa_stats_d *dfa_stats_p;
{
  htab_t htab;
  basic_block bb;
  gimple_stmt_iterator i;

  if (dfa_stats_p == NULL)
    abort ();

  memset ((void *)dfa_stats_p, 0, sizeof (struct dfa_stats_d));

  /* Walk all the trees in the function counting references.  Start at
     basic block 0, but don't stop at block boundaries.  */
  htab = htab_create (30, htab_hash_pointer, htab_eq_pointer, NULL);

  for (i = gsi_start_bb (BASIC_BLOCK (0)); !gsi_end_bb_p (i); gsi_step_bb (&i))
    walk_tree (gsi_stmt_ptr (i), collect_dfa_stats_r, (void *) dfa_stats_p,
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
collect_dfa_stats_r (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees ATTRIBUTE_UNUSED;
     void *data;
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
clobber_vars_r (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees ATTRIBUTE_UNUSED;
     void *data;
{
  enum tree_code code = TREE_CODE (*tp);

  /* Add every *_DECL node to VDEFS and VUSES.  */
  if (code == VAR_DECL || code == PARM_DECL)
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
compute_may_aliases ()
{
  unsigned long i;
  static htab_t vars_found;
  static htab_t indirect_refs_found;
  static htab_t addressable_vars_found;
  basic_block bb;
  gimple_stmt_iterator si;
  htab_t walk_state[3];

  timevar_push (TV_TREE_MAY_ALIAS);

  num_indirect_refs = 0;
  VARRAY_TREE_INIT (indirect_refs, 20, "indirect_refs");

  num_addressable_vars = 0;
  VARRAY_TREE_INIT (addressable_vars, 20, "addressable_vars");

  /* Hash table of all the objects the SSA builder needs to be aware of.  */
  vars_found = htab_create (50, htab_hash_var, htab_var_eq, NULL);

  /* Hash table of all the unique INDIRECT_REFs found.  */
  indirect_refs_found = htab_create (50, htab_hash_var, htab_var_eq, NULL);

  /* Hash table of all the unique addressable variables found.  */
  addressable_vars_found = htab_create (50, htab_hash_var, htab_var_eq, NULL);

  walk_state[0] = vars_found;
  walk_state[1] = indirect_refs_found;
  walk_state[2] = addressable_vars_found;

  FOR_EACH_BB (bb)
    for (si = gsi_start_bb (bb); !gsi_end_bb_p (si); gsi_step_bb (&si))
      walk_tree (gsi_stmt_ptr (si), find_vars_r, &walk_state, NULL);

  htab_delete (vars_found);
  htab_delete (indirect_refs_found);
  htab_delete (addressable_vars_found);

  if (flag_tree_points_to != PTA_NONE && indirect_refs_found)
    {
      timevar_push (TV_TREE_PTA);
      create_alias_vars (current_function_decl);
      timevar_pop (TV_TREE_PTA);
    }

  if (flag_tree_points_to == PTA_NONE)
    {
      num_alias_tags = 0;
      alias_tags = (tree *) xmalloc ((num_indirect_refs + num_addressable_vars)
		     		      * sizeof (tree));
      memset ((void *) alias_tags, 0,
	      ((num_indirect_refs + num_addressable_vars) * sizeof (tree)));
    }
  
  for (i = 0; i < num_indirect_refs; i++)
    find_may_aliases_for (i);

  if (flag_tree_points_to == PTA_NONE)
    {
      if (tree_ssa_dump_file && tree_ssa_dump_flags & TDF_ALIAS)
	dump_alias_info (tree_ssa_dump_file);

      free (alias_tags);
      alias_tags = NULL;
      num_alias_tags = 0;
    }

  num_indirect_refs = 0;
  indirect_refs = 0;
  num_addressable_vars = 0;
  addressable_vars = 0;
  timevar_pop (TV_TREE_MAY_ALIAS);
}


/* Return TRUE if variables V1 and V2 may alias.  */

bool
may_alias_p (v1, v2)
     tree v1;
     tree v2;
{
  tree ptr, var, ptr_sym, var_sym;
  HOST_WIDE_INT ptr_alias_set, var_alias_set;

  if (v1 == v2)
    return true;

  /* One of the two variables needs to be an INDIRECT_REF or GLOBAL_VAR,
     otherwise they can't possibly alias each other.  */
  if (TREE_CODE (v1) == INDIRECT_REF || v1 == global_var)
    {
      ptr = v1;
      var = v2;
    }
  else if (TREE_CODE (v2) == INDIRECT_REF || v2 == global_var)
    {
      ptr = v2;
      var = v1;
    }
  else
    return false;

  ptr_sym = get_base_symbol (ptr);
  var_sym = get_base_symbol (var);

  /* GLOBAL_VAR aliases every global variable, pointer dereference and
     locals that have had their address taken, unless points-to analysis is
     done.  This is because points-to is supposed to handle this case, and
     thus, can give a more accurate answer.   */
  if (ptr_sym == global_var
      && (TREE_ADDRESSABLE (var_sym)
	  || TREE_CODE (var) == INDIRECT_REF
	  || decl_function_context (var_sym) == NULL))
    {
      if (flag_tree_points_to == PTA_NONE)	 
	return true;
      else
	{
	  /* Right now, it's just not worth the time/space to make
	     points-to handle the global variables seperately (in
	     intraprocedural mode, anyway).  */	     
	  if (decl_function_context (var_sym) == NULL)
	    return true;
	  
	  /* For GLOBAL_VAR, we want to see if the variable aliases
	     GLOBAL_VAR, not if GLOBAL_VAR aliases the variable (since
	     the points-to sets are possibly directional, and
	     GLOBAL_VAR never gets assigned to, only assigned from). */ 
	  if (ptr_may_alias_var (var_sym, ptr_sym))
	    return true;  
	}
    }

  /* VAR_DECLs that have not had their address taken cannot be aliased.  */
  if (DECL_P (var) && !TREE_ADDRESSABLE (var))
    return false;

  /* Compute type-based alias information.  If the alias sets don't
     conflict then PTR cannot alias VAR.  */
  ptr_alias_set = get_alias_set (ptr);
  var_alias_set = get_alias_set (var);
  if (!alias_sets_conflict_p (ptr_alias_set, var_alias_set))
    return false;

  /* If -ftree-points-to is given, check if PTR may point to VAR.  */
  if (flag_tree_points_to)
    if (!ptr_may_alias_var (ptr_sym, var_sym))
      return false;

  return true;
}


/* Find variables that may be aliased by the variable (V1) at
   index VAR_INDEX in the alias_vars varray. 

   For every variable V2 that V1 may alias, add V2 to one of the alias sets
   in the array ALIAS_TAGS.

   Each entry I in ALIAS_TAGS represents a set of all the variables that
   are aliased by ALIAS_TAGS[I].  In the absence of points-to information,
   the ALIAS_TAGS array will tend to have few entries, and each entry will
   likely alias many program variables.
   
   This will negatively impact optimizations because variable uses will be
   reached by many definitions that can't really reach them.  See the
   documentation in tree-ssa.c.  */

static void
find_may_aliases_for (indirect_ref_index)
     int indirect_ref_index;
{
  unsigned long i;
  tree v1 = VARRAY_TREE (indirect_refs, indirect_ref_index);

#if defined ENABLE_CHECKING
  if (TREE_CODE (v1) != INDIRECT_REF)
    abort ();
#endif

  if (may_access_global_mem (v1))
    set_may_alias_global_mem (v1);

  /* Note that our aliasing properties are symmetric, so we can
     start this loop at INDIRECT_REF_INDEX + 1 to cut down on the
     runtime for this routine.  */
  for (i = indirect_ref_index; i < num_indirect_refs; i++)
    {
      tree v2 = VARRAY_TREE (indirect_refs, i);

      if (v1 == v2)
	continue;

      if (may_alias_p (v1, v2))
	{
	  tree at;

	  if (flag_tree_points_to == PTA_NONE)
	    {
	      /* Since we are not doing points-to analysis, we aggregate
		 all the aliases into a single representative alias that
		 represents a group of variables with similar aliasing
		 characteristics.  */
	      at = find_alias_tag (v1, v2);
	      if (at == NULL_TREE)
		at = alias_tags [num_alias_tags++] = v1;
	      add_may_alias (v2, at);
	    }
	  else
	    {
	      /* With points-to analysis, we can add all the aliases we find
		 for a variable because the alias sets are much smaller
		 than what we get when doing type-based aliasing.  */
	      at = v1;
	      add_may_alias (v2, at);
	      add_may_alias (at, v2);
	    }

	  /* If V2 may access global memory, mark both AT and V1 as aliases
	     for global memory.  */
	  if (may_access_global_mem (v2))
	    {
	      set_may_alias_global_mem (at);
	      set_may_alias_global_mem (v1);
	    }
	}
    }

  for (i = 0; i < num_addressable_vars; i++)
    {
      tree v2 = VARRAY_TREE (addressable_vars, i);

      if (v1 == v2)
	continue;

      if (may_alias_p (v1, v2))
	{
	  tree at;

	  if (flag_tree_points_to == PTA_NONE)
	    {
	      /* Since we are not doing points-to analysis, we aggregate
		 all the aliases into a single representative alias that
		 represents a group of variables with similar aliasing
		 characteristics.  */
	      at = find_alias_tag (v1, v2);
	      if (at == NULL_TREE)
		at = alias_tags [num_alias_tags++] = v1;
	      add_may_alias (v2, at);
	    }
	  else
	    {
	      /* With points-to analysis, we can add all the aliases we find
		 for a variable because the alias sets are much smaller
		 than what we get when doing type-based aliasing.  */
	      at = v1;
	      add_may_alias (v2, at);
	      add_may_alias (at, v2);
	    }

	  /* If V2 may access global memory, mark both AT and V1 as aliases
	     for global memory.  */
	  if (may_access_global_mem (v2))
	    {
	      set_may_alias_global_mem (at);
	      set_may_alias_global_mem (v1);
	    }
	}
    }
}


/* Add ALIAS to the set of variables that may alias VAR.  */

static void
add_may_alias (var, alias)
     tree var;
     tree alias;
{
  size_t i;
  var_ann_t ann = var_ann (var);

  if (ann == NULL)
    ann = create_var_ann (var);

  if (ann->may_aliases == NULL)
    VARRAY_TREE_INIT (ann->may_aliases, 2, "aliases");

  /* Avoid adding duplicates.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ann->may_aliases); i++)
    if (same_var_p (alias, VARRAY_TREE (ann->may_aliases, i)))
      return;

  VARRAY_PUSH_TREE (ann->may_aliases, alias);
}


/* Traverse the ALIAS_TAGS array looking for an alias tag that may alias
   variable V1 or V2.  If no entry is found for either V1 or V2, return
   NULL_TREE to tell the caller that it should create a new entry in the
   ALIAS_TAG array.  */

static tree
find_alias_tag (v1, v2)
     tree v1;
     tree v2;
{
  unsigned long i;

  for (i = 0; i < num_alias_tags; i++)
    if (may_alias_p (alias_tags[i], v1) || may_alias_p (alias_tags[i], v2))
      return alias_tags[i];

  return NULL_TREE;
}


/* Dump alias information on FILE.  */

void
dump_alias_info (file)
     FILE *file;
{
  unsigned long i, j;

  fprintf (file, "\nAlias information for %s: %ld sets\n\n",
	   current_function_name, num_alias_tags);

  for (i = 0; i < num_alias_tags; i++)
    {
      fprintf (file, "Alias set #%ld:\n", i);
      fprintf (file, "  Tag: ");
      dump_variable (file, alias_tags[i]);
      fprintf (file, "  Aliases: { ");

      for (j = 0; j < num_addressable_vars; j++)
	{
	  tree var = VARRAY_TREE (addressable_vars, j);
	  varray_type aliases = may_aliases (var);
	  if (aliases && VARRAY_TREE (aliases, 0) == alias_tags[i])
	    {
	      print_generic_expr (file, var, 0);
	      fprintf (file, " ");
	    }
	}

      for (j = 0; j < num_indirect_refs; j++)
	{
	  tree var = VARRAY_TREE (indirect_refs, j);
	  varray_type aliases = may_aliases (var);
	  if (aliases && VARRAY_TREE (aliases, 0) == alias_tags[i])
	    {
	      print_generic_expr (file, var, 0);
	      fprintf (file, " ");
	    }
	}
      fprintf (file, "}\n\n");
    }
}


/* Dump alias information on stderr.  */

void
debug_alias_info ()
{
  dump_alias_info (stderr);
}



/*---------------------------------------------------------------------------
			     Miscellaneous helpers
---------------------------------------------------------------------------*/
/* Return TRUE if expression EXPR may return a pointer to global memory.  */

static bool
may_access_global_mem (expr)
     tree expr;
{
  char class;
  tree sym;

  if (expr == NULL_TREE)
    return false;

  /* Function arguments and global variables may reference global memory.  */
  if ((sym = get_base_symbol (expr)) != NULL_TREE)
    {
      if (TREE_CODE (sym) == PARM_DECL
	  || decl_function_context (sym) == NULL_TREE)
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
	if (may_access_global_mem (TREE_OPERAND (expr, i)))
	  return true;
    }

  return false;
}


/* Remove variable DECL from the block that declares it.  */

void
remove_decl (decl)
     tree decl;
{
  tree *loc;
  
  loc = find_decl_location (decl, DECL_INITIAL (current_function_decl));
  if (loc)
    *loc = TREE_CHAIN (decl);
}


/* Find the location for declaration DECL in lexical block BLOCK.  All the
   subblocks of BLOCK are searched as well if BLOCK does not declare DECL.
   Return an address LOC such that *LOC == DECL or NULL if DECL couldn't be
   located.  */

tree *
find_decl_location (decl, block)
     tree decl;
     tree block;
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
find_vars_r (tp, walk_subtrees, data)
    tree *tp;
    int *walk_subtrees ATTRIBUTE_UNUSED;
    void *data ATTRIBUTE_UNUSED;
{
  htab_t vars_found = ((htab_t *) data)[0];
  htab_t indirect_refs_found = ((htab_t *) data)[1];
  htab_t addressable_vars_found = ((htab_t *) data)[2];
  tree var = *tp;

  /* Function calls.  If the callee is neither pure nor const, consider
     this a reference for an artificial variable called GLOBAL_VAR. 
     This variable is a pointer that will alias every global variable and
     locals that have had their address taken.

     Stores to *GLOBAL_VAR will reach uses of every call clobbered
     variable in the function.  Uses of *GLOBAL_VAR will be reached by
     definitions of call clobbered variables.
     
     This is used to model the effects that the called function may have on
     local and global variables that might be visible to it.  */
  if (TREE_CODE (var) == CALL_EXPR)
    {
      tree callee = get_callee_fndecl (var);
      int flags = (callee) ? flags_from_decl_or_type (callee) : 0;
      if (! (flags & (ECF_CONST | ECF_PURE)))
	{
	  if (!is_dereferenced (global_var))
	    {
	      var = build1 (INDIRECT_REF, TREE_TYPE (TREE_TYPE (global_var)),
			    global_var);
	      set_indirect_ref (global_var, var);
	    }
	  else
	    var = indirect_ref (global_var);
	}
    }

  if (SSA_VAR_P (var))
    {
      void **slot;
      tree sym;
      tree ind;

      /* Get the underlying symbol.  We will need it shortly.  */
      if (TREE_CODE (var) == INDIRECT_REF)
	{
	  sym = get_base_symbol (var);
	  ind = var;

	  if (!is_dereferenced (sym))
	    set_indirect_ref (sym, ind);
	  else
	    ind = indirect_ref (sym);
	}
      else
	{
	  ind = NULL_TREE;
	  sym = var;
	}

      /* Make VAR either the canonical INDIRECT_REF or the real symbol.  */
      var = (ind ? ind : sym);

      /* First handle an INDIRECT_REF.  */
      if (ind)
	{
	  slot = htab_find_slot (indirect_refs_found, (void *)var, INSERT);
	  if (*slot == NULL)
	    {
	      *slot = (void *)var;
	      VARRAY_PUSH_TREE (indirect_refs, var);
	      num_indirect_refs++;
	    }

	}
      else if (TREE_ADDRESSABLE (sym)
	       || decl_function_context (sym) == NULL)
	{
	  slot = htab_find_slot (addressable_vars_found, (void *)var, INSERT);

	  if (*slot == NULL)
	    {
	      *slot = (void *)var;
	      VARRAY_PUSH_TREE (addressable_vars, var);
	      num_addressable_vars++;
	    }
	}
	  
      slot = htab_find_slot (vars_found, (void *) var, INSERT);
      if (*slot == NULL)
	{
	  /* This is the first time we find this variable, add it to the
	     REFERENCED_VARS array.  */
	  *slot = (void *) var;
	  VARRAY_PUSH_TREE (referenced_vars, var);
	  num_referenced_vars++;
	}
    }

  return NULL_TREE;
}


/* Return a hash value for a variable.  If the variable is an INDIRECT_REF
   node, return the hash value for its pointer.  */

hashval_t
htab_hash_var (p)
     const void *p;
{
  tree v = (tree) p;
  if (TREE_CODE (v) == INDIRECT_REF)
    v = TREE_OPERAND (v, 0);
  return htab_hash_pointer (v);
}


/* Check if variables V1 and V2 are the same.
   Two variables are the same if both V1 and V2 are pointers to the same
   variable, or if they are both indirect refs to the same variable.  */

int
htab_var_eq (v1, v2)
     const void *v1;
     const void *v2;
{
  return (v1 == v2
	  || (TREE_CODE ((tree) v1) == INDIRECT_REF
	      && TREE_CODE ((tree) v2) == INDIRECT_REF
	      && TREE_OPERAND ((tree) v1, 0) == TREE_OPERAND ((tree) v2, 0)));
}


/* Return the virtual variable associated to the non-scalar variable VAR.  */

tree
get_virtual_var (var)
     tree var;
{
  enum tree_code code;

  STRIP_NOPS (var);

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
