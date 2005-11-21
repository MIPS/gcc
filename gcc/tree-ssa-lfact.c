/* Local factoring (code hoisting/sinking) on SSA trees.
   Copyright (C) 2004 Free Software Foundation, Inc.
 
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
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "obstack.h"
#include "basic-block.h"
#include "resource.h"
#include "ggc.h"
#include "flags.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "langhooks.h"
#include "tree-pass.h"
#include "tree-ssa-operands.h"
#include "fact-common.h"
#include "diagnostic.h"

/* This file provides the statement hoisting sinking on TREE SSA.
   The hoisting/sinking algorithm is:
  1) Collect basic blocks that have the same parents/children into sibling sets.
      e.g. block0
          if (...) { block1 } else { block2 }
      Block's 1 and 2 are in a sibling set.
  2) Try to hoist/sink statements from a sibling set.
  3) Do steps 2 for each sibling set.
  4) Repeat steps 2-3 until there is no more gain.
 
  TODO:
   * Hoist similar statements (not exactly same).
   * Move stmts with COMPONENT_REF or INDIRECT_REF on lhs/rhs
   * Move stmts with a function call on rhs containing more than one argument
     (1th stmt operand is a TREE_LIST in this case)
   * compare temporary variables (D_123_12 == D_213_56 ?? <- SSA_NAME_VAR differs)
*/

#define MAX_HTAB 1000

static int gain;

/* Local factoring - hoisting  */
static tree tree_find_first_from_begin (bb_decorator, int);
static int tree_find_insn_from_begin (tree, bb_decorator, int);
static void replace_def_vars_hoisting (bb_decorator, tree);
static int stmt_hoisting (bb_decorator);
static int tree_moveable_from_begin (basic_block, tree);
static int compare_hoisting_stmts (tree, tree);

/* Local factoring - sinking  */
static tree tree_find_first_from_end (bb_decorator, int);
static int tree_find_insn_from_end (tree, bb_decorator, int);
static void replace_def_vars_sinking ( /*bb_decorator, */ tree);
static int stmt_sinking (bb_decorator);
static int tree_moveable_from_end (basic_block, tree);
static int compare_sinking_stmts (tree, tree);
static int def_collector_phi (bb_decorator, tree, tree);
static int v_may_def_collector_phi (bb_decorator, tree, tree);
static int v_must_def_collector_phi (bb_decorator, tree, tree);

/* Common functions  */
static hashval_t lfact_iterative_hash_expr (tree, hashval_t);
static int find_stmt_in_set (htab_t *, tree);
static tree tree_find_first (bb_decorator decorator, int,
			     enum lfact_direction);
static int tree_find_insn (tree, bb_decorator, int, enum lfact_direction);
static int stmt_move (bb_decorator, enum lfact_direction);

static int tree_factoring (bb_decorator, enum lfact_direction);
static int simple_stmt (tree);
static int commutable_stmts (tree, tree);
static void get_stmts_of_bb (bb_decorator, enum lfact_direction, int flag);
static int compare_vars (tree, tree);

static void replace_use (use_operand_p, tree);
static void lfact_remove_phi_node (tree, tree, basic_block);

static void lfact_insert_stmt (basic_block, tree, enum lfact_direction);
static tree copy_and_insert_node (basic_block, tree, enum lfact_direction);

static int has_v_may_def (tree);
static int has_v_must_def (tree);
static int lhs_has_use (tree);

static tree same_defop_in_node (tree, tree);
static tree same_v_must_defop_in_node (tree, tree, int);
static tree same_v_may_defop_in_node (tree, tree, int);


static int find_abnormal_edge (bb_decorator);
static basic_block create_basic_block_by_factoring (bb_decorator,
						    basic_block);
static bb_decorator insert_new_bb_decorator (bb_decorator, basic_block);
static void delete_analogous (bb_decorator);

static bool tree_ssa_local_factoring_1 (bb_decorator, enum lfact_direction);

/* Global variables  */
static htab_t stmt_hoisting_table1;
static htab_t stmt_sinking_table1;
static htab_t *table1_p;
static htab_t stmt_hoisting_table2;
static htab_t stmt_sinking_table2;
static htab_t *table2_p;

/* Borrowed from tree.h for 'lfact_iterative_hash_expr()  */
#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<< 8); \
  c -= a; c -= b; c ^= ((b&0xffffffff)>>13); \
  a -= b; a -= c; a ^= ((c&0xffffffff)>>12); \
  b -= c; b -= a; b = (b ^ (a<<16)) & 0xffffffff; \
  c -= a; c -= b; c = (c ^ (b>> 5)) & 0xffffffff; \
  a -= b; a -= c; a = (a ^ (c>> 3)) & 0xffffffff; \
  b -= c; b -= a; b = (b ^ (a<<10)) & 0xffffffff; \
  c -= a; c -= b; c = (c ^ (b>>15)) & 0xffffffff; \
}


/* Borrowed from tree.h for 'lfact_iterative_hash_expr()' */
static inline hashval_t
lfact_iterative_hash_hashval_t (hashval_t val, hashval_t val2)
{
  /* The golden ratio; an arbitrary value.  */
  hashval_t a = 0x9e3779b9;

  mix (a, val, val2);
  return val2;
}

/* A local implementation of 'iterative_hash_expr()' from tree.h
   to produce hash values for statements containing the same
   SSA_NAMES */
static hashval_t
lfact_iterative_hash_expr (tree t, hashval_t val)
{
  int i;
  enum tree_code code;
  char class;

  if (t == NULL_TREE)
    return iterative_hash_expr (t, val);

  code = TREE_CODE (t);

  switch (code)
    {
    case INTEGER_CST:
    case REAL_CST:
    case STRING_CST:
      return iterative_hash_expr (t, val);
    case COMPLEX_CST:
      val = lfact_iterative_hash_expr (TREE_REALPART (t), val);
      return lfact_iterative_hash_expr (TREE_IMAGPART (t), val);
    case VECTOR_CST:
      return lfact_iterative_hash_expr (TREE_VECTOR_CST_ELTS (t), val);

    case SSA_NAME:
      return iterative_hash_expr (SSA_NAME_VAR (t), val);
    case VALUE_HANDLE:
      return iterative_hash_expr (t, val);

    case TREE_LIST:
      for (; t; t = TREE_CHAIN (t))
        val = lfact_iterative_hash_expr (TREE_VALUE (t), val);
      return val;
    case CONSTRUCTOR:
      {
        unsigned HOST_WIDE_INT idx;
        tree field, value;
        FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (t), idx, field, value)
          {
            val = lfact_iterative_hash_expr (field, val);
            val = lfact_iterative_hash_expr (value, val);
          }
        return val;
      }
    default:
      class = TREE_CODE_CLASS (code);

      if (class == tcc_declaration)
        {
          val = iterative_hash_expr (t, val);
        }
      else if (IS_EXPR_CODE_CLASS (class))
        {
          val = iterative_hash_object (code, val);

          if (code == NOP_EXPR
              || code == CONVERT_EXPR || code == NON_LVALUE_EXPR)
            {
              val += TYPE_UNSIGNED (TREE_TYPE (t));
              val = lfact_iterative_hash_expr (TREE_OPERAND (t, 0), val);
            }

          else if (commutative_tree_code (code))
            {
              hashval_t one =
                lfact_iterative_hash_expr (TREE_OPERAND (t, 0), 0);
              hashval_t two =
                lfact_iterative_hash_expr (TREE_OPERAND (t, 1), 0);
              hashval_t t;

              if (one > two)
                t = one, one = two, two = t;

              val = lfact_iterative_hash_hashval_t (one, val);
              val = lfact_iterative_hash_hashval_t (two, val);
            }
          else
            for (i = TREE_CODE_LENGTH (code) - 1; i >= 0; --i)
              val = lfact_iterative_hash_expr (TREE_OPERAND (t, i), val);
        }
      else
        val = iterative_hash_object (code, val);
      return val;
    }
}

/* Auxiliary functions for hashing statements.  */
static hashval_t
stmt_hash (const void *item)
{
  tree stmt = (tree) item;

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    {
      return (lfact_iterative_hash_expr (stmt, 0));
    }

  return (lfact_iterative_hash_expr (TREE_OPERAND (stmt, 1), 0));
}

/* Return 1 if P1 and P2 are equal.  */
static int
stmt_equal (const void *p1, const void *p2)
{
  tree t1 = (tree) p1;
  tree t2 = (tree) p2;

  if ((TREE_CODE (t2) != TREE_CODE (t1))
      || (TREE_TYPE (t1) != TREE_TYPE (t2)))
    return 0;

  if (commutative_tree_code (TREE_CODE (t1)))
    {
      gcc_assert (TREE_CODE_LENGTH (TREE_CODE (t1)) == 2);

      if (TREE_OPERAND (t1, 0) == NULL
	  && TREE_CODE (TREE_OPERAND (t1, 0)) == SSA_NAME
	  && (simple_cst_equal (TREE_OPERAND (t1, 0), TREE_OPERAND (t2, 0)))
	  && (simple_cst_equal (TREE_OPERAND (t1, 0), TREE_OPERAND (t2, 1))))
	return 0;

      if (TREE_OPERAND (t1, 1) == NULL
	  && TREE_CODE (TREE_OPERAND (t1, 1)) == SSA_NAME
	  && (simple_cst_equal (TREE_OPERAND (t1, 1), TREE_OPERAND (t2, 0)) !=
	      1)
	  && (simple_cst_equal (TREE_OPERAND (t1, 1), TREE_OPERAND (t2, 1)) !=
	      1))
	return 0;
    }
  else
    {
      int i;

      for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (t1)); i++)
	{
	  /* Operands must be equal.  */
	  if (simple_cst_equal (TREE_OPERAND (t1, i), TREE_OPERAND (t2, i)) !=
	      1)
	    return 0;
	}
    }


  if (stmt_hash (p1) != stmt_hash (p2))
    {
      return 0;
    }

  return 1;
}

/* Compare two vareiables and returns true if there SSA_NAME are the same.
   SSA version may differ.  */
