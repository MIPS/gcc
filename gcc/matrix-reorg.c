/* Matrix layout transformations.
   Copyright (C) 2004 Free Software Foundation, Inc.
   Contributed by Revital Eres <eres@il.ibm.com>
   
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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

/*  This phase tries to flatten and transpose matrices based
    on profiling information. This first implementation focuses on
    multidimensional global matrices which are defined dynamically.
    
    For example, consider the following three dimensional matrix:

    int *** arr;

    If arr was found suitable for this optimization we define a 
    new one dimensional array which is the equivalent flatten matrix 
    of arr and replace every access to arr with an access to the new 
    flatten matrix.

    Analysis phase:
    ===============
    The driver for this phase is decide_matrix_flattening ().
    In which each matrix that has all it's access sites analyzed is marked 
    as suitable for flattening.
    
    The next step is to decide; based on profiling information; if to flatten 
    the matrix as defined by the user or flatten it's transposed matrix.

    For example, consider the following nested loops - 

    for (int i = 0; i < n; i++)
       for (int j = 0; j < m; j++)
          A[i][j] = B[j][i];
                                                               t  
    The optimal storage order for matrix B is if we flattened B   
    while for matrix A is if we flattened A.
    

    Transformation phase:
    =====================
    In this phase we define the new flatten matrices and replace the 
    access sites of the matrices with a new sequence of stmts which 
    creates the new access to the new flattened matrix.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tree-inline.h"
#include "tree-flow.h"
#include "tree-flow-inline.h"
#include "langhooks.h"
#include "hashtab.h"
#include "toplev.h"
#include "flags.h"
#include "ggc.h"
#include "debug.h"
#include "target.h"
#include "cgraph.h"
#include "diagnostic.h"
#include "timevar.h"
#include "params.h"
#include "fibheap.h"
#include "c-common.h"
#include "intl.h"
#include "function.h"
#include "basic-block.h"
#include "tree-iterator.h"
#include "tree-pass.h"
#include "opts.h"

#define SWAP(X, Y) do { int tmp = (X); (X) = (Y); (Y) = tmp; } while (0)

/* Information about the matrix indices in a specific access site.  */

struct index_info
{
  /* The number of the dimension this index belogs to.  Starting 
     with 1 for innermost dimension.  */
  int dim_num;
  /* A statment iterator which points to the base of this dim 
     (initial address accessed by this dim).
     For example for the innder dim of array arr[i][j] the initial
     address is arr;  while the initial address of the
     outer dimension is arr[i].  */
  block_stmt_iterator bsi_initial_addr;
  /* The index of this dimension.
     Example: given A[i][3], The constant "3" is the 
     index for the outer dimension, while i 
     is the index for the inner dimension.  */
  tree *index;
  /* True iff this dimension is iterated by the innermost nested loop
     containing this access site.  */
  bool iterated_by_inner_most_loop_p;
};


/* Information about matrix access site.  
   For example: if an access site of matrix arr is arr[i][j]
   the ACCESS_SITE_INFO structure will have the address
   of arr as it's stmt.  The INDEX_INFO will hold information about the
   initial address and index of each dimension.  */

struct
access_site_info  
{
  /* The statment which points to the base addr of this array.  */
  tree stmt;
  /* The function containing the access site.  */
  tree function_decl;
  /* Information about each index in this access site.  */ 
  varray_type index_info;
  /* True if this access site has unexpected pattern.  */
  bool unexpected_access_p;
  /* True if the indunction variable of the inner-most loop
     containing this access site is unknown.  */
  bool unknown_inner_most_loop_iv;
  
  /* Next in the list of access sites.  */
  struct access_site_info *next;
};


/* Information about the allocation site of a matrix.  */

struct
allocation_info 
{
  /* The call expression to malloc of the innermost dimension.  */
  tree call_exp;
  /* Points to the statement which checks if
     the allocation of the innermost dimension failed.  */ 
  tree *malloc_check_exp;
  /* Bitmap for the allocation site of the matrix's dimensions.  The bit in
     ALLOC_SITE_NOT_FOUND [I] is set iff the allocation site of the 
     i-th dimension is not found yet.  */
  sbitmap alloc_site_not_found;
  /* The location of the allocation site.  */
  tree function_decl;
  block_stmt_iterator bsi;
};


/* Information about matrix to flatten.  */

struct 
matrix_info 
{
  /* Decl tree of this matrix.  */
  tree decl;
  /* Number of dimensions.  */
  int num_dims;
  /* The type of the matrix elements.  */
  tree type;
  /* An array which holds for each dimension it's size.  Example: 
     DIMENSION_SIZE [i] is the size of the i-th dimension.  */
  varray_type dimension_size;
  /* An array which holds for each dimension it's hottness level.  Intuitively
     The hottness level of a dimension is a function of how many times it 
     was the most frequent accessed dimension in the highly executed 
     access sites of this matrix. 
     
     As computed by following equation:
     
     m    n  
     ---  --- 
     \    \   
     /    /   dim_hot_level[i] +=
     ---  --- 
     j    i 
       accss_s[j]->dim[i]->iter_by_inner_loop_p x count(j)
     
     Where n is the number of dims and m is the
     number of the matrix access sites.  */
  gcov_type *dim_hot_level;
  /* The dimension which has the maximum hottness level 
     recorded in the above dim_hot_level array.  If the 
     hottset dimension is not the outermost dimension we decide 
     to flatten the transposed matrix.  */
  int hottest_dim;
  /* The list of access to this matrix.  */
  struct access_site_info *access_l;
  /* Holds information about the allocation site.  */
  struct allocation_info *allocation_info;
  /* When NULL, consider this matrix for flattening.  When non-NULL,
     points to the explanation why matrix can not be 
     flatten.  */
  const char *reorg_failed_reason;
  /* True iff this matrix can be flattened.  */
  bool flatten_p;
  /* True iff it is profitable to flatten the transposed
     matrix.  We consider an optimzal access to a matrix
     as an access such that the most frequent dimension which is 
     accessed is the outermost dimension.  Thus in the cases
     where the most frequent accessed dimension is not the outermost
     we would consider to flatten the transposed matrix.  */
  bool flatten_transpose_matrix_p;
  /* The new declaration of the matrix allocated
     as one dimension array.  */
  tree flatten_decl;
  /* The map from the dimensions before the transposing process
     to their location after the transposing process.  The transposing 
     process is done by transposing each consecutive two dimnensions 
     from the hottset dimension till the outermost such that in the end 
     of this process the hottest dimension is the outermost dimension.  
     Example - if a[i][j][k] was transposed to a[j][k][i] 
     than the map is as follows:
     DIM_MAP [0] = 2
     DIM_MAP [1] = 0
     DIM_MAP [2] = 1  */
  int *dim_map;
  /* Auxiliary information stored with the matrix.  */
  void *aux;
};

/* Information about data to reorg.  */

struct data_reorg 
{
  htab_t matrices_to_reorg;     /* A hashtable of array to transform.  */
  void *aux;                    /* Auxiliary information.  */
};

/* Return a hash for MTT, which is really a "matrix_info *".  */

static hashval_t
mtt_info_hash (const void *mtt)
{
  return htab_hash_pointer (((struct matrix_info *) mtt)->decl);
}

/* Return true if MTT1 and MTT2 (which are really both of type 
   "matrix_info *") refer to the same decl.  */

static int
mtt_info_eq (const void *mtt1, const void *mtt2)
{
  const struct matrix_info *i1 = mtt1;
  const struct matrix_info *i2 = mtt2;
  
  if (i1->decl == i2->decl)
    return true;
  
  return false;
}

/* Walk tree TP and find all access to matrices recorded in DATA
   which is really struct data_reorg *.  Called via walk_tree.  */

