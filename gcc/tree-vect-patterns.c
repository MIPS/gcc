/* Analysis Utilities for Loop Vectorization.
   Copyright (C) 2005 Free Software Foundation, Inc.
   Contributed by Dorit Nuzman <dorit@il.ibm.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "ggc.h"
#include "tree.h"

#include "target.h"
#include "basic-block.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "timevar.h"
#include "cfgloop.h"
#include "expr.h"
#include "optabs.h"
#include "params.h"
#include "tree-data-ref.h"
#include "tree-vectorizer.h"
#include "recog.h"
#include "toplev.h"

/* Funcion prototypes */
static void vect_pattern_recog_1 
  (tree (* ) (tree, tree *, tree *, varray_type *), block_stmt_iterator);
static bool widened_name_p (tree, tree, tree *, tree *);

/* Pattern recognition functions  */
_recog_func_ptr vect_pattern_recog_funcs[NUM_PATTERNS] = {
        vect_recog_unsigned_subsat_pattern,
	vect_recog_widen_mult_pattern,
	vect_recog_mult_hi_pattern,
	vect_recog_widen_sum_pattern,
	vect_recog_dot_prod_pattern,
	vect_recog_sad_pattern};


/* Function widened_name_p

   Check whether NAME, an ssa-name used in USE_STMT,
   is a result of a type-promotion, such that:
     DEF_STMT: NAME = NOP (name0)
   where the type of name0 (HALF_TYPE) is smaller than the type of NAME. 
*/

static bool
widened_name_p (tree name, tree use_stmt, tree *half_type, tree *def_stmt) 
{
  tree dummy;
  loop_vec_info loop_vinfo;
  stmt_vec_info stmt_vinfo;
  tree expr;
  tree type = TREE_TYPE (name);
  tree oprnd0;
  enum vect_def_type dt;
  tree def;
  
  stmt_vinfo = vinfo_for_stmt (use_stmt);
  loop_vinfo = STMT_VINFO_LOOP_VINFO (stmt_vinfo);

  if (!vect_is_simple_use (name, loop_vinfo, def_stmt, &def, &dt))
    return false;

  if (dt != vect_loop_def 
      && dt != vect_invariant_def && dt != vect_constant_def)
    return false;

  if (! *def_stmt)
    return false;

  if (TREE_CODE (*def_stmt) != MODIFY_EXPR)
    return false;

  expr = TREE_OPERAND (*def_stmt, 1);
  if (TREE_CODE (expr) != NOP_EXPR)
    return false;

  oprnd0 = TREE_OPERAND (expr, 0);

  *half_type = TREE_TYPE (oprnd0);
  if (!INTEGRAL_TYPE_P (type) || !INTEGRAL_TYPE_P (*half_type)
      || (TYPE_UNSIGNED (type) != TYPE_UNSIGNED (*half_type))
      || (TYPE_PRECISION (type) < (TYPE_PRECISION (*half_type) * 2)))
    return false;

  if (!vect_is_simple_use (oprnd0, loop_vinfo, &dummy, &dummy, &dt))
    return false;

  if (dt != vect_invariant_def && dt != vect_constant_def
      && dt != vect_loop_def)
    return false;

  return true;
}


/* Function vect_recog_sad_pattern.

   Try to find the sum-of-absolute-differences pattern:

    type x, y, absdiff;
    TYPE sad;

    absdiff = ABS (x - y);
    sad += absdiff;

   where 'TYPE' may be bigger than 'type'.

   Input:
   LAST_STMT: A stmt from which the pattern search begins. 

   Output:
   STMT_LIST: If this pattern is detected, STMT_LIST will hold the stmts that
   are part of the pattern. 

   PATTERN_TYPE: The vector type to be used for the returned new stmt.

   Return value: A new stmt that will be used to replace the sequence of
   stmts in STMT_LIST. In this case it will be:
   sad = SAD_EXPR <x, y, sad>
*/

