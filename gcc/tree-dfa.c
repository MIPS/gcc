/* Data flow functions for trees.
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "errors.h"
#include "expr.h"
#include "ggc.h"

/* This should be eventually be generalized to other languages, but
   this would require a shared function-as-trees infrastructure.  */
#include "c-common.h"
#include "c-tree.h"

#include "tree-optimize.h"
#include "tree-simple.h"
#include "tree-flow.h"
#include "tree-inline.h"

/* Local declarations.  */

/* Dump file and flags.  */
static FILE *dump_file;
static int dump_flags;

/* List of pointer references made in this function.  Used to insert
   V_MAY_DEF and V_MAY_USE references for variables that may have been
   aliased.  */
static ref_list pointer_refs;

struct clobber_data_d
{
  basic_block bb;
  tree parent_stmt;
  tree parent_expr;
};


/* Local functions.  */
static void find_refs_in_stmt PARAMS ((tree, basic_block));
static void find_refs_in_expr PARAMS ((tree *, HOST_WIDE_INT, basic_block, tree,
                                       tree));
static void add_referenced_var PARAMS ((tree));
static tree clobber_vars_r PARAMS ((tree *, int *, void *));
static void add_default_defs PARAMS ((void));
static void add_call_site_clobbers PARAMS ((void));
static void add_ptr_may_refs PARAMS ((void));


/* Global declarations.  */

/* Array of all variables referenced in the function.  */
unsigned long num_referenced_vars;
varray_type referenced_vars;

/* List of all call sites in the current function.  */
ref_list call_sites;

/* Next unique reference ID to be assigned by create_ref().  */
HOST_WIDE_INT next_tree_ref_id;

/* Reference types.  */
const HOST_WIDE_INT V_DEF	= 1 << 0;
const HOST_WIDE_INT V_USE	= 1 << 1;
const HOST_WIDE_INT V_PHI	= 1 << 2;
const HOST_WIDE_INT E_FCALL	= 1 << 3;
const HOST_WIDE_INT E_PHI	= 1 << 4;
const HOST_WIDE_INT E_USE	= 1 << 5;
const HOST_WIDE_INT E_KILL	= 1 << 6;

/* Reference type modifiers.  */
const HOST_WIDE_INT M_DEFAULT	= 1 << 8;
const HOST_WIDE_INT M_CLOBBER	= 1 << 9;
const HOST_WIDE_INT M_MAY	= 1 << 10;
const HOST_WIDE_INT M_PARTIAL	= 1 << 11;
const HOST_WIDE_INT M_INITIAL	= 1 << 12;
const HOST_WIDE_INT M_INDIRECT	= 1 << 13;
const HOST_WIDE_INT M_VOLATILE	= 1 << 14;
const HOST_WIDE_INT M_ADDRESSOF	= 1 << 15;


/* Look for variable references in every block of the flowgraph.  */

void
tree_find_refs ()
{
  basic_block bb;

  next_tree_ref_id = 0;
  call_sites = create_ref_list ();
  pointer_refs = create_ref_list ();

  FOR_EACH_BB (bb)
    {
      tree t = bb->head_tree;

      if (bb_empty_p (bb))
	continue;

      while (t)
	{
	  /* Some basic blocks are composed exclusively of expressions
	     (e.g., FOR_* and DO_COND nodes), these are handled when
	     find_refs_in_stmt processes their entry node.  */
	  if (statement_code_p (TREE_CODE (t)))
	    {
	      find_refs_in_stmt (t, bb);
	      if (t == bb->end_tree || is_ctrl_stmt (t))
		break;
	    }

	  t = TREE_CHAIN (t);
	}
    }

  add_default_defs ();
  add_call_site_clobbers ();
  add_ptr_may_refs ();

  /* Debugging dumps.  */
  dump_file = dump_begin (TDI_ssa, &dump_flags);
  if (dump_file)
    {
      if (dump_flags & TDF_REFS)
	dump_referenced_vars (dump_file);

      dump_end (TDI_ssa, dump_file);
    }

  delete_ref_list (pointer_refs);
}


/* Walk T looking for variable references.  BB is the basic block that
   contains T.  */