static tree
find_matrix_access_r (tree *tp, int *walk_subtrees, void *data)
{
  tree t = *tp;
  struct data_reorg *data_reorg = (struct data_reorg *)data;
  splay_tree_node n;
  struct matrix_info tmpmi, *mi;
  tree_stmt_iterator tsi;
  
  switch (TREE_CODE (t))
    {
    case VAR_DECL:
      n = splay_tree_lookup (data_reorg->aux, (splay_tree_key) t);
      if (n)
	{
	  tmpmi.decl = (tree) n->value;
	  mi = htab_find (data_reorg->matrices_to_reorg, &tmpmi);
	  if (mi)
	    {
	      mi->reorg_failed_reason = 
		N_("Matrix may escape.");
	      *walk_subtrees = 0; 
	    }
	}
      
      tmpmi.decl = t;
      mi = htab_find (data_reorg->matrices_to_reorg, &tmpmi);
      if (mi)
	{
	  mi->reorg_failed_reason = N_("Matrix may escape.");
	  *walk_subtrees = 0; 
	}
      break;
      
    case STATEMENT_LIST:      
      /* Track current statement while finding CALL_EXPRs.  */
      for (tsi = tsi_start (*tp); !tsi_end_p (tsi); tsi_next (&tsi))
	walk_tree (tsi_stmt_ptr (tsi), find_matrix_access_r, data,
		   NULL);
      break;
      
    default:
      /* Save some cycles by not walking types and declaration as we
	 won't find anything useful there anyway.  */
      if (DECL_P (*tp) || TYPE_P (*tp))
	{
	  *walk_subtrees = 0;
	  break;
	}
    }
  return NULL;
}

/* Locate and record all matrices recorded in DATA_REORG
   which may be exposed outside of the current function.  */

static void
find_matrix_escape_sites (struct data_reorg *data_reorg)
{
  struct cgraph_node *c_node;
  
  for (c_node = cgraph_nodes; c_node; c_node = c_node->next)
    {
      struct function *func = DECL_STRUCT_FUNCTION (c_node->decl);
      basic_block bb;
      
      if (!c_node->next_clone)
	{
	  block_stmt_iterator bsi;  
	  
	  if (!func || !ENTRY_BLOCK_PTR_FOR_FUNCTION (func))
	    continue;
	  
	  FOR_EACH_BB_FN (bb, func)
	    {
	      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
		{
		  splay_tree_node n;
		  struct matrix_info tmpmi, *mi;
		  tree stmt = bsi_stmt (bsi);
		  
		  if (TREE_CODE (stmt) == MODIFY_EXPR)
		    {
		      tree op0, op1;
		      
		      op0 = TREE_OPERAND (stmt, 0);
		      if (UNARY_CLASS_P (op0))
			op0 = TREE_OPERAND (op0, 0);
		      op1 = TREE_OPERAND (stmt, 1);
		      if (UNARY_CLASS_P (op1))
			op1 = TREE_OPERAND (op1, 0);
		      
		      if (TREE_CODE (op0) == VAR_DECL 
			  && TREE_CODE (op1) == VAR_DECL)
			{	
			  tmpmi.decl = op1;
			  mi = htab_find (data_reorg->matrices_to_reorg, 
					  &tmpmi);
			  
			  /* Record the mapping between a matrix 
			     and the temporary
			     which holds it's value.  */     
			  if (!mi)
			    continue;
			  n = splay_tree_lookup (data_reorg->aux, 
						 (splay_tree_key) mi->aux); 
			  if (n)
			    {
			      if((tree) n->key == op0)
				continue;
			      if ((tree) n->key != op0)
				splay_tree_remove (data_reorg->aux, n->key);
			    }
			  
			  splay_tree_insert (data_reorg->aux, 
					     (splay_tree_key) op0,
					     (splay_tree_value) op1);
			  mi->aux = (tree)op0;
			  continue;
			}
		      
		      if (TREE_CODE (op1) == VAR_DECL)
			{
			  n = splay_tree_lookup (data_reorg->aux, 
						 (splay_tree_key) op1);
			  if (n)
			    {
			      tmpmi.decl = (tree) n->value;
			      mi = htab_find (data_reorg->matrices_to_reorg, 
					      &tmpmi);
			      if (mi)
				mi->reorg_failed_reason = 
				  N_("Matrix may escape.");
			      continue;
			    }
			}
		      
		      if (TREE_CODE (op1) == ADDR_EXPR)
			walk_tree (&TREE_OPERAND (op1, 0), find_matrix_access_r, 
				   data_reorg, NULL);
		      if (TREE_CODE (op1) == CALL_EXPR
			  && TREE_OPERAND (op1, 1))
			walk_tree (&TREE_OPERAND (op1, 1), find_matrix_access_r, 
				   data_reorg, NULL);
		    }
		  
		  if (TREE_CODE (stmt) == CALL_EXPR)
		    walk_tree (&TREE_OPERAND (stmt, 1), find_matrix_access_r, 
			       data_reorg, NULL);
		}
	    }
	}
    }
  
  return;
}

/* Given a VAR_DECL check it's type to determine wheather it is 
   a definition of a dynamic allocated matrix and therefore is 
   a suitable candidate for the matrix transposing optimization.
   Return NULL if VAR_DECL is not such decl. Otherwise, allocate 
   an MATRIX_INFO structure fill it with the relevant information 
   and return a pointer to it.  
   TODO: handle also staticaly defined arrays.  */

static struct matrix_info *
analyze_matrix_decl (struct data_reorg *data_reorg, tree var_decl)
{
  struct matrix_info *m_node, tmpmi, *mi;
  tree var_type;
  int dim_num = 0;
  
#ifdef ENABLE_CHECKING
  gcc_assert (data_reorg && data_reorg->matrices_to_reorg);
#endif
  
  if (TREE_CODE (var_decl) == PARM_DECL)
    var_type = DECL_ARG_TYPE (var_decl);
  else if (TREE_CODE (var_decl) == VAR_DECL)
    var_type = TREE_TYPE (var_decl);
  else
    return NULL;
  
  if (TREE_CODE (var_type) != POINTER_TYPE)
    return NULL;
  
  while (TREE_CODE (var_type) == POINTER_TYPE)
    {
      var_type = TREE_TYPE (var_type);
      dim_num++;
    }
  
  if (dim_num == 1)
    return NULL;

  if (!COMPLETE_TYPE_P (var_type)
      || TREE_CODE (TYPE_SIZE_UNIT (var_type)) != INTEGER_CST)
    return NULL;
  
  /* Check to see if this pointer is already in there.  */
  tmpmi.decl = var_decl;
  mi = htab_find (data_reorg->matrices_to_reorg, &tmpmi);
  
  if (mi)
    return NULL;    
  
  /* Record the matrix.  */
  
  m_node = (struct matrix_info *) 
    xcalloc (1, sizeof (struct matrix_info));
  m_node->decl = var_decl;
  m_node->num_dims = dim_num;
  m_node->type = var_type;
  m_node->allocation_info = (struct allocation_info *) 
    xcalloc (1, sizeof (struct allocation_info));
  m_node->allocation_info->alloc_site_not_found = sbitmap_alloc (dim_num);
  sbitmap_ones (m_node->allocation_info->alloc_site_not_found);
  
  return m_node;
} 

/* Find all potential matrices. 
   TODO: currently we handle only multidimensional 
   dynamiclly allocated arrays.  */

static void
find_matrices_decl (struct data_reorg *data_reorg)
{
  struct matrix_info *tmp;
  PTR *slot;
  struct cgraph_varpool_node *current_varpool;
  
  if (!data_reorg)
    abort ();
  
  /* For every global variable in the program:
     - Check to see if it's of a candidate type; and record it.  */ 
  
  for (current_varpool = cgraph_varpool_nodes_queue; current_varpool;
       current_varpool = current_varpool->next_needed)
    {
      tree var_decl = current_varpool->decl;
      
      if (!var_decl || TREE_CODE (var_decl) != VAR_DECL)
	continue;
      
      if (data_reorg->matrices_to_reorg)
	if ((tmp = analyze_matrix_decl (data_reorg, var_decl)))
	  {
	    slot = htab_find_slot (data_reorg->matrices_to_reorg, 
				   tmp, INSERT);
	    *slot = tmp;
	  }
    }
  
  return;
}


