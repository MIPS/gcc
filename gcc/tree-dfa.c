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

/* Alias sets for type-based alias computation.  Each entry in the array
   ALIAS_SETS represents a unique tag that represents a group of variables
   with conflicting alias types.  This is only used when points-to analysis
   is disabled (-ftree-points-to).  */

struct alias_set_d
{
  tree tag;
  tree tag_sym;
  HOST_WIDE_INT tag_set;
  HOST_WIDE_INT tag_sym_set;
};

static varray_type alias_sets;


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
static void compute_alias_sets		PARAMS ((void));
static void register_alias_set		PARAMS ((tree, tree));
static void find_alias_for		PARAMS ((tree, tree));
static bool may_alias_p			PARAMS ((tree, tree, HOST_WIDE_INT,
						 tree, tree, HOST_WIDE_INT));
static bool may_access_global_mem_p 	PARAMS ((tree, tree));
static void set_def			PARAMS ((tree *, tree));
static void add_use			PARAMS ((tree *, tree));
static void add_vdef			PARAMS ((tree, tree, voperands_t));
static void add_vuse			PARAMS ((tree, tree, voperands_t));
static void add_stmt_operand		PARAMS ((tree *, tree, int, int,
      						 voperands_t));
static void add_immediate_use		PARAMS ((tree, tree));
static tree find_vars_r			PARAMS ((tree *, int *, void *));
static void add_referenced_var		PARAMS ((tree, tree, void *));
static void add_indirect_ref_var	PARAMS ((tree, void *));
static void compute_immediate_uses_for	PARAMS ((tree, int));
static void add_may_alias		PARAMS ((tree, tree, tree, tree));
static bool call_may_clobber		PARAMS ((tree));
static void find_vla_decls		PARAMS ((tree));
static tree find_vla_decls_r		PARAMS ((tree *, int *, void *));


/* Global declarations.  */

/* The total number of referenced variables in the function.  */
unsigned long num_referenced_vars;

/* Array of all variables referenced in the function.  */
varray_type referenced_vars;

/* The total number of unique aliased objects in the function.  */
static unsigned long num_aliased_objects;

/* Arrays for all the potentially aliased memory objects in the 
   function.

   A potentially aliased memory object can come from the following
   sources:

     * INDIRECT_REFs
     * Addressable variable which is used or assigned
     * global scoped variable which is used or assigned
     * ASM_OUTPUTs, ASM_CLOBBERS and ASM_INPUTs
     * CALL_EXPRs which load and store GLOBAL_VAR

   ALIASED_OBJECTS contains the object 
   ALIASED_OBJECTS_BASE contains the base symbol for those objects
   ALIASED_OBJECTS_ALIAS_SET contains the alias set for those objects.  */

static varray_type aliased_objects;
static varray_type aliased_objects_base;
static varray_type aliased_objects_alias_set;