tree vect_recog_sad_pattern (tree last_stmt, tree *type_in, tree *type_out,
                             varray_type *stmt_list)
{
  tree stmt, expr;
  tree oprnd0, oprnd1;
  stmt_vec_info stmt_vinfo = vinfo_for_stmt (last_stmt);
  tree type, half_type;
  tree pattern_expr;
  tree sum_oprnd;
  tree def_stmt;

  if (TREE_CODE (last_stmt) != MODIFY_EXPR)
    return NULL;

  expr = TREE_OPERAND (last_stmt, 1);
  type = TREE_TYPE (expr);

  /* Starting from LAST_STMT, follow the defs of its uses in search
     of the above pattern.  */

  if (TREE_CODE (expr) != PLUS_EXPR)
    return NULL;

  if (STMT_VINFO_IN_PATTERN_P (stmt_vinfo))
    {
      /* Has been detected as widening-summation?  */

      stmt = STMT_VINFO_RELATED_STMT (stmt_vinfo);
      expr = TREE_OPERAND (stmt, 1);
      type = TREE_TYPE (expr);
      if (TREE_CODE (expr) != WIDEN_SUM_EXPR)
	return NULL;
      oprnd0 = TREE_OPERAND (expr, 0);
      oprnd1 = TREE_OPERAND (expr, 1);
      VARRAY_PUSH_TREE (*stmt_list, last_stmt);
      half_type = TREE_TYPE (oprnd0);
    }
  else
    {
      if (STMT_VINFO_DEF_TYPE (stmt_vinfo) != vect_reduction_def)
        return NULL;
      oprnd0 = TREE_OPERAND (expr, 0);
      oprnd1 = TREE_OPERAND (expr, 1);
      if (TYPE_MAIN_VARIANT (TREE_TYPE (oprnd0)) != TYPE_MAIN_VARIANT (type)
          || TYPE_MAIN_VARIANT (TREE_TYPE (oprnd1)) != TYPE_MAIN_VARIANT (type))
        return NULL;
      VARRAY_PUSH_TREE (*stmt_list, last_stmt);

      if (widened_name_p (oprnd0, last_stmt, &half_type, &def_stmt))
	{
	  stmt = def_stmt;
	  VARRAY_PUSH_TREE (*stmt_list, stmt);
	  expr = TREE_OPERAND (stmt, 1);
	  oprnd0 = TREE_OPERAND (expr, 0);
	}
      else
	half_type = type;
    }

  sum_oprnd = oprnd1;

  /* Summation of absolute?  */

  stmt = SSA_NAME_DEF_STMT (oprnd0);
  gcc_assert (stmt);
  stmt_vinfo = vinfo_for_stmt (stmt);
  gcc_assert (stmt_vinfo);
  if (STMT_VINFO_IN_PATTERN_P (stmt_vinfo))
    return NULL;
  gcc_assert (STMT_VINFO_DEF_TYPE (stmt_vinfo) == vect_loop_def);
  gcc_assert (TREE_CODE (stmt) == MODIFY_EXPR);
  expr = TREE_OPERAND (stmt, 1);
  if (TREE_CODE (expr) != ABS_EXPR)
   return NULL;

  VARRAY_PUSH_TREE (*stmt_list, stmt);


  /* Absolute difference?  */

  oprnd0 = TREE_OPERAND (expr, 0);
  if (TREE_CODE (oprnd0) != SSA_NAME)
    return NULL;
  stmt = SSA_NAME_DEF_STMT (oprnd0);
  gcc_assert (stmt);
  stmt_vinfo = vinfo_for_stmt (stmt);
  gcc_assert (stmt_vinfo);
  if (STMT_VINFO_IN_PATTERN_P (stmt_vinfo))
    return NULL;
  if (STMT_VINFO_DEF_TYPE (stmt_vinfo) != vect_loop_def)
    return NULL;
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return NULL;

  expr = TREE_OPERAND (stmt, 1);
  if (TREE_CODE (expr) != MINUS_EXPR)
   return NULL;

  VARRAY_PUSH_TREE (*stmt_list, stmt);

  *type_in = half_type;
  *type_out = type;

  /* Pattern detected. Create a stmt to be used to replace the pattern: */
  oprnd0 = TREE_OPERAND (expr, 0);
  oprnd1 = TREE_OPERAND (expr, 1);
  pattern_expr = build3 (SAD_EXPR, type, oprnd0, oprnd1, sum_oprnd);
  if (vect_print_dump_info (REPORT_DETAILS))
    {
      fprintf (vect_dump, "vect_recog_sad_pattern: detected: ");
      print_generic_expr (vect_dump, pattern_expr, TDF_SLIM);
    }
  return pattern_expr;
}


/* Function vect_recog_mult_hi_pattern

   Try to find the pattern of a multiplication that computes the high part
   of the product:

    type x, y;
    TYPE prod, hi_part; 
    type hi_part';

    prod = x w* y; (a widening-multiplication pattern. see below).
    hi_part = prod >> size 
    hi_part' = (type) hi_part;
  
   where 'size' is the size of 'type', 
   and 'TYPE' is double the size of 'type'.

   Input:
   LAST_STMT: A stmt from which the pattern search begins. 

   Output:
   STMT_LIST: If this pattern is detected, STMT_LIST will hold the stmts that
   are part of the pattern. 

   PATTERN_TYPE: The vector type to be used for the returned new stmt.

   Return value: A new stmt that will be used to replace the sequence of
   stmts in STMT_LIST. In this case it will be:
   hi_part = MULT_HI_EXPR <x, y>
*/