/* Given BSI set it to point to the last stmt which defines VAR_DECL
   in the basic block which contains this statement.  Return false 
   iff there is no such statement.  */ 

static bool
last_var_def_in_bb (block_stmt_iterator *bsi, tree var_decl)
{
  tree stmt, first_stmt;
  block_stmt_iterator bsi1;
  basic_block bb;
  
  if (TREE_CODE (var_decl) != VAR_DECL)
    abort ();
  
  stmt = bsi_stmt (*bsi);
  bb = bb_for_stmt (stmt);
  bsi1 = bsi_start (bb);
  first_stmt = bsi_stmt (bsi1);
  for (; stmt != first_stmt; bsi_prev (bsi))
    {
      stmt = bsi_stmt (*bsi);      
      if (TREE_CODE (stmt) == MODIFY_EXPR
	  && TREE_OPERAND (stmt, 0) == var_decl)
	return true;
    }
  
  return false;
}


/* Helper function for get_var_next_use, called via walk_tree.  */

static tree
find_var_uses_r (tree *tp, int *walk_subtrees, void *data)
{
  tree t = *tp;
  tree var_decl = data;
  
  if (TREE_CODE (t) == VAR_DECL)
    if (t == var_decl)
      return t;
  
  if (DECL_P (t) || TYPE_P (t))
    *walk_subtrees = 0;
  
  return NULL_TREE;
}


/* Given BSI set it to point to the next statement which uses VAR_DECL
   in the basic block which contains this statement. Return false iff there 
   is no such statment.  */

static bool
next_var_use_in_bb (block_stmt_iterator *bsi, tree var_decl)
{
  tree stmt = bsi_stmt (*bsi);
  
  if (TREE_CODE (var_decl) != VAR_DECL)
    abort ();
  
  for (; !bsi_end_p (*bsi); bsi_next (bsi))
    {
      stmt = bsi_stmt (*bsi);
      if (TREE_CODE (stmt) == MODIFY_EXPR)
	stmt = TREE_OPERAND (stmt, 1);
      
      if (walk_tree (&stmt, find_var_uses_r, var_decl, NULL))
	return true;
      
      stmt = bsi_stmt (*bsi);
      if (TREE_CODE (stmt) == MODIFY_EXPR
	  && TREE_CODE (TREE_OPERAND (stmt, 0)) != VAR_DECL)
	{
	  stmt = TREE_OPERAND (stmt, 0);
	  if (walk_tree (&stmt, find_var_uses_r, var_decl, NULL))
	    return true;
	}
    }
  
  return false;
}


/* Return true iff VAR is *not* defined in the current
   function.  */

static bool
var_not_declared_in_fn_p (tree var)
{
  struct function *func = DECL_STRUCT_FUNCTION (current_function_decl);
  basic_block bb;
 
  if (TREE_CODE (var) != VAR_DECL)
    abort ();
  
  FOR_EACH_BB_FN(bb, func) 
    {
      block_stmt_iterator bsi;
      tree stmt;
      
      bsi = bsi_start (bb);
      
      for (stmt = bsi_stmt (bsi); !bsi_end_p (bsi); bsi_next (&bsi))
        {
          stmt = bsi_stmt (bsi);
          if (TREE_CODE (stmt) == MODIFY_EXPR
              && TREE_OPERAND (stmt, 0) == var)
            return false;
        }
    }
  return true;
}

/* Record an unrecognized access site NEW_SITE of matrix MI.  */

static void
add_unexpected_access_to_matrix (struct matrix_info *mi,
				 struct access_site_info *new_site)
{
  mi->reorg_failed_reason = 
    N_("Matrix has unexpected access pattern");
  new_site->unexpected_access_p = true;
  new_site->next = mi->access_l;
  mi->access_l = new_site;
  return;
}

/* Return true iff the next use of the variable
   defined in the stmt pointed by BSI is
   in condition expression.  */

static bool
next_use_in_cond_exp (block_stmt_iterator bsi)
{
  tree stmt, var;
  
  if (bsi_end_p (bsi))
    return false;
  
  stmt = bsi_stmt (bsi);
  if (!TREE_OPERAND (stmt, 0))
    return false;
  var = TREE_OPERAND (stmt, 0);
  if (TREE_CODE (var) != VAR_DECL)
    return false;
  if (!next_var_use_in_bb (&bsi, var))
    return false;
  stmt = bsi_stmt (bsi);
  if (TREE_CODE (stmt) == COND_EXPR)
    return true;
  
  return false;
}

/* Figure if BSI points to an allocation site of the I-th dimension of the 
   matrix recorded in MI.  */

static bool
analyze_matrix_allocation_site (block_stmt_iterator bsi, struct matrix_info *mi,
				int i)
{
  block_stmt_iterator bsi1, bsi_s;
  tree stmt, first_stmt, indx, curr, call_expr, args, var, prev_stmt, rhs;
  basic_block bb, prev_bb;
  tree *arg_to_malloc;
  
  stmt = bsi_stmt (bsi);
  bb = bb_for_stmt (stmt);
  prev_bb = bb->prev_bb;
  bsi_s = bsi_start (bb);
  first_stmt = bsi_stmt (bsi_s);
  if (!mi->dimension_size)
    VARRAY_GENERIC_PTR_INIT (mi->dimension_size, 
			     10, "dimensions size");
  
  bsi1 = bsi;

  /* Locate the call for malloc.  */
  
  stmt = bsi_stmt (bsi1);
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return false;
  
  var = TREE_OPERAND (stmt, 1);
  while (TREE_CODE (TREE_OPERAND (stmt, 1)) != CALL_EXPR)
    {
      if (TREE_CODE (var) != VAR_DECL)
	return false;
      
      if (!last_var_def_in_bb (&bsi1, var))
	return false;
      prev_stmt = stmt;
      stmt = bsi_stmt (bsi1);
      if (stmt == prev_stmt)
	return false;
      if (TREE_CODE (stmt) != MODIFY_EXPR)
	return false;
      var = TREE_OPERAND (stmt, 1);
      if (UNARY_CLASS_P (var))
	var = TREE_OPERAND (var, 0);
    }
  
  /* Found a call; now verify it is a call to malloc.  */
  stmt = bsi_stmt (bsi1);
  if (!(call_expr_flags (TREE_OPERAND (stmt, 1)) & ECF_MALLOC))
    return false;
  
  /* Get the index passed as an argument to malloc.  */
  call_expr = TREE_OPERAND (stmt, 1);
  mi->allocation_info->call_exp = call_expr;
  args = TREE_OPERAND (call_expr, 1);
  indx =  TREE_VALUE (args);
  
  if (TREE_CODE (indx) == VAR_DECL)
    {
      curr = stmt;
      /* Find the index passed to malloc.  */
      while (DECL_ARTIFICIAL (indx))
	{
	  if (!last_var_def_in_bb (&bsi1, indx))
	    {
	      mi->reorg_failed_reason = N_("Unrecognized allocation site");
	      return true;
	    }
	  prev_stmt = curr; 
	  curr = bsi_stmt (bsi1);
	  if (prev_stmt == curr)
	    return false;
	  indx = TREE_OPERAND (curr, 1);
	  if (UNARY_CLASS_P (indx))
	    indx = TREE_OPERAND (indx, 0);
	  else if (TREE_CODE (indx) == MULT_EXPR)
	    indx = TREE_OPERAND (indx, 0);
	  if (TREE_CODE (indx) != VAR_DECL)
	    return false;
	  stmt = bsi_stmt (bsi1);
	  if (TREE_CODE (stmt) != MODIFY_EXPR)
	    return false;
	}
      
      stmt = bsi_stmt (bsi1);
      arg_to_malloc = &TREE_OPERAND (stmt, 1);
      
      /* Check a matrix is allocated by validate that the argument
	 passed to malloc is an invariant.  
	 TODO: This check is currently done on the whole function;
	 it can be made on the loop containing this statment.  */
      
      if (i != 0)
	{
	  
	  rhs = TREE_OPERAND (stmt, 1);
	  if (UNARY_CLASS_P (rhs))
	    rhs = TREE_OPERAND (rhs, 0);
	  
	  if (TREE_CODE (rhs) != VAR_DECL)
	    {
	      mi->reorg_failed_reason = 
		N_("Pointer not allocated as a matrix.");
	      return true;
	    }
	  /* Make sure we are really alocation a matrix.  */
	  if (!var_not_declared_in_fn_p (rhs))
	    {
	      mi->reorg_failed_reason = 
		N_("Pointer not allocated as a matrix.");
	      return true;
	    }
	}
    }
  else if (TREE_CODE (indx) == INTEGER_CST)
    arg_to_malloc = &TREE_VALUE (args);
  else
    {
      mi->reorg_failed_reason = N_("Unrecognized allocation site");
      return true;
    }
  
  if (i == 0)
    {
      /* Record the location of the malloc of the innermost dimension.  */
      mi->allocation_info->function_decl = current_function_decl;
      mi->allocation_info->bsi = bsi;
    }
 
  if (!TEST_BIT (mi->allocation_info->alloc_site_not_found, i))
    {
      mi->reorg_failed_reason =
	N_("One of the dimensions is allocated more than once.");
      return true;
    }
  
  RESET_BIT (mi->allocation_info->alloc_site_not_found, i);
  /* Record the dimension size which is passed to malloc.  */
  VARRAY_PUSH_GENERIC_PTR (mi->dimension_size, 
			   arg_to_malloc);
  
  return true;
}


