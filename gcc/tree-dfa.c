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

/* Local declarations.  */
struct clobber_data_d
{
  basic_block bb;
  tree *parent_stmt_p;
};

struct replace_data_d
{
  tree old;
  tree new;
};

/** @brief Gathers DFA Statistics.  */
struct dfa_stats_d
{
  unsigned long num_tree_refs;
  unsigned long size_tree_refs;
  unsigned long num_tree_anns;
  unsigned long num_ref_list_nodes;
  unsigned long num_defs;
  unsigned long num_uses;
  unsigned long num_phis;
  unsigned long num_phi_args;
  unsigned long max_num_phi_args;
  unsigned long num_ephis;
  unsigned long num_euses;
  unsigned long num_ekills;
};

static tree *alias_leaders;
static unsigned long num_alias_leaders;

/* Data and functions shared with tree-ssa.c.  */
struct dfa_counts_d dfa_counts;
extern FILE *tree_ssa_dump_file;
extern int tree_ssa_dump_flags;


/* Local functions.  */
static void find_refs_in_expr		PARAMS ((tree *, enum tree_ref_type,
      						 unsigned, basic_block,
						 tree *));
static void add_referenced_var		PARAMS ((tree));
static void dump_if_different		PARAMS ((FILE *, const char * const,
     						 unsigned long, unsigned long));
static void collect_dfa_stats		PARAMS ((struct dfa_stats_d *));
static tree collect_dfa_stats_r		PARAMS ((tree *, int *, void *));
static void count_tree_refs		PARAMS ((struct dfa_stats_d *,
      						 ref_list));
static void count_ref_list_nodes	PARAMS ((struct dfa_stats_d *,
      						 ref_list));
static tree clobber_vars_r		PARAMS ((tree *, int *, void *));
static void compute_may_aliases		PARAMS ((void));
static void find_may_aliases_for	PARAMS ((tree));
static tree find_alias_leader		PARAMS ((tree));
static bool may_access_global_mem 	PARAMS ((tree));
static size_t tree_ref_size		PARAMS ((enum tree_ref_type));
static tree replace_ref_r		PARAMS ((tree *, int *, void *));
static void remove_def			PARAMS ((tree_ref));
static void reset_def_def_links		PARAMS ((ref_list, tree_ref, tree_ref));
static void replace_phi_arg_with	PARAMS ((tree_ref, tree_ref, tree_ref));


/* Global declarations.  */

/* Array of all variables referenced in the function.  */
unsigned long num_referenced_vars;
varray_type referenced_vars;

/* Next unique reference ID to be assigned by create_ref().  */
unsigned long next_tree_ref_id;

/* Artificial variable used to model the effects of function calls on
   every variable that they may use and define.  Calls to non-const and
   non-pure functions are assumed to use and clobber this variable.  The
   SSA builder will then consider this variable to be an alias for every
   global variable and every local that has had its address taken.  */
tree global_var;

/* Tree reference modifier bitmasks.  Used when calling create_ref.  */
const unsigned TRM_CLOBBER	= 1 << 0;
const unsigned TRM_MAY		= 1 << 1;
const unsigned TRM_PARTIAL	= 1 << 2;
const unsigned TRM_INITIAL	= 1 << 3;
const unsigned TRM_VOLATILE	= 1 << 4;
const unsigned TRM_RELOCATE	= 1 << 5;
const unsigned TRM_ADDRESSOF	= 1 << 6;


/** @brief Look for variable references in every block of the flowgraph.  */

void
find_tree_refs ()
{
  basic_block bb;

  /* Traverse every block in the function looking for variable references.  */
  FOR_EACH_BB (bb)
    {
      gimple_stmt_iterator i;

      for (i = gsi_start_bb (bb); !gsi_end_bb (i); gsi_step_bb (&i))
	find_refs_in_stmt (gsi_stmt_ptr (i), bb);
    }

  compute_may_aliases ();
}


/** @brief Walk STMT looking for variable references.
    @param stmt_p is a pointer to the statement STMT.
    @param bb is the basic block that contains STMT.  */

void
find_refs_in_stmt (stmt_p, bb)
     tree *stmt_p;
     basic_block bb;
{
  enum tree_code code;
  tree stmt = *stmt_p;

  if (stmt == error_mark_node || stmt == empty_stmt_node)
    return;

  STRIP_WFL (stmt);
  STRIP_NOPS (stmt);
  code = TREE_CODE (stmt);
  switch (code)
    {
    case COND_EXPR:
      find_refs_in_expr (&COND_EXPR_COND (stmt), V_USE, 0, bb, stmt_p);
      break;

    case SWITCH_EXPR:
      find_refs_in_expr (&SWITCH_COND (stmt), V_USE, 0, bb, stmt_p);
      break;

    case ASM_EXPR:
      find_refs_in_expr (&ASM_INPUTS (stmt), V_USE, 0, bb, stmt_p);
      find_refs_in_expr (&ASM_OUTPUTS (stmt), V_DEF, TRM_CLOBBER, bb, stmt_p);
      find_refs_in_expr (&ASM_CLOBBERS (stmt), V_DEF, TRM_CLOBBER, bb, stmt_p);
      break;

    case RETURN_EXPR:
      find_refs_in_expr (&TREE_OPERAND (stmt, 0), V_USE, 0, bb, stmt_p);
      break;

    case GOTO_EXPR:
      find_refs_in_expr (&GOTO_DESTINATION (stmt), V_USE, 0, bb, stmt_p);
      break;

    case LABEL_EXPR:
      find_refs_in_expr (&LABEL_EXPR_LABEL (stmt), V_USE, 0, bb, stmt_p);
      break;

      /* These nodes contain no variable references.  */
    case LOOP_EXPR:
    case BIND_EXPR:
    case CASE_LABEL_EXPR:
      break;

    default:
      find_refs_in_expr (stmt_p, V_USE, 0, bb, stmt_p);
    }
}


/** @brief Recursively scan an expression tree looking for
	   variable references.
    @param expr_p points to the expression that should be scanned.
    @param ref_type indicates what type of reference should be created.
    @param ref_mod is the set of modifier flags for REF_TYPE.
    @param bb is the basic block in which EXPR_P is located.
    @param parent_stmt_p is the parent statement in BB for EXPR_P.  **/

