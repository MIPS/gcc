/* SSA operands management for trees.
   Copyright (C) 2003 Free Software Foundation, Inc.

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
#include "flags.h"
#include "function.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "ggc.h"

/* Flags to describe operand properties in get_stmt_operands and helpers.  */

/* By default, operands are loaded.  */
#define opf_none	0

/* Operand is the target of an assignment expression.  */
#define opf_is_def 	(1 << 0)

/* No virtual operands should be created in the expression.  This is used
   when traversing ADDR_EXPR nodes which have different semantics than
   other expressions.  Inside an ADDR_EXPR node, the only operands that we
   need to consider are indices into arrays.  For instance, &a.b[i] should
   generate a USE of 'i' but it should not generate a VUSE for 'a' nor a
   VUSE for 'b'.  */
#define opf_no_vops 	(1 << 1)

/* Array for building all the def operands.  */
static GTY (()) varray_type build_defs;

/* Array for building all the use operands.  */
static GTY (()) varray_type build_uses;

/* Array for building all the vdef operands.  */
static GTY (()) varray_type build_vdefs;

/* Array for building all the vuse operands.  */
static GTY (()) varray_type build_vuses;

#ifdef ENABLE_CHECKING
tree check_build_stmt;
#endif

static void note_addressable (tree, stmt_ann_t);
static void get_expr_operands (tree, tree *, int, voperands_t);
static inline void append_def (tree *, tree);
static inline void append_use (tree *, tree);
static void append_vdef (tree, tree, voperands_t);
static void add_call_clobber_ops (tree, voperands_t);
static void add_call_read_ops (tree, voperands_t);
static void add_stmt_operand (tree *, tree, int, voperands_t);
static int get_call_flags (tree);

#define VECSIZE  		1024
#define SPECIAL_THRESHOLD	128
#define NUM_FREE		3

typedef struct vecmanage_d
{
  varray_type vecs;
  varray_type special;  /* Used for large vectors.  */
  size_t top;
  size_t type_size;
  void *free[NUM_FREE];
  void *free_32;
  void *free_128;
  void *free_1024;
} vecmanage_t;

typedef vecmanage_t *vecmanage_type;


/* Adds a new segment, and returns the new segments index.  */
static int
vecmanage_add_segment (vecmanage_type sa)
{
  void *ptr;
  ptr = xmalloc (sa->type_size * VECSIZE);
  VARRAY_PUSH_GENERIC_PTR_NOGC (sa->vecs, ptr);
  return VARRAY_ACTIVE_SIZE (sa->vecs) - 1;
}

static void *
vecmanage_add_special (vecmanage_type sa, size_t size)
{
  void *ptr;
  ptr = xmalloc (sa->type_size * size);
  if (!sa->special)
    VARRAY_GENERIC_PTR_NOGC_INIT (sa->special, 10, "vecmanage_special");
  VARRAY_PUSH_GENERIC_PTR_NOGC (sa->special, ptr);
  return ptr;
}

static void
vecmanage_init (vecmanage_type sa, size_t size)
{
  int x;
  sa->top = 0;
  sa->special = NULL;
  sa->type_size = size;
  for (x = 0; x < NUM_FREE; x++)
    sa->free[x] = NULL;
  /* Allocate the first segment.  */
  VARRAY_GENERIC_PTR_NOGC_INIT (sa->vecs, 10, "vecmanage_vecs");
  vecmanage_add_segment (sa);
}

static inline void 
vecmanage_tree_ptr_init (vecmanage_type sa)
{
  vecmanage_init (sa, sizeof (tree *));
}

static void
vecmanage_fini (vecmanage_type sa)
{
  int x;
  void *ptr;

  sa->top = 0;
  if (!sa->vecs)
    return;

  /* Free the allocated vectors in reverse order of allocation.  */
  for (x = VARRAY_ACTIVE_SIZE (sa->vecs) - 1; x >= 0; x--)
    {
      ptr = VARRAY_TOP_GENERIC_PTR_NOGC (sa->vecs);
      VARRAY_POP (sa->vecs);
      free (ptr);
    }
  VARRAY_FREE (sa->vecs);

  if (sa->special)
    {
      for (x = VARRAY_ACTIVE_SIZE (sa->special) - 1; x >= 0; x--)
	{
	  ptr = VARRAY_TOP_GENERIC_PTR_NOGC (sa->special);
	  VARRAY_POP (sa->special);
	  free (ptr);
	}
      VARRAY_FREE (sa->special);
    }
}