tree
vect_recog_mult_hi_pattern (tree last_stmt, tree *type_in, tree *type_out,
                            varray_type *stmt_list)
{
  tree stmt, expr;
  tree oprnd0, oprnd1;
  stmt_vec_info stmt_vinfo = vinfo_for_stmt (last_stmt);
  tree type, half_type;
  tree pattern_expr;

  if (STMT_VINFO_IN_PATTERN_P (vinfo_for_stmt (last_stmt)))
    return NULL;

  if (TREE_CODE (last_stmt) != MODIFY_EXPR)
    return NULL;

  expr = TREE_OPERAND (last_stmt, 1);
  half_type = TREE_TYPE (expr);

  /* Starting from LAST_STMT, follow the defs of its uses in search
     of the above pattern.  */

  if (TREE_CODE (expr) != NOP_EXPR && TREE_CODE (expr) != CONVERT_EXPR)
    return NULL;

  if (STMT_VINFO_DEF_TYPE (stmt_vinfo) != vect_loop_def)
    return NULL;

  oprnd0 = TREE_OPERAND (expr, 0);

  VARRAY_PUSH_TREE (*stmt_list, last_stmt);


  /* Check that oprnd0 is defined by a shift-right stmt.  */

  stmt = SSA_NAME_DEF_STMT (oprnd0);
  gcc_assert (stmt);
  stmt_vinfo = vinfo_for_stmt (stmt);
  if (!stmt_vinfo)
    return NULL;
  if (STMT_VINFO_IN_PATTERN_P (stmt_vinfo))
    return NULL;
  if (STMT_VINFO_DEF_TYPE (stmt_vinfo) != vect_loop_def)
    return NULL;
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return NULL;

  expr = TREE_OPERAND (stmt, 1);
  type = TREE_TYPE (expr);

  if (TREE_CODE (expr) != RSHIFT_EXPR)
    return NULL;
  
  if (STMT_VINFO_DEF_TYPE (stmt_vinfo) != vect_loop_def)
    return NULL;

  if (TYPE_PRECISION (type) != (TYPE_PRECISION (half_type) * 2))
        return NULL;

  oprnd0 = TREE_OPERAND (expr, 0);
  oprnd1 = TREE_OPERAND (expr, 1); 

  VARRAY_PUSH_TREE (*stmt_list, stmt);
  

  /* Check that oprnd0 is defined by a widening-mult, and that oprnd1
     is a constant which value is the size of half_type.  */

  if (!host_integerp (oprnd1, 1))
    return NULL;

  if (tree_int_cst_compare (oprnd1, TYPE_SIZE (half_type)) != 0)
    return NULL;

  if (TREE_CODE (oprnd0) != SSA_NAME)
    return NULL;

  stmt = SSA_NAME_DEF_STMT (oprnd0);
  gcc_assert (stmt);
  stmt_vinfo = vinfo_for_stmt (stmt);
  gcc_assert (stmt_vinfo);
  if (STMT_VINFO_DEF_TYPE (stmt_vinfo) != vect_loop_def)
    return NULL;
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return NULL;
  expr = TREE_OPERAND (stmt, 1);
  if (TREE_CODE (expr) != MULT_EXPR)
    return NULL;
  if (STMT_VINFO_IN_PATTERN_P (stmt_vinfo))
    {
      stmt = STMT_VINFO_RELATED_STMT (stmt_vinfo);
      expr = TREE_OPERAND (stmt, 1);
      if (TREE_CODE (expr) != WIDEN_MULT_EXPR)
        return NULL;
      oprnd0 = TREE_OPERAND (expr, 0);
      oprnd1 = TREE_OPERAND (expr, 1); 
    }
  else
    {
      tree half_type0, half_type1;
      tree def_stmt;

      oprnd0 = TREE_OPERAND (expr, 0);
      oprnd1 = TREE_OPERAND (expr, 1);
      if (TYPE_MAIN_VARIANT (TREE_TYPE (oprnd0)) != TYPE_MAIN_VARIANT (type)
          || TYPE_MAIN_VARIANT (TREE_TYPE (oprnd1)) != TYPE_MAIN_VARIANT (type))
        return NULL;
      VARRAY_PUSH_TREE (*stmt_list, stmt);
      if (!widened_name_p (oprnd0, stmt, &half_type0, &def_stmt))
        return NULL;
      oprnd0 = TREE_OPERAND (TREE_OPERAND (def_stmt, 1), 0);
      if (!widened_name_p (oprnd1, last_stmt, &half_type1, &def_stmt))
        return NULL;
      oprnd1 = TREE_OPERAND (TREE_OPERAND (def_stmt, 1), 0);
      if (TYPE_MAIN_VARIANT (half_type0) != TYPE_MAIN_VARIANT (half_type1))
        return NULL;
      if (TYPE_MAIN_VARIANT (half_type0) != TYPE_MAIN_VARIANT (half_type))
        return NULL;
    }

  *type_in = half_type;
  *type_out = NULL_TREE;

  /* Pattern detected. Create a stmt to be used to replace the pattern: */
  pattern_expr = build2 (MULT_HI_EXPR, half_type, oprnd0, oprnd1);
  if (vect_print_dump_info (REPORT_DETAILS))
    {
      fprintf (vect_dump, "vect_recog_mult_hi_pattern: detected: ");
      print_generic_expr (vect_dump, pattern_expr, TDF_SLIM);
    }
  return pattern_expr;
}


/* Function vect_recog_dot_prod_pattern

   Try to find the following pattern:

     type x_t, y_t;
     TYPE1 prod;
     TYPE2 sum = init;
   loop:
     sum_0 = phi <init, sum_1>

     S1  x_t = ...
     S2  y_t = ...
     S3  x_T = (TYPE1) x_t;
     S4  y_T = (TYPE1) y_t;
     S5  prod = x_T * y_T;
     [S6  prod = x_T * y_T;  #optional. only if TYPE1 != TYPE2]
     S7  sum_1 = prod + sum_0;

   where 'TYPE1' is exactly double the size of type 'type',
   and 'TYPE2' is the same size of 'TYPE1' or bigger.

   Input:
   LAST_STMT: A stmt from which the pattern search begins. In the example,
   when this function is called with S7, the pattern {S3,S4,S5,S6,S7} will be
   detected.

   Output:
   STMT_LIST: If this pattern is detected, STMT_LIST will hold the stmts that
   are part of the pattern. In the example, STMT_LIST will consist of 
   {S3,S4,S5,S6,S7}.

   PATTERN_TYPE: The vector type to be used for the returned new stmt.

   Return value: A new stmt that will be used to replace the sequence of
   stmts in STMT_LIST. In this case it will be:
   WIDEN_DOT_PRODUCT <x_t, y_t, sum_0>

   TODO: detect the widen_sum pattern before dot_product,
         and extend dot_product recognition to detect the
         pattern:
         DPROD = X w* Y;
         sum_1 = DPROD w+ sum_0;
*/