static void
find_refs_in_expr (expr_p, ref_type, ref_mod, bb, parent_stmt_p)
     tree *expr_p;
     enum tree_ref_type ref_type;
     unsigned ref_mod;
     basic_block bb;
     tree *parent_stmt_p;
{
  enum tree_code code;
  char class;
  tree expr = *expr_p;
  tree parent_stmt = *parent_stmt_p;

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
     mark the statement non GIMPLE and recursively clobber every
     variable referenced by PARENT_STMT.  FIXME  TREE_NOT_GIMPLE must die.  */
  if (parent_stmt && TREE_NOT_GIMPLE (expr))
    {
      struct clobber_data_d clobber_data;

      mark_not_simple (parent_stmt_p);
      clobber_data.bb = bb;
      clobber_data.parent_stmt_p = parent_stmt_p;
      walk_tree (parent_stmt_p, clobber_vars_r, &clobber_data, NULL);
      return;
    }

  /* If the parent statement is marked not-gimple, don't do anything.  This
     means that in a previous iteration we encountered a non-gimple
     sub-expression which already clobbered all the variables in the
     statement.  FIXME: TREE_NOT_GIMPLE must die.  */
  if (parent_stmt && TREE_NOT_GIMPLE (parent_stmt))
    return;

  STRIP_WFL (parent_stmt);
  STRIP_NOPS (parent_stmt);

  /* If we found a _DECL node, create a reference to it and return.  */
  if (code == VAR_DECL || code == PARM_DECL)
    {
      create_ref (expr, ref_type, ref_mod, bb, parent_stmt_p, 1);

      /* If we just created a V_DEF reference for a pointer variable 'p',
	 we have to clobber the associated '*p' variable, because now 'p'
	 is pointing to a different memory location.  */
      if (ref_type == V_DEF
	  && POINTER_TYPE_P (TREE_TYPE (expr)))
	{
	  if (indirect_var (expr) == NULL)
	    set_indirect_var (expr, create_indirect_ref (expr));

	  create_ref (indirect_var (expr), V_DEF, TRM_RELOCATE, bb,
		      parent_stmt_p, 1);

	  /* If the relocation of 'p' is due to a function call, an
	     assignment from a global or an assignment from an argument,
	     then mark '*p' as an alias for global memory.  */
	  if (TREE_CODE (parent_stmt) == MODIFY_EXPR
	      && may_access_global_mem (TREE_OPERAND (parent_stmt, 1)))
	    set_tree_flag (indirect_var (expr), TF_MAY_ALIAS_GLOBAL_MEM);
	}

      return;
    }

  /* Pointer dereferences are considered references to the virtual variable
     represented by the INDIRECT_REF node (called INDIRECT_VAR).  Since
     INDIRECT_REF trees are not always shared like VAR_DECL nodes, this
     pass will think that two INDIRECT_REFs to the same _DECL are different
     references.  For instance,

	    1	p = &a;
	    2	*p = 5;
	    3	b = *p + 3;

     The INDIRECT_REF(p) at line 3 and the one at line 2 are two different
     tree nodes in the intermediate representation of the program.
     So, create_ref() will create two tree_ref objects that point to two
     different variables.  Therefore, the SSA pass will never link the use
     of *p at line 3 with the definition of *p at line 2.

     One way of fixing this problem would be to create the references with
     the pointed-to variable.  However, this is also wrong because now the
     SSA pass will think that the definition of *p at line 2 is killing the
     definition of p at line 1.

     The way we deal with this situation is to store in the VAR_DECL node
     for 'p' a pointer to the first INDIRECT_REF(p) node that we find.
     The first time we call create_ref with INDIRECT_REF(p), we use the
     given INDIRECT_REF tree as the variable associated with the reference.
     Subsequent calls to create_ref with INDIRECT_REF(p) nodes, will use
     the first INDIRECT_REF node found.  */
  if (code == INDIRECT_REF)
    {
      tree ptr = TREE_OPERAND (expr, 0);
      tree ptr_sym = get_base_symbol (ptr);

      /* Create a V_USE reference for the pointer variable itself.  */
      find_refs_in_expr (&TREE_OPERAND (expr, 0), V_USE, 0, bb, parent_stmt_p);

      /* If this is the first INDIRECT_REF node we find for PTR, set EXPR
	 to be the indirect variable used to represent all dereferences of
	 PTR.  */
      if (indirect_var (ptr_sym) == NULL)
	set_indirect_var (ptr_sym, expr);

      create_ref (indirect_var (ptr_sym), ref_type, ref_mod, bb, parent_stmt_p,
		  1);

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
      find_refs_in_expr (&TREE_OPERAND (expr, 0), ref_type,
	                 ref_mod | TRM_PARTIAL, bb, parent_stmt_p);

      /* References on the RHS of the array are always used as indices.  */
      find_refs_in_expr (&TREE_OPERAND (expr, 1), V_USE, 0, bb, parent_stmt_p);
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
      find_refs_in_expr (&TREE_OPERAND (expr, 0), ref_type,
	                 ref_mod | TRM_PARTIAL, bb, parent_stmt_p);
      return;
    }

  /* Assignments.  These are the only expressions that create pure V_DEF
     references.  */
  if (code == INIT_EXPR || code == MODIFY_EXPR)
    {
      find_refs_in_expr (&TREE_OPERAND (expr, 1), V_USE, 0, bb, parent_stmt_p);
      find_refs_in_expr (&TREE_OPERAND (expr, 0), V_DEF, 0, bb, parent_stmt_p);
      return;
    }
 

  /* Function calls.  Create a V_USE reference for every argument in the call.
     If the callee is neither pure nor const, create a use and a def of
     *.GLOBAL_VAR.  This variable is a pointer that is assumed to point to
     every global variable and locals that have had their address taken.
     
     Definitions of this variable will reach uses of every call clobbered
     variable in the function.  Uses of *.GLOBAL_VAR will be reached by
     definitions of call clobbered variables.  This is used to model the
     effects that the called function may have on local and global
     variables that might be visible to it.  */
  if (code == CALL_EXPR)
    {
      tree callee;
      int flags;

      find_refs_in_expr (&TREE_OPERAND (expr, 0), V_USE, 0, bb, parent_stmt_p);

      find_refs_in_expr (&TREE_OPERAND (expr, 1), V_USE, 0, bb, parent_stmt_p);

      callee = get_callee_fndecl (expr);
      flags = (callee) ? flags_from_decl_or_type (callee) : 0;

      /* If the called function is neither pure nor const, we create a
	  may-use followed by a clobbering definition of GLOBAL_VAR.  */
      if (! (flags & (ECF_CONST | ECF_PURE)))
	{
	  create_ref (indirect_var (global_var), V_USE, TRM_MAY, bb,
		      parent_stmt_p, 1);
	  create_ref (indirect_var (global_var), V_DEF, TRM_CLOBBER, bb,
		      parent_stmt_p, 1);
	}

      return;
    }

  /* ADDR_EXPR nodes create an address-of use of their operand.  This means
     that the variable is not read, but its address is needed.  */
  if (code == ADDR_EXPR)
    {
      find_refs_in_expr (&TREE_OPERAND (expr, 0), V_USE, TRM_ADDRESSOF, bb,
			 parent_stmt_p);
      return;
    }

  /* Lists.  */
  if (code == TREE_LIST)
    {
      tree op;

      for (op = expr; op; op = TREE_CHAIN (op))
	find_refs_in_expr (&TREE_VALUE (op), ref_type, ref_mod, bb,
	                   parent_stmt_p);
      return;
    }

  /* Unary expressions.  */
  if (class == '1'
      || code == EXPR_WITH_FILE_LOCATION
      || code == VA_ARG_EXPR
      || code == BIT_FIELD_REF)
    {
      find_refs_in_expr (&TREE_OPERAND (expr, 0), ref_type, ref_mod, bb,
			 parent_stmt_p);
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
      find_refs_in_expr (&TREE_OPERAND (expr, 0), ref_type, ref_mod, bb,
		         parent_stmt_p);
      find_refs_in_expr (&TREE_OPERAND (expr, 1), ref_type, ref_mod, bb,
	                 parent_stmt_p);
      return;
    }

  /* If we get here, something has gone wrong.  */
  fprintf (stderr, "unhandled expression in find_refs_in_expr():\n");
  debug_tree (expr);
  fputs ("\n", stderr);
  abort ();
}


/** @brief Create and return an empty list of references.  */

ref_list
create_ref_list ()
{
  ref_list list = ggc_alloc (sizeof (struct ref_list_priv));
  list->first = list->last = NULL;
  return list;
}


/** @brief Free the nodes in LIST, but keep the empty list around 
	   (i.e., empty the list).  */

void 
empty_ref_list (list)
     ref_list list;
{
  if (list == NULL)
    return;

  list->first = list->last = NULL;
}


/** @brief Remove REF from LIST.  */

