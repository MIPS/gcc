/* SSA-PRE for trees.
   Copyright (C) 2001 Free Software Foundation, Inc.
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
#include "tree-optimize.h"
#include "tree-simple.h"
#include "tree-flow.h"
#include "tree-inline.h"
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
#define EXTRANEOUS_EPHI_REMOVAL 0

/* See http://citeseer.nj.nec.com/chow97new.html, and
   http://citeseer.nj.nec.com/kennedy99partial.html for details of the
   algorithm.

   kennedy99partial is newer, and is what this implementation is based
   on.
   Pieces are also taken from Open64's SSAPRE implementation.

*/
struct expr_info;
static hashval_t hash_expr_tree PARAMS ((const void *));
static int expr_lexically_eq PARAMS ((tree, tree));
static void free_expr_info PARAMS ((struct expr_info *));
static sbitmap compute_idfs PARAMS ((sbitmap *, tree));
static void compute_domchildren PARAMS ((dominance_info, sbitmap *));

static inline bool a_dom_b PARAMS ((basic_block, basic_block));
static void set_var_phis PARAMS ((varref, int));
static inline varref find_varref_for_var PARAMS ((struct expr_info *, tree, tree));
static inline varref find_def_for_stmt PARAMS ((tree));
static void expr_phi_insertion PARAMS ((sbitmap *, struct expr_info *));
static int pre_part_1_trav PARAMS ((struct expr_info *, void *));
static int add_call_to_ei PARAMS ((struct expr_info *, void *));
static void reset_down_safe PARAMS ((varref));
static void down_safety PARAMS ((struct expr_info *));
static void will_be_avail PARAMS ((struct expr_info *));
static void compute_can_be_avail PARAMS ((struct expr_info *));
static void reset_can_be_avail PARAMS ((struct expr_info *, varref));
static void compute_later PARAMS ((struct expr_info *));
static void reset_later PARAMS ((struct expr_info *, varref));
static void code_motion PARAMS ((struct expr_info *, tree));
static bool can_insert PARAMS ((varref));
static inline bool defs_match_p PARAMS ((struct expr_info *, tree, tree));
static inline bool defs_y_dom_x PARAMS ((varref, varref));
static void assign_new_class PARAMS ((varref, varray_type *));
static void insert_occ_in_preorder_dt_order_1 PARAMS ((struct expr_info *, fibheap_t, basic_block));
static void insert_occ_in_preorder_dt_order PARAMS ((struct expr_info *, fibheap_t));
static void insert_euse_in_preorder_dt_order_1 PARAMS ((struct expr_info *, fibheap_t, basic_block));
static void insert_euse_in_preorder_dt_order PARAMS ((struct expr_info *, fibheap_t));
static inline int opnum_of_phi PARAMS ((varref, int));
static varray_type phi_opnd_from_res PARAMS ((varref, int, basic_block));
static splay_tree rename_2 PARAMS ((struct expr_info *, varray_type *));
static void rename_1 PARAMS ((struct expr_info *));
static void finalize_2 PARAMS ((struct expr_info *));
static void finalize_1 PARAMS ((struct expr_info *, tree));
static varref phi_for_operand PARAMS ((struct expr_info *, varref));\
static void set_save PARAMS ((struct expr_info *, varref));
static void set_replacement PARAMS ((struct expr_info *, varref, varref));
static bool requires_edge_placement PARAMS ((varref));

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
  block = BB_FOR_STMT (stmt);
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

/* We need lexically equivalent expressions to be compared using the 
   equality function, so the hash needs to make sure they end up
   colliding.  */
