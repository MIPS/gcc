/* SSA-PRE for trees.
   Copyright (C) 2001, 2002, 2003 Free Software Foundation, Inc.
   Contributed by Daniel Berlin <dan@dberlin.org>

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
#include "tree.h"
#include "flags.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "errors.h"
#include "expr.h"
#include "diagnostic.h"
#include "ssa.h"
#include "varray.h"
#include "hashtab.h"
#include "splay-tree.h"
#include "ggc.h"
#include "fibheap.h"
#include "bitmap.h"
#include "splay-tree.h"
#include "tree-simple.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "tree-dump.h"
#include "timevar.h"

#define EXTRANEOUS_EPHI_REMOVAL 0
#define DEBUGGING_STRRED 1
#define DEBUGGING_SSA_UPDATE 0
#define DEBUGGING_EPHI_REMOVAL 0

/* See http://citeseer.nj.nec.com/chow97new.html, and
   http://citeseer.nj.nec.com/kennedy99partial.html for details of the
   algorithm.

   kennedy99partial is newer, and is what this implementation is based
   on.

   For strength reduction addition, see 
   http://citeseer.nj.nec.com/kennedy98strength.html

   Pieces are also taken from Open64's SSAPRE implementation.

*/
/* TODO:
   
   Commonize injuring def stuff into a seperate function (get_injuring_def or
   something). 

   Load PRE bug testing and fixing.

   Get rid of hunting through statements for refs, now that
   expressions have the refs we want as well -- This should substantially
   clean up the code, which was written when only statements had refs,
   because the expressions were not unshared.

   Remapping expression occurrences to the entire expression is a pain, and is
   only done to avoid having to grab the first operand all the time (the code
   was started before the change to include the *entire* expression in the
   ref_stmt was made).  It would clean up the code a lot and remove a lot of
   duplicated functions, whose only difference is one remaps and one doesn't.

   Strength reduction improvements.


*/
struct expr_info;
static int expr_lexically_eq PARAMS ((tree, tree));
static void free_expr_info PARAMS ((struct expr_info *));
static sbitmap compute_idfs PARAMS ((sbitmap *, tree));
static void compute_domchildren PARAMS ((dominance_info, sbitmap *));
static inline bool a_dom_b PARAMS ((basic_block, basic_block));
static void set_var_phis PARAMS ((struct expr_info *, tree_ref, int));
static inline tree_ref find_rhs_use_for_var PARAMS ((tree_ref, tree));
static inline tree_ref maybe_find_rhs_use_for_var PARAMS ((tree_ref, tree));
static inline tree_ref find_def_for_stmt PARAMS ((tree));
static void expr_phi_insertion PARAMS ((sbitmap *, struct expr_info *));
static int pre_part_1_trav PARAMS ((struct expr_info *, void *));
static int add_call_to_ei PARAMS ((struct expr_info *, void *));
static void reset_down_safe PARAMS ((tree_ref));
static void down_safety PARAMS ((struct expr_info *));
static void will_be_avail PARAMS ((struct expr_info *));
static bool call_modifies_slot PARAMS ((tree_ref, tree));
static void compute_can_be_avail PARAMS ((struct expr_info *));
static void reset_can_be_avail PARAMS ((struct expr_info *, tree_ref));
static void compute_later PARAMS ((struct expr_info *));
static void reset_later PARAMS ((struct expr_info *, tree_ref));
static void code_motion PARAMS ((struct expr_info *, tree));
static bool can_insert PARAMS ((tree_ref));
static inline bool defs_match_p PARAMS ((struct expr_info *, tree, tree));
static inline bool defs_y_dom_x PARAMS ((struct expr_info *, tree_ref, tree_ref));
static void assign_new_class PARAMS ((tree_ref, varray_type *, varray_type *));
static void insert_occ_in_preorder_dt_order_1 PARAMS ((struct expr_info *, 
						       fibheap_t, basic_block));
static void insert_occ_in_preorder_dt_order PARAMS ((struct expr_info *, 
						     fibheap_t));
static void insert_euse_in_preorder_dt_order_1 PARAMS ((struct expr_info *, 
							basic_block));
static void insert_euse_in_preorder_dt_order PARAMS ((struct expr_info *));
static inline int opnum_of_phi PARAMS ((tree_ref, int));
static varray_type phi_opnd_from_res PARAMS ((struct expr_info *, tree_ref, 
					      int, basic_block));
static splay_tree rename_2 PARAMS ((struct expr_info *, varray_type *));
static void rename_1 PARAMS ((struct expr_info *));
static void finalize_2 PARAMS ((struct expr_info *));
static bool finalize_1 PARAMS ((struct expr_info *, tree));

static tree_ref phi_operand_for_pred PARAMS ((tree_ref, edge));
static void set_save PARAMS ((struct expr_info *, tree_ref));
static void set_replacement PARAMS ((struct expr_info *, tree_ref, tree_ref));
#if 0
static bool requires_edge_placement PARAMS ((tree_ref));
#endif
static bool is_injuring_def PARAMS ((struct expr_info *, tree));
static bool is_strred_cand PARAMS ((tree));
static tree calculate_increment PARAMS ((struct expr_info *, tree));
static void repair_injury PARAMS ((struct expr_info *, tree_ref, tree, tree_ref));
#if 0
static void set_need_repair PARAMS ((struct expr_info *, tree_ref));
#endif
static void calculate_preorder PARAMS ((void));
static void update_phis_in_list PARAMS ((ref_list, tree_ref, tree_ref));
static void update_ssa_for_new_use PARAMS ((tree, tree, tree_ref, basic_block));
static int find_reaching_def_of_var PARAMS ((tree_ref, basic_block, tree_ref));
static void temp_fix_refs PARAMS ((tree, tree, tree *));
static void update_old_new PARAMS ((struct expr_info *, tree *, tree *));
static bool is_on_lhs PARAMS ((tree *, tree *));
static bool names_match_p PARAMS ((tree, tree));
static tree_ref get_injured_use PARAMS ((struct expr_info *, tree_ref, tree));
static inline bool is_a_call PARAMS ((tree));
static inline bool okay_injuring_def PARAMS ((tree_ref, tree));
static void process_left_occs_and_kills PARAMS ((varray_type, struct expr_info *,
						 tree_ref, tree));
static tree get_operand PARAMS ((tree, unsigned int));
static inline void add_left_occ PARAMS ((struct expr_info *, tree *));
static int *pre_preorder;
static dominance_info pre_idom;
static sbitmap *pre_dfs;
static FILE *dump_file;
static int dump_flags;
static int class_count;
static int preorder_count;
/*FIXME: Move into expr_info or make them go away. */
static tree_ref *avdefs;

static splay_tree new_stmt_map;
static splay_tree need_repair_map;

/* Map from pointers to statements we've replaced to pointers to where they are
now.  This is used to update the occurs list. We only need to update the occurs
list right before we create E_USE's (if we don't update them  at all, we create
E_USE's that point to the wrong place), and it would be O(n^2) to do it as we
replace statements (since we don't have something that gives us the index into
the occurs array an e_use came from, we'd have to search the entire occurs
array every time).  */
static splay_tree old_new_map;

/* sbitmap vector mapping blocks to the child blocks they dominate. */
static sbitmap *domchildren;
struct expr_info
{
  /* The actual expression.  */
  tree expr;  
  /* The occurrences. */
  varray_type occurs;
  /* The kills. */
  varray_type kills;
  /* The left occurrences. */
  varray_type lefts;
  /* An array of real occurrences. */
  varray_type reals;
  /* The ephis we've added for this expr. */
  varray_type phis;
  /* All the erefs. */
  varray_type erefs;
  /* All the refs. */
  varray_type refs;
  /* True if it's a strength reduction candidate. */
  bool strred_cand;
  /* Map of repairs we've already completed. */
  htab_t repaired;
  /* List of injury defs we need to fix reaching defs on *after* other
     insertions. */ 
  ref_list injfixups;
  /* The euses/ephis in preorder dt order. */
  ref_list euses_dt_order;
};

/* Returns true if a dominates b */
static inline bool 
a_dom_b (a, b)
     basic_block a;
     basic_block b;
{
  if (a->index == -1)
    return true;
  if (b->index == -1)
    return false;
  if (a->index == -2 && b->index != -2)
    return false;
  if (b->index == -2)
    return true;
  return dominated_by_p (pre_idom, b, a);
}


/* Compute the domchildren sbitmap vector from the list of immediate
   dominators.  */     
static void
compute_domchildren (idom, domc)
     dominance_info idom;
     sbitmap *domc;
{
  basic_block bb;
  FOR_EACH_BB (bb)
    {     
      basic_block dom;
      dom = get_immediate_dominator (idom, bb);
      if (dom && dom->index >= 0)
        SET_BIT (domc[dom->index], bb->index);
    }
}

/* Compute the iterated dominance frontier of a statement. */
static sbitmap
compute_idfs (dfs, stmt)
     sbitmap *dfs;
     tree stmt;
{
  fibheap_t worklist;
  sbitmap inworklist = sbitmap_alloc (last_basic_block);
  sbitmap idf = sbitmap_alloc (last_basic_block);
  size_t i;
  basic_block block;
  worklist = fibheap_new (); 
  sbitmap_zero (inworklist);
  sbitmap_zero (idf);
  block = bb_for_stmt (stmt);
  fibheap_insert (worklist, block->index, (void *)(size_t)block->index);
  SET_BIT (inworklist, block->index);
 
  while (!fibheap_empty (worklist))
    {
      int a = (size_t) fibheap_extract_min (worklist);
      sbitmap_a_or_b (idf, idf, dfs[a]);
      EXECUTE_IF_SET_IN_SBITMAP (dfs[a], 0, i,
      {
	if (!TEST_BIT (inworklist, i))
	  {
	    SET_BIT (inworklist, i);
	    fibheap_insert (worklist, i, (void *)i);
	  }
      });
    }
  fibheap_delete (worklist); 
  sbitmap_free (inworklist);
  return idf;
  
}
/* Return true if EXPR is a strength reduction candidate. */
static bool
is_strred_cand (expr)
     tree expr;
{
  tree_ref def, use;
  ref_list_iterator i;
  
  /* Multiplications that def a variable they use can't be strength reduced.
     Removing the multiplications would require adding more multiplications.
     IE a = a * 5. */
  if (TREE_CODE (TREE_OPERAND (expr, 1)) != MULT_EXPR )
    return false;
  
  def = use = NULL;
  for (i = rli_start (tree_refs (expr)); !rli_after_end (i); rli_step (&i))
    if (ref_type (rli_ref (i)) == V_DEF)	
	def = rli_ref (i);

  if (!def)
    return true;

  for (i = rli_start (tree_refs (expr)); !rli_after_end (i); rli_step (&i))
    {
      tree_ref ref = rli_ref (i);

      if (ref_type (ref) == V_USE)
	{
	  use = ref;
	  if (ref_var (def) == ref_var (use))
	    return false;
	}
    }
  
  return true;
}
static inline bool
okay_injuring_def (inj, var)
     tree_ref inj;
     tree var;
{
  if (!inj
      || ref_type (inj) == V_PHI
#if 0
      || !ref_defines (inj, var)
#endif
      || !maybe_find_rhs_use_for_var (inj, var))
    return false;
  return true;
}

/* Return true is INJ is an injuring definition */
static bool 
is_injuring_def (ei, inj)
     struct expr_info *ei;
     tree inj;
{
  if (!inj || !is_simple_modify_expr (inj))
    return false;
  if (TREE_CODE (TREE_OPERAND (inj, 1)) != PLUS_EXPR
      && TREE_CODE (TREE_OPERAND (inj, 1)) != MINUS_EXPR)
    return false;

  /* given inj: a1 = a2 + 5
     real: a3 + c
     we are testing:
     if (a1 != a3
     || ! (a2 exists)
     || a2 != a3)
     return false
  */
  if (TREE_OPERAND (inj, 0) != TREE_OPERAND (ei->expr, 0)
      || ! TREE_OPERAND (TREE_OPERAND (inj, 1), 0)
      || TREE_OPERAND (TREE_OPERAND (inj, 1), 0) != TREE_OPERAND (ei->expr, 0))
    return false;

  if (TREE_CODE (ei->expr) == MULT_EXPR)
    {
      tree irhs1;
      tree irhs2;
      tree irhs;
      irhs = TREE_OPERAND (inj, 1);	
      irhs1 = TREE_OPERAND (irhs, 0);
      irhs2 = TREE_OPERAND (irhs, 1);
      
      if (TREE_CODE (irhs2) != INTEGER_CST)
	return false;
      if (tree_low_cst (irhs2, 0) == 1)
	return true;
      if (really_constant_p (irhs2) 
	  && really_constant_p (TREE_OPERAND (ei->expr, 1)))
	return true;
      /* We don't support "the injury is inside a loop,expr is 
	 loop-invariant, and b is either loop-invariant or is 
	 another induction variable with respect to the loop." */
      return false;
    }
  return true;
}