void 
remove_ref_from_list (list, ref)
     ref_list list;
     tree_ref ref;
{
  struct ref_list_node *tmp;

  if (!list || !list->first || !list->last)
    return;

  tmp = find_list_node (list, ref);
  if (!tmp)
    return;

  if (tmp == list->first)
    list->first = tmp->next;
  if (tmp == list->last)
    list->last = tmp->prev;
  if (tmp->next)
    tmp->next->prev = tmp->prev;
  if (tmp->prev)
    tmp->prev->next = tmp->next;

  return;
}


/** @brief Add REF to the beginning of LIST.  */

void
add_ref_to_list_begin (list, ref)
     ref_list list;
     tree_ref ref;
{
  struct ref_list_node *node = ggc_alloc (sizeof (struct ref_list_node));
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


/** @brief Add REF to the end of LIST.  */

void
add_ref_to_list_end (list, ref)
     ref_list list;
     tree_ref ref;
{
  struct ref_list_node *node = ggc_alloc (sizeof (struct ref_list_node));
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


/** @brief Add the contents of the list TOADD to the list LIST,
	   at the beginning of LIST.  */

void 
add_list_to_list_begin (list, toadd)
     ref_list list;
     ref_list toadd;
{
  ref_list_iterator i;

  for (i = rli_start (toadd); !rli_after_end (i); rli_step (&i))
    add_ref_to_list_begin (list, rli_ref (i));
}


/** @brief Add the contents of the list TOADD to the list LIST, at
	   the end of LIST.  */

void
add_list_to_list_end (list, toadd)
     ref_list list;
     ref_list toadd;
{
  ref_list_iterator i;
 
  for (i = rli_start (toadd); !rli_after_end (i); rli_step (&i))
    add_ref_to_list_end (list, rli_ref (i));
}


/** @brief Find the list container for reference REF in LIST.  */

struct ref_list_node *
find_list_node (list, ref)
     ref_list list;
     tree_ref ref;
{
  struct ref_list_node *node = NULL;

  if (list->first == NULL)
    return NULL;

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

/** @brief Start a new forward iterator on the first element of LIST.  */

ref_list_iterator
rli_start (list)
     ref_list list;
{
  ref_list_iterator i;

  if (list)
    {
      /* rli_delete may have left list->first empty.  Reset it here.  */
      while (list->first && list->first->ref == NULL)
	list->first = list->first->next;

      i.node = list->first;
    }
  else
    i.node = NULL;

  return i;
}


/** @brief Start a new iterator on the last element of LIST.  */

ref_list_iterator
rli_start_last (list)
     ref_list list;
{
  ref_list_iterator i;

  if (list)
    {
      /* rli_delete may have left list->last empty.  Reset it here.  */
      while (list->last && list->last->ref == NULL)
	list->last = list->last->prev;

      i.node = list->last;
    }
  else
    i.node = NULL;

  return i;
}


/** @brief Start a new iterator on a specific list node N.  */

ref_list_iterator
rli_start_at (n)
     struct ref_list_node *n;
{
  ref_list_iterator i;
  i.node = n;
  return i;
}


/** @brief Delete the reference at the current position of iterator I.
   
    NOTE: This will not remove the first and last element of a list.  It
    will merely nullify the reference that they are pointing to.  This will
    be "corrected" the next time an iterator is started on the same list.

    @todo FIXME: This is a bit hackish, but otherwise we need to either
    keep handles on ref_list nodes in the iterator or include the list in
    the structure.  */

void
rli_delete (i)
     ref_list_iterator i;
{
  struct ref_list_node *n = i.node;

  n->ref = NULL;

  if (n->prev)
    n->prev->next = n->next;

  if (n->next)
    n->next->prev = n->prev;
}


/** @brief Add REF after NODE.  */

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
      struct ref_list_node *new = ggc_alloc (sizeof (struct ref_list_node));
      new->ref = ref;
      new->prev = node;
      new->next = node->next;
      node->next = new;
    }
}


/** @brief Return the size in bytes of a reference of type REF_TYPE.  */

static size_t
tree_ref_size (ref_type)
     enum tree_ref_type ref_type;
{
  switch (ref_type)
    {
    case V_DEF:
      return sizeof (struct var_def_d);
    case V_USE:
      return sizeof (struct var_use_d);
    case V_PHI:
      return sizeof (struct var_phi_d);
    case E_PHI:
      return sizeof (struct expr_phi_d);
    case E_USE:
      return sizeof (struct expr_use_d);
    case E_KILL:
      return sizeof (struct expr_ref_common);
    case E_EXIT:
      return sizeof (struct expr_ref_common);
    case E_LEFT:
      return sizeof (struct expr_ref_common);
    }

  abort ();
}


/** @brief Create a new variable reference
    @param var is the variable for which the new reference must be created.
    @param ref_type is the type of reference to create (V_DEF, V_USE, V_PHI, etc).
    @param ref_mod is the set of modifier flags for REF_TYPE.
    @param bb is the basic block in which the reference must be created.
    @param parent_stmt_p is the statement in which the reference occurs.
    @param add_to_bb should be 1 if the caller wants the reference to be added
	   to the list of references for BB (i.e., bb_refs (BB)).

    The modifier REF_MOD is a bitmask built from the TRM_* constants.  This
    bitmask is used to set the corresponding bitfield in the various tree_ref
    structures.

    PARENT_STMT_P can be NULL in the case of artificial references (PHI nodes,
    default definitions, etc).

    If ADD_TO_BB is 1, the reference is added at the end of the list. This is
    a problem for certain types of references like V_PHI that need to be added
    in specific places within the list of references for the BB. If ADD_TO_BB
    is 0, the caller is responsible for the placement of the newly created
    reference.  */

tree_ref
create_ref (var, ref_type, ref_mod, bb, parent_stmt_p, add_to_bb)
     tree var;
     enum tree_ref_type ref_type;
     unsigned ref_mod;
     basic_block bb;
     tree *parent_stmt_p;
     int add_to_bb;
{
  size_t size;
  tree_ref ref;
  tree parent_stmt = (parent_stmt_p) ? *parent_stmt_p : NULL_TREE;

#if defined ENABLE_CHECKING
  if (bb == NULL)
    abort ();

  if ((ref_type == V_DEF
       || ref_type == V_USE
       || ref_type == V_PHI)
      && ! DECL_P (var)
      && TREE_CODE (var) != INDIRECT_REF)
    abort ();
#endif

  /* Set the M_VOLATILE modifier if the reference is to a volatile
     variable.  */
  if (var && TREE_THIS_VOLATILE (var))
    ref_mod |= TRM_VOLATILE;

  size = tree_ref_size (ref_type);
  ref = (tree_ref) ggc_alloc (size);
  memset ((void *) ref, 0, size);

  ref->common.id = next_tree_ref_id++;
  ref->common.var = var;
  ref->common.type = ref_type;
  ref->common.bb = bb;
  ref->common.stmt_p = parent_stmt_p;

  /* Set the reference type modifier flags.  */
  if (ref_mod & TRM_MAY)
    ref->vref.m_may = 1;

  if (ref_mod & TRM_PARTIAL)
    ref->vref.m_partial = 1;

  if (ref_mod & TRM_VOLATILE)
    ref->vref.m_volatile = 1;

  /* Create containers according to the type of reference.  */
  if (ref_type == V_DEF || ref_type == V_PHI)
    {
      if (ref_mod & TRM_CLOBBER)
	ref->vdef.m_clobber = 1;

      if (ref_mod & TRM_RELOCATE)
	ref->vdef.m_relocate = 1;

      if (ref_mod & TRM_INITIAL)
	ref->vdef.m_initial = 1;

      ref->vdef.imm_uses = create_ref_list ();
      ref->vdef.reached_uses = create_ref_list ();

      if (ref_type == V_PHI)
	VARRAY_GENERIC_PTR_INIT (ref->vphi.phi_args, 2, "phi_args");
    }
  else if (ref_type == V_USE)
    {
      if (ref_mod & TRM_ADDRESSOF)
	ref->vuse.m_addressof = 1;

      ref->vuse.rdefs = create_ref_list ();
    }
  else if (ref_type == E_PHI)
    {
      varray_type temp;
      VARRAY_GENERIC_PTR_INIT (temp, 1, "ephi_chain");
      set_exprphi_phi_args (ref, temp);
      set_exprphi_downsafe (ref, true);
      set_exprphi_canbeavail (ref, true);
      set_exprphi_later (ref, true);
      set_exprphi_extraneous (ref, true);
    }

  if (var)
    {
      /* Add the variable to the list of variables referenced in this
	 function.  But only for actual variable defs or uses in the code.  */
      if ((ref_type == V_DEF || ref_type == V_USE)
	  && (DECL_P (var) || TREE_CODE (var) == INDIRECT_REF))
	add_referenced_var (var);

      /* Add this reference to the list of references for the variable.  */
      add_tree_ref (var, ref);

      /* In some cases the parent statement and parent expression are the
	 same tree node.  For instance 'a = 5;'.  Avoid adding the same
	 reference twice to the same list in these cases.  */
      if (parent_stmt)
	add_tree_ref (parent_stmt, ref);
    }

  /* If requested, add this reference to the list of references for the basic
     block.  */
  if (add_to_bb)
    add_ref_to_list_end (bb_refs (bb), ref);

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
  dfa_counts.num_phi_args++;

  arg->def = def;
  arg->e = e;

  VARRAY_PUSH_GENERIC_PTR (phi->vphi.phi_args, (PTR)arg);
}


/* Add a new argument to EPHI for definition DEF reaching in via edge E.  */

void
add_ephi_arg (phi, def, e)
     tree_ref phi;
     tree_ref def;
     edge e;
{
  phi_node_arg arg;

  arg = (phi_node_arg) ggc_alloc (sizeof (*arg));
  memset ((void *) arg, 0, sizeof (*arg));

  arg->def = def;
  arg->e = e;

  VARRAY_PUSH_GENERIC_PTR (phi->ephi.phi_args, (PTR)arg);
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


/*---------------------------------------------------------------------------
			     Code replacement
---------------------------------------------------------------------------*/
/* Replace reference REF with operand OP, which must be a constant or a
   variable.  
   
   1- REF is removed from the list of references for its variable
      and statement.
      
   2- If OP is a variable, a new reference for it is created.
   
   3- The SSA web is updated by removing all use-def and def-use links
      reaching this reference.  */

void
replace_ref_with (ref, op)
     tree_ref ref;
     tree op;
{
#if 1
  /* FIXME  We still can't replace a reference with another variable.  For
	    that we need to be able to insert new references in the SSA
	    web, not just remove them.  */
  if (!really_constant_p (op))
    abort ();
#endif

  try_replace_ref_with (ref_stmt (ref), ref, op);
  remove_ref (ref);
}


/* Replace reference REF in statement STMT with tree node OP.  Note that
   this does not update the SSA web nor the references made by the program.  */

void
try_replace_ref_with (stmt, ref, op)
     tree stmt;
     tree_ref ref;
     tree op;
{
  enum tree_code code;
  struct replace_data_d replace;

  STRIP_WFL (stmt);
  STRIP_NOPS (stmt);

#if defined ENABLE_CHECKING
  if (!really_constant_p (op)
      && !DECL_P (op)
      && TREE_CODE (op) != INDIRECT_REF
      && TREE_CODE (op) != COMPONENT_REF)
    abort ();

  if (ref_type (ref) != V_DEF && ref_type (ref) != V_USE)
    abort ();

  if (ref_type (ref) == V_DEF && really_constant_p (op))
    abort ();
#endif

  replace.old = ref_var (ref);
  replace.new = op;

  code = TREE_CODE (stmt);
  switch (code)
    {
    case INIT_EXPR:
    case MODIFY_EXPR:
      /* The LHS of an assignment can only be a variable that we match
	 exactly.  */
      if (ref_type (ref) == V_DEF
	  && same_var_p (TREE_OPERAND (stmt, 0), replace.old))
	TREE_OPERAND (stmt, 0) = replace.new;
      else
	{
	  /* If the LHS of the assignment is a VAR_DECL, then it can have
	     no V_USE references and should be ignored.  Otherwise, given
	     an expression of the form 'a = a + 1', if we were replacing
	     uses of 'a' with '3' we would end up with '3 = 3 + 1'.  */
	  if (!DECL_P (TREE_OPERAND (stmt, 0)))
	    walk_tree (&TREE_OPERAND (stmt, 0), replace_ref_r, &replace,
		      NULL);
	  walk_tree (&TREE_OPERAND (stmt, 1), replace_ref_r, &replace,
		     NULL);
	}
      break;

    case CALL_EXPR:
      {
	tree old_type = TREE_TYPE (replace.old);
	tree new_type = TREE_TYPE (replace.new);

	if (old_type != new_type)
	  replace.new = fold (convert_to_pointer (old_type, replace.new));

	walk_tree (&TREE_OPERAND (stmt, 0), replace_ref_r, &replace, NULL);
	walk_tree (&TREE_OPERAND (stmt, 1), replace_ref_r, &replace, NULL);
      }
      break;

    case COND_EXPR:
      walk_tree (&COND_EXPR_COND (stmt), replace_ref_r, &replace, NULL);
      break;

    case SWITCH_EXPR:
      walk_tree (&SWITCH_COND (stmt), replace_ref_r, &replace, NULL);
      break;

    case ASM_EXPR:
      if (ref_type (ref) == V_USE)
	walk_tree (&ASM_INPUTS (stmt), replace_ref_r, &replace, NULL);
      else
	{
	  walk_tree (&ASM_OUTPUTS (stmt), replace_ref_r, &replace, NULL);
	  walk_tree (&ASM_CLOBBERS (stmt), replace_ref_r, &replace, NULL);
	}
      break;

    case RETURN_EXPR:
      walk_tree (&TREE_OPERAND (stmt, 0), replace_ref_r, &replace, NULL);
      break;

    case GOTO_EXPR:
      walk_tree (&GOTO_DESTINATION (stmt), replace_ref_r, &replace, NULL);
      break;

    case LABEL_EXPR:
      walk_tree (&LABEL_EXPR_LABEL (stmt), replace_ref_r, &replace, NULL);
      break;

      /* These nodes contain no variable references.  */
    case LOOP_EXPR:
    case BIND_EXPR:
    case CASE_LABEL_EXPR:
      break;

    default:
      abort ();
    }
}


/* Call back for walk_tree to replace references in expression *TP.  */

static tree
replace_ref_r (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees ATTRIBUTE_UNUSED;
     void *data;
{
  struct replace_data_d *replace_data = (struct replace_data_d *)data;
  tree t = *tp;
  tree old = replace_data->old;
  tree new = replace_data->new;

  if (same_var_p (old, t))
    *tp = new;

  return NULL_TREE;
}


/* Return true if V1 and V2 are the same variable.  */

bool
same_var_p (v1, v2)
     tree v1;
     tree v2;
{
  return (v1 == v2
	  || (TREE_CODE (v1) == INDIRECT_REF
	      && TREE_CODE (v2) == INDIRECT_REF
	      && TREE_OPERAND (v1, 0) == TREE_OPERAND (v2, 0)));
}


/* Replace the statement for a reference with a new statement.

   FIXME: Need to properly update DFA and CFG information.  */

void
replace_ref_stmt_with (ref, stmt)
     tree_ref ref;
     tree stmt;
{
  if (ref->common.stmt_p)
    *(ref->common.stmt_p) = stmt;
}


/* Remove reference REF from the program.  Update all the places in the SSA
   web where this reference is used.  */

void
remove_ref (ref)
     tree_ref ref;
{
  tree var;
  tree_ref rdef;

  if (ref == NULL)
    return;

  var = ref_var (ref);
  rdef = imm_reaching_def (ref);

  if (ref_type (ref) == V_USE)
    {
      /* Remove REF from the imm-uses list of its immediate reaching
	 definition.  */
      if (rdef)
	remove_ref_from_list (imm_uses (rdef), ref);
    }
  else if (ref_type (ref) == V_DEF || ref_type (ref) == V_PHI)
    remove_def (ref);
  else
    abort ();

  remove_ref_from_list (tree_refs (var), ref);
  remove_ref_from_list (tree_refs (ref_stmt (ref)), ref);
  remove_ref_from_list (bb_refs (ref_bb (ref)), ref);

  /* If there are no more references to REF's variable, remove it from the
     symbol table.  */
  if (ref_list_is_empty (tree_refs (var))
      && TREE_CODE (var) == VAR_DECL)
    remove_decl (ref_var (ref));
}


/* Remove a V_DEF or V_PHI reference from the SSA web.  Note that this does
   not remove the reference from all the appropriate places.  It only
   removes def-use and def-def edges that involve this reference.  This
   function is only a helper for remove_ref.  */

static void
remove_def (def)
     tree_ref def;
{
  ref_list_iterator i;
  tree_ref rdef;
  tree var;

  rdef = imm_reaching_def (def);
  var = ref_var (def);

  /* Every use or PHI node reached by this definition should now be reached
     by DEF's immediate reaching definition.  In the case of PHI nodes, we
     look for DEF in the list of arguments and replace it with DEF's
     immediate reaching definition.  */
  for (i = rli_start (imm_uses (def)); !rli_after_end (i); rli_step (&i))
    {
      tree_ref r = rli_ref (i);

      if (ref_type (r) == V_USE)
	set_imm_reaching_def (r, rdef);
      else if (ref_type (r) == V_PHI)
	replace_phi_arg_with (r, def, rdef);
      else
	abort ();
    }

  /* Fix all the definitions that contain a def-def link back to DEF.
     Since we don't keep bidirectional def-def links, we don't know
     what other definitions may have a def-def link back to DEF.  There
     are several ways we could look for them:

     - Keep bidirectional def-def links similarly to the def-use links
       we keep.  This would increase memory usage.

     - Traverse all the definitions in blocks dominated by DEF's block.
       This would work, but this function is typically called while
       removing blocks from the flowgraph, therefore it is not always
       possible to compute dominance information (parts of the graph may be
       disconnected).

     - Traverse all the definitions for DEF's variable and its aliases.
       This is the solution that we implement here.  It has the drawback
       that if DEF's variable has many aliases, we will be traversing a lot
       of lists.  */
  reset_def_def_links (tree_refs (var), def, rdef);

  /* If DEF's variable is aliased we must now look through all the
     definitions for variables in its alias set.
     
     FIXME  this can be very expensive.  Maybe it's better to keep
     bi-directional def-def links after all?  */
  if (is_aliased (var))
    {
      unsigned long j;

      for (j = 0; j < num_referenced_vars; j++)
	{
	  tree alias = referenced_var (j);
	  if (alias != var && alias_leader (alias) == alias_leader (var))
	    reset_def_def_links (tree_refs (alias), def, rdef);
	}
    }

  /* If DEF is a PHI node, then all of its arguments have it in their list
     of immediate uses.  Remove DEF from the list of immediate uses of each
     of its arguments.  */
  if (ref_type (def) == V_PHI)
    {
      unsigned int j;
      for (j = 0; j < num_phi_args (def); j++)
	{
	  tree_ref arg_def = phi_arg_def (phi_arg (def, j));
	  if (arg_def)
	    remove_ref_from_list (imm_uses (arg_def), def);
	}
    }
}


/* Change the def-def link for every definition in REFS.  If a definition D
   in REFS was pointing to OLD_RDEF, make it point to NEW_RDEF.  */

static void
reset_def_def_links (refs, old_rdef, new_rdef)
     ref_list refs;
     tree_ref old_rdef;
     tree_ref new_rdef;
{
  ref_list_iterator i;

  for (i = rli_start (refs); !rli_after_end (i); rli_step (&i))
    {
      tree_ref d = rli_ref (i);
      if (ref_type (d) == V_DEF && imm_reaching_def (d) == old_rdef)
	set_imm_reaching_def (rli_ref (i), new_rdef);
    }
}


/* Find argument OLD_DEF in the list of arguments of phi node PHI and
   replace it with argument NEW_DEF.  Note that this assumes that both
   OLD_DEF and NEW_DEF are coming from the same edge (not necessarily the
   same basic block).  That is, OLD_DEF reaches NEW_DEF.  */

static void
replace_phi_arg_with (phi, old_def, new_def)
     tree_ref phi;
     tree_ref old_def;
     tree_ref new_def;
{
  unsigned int i;

  for (i = 0; i < num_phi_args (phi); i++)
    {
      phi_node_arg arg = phi_arg (phi, i);
      if (phi_arg_def (arg) == old_def)
	{
	  arg->def = new_def;
	  return;
	}
    }
}


/*---------------------------------------------------------------------------
			    Manage annotations
---------------------------------------------------------------------------*/

/* Create a new annotation for tree T.  */

tree_ann
create_tree_ann (t)
     tree t;
{
  tree_ann ann;

#if defined ENABLE_CHECKING
  if (t == empty_stmt_node
      || t == NULL_TREE
      || TREE_CODE_CLASS (TREE_CODE (t)) == 'c'
      || TREE_CODE_CLASS (TREE_CODE (t)) == 't')
    abort ();
#endif

  ann = (tree_ann) ggc_alloc (sizeof (*ann));
  memset ((void *) ann, 0, sizeof (*ann));
  STRIP_WFL (t);
  STRIP_NOPS (t);
  t->common.ann = ann;
  return ann;
}


/*---------------------------------------------------------------------------
			     Miscellaneous helpers
---------------------------------------------------------------------------*/

/* Return 1 if the function may call itself.
   
   FIXME Currently this is very limited because we do not have call-graph
	 information.  */

bool
function_may_recurse_p ()
{
  basic_block bb;

  /* If we only make calls to pure and/or builtin functions, then the
     function is not recursive.  */
  FOR_EACH_BB (bb)
    {
      ref_list_iterator i;

      for (i = rli_start (bb_refs (bb)); !rli_after_end (i); rli_step (&i))
	if (ref_var (rli_ref (i)) == global_var
	    && is_clobbering_def (rli_ref (i)))
	  return true;
    }

  return false;
}



/*---------------------------------------------------------------------------
			      Debugging functions
---------------------------------------------------------------------------*/

/* Display variable reference REF on stream OUTF.  PREFIX is a string that
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

  s_indent = (char *) alloca ((size_t) indent + 1);
  memset ((void *) s_indent, ' ', (size_t) indent);
  s_indent[indent] = '\0';

  fprintf (outf, "%s%s", s_indent, prefix);

  if (ref == NULL)
    {
      fprintf (outf, "<nil>\n");
      return;
    }

  lineno = get_lineno (ref_stmt (ref));
  bbix = (ref_bb (ref)) ? ref_bb (ref)->index : -1;
  type = ref_type_name (ref);

  fprintf (outf, "%s(", type);

  if (ref_var (ref))
    print_generic_expr (outf, ref_var (ref), 0);
  else
    fprintf (outf, "<nil>");

  fprintf (outf, "): line %d, bb %d, id %lu, ", lineno, bbix, ref_id (ref));

  if (ref_stmt (ref))
    print_generic_stmt (outf, ref_stmt (ref), TDF_SLIM);
  else
    fprintf (outf, "<nil>");

  /* Dump specific contents for the different types of references.  */
  if (details)
    {
      if (ref_type (ref) == V_PHI)
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

      if ((ref_type (ref) == E_PHI) && exprphi_phi_args (ref))
	{
	  if (details)
	    fprintf (outf, " class:%d downsafe:%d can_be_avail:%d later:%d\n", 
		     exprref_class (ref), exprphi_downsafe (ref), 
		     exprphi_canbeavail (ref), exprphi_later (ref));
	  fputs (" expr-phi-args:\n", outf);
	  dump_phi_args (outf, prefix, exprphi_phi_args (ref), indent + 4, 1);
	}	

      if (ref_type (ref) == V_DEF && imm_uses (ref))
	{
	  fputs (" immediate uses:\n", outf);
	  dump_ref_list (outf, prefix, imm_uses (ref), indent + 4, 0);
	}

      if (ref_type (ref) == V_USE && imm_reaching_def (ref))
	{
	  fputs (" immediate reaching def:\n", outf);
	  dump_ref (outf, prefix, imm_reaching_def (ref), indent + 4, 0);
	}	  

      if ((ref_type (ref) == E_USE) && expruse_phiop (ref))
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
  ref_list_iterator i;

  for (i = rli_start (reflist); !rli_after_end (i); rli_step (&i))
    dump_ref (outf, prefix, rli_ref (i), indent, details);
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


/* Dump the list of all the referenced variables in the current function.
   If DETAILS is nonzero, each reference is dumped in full.  */

void
dump_referenced_vars (file, details)
     FILE *file;
     int details;
{
  unsigned long i;

  fprintf (file, "\nReferenced variables in %s: %lu\n\n", 
	   get_name (current_function_decl), num_referenced_vars);

  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);
      fprintf (file, "Variable: ");
      dump_variable (file, var);
      dump_ref_list (file, "", tree_refs (var), 2, details);
      fprintf (file, "\n");
    }
}


/* Dump the list of all the referenced variables to stderr.  If DETAILS is
   nonzero, each reference is dumped in full.  */

void
debug_referenced_vars (details)
     int details;
{
  dump_referenced_vars (stderr, details);
}


/* Dump a variable and its may-aliases to FILE.  */

void
dump_variable (file, var)
     FILE *file;
     tree var;
{
  print_generic_expr (file, var, 0);
  
  if (alias_leader (var))
    {
      fprintf (file, ", alias leader: ");
      print_generic_expr (file, alias_leader (var), 0);
    }

  if (may_alias_global_mem_p (var))
    fprintf (file, ", may alias global memory");

  fprintf (file, "\n");
}


/* Dump a variable and its may-aliases to stderr.  */

void
debug_variable (var)
     tree var;
{
  dump_variable (stderr, var);
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
    {
      phi_node_arg arg = VARRAY_GENERIC_PTR (args, i);
      dump_ref (outf, prefix, (arg) ? phi_arg_def (arg) : NULL, indent,
	        details);
    }
}


/* Dump various DFA statistics to FILE.  */

#define SCALE(x) ((unsigned long) ((x) < 1024*10 \
		  ? (x) \
		  : ((x) < 1024*1024*10 \
		     ? (x) / 1024 \
		     : (x) / (1024*1024))))