/* Given statment pointed by BSI; locate the induction variable's
   increament statement which is assumed to belong to the inner-most 
   loop containing this statment.  FIXME: Analyze the statment using 
   cfg (or loop utility when available).  */

static tree
get_incr_stmt (block_stmt_iterator bsi)
{
  tree stmt;
  basic_block bb = bb_for_stmt (bsi_stmt (bsi));
  
  while (bb)
    {
      for (; !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  stmt = bsi_stmt (bsi);
	  if (TREE_CODE (stmt) != MODIFY_EXPR)
	    continue;
	  if (TREE_CODE (TREE_OPERAND (stmt, 1)) != PLUS_EXPR)
	    continue;
	  if (TREE_OPERAND (stmt, 0) != TREE_OPERAND (TREE_OPERAND (stmt, 1), 0))
	    continue;
	  if (TREE_CODE (TREE_OPERAND (stmt, 0)) != VAR_DECL)
	    continue;
	  
	  return stmt;
	}
      bb = bb->next_bb;
      if (bb)
	bsi = bsi_start (bb);
    }
  
  return NULL_TREE;
}


/* Given BSI which points to the I-th dimension initial address
   find the index to this dimension.  INCR_INDEX is the induction
   variable of the inner-most loop which contains this access.  
   The offsets can be constant, zero or variable.  */

static struct index_info *
analyze_index (block_stmt_iterator bsi_initial_addr, int i, 
	       tree incr_index)
{
  tree stmt, dim_initial_addr, use, var;
  struct index_info *dim = NULL;
  basic_block bb;
  block_stmt_iterator bsi;
  
  if (bsi_end_p (bsi_initial_addr)) 
    return NULL;
  
  stmt = bsi_stmt (bsi_initial_addr);
  bb = bb_for_stmt (stmt);
  
  bsi = bsi_initial_addr;
  stmt = bsi_stmt (bsi);
  if (!TREE_OPERAND (stmt, 0))
    return NULL;
  
  dim_initial_addr = TREE_OPERAND (stmt, 0);
  
  /* Find the next use of this dim base address.  */
  if (!next_var_use_in_bb (&bsi, dim_initial_addr))
    return NULL;
  
  use = bsi_stmt (bsi);
  
  if (TREE_CODE (use) == MODIFY_EXPR)
    {
      tree rhs, lhs, offset, indx;
      
      /* Allocate a new index_info.  */
      dim = (struct index_info *)
	xcalloc (1, sizeof (struct index_info));
      dim->dim_num = i;
      dim->bsi_initial_addr = bsi_initial_addr;
      rhs = TREE_OPERAND (use, 1);
      lhs = TREE_OPERAND (use, 0);
      
      if ((TREE_CODE (rhs) == INDIRECT_REF
	   && TREE_OPERAND (rhs, 0) == dim_initial_addr)
	  || (TREE_CODE (lhs) == INDIRECT_REF
	      && TREE_OPERAND (lhs, 0) == dim_initial_addr))
	{
	  /* The index of this dim is zero.  */
	  dim->iterated_by_inner_most_loop_p = false;
	  dim->index = NULL;
	}
      if (TREE_CODE (rhs) == PLUS_EXPR)
	{
	  tree op1, op2;
	  
	  /* dim base addr + offset  */
	  op1 = TREE_OPERAND (rhs, 0);
	  op2 = TREE_OPERAND (rhs, 1);
	  /* Record the information about the offset
	     and the base address.  */
	  
	  if (op1 == dim_initial_addr)
	    {
	      offset = op2;
	      dim->index = &TREE_OPERAND (TREE_OPERAND (use, 1), 1);
	    }
	  else
	    {
	      offset = op1;
	      dim->index = &TREE_OPERAND (TREE_OPERAND (use, 1), 0);
	    }
	  
	  if (TREE_CODE (offset) == INTEGER_CST)
	    {
	      /* The index is a constant.  */
	      dim->iterated_by_inner_most_loop_p = false;
	    }
	  else
	    {
	      /* The index is a variable.  Figure
		 if this var is the induction variable
		 of the inner-most loop containing this access
		 site.  TODO: Replace this analysis when loop 
		 utility is available.  */
	      
	      if (!last_var_def_in_bb (&bsi, offset))
		return NULL;
	      offset = bsi_stmt (bsi);
	      indx = TREE_OPERAND (offset, 1);
	      if (UNARY_CLASS_P (indx))
		indx = TREE_OPERAND (indx, 0);
	      if (TREE_CODE (indx) != VAR_DECL)
		return NULL;
	      
	      if (!last_var_def_in_bb (&bsi, indx))
		return NULL;
	      stmt = bsi_stmt (bsi);
	      rhs = TREE_OPERAND (stmt, 1);
	      
	      /* Find the index of this dimension.
		 The index may be scaled by a factor
		 of the matrix's elements size before added to base address
		 of this dimension; For example, the access arr[i]  
		 is translated to:
		 
		 offset = index * scale;
		 access = arr + offset;
	      */
	      
	      if (rhs
		  && TREE_CODE (rhs) == MULT_EXPR)
		{
		  if (TREE_CODE (TREE_OPERAND (rhs, 1)) == VAR_DECL)
		    var = TREE_OPERAND (rhs, 1);
                  else
		    var = TREE_OPERAND (rhs, 0);
		  
		  if (!last_var_def_in_bb (&bsi, var))
		    return NULL;
		  stmt = bsi_stmt (bsi);
		  dim->index = &TREE_OPERAND (stmt, 1);
		  /* Record the index.  */
		  if (UNARY_CLASS_P (*dim->index))
		    dim->index = &TREE_OPERAND (*dim->index, 0);
		  
		  if (incr_index && incr_index == *dim->index)
		    dim->iterated_by_inner_most_loop_p = true;
		}
	      else
		{
		  dim->index = &TREE_OPERAND (stmt, 1);
		  if (UNARY_CLASS_P (*dim->index))
                    dim->index = &TREE_OPERAND (*dim->index, 0);
		  
		  if (incr_index && incr_index == *dim->index)
		    dim->iterated_by_inner_most_loop_p = true;
		}
	    }
	}   
    }
  
  return dim;
}