static int
compare_vars (tree var1, tree var2)
{
  if (var1 == NULL || var2 == NULL)
    return 0;

  if ((TREE_CODE (var1) != TREE_CODE (var2))
      || (TREE_TYPE (var1) != TREE_TYPE (var2)))
    return 0;

  /* If variables are SSA_NAMEs their SSA_NAME_VAR must be equal.  */
  if (TREE_CODE (var1) == SSA_NAME)
    return (SSA_NAME_VAR (var1) == SSA_NAME_VAR (var2));

  /* If variables are VAR_DECL their DECL_NAME must be equal.  */
  if (TREE_CODE (var1) == VAR_DECL)
    return (DECL_NAME (var1) == DECL_NAME (var2));

  /* If var is an array_ref its first operand is a var_decl so we
     can compare these var_decl operands like before.  */
  if (TREE_CODE (var1) == ARRAY_REF)
    return (compare_vars (TREE_OPERAND (var1, 0), TREE_OPERAND (var2, 0)));

  /* If var is an indirect_ref its first operand is an ssa_name so we
     can compare it like before.  */
  if (TREE_CODE (var1) == INDIRECT_REF)
    return (compare_vars (TREE_OPERAND (var1, 0), TREE_OPERAND (var2, 0)));

  /* If var is a component_ref its first and second operands are ssa_names or
     var_decls so we compare these operands like before.  */
  if (TREE_CODE (var1) == COMPONENT_REF)
    return (compare_vars (TREE_OPERAND (var1, 0), TREE_OPERAND (var2, 0))
	    && compare_vars (TREE_OPERAND (var1, 1), TREE_OPERAND (var2, 1)));

  return 0;
}

/* P1 and P2 stmt may equal if they differ only in their SSA_NAME operand's
   SSA_VERSION (for example C_x1=A_y1+B_z1 == C_x2=A_y2+B_z2).  */
static int
stmt_may_equal (const void *p1, const void *p2)
{
  tree t1 = (tree) p1;
  tree t2 = (tree) p2;

  if ((TREE_CODE (t2) != TREE_CODE (t1))
      || (TREE_TYPE (t1) != TREE_TYPE (t2)))
    return 0;

  if (commutative_tree_code (TREE_CODE (t1))
      && TREE_CODE_LENGTH (TREE_CODE (t1)) == 2)
    {
      if (TREE_OPERAND (t1, 0) == NULL
	  && TREE_CODE (TREE_OPERAND (t1, 0)) == SSA_NAME
	  && !compare_vars (TREE_OPERAND (t1, 0), TREE_OPERAND (t2, 0))
	  && !compare_vars (TREE_OPERAND (t1, 0), TREE_OPERAND (t2, 1)))
	return 0;

      if (TREE_OPERAND (t1, 1) == NULL
	  && TREE_CODE (TREE_OPERAND (t1, 1)) == SSA_NAME
	  && !compare_vars (TREE_OPERAND (t1, 1), TREE_OPERAND (t2, 0))
	  && !compare_vars (TREE_OPERAND (t1, 1), TREE_OPERAND (t2, 1)))
	return 0;
    }
  else
    {
      int i;

      for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (t1)); i++)
	{
	  tree op1 = TREE_OPERAND (t1, i);
	  tree op2 = TREE_OPERAND (t2, i);

	  if ((op1 != NULL) && TREE_CODE (op1) == SSA_NAME)
	    {
	      if (!compare_vars (op1, op2))
		return 0;
	    }
	  else
	    {
	      /* If operands aren't SSA_NAMEs they must be equal.  */
	      if (simple_cst_equal (op1, op2) != 1)
		return 0;
	    }
	}
    }

  if (stmt_hash (p1) != stmt_hash (p2))
    {
      return 0;
    }

  return 1;
}

/* Looking for STMT node in the TABLE.  */
static int
find_stmt_in_set (htab_t * table, tree stmt)
{
  if (TREE_CODE (stmt) == MODIFY_EXPR
      && htab_find (*table, TREE_OPERAND (stmt, 1)))
    return 1;
  return 0;
}

/* Collection of the statments, which are included by all basic blocks
   chained from decorator.  The DECORATOR is the first element of the
   sibling set, and we can travel the whole set by the help of the
   'next_sibling' field of bb_decorator.  In the result *table1_p
   contains the common statements of the investigated sibling set.
 
   if flag is set it collects statements which are included by at least
   two basic blocks of the sibling set. */
static void
get_stmts_of_bb (bb_decorator decorator, enum lfact_direction alg, int flag)
{
  basic_block bb = decorator->curr;
  bb_decorator di;
  block_stmt_iterator si;
  void **slot;
  int i = 0;

  if (alg == LFD_HOISTING)
    {
      table1_p = &stmt_hoisting_table1;
      table2_p = &stmt_hoisting_table2;
    }
  else
    {
      table1_p = &stmt_sinking_table1;
      table2_p = &stmt_sinking_table2;
    }

  htab_empty (*table1_p);
  htab_empty (*table2_p);

  /* Collection of the statements of the first block.  */
  for (si = (alg == LFD_HOISTING) ? bsi_start (bb) : bsi_last (bb);
       !bsi_end_p (si);
       (alg == LFD_HOISTING) ? bsi_next (&si) : bsi_prev (&si))
    {
      tree stmt = bsi_stmt (si);
      if (simple_stmt (stmt))
	{
	  if (!htab_find (*table1_p, TREE_OPERAND (stmt, 1)))
	    {
	      slot =
		htab_find_slot (*table1_p, TREE_OPERAND (stmt, 1), INSERT);
	      *slot = TREE_OPERAND (stmt, 1);
	    }
	}
    }

  /* Collection of the common statements of the next block and till now
     investigated blocks.  */
  for (di = decorator->next_sibling; di; di = di->next_sibling)
    {
      htab_t *temp_table;
      i++;
      for (si =
	   (alg == LFD_HOISTING) ? bsi_start (di->curr) : bsi_last (di->curr);
	   !bsi_end_p (si);
	   (alg == LFD_HOISTING) ? bsi_next (&si) : bsi_prev (&si))
	{
	  tree stmt = bsi_stmt (si);
	  if (simple_stmt (stmt))
	    {
	      /* If we find a stmt, which is included by the table pointed by
	         table1_p, we insert it into the other table.  */
	      if (htab_find (*table1_p, TREE_OPERAND (stmt, 1)))
		{
		  slot =
		    htab_find_slot (*table2_p, TREE_OPERAND (stmt, 1),
				    INSERT);
		  *slot = TREE_OPERAND (stmt, 1);
		}
	      else if (flag)
		{
		  slot =
		    htab_find_slot (*table1_p, TREE_OPERAND (stmt, 1),
				    INSERT);
		  *slot = TREE_OPERAND (stmt, 1);
		}
	    }
	}
      if (!flag)
	{
	  htab_empty (*table1_p);
	  temp_table = table1_p;
	  table1_p = table2_p;
	  table2_p = temp_table;
	}
    }

  if (flag)
    {
      htab_t *temp_table;

      htab_empty (*table1_p);
      temp_table = table1_p;
      table1_p = table2_p;
      table2_p = temp_table;
    }

}

/* It returns with true, if stmt has any v_must_def operand.  */
static int
has_v_must_def (tree stmt)
{
  return !ZERO_SSA_OPERANDS (stmt, SSA_OP_VMUSTDEF);
}

/* It returns with true, if stmt has any v_may_def operand.  */
static int
has_v_may_def (tree stmt)
{
  return !ZERO_SSA_OPERANDS (stmt, SSA_OP_VMAYDEF);
}

/* Returns with true if if lhs of stmt has any use operand.  */
static int
lhs_has_use (tree stmt)
{
  return TREE_CODE_LENGTH (TREE_CODE (stmt)) > 1;
}


/* If the STMT1 and STMT2 nodes are commutable, this function returns
   with 1.  */
static int
commutable_stmts (tree stmt1, tree stmt2)
{
  ssa_op_iter op_iter;
  imm_use_iterator use_iter;
  use_operand_p use_p;
  tree var;

  /* Now we suppose that a CALL_EXPR is not moveable.  */
  if (TREE_CODE (stmt1) == CALL_EXPR)
    return 0;

  FOR_EACH_SSA_TREE_OPERAND (var, stmt1, op_iter, SSA_OP_ALL_DEFS)
    FOR_EACH_IMM_USE_FAST (use_p, use_iter, var)
    if (simple_cst_equal (USE_STMT (use_p), stmt2) == 1)
    return 0;
  return 1;
}

/* Auxiliary function for 'tree_find_first_from_begin' and
   'tree_find_insn_from_begin'.  */
static int
tree_moveable_from_begin (basic_block bb, tree stmt)
{
  block_stmt_iterator si;
  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    {
      tree stmt2 = bsi_stmt (si);

      if (simple_cst_equal (stmt2, stmt) == 1)
	return 1;
      if (!commutable_stmts (stmt2, stmt))
	return 0;
    }
  return 0;
}

/* Auxiliary function for 'tree_find_first_from_end' and
   'tree_find_insn_from_end'.  */
static int
tree_moveable_from_end (basic_block bb, tree stmt)
{
  block_stmt_iterator si;
  for (si = bsi_last (bb); !bsi_end_p (si); bsi_prev (&si))
    {
      tree stmt2 = bsi_stmt (si);
      if (simple_cst_equal (stmt2, stmt) == 1)
	return 1;
      if (!commutable_stmts (stmt, stmt2))
	return 0;
    }

  return 0;
}

/* Common function for calling tree_find_first_from_begin and
   tree_find_first_from_end.  */
static tree
tree_find_first (bb_decorator decorator, int max_depth,
		 enum lfact_direction alg)
{
  return (alg == LFD_HOISTING ?
	  tree_find_first_from_begin (decorator, max_depth) :
	  tree_find_first_from_end (decorator, max_depth));
}

/* Common function for calling tree_find_insn_from_begin and
   tree_find_insn_from_end.  */
static int
tree_find_insn (tree insn,
		bb_decorator decorator,
		int max_depth, enum lfact_direction alg)
{
  return (alg == LFD_HOISTING ?
	  tree_find_insn_from_begin (insn, decorator, max_depth) :
	  tree_find_insn_from_end (insn, decorator, max_depth));
}