#define LABEL(x) ((x) < 1024*10 ? 'b' : ((x) < 1024*1024*10 ? 'k' : 'M'))
#define PERCENT(x,y) ((float)(x) * 100.0 / (float)(y))

void
dump_dfa_stats (file)
     FILE *file;
{
  struct dfa_stats_d dfa_stats;
  unsigned long size, total = 0;
  const char * const fmt_str   = "%-30s%-13s%12s\n";
  const char * const fmt_str_1 = "%-30s%13lu%11lu%c\n";
  const char * const fmt_str_2 = "%-30s%6lu (%3.0f%%)\n";
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

  size = dfa_stats.num_tree_anns * sizeof (struct tree_ann_d);
  total += size;
  fprintf (file, fmt_str_1, "Trees annotated", dfa_stats.num_tree_anns,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_ref_list_nodes * sizeof (struct ref_list_node);
  total += size;
  fprintf (file, fmt_str_1, "ref_list nodes", dfa_stats.num_ref_list_nodes,
	   SCALE (size), LABEL (size));

  size = dfa_stats.num_phi_args * sizeof (struct phi_node_arg_d);
  total += size;
  fprintf (file, fmt_str_1, "PHI arguments", dfa_stats.num_phi_args,
	   SCALE (size), LABEL (size));

  size = dfa_stats.size_tree_refs;
  total += size;
  fprintf (file, fmt_str_1, "Variable references", dfa_stats.num_tree_refs,
	   SCALE (size), LABEL (size));

  if (dfa_stats.num_tree_refs == 0)
    dfa_stats.num_tree_refs = 1;

  if (dfa_stats.num_defs)
    fprintf (file, fmt_str_2, "    V_DEF", dfa_stats.num_defs,
	     PERCENT (dfa_stats.num_defs, dfa_stats.num_tree_refs));

  if (dfa_stats.num_uses)
    fprintf (file, fmt_str_2, "    V_USE", dfa_stats.num_uses,
	     PERCENT (dfa_stats.num_uses, dfa_stats.num_tree_refs));

  if (dfa_stats.num_phis)
    fprintf (file, fmt_str_2, "    V_PHI", dfa_stats.num_phis,
	     PERCENT (dfa_stats.num_phis, dfa_stats.num_tree_refs));

  if (dfa_stats.num_ephis)
    fprintf (file, fmt_str_2, "    E_PHI", dfa_stats.num_ephis,
	     PERCENT (dfa_stats.num_ephis, dfa_stats.num_tree_refs));

  if (dfa_stats.num_euses)
    fprintf (file, fmt_str_2, "    E_USE", dfa_stats.num_euses,
	     PERCENT (dfa_stats.num_euses, dfa_stats.num_tree_refs));

  if (dfa_stats.num_ekills)
    fprintf (file, fmt_str_2, "    E_KILL", dfa_stats.num_ekills,
	     PERCENT (dfa_stats.num_ekills, dfa_stats.num_tree_refs));

  fprintf (file, "---------------------------------------------------------\n");
  fprintf (file, fmt_str_3, "Total memory used by DFA/SSA data", SCALE (total),
	   LABEL (total));
  fprintf (file, "---------------------------------------------------------\n");
  fprintf (file, "\n");

  if (dfa_stats.num_phis)
    fprintf (file, "Average number of PHI arguments per PHI node: %.1f (max: %lu)\n",
	     (float) dfa_stats.num_phi_args / (float) dfa_stats.num_phis,
	     dfa_stats.max_num_phi_args);

  fprintf (file, "\n");

  dump_if_different (file, "Discrepancy in variable references: %ld\n",
		     next_tree_ref_id, dfa_stats.num_tree_refs);

  dump_if_different (file, "Discrepancy in PHI arguments: %ld\n",
		     dfa_counts.num_phi_args, dfa_stats.num_phi_args);

  fprintf (file, "\n");
}


static void
dump_if_different (file, fmt_str, expected, counted)
     FILE *file;
     const char * const fmt_str;
     unsigned long expected;
     unsigned long counted;
{
  if (expected != counted)
    {
      fprintf (file, fmt_str, labs (expected - counted));
      fprintf (file, "\texpected: %lu\n", expected);
      fprintf (file, "\tcounted:  %lu\n", counted);
    }
}


/* Dump DFA statistics on stderr.  */

void
debug_dfa_stats ()
{
  dump_dfa_stats (stderr);
}


/* Collect DFA statistics into *DFA_STATS_P.  */

static void
collect_dfa_stats (dfa_stats_p)
     struct dfa_stats_d *dfa_stats_p;
{
  htab_t htab;
  basic_block bb;
  tree star_global_var;
  gimple_stmt_iterator i;

  if (dfa_stats_p == NULL)
    abort ();

  memset ((void *)dfa_stats_p, 0, sizeof (struct dfa_stats_d));

  /* Walk all the trees in the function counting references.  Start at
     basic block 0, but don't stop at block boundaries.  */
  htab = htab_create (30, htab_hash_pointer, htab_eq_pointer, NULL);
  for (i = gsi_start_bb (BASIC_BLOCK (0)); !gsi_end_bb (i); gsi_step (&i))
    {
      tree *stmt_p = gsi_stmt_ptr (i);
      walk_tree (stmt_p, collect_dfa_stats_r, (void *) dfa_stats_p,
	         (void *) htab);
    }
  htab_delete (htab);

  /* Also look into GLOBAL_VAR (which is not actually part of the program).  */
  walk_tree (&global_var, collect_dfa_stats_r, (void *) dfa_stats_p, NULL);
  star_global_var = indirect_var (global_var);
  walk_tree (&star_global_var, collect_dfa_stats_r, (void *) dfa_stats_p, NULL);

  FOR_EACH_BB (bb)
    count_tree_refs (dfa_stats_p, bb_refs (bb));
}


/* Callback for walk_tree to collect DFA statistics for a tree and its
   children.  */

static tree
collect_dfa_stats_r (tp, walk_subtrees, data)
     tree *tp;
     int *walk_subtrees ATTRIBUTE_UNUSED;
     void *data;
{
  tree_ann ann;
  tree t = *tp;
  struct dfa_stats_d *dfa_stats_p = (struct dfa_stats_d *)data;

  /* Don't call tree_annotation here because it strips the WFL and NOPS
     wrappers from T.  */
  ann = t->common.ann;
  if (ann)
    {
      dfa_stats_p->num_tree_anns++;
      count_ref_list_nodes (dfa_stats_p, ann->refs);
    }

  return NULL;
}


/* Update DFA_STATS_P with the number of tree_ref objects in LIST.  */

static void
count_tree_refs (dfa_stats_p, list)
     struct dfa_stats_d *dfa_stats_p;
     ref_list list;
{
  ref_list_iterator i;

  for (i = rli_start (list); !rli_after_end (i); rli_step (&i))
    {
      tree_ref ref = rli_ref (i);
      dfa_stats_p->num_tree_refs++;
      dfa_stats_p->size_tree_refs += tree_ref_size (ref_type (ref));

      if (ref_type (ref) == V_DEF)
	{
	  dfa_stats_p->num_defs++;
	  count_ref_list_nodes (dfa_stats_p, imm_uses (ref));
	  count_ref_list_nodes (dfa_stats_p, reached_uses (ref));
	}
      else if (ref_type (ref) == V_USE)
	{
	  dfa_stats_p->num_uses++;
	  count_ref_list_nodes (dfa_stats_p, reaching_defs (ref));
	}
      else if (ref_type (ref) == V_PHI)
	{
	  unsigned int num = num_phi_args (ref);
	  dfa_stats_p->num_phis++;
	  dfa_stats_p->num_phi_args += num;
	  if (num > dfa_stats_p->max_num_phi_args)
	    dfa_stats_p->max_num_phi_args = num;
	}
      else if (ref_type (ref) == E_PHI)
	dfa_stats_p->num_ephis++;
      else if (ref_type (ref) == E_USE)
	dfa_stats_p->num_euses++;
      else if (ref_type (ref) == E_KILL)
	dfa_stats_p->num_ekills++;
    }
}


/* Count the number of nodes in a ref_list container.  */

static void
count_ref_list_nodes (dfa_stats_p, list)
     struct dfa_stats_d *dfa_stats_p;
     ref_list list;
{
  ref_list_iterator i;

  for (i = rli_start (list); !rli_after_end (i); rli_step (&i))
    dfa_stats_p->num_ref_list_nodes++;
}


/* Return the reference type and modifiers for reference REF as a string.  */

const char *
ref_type_name (ref)
     tree_ref ref;
{
#define max 80
  static char str[max];
  enum tree_ref_type type = ref_type (ref);

  strncpy (str, type == V_DEF ? "V_DEF"
	        : type == V_USE ? "V_USE"
	        : type == V_PHI ? "V_PHI"
	        : type == E_PHI ? "E_PHI"
	        : type == E_USE ? "E_USE"
	        : type == E_KILL ? "E_KILL"
	        : type == E_EXIT ? "E_EXIT"
	        : type == E_LEFT ? "E_LEFT"
	        : "???",
	   max);
  
  if (ref_type (ref) == E_USE 
      || ref_type (ref) == E_KILL
      || ref_type (ref) == E_EXIT
      || ref_type (ref) == E_LEFT
      || ref_type (ref) == E_PHI)
    return str;
  
  if (is_may_ref (ref))
    strncat (str, "/may", max - strlen (str));
  
  if (is_partial_ref (ref))
    strncat (str, "/partial", max - strlen (str));

  if (is_clobbering_def (ref))
    strncat (str, "/clobber", max - strlen (str));

  if (is_initializing_def (ref))
    strncat (str, "/initial", max - strlen (str));

  if (is_volatile_ref (ref))
    strncat (str, "/volatile", max - strlen (str));

  if (is_relocating_def (ref))
    strncat (str, "/relocate", max - strlen (str));

  if (is_addressof_use (ref))
    strncat (str, "/addressof", max - strlen (str));

  return str;
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
      create_ref (*tp, V_USE, TRM_MAY, clobber->bb, clobber->parent_stmt_p, 1);
      create_ref (*tp, V_DEF, TRM_CLOBBER, clobber->bb, clobber->parent_stmt_p,
		  1);
    }

  return NULL;
}