/* Determine if two expressions are lexically equivalent.
   Return a value like one that would be used for qsort comparisons,
   so 0 if equal, something else otherwise.

   */
static int 
expr_lexically_eq (v1, v2)
     tree v1;
     tree v2;
{
  const tree t1 = (tree) v1;
  const tree t2 = (tree) v2;
  /* XXX: Is this too restrictive, not restrictive enough, or just right? */
  return operand_equal_p (t1, t2, 0);
}

/* Free an expression info structure.  */
static void 
free_expr_info (v1)
     struct expr_info * v1;
{
  struct expr_info *e1 = (struct expr_info *)v1;
  VARRAY_CLEAR (e1->occurs);
  VARRAY_CLEAR (e1->kills);
  VARRAY_CLEAR (e1->lefts);
  VARRAY_CLEAR (e1->reals);
  VARRAY_CLEAR (e1->phis);
  VARRAY_CLEAR (e1->erefs);
  VARRAY_CLEAR (e1->refs);
  htab_delete (e1->repaired);
  /*free (e1);*/
}

/* dfphis and varphis, from the paper. */
static sbitmap dfphis;
static sbitmap *varphis;

/* Figure out which variables are defined by PHI's, and record them so
   we can insert ExprPHI's in the right places. */

static void
set_var_phis (ei, phi, i)
     struct expr_info *ei;
     tree_ref phi;
     int i;
{
  /* 
     if (phi ! a member of varphis[i]) 
     {
        varphis[i] = varphis[i] U {block containing phi} 
        for each operand V of phi do
        if (V is defined by phi)
           set_var_phis (Phi(V), i, j)
     }
  */
  if (!TEST_BIT (varphis[i], ref_bb (phi)->index))
    {
      tree_ref phi_operand;
      size_t curr_phi_operand;
      SET_BIT (varphis[i], ref_bb (phi)->index);
      for (curr_phi_operand = 0; 
           curr_phi_operand < num_phi_args (phi); 
           curr_phi_operand++)
        {
          phi_operand = phi_arg_def (phi_arg (phi, curr_phi_operand));
	  if (ei->strred_cand && ref_type (phi_operand) != V_PHI)
	    while (is_injuring_def (ei, ref_stmt (phi_operand)))
	      {
		tree_ref ird;
		phi_operand = find_rhs_use_for_var (phi_operand, 
						    ref_var
						    (phi_operand));
		ird = imm_reaching_def (phi_operand);
		if (!okay_injuring_def (ird, ref_var (phi_operand)))
		  {
		    phi_operand = ird;
		    break;
		  }
		
		phi_operand = find_rhs_use_for_var (ird, 
						    ref_var (phi_operand));
		phi_operand = imm_reaching_def (phi_operand);
	      }
          if (ref_type (phi_operand) == V_PHI)
            set_var_phis (ei, phi_operand, i);
        }
    }
}

/* Helper macro to simplify accessing the phi for a given block. */
#define phi_at_block(expr, b) VARRAY_GENERIC_PTR ((expr)->phis, pre_preorder[(b)->index])

/* Given a statement, find the V_DEF in it. */
static inline tree_ref
find_def_for_stmt (stmt)
     tree stmt;
{
  ref_list_iterator i;
  for (i = rli_start (tree_refs (stmt)); !rli_after_end (i); rli_step (&i))
    if (ref_type (rli_ref (i)) == V_DEF && ref_var (rli_ref (i)) != global_var)
      return rli_ref (i);

  return NULL;
}

/* Given the DEF, and a VAR, find the V_USE of VAR contained in the expression
   of DEF. Abort if not found. */ 
static inline tree_ref
find_rhs_use_for_var (def, var)
     tree_ref def;
     tree var;
{
  tree_ref ret = maybe_find_rhs_use_for_var (def, var);
  if (!ret)
    abort ();
  return ret;
}

/* Given the the DEF, and a VAR, find the V_USE of the VAR contained
   in that occurrence, if there is one. */
static inline tree_ref
maybe_find_rhs_use_for_var (def, var)
     tree_ref def;
     tree var;
{
  ref_list_iterator i;

  /* Now look for the use of var in that expression. */
  i = rli_start (tree_refs (ref_stmt (def)));
  for (; !rli_after_end (i); rli_step (&i))
    {
      tree_ref ref = rli_ref (i);

      if (ref_type (ref) != V_USE
	  || !is_simple_modify_expr (ref_stmt (ref)))
        continue;
      if (ref_var (ref) != var)
        continue;
      return ref;
    }
  return NULL;
}
/* Used soley by defs_match_p to make sure we are comparing RHS uses, not LHS
   ones. 
   The GIMPLE grammar thankfully gives us a strict set of things that will
   appear on the LHS.
   Thus, we know we aren't being handed weird crap.
*/
static bool
is_on_lhs (var_p, expr_p)
     tree *var_p;
     tree *expr_p;
{
  tree expr = *expr_p;
  STRIP_NOPS (expr);
  switch (TREE_CODE (expr))
    {
    case MODIFY_EXPR:
    case INIT_EXPR:
      return is_on_lhs (var_p, &TREE_OPERAND (expr, 0));
    case COMPONENT_REF:
    case INDIRECT_REF:
    case ADDR_EXPR:
    case REFERENCE_EXPR:
      return is_on_lhs (var_p, &TREE_OPERAND (expr, 0));
    case VAR_DECL:
    case FIELD_DECL:
    case PARM_DECL:
    case FUNCTION_DECL:
      return var_p == expr_p;
    default:
      abort();
    }
}

static bool
names_match_p (x, y)
     tree x;
     tree y;
{
  bool done = false;
  while (!done)
    {
      done = true;
      switch (TREE_CODE (x))
	{
	case INDIRECT_REF:
	case ADDR_EXPR:
	  x = TREE_OPERAND (x, 0);
	  done = false;
	  break;
	case COMPONENT_REF:
	  x = TREE_OPERAND (x, 1);
	  done = false;
	  break;
	default:
	  break;
	}
    }
  
  done = false;
  while (!done)
    {
      done = true;
      switch (TREE_CODE (y))
	{
	case INDIRECT_REF:
	case ADDR_EXPR:
	  y = TREE_OPERAND (y, 0);
	  done = false;
	  break;
	case COMPONENT_REF:
	  y = TREE_OPERAND (y, 1);
	  done = false;
	  break;
	default:
	  break;
	}
    }
  
  return y == x;
}
static tree_ref
get_injured_use (ei, start, var)
     struct expr_info *ei;
     tree_ref start;
     tree var;
{
  tree_ref end = start;
#if ENABLE_CHECKING
  if (ref_type (start) != V_USE)
    abort ();
#endif
  while (is_injuring_def (ei, ref_stmt (imm_reaching_def (end))))
    {
      end = find_rhs_use_for_var (imm_reaching_def (end), var);
      if (!okay_injuring_def (imm_reaching_def (end), var))
	break;
      end = find_rhs_use_for_var (imm_reaching_def (end), var);
    }
  return end;
}
	
     
/* Determine if the definitions of variables in Y dominate the basic
   block of X. */
static inline bool 
defs_y_dom_x (ei, y, x)
     struct expr_info *ei;
     tree_ref y;
     tree_ref x;
{
  size_t i;

  /* It's a binary or unary expression, so it has only two operands at
     most. */
  for (i =  0; i < 2; i++)
    {
      ref_list_iterator j;
      tree yexpr, refexpr;
      
      yexpr = ref_stmt (y);
      yexpr = TREE_OPERAND (yexpr, 1);

      if (!get_operand (yexpr, i))
        continue;

      j = rli_start (tree_refs (ref_stmt (y)));
      for (; !rli_after_end (j); rli_step (&j))
        {
	  tree_ref ref = rli_ref (j);

          /* Find the ref for this operand. */
	  if (ref_type (ref) != V_USE)
	    continue;
	  refexpr = ref_stmt (ref);
	  if (!is_simple_modify_expr (refexpr))
            continue;
          if (!names_match_p (ref_var (ref), get_operand (yexpr, i)))
            continue;
          if (!imm_reaching_def (ref))
            return false;

	  if (ei->strred_cand)
	    ref = get_injured_use (ei, ref, get_operand (yexpr, i));

          if (!(a_dom_b (ref_bb (imm_reaching_def (ref)), ref_bb (x))))
            return false;
	  /* XXX: Honestly, i can't remember why i had this here. 
	     break;
	  */
        }
    }
  return true;
}

/* Return true if the variables in t1 have the same defs as the
   variables in t2.  */
static inline bool
defs_match_p (ei, t1, t2)
     struct expr_info *ei;
     tree t1;
     tree t2;
{
  tree_ref use2;
  tree origt2;
  ref_list_iterator i;
  
  origt2 = t2;
  t2 = TREE_OPERAND (t2, 1);
  
  for (i = rli_start (tree_refs (t1)); !rli_after_end (i); rli_step (&i))
    {
      tree_ref use1 = rli_ref (i);
      tree use1expr = ref_stmt (use1);
      if (ref_type (use1) != V_USE
	  || is_partial_use (use1)
	  || !is_simple_modify_expr (use1expr)
	  || is_on_lhs (&TREE_OPERAND (use1expr, 0), use1->common.stmt_p))
        continue;

      use1expr = TREE_OPERAND (use1expr, 1);
      use2 = maybe_find_rhs_use_for_var (find_def_for_stmt (origt2),
					 ref_var (use1));
      if (!use2 && (TREE_CODE (use1expr) != TREE_CODE (t2)))
	continue;	
      else if (!use2) 
	return false;
      /* Find the injuring definition, if one exists. */
      if (ei->strred_cand)
	use1 = get_injured_use (ei, use1, ref_var (use1));

      if (imm_reaching_def (use2) != imm_reaching_def (use1))
        return false;
    }
  return true;
}

/* Assign a new redundancy class to the occurrence, and push it on the
   stack.  */ 
static void
assign_new_class (occ, stack, stack2)
     tree_ref occ;
     varray_type *stack;
     varray_type *stack2;
{
  /* class(occ) <- count
     Push(occ, stack)
     count <- count + 1
  */
  set_exprref_class (occ, class_count);
  VARRAY_PUSH_GENERIC_PTR (*stack, occ);
  VARRAY_PUSH_GENERIC_PTR (*stack2, occ);
  class_count++;
}

/* Insert the expressions in preorder DT order in the ref_list.  */
static void
insert_euse_in_preorder_dt_order_1 (ei, block)
     struct expr_info *ei;
     basic_block block;
{
  size_t i;
  varray_type bbrefs = ei->erefs;
  for (i = 0; i < VARRAY_ACTIVE_SIZE (bbrefs); i++)
    {
      tree_ref ref = VARRAY_GENERIC_PTR (bbrefs, i);
      if (ref_bb (ref) != block)
	continue;
      if (ref_type (ref) == E_USE 
	  || ref_type (ref) == E_PHI 
	  || ref_type (ref) == E_LEFT)
	add_ref_to_list_end (ei->euses_dt_order, ref);
    }
  EXECUTE_IF_SET_IN_SBITMAP (domchildren[block->index], 0, i, 
  {
    insert_euse_in_preorder_dt_order_1 (ei, BASIC_BLOCK (i));
  });
}

static void
insert_euse_in_preorder_dt_order (ei)
     struct expr_info *ei;
{
  insert_euse_in_preorder_dt_order_1 (ei, ENTRY_BLOCK_PTR->next_bb);
}

