/* SSA-PRE for trees.
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
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
/* This should be eventually be generalized to other languages, but
   this would require a shared function-as-trees infrastructure.  */
#include "c-common.h"
#include "c-tree.h"
#include "bitmap.h"
#include "splay-tree.h"
#include "tree-simple.h"
#include "tree-flow.h"
#include "tree-inline.h"

#define EXTRANEOUS_EPHI_REMOVAL 0
#define DEBUGGING_STRRED 1
#define DEBUGGING_SSA_UPDATE 1

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
   Get rid of hunting through statements for refs, now that
   expressions have the refs we want as well -- This should substantially
   clean up the code, which was written when only statements had refs,
   because the expressions were not unshared.

   Remapping expression occurrences to the entire expression is a pain, and is
   only done to avoid having to grab the first operand all the time (the code
   was started before the change to include the *entire* expression in the
   ref_expr was made).  It would clean up the code a lot and remove a lot of
   duplicated functions, whose only difference is one remaps and one doesn't.

   Strength reduction improvements.

   Register promotion.
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
static void compute_can_be_avail PARAMS ((struct expr_info *));
static void reset_can_be_avail PARAMS ((struct expr_info *, tree_ref));
static void compute_later PARAMS ((struct expr_info *));
static void reset_later PARAMS ((struct expr_info *, tree_ref));
static void code_motion PARAMS ((struct expr_info *, tree));
static bool can_insert PARAMS ((tree_ref));
static inline bool defs_match_p PARAMS ((struct expr_info *, tree, tree));
static inline bool defs_y_dom_x PARAMS ((struct expr_info *, tree_ref, tree_ref));
static void assign_new_class PARAMS ((tree_ref, varray_type *));
static void insert_occ_in_preorder_dt_order_1 PARAMS ((struct expr_info *, 
						       fibheap_t, basic_block));
static void insert_occ_in_preorder_dt_order PARAMS ((struct expr_info *, 
						     fibheap_t));
static void insert_euse_in_preorder_dt_order_1 PARAMS ((struct expr_info *, 
							fibheap_t, basic_block));
static void insert_euse_in_preorder_dt_order PARAMS ((struct expr_info *, 
						      fibheap_t));
static inline int opnum_of_phi PARAMS ((tree_ref, int));
static varray_type phi_opnd_from_res PARAMS ((struct expr_info *, tree_ref, 
					      int, basic_block));
static splay_tree rename_2 PARAMS ((struct expr_info *, varray_type *));
static void rename_1 PARAMS ((struct expr_info *));
static void finalize_2 PARAMS ((struct expr_info *));
static void finalize_1 PARAMS ((struct expr_info *, tree));
static tree_ref phi_for_operand PARAMS ((struct expr_info *, tree_ref));
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


/* sbitmap vector mapping blocks to the child blocks they dominate. */
static sbitmap *domchildren;
struct expr_info
{
  /* The actual expression.  */
  tree expr;  
  /* The occurrences, which can include calls. */
  varray_type occurs;
  /* Statements for the occurrences. */
  varray_type occurstmts;
  /* An array of real occurrences. */
  varray_type reals;
  /* An array mapping reals to the containing statements (so we can
     get defs/uses/etc about them. */
  varray_type realstmts;
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
  