/* Common function for calling stmt_hoisting and stmt_sinking.  */
static int
stmt_move (bb_decorator decorator, enum lfact_direction alg)
{
  return (alg == LFD_HOISTING ?
	  stmt_hoisting (decorator) : stmt_sinking (decorator));
}

/* Finding the first hoistable stmt of the basic block of DECORATOR. If this
   stmt is existing, the moveable_node field of decorator will be set. With
   parameter MAX_DEPTH we can modulate the depth of the search.  If MAX_DEPTH
   is 0, there is no restriction.  */
static tree
tree_find_first_from_begin (bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  block_stmt_iterator bsi = bsi_start (bb);
  tree stmt = NULL;
  int depth = 0;

  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    {
      stmt = bsi_stmt (bsi);

      if (depth < decorator->last_depth)
	{
	  depth++;
	  continue;
	}

      if (TREE_CODE (stmt) == RETURN_EXPR || is_ctrl_stmt (stmt))
	{
	  /* STMT is not moveable.  */
	  depth++;
	  decorator->last_depth = depth;
	  return (NULL_TREE);
	}

      if (!find_stmt_in_set (table1_p, stmt))
	{
	  /* STMT is not a potantial hoistable statement.  */
	  depth++;
	  decorator->last_depth = depth;
	  continue;
	}

      if (tree_moveable_from_begin (bb, stmt)
	  && commutable_stmts (stmt,
			       bsi_stmt (bsi_last (EDGE_PRED (bb, 0)->src))))
	{
	  /* STMT is moveable.  */
	  decorator->moveable_node = bsi;
	  return stmt;
	}

      depth++;
      decorator->last_depth = depth;

      if (depth == max_depth - 1)
	return (NULL_TREE);
    }
  if (bsi_end_p (bsi))
    return (NULL_TREE);
  return stmt;
}

/* Finding the first hoistable stmt of the basic block of DECORATOR. If this
   stmt is existing, the moveable_node field of decorator will be set. With
   parameter MAX_DEPTH we can modulate the depth of the search.  If MAX_DEPTH
   is 0, there is no restriction.  */
static tree
tree_find_first_from_end (bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  block_stmt_iterator bsi = bsi_last (bb);
  tree stmt = NULL;
  int depth = 0;

  for (bsi = bsi_last (bb); !bsi_end_p (bsi); bsi_prev (&bsi))
    {
      stmt = bsi_stmt (bsi);

      if (depth < decorator->last_depth)
	{
	  depth++;
	  continue;
	}

      if ((TREE_CODE (stmt) == RETURN_EXPR)
	  || (!find_stmt_in_set (table1_p, stmt)) || is_ctrl_stmt (stmt))
	{
	  depth++;
	  decorator->last_depth = depth;
	  continue;
	}

      if (tree_moveable_from_end (bb, stmt))
	{
	  decorator->moveable_node = bsi;
	  return stmt;
	}

      depth++;
      decorator->last_depth = depth;

      if (depth == max_depth - 1)
	return (NULL_TREE);
    }
  if (bsi_end_p (bsi))
    return (NULL_TREE);
  return stmt;
}

static int
defs_equal (tree stmt1, tree stmt2)
{
  def_operand_p def1_p, def2_p;
  ssa_op_iter iter1, iter2;

  if (NUM_SSA_OPERANDS (stmt1, SSA_OP_DEF) !=
      NUM_SSA_OPERANDS (stmt2, SSA_OP_DEF))
    return 0;

  FOR_EACH_SSA_DEF_OPERAND (def1_p, stmt1, iter1, SSA_OP_DEF)
  {
    int same = 0;
    FOR_EACH_SSA_DEF_OPERAND (def2_p, stmt2, iter2, SSA_OP_DEF)
    {
      if (compare_vars (DEF_FROM_PTR (def1_p), DEF_FROM_PTR (def2_p)))
	same++;
    }
    if (same != 1)
      return 0;
  }

  return 1;
}

/* Compares v_may_defs of two statements and returns true if the rhs of
   v_may_defs are equal. if mayflag is true then it compares v_may_defs using
   compare_vars(); */
static int
v_may_defs_equal (tree stmt1, tree stmt2, int mayflag)
{
  use_operand_p use1_p, use2_p;
  ssa_op_iter iter1, iter2;

  if (NUM_SSA_OPERANDS (stmt1, SSA_OP_VMAYDEF) !=
      NUM_SSA_OPERANDS (stmt2, SSA_OP_VMAYDEF))
    return 0;

  FOR_EACH_SSA_USE_OPERAND (use1_p, stmt1, iter1, SSA_OP_VMAYUSE)
  {
    int same = 0;
    FOR_EACH_SSA_USE_OPERAND (use2_p, stmt2, iter2, SSA_OP_VMAYUSE)
    {
      if (USE_FROM_PTR (use1_p) == USE_FROM_PTR (use2_p)
	  || (mayflag
	      && compare_vars (USE_FROM_PTR (use1_p), USE_FROM_PTR (use2_p))))
	same++;
    }
    if (same != 1)
      return 0;
  }

  return 1;
}

/* Compares v_must_defs of two statements and returns true if the rhs of
   v_must_defs are equal. if mayflag is true then it compares v_must_defs using
   compare_vars().  */
static int
v_must_defs_equal (tree stmt1, tree stmt2, int mayflag)
{
  use_operand_p use_p1, use_p2;
  ssa_op_iter iter1, iter2;

  FOR_EACH_SSA_USE_OPERAND (use_p1, stmt1, iter1, SSA_OP_VMUSTKILL)
  {
    int same = 0;
    FOR_EACH_SSA_USE_OPERAND (use_p2, stmt2, iter2, SSA_OP_VMUSTKILL)
    {
      if (USE_FROM_PTR (use_p1) == USE_FROM_PTR (use_p2)
	  || (mayflag
	      && compare_vars (USE_FROM_PTR (use_p1), USE_FROM_PTR (use_p2))))
	same++;
    }
    if (same != 1)
      return 0;
  }

  return 1;
}

/* The equality between assignment statements is the following: if the rhs of
   the statements are equal, and the lhs of the stamtements are derived from
   the same variable, the statements are equal. In other cases the equality
   relation is the familiar equality. It returns with 1, if STMT1 and STMT2
   are equal. */
static int
compare_hoisting_stmts (tree stmt1, tree stmt2)
{
  if (TREE_CODE (stmt1) != TREE_CODE (stmt2))
    return 0;

  if (TREE_CODE (stmt1) == MODIFY_EXPR)
    {
      if (!stmt_equal (TREE_OPERAND (stmt1, 1), TREE_OPERAND (stmt2, 1)))
	return 0;

      if (!compare_vars (TREE_OPERAND (stmt1, 0), TREE_OPERAND (stmt2, 0)))
	return 0;

      if (!defs_equal (stmt1, stmt2) || !v_must_defs_equal (stmt1, stmt2, 0)
	  || !v_may_defs_equal (stmt1, stmt2, 0))
	return 0;

      /* If the node has any use operand on the left side these operand
         must be the same too.  */
      if (lhs_has_use (stmt1) && lhs_has_use (stmt2))
	{
	  size_t i;
	  for (i = 0;
	       i < TREE_CODE_LENGTH (TREE_CODE (TREE_OPERAND (stmt1, 0)));
	       i++)
	    {
	      if (simple_cst_equal
		  (TREE_OPERAND (TREE_OPERAND (stmt1, 0), i),
		   TREE_OPERAND (TREE_OPERAND (stmt2, 0), i)) != 1)
		return 0;
	    }
	}

      return 1;
    }
  else
    return (simple_cst_equal (stmt1, stmt2));
}

static int
used_by_phi (tree phi, tree var)
{
  int j;

  if (TREE_CODE (phi) != PHI_NODE)
    return 0;

  for (j = 0; j < PHI_NUM_ARGS (phi); j++)
    {
      if (PHI_ARG_DEF (phi, j) == var)
	return 1;
    }

  return 0;
}

static int
last_use_of_defop (bb_decorator first, basic_block bb, tree orig_stmt)
{
  def_operand_p def_p;
  ssa_op_iter iter;

  tree phi;

  FOR_EACH_SSA_DEF_OPERAND (def_p, orig_stmt, iter, SSA_OP_DEF)
  {
    tree var = DEF_FROM_PTR (def_p);
    int usenum = 0;
    for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
      {
	if (used_by_phi (phi, var))
	  {
	    if (!def_collector_phi (first, phi, var))
	      return 0;
	    ++usenum;
	  }

	if (usenum > 1)
	  return 0;
      }
  }

  FOR_EACH_SSA_DEF_OPERAND (def_p, orig_stmt, iter, SSA_OP_VMUSTDEF)
  {
    tree var = DEF_FROM_PTR (def_p);
    int usenum = 0;

    for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
      {
	if (used_by_phi (phi, var))
	  {
	    if (!v_must_def_collector_phi (first, phi, var))
	      return 0;
	    ++usenum;
	  }

	if (usenum > 1)
	  return 0;
      }
  }

  FOR_EACH_SSA_DEF_OPERAND (def_p, orig_stmt, iter, SSA_OP_VMAYDEF)
  {
    tree var = DEF_FROM_PTR (def_p);
    int usenum = 0;

    for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
      {
	if (used_by_phi (phi, var))
	  {
	    if (!v_may_def_collector_phi (first, phi, var))
	      return 0;
	    ++usenum;
	  }

	if (usenum > 1)
	  return 0;
      }
  }
  return 1;
}


/* Auxiliary function for 'compare_sinking_stmt()' */
static int
last_def_of_useops (tree orig_stmt)
{
  basic_block bb = bb_for_stmt (orig_stmt);
  block_stmt_iterator bsi;

  for (bsi = bsi_last (bb); !bsi_end_p (bsi); bsi_prev (&bsi))
    {
      tree stmt = bsi_stmt (bsi);

      if (!simple_cst_equal (orig_stmt, stmt))
	{
	  use_operand_p use_p;
	  def_operand_p def_p;
	  ssa_op_iter iter0, iter1;

	  FOR_EACH_SSA_USE_OPERAND (use_p, orig_stmt, iter0, SSA_OP_USE)
	  {
	    FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, iter1, SSA_OP_DEF)
	    {
	      if (simple_cst_equal
		  (DEF_FROM_PTR (def_p), USE_FROM_PTR (use_p)) == 1)
		return 0;

	    }
	  }
	}
      else
	{
	  return 1;
	}
    }
  return 1;
}