static inline bool
is_a_call (expr)
     tree expr;
{
  STRIP_NOPS (expr);
  if (TREE_CODE (expr) == CALL_EXPR)
    return true;
  if (is_simple_modify_expr (expr) 
      && TREE_CODE (TREE_OPERAND (expr, 1)) == CALL_EXPR)
    return true;
  return false;
}
/* Insert the occurrences in preorder DT order, in the fibheap.  */
static void
insert_occ_in_preorder_dt_order_1 (ei, fh, block)
     struct expr_info *ei;
     fibheap_t fh;
     basic_block block;
{
  size_t i;
  edge succ; 
  if (phi_at_block (ei, block) != NULL)
    fibheap_insert (fh, preorder_count++, phi_at_block (ei, block));

  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->occurs); i++)
    {
      tree_ref newref;
      tree *current;
      current = VARRAY_GENERIC_PTR (ei->occurs, i);
      current = current ? current : VARRAY_GENERIC_PTR (ei->kills, i);
      current = current ? current : VARRAY_GENERIC_PTR (ei->lefts, i);
      if (bb_for_stmt (*current) != block)
        continue;

      if (VARRAY_GENERIC_PTR (ei->kills, i) != NULL)
	{
	  tree *killexpr  = VARRAY_GENERIC_PTR (ei->kills, i);
	  tree killname = ei->expr;
	  newref = create_ref (killname, E_KILL, 0, block, killexpr, 1);
	  VARRAY_PUSH_GENERIC_PTR (ei->erefs, newref);
	  fibheap_insert (fh, preorder_count++, newref);
	}
      else if (VARRAY_GENERIC_PTR (ei->lefts, i) != NULL)
	{
	  tree *leftexpr = VARRAY_GENERIC_PTR (ei->lefts, i);
	  tree leftname = ei->expr;
	  newref = create_ref (leftname, E_LEFT, 0, block, leftexpr, 1);
	  VARRAY_PUSH_GENERIC_PTR (ei->erefs, newref);
	  fibheap_insert (fh, preorder_count++, newref);
	}
      else
	{
	  tree *occurexpr = VARRAY_GENERIC_PTR (ei->occurs, i);
	  tree occurname;
	  occurname = ei->expr;
	  newref = create_ref (occurname, E_USE, 0, block, occurexpr, 1);
	  VARRAY_PUSH_GENERIC_PTR (ei->erefs, newref);
	  set_expruse_def (newref, NULL);
	  set_exprref_class (newref, 0);
	  set_expruse_phiop (newref, false);
	  set_exprref_processed (newref, false);
	  set_exprref_processed2 (newref, false);
	  set_expruse_has_real_use (newref, false);
	  fibheap_insert (fh, preorder_count++, newref);
	}     
    }
 
  /* Insert the phi operand occurrences's in the heap at the
     successors.*/
  for (succ = block->succ; succ; succ = succ->succ_next)
    {
      if (succ->dest != EXIT_BLOCK_PTR)
        {
          if (phi_at_block (ei, succ->dest) != NULL)
            {
              tree_ref newref = create_ref (NULL, E_USE, 0, block, 0, 1);
              tree_ref phi = phi_at_block (ei, succ->dest);
	      VARRAY_PUSH_GENERIC_PTR (ei->erefs, newref);
              set_expruse_def (newref, NULL);
              set_exprref_class (newref, 0);
              set_expruse_phiop (newref, true);
	      set_expruse_phi (newref, phi);
              set_expruse_has_real_use (newref, false);
	      set_exprref_save (newref, false);
	      set_exprref_reload (newref, false);
	      set_exprref_inserted (newref, false);
	      set_exprref_processed (newref, false);
	      set_exprref_processed2 (newref, false);	 
	      add_ephi_arg (phi, newref, succ);
              fibheap_insert (fh, preorder_count++, newref);
            }
        }      
      else
        {
#if 0
	  /* No point in inserting exit blocks into heap first, since
	     they'll never be anything on the stack. */
	  if (preorder_count != 0)
	    {
	      tree_ref newref;
	      newref = create_ref (NULL, E_EXIT, 0, block, NULL, 1);
	      VARRAY_PUSH_GENERIC_PTR (ei->erefs, newref);
	      fibheap_insert (fh, preorder_count++, newref);
	    }
#endif
        }
          
    }
  EXECUTE_IF_SET_IN_SBITMAP (domchildren[block->index], 0, i, 
  {
    insert_occ_in_preorder_dt_order_1 (ei, fh, BASIC_BLOCK (i));
  });
}

static void
insert_occ_in_preorder_dt_order (ei, fh)
     struct expr_info *ei;
     fibheap_t fh;
{
  preorder_count = 0;
  insert_occ_in_preorder_dt_order_1 (ei, fh, ENTRY_BLOCK_PTR->next_bb);
  /* No point in inserting exit blocks into heap first, since
     they'll never be anything on the stack. */
  if (preorder_count != 0)
    {
      tree_ref newref;
      newref = create_ref (NULL, E_EXIT, 0, EXIT_BLOCK_PTR, NULL, 0);
      VARRAY_PUSH_GENERIC_PTR (ei->erefs, newref);
      fibheap_insert (fh, preorder_count++, newref);
    }
}

/* Determine the phi operand index for J, for PHI.  */
static inline int
opnum_of_phi (phi, j)
     tree_ref phi;
     int j;
{
  size_t i;
  /* We can't just count predecessors, since tree-ssa.c generates them
     when it sees a phi in the successor during it's traversal.  So the
     order is dependent on the traversal order.  */
  for (i = 0 ; i < num_phi_args (phi); i++)
    if (phi_arg_edge (phi_arg (phi, i))->src->index == j)
      return i;
  
  abort();
}

/* Factor variables through phi operands.  */
static varray_type
phi_opnd_from_res (ei, Z, j, b)
     struct expr_info *ei;
     tree_ref Z;
     int j;
     basic_block b;
{
  varray_type Q;
  size_t i = 0;
  ref_list_iterator k;

  VARRAY_GENERIC_PTR_INIT (Q, 1, "Temp ops");

  /* b <- block containing phi that defines Z.
     Q <- copy of Z */
  k = rli_start (tree_refs (ref_stmt (Z)));
  for (; !rli_after_end (k); rli_step (&k))
    VARRAY_PUSH_GENERIC_PTR (Q, rli_ref (k));

  /* For each variable v in Z */
  k = rli_start (tree_refs (ref_stmt (Z)));
  for (; !rli_after_end (k); rli_step (&k))
    {
      tree_ref v = rli_ref (k);
      if (ref_type (v) == V_USE)
	if (ei->strred_cand)
	  v = get_injured_use (ei, v, ref_var (v));

      /* If v is defined by phi in b */
      if (ref_type (v) == V_USE
          && ref_type (imm_reaching_def (v)) == V_PHI)
        {
          tree_ref phi = imm_reaching_def (v);
          if (ref_bb (phi) == b)
	    {
	      /* replace v in Q by j'th operand of v's phi */
              int opnum = opnum_of_phi (phi, j);
              tree_ref op = phi_arg_def (phi_arg (phi, opnum));
              VARRAY_GENERIC_PTR (Q, i) = op;
            }
          
        }
      else if (ref_type (v) == V_DEF 
               || ref_type (v) == V_PHI
               || ref_type (v) == E_USE)
	VARRAY_GENERIC_PTR (Q, i) = NULL;
      i++;
    }
  return Q;
}

/* Second pass of delayed renaming algorithm.  */
static splay_tree
rename_2 (ei, rename2_set)
     struct expr_info *ei;
     varray_type *rename2_set;
{
  size_t i;
  splay_tree touched_set;

  /* Keep track of which phi operands we touch.  */
  touched_set = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);

  /* For each exprphi f in the program do
	for each operand w of f do
	  processed (w) = false.  

	  This was done earlier, i'm just including the comment for reference
	  purposes. */
  /* while (set_for_rename2 not empty) do { */
  while (VARRAY_ACTIVE_SIZE (*rename2_set) > 0)
    {
      tree_ref Z; 
      tree_ref phiZ;
      size_t curr_phiop;

      /* Remove real occurrence Z from set_for_rename2.  */
      Z = VARRAY_TOP_GENERIC_PTR (*rename2_set);
      VARRAY_POP (*rename2_set);

      splay_tree_insert (touched_set, (splay_tree_key) Z, 0);
      
      /* f <- phi that defines Z. */
      phiZ = expruse_def (Z);
      
      for (curr_phiop = 0; curr_phiop < num_ephi_args (phiZ); curr_phiop++)
	{
          tree_ref w;
	  i = phi_arg_edge (ephi_arg (phiZ, curr_phiop))->src->index;
	  w = phi_arg_def (ephi_arg (phiZ, curr_phiop));
	  
	  /* if (not processed(w)) { */
          if (!exprref_processed (w))
            {
              /* j = i because we lookup the real index in the
                 function. */
              int j = i;
              size_t k;
              varray_type Y;
              tree_ref X;
	      
	      /* j <- index of w in f
   		 Y <- phi_operand_from_res (Z, j)
		 X <- def(w)  */     
              Y = phi_opnd_from_res (ei, Z, j, ref_bb (phiZ));
              X = expruse_def (w);
	      
	      if (!X)
	        {
	          VARRAY_CLEAR (Y);
		  continue;
	        }

	      /* if (X is a real occurrence) */
	      if (ref_type (X) == E_USE || ref_type (X) == E_LEFT)
                {
                  bool match = true;
		  ref_list_iterator l;
		  
		  /* if (all corresponding variables in Y and X have
		     the same SSA version) */
		  k = 0;
		  l = rli_start (tree_refs (ref_stmt (X)));
		  for (; !rli_after_end (l); rli_step (&l))
		  {
		    tree_ref op1 = rli_ref (l);
		    tree_ref op2;
		    if (ref_type (op1) != V_USE)
		      continue;
		    if (ei->strred_cand)
		      op1 = get_injured_use (ei, op1, ref_var (op1));

		    if (ref_type (op1) == V_USE)
		      op1 = imm_reaching_def (op1);

		    op2 = VARRAY_GENERIC_PTR (Y, k);
		    
		    if (op2 && ref_type (op2) == V_USE)
		      op2 = imm_reaching_def (op2);

		    if (ei->strred_cand)
		      while (is_injuring_def (ei, ref_stmt (op2)))
			{
			  tree_ref ird;
			  op2 = find_rhs_use_for_var (op2, ref_var
						      (op2));
			  ird = imm_reaching_def (op2);
			  if (!okay_injuring_def (ird, ref_var (op2)))
			    {
			      op2 = ird;
			      break;
			    }
			  
			  op2 = find_rhs_use_for_var (imm_reaching_def (op2), 
						      ref_var (op2)) ;
			  op2 = imm_reaching_def (op2);
			}
		    if (op2 && ref_type (op2) == V_USE)
		      op2 = imm_reaching_def (op2);
		    
		    if ((op1 && op2 && op1 != op2
			 && ref_var (op1) == ref_var (op2))
			|| (op1 && !op2) 
			|| (!op1 && op2))
		      {
			match = false;
			break;
		      }
		    k++;
		  }
                  if (!match)
                    {		
		      /* def(w) <- tack  */
                      set_expruse_def (w, NULL);
		      set_expruse_has_real_use (w, false);
                    } 
		  else
		    {	
		      /* no change needed  */
		      set_expruse_has_real_use (w, true);
		    }
                }
              else /* X is a ExprPhi occurrence */
                {
                  bool match = true;

		  /* if (definitions of all variables in Y dominate X) */
                  for (k = 0; k < VARRAY_ACTIVE_SIZE (Y); k++)
                    {
                      tree_ref ref = VARRAY_GENERIC_PTR (Y, k);
                      if (!ref)
                        continue;
                      if (ref_type (ref) == V_USE)
                        ref = imm_reaching_def (ref);
                      
                      if (!a_dom_b (ref_bb (ref), ref_bb (X)))
                        {
                          match = false;
                          break;
                        }
                    }
                  if (match)
                    { 
		      /* No change needed. 
			 set_for_rename2 <- set_for_rename2 U {Y} */
                      VARRAY_PUSH_GENERIC_PTR (*rename2_set, Z);
                    }
                  else
                    {
		      /* def(w) <- tack */
		      set_expruse_def (w, NULL);
                      set_exprphi_downsafe (phiZ, false);
                    }
                }
	      /* processed (w) <- true */
	      set_exprref_processed (w, true);
              VARRAY_CLEAR (Y); 
            }
        }
    }
  return touched_set;
}

/* First pass of delayed renaming algorithm.  */
static void
rename_1 (ei)
     struct expr_info *ei;
{
  fibheap_t fh = fibheap_new ();
  tree_ref y;
  varray_type stack;
  varray_type stack2;
  varray_type rename2_set;
  varray_type recheck_set;
  splay_tree touched_set;
  size_t i;
  
  /* count <- 0
     stack <- empty
     set_for_rename2 <- {} */
  VARRAY_GENERIC_PTR_INIT (stack, 1, "Stack for renaming");
  VARRAY_GENERIC_PTR_INIT (stack2, 1, "Stack for downsafety");
  VARRAY_GENERIC_PTR_INIT (rename2_set, 1, "Rename2 set");
  VARRAY_GENERIC_PTR_INIT (recheck_set, 1, "Recheck set");
  class_count = 1;
  