/* Artificial variable used to model the effects of function calls on every
   variable that they may use and define.  Calls to non-const and non-pure
   functions are assumed to use and clobber this variable.

   Aliased loads and stores will be considered aliased with this variable.  */
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
	add_stmt_operand (expr_p, stmt, is_def, false, prev_vops);
      else
	get_expr_operands (stmt, &TREE_OPERAND (expr, 0), is_def, prev_vops);

      get_expr_operands (stmt, &TREE_OPERAND (expr, 1), false, prev_vops);
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
      tree op;
      bool may_clobber = call_may_clobber (expr);

      /* Find uses in the called function.  */
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), false, prev_vops);

      if (may_clobber)
	{
	  /* If the called function is neither pure nor const, we create a
	     clobbering definition of *GLOBAL_VAR.  */
	  tree v = indirect_ref (global_var);
	  add_stmt_operand (&v, stmt, true, true, prev_vops);
	}

      /* Add all the arguments to the function.  If the function will not
	 clobber any local variable, check if it may dereference a local
	 pointer.  If so, add a VUSE for the dereferenced pointer.  This is
	 to address the following problem: Supose that function 'foo' is
	 constant but receives a pointer to a local variable:

	    int foo (int *x)
	    {
	      return *x;
	    }

	    int bar()
	    {
	      i = 10;
	      p = &i;
	      return foo (p);
	    }

	 Since p has never been dereferenced in function bar(), the alias
	 analyzer will never associate 'i' with '*p', which then causes DCE
	 to kill the assignment to 'i' because it's never used in bar().
	 To address this problem, we add a VUSE<*p> at the call site of
	 foo().  */
      for (op = TREE_OPERAND (expr, 1); op; op = TREE_CHAIN (op))
	{
	  tree arg = TREE_VALUE (op);

	  add_stmt_operand (&TREE_VALUE (op), stmt, false, false, prev_vops);

	  /* If the function may not clobber locals, add a VUSE<*p> for
	     every pointer p passed in the argument list (see note above).  */
	  if (!may_clobber
	      && SSA_DECL_P (arg)
	      && POINTER_TYPE_P (TREE_TYPE (arg)))
	    {
	      tree deref = indirect_ref (arg);
	      add_stmt_operand (&deref, stmt, false, true, prev_vops);
	    }
	}

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
  varray_type aliases;
  size_t i;

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

  aliases = may_aliases (var);
  if (aliases == NULL)
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

  /* An assignment to a pointer variable 'p' may make 'p' point to memory
     outside of the current scope.  Therefore, dereferences of 'p' should be
     treated as references to global variables.  */
  if (is_def
      && SSA_DECL_P (var)
      && POINTER_TYPE_P (TREE_TYPE (var)))
    {
      tree deref;

      if (TREE_CODE (stmt) == MODIFY_EXPR
	  && may_access_global_mem_p (TREE_OPERAND (stmt, 1),
		  		    get_base_symbol (TREE_OPERAND (stmt, 1))))
	set_may_point_to_global_mem (var);

      /* A definition of a pointer variable 'p' clobbers its associated
	 indirect variable '*p', because now 'p' is pointing to a different
	 memory location.  */
      deref = indirect_ref (var);
      if (deref)
	{
	  /* Add a VDEF for '*p' (or its aliases if it has any).  */
	  aliases = may_aliases (deref);
	  if (aliases == NULL)
	    add_vdef (deref, stmt, prev_vops);
	  else
	    for (i = 0; i < VARRAY_ACTIVE_SIZE (aliases); i++)
	      add_vdef (VARRAY_TREE (aliases, i), stmt, prev_vops);

	  /* If 'p' may point to global memory, then '*p' and its aliases are
	     an alias for global memory.  */
	  if (may_point_to_global_mem_p (var))
	    {
	      varray_type aliases = may_aliases (deref);
	      set_may_alias_global_mem (deref);
	      for (i = 0; aliases && i < VARRAY_ACTIVE_SIZE (aliases); i++)
		{
		  tree alias = VARRAY_TREE (aliases, i);
		  set_may_alias_global_mem (alias);
		  if (TREE_CODE (alias) == INDIRECT_REF)
		    set_may_point_to_global_mem (TREE_OPERAND (alias, 0));
		}
	    }
	}
    }
  else
    {
      /* If VAR is a pointer dereference, we need to add a VUSE for its
	 base pointer.  If needed, strip its SSA version, to access the
	 base pointer.  Otherwise we won't recognize use of pointers after
	 variables have been renamed. For instance, in (*p)_35, we need to
	 add an operand for 'p', and for that we need to remove the SSA
	 version number first.  */
      if (TREE_CODE (var) == SSA_NAME)
	var = SSA_NAME_VAR (var);

      if (TREE_CODE (var) == INDIRECT_REF)
	add_stmt_operand (&TREE_OPERAND (var, 0), stmt, false, true, prev_vops);
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
	  if (var == SSA_NAME_VAR (VDEF_RESULT (d)))
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
    if (var == VDEF_RESULT (VARRAY_TREE (ann->vops->vdef_ops, i)))
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
	  if (var == SSA_NAME_VAR (u))
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
    if (var == VARRAY_TREE (ann->vops->vuse_ops, i))
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
  ann->common.stmt = t; 
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
  varray_type aliases;

  if (var == NULL_TREE)
    {
      fprintf (file, "<nil>");
      return;
    }

  print_generic_expr (file, var, 0);
  
  aliases = may_aliases (var);
  if (aliases)
    {
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

  if (is_vla_decl (var))
    fprintf (file, ", is used to declare a VLA");

  if (may_point_to_global_mem_p (var))
    fprintf (file, ", may point to global memory");

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
  block_stmt_iterator si;

  fprintf (file, "\nDef-use edges for function %s\n", current_function_name);

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

  size = num_aliased_objects * sizeof (tree);
  total += size;
  fprintf (file, fmt_str_1, "Aliased objects", num_aliased_objects, 
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

struct walk_state
{
  htab_t vars_found;
  htab_t aliased_objects_found;
  int is_store;
};

/* Compute may-alias information for every variable referenced in the
   program.  Note that in the absence of points-to analysis
   (-ftree-points-to), this may compute a much bigger set than necessary.  */

void
compute_may_aliases ()
{
  static htab_t vars_found;
  static htab_t aliased_objects_found;
  basic_block bb;
  block_stmt_iterator si;
  struct walk_state walk_state;

  /* Walk the lexical blocks in the function looking for variables that may
     have been used to declare VLAs.  Those variables will be considered
     implicitly live by passes like DCE.  FIXME: This is a hack.  GIMPLE
     should expose VLAs in the code.  */
  find_vla_decls (DECL_INITIAL (current_function_decl));

  timevar_push (TV_TREE_MAY_ALIAS);

  num_aliased_objects = 0;
  VARRAY_TREE_INIT (aliased_objects, 20, "aliased_objects");
  VARRAY_TREE_INIT (aliased_objects_base, 20, "aliased_objects_base");
  VARRAY_WIDE_INT_INIT (aliased_objects_alias_set, 20, "aliased_objects_alias_set");

  /* Hash table of all the objects the SSA builder needs to be aware of.  */
  vars_found = htab_create (50, htab_hash_pointer, htab_eq_pointer, NULL);

  /* Hash table of all the unique aliased objects found.  */
  aliased_objects_found = htab_create (50, htab_hash_pointer, htab_eq_pointer,
				     NULL);

  walk_state.vars_found = vars_found;
  walk_state.aliased_objects_found = aliased_objects_found;
  walk_state.is_store = 0;

  /* Find all the variables referenced in the function.  */
  FOR_EACH_BB (bb)
    for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
      walk_tree (bsi_stmt_ptr (si), find_vars_r, &walk_state, NULL);

  htab_delete (vars_found);
  htab_delete (aliased_objects_found);

  if (flag_tree_points_to != PTA_NONE && aliased_objects_found)
    {
      timevar_push (TV_TREE_PTA);
      create_alias_vars (current_function_decl);
      timevar_pop (TV_TREE_PTA);
    }

  compute_alias_sets ();

  num_aliased_objects = 0;
  aliased_objects = 0;
  aliased_objects_base = 0;
  aliased_objects_alias_set = 0;

  timevar_pop (TV_TREE_MAY_ALIAS);
}


/* Compute type-based alias sets.  This can be used in conjunction with
   points-to analysis.
 
   First we compute alias sets for objects which are stored into.  Stores
   create the only may alias relationships we care about.  If there are
   no aliased stores, then there's really nothing to do. 
  
   Once we have alias sets for the aliased stores, we can check for
   conflicts with all the aliased objects in the function.
   
   Each entry I in ALIAS_SETS represents a set of all the variables that
   are aliased by ALIAS_SETS[I].  The ALIAS_SETS array tends to have few
   entries, and each entry will likely alias many program variables.
   
   This will negatively impact optimizations because variable uses will be
   reached by many definitions that can't really reach them.  See the
   documentation in tree-ssa.c.  */

static void
compute_alias_sets ()
{
  size_t i;
  tree var, sym, deref_gv;

  VARRAY_GENERIC_PTR_INIT (alias_sets, 20, "alias_sets");

  /* For each object that is stored in the program, compute its alias set
     and register it in ALIAS_SETS.  If P's alias set does not conflict
     with any entry in ALIAS_SETS, or if it conflicts with more than one
     entry, create a new entry for P.

     We need to treat GLOBAL_VAR separately.  Since GLOBAL_VAR aliases
     variables that might have been otherwise unaliased, we register it
     first so that we make sure that if GLOBAL_VAR is needed for this
     function, it is always an alias tag.  Otherwise we will miss the
     following case:  Suppose that *P and Q are two variables that don't
     alias each other:

	foo (*P)
	{
	  *P = ...;
	  bar (&Q);
	}

     If *P is added to ALIAS_SETS first, then we will build an alias set
     tagged with *P which contains *P and GLOBAL_VAR.  However, since Q
     does not conflict with *P, it will never be added to the set.  */
  deref_gv = indirect_ref (global_var);
  if (deref_gv)
    register_alias_set (deref_gv, global_var);

  for (i = 0; i < num_aliased_objects; i++)
    {
      var = VARRAY_TREE (aliased_objects, i);
      sym = VARRAY_TREE (aliased_objects_base, i);
      if (var_ann (var)->is_stored && var != deref_gv)
	register_alias_set (var, sym);
    }

  /* Now for every potentially aliased object, see the object's alias
     set conflicts with any of the alias sets for objects which were
     stored.  */
  for (i = 0; i < num_aliased_objects; i++)
    {
      var = VARRAY_TREE (aliased_objects, i);
      sym = VARRAY_TREE (aliased_objects_base, i);
      find_alias_for (var, sym);
    }

  /* Debugging dumps.  */
  if (tree_ssa_dump_file && tree_ssa_dump_flags & TDF_ALIAS)
    {
      dump_alias_info (tree_ssa_dump_file);
      dump_referenced_vars (tree_ssa_dump_file);
    }

  /* Deallocate memory used by ALIAS_SETS.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (alias_sets); i++)
    {
      struct alias_set_d *as;
      as = (struct alias_set_d *) VARRAY_GENERIC_PTR (alias_sets, i);
      free (as);
    }

  alias_sets = NULL;
}


/* Try to add DEREF as a new new alias tag in ALIAS_SETS.  If a conflicting
   tag is already present, then instead of adding a new entry, DEREF is
   marked as an alias of the existing entry.  DEREF_SYM is the base symbol
   of DEREF.  */

static void
register_alias_set (deref, deref_sym)
     tree deref;
     tree deref_sym;
{
  size_t i, num_sets;
  struct alias_set_d *curr, *prev;
  HOST_WIDE_INT deref_set, deref_sym_set;
  long last_found;

  deref_set = get_alias_set (deref);
  deref_sym_set = get_alias_set (deref_sym);

  /* FIXME: ALIAS_SETS will usually be small (<< 10 entries).  If it becomes
     a performance problem, try with a splay tree.  */
  last_found = -1;
  prev = NULL;
  for (i = 0; i < VARRAY_ACTIVE_SIZE (alias_sets); i++)
    {
      curr = (struct alias_set_d *) VARRAY_GENERIC_PTR (alias_sets, i);

      if (may_alias_p (deref, deref_sym, deref_set,
	               curr->tag, curr->tag_sym, curr->tag_set))
	{
	  /* If this is the first time we find a conflict, mark the entry
	     index where the conflict was found and continue.  */
	  if (last_found == -1)
	    {
	      last_found = i;
	      continue;
	    }

	  /* We had found a conflict before, this means that DEREF may
	     alias variables that are in different alias sets.  If this
	     happens, we need to aggregate the two alias sets into a new
	     one with DEREF as its tag.

	     Replace the tag for the previous entry with DEREF and remove
	     the current entry by swapping it with the last element in the
	     ALIAS_SETS array.  */
	  prev = (struct alias_set_d *) VARRAY_GENERIC_PTR (alias_sets,
							    last_found);

	  /* We only need to update the previous tag once.  */
	  if (prev->tag != deref)
	    {
	      prev->tag = deref;
	      prev->tag_sym = deref_sym;
	      prev->tag_set = deref_set;
	      prev->tag_sym_set = deref_sym_set;
	    }

	  /* Swap the current and the last element, and pop the array.  */
	  num_sets = VARRAY_ACTIVE_SIZE (alias_sets);
	  if (i < num_sets - 1)
	    {
	      VARRAY_GENERIC_PTR (alias_sets, i) =
		  VARRAY_GENERIC_PTR (alias_sets, num_sets - 1);
	      i--;	/* Reset the iterator to avoid missing the entry we
			   just swapped.  */
	    }

	  VARRAY_POP (alias_sets);
	}
    }

  /* If DEREF aliased precisely one entry, then we may want to
     record DEREF as the alias set leader rather than the existing
     entry.  For example, if *p conflicted with q, we want to
     have *p be the alias leader.

     By doing this we can continue to not consider VAR_DECLs with the
     same alias set as not aliasing each other in may_alias_p.  */
  if (prev == NULL && last_found != -1)
    {
      prev
	= (struct alias_set_d *) VARRAY_GENERIC_PTR (alias_sets, last_found);

      /* If the existing entry was not an INDIRECT_REF and the current
         object is an INDIRECT_REF, then replace the existing entry with
	 the current entry.  */
      if (TREE_CODE (prev->tag) != INDIRECT_REF
	  && TREE_CODE (deref) == INDIRECT_REF)
	{
	  prev->tag = deref;
	  prev->tag_sym = deref_sym;
	  prev->tag_set = deref_set;
	  prev->tag_sym_set = deref_sym_set;
	}
    }

  /* If we didn't find an existing set that conflicts with SET.  Add it.  */
  if (last_found == -1)
    {
      curr = xmalloc (sizeof (*curr));
      curr->tag = deref;
      curr->tag_set = deref_set;
      curr->tag_sym = deref_sym;
      curr->tag_sym_set = deref_sym_set;
      VARRAY_PUSH_GENERIC_PTR (alias_sets, (void *) curr);
    }
}

/* Set an alias between VAR and any entry in ALIAS_SETS that has a
   conflicting alias set with VAR.  SYM is VAR's base symbol (needed when
   VAR is an INDIRECT_REF node).

   We can have multiple entries with the same alias set on the
   aliase_sets list.  How?  Consider two addressable VAR_DECLs of
   the same type.  They have the same alias set, but they are
   distinct objects (ie, they are known to be distinct from
   each other).  */

static void
find_alias_for (var, sym)
     tree var;
     tree sym;
{
  size_t i;
  HOST_WIDE_INT var_set;

  var_set = get_alias_set (var);

  for (i = 0; i < VARRAY_ACTIVE_SIZE (alias_sets); i++)
    {
      struct alias_set_d *as;

      as = (struct alias_set_d *) VARRAY_GENERIC_PTR (alias_sets, i);

      /* If AS->TAG aliases VAR, add it to the set of may-aliases for VAR
	 and return.  */
      if (may_alias_p (var, sym, var_set, as->tag, as->tag_sym, as->tag_set))
	{
	  /* Set the tag to be the alias for VAR.  */
	  add_may_alias (var, sym, as->tag, as->tag_sym);

	  if (may_aliases (as->tag) == NULL)
	    {
	      /* Force the alias tag to alias itself.  This avoids problems
		 when the program is accessing the alias tag directly.  For
		 instance, suppose that '*p' is the alias tag for 'i' and
		 'j':

			1  i = ...
			2  *p = ...
			3  ... = i

		If '*p' is not made an alias of itself, then the assignment
		at line 2 will be considered a killing definition of '*p'.
		This would make the assignment to 'i' at line 1 appear dead
		because the use of 'i' at line 3 is now reached by the
		assignment to '*p' at line 2.

		If '*p' does not alias 'i' at runtime, the compiler
		would've generated wrong code.  This fixes the regression
		of gcc.c-torture/execute/950929-1.c.  */
	      add_may_alias (as->tag, as->tag_sym, as->tag, as->tag_sym);
	    }
	}
    }
}


/* Return TRUE if variables V1 and V2 may alias. 
 
   V1_BASE is the base symbol for V1
   V1_ALIAS_SET is the alias set for V1
   V2_BASE is the base symbol for V2
   V2_ALIAS_SET is the base symbol for V2.  */

static bool
may_alias_p (v1, v1_base, v1_alias_set, v2, v2_base, v2_alias_set)
     tree v1;
     tree v1_base;
     HOST_WIDE_INT v1_alias_set;
     tree v2;
     tree v2_base;
     HOST_WIDE_INT v2_alias_set;
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
      ptr_sym = v1_base;
      ptr_alias_set = v1_alias_set;
      var = v2;
      var_sym = v2_base;
      var_alias_set = v2_alias_set;
    }
  else if (TREE_CODE (v2) == INDIRECT_REF || v2 == global_var)
    {
      ptr = v2;
      ptr_sym = v2_base;
      ptr_alias_set = v2_alias_set;
      var = v1;
      var_sym = v1_base;
      var_alias_set = v1_alias_set;
    }
  else
    return false;

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

  /* If the alias sets don't conflict then PTR cannot alias VAR.  */
  if (!alias_sets_conflict_p (ptr_alias_set, var_alias_set))
    {
      /* Handle aliases to structure fields.  If both VAR and PTR are
	 aggregate types, they may not have conflicting types, but one of
	 the structures could contain a pointer to the other one.

	 For instance, given

		struct P *p;
		struct Q *q;

	 It may happen that '*p' and '*q' can't alias because 'struct P'
	 and 'struct Q' have non-conflicting alias sets.  However, it could
	 happen that one of the fields in 'struct P' is a 'struct Q *' and
	 vice-versa.

	 Therefore, we also need to check if 'struct P' aliases 'struct Q *'
	 or 'struct Q' aliases 'struct P *'.  Notice, that since GIMPLE
	 does not have more than one-level pointers, we don't need to
	 recurse into the structures.  */
      if (TREE_CODE (var) == INDIRECT_REF
	  && AGGREGATE_TYPE_P (TREE_TYPE (ptr))
	  && AGGREGATE_TYPE_P (TREE_TYPE (var))
	  && (alias_sets_conflict_p (ptr_alias_set, get_alias_set (var_sym))
	     || alias_sets_conflict_p (var_alias_set, get_alias_set (ptr_sym))))
	return true;
      else
	return false;
    }

  /* If -ftree-points-to is given, check if PTR may point to VAR.  */
  if (flag_tree_points_to)
    if (!ptr_may_alias_var (ptr_sym, var_sym))
      return false;

  return true;
}

/* Add ALIAS to the set of variables that may alias VAR.  VAR_SYM and
   ALIAS_SYM are the base symbols for VAR and ALIAS respectively.  */

static void
add_may_alias (var, var_sym, alias, alias_sym)
     tree var;
     tree var_sym;
     tree alias;
     tree alias_sym;
{
  size_t i;
  var_ann_t v_ann = var_ann (var);
  var_ann_t a_ann = var_ann (alias);

  if (v_ann == NULL)
    v_ann = create_var_ann (var);

  if (v_ann->may_aliases == NULL)
    VARRAY_TREE_INIT (v_ann->may_aliases, 2, "aliases");

  /* Avoid adding duplicates.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (v_ann->may_aliases); i++)
    if (alias == VARRAY_TREE (v_ann->may_aliases, i))
      return;

  /* If either VAR or ALIAS may access global memory, then mark the other
     one as a global memory alias.  */
  if (var != alias)
    {
      if (a_ann == NULL)
	a_ann = create_var_ann (alias);

      if (may_access_global_mem_p (var, var_sym))
	a_ann->may_alias_global_mem = 1;

      if (may_access_global_mem_p (alias, alias_sym))
	v_ann->may_alias_global_mem = 1;
    }

  VARRAY_PUSH_TREE (v_ann->may_aliases, alias);
}


/* Dump alias information on FILE.  */

void
dump_alias_info (file)
     FILE *file;
{
  unsigned long i, j;

  if (alias_sets == NULL)
    return;

  fprintf (file, "\nAlias information for %s: %ld sets\n\n",
	   current_function_name, (long) VARRAY_ACTIVE_SIZE (alias_sets));

  for (i = 0; i < VARRAY_ACTIVE_SIZE (alias_sets); i++)
    {
      struct alias_set_d *as;

      as = (struct alias_set_d *) VARRAY_GENERIC_PTR (alias_sets, i);

      fprintf (file, "Alias set #%ld:\n", i);
      fprintf (file, "  Tag: ");
      dump_variable (file, as->tag);
      fprintf (file, "  Aliases objects: { ");

      for (j = 0; j < num_aliased_objects; j++)
	{
	  tree var = VARRAY_TREE (aliased_objects, j);
	  varray_type aliases = may_aliases (var);
	  if (aliases && VARRAY_TREE (aliases, 0) == as->tag)
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
/* Return TRUE if expression EXPR may reference memory outside the current
   function scope.  */

static bool
may_access_global_mem_p (expr, expr_base)
     tree expr;
     tree expr_base;
{
  char class;

  if (expr == NULL_TREE)
    return false;

  /* Function arguments and global variables may reference global memory.  */
  if (expr_base != NULL_TREE)
    {
      if (TREE_CODE (expr_base) == PARM_DECL
	  || decl_function_context (expr_base) == NULL_TREE)
	return true;
    }

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
	if (may_access_global_mem_p (TREE_OPERAND (expr, i),
				   get_base_symbol (TREE_OPERAND (expr, i))))
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
    void *data;
{
  tree t = *tp;
  struct walk_state *walk_state = (struct walk_state *)data;
  int saved_is_store = walk_state->is_store;

  if (TREE_CODE (t) == MODIFY_EXPR || TREE_CODE (t) == INIT_EXPR)
    {
      walk_state->is_store = 1;
      walk_tree (&TREE_OPERAND (t, 0), find_vars_r, data, NULL);
      walk_state->is_store = 0;
      walk_tree (&TREE_OPERAND (t, 1), find_vars_r, data, NULL);
      walk_state->is_store = saved_is_store;
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

  if (SSA_VAR_P (*tp))
    {
      tree sym, var, deref;

      var = *tp;
      sym = get_base_symbol (var);

      /* If VAR is an INDIRECT_REF node rewrite *TP with the first
	 INDIRECT_REF we found (if any).  We need to share INDIRECT_REF
	 nodes because we treat them as regular variables and several
	 passes rely on pointer equality for testing if two variables are
	 the same.  */
      if (TREE_CODE (var) == INDIRECT_REF)
	{
	  deref = indirect_ref (sym);
	  if (deref)
	    *tp = var = deref;
	  else
	    set_indirect_ref (sym, var);
	}

      add_referenced_var (var, sym, data);

      return NULL_TREE;
    }


  /* Function calls.  Consider them a reference for an artificial variable
     called GLOBAL_VAR.  This variable is a pointer that will alias every
     global variable and locals that have had their address taken.  The
     exception to this rule are functions marked pure, const or if they are
     known to not return.

     Stores to *GLOBAL_VAR will reach uses of every call clobbered variable
     in the function.  Uses of *GLOBAL_VAR will be reached by definitions
     of call clobbered variables.

     This is used to model the effects that the called function may have on
     local and global variables that might be visible to it.  */
  if (TREE_CODE (*tp) == CALL_EXPR)
    {
      if (call_may_clobber (*tp))
	{
          walk_state->is_store = 1;
	  add_indirect_ref_var (global_var, data);
	}
      else
	{
	  tree op;

          walk_state->is_store = 0;
	  /* If the function does not clobber locals, it still may
	     dereference them.  Scan its operands to see if it receives any
	     pointers.  For every pointer 'p' add '*p' to the list of
	     referenced variables.  */
	  for (op = TREE_OPERAND (*tp, 1); op; op = TREE_CHAIN (op))
	    {
	      tree arg = TREE_VALUE (op);
	      if (SSA_DECL_P (arg) && POINTER_TYPE_P (TREE_TYPE (arg)))
		add_indirect_ref_var (arg, data);
	    }
	}
      walk_state->is_store = saved_is_store;
    }

  return NULL_TREE;
}


/* Add VAR to the list of dereferenced variables.  SYM is the base symbol
   when VAR is anything but a _DECL node.  Otherwise, SYM and VAR are the
   same tree.
   
   Also add VAR to the ALIASED_OBJECTS set of varrays that are needed for
   alias analysis.  DATA is an array with three two tables used to avoid
   adding the same variable more than once to its corresponding set as
   well as a bit indicating if we're processing a load or store.  Note
   that this function assumes that VAR is a valid SSA variable.  */

static void
add_referenced_var (var, sym, data)
     tree var;
     tree sym;
     void *data;
{
  void **slot;
  struct walk_state *walk_state = (struct walk_state *)data;
  htab_t vars_found = walk_state->vars_found;
  htab_t aliased_objects_found = walk_state->aliased_objects_found;
  int is_store = walk_state->is_store;

  /* First handle aliasing information.  */
  if (TREE_CODE (var) == INDIRECT_REF
      || TREE_ADDRESSABLE (sym)
      || decl_function_context (sym) == NULL)
    {
      var_ann_t ann;
      slot = htab_find_slot (aliased_objects_found, (void *) var, INSERT);
      if (*slot == NULL)
	{
	  *slot = (void *) var;

	  VARRAY_PUSH_TREE (aliased_objects, var);
	  VARRAY_PUSH_TREE (aliased_objects_base, sym);
	  VARRAY_PUSH_INT (aliased_objects_alias_set, get_alias_set (var));
	  num_aliased_objects++;
	}

      /* Note if this object was loaded or stored.  */
      ann = var_ann (var);
      if (! ann)
	ann = create_var_ann (var);
      if (is_store)
	ann->is_stored = 1;
      else
	ann->is_loaded = 1;
    }

  slot = htab_find_slot (vars_found, (void *) var, INSERT);
  if (*slot == NULL)
    {
      var_ann_t ann;

      /* This is the first time we find this variable, add it to the
         REFERENCED_VARS array, also annotate it with its unique id.  */
      *slot = (void *) var;
      VARRAY_PUSH_TREE (referenced_vars, var);
      ann = var_ann (var);
      if (! ann)
	ann = create_var_ann (var);
      ann->uid = num_referenced_vars++;

      /* Arguments or global variable pointers may point to memory outside
	 the current function.  */
      if (POINTER_TYPE_P (TREE_TYPE (var))
	  && DECL_P (var)
	  && (TREE_CODE (var) == PARM_DECL
	      || decl_function_context (var) == NULL_TREE))
	set_may_point_to_global_mem (var);

      /* Dereferences of pointers that may point to global memory must be
	 marked as global memory aliases.  Note that we still can't use the
	 may_point_to_global_mem_p predicate here, because we may not have
	 processed the base pointer yet.  */
      if (TREE_CODE (var) == INDIRECT_REF
	  && (TREE_CODE (sym) == PARM_DECL
	      || decl_function_context (sym) == NULL_TREE))
	{
	  set_may_point_to_global_mem (sym);
	  set_may_alias_global_mem (var);
	}
    }
}


/* Add a reference to the INDIRECT_REF node of variable VAR.  If VAR has
   not been dereferenced yet, create a new INDIRECT_REF node for it.  DATA
   is as in add_referenced_var.  Note that VAR is assumed to be a valid
   pointer decl.  */

static void
add_indirect_ref_var (ptr, data)
     tree ptr;
     void *data;
{
  tree deref = indirect_ref (ptr);
  if (deref == NULL_TREE)
    {
      deref = build1 (INDIRECT_REF, TREE_TYPE (TREE_TYPE (ptr)), ptr);
      set_indirect_ref (ptr, deref);
    }

  add_referenced_var (deref, ptr, data);
}


/* Return the virtual variable associated to the non-scalar variable VAR.  */

tree
get_virtual_var (var)
     tree var;
{
  enum tree_code code;

  STRIP_NOPS (var);

  if (TREE_CODE (var) == SSA_NAME)
    var = SSA_NAME_VAR (var);

  code = TREE_CODE (var);

  while (code == ARRAY_REF
         || code == COMPONENT_REF
	 || code == REALPART_EXPR
	 || code == IMAGPART_EXPR
	 || (code == INDIRECT_REF
	     && !DECL_P (TREE_OPERAND (var, 0))))
    {
      var = TREE_OPERAND (var, 0);
      code = TREE_CODE (var);
    }

  return var;
}


/* Return true if EXPR is a CALL_EXPR tree to a function that may clobber
   globals and local addressable variables.  */

static bool
call_may_clobber (expr)
     tree expr;
{
  tree callee;
  int flags;

  if (TREE_CODE (expr) != CALL_EXPR)
    return false;

  callee = get_callee_fndecl (expr);
  flags = (callee) ? flags_from_decl_or_type (callee) : 0;
  return (! (flags & (ECF_CONST | ECF_PURE | ECF_NORETURN)));
}


/* Mark variables that are being used to declare VLAs (Variable Length
   Arrays).  Those variables will be considered implicitly live by the
   dataflow routines.  FIXME: This is a huge ugly hack.  GIMPLE should
   expose VLAs more gracefully.  */

static void
find_vla_decls (block)
    tree block;
{
  tree sub, decl;

  /* Check all the arrays declared in the block.  */
  for (decl = BLOCK_VARS (block); decl; decl = TREE_CHAIN (decl))
    {
      int inside_vla = 0;
      walk_tree (&decl, find_vla_decls_r, &inside_vla, NULL);
    }

  /* Now repeat the search in any sub-blocks.  */
  for (sub = BLOCK_SUBBLOCKS (block); sub; sub = TREE_CHAIN (sub))
    find_vla_decls (sub);
}


/* Callback for walk_tree used by find_vla_decls to analyze each array in a
   lexical block.  For each array type, it walks its TYPE_SIZE and
   TYPE_SIZE_UNIT trees looking for VAR_DECLs.  Those VAR_DECLs will be
   marked as needed for VLA.  */

static tree
find_vla_decls_r (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees ATTRIBUTE_UNUSED;
     void *data ATTRIBUTE_UNUSED;
{
  int *inside_vla = (int *) data;

  if (TREE_CODE (*tp) == ARRAY_TYPE)
    {
      *inside_vla = 1;
      walk_tree (&TYPE_SIZE (*tp), find_vla_decls_r, inside_vla, NULL);
      walk_tree (&TYPE_SIZE_UNIT (*tp), find_vla_decls_r, inside_vla, NULL);
    }
  else if (TREE_CODE (*tp) == TYPE_DECL)
    {
      *inside_vla = 1;
      walk_tree (&TYPE_FIELDS (TREE_TYPE (*tp)), find_vla_decls_r,
	         inside_vla, NULL);
    }
  else if (*inside_vla && SSA_DECL_P (*tp))
    set_vla_decl (*tp);

  return NULL_TREE;
}