static void **
check_free (vecmanage_type sa, size_t num)
{
  void **vec = NULL;

  if (num <= NUM_FREE && sa->free[num - 1])
    {
      vec = (void **)sa->free[num - 1];
      sa->free[num - 1] = *vec;
    }
  return vec;
}
/* Return a vector of contguous memory of a specified size.  */

static void *
vecmanage_new_vector (vecmanage_type sa, size_t num)
{
  int vec_num;
  size_t vec_index;
  void **vec;
  vec_num = VARRAY_ACTIVE_SIZE (sa->vecs) - 1;
  vec_index = sa->top;

  vec = check_free (sa, num);
  if (vec)
    return vec;
  if (vec_index + num >= VECSIZE)
    {
      if (num < SPECIAL_THRESHOLD)
        {
	  vec_num = vecmanage_add_segment (sa);
	  vec_index = 0;
	}
      else
        {
	  vec = vecmanage_add_special (sa, num);
	  return vec;
	}
    }

  vec = (void **)VARRAY_GENERIC_PTR_NOGC (sa->vecs, vec_num);
  vec = &((vec)[vec_index]);
  sa->top = vec_index + num;
  return vec;
}

static inline tree **
vecmanage_new_tree_ptr_vector (vecmanage_type sa, size_t num)
{
  return (tree **)vecmanage_new_vector (sa, num);
}

static inline void
vecmanage_free_vector (vecmanage_type sa, void **vec, size_t size)
{
#ifdef ENABLE_CHECKING
  if (size == 0)
    abort ();
#endif
  /* Put it into one of the free lists.  */
  if (size > NUM_FREE)
    size = 2;

  vec[0] = sa->free[size - 1];
  sa->free[size - 1] = (void *)vec;
  return;
}

vecmanage_t ssa_operands;

static inline tree **
allocate_ssa_op_vec (unsigned num)
{
  return vecmanage_new_tree_ptr_vector (&ssa_operands, num);
}

static inline void
free_ssa_op_vec (tree **vec, size_t size)
{
  vecmanage_free_vector (&ssa_operands, (void **)vec, size);
}
static inline tree *
allocate_ssa_virtual_op_vec (unsigned num)
{
  return (tree *)ggc_alloc (num * sizeof (tree));
}

static inline operands_t
allocate_operands_t (void)
{
  operands_t ptr;
  ptr = (operands_t)ggc_alloc (sizeof (struct operands_d));
  memset ((void *) ptr , 0, sizeof (struct operands_d));
  return ptr;
}

static inline voperands_t
allocate_voperands_t (void)
{
  voperands_t ptr;
  ptr = (voperands_t)ggc_alloc (sizeof (struct voperands_d));
  memset ((void *) ptr , 0, sizeof (struct voperands_d));
  return ptr;
}

static inline void
free_uses (use_optype uses)
{
  if (NUM_USES (uses) > 0)
    {
      free_ssa_op_vec (uses->uses, uses->num_uses);
      uses->num_uses = 0;
      uses->uses = NULL;
    }
}

static inline void
free_defs (def_optype defs)
{
  if (NUM_DEFS (defs) > 0)
    {
      free_ssa_op_vec (defs->defs, defs->num_defs);
      defs->num_defs = 0;
      defs->defs = NULL;
    }
}

void
init_ssa_operands (void)
{
  VARRAY_TREE_PTR_INIT (build_defs, 5, "build defs");
  VARRAY_TREE_PTR_INIT (build_uses, 10, "build uses");
  VARRAY_TREE_INIT (build_vdefs, 10, "build vdefs");
  VARRAY_TREE_INIT (build_vuses, 10, "build vuses");
  vecmanage_tree_ptr_init (&ssa_operands);
}

void
fini_ssa_operands (void)
{
  vecmanage_fini (&ssa_operands);
}

static inline tree **
finalize_new_ssa_operands (varray_type build)
{
  tree **vec;
  unsigned num, x;

  num = VARRAY_ACTIVE_SIZE (build);
  if (num == 0)
    return 0;

  vec = allocate_ssa_op_vec (num);
  for (x = 0; x < num; x++)
    vec[x] = VARRAY_TREE_PTR (build, x);

  VARRAY_POP_ALL (build);
  return vec;
}