tree
vect_recog_dot_prod_pattern (tree last_stmt, tree *type_in, tree *type_out,
                             varray_type *stmt_list)
{
  tree stmt, expr;
  tree oprnd0, oprnd1;
  tree oprnd00, oprnd01;
  stmt_vec_info stmt_vinfo = vinfo_for_stmt (last_stmt);
  tree type, half_type;
  tree pattern_expr;
  tree prod_type;

  if (TREE_CODE (last_stmt) != MODIFY_EXPR)
    return NULL;

  expr = TREE_OPERAND (last_stmt, 1);
  type = TREE_TYPE (expr);

  /* Look for the following pattern 
	  DX = (TYPE1) X;
	  DY = (TYPE1) Y;
          DPROD = DX * DY; 
	  DDPROD = (TYPE2) DPROD;
          sum_1 = DDPROD + sum_0;
     In which 
     - DX is double the size of X
     - DY is double the size of Y
     - DX, DY, DPROD all have the same type
     - sum is the same size of PROD or bigger
     - sum has been recognized as a reduction variable.

     This is equivalent to:
       DPROD = X w* Y;
       sum_1 = DPROD w+ sum_0;
     or
       DPROD = X w* Y;
       sum_1 = DPROD + sum_0;
   */

  /* Starting from LAST_STMT, follow the defs of its uses in search
     of the above pattern.  */

  if (TREE_CODE (expr) != PLUS_EXPR)
    return NULL;

  if (STMT_VINFO_IN_PATTERN_P (stmt_vinfo))
    {
      /* Has been detected as widening-summation?  */

      stmt = STMT_VINFO_RELATED_STMT (stmt_vinfo);
      gcc_assert (TREE_CODE (stmt) == MODIFY_EXPR);
      expr = TREE_OPERAND (stmt, 1);
      type = TREE_TYPE (expr);
      if (TREE_CODE (expr) != WIDEN_SUM_EXPR)
        return NULL;
      oprnd0 = TREE_OPERAND (expr, 0);
      oprnd1 = TREE_OPERAND (expr, 1);
      half_type = TREE_TYPE (oprnd0);
    }
  else
    {
      tree def_stmt;

      if (STMT_VINFO_DEF_TYPE (stmt_vinfo) != vect_reduction_def)
        return NULL;
      oprnd0 = TREE_OPERAND (expr, 0);
      oprnd1 = TREE_OPERAND (expr, 1);
      if (TYPE_MAIN_VARIANT (TREE_TYPE (oprnd0)) != TYPE_MAIN_VARIANT (type)
          || TYPE_MAIN_VARIANT (TREE_TYPE (oprnd1)) != TYPE_MAIN_VARIANT (type))
        return NULL;
      stmt = last_stmt;
      VARRAY_PUSH_TREE (*stmt_list, stmt);

      if (widened_name_p (oprnd0, stmt, &half_type, &def_stmt))
        {
          stmt = def_stmt;
          VARRAY_PUSH_TREE (*stmt_list, stmt);
          expr = TREE_OPERAND (stmt, 1);
          oprnd0 = TREE_OPERAND (expr, 0);
        }
      else
        half_type = type;
    }

  VARRAY_PUSH_TREE (*stmt_list, last_stmt);
 
  /* So far so good.
     Assumption: oprnd1 is the reduction variable (defined by a loop-header
     phi), and oprnd0 is an ssa-name defined by a stmt in the loop body.
     Left to check that oprnd0 is defined by a (widen_)mult_expr  */
  prod_type = type;

  stmt = SSA_NAME_DEF_STMT (oprnd0);
  gcc_assert (stmt);
  stmt_vinfo = vinfo_for_stmt (stmt);
  gcc_assert (stmt_vinfo);
  gcc_assert (STMT_VINFO_DEF_TYPE (stmt_vinfo) == vect_loop_def);
  gcc_assert (TREE_CODE (stmt) == MODIFY_EXPR);
  expr = TREE_OPERAND (stmt, 1);
  if (TREE_CODE (expr) != MULT_EXPR)
    return NULL;
  if (STMT_VINFO_IN_PATTERN_P (stmt_vinfo))
    {
      stmt = STMT_VINFO_RELATED_STMT (stmt_vinfo);
      gcc_assert (TREE_CODE (stmt) == MODIFY_EXPR);
      expr = TREE_OPERAND (stmt, 1);
      if (TREE_CODE (expr) != WIDEN_MULT_EXPR)
        return NULL;
      stmt_vinfo = vinfo_for_stmt (stmt);
      gcc_assert (stmt_vinfo);
      gcc_assert (STMT_VINFO_DEF_TYPE (stmt_vinfo) == vect_loop_def);
      oprnd00 = TREE_OPERAND (expr, 0);
      oprnd01 = TREE_OPERAND (expr, 1);
    }
  else
    {
      tree half_type0, half_type1;
      tree def_stmt;

      oprnd0 = TREE_OPERAND (expr, 0);
      oprnd1 = TREE_OPERAND (expr, 1);
      if (TYPE_MAIN_VARIANT (TREE_TYPE (oprnd0)) != TYPE_MAIN_VARIANT (type)
          || TYPE_MAIN_VARIANT (TREE_TYPE (oprnd1)) != TYPE_MAIN_VARIANT (type))
        return NULL;
      VARRAY_PUSH_TREE (*stmt_list, stmt);
      if (!widened_name_p (oprnd0, stmt, &half_type0, &def_stmt))
        return NULL;
      oprnd00 = TREE_OPERAND (TREE_OPERAND (def_stmt, 1), 0);
      if (!widened_name_p (oprnd1, stmt, &half_type1, &def_stmt))
        return NULL;
      oprnd01 = TREE_OPERAND (TREE_OPERAND (def_stmt, 1), 0);
      if (TYPE_MAIN_VARIANT (half_type0) != TYPE_MAIN_VARIANT (half_type1))
        return NULL;
    }

  half_type = TREE_TYPE (oprnd00);
  *type_in = half_type;
  *type_out = type;

  /* Pattern detected. Create a stmt to be used to replace the pattern: */
  pattern_expr = build3 (DOT_PROD_EXPR, type, oprnd00, oprnd01, oprnd1);
  if (vect_print_dump_info (REPORT_DETAILS))
    {
      fprintf (vect_dump, "vect_recog_dot_prod_pattern: detected: ");
      print_generic_expr (vect_dump, pattern_expr, TDF_SLIM);
    }
  return pattern_expr;
}