  STRIP_WFL (expr);
  /* Multiplications that def a variable they use can't be strength reduced.
     Removing the multiplications would require adding more multiplications.
     IE a = a * 5. */
  if (TREE_CODE (TREE_OPERAND (expr, 1)) != MULT_EXPR)
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

/* Return true is INJ is an injuring definition */
static bool 
is_injuring_def (ei, inj)
	struct expr_info *ei;
     tree inj;
{
  if (!inj || !is_simple_modify_expr (inj))
    return false;
  STRIP_WFL (inj);
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
  VARRAY_CLEAR (e1->occurstmts);
  VARRAY_CLEAR (e1->reals);
  VARRAY_CLEAR (e1->realstmts);
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
	    while (is_injuring_def (ei, ref_expr (phi_operand)))
	      {
		tree_ref ird;
		phi_operand = find_rhs_use_for_var (phi_operand, 
						    ref_var
						    (phi_operand));
		ird = imm_reaching_def (phi_operand);
		if (is_default_def (ird)
		    || ref_type (ird) == V_PHI
		    || !maybe_find_rhs_use_for_var (ird, 
						    ref_var (phi_operand)))
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

/* Helper macro to simplify accessing a phi operand for a given block. */
#define phi_operand(phi, b) VARRAY_GENERIC_PTR (exprphi_phi_args (phi), pre_preorder[(b)->index])

/* Given a statement, find the V_DEF in it. */
static inline tree_ref
find_def_for_stmt (stmt)
     tree stmt;
{
  ref_list_iterator i;

  for (i = rli_start (tree_refs (stmt)); !rli_after_end (i); rli_step (&i))
    if (ref_type (rli_ref (i)) == V_DEF)
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
  i = rli_start (tree_refs (ref_expr (def)));
  for (; !rli_after_end (i); rli_step (&i))
    {
      tree_ref ref = rli_ref (i);

      if (ref_type (ref) != V_USE
	  || !is_simple_modify_expr (ref_expr (ref)))
        continue;
      if (ref_var (ref) != var)
        continue;
      return ref;
    }
  return NULL;
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
      
      yexpr = ref_expr (y);
      STRIP_WFL (yexpr);
      yexpr = TREE_OPERAND (yexpr, 1);
      
      if (!TREE_OPERAND (yexpr, i))
        continue;

      j = rli_start (tree_refs (ref_stmt (y)));
      for (; !rli_after_end (j); rli_step (&j))
        {
	  tree_ref ref = rli_ref (j);

          /* Find the ref for this operand. */
	  if (ref_type (ref) != V_USE)
	    continue;
	  refexpr = ref_expr (ref);
	  STRIP_WFL (refexpr);
	  if (!is_simple_modify_expr (refexpr)
              || TREE_OPERAND (refexpr, 1) != yexpr)
            continue;
          if (ref_var (ref) != TREE_OPERAND (yexpr, i))
            continue;
          if (!imm_reaching_def (ref))
            return false;

	  if (ei->strred_cand)
	    while (is_injuring_def (ei, ref_expr (imm_reaching_def (ref))))
	      {
		ref = find_rhs_use_for_var (imm_reaching_def (ref), 
					    TREE_OPERAND (yexpr, i));
		if (ref_type (imm_reaching_def (ref)) == V_PHI
		    || is_default_def (imm_reaching_def (ref)))
		  break;
		ref = find_rhs_use_for_var (imm_reaching_def (ref), 
					    TREE_OPERAND (yexpr, i));
	      }
          if (!(a_dom_b (ref_bb (imm_reaching_def (ref)), ref_bb (x))))
            return false;
	  break;
        }
    }
  return true;
}

/* Return true if the variables in t1 have the same defs as the
   variables in t2.  
   NB: t1 is expected to be a statement or assignment (IE both left and right sides).
   t2 is expected to be the right hand side only.*/
static inline bool
defs_match_p (ei, t1, t2)
     struct expr_info *ei;
     tree t1;
     tree t2;
{
  tree_ref use2;
  tree origt2;
  ref_list_iterator i;
  
  STRIP_WFL (t1);
  STRIP_WFL (t2);
  origt2 = t2;
  t2 = TREE_OPERAND (t2, 1);
  
  for (i = rli_start (tree_refs (t1)); !rli_after_end (i); rli_step (&i))
    {
      tree_ref use1 = rli_ref (i);
      tree use1expr = ref_expr (use1);
      if (ref_type (use1) != V_USE
	  || !is_simple_modify_expr (use1expr))
        continue;

      STRIP_WFL (use1expr);
      use1expr = TREE_OPERAND (use1expr, 1);
      if (TREE_CODE (use1expr) != TREE_CODE (t2))
        continue;
      use2 = find_rhs_use_for_var (find_def_for_stmt (origt2), ref_var (use1));
      if (!use2) 
	return false;
      /* Find the injuring definition, if one exists. */
      if (ei->strred_cand)
	while (is_injuring_def (ei, ref_expr (imm_reaching_def (use1))))
	  {
	    /* First, we get the use in the *injuring definition*. */
	    use1 = find_rhs_use_for_var (imm_reaching_def (use1), 
					 ref_var (use1));
	    /* Now, use the use we just got to get the use from before it was
	       injured. */
	    use1 = find_rhs_use_for_var (imm_reaching_def (use1), 
					 ref_var (use1));
	  }
      if (imm_reaching_def (use2) != imm_reaching_def (use1))
        return false;
    }
  return true;
}

/* Assign a new redundancy class to the occurrence, and push it on the
   stack.  */ 
static void
assign_new_class (occ, stack)
     tree_ref occ;
     varray_type *stack;
{
  /* class(occ) <- count
     Push(occ, stack)
     count <- count + 1
  */
  set_exprref_class (occ, class_count);
  VARRAY_PUSH_GENERIC_PTR (*stack, occ);
  class_count++;
}

/* Insert the expressions in preorder DT order in the fibheap.  */
static void
insert_euse_in_preorder_dt_order_1 (ei, fh, block)
     struct expr_info *ei;
     fibheap_t fh;
     basic_block block;
{
  size_t i;
  varray_type bbrefs = ei->erefs;
  for (i = 0; i < VARRAY_ACTIVE_SIZE (bbrefs); i++)
    {
      tree_ref ref = VARRAY_GENERIC_PTR (bbrefs, i);
      if (ref_bb (ref) != block)
	continue;
      if (ref_type (ref) == E_USE || ref_type (ref) == E_PHI)
	fibheap_insert (fh, preorder_count++, ref);
    }
  EXECUTE_IF_SET_IN_SBITMAP (domchildren[block->index], 0, i, 
  {
    insert_euse_in_preorder_dt_order_1 (ei, fh, BASIC_BLOCK (i));
  });
}

static void
insert_euse_in_preorder_dt_order (ei, fh)
     struct expr_info *ei;
     fibheap_t fh;
{
  preorder_count = 0;
  insert_euse_in_preorder_dt_order_1 (ei, fh, ENTRY_BLOCK_PTR->next_bb);
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
      tree *occurexpr = VARRAY_GENERIC_PTR (ei->occurs, i);
      tree *occurstmt = VARRAY_GENERIC_PTR (ei->occurstmts, i);
      tree_ref occurref = VARRAY_GENERIC_PTR (ei->refs, i);
      if (ref_bb (occurref) != block)
        continue;
      if (TREE_CODE (*occurexpr) == CALL_EXPR)
	{
	  newref = create_ref (*occurexpr,
				  E_KILL, 0, block, occurstmt,
				  occurexpr, NULL, true);
	  VARRAY_PUSH_GENERIC_PTR (ei->erefs, newref);
	  fibheap_insert (fh, preorder_count++, newref);
	}
      else
	{
	  newref = create_ref (*occurexpr,
				  E_USE, 0, block, occurstmt, 
				  occurexpr, NULL, true);
	  VARRAY_PUSH_GENERIC_PTR (ei->erefs, newref);
	  set_expruse_def (newref, NULL);
	  set_exprref_class (newref, 0);
	  set_expruse_phiop (newref, false);
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
              tree_ref newref = create_ref (NULL, E_USE, 0, block, 0, 0, 0, true);
              tree_ref phi = phi_at_block (ei, succ->dest);
	      VARRAY_PUSH_GENERIC_PTR (ei->erefs, newref);
              set_expruse_def (newref, NULL);
              set_exprref_class (newref, 0);
              set_expruse_phiop (newref, true);
              set_expruse_has_real_use (newref, false);
	      set_exprref_save (newref, false);
	      set_exprref_reload (newref, false);
	      set_exprref_inserted (newref, false);
              phi_operand (phi, block) = newref;
              fibheap_insert (fh, preorder_count++, newref);
            }
        }      
      else
        {
	  /* No point in inserting exit blocks into heap first, since
	     they'll never be anything on the stack. */
	  if (preorder_count != 0)
	    fibheap_insert (fh, preorder_count++, (void *) EXIT_BLOCK_PTR);
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
	  while  (is_injuring_def (ei, ref_expr (imm_reaching_def (v))))
	    {
	      v = find_rhs_use_for_var (imm_reaching_def (v), ref_var (v));
	      if (is_default_def (imm_reaching_def (v))
		  || ref_type (imm_reaching_def (v)) == V_PHI)
		break;
	      v = find_rhs_use_for_var (imm_reaching_def (v), ref_var (v));
	    }
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
  basic_block block;

  /* Keep track of which phi operands we touch.  */
  touched_set = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);

  /* For each exprphi f in the program do
	for each operand w of f do
	  processed (w) = false.  */
  for (i = 0; i < VARRAY_SIZE (ei->phis); i++)
    {
      tree_ref phi = VARRAY_GENERIC_PTR (ei->phis, i);
      if (phi != NULL)
        bitmap_clear (exprphi_processed(phi));
    }
  
  /* while (set_for_rename2 not empty) do { */
  while (VARRAY_ACTIVE_SIZE (*rename2_set) > 0)
    {
      tree_ref Z; 
      tree_ref phiZ;

      /* Remove real occurrence Z from set_for_rename2.  */
      Z = VARRAY_TOP_GENERIC_PTR (*rename2_set);
      VARRAY_POP (*rename2_set);

      splay_tree_insert (touched_set, (splay_tree_key) Z, 0);
      
      /* f <- phi that defines Z. */
      phiZ = expruse_def (Z);
      
      /* for each operand w of f do { */
      FOR_EACH_BB (block)
	{
          tree_ref w;
	  i = block->index;
	  
          if (phi_operand (phiZ, block) == NULL)
            continue;
          w = phi_operand (phiZ, block);
	  
	  /* if (not processed(w)) { */
          if (!bitmap_bit_p (exprphi_processed (phiZ), i))
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
	      if (ref_type (X) == E_USE)
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
		    if ((ref_type (op1) != V_USE))
		      continue;
		    if (ei->strred_cand)
		      while (is_injuring_def (ei, ref_expr (imm_reaching_def (op1))))
			{
			  op1 = find_rhs_use_for_var (imm_reaching_def (op1),
						      ref_var (op1));

			  if (is_default_def (imm_reaching_def (op1))
			      || ref_type (imm_reaching_def (op1)) == V_PHI)
			    break;

			  op1 = find_rhs_use_for_var (imm_reaching_def (op1),
						      ref_var (op1)) ;
			}

		    if (ref_type (op1) == V_USE)
		      op1 = imm_reaching_def (op1);

		    op2 = VARRAY_GENERIC_PTR (Y, k);
		    
		    if (op2 && ref_type (op2) == V_USE)
		      op2 = imm_reaching_def (op2);

		    if (ei->strred_cand)
		      while (is_injuring_def (ei, ref_expr (op2)))
			{
			  tree_ref ird;
			  op2 = find_rhs_use_for_var (op2, ref_var
						      (op2));
			  ird = imm_reaching_def (op2);
			  if (is_default_def (ird)
			      || ref_type (ird) == V_PHI
			      || !maybe_find_rhs_use_for_var (ird, 
							      ref_var (op2)))
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
		    
		    if (op1 != op2)
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
	      bitmap_set_bit (exprphi_processed (phiZ), i);
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
  varray_type rename2_set;
  varray_type recheck_set;
  splay_tree touched_set;
  size_t i;
  
  /* count <- 0
     stack <- empty
     set_for_rename2 <- {} */
  VARRAY_GENERIC_PTR_INIT (stack, 1, "Stack");
  VARRAY_GENERIC_PTR_INIT (rename2_set, 1, "Rename2 set");
  VARRAY_GENERIC_PTR_INIT (recheck_set, 1, "Recheck set");
  class_count = 1;
  
 /* for each occurrence Y in the current expression in preorder DT
     traversal order do { */
  insert_occ_in_preorder_dt_order (ei, fh);
  while (!fibheap_empty (fh))
    {
      y = fibheap_extract_min (fh);
      if ((basic_block) y == EXIT_BLOCK_PTR)
        {
	  if (VARRAY_ACTIVE_SIZE (stack) > 0)
            {
              tree_ref stackref = VARRAY_TOP_GENERIC_PTR (stack);
              if (ref_type (stackref) == E_PHI)
                set_exprphi_downsafe (stackref, false);
            }
          continue;
        }
      if (ref_type (y) == E_KILL 
	  && TREE_CODE (ref_expr (y)) == CALL_EXPR)
        {
	  if (VARRAY_ACTIVE_SIZE (stack) > 0)
            {
              tree_ref stackref = VARRAY_TOP_GENERIC_PTR (stack);
              if (ref_type (stackref) == E_PHI)
                set_exprphi_downsafe (stackref, false);
	      VARRAY_POP_ALL (stack);
            }
          continue;       
        }
      
      /* while (Top(stack) does not dominate Y) do 
	   Pop(stack)  */
      while (VARRAY_ACTIVE_SIZE (stack) > 0)
        {
          tree_ref stackref = VARRAY_TOP_GENERIC_PTR (stack);
          if (a_dom_b (ref_bb (stackref), ref_bb (y)))
            break;           
          VARRAY_POP (stack);
        }

      /* if (Y is an exprphi occurrence)
           assign_new_class (Y)  */
      if (ref_type (y) == E_PHI)
        {
	  assign_new_class (y, &stack);
        }
      /* else if (Y is a real occurrence) */
      else if (ref_type (y) == E_USE && expruse_phiop (y) == false)
        {
	  /* if (stack is empty)
	       Assign_New_Class(Y) */
          if (VARRAY_ACTIVE_SIZE (stack) == 0)
            {
              assign_new_class ( y, &stack);
            }
          /* else */
          else
            {
	      /* X <- Top(stack) */
              tree_ref x = VARRAY_TOP_GENERIC_PTR (stack);
	      
              /* If (X is a real occurrence) */
              if (ref_type (x) == E_USE)
                {
                  /* If (all corresponding variables in Y and X have
                     the same SSA version) */
                  if (defs_match_p (ei, ref_stmt (y), ref_expr (x)))
                    {
                      /* class(Y) <- class(X)
                         def(Y) <- X */
                      set_exprref_class (y, exprref_class (x));
#if 0
                      EXPRUSE_DEF (y) = x;
#else
		      set_expruse_def (y, expruse_def (x) != NULL ? expruse_def (x) : x);
#endif
                    }
		  /* else Assign_New_Class (Y) */
                  else
                    {
                      assign_new_class ( y, &stack);
                    }
                }
              else /* X is a ExprPhi occurrence. */
                {
                  if (ref_type (x) != E_PHI)
                    abort();
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

		      VARRAY_PUSH_GENERIC_PTR (stack, y);		      
		      VARRAY_PUSH_GENERIC_PTR (rename2_set, y);
                    }
                  else
                    {
		      set_exprphi_downsafe (x, false);
                      assign_new_class ( y, &stack);
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
            }
          else
            {

              /* X = Top(stack)
                 class(Y) <- class(X)
                 def(Y) <- X */
              tree_ref x = VARRAY_TOP_GENERIC_PTR (stack);
              set_exprref_class (y, exprref_class (x));
              set_expruse_def (y, x);
	      if (ref_type (x) != E_PHI)
		{
		  set_expruse_has_real_use (y, true);
		  if (expruse_def (x) && ref_type (expruse_def (x)) == E_PHI)
			VARRAY_PUSH_GENERIC_PTR (rename2_set, x);
		}
	      else
		{
		  set_expruse_has_real_use (y, false);
		  VARRAY_PUSH_GENERIC_PTR (recheck_set, y);
		}
	    }
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
      bool needcheck = true;
      edge succ;
      if (!ref)
	continue;
      def = expruse_def (ref);
      for (succ = ref_bb (ref)->succ; succ; succ = succ->succ_next)
	{
	  if (phi_at_block (ei, succ->dest) != NULL)
	    {
	      tree_ref phi = phi_at_block (ei, succ->dest);
	      if (bitmap_bit_p (exprphi_processed (phi), 
				ref_bb (ref)->index))
		needcheck = false;
	    }
	}
      if (!needcheck)
	continue;

      if (def && (ref_type (def) == E_PHI))
	set_exprphi_downsafe (def, false);

      set_expruse_def (ref, NULL);
    }
  fibheap_delete (fh);
  VARRAY_CLEAR (stack);
  VARRAY_CLEAR (rename2_set);
  VARRAY_CLEAR (recheck_set);
  splay_tree_delete (touched_set);
}

/* XXX: Do this smarter */
/* Figure out which PHI contains the operand.  */
static tree_ref
phi_for_operand (ei, operand)
     struct expr_info *ei;
     tree_ref operand;
{
  int count = 0;
  tree_ref retval = NULL;
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      tree_ref phi = phi_at_block (ei, bb);
      if (phi != NULL)
        {
	  basic_block bb2;
	  FOR_EACH_BB (bb2)
	    {
	      if (phi_operand (phi, bb2) ==  operand)
	      {
		count++;
		retval = phi;
	      }	  
	    }
	}
    }
  /* Theoretically, phi operand to phi is a 1 to many mapping.  But we
     don't return more than one value right now, so if we found multiple
     phis containing this operand, abort.  */
  if (count > 1)
    abort();
  
  return retval;
}

/* Figure out which expressions need to be saved.  */
static void
set_save (ei, X)
     struct expr_info *ei;
     tree_ref X;
{
  basic_block bb;
  if (ref_type (X) == E_USE && !expruse_phiop (X))
    set_exprref_save (X, true);
  else if (ref_type (X) == E_PHI)
    {
      int i;
      FOR_EACH_BB (bb)
	{
	  tree_ref w = phi_operand (X, bb);
	  i = bb->index;

	  if (w && !bitmap_bit_p (exprphi_processed (X), i))
	    {
	      bitmap_set_bit (exprphi_processed (X), i);
	      set_save (ei, expruse_def (w));
	    }
	}
    }
#if EXTRANEOUS_EPHI_REMOVAL 
  if (ref_type (X) == E_USE && !expruse_phiop (X))
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
  basic_block bb1;
  FOR_EACH_BB (bb1)
    {
      size_t k;
      basic_block bb2;
      tree_ref f = phi_at_block (ei, bb1);
      if (f == NULL || !exprphi_willbeavail (f) || f == g)
	continue;
      FOR_EACH_BB (bb2)
	{
	  tree_ref operand = phi_operand (f, bb2);
	  if (operand && expruse_def (operand) == g)
	    {
	      if (exprphi_extraneous (f))
		set_replacement (ei, f, replacing_def);
	      else
		phi_operand (f, bb2) = replacing_def;
	    }
	}
      for (k = 0; k < VARRAY_ACTIVE_SIZE (ei->erefs); k++)
	{
	  tree_ref X = VARRAY_GENERIC_PTR (ei->erefs, k);
	  if (ref_type (X) == E_USE 
	      && expruse_phiop (X) == false
	      && exprref_reload (X)
	      && expruse_def (X) == g)
	    {
	      set_expruse_def (X, replacing_def);
	    }
	}
    }
  phi_at_block (ei, ref_bb (g)) = NULL;
  
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
	  bitmap_clear (exprphi_processed (ref));
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
	  for (k = 0; k < n_basic_blocks; k++)
	    {
	      tree_ref w = phi_operand (ref, BASIC_BLOCK (k));
	      tree_ref defw;
	      if (!w || !expruse_def (w) )
		continue;
	      defw = expruse_def (w);
	      if ((ref_type (defw) == E_PHI 
		   && !exprphi_extraneous (defw))
		  || (ref_type (defw) == E_USE 
		      && !expruse_phiop (defw)))
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

/* First part of finalize algorithm.  */
static void
finalize_1 (ei, temp)
     struct expr_info *ei;
     tree temp;
{
  tree_ref X;
  int x;
  fibheap_t fh;
  avdefs = xcalloc (class_count + 1,sizeof (tree_ref));
  fh = fibheap_new ();
  insert_euse_in_preorder_dt_order (ei, fh);
  while (!fibheap_empty (fh))
    {
      X = fibheap_extract_min (fh);
      x = exprref_class (X);
      if (ref_type (X) == E_PHI)
        {
          if (exprphi_willbeavail (X))
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
              set_expruse_def (X, avdefs[x]);
            }
        }
      else
        {
          tree_ref phi = phi_for_operand (ei, X);
          if (!phi)
            abort ();
          if (exprphi_willbeavail (phi))           
            {
              if (can_insert (X))
                {
		  tree stmt;
		  tree expr;
		  tree *place;
		  basic_block bb;
		  tree endtree;

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
		  expr = fold (build_modify_expr (temp, NOP_EXPR, 
						  deep_copy_node (ei->expr)));
		  bb = ref_bb (X);
		  endtree = last_stmt (bb);
		  /* FIXME: Need DFA inserting updating to do this the right
		     way. */
		  /* If it's a goto, we need to insert *before* it.
		     This might not be necessary once goto elimination
		     is functional.  */
		  if (is_ctrl_altering_stmt (endtree))
		    {
		      stmt = build (COMPOUND_EXPR, void_type_node, expr, endtree);
		      place = &TREE_OPERAND (stmt, 0);
		    }
		  else
		    {
		      stmt = build (COMPOUND_EXPR, void_type_node, endtree, expr);
		      place = &TREE_OPERAND (stmt, 1);
		    }
		  
		  *bb->end_tree_p = stmt;
		  TREE_CHAIN (stmt) = TREE_CHAIN (endtree);
		  TREE_CHAIN (endtree) = NULL;
		  TREE_TYPE (stmt) = TREE_TYPE (expr);

#if WAITING_FOR_DFA_UPDATE
		  /* Update the SSA for the newly inserted definition.
		     This is a phi operand occurrence insertion, so we know
		     *something* caused one or more variables in our expression
		     to be modified in this block. 
		     We can find the definitions of the modified variables by
		     finding the real phis for each variable (if one exists),
		     which will be in the ephi's basic block.
		     We can find the definitions of non-modified variables
		     by walking back up the dominator tree, seeing if we have a
		     def or use of the variable in that bb, and getting it from
		     that. 
		  */
		/* FIXME: Hack.  Passing the address of local trees to
		   create_ref is not correct.  We should keep statement and
		   expression pointers here.  */
		  find_refs_in_stmt (stmt, bb);
		  i = rli_start (tree_refs (stmt));
		  for (; !rli_after_end (i); rli_step (&i))
		    {
		      if (ref_type (rli_ref (i)) != V_USE)
			continue;
		      find_reaching_def_of_var (rli_ref (i), bb, phi);  
		    }
		  
		  splay_tree_insert (new_stmt_map, 
				     (splay_tree_key) expr,
				     (splay_tree_value)  stmt);
		  
		 

#endif
                  /*
                    expruse_def (X) = new occurrence. 
		  */		  

		  set_expruse_def (X,create_ref (expr, E_USE, 0,
						 ref_bb (X), bb->end_tree_p,
						 place, place,  true));
		  VARRAY_PUSH_GENERIC_PTR (ei->erefs, expruse_def (X));
		  set_exprref_reload (expruse_def (X), false);
		  set_exprref_save (expruse_def (X), true);
		  set_exprref_inserted (expruse_def (X), true);
		  set_expruse_phiop (expruse_def (X), false);
		  set_expruse_has_real_use (expruse_def (X), true);
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
  fibheap_delete (fh);
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

  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->reals); i++)
    {
      tree real = VARRAY_TREE (ei->reals, i);
      sbitmap temp = compute_idfs (dfs, VARRAY_TREE (ei->realstmts, i));      
      sbitmap_a_or_b (dfphis, dfphis, temp);
      sbitmap_free (temp);
      if (is_simple_id (TREE_OPERAND (real, 0)) 
	  || (TREE_OPERAND (real, 1) 
	      && is_simple_id (TREE_OPERAND (real, 1))))
        {
          int varcount = 0;
	  ref_list_iterator j;

	  j = rli_start (tree_refs (VARRAY_TREE (ei->realstmts, i)));
	  for (; !rli_after_end (j); rli_step (&j))
	  {
	    tree_ref ref = rli_ref (j);
	    if (ei->strred_cand)
	      while (ref_type (ref) == V_USE
		     && ref_var (ref) == TREE_OPERAND (real, 0)
		     && imm_reaching_def (ref)
		     && is_injuring_def (ei, ref_expr (imm_reaching_def (ref))))
		{
		  ref = find_rhs_use_for_var (imm_reaching_def (ref), 
					      ref_var (ref));
		  if (is_default_def (imm_reaching_def (ref))
		      || ref_type (imm_reaching_def (ref)) == V_PHI)
		    break;
		  
		  ref = find_rhs_use_for_var (imm_reaching_def (ref), 
					      ref_var (ref));
		}
	      /* ??? If the trees aren't shared, will the last part of this ||
		 work right? */ 
              if (ref_type (ref) != V_USE
                  || !is_simple_modify_expr (ref_expr (ref))
                  /*|| TREE_OPERAND (ref_expr (ref), 1) != real*/)
                continue;
              if (ref_var (ref) != TREE_OPERAND (real, 0)
                  && (TREE_OPERAND (real, 1) == NULL 
		      || ref_var (ref) != TREE_OPERAND (real, 1)))
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
    tree_ref ref = create_ref (ei->expr, E_PHI, 0,
                                BASIC_BLOCK (i), 
                                NULL, NULL, NULL, true);
    VARRAY_PUSH_GENERIC_PTR (ei->erefs, ref);
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
  basic_block bb;
  
  if (expruse_has_real_use (phiop))
    return;
  phi = expruse_def (phiop);
  if (!phi || ref_type (phi) != E_PHI)
    return;
  if (!exprphi_downsafe (phi))
    return;  
  set_exprphi_downsafe (phi, false);
  FOR_EACH_BB (bb)
    if (phi_operand (phi, bb) != NULL)
      reset_down_safe (phi_operand (phi, bb));
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
      basic_block block2;
      if (phi == NULL || exprphi_downsafe (phi))
        continue;
      FOR_EACH_BB (block2)
        if (phi_operand (phi, block2) != NULL)
          reset_down_safe (phi_operand (phi, block2));
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
	  tree_ref operand = phi_operand (phi, pred->src);
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
              tree_ref operand = phi_operand (phi, pred->src);
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
  basic_block block;

  set_exprphi_canbeavail (phi, false);
  FOR_EACH_BB (block)
    {
      edge pred;
      tree_ref other = phi_at_block (ei, block);
      
      if (other == NULL)
        continue;
      
      for (pred = block->pred; pred; pred = pred->pred_next)
        {
          if (pred->src != ENTRY_BLOCK_PTR)
            {
              tree_ref operand = phi_operand (other, pred->src);
              if (expruse_def (operand) == phi)
                {
                  if (!expruse_has_real_use(operand))
		    {
		      if (!exprphi_downsafe (other) 
			  && exprphi_canbeavail (other))
			reset_can_be_avail (ei, other);
		    }
                }
            }
        }
    }
}

/*  Reset later flags.  */
static void 
reset_later (ei, phi)
     struct expr_info *ei;
     tree_ref phi;
{
  basic_block block;

  set_exprphi_later (phi, false);
  FOR_EACH_BB (block)
    {
      edge pred;
      tree_ref other = phi_at_block (ei, block);
      
      if (other == NULL)
        continue;
      
      for (pred = block->pred; pred; pred = pred->pred_next)
        {
          if (pred->src != ENTRY_BLOCK_PTR)
            {
              tree_ref operand = phi_operand (other, pred->src);
              if (expruse_def (operand) == phi)
                {
                  if (exprphi_later (other))                
                    reset_later (ei, other);
                }
            }
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
      set_exprphi_later (phi,exprphi_canbeavail (phi));
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
              tree_ref operand = phi_operand (phi, pred->src);
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
      if (ref_expr (p))
	vp = find_use_for_var (ref_expr (p), v);
      vq = find_use_for_var (ref_expr (q), v);
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
  STRIP_WFL (expr);
  
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
     
  for (i = 0; i < 2; i++)
    {
      tree incr = integer_zero_node;
      tree_ref v;
      if (!DECL_P (TREE_OPERAND (ei->expr, i)))
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
	  if (ref_var (find_def_for_stmt (ref_stmt (v))) 
	      != TREE_OPERAND  (ei->expr, i))
	    continue;
	  while (is_injuring_def (ei, ref_expr (v)))
	    {
	      add_ref_to_list_begin (toprocess, v);
	      v = find_rhs_use_for_var (v, TREE_OPERAND (ei->expr, i));
	      if (is_default_def (imm_reaching_def (v))
		  || ref_type (imm_reaching_def (v)) == V_PHI
		  || !maybe_find_rhs_use_for_var (imm_reaching_def (v),
						  TREE_OPERAND (ei->expr, i)))
		break;
	      v = find_rhs_use_for_var (imm_reaching_def (v), 
					TREE_OPERAND (ei->expr, i));
	    }
	  
	  /* The *first* reference will probably not have a reaching def we can
	     set yet, since it'll come from a save that hasn't been done
	     yet. */
	  for (j = rli_start (toprocess); !rli_after_end (j); rli_step (&j))
	    {
	      v = rli_ref (j);
	      if (htab_find (ei->repaired, ref_expr (v)) == NULL)
		{
#if DEBUGGING_STRRED
		  if (dump_file)
		      {
			fprintf (dump_file, "Injuring def to repair is: ");
			print_generic_expr (dump_file, ref_expr (v), 0);
			fprintf (dump_file, "\n");
		      }
#endif
		    incr = calculate_increment (ei, ref_expr (v));
		    expr = build_modify_expr (temp, NOP_EXPR, 
					      fold (build (PLUS_EXPR, 
							  TREE_TYPE (temp),
							  temp, incr)));
		    stmt = build (COMPOUND_EXPR, void_type_node, ref_stmt (v), expr);
		    TREE_TYPE (stmt) = TREE_TYPE (expr);
		    bb = ref_bb (use);
		    replace_ref_stmt_with (v, stmt);
		    *(htab_find_slot (ei->repaired, ref_expr (v), INSERT)) = ref_expr (v);
#if WAITING_FOR_DFA_UPDATE		  
		    /* Update SSA for the repair.  
		      1. Find the refs in the statement.
		      2. Add the ref to the list of refs to find reaching defs
		      for later (we don't necessarily have the eventual defs
		      inserted yet). 
		      3. Insert into new statement map the tuple (orig_euse,
		      repair stmt).
		    */
		  /* FIXME: Hack.  Passing the address of local trees to
		    create_ref is not correct.  We should keep statement and
		    expression pointers here.  */
		    find_refs_in_stmt (stmt, bb);
		    add_ref_to_list_begin (ei->injfixups, 
					  find_def_for_stmt (stmt));
		    splay_tree_insert (new_stmt_map, 
				      (splay_tree_key) orig_euse,
				      (splay_tree_value) stmt);
#endif
		  }
	    }
	}
      else if (ref_type (imm_reaching_def (v)) == V_PHI)
	{
	  size_t curr_phi_operand;
	  tree_ref phi = imm_reaching_def (v);
	  for (curr_phi_operand = 0; 
	       curr_phi_operand < num_phi_args (phi); 
	       curr_phi_operand++)
	    {
	      phi_node_arg phi_oper = phi_arg (phi, curr_phi_operand);
	      tree_ref phi_op = phi_arg_def (phi_oper);
	      tree_ref ephi = phi_at_block (ei, ref_bb (phi));
	      tree_ref ephi_op = phi_operand (ephi, 
					      phi_arg_edge (phi_oper)->src);
	      repair_injury (ei, phi_op, temp, ephi_op);
	    }
	}
      else
	abort ();
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
    return NULL;
  
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
	  || ref_expr (var) == ref_expr (tempref)) 
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
     tree_ref defby;
     basic_block bb;
     
{
  ref_list_iterator i;
  splay_tree_node reachingnode;
  tree_ref use_orig_ref;
  tree_ref olddefref, newdefref, newuseref;
  tree reachingstmt;
  tree newmodifyexpr, useexpr;
  ref_list todelete = create_ref_list ();
  ref_list reachedtoadd = create_ref_list ();
  ref_list immtoadd = create_ref_list ();
  
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

  for (i = rli_start (tree_refs (newuse)); !rli_after_end (i); rli_step (&i))
    {
      if (ref_type (rli_ref (i)) == V_DEF)
	{
	  use_orig_ref = rli_ref (i);
	  break;
	}
    }
#if WAITING_FOR_DFA_UPDATE
  olddefref = find_def_for_stmt (ref_stmt (use_orig_ref));
  i = rli_start (tree_refs (ref_expr (use_orig_def)));
  for (; !rli_after_end (i); rli_step (&i))
    {
      tree_ref tempref = rli_ref (i);
      remove_ref_from_list (tree_refs (ref_stmt (use_orig_ref)),
	  tempref);
      if (ref_type (tempref) == V_DEF)
	{
	  add_list_to_list_end (immtoadd, imm_uses (tempref));
	  add_list_to_list_end (reachedtoadd, 
	      reached_uses (tempref));
	}
      if (ref_type (tempref) != V_USE)
	continue;
      remove_tree_ref (ref_var (tempref), tempref);
      remove_bb_ref (bb, tempref);
      if (imm_reaching_def (tempref) != NULL)
	{
	  tree_ref rdef = imm_reaching_def (tempref);

	  remove_ref_from_list (imm_uses (rdef), tempref);
	  remove_ref_from_list (reached_uses (rdef), tempref);
	}
      add_ref_to_list_end (todelete, tempref);		
    }
  
  for (i = rli_start (todelete); !rli_after_end (i); rli_step (&i))
    remove_tree_ref (ref_expr (use_orig_ref), rli_ref (i));

  ref_expr (use_orig_ref)->common.ann = NULL;
#endif
  useexpr = ref_expr (use_orig_ref);
  STRIP_WFL (useexpr);
  newmodifyexpr = build_modify_expr (TREE_OPERAND (useexpr, 0), 
				     NOP_EXPR, temp);
  replace_ref_expr_with (use_orig_ref, newmodifyexpr);

#if WAITING_FOR_DFA_UPDATE

  /* FIXME: Hack.  Passing the address of local trees to create_ref is not
     correct.  We should keep statement and expression pointers here.  */
  newdefref = create_ref (TREE_OPERAND (ref_expr (use_orig_ref), 0),
			  V_DEF, 0, bb, ref_stmt (use_orig_ref), 
			  ref_expr (use_orig_ref),
			  &TREE_OPERAND (ref_expr (use_orig_ref), 0),
			  true);
  add_list_to_list_end (reached_uses (newdefref), reachedtoadd);
  add_list_to_list_end (imm_uses (newdefref), immtoadd);
  
  update_phis_in_list (reached_uses (newdefref), olddefref, newdefref);
  update_phis_in_list (imm_uses (newdefref), olddefref, newdefref);
  
  /* FIXME: Hack.  Passing the address of local trees to create_ref is not
     correct.  We should keep statement and expression pointers here.  */
  newuseref = create_ref (temp, V_USE, 0, bb, ref_stmt (use_orig_ref),
			  ref_expr (use_orig_ref), 
			  find_expr_in_tree (ref_expr (use_orig_ref), temp), 
			  true);
  if ( (ref_type (defby) != E_PHI))
    {
      /* For non-PHI's we created, the  map is euse expression -> new pretmp
	 save stmt. */ 
      reachingnode = splay_tree_lookup (new_stmt_map, 
					(splay_tree_key) ref_expr (defby));
      if (!reachingnode)
	abort();
      reachingstmt = (tree) reachingnode->value;
      
      set_imm_reaching_def (newuseref, find_def_for_stmt (reachingstmt));
      add_ref_to_list_end (reached_uses (find_def_for_stmt (reachingstmt)), 
			   newuseref);
      add_ref_to_list_end (imm_uses (find_def_for_stmt (reachingstmt)), 
			   newuseref);      
    }
  else
    {
      /* For PHI nodes we create, the map is ephi -> inserted phi. 
	 it. */
      tree_ref reachingphi;
      reachingnode = splay_tree_lookup (new_stmt_map,
					(splay_tree_key) defby);
      if (!reachingnode)
	abort();
      
      reachingphi = (tree_ref) reachingnode->value;
      set_imm_reaching_def (newuseref, reachingphi);
      add_ref_to_list_end (reached_uses (reachingphi), newuseref);
      add_ref_to_list_end (imm_uses (reachingphi), newuseref);
    }
#endif      
  
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
  fibheap_t exprs;
  tree_ref use;
  
  ei->injfixups = create_ref_list ();
  need_repair_map = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);
  exprs = fibheap_new ();
  if (ei->strred_cand)
    {
      insert_euse_in_preorder_dt_order (ei, exprs);
      while (!fibheap_empty (exprs))
	{
	  use = fibheap_extract_min (exprs);
	  if ((ref_type (use) == E_USE)
	      && exprref_reload (use) 
	      && !exprref_inserted (use))
	    {
	      repair_injury (ei, use, temp, use);
	      /*	    set_need_repair (ei, use);*/
	    }
	}
    }
  insert_euse_in_preorder_dt_order (ei, exprs);
  while (!fibheap_empty (exprs))
    {
      use = fibheap_extract_min (exprs);
      if (ref_type (use) == E_USE /*&& !EXPRUSE_PHIOP (use) */
	  && !exprref_inserted (use))
	{
	  if (expruse_phiop (use) 
	      && (exprref_reload (use) || exprref_save (use)))
	    abort();
	  if (exprref_save (use))
	    {
	      basic_block use_bb = ref_bb (use);
	      tree use_stmt = ref_stmt (use);
	      tree use_expr = ref_expr (use);
	      tree_ref use_orig_ref = NULL;
	      tree newexpr, newstmt;
	      size_t j;	      
	      ref_list_iterator i;
	      
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
			   EXPR_WFL_LINENO (use_expr));
		}
	      STRIP_WFL (use_expr);
	      newexpr = fold (build_modify_expr (temp, NOP_EXPR, 
						 deep_copy_node (TREE_OPERAND (use_expr, 1))));
	      newstmt = build (COMPOUND_EXPR, void_type_node, newexpr,
			       use_stmt);
	      replace_ref_stmt_with (use, newstmt);
	      use->common.stmt_p = &TREE_OPERAND (newstmt, 1);
	      use->common.expr_p = &TREE_OPERAND (newstmt, 1);
	      /* REMOVE AFTER DFA UPDATE */
	      i = rli_start (tree_refs (use_stmt));
	      for (; !rli_after_end (i); rli_step (&i))
		{
		  tree_ref a = rli_ref (i);
		  if (ref_stmt (a) == newstmt)
		    {
		      a->common.stmt_p = &TREE_OPERAND (newstmt, 1);
		      a->common.expr_p = &TREE_OPERAND (newstmt, 1);
		    }
		}	      
	      /* END REMOVE AFTER DFA UPDATE */
	      update_ssa_for_new_use (temp, ref_expr (use), use, use_bb);

#if WAITING_FOR_DFA_UPDATE
	      /* Update the SSA representation for the new def of the
		 temporary. 
		 1. Create the defs and uses for the new statement.
		 2. Copy the reaching def from the old uses (in the assignment 
		 expression we are going to replace with the temporary next).
	      */
	      /* FIXME: Hack.  Passing the address of local trees to
		 create_ref is not correct.  We should keep statement and
		 expression pointers here.  */
	      find_refs_in_stmt (newstmt, use_bb);
	      
	      i = rli_start (tree_refs (newexpr));
	      for (; !rli_after_end (i); rli_step (&i))
		{
		  tempref = rli_ref (i);
		  tree_ref olddef;
		  if (ref_type (tempref) != V_USE)
		    continue;

		  olddef = find_use_for_var (use_expr, ref_var (tempref));
		  olddef = imm_reaching_def (olddef);

		  set_imm_reaching_def (tempref, olddef);
		}
	      
	      splay_tree_insert (new_stmt_map, 
				 (splay_tree_key) ref_expr (use), 
				 (splay_tree_value) newstmt);
	      update_ssa_for_new_use (temp, ref_expr (use), use, use_bb);
#if DEBUGGING_SSA_UPDATE 
	      if (dump_file)
		{
		  fprintf (dump_file, "\nUpdated a save use:\n");
		  dump_ref_list (dump_file, "", tree_refs (use_stmt), 0, 1);
		}
#endif
#endif
	    }
	  else if (exprref_reload (use))
	    {
	      basic_block use_bb = ref_bb (use);
	      tree use_stmt = ref_stmt (use);
	      tree use_expr = ref_expr (use);
	      if (dump_file)
                {
		  fprintf (dump_file, "In BB %d, insert reload of ", 
			   use_bb->index);
                  print_generic_expr (dump_file, ei->expr, 0);
                  fprintf (dump_file, " from ");
                  print_generic_expr (dump_file, temp, 0);
                  fprintf (dump_file, " in statement ");
                  print_generic_expr (dump_file, TREE_OPERAND (use_stmt, 0), 0);
                  fprintf (dump_file, " on line %d\n", 
			   EXPR_WFL_LINENO (use_stmt));
                }
	      /* Update the SSA representation for a new use of the temporary.
		 Do exactly what we do for the new use generated by the save. 
	      */
	      update_ssa_for_new_use (temp, ref_expr (use), expruse_def (use),
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
#if WAITING_FOR_DFA_UPDATE
      else if (ref_type (use) == E_PHI)
	{
	  /* E_PHI's become real PHI's of the temporary. */
	  tree_ref phi;
	  tree_ref phiop;
	  size_t i;
	  splay_tree_node phidefnode, defbynode;
#if DEBUGGING_SSA_UPDATE
	  if (dump_file)
	    {
	      fprintf (dump_file, "Creating a new phi for an EPHI\n");
	    }
#endif

	  /* FIXME: Hack.  Passing the address of local trees to
	     create_ref is not correct.  We should keep statement and
	     expression pointers here.  */
	  phi = create_ref (temp, V_PHI, 0, ref_bb (use), 
			    first_stmt (ref_bb (use)),
			    NULL, NULL, false);
	  add_ref_to_list_begin (bb_refs (ref_bb (use)), phi);
	  splay_tree_insert (new_stmt_map, 
			     (splay_tree_key) use, 
			     (splay_tree_value) phi);
	  for (i = 0; i < VARRAY_SIZE (exprphi_phi_args (use)); i++)
	  {
	    edge pred;
	    tree_ref defby;
	    
	    phiop = VARRAY_GENERIC_PTR (exprphi_phi_args (use), i);
	    if (!phiop || expruse_def (phiop) == NULL)
	      continue;
	    
	    for (pred = ref_bb (use)->pred; pred; pred = pred->pred_next)
	      {
		if (pred->src == ref_bb (phiop))
		  break;
	      }
	    if (!pred)
	      abort();
	    /*  Sigh. For strength reduction, the ephi can quite possibly
	        have arguments that are all the same (since the injuries are
		transparent).  Thus, for these cases, we store the
		new phi def in the map under the key of the phi operand, 
		rather than the phi operand's definition.  
		So, we see if we have something in the map for the phiop, and,
		if so, use that node.
	    */
	    
	    defbynode = splay_tree_lookup (new_stmt_map, 
					   (splay_tree_key) phiop);
	    defby = defbynode ? phiop : expruse_def (phiop);
	    if (!expruse_phiop (defby) 
		&& !exprref_inserted (defby) 
		&& !exprref_save (defby))
	      continue;
	    if (ref_type (defby) == E_USE)
	      {
		tree_ref tempdef;
		if (expruse_phiop (defby))
		  
		  phidefnode = splay_tree_lookup (new_stmt_map,
						  (splay_tree_key) defby);
		else
		  phidefnode = splay_tree_lookup (new_stmt_map,
						  (splay_tree_key) ref_expr (defby));
		if (!phidefnode)
		  abort();
		tempdef = find_def_for_stmt ((tree) phidefnode->value);
		add_phi_arg (phi, tempdef, pred);
		if (find_list_node (reached_uses (tempdef), phi) == NULL)
		  add_ref_to_list_end (reached_uses (tempdef), phi);
		if (find_list_node (imm_uses (tempdef), phi) == NULL)
		  add_ref_to_list_end (imm_uses (tempdef), phi);
		
	      }
	    else
	      {
		phidefnode = splay_tree_lookup (new_stmt_map,
						(splay_tree_key) defby);
		if (!phidefnode)
		  abort();
		add_phi_arg (phi, (tree_ref) phidefnode->value, pred);
		
	      }  
	  }
	}
    }
#endif
#if WAITING_FOR_DFA_UPDATE
  for (i = rli_start (ei->injfixups); !rli_after_end (i); rli_step (&i))
  {
    ref_list_iterator j;
    tempref = rli_ref (i);

    j = rli_start (tree_refs (ref_expr (tempref)));
    for (; !rli_after_end (j); rli_step (&j))
      {
	tree_ref tempref2 = rli_ref (j);

	if (ref_type (tempref2) != V_USE)
	  continue;    
	find_reaching_def_of_var (tempref2, ref_bb (tempref2), NULL);
#if DEBUGGING_SSA_UPDATE
	if (dump_file)
	  {
	    fprintf (dump_file, "Injury use and new reaching def:\n");
	    dump_ref (dump_file, "", tempref2, 0, 1);
	  }
#endif
      }
#endif
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

/* Add call expression to expr-infos.  */
static int
add_call_to_ei (slot, data)
     struct expr_info *slot;
     void *data;
{
  tree_ref call = (tree_ref) data;
  struct expr_info *ei = (struct expr_info *) slot;
  VARRAY_PUSH_GENERIC_PTR (ei->occurs, call->common.expr_p);
  VARRAY_PUSH_GENERIC_PTR (ei->occurstmts, call->common.stmt_p);
  VARRAY_PUSH_GENERIC_PTR (ei->refs, call);
  return 0;
}

/* Traverse over expressions to perform PRE on.  */
static int
pre_part_1_trav (slot, data)
     struct expr_info *slot;
     void *data;
{
  tree temp;
  tree fnbody;
  struct expr_info *ei = (struct expr_info *) slot;
  if (VARRAY_ACTIVE_SIZE (ei->reals) < 2)
    return 0;

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
    size_t i;
    for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->realstmts) && !def; i++)
      def = find_def_for_stmt (VARRAY_TREE (ei->realstmts, 0));

    if (def != NULL)
      type_of_expr = TREE_TYPE (ref_var (def));
    else
      type_of_expr = TREE_TYPE (VARRAY_TREE (ei->reals, 0));

    temp = create_tmp_var (type_of_expr, "pretmp");
    finalize_1 (ei, temp);
    finalize_2 (ei);
    code_motion (ei, temp);
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

void 
tree_perform_ssapre ()
{
  tree fn = DECL_SAVED_TREE (current_function_decl);
    /* First, we need to find our candidate expressions. */
  varray_type bexprs;
  htab_t seen = htab_create (37, htab_hash_pointer, htab_eq_pointer, NULL);
  basic_block bb;
  size_t j, k;

  new_stmt_map = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);
  
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
  FOR_EACH_BB (bb)
    {    
      ref_list_iterator i;

      htab_empty (seen);
      for (i = rli_start (bb_refs (bb)); !rli_after_end (i); rli_step (&i))
	{
	  tree_ref ref = rli_ref (i);
	  tree expr = ref_expr (ref);
	  tree orig_expr = expr;
	  tree stmt = ref_stmt (ref);
	  if (ref_type (ref) != V_USE)
	    continue;
	  STRIP_WFL (expr);
	  if (htab_find (seen, orig_expr) != NULL)
	    continue;
	  if (is_simple_modify_expr (expr))
	    {
	      expr = TREE_OPERAND (expr, 1);
	    }
	  if ((is_simple_binary_expr (expr)
/*	       || is_simple_cast (expr)
	       || is_simple_unary_expr (expr)*/)
	     && TREE_CODE (expr) != INDIRECT_REF
	     && !is_simple_condexpr (expr))
	    {
	      struct expr_info *slot = NULL;
	      size_t k;
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
		  VARRAY_PUSH_GENERIC_PTR (slot->occurs, ref->common.expr_p);
		  VARRAY_PUSH_GENERIC_PTR (slot->occurstmts, ref->common.stmt_p);
		  VARRAY_PUSH_TREE (slot->reals, expr);
		  VARRAY_PUSH_TREE (slot->realstmts, stmt);
		  VARRAY_PUSH_GENERIC_PTR (slot->refs, ref);
		  slot->strred_cand &= is_strred_cand (orig_expr);
		}
	      else
		{
		  slot = ggc_alloc (sizeof (struct expr_info));
		  slot->expr = expr;
		  VARRAY_GENERIC_PTR_INIT (slot->occurs, 1, "Kills and occurrence");
		  VARRAY_GENERIC_PTR_INIT (slot->occurstmts, 1, 
				    "Kills and occurrence statements");
		  VARRAY_TREE_INIT (slot->reals, 1, "Real occurrences");
		  VARRAY_TREE_INIT (slot->realstmts, 1, 
				    "Real occurrence statements");
		  VARRAY_GENERIC_PTR_INIT (slot->phis, last_basic_block, 
					   "EPHIs");
		  VARRAY_GENERIC_PTR_INIT (slot->erefs, 1, "EREFs");
		  VARRAY_GENERIC_PTR_INIT (slot->refs, 1, "REFs");
		  VARRAY_PUSH_GENERIC_PTR (slot->occurs, ref->common.expr_p);
		  VARRAY_PUSH_GENERIC_PTR (slot->occurstmts, ref->common.stmt_p);
		  VARRAY_PUSH_TREE (slot->reals, expr);
		  VARRAY_PUSH_TREE (slot->realstmts, stmt);
		  VARRAY_PUSH_GENERIC_PTR (slot->refs, ref);

		  VARRAY_PUSH_GENERIC_PTR (bexprs, slot);
		  slot->strred_cand = is_strred_cand (orig_expr);
		  slot->repaired = htab_create (7, htab_hash_pointer, 
						htab_eq_pointer, NULL);
		}
	    }
	  else if (TREE_CODE (expr) == CALL_EXPR 
		   && TREE_CODE (ref_var (ref)) == FUNCTION_DECL)
	    {
	      if (!DECL_IS_PURE (ref_var (ref)))
	      {
		for (k = 0; k < VARRAY_ACTIVE_SIZE (bexprs); k++)
	  	  add_call_to_ei (VARRAY_GENERIC_PTR (bexprs, k), ref);
	      }
	    }  
	  *(htab_find_slot  (seen, orig_expr, INSERT)) = orig_expr;
	}
    }
  for (j = 0; j < VARRAY_ACTIVE_SIZE (bexprs); j++)
     pre_part_1_trav (VARRAY_GENERIC_PTR (bexprs, j), pre_dfs);
  /*  simplify_stmt (fn, NULL_TREE); */
  /* Debugging dump after SSA PRE */
  if (dump_file)
    {
      fprintf (dump_file, "%s()\n", get_name (current_function_decl));
      
      if (dump_flags & TDF_RAW)
        dump_node (fn, TDF_SLIM | dump_flags, dump_file);
      else
        print_generic_stmt (dump_file, fn, dump_flags);

      dump_end (TDI_pre, dump_file);
    }
  
  for (j = 0; j < VARRAY_ACTIVE_SIZE (bexprs); j++)
    free_expr_info (VARRAY_GENERIC_PTR (bexprs, j));
  VARRAY_CLEAR (bexprs);
  htab_delete (seen);
  free_dominance_info (pre_idom);
  free (pre_preorder);
  sbitmap_vector_free (pre_dfs);
  sbitmap_vector_free (domchildren);

  splay_tree_delete (new_stmt_map);
  
}