/* Given access site pointed by BSI of the matrix MI preform an analysis
   to check if it is a recognized access site.  */

static void
analyze_matrix_access_site (block_stmt_iterator bsi, 
			    struct matrix_info *mi)
{
  struct index_info *dim = NULL;
  struct access_site_info *new_site;
  int dim_c = 0;
  tree stmt, incr_stmt, initial_addr2, lhs, rhs;
  block_stmt_iterator bsi1 = bsi, bsi2, bsi_initial_addr2;
  
  stmt = bsi_stmt (bsi);
  if (next_use_in_cond_exp (bsi))
    {
      /* If the next use of the matrix initial address is in
	 condition expression do not count it as access site.  */
      bsi2 = bsi;
      next_var_use_in_bb (&bsi2, TREE_OPERAND (stmt, 0));
      stmt = bsi_stmt (bsi2);
      mi->allocation_info->malloc_check_exp = &TREE_OPERAND (stmt, 0);        
    }
  
  new_site = (struct access_site_info *)
    xcalloc (1, sizeof (struct access_site_info));
  
  VARRAY_GENERIC_PTR_INIT (new_site->index_info, 10, "access site index info");
  
  /* Locate the induction variable of the inner-most loop
     containing this access site.  */
  incr_stmt = get_incr_stmt (bsi);
  
  while (dim_c < mi->num_dims)
    {
      if (incr_stmt)
        dim = analyze_index (bsi1, dim_c, TREE_OPERAND (incr_stmt, 0));
      else
	{
	  new_site->unknown_inner_most_loop_iv = true;
	  dim = analyze_index (bsi1, dim_c, incr_stmt);
	}
      
      if (!dim)
	{
	  /* Check if the next use of the address is in
	     condition expression.  */
          if (next_use_in_cond_exp (bsi1))
            return;
	  
	  /* Unexpected accesss pattern.  */
          add_unexpected_access_to_matrix (mi, new_site);
          return;
	}
      
      /* Record the index info of the current dimension.  */
      VARRAY_PUSH_GENERIC_PTR (new_site->index_info, dim);
      dim_c++;
      
      if (TREE_CODE (TREE_OPERAND (bsi_stmt (bsi1), 0)) != VAR_DECL
	  || !next_var_use_in_bb (&bsi1, TREE_OPERAND (bsi_stmt (bsi1), 0)))
	{
	  add_unexpected_access_to_matrix (mi, new_site);
          return;
	}
    
      if (dim->index)
	next_var_use_in_bb (&bsi1, TREE_OPERAND (bsi_stmt (bsi1), 0));

      if (dim_c < mi->num_dims
          && (!TREE_OPERAND (bsi_stmt (bsi1), 1)
	  || TREE_CODE (TREE_OPERAND (bsi_stmt (bsi1), 1)) != INDIRECT_REF))
	{
	  /* It can be the matrix allocation site.  */
	  if (analyze_matrix_allocation_site (bsi1, mi, dim_c))
	    return;
	  add_unexpected_access_to_matrix (mi, new_site);
          return;
	}
    }
  
  /* Double check all the dimensions were recognized.  */
  if (VARRAY_ACTIVE_SIZE (new_site->index_info) != (unsigned) mi->num_dims)
    {
      add_unexpected_access_to_matrix (mi, new_site);
      return;
    }
  
  /* Preform a check to validate the the address of this elemnent
     is not taken.  */
  
  bsi_initial_addr2 = dim->bsi_initial_addr;
  bsi2 = dim->bsi_initial_addr;
  initial_addr2 = TREE_OPERAND (bsi_stmt (bsi_initial_addr2), 0);
  next_var_use_in_bb (&bsi2, initial_addr2);
  
  if (dim->index)
    {
      if (!TREE_OPERAND (bsi_stmt (bsi2), 0)
	  || TREE_CODE (TREE_OPERAND (bsi_stmt (bsi2), 0)) != VAR_DECL)
	{
	  add_unexpected_access_to_matrix (mi, new_site);
          return;
	}
      next_var_use_in_bb (&bsi2, TREE_OPERAND (bsi_stmt (bsi2), 0));
    }
  lhs = TREE_OPERAND (bsi_stmt (bsi2), 1);
  rhs = TREE_OPERAND (bsi_stmt (bsi2), 0);
  
  if (TREE_CODE (lhs) != INDIRECT_REF 
      && TREE_CODE (rhs) != INDIRECT_REF)
    {
      /* Unexpected accesss pattern.  */
      add_unexpected_access_to_matrix (mi, new_site);
      return;
    }
  
  new_site->function_decl = current_function_decl;
  new_site->stmt = stmt;
  
  /* Add the new site to the list.  */
  new_site->next = mi->access_l;
  mi->access_l = new_site;
}


/* Find access sites to matrices which recoreded in DATA_REORG 
   in basic block BB.  */

static void
find_matrix_access_in_bb (struct data_reorg *data_reorg, basic_block bb)
{
  block_stmt_iterator bsi;
  struct matrix_info tmpmi, *mi;
  
 /* Go over the basic block statements and create a linked list of
     the accessed fields.  */
  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
    {           
      tree stmt = bsi_stmt (bsi);
            
      if (TREE_CODE (stmt) != MODIFY_EXPR)
        continue;

      if (TREE_CODE (TREE_OPERAND (stmt, 1)) == VAR_DECL)
        {   
          /* Check to see if a matrix is accessed.  */
          tmpmi.decl = TREE_OPERAND (stmt, 1);
          mi = htab_find (data_reorg->matrices_to_reorg, &tmpmi);
          if (mi && !mi->reorg_failed_reason)
            analyze_matrix_access_site (bsi, mi);
        }                   
      if (TREE_CODE (TREE_OPERAND (stmt, 0)) == VAR_DECL)
        { 
          tmpmi.decl = TREE_OPERAND (stmt, 0);
          mi = htab_find (data_reorg->matrices_to_reorg, &tmpmi);
          if (mi && !mi->reorg_failed_reason)
            analyze_matrix_allocation_site (bsi, mi, 0);
        }
    }   
  
  return;
}


/* Build a list of all the references to a matrices recoreded 
   in DATA_REORG in each one of the basic blocks of function F.  */

static void
build_matrix_access_list_in_func (struct data_reorg *data_reorg, 
				  struct function *f)
{
  basic_block bb;
  
  if (!f || !ENTRY_BLOCK_PTR_FOR_FUNCTION (f))
    return;
  
  current_function_decl = f->decl; 
  /* Build the access list in each one of the basic blocks.  */
  FOR_EACH_BB_FN (bb, f)
    {
      find_matrix_access_in_bb (data_reorg, bb);
    }
}

/* Given struct DATA_REORG locate and record all 
   references to matrices in the program.  */

static void
find_references_to_matrices (struct data_reorg *data_reorg)
{
  struct cgraph_node *c_node;
  
  for (c_node = cgraph_nodes; c_node; c_node = c_node->next)
    {
      struct function *func = DECL_STRUCT_FUNCTION (c_node->decl);
      
      /* There's no point in re-analyzing the same function definition
	 more than once... */
      
      if (!c_node->next_clone)
	build_matrix_access_list_in_func (data_reorg, func);
    } 
  current_function_decl = NULL;
  
  return;
}


/* Given matrix MI, allocate a new one dimensional array which is the
   flatten matrix of MI.  */

