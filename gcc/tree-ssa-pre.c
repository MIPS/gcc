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
#include "errors.h"
#include "ggc.h"
#include "tree.h"

/* These RTL headers are needed for basic-block.h.  */
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "diagnostic.h"
#include "tree-inline.h"
#include "tree-flow.h"
#include "tree-simple.h"
#include "tree-dump.h"
#include "timevar.h"
#include "fibheap.h"
#include "hashtab.h"
#include "ssa.h"

/* See http://citeseer.nj.nec.com/chow97new.html, and
   http://citeseer.nj.nec.com/kennedy99partial.html for details of the
   algorithm.

   kennedy99partial is newer, and is what this implementation is based
   on.

   For strength reduction addition, see 
   http://citeseer.nj.nec.com/kennedy98strength.html

   Pieces are also taken from Open64's SSAPRE implementation.

   
   Since the papers are a bit dense to read, take a while to grasp,
   and have a few bugs, i'll give a quick rundown:
   
   Normally, in non-SSA form, one performs PRE on expressions using
   bit vectors, determining properties for all expressions at once
   through bitmap operations and iterative dataflow.
   
   SSAPRE, like most non-SSA->SSA algorithm conversions, operates one
   expression at a time, and doesn't use bitvectors or iterative dataflow.
   
   To be able to do this, we need an SSA form for expressions.
   If you are alread confused, you likely think an expression, as
   used here, is something like "b_3 = a_2 + 5".  It's not. It's "a +
   5". "a_2 + 5" is an *occurrence* of the expression "a + 5".  Just
   like PRE, it's lexical equivalence that matters.
   Compilers generally give you an SSA form for variables, and maybe
   arrays (and/or conditionals).  But not for expressions.
   
   GCC doesn't give you one either, so we have to build it.
   Thus, the first steps of SSAPRE are to do just these things.

   First we collect lists of occurrences of expressions we are going
   to operate on.
   Note that:
   Unlike the paper, we don't have to ever add newly formed
   expressions to the list (for normal SSAPRE, anyway), because we
   don't have expressions with more than two operators, and each
   operator is either a constant or a variable.  Thus, no second
   order effects.
   
   Once we have the lists of occurrences, we process one expression
   at a time, doing the following:
   1. Using a slightly modified SSA phi placement algorithm, place
   expression PHI's for expressions.
   2. Using a two step optimistic SSA renaming algorithm, version the
   nodes and link phi operands to their real occurrences, if they
   exist.  This creates a factored graph of our expression SSA occurrences.
   3. Using the factored graph, compute downsafe, avail, and later for
   EPHIs.
   4. Using EPHI availability information and versions, compute what
   occurrences need to have saves, and what occurrences can be
   reloaded from an already saved value.
   5. Insert the saves and reloads, and transform EPHIs into regular
   phis of the temporary we use for insertion/saving.  */

/* TODOS: 
   
Do strength reduction on a +-b and -a, not just a * <constant>.  
Get rid of the ephis array in expr_info, since it's not necessary
anymore.  */
/* Debugging dumps.  */
static FILE *dump_file;
static int dump_flags;

struct expr_info;
static bool expr_lexically_eq PARAMS ((const tree, const tree));
static void free_expr_info PARAMS ((struct expr_info *));
static bitmap compute_idfs PARAMS ((bitmap *, tree));
static void compute_domchildren PARAMS ((dominance_info, sbitmap *));
static inline bool a_dom_b PARAMS ((tree, tree));
static void set_var_phis PARAMS ((struct expr_info *, tree));
static void calculate_preorder PARAMS ((void));
static bool names_match_p PARAMS ((const tree, const tree));
static bool is_strred_cand PARAMS ((const tree));
static int pre_expression PARAMS ((struct expr_info *, void *));
static bool is_injuring_def PARAMS ((struct expr_info *, tree));
static inline bool okay_injuring_def PARAMS ((tree, tree));
static void expr_phi_insertion PARAMS ((bitmap *, struct expr_info *));
static tree factor_through_injuries PARAMS ((struct expr_info *, tree, tree));
static inline tree maybe_find_rhs_use_for_var PARAMS ((tree, tree));
static inline tree find_rhs_use_for_var PARAMS ((tree, tree));
static tree create_ephi_node PARAMS ((basic_block));
static inline int opnum_of_phi PARAMS ((tree, int));
static varray_type phi_opnd_from_res PARAMS ((struct expr_info *, tree, 
					      int, basic_block));
static splay_tree rename_2 PARAMS ((struct expr_info *, varray_type *));
static void rename_1 PARAMS ((struct expr_info *));
static void assign_new_class PARAMS ((tree, varray_type *, varray_type *));
static void insert_occ_in_preorder_dt_order_1 PARAMS ((struct expr_info *, 
						       fibheap_t,
						       basic_block));
static void insert_occ_in_preorder_dt_order PARAMS ((struct expr_info *, 
						     fibheap_t));
static void insert_euse_in_preorder_dt_order_1 PARAMS ((struct expr_info *, 
							basic_block));
static void insert_euse_in_preorder_dt_order PARAMS ((struct expr_info *));
static inline bool defs_match_p PARAMS ((struct expr_info *, const varray_type,
					 const tree));
static inline bool defs_y_dom_x PARAMS ((struct expr_info *, const varray_type,
					 const tree));
static void reset_down_safe PARAMS ((tree));
static void down_safety PARAMS ((struct expr_info *));
static void will_be_avail PARAMS ((struct expr_info *));
static void compute_can_be_avail PARAMS ((struct expr_info *));
static void reset_can_be_avail PARAMS ((struct expr_info *, tree));
static void compute_later PARAMS ((struct expr_info *));
static void reset_later PARAMS ((struct expr_info *, tree));
static inline tree ephi_operand_for_pred PARAMS ((tree, edge));
static void add_ephi_arg PARAMS ((tree, tree, edge));
static bool finalize_1 PARAMS ((struct expr_info *, tree));
static void finalize_2 PARAMS ((struct expr_info *));
static void code_motion PARAMS ((struct expr_info *, tree));
static tree calculate_increment PARAMS ((struct expr_info *, tree));
static void repair_euse_injury PARAMS ((struct expr_info *, tree, tree));
static void repair_ephi_injury PARAMS ((struct expr_info *, tree, tree));
static void repair_phi_injury PARAMS ((struct expr_info *, tree, tree));
static void repair_use_injury PARAMS ((struct expr_info *, tree, tree));
static bool can_insert PARAMS ((tree));
static void set_save PARAMS ((struct expr_info *, tree));
static void update_old_new PARAMS ((struct expr_info *, tree *, tree *));
static tree reaching_def PARAMS ((tree, tree, basic_block, tree));
static tree * do_proper_save PARAMS ((struct expr_info *, tree , tree, tree));
static void process_left_occs_and_kills PARAMS ((varray_type, 
						 struct expr_info *, tree *));
static bool injured_ephi_operand PARAMS ((struct expr_info *, tree, int));
static void set_replacement PARAMS ((struct expr_info *, tree, tree));
static void remove_ephi PARAMS ((struct expr_info *, tree));
static inline bool ephi_has_bottom PARAMS ((tree));
static int add_call_to_ei PARAMS ((struct expr_info *, void *));
static bool call_modifies_slot PARAMS ((tree *, tree));
static tree create_expr_ref PARAMS ((tree, enum tree_code, basic_block, 
				     tree *));
static void set_expruse_def PARAMS ((tree, tree));
static inline bool ephi_will_be_avail PARAMS ((tree));
static int occ_compare PARAMS ((const void *, const void *));
static inline tree ephi_at_block PARAMS ((basic_block));
static inline tree ephi_operand_for_pred PARAMS ((tree, edge));
static void compute_dt_preorder PARAMS ((void));
static int search_dt_preorder PARAMS ((basic_block, int));
static bool requires_edge_placement PARAMS ((tree));
static int *pre_preorder;
static dominance_info pre_idom;
static bitmap *pre_dfs;
static int class_count;
static int preorder_count;
/* XXX: Move into expr_info or make it go away. */
static tree *avdefs;
static splay_tree idom_of_ephi;
static splay_tree dfn;

static struct pre_stats_d
{
  int reloads;
  int saves;
  int repairs;
  int newphis;
} pre_stats = {0, 0, 0, 0};


/* Map from pointers to statements we've replaced to pointers to where
   they are now.  This is used to update the occurs lists to account
   for movement (since the occurs list is a bunch of pointers to
   trees, not the trees themselves). */

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
  /* All the erefs. */
  varray_type erefs;
  /* True if it's a strength reduction candidate. */
  bool strred_cand;
  /* Map of repairs we've already completed. */
  htab_t repaired;
  /* The euses/ephis in preorder dt order. */
  varray_type euses_dt_order;
};

/* Add tree DEF coming from edge E as an argument to PHI. */
static void
add_ephi_arg (phi, def, e)
     tree phi;
     tree def;
     edge e;
{
  int i = EPHI_NUM_ARGS (phi);
  
  EPHI_ARG_DEF (phi, i) = def;
  EPHI_ARG_EDGE (phi, i) = e;
  EPHI_NUM_ARGS (phi)++;
}

/* Create a new EPHI node at basic block BB.  */
static tree
create_ephi_node (bb)
     basic_block bb;
{
  tree phi;
  int len;
  edge e;
  size_t size;
  bb_ann_t ann;

  for (len = 0, e = bb->pred; e; e = e->pred_next)
    len++;
  size = (sizeof (struct tree_ephi_node) 
	  + ((len - 1) * sizeof (struct phi_arg_d)));
  
  phi = ggc_alloc_tree (size);
  memset (phi, 0, size);
  ann = bb_ann (bb);
  if (ann->ephi_nodes == NULL)
    ann->ephi_nodes = phi;
  else
    chainon (ann->ephi_nodes, phi);

  TREE_SET_CODE (phi, EPHI_NODE);
  EPHI_NUM_ARGS (phi) = 0;
  EPHI_ARG_CAPACITY (phi) = len;

  /* Associate BB to the PHI node.  */
  set_bb_for_stmt (phi, bb);

  return phi;
}

/* Given DEF (which can be an SSA_NAME or entire statement), and VAR,
   find a use of VAR on the RHS of DEF, if one exists. Abort if we
   can't find one.  */
static inline tree
find_rhs_use_for_var (def, var)
     tree def;
     tree var;
{
  tree ret = maybe_find_rhs_use_for_var (def, var);
  if (!ret)
    abort ();
  return ret;
}

/* Given DEF (which can be an SSA_NAME or entire statement), and VAR,
   find a use of VAR on the RHS of DEF, if one exists.  Return NULL if
   we cannot find one.  */
static inline tree
maybe_find_rhs_use_for_var (def, var)
     tree def;
     tree var;
{
  varray_type uses;
  size_t i;
  
  if (SSA_DECL_P (def))
    {    
      if (names_match_p (var, def))
	return def;
      return NULL_TREE;
    }
  get_stmt_operands (tree_stmt (def));
  uses = use_ops (tree_stmt (def));
  
  if (!uses)
    return NULL_TREE;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (uses); i++)
    {
      tree *usep = VARRAY_GENERIC_PTR (uses, i);
      tree use = *usep;
      if (names_match_p (use, var))
	return use;
    }
  return NULL_TREE;
}