 /* for each occurrence Y in the current expression in preorder DT
     traversal order do { */
  insert_occ_in_preorder_dt_order (ei, fh);
  while (!fibheap_empty (fh))
    {
      y = fibheap_extract_min (fh);
      /* while (Top(stack) does not dominate Y) do 
	 Pop(stack)  */
      while (VARRAY_ACTIVE_SIZE (stack) > 0)
        {
          tree_ref stackref = VARRAY_TOP_GENERIC_PTR (stack);
          if (a_dom_b (ref_bb (stackref), ref_bb (y)))
            break;           
          VARRAY_POP (stack);
        }
      while (VARRAY_ACTIVE_SIZE (stack2) > 0)
        {
          tree_ref stackref = VARRAY_TOP_GENERIC_PTR (stack2);
          if (a_dom_b (ref_bb (stackref), ref_bb (y)))
            break;           
          VARRAY_POP (stack2);
        }
      if (ref_type (y) == E_EXIT)
        {
	  if (VARRAY_ACTIVE_SIZE (stack2) > 0)
            {
              tree_ref stackref = VARRAY_TOP_GENERIC_PTR (stack2);
              if (ref_type (stackref) == E_PHI)
                set_exprphi_downsafe (stackref, false);
            }
          continue;
        }
      /* if (Y is an exprphi occurrence)
           assign_new_class (Y)  */
      if (ref_type (y) == E_PHI)
        {
	  assign_new_class (y, &stack, &stack2);
        }
      /* else if (Y is a real occurrence) */
      else if (ref_type (y) == E_USE && expruse_phiop (y) == false)
        {
	  /* if (stack is empty)
	       Assign_New_Class(Y) */
          if (VARRAY_ACTIVE_SIZE (stack) == 0)
            {
              assign_new_class ( y, &stack, &stack2);
            }
          /* else */
          else
            {
	      /* X <- Top(stack) */
              tree_ref x = VARRAY_TOP_GENERIC_PTR (stack);
	      
              /* If (X is a real occurrence or a left occurrence) */
              if (ref_type (x) == E_USE || ref_type (x) == E_LEFT)
                {
                  /* If (all corresponding variables in Y and X have
                     the same SSA version) */
                  if (defs_match_p (ei, ref_stmt (y), ref_stmt (x)))
                    {
                      /* class(Y) <- class(X)
                         def(Y) <- X */
                      set_exprref_class (y, exprref_class (x));
#if 1
		      set_expruse_def (y, x);
#else
		      set_expruse_def (y, expruse_def (x) != NULL ? expruse_def (x) : x);
#endif
		      VARRAY_PUSH_GENERIC_PTR (stack2, y);
                    }
		  /* else Assign_New_Class (Y) */
                  else
                    {
                      assign_new_class ( y, &stack, &stack2);
                    }
                }
	      else if (ref_type (x) == E_KILL)
		{
		  assign_new_class (y, &stack, &stack2);
		}
              else /* X is a ExprPhi occurrence. */
                {
#if ENABLE_CHECKING
                  if (ref_type (x) != E_PHI)
                    abort();
#endif
		  /* if (definitions of all variables in Y dominate X)
		     { */
		  if (defs_y_dom_x (ei, y, x))
                    {
                      /* class(Y) <- class(X)
                         def(Y) <- X
                         set_for_rename2 <- set_for_rename2 U {Y} */
                      set_exprref_class (y, exprref_class (x));
                      set_expruse_def (y, x);


/* Open64 does this, and without it, we prevent optimization on the
   following code:


        int a;
        int b;
        b = 5;
        int e = 0;
        int f = 0;
        if (argc)
        {
                a = 3;
                e = a + b;
        }
        else
        {
                a = 4;
        }
        f = a + b;

Without pushing here, the expr-phi that appears right before f = a + b
will be at the top of stack when we hit the exit node. Thus, it will
be marked *not* down safe.  This prevents optimization from occurring
anywhere.

Clearly, however, a + b is partially redundant, and the phi *will* be
downsafe at the end of PRE, because we will insert an evaluation right
after a = 4;
There is provably no harm in pushing anyway, because if the pushed value
doesn't dominate whatever  occurrences we see while it's at the top,
we'll pop it, and if the phi isn't really going to be downsafe, we'll
catch it in rename_2 or during downsafety propagation. */

		      VARRAY_PUSH_GENERIC_PTR (stack2, y);
		      VARRAY_PUSH_GENERIC_PTR (rename2_set, y);
                    }
                  else
                    {
		      set_exprphi_downsafe (x, false);
                      assign_new_class ( y, &stack, &stack2);
                    }
                } 
            } 
        }
      /* else if (Y is an exprphi operand occurrence) */
      else if (ref_type (y) == E_USE && expruse_phiop (y) == true)
        {
          if (VARRAY_ACTIVE_SIZE (stack) == 0)
            {
              /* def(Y) <- tack */
	      set_expruse_def (y, NULL);
	      set_expruse_has_real_use (y, false);
	      set_exprref_processed (y, true);
            }
          else
            {
              /* X = Top(stack)
                 class(Y) <- class(X)
                 def(Y) <- X */
              tree_ref x = VARRAY_TOP_GENERIC_PTR (stack);
	      tree_ref x2 = VARRAY_TOP_GENERIC_PTR (stack2);
	      if (ref_type (x) == E_KILL)
		{		  
		  set_expruse_def (y, NULL);
		  set_exprref_processed (y, true);
		  if (ref_type (x2) == E_PHI)
		    {
		      set_exprphi_downsafe (x2, false);
		    }
		}
	      else
		{
		  
		  set_exprref_class (y, exprref_class (x));
		  set_expruse_def (y, x);
		  if ((ref_type (x2) == E_USE && !expruse_phiop (x2)) 
		      || ref_type (x2) == E_LEFT)
		    {
		      set_expruse_has_real_use (y, true);       	      
#if 0
		      if (expruse_def (x) && ref_type (expruse_def (x)) == E_PHI)
			VARRAY_PUSH_GENERIC_PTR (rename2_set, x);
#endif
		    }
		  else
		    {
		      set_expruse_has_real_use (y, false);
		      VARRAY_PUSH_GENERIC_PTR (recheck_set, y);
		    }
		}
	    }
        }
      else if (ref_type (y) == E_KILL)
	{
	  VARRAY_PUSH_GENERIC_PTR (stack, y);
	}
      else if (ref_type (y) == E_LEFT)
	{
	  assign_new_class (y, &stack, &stack2);
	}
      else
        abort();
    }
  /* This is based on Open64, rather than something in the paper. 
     If phi_result, phi_operand is still delayed, there must be no
     real occurrence between them, and thus, it's okay to set
     downsafe.
  */
  touched_set = rename_2 (ei, &rename2_set);
  for (i = 0; i < VARRAY_ACTIVE_SIZE (recheck_set); i++)
    {
      
      if (splay_tree_lookup (touched_set, (splay_tree_key)
			     VARRAY_GENERIC_PTR (recheck_set, i)) != NULL)
	VARRAY_GENERIC_PTR (recheck_set, i) = NULL;
    }

  for (i = 0; i < VARRAY_ACTIVE_SIZE (recheck_set); i++)
    {
      tree_ref ref = VARRAY_GENERIC_PTR (recheck_set, i);
      tree_ref def;

      if (!ref)
	continue;

      def = expruse_def (ref);
      if (exprref_processed (ref))
	continue;

      if (def && (ref_type (def) == E_PHI))
	set_exprphi_downsafe (def, false);

      set_expruse_def (ref, NULL);
    }
  fibheap_delete (fh);
  VARRAY_CLEAR (stack);
  VARRAY_CLEAR (stack2);
  VARRAY_CLEAR (rename2_set);
  VARRAY_CLEAR (recheck_set);
  splay_tree_delete (touched_set);
}

static tree_ref
phi_operand_for_pred (phi, e)
     tree_ref phi;
     edge e;
{
  size_t i;
  for (i = 0; i < num_ephi_args (phi); i++)
    if (phi_arg_edge (ephi_arg (phi, i)) == e)
      return phi_arg_def (ephi_arg (phi, i));
  abort ();
}
     
/* Figure out which expressions need to be saved.  */
static void
set_save (ei, X)
     struct expr_info *ei;
     tree_ref X;
{
  if ((ref_type (X) == E_USE && !expruse_phiop (X))
      || ref_type (X) == E_LEFT)
    set_exprref_save (X, true);
  else if (ref_type (X) == E_PHI)
    {
      int i;
      size_t curr_phiop;
      for (curr_phiop = 0; curr_phiop < num_ephi_args (X); curr_phiop++)
	{
	  tree_ref w = phi_arg_def (ephi_arg (X, curr_phiop));
	  i = phi_arg_edge (ephi_arg (X, curr_phiop))->src->index;

	  if (!exprref_processed2 (w))
	    {
	      set_exprref_processed2 (w, true);
	      set_save (ei, expruse_def (w));
	    }
	}
    }
#if EXTRANEOUS_EPHI_REMOVAL 
  if ((ref_type (X) == E_USE && !expruse_phiop (X))
      || ref_type (X) == E_LEFT)
    {
      sbitmap idfs = compute_idfs (pre_dfs, ref_stmt (X));
      int i;
      EXECUTE_IF_SET_IN_SBITMAP (idfs, 0, i,
      {
	tree_ref phi = phi_at_block (ei, BASIC_BLOCK (i));
	if (phi != NULL && exprphi_willbeavail (phi))
	  set_exprphi_extraneous (phi, false);
      });	  
      sbitmap_free (idfs);
    }
#endif
}

/* Set replacement for ExprPhi minimization.  */
static void
set_replacement (ei, g, replacing_def)
     struct expr_info *ei;
     tree_ref g;
     tree_ref replacing_def;
{
  ref_list_iterator rli;
  
  rli = rli_start (exprref_uses (g));
  for (; !rli_after_end (rli); rli_step (&rli))
    {
      tree_ref x = rli_ref (rli);
      
      if (ref_type (x) == E_USE && expruse_phiop (x))
	{
	  tree_ref f = expruse_phi (x);
	  if (exprphi_extraneous (f) && !exprref_processed (f))
	    {
	      set_exprref_processed (f, true);
	      set_replacement (ei, f, replacing_def);
	    }
	  else if (!exprphi_extraneous (f))
	    {
#if DEBUGGING_EPHI_REMOVAL 
	      if (dump_file)
		fprintf (dump_file, "Replacing id %lu with id %lu\n", ref_id (x), ref_id (replacing_def));
#endif
	      set_exprref_class (x, exprref_class (replacing_def));
	      set_expruse_def (x, replacing_def);
	    }
	}
    }
  rli = rli_start (exprref_uses (g));
  for (; !rli_after_end (rli); rli_step (&rli))
    {
      tree_ref x = rli_ref (rli);
      if (ref_type (x) == E_USE && !expruse_phiop (x) 
	  && exprref_reload (x))
	{
	  set_exprref_class (x, exprref_class (replacing_def));
	  set_expruse_def (x, replacing_def);
	}
    }

#if DEBUGGING_EPHI_REMOVAL
      if (dump_file)
	fprintf (dump_file, "Removing E_PHI id %lu\n", ref_id (g));
#endif
  phi_at_block (ei, ref_bb (g)) = NULL;
  remove_ref_from_list (ei->euses_dt_order, g);
}

/* Second part of finalize algorithm.  */
static void 
finalize_2 (ei)
     struct expr_info *ei;
{
  size_t i;
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->erefs); i++)
    {
      tree_ref ref = VARRAY_GENERIC_PTR (ei->erefs, i);
      if (ref_type (ref) == E_PHI)
	{
	  if (exprphi_willbeavail (ref))
	    set_exprphi_extraneous (ref, true);
	}
      else if (ref_type (ref) == E_USE 
	       && expruse_phiop (ref) == false)
	{
	  set_exprref_save (ref, false);
	}
    }  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->erefs); i++)
    {
      tree_ref ref = VARRAY_GENERIC_PTR (ei->erefs, i);
      if (ref_type (ref) == E_USE 
	  && expruse_phiop (ref) == false 
	  && !exprref_inserted (ref))
	{
	  if (exprref_reload (ref))
	    set_save (ei, expruse_def (ref));
	}
    }
  /* This is pointless unless we plan on performing more ESSA passes. */
#if EXTRANEOUS_EPHI_REMOVAL 
  for (i = 0; i < n_basic_blocks; i++)
    {
      tree_ref ref = phi_at_block (ei, BASIC_BLOCK (i));
      if (!ref)
	continue;     
      if (exprphi_willbeavail (ref))
	{
	  int k;
	  for (k = 0; k < num_ephi_args (ref); k++)
	    {
	      tree_ref w = phi_arg_def (ephi_arg (ref, k));
	      tree_ref defw;
	      if (!w || !expruse_def (w) )
		continue;
	      defw = expruse_def (w);
	      if ((ref_type (defw) == E_PHI && !exprphi_extraneous (defw))
		  || (ref_type (defw) == E_USE && !expruse_phiop (defw)) 
		  || ref_type (defw) == E_LEFT)
		set_replacement (ei, ref, expruse_def (w));  
	    }
	}
      else
	{
	  phi_at_block (ei, ref_bb (ref)) = NULL;
	}
    }