static void
flatten_allocation_site (struct matrix_info *mi)
{
  tree modify_stmt, list_p = NULL_TREE, new_var;
  tree call_exp, convert_exp, mult_exp;
  varray_type alloc_s, tmp_dim_size;
  struct cgraph_varpool_node *old_decl_node, *node;
  int i;
  struct cgraph_edge *edge;
  
  VARRAY_TREE_INIT (tmp_dim_size, 10, "tmp vars of dim size");
  
  current_function_decl = mi->allocation_info->function_decl;
  cfun = DECL_STRUCT_FUNCTION (current_function_decl);
  
  alloc_s = mi->dimension_size;
  
  /* For each matrix dimension generate an assigment statement
     which defines a temporary variable to holds it's size.
     Example: if the current dimension's size is i, then generate
     the assignment - i_tmp = i.  */
  
  for (i = 0; i < mi->num_dims; i++)
    {
      tree tmp_dimension_size, dimension_size, *dim_size;
      
      dim_size = VARRAY_GENERIC_PTR (alloc_s, i);
      
      /* Find the argument passed to each dim's malloc call
	 as this argument is the size of the dimension.  */
      
      if (TREE_CODE (*dim_size) == INTEGER_CST)
	{
	  int size, elem_type_size, pointer_type_size;
	  
	  elem_type_size = 
	    TREE_INT_CST_LOW (TYPE_SIZE_UNIT (mi->type));
	  pointer_type_size = GET_MODE_SIZE (ptr_mode);
	  
	  size = TREE_INT_CST_LOW (*dim_size);
	  if (i == mi->num_dims - 1)
	    dimension_size = build_int_cst (unsigned_type_node,
					    (size / elem_type_size));
	  else
	    dimension_size = build_int_cst (unsigned_type_node,
					    (size / pointer_type_size));
	}				
      else 
	dimension_size = *dim_size;
      
      tmp_dimension_size = 
	create_tmp_var (TREE_TYPE (dimension_size), "flat");
      VARRAY_PUSH_TREE (tmp_dim_size, tmp_dimension_size);
      modify_stmt = 
	build2 (MODIFY_EXPR, TREE_TYPE (tmp_dimension_size),
		tmp_dimension_size, dimension_size);
      
      append_to_statement_list (modify_stmt, &list_p);
    }
  
  /* Generate the declaration of the new flatten array.  */
  mi->flatten_decl = copy_node (mi->decl);
  DECL_ARTIFICIAL (mi->flatten_decl) = 1;
  DECL_NAME (mi->flatten_decl) =
    create_tmp_var_name (IDENTIFIER_POINTER (DECL_NAME (mi->decl)));
  TREE_TYPE (mi->flatten_decl) = 
    build_pointer_type_for_mode (mi->type,
				 TYPE_MODE (TREE_TYPE (mi->decl)),
				 TYPE_REF_CAN_ALIAS_ALL (TREE_TYPE (mi->decl)));
  
  old_decl_node = cgraph_varpool_node (mi->decl);
  node = cgraph_varpool_node (mi->flatten_decl);
  if (old_decl_node->needed)
    cgraph_varpool_mark_needed_node (node);
  cgraph_varpool_finalize_decl (mi->flatten_decl);
  
  /* Generate the size of the new flatten matrix which
     is the multiplication of all the sizes of the matrix dimensions as 
     described in the following equation:
     
        n
     -------
      |   | 
      |   |  size of dimension (i)
      |   |
      i = 0
      
      Where n is the number of dimensions.  */

  /* Intialize the multiplication.  */
  new_var = integer_one_node;
  
  for (i = 0 ;i < mi->num_dims; i++)
    {
      tree tmp_dimension_size;
      
      tmp_dimension_size = VARRAY_GENERIC_PTR (tmp_dim_size, i);
     
      mult_exp = build2 (MULT_EXPR, 
			 TREE_TYPE (new_var), 
			 new_var, tmp_dimension_size);
      
      new_var = 
	create_tmp_var (TREE_TYPE (new_var), "flat");
      
      modify_stmt = 
	build2 (MODIFY_EXPR, TREE_TYPE (new_var), 
		new_var, mult_exp);
      append_to_statement_list (modify_stmt, &list_p);
    }
  
  mult_exp = build2 (MULT_EXPR, TREE_TYPE (new_var),
                     new_var, TYPE_SIZE_UNIT (mi->type));
  
  new_var = create_tmp_var (TREE_TYPE (new_var), "flat");
  
  modify_stmt = build2 (MODIFY_EXPR, TREE_TYPE (new_var),
                        new_var, mult_exp);
  append_to_statement_list (modify_stmt, &list_p);
  
  edge = cgraph_edge (cgraph_node (current_function_decl), 
                      mi->allocation_info->call_exp);
  /* Send the size we generate to malloc and copy also the edge 
     in the call graph.  */
  call_exp = copy_node (mi->allocation_info->call_exp);
  if (edge)
    cgraph_clone_edge (edge, cgraph_node (current_function_decl), 
                       call_exp, REG_BR_PROB_BASE);
  
  TREE_OPERAND (call_exp, 1) = copy_node (TREE_OPERAND (call_exp, 1));
  TREE_VALUE (TREE_OPERAND (call_exp, 1)) = new_var;
  new_var = create_tmp_var (ptr_type_node, "flat");
  modify_stmt = build2 (MODIFY_EXPR, TREE_TYPE (call_exp),
                        new_var, call_exp);
  append_to_statement_list (modify_stmt, &list_p);
  convert_exp = build1 (CONVERT_EXPR, 
			TREE_TYPE (mi->flatten_decl), new_var);
  
  new_var = create_tmp_var (TREE_TYPE (mi->flatten_decl), "flat");
  modify_stmt = build2 (MODIFY_EXPR, TREE_TYPE (mi->flatten_decl),
                        new_var, convert_exp);
  append_to_statement_list (modify_stmt, &list_p);
  
  modify_stmt = build2 (MODIFY_EXPR, TREE_TYPE (mi->flatten_decl),
                        mi->flatten_decl, new_var);
  append_to_statement_list (modify_stmt, &list_p);
  
  bsi_insert_before (&mi->allocation_info->bsi, list_p, BSI_SAME_STMT);

  /* If there is a check to see if the call to malloc was failed
     also add the new array to this check.  */
  if (mi->allocation_info->malloc_check_exp)
    {
      tree cond_exp, new_cond, lhs, rhs;
      
      cond_exp = *mi->allocation_info->malloc_check_exp;
      new_cond = copy_node (cond_exp);
      lhs = TREE_OPERAND (new_cond, 0);
      rhs = TREE_OPERAND (new_cond, 1);
      if (TREE_CODE (lhs) == VAR_DECL)
	TREE_OPERAND (new_cond, 0) = new_var;
      else 
	TREE_OPERAND (new_cond, 1) = new_var;
      new_cond = build2 (TRUTH_OR_EXPR, TREE_TYPE (cond_exp), cond_exp, new_cond);
      *mi->allocation_info->malloc_check_exp = new_cond;
    }
  
  VARRAY_CLEAR (tmp_dim_size);
}


/* Given the access site ACCESS_S of the matrix MI;
   generate a new sequence of statements to access
   the new flatten matrix of MI.  */

static void
flatten_matrix_access_site (struct access_site_info *access_s, 
			    struct matrix_info *mi)
{
  block_stmt_iterator bsi_use, bsi, bsi_link_before;
  varray_type alloc_s, tmp_dim_size;
  tree stmt, convert_exp, plus_exp, new_var = NULL_TREE, 
    list_p = NULL_TREE, accumulator, modify_stmt, mult_exp;
  tree *final_address, rhs, lhs, initial_addr, tmp_arr;
  struct index_info *indx_info, *indx_info_of_outermost_dim;
  int i, j, n;
  
  VARRAY_TREE_INIT (tmp_dim_size, 10, "tmp vars of dim size");
  