/* Function vect_recog_widen_mult_pattern

   Try to find the following pattern:

     type a_t, b_t;
     TYPE a_T, b_T, prod_T;

     S1  a_t = ;
     S2  b_t = ;
     S3  a_T = (TYPE) a_t;
     S4  b_T = (TYPE) b_t;
     S5  prod_T = a_T * b_T;

   where type 'TYPE' is at least double the size of type 'type', 
   i.e - we're multiplying elements of type 'type' into a result of type 'TYPE'.

   For example:

     unsigned short *in1, *in2, A, B;
     unsigned int DA, DB, prod;

     S1  A = *in1;
     S2  B = *in2;
     S3  DA = (unsigned int) A;
     S4  DB = (unsigned int) B;
     S5  prod = DA * DB;


   Input:
   LAST_STMT: A stmt from which the pattern search begins. In the example,
   when this function is called with S5, the pattern {S3,S4,S5} is be detected.

   Output:
   STMT_LIST: If this pattern is detected, STMT_LIST will hold the stmts that
   are part of the pattern. In the example, STMT_LIST consists of {S3,S4,S5}.

   PATTERN_TYPE: The vector type to be used for the returned new stmt.
   For the example above, for targets that support vectors of 8 shorts,
   we want to return PATTERN_TYPE=V8HI. (Note that the result
   contains 4 ints, because we're dealing with fixed-sized vectors,
   but it doesn't change the fact that the vectorization factor is 8,
   as the 4 results correspond to 8 elements from 8 different iterations).

   Return value: A new stmt that will be used to replace the sequence of
   stmts in STMT_LIST. In this case it will be:
   WIDEN_MULT <A, B>
*/
   
tree
vect_recog_widen_mult_pattern (tree last_stmt, tree *type_in, tree *type_out,
                               varray_type *stmt_list)
{
  tree expr;
  tree def_stmt0, def_stmt1;
  tree oprnd0, oprnd1;
  tree type, half_type0, half_type1;
  tree pattern_expr;
  tree vectype;
  tree dummy;
  enum tree_code dummy_code;

  if (STMT_VINFO_IN_PATTERN_P (vinfo_for_stmt (last_stmt)))
    return NULL;

  if (TREE_CODE (last_stmt) != MODIFY_EXPR)
    return NULL;

  expr = TREE_OPERAND (last_stmt, 1);
  type = TREE_TYPE (expr);

  /* Starting from LAST_STMT, follow the defs of its uses in search
     of the above pattern.  */
   
  if (TREE_CODE (expr) != MULT_EXPR)
    return NULL;

  oprnd0 = TREE_OPERAND (expr, 0);
  oprnd1 = TREE_OPERAND (expr, 1);
  if (TYPE_MAIN_VARIANT (TREE_TYPE (oprnd0)) != TYPE_MAIN_VARIANT (type) 
      || TYPE_MAIN_VARIANT (TREE_TYPE (oprnd1)) != TYPE_MAIN_VARIANT (type))
    return NULL;

  VARRAY_PUSH_TREE (*stmt_list, last_stmt);

  /* Check argument 0 */
  if (!widened_name_p (oprnd0, last_stmt, &half_type0, &def_stmt0))
    return NULL;
  oprnd0 = TREE_OPERAND (TREE_OPERAND (def_stmt0, 1), 0);

  /* Check argument 1 */
  if (!widened_name_p (oprnd1, last_stmt, &half_type1, &def_stmt1))
    return NULL;
  oprnd1 = TREE_OPERAND (TREE_OPERAND (def_stmt1, 1), 0);

  if (TYPE_MAIN_VARIANT (half_type0) != TYPE_MAIN_VARIANT (half_type1))
    return NULL;

  VARRAY_PUSH_TREE (*stmt_list, def_stmt0);
  VARRAY_PUSH_TREE (*stmt_list, def_stmt1);

  /* Check target support  */
  vectype = get_vectype_for_scalar_type (half_type0);
  if (!supportable_widening_operation (WIDEN_MULT_EXPR, last_stmt, vectype,
                                       &dummy, &dummy, &dummy_code,
                                       &dummy_code))
    return NULL;

  *type_in = vectype;
  *type_out = NULL_TREE;

  /* Pattern detected. Create a stmt to be used to replace the pattern: */
  pattern_expr = build2 (WIDEN_MULT_EXPR, type, oprnd0, oprnd1);
  if (vect_print_dump_info (REPORT_DETAILS))
    {
      fprintf (vect_dump, "vect_recog_widen_mult_pattern: detected: ");
      print_generic_expr (vect_dump, pattern_expr, TDF_SLIM);
    }
  return pattern_expr;
}


/* Function vect_recog_widen_sum_pattern

   Try to find the following pattern:

     type x_t;
     TYPE x_T, sum = init;
   loop:
     sum_0 = phi <init, sum_1>

     S1  x_t = *p;
     S2  x_T = (TYPE) x_t;
     S3  sum_1 = x_T + sum_0;

   where type 'TYPE' is at least double the size of type 'type', 
   i.e - we're summing elements of type 'type' into an accumulator 
   of type 'TYPE'.

   For example:

     unsigned short *data, X;
     unsigned int DX, sum = init;
   loop:
     sum_0 = phi <init, sum_1>

     S1  X = *data.1_20;
     S2  DX = (unsigned int) X;
     S3  sum_1 = DX + sum_0;


   Input:
   LAST_STMT: A stmt from which the pattern search begins. In the example,
   when this function is called with S3, the pattern {S2,S3} will be detected.

   Output:
   STMT_LIST: If this pattern is detected, STMT_LIST will hold the stmts that
   are part of the pattern. In the example, STMT_LIST will consist of {S2,S3}.

   PATTERN_TYPE: The vector type to be used for the returned new stmt.
   For the example above, for targets that support vectors of 8 shorts,
   we want to return PATTERN_TYPE=V8HI. (Note that the result
   contains 4 ints, because we're dealing with fixed-sized vectors,
   but it doesn't change the fact that the vectorization factor is 8, 
   as the 4 results correspond to 8 elements from 8 different iterations).

   Return value: A new stmt that will be used to replace the sequence of
   stmts in STMT_LIST. In this case it will be:
   WIDEN_SUM <X, sum_0>
*/