#endif
}
static void
update_old_new (ei, old, new)
     struct expr_info *ei;
     tree *old;
     tree *new;
{
  splay_tree_node result;
  
  result = splay_tree_lookup (old_new_map, (splay_tree_key)old);
  if (result)
    {
      size_t i;
      for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->occurs); i++)
	{
	  if (VARRAY_GENERIC_PTR (ei->occurs, i) == old)
	    VARRAY_GENERIC_PTR (ei->occurs, i) = (PTR) result->value;
	}
      for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->kills); i++)
	{
	  if (VARRAY_GENERIC_PTR (ei->kills, i) == old)
	    VARRAY_GENERIC_PTR (ei->kills, i) = (PTR) result->value;
	}
      for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->lefts); i++)
	{
	  if (VARRAY_GENERIC_PTR (ei->lefts, i) == old)
	    VARRAY_GENERIC_PTR (ei->lefts, i) = (PTR) result->value;
	}
    }
  splay_tree_insert (old_new_map, (splay_tree_key) old, (splay_tree_value) new);
}
/* This routine is a temporary thing. It is meant to fix up refs that *MUST* be
   fixed up in order for the algorithm to continue to work properly (as opposed
   to  those we fix up in order to update the SSA representation).  Their will
   be  routines that do this for us eventually. */
static void
temp_fix_refs (lookin, lookfor, replacewith)
     tree lookin;
     tree lookfor;
     tree *replacewith;
{
  ref_list_iterator rli;

  rli = rli_start (tree_refs (lookin));
  for (; !rli_after_end (rli); rli_step (&rli))
    {
      tree_ref ref = rli_ref (rli);
      if (ref_stmt (ref) == lookfor)
	ref->common.stmt_p = replacewith;
    }
}


/* First part of finalize algorithm.  */
static bool
finalize_1 (ei, temp)
     struct expr_info *ei;
     tree temp;
{
  tree_ref X;
  int x;
  ref_list_iterator euse_rli;
  bool made_a_reload = false;
  
  avdefs = xcalloc (class_count + 1,sizeof (tree_ref));  
  ei->euses_dt_order = create_ref_list ();
  
  insert_euse_in_preorder_dt_order (ei);
  
  euse_rli = rli_start (ei->euses_dt_order);
  for (; !rli_after_end (euse_rli); rli_step (&euse_rli))
    {
      X = rli_ref (euse_rli);
      x = exprref_class (X);
      if (ref_type (X) == E_PHI)
        {
          if (exprphi_willbeavail (X))
            avdefs[x] = X;
        }
      else if (ref_type (X) == E_LEFT)
	{
	  avdefs[x] = X;
	}
      else if (ref_type (X) == E_USE && expruse_phiop (X) == false)
        {
          if (avdefs[x] == NULL 
              || !a_dom_b (ref_bb (avdefs[x]), ref_bb (X)))
            {
	      set_exprref_reload (X, false);
              avdefs[x] = X;
            }
          else
            {
	      set_exprref_reload (X, true);
	      made_a_reload = true;
              set_expruse_def (X, avdefs[x]);
            }
        }
      else
        {
          tree_ref phi = expruse_phi (X);
          if (!phi)
            abort ();
          if (exprphi_willbeavail (phi))           
            {
              if (can_insert (X))
                {
		  tree stmt;
		  tree expr;
		  tree *newexprplace;
		  tree *otherexprplace;
		  basic_block bb;
		  tree endtree;
		  tree *endtreep;
		  tree copy;

                  /* Insert definition of expr at end of BB containing X. */
		  if (dump_file)
		    {
		      fprintf (dump_file, "In BB %d, insert save of ",
			       ref_bb (X)->index);
		      print_generic_expr (dump_file, ei->expr, 0);
		      fprintf (dump_file, " to ");
		      print_generic_expr (dump_file, temp, 0);
		      fprintf (dump_file, " after ");
		      print_generic_stmt (dump_file, last_stmt (ref_bb (X)),
			                  dump_flags);
		      fprintf (dump_file, 
			       " (at end of BB), because of ExprPhi");
		      fprintf (dump_file, " in BB %d\n", 
			       ref_bb (phi)->index);
		    }
		  copy = ei->expr;
		  walk_tree (&copy, copy_tree_r, NULL, NULL);
		  expr = fold (build (INIT_EXPR, TREE_TYPE (temp), temp, copy));
		  set_bb_for_stmt (expr, ref_bb (X));
		  bb = ref_bb (X);
		  endtree = last_stmt (bb);
		  endtreep = last_stmt_ptr (bb);
		  /* FIXME: Need DFA inserting updating to do this the right
		     way. */
		  /* If it's a goto, we need to insert *before* it.
		     This might not be necessary once goto elimination
		     is functional.  
		     If it's an empty statement, always insert before. */
		  if (!endtree)
		    {
		      stmt = build (COMPOUND_EXPR, void_type_node,
				    expr, empty_stmt_node);
		      newexprplace = &TREE_OPERAND (stmt, 0);
		      otherexprplace = &TREE_OPERAND (stmt, 1);
		    }
		  else if (is_ctrl_altering_stmt (endtree))
		    {
		      stmt = build (COMPOUND_EXPR, void_type_node, 
				    expr, endtree);
		      newexprplace = &TREE_OPERAND (stmt, 0);
		      otherexprplace = &TREE_OPERAND (stmt, 1);
		    }
		  else
		    {
		      stmt = build (COMPOUND_EXPR, void_type_node, 
				    endtree, expr);
		      newexprplace = &TREE_OPERAND (stmt, 1);
		      otherexprplace = &TREE_OPERAND (stmt, 0);
		    }
		  /* FIXME: This can't be fixed without the insertion machinery
		     knowing what to do about it. */
		  if (!endtree)
		    abort ();
		  else
		    {
		      *endtreep = stmt;
		      update_old_new (ei, endtreep, otherexprplace);
		      /* REMOVE AFTER DFA UPDATE */
		      temp_fix_refs (*otherexprplace, stmt, otherexprplace);
		      /* END REMOVE AFTER DFA UPDATE */
		    }
		  set_bb_for_stmt (stmt, bb);
		  
                  /*
                    expruse_def (X) = new occurrence. 
		  */		  

		  set_expruse_def (X,create_ref (expr, E_USE, 0, ref_bb (X),
			                         newexprplace, 1));
		  set_bb_for_stmt (*newexprplace, ref_bb (X));
		  
		  VARRAY_PUSH_GENERIC_PTR (ei->erefs, expruse_def (X));
		  set_exprref_reload (expruse_def (X), false);
		  set_exprref_save (expruse_def (X), true);
		  set_exprref_inserted (expruse_def (X), true);
		  set_expruse_phiop (expruse_def (X), false);
		  set_expruse_has_real_use (expruse_def (X), true);
		  set_exprref_processed (X, false);
		  set_exprref_processed2 (X, false);
		  set_exprref_save (X, false);
		  set_exprref_reload (X, false);
                }
              else
                {
                  set_expruse_def (X, avdefs[x]); 
                }
            }
        }
    }
  return made_a_reload;
}

/* ExprPhi insertion algorithm.  */
static void
expr_phi_insertion (dfs, ei)
     sbitmap *dfs;
     struct expr_info *ei;
{
  size_t i;
  dfphis = sbitmap_alloc (last_basic_block);
  sbitmap_zero (dfphis);
  varphis = sbitmap_vector_alloc (2, last_basic_block);
  sbitmap_vector_zero (varphis, 2);  

  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->occurs); i++)
    {
      tree *occurp = VARRAY_GENERIC_PTR (ei->occurs, i);
      tree occur = occurp ? *occurp : NULL;
      tree *killp = VARRAY_GENERIC_PTR (ei->kills, i);
      tree kill = killp ? *killp : NULL;
      tree *leftp = VARRAY_GENERIC_PTR (ei->lefts, i);
      tree left = leftp ? *leftp : NULL;
      sbitmap temp;

#if ENABLE_CHECKING
      if ((kill && occur) || (left && occur) || (kill && left))
	abort();
#endif
      occurp = occur ? occurp : kill ? killp : leftp;      
      occur = occur ? occur : kill ? kill : left;
      temp = compute_idfs (dfs, occur);
      sbitmap_a_or_b (dfphis, dfphis, temp);
      sbitmap_free (temp);
      if (kill != NULL)
	continue;
      occur = TREE_OPERAND (occur, 1);
      if (is_simple_id (get_operand (occur, 0))
	  || (get_operand (occur, 1) 
	      && is_simple_id (get_operand (occur, 1))))
        {
          int varcount = 0;
	  ref_list_iterator j;
	  
	  j = rli_start (tree_refs (*occurp));
	  for (; !rli_after_end (j); rli_step (&j))
	    {
	      tree_ref ref = rli_ref (j);
	      if (ei->strred_cand)
		if (ref_type (ref) == V_USE
		    && names_match_p (ref_var (ref), get_operand (occur, 0))
		    && imm_reaching_def (ref))
		  ref = get_injured_use (ei, ref, ref_var (ref));
	      /* ??? If the trees aren't shared, will the last part of this ||
		 work right? */ 
	      if (ref_type (ref) != V_USE
		  || !is_simple_modify_expr (ref_stmt (ref))
		  /*|| TREE_OPERAND (ref_stmt (ref), 1) != real*/)
		continue;
	      if (ref_var (ref) != get_operand (occur, 0)
		  && (get_operand (occur, 1) == NULL 
		      || ref_var (ref) != get_operand (occur, 1)))
		continue; 
	      if (!imm_reaching_def (ref)
		  || ref_type (imm_reaching_def (ref)) != V_PHI)
		continue;
	      set_var_phis (ei, imm_reaching_def (ref), varcount++);
	    }
        }
    }
  sbitmap_a_or_b (dfphis, dfphis, varphis[0]);
  sbitmap_a_or_b (dfphis, dfphis, varphis[1]);

  EXECUTE_IF_SET_IN_SBITMAP(dfphis, 0, i, 
  {
    tree_ref ref = create_ref (ei->expr, E_PHI, 0, BASIC_BLOCK (i), NULL, 1);
    VARRAY_PUSH_GENERIC_PTR (ei->erefs, ref);
    set_exprref_processed (ref, false);
    set_exprref_processed2 (ref, false);
    set_exprphi_downsafe (ref, true);
    set_exprphi_canbeavail (ref, true);
    set_exprphi_later (ref, true);
    set_exprphi_extraneous (ref, true);
    VARRAY_GENERIC_PTR (ei->phis, pre_preorder[i]) = ref;
  });
  sbitmap_free (dfphis);
  sbitmap_vector_free (varphis);
}

/* Reset down safety flags.  */
static void 
reset_down_safe (phiop)
     tree_ref phiop;
{
  tree_ref phi;
  size_t i;
  
  if (expruse_has_real_use (phiop))
    return;
  phi = expruse_def (phiop);
  if (!phi || ref_type (phi) != E_PHI)
    return;
  if (!exprphi_downsafe (phi))
    return;  
  set_exprphi_downsafe (phi, false);
  for (i = 0; i < num_ephi_args (phi); i++)
    reset_down_safe (phi_arg_def (ephi_arg (phi, i)));
}

/* Compute down_safety.  */
static void
down_safety (ei)
     struct expr_info *ei;
{
  basic_block block;
  FOR_EACH_BB (block)
    {
      tree_ref phi = phi_at_block (ei, block);
      size_t i;
      
      if (phi == NULL || exprphi_downsafe (phi))
        continue;
      for (i = 0; i < num_ephi_args (phi); i++)       
	reset_down_safe (phi_arg_def (ephi_arg (phi, i)));
    }      
} 

/* Determine whether the given PHI will require us to insert on a critical
   edge. */  
#if 0
static bool
requires_edge_placement (phi)
     tree_ref phi;
{
  edge pred;
  for (pred = ref_bb (phi)->pred; pred; pred = pred->pred_next)
    {
      if (pred->src != ENTRY_BLOCK_PTR)
	{
	  tree_ref operand = phi_operand_for_pred (phi, pred);
	  if (expruse_def (operand) == NULL && EDGE_CRITICAL_P (pred))
	    return true;
	}
    }
  return false;
}

#endif

/* Compute can_be_avail.  */
static void
compute_can_be_avail (ei)
     struct expr_info *ei;
{
  basic_block block;

  FOR_EACH_BB (block)
    {
      tree_ref phi = phi_at_block (ei, block);
      if (phi == NULL)
        continue;
      set_exprphi_canbeavail (phi, true);
    }

  FOR_EACH_BB (block)
    {     
      tree_ref phi = phi_at_block (ei, block);
      edge pred;
      if (phi == NULL)
        continue;
      if (!exprphi_canbeavail(phi))
        continue;
      
      /* We don't need this right now because our insertion mechanism
	 can build scopes and whatnot on the fly, which is the equivalent of
	 creating new bb's */
#if 0
      if (requires_edge_placement (phi))
	{
	  reset_can_be_avail (ei, phi);
	  continue;
	}      
#endif
      if (exprphi_downsafe (phi))
        continue;
      for (pred = block->pred; pred; pred = pred->pred_next)
        {
          if (pred->src != ENTRY_BLOCK_PTR)
            {
              tree_ref operand = phi_operand_for_pred (phi, pred);
              if (expruse_def (operand) == NULL)
                {
                  reset_can_be_avail (ei, phi);
                }
            }
        }
    }
}