/* STMT1 and STMT2 statements may equal if the their lhs are derived from the
   same variable and if the rhs of the statements contains the same ssa names
   for variables. the defined (lhs) variables are equal if they aren't used
   again later and the use_ops are the same if they aren't defined again
   later.  */
static int
compare_sinking_stmts (tree stmt1, tree stmt2)
{
  if (TREE_CODE (stmt1) != TREE_CODE (stmt2))
    return 0;

  if (TREE_CODE (stmt1) == MODIFY_EXPR)
    {
      if (!stmt_may_equal (TREE_OPERAND (stmt1, 1), TREE_OPERAND (stmt2, 1)))
	return 0;

      if (!compare_vars (TREE_OPERAND (stmt1, 0), TREE_OPERAND (stmt2, 0)))
	return 0;

      if (!defs_equal (stmt1, stmt2) || !v_must_defs_equal (stmt1, stmt2, 1)
	  || !v_may_defs_equal (stmt1, stmt2, 1))
	return 0;

      /* If the node has any use operand on the left side these operand
         must have the same ssa_name too.  */
      if (lhs_has_use (stmt1) && lhs_has_use (stmt2))
	{
	  size_t i;
	  for (i = 0;
	       i < TREE_CODE_LENGTH (TREE_CODE (TREE_OPERAND (stmt1, 0)));
	       i++)
	    {
	      tree op1 = TREE_OPERAND (TREE_OPERAND (stmt1, 0), i);
	      tree op2 = TREE_OPERAND (TREE_OPERAND (stmt2, 0), i);

	      if ((op1 != NULL) && TREE_CODE (op1) == SSA_NAME)
		{
		  if (!compare_vars (op1, op2))
		    return 0;
		}
	      else
		{
		  /* If operands aren't SSA_NAMEs they must be equal.  */
		  if (simple_cst_equal (op1, op2) != 1)
		    return 0;
		}
	    }
	}

      if (last_def_of_useops (stmt1) && last_def_of_useops (stmt2))
	return 1;
    }

  return (simple_cst_equal (stmt1, stmt2) == 1);
}


/* Finding INSN in the basic block pointed by decorator->curr. If we are find
   such insn in the basic block, the decorator->moveable_node will be set, and
   if it hoistable, too, the function returns with 1.  */
static int
tree_find_insn_from_begin (tree insn, bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  block_stmt_iterator bsi;
  int depth = 0;

  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    {
      tree stmt = bsi_stmt (bsi);

      if (compare_hoisting_stmts (insn, stmt))
	{
	  decorator->moveable_node = bsi;
	  if (!tree_moveable_from_begin (bb, stmt))
	    {
	      return 0;
	    }
	  return 1;
	}
      depth++;
      if (depth == max_depth)
	return 0;
    }
  return 0;
}

/* Finding INSN in the basic block pointed by decorator->curr. If we are find
   such insn in the basic block, the decorator->moveable_node will be set, and
   if it sinkable, too, the function returns with 1.  */
static int
tree_find_insn_from_end (tree insn, bb_decorator decorator, int max_depth)
{
  basic_block bb = decorator->curr;
  block_stmt_iterator bsi;
  int depth = 0;

  for (bsi = bsi_last (bb); !bsi_end_p (bsi); bsi_prev (&bsi))
    {
      tree stmt = bsi_stmt (bsi);
      if (compare_sinking_stmts (insn, stmt))
	{
	  decorator->moveable_node = bsi;
	  if (!tree_moveable_from_end (bb, stmt))
	    {
	      return 0;
	    }
	  return 1;
	}
      depth++;
      if (depth == max_depth)
	return 0;
    }
  return 0;
}

/* Try to move only the simple statements.
   A statement is simple if it's a MODIFY expr and the lhs contains only one
   SSA_NAME without any v_may_def or v_must_def or the lhs is a VAR_DECL or an
   ARRAY_REF with at least one v_may_def or v_must_def. */
static int
simple_stmt (tree stmt)
{
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return 0;

  if (TREE_CODE (TREE_OPERAND (stmt, 0)) == SSA_NAME)
    return 1;

  if (((TREE_CODE (TREE_OPERAND (stmt, 0)) == VAR_DECL)
       || (TREE_CODE (TREE_OPERAND (stmt, 0)) == ARRAY_REF)
       || (TREE_CODE (TREE_OPERAND (stmt, 0)) == INDIRECT_REF)
       || (TREE_CODE (TREE_OPERAND (stmt, 0)) == COMPONENT_REF))
      && ((has_v_must_def (stmt)) || (has_v_may_def (stmt))))
    return 1;

  return 0;
}

/* Auxiliary function for 'replace_def_vars' function.  */
static tree
same_defop_in_node (tree def, tree new_node)
{
  def_operand_p def_p;
  ssa_op_iter iter;

  FOR_EACH_SSA_DEF_OPERAND (def_p, new_node, iter, SSA_OP_DEF)
    if (compare_vars (DEF_FROM_PTR (def_p), def))
    return DEF_FROM_PTR (def_p);

  return NULL_TREE;
}

/* Auxiliary function for 'replace_def_vars' function.  */
static tree
same_v_must_defop_in_node (tree mustdef_op, tree node, int mayflag)
{
  use_operand_p use_p;
  def_operand_p def_p;
  ssa_op_iter iter;

  FOR_EACH_SSA_MUSTDEF_OPERAND (def_p, use_p, node, iter)
  {
    if (simple_cst_equal (USE_FROM_PTR (use_p), mustdef_op))
      return DEF_FROM_PTR (def_p);
    if (mayflag && compare_vars (USE_FROM_PTR (use_p), mustdef_op))
      return DEF_FROM_PTR (def_p);
  }

  return NULL_TREE;
}

/* Auxiliary function for 'replace_def_vars' function.  */
static tree
same_v_may_defop_in_node (tree maydef_op, tree node, int mayflag)
{
  use_operand_p use_p;
  def_operand_p def_p;
  ssa_op_iter iter;

  FOR_EACH_SSA_MAYDEF_OPERAND (def_p, use_p, node, iter)
  {
    if (simple_cst_equal (USE_FROM_PTR (use_p), maydef_op))
      return DEF_FROM_PTR (def_p);
    if (mayflag && compare_vars (USE_FROM_PTR (use_p), maydef_op))
      return DEF_FROM_PTR (def_p);
  }

  return NULL_TREE;
}

/* Replaces use operand to a NEW_DEF, and sets up abnormal phi if needed.  */
static void
replace_use (use_operand_p use_p, tree new_def)
{
  if (TREE_CODE (USE_FROM_PTR (use_p)) != TREE_CODE (new_def))
    return;

  if ((TREE_CODE (USE_STMT (use_p)) == PHI_NODE) &&
      (TREE_CODE (USE_FROM_PTR (use_p)) == SSA_NAME)
      && SSA_NAME_OCCURS_IN_ABNORMAL_PHI (USE_FROM_PTR (use_p)))
    {
      SET_USE (use_p, new_def);
      SSA_NAME_OCCURS_IN_ABNORMAL_PHI (new_def) = 1;
    }
  else
    {
      SET_USE (use_p, new_def);
    }
}

/* We must rename the decent use operand of the immediate uses of the hoisted
   assignment statement. Starting from FIRST, we can travel the whole
   sibling set. NEW_NODE is the newly created node.   */
static void
replace_def_vars_hoisting (bb_decorator first, tree new_node)
{
  bb_decorator di;

  for (di = first; di; di = di->next_sibling)
    {
      tree stmt = bsi_stmt (di->moveable_node);
      imm_use_iterator iter;
      use_operand_p use_p;
      def_operand_p def_p;
      ssa_op_iter ssa_iter;

      /* If we find use operand equal to the current hoisted satement's defined
         operand we look for it's new ssa name in the NEW_NODE - using
         same_*_defop_in_node - and replace the use operand to the new name
         stored in global_tree.  */
      FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, ssa_iter, SSA_OP_DEF)
      {
	tree old_def = DEF_FROM_PTR (def_p);
	tree new_def = same_defop_in_node (old_def, new_node);

	FOR_EACH_IMM_USE_SAFE (use_p, iter, old_def)
	  replace_use (use_p, new_def);
      }

      FOR_EACH_SSA_MUSTDEF_OPERAND (def_p, use_p, stmt, ssa_iter)
      {
	tree old_def = DEF_FROM_PTR (def_p);
	tree new_def =
	  same_v_must_defop_in_node (USE_FROM_PTR (use_p), new_node, 0);
	use_operand_p use_p2;

	FOR_EACH_IMM_USE_SAFE (use_p2, iter, old_def)
	  replace_use (use_p2, new_def);
      }

      FOR_EACH_SSA_MAYDEF_OPERAND (def_p, use_p, stmt, ssa_iter)
      {
	tree old_def = DEF_FROM_PTR (def_p);
	tree new_def =
	  same_v_may_defop_in_node (USE_FROM_PTR (use_p), new_node, 0);
	use_operand_p use_p2;

	FOR_EACH_IMM_USE_SAFE (use_p2, iter, old_def)
	  replace_use (use_p2, new_def);
      }
    }
}

/* Inserts a node to the begining or to the end of BB depending on the
   current direction.  */
static void
lfact_insert_stmt (basic_block bb, tree stmt, enum lfact_direction alg)
{
  block_stmt_iterator pos;
  if (alg == LFD_SINKING)
    {
      pos = bsi_start (bb);
      bsi_insert_after (&pos, stmt, BSI_NEW_STMT);
    }
  else
    {
      pos = bsi_last (bb);
      bsi_insert_before (&pos, stmt, BSI_NEW_STMT);
    }
}