tree
vect_recog_widen_sum_pattern (tree last_stmt, tree *type_in, tree *type_out,
			      varray_type *stmt_list)
{
  tree stmt, expr;
  tree oprnd0, oprnd1;
  stmt_vec_info stmt_vinfo = vinfo_for_stmt (last_stmt);
  tree type, half_type;
  tree pattern_expr;

  if (STMT_VINFO_IN_PATTERN_P (vinfo_for_stmt (last_stmt)))
    return NULL;

  if (TREE_CODE (last_stmt) != MODIFY_EXPR)
    return NULL;

  expr = TREE_OPERAND (last_stmt, 1);
  type = TREE_TYPE (expr);
   
  /* Look for the following pattern 
          DX = (TYPE) X; 
          sum_1 = DX + sum_0;
     In which DX is at least double the size of X, and sum_1 has been
     recognized as reduction.
   */
   
  /* Starting from LAST_STMT, follow the defs of its uses in search
     of the above pattern.  */

  if (TREE_CODE (expr) != PLUS_EXPR)
    return NULL;

  if (STMT_VINFO_DEF_TYPE (stmt_vinfo) != vect_reduction_def)
    return NULL;
  
  oprnd0 = TREE_OPERAND (expr, 0);
  oprnd1 = TREE_OPERAND (expr, 1);
  if (TYPE_MAIN_VARIANT (TREE_TYPE (oprnd0)) != TYPE_MAIN_VARIANT (type) 
      || TYPE_MAIN_VARIANT (TREE_TYPE (oprnd1)) != TYPE_MAIN_VARIANT (type))
    return NULL;
  
  VARRAY_PUSH_TREE (*stmt_list, last_stmt);
  
  /* So far so good.
     Assumption: oprnd1 is the reduction variable (defined by a loop-header
     phi), and oprnd0 is an ssa-name defined by a stmt in the loop body.
     Left to check that oprnd0 is defined by a cast from type 'type'
     to type 'TYPE'.

          DX = (TYPE) X;
          sum_1 = DX + sum_0;
   */

  if (!widened_name_p (oprnd0, last_stmt, &half_type, &stmt))
    return NULL;

  oprnd0 = TREE_OPERAND (TREE_OPERAND (stmt, 1), 0);
  VARRAY_PUSH_TREE (*stmt_list, stmt);

  *type_in = half_type;
  *type_out = type;

  /* Pattern detected. Create a stmt to be used to replace the pattern: */
  pattern_expr = build2 (WIDEN_SUM_EXPR, type, oprnd0, oprnd1);
  if (vect_print_dump_info (REPORT_DETAILS))
    {
      fprintf (vect_dump, "vect_recog_widen_sum_pattern: detected: ");
      print_generic_expr (vect_dump, pattern_expr, TDF_SLIM);
    }
  return pattern_expr;
}   

 
/* Function vect_recog_unsigned_subsat_pattern
  
   Try to find a pattern of USAT(a-b) - an unsigned saturating subtraction. 
   It can take any of the following forms:
   
   form1: a > (b - 1) ? a - b : 0
   form2: a >= b ? a - b : 0 
   form3: (a - b > 0) ? a - b : 0
  
   FORNOW: Detect only form1.

   For example, this may look like:
   S1: x = a - cnst
   S2: a > (cnst_minus_1) ? x : 0

   Input:
   LAST_STMT: A stmt from which the pattern search begins. In the example,
   when this function is called with S2, the pattern {S1,S2} will be detected.

   Output:
   STMT_LIST: If this pattern is detected, STMT_LIST will hold the stmts that 
   are part of the pattern. In the example, STMT_LIST will consist of {S1,S2}.

   Return value: A new stmt that will be used to replace the sequence of
   stmts in STMT_LIST. In this case it will be: SAT_MINUS_EXPR (a,b).
*/  
  
