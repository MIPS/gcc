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
#include "tree-optimize.h"
#include "tree-simple.h"
#include "tree-flow.h"
#include "tree-inline.h"

#define EXTRANEOUS_EPHI_REMOVAL 0

/* See http://citeseer.nj.nec.com/chow97new.html, and
   http://citeseer.nj.nec.com/kennedy99partial.html for details of the
   algorithm.

   kennedy99partial is newer, and is what this implementation is based
   on.
   Pieces are also taken from Open64's SSAPRE implementation.

*/
/* TODO:
   Strength reduction.
   Register promotion.
*/
struct expr_info;
static int expr_lexically_eq PARAMS ((tree, tree));
static void free_expr_info PARAMS ((struct expr_info *));
static sbitmap compute_idfs PARAMS ((sbitmap *, tree));
static void compute_domchildren PARAMS ((dominance_info, sbitmap *));

static inline bool a_dom_b PARAMS ((basic_block, basic_block));
static void set_var_phis PARAMS ((tree_ref, int));
static inline tree_ref find_tree_ref_for_var PARAMS ((struct expr_info *, tree, tree));
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
static inline bool defs_y_dom_x PARAMS ((tree_ref, tree_ref));
static void assign_new_class PARAMS ((tree_ref, varray_type *));
static void insert_occ_in_preorder_dt_order_1 PARAMS ((struct expr_info *, fibheap_t, basic_block));
static void insert_occ_in_preorder_dt_order PARAMS ((struct expr_info *, fibheap_t));
static void insert_euse_in_preorder_dt_order_1 PARAMS ((struct expr_info *, fibheap_t, basic_block));
static void insert_euse_in_preorder_dt_order PARAMS ((struct expr_info *, fibheap_t));
static inline int opnum_of_phi PARAMS ((tree_ref, int));
static varray_type phi_opnd_from_res PARAMS ((tree_ref, int, basic_block));
static splay_tree rename_2 PARAMS ((struct expr_info *, varray_type *));
static void rename_1 PARAMS ((struct expr_info *));
static void finalize_2 PARAMS ((struct expr_info *));
static void finalize_1 PARAMS ((struct expr_info *, tree));
static tree_ref phi_for_operand PARAMS ((struct expr_info *, tree_ref));\
static void set_save PARAMS ((struct expr_info *, tree_ref));
static void set_replacement PARAMS ((struct expr_info *, tree_ref, tree_ref));
static bool requires_edge_placement PARAMS ((tree_ref));
static void calculate_preorder PARAMS ((void));
static int *pre_preorder;
static dominance_info pre_idom;
static sbitmap *pre_dfs;
static FILE *dump_file;
static int dump_flags;
static int class_count;
static int preorder_count;

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
  fibheap_insert (worklist, block->index, (void *)block->index);
  SET_BIT (inworklist, block->index);
 
  while (!fibheap_empty (worklist))
    {
      int a = (int) fibheap_extract_min (worklist);
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


/* Determine if two expressions are lexically equivalent.
   Return a value like one that would be used for qsort comparisons,
   so 0 if equal, something else otherwise.
   -+
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
  /*free (e1);*/
}

/* dfphis and varphis, from the paper. */
static sbitmap dfphis;
static sbitmap *varphis;

/* Figure out which variables are defined by PHI's, and record them so
   we can insert ExprPHI's in the right places. */
static void
set_var_phis (phi, i)
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
          if (ref_type (phi_operand) & V_PHI)
            set_var_phis (phi_operand, i);
        }
    }
}

/* Helper macro to simplify accessing the phi for a given block. */
#define phi_at_block(expr, b) VARRAY_GENERIC_PTR ((expr)->phis, pre_preorder[(b)->index])

/* Helper macro to simplify accessing a phi operand for a given block. */
#define phi_operand(phi, b) VARRAY_GENERIC_PTR (EXPRPHI_PHI_ARGS (phi), pre_preorder[(b)->index])

/* Given a statement, find the V_DEF in it. */
static inline tree_ref
find_def_for_stmt (stmt)
     tree stmt;
{

  struct ref_list_node *tmp;
  tree_ref ref;

  FOR_EACH_REF (ref, tmp, tree_refs (stmt))
    {
      if (ref_type (ref) & V_DEF)
        return ref;
    }
  return NULL;
}

/* Given the expression info, the real occurrence, and a variable,
   find the V_USE of the variable  contained in that occurrence. */