/* Reset can_be_avail flags.  */
static void
reset_can_be_avail (ei, phi)
     struct expr_info *ei;   
     tree_ref phi;
{
  ref_list_iterator rli;
  
  set_exprphi_canbeavail (phi, false);
  rli = rli_start (exprref_uses (phi));
  for (; !rli_after_end (rli); rli_step (&rli))
    {
      tree_ref w = rli_ref (rli);
      
      if (ref_type (w) == E_USE && expruse_phiop (w)
		      && !expruse_has_real_use (w))
	{
	  tree_ref f = expruse_phi (w);
	  if (!exprphi_downsafe (f) && exprphi_canbeavail (f))
	    reset_can_be_avail (ei, f);
	}
    }
}

/*  Reset later flags.  */
static void 
reset_later (ei, phi)
     struct expr_info *ei;
     tree_ref phi;
{
  ref_list_iterator rli;

  set_exprphi_later (phi, false);  
  rli = rli_start (exprref_uses (phi));
  for (; !rli_after_end (rli); rli_step (&rli))
    {
      tree_ref w = rli_ref (rli);
#if ENABLE_CHECKING
      if (expruse_def (w) != phi)
	abort ();
#endif
      if (ref_type (w) == E_USE && expruse_phiop (w))
	{
	  tree_ref f = expruse_phi (w);
	  if (exprphi_later (f))
	    reset_later (ei, f);
	}
    }  
}

/*  Compute later flags.  */
static void
compute_later (ei)
     struct expr_info *ei;
{
  basic_block block;
  FOR_EACH_BB (block)
    {
      tree_ref phi = phi_at_block (ei, block);
      if (phi == NULL)
        continue;
      set_exprphi_later (phi ,exprphi_canbeavail (phi));
    }
      
  FOR_EACH_BB (block)
    {
      tree_ref phi = phi_at_block (ei, block);
      edge pred;
      if (phi == NULL)
        continue;
      if (!exprphi_later (phi))
        continue;

      for (pred = block->pred; pred; pred = pred->pred_next)
        {
          if (pred->src != ENTRY_BLOCK_PTR)
            {
              tree_ref operand = phi_operand_for_pred (phi, pred);
              if (expruse_def (operand) != NULL 
		  && expruse_has_real_use (operand))
	      {
                reset_later (ei, phi);
	      }
            }
        }
    }
}

/* Determine if we can insert the phi.  */
static bool
can_insert (op)
     tree_ref op;
{
  tree_ref def;
  
  if (expruse_def (op) == NULL)
    return true;
  
  def = expruse_def (op);
 
  if (!expruse_has_real_use (op))
    if (ref_type (def) == E_PHI && !(exprphi_willbeavail (def)))
      return true;

  return false;
}

#if 0
static void
set_need_repair (ei, q)
     struct expr_info *ei;
     tree_ref q;
{
  tree_ref p = avdefs[exprref_class (q)];
  int i;
  if (expruse_phiop (q))
    q = expruse_def (q);	  
  for (i = 0; i < 2; i ++)
    {
      tree v;
      tree_ref vp = NULL;
      tree_ref vq = NULL;
      if (!TREE_OPERAND (ei->expr, i)
	  || !DECL_P (TREE_OPERAND (ei->expr, i)))
	continue;
      v = TREE_OPERAND (ei->expr, i);
      if (ref_stmt (p))
	vp = find_use_for_var (ref_stmt (p), v);
      vq = find_use_for_var (ref_stmt (q), v);
      if (vp != vq)
	{
	  splay_tree_insert (need_repair_map, 
			     (splay_tree_key) imm_reaching_def (vq), 1);
	}
    }
}
#endif

/* Calculate the increment necessary due to EXPR for the temporary. */
static tree
calculate_increment (ei, expr)
     struct expr_info *ei;
     tree expr;
{
  tree incr;
  
  /*XXX: Currently assume it's like a = a + 5, thus, this will give us the 5. 
   */
  incr = TREE_OPERAND (TREE_OPERAND (expr, 1), 1);
  if (TREE_CODE (incr) != INTEGER_CST)
    abort();

  /*XXX: Currently assume it's a * <constant>, thus this will give us
    constant. */ 
  incr = fold (build (MULT_EXPR, TREE_TYPE (ei->expr), 
		      incr, TREE_OPERAND (ei->expr, 1)));
#if DEBUGGING_STRRED
  if (dump_file)
    {
      fprintf (dump_file, "Increment calculated to be: ");
      print_generic_expr (dump_file, incr, 0);
      fprintf (dump_file, "\n");
    }
#endif
  return incr;
}

/* Repair the injury for USE. Currently ugly as hell, i'm just making it do
 *something* so i can make sure we are choosing candidates and renaming
 properly. It's uncommented so that you don't accidently try to understand it
 or modify it, since it will be going away post-haste.*/
static void
repair_injury (ei, use, temp, orig_euse)
     struct expr_info *ei;
     tree_ref use;
     tree temp;
     tree_ref orig_euse;
{
  
  int i;
  tree expr, stmt;
  basic_block bb;
  ref_list toprocess = create_ref_list();

  if (htab_find (ei->repaired, use) != NULL)
      return;

  *(htab_find_slot (ei->repaired, use, INSERT)) = use;
  
  for (i = 0; i < 2; i++)
    {
      tree incr = integer_zero_node;
      tree_ref v;
      if (!DECL_P (get_operand (ei->expr, i)))
	continue;
      if (ref_type (use) == E_USE)
	v = find_rhs_use_for_var (use, TREE_OPERAND (ei->expr, i));
      else 
	v = use;      
      
      if (ref_type (v) == V_DEF)
	{
	  ref_list_iterator j;

	  /* If this isn't a def of *this* variable, ignore it, since it can't
	   *possibly* injure it. */
	  if (htab_find (ei->repaired, ref_stmt (v))
	      || !ref_defines (find_def_for_stmt (ref_stmt (v)), 
			       TREE_OPERAND (ei->expr, i)))

	    continue;
	  while (is_injuring_def (ei, ref_stmt (v)))
	    {
	      add_ref_to_list_begin (toprocess, v);
	      v = find_rhs_use_for_var (v, TREE_OPERAND (ei->expr, i));
	      if (!okay_injuring_def (imm_reaching_def (v), ref_var (v)))
		break;
	      v = find_rhs_use_for_var (imm_reaching_def (v), 
					TREE_OPERAND (ei->expr, i));
	    }
	  
	  /* The *first* reference will probably not have a reaching def we can
	     set yet, since it'll come from a save that hasn't been done
	     yet. */
	  for (j = rli_start (toprocess); !rli_after_end (j); rli_step (&j))
	    {
	      tree oldstmt;

	      v = rli_ref (j);
	      if (htab_find (ei->repaired, ref_stmt (v)) == NULL)
		{
#if DEBUGGING_STRRED
		  if (dump_file)
		      {
			fprintf (dump_file, "Injuring def to repair is: ");
			print_generic_stmt (dump_file, ref_stmt (v), 0);
			fprintf (dump_file, "\n");
		      }
#endif
		    incr = calculate_increment (ei, ref_stmt (v));
		    expr = build (INIT_EXPR, TREE_TYPE (temp), temp,
				  fold (build (PLUS_EXPR, TREE_TYPE (temp),
					       temp, incr)));
		    oldstmt = ref_stmt (v);
		    stmt = build (COMPOUND_EXPR, void_type_node, oldstmt,
				  expr);
		    TREE_TYPE (stmt) = TREE_TYPE (expr);
		    bb = ref_bb (use);
		    replace_ref_stmt_with (v, stmt);
		    update_old_new (ei, v->common.stmt_p, &TREE_OPERAND (stmt, 0));
		    /* REMOVE AFTER DFA UPDATE */
		    temp_fix_refs (oldstmt, stmt, &TREE_OPERAND (stmt, 0));
		    /* END REMOVE AFTER DFA UPDATE */
		    *(htab_find_slot (ei->repaired, ref_stmt (v), INSERT)) = ref_stmt (v);
		  }
	    }
	}
      else if (ref_type (v) == V_PHI 
	       || ref_type (imm_reaching_def (v)) == V_PHI)
	{
	  size_t curr_phi_operand;
	  tree_ref phi = ref_type (v) == V_PHI ? v :  imm_reaching_def (v);
	  for (curr_phi_operand = 0; 
	       curr_phi_operand < num_phi_args (phi); 
	       curr_phi_operand++)
	    {
	      phi_node_arg phi_oper = phi_arg (phi, curr_phi_operand);
	      tree_ref phi_op = phi_arg_def (phi_oper);
	      tree_ref ephi = phi_at_block (ei, ref_bb (phi));
	      tree_ref ephi_op;
	      if (!ephi)
		continue;
	      ephi_op = phi_operand_for_pred (ephi, 
					      phi_arg_edge (phi_oper));
	      repair_injury (ei, phi_op, temp, ephi_op);
	    }
	}
      else
	repair_injury (ei, imm_reaching_def (v), temp, orig_euse);
    }
}

/* Given a VAR reference, a basic block to look in (STMTBB), and optionally, a
   ePHI that is for the expression containing the variable, find and set the
   immediate reaching definition of VAR. */
static int
find_reaching_def_of_var (var, stmtbb, phi)
     tree_ref var;
     basic_block stmtbb;
     tree_ref phi;
{
  ref_list_iterator i;
  struct ref_list_node *thisref;
  struct ref_list_node *fromhere;

  if (!stmtbb)
    return 0;
  
  if (phi)
    for (i = rli_start (bb_refs (ref_bb (phi)));
	 !rli_after_end (i);
	 rli_step (&i))
      {
	tree_ref tempref = rli_ref (i);
	if (ref_type (tempref) != V_PHI)
	  continue;
	if (ref_var (tempref) == ref_var (var))
	  {
	    int opnum;
	    phi_node_arg realphiarg;

	    opnum = opnum_of_phi (tempref, stmtbb->index);
	    realphiarg = VARRAY_GENERIC_PTR (phi_args (tempref), opnum);

	    set_imm_reaching_def (var, phi_arg_def (realphiarg));
	    add_ref_to_list_end (imm_uses (phi_arg_def (realphiarg)), var);

	    return 1;
	  }   
      }
  
  /* If this is the bb of the var, we start looking *before* the var */
  fromhere = bb_refs (stmtbb)->last;
  if (ref_bb (var) == stmtbb)
    {
      thisref = find_list_node (bb_refs (stmtbb), var);
      if (thisref)
	fromhere = thisref;
    }

  for (i = rli_start_at (fromhere); !rli_after_end (i); rli_step_rev (&i))
    {
      tree_ref tempref = rli_ref (i);
      if ((ref_type (tempref) != V_DEF
	    && ref_type (tempref) != V_USE)
	  || ref_stmt (var) == ref_stmt (tempref)) 
	continue;
      if (ref_var (tempref) == ref_var (var))
	{
	  if (ref_type (tempref) == V_USE)
	    {
	      set_imm_reaching_def (var, imm_reaching_def (tempref));
	      add_ref_to_list_end (imm_uses (imm_reaching_def (tempref)), var);
	    }
	  else
	    {
	      set_imm_reaching_def (var, tempref);
	      add_ref_to_list_end (imm_uses (tempref), var);
	    }
	  return 1;
	}
    }
  return find_reaching_def_of_var (var,
				   get_immediate_dominator (pre_idom, stmtbb),
				   NULL);    
}

/* Update the SSA representation to account for the new use of the temporary
   TEMP in NEWUSE, located in BB. 
   This *actually* modifies the trees, so that newuse will be a use of the
   temporary, instead of the expression it is now.  */
static void
update_ssa_for_new_use (temp, newuse, defby, bb)
     tree temp;
     tree newuse;
     tree_ref defby ATTRIBUTE_UNUSED;
     basic_block bb;
     
{

  tree_ref use_orig_ref;
  tree newmodifyexpr, useexpr;
  
  /* Update the SSA info for the new use of the temporary.
     Complex process to do so. 
     1. Delete the uses in the expression we are replacing with 
     a use of the temporary, from all the lists they might
     appear in.
     2. Make a copy of the reached uses/immediate uses of the def
     in the expression.
     3. Delete the def (It's easier to redo it than update it) in
     the expression.
     4. Do the actual expression replacement.
     5. Create a new def for the expression, move the old immediate
     defs and uses.
     6. Update the arguments of any phis reached by the old def,
     that point to the old def, to point to the new def.
     7. Create a new use for the temporary, with a reaching def 
     of the def of the assignment expression we just created for
     this save. 
     
     This is a bit simplified, since when i update a reaching def, we update
     the reaching def's immediate uses, etc.
  */
  use_orig_ref = find_def_for_stmt (newuse);
  useexpr = ref_stmt (use_orig_ref);
  if (TREE_CODE (useexpr) == INIT_EXPR)
    newmodifyexpr = build (INIT_EXPR, TREE_TYPE (useexpr), 
			   TREE_OPERAND (useexpr, 0), temp);
  else
    newmodifyexpr = build (INIT_EXPR, TREE_TYPE (TREE_OPERAND (useexpr, 0)),
		           TREE_OPERAND (useexpr, 0), temp);
  replace_ref_stmt_with (use_orig_ref, newmodifyexpr);
  set_bb_for_stmt (newmodifyexpr, bb);  
}