static inline tree *
finalize_new_ssa_virtual_operands (varray_type build)
{
  tree *vec;
  unsigned num, x;

  num = VARRAY_ACTIVE_SIZE (build);
  if (num == 0)
    return 0;

  vec = allocate_ssa_virtual_op_vec (num);
  for (x = 0; x < num; x++)
    vec[x] = VARRAY_TREE (build, x);

  VARRAY_POP_ALL (build);
  return vec;
}

static void
finalize_ssa_defs (tree stmt)
{
  unsigned num;
  tree **vec;
  stmt_ann_t ann;

  num = VARRAY_ACTIVE_SIZE (build_defs);
  if (num == 0)
    return;

  vec = finalize_new_ssa_operands (build_defs);
#ifdef ENABLE_CHECKING
  /* There should only be a single real definition per assignment.  */
  if (TREE_CODE (stmt) == MODIFY_EXPR && num > 1)
    abort ();
#endif

  ann = stmt_ann (stmt);
  if (ann->ops == NULL)
    ann->ops = allocate_operands_t ();
  ann->ops->def_ops.num_defs = num;
  ann->ops->def_ops.defs = vec;
}

static void
finalize_ssa_uses (tree stmt)
{
  unsigned num;
  tree **vec;
  stmt_ann_t ann;

  num = VARRAY_ACTIVE_SIZE (build_uses);
  if (num == 0)
    return;

  vec = finalize_new_ssa_operands (build_uses);

  ann = stmt_ann (stmt);
  if (ann->ops == NULL)
    ann->ops = allocate_operands_t ();
  ann->ops->use_ops.num_uses = num;
  ann->ops->use_ops.uses = vec;
#ifdef ENABLE_CHECKING
  {
    unsigned x;
    /* If the pointer to the operand is the statement itself, something is
       wrong.  It means that we are pointing to a local variable (the 
       initial call to get_stmt_operands does not pass a pointer to a 
       statement).  */
    for (x = 0; x < num; x++)
      if (*(vec[x]) == stmt)
	abort ();
  }
#endif
}

static void
finalize_ssa_vdefs (tree stmt)
{
  unsigned num;
  tree *vec;
  stmt_ann_t ann;

  num = VARRAY_ACTIVE_SIZE (build_vdefs);
  if (num == 0)
    return;

  vec = finalize_new_ssa_virtual_operands (build_vdefs);

#ifdef ENABLE_CHECKING
  /* VDEFs must be entered in pairs of result/uses.  */
  if (num % 2 != 0)
    abort();
#endif

  ann = stmt_ann (stmt);
  if (ann->vops == NULL)
    ann->vops = allocate_voperands_t ();
  ann->vops->vdef_ops.num_vdefs = num / 2;
  ann->vops->vdef_ops.vdefs = vec;
}