static inline tree_ref
find_tree_ref_for_var (ei, real, var)
     struct expr_info *ei;
     tree real;
     tree var;
{
  struct ref_list_node *tmp;
  tree_ref ref;
  size_t realnum;


  /* Find the realstmt for the real. */
  for (realnum = 0; realnum < VARRAY_ACTIVE_SIZE (ei->reals); realnum++)
    if (VARRAY_TREE (ei->reals, realnum) == real)
      break;

  /* Abort if we can't find one. */
  if (realnum == VARRAY_ACTIVE_SIZE (ei->reals))
    abort();

  /* Now look for the use of var in that statement. */
  FOR_EACH_REF (ref, tmp, tree_refs (VARRAY_TREE (ei->realstmts, realnum)))
    {
      if (!(ref_type (ref) & V_USE)
	  || !is_simple_modify_expr (ref_expr (ref))
          || TREE_OPERAND (ref_expr (ref), 1) != real)
        continue;
      if (ref_var (ref) != var)
        continue;
      if (!imm_reaching_def (ref))
        continue;
      return ref;
    }
  return NULL;
}
/* Determine if the definitions of variables in Y dominate the basic
   block of X. */
static inline bool 
defs_y_dom_x ( y, x)
     tree_ref y;
     tree_ref x;
{
  size_t i;
  

  /* It's a binary or unary expression, so it has only two operands at
     most. */

  for (i =  0; i < 2; i++)
    {
      tree_ref ref;
      struct ref_list_node *tmp;
      if (!TREE_OPERAND (ref_expr (y), i))
        continue;
      FOR_EACH_REF (ref, tmp, tree_refs (ref_stmt (y)))
        {
          /* Find the ref for this operand. */
          if (!(ref_type (ref) & V_USE)
	      || !is_simple_modify_expr (ref_expr (ref))
              || TREE_OPERAND (ref_expr (ref), 1) != ref_expr (y))
            continue;
          if (ref_var (ref) != TREE_OPERAND (ref_expr (y), i))
            continue;
          if (!imm_reaching_def (ref))
            return false;
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
  tree_ref use1;
  tree_ref use2;
  struct ref_list_node *tmp;

  FOR_EACH_REF (use1, tmp, tree_refs (t1))
    {
      tree use1expr = ref_expr (use1);
      if (!(ref_type (use1) & V_USE) 
	  || !is_simple_modify_expr (use1expr))
        continue;
      use1expr = TREE_OPERAND (use1expr, 1);
      if (TREE_CODE (use1expr) != TREE_CODE (t2))
        continue;
      use2 = find_tree_ref_for_var (ei, t2, ref_var (use1));
      if (!use2 || (imm_reaching_def (use2) != imm_reaching_def (use1)))
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
  EXPRREF_CLASS (occ) = class_count;
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
      if ((ref_type (ref) & E_USE) || ref_type (ref) == E_PHI)
        {
          fibheap_insert (fh, preorder_count++, ref);
        }
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
    {
      fibheap_insert (fh, preorder_count++, phi_at_block (ei, block));
    }
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->occurs); i++)
    {
      tree_ref newref;
      tree occurstmt = VARRAY_TREE (ei->occurstmts, i);
      tree_ref occurref = VARRAY_GENERIC_PTR (ei->refs, i);
      if (ref_bb (occurref) != block)
        continue;
      if (TREE_CODE (VARRAY_TREE (ei->occurs, i)) == CALL_EXPR)
	{
	  newref = create_ref (VARRAY_TREE (ei->occurs, i),
				  E_KILL, block, occurstmt,
				  VARRAY_TREE (ei->occurs, i), NULL, true);
	  VARRAY_PUSH_GENERIC_PTR (ei->erefs, newref);
	  fibheap_insert (fh, preorder_count++, newref);
	}
      else
	{
	  newref = create_ref (VARRAY_TREE (ei->occurs, i), 
				  E_USE, block, occurstmt, 
				  VARRAY_TREE (ei->occurs, i), NULL, true);
	  VARRAY_PUSH_GENERIC_PTR (ei->erefs, newref);
	  EXPRUSE_DEF (newref) = NULL;
	  EXPRREF_CLASS (newref) = 0;
	  EXPRUSE_PHIOP (newref) = 0;
	  EXPRUSE_HAS_REAL_USE (newref) = 0;
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
              tree_ref newref = create_ref (NULL, E_USE, block, 0, 0, 0, true);
              tree_ref phi = phi_at_block (ei, succ->dest);
	      VARRAY_PUSH_GENERIC_PTR (ei->erefs, newref);
              EXPRUSE_DEF (newref) = NULL;
              EXPRREF_CLASS (newref) = 0;
              EXPRUSE_PHIOP (newref) = 1;
              EXPRUSE_HAS_REAL_USE (newref) = 0;
	      EXPRREF_SAVE (newref) = 0;
	      EXPRREF_RELOAD (newref) = 0;
	      EXPRREF_INSERTED (newref) = 0;
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
    {
      if (phi_arg_edge (phi_arg (phi, i))->src->index == j)
	return i;
    }
  
  abort();
}

/* Factor variables through phi operands.  */
static varray_type
phi_opnd_from_res (Z, j, b)
     tree_ref Z;
     int j;
     basic_block b;
{
  varray_type Q;
  tree_ref v;
  size_t i = 0;
  struct ref_list_node *tmp;

  VARRAY_GENERIC_PTR_INIT (Q, 1, "Temp ops");

  /* b <- block containing phi that defines Z.
     Q <- copy of Z */
  FOR_EACH_REF (v, tmp, tree_refs (ref_stmt (Z)))
    VARRAY_PUSH_GENERIC_PTR (Q, v);

  /* For each variable v in Z */
  FOR_EACH_REF (v, tmp, tree_refs (ref_stmt (Z)))
    {
      /* If v is defined by phi in b */
      if (ref_type (v) & V_USE
          && ref_type (imm_reaching_def (v)) & V_PHI)
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
      else if (ref_type (v) & V_DEF 
               || ref_type (v) & V_PHI
               || ref_type (v) & E_USE)
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
        bitmap_clear (EXPRPHI_PROCESSED(phi));
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
      phiZ = EXPRUSE_DEF (Z);
      
      /* for each operand w of f do { */
      FOR_EACH_BB (block)
	{
          tree_ref w;
	  i = block->index;
	  
          if (phi_operand (phiZ, block) == NULL)
            continue;
          w = phi_operand (phiZ, block);

	  /* if (not processed(w)) { */
          if (!bitmap_bit_p (EXPRPHI_PROCESSED (phiZ), i))
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
              Y = phi_opnd_from_res (Z, j, ref_bb (phiZ));
              X = EXPRUSE_DEF (w);
	      
	      if (!X)
	        {
	          VARRAY_CLEAR (Y);
		  continue;
	        }
	      
	      /* if (X is a real occurrence) */
	      if (ref_type (X) & E_USE)
                {
                  bool match = true;
		  tree_ref op1;
		  struct ref_list_node *tmp2;
		  /* if (all corresponding variables in Y and X have
		     the same SSA version) */
		  k = 0;
		  FOR_EACH_REF (op1, tmp2, tree_refs (ref_stmt (X)))
                    {
                      tree_ref op2;
                      if (!(ref_type (op1) & V_USE))
                        continue;
                      if (ref_type (op1) & V_USE)
                        op1 = imm_reaching_def (op1);
		      op2 = VARRAY_GENERIC_PTR (Y, k);
		      
                      if (op2 && ref_type (op2) & V_USE)
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
                      EXPRUSE_DEF (w) = NULL;
		      EXPRUSE_HAS_REAL_USE (w) = 0;
                    } 
		  else
		    {	
		      /* no change needed  */
		      EXPRUSE_HAS_REAL_USE (w) = 1;
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
                      if (ref_type (ref) & V_USE)
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
		      EXPRUSE_DEF (w) = NULL;
                      EXPRPHI_DOWNSAFE (phiZ) = 0;
                    }
                }
	      /* processed (w) <- true */
	      bitmap_set_bit (EXPRPHI_PROCESSED (phiZ), i);
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
              if (ref_type (stackref) & E_PHI)
                EXPRPHI_DOWNSAFE (stackref) = 0;
            }
          continue;
        }
      if (ref_type (y) & E_KILL 
	  && TREE_CODE (ref_expr (y)) == CALL_EXPR)
        {
	  if (VARRAY_ACTIVE_SIZE (stack) > 0)
            {
              tree_ref stackref = VARRAY_TOP_GENERIC_PTR (stack);
              if (ref_type (stackref) & E_PHI)
                EXPRPHI_DOWNSAFE (stackref) = 0;
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
      if (ref_type (y) & E_PHI)
        {
	  assign_new_class (y, &stack);
        }
      /* else if (Y is a real occurrence) */
      else if (ref_type (y) & E_USE && EXPRUSE_PHIOP (y) == 0)
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
              if (ref_type (x) & E_USE)
                {
                  /* If (all corresponding variables in Y and X have
                     the same SSA version) */
                  if (defs_match_p (ei, ref_stmt (y), ref_expr (x)))
                    {
                      /* class(Y) <- class(X)
                         def(Y) <- X */
                      EXPRREF_CLASS (y) = EXPRREF_CLASS (x);
#if 0
                      EXPRUSE_DEF (y) = x;
#else
		      EXPRUSE_DEF (y) = EXPRUSE_DEF (x) != NULL ? EXPRUSE_DEF (x) : x;
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
                  if (!(ref_type (x) & E_PHI))
                    abort();
		  /* if (definitions of all variables in Y dominate X)
		     { */
		  if (defs_y_dom_x (y, x))
                    {
                      /* class(Y) <- class(X)
                         def(Y) <- X
                         set_for_rename2 <- set_for_rename2 U {Y} */
                      EXPRREF_CLASS (y) = EXPRREF_CLASS (x);
                      EXPRUSE_DEF (y) = x;


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
		      EXPRPHI_DOWNSAFE (x) = 0;
                      assign_new_class ( y, &stack);
                    }
                } 
            } 
        }
      /* else if (Y is an exprphi operand occurrence) */
      else if (ref_type (y) & E_USE && EXPRUSE_PHIOP (y) == 1)
        {
          if (VARRAY_ACTIVE_SIZE (stack) == 0)
            {
              /* def(Y) <- tack */
	      EXPRUSE_DEF (y) = NULL;
	      EXPRUSE_HAS_REAL_USE (y) = 0;
            }
          else
            {

              /* X = Top(stack)
                 class(Y) <- class(X)
                 def(Y) <- X */
              tree_ref x = VARRAY_TOP_GENERIC_PTR (stack);
              EXPRREF_CLASS (y) = EXPRREF_CLASS (x);
              EXPRUSE_DEF (y) = x;
	      if (!(ref_type (x) & E_PHI))
		{
		  EXPRUSE_HAS_REAL_USE (y) = 1;
		  if (EXPRUSE_DEF (x) && ref_type (EXPRUSE_DEF (x)) & E_PHI)
			VARRAY_PUSH_GENERIC_PTR (rename2_set, x);
		}
	      else
		{
		  EXPRUSE_HAS_REAL_USE (y) = 0;
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
      def = EXPRUSE_DEF (ref);
      for (succ = ref_bb (ref)->succ; succ; succ = succ->succ_next)
	{
	  if (phi_at_block (ei, succ->dest) != NULL)
	    {
	      tree_ref phi = phi_at_block (ei, succ->dest);
	      if (bitmap_bit_p (EXPRPHI_PROCESSED (phi), 
				ref_bb (ref)->index))
		needcheck = false;
	    }
	}
      if (!needcheck)
	continue;

      if (def && (ref_type (def) & E_PHI))
	{
	  EXPRPHI_DOWNSAFE (def) = 0;
	}
      EXPRUSE_DEF (ref) = NULL;
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
  if (ref_type (X) & E_USE && !EXPRUSE_PHIOP (X))
    EXPRREF_SAVE (X) = 1;
  else if (ref_type (X) & E_PHI)
    {
      int i;
      FOR_EACH_BB (bb)
	{
	  tree_ref w = phi_operand (X, bb);
	  i = bb->index;

	  if (w && !bitmap_bit_p (EXPRPHI_PROCESSED (X), i))
	    {
	      bitmap_set_bit (EXPRPHI_PROCESSED (X), i);
	      set_save (ei, EXPRUSE_DEF (w));
	    }
	}
    }
#if EXTRANEOUS_EPHI_REMOVAL 
  if (ref_type (X) & E_USE && !EXPRUSE_PHIOP (X))
    {
      sbitmap idfs = compute_idfs (pre_dfs, ref_stmt (X));
      int i;
      EXECUTE_IF_SET_IN_SBITMAP (idfs, 0, i,
      {
	tree_ref phi = phi_at_block (ei, BASIC_BLOCK (i));
	if (phi != NULL && EXPRPHI_WILLBEAVAIL (phi))
	  EXPRPHI_EXTRANEOUS (phi) = 0;
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
      if (f == NULL || !EXPRPHI_WILLBEAVAIL (f) || f == g)
	continue;
      FOR_EACH_BB (bb2)
	{
	  tree_ref operand = phi_operand (f, bb2);
	  if (operand && EXPRUSE_DEF (operand) == g)
	    {
	      if (EXPRPHI_EXTRANEOUS (f))
		set_replacement (ei, f, replacing_def);
	      else
		phi_operand (f, bb2) = replacing_def;
	    }
	}
      for (k = 0; k < VARRAY_ACTIVE_SIZE (ei->erefs); k++)
	{
	  tree_ref X = VARRAY_GENERIC_PTR (ei->erefs, k);
	  if (ref_type (X) & E_USE 
	      && EXPRUSE_PHIOP (X) == 0 
	      && EXPRREF_RELOAD (X)
	      && EXPRUSE_DEF (X) == g)
	    {
	      EXPRUSE_DEF (X) = replacing_def;
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
      if (ref_type (ref) & E_PHI)
	{
	  bitmap_clear (EXPRPHI_PROCESSED (ref));
	  if (EXPRPHI_WILLBEAVAIL (ref))
	    EXPRPHI_EXTRANEOUS (ref) = 1;
	}
      else if (ref_type (ref) & E_USE 
	       && EXPRUSE_PHIOP (ref) == 0)
	{
	  EXPRREF_SAVE (ref) = 0;	 
	}
    }  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->erefs); i++)
    {
      tree_ref ref = VARRAY_GENERIC_PTR (ei->erefs, i);
      if (ref_type (ref) & E_USE 
	  && EXPRUSE_PHIOP (ref) == 0 && !EXPRREF_INSERTED (ref))
	{	 
	  if (EXPRREF_RELOAD (ref))
	    set_save (ei, EXPRUSE_DEF (ref));
	}
    }
  /* This is pointless unless we plan on performing more ESSA passes. */
#if EXTRANEOUS_EPHI_REMOVAL 
  for (i = 0; i < n_basic_blocks; i++)
    {
      tree_ref ref = phi_at_block (ei, BASIC_BLOCK (i));
      if (!ref)
	continue;     
      if (EXPRPHI_WILLBEAVAIL (ref))
	{
	  int k;
	  for (k = 0; k < n_basic_blocks; k++)
	    {
	      tree_ref w = phi_operand (ref, BASIC_BLOCK (k));
	      tree_ref defw;
	      if (!w || !EXPRUSE_DEF (w) )
		continue;
	      defw = EXPRUSE_DEF (w);
	      if ((ref_type (defw) & E_PHI 
		   && !EXPRPHI_EXTRANEOUS (defw))
		  || (ref_type (defw) & E_USE 
		      && !EXPRUSE_PHIOP (defw)))
		set_replacement (ei, ref, EXPRUSE_DEF (w));  
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
  tree_ref *avdefs;
  tree_ref X;
  int x;
  fibheap_t fh;
  avdefs = xcalloc (class_count + 1,sizeof (tree_ref));
  fh = fibheap_new ();
  insert_euse_in_preorder_dt_order (ei, fh);
  while (!fibheap_empty (fh))
    {
      X = fibheap_extract_min (fh);
      x = EXPRREF_CLASS (X);
      if (ref_type (X) & E_PHI)
        {
          if (EXPRPHI_WILLBEAVAIL (X))
            avdefs[x] = X;
        }
      else if (ref_type (X) & E_USE && EXPRUSE_PHIOP (X) == 0)
        {
          if (avdefs[x] == NULL 
              || !a_dom_b (ref_bb (avdefs[x]), ref_bb (X)))
            {
	      EXPRREF_RELOAD (X) = 0;
              avdefs[x] = X;
            }
          else
            {
	      EXPRREF_RELOAD (X) = 1;
              EXPRUSE_DEF (X) = avdefs[x];
            }
        }
      else
        {
          tree_ref phi = phi_for_operand (ei, X);
          if (!phi)
            abort ();
          if (EXPRPHI_WILLBEAVAIL (phi))           
            {
              if (can_insert (X))
                {
		  tree stmt;
		  tree expr;
		  basic_block bb;
		  tree endtree;
                  /* Insert definition of expr at end of BB containing X. */
		  if (dump_file)
		    {
		      fprintf (dump_file, "In BB %d, insert save of ",
			       ref_bb (X)->index);
		      print_c_tree (dump_file, ei->expr);
		      fprintf (dump_file, " to ");
		      print_c_tree (dump_file, temp);
		      fprintf (dump_file, " after ");
		      print_c_node (dump_file, BLOCK_END_TREE (ref_bb(X)->index));
		      fprintf (dump_file, " (at end of BB), because of ExprPhi");
		      fprintf (dump_file, " in BB %d\n", 
			       ref_bb (phi)->index);
		    }
		  expr = build_modify_expr (temp, NOP_EXPR, ei->expr);
		  stmt = build_stmt (EXPR_STMT, expr);
		  TREE_TYPE (stmt) = TREE_TYPE (expr);
		  bb = ref_bb (X);
		  /* If it's a goto, we need to insert *before* it.
		     This might not be necessary once goto elimination
		     is functional.  */
		  endtree = BLOCK_END_TREE (bb->index);
		  if (is_ctrl_altering_stmt (endtree))
		    insert_stmt_tree_before (stmt, endtree, bb);
		  else
		    insert_stmt_tree_after (stmt, endtree, bb);
		  
		  EXPRUSE_DEF (X) = create_ref (expr, E_USE, 
						ref_bb (X), stmt, expr,
						&TREE_OPERAND (stmt, 0), true);
		  VARRAY_PUSH_GENERIC_PTR (ei->erefs, EXPRUSE_DEF (X));
		  EXPRREF_RELOAD (EXPRUSE_DEF (X)) = 0;
		  EXPRREF_SAVE (EXPRUSE_DEF (X)) = 1;
		  EXPRREF_INSERTED (EXPRUSE_DEF (X)) = 1;
		  EXPRUSE_PHIOP (EXPRUSE_DEF (X)) = 0;
		  EXPRUSE_HAS_REAL_USE (EXPRUSE_DEF (X)) = 1;
		  EXPRREF_SAVE (X) = 0;
		  EXPRREF_RELOAD (X) = 0;
                  /*
                    EXPRUSE_DEF (X) = new occurrence. 
		  */
                }
              else
                {
                  EXPRUSE_DEF (X) = avdefs[x];		  
                }
            }
        }
    }
  fibheap_delete (fh);
  free (avdefs);
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
          tree_ref ref;
	  struct ref_list_node *tmp;
          int varcount = 0;
	  FOR_EACH_REF (ref, tmp, tree_refs (VARRAY_TREE (ei->realstmts, i)))
            {
              if (!(ref_type (ref) & V_USE )
                  || !is_simple_modify_expr (ref_expr (ref))
                  || TREE_OPERAND (ref_expr (ref), 1) != real)
                continue;
              if (ref_var (ref) != TREE_OPERAND (real, 0)
                  && (TREE_OPERAND (real, 1) == NULL 
		      || ref_var (ref) != TREE_OPERAND (real, 1)))
                continue;
              if (!imm_reaching_def (ref)
		  || !(ref_type (imm_reaching_def (ref)) & V_PHI))
                continue;
              set_var_phis (imm_reaching_def (ref), varcount++);
            }
        }
    }
  sbitmap_a_or_b (dfphis, dfphis, varphis[0]);
  sbitmap_a_or_b (dfphis, dfphis, varphis[1]);

  EXECUTE_IF_SET_IN_SBITMAP(dfphis, 0, i, 
  {
    tree_ref ref = create_ref (ei->expr, E_PHI, 
                                BASIC_BLOCK (i), 
                                NULL, NULL, NULL, true);
    VARRAY_PUSH_GENERIC_PTR (ei->erefs, ref);
    EXPRPHI_DOWNSAFE (ref) = 1;
    EXPRPHI_CANBEAVAIL (ref) = 1;
    EXPRPHI_LATER (ref) = 1;
    EXPRPHI_EXTRANEOUS (ref) = 1;		     
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
  
  if (EXPRUSE_HAS_REAL_USE (phiop))
    return;
  phi = EXPRUSE_DEF (phiop);
  if (!phi || !(ref_type (phi) & E_PHI))
    return;
  if (!EXPRPHI_DOWNSAFE (phi))
    return;  
  EXPRPHI_DOWNSAFE (phi) = 0;
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
      if (phi == NULL || EXPRPHI_DOWNSAFE (phi))
        continue;
      FOR_EACH_BB (block2)
        if (phi_operand (phi, block2) != NULL)
          reset_down_safe (phi_operand (phi, block2));
    }      
} 
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
	  if (EXPRUSE_DEF (operand) == NULL && EDGE_CRITICAL_P (pred))
	    return true;
	}
    }
  return false;
}
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
      EXPRPHI_CANBEAVAIL (phi) = 1;
    }

  FOR_EACH_BB (block)
    {     
      tree_ref phi = phi_at_block (ei, block);
      edge pred;
      if (phi == NULL)
        continue;
      if (!EXPRPHI_CANBEAVAIL(phi))
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
      if (EXPRPHI_DOWNSAFE (phi))
        continue;
      for (pred = block->pred; pred; pred = pred->pred_next)
        {
          if (pred->src != ENTRY_BLOCK_PTR)
            {
              tree_ref operand = phi_operand (phi, pred->src);
              if (EXPRUSE_DEF (operand) == NULL)
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

  EXPRPHI_CANBEAVAIL (phi) = 0;
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
              if (EXPRUSE_DEF (operand) == phi)
                {
                  if (!EXPRUSE_HAS_REAL_USE(operand))
		    {
		      if (!EXPRPHI_DOWNSAFE (other) 
			  && EXPRPHI_CANBEAVAIL (other))
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
  EXPRPHI_LATER (phi) = 0;
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
              if (EXPRUSE_DEF (operand) == phi)
                {
                  if (EXPRPHI_LATER (other))                
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
      EXPRPHI_LATER (phi) = EXPRPHI_CANBEAVAIL (phi);
    }
      
  FOR_EACH_BB (block)
    {
      tree_ref phi = phi_at_block (ei, block);
      edge pred;
      if (phi == NULL)
        continue;
      if (!EXPRPHI_LATER (phi))
        continue;

      for (pred = block->pred; pred; pred = pred->pred_next)
        {
          if (pred->src != ENTRY_BLOCK_PTR)
            {
              tree_ref operand = phi_operand (phi, pred->src);
              if (EXPRUSE_DEF (operand) != NULL 
		  && EXPRUSE_HAS_REAL_USE (operand))
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
  if (EXPRUSE_DEF (op) == NULL)
    return true;
  def = EXPRUSE_DEF (op);
  if (!EXPRUSE_HAS_REAL_USE (op))
    if (ref_type (def) & E_PHI && !(EXPRPHI_WILLBEAVAIL (def)))
      return true;
  return false;
}

/* Perform actual code motion.  */                        
static void
code_motion (ei, temp)
     struct expr_info *ei;
     tree temp;
{
  fibheap_t exprs;
  tree_ref use;
  
  exprs = fibheap_new ();
  insert_euse_in_preorder_dt_order (ei, exprs);
  while (!fibheap_empty (exprs))
    {
      use = fibheap_extract_min (exprs);
      if (ref_type (use) & E_USE /*&& !EXPRUSE_PHIOP (use) */
	  && !EXPRREF_INSERTED (use))
	{
	  if (EXPRUSE_PHIOP (use) && (EXPRREF_RELOAD (use) || EXPRREF_SAVE (use)))
	    abort();
	  if (EXPRREF_SAVE (use))
	    {
	      basic_block use_bb = ref_bb (use);
	      tree use_stmt = ref_stmt (use);
	      tree use_expr = ref_expr (use);
	      tree newexpr;
	      if (dump_file)
		{
		  fprintf (dump_file, "In BB %d, insert save of ", 
			   use_bb->index);
		  print_c_tree (dump_file, use_expr);
		  fprintf (dump_file, " to ");
		  print_c_tree (dump_file, temp);
		  fprintf (dump_file, " in statement ");
		  print_c_tree (dump_file, TREE_OPERAND (use_stmt, 0));
		  fprintf (dump_file, " on line %d\n", STMT_LINENO (use_stmt));
		}
	      newexpr = build_modify_expr (temp, NOP_EXPR, use_expr);
	      replace_expr_in_tree (use_stmt, use_expr, newexpr);
	    }
	  else if (EXPRREF_RELOAD (use))
	    {
	      basic_block use_bb = ref_bb (use);
	      tree use_stmt = ref_stmt (use);
	      tree use_expr = ref_expr (use);
	      if (dump_file)
                {
		  fprintf (dump_file, "In BB %d, insert reload of ", 
			   use_bb->index);
                  print_c_tree (dump_file, use_expr);
                  fprintf (dump_file, " from ");
                  print_c_tree (dump_file, temp);
                  fprintf (dump_file, " in statement ");
                  print_c_tree (dump_file, TREE_OPERAND (use_stmt, 0));
                  fprintf (dump_file, " on line %d\n", STMT_LINENO (use_stmt));
                }
              replace_expr_in_tree (use_stmt, use_expr, temp);
	    }
	}
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

/* Add call expression to expr-infos.  */
static int
add_call_to_ei (slot, data)
     struct expr_info *slot;
     void *data;
{
  tree_ref call = (tree_ref) data;
  struct expr_info *ei = (struct expr_info *) slot;
  VARRAY_PUSH_TREE (ei->occurs, ref_expr (call));
  VARRAY_PUSH_TREE (ei->occurstmts, ref_stmt (call));
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

  struct expr_info *ei = (struct expr_info *) slot;
  if (VARRAY_ACTIVE_SIZE (ei->reals) < 2)
    return 0;
  expr_phi_insertion ((sbitmap *)data, ei);
  rename_1 (ei);
  if (dump_file)
    {
      fprintf (dump_file, "Occurrences for expression ");
      print_c_tree (dump_file, ei->expr);
      fprintf (dump_file, "\n");
      dump_ref_array (dump_file, "", ei->refs, 0, 1);
    }
  down_safety (ei);
  will_be_avail (ei);
  if (dump_file)
    {
      fprintf (dump_file, "ExprPhi's for expression ");
      print_c_tree (dump_file, ei->expr);
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

    pushlevel (0);
    temp = create_tmp_var (type_of_expr, "pretmp");
    declare_tmp_vars (getdecls(), 
		      COMPOUND_BODY (DECL_SAVED_TREE (current_function_decl)));
    poplevel (1, 1, 0);
    finalize_1 (ei, temp);
    finalize_2 (ei);
    code_motion (ei, temp);
  }
  
  return 0;
}

static
void calculate_preorder ()
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

  dump_file = dump_begin (TDI_ssa_pre, &dump_flags);
  calculate_preorder ();
  FOR_EACH_BB (bb)
    {    
      tree_ref ref;
      struct ref_list_node *tmp;

      htab_empty (seen);
      FOR_EACH_REF (ref, tmp, bb_refs (bb))
	{
	  tree expr = ref_expr (ref);
	  tree stmt = ref_stmt (ref);
	  if (!(ref_type (ref) & V_USE))
	    continue;
	  if (htab_find (seen, expr) != NULL)
	    continue;
	  if (is_simple_modify_expr (expr))
		  expr = TREE_OPERAND (expr, 1);
	  if ((is_simple_binary_expr (expr)
/*	       || is_simple_cast (expr)
	       || is_simple_unary_expr (expr)*/)
	     && TREE_CODE (expr) != INDIRECT_REF)
	    {
	      struct expr_info *slot;
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
		  VARRAY_PUSH_TREE (slot->occurs, expr);
		  VARRAY_PUSH_TREE (slot->occurstmts, stmt);
		  VARRAY_PUSH_TREE (slot->reals, expr);
		  VARRAY_PUSH_TREE (slot->realstmts, stmt);
		  VARRAY_PUSH_GENERIC_PTR (slot->refs, ref);
		}
	      else
		{
		  slot = ggc_alloc (sizeof (struct expr_info));
		  slot->expr = expr;
		  VARRAY_TREE_INIT (slot->occurs, 1, "Kills and occurrence");
		  VARRAY_TREE_INIT (slot->occurstmts, 1, 
				    "Kills and occurrence statements");
		  VARRAY_TREE_INIT (slot->reals, 1, "Real occurrences");
		  VARRAY_TREE_INIT (slot->realstmts, 1, 
				    "Real occurrence statements");
		  VARRAY_GENERIC_PTR_INIT (slot->phis, last_basic_block, 
					   "EPHIs");
		  VARRAY_GENERIC_PTR_INIT (slot->erefs, 1, "EREFs");
		  VARRAY_GENERIC_PTR_INIT (slot->refs, 1, "REFs");
		  VARRAY_PUSH_TREE (slot->occurs, expr);
		  VARRAY_PUSH_TREE (slot->occurstmts, stmt);
		  VARRAY_PUSH_TREE (slot->reals, expr);
		  VARRAY_PUSH_TREE (slot->realstmts, stmt);
		  VARRAY_PUSH_GENERIC_PTR (slot->refs, ref);

		  VARRAY_PUSH_GENERIC_PTR (bexprs, slot);
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
	  *(htab_find_slot  (seen, ref_expr (ref), INSERT)) = ref_expr (ref);
	}
    }
  for (j = 0; j < VARRAY_ACTIVE_SIZE (bexprs); j++)
     pre_part_1_trav (VARRAY_GENERIC_PTR (bexprs, j), pre_dfs);
  /*  simplify_stmt (fn, NULL_TREE); */
  /* Debugging dump after SSA PRE */
  if (dump_file)
    {
      fprintf (dump_file, "%s()\n",
               IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));
      
      if (dump_flags & TDF_RAW)
        dump_node (fn, TDF_SLIM | dump_flags, dump_file);
      else
        print_c_tree (dump_file, fn);

      dump_end (TDI_ssa_pre, dump_file);
    }
  
  for (j = 0; j < VARRAY_ACTIVE_SIZE (bexprs); j++)
    free_expr_info (VARRAY_GENERIC_PTR (bexprs, j));
  VARRAY_CLEAR (bexprs);
  htab_delete (seen);
  free_dominance_info (pre_idom);
  free (pre_preorder);
  sbitmap_vector_free (pre_dfs);
  sbitmap_vector_free (domchildren);
}