static void
find_refs_in_stmt (t, bb)
     tree t;
     basic_block bb;
{
  enum tree_code code;

  if (t == NULL || t == error_mark_node)
    return;

  code = TREE_CODE (t);
  switch (code)
    {
    case EXPR_STMT:
      find_refs_in_expr (&EXPR_STMT_EXPR (t), V_USE, bb, t, EXPR_STMT_EXPR (t));
      break;

    case IF_STMT:
      find_refs_in_expr (&IF_COND (t), V_USE, bb, t, IF_COND (t));
      break;
      
    case SWITCH_STMT:
      find_refs_in_expr (&SWITCH_COND (t), V_USE, bb, t, SWITCH_COND (t));
      break;

    case WHILE_STMT:
      find_refs_in_expr (&WHILE_COND (t), V_USE, bb, t, WHILE_COND (t));
      break;

    case FOR_STMT:
      /* Grr, the FOR_INIT_STMT node of a FOR_STMT is also a statement,
	 which is handled by the main loop in tree_find_tree_refs.  */
      if (for_cond_bb (bb))
	find_refs_in_expr (&FOR_COND (t), V_USE, for_cond_bb (bb), t,
			   FOR_COND (t));

      if (for_expr_bb (bb))
	find_refs_in_expr (&FOR_EXPR (t), V_USE, for_expr_bb (bb), t,
			   FOR_EXPR (t));
      break;

    case DO_STMT:
      if (do_cond_bb (bb))
	find_refs_in_expr (&DO_COND (t), V_USE, do_cond_bb (bb), t,
	                   DO_COND (t));
      break;

    case ASM_STMT:
      find_refs_in_expr (&ASM_INPUTS (t), V_USE, bb, t, ASM_INPUTS (t));
      find_refs_in_expr (&ASM_OUTPUTS (t), V_DEF | M_CLOBBER, bb, t,
	                 ASM_OUTPUTS (t));
      find_refs_in_expr (&ASM_CLOBBERS (t), V_DEF | M_CLOBBER, bb, t,
	                 ASM_CLOBBERS (t));
      break;

    case RETURN_STMT:
      find_refs_in_expr (&RETURN_STMT_EXPR (t), V_USE, bb, t, 
	                 RETURN_STMT_EXPR (t));
      break;

    case GOTO_STMT:
      find_refs_in_expr (&GOTO_DESTINATION (t), V_USE, bb, t,
	                 GOTO_DESTINATION (t));
      break;

    case DECL_STMT:
      if (TREE_CODE (DECL_STMT_DECL (t)) == VAR_DECL
	  && DECL_INITIAL (DECL_STMT_DECL (t)))
	{
	  find_refs_in_expr (&DECL_INITIAL (DECL_STMT_DECL (t)), V_USE, bb, t,
	                     DECL_INITIAL (DECL_STMT_DECL (t)));
	  find_refs_in_expr (&DECL_STMT_DECL (t), V_DEF | M_INITIAL, bb, t,
			     DECL_STMT_DECL (t));
	}
      break;

    /* FIXME  CLEANUP_STMTs are not simplified.  Clobber everything.  */
    case CLEANUP_STMT:
      walk_tree (&CLEANUP_DECL (t), clobber_vars_r, NULL, NULL);
      walk_tree (&CLEANUP_EXPR (t), clobber_vars_r, NULL, NULL);
      break;

    case LABEL_STMT:
      find_refs_in_expr (&LABEL_STMT_LABEL (t), V_USE, bb, t,
			 LABEL_STMT_LABEL (t));
      break;

    case STMT_EXPR:
      find_refs_in_stmt (STMT_EXPR_STMT (t), bb);
      break;

    case CONTINUE_STMT:
    case CASE_LABEL:
    case BREAK_STMT:
    case COMPOUND_STMT:
    case SCOPE_STMT:
    case FILE_STMT:
      break;				/* Nothing to do.  */

    default:
      {
	prep_stmt (t);
	error ("unhandled statement node in find_refs_in_stmt():");
	fprintf (stderr, "\n");
	tree_debug_bb (bb);
	fprintf (stderr, "\n");
	debug_tree (t);
	fprintf (stderr, "\n");
	abort ();
      }
    }
}


/* Recursively scan the expression tree pointed by EXPR_P looking for
   variable references.
   
   REF_TYPE indicates what type of reference should be created.

   BB, PARENT_STMT and PARENT_EXPR are the block, statement and expression
      trees containing *EXPR_P.
      
   NOTE: PARENT_EXPR is the root node of the expression tree hanging off of
      PARENT_STMT.  For instance, given the statement 'a = b + c;', the
      parent expression for all references inside the statement is the '='
      node.  */