tree
vect_recog_unsigned_subsat_pattern (tree last_stmt, tree *type_in, tree *type_out, 
				    varray_type *stmt_list)
{
  tree stmt, expr;
  tree type;
  tree cond_expr;
  tree then_clause = NULL_TREE;
  tree else_clause = NULL_TREE;
  enum tree_code code;
  tree zero;
  tree a, b, a_minus_b, b_minus_1;
  tree pattern_expr;
  tree new;

  if (STMT_VINFO_IN_PATTERN_P (vinfo_for_stmt (last_stmt)))
    return NULL;

  if (TREE_CODE (last_stmt) != MODIFY_EXPR)
    return NULL;

  expr = TREE_OPERAND (last_stmt, 1);
  type = TREE_TYPE (expr);

  /* Look for the following pattern
	a_minus_b = a - b 
        x = (a > b_minus_1) ? a_minus_b : 0
     in which all variables are of the same unsigned type.
     This is equivalent to: USAT (name, k).
   */

  /* Starting from LAST_STMT, follow the defs of its uses in search
     of the above pattern.  */

  /* Expecting a cond_expr of one of the following forms:
	   x = (a > b_minus_1) ? a_minus_b : 0
	   x = (a <= b_minus_1) ? 0 : a_minus_b 
     such that:
     - x, a, a_minus_b are SSA_NAMES of type T
     - b_minus_1 is an SSA_NAME or a constant also of type T
     - T is an unsigned integer (uchar/ushort/uint/ulong...)
   */

  if (TREE_CODE (expr) != COND_EXPR)
    return NULL;

  if (!TYPE_UNSIGNED (type) || TREE_CODE (type) != INTEGER_TYPE) /* CHECKME */
    return NULL;

  cond_expr = TREE_OPERAND (expr, 0);
  code = TREE_CODE (cond_expr);
  then_clause = TREE_OPERAND (expr, 1);
  else_clause = TREE_OPERAND (expr, 2);

  if (TREE_CODE (then_clause) == SSA_NAME
      && TREE_TYPE (then_clause) == type)
    {
      a_minus_b = then_clause;
      zero = else_clause;
    }
  else if (TREE_CODE (else_clause) == SSA_NAME
	   && TREE_TYPE (else_clause) == type)
    {
      a_minus_b = else_clause;
      zero = then_clause;
    }
  else
    return NULL;

  if (!integer_zerop (zero))
    return NULL;

  if ((code == GT_EXPR && then_clause == a_minus_b)
      || (code == LE_EXPR && then_clause == zero))
    {
      /* x = (a > b_minus_1) ? a_minus_b : 0, or
	 x = (a <= b_minus_1) ? 0 : a_minus_b */
      a = TREE_OPERAND (cond_expr, 0);
      b_minus_1 = TREE_OPERAND (cond_expr, 1);	
    }
  else if  ((code == GT_EXPR && then_clause == a_minus_b)
      || (code == LE_EXPR && then_clause == zero))
    {
      /* x = (b_minus_1 < a) ? a_minus_b : 0, or
         x = (b_minus_1 >= a) ? 0 : a_minus_b */
      a = TREE_OPERAND (cond_expr, 1);
      b_minus_1 = TREE_OPERAND (cond_expr, 0);
    }
  else
    return NULL;
	
  if (TYPE_MAIN_VARIANT (TREE_TYPE (a)) != TYPE_MAIN_VARIANT (type))
    return NULL;

  VARRAY_PUSH_TREE (*stmt_list, last_stmt);
  
  /* So far so good. Left to check that:
	- a_minus_b == a - b
	- b_minus_1 == b - 1
   */

  stmt = SSA_NAME_DEF_STMT (a_minus_b);
  if (!stmt || TREE_CODE (stmt) != MODIFY_EXPR)
    return NULL;

  if (STMT_VINFO_IN_PATTERN_P (vinfo_for_stmt (stmt)))
    return NULL;

  expr = TREE_OPERAND (stmt, 1);
  if (TREE_CODE (expr) != MINUS_EXPR)
    return NULL;

  if (TREE_OPERAND (expr, 0) != a)
    return NULL;

  b = TREE_OPERAND (expr, 1);
  /* CHECKME: */
  if (host_integerp (b, 1))
    new = fold (int_const_binop (MINUS_EXPR, b, integer_one_node, 1));
  else if (TREE_CODE (b) == SSA_NAME)
    new = fold (build2 (MINUS_EXPR, type, b, integer_one_node));
  else
    return NULL;

  if (!expressions_equal_p (b_minus_1, new))
    return NULL;

  VARRAY_PUSH_TREE (*stmt_list, stmt);

  *type_in = type;
  *type_out = NULL_TREE;

  /* Pattern detected. Create a stmt to be used to replace the pattern: */
  pattern_expr = build2 (SAT_MINUS_EXPR, type, a, b);
  if (vect_print_dump_info (REPORT_DETAILS))
    {
      fprintf (vect_dump, "vect_recog_unsigned_subsat_pattern: detetced: ");
      print_generic_expr (vect_dump, pattern_expr, TDF_SLIM);
    }
  return pattern_expr;
}


/* Function vect_pattern_recog_1 

   Input:
   PATTERN_RECOG_FUNC: A pointer to a function that detects a certain
        computation pattern.
   STMT: A stmt from which the pattern search should start.

   If PATTERN_RECOG_FUNC successfully detected the pattern, it creates an
   expression that computes the same functionality and can be used to 
   replace the sequence of stmts that are involved in the pattern. 
   This function checks if the returned expression is supported in vector
   form by the target and does some bookeeping, as explained in the
   documentation for vect_recog_pattern.  */

static void
vect_pattern_recog_1 (
	tree (* pattern_recog_func) (tree, tree *, tree *, varray_type *),
	block_stmt_iterator si)
{
  tree stmt = bsi_stmt (si);
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  stmt_vec_info pattern_stmt_info;
  loop_vec_info loop_vinfo = STMT_VINFO_LOOP_VINFO (stmt_info);
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  varray_type stmt_list;
  tree pattern_expr;
  tree pattern_vectype;
  tree type_in, type_out;
  tree pattern_type;
  enum tree_code code;
  tree var, var_name;
  stmt_ann_t ann;

  VARRAY_TREE_INIT (stmt_list, 10, "stmt list");
  pattern_expr = (* pattern_recog_func) (stmt, &type_in, &type_out, &stmt_list);
  if (!pattern_expr) 
    { 
      varray_clear (stmt_list); 
      return; 
    } 
 
  if (VECTOR_MODE_P (TYPE_MODE (type_in))) 
    { 
      /* No need to check target support (already checked by the pattern 
         recognition function).  */ 
      pattern_vectype = type_in;
    }
  else
    {
      enum tree_code vec_mode;
      enum insn_code icode;
      optab optab;

      /* Check target support  */
      pattern_vectype = get_vectype_for_scalar_type (type_in);
      optab = optab_for_tree_code (TREE_CODE (pattern_expr), pattern_vectype);
      vec_mode = TYPE_MODE (pattern_vectype);
      if (!optab
          || (icode = optab->handlers[(int) vec_mode].insn_code) ==
              CODE_FOR_nothing
          || (type_out
              && (insn_data[icode].operand[0].mode !=
                  TYPE_MODE (get_vectype_for_scalar_type (type_out)))))
        {
          varray_clear (stmt_list);
          return;
        }
    }

  /* Found a vectorizable pattern.  */
  if (vect_print_dump_info (REPORT_DETAILS))
    {
      fprintf (vect_dump, "pattern recognized: "); 
      print_generic_expr (vect_dump, pattern_expr, TDF_SLIM);
    }
  
  /* Mark the stmts that are involved in the pattern,
     and create a new stmt to express the pattern and add it to the code.  */
  code = TREE_CODE (pattern_expr);
  pattern_type = TREE_TYPE (pattern_expr);
  var = create_tmp_var (pattern_type, "patt");
  add_referenced_tmp_var (var);
  var_name = make_ssa_name (var, NULL_TREE);
  pattern_expr = build2 (MODIFY_EXPR, void_type_node, var_name, pattern_expr);
  SSA_NAME_DEF_STMT (var_name) = pattern_expr;
  bsi_insert_before (&si, pattern_expr, BSI_SAME_STMT);
  ann = stmt_ann (pattern_expr);
  set_stmt_info ((tree_ann_t)ann, new_stmt_vec_info (pattern_expr, loop_vinfo));
  pattern_stmt_info = vinfo_for_stmt (pattern_expr);
  
  STMT_VINFO_RELATED_STMT (pattern_stmt_info) = stmt;
  STMT_VINFO_DEF_TYPE (pattern_stmt_info) = STMT_VINFO_DEF_TYPE (stmt_info);
  STMT_VINFO_VECTYPE (pattern_stmt_info) = pattern_vectype;

  while (VARRAY_ACTIVE_SIZE (stmt_list) > 0)
    {
      tree stmt_in_pattern = VARRAY_TOP_TREE (stmt_list);
      basic_block bb = bb_for_stmt (stmt_in_pattern);

      VARRAY_POP (stmt_list);
      if (flow_bb_inside_loop_p (loop, bb))
        {
          stmt_vec_info stmt_vinfo = vinfo_for_stmt (stmt_in_pattern);

          STMT_VINFO_IN_PATTERN_P (stmt_vinfo) = true;
          STMT_VINFO_RELATED_STMT (stmt_vinfo) = pattern_expr;
        }
    }
  varray_clear (stmt_list);

  return;
}