/* Determine if an injuring def is one which we can repair, and thus,
   ignore for purposes of determining the version of a variable.  */
static inline bool
okay_injuring_def (inj, var)
     tree inj;
     tree var;
{
  /* Acceptable injuries are those which 
     1. aren't empty statements.
     2. aren't phi nodes.
     3. contain a use of VAR on the RHS.  */
  if (!inj || inj == empty_stmt_node 
      || TREE_CODE (inj) == PHI_NODE
      || !maybe_find_rhs_use_for_var (inj, var))
    return false;
  return true;
}

/* Return true if INJ is an injuring definition */
static bool 
is_injuring_def (ei, inj)
     struct expr_info *ei;
     tree inj;
{
  /* Things that are never injuring definitions. */
  if (!inj || inj == empty_stmt_node || TREE_CODE (inj) == PHI_NODE) 
    return false;
  
  /* Things we can't handle. */
  if (TREE_CODE (TREE_OPERAND (inj, 1)) != PLUS_EXPR
      && TREE_CODE (TREE_OPERAND (inj, 1)) != MINUS_EXPR)
    return false;

  /* given inj: a1 = a2 + 5
     expr: a3 * c
     we are testing:
     if (a1 != a3
     || ! (a2 exists)
     || a2 != a3)
     return false
     
     Or, in english,  if either the assigned-to variable in
     the injury is different from the first variable in the
     expression, or the incremented variable is different from the
     first variable in the expression, punt.
     
     This makes sure we have something of the form

     a = a {+,-} {expr}
     for an expression like "a * 5".
     
     This limitation only exists because we don't know how to repair
     other forms of increments/decrements. */
  if (!names_match_p (TREE_OPERAND (inj, 0), TREE_OPERAND (ei->expr, 0))
      || !TREE_OPERAND (TREE_OPERAND (inj, 1), 0)
      || !names_match_p (TREE_OPERAND (TREE_OPERAND (inj, 1), 0),
			 TREE_OPERAND (ei->expr, 0)))
    return false;

  /* If we are strength reducing a multiply, we have the additional
     constraints that
     1. {expr} is 1
     2. {expr} and the RHS of the expression are constants. */
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

/* Find the statement defining VAR, ignoring injuries we can repair.  
   START is the first potential injuring def. */
static tree
factor_through_injuries (ei, start, var)
     struct expr_info *ei;
     tree start;
     tree var;
{
  tree end = start;

  while (is_injuring_def (ei, SSA_NAME_DEF_STMT (end)))
    {
      end = find_rhs_use_for_var (SSA_NAME_DEF_STMT (end), var);
      if (!okay_injuring_def (SSA_NAME_DEF_STMT (end), var))
	break;
      if (dump_file)
	{
	  fprintf (dump_file, "Found a real injury:");
	  print_generic_stmt (dump_file, SSA_NAME_DEF_STMT (end), 0);
	  fprintf (dump_file, "\n");
	}
      end = find_rhs_use_for_var (SSA_NAME_DEF_STMT (end), var);
    }
  return end;
}

static inline bool
ephi_has_bottom (ephi)
     tree ephi;
{
  int i;
  for (i = 0 ; i < EPHI_NUM_ARGS (ephi); i++)
    {
      tree operand = EPHI_ARG_DEF (ephi, i);
      if (EUSE_DEF (operand) == NULL_TREE)
	return true;
    }
  return false;
}

static inline bool
ephi_will_be_avail (ephi)
     tree ephi;
{
  if (EPHI_CAN_BE_AVAIL (ephi))
    if (!EPHI_LATER (ephi))
      {
	/*	int i;
	for (i = 0; i < EPHI_NUM_ARGS (ephi); i++)
	  if (EUSE_DEF (EPHI_ARG_DEF (ephi, i)) == NULL_TREE)
	  return false;*/
	return true;
      }
  return false;
}
/* Set DEF to be the new definition of REF, and update the approriate
   use arrays to reflect this. */

static void
set_expruse_def (ref, def)
     tree ref;
     tree def;
{
  if (EUSE_DEF (ref))
    {
      tree olddef = EUSE_DEF (ref);
      size_t i;
      for (i = 0; i < VARRAY_ACTIVE_SIZE (EREF_USES (olddef)); i++)
	{
	  if (VARRAY_TREE (EREF_USES (olddef), i) == ref)
	    VARRAY_TREE (EREF_USES (olddef), i) = NULL_TREE;
	}
    }
  if (def)
    {
      if (!EREF_USES (def))
	VARRAY_TREE_INIT (EREF_USES (def), 1, "Uses");      
      VARRAY_PUSH_TREE (EREF_USES (def), ref);
    }
  else
    {
      EREF_CLASS (ref) = -1;
    }
  
  EUSE_DEF (ref) = def;
}

static tree
create_expr_ref (expr, type, bb, parent)
     tree expr;
     enum tree_code type;
     basic_block bb;
     tree *parent;
{
  tree ret;
  if (type == EPHI_NODE)
    ret = create_ephi_node (bb);
  else
    ret = make_node (type);
  
  EREF_NAME (ret) = expr;
  set_bb_for_stmt (ret, bb);
  EREF_STMT (ret) = parent;
  EREF_SAVE (ret) = false;
  
  return ret;
}

     
/* dfphis and varphis, from the papers. */
static bitmap dfphis;
static bitmap varphis;


/* Function to recursively figure out where EPHI's need to be placed
   because of PHI's.
   This is because they are also partially anticipated
   expression points (because it means some expression alteration
   reaches that merge point). 
   
   We need to do this recursively, because we have to figure out
   EPHI's for the variables in the PHI as well. */

static void
set_var_phis (ei, phi)
     struct expr_info *ei;
     tree phi;
{
  /* If we've already got an EPHI set to be placed in PHI's BB, we
     don't need to do this. */
  if (!bitmap_bit_p (varphis, bb_for_stmt (phi)->index)
      && !bitmap_bit_p (dfphis, bb_for_stmt (phi)->index))
    {
      tree phi_operand;
      int curr_phi_operand;
      bitmap_set_bit (varphis, bb_for_stmt (phi)->index);
      for (curr_phi_operand = 0; 
           curr_phi_operand < PHI_NUM_ARGS (phi);
           curr_phi_operand++)
        {
          phi_operand = PHI_ARG_DEF (phi, curr_phi_operand);
	  /* For strength reduction, factor through injuries we can
	     repair. */
	  if (ei->strred_cand && TREE_CODE (phi_operand) != PHI_NODE)
	    {
	      phi_operand = factor_through_injuries (ei, phi_operand, 
						     SSA_NAME_VAR (phi_operand));
	      phi_operand = SSA_NAME_DEF_STMT (phi_operand);
	      if (dump_file)
		{
		  fprintf (dump_file, "After factoring through injuries:");
		  print_generic_stmt (dump_file, phi_operand, 0);
		  fprintf (dump_file, "\n");
		}
	    }
	  
	  /* If our phi operand is defined by a phi, we need to
	     record where the phi operands alter the expression as
	     well, and place EPHI's at each point. */
          if (TREE_CODE (phi_operand) == PHI_NODE)
            set_var_phis (ei, phi_operand);
        }
    }
}

/* EPHI insertion algorithm.  */
static void
expr_phi_insertion (dfs, ei)
     bitmap *dfs;
     struct expr_info *ei;
{
  size_t i;
  dfphis = BITMAP_XMALLOC ();
  bitmap_zero (dfphis);
  varphis = BITMAP_XMALLOC ();
  bitmap_zero (varphis);

  /*  Compute where we need to place EPHIS. There are two types of
      places we need EPHI's: Those places we would normally place a
      PHI for the occurrence (calculated by determining the IDF+ of
      the statement), and those places we need an EPHI due to partial
      anticipation.  */
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->occurs); i++)
    {
      tree *occurp = VARRAY_GENERIC_PTR (ei->occurs, i);
      tree occur = occurp ? *occurp : NULL;
      tree *killp = VARRAY_GENERIC_PTR (ei->kills, i);
      tree kill = killp ? *killp : NULL;
      tree *leftp = VARRAY_GENERIC_PTR (ei->lefts, i);
      tree left = leftp ? *leftp : NULL;
      bitmap temp;

#if ENABLE_CHECKING
      if ((kill && occur) || (left && occur) || (kill && left))
	abort();
#endif
      occurp = occur ? occurp : kill ? killp : leftp;      
      occur = occur ? occur : kill ? kill : left;
      temp = compute_idfs (dfs, occur);
      bitmap_a_or_b (dfphis, dfphis, temp);
      BITMAP_XFREE (temp);
      if (kill != NULL)
	continue;
      occur = TREE_OPERAND (occur, 1);
      /*if (is_simple_id (get_operand (occur, 0))
	  || (get_operand (occur, 1) 
	  && is_simple_id (get_operand (occur, 1))))*/
      {
	  varray_type uses;
	  size_t j;

	  get_stmt_operands (*occurp);
	  uses = use_ops (*occurp);	  
	  for (j = 0; j < VARRAY_ACTIVE_SIZE (uses); j ++)
	    {
	      tree *usep = VARRAY_GENERIC_PTR (uses, j);
	      tree use = *usep;
	      if (ei->strred_cand)
		use = factor_through_injuries (ei, use, SSA_NAME_VAR (use));
	      if (TREE_CODE (SSA_NAME_DEF_STMT (use)) != PHI_NODE)
		continue;
	      set_var_phis (ei, SSA_NAME_DEF_STMT (use));
	    }
      }
    }
  /* Union the results of the dfphis and the varphis to get the
     answer to everywhere we need EPHIS. */
  bitmap_a_or_b (dfphis, dfphis, varphis);
  
  /* Now create the EPHI's in each of these blocks. */
  EXECUTE_IF_SET_IN_BITMAP(dfphis, 0, i, 
  {
    tree ref = create_expr_ref (ei->expr, EPHI_NODE, BASIC_BLOCK (i), NULL);
    VARRAY_PUSH_TREE (ei->erefs, ref);
    EREF_PROCESSED (ref) = false;
    EREF_PROCESSED2 (ref) = false;
    EPHI_DOWNSAFE (ref) = true;
    EPHI_CAN_BE_AVAIL (ref) = true;
    EPHI_LATER (ref) = true;
    EPHI_EXTRANEOUS (ref) = true;
    EPHI_DEAD (ref) = true;
  });
  BITMAP_XFREE (dfphis);
  BITMAP_XFREE (varphis);
}