static void
find_refs_in_expr (expr_p, ref_type, bb, parent_stmt, parent_expr)
     tree *expr_p;
     HOST_WIDE_INT ref_type;
     basic_block bb;
     tree parent_stmt;
     tree parent_expr;
{
  enum tree_code code;
  char class;
  tree expr = *expr_p;
  struct clobber_data_d clobber_data;

  if (expr == NULL || expr == error_mark_node)
    return;

  code = TREE_CODE (expr);
  class = TREE_CODE_CLASS (code);

  /* Expressions that make no memory references.  */
  if (class == 'c'
      || class == 't'
      || class == 'b'
      || code == RESULT_DECL
      || code == FUNCTION_DECL
      || code == LABEL_DECL)
    return;

  /* If this reference is associated with a non SIMPLE expression, then we
     mark the parent expression non SIMPLE and recursively clobber every
     variable referenced by PARENT_EXPR.  */
  if (parent_expr && tree_flags (expr) & TF_NOT_SIMPLE)
    {
      set_tree_flag (parent_expr, TF_NOT_SIMPLE);
      clobber_data.bb = bb;
      clobber_data.parent_expr = parent_expr;
      clobber_data.parent_stmt = parent_stmt;
      walk_tree (&parent_expr, clobber_vars_r, &clobber_data, NULL);
      return;
    }

  /* If we found a _DECL node, create a reference to it and return.  */
  if (code == VAR_DECL || code == PARM_DECL)
    {
      tree_ref ref = create_ref (expr, ref_type, bb, parent_stmt, parent_expr,
	                         expr_p, true);

      /* If this was an indirect reference, add the reference to the list of
	 pointer references so that we can create may-def/may-uses for aliased
	 variables after we're done here.  */
      if (ref_type & M_INDIRECT)
	add_ref_to_list_end (pointer_refs, ref);

      return;
    }

  /* Pointer dereferences.  Make an indirect reference to the pointer
     symbol.  After we finish creating all references, another pass will
     create may-def/may-uses for variables that this pointer might be
     pointing to.  */
  if (code == INDIRECT_REF)
    {
      /* Create a V_USE reference for the pointer itself.  Do this before
	 creating the indirect reference.  Otherwise, if the indirect
	 reference is a V_DEF, this use will be chained to it by the SSA
	 builder.  */
      find_refs_in_expr (&TREE_OPERAND (expr, 0), V_USE, bb, parent_stmt,
	                 parent_expr);

      /* Modify the incoming reference type to be an indirect reference, so
	 that the reference we create when we get to the pointer symbol can
	 be added to the list of pointer references.  */
      find_refs_in_expr (&TREE_OPERAND (expr, 0), ref_type | M_INDIRECT,
			 bb, parent_stmt, parent_expr);

      return;
    }

  /* For array references we default to treating accesses to individual
     elements of the array as if they were to the whole array.  Create a
     partial reference for the base symbol of the array (its LHS) and
     recurse into the RHS look for variables used as index expressions.

     FIXME Further analysis is needed to get proper array-SSA information.  */
  if (code == ARRAY_REF)
    {
      /* Change the reference type to a partial def/use when processing
	 the LHS of the reference.  */
      find_refs_in_expr (&TREE_OPERAND (expr, 0), ref_type | M_PARTIAL, bb,
	                 parent_stmt, parent_expr);

      /* References on the RHS of the array are always used as indices.  */
      find_refs_in_expr (&TREE_OPERAND (expr, 1), V_USE, bb, parent_stmt,
			 parent_expr);
      return;
    }

  /* Similarly to arrays, references to compound variables (complex types
     and structures/unions) are globbed.  Create a partial reference for
     the base symbol of the reference.

     FIXME This means that

     			a.x = 6;
			a.y = 7;
			foo (a.x, a.y);

	   will not be constant propagated because the two partial
	   definitions to 'a' will kill each other.  SSA needs to be
	   enhanced to deal with this case.  */
  if (code == IMAGPART_EXPR || code == REALPART_EXPR || code == COMPONENT_REF)
    {
      /* Modify the reference to be a partial reference of the LHS of the
	 expression.  */
      find_refs_in_expr (&TREE_OPERAND (expr, 0), ref_type | M_PARTIAL, bb,
	                 parent_stmt, parent_expr);
      return;
    }

  /* Assignments.  These are the only expressions that create V_DEF
     references besides DECL_STMTs.  */
  if (code == INIT_EXPR || code == MODIFY_EXPR)
    {
      find_refs_in_expr (&TREE_OPERAND (expr, 1), V_USE, bb, parent_stmt,
			 parent_expr);
      find_refs_in_expr (&TREE_OPERAND (expr, 0), V_DEF, bb, parent_stmt,
			 parent_expr);
      return;
    }

  /* Function calls.  Create a V_USE reference for every argument in the
     call.  Also add the reference to the CALL_EXPR to the list of function
     calls so that we can add may-defs/may-uses after all the references in
     the function have been found.  */
  if (code == CALL_EXPR)
    {
      tree_ref ref;

      /* Find references in the call address.  */
      find_refs_in_expr (&TREE_OPERAND (expr, 0), V_USE, bb, parent_stmt,
			 parent_expr);

      /* Find references in the argument list.  */
      find_refs_in_expr (&TREE_OPERAND (expr, 1), V_USE, bb, parent_stmt,
			 parent_expr);

      /* Create a reference to the call expression and add it to the list of
	 call sites.  */
      ref = create_ref (expr, E_FCALL, bb, parent_stmt, parent_expr, expr_p,
	                true);
      add_ref_to_list_end (call_sites, ref);

      return;
    }

  /* ADDR_EXPR nodes create an address-of use of their operand.  This means
     that the variable is not read, but its address is needed.  */
  if (code == ADDR_EXPR)
    {
      find_refs_in_expr (&TREE_OPERAND (expr, 0), V_USE|M_ADDRESSOF, bb,
			 parent_stmt, parent_expr);
      return;
    }

  /* Lists.  */
  if (code == TREE_LIST)
    {
      tree op;

      for (op = expr; op; op = TREE_CHAIN (op))
	find_refs_in_expr (&TREE_VALUE (op), ref_type, bb, parent_stmt,
	    parent_expr);
      return;
    }

  /* Unary expressions.  */
  if (class == '1'
      || code == EXPR_WITH_FILE_LOCATION
      || code == VA_ARG_EXPR)
    {
      find_refs_in_expr (&TREE_OPERAND (expr, 0), ref_type, bb, parent_stmt,
			 parent_expr);
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
      find_refs_in_expr (&TREE_OPERAND (expr, 0), ref_type, bb, parent_stmt,
			 parent_expr);
      find_refs_in_expr (&TREE_OPERAND (expr, 1), ref_type, bb, parent_stmt,
			 parent_expr);
      return;
    }

  /* If we get here, something has gone wrong.  */
  prep_stmt (parent_stmt);
  error ("unhandled expression in find_refs_in_expr():");
  debug_tree (expr);
  fputs ("\n", stderr);
  abort ();
}


/* Create and return an empty list of references.  */

ref_list
create_ref_list ()
{
  ref_list list = xmalloc (sizeof (struct ref_list_priv));
  list->first = list->last = NULL;
  return list;
}

/* Free the nodes in LIST, but keep the empty list around.  
   (i.e., empty the list).  */

void 
empty_ref_list (list)
     ref_list list;
{
  struct ref_list_node *node;

  if (list == NULL)
    return;

  for (node = list->first; node; )
    {
      struct ref_list_node *tmp;
      tmp = node;
      node = node->next;
      free (tmp);
    }
  list->first = list->last = NULL;
}


/* Delete LIST, including the list itself.
   (i.e., destroy the list).  */

void
delete_ref_list (list)
     ref_list list;
{
  struct ref_list_node *node;

  if (list == NULL)
    return;

  for (node = list->first; node; )
    {
      struct ref_list_node *tmp;
      tmp = node;
      node = node->next;
      free (tmp);
    }
  free (list);
}