static hashval_t 
hash_expr_tree (v)
     const void *v;
{
  tree t = (tree) v;
  tree op0, op1;
  hashval_t ret = 0;

  STRIP_NOPS (t);
  ret += TREE_CODE (t);
  if (TREE_CODE_CLASS (TREE_CODE (t)) != 'd')
  {
  if (TREE_OPERAND (t, 0))
    {
       op0 = TREE_OPERAND (t, 0);
       STRIP_NOPS (op0);
       ret += TREE_CODE (op0);
    }
  if (TREE_OPERAND (t, 1))
    {
      op1 = TREE_OPERAND (t, 1);
      STRIP_NOPS (op1);
      ret += TREE_CODE (op1);
    }
  }
  return ret;
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
     varref phi;
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
  if (!TEST_BIT (varphis[i], VARREF_BB (phi)->index))
    {
      varref phi_operand;
      varray_type phi_chain;
      size_t curr_phi_operand;
      SET_BIT (varphis[i], VARREF_BB (phi)->index);
      phi_chain = VARDEF_PHI_CHAIN (phi);
      for (curr_phi_operand = 0; 
           curr_phi_operand < VARRAY_ACTIVE_SIZE (phi_chain); 
           curr_phi_operand++)
        {
          phi_operand = VARRAY_GENERIC_PTR (phi_chain, curr_phi_operand);
          if (VARREF_TYPE (phi_operand) == VARPHI)
            set_var_phis (phi_operand, i);
        }
    }
}

/* Helper macro to simplify accessing the phi for a given block. */
#define phi_at_block(expr, b) VARRAY_GENERIC_PTR ((expr)->phis, pre_preorder[(b)->index])

/* Helper macro to simplify accessing a phi operand for a given block. */
#define phi_operand(phi, b) VARRAY_GENERIC_PTR (EXPRPHI_PHI_CHAIN (phi), pre_preorder[(b)->index])

/* Given a statement, find the VARDEF in it. */
static inline varref
find_def_for_stmt (stmt)
     tree stmt;
{

  struct ref_list_node *tmp;
  varref ref;

  FOR_EACH_REF (ref, tmp, TREE_REFS (stmt))
    {
      if (VARREF_TYPE (ref) == VARDEF)
        return ref;
    }
  return NULL;
}

/* Given the expression info, the real occurrence, and a variable,
   find the VARUSE of the variable  contained in that occurrence. */
static inline varref
find_varref_for_var (ei, real, var)
     struct expr_info *ei;
     tree real;
     tree var;
{
  struct ref_list_node *tmp;
  varref ref;
  size_t realnum;


  /* Find the realstmt for the real. */
  for (realnum = 0; realnum < VARRAY_ACTIVE_SIZE (ei->reals); realnum++)
    if (VARRAY_TREE (ei->reals, realnum) == real)
      break;

  /* Abort if we can't find one. */
  if (realnum == VARRAY_ACTIVE_SIZE (ei->reals))
    abort();

  /* Now look for the use of var in that statement. */
  FOR_EACH_REF (ref, tmp, TREE_REFS (VARRAY_TREE (ei->realstmts, realnum)))
    {
      if (VARREF_TYPE (ref) != VARUSE
          || VARREF_EXPR (ref) != real)
        continue;
      if (VARREF_SYM (ref) != var)
        continue;
      if (!VARUSE_CHAIN (ref))
        continue;
      return ref;
    }
  return NULL;
}
/* Determine if the definitions of variables in Y dominate the basic
   block of X. */