/* Function vect_pattern_recog

   Input:
   LOOP_VINFO - a struct_loop_info of a loop in which we want to look for
        computation idioms.

   Output - for each computation idiom that is detected we insert a new stmt
        that provides the same functionality and that can be vectorized. We
        also record some information in the struct_stmt_info of the relevant
        stmts, as explained below:

   At the entry to this function we have the following stmts, with the
   following initial value in the STMT_VINFO fields:

         stmt                     in_pattern_p  related_stmt    vec_stmt
         S1: a_i = ....                 false
         S2: a_2 = ..use(a_i)..         false
         S3: a_1 = ..use(a_2)..         false
         S4: a_0 = ..use(a_1)..         false
         S5: ... = ..use(a_0)..         false


   Say the sequence {S1,S2,S3,S4} was detected as a pattern that can be
   represented by a single stmt. We then:
   - create a new stmt S6 that will replace the pattern.
   - insert the new stmt S6 before the last stmt in the pattern
   - fill in the STMT_VINFO fields as follows:

                                  in_pattern_p  related_stmt    vec_stmt
         S1: a_i = ....                 true
         S2: a_2 = ..use(a_i)..         true
         S3: a_1 = ..use(a_2)..         true
       > S6: a_new = ....               false   S4
         S4: a_0 = ..use(a_1)..         true    S6
         S5: ... = ..use(a_0)..         false

   (the last stmt in the pattern (S4) and the new pattern stmt (S6) point
    to each other through the RELATED_STMT field).

   S6 will be marked as relevant in vect_mark_stmts_to_be_vectorized instead
   of S4 because it will replace all its uses.  Stmts {S1,S2,S3} will
   remain irrelevant unless used by stmts other than S4.

   If vectorization succeeds, vect_transform_stmt will skip over {S1,S2,S3}
   (because they are marked as irrelevent). It will vectorize S6, and record
   a pointer to the new vector stmt VS6 both from S6 (as usual), and also 
   from S4. We do that so that when we get to vectorizing stmts that use the
   def of S4 (like S5 that uses a_0), we'll know where to take the relevant
   vector-def from. S4 will be skipped, and S5 will be vectorized as usual:

                                  in_pattern_p  related_stmt    vec_stmt
         S1: a_i = ....                 true            
         S2: a_2 = ..use(a_i)..         true    
         S3: a_1 = ..use(a_2)..         true    
       > VS6: va_new = ....     
         S6: a_new = ....               false   S4              VS6
         S4: a_0 = ..use(a_1)..         true    S6              VS6
       > VS5: ... = ..vuse(va_new)..
         S5: ... = ..use(a_0).. false   

   DCE could then get rid of {S1,S2,S3,S4,S5,S6} (if their defs are not used
   elsewhere), and we'll end up with:

        VS6: va_new = .... 
        VS5: ... = ..vuse(va_new)..

   If vectorization does not succeed, DCE will clean S6 away (its def is
   not used), and we'll end up with the original sequence.
*/

void
vect_pattern_recog (loop_vec_info loop_vinfo)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  basic_block *bbs = LOOP_VINFO_BBS (loop_vinfo);
  unsigned int nbbs = loop->num_nodes;
  block_stmt_iterator si;
  tree stmt;
  unsigned int i, j;
  tree (* pattern_recog_func) (tree, tree *, tree *, varray_type *);

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "=== vect_pattern_recog ===");

  /* Scan through the loop stmts, applying the pattern recognition
     functions starting at each stmt visited:  */
  for (i = 0; i < nbbs; i++)
    {
      basic_block bb = bbs[i];
      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
        {
          stmt = bsi_stmt (si);

          /* Scan over all generic vect_recog_xxx_pattern functions.  */
          for (j = 0; j < NUM_PATTERNS; j++)
            {
              pattern_recog_func = vect_pattern_recog_funcs[j];
              vect_pattern_recog_1 (pattern_recog_func, si);
            }
        }
    }
}