static inline tree 
ephi_at_block (bb)
     basic_block bb;
{
  bb_ann_t ann = bb_ann (bb);
  if (ann->ephi_nodes)
    return ann->ephi_nodes;
  else
    return NULL_TREE;
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
  if (ephi_at_block (block) != NULL_TREE)
    fibheap_insert (fh, preorder_count++, ephi_at_block (block));

  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->occurs); i++)
    {
      tree newref;
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
	  newref = create_expr_ref (killname, EKILL_NODE, block, killexpr);
	  VARRAY_PUSH_TREE (ei->erefs, newref);
	  fibheap_insert (fh, preorder_count++, newref);
	}
      else if (VARRAY_GENERIC_PTR (ei->lefts, i) != NULL)
	{
	  tree *leftexpr = VARRAY_GENERIC_PTR (ei->lefts, i);
	  tree leftname = ei->expr;
	  newref = create_expr_ref (leftname, ELEFT_NODE, block, leftexpr);
	  VARRAY_PUSH_TREE (ei->erefs, newref);
	  fibheap_insert (fh, preorder_count++, newref);
	}
      else
	{
	  tree *occurexpr = VARRAY_GENERIC_PTR (ei->occurs, i);
	  tree occurname;
	  occurname = ei->expr;
	  newref = create_expr_ref (occurname, EUSE_NODE, block, occurexpr);
	  VARRAY_PUSH_TREE (ei->erefs, newref);
	  
	  set_expruse_def (newref, NULL_TREE);
	  EREF_CLASS (newref) = -1;
	  EUSE_PHIOP (newref) = false;
	  EREF_PROCESSED (newref) = false;
	  EREF_PROCESSED2 (newref) = false;
	  EUSE_HAS_REAL_USE (newref) = false;	  
	  fibheap_insert (fh, preorder_count++, newref);
	}     
    }
 
  /* Insert the phi operand occurrences's in the heap at the
     successors.*/
  for (succ = block->succ; succ; succ = succ->succ_next)
    {
      if (succ->dest != EXIT_BLOCK_PTR)
        {
          if (ephi_at_block (succ->dest) != NULL)
            {
              tree newref = create_expr_ref (0, EUSE_NODE, block, NULL);
              tree ephi = ephi_at_block (succ->dest);
	      VARRAY_PUSH_TREE (ei->erefs, newref);
              set_expruse_def (newref, NULL);
	      EREF_CLASS (newref) = -1;
	      EUSE_PHIOP (newref) = true;
	      EUSE_PHI (newref) = ephi;
	      EUSE_HAS_REAL_USE (newref) = false;
	      EREF_SAVE (newref) = false;
	      EREF_RELOAD (newref) = false;
	      EUSE_INSERTED (newref) = false;
	      EREF_PROCESSED (newref) = false;
	      EREF_PROCESSED2 (newref) = false;
	      add_ephi_arg (ephi, newref, succ);
              fibheap_insert (fh, preorder_count++, newref);
            }
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
      tree newref;
      newref = create_expr_ref (ei->expr, EEXIT_NODE, EXIT_BLOCK_PTR, NULL);
      VARRAY_PUSH_TREE (ei->erefs, newref);
      fibheap_insert (fh, preorder_count++, newref);
    }
}

/* Assign a new redundancy class to the occurrence, and push it on the
   stack.  */ 
static void
assign_new_class (occ, stack, stack2)
     tree occ;
     varray_type *stack;
     varray_type *stack2;
{
  /* class(occ) <- count
     Push(occ, stack)
     count <- count + 1
  */
  EREF_CLASS (occ) = class_count;
  VARRAY_PUSH_TREE (*stack, occ);
  VARRAY_PUSH_TREE (*stack2, occ);
  class_count++;
}
     
/* Determine if the definitions of variables in Y dominate the basic
   block of X. */
static inline bool 
defs_y_dom_x (ei, yuses, x)
     struct expr_info *ei;
     const varray_type yuses;
     const tree x;
{
  size_t i;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (yuses); i++)
    {
      tree *usep = VARRAY_GENERIC_PTR (yuses, i);
      tree use;
      if (!usep) 
	continue;
      use = *usep;
      
      if (ei->strred_cand)
	use = factor_through_injuries (ei, use, SSA_NAME_VAR (use));
      
      if (a_dom_b (x, SSA_NAME_DEF_STMT (use)))
	return false;
    }
  return true;
}

static inline bool
defs_match_p (ei, t1uses, t2)
     struct expr_info *ei;
     const varray_type t1uses;
     const tree t2;
{
  size_t i;
  for (i = 0; i < VARRAY_ACTIVE_SIZE (t1uses); i++)
    {
      tree *use1p = VARRAY_GENERIC_PTR (t1uses, i);
      tree use1;
      tree use2;

      if (!use1p)
	continue;
      use1 = *use1p;
      use2 = maybe_find_rhs_use_for_var (t2, 
					 SSA_NAME_VAR (use1));
      if (!use2)
	return false;
      if (ei->strred_cand)
	use1 = factor_through_injuries (ei, use1, SSA_NAME_VAR (use1));
      
      if (SSA_NAME_DEF_STMT (use1) ==empty_stmt_node || SSA_NAME_DEF_STMT (use2) == empty_stmt_node)
	return false;
      
      if (SSA_NAME_DEF_STMT (use1) != SSA_NAME_DEF_STMT (use2))
	return false;
    }
  return true;
}

/* Determine the phi operand index for J, for PHI.  */
static inline int
opnum_of_phi (phi, j)
     tree phi;
     int j;
{
  int i;
  /* We can't just count predecessors, since tree-ssa.c generates them
     when it sees a phi in the successor during it's traversal.  So the
     order is dependent on the traversal order.  */
  for (i = 0 ; i < PHI_NUM_ARGS (phi); i++)
    if (PHI_ARG_EDGE (phi, i)->src->index == j)
      return i;
  
  abort();
}
static varray_type
phi_opnd_from_res (ei, Z, j, b)
     struct expr_info *ei;
     tree Z;
     int j;
     basic_block b;
{
  varray_type Q;
  unsigned int i;
  size_t k = 0;
  
  varray_type uses;
  VARRAY_GENERIC_PTR_INIT (Q, 1, "Temp ops");

  get_stmt_operands (*(EREF_STMT (Z)));  
  uses = use_ops (*(EREF_STMT (Z)));
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (uses); i++)
    VARRAY_PUSH_GENERIC_PTR (Q, VARRAY_GENERIC_PTR (uses, i));
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (uses); i++)
    {
      tree *vp = VARRAY_GENERIC_PTR (uses, i);
      tree v = *vp;
      
      if (ei->strred_cand)
	v = factor_through_injuries (ei, v, SSA_NAME_VAR (v));
      if (TREE_CODE (SSA_NAME_DEF_STMT (v)) == PHI_NODE)
	{
	  tree phi = SSA_NAME_DEF_STMT (v);
	  if (bb_for_stmt (phi) == b)
	    {
	      int opnum = opnum_of_phi (phi, j);
	      VARRAY_GENERIC_PTR (Q, k) = &PHI_ARG_DEF (phi, opnum);
	    }
	}
      else
	VARRAY_GENERIC_PTR (Q, k) = NULL;
      k++;
    }
  return Q;
}

static splay_tree 
rename_2 (ei, rename2_set)
     struct expr_info *ei;
     varray_type *rename2_set;
{
  unsigned int i;
  splay_tree touched_set;

  touched_set = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);
  
  while (VARRAY_ACTIVE_SIZE (*rename2_set) > 0)
    {
      tree Z;
      tree phiZ;
      int curr_phiop;
      
      Z = VARRAY_TOP_TREE (*rename2_set);
      VARRAY_POP (*rename2_set);
      
      splay_tree_insert (touched_set, (splay_tree_key) Z, 0);
      
      phiZ = EUSE_DEF (Z);
      
      for (curr_phiop = 0; 
	   curr_phiop < EPHI_NUM_ARGS (phiZ);
	   curr_phiop++)
	{
	  tree w;
	  i = EPHI_ARG_EDGE (phiZ, curr_phiop)->src->index;
	  w = EPHI_ARG_DEF (phiZ, curr_phiop);
	  if (!EREF_PROCESSED (w))
	    {
	      int j = i;
	      varray_type Y;
	      tree X;
	      
	      Y = phi_opnd_from_res (ei, Z, j, bb_for_stmt (phiZ));
	      X = EUSE_DEF (w);
	      if (!X)
		{
		  VARRAY_CLEAR (Y);
		  continue;
		}
	      if (TREE_CODE (X) == EUSE_NODE || TREE_CODE (X) == ELEFT_NODE)
		{
		  if (!defs_match_p (ei, Y, *(EREF_STMT (X))))
		    set_expruse_def (w, NULL);
		}
	      else
		{
		  if (defs_y_dom_x (ei, Y, X))
		    {
		      VARRAY_PUSH_TREE (*rename2_set, Z);
		    }
		  else
		    {
		      splay_tree_node result;
		      set_expruse_def (w, NULL);
		      result = splay_tree_lookup (idom_of_ephi, (splay_tree_key) w);
		      if (result && ((tree)result->value) == X)
			EPHI_DOWNSAFE (phiZ) = false;
		    }
		}
	      EREF_PROCESSED (w) = true;
	      VARRAY_CLEAR (Y);
	    }
	}
    }
  return touched_set;
}

static int 
occ_compare (a, b)
     const void *a;
     const void *b;
{
  tree occ1 = *(tree *) a;
  tree occ2 = *(tree *) b;
  splay_tree_node result1;
  splay_tree_node result2;
  
  if (occ1 == occ2)
    return 0;
  if (TREE_CODE (occ1) == EEXIT_NODE)
    return 1;
  if (TREE_CODE (occ2) == EEXIT_NODE)
    return -1;
  
  result1 = splay_tree_lookup (dfn, (splay_tree_key) bb_for_stmt (occ1));
  result2 = splay_tree_lookup (dfn, (splay_tree_key) bb_for_stmt (occ2));
  if (result2->value == result1->value)
    return a_dom_b (occ1, occ2) ? -1 : 1;
  else
    return (result1->value < result2->value) ? -1 : 1;
}

static void
rename_1 (ei)
     struct expr_info *ei;
{
  fibheap_t fh = fibheap_new ();
  tree *occs;
  tree y;
  varray_type stack, stack2, rename2_set;
  splay_tree touched_set;
  size_t i;
  
  VARRAY_TREE_INIT (stack, 1, "Stack for renaming");
  VARRAY_TREE_INIT (stack2, 1, "Stack for downsafety");
  VARRAY_TREE_INIT (rename2_set, 1, "Rename2 set");

  insert_occ_in_preorder_dt_order (ei, fh);
  occs = xcalloc (VARRAY_ACTIVE_SIZE (ei->erefs), sizeof (tree));
  for ( i = 0; i < VARRAY_ACTIVE_SIZE (ei->erefs); i++)
    occs[i] = VARRAY_TREE (ei->erefs, i);
  qsort (occs, VARRAY_ACTIVE_SIZE (ei->erefs), sizeof (tree),
	 occ_compare);
  
#if ENABLE_CHECKING
  {
    /* Verify that we didn't screw up the pre-order dt-order.
       Since we need something to compare to, we compute everything
       but domination between two things in the same block using
       a slightly different method.  */
    fibheap_t fhnew = fibheap_new ();
    i = 0;
    while (!fibheap_empty (fh))
      {
	y = fibheap_extract_min (fh);
	if (y != occs[i])
	  abort ();
	fibheap_insert (fhnew, i, (void *)y);
	i++;
      }
    fibheap_delete (fh);
    fh = fhnew;
  }
#endif
  free (occs);
  