  current_function_decl = access_s->function_decl;
  cfun = DECL_STRUCT_FUNCTION (current_function_decl);  

  alloc_s = mi->dimension_size;
  
  /* For each matrix dimension generate an assigment statement
     which defines a temporary variable to holds it's size.
     Example: if the current dimension's size is i, then generate
     the assignment - i_tmp = i.  */

  for (i = 0; i < mi->num_dims; i++)
    {
      tree tmp_dimension_size, dimension_size, *dim_size;

      dim_size = VARRAY_GENERIC_PTR (alloc_s, i);
  
      if (TREE_CODE (*dim_size) == INTEGER_CST)
	{
	  int size, elem_type_size, pointer_type_size;
	  
	  elem_type_size = 
	    TREE_INT_CST_LOW (TYPE_SIZE_UNIT (mi->type));
	  pointer_type_size = GET_MODE_SIZE (ptr_mode);
	  
	  size = TREE_INT_CST_LOW (*dim_size);
	  if (i == mi->num_dims - 1)
	    dimension_size = build_int_cst (unsigned_type_node,
					    (size / elem_type_size));
	  else
	    dimension_size = build_int_cst (unsigned_type_node,
					    (size / pointer_type_size));
	}				
      else 
	dimension_size = *dim_size;

      tmp_dimension_size = 
	create_tmp_var (TREE_TYPE (dimension_size), "flat");
      VARRAY_PUSH_TREE (tmp_dim_size, tmp_dimension_size);
      modify_stmt = 
	build2 (MODIFY_EXPR, TREE_TYPE (tmp_dimension_size),
		tmp_dimension_size, dimension_size);
      
      append_to_statement_list (modify_stmt, &list_p);
    }
  
  /* Generate the sequence of expressions which calculate the offset 
     to the new flatten matrix.  The new offset is the summation
     of each dimension's index multiply with the sizes of all the dimensions 
     which are higher then him; as discribed in the following
     equation:
     
      n            n
     ---        -------
     \           |   |
     /     i *   |   | size of dimension (j) 
     ---         |   |
    i = 0      j = i + 1

    Where n is the matrix number of dimensions.  Example: the offset 
    to arr[i][j] is (i * n + j).  */

  /* Initialze the summation.  */

  accumulator = integer_zero_node;
  
  for (i = 0; i < mi->num_dims; i++)
    {
      tree dimension_index;

      /* Get the index of the i-th dimension.  */
      if (mi->dim_map)
        n = mi->dim_map [i];
      else
        n = i;

      indx_info = VARRAY_GENERIC_PTR (access_s->index_info, n);

      if (!indx_info->index)
	dimension_index = build_int_cst (unsigned_type_node, 0);
      else if (TREE_CODE (*indx_info->index) == VAR_DECL)
	dimension_index = *indx_info->index;
      else 
	{
	  int offset, pointer_type_size, elem_type_size;
	  
	  pointer_type_size = GET_MODE_SIZE (ptr_mode);
	  elem_type_size = 
	    TREE_INT_CST_LOW (TYPE_SIZE_UNIT (mi->type));
	  
	  offset = TREE_INT_CST_LOW (*indx_info->index);
	  
	  if (n == mi->num_dims - 1)
	    dimension_index = build_int_cst (unsigned_type_node,
					     (offset / elem_type_size));
	  else
	    dimension_index = build_int_cst (unsigned_type_node,
					     (offset / pointer_type_size));
	}
      
      /* Multiply the index with the size of all it's higher dimwnsions.  */

      /* Initialize the multiplication.  */
      new_var = dimension_index;
      
      for (j = i + 1 ;j < mi->num_dims; j++)
	{
	  tree tmp_dimension_size;
          if (mi->dim_map)
	    n = mi->dim_map [j];
          else
            n = j;

	  tmp_dimension_size = VARRAY_GENERIC_PTR (tmp_dim_size, n);

	  mult_exp = build2 (MULT_EXPR, 
			     TREE_TYPE (new_var), 
			     new_var, tmp_dimension_size);
	  
	  new_var = 
	    create_tmp_var (TREE_TYPE (new_var), "flat");
	  
	  modify_stmt = 
	    build2 (MODIFY_EXPR, TREE_TYPE (new_var), 
		    new_var, mult_exp);
	  append_to_statement_list (modify_stmt, &list_p);
	}

      plus_exp = build2 (PLUS_EXPR, TREE_TYPE (new_var),
			 accumulator, new_var);
      
      accumulator = create_tmp_var (TREE_TYPE (accumulator), "flat");
      modify_stmt = build2 (MODIFY_EXPR, void_type_node, 
			    accumulator, plus_exp);
      
      append_to_statement_list (modify_stmt, &list_p);
      
    }
  
  /* Multiply the access with the matrix element size.  */ 
  mult_exp = build2 (MULT_EXPR, TREE_TYPE (accumulator), 
		     TYPE_SIZE_UNIT (mi->type),
		     accumulator);
  new_var = 
    create_tmp_var (TREE_TYPE (new_var), "flat");
  modify_stmt = build2 (MODIFY_EXPR, void_type_node,
			new_var, mult_exp);
  
  append_to_statement_list (modify_stmt, &list_p);
  
  /* The expressions were generated. Now find the use of this access
     so it can be replaced with the new offset.  */

  indx_info_of_outermost_dim = VARRAY_GENERIC_PTR (access_s->index_info, mi->num_dims - 1);
  bsi = indx_info_of_outermost_dim->bsi_initial_addr;
  initial_addr = TREE_OPERAND (bsi_stmt (bsi), 0);
  next_var_use_in_bb (&bsi, initial_addr);
  bsi_use = bsi;
  
  if (indx_info_of_outermost_dim->index)
    next_var_use_in_bb (&bsi, TREE_OPERAND (bsi_stmt (bsi_use), 0));
  
  bsi_link_before = bsi;
  stmt = bsi_stmt (bsi);
  lhs = TREE_OPERAND (stmt, 0);
  rhs = TREE_OPERAND (stmt, 1);
  if (!indx_info_of_outermost_dim->index)
    bsi_use = indx_info_of_outermost_dim->bsi_initial_addr;
  
  if (TREE_CODE (lhs) == INDIRECT_REF
      && (TREE_OPERAND (bsi_stmt (bsi_use), 0) 
	  == TREE_OPERAND (lhs, 0)))
    final_address = &TREE_OPERAND (TREE_OPERAND (stmt, 0), 0);
  else if (TREE_CODE (rhs) == INDIRECT_REF
	   && (TREE_OPERAND (bsi_stmt (bsi_use), 0) 
	       == TREE_OPERAND (rhs, 0)))
    final_address = &TREE_OPERAND (TREE_OPERAND (stmt, 1), 0);
  else
    abort ();

  /* Finally add the offset to the initial address of the new flatten
     matrix.  */
  
  tmp_arr = create_tmp_var (TREE_TYPE (mi->flatten_decl), "tmp_arr");
  modify_stmt = build2 (MODIFY_EXPR, void_type_node,
                        tmp_arr, mi->flatten_decl);
  append_to_statement_list (modify_stmt, &list_p);

  convert_exp = 
    build1 (CONVERT_EXPR, TREE_TYPE (*final_address), new_var);
  
  new_var = create_tmp_var (TREE_TYPE (*final_address), "flat");
  modify_stmt = build2 (MODIFY_EXPR, void_type_node,
			new_var, convert_exp);
  append_to_statement_list (modify_stmt, &list_p);
  
  plus_exp = build2 (PLUS_EXPR, TREE_TYPE (new_var),
                     new_var, tmp_arr); 
  modify_stmt = build2 (MODIFY_EXPR, void_type_node,
			*final_address, plus_exp); 
  append_to_statement_list (modify_stmt, &list_p);
  