/* Remove REF from LIST.  */

void 
remove_ref_from_list (list, ref)
     ref_list list;
     tree_ref ref;
{
  struct ref_list_node *tmp;

  if (!list || !list->first || !list->last)
    return;

  tmp = find_list_node (list, ref);

  if (tmp == list->first)
    list->first = tmp->next;
  if (tmp == list->last)
    list->last = tmp->prev;
  if (tmp->next)
    tmp->next->prev = tmp->prev;
  if (tmp->prev)
    tmp->prev->next = tmp->next;

  free (tmp);
  return;
}


/* Add REF to the beginning of LIST.  */

void
add_ref_to_list_begin (list, ref)
     ref_list list;
     tree_ref ref;
{
  struct ref_list_node *node = xmalloc (sizeof (struct ref_list_node));
  node->ref = ref;
  if (list->first == NULL)
    {
      node->prev = node->next = NULL;
      list->first = list->last = node;
      return;
    }
  node->prev = NULL;
  node->next = list->first;
  list->first->prev = node;
  list->first = node;
}


/* Add REF to the end of LIST.  */

void
add_ref_to_list_end (list, ref)
     ref_list list;
     tree_ref ref;
{
  struct ref_list_node *node = xmalloc (sizeof (struct ref_list_node));
  node->ref = ref;

  if (list->first == NULL)
    {
      node->prev = node->next = NULL;
      list->first = list->last = node;
      return;
    }
  node->prev = list->last;
  node->next = NULL;
  list->last->next = node;
  list->last = node;
}


/* Find the list container for reference REF in LIST.  */

struct ref_list_node *
find_list_node (list, ref)
     ref_list list;
     tree_ref ref;
{
  struct ref_list_node *node = NULL;

  if (ref == list->first->ref)
    node = list->first;
  else if (ref == list->last->ref)
    node = list->last;
  else
    {
      for (node = list->first; node; node = node->next)
	if (node->ref == ref)
	  break;
    }

  return node;
}


/* Add REF after NODE.  */

void
add_ref_to_list_after (list, node, ref)
     ref_list list;
     struct ref_list_node *node;
     tree_ref ref;
{
  if (node == list->last)
    add_ref_to_list_end (list, ref);
  else if (node == list->first)
    add_ref_to_list_begin (list, ref);
  else
    {
      struct ref_list_node *new = xmalloc (sizeof (struct ref_list_node));
      new->ref = ref;
      new->prev = node;
      new->next = node->next;
      node->next = new;
    }
}


/* Create references and associations to variables and basic blocks.  */

/* Create a new variable reference for variable VAR.

   REF_TYPE is the type of reference to create (V_DEF, V_USE, V_PHI, etc).
    
   BB, PARENT_STMT, PARENT_EXPR and OPERAND_P give the exact location of the
      reference.  PARENT_STMT, PARENT_EXPR and OPERAND_P can be NULL in the
      case of artificial references (PHI nodes, default definitions, etc).

   ADD_TO_BB should be true if the caller wants the reference to be added
      to the list of references for BB (i.e., bb_refs (BB)).  In that case,
      the reference is added at the end of the list.
      
      This is a problem for certain types of references like V_PHI or
      default defs that need to be added in specific places within the list
      of references for the BB.  If ADD_TO_BB is false, the caller is
      responsible for the placement of the newly created reference.  */

tree_ref
create_ref (var, ref_type, bb, parent_stmt, parent_expr, operand_p, add_to_bb)
     tree var;
     HOST_WIDE_INT ref_type;
     basic_block bb;
     tree parent_stmt;
     tree parent_expr;
     tree *operand_p;
     int add_to_bb;
{
  tree_ref ref;

#if defined ENABLE_CHECKING
  if (bb == NULL)
    abort ();

  if (ref_type & (V_DEF | V_USE | V_PHI)
      && TREE_CODE_CLASS (TREE_CODE (var)) != 'd')
    abort ();
#endif

  /* Set the M_VOLATILE modifier, if the reference is to a volatile
     variable.  */
  if (var && TREE_THIS_VOLATILE (var))
    ref_type |= M_VOLATILE;

  ref = (tree_ref) ggc_alloc (sizeof (*ref));
  memset ((void *) ref, 0, sizeof (*ref));

  ref->common.id = next_tree_ref_id++;
  ref->common.var = var;
  ref->common.type = ref_type;
  ref->common.stmt = parent_stmt;
  ref->common.bb = bb;
  ref->common.expr = parent_expr;
  ref->common.operand_p = operand_p;
  ref->common.orig_operand = (operand_p) ? *operand_p : NULL;

  /* Create containers according to the type of reference.  */
  if (ref_type & (V_DEF | V_PHI))
    {
      ref->vdef.imm_uses = create_ref_list ();
      ref->vdef.reached_uses = create_ref_list ();
      if (ref_type & V_PHI)
	{
	  unsigned num;
	  edge in;

	  /* Count the number of incoming edges.  */
	  for (in = bb->pred, num = 0; in; in = in->pred_next)
	    num++;

	  VARRAY_GENERIC_PTR_INIT (ref->vphi.phi_args, num, "phi_args");
	}
    }
  else if (ref_type & V_USE)
    ref->vuse.rdefs = create_ref_list ();
  else if (ref_type & E_PHI)
    {
      varray_type temp;
      VARRAY_GENERIC_PTR_INIT (temp, 
			       last_basic_block, "ephi_chain");
      set_exprphi_phi_args (ref, temp);
      set_exprphi_processed (ref, BITMAP_XMALLOC ());
      set_exprphi_downsafe (ref, true);
      set_exprphi_canbeavail (ref, true);
      set_exprphi_later (ref, true);
      set_exprphi_extraneous (ref, true);
    }

  if (var)
    {
      /* Add the variable to the list of variables referenced in this
	 function.  But only for actual variable defs or uses in the code.  */
      if ((ref_type & (V_DEF | V_USE))
	  && TREE_CODE_CLASS (TREE_CODE (var)) == 'd')
	add_referenced_var (var);

      /* Add this reference to the list of references for the variable.  */
      add_tree_ref (var, ref);

      /* Add this reference to the list of references for the containing
	 statement.  */
      if (parent_stmt)
	add_tree_ref (parent_stmt, ref);

      /* Ditto for the expression containing this reference.  NOTE: This is
	 only valid for unshared tree expressions, which are only
	 guaranteed in SIMPLE form.  */
      if (parent_expr)
	add_tree_ref (parent_expr, ref);
    }

  /* If requested, add this reference to the list of references for the basic
     block.  */
  if (add_to_bb)
    add_ref_to_list_end (bb_refs (bb), ref);

  /* If this is an unmodified V_DEF reference, then this reference
     represents the output of its parent expression (i.e., the reference is
     the LHS of an assignment expression).  In this case, tell the parent
     expression about it.

     This is useful for algorithms like constant propagation when
     evaluating expressions, the output reference for the expression is
     where the lattice value of the expression can be stored.  */
  if (ref_type == V_DEF)
    {
#if defined ENABLE_CHECKING
      if (TREE_CODE (parent_expr) != MODIFY_EXPR
	  && TREE_CODE (parent_expr) != INIT_EXPR)
	abort ();
#endif
      set_output_ref (parent_expr, ref);
    }


  return ref;
}