  while (!fibheap_empty (fh))
    {
      y = fibheap_extract_min (fh);
      
      while (VARRAY_ACTIVE_SIZE (stack) > 0)
	{
	  tree stackref = VARRAY_TOP_TREE (stack);
	  if (!a_dom_b ( stackref, y))
	      VARRAY_POP (stack);
	  else
	    break;
	}
      
      while (VARRAY_ACTIVE_SIZE (stack2) > 0)
	{
	  tree stackref = VARRAY_TOP_TREE (stack2);
	  if (!a_dom_b (stackref, y))
	    VARRAY_POP (stack2);
	  else
	    break;
	}
      
      if (TREE_CODE (y) == EEXIT_NODE)
	{
	  if (VARRAY_ACTIVE_SIZE (stack2) > 0)
	    {
	      tree stackref = VARRAY_TOP_TREE (stack2);
	      if (TREE_CODE (stackref) == EPHI_NODE)
		EPHI_DOWNSAFE (stackref) = false;
	    }
	  continue;
	}
      if (TREE_CODE (y) == EPHI_NODE)
	{
	  assign_new_class (y, &stack, &stack2);
	}
      else if (TREE_CODE (y) == EUSE_NODE && !EUSE_PHIOP (y))
	{
	  get_stmt_operands (*(EREF_STMT(y)));
	  if (VARRAY_ACTIVE_SIZE (stack) == 0)
	    {
	      assign_new_class (y, &stack, &stack2);
	    }
	  else
	    {
	      tree x = VARRAY_TOP_TREE (stack);
	      if ((TREE_CODE (x) == EUSE_NODE && !EUSE_PHIOP (x))
		  || TREE_CODE (x) == ELEFT_NODE)
		{
		  if (defs_match_p (ei, use_ops (*(EREF_STMT (y))),
				    *(EREF_STMT (x))))
		    {
		      EREF_CLASS (y) = EREF_CLASS (x);
		      set_expruse_def (y, x);
		      VARRAY_PUSH_GENERIC_PTR (stack2, y);
		    }
		  else
		    assign_new_class (y, &stack, &stack2);
		}
	      else if (TREE_CODE (x) == EKILL_NODE)
		{
		  tree x2 = VARRAY_TOP_TREE (stack2);
		  if (TREE_CODE (x2) == EPHI_NODE)
		    EPHI_DOWNSAFE (x2) = false;
		  
		  assign_new_class (y, &stack, &stack2);
		}
	      else
		{
		  if (defs_y_dom_x (ei, use_ops (*(EREF_STMT (y))), x))
		    {
		      EREF_CLASS (y) = EREF_CLASS (x);
		      set_expruse_def (y, x);
		      VARRAY_PUSH_GENERIC_PTR (stack2, y);
		      VARRAY_PUSH_GENERIC_PTR (rename2_set, y);
		    }
		  else
		    {
		      tree x2 = VARRAY_TOP_TREE (stack2);
		      if (TREE_CODE (x2) == EPHI_NODE)
			{
			  EPHI_DOWNSAFE (x2) = false;
#if ENABLE_CHECKING
			  if (x2 != x)
			    abort ();
#endif
			}
		      
		      assign_new_class (y, &stack, &stack2);
		    }
		}
	    }
	}
      else if (TREE_CODE (y) == EUSE_NODE && EUSE_PHIOP (y))
	{
	  if (VARRAY_ACTIVE_SIZE (stack) == 0)
	    {
	      set_expruse_def (y, NULL);
	      EREF_PROCESSED (y) = true;
	    }
	  else
	    {
	      tree x = VARRAY_TOP_TREE (stack);
	      tree x2 = VARRAY_TOP_TREE (stack2);
	      
	      if (TREE_CODE (x) == EKILL_NODE)
		{
		  set_expruse_def (y, NULL);
		  EREF_PROCESSED (y) = true;
		  if (TREE_CODE (x2) == EPHI_NODE)
		    EPHI_DOWNSAFE (x2) = false;
		}
	      else 
		{
		  EREF_CLASS (y) = EREF_CLASS (x);
		  set_expruse_def (y, x);
		  splay_tree_insert (idom_of_ephi, (splay_tree_key) y, 
				     (splay_tree_value)x2);
		  if ((TREE_CODE (x2) == EUSE_NODE && !EUSE_PHIOP (x2))
		      || TREE_CODE (x2) == ELEFT_NODE)
		    {
		      EUSE_HAS_REAL_USE (y) = true;
		    }
		}	    
	    }
	}
      else if (TREE_CODE (y) == EKILL_NODE)
	{
	  VARRAY_PUSH_TREE (stack, y);
	}
      else if (TREE_CODE (y) == ELEFT_NODE)
	{
	  assign_new_class (y, &stack, &stack2);
	}
      else
        abort();
    }
  touched_set = rename_2 (ei, &rename2_set);
  fibheap_delete (fh);
  VARRAY_CLEAR (stack);
  VARRAY_CLEAR (stack2);
  VARRAY_CLEAR (rename2_set);
  splay_tree_delete (touched_set);
}

/* Reset down safety flags.  */
static void 
reset_down_safe (ephiop)
     tree ephiop;
{
  tree ephi;
  int i;
  
  if (EUSE_HAS_REAL_USE (ephiop))
    return;
  ephi = EUSE_DEF (ephiop);
  if (!ephi || TREE_CODE (ephi) != EPHI_NODE)
    return;
  if (!EPHI_DOWNSAFE (ephi))
    return;  
  EPHI_DOWNSAFE (ephi) = false;
  for (i = 0; i < EPHI_NUM_ARGS (ephi); i++)
    reset_down_safe (EPHI_ARG_DEF (ephi, i));
}

/* Compute down_safety.  */
static void
down_safety (ei)
     struct expr_info *ei;
{
  size_t i;
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->euses_dt_order); i++)
    {
      int j;
      tree ephi = VARRAY_TREE (ei->euses_dt_order, i);
      if (TREE_CODE (ephi) != EPHI_NODE)
	continue;

      if (!EPHI_DOWNSAFE (ephi))
	for (j = 0; j < EPHI_NUM_ARGS (ephi); j++)
	  reset_down_safe (EPHI_ARG_DEF (ephi, j));
    }      
} 
static inline tree
ephi_operand_for_pred (ephi, e)
     tree ephi;
     edge e;
{
  int i;
  for (i = 0; i < EPHI_NUM_ARGS (ephi); i++)
    if (EPHI_ARG_EDGE (ephi, i) == e)
      return EPHI_ARG_DEF (ephi, i);
  abort ();
}
/* This is based on Open64's require_edge_placement.
   The idea is that EPHI's with NULL ephi operands in blocks with multiple
   successors would require edge placement in order to establish
   availability.  We can't do edge placement right now.  */
static bool
requires_edge_placement (ephi)
     tree ephi ATTRIBUTE_UNUSED;
{
#if 0
  int i;
  for (i = 0; i < EPHI_NUM_ARGS (ephi); i++)
    {
      tree operand = EPHI_ARG_DEF (ephi, i);
      edge e;
      basic_block operand_bb = bb_for_stmt (operand);
      
      if (!EUSE_DEF (operand))
	{
	  for (e = operand_bb->pred; e; e = e->pred_next)
	    if (EDGE_CRITICAL_P (e))
	      return true;
	  for (e = operand_bb->succ; e; e = e->succ_next)
	    if (EDGE_CRITICAL_P (e))
	      return true;

	}
    }
#endif
  return false;
}

/* Compute can_be_avail.  */
static void
compute_can_be_avail (ei)
     struct expr_info *ei;
{
  size_t i;
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->euses_dt_order); i++)
    {
      tree ephi = VARRAY_TREE (ei->euses_dt_order, i);
      if (TREE_CODE (ephi) != EPHI_NODE)
	continue;
      EPHI_CAN_BE_AVAIL (ephi) = true;
    }
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->euses_dt_order); i++)
    {
      tree ephi = VARRAY_TREE (ei->euses_dt_order, i);
      if (TREE_CODE (ephi) != EPHI_NODE)
	continue;
      if (!EPHI_DOWNSAFE (ephi) && EPHI_CAN_BE_AVAIL (ephi) 
	  && ephi_has_bottom (ephi))
	reset_can_be_avail (ei, ephi);
#if 1
      /* There are some EPHI's which might require edge placement in
	 order to do insertion. We'll set these EPHIs to not be
	 available.  */
      if (requires_edge_placement (ephi))
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "Marking ephi in block %d as not available due to edge placement requirement\n", bb_for_stmt (ephi)->index);
	  reset_can_be_avail (ei, ephi);
	}
#endif 
    }
}

/* Reset can_be_avail flags.  */
static void
reset_can_be_avail (ei, ephi)
     struct expr_info *ei;   
     tree ephi;
{
  varray_type uses;
  size_t i;

  EPHI_CAN_BE_AVAIL (ephi) = false;
  uses = EREF_USES (ephi);
  
  if (!uses)
    return;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (uses); i++)
    {
      tree w = VARRAY_TREE (uses, i);
      if (!w)
	continue;
      if (TREE_CODE (w) == EUSE_NODE && EUSE_PHIOP (w))
	{
	  tree f = EUSE_PHI (w);
	  if (!EPHI_DOWNSAFE (f) && EPHI_CAN_BE_AVAIL (f))
	    reset_can_be_avail (ei, f);
	}
    }
}


/*  Reset later flags.  */
static void 
reset_later (ei, ephi)
     struct expr_info *ei;
     tree ephi;
{
  varray_type uses;
  size_t i;
  
  EPHI_LATER (ephi) = false;
  uses = EREF_USES (ephi);
  
  if (!uses)
    return;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (uses); i++)
    {
      tree w = VARRAY_TREE (uses, i);
      if (!w)
	continue;
#if ENABLE_CHECKING
      if (EUSE_DEF (w) != ephi)
	abort ();
#endif
      if (TREE_CODE (w) == EUSE_NODE && EUSE_PHIOP (w))
	{
	  tree f = EUSE_PHI (w);
	  if (EPHI_LATER (f))
	    reset_later (ei, f);
	}
    }
}


/*  Compute later flags.  */
static void
compute_later (ei)
     struct expr_info *ei;
{
  size_t i;
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->euses_dt_order); i++)
    {
      tree ephi = VARRAY_TREE (ei->euses_dt_order, i);
      if (TREE_CODE (ephi) != EPHI_NODE)
	continue;
      EPHI_LATER (ephi) = EPHI_CAN_BE_AVAIL (ephi);
    }
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->euses_dt_order); i++)
    {
      int j;
      tree ephi = VARRAY_TREE (ei->euses_dt_order, i);
      bool exists = false;
      if (TREE_CODE (ephi) != EPHI_NODE)
	continue;      
      if (!EPHI_LATER (ephi))
        continue;
      for (j = 0; j < EPHI_NUM_ARGS (ephi); j++)
	{
	  tree operand = EPHI_ARG_DEF (ephi, j);
	  if (EUSE_DEF (operand) != NULL_TREE 
	      && EUSE_HAS_REAL_USE (operand))		
	    exists = true;
	}
      if (exists)
	reset_later (ei, ephi); 
    }
}

/* Compute will_be_avail.  */
static void
will_be_avail (ei)
     struct expr_info *ei;
{
  compute_can_be_avail (ei);
  compute_later (ei);
}