/* Compute may-alias information for every variable referenced in the
   program.  NOTE: in the absence of points-to analysis (-ftree-points-to),
   this computes a bigger set than necessary.  */

static void
compute_may_aliases ()
{
  unsigned long i;

  timevar_push (TV_TREE_MAY_ALIAS);

  if (flag_tree_points_to != PTA_NONE && num_referenced_vars)
    {
      timevar_push (TV_TREE_PTA);
      create_alias_vars ();
      timevar_pop (TV_TREE_PTA);
    }

  num_alias_leaders = 0;
  alias_leaders = (tree *) xmalloc (num_referenced_vars * sizeof (tree));
  if (alias_leaders == NULL)
    abort ();
  memset ((void *) alias_leaders, 0, num_referenced_vars * sizeof (tree));
  
  for (i = 0; i < num_referenced_vars; i++)
    {
      tree var = referenced_var (i);

      /* Find aliases for pointer variables.  */
      if (TREE_CODE (var) == INDIRECT_REF)
	find_may_aliases_for (var);
    }

  timevar_pop (TV_TREE_MAY_ALIAS);

  /* Debugging dumps.  */
  if (tree_ssa_dump_file && tree_ssa_dump_flags & TDF_ALIAS)
    {
      unsigned long i, j;

      fprintf (tree_ssa_dump_file, "\nAlias information for %s: %ld sets\n\n",
	       get_name (current_function_decl), num_alias_leaders);

      for (i = 0; i < num_alias_leaders; i++)
	{
	  fprintf (tree_ssa_dump_file, "Alias set #%ld:\n", i);
	  fprintf (tree_ssa_dump_file, "  Leader: ");
	  dump_variable (tree_ssa_dump_file, alias_leaders[i]);
	  fprintf (tree_ssa_dump_file, "  Aliases: { ");
	  for (j = 0; j < num_referenced_vars; j++)
	    if (alias_leader (referenced_var (j)) == alias_leaders[i])
	      {
		print_generic_expr (tree_ssa_dump_file, referenced_var (j), 0);
		fprintf (tree_ssa_dump_file, " ");
	      }
	  fprintf (tree_ssa_dump_file, "}\n\n");
	}
    }

  free (alias_leaders);
  alias_leaders = NULL;
  num_alias_leaders = 0;
}