/* Copy the moveable node 'stmt' and insert it to 'bb'.  */
static tree
copy_and_insert_node (basic_block bb, tree stmt, enum lfact_direction alg)
{
  tree new_node;
  def_operand_p def_p;
  ssa_op_iter iter;

  /* copy the statement */
  new_node = unshare_expr (stmt);

  lfact_insert_stmt (bb, new_node, alg);
  copy_virtual_operands (new_node, stmt);

  /* If def_ops are SSA_NAMEs we use new version for these becouse we may copy
     the node more than once.  */
  FOR_EACH_SSA_DEF_OPERAND (def_p, new_node, iter,
			    (SSA_OP_DEF | SSA_OP_VMUSTDEF | SSA_OP_VMAYDEF))
    if (TREE_CODE (DEF_FROM_PTR (def_p)) == SSA_NAME)
    {
      SET_DEF (def_p, duplicate_ssa_name (DEF_FROM_PTR (def_p), NULL));
      SSA_NAME_DEF_STMT (DEF_FROM_PTR (def_p)) = new_node;
    }

  return new_node;
}

static tree
defined_by_phi (basic_block bb, tree var)
{
  tree phi;
  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      if (PHI_RESULT (phi) == var)
	return phi;
    }

  return NULL_TREE;
}

/* If there is a hoistable stmt, then all moveable_node fields of bb_decorator
   of the sibling set members are set. We must create new node(s), and modify
   the immediate uses, and remove this stmts.  */
static int
stmt_hoisting (bb_decorator first)
{
  bb_decorator di;
  int i;
  tree list = NULL, *last = &list, node;

  for (i = 0; i < first->num_pred_bb; i++)
    {
      edge e = EDGE_PRED (first->curr, i);
      basic_block bb = e->src;
      tree stmt = bsi_stmt (first->moveable_node);
      tree new_node;

      /* copy and insert the new node */
      new_node = copy_and_insert_node (bb, stmt, LFD_HOISTING);
      gain--;

      /* chain new nodes to a stmt list for later uses */
      node = build_tree_list (new_node, stmt);
      *last = node;
      last = &TREE_CHAIN (node);

    }

  /* If a use operand is defined by a phi node in the basicblock the statement
     is hoisted from then we have to replace the same use operand of the new node
     to the variable defined by this phi.  */
  for (di = first; di; di = di->next_sibling)
    {
      tree stmt = bsi_stmt (first->moveable_node);

      /* Do this check for each hoisted stmt.  */
      for (node = list; node; node = TREE_CHAIN (node))
	{
	  tree new_node = TREE_PURPOSE (node);
	  edge ed = find_edge (bb_for_stmt (new_node), di->curr);
	  use_operand_p stmt_use_p, new_use_p;
	  ssa_op_iter stmt_iter, new_iter;

	  /* We iterate the new node's use operands and the old statements'
	     together so we can replace the same use operand of the new
	     node. */
	  for ((stmt_use_p =
		op_iter_init_use (&stmt_iter, stmt,
				  SSA_OP_USE | SSA_OP_VUSE | SSA_OP_VMAYUSE |
				  SSA_OP_VMUSTKILL), new_use_p =
		op_iter_init_use (&new_iter, new_node,
				  SSA_OP_USE | SSA_OP_VUSE | SSA_OP_VMAYUSE |
				  SSA_OP_VMUSTKILL));
	       (!op_iter_done (&stmt_iter), !op_iter_done (&new_iter));
	       (stmt_use_p = op_iter_next_use (&stmt_iter), new_use_p =
		op_iter_next_use (&new_iter)))
	    {
	      tree var = USE_FROM_PTR (stmt_use_p);
	      tree phi = defined_by_phi (di->curr, var);

	      if (phi != NULL_TREE)
		SET_USE (new_use_p, PHI_ARG_DEF (phi, ed->dest_idx));
	    }
	}

    }

  /* if there are more parent basic blocks for hoistable statements in the
     basic blocks of the sibling set we have to create new phi nodes for
     new defined variables otherewise it's enough to replace the immediate
     uses for it */
  if (first->num_pred_bb > 1)
    {
      for (di = first; di; di = di->next_sibling)
	{
	  basic_block bb = di->curr;
	  tree stmt = bsi_stmt (di->moveable_node);
	  def_operand_p def_p;
	  ssa_op_iter iter;

	  /* creating phi for def ops */
	  FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, iter, SSA_OP_DEF)
	  {
	    tree defop = DEF_FROM_PTR (def_p);
	    tree phi = create_phi_node (defop, bb);
	    SSA_NAME_DEF_STMT (defop) = phi;

	    for (node = list; node; node = TREE_CHAIN (node))
	      {
		tree newnode = TREE_PURPOSE (node);
		tree newvar = same_defop_in_node (defop, newnode);
		basic_block parent_bb = bb_for_stmt (newnode);
		edge ed = find_edge (parent_bb, bb);

		add_phi_arg (phi, newvar, ed);
	      }
	  }

	  /* Creating phi for vmust_def ops.  */
	  FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, iter, SSA_OP_VMUSTDEF)
	  {
	    tree defop = DEF_FROM_PTR (def_p);
	    tree phi = create_phi_node (defop, bb);
	    SSA_NAME_DEF_STMT (defop) = phi;

	    for (node = list; node; node = TREE_CHAIN (node))
	      {
		tree newnode = TREE_PURPOSE (node);
		tree newvar = same_v_must_defop_in_node (defop, newnode, 0);
		basic_block parent_bb = bb_for_stmt (newnode);
		edge ed = find_edge (parent_bb, bb);

		add_phi_arg (phi, newvar, ed);
	      }
	  }

	  /* Creating phi for vmay_defops.  */
	  FOR_EACH_SSA_DEF_OPERAND (def_p, stmt, iter, SSA_OP_VMAYDEF)
	  {
	    tree defop = DEF_FROM_PTR (def_p);
	    tree phi = create_phi_node (defop, bb);
	    SSA_NAME_DEF_STMT (defop) = phi;

	    for (node = list; node; node = TREE_CHAIN (node))
	      {
		tree newnode = TREE_PURPOSE (node);
		tree newvar = same_v_may_defop_in_node (defop, newnode, 0);
		basic_block parent_bb = bb_for_stmt (newnode);
		edge ed = find_edge (parent_bb, bb);

		add_phi_arg (phi, newvar, ed);
	      }
	  }

	}
    }
  else
    {
      tree new_node = TREE_PURPOSE (list);
      replace_def_vars_hoisting (first, new_node);
    }

  if (dump_file)
    {
      for (node = list; node; node = TREE_CHAIN (node))
	{
	  tree new_node = TREE_PURPOSE (node);
	  basic_block bb = bb_for_stmt (new_node);
	  fprintf (dump_file, "STMT HOISTING:\n");
	  for (di = first; di; di = di->next_sibling)
	    {
	      fprintf (dump_file, " orig stmt (in bb %d): ", di->curr->index);
	      print_generic_stmt (dump_file, bsi_stmt (di->moveable_node),
				  TDF_SLIM);
	    }
	  fprintf (dump_file, " new stmt (in bb %d): ", bb->index);
	  print_generic_stmt (dump_file, new_node, TDF_SLIM);
	  fprintf (dump_file, "\n");
	}
    }

  /* Remove old statements.  */
  for (di = first; di; di = di->next_sibling)
    {
      gain++;
      bsi_remove (&di->moveable_node);
    }

  return 1;
}

/* Replaces the def vars of sinked statements to the new def vars.  */
static void
replace_def_vars_sinking (tree treelist)
{
  tree node;

  for (node = treelist; node; node = TREE_CHAIN (node))
    {
      tree newvar = TREE_PURPOSE (node);
      tree oldvar = TREE_VALUE (node);

      imm_use_iterator use_iter;
      use_operand_p use_p;

      FOR_EACH_IMM_USE_SAFE (use_p, use_iter, oldvar)
	replace_use (use_p, newvar);
    }
}


/* Like the original 'remove_phi_node()' in 'tree-phinodes.c' but it doesn't
   release the phi result.  */
void
lfact_remove_phi_node (tree phi, tree prev, basic_block bb)
{
  if (prev)
    {
      PHI_CHAIN (prev) = PHI_CHAIN (phi);
      release_phi_node (phi);
    }
  else if (phi == phi_nodes (bb))
    {
      bb->phi_nodes = PHI_CHAIN (phi);
      release_phi_node (phi);
    }
  else
    {
      tree prev, t;
      prev = NULL_TREE;
      for (t = phi_nodes (bb); t && t != phi; t = PHI_CHAIN (t))
	prev = t;
      if (t)
	lfact_remove_phi_node (t, prev, bb);
    }
}

/* A collector phi is a phi node that doesn't do any optimalization but only
   collects the variables with same ssa names from the parent basic blocks
   especially from the sibling set.
   So this function checks if phi node 'phi' collects def_ops from moveable
   nodes of siblingset 'first' with the same ssa_name of defvar.  */
static int
def_collector_phi (bb_decorator first, tree phi, tree defvar)
{
  basic_block bb = bb_for_stmt (phi);
  bb_decorator di;
  size_t j;

  if (TREE_CODE (phi) != PHI_NODE || !bb)
    return 0;

  if (!compare_vars (PHI_RESULT (phi), defvar))
    return 0;

  for (j = 0; j < (unsigned) PHI_NUM_ARGS (phi); j++)
    {
      int same = 0;

      for (di = first; di; di = di->next_analogous)
	{
	  tree stmt = bsi_stmt (di->moveable_node);
	  edge e = find_edge (di->curr, bb);
	  tree var = same_defop_in_node (defvar, stmt);

	  if ((PHI_ARG_DEF (phi, j) == var && PHI_ARG_EDGE (phi, j) == e))
	    ++same;
	}

      if (same > 1)
	{
	  return 0;
	}
      else if (same == 0)
	{
	  /* If there's no matching defop from moveable statements for a phi
	     argument this phi argument must be from a nonanalogous basicblock of
	     the sibling set. */
	  for (di = first; di; di = di->next_sibling)
	    {
	      bb_decorator di2;
	      edge e = find_edge (di->curr, bb);
	      int analogous = 0;

	      for (di2 = first; di2; di2 = di2->next_analogous)
		if (di == di2)
		  analogous = 1;

	      if (PHI_ARG_EDGE (phi, j) == e && analogous)
		return 0;
	    }
	}
    }

  return 1;
}