/* Insert the expressions in preorder DT order in the ref_list.  */
static void
insert_euse_in_preorder_dt_order_1 (ei, block)
     struct expr_info *ei;
     basic_block block;
{
  size_t i;
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->erefs); i++)
    {
      tree ref = VARRAY_TREE (ei->erefs, i);
      if (!ref)
	continue;
      if (bb_for_stmt (ref) != block)
	continue;
      if (TREE_CODE (ref) == EUSE_NODE 
	  || TREE_CODE (ref) == EPHI_NODE
	  || TREE_CODE (ref) == ELEFT_NODE)
	VARRAY_PUSH_TREE (ei->euses_dt_order, ref);
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
  VARRAY_CLEAR (ei->euses_dt_order);
  insert_euse_in_preorder_dt_order_1 (ei, ENTRY_BLOCK_PTR->next_bb);
}

/* Determine if we can insert the ephi.  */
static bool
can_insert (op)
     tree op;
{
  tree def;
  
  if (EUSE_DEF (op) == NULL)
    return true;
  
  def = EUSE_DEF (op);
  if (!def)
    return true;
  
  if (!EUSE_HAS_REAL_USE (op))
    if (TREE_CODE (def) == EPHI_NODE && !(ephi_will_be_avail (def)))
      return true;

  return false;
}
/* Hunt down the right reaching def for VAR, starting with BB. */
static tree
reaching_def (var, currstmt, bb, ignore)
     tree var;
     tree currstmt;
     basic_block bb;
     tree ignore;
{
  tree curruse = NULL_TREE;
  block_stmt_iterator bsi;
  basic_block dom;
  tree phi;

  /* Check phis first. */
  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
    {
      if (phi == currstmt)
	break;
      if (phi == ignore)
	continue;
      if (names_match_p (var, PHI_RESULT (phi)))
	curruse = PHI_RESULT (phi);
    }
  
  /* We can't walk BB's backwards right now, so we have to walk *all*
     the statements, and choose the last name we find. */  
  bsi = bsi_start (bb);
  for (; !bsi_end_p (bsi); bsi_next (&bsi))
    {
      varray_type uses;
      tree *def;
      size_t i;

      if (bsi_stmt (bsi) == currstmt)
	break;
      
      get_stmt_operands (bsi_stmt (bsi));      
      uses = use_ops (bsi_stmt (bsi));
      if (uses)
	for (i = 0; i < VARRAY_ACTIVE_SIZE (uses); i++)
	  {
	    tree *usep = VARRAY_GENERIC_PTR (uses, i);
	    tree use = *usep;
	    
	    if (use == ignore)
	      continue;
	    if (names_match_p (var, use))
	      curruse = use;
	  }
       
      def = def_op (bsi_stmt (bsi));
      if (def && *def != ignore && names_match_p (var, *def))
	curruse = *def;
      
    }
  if (curruse != NULL_TREE)
    return curruse;
  dom = get_immediate_dominator (pre_idom, bb);
  if (!dom)
    return curruse;
  return reaching_def (var, currstmt, dom, ignore);
}

static void
update_old_new (ei, old, new)
     struct expr_info *ei;
     tree *old;
     tree *new;
{
  splay_tree_node result;
  size_t i;

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
  splay_tree_insert (old_new_map, (splay_tree_key) old,
		     (splay_tree_value) new);
  /* Arghghghgh. We have to update the EREFS to be able to repair
     injuries that occur without problems right now.  */
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->erefs); i++)
    {
      if (!VARRAY_TREE (ei->erefs, i))
	continue;
      if (EREF_STMT (VARRAY_TREE (ei->erefs, i)) == old)
	EREF_STMT (VARRAY_TREE (ei->erefs, i)) = new;
    }
}      
static bool
finalize_1 (ei, temp)
     struct expr_info *ei;
     tree temp;
{
  tree x;
  int nx;
  bool made_a_reload = false;
  size_t i;

  avdefs = xcalloc (class_count + 1, sizeof (tree));
  
  insert_euse_in_preorder_dt_order (ei);
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->euses_dt_order); i++)
    {
      x = VARRAY_TREE (ei->euses_dt_order, i);
      nx = EREF_CLASS (x);
      
      if (TREE_CODE (x) == EPHI_NODE)
	{
	  if (ephi_will_be_avail (x))
	    avdefs[nx] = x;
	}
      else if (TREE_CODE (x) == ELEFT_NODE)
	{
	  avdefs[nx] = x;
	}
      else if (TREE_CODE (x) == EUSE_NODE && !EUSE_PHIOP (x))
	{
	  if (avdefs[nx] == NULL
	      || !a_dom_b (avdefs[nx],x))
	    {
	      EREF_RELOAD (x) = false;
	      avdefs[nx] = x;
	      EUSE_DEF (x) = NULL;
	    }
	  else
	    {
	      EREF_RELOAD (x) = true;
	      made_a_reload = true;
	      set_expruse_def (x, avdefs[nx]);
#if ENABLE_CHECKING
	      if (EREF_CLASS (x) != EREF_CLASS (avdefs[nx]))
		abort ();
#endif
	    }
	}
      else
	{
	  tree ephi = EUSE_PHI (x);
#if ENABLE_CHECKING	  
	  if (!ephi)
	    abort ();
#endif
	  if (ephi_will_be_avail (ephi))
	    {
	      if (can_insert (x))
		{
		  tree expr;
		  tree copy;
		  tree newtemp;
		  tree endtree;
		  tree *endtreep;
		  basic_block bb = bb_for_stmt (x);
		  
		  /* Insert definition of expr at end of BB containing x. */
	
		  copy = ei->expr;
		  walk_tree (&copy, copy_tree_r, NULL, NULL);
		  expr = build (MODIFY_EXPR, TREE_TYPE  (ei->expr),
				temp, copy);
		  newtemp = make_ssa_name (temp, expr);
		  TREE_OPERAND (expr, 0) = newtemp;
		  TREE_OPERAND (copy, 0) =
		    reaching_def (TREE_OPERAND (copy, 0),
				  NULL_TREE, bb_for_stmt (x), NULL_TREE);
		  
		  if (TREE_OPERAND (copy, 1) 
		      && SSA_DECL_P (TREE_OPERAND (copy, 1)))
		    TREE_OPERAND (copy, 1) = 
		      reaching_def (TREE_OPERAND (copy, 1),
				    NULL_TREE, bb_for_stmt (x), NULL_TREE);
		  if (dump_file)
		    {
		      fprintf (dump_file, "In BB %d, insert save of ",
			       bb_for_stmt (x)->index);
		      print_generic_expr (dump_file, expr, 0);
		      fprintf (dump_file, " to ");
		      print_generic_expr (dump_file, newtemp, 0);
		      fprintf (dump_file, " after ");
		      print_generic_stmt (dump_file, 
					  last_stmt (bb_for_stmt (x)),
			                  dump_flags);
		      fprintf (dump_file, 
			       " (at end of BB), because of EPHI");
		      fprintf (dump_file, " in BB %d\n", 
			       bb_for_stmt (ephi)->index);
		    }
		  endtree = last_stmt (bb);
		  endtreep = last_stmt_ptr (bb);
		  set_bb_for_stmt (expr, bb);

		  if (is_ctrl_altering_stmt (endtree) || is_ctrl_stmt (endtree))
		    bb->end_tree_p = do_proper_save (ei, endtree, 
						     expr, endtree);
		  else
		    bb->end_tree_p  = do_proper_save (ei, endtree, 
						      endtree, expr);
		  set_expruse_def (x, create_expr_ref (ei->expr, EUSE_NODE,
						       bb, 0));
		  VARRAY_PUSH_TREE (ei->erefs, EUSE_DEF (x));
		  EREF_RELOAD (EUSE_DEF (x)) = false;
		  EREF_SAVE (EUSE_DEF (x)) = false;
		  EUSE_INSERTED (EUSE_DEF (x)) = true;
		  EUSE_PHIOP (EUSE_DEF (x)) = false;
		  EUSE_HAS_REAL_USE (x) = true; 
		  EREF_PROCESSED (x) = false;
		  EREF_PROCESSED2 (x) = false;
		  EREF_SAVE (x) = false;
		  EREF_RELOAD (x) = false;
		  pre_stats.saves++;
		}
	      else
		{
		  set_expruse_def (x, avdefs[nx]);
		}
	    }
	}
    }
  return made_a_reload;
}
/* Determine if operand OPNUM of EPHI is an injured operand.
   Used during EPHI minimization to make sure we keep EPHI's 
   that we need to do injury repair on.  */
static bool
injured_ephi_operand (ei, ephi, opnum)
     struct expr_info *ei;
     tree ephi;
     int opnum;
{
  int i;
  tree operand = EPHI_ARG_DEF (ephi, opnum);

  if (!EUSE_DEF (operand) || TREE_CODE (EUSE_DEF (operand)) == EPHI_NODE)
    return false;
  
  for (i = 0; i < 2; i++)
    {
      int j;
      tree phi;
      if (!SSA_DECL_P (TREE_OPERAND (ei->expr, i)))
	continue;
      for (phi = phi_nodes (bb_for_stmt (ephi)); phi; phi = TREE_CHAIN (phi))
	{
	  tree phires = PHI_RESULT (phi);
	  tree eop = TREE_OPERAND (ei->expr, i);
	  if (SSA_NAME_VAR (phires) == SSA_NAME_VAR (eop))
	    break;
	}
      if (!phi)
	continue;
      for (j = 0; j < PHI_NUM_ARGS (phi); j++)
	{
	  if (PHI_ARG_EDGE (phi, j) == EPHI_ARG_EDGE (ephi, opnum))
	    if (is_injuring_def (ei, SSA_NAME_DEF_STMT (PHI_ARG_DEF (phi, j))))
	      return true;
	}
    }
  return false;  
}
static void
set_save (ei, X)
     struct expr_info *ei;
     tree X;
{
  if ((TREE_CODE (X) == EUSE_NODE && !EUSE_PHIOP (X))
      || TREE_CODE (X) == ELEFT_NODE)
    EREF_SAVE (X) = true;
  else if (TREE_CODE (X) == EPHI_NODE)
    {
      int curr_phiop;
      for (curr_phiop = 0; curr_phiop < EPHI_NUM_ARGS (X); curr_phiop++)
	{
	  tree w = EPHI_ARG_DEF (X, curr_phiop);
	  if (!EREF_PROCESSED2 (w))
	    {
	      EREF_PROCESSED2 (w) = true;    
	      set_save (ei, EUSE_DEF (w));
	    }
	  /* While we can't be sure exactly where the injury
	     replacements will occur until code_motion, it suffices
	     to not remove EPHI's who are in the DF+ of an injured
	     EPHI operand.  */
	  if (ei->strred_cand && injured_ephi_operand (ei, X, curr_phiop))
	    {
	      bitmap idfs = compute_idfs (pre_dfs, w);
	      int i;
	      EXECUTE_IF_SET_IN_BITMAP (idfs, 0, i,
              {
		tree ephi = ephi_at_block (BASIC_BLOCK (i));
		if (ephi != NULL && ephi_will_be_avail (ephi))
		  EPHI_EXTRANEOUS (ephi) = false;
		
	      });	  
	      BITMAP_XFREE (idfs);      
	    }
	}  
    }
  

  if ((TREE_CODE (X) == EUSE_NODE && !EUSE_PHIOP (X))
      || TREE_CODE (X) == ELEFT_NODE)
    {
      bitmap idfs = compute_idfs (pre_dfs, X);
      int i;
      EXECUTE_IF_SET_IN_BITMAP (idfs, 0, i,
      {
	tree ephi = ephi_at_block (BASIC_BLOCK (i));
	if (ephi != NULL && ephi_will_be_avail (ephi))
	  EPHI_EXTRANEOUS (ephi) = false;

      });	  
      BITMAP_XFREE (idfs);
    }
}
static void
remove_ephi (ei, ephi)
     struct expr_info *ei;
     tree ephi;   
{
  size_t i;
  int j;
  
  if (dump_file)
    fprintf (dump_file, "Removing ephi in block %d\n", 
	     bb_for_stmt (ephi)->index);
  bb_ann (bb_for_stmt (ephi))->ephi_nodes = NULL_TREE;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->erefs); i++)
    if (VARRAY_TREE (ei->erefs, i) == ephi)
      {
	VARRAY_TREE (ei->erefs, i) = NULL_TREE;
	break;
      }
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->euses_dt_order); i++)
    if (VARRAY_TREE (ei->euses_dt_order, i) == ephi)
      {
	VARRAY_TREE (ei->euses_dt_order, i) = NULL_TREE;
	break;
      }
  
  for (j = 0; j < EPHI_NUM_ARGS (ephi); j++)
    {
      tree w = EPHI_ARG_DEF (ephi, j);
      
      for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->erefs); i++)
	if (VARRAY_TREE (ei->erefs, i) == w)
	  {
	    VARRAY_TREE (ei->erefs, i) = NULL_TREE;
	    break;
	  }
      
      for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->euses_dt_order); i++)
	if (VARRAY_TREE (ei->euses_dt_order, i) == w)
	  {
	    VARRAY_TREE (ei->euses_dt_order, i) = NULL_TREE;
	    break;
	  }
      
      if (EUSE_DEF (w) != NULL)
	{
	  tree def = EUSE_DEF (w);
	  for (i = 0; i < VARRAY_ACTIVE_SIZE (EREF_USES (def)); i++)
	    {
	      if (VARRAY_TREE (EREF_USES (def), i) == w)
		{
		  VARRAY_TREE (EREF_USES (def), i) = NULL_TREE;
		  break;
		}
	    }
	}
    }
}