/* Add a new argument to PHI for definition DEF reaching in via edge E.  */

void
add_phi_arg (phi, def, e)
     tree_ref phi;
     tree_ref def;
     edge e;
{
  phi_node_arg arg;

  arg = (phi_node_arg) ggc_alloc (sizeof (*arg));
  memset ((void *) arg, 0, sizeof (*arg));

  arg->def = def;
  arg->e = e;

  VARRAY_PUSH_GENERIC_PTR (phi->vphi.phi_args, (PTR)arg);
}


/* Add a unique copy of variable VAR to the list of referenced variables.  */

static void
add_referenced_var (var)
     tree var;
{
  if (tree_flags (var) & TF_REFERENCED)
    return;

  /* The variable has not been referenced yet.  Mark it referenced and add it
     to the list.  */
  set_tree_flag (var, TF_REFERENCED);
  VARRAY_PUSH_TREE (referenced_vars, var);
  num_referenced_vars = VARRAY_ACTIVE_SIZE (referenced_vars);
}


/* Manage annotations.  */

/* Create a new annotation for tree T.  */

tree_ann
create_tree_ann (t)
     tree t;
{
  tree_ann ann = (tree_ann) ggc_alloc (sizeof (*ann));
  memset ((void *) ann, 0, sizeof (*ann));
  ann->refs = create_ref_list ();
  t->common.aux = (void *) ann;
  return ann;
}


/* Remove the annotation for tree T.  */

void
remove_tree_ann (t)
     tree t;
{
  tree_ann ann = (tree_ann)t->common.aux;

  if (ann == NULL)
    return;

  ann->bb = NULL;
  delete_ref_list (ann->refs);
  ann->currdef = NULL;
  ann->compound_parent = NULL;
  ann->output_ref = NULL;
  t->common.aux = NULL;
}


/* Miscellaneous helpers.  */

/* Return 1 if the function may call itself.
   
   FIXME Currently this is very limited because we do not have call-graph
	 information.  */

int
function_may_recurse_p ()
{
  basic_block bb;

  /* If we only make calls to pure and/or builtin functions, then the
     function is not recursive.  */
  FOR_EACH_BB (bb)
    {
      struct ref_list_node *tmp;
      tree_ref ref;

      FOR_EACH_REF (ref, tmp, bb_refs (bb))
	{
	  tree fcall = ref_var (ref);

	  if (fcall == current_function_decl
	      || (TREE_CODE (fcall) == FUNCTION_DECL
		  && ! DECL_IS_PURE (fcall)
		  && ! DECL_BUILT_IN (fcall)))
	    return 1;
	}
    }

  return 0;
}


/*  Push into variable array *FCALLS_P all the function call references made
    in this function.

    WHICH is a bitmask specifying the type of function call that the caller
      is interested in (see tree-flow.h).  */

void
get_fcalls (fcalls_p, which)
     varray_type *fcalls_p;
     unsigned which;
{
  basic_block bb;

#if defined ENABLE_CHECKING
  if (fcalls_p == NULL || *fcalls_p == NULL)
    abort ();
#endif

  FOR_EACH_BB (bb)
    {
      struct ref_list_node *tmp;
      tree_ref ref;
      FOR_EACH_REF (ref, tmp, bb_refs (bb))
	{
	  tree fcall = ref_var (ref);

	  if (TREE_CODE (fcall) == FUNCTION_DECL)
	    {
	      if ((which & FCALL_NON_PURE)
		  && ! DECL_IS_PURE (fcall)
		  && ! DECL_BUILT_IN (fcall))
		VARRAY_PUSH_GENERIC_PTR (*fcalls_p, ref);
	      
	      else if ((which & FCALL_PURE) && DECL_IS_PURE (fcall))
		VARRAY_PUSH_GENERIC_PTR (*fcalls_p, ref);

	      else if ((which & FCALL_BUILT_IN) && DECL_BUILT_IN (fcall))
		VARRAY_PUSH_GENERIC_PTR (*fcalls_p, ref);
	    }
	}
    }
}