static inline void
finalize_ssa_vuses (tree stmt)
{
  unsigned num;
  tree *vec;
  stmt_ann_t ann;
  vdef_optype vdefs;

#ifdef ENABLE_CHECKING
  if (VARRAY_ACTIVE_SIZE (build_vdefs) > 0)
    {
      fprintf (stderr, "Please finalize VDEFs before finalize VUSES.\n");
      abort ();
    }
#endif

  num = VARRAY_ACTIVE_SIZE (build_vuses);
  if (num == 0)
    return;

  /* Remove superfluous VUSE operands.  If the statement already has a
   VDEF operation for a variable 'a', then a VUSE for 'a' is not
   needed because VDEFs imply a VUSE of the variable.  For instance,
   suppose that variable 'a' is aliased:

	      # VUSE <a_2>
	      # a_3 = VDEF <a_2>
	      a = a + 1;

  The VUSE <a_2> is superfluous because it is implied by the VDEF
  operation.  */

  ann = stmt_ann (stmt);
  vdefs = VDEF_OPS (ann);
  if (NUM_VDEFS (vdefs) > 0)
    {
      size_t i, j;
      for (i = 0; i < VARRAY_ACTIVE_SIZE (build_vuses); i++)
	{
	  bool found = false;
	  for (j = 0; j < NUM_VDEFS (vdefs); j++)
	    {
	      tree vuse_var, vdef_var;
	      tree vuse = VARRAY_TREE (build_vuses, i);
	      tree vdef = VDEF_OP (vdefs, j);

	      if (TREE_CODE (vuse) == SSA_NAME)
		vuse_var = SSA_NAME_VAR (vuse);
	      else
		vuse_var = vuse;

	      if (TREE_CODE (vdef) == SSA_NAME)
		vdef_var = SSA_NAME_VAR (vdef);
	      else
		vdef_var = vdef;

	    if (vuse_var == vdef_var)
	      {
		found = true;
		break;
	      }
	    }

	  /* If we found a useless VUSE operand, remove it from the
	     operand array by replacing it with the last active element
	     in the operand array (unless the useless VUSE was the
	     last operand, in which case we simply remove it.  */
	  if (found)
	    {
	      if (i != VARRAY_ACTIVE_SIZE (build_vuses) - 1)
		{
		  VARRAY_TREE (build_vuses, i)
		    = VARRAY_TREE (build_vuses,
				   VARRAY_ACTIVE_SIZE (build_vuses) - 1);
		}
	      VARRAY_POP (build_vuses);

	      /* We want to rescan the element at this index, unless
		 this was the last element, in which case the loop
		 terminates.  */
	      i--;
	    }
	}
    }

  num = VARRAY_ACTIVE_SIZE (build_vuses);
  vec = finalize_new_ssa_virtual_operands (build_vuses);

  if (ann->vops == NULL)
    ann->vops = allocate_voperands_t ();
  ann->vops->vuse_ops.num_vuses = num;
  ann->vops->vuse_ops.vuses = vec;
}

extern void
finalize_ssa_stmt_operands (tree stmt)
{
#ifdef ENABLE_CHECKING
  if (check_build_stmt == NULL)
    abort();
#endif

  finalize_ssa_defs (stmt);
  finalize_ssa_uses (stmt);
  finalize_ssa_vdefs (stmt);
  finalize_ssa_vuses (stmt);

#ifdef ENABLE_CHECKING
  check_build_stmt = NULL;
#endif
}


extern void
verify_start_operands (tree stmt ATTRIBUTE_UNUSED)
{
#ifdef ENABLE_CHECKING
  if (VARRAY_ACTIVE_SIZE (build_defs) > 0 
      || VARRAY_ACTIVE_SIZE (build_uses) > 0
      || VARRAY_ACTIVE_SIZE (build_vuses) > 0
      || VARRAY_ACTIVE_SIZE (build_vdefs) > 0)
    abort ();
  if (check_build_stmt != NULL)
    abort();
  check_build_stmt = stmt;
#endif
}


/* Add DEF_P to the list of pointers to operands defined by STMT.  */

static inline void
append_def (tree *def_p, tree stmt ATTRIBUTE_UNUSED)
{
#ifdef ENABLE_CHECKING
  if (check_build_stmt != stmt)
    abort();
#endif
  VARRAY_PUSH_TREE_PTR (build_defs, def_p);
}


/* Add USE_P to the list of pointers to operands used by STMT.  */

static inline void
append_use (tree *use_p, tree stmt ATTRIBUTE_UNUSED)
{
#ifdef ENABLE_CHECKING
  if (check_build_stmt != stmt)
    abort();
#endif
  VARRAY_PUSH_TREE_PTR (build_uses, use_p);
}


/* Add a new virtual def for variable VAR to statement STMT.  If PREV_VOPS
   is not NULL, the existing entries are preserved and no new entries are
   added here.  This is done to preserve the SSA numbering of virtual
   operands.  */