/* Return true if V1 and V2 may alias.  */

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
	  
	  /* For global_var, we want to see if the variable aliases
	     global_var, not if global_var aliases the variable (since
	     the points-to sets are possibly directional, and
	     global_var never gets assigned to, only assigned from). */ 
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


/* Find variables that V1 may be aliasing.  For every variable V2 that V1
   may alias, add V2 to one of the alias sets in the array ALIAS_LEADERS.

   Each entry I in ALIAS_LEADERS represents a set of all the variables that
   are aliased by ALIAS_LEADERS[I].  In the absence of points-to
   information, the ALIAS_LEADERS array will tend to have few entries, and
   each entry will likely alias many program variables.
   
   This will negatively impact optimizations because variable uses will be
   reached by many definitions that can't really reach them.  See the
   documentation in tree-ssa.c.  */

static void
find_may_aliases_for (v1)
     tree v1;
{
  unsigned long i;

  if (may_access_global_mem (v1))
    set_tree_flag (v1, TF_MAY_ALIAS_GLOBAL_MEM);

  for (i = 0; i < num_referenced_vars; i++)
    {
      tree v2 = referenced_var (i);

      if (v1 == v2)
	continue;

      if (may_alias_p (v1, v2))
	{
	  tree al = find_alias_leader (v2);
	  if (al == NULL_TREE)
	    al = alias_leaders [num_alias_leaders++] = v1;

	  set_alias_leader (v2, al);

	  /* If V2 may access global memory, mark both AL and V1 as aliases
	     for global memory.  */
	  if (may_access_global_mem (v2))
	    {
	      set_tree_flag (al, TF_MAY_ALIAS_GLOBAL_MEM);
	      set_tree_flag (v1, TF_MAY_ALIAS_GLOBAL_MEM);
	    }
	}
    }
}