/* Like def_collector_phi().  */
static int
v_may_def_collector_phi (bb_decorator first, tree phi, tree defvar)
{
  basic_block bb = bb_for_stmt (phi);
  bb_decorator di;
  size_t j;

  if (TREE_CODE (phi) != PHI_NODE || !bb)
    return 0;

  if (!compare_vars (PHI_RESULT (phi), defvar))
    return 0;

  for (j = 0; j < (unsigned) PHI_NUM_ARGS (phi); j++)
    {
      int same = 0;

      for (di = first; di; di = di->next_analogous)
	{
	  tree stmt = bsi_stmt (di->moveable_node);
	  edge e = find_edge (di->curr, bb);
	  tree var = same_v_may_defop_in_node (defvar, stmt, 1);

	  if ((PHI_ARG_DEF (phi, j) == var && PHI_ARG_EDGE (phi, j) == e))
	    ++same;
	}

      if (same > 1)
	{
	  return 0;
	}
      else if (same == 0)
	{
	  /* If there's no matching defop from moveable statements for a phi
	     argument this phi argument must be from a nonanalogous basicblock of
	     the sibling set.  */
	  for (di = first; di; di = di->next_sibling)
	    {
	      bb_decorator di2;
	      edge e = find_edge (di->curr, bb);
	      int analogous = 0;

	      for (di2 = first; di2; di2 = di2->next_analogous)
		if (di == di2)
		  analogous = 1;

	      if (PHI_ARG_EDGE (phi, j) == e && analogous)
		return 0;
	    }
	}
    }

  return 1;
}

/* Like def_collector_phi().  */
static int
v_must_def_collector_phi (bb_decorator first, tree phi, tree defvar)
{
  basic_block bb = bb_for_stmt (phi);
  bb_decorator di;
  size_t j;

  if (TREE_CODE (phi) != PHI_NODE || !bb)
    return 0;

  if (!compare_vars (PHI_RESULT (phi), defvar))
    return 0;

  for (j = 0; j < (unsigned) PHI_NUM_ARGS (phi); j++)
    {
      int same = 0;

      for (di = first; di; di = di->next_analogous)
	{
	  tree stmt = bsi_stmt (di->moveable_node);
	  edge e = find_edge (di->curr, bb);
	  tree var = same_v_must_defop_in_node (defvar, stmt, 1);

	  if ((PHI_ARG_DEF (phi, j) == var && PHI_ARG_EDGE (phi, j) == e))
	    ++same;
	}

      if (same > 1)
	{
	  return 0;
	}
      else if (same == 0)
	{
	  /* If there's no matching defop from moveable statements for a phi
	     argument this phi argument must be from a nonanalogous basicblock of
	     the sibling set.  */
	  for (di = first; di; di = di->next_sibling)
	    {
	      bb_decorator di2;
	      edge e = find_edge (di->curr, bb);
	      int analogous = 0;

	      for (di2 = first; di2; di2 = di2->next_analogous)
		if (di == di2)
		  analogous = 1;

	      if (PHI_ARG_EDGE (phi, j) == e && analogous)
		return 0;
	    }
	}
    }

  return 1;
}


/* Collects use ops from sinked statements and creates a new phi node for
   these operands. The return value is this phi node.  */
static tree
create_phi_for_use (bb_decorator first, basic_block bb, tree useop, int flags)
{
  bb_decorator di;
  tree newop;
  tree phi;

  /* Create the new phi node.  */
  phi = create_phi_node (useop, bb);
  newop = make_ssa_name (SSA_NAME_VAR (useop), phi);
  SSA_NAME_DEF_STMT (newop) = phi;
  SET_PHI_RESULT (phi, newop);

  for (di = first; di; di = di->next_analogous)
    {
      edge ed = find_edge (di->curr, bb);
      tree oldstmt = bsi_stmt (di->moveable_node);
      use_operand_p use_p;
      ssa_op_iter iter;

      FOR_EACH_SSA_USE_OPERAND (use_p, oldstmt, iter, flags)
      {
	int isinphi = 0;
	int l = 0;

	if (!compare_vars (USE_FROM_PTR (use_p), useop))
	  continue;

	/* We have to check if this operand is already added.  */
	while (!isinphi && l < PHI_NUM_ARGS (phi))
	  {
	    if ((simple_cst_equal
		 (PHI_ARG_DEF (phi, l), USE_FROM_PTR (use_p)) == 1)
		&& (PHI_ARG_EDGE (phi, l) == ed))
	      isinphi = 1;
	    l++;
	  }

	if (!isinphi)
	  add_phi_arg (phi, USE_FROM_PTR (use_p), ed);
      }

    }

  return phi;
}

/* sinking a statement has the following steps:
   for any following basic blocks:
   1. copy a sinkable node and insert it to it's proper place this node will be
      "new_node"
   2. looking for phi nodes collecting the defops,v_may_defops,v_must_defops of
      the statements
   3. if a phi node exists,
      3a. rename the new_node's v_may/v_must/defop to this phi nodes phi_result
      3b. remove the phi node
   4. creating phi nodes for use and vuse operands to collect the uses/vuses
      of the sinked statements
   5. creating phi nodes for v_may/v_must_def operands
   6. recompute immedieate uses
   after it:
   7. replace immediate uses of the old defops from sinked statements to the
      new defops
   8. remove old statements
   */

static int
stmt_sinking (bb_decorator first)
{
  int i;
  bb_decorator di;
  tree list = NULL, *last = &list, node;

  for (i = 0; i < first->num_succ_bb; i++)
    {
      edge e = EDGE_SUCC (first->curr, i);
      basic_block bb = e->dest;
      tree new_node;
      tree stmt = bsi_stmt (first->moveable_node);
      use_operand_p use_p;
      def_operand_p def_p;
      ssa_op_iter iter;

      /* Copy the first sinkable node and insert to it's proper place.  */
      new_node = copy_and_insert_node (bb, stmt, LFD_SINKING);
      gain--;

      /* A phi node may exist to collect the def ops of the moveable nodes.
         if this phi node exist we should remove it and replace the new
         node's def_ops/v_must_def_ops/v_may_def_ops to phi result.  */

      FOR_EACH_SSA_DEF_OPERAND (def_p, new_node, iter, SSA_OP_DEF)
      {
	tree def = DEF_FROM_PTR (def_p);
	tree phi = NULL_TREE;

	if (TREE_CODE (def) != SSA_NAME)
	  continue;

	/* Looking for the phi node which collects the def ops of the
	   moveable statements.  */
	for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	  {
	    if (def_collector_phi (first, phi, def))
	      break;
	  }
	/* If this phi node exists we replace new_node's defop and remove
	   phi.  */
	if (phi)
	  {
	    SET_DEF (def_p, PHI_RESULT (phi));
	    lfact_remove_phi_node (phi, NULL, bb);
	    SSA_NAME_DEF_STMT (PHI_RESULT (phi)) = new_node;
	  }

	/* Collect new defops to a list. later we replace immediate uses of
	   old defop using this list.  */
	for (di = first; di; di = di->next_analogous)
	  {
	    tree sinked_stmt = bsi_stmt (di->moveable_node);
	    tree newvar = DEF_FROM_PTR (def_p);
	    tree oldvar = same_defop_in_node (newvar, sinked_stmt);

	    node = build_tree_list (newvar, oldvar);
	    *last = node;
	    last = &TREE_CHAIN (node);
	  }
      }

      /* Do the same for v_may_defs like before.  */
      FOR_EACH_SSA_DEF_OPERAND (def_p, new_node, iter, SSA_OP_VMAYDEF)
      {
	tree mdef = DEF_FROM_PTR (def_p);
	tree phi = NULL_TREE;

	if (TREE_CODE (mdef) != SSA_NAME)
	  continue;

	/* Looking for the phi node which collects the v_may_def ops of
	   the moveable statements.  */
	for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	  {
	    if (v_may_def_collector_phi (first, phi, mdef))
	      break;
	  }

	/* If this phi node exists we replace new_node's defop and remove
	   phi. */
	if (phi)
	  {
	    SET_DEF (def_p, PHI_RESULT (phi));
	    lfact_remove_phi_node (phi, NULL, bb);
	    SSA_NAME_DEF_STMT (PHI_RESULT (phi)) = new_node;
	  }

	/* Collect new defops to a list. later we replace immediate uses of
	   old defop using this list.  */
	for (di = first; di; di = di->next_analogous)
	  {
	    tree sinked_stmt = bsi_stmt (di->moveable_node);
	    tree newvar = DEF_FROM_PTR (def_p);
	    tree oldvar = same_v_may_defop_in_node (newvar, sinked_stmt, 1);

	    node = build_tree_list (newvar, oldvar);
	    *last = node;
	    last = &TREE_CHAIN (node);
	  }
      }

      /* Do the same for v_must_defs like before.  */
      FOR_EACH_SSA_DEF_OPERAND (def_p, new_node, iter, SSA_OP_VMUSTDEF)
      {
	tree mdef = DEF_FROM_PTR (def_p);
	tree phi = NULL_TREE;

	if (TREE_CODE (mdef) != SSA_NAME)
	  continue;

	/* Looking for the phi node which collects the v_must_def ops
	   of the moveable statements.  */
	for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	  if (v_must_def_collector_phi (first, phi, mdef))
	    break;

	/* If this phi node exists we replace new_node's defop and remove
	   phi. */
	if (phi)
	  {
	    SET_DEF (def_p, PHI_RESULT (phi));
	    lfact_remove_phi_node (phi, NULL, bb);
	    SSA_NAME_DEF_STMT (PHI_RESULT (phi)) = new_node;
	  }

	/* Collect new defops to a list. later we replace immediate uses of
	   old defop using this list.  */
	for (di = first; di; di = di->next_analogous)
	  {
	    tree sinked_stmt = bsi_stmt (di->moveable_node);
	    tree newvar = DEF_FROM_PTR (def_p);
	    tree oldvar = same_v_must_defop_in_node (newvar, sinked_stmt, 1);

	    node = build_tree_list (newvar, oldvar);
	    *last = node;
	    last = &TREE_CHAIN (node);
	  }
      }

      /* Create new phi node for sinked stmt's uses.  */
      FOR_EACH_SSA_USE_OPERAND (use_p, new_node, iter, SSA_OP_USE)
      {
	tree use_var = USE_FROM_PTR (use_p);

	/* If use operand appears more then once (a=b*b) we create only one
	   phi node for it.  */
	if ((TREE_CODE (use_var) == SSA_NAME)
	    && (!defined_by_phi (bb, use_var)))
	  {
	    tree phi = create_phi_for_use (first, bb, use_var, SSA_OP_USE);
	    SET_USE (use_p, PHI_RESULT (phi));
	  }
      }

      /* Create new phi node for sinked stmt's vuses.  */
      FOR_EACH_SSA_USE_OPERAND (use_p, new_node, iter, SSA_OP_VUSE)
      {
	tree use_var = USE_FROM_PTR (use_p);

	/* If use operand appears more then once (a=b*b) we create only one
	   phi node for it.  */
	if ((TREE_CODE (use_var) == SSA_NAME)
	    && (!defined_by_phi (bb, use_var)))
	  {
	    tree phi = create_phi_for_use (first, bb, use_var, SSA_OP_VUSE);
	    SET_USE (use_p, PHI_RESULT (phi));
	  }
      }

      /* We must create phi nodes for vmust/may_defs too.
         sinking a vmust/may_def will look sg like:

         before sinking:
         <L0>:;
         ..
         # a_22 = V_MUST_DEF <a_15>;
         a=sg.;
         goto <L2>;
         <L1>:;
         ..
         # a_23 = V_MUST_DEF <a_16>;
         a=sg.;
         <L2>:;
         ....

         after sinking:
         <L0>:;
         ...
         <L1>:;
         ...
         # a_24 = PHI <a_15(1), a_16(2)>;
         <L2>:;
         # a_25 = V_MUST_DEF <a_24>;
         a=sg.
       */

      FOR_EACH_SSA_MUSTDEF_OPERAND (def_p, use_p, new_node, iter)
      {
	tree use_var = USE_FROM_PTR (use_p);
	if (TREE_CODE (use_var) == SSA_NAME)
	  {
	    tree phi =
	      create_phi_for_use (first, bb, use_var, SSA_OP_VMUSTKILL);

	    SET_USE (use_p, PHI_RESULT (phi));
	  }
      }

      /* The same for v_may_defs like boefore for v_must_defs.  */
      FOR_EACH_SSA_MAYDEF_OPERAND (def_p, use_p, new_node, iter)
      {
	tree use_var = USE_FROM_PTR (use_p);
	if (TREE_CODE (use_var) == SSA_NAME)
	  {
	    tree phi =
	      create_phi_for_use (first, bb, use_var, SSA_OP_VMAYUSE);

	    SET_USE (use_p, PHI_RESULT (phi));
	  }
      }

      /* Ensure that the PHI node chain is in the correct order.  */
      set_phi_nodes (bb, phi_reverse (phi_nodes (bb)));

      if (dump_file)
	{
	  fprintf (dump_file, "STMT SINKING:\n");
	  for (di = first; di; di = di->next_analogous)
	    {
	      fprintf (dump_file, " orig stmt (in bb %d): ", di->curr->index);
	      print_generic_stmt (dump_file, bsi_stmt (di->moveable_node),
				  TDF_SLIM);
	    }
	  fprintf (dump_file, " new stmt (in bb %d): ", bb->index);
	  print_generic_stmt (dump_file, new_node, TDF_SLIM);
	  fprintf (dump_file, "\n");
	}

    }

  /* After sinking the statements, in next basicblocks of the cfg may remain
     statement still using the def vars of the old statements. we have to
     replace these uses to the new ones.  */
  replace_def_vars_sinking (list);

  /* Remove old statements.  */
  for (di = first; di; di = di->next_analogous)
    {
      gain++;
      bsi_remove (&di->moveable_node);
    }

  return 1;
}