/* Update all the phi argument definitions in REFS so that arguments defined by
   OLD are now defined by NEW.  */
static void
update_phis_in_list (refs, old, new)
     ref_list refs;
     tree_ref old;
     tree_ref new;   
{
  size_t i;
  ref_list_iterator j;
  
  for (j = rli_start (refs); !rli_after_end (j); rli_step (&j))
    {
      tree_ref tempref = rli_ref (j);

      if (ref_type (tempref) != V_PHI)
	continue;

      for (i = 0; i < VARRAY_ACTIVE_SIZE (phi_args (tempref)); i++)
	{
	  if (phi_arg_def (phi_arg (tempref, i)) == old)
	    phi_arg (tempref, i)->def = new;
	}
    }
}

/* Perform actual code motion.  */                        
static void
code_motion (ei, temp)
     struct expr_info *ei;
     tree temp;
{

  tree_ref use;
  ref_list_iterator euse_rli;
  ei->injfixups = create_ref_list ();
  need_repair_map = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);

  if (ei->strred_cand)
    {
      euse_rli = rli_start (ei->euses_dt_order);
      for (; !rli_after_end (euse_rli); rli_step (&euse_rli))
	{
	  use = rli_ref (euse_rli);
	  if ((ref_type (use) == E_USE)
	      && exprref_reload (use) 
	      && !exprref_inserted (use))
	    {
	      repair_injury (ei, use, temp, use);
	    }
	}
    }
  euse_rli = rli_start (ei->euses_dt_order);
  for (; !rli_after_end (euse_rli); rli_step (&euse_rli))
    {
      use = rli_ref (euse_rli);
      if ((ref_type (use) == E_USE
	   && !exprref_inserted (use)) || ref_type (use) == E_LEFT)
	{
#if ENABLE_CHECKING
	  if (ref_type (use) != E_LEFT && expruse_phiop (use) 
	      && (exprref_reload (use) || exprref_save (use)))
	    abort();
#endif
	  if (exprref_save (use))
	    {
	      basic_block use_bb = ref_bb (use);
	      tree use_stmt = ref_stmt (use);
	      tree use_expr = ref_stmt (use);
	      tree_ref use_orig_ref = NULL;
	      tree newexpr, newstmt;
	      ref_list_iterator i;
	      tree copy;
	      
	      i = rli_start (tree_refs (use_expr));
	      for (; !rli_after_end (i); rli_step (&i))
		{
		  if (ref_type (rli_ref (i)) == V_DEF)
		    {
		      use_orig_ref = rli_ref (i);
		      break;
		    }
		}
	      if (!use_orig_ref)
		abort ();
	      if (dump_file)
		{
		  fprintf (dump_file, "In BB %d, insert save of ", 
			   use_bb->index);
		  print_generic_expr (dump_file, ei->expr, 0);
		  fprintf (dump_file, " to ");
		  print_generic_expr (dump_file, temp, 0);
		  fprintf (dump_file, " before statement ");
		  print_generic_expr (dump_file,TREE_OPERAND (use_stmt, 0), 0);
		  fprintf (dump_file, " on line %d\n", 
			   TREE_LINENO (use_expr));
		}
	      copy = TREE_OPERAND (use_expr, 1);
	      walk_tree (&copy, copy_tree_r, NULL, NULL);
	      newexpr = build (INIT_EXPR, TREE_TYPE (temp), temp, copy);
	      newexpr = fold (newexpr);
	      newstmt = build (COMPOUND_EXPR, void_type_node, newexpr,
			       use_stmt);
	      set_bb_for_stmt (newexpr, use_bb);
	      replace_ref_stmt_with (use, newstmt);
	      set_bb_for_stmt (newstmt, use_bb);
	      update_old_new (ei, use->common.stmt_p, &TREE_OPERAND (newstmt, 1));

	      
	      use->common.stmt_p = &TREE_OPERAND (newstmt, 1);

	      /* REMOVE AFTER DFA UPDATE */
	      temp_fix_refs (use_stmt, newstmt, &TREE_OPERAND (newstmt, 1));
	      /* END REMOVE AFTER DFA UPDATE */
	      update_ssa_for_new_use (temp, ref_stmt (use), use, use_bb);

	    }
	  else if (exprref_reload (use))
	    {
	      basic_block use_bb = ref_bb (use);
	      tree use_stmt = ref_stmt (use);

	      if (dump_file)
                {
		  fprintf (dump_file, "In BB %d, insert reload of ", 
			   use_bb->index);
                  print_generic_expr (dump_file, ei->expr, 0);
                  fprintf (dump_file, " from ");
                  print_generic_expr (dump_file, temp, 0);
                  fprintf (dump_file, " in statement ");
                  print_generic_stmt (dump_file, use_stmt, 0);
                  fprintf (dump_file, " on line %d\n", 
			   TREE_LINENO (use_stmt));
                }
	      /* Update the SSA representation for a new use of the temporary.
		 Do exactly what we do for the new use generated by the save. 
	      */
	      update_ssa_for_new_use (temp, ref_stmt (use), expruse_def (use),
				      ref_bb (use));
#if DEBUGGING_SSA_UPDATE
	      if (dump_file)
		{
		  fprintf (dump_file, "\nUpdated a reload use:\n");
		  dump_ref_list (dump_file, "", tree_refs (use_stmt), 0, 1);
		}
#endif      
	    }
	}
  }

  free (avdefs);
  splay_tree_delete (need_repair_map);
}
  
/* Compute will_be_avail.  */
static void
will_be_avail (ei)
     struct expr_info *ei;
{
  compute_can_be_avail (ei);
  compute_later (ei);
}

static bool
call_modifies_slot (call, expr)
     tree_ref call;
     tree expr;
{

  ref_list_iterator rli;
  
  for (rli = rli_start (tree_refs (ref_stmt (call))); 
       !rli_after_end (rli);
       rli_step (&rli))
    {
      size_t i;
      if (TREE_CODE_CLASS (TREE_CODE (expr)) == 'd')
	{
	  if (ref_defines (rli_ref (rli), expr))
	    return true;
	}
      else
	{
	  for (i = 0; i < 2; i++)
	    {
	      if (!TREE_OPERAND (expr, i))
		continue;
	      if (ref_defines (rli_ref (rli), TREE_OPERAND (expr, i)))
		return true;
	    }
	}
    }
  return false;
}
/* Add call expression to expr-infos.  */
static int
add_call_to_ei (slot, data)
     struct expr_info *slot;
     void *data;
{
  tree_ref call = (tree_ref) data;
  struct expr_info *ei = (struct expr_info *) slot;
  if (call_modifies_slot (call, ei->expr))
    {
      VARRAY_PUSH_GENERIC_PTR (ei->occurs, NULL);
      VARRAY_PUSH_GENERIC_PTR (ei->lefts, NULL);
      VARRAY_PUSH_GENERIC_PTR (ei->kills, call->common.stmt_p);
      VARRAY_PUSH_GENERIC_PTR (ei->refs, call);
    }
  return 0;
}

/* Traverse over expressions to perform PRE on.  */
static int
pre_part_1_trav (slot, data)
     struct expr_info *slot;
     void *data;
{
  tree temp;
  size_t i;
  bool changed = true;
  struct expr_info *ei = (struct expr_info *) slot;

  if (VARRAY_ACTIVE_SIZE (ei->reals) < 2 
      &&  TREE_CODE (ei->expr) != INDIRECT_REF)
    return 0;

  /* Have to iterate until we are done changing, since we might have replaced
     what we replaced (IE processing a single expression may cause a to move to
  b, then b to c. If we don't iterate, we'll only change a to b. */ 
  while (changed)
    {
      changed = false;
      for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->occurs); i++)
	{
	  splay_tree_node result;
	  tree *occ = (tree *) VARRAY_GENERIC_PTR (ei->occurs, i);
	  result = splay_tree_lookup (old_new_map, (splay_tree_key) occ);
	  if (result)
	    {
	      changed = true;
	      VARRAY_GENERIC_PTR (ei->occurs, i) = (PTR) result->value;
	    }
	}
      for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->lefts); i++)
	{
	  splay_tree_node result;
	  tree *occ = (tree *) VARRAY_GENERIC_PTR (ei->lefts, i);
	  result = splay_tree_lookup (old_new_map, (splay_tree_key) occ);
	  if (result)
	    {
	      changed = true;
	      VARRAY_GENERIC_PTR (ei->lefts, i) = (PTR) result->value;
	    }
	}
      for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->kills); i++)
	{
	  splay_tree_node result;
	  tree *occ = (tree *) VARRAY_GENERIC_PTR (ei->kills, i);
	  result = splay_tree_lookup (old_new_map, (splay_tree_key) occ);
	  if (result)
	    {
	      changed = true;
	      VARRAY_GENERIC_PTR (ei->kills, i) = (PTR) result->value;
	    }
	}
    }
  expr_phi_insertion ((sbitmap *)data, ei);
  rename_1 (ei);
  if (dump_file)
    {
      fprintf (dump_file, "Occurrences for expression ");
      print_generic_expr (dump_file, ei->expr, 0);
      fprintf (dump_file, "\n");
      dump_ref_array (dump_file, "", ei->refs, 0, 1);
    }
  down_safety (ei);
  will_be_avail (ei);
  if (dump_file)
    {
      fprintf (dump_file, "ExprPhi's for expression ");
      print_generic_expr (dump_file, ei->expr, 0);
      fprintf (dump_file, "\n");
      dump_ref_array (dump_file, "", ei->phis, 0, 1);
    }
  {
    tree_ref def = NULL;
    tree type_of_expr;
    type_of_expr = TREE_TYPE (ei->expr);

    temp = create_tmp_var (type_of_expr, "pretmp");
    if (finalize_1 (ei, temp))
      {
	finalize_2 (ei);
	code_motion (ei, temp);
      }
  }
  return 0;
}

static void 
calculate_preorder ()
{
  edge *stack;
  int sp;
  int prenum = 0;
  sbitmap visited;

  /* Allocate the preorder number arrays.  */
  pre_preorder = (int *) xcalloc (last_basic_block+1, sizeof (int));
  
  /* Allocate stack for back-tracking up CFG.  */
  stack = (edge *) xmalloc ((last_basic_block + 1) * sizeof (edge));
  sp = 0;

  /* Allocate bitmap to track nodes that have been visited.  */
  visited = sbitmap_alloc (last_basic_block);
  
  /* None of the nodes in the CFG have been visited yet.  */
  sbitmap_zero (visited);

  /* Push the first edge on to the stack.  */
  stack[sp++] = ENTRY_BLOCK_PTR->succ;

  while (sp)
    {
      edge e;
      basic_block src;
      basic_block dest;

      /* Look at the edge on the top of the stack.  */
      e = stack[sp - 1];
      src = e->src;
      dest = e->dest;

      /* Check if the edge destination has been visited yet.  */
      if (dest != EXIT_BLOCK_PTR && ! TEST_BIT (visited, dest->index))
	{
	  /* Mark that we have visited the destination.  */
	  SET_BIT (visited, dest->index);

	  pre_preorder[dest->index] = prenum++;

	  if (dest->succ)
	    {
	      /* Since the DEST node has been visited for the first
		 time, check its successors.  */
	      stack[sp++] = dest->succ;
	    }
	}
      else
	{
	  
	  if (e->succ_next)
	    stack[sp - 1] = e->succ_next;
	  else
	    sp--;
	}
    }
  free (stack);
  sbitmap_free (visited);
}
static tree
get_operand (expr, num)
     tree expr;
     unsigned int num;
{
  if (is_simple_binary_expr (expr))
    return TREE_OPERAND (expr, num);
  else if (TREE_CODE (expr) == INDIRECT_REF)
    {    
      if (num == 0)
	return expr;
    }
  else if (TREE_CODE (expr) == COMPONENT_REF)
    {      
      if (num == 0)
	return expr;      
    }
  
  return NULL;
}
static inline void
add_left_occ (ei, leftocc)
     struct expr_info *ei;
     tree *leftocc;
{
  VARRAY_PUSH_GENERIC_PTR (ei->lefts, leftocc);
  VARRAY_PUSH_GENERIC_PTR (ei->kills, NULL);
  VARRAY_PUSH_GENERIC_PTR (ei->occurs, NULL);
}