/* Traverse the ALIAS_LEADERS array looking for an alias leader that may
   alias var.  If an alias leader AL is found, make AL be the alias leader
   for VAR.  Otherwise return NULL_TREE so that the caller may create a new
   entry in the ALIAS_LEADER array.  */

static tree
find_alias_leader (var)
     tree var;
{
  unsigned long i;

  for (i = 0; i < num_alias_leaders; i++)
    if (may_alias_p (alias_leaders[i], var))
      return alias_leaders[i];

  return NULL_TREE;
}


/* Return true if EXPR may be a pointer to global memory.  */

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


/* Return true if REF is a V_DEF reference for VAR.  This function handles
   relocations of pointers.  Relocating a pointer, clobbers any dereference
   of the pointer, but it does not affect any of its aliases.  For
   instance,

   	  1	p = &a			V_DEF(p) = &a
	  2	*p = 10			V_DEF(*p) = 10
	  3	p = ...			V_DEF(p) = ... => V_DEF/relocate(*p)
	  4	c = a + *p		V_DEF(c) = V_USE(a) + V_USE(*p)

   The definition of 'p' at line 2 clobbers '*p' because now 'p' points to
   a different location.  Therefore, the use of '*p' at line 3 cannot be
   reached by the assignment to '*p' at line 2.  However, relocating 'p'
   in this case should not modify 'a', therefore 'a' should still be
   reached by the assignment to '*p' at line 2.  */