/* Return true if FCALL is a CALL_EXPR node to a pure, built-in function or
   const function.  */

bool
is_pure_fcall (fcall)
     tree fcall;
{
  tree fn = get_callee_fndecl (fcall);

  return (fn
	  && (DECL_IS_PURE (fn)
	      || DECL_BUILT_IN (fn)
	      || (TREE_READONLY (fn) && ! TREE_THIS_VOLATILE (fn))));
}


/* Return true if FCALL receives arguments by reference (i.e. pointers).  */

bool
fcall_takes_ref_args (fcall)
     tree fcall;
{
  tree op;

  if (TREE_CODE (fcall) != CALL_EXPR)
    abort ();

  for (op = TREE_OPERAND (fcall, 1); op; op = TREE_CHAIN (op))
    if (POINTER_TYPE_P (TREE_TYPE (TREE_VALUE (op))))
      return true;

  return false;
}


/* Return the basic block containing the statement that declares DECL.  */

basic_block
find_declaration (decl)
     tree decl;
{
  basic_block bb;
  tree t;
  tree_ref first_ref;

  /* Start with the first reference of DECL and walk the flowgraph
     backwards looking for a node with the scope block declaring the
     original variable.  */
  if (!tree_refs (decl) || !tree_refs (decl)->first)
    return NULL;
  first_ref = tree_refs (decl)->first->ref;
  t = ref_stmt (first_ref);
  FOR_BB_BETWEEN (bb, bb_for_stmt (t), ENTRY_BLOCK_PTR->next_bb, prev_bb)
    {
      if (TREE_CODE (bb->head_tree) == SCOPE_STMT
	  && SCOPE_STMT_BLOCK (bb->head_tree))
	{
	  tree block = SCOPE_STMT_BLOCK (bb->head_tree);
	  tree var;

	  for (var = BLOCK_VARS (block); var; var = TREE_CHAIN (var))
	    if (var == decl)
	      return bb;
	}
    }

  return NULL;
}



/* Debugging functions.  */

/*  Display variable reference REF on stream OUTF. PREFIX is a string that
    is prefixed to every line of output, and INDENT is the amount of left
    margin to leave.  If DETAILS is nonzero, the output is more verbose.  */

void
dump_ref (outf, prefix, ref, indent, details)
     FILE *outf;
     const char *prefix;
     tree_ref ref;
     int indent;
     int details;
{
  int lineno, bbix;
  const char *type;
  char *s_indent;

  if (ref == NULL)
    return;

  s_indent = (char *) alloca ((size_t) indent + 1);
  memset ((void *) s_indent, ' ', (size_t) indent);
  s_indent[indent] = '\0';

  lineno = (ref_stmt (ref)) ? STMT_LINENO (ref_stmt (ref)) : -1;

  bbix = (ref_bb (ref)) ? ref_bb (ref)->index : -1;

  type = ref_type_name (ref_type (ref));

  fprintf (outf, "%s%s%s(", s_indent, prefix, type);

  if (ref_var (ref))
    print_c_node (outf, ref_var (ref));
  else
    fprintf (outf, "nil");

  fprintf (outf, "): line %d, bb %d, id ", lineno, bbix);
  fprintf (outf, HOST_WIDE_INT_PRINT_DEC, ref_id (ref));
  fprintf (outf, ", ");

  if (ref_expr (ref))
    print_c_node (outf, ref_expr (ref));
  else
    fprintf (outf, "<nil>");

  /* Dump specific contents for the different types of references.  */
  if (details)
    {
      if (ref_type (ref) & V_PHI)
	{
	  if (phi_args (ref))
	    {
	      fputs (" phi-args:\n", outf);
	      dump_phi_args (outf, prefix, phi_args (ref), indent + 4, 0);
	    }

	  if (imm_uses (ref))
	    {
	      fputs ("    immediate uses:\n", outf);
	      dump_ref_list (outf, prefix, imm_uses (ref), indent + 4, 0);
	    }
	}

      if ((ref_type (ref) & E_PHI) && exprphi_phi_args (ref))
	{
	  if (details)
	    fprintf (outf, " class:%d downsafe:%d can_be_avail:%d later:%d\n", 
		     exprref_class (ref), exprphi_downsafe (ref), 
		     exprphi_canbeavail (ref), exprphi_later (ref));
	  fputs (" expr-phi-args:\n", outf);
	  dump_ref_array (outf, prefix, exprphi_phi_args (ref), indent + 4, 1);
	}	

      if ((ref_type (ref) & V_DEF) && imm_uses (ref))
	{
	  fputs (" immediate uses:\n", outf);
	  dump_ref_list (outf, prefix, imm_uses (ref), indent + 4, 0);
	}

      if (ref_type (ref) & V_USE && imm_reaching_def (ref))
	{
	  fputs (" immediate reaching def:\n", outf);
	  dump_ref (outf, prefix, imm_reaching_def (ref), indent + 4, 0);
	}	  

      if ((ref_type (ref) & E_USE) && expruse_phiop (ref) == true)
	{
	  char *temp_indent;
	  fprintf (outf, " class:%d has_real_use:%d  operand defined by:\n", 
		   exprref_class (ref), expruse_has_real_use (ref));	  
	  temp_indent = (char *) alloca ((size_t) indent + 4 + 1);
	  memset ((void *) temp_indent, ' ', (size_t) indent + 4);
	  temp_indent[indent + 4] = '\0';
	  if (expruse_def (ref) == NULL)
	    fprintf (outf, "%snothing\n", temp_indent);
	  else
	    dump_ref (outf, prefix, expruse_def (ref), indent + 4, 0);
	}
    }

  fputc ('\n', outf);
}