/* It is not safe to modify the destination of edge of a bb, if this edge
   is ABNORMAL.  */
static int
find_abnormal_edge (bb_decorator first)
{
  int num_of_not_abnormal_succ = 0;
  bb_decorator di;
  for (di = first; di; di = di->next_analogous)
    {
      if (EDGE_SUCC (di->curr, 0)->flags & EDGE_ABNORMAL)
	{
	  if (di->prev_analogous)
	    di->prev_analogous->next_analogous = di->next_analogous;
	  if (di->next_analogous)
	    di->next_analogous->prev_analogous = di->prev_analogous;
	  di->prev_analogous = di->next_analogous = NULL;
	}
      else
	num_of_not_abnormal_succ++;
    }
  return (num_of_not_abnormal_succ > 1 ? 1 : 0);
}

/* Deletion of the 'analogous' relations from FIRST.  */
void
delete_analogous (bb_decorator first)
{
  bb_decorator last = NULL;
  bb_decorator di;
  for (di = first; di; di = di->next_analogous)
    {
      if (last)
	last->next_analogous = NULL;
      last = di;
      di->prev_analogous = NULL;
    }
}

/* Like ssa_redirect_edge(), it redirects an edge to the destination
   basicblock and removes appropiate phi arguments from destination phi nodes.
   the removed phi arguments are chained one by one to PENDING_STMT list.  */
static edge
lfact_redirect_edge (edge e, basic_block dest)
{
  tree phi;
  tree list = NULL, *last = &list;
  tree src, dst, node;

  for (phi = phi_nodes (e->dest); phi; phi = PHI_CHAIN (phi))
    {
      if (PHI_ARG_DEF (phi, e->dest_idx) == NULL_TREE)
	continue;

      src = PHI_ARG_DEF (phi, e->dest_idx);
      dst = phi;
      node = build_tree_list (dst, src);
      *last = node;
      last = &TREE_CHAIN (node);
    }

  /* Remove phi arguments associated with edge and redirect it.  */
  e = redirect_edge_and_branch (e, dest);
  PENDING_STMT (e) = list;

  return e;
}

/* Flush the pending statements of edges pointing to NEW_BB. flushing pending
   statements means that we create new phi nodes at the begining of NEW_BB and
   modify old ones. after all we clean PENDING_STMT for each edge.  */
static void
lfact_flush_pending_stmts (basic_block new_bb)
{
  edge e;
  edge_iterator ei;
  edge new_edge_to = EDGE_SUCC (new_bb, 0);
  tree phi;

  /* Ee collect removed arguments of the phi nodes and create new phi node in
     new_bb for these.  */
  for (phi = phi_nodes (new_edge_to->dest); phi; phi = PHI_CHAIN (phi))
    {
      tree new_phi = NULL_TREE;

      /* PENDING_STMT is a TREE_LIST with the phi node the variable is
         removed from as purpose and with the variable as value. so we walk
         over this tree list for eache edge and if a variable is removed from
         the current phi we add this variable to the new phi node.  */
      FOR_EACH_EDGE (e, ei, new_bb->preds)
      {
	tree list;

	if (!PENDING_STMT (e))
	  continue;

	for (list = PENDING_STMT (e); list; list = TREE_CHAIN (list))
	  {
	    if (TREE_PURPOSE (list) != phi)
	      continue;

	    if (new_phi == NULL_TREE)
	      new_phi =
		create_phi_node (duplicate_ssa_name
				 (PHI_RESULT (TREE_PURPOSE (list)), NULL),
				 new_bb);
	    SSA_NAME_DEF_STMT (PHI_RESULT (new_phi)) = new_phi;

	    add_phi_arg (new_phi, TREE_VALUE (list), e);
	  }
      }

      if (new_phi != NULL_TREE)
	add_phi_arg (phi, PHI_RESULT (new_phi), new_edge_to);
    }

  /* Clean the stmt list.  */
  FOR_EACH_EDGE (e, ei, new_bb->preds) PENDING_STMT (e) = NULL;
}

/* Creation of a new basic block for sinking and sinking same stmts into it
   the new basic block will be insertend between 'first' bb_decorator's
   current analogous basic blocks and between 'next_bb'.  */
static basic_block
create_basic_block_by_factoring (bb_decorator first, basic_block next_bb)
{
  edge new_edge_to, e;
  edge_iterator ei;

  bb_decorator di;

  basic_block prev = next_bb->prev_bb;
  basic_block new_bb;

  /* Create a new empty basicblock.  */
  new_bb = create_empty_bb (prev);
  new_edge_to = make_single_succ_edge (new_bb, next_bb, EDGE_FALLTHRU);
  gain--;

  for (di = first; di; di = di->next_analogous)
    {
      basic_block bb = di->curr;

      FOR_EACH_EDGE (e, ei, bb->succs)
      {
	if (e->dest != next_bb)
	  continue;

	e = lfact_redirect_edge (e, new_bb);
	gcc_assert (e);

      }
    }

  /* After redirecting an edge we must modify the
     phi nodes in the destination and the source
     basicblock. for example:
     <s1>:
     ... goto dst
     <s2>:
     ... goto dst
     # c_5 = PHI<c_1(s1), c_2(s2), c_3(sg)>
     <dst>:
     ....

     should be:
     <s1>:
     ... goto new
     <s2>:
     ... goto new
     # c_4 = PHI<c_1(s1), c2(s2)>
     <new>:
     ... goto dst
     # c_5 = PHI<c_4(new), c3(sg)>
     <dst>:
     ...

     this is done by flushing the pending stmts.  */

  lfact_flush_pending_stmts (new_bb);

  /* Ensure that the PHI node chain is in the correct order.  */
  set_phi_nodes (new_bb, phi_reverse (phi_nodes (new_bb)));

  /* Update the dominators.  */
  if (dom_info_available_p (CDI_DOMINATORS))
    {
      basic_block dom;

      for (di = first; di; di = di->next_analogous)
	{
	  dom = recount_dominator (CDI_DOMINATORS, di->curr);
	  set_immediate_dominator (CDI_DOMINATORS, di->curr, dom);
	}

      dom = recount_dominator (CDI_DOMINATORS, new_bb);
      set_immediate_dominator (CDI_DOMINATORS, new_bb, dom);

      dom = recount_dominator (CDI_DOMINATORS, next_bb);
      set_immediate_dominator (CDI_DOMINATORS, next_bb, dom);
    }

  if (dump_file)
    fprintf (dump_file, "new block %d created for sinking.\n", new_bb->index);

  return new_bb;
}