bool
ref_defines (ref, var)
     tree_ref ref;
     tree var;
{
  tree rvar;

  if (ref_type (ref) != V_DEF)
    return false;
  
  rvar = ref_var (ref);

  /* If this is a relocating definition, then it only reaches VAR if both
     REF's variable and VAR are the same.  */
  if (is_relocating_def (ref))
    return (rvar == var);

  /* Otherwise, REF is a definition for VAR if either VAR and REF's
     variable are the same or if VAR is an alias for REF's variable.  */
  return (rvar == var || may_alias_p (rvar, var));
}


/* Return true if DEF is a killing definition for VAR.  Note, this assumes
   that DEF reaches the use of VAR.  */ 

bool
is_killing_def (def, var)
     tree_ref def;
     tree var;
{
  tree def_var;

  if (def == NULL)
    return false;

  def_var = ref_var (def);

  if (ref_type (def) != V_DEF && ref_type (def) != V_PHI)
    return false;

  /* Partial and potential definitions are no killers.  */
  if (is_partial_ref (def)
      || is_may_ref (def))
    return false;

  /* Common case.  Both references are for the same variable.  */
  if (def_var == var)
    return true;

#if 0
  /* If DEF_VAR must-alias VAR, then DEF is a killing definition for
     VAR.  */
  if (are_must_aliased (def_var, var))
    return true;
#endif

  return false;
}


/* Return which tree_ref structure is used by REF.  */

enum tree_ref_structure_enum
tree_ref_structure (ref)
     tree_ref ref;
{
  enum tree_ref_type type = ref_type (ref);

  if (type == V_DEF)
    return TR_VAR_DEF;
  else if (type == V_USE)
    return TR_VAR_USE;
  else if (type == V_PHI)
    return TR_VAR_PHI;
  else if (type == E_PHI)
    return TR_EXPR_PHI;
  else if (type == E_USE)
    return TR_EXPR_USE;
  else if (type == E_KILL)
    return TR_EXPR_REF_COMMON;
  else if (type == E_EXIT)
    return TR_EXPR_REF_COMMON;
  else if (type == E_LEFT)
    return TR_EXPR_REF_COMMON;

  abort ();
}


/* Remove DECL from the block that declares it.  */

void
remove_decl (decl)
     tree decl;
{
  tree *loc;
  
  loc = find_decl_location (decl, DECL_INITIAL (current_function_decl));
  if (loc)
    *loc = TREE_CHAIN (decl);
}


/* Find the location for DECL's declaration starting in BLOCK.  This
   returns an address LOC such that *LOC == DECL or NULL if DECL couldn't
   be located.  */

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


/* Return the V_DEF reference at the LHS of an assignment statement T.  */

tree_ref
output_ref (t)
     tree t;
{
  STRIP_WFL (t);
  STRIP_NOPS (t);
  if (is_assignment_stmt (t))
    {
      ref_list_iterator i;
      for (i = rli_start (tree_refs (t)); !rli_after_end (i); rli_step (&i))
	{
	  tree_ref def = rli_ref (i);
	  if (ref_type (def) == V_DEF && !def->vdef.m_clobber)
	    return def;
	}
    }

  return NULL;
}