  bsi_insert_before (&bsi_link_before, list_p, BSI_SAME_STMT);
  
  /* Clean up.  */
  cfun = NULL;
  VARRAY_CLEAR (tmp_dim_size);
}


/* Flatten matrix. Callbacks for htab_traverse. */

static int
apply_matrix_flattening (void **slot, void *data ATTRIBUTE_UNUSED)
{
  struct matrix_info *mi = *slot;   /* The matrix to flatten.  */
  struct access_site_info *access_s;
  
  if (!mi->flatten_p)
    return 1;

  /* Flatten allocation site.  */
  flatten_allocation_site (mi);
  
  /* Flatten accesses sites.  */
  for (access_s = mi->access_l; access_s; access_s = access_s->next)
    flatten_matrix_access_site (access_s, mi);
  
  /* Continue traversing the hash table.  */
  return 1;
}


/* Transpose matrix by prompoting the hottset dimension to be 
   the outermost dimension of this matrix.  If the hottset dimension is 
   not the outermost dimension we transpose 
   each subsequent two dimensions starting from the hottest dim 
   such that in the end of this process the dimension which is 
   the hottest is the outermost.  Callbacks for htab_traverse. */

static int
transpose_matrix (void **slot, void *data ATTRIBUTE_UNUSED)
{
  struct matrix_info *mi = *slot;   /* The matrix to transpose.  */
  int i;
  
  if (!mi->flatten_p || !mi->flatten_transpose_matrix_p)
    return 1;
  
  mi->dim_map = xcalloc (mi->num_dims + 1, sizeof (int));
  
  /* Intialize the dimension map.  */
  
  for (i = 0; i < mi->num_dims; i++)
    mi->dim_map [i] = i;
  
  /* Promote the hottest dim to be the outermost dimension of
     this matrix.  */
  for (i = mi->hottest_dim; i < mi->num_dims - 1; i++)
    SWAP (mi->dim_map [i], mi->dim_map [i+1]);
  
  /* Continue traversing the hash table.  */
  return 1; 
}

/* Decide whether the matrix is suitable for flattening.
   If it is suitable decide based on the profiling information
   whether to flatten the transposed matrix or the matrix as 
   the user defined it.  Callbacks for htab_traverse.  */

static int
decide_matrix_flattening (void **slot, void *data ATTRIBUTE_UNUSED)
{
  struct matrix_info *mi = *slot;
  struct access_site_info *access_s;
  struct index_info *index_info;
  unsigned i;
  int j;
  gcov_type max_hot_level;
  gcov_type unknown = 0;  /* In some access sites the induction variable 
                             of the inner-most loop containing it is 
			     unknown due to unrecognized increament 
			     statement.  */
  
  if (mi->reorg_failed_reason)
    {
      mi->flatten_p = false;
      return 1;
    }
  
  if (!mi->allocation_info
      || !mi->dimension_size
      || (VARRAY_ACTIVE_SIZE (mi->dimension_size) 
	  != (unsigned) mi->num_dims)
      || sbitmap_first_set_bit (mi->allocation_info->alloc_site_not_found) != -1)
    {
      mi->reorg_failed_reason = 
	N_("matrix allocation site were not found.");
      mi->flatten_p = false;
      return 1;
    }
  
  mi->flatten_p = true;
  
  /* Decide wheather to flatten the transposed matrix.  */
  mi->dim_hot_level = (gcov_type *) xcalloc (mi->num_dims + 1, 
					     sizeof (gcov_type));
  
  for (access_s = mi->access_l; access_s; access_s = access_s->next)
    if (access_s->index_info)
      {
	if (access_s->unknown_inner_most_loop_iv)
          unknown += (bb_for_stmt (access_s->stmt))->count;
	else
	  for (i = 0; i < VARRAY_ACTIVE_SIZE (access_s->index_info); i++)
	    {
	      index_info = VARRAY_GENERIC_PTR (access_s->index_info, i);
	      if (index_info->iterated_by_inner_most_loop_p)
		mi->dim_hot_level[i] += (bb_for_stmt (access_s->stmt))->count; 
	    }
      }
  
  max_hot_level = unknown;
  mi->hottest_dim = mi->num_dims - 1;
  for (j = 0; j < mi->num_dims; j++)
    {
      if (max_hot_level < mi->dim_hot_level[j])
	mi->hottest_dim = j;
      max_hot_level = MAX (max_hot_level, mi->dim_hot_level[j]);
    }
  
  /* Flatten the transposed matrix only if the hottset dimension is not
     the outermost dimension.  */
  if (mi->num_dims - 1 != mi->hottest_dim)
    mi->flatten_transpose_matrix_p = true;
  else
    mi->flatten_transpose_matrix_p = false;
  
  /* Continue traversing the hash table.  */
  return 1;
}

/* Release the data structures used for matrix info.
   Callbacks for htab_traverse. */

static int
free_matrix (void **slot, void *data ATTRIBUTE_UNUSED)
{
  struct matrix_info *mi = *slot;
  struct access_site_info *access_s, *tmp;
  
  /* release the access site list.  */
  access_s = mi->access_l; 
  while (access_s)
    {
      if (access_s->index_info)
	VARRAY_CLEAR (access_s->index_info);
      tmp = access_s;
      access_s = access_s->next;
      free (tmp);
    }
  
  if (mi->allocation_info && mi->dimension_size)
    VARRAY_CLEAR (mi->dimension_size);

  if (mi->allocation_info)
    sbitmap_free (mi->allocation_info->alloc_site_not_found);

  if (mi->allocation_info)
    free (mi->allocation_info);
  
  if (mi->dim_hot_level)
    free (mi->dim_hot_level);

  if (mi->dim_map)
    free (mi->dim_map);
  
  /* Continue traversing the hash table.  */
  return 1;
}


/* Release DATA_REORG.  */

static void
free_data_reorg (struct data_reorg *data_reorg)
{
  if (data_reorg->matrices_to_reorg)
    {
      htab_traverse (data_reorg->matrices_to_reorg,
                     free_matrix, NULL);
      htab_delete (data_reorg->matrices_to_reorg);
    }
  free (data_reorg);
}


/* Perform matrix flattening and transposing.  */

void
matrix_reorg (void)
{
  struct data_reorg *data_reorg = NULL;
  
  /* Verify that this compiler invocation was passed *all* 
     the user-written
     code for this program.  */
  
  if (!flag_whole_program)
    {
      inform
	("Whole program not passed to compiler: Can't perform matrix transposing.");
      return;
    }
  
  data_reorg = xcalloc (1, sizeof (struct data_reorg));
  
  if (flag_matrix_flattening)
    data_reorg->matrices_to_reorg = htab_create (37, mtt_info_hash,
						 mtt_info_eq, free);
  else
    return;

  /* Find and record all potential matrices in the program.  */
  find_matrices_decl (data_reorg);
 
  /* Create a mapping between temporaries which
     hold the value of matrix decl.  */
  data_reorg->aux = (splay_tree) splay_tree_new (splay_tree_compare_pointers,
                                                 NULL, NULL);

  /* Find all the matrices which are unsafe for this optimization.  */
  find_matrix_escape_sites (data_reorg);

  splay_tree_delete (data_reorg->aux);

 
  /* Find and record all matrix references.  */
  find_references_to_matrices (data_reorg);
  
  /* Decide if to flatten the matrices.  */
  htab_traverse (data_reorg->matrices_to_reorg,
		 decide_matrix_flattening, NULL);
  
  /* Transposed selected matrices.  */
  htab_traverse (data_reorg->matrices_to_reorg,
		 transpose_matrix, NULL);
  
  /* Flatten the selected matrices.  */
  htab_traverse (data_reorg->matrices_to_reorg,
		 apply_matrix_flattening, NULL);
  
  /* Clean up.  */
  free_data_reorg (data_reorg);
}