static void
append_vdef (tree var, tree stmt, voperands_t prev_vops)
{
  stmt_ann_t ann;
  size_t i;
  tree result, source;

#ifdef ENABLE_CHECKING
  if (check_build_stmt != stmt)
    abort();
#endif

  ann = stmt_ann (stmt);

  /* Don't allow duplicate entries.  */

  for (i = 0; i < VARRAY_ACTIVE_SIZE (build_vdefs); i += 2)
    {
      tree result = VARRAY_TREE (build_vdefs, i);
      if (var == result
	  || (TREE_CODE (result) == SSA_NAME
	      && var == SSA_NAME_VAR (result)))
	return;
    }

  /* If the statement already had virtual definitions, see if any of the
     existing VDEFs matches VAR.  If so, re-use it, otherwise add a new
     VDEF for VAR.  */
  result = NULL_TREE;
  source = NULL_TREE;
  if (prev_vops)
    for (i = 0; i < NUM_VDEFS (&(prev_vops->vdef_ops)); i++)
      {
	result = VDEF_RESULT (&(prev_vops->vdef_ops), i);
	if (result == var
	    || (TREE_CODE (result) == SSA_NAME
		&& SSA_NAME_VAR (result) == var))
	  {
	    source = VDEF_OP (&(prev_vops->vdef_ops), i);
	    break;
	  }
      }

  /* If no previous VDEF operand was found for VAR, create one now.  */
  if (source == NULL_TREE)
    {
      result = var;
      source = var;
    }

  VARRAY_PUSH_TREE (build_vdefs, result);
  VARRAY_PUSH_TREE (build_vdefs, source);
}


/* Add VAR to the list of virtual uses for STMT.  If PREV_VOPS
   is not NULL, the existing entries are preserved and no new entries are
   added here.  This is done to preserve the SSA numbering of virtual
   operands.  */

static void
append_vuse (tree var, tree stmt, voperands_t prev_vops)
{
  stmt_ann_t ann;
  size_t i;
  bool found;
  tree vuse;

#ifdef ENABLE_CHECKING
  if (check_build_stmt != stmt)
    abort();
#endif

  ann = stmt_ann (stmt);

  /* Don't allow duplicate entries.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (build_vuses); i++)
    {
      tree vuse_var = VARRAY_TREE (build_vuses, i);
      if (var == vuse_var
	  || (TREE_CODE (vuse_var) == SSA_NAME
	      && var == SSA_NAME_VAR (vuse_var)))
	return;
    }

  /* If the statement already had virtual uses, see if any of the
     existing VUSEs matches VAR.  If so, re-use it, otherwise add a new
     VUSE for VAR.  */
  found = false;
  vuse = NULL_TREE;
  if (prev_vops)
    for (i = 0; i < NUM_VUSES (&(prev_vops->vuse_ops)); i++)
      {
	vuse = VUSE_OP (&(prev_vops->vuse_ops), i);
	if (vuse == var
	    || (TREE_CODE (vuse) == SSA_NAME
		&& SSA_NAME_VAR (vuse) == var))
	  {
	    found = true;
	    break;
	  }
      }

  /* If VAR existed already in PREV_VOPS, re-use it.  */
  if (found)
    var = vuse;

  VARRAY_PUSH_TREE (build_vuses, var);
}


/* External entry point which by-passes the previous vops mechanism.  */
void
add_vuse (tree var, tree stmt)
{
  append_vuse (var, stmt, NULL);
}



/* Return the ECF_ flags associated with the function called by the
   CALL_EXPR node EXPR.  */