static bb_decorator
insert_new_bb_decorator (bb_decorator decorator, basic_block new_bb)
{
  bb_decorator last_di;
  bb_decorator new_di = NULL;

  last_di = decorator;
  while (last_di->next_decorator)
    last_di = last_di->next_decorator;

  new_di = xcalloc (1, sizeof (struct bb_decorator_def));
  new_di->num_pred_bb = EDGE_COUNT (new_bb->preds);
  new_di->num_succ_bb = EDGE_COUNT (new_bb->succs);
  new_di->curr = new_bb;
  new_di->prev_decorator = last_di;
  last_di->next_decorator = new_di;

  return new_di;
}

/* If we insert a new basic block, the relations between the basic blocks will
   be changed, so we must modifying some concatenation.  */
static void
ssa_modifying_concatenation (bb_decorator first)
{
  bb_decorator di;
  bb_decorator last_analogous = first;
  bb_decorator last_nonanalogous = first->prev_sibling;

  if (first->prev_sibling)
    {
      (first->prev_sibling)->next_sibling = NULL;
      first->prev_sibling = NULL;
    }

  for (di = first->next_sibling; di; di = di->next_sibling)
    {
      if (di == last_analogous->next_analogous)
	{

	  last_analogous->next_sibling = di;
	  di->prev_sibling = last_analogous;
	  last_analogous = di;
	}
      else
	{
	  if (last_nonanalogous)
	    last_nonanalogous->next_sibling = di;
	  di->prev_sibling = last_nonanalogous;
	  last_nonanalogous = di;
	}
    }

  if (last_nonanalogous)
    last_nonanalogous->next_sibling = NULL;

  if (last_analogous)
    last_analogous->next_sibling = NULL;

  delete_analogous (first);
}


static int
check_next_bb_phis (bb_decorator first)
{
  bb_decorator di;
  int i;

  for (i = 0; i < first->num_succ_bb; i++)
    {
      basic_block bb = EDGE_SUCC (first->curr, i)->dest;

      for (di = first; di; di = di->next_analogous)
	{
	  if (!last_use_of_defop (first, bb, bsi_stmt (di->moveable_node)))
	    return 0;
	}
    }

  return 1;
}

static int
tree_factoring_sinking (bb_decorator decorator, enum lfact_direction alg)
{
  bb_decorator di = decorator;
  bb_decorator siblings;
  int change = 0;
  int walkover = 0;

  while (di)
    {
      di->last_depth = 0;
      di = di->next_decorator;
    }

  di = decorator;
  siblings = di;

  while (di)
    {
      if (siblings->next_sibling && (walkover = 1 || siblings->prev_sibling))
	{
	  bb_decorator di2;
	  tree insn = NULL;
	  unsigned long num_of_next_bb = siblings->num_succ_bb;
	  unsigned long num_of_good_bb = 0;
	  int insert_bb = 0;
	  bb_decorator last_sibling;
	  if (siblings->prev_sibling)
	    {
	      insert_bb = 1;
	      walkover = 1;
	    }
	  else
	    {
	      get_stmts_of_bb (siblings, alg, 1);
	    }

	  insn = tree_find_first (siblings, 0, alg);

	  if (!insn)
	    {
	      siblings = siblings->next_sibling;
	      continue;
	    }

	  last_sibling = siblings;
	  num_of_good_bb = 1;

	  for (di2 = siblings->next_sibling; di2; di2 = di2->next_sibling)
	    {
	      if (tree_find_insn (insn, di2, 0, alg))
		{
		  last_sibling->next_analogous = di2;
		  di2->prev_analogous = last_sibling;
		  last_sibling = di2;
		  num_of_good_bb++;
		}
	    }

	  if (last_sibling)
	    last_sibling->next_analogous = NULL;

	  if (num_of_good_bb <= num_of_next_bb)
	    {
	      siblings->last_depth++;
	      delete_analogous (siblings);
	      continue;
	    }

	  if (!insert_bb && EDGE_SUCC (siblings->curr, 0))
	    {
	      if (num_of_good_bb <
		  EDGE_COUNT ((EDGE_SUCC (siblings->curr, 0))->dest->preds))
		insert_bb = 1;
	    }

	  /* Sinking a statement by creating a new basic block for it needs an
	     extra goto. so it doesn't worth sinking if the number of sinkable
	     statements is less then two times the number of basic blocks we
	     should create.  */
	  if (insert_bb && (num_of_good_bb <= num_of_next_bb * 2))
	    {
	      siblings->last_depth++;
	      delete_analogous (siblings);
	      continue;
	    }

	  /* If we sink statements without inserting any new basic block for it
	     we should check the destination block's phi nodes. if any of this
	     phi nodes uses a defop of the sinkable statements and this phi
	     node is not a 'collector_phi' (which only collects the variables
	     from the parent blocks) then the statements are not sinkable.
	     if we create a basic block for sinking we build the phi nodes too
	     so at this point this checking is not necesarry.  */
	  if (!check_next_bb_phis (siblings))
	    {
	      siblings->last_depth++;
	      delete_analogous (siblings);
	      continue;
	    }


	  if (!insert_bb && siblings)
	    {
	      /* These statements are in a full sibling set.  */
	      if (stmt_move (siblings, alg))
		change = 1;
	      continue;
	    }
	  else
	    {
	      bb_decorator new_bb_decorator;

	      if (optimize_size && find_abnormal_edge (siblings))
		{
		  int i = 0;

		  for (i = 0; i < siblings->num_succ_bb; i++)
		    {
		      basic_block next_bb =
			EDGE_SUCC (siblings->curr, i)->dest;
		      basic_block new_bb =
			create_basic_block_by_factoring (siblings, next_bb);

		      new_bb_decorator =
			insert_new_bb_decorator (decorator, new_bb);
		    }

		  if (stmt_move (siblings, alg))
		    change = 1;

		  ssa_modifying_concatenation (siblings);
		}
	      else
		siblings->last_depth++;
	    }

	  delete_analogous (siblings);
	}
      else if (siblings->next_sibling)
	{
	  siblings = siblings->next_sibling;
	}
      else
	{
	  di = di->next_decorator;
	  siblings = di;
	  walkover = 0;
	}
    }

  return change;
}


static int
tree_factoring_hoisting (bb_decorator decorator, enum lfact_direction alg)
{
  bb_decorator di = decorator;
  int change = 0;
  int not_moveable = 1;

  while (di)
    {
      di->last_depth = 0;
      di = di->next_decorator;
    }

  di = decorator;

  while (di)
    {
      if ((!di->prev_sibling) && (di->next_sibling))
	{
	  bb_decorator di2;
	  tree insn = NULL;

	  if (not_moveable)
	    {
	      get_stmts_of_bb (di, alg, 0);
	      not_moveable = 0;
	    }

	  insn = tree_find_first (di, 0, alg);

	  if (!insn)
	    {
	      not_moveable = 1;
	      di = di->next_decorator;
	      continue;
	    }

	  for (di2 = di->next_sibling; di2; di2 = di2->next_sibling)
	    {
	      if (!tree_find_insn (insn, di2, 0, alg))
		{
		  not_moveable = 1;
		  di->last_depth++;
		  break;
		}
	    }

	  if (!not_moveable && di)
	    {
	      if (stmt_move (di, alg))
		change = 1;
	    }
	}
      else
	di = di->next_decorator;
    }
  return change;
}

/* Main function of local factoring.  */
static int
tree_factoring (bb_decorator decorator, enum lfact_direction alg)
{
  if (alg == LFD_SINKING)
    return tree_factoring_sinking (decorator, alg);
  else
    return tree_factoring_hoisting (decorator, alg);
}

/* Call the local factoring algorithms in a parameter direction.  */
static bool
tree_ssa_local_factoring_1 (bb_decorator decorator, enum lfact_direction alg)
{
  gain = 0;
  if (alg == LFD_HOISTING)
    collect_full_sibling (decorator, alg);
  else
    collect_family_sibling (decorator, alg);

  cost_analyzer (decorator, alg);

  if (dump_file)
    dump_siblings (dump_file, decorator, alg);

  while (tree_factoring (decorator, alg))
    ;

  if (dump_file && gain)
    fprintf (dump_file, "After %s the gain is %d instruction\n",
	     alg == LFD_HOISTING ? "hoisting" : "sinking", gain);
  return (gain > 0);
}

/* Main function of local factoring.  */
void
tree_ssa_local_factoring (void)
{
  bool has_gain = true;
  bb_decorator decorator = NULL;
  stmt_sinking_table1 =
    htab_create (MAX_HTAB, stmt_hash, stmt_may_equal, NULL);
  stmt_sinking_table2 =
    htab_create (MAX_HTAB, stmt_hash, stmt_may_equal, NULL);
  stmt_hoisting_table1 = htab_create (MAX_HTAB, stmt_hash, stmt_equal, NULL);
  stmt_hoisting_table2 = htab_create (MAX_HTAB, stmt_hash, stmt_equal, NULL);
  decorator = init_factoring (decorator);

  if (dump_file)
    dump_cfg_info (dump_file);

  while (has_gain)
    {
      has_gain = tree_ssa_local_factoring_1 (decorator, LFD_SINKING);
      has_gain = tree_ssa_local_factoring_1 (decorator, LFD_HOISTING)
	|| has_gain;
    }

  free_bb_decorator_list (decorator);
  htab_delete (stmt_hoisting_table1);
  htab_delete (stmt_hoisting_table2);
  htab_delete (stmt_sinking_table1);
  htab_delete (stmt_sinking_table2);
}

/* The gate of local factoring.  */
bool
is_tree_lfact (void)
{
  return ((flag_tree_lfact == 1) ? 1 : 0);
}

struct tree_opt_pass pass_tree_lfact = {
  "lfact",			/* name */
  is_tree_lfact,		/* gate */
  tree_ssa_local_factoring,	/* execute */
  NULL,				/* sub */
  NULL,				/* next */
  0,				/* static_pass_number */
  0,				/* tv_id */
  0,				/* properties_required */
  0,				/* properties_provided */
  0,				/* properties_destroyed */
  0,				/* todo_flags_start */
  TODO_dump_func,		/* todo_flags_finish */
  0				/* letter */
};