static void
process_left_occs_and_kills (bexprs, slot, ref, expr)
     varray_type bexprs;
     struct expr_info *slot;
     tree_ref ref; 
     tree expr;
{
  size_t k;
  
  if (!is_simple_modify_expr (expr))
    return;
  if (TREE_CODE (expr) == CALL_EXPR)
    {
      tree callee = get_callee_fndecl (expr);
      if (!callee || !DECL_IS_PURE (callee))
	{
	  for (k = 0; k < VARRAY_ACTIVE_SIZE (bexprs); k++)
	    if (VARRAY_GENERIC_PTR (bexprs, k) != slot)
	      add_call_to_ei (VARRAY_GENERIC_PTR (bexprs, k), ref);
	}
    }
  else if (TREE_CODE (TREE_OPERAND (expr, 1)) == CALL_EXPR)
    {
      tree callee = get_callee_fndecl (TREE_OPERAND (expr, 1));
      if (!callee || !DECL_IS_PURE (callee))
	{		
	  for (k = 0; k < VARRAY_ACTIVE_SIZE (bexprs); k++)
	    if (VARRAY_GENERIC_PTR (bexprs, k) != slot)
	      add_call_to_ei (VARRAY_GENERIC_PTR (bexprs, k), ref);
	}
    }  
  if (DECL_P (TREE_OPERAND (expr, 0)))
    {
      tree op0_base = get_base_symbol (TREE_OPERAND (expr, 0));
      HOST_WIDE_INT op0_alias_set = get_alias_set (TREE_OPERAND (expr, 0));

      for (k = 0; k < VARRAY_ACTIVE_SIZE (bexprs); k++)
	{
	  tree op;
	  struct expr_info *ei = VARRAY_GENERIC_PTR (bexprs, k);
	  if (ei == slot)
	    continue;
	  if (TREE_CODE (ei->expr) == INDIRECT_REF 
	      || TREE_CODE (ei->expr) == COMPONENT_REF)
	    {	  
	      op = TREE_CODE (ei->expr) == INDIRECT_REF 
		? indirect_var (get_base_symbol (ei->expr)) : NULL;
/*		: component_ref (ei->expr, get_component_ref_index (ei->expr)); */
	      
	      if (may_alias_p (op,
			       get_base_symbol (op),
			       get_alias_set (op),
			       TREE_OPERAND (expr, 0),
			       op0_base,
			       op0_alias_set)
		  || op  == TREE_OPERAND (expr, 0))
		{
		  add_left_occ (ei, ref->common.stmt_p);
		}
	    }
	  else if (DECL_P (ei->expr))
	    {
	      if (may_alias_p (ei->expr,
			       get_base_symbol (ei->expr),
			       get_alias_set (ei->expr),
			       TREE_OPERAND (expr, 0),
			       op0_base,
			       op0_alias_set)
		  || ei->expr == TREE_OPERAND (expr, 0))
		{
		  add_left_occ (ei, ref->common.stmt_p);
		}
	    }
	}
    }
  else if (TREE_CODE (TREE_OPERAND (expr, 0)) == INDIRECT_REF)
    {
      for (k = 0; k < VARRAY_ACTIVE_SIZE (bexprs); k++)
	{
	  tree op1;
	  tree op2;
	  struct expr_info *ei = VARRAY_GENERIC_PTR (bexprs, k);
	  if (ei == slot)
	    continue;
	  if (TREE_CODE (ei->expr) == INDIRECT_REF 
	      || TREE_CODE (ei->expr) == COMPONENT_REF)
	  {
	    op1 = TREE_CODE (ei->expr) == INDIRECT_REF 
	      ? indirect_var (get_base_symbol (ei->expr))  : NULL;
	      /*: component_ref (ei->expr, get_component_ref_index (ei->expr));*/
	      op2 = indirect_var (get_base_symbol (TREE_OPERAND (expr, 0)));
	      if (may_alias_p (op1,
			       get_base_symbol (op1),
			       get_alias_set (op1),
			       op2,
			       get_base_symbol (op2),
			       get_alias_set (op2))
		   || op1 == op2)
		{
		  add_left_occ (ei, ref->common.stmt_p);
		}
	  }
	  else if (DECL_P (ei->expr))
	    {
	      tree op2 = indirect_var (get_base_symbol (TREE_OPERAND (expr, 0)));
	      if (may_alias_p (ei->expr,
			       get_base_symbol (ei->expr),
			       get_alias_set (ei->expr),
			       op2,
			       get_base_symbol (op2),
			       get_alias_set (op2))
		{
		  add_left_occ (ei, ref->common.stmt_p);
		}
	    }
	}
    }
  else if (TREE_CODE (TREE_OPERAND (expr, 0)) == COMPONENT_REF)
    {
      for (k = 0; k < VARRAY_ACTIVE_SIZE (bexprs); k++)
	{
	  tree op1;
	  tree op2;
	  struct expr_info *ei = VARRAY_GENERIC_PTR (bexprs, k);
	  if (ei == slot)
	    continue;
	  if (TREE_CODE (ei->expr) == COMPONENT_REF 
	      || TREE_CODE (ei->expr) == INDIRECT_REF)
	    {
	      op1 = TREE_CODE (ei->expr) == INDIRECT_REF 
		? indirect_var (get_base_symbol (ei->expr))  : NULL;
/*		: component_ref (ei->expr, get_component_ref_index (ei->expr));*/
	      op2 = TREE_OPERAND (expr, 0);
/*	      op2 = component_ref (op2, get_component_ref_index (op2));*/
	      if (may_alias_p (op1,
			       get_base_symbol (op1),
			       get_alias_set (op1),
			       op2,
			       get_base_symbol (op2),
			       get_alias_set (op2))
		   || op1 == op2)
		{
		  add_left_occ (ei, ref->common.stmt_p);
		}
	    }	  
	  else if (DECL_P (ei->expr))
	    {
	      tree op2 = TREE_OPERAND (expr, 0);
/*	      op2 = component_ref (op2, get_component_ref_index (op2));*/
	      if (may_alias_p (ei->expr,
			       get_base_symbol (ei->expr),
			       get_alias_set (ei->expr),
			       op2,
			       get_base_symbol (op2),
			       get_alias_set (op2))
		{
		  add_left_occ (ei, ref->common.stmt_p);
		}
	    }
	}
    }
}

void 
tree_perform_ssapre (fndecl)
     tree fndecl;
{
  tree fn = DECL_SAVED_TREE (fndecl);
    /* First, we need to find our candidate expressions. */
  varray_type bexprs;
  htab_t seen = htab_create (37, htab_hash_pointer, htab_eq_pointer, NULL);
  htab_t processed = htab_create (37, htab_hash_pointer, htab_eq_pointer, NULL);
  
  basic_block bb;
  size_t j, k;

  timevar_push (TV_TREE_PRE);

  new_stmt_map = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);
  old_new_map = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);

  VARRAY_GENERIC_PTR_INIT (bexprs, 1, "bexprs");
  
  /* Compute immediate dominators.  */
  pre_idom = calculate_dominance_info (CDI_DOMINATORS);
  domchildren = sbitmap_vector_alloc (last_basic_block, last_basic_block);
  sbitmap_vector_zero (domchildren, last_basic_block);
  compute_domchildren (pre_idom, domchildren);

  /* Compute dominance frontiers.  */
  pre_dfs = sbitmap_vector_alloc (last_basic_block, last_basic_block);
  sbitmap_vector_zero (pre_dfs, last_basic_block);
  compute_dominance_frontiers (pre_dfs, pre_idom);

  dump_file = dump_begin (TDI_pre, &dump_flags);
  calculate_preorder ();

  /* First collect the regular occurrences. */
  FOR_EACH_BB (bb)
    {    
      ref_list_iterator i;

      htab_empty (seen);
      for (i = rli_start (bb_refs (bb)); !rli_after_end (i); rli_step (&i))
	{
	  tree_ref ref = rli_ref (i);
	  tree expr = ref_stmt (ref);
	  tree orig_expr = expr;
	  tree stmt = ref_stmt (ref);
	  struct expr_info *slot = NULL;
	  
	  if (ref_stmt (ref) == NULL_TREE)
	    continue;

	  if (ref_type (ref) == V_USE 
		&& htab_find (seen, orig_expr) != NULL)
	    continue;

	  if (is_simple_modify_expr (expr))
	    expr = TREE_OPERAND (expr, 1);

	  if (ref_type (ref) == V_USE 
	      && (is_simple_binary_expr (expr)
		  || (TREE_CODE (expr) == INDIRECT_REF 
		      && DECL_P (TREE_OPERAND (expr, 0)))
		  || TREE_CODE (expr) == COMPONENT_REF
	      	  || (DECL_P (expr) && !TREE_READONLY (expr))))
	    {
	      for (k = 0; k < VARRAY_ACTIVE_SIZE (bexprs); k++)
	        {
		  slot = VARRAY_GENERIC_PTR (bexprs, k);
		  if (expr_lexically_eq (slot->expr, expr))
		    break;
		}
	      if (k >= VARRAY_ACTIVE_SIZE (bexprs))
	        slot = NULL;
	      if (slot)
		{
		  VARRAY_PUSH_GENERIC_PTR (slot->occurs, 
					   ref->common.stmt_p);
		  VARRAY_PUSH_GENERIC_PTR (slot->kills, NULL);
		  VARRAY_PUSH_GENERIC_PTR (slot->lefts, NULL);
		  VARRAY_PUSH_TREE (slot->reals, stmt);
		  VARRAY_PUSH_GENERIC_PTR (slot->refs, ref);
		  slot->strred_cand &= is_strred_cand (orig_expr);
		}
	      else
		{
		  slot = ggc_alloc (sizeof (struct expr_info));
		  if (TREE_CODE (expr) == INDIRECT_REF
		      && DECL_P (TREE_OPERAND (expr, 0)))
		    {
		      slot->expr = get_base_symbol (expr);
		      slot->expr = indirect_var (slot->expr);
		    }
		  else
		    slot->expr = expr;
		  VARRAY_GENERIC_PTR_INIT (slot->occurs, 1, 
					   "Occurrence");
		  VARRAY_GENERIC_PTR_INIT (slot->kills, 1,
					   "Kills");
		  VARRAY_GENERIC_PTR_INIT (slot->lefts, 1,
					   "Left occurrences");
		  VARRAY_TREE_INIT (slot->reals, 1, "Real occurrences");
		  VARRAY_GENERIC_PTR_INIT (slot->phis, last_basic_block, 
					   "EPHIs");
		  VARRAY_GENERIC_PTR_INIT (slot->erefs, 1, "EREFs");
		  VARRAY_GENERIC_PTR_INIT (slot->refs, 1, "REFs");
		  VARRAY_PUSH_GENERIC_PTR (slot->occurs, ref->common.stmt_p);
		  VARRAY_PUSH_GENERIC_PTR (slot->kills, NULL);
		  VARRAY_PUSH_GENERIC_PTR (slot->lefts, NULL);
		  VARRAY_PUSH_TREE (slot->reals, stmt);
		  VARRAY_PUSH_GENERIC_PTR (slot->refs, ref);

		  VARRAY_PUSH_GENERIC_PTR (bexprs, slot);
		  slot->strred_cand = is_strred_cand (orig_expr);
		  slot->repaired = htab_create (7, htab_hash_pointer, 
						htab_eq_pointer, NULL);
		}
	    }

	  if (ref_type (ref) == V_USE)
	    *(htab_find_slot  (seen, orig_expr, INSERT)) = orig_expr;
	}
    }
  /* Now collect the left occurrences and kills. */
  FOR_EACH_BB (bb)
    {    
      ref_list_iterator i;

      for (i = rli_start (bb_refs (bb)); !rli_after_end (i); rli_step (&i))
	{
	  tree_ref ref = rli_ref (i);
	  tree expr = ref_stmt (ref);
	  tree orig_expr = expr;
	  struct expr_info *slot = NULL;
	  
	  if (ref_stmt (ref) == NULL_TREE)
	    continue;

	  if (is_simple_modify_expr (expr))
	    expr = TREE_OPERAND (expr, 1);
	  if (ref_type (ref) != V_USE)
	    {
	      if (htab_find (processed, orig_expr) == NULL)
		process_left_occs_and_kills (bexprs, slot, ref, orig_expr);
	      *(htab_find_slot (processed, orig_expr, INSERT)) = orig_expr;
	    }	 
	}
    }
  for (j = 0; j < VARRAY_ACTIVE_SIZE (bexprs); j++)
     pre_part_1_trav (VARRAY_GENERIC_PTR (bexprs, j), pre_dfs);
  for (j = 0; j < VARRAY_ACTIVE_SIZE (bexprs); j++)
    free_expr_info (VARRAY_GENERIC_PTR (bexprs, j));

  VARRAY_CLEAR (bexprs);
  htab_delete (seen);
  free_dominance_info (pre_idom);
  free (pre_preorder);
  sbitmap_vector_free (pre_dfs);
  sbitmap_vector_free (domchildren);

  splay_tree_delete (new_stmt_map);
  splay_tree_delete (old_new_map);  
  
  dump_end (TDI_pre, dump_file);
  timevar_pop (TV_TREE_PRE);

  /* Debugging dump after SSA PRE */
  dump_function (TDI_pre, fndecl);
}