static inline bool 
defs_y_dom_x ( y, x)
     varref y;
     varref x;
{
  size_t i;
  

  /* It's a binary or unary expression, so it has only two operands at
     most. */

  for (i =  0; i < 2; i++)
    {
      varref ref;
      struct ref_list_node *tmp;
      if (!TREE_OPERAND (EXPRREF_EXPR (y), i))
        continue;
      FOR_EACH_REF (ref, tmp, TREE_REFS (EXPRREF_STMT (y)))
        {
          /* Find the ref for this operand. */
          if (VARREF_TYPE (ref) != VARUSE
              || VARREF_EXPR (ref) != EXPRREF_EXPR (y))
            continue;
          if (VARREF_SYM (ref) != TREE_OPERAND (EXPRREF_EXPR (y), i))
            continue;
          if (!VARUSE_CHAIN (ref))
            return false;
          if (!(a_dom_b (VARREF_BB (VARUSE_CHAIN (ref)), EXPRREF_BB (x))))
            return false;
	  break;
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
  varref use1;
  varref use2;
  struct ref_list_node *tmp;

  FOR_EACH_REF (use1, tmp, TREE_REFS (t1))
    {
      if (VARREF_TYPE (use1) != VARUSE || 
                      (TREE_CODE (VARREF_EXPR (use1)) != TREE_CODE (t2)))
        continue;
      use2 = find_varref_for_var (ei, t2, VARREF_SYM (use1));
      if (!use2 || (VARUSE_CHAIN (use2) != VARUSE_CHAIN (use1)))
        return false;
    }
  return true;
}

/* Assign a new redundancy class to the occurrence, and push it on the
   stack.  */ 
static void
assign_new_class (occ, stack)
     varref occ;
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
      varref ref = VARRAY_GENERIC_PTR (bbrefs, i);
      if (EXPRREF_BB (ref) != block)
	continue;
      if (EXPRREF_TYPE (ref) == EXPRUSE || EXPRREF_TYPE (ref) == EXPRPHI)
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
      varref newref;
      tree occurstmt = VARRAY_TREE (ei->occurstmts, i);
      varref occurref = VARRAY_GENERIC_PTR (ei->refs, i);
      if (VARREF_BB (occurref) != block)
	/*BB_FOR_STMT (occurstmt) != block)*/
        continue;
      if (TREE_CODE (VARRAY_TREE (ei->occurs, i)) == CALL_EXPR)
	{
	  newref = create_ref (VARRAY_TREE (ei->occurs, i),
				  EXPRKILL, block, occurstmt,
				  VARRAY_TREE (ei->occurs, i));
	  VARRAY_PUSH_GENERIC_PTR (ei->erefs, newref);
	  fibheap_insert (fh, preorder_count++, newref);
	}
      else
	{
	  newref = create_ref (VARRAY_TREE (ei->occurs, i), 
				  EXPRUSE, block, occurstmt, 
				  VARRAY_TREE (ei->occurs, i));
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
              varref newref = create_ref (NULL, EXPRUSE, block, 0, 0);
              varref phi = phi_at_block (ei, succ->dest);
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
     varref phi;
     int j;
{
  size_t i;
  /* We can't just count predecessors, since tree-ssa.c generates them
     when it sees a phi in the successor during it's traversal.  So the
     order is dependent on the traversal order, which i don't feel like
     duplicating. Thus the existence of VARDEF_PHI_CHAIN_BB. */
  for (i = 0 ; i < VARRAY_ACTIVE_SIZE (VARDEF_PHI_CHAIN_BB (phi)); i++)
    {
      basic_block bb = VARRAY_BB (VARDEF_PHI_CHAIN_BB (phi), i);
      if (bb->index == j)
	return i;
    }
  
  abort();
}

/* Factor variables through phi operands.  */
static varray_type
phi_opnd_from_res (Z, j, b)
     varref Z;
     int j;
     basic_block b;
{
  varray_type Q;
  varref v;
  size_t i = 0;
  struct ref_list_node *tmp;

  VARRAY_GENERIC_PTR_INIT (Q, 1, "Temp ops");

  /* b <- block containing phi that defines Z.
     Q <- copy of Z */
  FOR_EACH_REF (v, tmp, TREE_REFS (EXPRREF_STMT (Z)))
    VARRAY_PUSH_GENERIC_PTR (Q, v);

  /* For each variable v in Z */
  FOR_EACH_REF (v, tmp, TREE_REFS (EXPRREF_STMT (Z)))
    {
      /* If v is defined by phi in b */
      if (VARREF_TYPE (v) == VARUSE
          && VARREF_TYPE (VARUSE_CHAIN (v)) == VARPHI)
        {
          varref phi = VARUSE_CHAIN (v);
          if (VARREF_BB (phi) == b)
	    {
	      /* replace v in Q by j'th operand of v's phi */
              int opnum = opnum_of_phi (phi, j);
              varref op = VARRAY_GENERIC_PTR (VARDEF_PHI_CHAIN (phi), opnum);
              VARRAY_GENERIC_PTR (Q, i) = op;
            }
          
        }
      else if (VARREF_TYPE (v) == VARDEF 
               || VARREF_TYPE (v) == VARPHI
               || EXPRREF_TYPE (v) == EXPRUSE)
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
      varref phi = VARRAY_GENERIC_PTR (ei->phis, i);
      if (phi != NULL)
        bitmap_clear (EXPRPHI_PROCESSED(phi));
    }
  
  /* while (set_for_rename2 not empty) do { */
  while (VARRAY_ACTIVE_SIZE (*rename2_set) > 0)
    {
      varref Z; 
      varref phiZ;

      /* Remove real occurrence Z from set_for_rename2.  */
      Z = VARRAY_TOP_GENERIC_PTR (*rename2_set);
      VARRAY_POP (*rename2_set);

      splay_tree_insert (touched_set, (splay_tree_key) Z, 0);
      
      /* f <- phi that defines Z. */
      phiZ = EXPRUSE_DEF (Z);
      
      /* for each operand w of f do { */
      FOR_EACH_BB (block)
	{
          varref w;
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
              varref X;
	      
	      /* j <- index of w in f
   		 Y <- phi_operand_from_res (Z, j)
		 X <- def(w)  */     
              Y = phi_opnd_from_res (Z, j, EXPRREF_BB (phiZ));
              X = EXPRUSE_DEF (w);
	      
	      if (!X)
	        {
	          VARRAY_CLEAR (Y);
		  continue;
	        }
	      
	      /* if (X is a real occurrence) */
	      if (EXPRREF_TYPE (X) == EXPRUSE)
                {
                  bool match = true;
		  varref op1;
		  struct ref_list_node *tmp2;
		  /* if (all corresponding variables in Y and X have
		     the same SSA version) */
		  k = 0;
		  FOR_EACH_REF (op1, tmp2, TREE_REFS (EXPRREF_STMT (X)))
                    {
                      varref op2;
                      if (VARREF_TYPE (op1) != VARUSE)
                        continue;
                      if (VARREF_TYPE (op1) == VARUSE)
                        op1 = VARUSE_CHAIN (op1);
		      op2 = VARRAY_GENERIC_PTR (Y, k);
		      
                      if (op2 && VARREF_TYPE (op2) == VARUSE)
                        op2 = VARUSE_CHAIN (op2);
                      
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
                      varref ref = VARRAY_GENERIC_PTR (Y, k);
                      if (!ref)
                        continue;
                      if (VARREF_TYPE (ref) == VARUSE)
                        ref = VARUSE_CHAIN (ref);
                      
                      if (!a_dom_b (VARREF_BB (ref), EXPRREF_BB (X)))
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
  varref y;
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
              varref stackref = VARRAY_TOP_GENERIC_PTR (stack);
              if (EXPRREF_TYPE (stackref) == EXPRPHI)
                EXPRPHI_DOWNSAFE (stackref) = 0;
            }
          continue;
        }
      if (EXPRREF_TYPE (y) == EXPRKILL 
	  && TREE_CODE (EXPRREF_EXPR (y)) == CALL_EXPR)
        {
		if (VARRAY_ACTIVE_SIZE (stack) > 0)
            {
              varref stackref = VARRAY_TOP_GENERIC_PTR (stack);
              if (EXPRREF_TYPE (stackref) == EXPRPHI)
                EXPRPHI_DOWNSAFE (stackref) = 0;
	      VARRAY_POP_ALL (stack);
            }
          continue;       
        }
      
      /* while (Top(stack) does not dominate Y) do 
	   Pop(stack)  */
      while (VARRAY_ACTIVE_SIZE (stack) > 0)
        {
          varref stackref = VARRAY_TOP_GENERIC_PTR (stack);
          if (a_dom_b (EXPRREF_BB (stackref), EXPRREF_BB (y)))
            break;           
          VARRAY_POP (stack);
        }

      /* if (Y is an exprphi occurrence)
           assign_new_class (Y)  */
      if (EXPRREF_TYPE (y) == EXPRPHI)
        {
	  assign_new_class (y, &stack);
        }
      /* else if (Y is a real occurrence) */
      else if (EXPRREF_TYPE (y) == EXPRUSE && EXPRUSE_PHIOP (y) == 0)
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
              varref x = VARRAY_TOP_GENERIC_PTR (stack);
	      
              /* If (X is a real occurrence) */
              if (EXPRREF_TYPE (x) == EXPRUSE)
                {
                  /* If (all corresponding variables in Y and X have
                     the same SSA version) */
                  if (defs_match_p (ei, EXPRREF_STMT (y), EXPRREF_EXPR (x)))
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
                  if (EXPRREF_TYPE (x) != EXPRPHI)
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
      else if (EXPRREF_TYPE (y) == EXPRUSE && EXPRUSE_PHIOP (y) == 1)
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
              varref x = VARRAY_TOP_GENERIC_PTR (stack);
              EXPRREF_CLASS (y) = EXPRREF_CLASS (x);
              EXPRUSE_DEF (y) = x;
	      if (EXPRREF_TYPE (x) != EXPRPHI)		
		{
		  EXPRUSE_HAS_REAL_USE (y) = 1;
		  if (EXPRUSE_DEF (x) && EXPRREF_TYPE (EXPRUSE_DEF (x)) == EXPRPHI)
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
      varref ref = VARRAY_GENERIC_PTR (recheck_set, i);
      varref def;
      bool needcheck = true;
      edge succ;
      if (!ref)
	continue;
      def = EXPRUSE_DEF (ref);
      for (succ = EXPRREF_BB (ref)->succ; succ; succ = succ->succ_next)
	{
	  if (phi_at_block (ei, succ->dest) != NULL)
	    {
	      varref phi = phi_at_block (ei, succ->dest);
	      if (bitmap_bit_p (EXPRPHI_PROCESSED (phi), 
				EXPRREF_BB (ref)->index))
		needcheck = false;
	    }
	}
      if (!needcheck)
	continue;

      if (def && EXPRREF_TYPE (def) == EXPRPHI)
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
static varref
phi_for_operand (ei, operand)
     struct expr_info *ei;
     varref operand;
{
  int count = 0;
  varref retval = NULL;
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      varref phi = phi_at_block (ei, bb);
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
     varref X;
{
  basic_block bb;
  if (EXPRREF_TYPE (X) == EXPRUSE && !EXPRUSE_PHIOP (X))
    EXPRREF_SAVE (X) = 1;
  else if (EXPRREF_TYPE (X) == EXPRPHI)
    {
      int i;
      FOR_EACH_BB (bb)
	{
	  varref w = phi_operand (X, bb);
	  i = bb->index;

	  if (w && !bitmap_bit_p (EXPRPHI_PROCESSED (X), i))
	    {
	      bitmap_set_bit (EXPRPHI_PROCESSED (X), i);
	      set_save (ei, EXPRUSE_DEF (w));
	    }
	}
    }
#if EXTRANEOUS_EPHI_REMOVAL 
  if (EXPRREF_TYPE (X) == EXPRUSE && !EXPRUSE_PHIOP (X))
    {
      sbitmap idfs = compute_idfs (pre_dfs, EXPRREF_STMT (X));
      int i;
      EXECUTE_IF_SET_IN_SBITMAP (idfs, 0, i,
      {
	varref phi = phi_at_block (ei, BASIC_BLOCK (i));
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
     varref g;
     varref replacing_def;
{
  basic_block bb1;
  FOR_EACH_BB (bb1)
    {
      size_t k;
      basic_block bb2;
      varref f = phi_at_block (ei, bb1);
      if (f == NULL || !EXPRPHI_WILLBEAVAIL (f) || f == g)
	continue;
      FOR_EACH_BB (bb2)
	{
	  varref operand = phi_operand (f, bb2);
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
	  varref X = VARRAY_GENERIC_PTR (ei->erefs, k);
	  if (EXPRREF_TYPE (X) == EXPRUSE 
	      && EXPRUSE_PHIOP (X) == 0 
	      && EXPRREF_RELOAD (X)
	      && EXPRUSE_DEF (X) == g)
	    {
	      EXPRUSE_DEF (X) = replacing_def;
	    }
	}
    }
  phi_at_block (ei, EXPRREF_BB (g)) = NULL;
  
}

/* Second part of finalize algorithm.  */
static void 
finalize_2 (ei)
     struct expr_info *ei;
{
  size_t i;
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->erefs); i++)
    {
      varref ref = VARRAY_GENERIC_PTR (ei->erefs, i);
      if (EXPRREF_TYPE (ref) == EXPRPHI)
	{
	  bitmap_clear (EXPRPHI_PROCESSED (ref));
	  if (EXPRPHI_WILLBEAVAIL (ref))
	    EXPRPHI_EXTRANEOUS (ref) = 1;
	}
      else if (EXPRREF_TYPE (ref) == EXPRUSE 
	       && EXPRUSE_PHIOP (ref) == 0)
	{
	  EXPRREF_SAVE (ref) = 0;	 
	}
    }  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->erefs); i++)
    {
      varref ref = VARRAY_GENERIC_PTR (ei->erefs, i);
      if (EXPRREF_TYPE (ref) == EXPRUSE 
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
      varref ref = phi_at_block (ei, BASIC_BLOCK (i));
      if (!ref)
	continue;     
      if (EXPRPHI_WILLBEAVAIL (ref))
	{
	  int k;
	  for (k = 0; k < n_basic_blocks; k++)
	    {
	      varref w = phi_operand (ref, BASIC_BLOCK (k));
	      varref defw;
	      if (!w || !EXPRUSE_DEF (w) )
		continue;
	      defw = EXPRUSE_DEF (w);
	      if ((EXPRREF_TYPE (defw) == EXPRPHI 
		   && !EXPRPHI_EXTRANEOUS (defw))
		  || (EXPRREF_TYPE (defw) == EXPRUSE 
		      && !EXPRUSE_PHIOP (defw)))
		set_replacement (ei, ref, EXPRUSE_DEF (w));  
	    }
	}
      else
	{
	  phi_at_block (ei, EXPRREF_BB (ref)) = NULL;
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
  varref *avdefs;
  varref X;
  int x;
  fibheap_t fh;
  avdefs = xcalloc (class_count + 1,sizeof (varref));
  fh = fibheap_new ();
  insert_euse_in_preorder_dt_order (ei, fh);
  while (!fibheap_empty (fh))
    {
      X = fibheap_extract_min (fh);
      x = EXPRREF_CLASS (X);
      if (EXPRREF_TYPE (X) == EXPRPHI)
        {
          if (EXPRPHI_WILLBEAVAIL (X))
            avdefs[x] = X;
        }
      else if (EXPRREF_TYPE (X) == EXPRUSE && EXPRUSE_PHIOP (X) == 0)
        {
          if (avdefs[x] == NULL 
              || !a_dom_b (EXPRREF_BB (avdefs[x]), EXPRREF_BB (X)))
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
          varref phi = phi_for_operand (ei, X);
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
			       EXPRREF_BB (X)->index);
		      print_c_tree (dump_file, ei->expr);
		      fprintf (dump_file, " to ");
		      print_c_tree (dump_file, temp);
		      fprintf (dump_file, " after ");
		      print_c_node (dump_file, BLOCK_END_TREE (EXPRREF_BB(X)->index));
		      fprintf (dump_file, " (at end of BB), because of ExprPhi");
		      fprintf (dump_file, " in BB %d\n", 
			       EXPRREF_BB (phi)->index);
		    }
		  expr = build_modify_expr (temp, NOP_EXPR, ei->expr);
		  stmt = build_stmt (EXPR_STMT, expr);
		  bb = EXPRREF_BB (X);
		  /* If it's a goto, we need to insert *before* it.
		     This might not be necessary once goto elimination
		     is functional.  */
		  endtree = BLOCK_END_TREE (bb->index);
		  if (is_ctrl_altering_stmt (endtree))
		    insert_stmt_tree_before (stmt, endtree, bb);
		  else
		    insert_stmt_tree_after (stmt, endtree, bb);
		  
		  EXPRUSE_DEF (X) = create_ref (expr, EXPRUSE, 
						EXPRREF_BB (X), stmt, expr);
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
          varref ref;
	  struct ref_list_node *tmp;
          int varcount = 0;
	  FOR_EACH_REF (ref, tmp, TREE_REFS (VARRAY_TREE (ei->realstmts, i)))
            {
              if (VARREF_TYPE (ref) != VARUSE 
                  || VARREF_EXPR (ref) != real)
                continue;
              if (VARREF_SYM (ref) != TREE_OPERAND (real, 0)
                  && (TREE_OPERAND (real, 1) == NULL 
		      || VARREF_SYM (ref) != TREE_OPERAND (real, 1)))
                continue;
              if (VARREF_TYPE (VARUSE_CHAIN (ref)) != VARPHI)
                continue;
              set_var_phis (VARUSE_CHAIN (ref), varcount++);
            }
        }
    }
  sbitmap_a_or_b (dfphis, dfphis, varphis[0]);
  sbitmap_a_or_b (dfphis, dfphis, varphis[1]);

  EXECUTE_IF_SET_IN_SBITMAP(dfphis, 0, i, 
  {
    varref ref = create_ref (NULL, EXPRPHI, 
                                BASIC_BLOCK (i), 
                                NULL, ei->expr);
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
     varref phiop;
{
  varref phi;
  basic_block bb;
  
  if (EXPRUSE_HAS_REAL_USE (phiop))
    return;
  phi = EXPRUSE_DEF (phiop);
  if (!phi || EXPRREF_TYPE (phi) != EXPRPHI)
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
      varref phi = phi_at_block (ei, block);
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
     varref phi;
{
  edge pred;
  for (pred = VARREF_BB (phi)->pred; pred; pred = pred->pred_next)
    {
      if (pred->src != ENTRY_BLOCK_PTR)
	{
	  varref operand = phi_operand (phi, pred->src);
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
      varref phi = phi_at_block (ei, block);
      if (phi == NULL)
        continue;
      EXPRPHI_CANBEAVAIL (phi) = 1;
    }

  FOR_EACH_BB (block)
    {     
      varref phi = phi_at_block (ei, block);
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
              varref operand = phi_operand (phi, pred->src);
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
     varref phi;
{
  basic_block block;

  EXPRPHI_CANBEAVAIL (phi) = 0;
  FOR_EACH_BB (block)
    {
      edge pred;
      varref other = phi_at_block (ei, block);
      
      if (other == NULL)
        continue;
      
      for (pred = block->pred; pred; pred = pred->pred_next)
        {
          if (pred->src != ENTRY_BLOCK_PTR)
            {
              varref operand = phi_operand (other, pred->src);
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
     varref phi;
{

  basic_block block;
  EXPRPHI_LATER (phi) = 0;
  FOR_EACH_BB (block)
    {
      edge pred;
      varref other = phi_at_block (ei, block);
      
      if (other == NULL)
        continue;
      
      for (pred = block->pred; pred; pred = pred->pred_next)
        {
          if (pred->src != ENTRY_BLOCK_PTR)
            {
              varref operand = phi_operand (other, pred->src);
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
      varref phi = phi_at_block (ei, block);
      if (phi == NULL)
        continue;
      EXPRPHI_LATER (phi) = EXPRPHI_CANBEAVAIL (phi);
    }
      
  FOR_EACH_BB (block)
    {
      varref phi = phi_at_block (ei, block);
      edge pred;
      if (phi == NULL)
        continue;
      if (!EXPRPHI_LATER (phi))
        continue;

      for (pred = block->pred; pred; pred = pred->pred_next)
        {
          if (pred->src != ENTRY_BLOCK_PTR)
            {
              varref operand = phi_operand (phi, pred->src);
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
     varref op;
{
  
  varref def;
  if (EXPRUSE_DEF (op) == NULL)
    return true;
  def = EXPRUSE_DEF (op);
  if (!EXPRUSE_HAS_REAL_USE (op))
    if (EXPRREF_TYPE (def) == EXPRPHI && !(EXPRPHI_WILLBEAVAIL (def)))
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
  varref use;
  
  exprs = fibheap_new ();
  insert_euse_in_preorder_dt_order (ei, exprs);
  while (!fibheap_empty (exprs))
    {
      use = fibheap_extract_min (exprs);
      if (EXPRREF_TYPE (use) == EXPRUSE /*&& !EXPRUSE_PHIOP (use) */
	  && !EXPRREF_INSERTED (use))
	{
	  if (EXPRUSE_PHIOP (use) && (EXPRREF_RELOAD (use) || EXPRREF_SAVE (use)))
	    abort();
	  if (EXPRREF_SAVE (use))
	    {
	      basic_block use_bb = EXPRREF_BB (use);
	      tree use_stmt = EXPRREF_STMT (use);
	      tree use_expr = EXPRREF_EXPR (use);
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
	      basic_block use_bb = EXPRREF_BB (use);
	      tree use_stmt = EXPRREF_STMT (use);
	      tree use_expr = EXPRREF_EXPR (use);
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
  varref call = (varref) data;
  struct expr_info *ei = (struct expr_info *) slot;
  VARRAY_PUSH_TREE (ei->occurs, VARREF_EXPR (call));
  VARRAY_PUSH_TREE (ei->occurstmts, VARREF_STMT (call));
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
      dump_varref_array (dump_file, "", ei->refs, 0, 1);
    }
  down_safety (ei);
  will_be_avail (ei);
  if (dump_file)
    {
      fprintf (dump_file, "ExprPhi's for expression ");
      print_c_tree (dump_file, ei->expr);
      fprintf (dump_file, "\n");
      dump_varref_array (dump_file, "", ei->phis, 0, 1);
    }
  {
    varref def = NULL;
    tree type_of_expr;
    size_t i;
    for (i = 0; i < VARRAY_ACTIVE_SIZE (ei->realstmts) && !def; i++)
      def = find_def_for_stmt (VARRAY_TREE (ei->realstmts, 0));

    if (def != NULL)
      type_of_expr = TREE_TYPE (VARREF_SYM (def));
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
  /* Debugging dump before SSA PRE */
  dump_file = dump_begin (TDI_ssa_pre, &dump_flags);
  if (dump_file)
    {
      fprintf (dump_file, "\n%s()    (ORIGINAL)\n",
               IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));
      
      if (dump_flags & TDF_UNPARSE)
        print_c_tree (dump_file, fn);
      else
        dump_node (fn, TDF_SLIM | dump_flags, dump_file);
    }
  
  /* Compute immediate dominators.  */
  pre_idom = calculate_dominance_info (CDI_DOMINATORS);
  domchildren = sbitmap_vector_alloc (last_basic_block, last_basic_block);
  sbitmap_vector_zero (domchildren, last_basic_block);
  compute_domchildren (pre_idom, domchildren);
  /* Compute dominance frontiers.  */
  pre_dfs = sbitmap_vector_alloc (last_basic_block, last_basic_block);
  sbitmap_vector_zero (pre_dfs, last_basic_block);
  compute_dominance_frontiers (pre_dfs, pre_idom);

  pre_preorder = xmalloc (last_basic_block * sizeof (int));
  flow_preorder_transversal_compute (pre_preorder);
  FOR_EACH_BB (bb)
    {    
      varref ref;
      struct ref_list_node *tmp;

      htab_empty (seen);
      FOR_EACH_REF (ref, tmp, BB_REFS (bb))
	{
	  tree expr = VARREF_EXPR (ref);
	  tree stmt = VARREF_STMT (ref);
	  if (VARREF_TYPE (ref) != VARUSE)
	    continue;
	  if (htab_find (seen, expr) != NULL)
	    continue;
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
		   && TREE_CODE (VARREF_SYM (ref)) == FUNCTION_DECL)
	    {
	      if (!DECL_IS_PURE (VARREF_SYM (ref)))
	      {
		for (k = 0; k < VARRAY_ACTIVE_SIZE (bexprs); k++)
	  	  add_call_to_ei (VARRAY_GENERIC_PTR (bexprs, k), ref);
	      }
	    }  
	  *(htab_find_slot  (seen, expr, INSERT)) = expr;
	}
    }
  for (j = 0; j < VARRAY_ACTIVE_SIZE (bexprs); j++)
     pre_part_1_trav (VARRAY_GENERIC_PTR (bexprs, j), pre_dfs);
  /*  simplify_stmt (fn, NULL_TREE); */
  /* Debugging dump after SSA PRE */
  if (dump_file)
    {
      fprintf (dump_file, "\n%s()    (OPTIMIZED)\n",
               IDENTIFIER_POINTER (DECL_NAME (current_function_decl)));
      
      if (dump_flags & TDF_UNPARSE)
        print_c_tree (dump_file, fn);
      else
        dump_node (fn, TDF_SLIM | dump_flags, dump_file);
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