/* Set replacement for EPHI minimization.  */
static void
set_replacement (ei, g, replacing_def)
     struct expr_info *ei;
     tree g;
     tree replacing_def;
{
  size_t i;
  
  if (EREF_USES (g))
    {
      for (i = 0; i < VARRAY_ACTIVE_SIZE (EREF_USES (g)); i++)
	{
	  tree x = VARRAY_TREE (EREF_USES (g), i);
	  if (!x)
	    continue;
	  if (TREE_CODE (x) == EUSE_NODE && EUSE_PHIOP (x))
	    {
	      tree f = EUSE_PHI (x);
	      if (EPHI_EXTRANEOUS (f) && !EREF_PROCESSED (f))
		{
		  EREF_PROCESSED (f) = true;
		  set_replacement (ei, f, replacing_def);
		}
	      else if (!EPHI_EXTRANEOUS (f))
		{
		  EREF_CLASS (x) = EREF_CLASS (replacing_def);
		  set_expruse_def (x, replacing_def);    
		}
	    }
	}
      
      for (i = 0; i < VARRAY_ACTIVE_SIZE (EREF_USES (g)); i++)
	{
	  tree x = VARRAY_TREE (EREF_USES (g), i);
	  if (!x)
	    continue;
	  if (TREE_CODE (x) == EUSE_NODE && !EUSE_PHIOP (x)
	      && EREF_RELOAD (x))
	    {
	      EREF_CLASS (x) = EREF_CLASS (replacing_def);
	      set_expruse_def (x, replacing_def);
	    }
	}
    }
  remove_ephi (ei, g);
}

static void 
finalize_2 (ei)
     struct expr_info *ei;
{
  size_t i;

  insert_euse_in_preorder_dt_order (ei);
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->euses_dt_order); i++)
    {
      tree ref = VARRAY_TREE (ei->euses_dt_order, i);
      if (TREE_CODE (ref) == EPHI_NODE)
	{
	  if (ephi_will_be_avail (ref))
	    EPHI_EXTRANEOUS (ref) = true;
	}
    }
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->euses_dt_order); i++)
    {
      tree ref = VARRAY_TREE (ei->euses_dt_order, i);
      if (TREE_CODE (ref) == EUSE_NODE
	  && !EUSE_PHIOP (ref)
	  && EREF_RELOAD (ref))
	{
	    set_save (ei, EUSE_DEF (ref));
	}
    }

  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->euses_dt_order); i++)
    {
      tree ephi = VARRAY_TREE (ei->euses_dt_order, i);
      if (!ephi || TREE_CODE (ephi) != EPHI_NODE)
	continue;
      if (ephi_will_be_avail (ephi))
	{
	  if (EPHI_EXTRANEOUS (ephi))
	    {      
	      int k;
	      for (k = 0; k < EPHI_NUM_ARGS (ephi); k++)
		{
		  tree w = EPHI_ARG_DEF (ephi, k);
		  tree defw;
		  if (!w || !EUSE_DEF (w))
		    continue;
		  defw = EUSE_DEF (w);
		  if ((TREE_CODE (defw) == EPHI_NODE && !EPHI_EXTRANEOUS (defw))
		      || (TREE_CODE (defw) == EUSE_NODE && !EUSE_PHIOP (defw))
		      || TREE_CODE (defw) == ELEFT_NODE)
		    set_replacement (ei, ephi, EUSE_DEF (w));
		}
	    }
	}
      else
	{
	  int curr_phiop;
	  bool cant_remove = false;
	  for (curr_phiop = 0; curr_phiop < EPHI_NUM_ARGS (ephi); curr_phiop++)
	    if (ei->strred_cand && injured_ephi_operand (ei, ephi, curr_phiop))
	      {
		cant_remove = true;
		break;
	      }
	  if (!cant_remove)
	    remove_ephi (ei, ephi);  
	}
    }
}


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
  if (TREE_CODE (ei->expr) == MULT_EXPR)
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
static void
repair_ephi_injury (ei, ephi, temp)
     struct expr_info *ei;
     tree ephi;
     tree temp;
{
  tree t;
  for (t = phi_nodes (bb_for_stmt (ephi)); t; t = TREE_CHAIN (t))
    {
      repair_phi_injury (ei, t, temp);
    }
}
static void
repair_phi_injury (ei, phi, temp)
     struct expr_info *ei;
     tree phi;
     tree temp;
{
  int curr_phi_oper;
  if (htab_find (ei->repaired, phi) != NULL)
    return;
  *(htab_find_slot (ei->repaired, phi, INSERT)) = phi;
  
  for (curr_phi_oper = 0; 
       curr_phi_oper < PHI_NUM_ARGS (phi);
       curr_phi_oper++)
    {
      repair_use_injury (ei, PHI_ARG_DEF (phi, curr_phi_oper), temp);
    }
}

static void
repair_use_injury (ei, use, temp)
     struct expr_info *ei;
     tree use;
     tree temp;
     
{
  tree stmt;
  tree var;
  varray_type toprocess;
  VARRAY_TREE_INIT (toprocess, 1, "");
  if (htab_find (ei->repaired, use) != NULL)
    return;
  *(htab_find_slot (ei->repaired, use, INSERT)) = use;

  var = use;
  stmt = SSA_NAME_DEF_STMT (use);
  while (is_injuring_def (ei, stmt))
    {
      VARRAY_PUSH_TREE (toprocess, stmt);
      var = find_rhs_use_for_var (stmt, var);
      if (!okay_injuring_def (SSA_NAME_DEF_STMT (var), var))
	break;
      stmt = SSA_NAME_DEF_STMT (var);
    }	           
  while (VARRAY_ACTIVE_SIZE (toprocess) > 0)
    {
      tree incr;
      tree expr;
      tree newtemp;
      tree injury = VARRAY_TOP_TREE (toprocess);
      VARRAY_POP (toprocess);
      
      if (htab_find (ei->repaired, injury) != NULL)
	continue;
      
      *(htab_find_slot (ei->repaired, injury, INSERT)) = injury;

      if (dump_file)
	{
	  fprintf (dump_file, "Injury repaired:");
	  print_generic_stmt (dump_file, injury, 0);
	  fprintf (dump_file, "\n");
	}
      incr = calculate_increment (ei, injury);
      expr = build (PLUS_EXPR, TREE_TYPE (temp), temp, incr);
      TREE_OPERAND (expr, 0) = reaching_def (temp, injury, 
					     bb_for_stmt (injury), NULL_TREE);
      expr = build (MODIFY_EXPR, TREE_TYPE (temp), temp, expr);
      newtemp = make_ssa_name (temp, expr);
      modify_stmt (expr);
      TREE_OPERAND (expr, 0) = newtemp;
      set_bb_for_stmt (expr, bb_for_stmt (injury));
      do_proper_save (ei, injury, injury, expr);
    }
}

/* Repair the injury for USE. */
static void
repair_euse_injury (ei, euse, temp)
     struct expr_info *ei; 
     tree euse; 
     tree temp;
{

  int i;

  if (htab_find (ei->repaired, euse) != NULL)
      return;

  *(htab_find_slot (ei->repaired, euse, INSERT)) = euse;
  
  for (i = 0; i < 2; i++)
    {
      tree var;      
      if (!SSA_DECL_P (TREE_OPERAND (ei->expr, i)))
	continue;
      var = find_rhs_use_for_var (*(EREF_STMT (euse)), 
				  TREE_OPERAND (ei->expr, i));
      repair_use_injury (ei, var, temp);
    }
}
/* Perform a replacement, replacing where USE currently is with
   FIRSTEXPR followed by SECONDEXPR.
   
   Returns the new pointer to SECONDEXPR.

   This is its own function because there are two distinct cases
   that need to be handled, since we are really performing insertion
   using replacement.  
   The constraint on replacing is that we can't replace the first
   argument of a COMPOUND_EXPR with another COMPOUND_EXPR, but we
   *can* replace the second argument of a COMPOUND_EXPR with a
   COMPOUND_EXPR.
   
   If we ever get a bsi_insert, we can just use that instead of this
   function.  */