static int
get_call_flags (tree expr)
{
  tree callee;

#if defined ENABLE_CHECKING
  if (TREE_CODE (expr) != CALL_EXPR)
    abort ();
#endif

  callee = get_callee_fndecl (expr);
  return (callee) ? flags_from_decl_or_type (callee) : 0;
}

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

  /* Ignore error statements.  */
  if (TREE_CODE (stmt) == ERROR_MARK)
    return;

  /* If the statement has not been modified, the operands are still valid.  */
  if (!stmt_modified_p (stmt))
    return;

  ann = get_stmt_ann (stmt);

  /* Non-GIMPLE statements are just marked unmodified.  */
  if (TREE_NOT_GIMPLE (stmt))
    {
      ann->modified = 0;
      return;
    }

  /* Remove any existing operands as they will be scanned again.  */
  if (ann->ops)
    {
      free_defs (&(ann->ops->def_ops));
      free_uses (&(ann->ops->use_ops));
    }

  /* Before removing existing virtual operands, save them in PREV_VOPS so 
     that we can re-use their SSA versions.  */
  if (ann->vops)
    prev_vops = ann->vops;

  ann->vops = NULL;

  start_ssa_stmt_operands (stmt);

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
      {
	int noutputs = list_length (ASM_OUTPUTS (stmt));
	const char **oconstraints
	  = (const char **) alloca ((noutputs) * sizeof (const char *));
	int i;
	tree link;
	const char *constraint;
	bool allows_mem, allows_reg, is_inout;

	for (i=0, link = ASM_OUTPUTS (stmt); link;
	     ++i, link = TREE_CHAIN (link))
	  {
	    oconstraints[i] = constraint
	      = TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));
	    parse_output_constraint (&constraint, i, 0, 0,
				     &allows_mem, &allows_reg, &is_inout);
	    if (allows_reg && is_inout)
	      /* This should have been split in gimplify_asm_expr.  */
	      abort ();
	    if (!allows_reg && allows_mem)
	      note_addressable (TREE_VALUE (link), ann);
	    get_expr_operands (stmt, &TREE_VALUE (link), opf_is_def,
			       prev_vops);
	  }
	for (link = ASM_INPUTS (stmt); link; link = TREE_CHAIN (link))
	  {
	    constraint
	      = TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));
	    parse_input_constraint (&constraint, 0, 0, noutputs, 0,
				    oconstraints, &allows_mem, &allows_reg);
	    if (!allows_reg && allows_mem)
	      note_addressable (TREE_VALUE (link), ann);
	    get_expr_operands (stmt, &TREE_VALUE (link), 0, prev_vops);
	  }
	for (link = ASM_CLOBBERS (stmt); link; link = TREE_CHAIN (link))
	  if (!strcmp (TREE_STRING_POINTER (TREE_VALUE (link)), "memory")
	      && num_call_clobbered_vars > 0)
	    add_call_clobber_ops (stmt, prev_vops);
      }
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
    case BIND_EXPR:
    case CASE_LABEL_EXPR:
    case TRY_CATCH_EXPR:
    case TRY_FINALLY_EXPR:
    case EH_FILTER_EXPR:
    case CATCH_EXPR:
    case RESX_EXPR:
      break;

    default:
      /* Notice that if get_expr_operands tries to use &STMT as the operand
	 pointer (which may only happen for USE operands), we will abort in
	 append_use.  This default will handle statements like empty statements,
	 CALL_EXPRs or VA_ARG_EXPRs that may appear on the RHS of a statement
	 or as statements themselves.  */
      get_expr_operands (stmt, &stmt, opf_none, prev_vops);
      break;
    }

  finalize_ssa_stmt_operands (stmt);

  /* Clear the modified bit for STMT.  Subsequent calls to
     get_stmt_operands for this statement will do nothing until the
     statement is marked modified by a call to modify_stmt().  */
  ann->modified = 0;
}


/* Recursively scan the expression pointed by EXPR_P in statement STMT.
   FLAGS is one of the OPF_* constants modifying how to interpret the
   operands found.  PREV_VOPS is as in append_vdef and append_vuse.  */