/*  Display variable reference REF on stderr.  */

void
debug_ref (ref)
     tree_ref ref;
{
  dump_ref (stderr, "", ref, 0, 1);
}


/*  Display a list of variable references on stream OUTF. PREFIX is a
    string that is prefixed to every line of output, and INDENT is the
    amount of left margin to leave.  If DETAILS is nonzero, the output is
    more verbose.  */

void
dump_ref_list (outf, prefix, reflist, indent, details)
     FILE *outf;
     const char *prefix;
     ref_list reflist;
     int indent;
     int details;
{
  struct ref_list_node *tmp;
  tree_ref ref;

  if (reflist == NULL)
    return;

  FOR_EACH_REF (ref, tmp, reflist)
    dump_ref (outf, prefix, ref, indent, details);
}



/*  Display a list of variable references on stream OUTF. PREFIX is a
    string that is prefixed to every line of output, and INDENT is the
    amount of left margin to leave.  If DETAILS is nonzero, the output is
    more verbose.  */

void
dump_ref_array (outf, prefix, reflist, indent, details)
     FILE *outf;
     const char *prefix;
     varray_type reflist;
     int indent;
     int details;
{
  size_t i;

  if (reflist == NULL)
    return;

  for (i = 0; i < VARRAY_SIZE (reflist); i++)
    if (VARRAY_GENERIC_PTR (reflist, i))
      dump_ref (outf, prefix, VARRAY_GENERIC_PTR (reflist, i), 
		   indent, details);
}

/*  Dump REFLIST on stderr.  */

void
debug_ref_list (reflist)
     ref_list reflist;
{
  dump_ref_list (stderr, "", reflist, 0, 1);
}


/* Dump REFLIST on stderr.  */

void
debug_ref_array (reflist)
    varray_type reflist;
{
  dump_ref_array (stderr, "", reflist, 0, 1);
}


/* Dump the list of all the referenced variables in the current function.  */

void
dump_referenced_vars (file)
     FILE *file;
{
  size_t i;

  fprintf (file, "\n;; Function %s: %lu referenced variables\n\n", 
	   IDENTIFIER_POINTER (DECL_NAME (current_function_decl)),
	   num_referenced_vars);

  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      print_node_brief (file, "", var, 0);
      fprintf (file, " (");
      print_c_node (file, var);
      fprintf (file, " )\n");
      dump_ref_list (file, "", tree_refs (var), 4, 1);
      fputc ('\n', file);
    }
}


/* Dump the list of all the referenced variables to stderr.  */

void
debug_referenced_vars ()
{
  dump_referenced_vars (stderr);
}


/* Dump the given array of phi arguments on stderr.  */

void
debug_phi_args (args)
     varray_type args;
{
  dump_phi_args (stderr, "", args, 0, 0);
}


/* Display the given array of PHI arguments definitions on stream OUTF.  PREFIX
   is a string that is prefixed to every line of output, and INDENT is the
   amount of left margin to leave.  If DETAILS is nonzero, the output is more
   verbose.  */

void
dump_phi_args (outf, prefix, args, indent, details)
     FILE *outf;
     const char *prefix;
     varray_type args;
     int indent;
     int details;
{
  size_t i;

  if (args == NULL)
    return;

  for (i = 0; i < VARRAY_SIZE (args); i++)
    dump_ref (outf, prefix, phi_arg_def (VARRAY_GENERIC_PTR (args, i)), indent,
	      details);
}


/* Return the reference type as a string.  */

const char *
ref_type_name (type)
     HOST_WIDE_INT type;
{
#define max 80
  static char str[max];

#if defined ENABLE_CHECKING
  if (!validate_ref_type (type))
    abort ();
#endif

  strncpy (str, type & V_DEF ? "V_DEF"
	        : type & V_USE ? "V_USE"
	        : type & V_PHI ? "V_PHI"
	        : type & E_FCALL ? "E_FCALL"
	        : type & E_PHI ? "E_PHI"
	        : type & E_USE ? "E_USE"
	        : type & E_KILL ? "E_KILL"
	        : "???",
	   max);

  if (type & M_DEFAULT)
    strncat (str, "/default", max - strlen (str));

  if (type & M_MAY)
    strncat (str, "/may", max - strlen (str));

  if (type & M_PARTIAL)
    strncat (str, "/partial", max - strlen (str));

  if (type & M_CLOBBER)
    strncat (str, "/clobber", max - strlen (str));

  if (type & M_INITIAL)
    strncat (str, "/initial", max - strlen (str));

  if (type & M_INDIRECT)
    strncat (str, "/indirect", max - strlen (str));

  if (type & M_VOLATILE)
    strncat (str, "/volatile", max - strlen (str));

  if (type & M_ADDRESSOF)
    strncat (str, "/addressof", max - strlen (str));

  return str;
}


/* Return true if TYPE is a valid reference type.  */

bool
validate_ref_type (type)
     HOST_WIDE_INT type;
{
  type &= ~M_VOLATILE;

  if (type & V_DEF)
    {
      type &= ~(M_DEFAULT | M_MAY | M_PARTIAL | M_CLOBBER | M_INITIAL
	        | M_INDIRECT);
      return type == V_DEF;
    }
  else if (type & V_USE)
    {
      type &= ~(M_DEFAULT | M_MAY | M_PARTIAL | M_INDIRECT | M_ADDRESSOF);
      return type == V_USE;
    }
  else if (type & V_PHI)
    {
      return type == V_PHI;
    }
  else if (type & E_FCALL)
    {
      return type == E_FCALL;
    }
  else if (type & E_PHI)
    {
      return type == E_PHI;
    }
  else if (type & E_USE)
    {
      return type == E_USE;
    }
  else if (type & E_KILL)
    {
      return type == E_KILL;
    }

  return false;
}