static tree *
do_proper_save (ei, use, firstexpr, secondexpr)
     struct expr_info *ei;
     tree use;
     tree firstexpr;
     tree secondexpr;
{
  basic_block bb = bb_for_stmt (use);
  block_stmt_iterator bsi;
  
  if (*bb->head_tree_p == use && *bb->end_tree_p == use)
    {
      tree stmt = build (COMPOUND_EXPR, void_type_node,
			 firstexpr, secondexpr);
      tree *useplace;
      set_bb_for_stmt (stmt, bb);
      if (use == firstexpr)
	useplace = &TREE_OPERAND (stmt, 0);
      else if (use == secondexpr)
	useplace = &TREE_OPERAND (stmt, 1);
      else
	abort ();
      
      update_old_new (ei, bb->head_tree_p, useplace);
      *bb->head_tree_p = stmt;
      bb->end_tree_p = &TREE_OPERAND (stmt, 1);
      return &TREE_OPERAND (stmt, 1);
    }
  
  bsi = bsi_start (bb);
  for (; !bsi_end_p (bsi); bsi_next (&bsi))
    {
      tree *containerp = bsi_container (bsi);
      if (containerp && TREE_CODE (*containerp) == COMPOUND_EXPR)
	{
	  tree *co0p = &TREE_OPERAND (*containerp, 0);
	  tree *co1p = &TREE_OPERAND (*containerp, 1);
	  if (*co0p == use)
	    {
	      /* This is the fun corner case. */
	      *co0p = firstexpr;
	      *co1p = build (COMPOUND_EXPR, void_type_node,
			     secondexpr, *co1p);

	      if (use == secondexpr)
		update_old_new (ei, co0p, &TREE_OPERAND (*co1p, 0));
	      update_old_new (ei, co1p, &TREE_OPERAND (*co1p, 1));
	      set_bb_for_stmt (*co1p, bb);
	      /* Update basic block boundary, if necessary. */
	      if (bb->end_tree_p == co1p)
		bb->end_tree_p = &TREE_OPERAND (*co1p, 1);
	      else if (bb->end_tree_p == containerp)
		bb->end_tree_p = co1p;
	      
	      return &TREE_OPERAND (*co1p, 0);
	    }
	  else if (*co1p == use)
	    {
	      *co1p = build (COMPOUND_EXPR, void_type_node,
			     firstexpr, secondexpr);
	      if (firstexpr == use)
		update_old_new (ei, co1p, &TREE_OPERAND (*co1p, 0));
	      else
		update_old_new (ei, co1p, &TREE_OPERAND (*co1p, 1));
	      set_bb_for_stmt (*co1p, bb);
	      if (bb->end_tree_p == co1p)
		bb->end_tree_p = &TREE_OPERAND (*co1p, 1);
	      else if (bb->end_tree_p == containerp)
		bb->end_tree_p = &TREE_OPERAND (*co1p, 1);
	      
	      return &TREE_OPERAND (*co1p, 1);
	    }
	}
    }
  abort ();
}

static void
code_motion (ei, temp)
     struct expr_info *ei;
     tree temp;
{
  tree use;
  tree newtemp;
  size_t euse_iter;
  

  insert_euse_in_preorder_dt_order (ei);

  for (euse_iter = 0; 
       euse_iter < VARRAY_ACTIVE_SIZE (ei->euses_dt_order);
       euse_iter++)
    {
      use = VARRAY_TREE (ei->euses_dt_order, euse_iter);
#if ENABLE_CHECKING
      if (TREE_CODE (use) == EUSE_NODE && EUSE_PHIOP (use)
	  && (EREF_RELOAD (use) || EREF_SAVE (use)))
	abort ();
#endif
      if (EREF_SAVE (use) && !EUSE_INSERTED (use))
	{
	  tree newexpr;
	  tree *use_stmt_p;
	  tree copy;
	  
	  if (ei->strred_cand)
	    repair_euse_injury (ei, use, temp);

	  use_stmt_p = EREF_STMT (use);
	  
	  copy = TREE_OPERAND (*use_stmt_p, 1);
	  walk_tree (&copy, copy_tree_r, NULL, NULL);
	  newexpr = build (MODIFY_EXPR, TREE_TYPE (temp), temp, copy);
	  newtemp = make_ssa_name (temp, newexpr);
	  
	  TREE_OPERAND (newexpr, 0) = newtemp;
	  TREE_OPERAND (*use_stmt_p, 1) = newtemp;

	  if (dump_file)
	    {
	      fprintf (dump_file, "In BB %d, insert save of ", 
		       bb_for_stmt (use)->index);
	      print_generic_expr (dump_file, copy, 0);
	      fprintf (dump_file, " to ");
	      print_generic_expr (dump_file, newtemp, 0);
	      fprintf (dump_file, " before statement ");
	      print_generic_expr (dump_file, *use_stmt_p, 0);
	      fprintf (dump_file, "\n");
	      if (TREE_LOCUS (*use_stmt_p))
		fprintf (dump_file, " on line %d\n", 
			 TREE_LINENO (*use_stmt_p));
	    }	  
	  modify_stmt (newexpr);
	  modify_stmt (*use_stmt_p);
	  set_bb_for_stmt (newexpr, bb_for_stmt (use));
	  
	  EREF_STMT (use) = do_proper_save (ei, *use_stmt_p, newexpr, 
					    *use_stmt_p);
	  pre_stats.saves++;
	  
	  
	} 
      else if (EREF_RELOAD (use))
	{
	  tree *use_stmt_p;
	  basic_block bb;
	  tree newtemp;

	  if (ei->strred_cand)
	    repair_euse_injury (ei, use, temp);

	  use_stmt_p = EREF_STMT (use);
	  bb = bb_for_stmt (*use_stmt_p);
	  newtemp = reaching_def (temp, *use_stmt_p, bb, NULL_TREE);
	  if (dump_file)
	    {
	      fprintf (dump_file, "In BB %d, insert reload of ", 
		       bb->index);
	      print_generic_expr (dump_file, TREE_OPERAND (*use_stmt_p, 1), 0);
	      fprintf (dump_file, " from ");
	      print_generic_expr (dump_file, newtemp, 0);
	      fprintf (dump_file, " in statement ");
	      print_generic_stmt (dump_file, *use_stmt_p, 0);
	      fprintf (dump_file, "\n");
	      if (TREE_LOCUS (*use_stmt_p))
		fprintf (dump_file, " on line %d\n", 
			 TREE_LINENO (*use_stmt_p));
	    }	
	  TREE_OPERAND (*use_stmt_p, 1) = newtemp;
	  modify_stmt (*use_stmt_p);
	  pre_stats.reloads++;
	}
      else if (TREE_CODE (use) == EPHI_NODE)
	{
	  int i;
	  if (dump_file)
	    {
	      fprintf (dump_file, "In BB %d, insert PHI to replace EPHI\n", 
		       bb_for_stmt (use)->index);     
	    }
	  newtemp = create_phi_node (temp, bb_for_stmt (use));
	  if (ei->strred_cand)
	    repair_ephi_injury (ei, use, temp);
	  for (i = 0; i < EPHI_NUM_ARGS (use); i++)
	    {
	      tree rdef;
	      rdef = reaching_def (temp, NULL_TREE, 
			      	   EPHI_ARG_EDGE (use, i)->src, newtemp);
	      if (rdef)
  	        add_phi_arg (newtemp, rdef, EPHI_ARG_EDGE (use, i));
	    }
	  pre_stats.newphis++;
	  
	}
      
    }
  free (avdefs);
}