static void
get_expr_operands (tree stmt, tree *expr_p, int flags, voperands_t prev_vops)
{
  enum tree_code code;
  char class;
  tree expr = *expr_p;

  if (expr == NULL || expr == error_mark_node)
    return;

#if defined ENABLE_CHECKING
  /* non-GIMPLE expressions should not appear here.  */
  if (TREE_NOT_GIMPLE (expr))
    abort ();
#endif

  code = TREE_CODE (expr);
  class = TREE_CODE_CLASS (code);

  /* Expressions that make no memory references.  */
  if (class == 'c'
      || class == 't'
      || class == 'b'
      || code == FUNCTION_DECL
      || code == EXC_PTR_EXPR
      || code == FILTER_EXPR
      || code == LABEL_DECL)
    return;

  /* We could have the address of a component, array member, etc which
     has interesting variable references.  */
  if (code == ADDR_EXPR)
    {
      enum tree_code subcode = TREE_CODE (TREE_OPERAND (expr, 0));

      /* Taking the address of a variable does not represent a
	 reference to it, but the fact that STMT takes its address will be
	 of interest to some passes (e.g. must-alias resolution).  */
      add_stmt_operand (expr_p, stmt, 0, NULL);

      /* If the address is invariant, there may be no interesting variable
	 references inside.  */
      if (is_gimple_min_invariant (expr))
	return;

      /* There should be no VUSEs created, since the referenced objects are
	 not really accessed.  The only operands that we should find here
	 are ARRAY_REF indices which will always be real operands (GIMPLE
	 does not allow non-registers as array indices).  */
      flags |= opf_no_vops;

      /* Avoid recursion.  */
      code = subcode;
      class = TREE_CODE_CLASS (code);
      expr_p = &TREE_OPERAND (expr, 0);
      expr = *expr_p;
    }

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
      tree *pptr = &TREE_OPERAND (expr, 0);
      tree ptr = *pptr;

      if (SSA_VAR_P (ptr))
	{
	  if (TREE_CODE (ptr) == SSA_NAME)
	    ptr = SSA_NAME_VAR (ptr);
	  ann = var_ann (ptr);
	  if (ann->mem_tag)
	    add_stmt_operand (&ann->mem_tag, stmt, flags, prev_vops);
	}

      /* If a constant is used as a pointer, we can't generate a real
	 operand for it but we mark the statement volatile to prevent
	 optimizations from messing things up.  */
      else if (TREE_CODE (ptr) == INTEGER_CST)
	{
	  stmt_ann (stmt)->has_volatile_ops = true;
	  return;
	}

      /* Everything else should have been folded elsewhere.  */
      else
	abort ();

      /* Add a USE operand for the base pointer.  */
      get_expr_operands (stmt, pptr, opf_none, prev_vops);
      return;
    }

  /* Treat array references as references to the virtual variable
     representing the array.  The virtual variable for an ARRAY_REF
     is the VAR_DECL for the array.  */
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
      int call_flags = get_call_flags (expr);

      /* Find uses in the called function.  */
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), opf_none, prev_vops);

      for (op = TREE_OPERAND (expr, 1); op; op = TREE_CHAIN (op))
        get_expr_operands (stmt, &TREE_VALUE (op), opf_none, prev_vops);

      if (num_call_clobbered_vars > 0)
	{
	  if (!(call_flags & (ECF_CONST | ECF_PURE | ECF_NORETURN)))
	    add_call_clobber_ops (stmt, prev_vops);
	  else if (!(call_flags & (ECF_CONST | ECF_NORETURN)))
	    add_call_read_ops (stmt, prev_vops);
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
      add_stmt_operand (&TREE_OPERAND (expr, 0), stmt, opf_is_def, prev_vops);
      return;
    }

  /* Unary expressions.  */
  if (class == '1'
      || code == TRUTH_NOT_EXPR
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
   get_expr_operands.  If *VAR_P is a GIMPLE register, it will be added to
   the statement's real operands, otherwise it is added to virtual
   operands.

   PREV_VOPS is used when adding virtual operands to statements that
      already had them (See append_vdef and append_vuse).  */

static void
add_stmt_operand (tree *var_p, tree stmt, int flags, voperands_t prev_vops)
{
  bool is_real_op;
  tree var, sym;
  varray_type aliases;
  size_t i;
  stmt_ann_t s_ann;
  var_ann_t v_ann;

  var = *var_p;
  STRIP_NOPS (var);

  s_ann = stmt_ann (stmt);

  /* If the operand is an ADDR_EXPR, add its operand to the list of
     variables that have had their address taken in this statement.  */
  if (TREE_CODE (var) == ADDR_EXPR)
    {
      note_addressable (TREE_OPERAND (var, 0), s_ann);
      return;
    }

  /* If the original variable is not a scalar, it will be added to the list
     of virtual operands.  In that case, use its base symbol as the virtual
     variable representing it.  */
  is_real_op = is_gimple_reg (var);
  if (!is_real_op && !DECL_P (var))
    var = get_virtual_var (var);

  /* If VAR is not a variable that we care to optimize, do nothing.  */
  if (var == NULL_TREE || !SSA_VAR_P (var))
    return;

  sym = get_base_symbol (var);
  v_ann = var_ann (sym);

  /* FIXME: We currently refuse to optimize variables that have hidden uses
     (variables used in VLA declarations, MD builtin calls and variables
     from the parent function in nested functions).  This is because not
     all uses of these variables are exposed in the IL or the statements
     that reference them are not in GIMPLE form.  If that's the case, mark
     the statement as having volatile operands and return.  */
  if (v_ann->has_hidden_use)
    {
      s_ann->has_volatile_ops = 1;
      return;
    }

  if (is_real_op)
    {
      /* The variable is a GIMPLE register.  Add it to real operands.  */
      if (flags & opf_is_def)
	append_def (var_p, stmt);
      else
	append_use (var_p, stmt);
    }
  else
    {
      /* The variable is not a GIMPLE register.  Add it (or its aliases) to
	 virtual operands.  */
      if (flags & opf_no_vops)
	return;

      aliases = v_ann->may_aliases;
      if (aliases == NULL)
	{
	  /* The variable is not aliased or it is an alias tag.  */
	  if (flags & opf_is_def)
	    {
	      append_vdef (var, stmt, prev_vops);
	      if (v_ann->is_alias_tag)
		s_ann->makes_aliased_stores = 1;
	    }
	  else
	    {
	      append_vuse (var, stmt, prev_vops);
	      if (v_ann->is_alias_tag)
		s_ann->makes_aliased_loads = 1;
	    }
	}
      else
	{
	  /* The variable is aliased.  Add its aliases to the virtual
	     operands.  */
	  if (VARRAY_ACTIVE_SIZE (aliases) == 0)
	    abort ();

	  if (flags & opf_is_def)
	    {
	      /* If the variable is also an alias tag, add a virtual operand
		for it, otherwise we will miss representing references to the
		members of the variable's alias set.  This fixes the bug in
		gcc.c-torture/execute/20020503-1.c.  */
	      if (v_ann->is_alias_tag)
		append_vdef (var, stmt, prev_vops);

	      for (i = 0; i < VARRAY_ACTIVE_SIZE (aliases); i++)
		append_vdef (VARRAY_TREE (aliases, i), stmt, prev_vops);

	      s_ann->makes_aliased_stores = 1;
	    }
	  else
	    {
	      if (v_ann->is_alias_tag)
		append_vuse (var, stmt, prev_vops);

	      for (i = 0; i < VARRAY_ACTIVE_SIZE (aliases); i++)
		append_vuse (VARRAY_TREE (aliases, i), stmt, prev_vops);

	      s_ann->makes_aliased_loads = 1;
	    }
	}
    }
}

/* Record that VAR had its address taken in the statement with annotations
   S_ANN.  */

static void
note_addressable (tree var, stmt_ann_t s_ann)
{
  var = get_base_symbol (var);
  if (var && SSA_VAR_P (var))
    {
      if (s_ann->addresses_taken == NULL)
	VARRAY_TREE_INIT (s_ann->addresses_taken, 2, "addresses_taken");
      VARRAY_PUSH_TREE (s_ann->addresses_taken, var);
    }
}


/* Add clobbering definitions for .GLOBAL_VAR or for each of the call
   clobbered variables in the function.  */

static void
add_call_clobber_ops (tree stmt, voperands_t prev_vops)
{
  /* Functions that are not const, pure or never return may clobber
     call-clobbered variables.  */
  stmt_ann (stmt)->makes_clobbering_call = true;

  /* If we had created .GLOBAL_VAR earlier, use it.  Otherwise, add a VDEF
     operand for every call clobbered variable.  See add_referenced_var for
     the heuristic used to decide whether to create .GLOBAL_VAR or not.  */
  if (global_var)
    add_stmt_operand (&global_var, stmt, opf_is_def, prev_vops);
  else
    {
      size_t i;

      for (i = 0; i < num_call_clobbered_vars; i++)
	{
	  tree var = call_clobbered_var (i);

	  /* If VAR is read-only, don't add a VDEF, just a VUSE operand.  */
	  if (!TREE_READONLY (var))
	    add_stmt_operand (&var, stmt, opf_is_def, prev_vops);
	  else
	    add_stmt_operand (&var, stmt, opf_none, prev_vops);
	}
    }
}


/* Add VUSE operands for .GLOBAL_VAR or all call clobbered variables in the
   function.  */

static void
add_call_read_ops (tree stmt, voperands_t prev_vops)
{
  /* Otherwise, if the function is not pure, it may reference memory.  Add
     a VUSE for .GLOBAL_VAR if it has been created.  Otherwise, add a VUSE
     for each call-clobbered variable.  See add_referenced_var for the
     heuristic used to decide whether to create .GLOBAL_VAR.  */
  if (global_var)
    add_stmt_operand (&global_var, stmt, opf_none, prev_vops);
  else
    {
      size_t i;

      for (i = 0; i < num_call_clobbered_vars; i++)
	{
	  tree var = call_clobbered_var (i);
	  add_stmt_operand (&var, stmt, opf_none, prev_vops);
	}
    }
}

#include "gt-tree-ssa-operands.h"