/* Callback for walk_tree.  Create a may-def/may-use reference for every _DECL
   and compound reference found under *TP.  */

static tree
clobber_vars_r (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees ATTRIBUTE_UNUSED;
     void *data;
{
  enum tree_code code = TREE_CODE (*tp);
  struct clobber_data_d *clobber = (struct clobber_data_d *)data;

  /* Create may-use and clobber references for every *_DECL in sight.  */
  if (code == VAR_DECL || code == PARM_DECL)
    {
      create_ref (*tp, V_USE | M_MAY, clobber->bb, clobber->parent_stmt,
		  clobber->parent_expr, NULL, true);
      create_ref (*tp, V_DEF | M_CLOBBER, clobber->bb, clobber->parent_stmt,
		  clobber->parent_expr, NULL, true);
    }

  return NULL;
}


/* Insert default definitions for all the variables referenced in the function.
   This allows the identification of variables that have been used without a
   preceding definition.  It also allows for default values to be assumed by
   transformations like constant propagation.  */

static void
add_default_defs ()
{
  unsigned int i;
  basic_block start = ENTRY_BLOCK_PTR->succ->dest;
  ref_list start_refs = bb_refs (start);

  for (i = 0; i < num_referenced_vars; i++)
    {
      tree_ref ref = create_ref (referenced_var (i), V_DEF | M_DEFAULT,
				 start, NULL, NULL, NULL, false);
      add_ref_to_list_begin (start_refs, ref);
    }
}


/* Insert may-use/clobber references at every call site for every variable
   that the called function might have access to.  */

static void
add_call_site_clobbers ()
{
  tree_ref ref;
  struct ref_list_node *tmp;

  FOR_EACH_REF (ref, tmp, call_sites)
    {
      unsigned int i;
      bool add_addressable;
      basic_block bb = ref_bb (ref);
      tree stmt = ref_stmt (ref);
      tree expr = ref_expr (ref);
      tree fcall = ref_var (ref);
      ref_list refs = bb_refs (bb);

      /* Skip pure and built-in functions.  */
      if (is_pure_fcall (fcall))
	continue;

      /* If the function receives arguments by reference, we must also
	 clobber any variables that might have had their address taken.  */
      add_addressable = fcall_takes_ref_args (fcall);

      for (i = 0; i < num_referenced_vars; i++)
	{
	  tree sym = referenced_var (i);

	  /* Skip artificial variables.  */
	  if (DECL_ARTIFICIAL (sym))
	    continue;

	  /* Add global _DECLs and addressable _DECLs (only if the function
	     receives pointer arguments).  */
	  if ((add_addressable && TREE_ADDRESSABLE (sym))
	      || decl_function_context (sym) == NULL)
	    {
	      struct ref_list_node *call_ref_node;
	      tree_ref clobber, may_use;
	      
	      clobber = create_ref (sym, V_DEF | M_CLOBBER, bb, stmt, expr,
				    NULL, false);
	      may_use = create_ref (sym, V_USE | M_MAY, bb, stmt, expr, NULL,
		                    false);

	      /* Notice that we first add the clobber and then the may-use
		 reference so that the may-use appears first.  This is so
		 that any prior definitions of SYM may reach the call site
		 and any subsequent uses of SYM are reached by the clobber.  */
	      call_ref_node = find_list_node (refs, ref);
	      add_ref_to_list_after (refs, call_ref_node, clobber);
	      add_ref_to_list_after (refs, call_ref_node, may_use);
	    }
	}
    }
}


/* Insert may-def/may-use references for aliased variables at every pointer
   reference site.  */

static void
add_ptr_may_refs ()
{
  struct ref_list_node *tmp;
  tree_ref ref;

  FOR_EACH_REF (ref, tmp, pointer_refs)
    {
      unsigned int i;
      tree ptr_sym = ref_var (ref);
      HOST_WIDE_INT type = ref_type (ref);
      basic_block bb = ref_bb (ref);
      tree stmt = ref_stmt (ref);
      tree expr = ref_expr (ref);
      ref_list refs = bb_refs (bb);
      struct ref_list_node *ptr_ref_node;
      HOST_WIDE_INT sym_alias_set, ptr_sym_alias_set;

      /* The may-refs we are going to introduce are not indirect.  */
      type &= ~M_INDIRECT;

      ptr_sym_alias_set = get_alias_set (TREE_TYPE (TREE_TYPE (ptr_sym)));

      for (i = 0; i < num_referenced_vars; i++)
	{
	  tree sym = referenced_var (i);

	  /* Skip artificial variables.  */
	  if (DECL_ARTIFICIAL (sym))
	    continue;

	  sym_alias_set = get_alias_set (TREE_TYPE (sym));

	  /* Only add variables that have had their address taken and have
	     conflicting alias sets with the pointer reference.  */
	  if (TREE_ADDRESSABLE (sym)
	      && alias_sets_conflict_p (sym_alias_set, ptr_sym_alias_set))
	    {
	      tree_ref may_ref;
	      
	      may_ref = create_ref (sym, type | M_MAY, bb, stmt, expr, NULL,
		                    false);
	      ptr_ref_node = find_list_node (refs, ref);
	      add_ref_to_list_after (refs, ptr_ref_node, may_ref);
	    }
	}
    }
}