/* Returns true if a dominates b */
static inline bool 
a_dom_b (a, b)
     tree a;
     tree b;
{
  bool ret = false;
  
#if ENABLE_CHECKING
  if (a == b)
    abort ();
#endif
  if (bb_for_stmt (a) != bb_for_stmt (b))
    ret = dominated_by_p (pre_idom, bb_for_stmt (b), bb_for_stmt (a));
  else
    {
      if ((TREE_CODE (a) == EUSE_NODE && EUSE_PHIOP (a))
	  && (TREE_CODE (b) == EUSE_NODE && EUSE_PHIOP (b)))
	{
	  ret = false;
	}
      else if (TREE_CODE (a) == EUSE_NODE && EUSE_PHIOP (a))
	{
	  ret = false;
	}
      else if (TREE_CODE (b) == EUSE_NODE && EUSE_PHIOP (b))
	{
	  ret = true;
	}
      else if (TREE_CODE (a) == PHI_NODE && TREE_CODE (b) == PHI_NODE)
	{
	  ret = true;
	}
      else if (TREE_CODE (a) == PHI_NODE)
	{
	  ret = true;
	}
      else if (TREE_CODE (b) == PHI_NODE)
	{
	  ret = false;
	}
      else if (TREE_CODE (a) == EPHI_NODE
	       && TREE_CODE (b) == EPHI_NODE)
	{
	  abort ();
	}
      else if (TREE_CODE (a) == EPHI_NODE)
	{
	  ret = true;
	}
      else if (TREE_CODE (b) == EPHI_NODE)
	{
	  ret = false;
	}
      else
	{
	  block_stmt_iterator bsi;
	  tree astmt = a;
	  tree bstmt = b;

	  bsi = bsi_start (bb_for_stmt (a));
	  if (TREE_CODE (a) == EUSE_NODE)
	    astmt = *(EREF_STMT (a));
	  if (TREE_CODE (b) == EUSE_NODE)
	    bstmt = *(EREF_STMT (b));
	  
	  if (!a || !b)
	    abort ();
	  for (; !bsi_end_p (bsi); bsi_next (&bsi))
	    {
	      if (bsi_stmt (bsi) == astmt
		  || bsi_stmt (bsi) == bstmt)
		break;
	    }
	  ret = (bsi_stmt (bsi) == astmt);
	}
    }
  return ret;
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
static bitmap
compute_idfs (dfs, stmt)
     bitmap *dfs;
     tree stmt;
{
  fibheap_t worklist;
  sbitmap inworklist = sbitmap_alloc (last_basic_block);
  bitmap idf = BITMAP_XMALLOC ();
  size_t i;
  basic_block block;
  worklist = fibheap_new (); 
  sbitmap_zero (inworklist);
  bitmap_zero (idf);
  block = bb_for_stmt (stmt);
  fibheap_insert (worklist, block->index, (void *)(size_t)block->index);
  SET_BIT (inworklist, block->index);
 
  while (!fibheap_empty (worklist))
    {
      int a = (size_t) fibheap_extract_min (worklist);
      bitmap_a_or_b (idf, idf, dfs[a]);
      EXECUTE_IF_SET_IN_BITMAP (dfs[a], 0, i,
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

/* Return true if EXPR is a strength reduction candidate. */
static bool
is_strred_cand (expr)
     const tree expr;
{
  if (TREE_CODE (TREE_OPERAND (expr, 1)) != MULT_EXPR
      && TREE_CODE (TREE_OPERAND (expr, 1)) != MINUS_EXPR
      && TREE_CODE (TREE_OPERAND (expr, 1)) != NEGATE_EXPR
      && TREE_CODE (TREE_OPERAND (expr, 1)) != PLUS_EXPR)
    return false;
  return true;
}
static bool
names_match_p (t1, t2)
     const tree t1;
     const tree t2;
{
  tree name1, name2;
  
  if (t1 == t2)
    return true;
  
  if (TREE_CODE (t1) == SSA_NAME)
    name1 = SSA_NAME_VAR (t1);
  else if (DECL_P (t1))
    name1 = t1;
  else
    name1 = NULL_TREE;
  
  if (TREE_CODE (t2) == SSA_NAME)
    name2 = SSA_NAME_VAR (t2);
  else if (DECL_P (t2))
    name2 = t2;
  else
    name2 = NULL_TREE;
  
  if (name1 == NULL_TREE && name2 != NULL_TREE)
    return false;
  if (name2 == NULL_TREE && name1 != NULL_TREE)
    return false;
  if (name1 == NULL_TREE && name2 == NULL_TREE)
    return operand_equal_p (t1, t2, 0);
  
  return name1 == name2;
}

	   
/* Determine if two expressions are lexically equivalent. */
static bool
expr_lexically_eq (v1, v2)
     const tree v1;
     const tree v2;
{
  if (TREE_CODE_CLASS (TREE_CODE (v1)) != TREE_CODE_CLASS (TREE_CODE (v2)))
    return false;
  if (TREE_CODE (v1) != TREE_CODE (v2))
    return false;
  switch (TREE_CODE_CLASS (TREE_CODE (v1)))
    {
    case '1':
      return names_match_p (TREE_OPERAND (v1, 0), TREE_OPERAND (v2, 0));
    case 'd':
      return names_match_p (v1, v2);
    case '2':
      {
	bool match;
	match = names_match_p (TREE_OPERAND (v1, 0), TREE_OPERAND (v2, 0));
	if (!match)
	  return false;
	match = names_match_p (TREE_OPERAND (v1, 1), TREE_OPERAND (v2, 1));
	if (!match)
	  return false;
	return true;
      }
    default:
      return false;
    }
  
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
  VARRAY_CLEAR (e1->erefs);
  VARRAY_CLEAR (e1->euses_dt_order);
  htab_delete (e1->repaired);
}
static bool
call_modifies_slot (call, expr)
     tree *call ATTRIBUTE_UNUSED;
     tree expr ATTRIBUTE_UNUSED;
{
  /* No load PRE yet, so this is always false. */
  return false;
}

/* Add call expression to expr-infos.  */
static int
add_call_to_ei (slot, data)
     struct expr_info *slot;
     void *data;
{
  tree *call = (tree *) data;
  struct expr_info *ei = (struct expr_info *) slot;
  if (call_modifies_slot (call, ei->expr))
    {
      VARRAY_PUSH_GENERIC_PTR (ei->occurs, NULL);
      VARRAY_PUSH_GENERIC_PTR (ei->lefts, NULL);
      VARRAY_PUSH_GENERIC_PTR (ei->kills, call);
    }
  return 0;
}

static void
process_left_occs_and_kills (bexprs, slot, exprp)
     varray_type bexprs;
     struct expr_info *slot ATTRIBUTE_UNUSED;
     tree *exprp;
{
  size_t k;
  tree expr = *exprp;
  if (TREE_CODE (expr) == CALL_EXPR)
    {
      tree callee = get_callee_fndecl (expr);
      if (!callee || !(call_expr_flags (expr) & (ECF_PURE | ECF_CONST)))
	{
	  for (k = 0; k < VARRAY_ACTIVE_SIZE (bexprs); k++)
	      add_call_to_ei (VARRAY_GENERIC_PTR (bexprs, k), exprp);
	}
    }
  else if (TREE_CODE (expr) == MODIFY_EXPR 
	   && TREE_CODE (TREE_OPERAND (expr, 1)) == CALL_EXPR)
    {
      tree op = TREE_OPERAND (expr, 1);
      tree callee = get_callee_fndecl (op);
      if (!callee|| !(call_expr_flags (op) & (ECF_PURE | ECF_CONST)))
	{
	  for (k = 0; k < VARRAY_ACTIVE_SIZE (bexprs); k++)
	      add_call_to_ei (VARRAY_GENERIC_PTR (bexprs, k), exprp);
	}
    }
}

static int
pre_expression (slot, data)
     struct expr_info *slot;
     void *data;
{
  size_t i;
  bool changed = true;
  struct expr_info *ei = (struct expr_info *) slot;
  basic_block bb;
  tree temp;

  if (VARRAY_ACTIVE_SIZE (ei->reals) < 2 
      &&  TREE_CODE (ei->expr) != INDIRECT_REF)
    return 0;
  
  idom_of_ephi = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);
 
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
  expr_phi_insertion ((bitmap *)data, ei);

  rename_1 (ei);
  if (dump_file)
    {
      size_t i;
      fprintf (dump_file, "Occurrences for expression ");
      print_generic_expr (dump_file, ei->expr, 0);
      fprintf (dump_file, "\n");
      for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->erefs); i++)
	{
	  print_generic_expr (dump_file, VARRAY_TREE (ei->erefs, i), 1);
	  fprintf (dump_file, "\n");
	}
    }
  insert_euse_in_preorder_dt_order (ei);
  down_safety (ei);
  will_be_avail (ei);

  if (dump_file)
    {
      fprintf (dump_file, "EPHI's for expression ");
      print_generic_expr (dump_file, ei->expr, 0);
      fprintf (dump_file, "\n");
      FOR_EACH_BB (bb)
      {
	if (ephi_at_block (bb) != NULL)
	  {
	    print_generic_expr (dump_file, ephi_at_block (bb), 1);
	    fprintf (dump_file, "\n");
	  }
      }
    }
  temp = create_tmp_var (TREE_TYPE (ei->expr), "pretmp");
  create_var_ann (temp);
  if (finalize_1 (ei, temp))
    {
      finalize_2 (ei);
      code_motion (ei, temp);
    }
  FOR_EACH_BB (bb)
  {
    bb_ann_t ann = bb_ann (bb);
    ann->ephi_nodes = NULL_TREE;
  }
  splay_tree_delete (idom_of_ephi);
  return 0;
}

static int
search_dt_preorder (bb, num)
     basic_block bb;
     int num;
{
  int i;
  splay_tree_insert (dfn, (splay_tree_key) bb, num);
  if (bb_ann (bb)->dom_children)
    EXECUTE_IF_SET_IN_BITMAP (bb_ann (bb)->dom_children, 0, i,
    {
      num = search_dt_preorder (BASIC_BLOCK (i), ++num);
    });
  return num;
}

			      
static void 
compute_dt_preorder ()
{
  search_dt_preorder (ENTRY_BLOCK_PTR, 0);
}

void 
tree_perform_ssapre (fndecl)
  tree fndecl;
{
  block_stmt_iterator j;
  basic_block block;
  varray_type bexprs;
  size_t k;
  int i;
  
  timevar_push (TV_TREE_PRE);

  old_new_map = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);
  VARRAY_GENERIC_PTR_INIT (bexprs, 1, "bexprs");
  
  /* Compute immediate dominators.  */
  pre_idom = calculate_dominance_info (CDI_DOMINATORS);
  domchildren = sbitmap_vector_alloc (last_basic_block, last_basic_block);
  sbitmap_vector_zero (domchildren, last_basic_block);
  compute_domchildren (pre_idom, domchildren);

  /* Compute dominance frontiers.  */
  pre_dfs = (bitmap *) xmalloc (sizeof (bitmap) * n_basic_blocks);
  for (i = 0; i < n_basic_blocks; i++)
     pre_dfs[i] = BITMAP_XMALLOC ();
  compute_dominance_frontiers (pre_dfs, pre_idom);

  dump_file = dump_begin (TDI_pre, &dump_flags);
  calculate_preorder ();
  dfn = splay_tree_new (splay_tree_compare_pointers, NULL, NULL);
  
  compute_dt_preorder ();
  
  /* Compute immediate uses.  */
  compute_immediate_uses (TDFA_USE_OPS);
  FOR_EACH_BB (block)
    for (j = bsi_start (block); !bsi_end_p (j); bsi_next (&j))
      {
	tree expr = bsi_stmt (j);
	tree orig_expr = bsi_stmt (j);
	tree stmt = bsi_stmt (j);
	struct expr_info *slot = NULL;
	
	if (TREE_CODE (expr) == MODIFY_EXPR 
	    || TREE_CODE (expr) == INIT_EXPR)
	  expr = TREE_OPERAND (expr, 1);	
	if (TREE_CODE_CLASS (TREE_CODE (expr)) == '2')
	  {
	    if (!DECL_P (TREE_OPERAND (expr, 0))
		&& !DECL_P (TREE_OPERAND (expr, 1)))
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
					     bsi_stmt_ptr (j));
		    VARRAY_PUSH_GENERIC_PTR (slot->kills, NULL);
		    VARRAY_PUSH_GENERIC_PTR (slot->lefts, NULL);
		    VARRAY_PUSH_TREE (slot->reals, stmt);
		    slot->strred_cand &= is_strred_cand (orig_expr);
		  }
		else
		  {
		    slot = ggc_alloc (sizeof (struct expr_info));
		    slot->expr = expr;
		    VARRAY_GENERIC_PTR_INIT (slot->occurs, 1, 
					     "Occurrence");
		    VARRAY_GENERIC_PTR_INIT (slot->kills, 1,
					     "Kills");
		    VARRAY_GENERIC_PTR_INIT (slot->lefts, 1,
					     "Left occurrences");
		    VARRAY_TREE_INIT (slot->reals, 1, "Real occurrences");
		    VARRAY_TREE_INIT (slot->erefs, 1, "EREFs");
		    VARRAY_TREE_INIT (slot->euses_dt_order, 1, "EUSEs");
 
		    VARRAY_PUSH_GENERIC_PTR (slot->occurs, bsi_stmt_ptr (j));
		    VARRAY_PUSH_GENERIC_PTR (slot->kills, NULL);
		    VARRAY_PUSH_GENERIC_PTR (slot->lefts, NULL);
		    VARRAY_PUSH_TREE (slot->reals, stmt);
		    
		    
		    VARRAY_PUSH_GENERIC_PTR (bexprs, slot);
		    slot->strred_cand = is_strred_cand (orig_expr);
		    slot->repaired = htab_create (7, htab_hash_pointer, 
						  htab_eq_pointer, NULL);
		  }
	      } 
	  }
	process_left_occs_and_kills (bexprs, slot, bsi_stmt_ptr (j));
      }
  
  for (k = 0; k < VARRAY_ACTIVE_SIZE (bexprs); k++)
    pre_expression (VARRAY_GENERIC_PTR (bexprs, k), pre_dfs);
  for (k = 0; k < VARRAY_ACTIVE_SIZE (bexprs); k++)
    free_expr_info (VARRAY_GENERIC_PTR (bexprs, k));

  /* Debugging dumps.  */
  if (dump_file)
    {
      if (dump_flags & TDF_STATS)
	{
	  fprintf (dump_file, "PRE stats:\n");
	  fprintf (dump_file, "Reloads:%d\n", pre_stats.reloads);
	  fprintf (dump_file, "Saves:%d\n", pre_stats.saves);
	  fprintf (dump_file, "Repairs:%d\n", pre_stats.repairs);
	  fprintf (dump_file, "New phis:%d\n", pre_stats.newphis);
	}
      dump_end (TDI_pre, dump_file);
    }  
  dump_function (TDI_pre, fndecl);
  splay_tree_delete (old_new_map);
  memset (&pre_stats, 0, sizeof (struct pre_stats_d));
  VARRAY_CLEAR (bexprs);  
  free_dominance_info (pre_idom);
  free (pre_preorder);
  for (i = 0; i < n_basic_blocks; i++)
     BITMAP_XFREE (pre_dfs[i]);
  free (pre_dfs);
  splay_tree_delete (dfn);
  sbitmap_vector_free (domchildren);
  timevar_pop (TV_TREE_PRE);
}
