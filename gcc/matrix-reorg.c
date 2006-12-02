/* Matrix layout transformations.
   Copyright (C) 2006 Free Software Foundation, Inc.
   Contributed by Razya Ladelsky <razya@il.ibm.com>
   Originally written by Revital Eres and Mustafa Hagog.
   
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

/*  
   Matrix flattening optimization tries to replace a N-dimernsional 
   matrix with its equivalent M-dimensional matrix, where M < N.
   This first implementation focuses on global matrices defined dynamically.

   When N==1, we actually flatten the whole matrix.
   For instance consider a two-dimensional array a [dim1] [dim2].
   The code for allocating space for it usually looks like:

     a = (int **)  malloc(dim1 * sizeof(int *));
     for (i=0; i<dim1; i++)
        a[i] = (int *) malloc (dim2 * sizeof(int));

   If the arrays "a" is found suitable for this optimization,
   its allocation is replaced by:

     a = (int *) malloc (dim1 * dim2 *sizeof(int));

   and all the references to a[i][j] are replaced by a[i * dim2 + j].

   The two main phases of the optimization are the analysis
   and transformation.
   The driver of the optimization is matrix_reorg ().

    
      
   Analysis phase:
   ===============

   The analysis part of the optimization determines K, the escape 
   level of a N-dimensional matrix (K <= N), that allows flattening of 
   the external dimensions 0,1,..., K-1. Escape level 0 means that the
   whole matrix escapes and no flattening is possible.
     
   The analysis part is implemented in analyze_matrix_allocation_site() 
   and analyze_matrix_accesses().

   Transformation phase:
   =====================
   In this phase we define the new flattened matrices that replace the 
   original matrices in the code. 
   Implemented in transform_allocation_sites(), 
   transform_access_sites().  

   Matrix Transposing
   ==================
   The idea of Matrix Transposing is organizing the matrix in a different 
   layout such that the dimensions are reordered.
   This could produce better cache behaviour in some cases.

   For example, lets look at the matrix accesses in the following loop:

   for (i=0; i<N; i++)
    for (j=0; j<M; j++)
     access to a[i][j]

   This loop can produce good cache behavior because the elements of 
   the inner dimension are accessed sequentially.

  However, if the accesses of the matrix were of the following form:

  for (i=0; i<N; i++)
   for (j=0; j<M; j++)
     access to a[j][i]

  In this loop we iterate the columns and not the rows. 
  Therefore, replacing the rows and columns 
  would have had an organization with better (cache) locality.
  Replacing the dimensions of the matrix is called matrix transposing.

  This  example, of course, could be enhanced to multiple dimensions matrices 
  as well.

  Since a program could include all kind of accesses, there is a decision 
  mechanism, implemented in analyze_transpose(), which implements a  
  hueristic that tries to determine whether to transpose the matrix or not,
  according to the form of the more dominant accesses.
  This decision is transferred to the flattening mechanism, and whether 
  the matrix was transposed or not, the matrix is flattened (if possible).
  
  This decision making is based on profiling information and loop information.
  If profiling information is available, decision making mechanism will be 
  operated, otherwise the matrix will only be flattened (if possible).

  Both optimizations are described in a paper which was presented in 
  GCC Summit 2006.

 */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "c-tree.h"
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
#include "cfgloop.h"
#include "tree-iterator.h"
#include "tree-pass.h"
#include "opts.h"
#include "tree-data-ref.h"
#include "tree-chrec.h"
#include "tree-scalar-evolution.h"

/*
   We need to collect a lot of data from the original malloc,
   particularly as the gimplifier has converted:

   orig_var = (struct_type *) malloc (x * sizeof (struct_type *));

   into

   T3 = <constant> ;  ** <constant> is amount to malloc; precomputed **
   T4 = malloc (T3);
   T5 = (struct_type *) T4;
   orig_var = T5;

   The following struct fields allow us to collect all the necessary data from
   the gimplified program.  The comments in the struct below are all based
   on the gimple example above.  */

struct malloc_call_data
{
  tree call_stmt;		/* Tree for "T4 = malloc (T3);"                     */
  tree size_var;		/* Var decl for T3.                                 */
  tree result_var;		/* Var decl for T4.                                 */
  tree final_lhs;		/* Var decl for T5.                                 */
  tree malloc_size;		/* Tree for "<constant>", the rhs assigned to T3.   */
  tree malloc_type;		/* Decl for "(struct_type *)" from cast statement.  */
  tree num_elts;		/* Needed for "calloc" calls.                       */
  tree ptr_var;			/* Needed for "realloc" calls.                      */
};

/* The front end of the compiler, when parsing statements of the form:

   var = (type_cast) malloc (sizeof (type));

   always converts this single statement into the following statements
   (GIMPLE form):

   T.1 = sizeof (type);
   T.2 = malloc (T.1);
   T.3 = (type_cast) T.2;
   var = T.3;

   Since we need to create new malloc statements and modify the original
   statements somewhat, we need to find all four of the above statements.
   Currently record_call_1 (called for building cgraph edges) finds and
   records the statements containing the actual call to malloc, but we
   need to find the rest of the variables/statements on our own.  That
   is what the following function does.  */
static void
collect_data_for_malloc_call (tree stmt, struct malloc_call_data *m_data)
{
  tree size_var;
  tree result_var;
  tree malloc_fn_decl;
  tree tmp;
  tree arg1;
  tree arg2;
  tree num_elts = NULL_TREE;
  tree ptr_var = NULL_TREE;
  bool is_malloc = false;
  bool is_calloc = false;
  bool is_realloc = false;
  const char *malloc_fname;

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    fatal_error ("Call to malloc not part of an assignment.");

  result_var = TREE_OPERAND (stmt, 0);

  tmp = TREE_OPERAND (stmt, 1);

  malloc_fn_decl = TREE_OPERAND (tmp, 0);
  if (TREE_CODE (malloc_fn_decl) != ADDR_EXPR)
    fatal_error ("Expected ADDR_EXPR in call; found something else.");

  malloc_fn_decl = TREE_OPERAND (malloc_fn_decl, 0);

  if (TREE_CODE (malloc_fn_decl) != FUNCTION_DECL)
    fatal_error ("Expected FUNCTION_DECL, found something else.");

  malloc_fname = IDENTIFIER_POINTER (DECL_NAME (malloc_fn_decl));
  if (strcmp (malloc_fname, "malloc") == 0
      || strcmp (malloc_fname, "xmalloc") == 0)
    is_malloc = true;
  else if (strcmp (malloc_fname, "calloc") == 0)
    is_calloc = true;
  else if (strcmp (malloc_fname, "realloc") == 0)
    is_realloc = true;

  if (!is_malloc && !is_calloc && !is_realloc)
    fatal_error ("Unidentified version of MALLOC used.");
  else if (TREE_CODE (tmp) == CALL_EXPR)
    {
      tmp = TREE_OPERAND (tmp, 1);	/* Get the argument.  */
      arg1 = TREE_VALUE (tmp);	/* Arguments are in a TREE_LIST.  */
      if (is_malloc)
	size_var = arg1;
      else if (is_calloc)
	{
	  num_elts = arg1;
	  tmp = TREE_CHAIN (tmp);
	  arg2 = TREE_VALUE (tmp);
	  size_var = arg2;
	}
      else			/* is_realloc == true  */
	{
	  ptr_var = arg1;
	  tmp = TREE_CHAIN (tmp);
	  arg2 = TREE_VALUE (tmp);
	  size_var = arg2;
	}
      m_data->call_stmt = stmt;
      m_data->size_var = size_var;
      m_data->result_var = result_var;
      if (TREE_CODE (size_var) != VAR_DECL)
	m_data->malloc_size = size_var;
      else
	m_data->malloc_size = NULL_TREE;
      m_data->malloc_type = NULL_TREE;
      m_data->final_lhs = NULL_TREE;
      m_data->num_elts = num_elts;
      m_data->ptr_var = ptr_var;
    }
  else
    fatal_error ("Was expecting a call; found something else.");
}

/* Information about matrix access site.
   For example: if an access site of matrix arr is arr[i][j]
   the ACCESS_SITE_INFO structure will have the address
   of arr as it's stmt.  The INDEX_INFO will hold information about the
   initial address and index of each dimension.  */
struct access_site_info
{
  /* The statement (INDIRECT_REF or PLUS_EXPR).  */
  tree stmt;
  /* In case of PLUS_EXPR, what is the offset.  */
  tree offset;
  /* The indrection level of this statement.  */
  int level;
  /* TRURE for allocation site FALSE for access site.  */
  bool is_alloc;
  /* The function containing the access site.  */
  tree function_decl;
  /* Ths access is iterated in the inner most loop */
  bool iterated_by_inner_most_loop_p;
};

/* Information about the allocation site of a matrix.  */
struct allocation_info
{
  /* The call expression to malloc of the innermost dimension.  */
  tree call_exp;
  /* Bitmap for the allocation site of the matrix's dimensions.  The bit in
     ALLOC_SITE_NOT_FOUND [I] is set iff the allocation site of the
     i-th dimension is not found yet.  */
  sbitmap alloc_site_not_found;
  tree stmt;
};

/* Information about matrix to flatten.  */
struct matrix_info
{
  /* Decl tree of this matrix.  */
  tree decl;
  /* Number of dimentions; currently this is the number
     of "*" in the type declaration.  */
  int num_dims;
  /* this is the maximum number of indirections used to access the
     matrix, which means that the type of element is:
     (*^(num_dims - actual_dims))TYPE.
     0 when we still don't know it.  */
  int actual_dims;

  /* Minimum indirection level that escapes, 0 means that
     the whole matrix escapes, k means that dimentions
     0 to ACTUAL_DIM - k escapes.  */
  int min_indirect_level_escape;
  tree min_indirect_level_escape_stmt;

  /* Hold the allocation site for each level (dimention).
     We can use NUM_DIMS as the upper bound and allocate the array
     once with this number of elements and no need to use realloc and
     MAX_MALLOCED_LEVEL.  */
  struct allocation_info **malloc_for_level;
  int max_malloced_level;
  /* The location of the allocation sites (they must be in one
     function).  */
  tree allocation_function_decl;

  /* The calls to free for each level of indirection.  */
  struct free_info
  {
    tree stmt;
    tree func;
  } *free_stmts;

  /* The type of the matrix elements. this is not important actually
     we care about the size of the elements only. */
  tree type;

  /* An array which holds for each dimension it's size. where
     dimention 0 is the outer most (one that contains all the others).
   */
  tree *dimension_size;
  /* An array which holds for each dimension it's original size 
     (before transposing and flattening take place).  */
  tree *dimension_size_orig;
  /* An array which holds for each dimension the size of the type of
     of elements accessed in that level (in bytes).  */
  HOST_WIDE_INT *dimension_type_size;
  int dimension_type_size_len;
  /* An array collecting the count of accesses for each dimension.  */
  gcov_type *dim_hot_level;
  /* An array of the accesses to be flattened.
     elements are of type "struct access_site_info *".  */
  varray_type access_l;
  /* Hold information about the allocation site.  */
  struct allocation_info *allocation_info;
  /* A map of how the dimensions will be organized at the end of 
     the analyses.  */
  int *dim_map;
  /* Auxiliary information stored with the matrix.  */
  void *aux;
};

/* In each phi node we want to record the indirection level we have when we
   get to the phi node.  Usually we will have phi nodes with more than two
   arguments, then we must assure that all of them get to the phi node with
   the same indirection level, otherwise it's not safe to do the flattening.
   So we record the information regarding the indirection level each time we
   get to the phi node in this hash table.  */
struct matrix_access_phi_node
{
  tree phi;
  int indirection_level;
};

struct ssa_acc_in_tree
{
  /* The varaible whose accesses in the tree we are looking for.  */
  tree ssa_var;
  /* The tree and code inside it the ssa_var is accessed, currently
     it could be an INDIRECT_REF or CALL_EXPR.  */
  enum tree_code t_code;
  tree t_tree;
  /* The place in the containing tree.  */
  tree *tp;
  tree second_op;
  bool var_found;
};

static void find_escaping_matrices_and_flatten (void);
static int transform_allocation_sites (void **, void *);
static int transform_access_sites (void **, void *);
static int analyze_transpose (void **, void *);
static void ipa_intra_ssa_passes (tree);
static void ssa_analyze_matrix (void);
static int dump_matrix_reorg_analysis (void **, void *);

/* We use find_escaping_matrices_and_flatten twice; the first for the
   analysis phase and the second to collect the access sites for the
   transformation phase this varaible tells in what phase we are.  */
static bool transform_accesses;
static int check_transpose_p;
static bool analyze_transpose_p;

static hashval_t
mat_acc_phi_hash (const void *p)
{
  const struct matrix_access_phi_node *ma_phi = p;

  return htab_hash_pointer (ma_phi->phi);
}

/* Equality means phi node pointers are the same.  */
static int
mat_acc_phi_eq (const void *p1, const void *p2)
{
  const struct matrix_access_phi_node *phi1 = p1;
  const struct matrix_access_phi_node *phi2 = p2;

  if (phi1->phi == phi2->phi)
    return 1;

  return 0;
}

/* Hold the PHI nodes we visit during the travesal for escaping
   analysis.  */
htab_t htab_mat_acc_phi_nodes = NULL;

/* This hash-table holds the information about the matrices we are
   going to handle.  */
static htab_t matrices_to_reorg = NULL;

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

/* Return the inner most tree that is not a cast.  */
static tree
get_inner_of_cast_expr (tree t)
{
  while (TREE_CODE (t) == CONVERT_EXPR || TREE_CODE (t) == NOP_EXPR)
    t = TREE_OPERAND (t, 0);

  return t;
}

/* Return False if there's suspicion for hand written vector in STMT.  
   In this situation, all matrices should not be flattened.  */
static bool
can_flatten_matrices (tree stmt)
{
  tree t;

  switch (TREE_CODE (stmt))
    {
    case MODIFY_EXPR:
      t = TREE_OPERAND (stmt, 1);
      while (TREE_CODE (t) == CONVERT_EXPR || TREE_CODE (t) == NOP_EXPR)
	{
	  if (TREE_TYPE (t))
	    {
	      if (TREE_CODE (TREE_TYPE (t)) == POINTER_TYPE)
		{
		  tree pointee;

		  pointee = TREE_TYPE (t);
		  while (POINTER_TYPE_P (pointee))
		    pointee = TREE_TYPE (pointee);
		  if (TREE_CODE (pointee) == VECTOR_TYPE)
		    {
		      if (dump_file)
			fprintf (dump_file,
				 "Found vector type, don't flatten matrix\n");
		      return false;
		    }
		}
	    }
	  t = TREE_OPERAND (t, 0);
	}
      break;
    case ASM_EXPR:
      /* Asm code could contain vector operations.  */
      return false;
      break;
    default:
      break;
    }
  return true;
}

/* Return false if there are hand-written vectors in the program.  
   We disable the falattening in such a case.  */
static bool
flatten_matrices (struct cgraph_node *node)
{
  tree decl;
  struct function *func;
  basic_block bb;
  block_stmt_iterator bsi;

  decl = node->decl;
  if (node->analyzed)
    {
      func = DECL_STRUCT_FUNCTION (decl);
      FOR_EACH_BB_FN (bb, func)
	for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	if (!can_flatten_matrices (bsi_stmt (bsi)))
	  return false;
    }
  return true;
}

/* Given a VAR_DECL, check its type to determine whether it is
   a definition of a dynamic allocated matrix and therefore is
   a suitable candidate for the matrix flattening optimization.
   Return NULL if VAR_DECL is not such decl.  Otherwise, allocate
   a MATRIX_INFO structure, fill it with the relevant information
   and return a pointer to it.
   TODO: handle also staticaly defined arrays.  */
static struct matrix_info *
analyze_matrix_decl (tree var_decl)
{
  struct matrix_info *m_node, tmpmi, *mi;
  tree var_type;
  int dim_num = 0;

#ifdef ENABLE_CHECKING
  gcc_assert (matrices_to_reorg);
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

  if (dim_num < 1)
    return NULL;

  if (!COMPLETE_TYPE_P (var_type)
      || TREE_CODE (TYPE_SIZE_UNIT (var_type)) != INTEGER_CST)
    return NULL;

  /* Check to see if this pointer is already in there.  */
  tmpmi.decl = var_decl;
  mi = htab_find (matrices_to_reorg, &tmpmi);

  if (mi)
    return NULL;

  /* Record the matrix.  */

  m_node = (struct matrix_info *) xcalloc (1, sizeof (struct matrix_info));
  m_node->decl = var_decl;
  m_node->num_dims = dim_num;
  m_node->type = var_type;
  m_node->free_stmts
    = (struct free_info *) xcalloc (dim_num, sizeof (struct free_info));

  /* Init min_indirect_level_escape to -1 to indicate that escaping
     is not made yet.  */
  m_node->min_indirect_level_escape = -1;
  m_node->allocation_info = (struct allocation_info *)
    xcalloc (1, sizeof (struct allocation_info));
  m_node->allocation_info->alloc_site_not_found = sbitmap_alloc (dim_num);
  sbitmap_ones (m_node->allocation_info->alloc_site_not_found);

  return m_node;
}

/* Free matrix E.  */
static void
mat_free (void *e)
{
  struct matrix_info *mat = (struct matrix_info *) e;

  if (mat->free_stmts)
    free (mat->free_stmts);
  if (mat->allocation_info && mat->allocation_info->alloc_site_not_found)
    sbitmap_free (mat->allocation_info->alloc_site_not_found);
  if (mat->allocation_info)
    free (mat->allocation_info);
  if (mat)
    free (mat);
}

/* Find all potential matrices.
   TODO: currently we handle only multidimensional
   dynamiclly allocated arrays.  */
static void
find_matrices_decl (void)
{
  struct matrix_info *tmp;
  PTR *slot;
  struct cgraph_varpool_node *current_varpool;

  if (!matrices_to_reorg)
    abort ();

  /* For every global variable in the program:
     Check to see if it's of a candidate type and record it.  */

  for (current_varpool = cgraph_varpool_nodes_queue; current_varpool;
       current_varpool = current_varpool->next_needed)
    {
      tree var_decl = current_varpool->decl;

      if (!var_decl || TREE_CODE (var_decl) != VAR_DECL)
	continue;

      if (matrices_to_reorg)
	if ((tmp = analyze_matrix_decl (var_decl)))
	  {
	    if (!TREE_ADDRESSABLE (var_decl))
	      {
		slot = htab_find_slot (matrices_to_reorg, tmp, INSERT);
		*slot = tmp;
	      }
	  }
    }
  return;
}

/* Mark that the matrix MI escapes at level L.  */
static void
mark_min_matrix_escape_level (struct matrix_info *mi, int l, tree s)
{
  if (mi->min_indirect_level_escape == -1
      || (mi->min_indirect_level_escape > l))
    {
      mi->min_indirect_level_escape = l;
      mi->min_indirect_level_escape_stmt = s;
    }
}

/* Create a new global variable of type TYPE.  */
static tree
add_new_global_for_decl (tree type)
{
  tree new_decl;
  struct cgraph_varpool_node *new_node;

  new_decl = create_tmp_var (type, NULL);
  DECL_NAME (new_decl) = create_tmp_var_name (NULL);
  TREE_READONLY (new_decl) = 0;
  TREE_STATIC (new_decl) = 1;
  TREE_USED (new_decl) = 1;
  DECL_CONTEXT (new_decl) = NULL_TREE;
  DECL_ABSTRACT (new_decl) = 0;
  lang_hooks.dup_lang_specific_decl (new_decl);
  create_var_ann (new_decl);
  new_node = cgraph_varpool_node (new_decl);
  notice_global_symbol (new_decl);
  cgraph_varpool_mark_needed_node (new_node);
  add_referenced_var (new_decl);
  cgraph_varpool_finalize_decl (new_decl);

  return new_node->decl;
}

/* Find if the SSA varaible is accessed inside the
   tree and record the tree containing it.
   The only relevant uses are the case of SSA_NAME, or SSA inside
   INDIRECT_REF, CALL_EXPR, PLUS_EXPR, MULT_EXPR.  */
static void
ssa_accessed_in_tree (tree t, struct ssa_acc_in_tree *a)
{
  tree call, decl;
  a->t_code = TREE_CODE (t);
  switch (a->t_code)
    {
      tree args, op1, op2;

    case SSA_NAME:
      if (t == a->ssa_var)
	a->var_found = true;
      break;
    case INDIRECT_REF:
      if (SSA_VAR_P (TREE_OPERAND (t, 0))
	  && TREE_OPERAND (t, 0) == a->ssa_var)
	a->var_found = true;
      break;
    case CALL_EXPR:
      for (args = TREE_OPERAND (t, 1); args; args = TREE_CHAIN (args))
	{
	  tree arg = TREE_VALUE (args);

	  if (arg == a->ssa_var)
	    {
	      a->var_found = true;
	      call = get_call_expr_in (t);
	      if (call && (decl = get_callee_fndecl (call)))
		a->t_tree = decl;
	      break;
	    }
	}
      break;
    case PLUS_EXPR:
    case MULT_EXPR:
      op1 = TREE_OPERAND (t, 0);
      op2 = TREE_OPERAND (t, 1);

      if (op1 == a->ssa_var)
	{
	  a->var_found = true;
	  a->second_op = op2;
	}
      else if (op2 == a->ssa_var)
	{
	  a->var_found = true;
	  a->second_op = op1;
	}
      break;
    default:
      break;
    }
}

/* Record the access/allocation site information for matrix MI so we can 
   handle it later in transformation.  */
static void
record_access_alloc_site_info (struct matrix_info *mi, tree stmt, tree offset,
			       int level, bool is_alloc)
{
  struct access_site_info *acc_info;

  if (!transform_accesses && !analyze_transpose_p)
    return;
  if (!mi->access_l)
    VARRAY_GENERIC_PTR_INIT (mi->access_l, 37, "");

  acc_info
    = (struct access_site_info *)
    xcalloc (1, sizeof (struct access_site_info));
  acc_info->stmt = stmt;
  acc_info->offset = offset;
  acc_info->function_decl = current_function_decl;
  acc_info->level = level;
  acc_info->is_alloc = is_alloc;
  VARRAY_PUSH_GENERIC_PTR (mi->access_l, acc_info);
}

/* Record the malloc as the allocation site of the given LEVEL.  But
   first we Make sure that all the size parameters passed to malloc in
   all the allocation sites could be pre-calculated before the call to
   the malloc of level 0 (the main malloc call).  */
static void
add_allocation_site (struct matrix_info *mi, tree stmt, int level)
{
  struct malloc_call_data mcd;

  /* Make sure that the allocation sites are in the same function.  */
  if (!mi->allocation_function_decl)
    mi->allocation_function_decl = current_function_decl;
  else if (mi->allocation_function_decl != current_function_decl)
    {
      int min_malloc_level;

      gcc_assert (mi->malloc_for_level);

      /* Find the minimum malloc level that already has been seen;
         we known its allocation function must be
         MI->allocation_function_decl since it's different than
         CURRENT_FUNCTION_DECL then the escaping level should be
         MIN (LEVEL, MIN_MALLOC_LEVEL) - 1 , and the allocation function
         must be set accodingly.  */
      for (min_malloc_level = 0;
	   min_malloc_level < mi->max_malloced_level
	   && mi->malloc_for_level[min_malloc_level]; min_malloc_level++);
      if (level < min_malloc_level)
	{
	  mi->allocation_function_decl = current_function_decl;
	  mark_min_matrix_escape_level (mi, min_malloc_level, stmt);
	}
      else
	{
	  mark_min_matrix_escape_level (mi, level, stmt);
	  /* cannot be that (level == min_malloc_level) 
	     we would have returned earlier.  */
	  return;
	}
    }

  /* Find the correct malloc (realloc/calloc) information.  */
  collect_data_for_malloc_call (stmt, &mcd);

  /* We accept only calls to malloc-like functions; we do not accept
     calls like calloc and realloc.  */
  if (!mi->malloc_for_level)
    {
      mi->malloc_for_level =
	xcalloc (level + 1, sizeof (struct allocation_info *));
      mi->max_malloced_level = level + 1;
    }
  else if (mi->max_malloced_level <= level)
    {
      mi->malloc_for_level
	= xrealloc (mi->malloc_for_level,
		    (level + 1) * sizeof (struct allocation_info *));

      /* Zero the newly allocated items.  */
      memset (&(mi->malloc_for_level[mi->max_malloced_level + 1]),
	      0, (level - mi->max_malloced_level)
	      * sizeof (struct allocation_info *));

      mi->max_malloced_level = level + 1;
    }
  mi->malloc_for_level[level] = xcalloc (1, sizeof (struct allocation_info));
  mi->malloc_for_level[level]->stmt = stmt;
}

/* Given an assignment statement STMT that we know that its
   left-hand-side is the matrix varaible, we traverse the immediate
   uses backwords until we get to a malloc site.  We make sure that
   there is one and only one malloc site that sets this varaible.  When
   we are performing the flattening we generate a new varaible that
   will hold the size for each dimention; each malloc that allocates a
   dimention has the size parameter; we use that parameter to
   initialize the dimention size varaible so we can use it later in
   the address calculations.  Return if STMT is related to an
   allocation site.  */
static void
analyze_matrix_allocation_site (struct matrix_info *mi, tree stmt,
				int level, sbitmap visited)
{
  if (transform_accesses)
    return;

  if (TREE_CODE (stmt) == MODIFY_EXPR)
    {
      tree rhs = TREE_OPERAND (stmt, 1);

      rhs = get_inner_of_cast_expr (rhs);
      if (TREE_CODE (rhs) == SSA_NAME)
	{
	  tree def = SSA_NAME_DEF_STMT (rhs);

	  analyze_matrix_allocation_site (mi, def, level, visited);
	  return;
	}

      /* A result of call to malloc.  */
      else if (TREE_CODE (rhs) == CALL_EXPR)
	{
	  int call_flags = call_expr_flags (rhs);

	  if (!(call_flags & ECF_MALLOC))
	    {
	      mark_min_matrix_escape_level (mi, level, stmt);
	      return;
	    }

	  /* This is a call to malloc.  Check to see if this is the first
	     call in this indirection level; if so, mark it; if not, mark
	     as escaping.  */
	  if (mi->malloc_for_level
	      && mi->malloc_for_level[level]
	      && mi->malloc_for_level[level]->stmt != stmt)
	    {
	      mark_min_matrix_escape_level (mi, level, stmt);
	      return;
	    }
	  else
	    add_allocation_site (mi, stmt, level);
	  return;
	}
      /* If we are back to the original matrix varaible then we
         are sure that this is analyzed as an access site.  */
      else if (rhs == mi->decl)
	return;
    }
  /* Looks like we don't know what is happenening in this
     statement so be in the safe side and mark it as escaping.  */
  mark_min_matrix_escape_level (mi, level, stmt);
}

/* The transposing decision making.
   In order to to calculate the profitability of transposing, we collect two 
   types of informationi regarding the accesses:
   1. profiling information used to express the hotness of an access, that
   is how often the matrix is accessed by this access site (count of the 
   access site). 
   2. which dimension in the access site is iterated by the inner
   most loop containing this access.

   The matrix will have a calculated value of weighted hotness for each 
   dimension.
   Intuitively the hotness level of a dimension is a function of how 
   many times it was the most frequently accessed dimension in the 
   highly executed access sites of this matrix.

   As computed by following equation:
   m      n 
   __   __  
   \    \  dim_hot_level[i] +=   
   /_   /_
   j     i 
                 acc[j]->dim[i]->iter_by_inner_loop * count(j)

  Where n is the number of dims and m is the number of the matrix
  access sites. acc[j]->dim[i]->iter_by_inner_loop is 1 if acc[j]
  iterates over dim[i] in innermost loop, and is 0 otherwise.

  The organization of the new matrix should be according to the
  hotness of each dimension. The hotness of the dimension implies
  the locality of the elements.*/

static int
analyze_transpose (void **slot, void *data ATTRIBUTE_UNUSED)
{
  struct matrix_info *mi = *slot;
  int min_escape_l = mi->min_indirect_level_escape;
  struct loop *loop;
  affine_iv iv;

  if (min_escape_l < 2 || !mi->access_l)
    {
      if (mi->access_l)
	while (VARRAY_ACTIVE_SIZE (mi->access_l))
	  {
	    struct access_site_info *acc_info;
	    acc_info = VARRAY_TOP_GENERIC_PTR (mi->access_l);
	    free (acc_info);
	    VARRAY_POP (mi->access_l);
	  }
      return 1;
    }
  if (!mi->dim_hot_level)
    mi->dim_hot_level =
      (gcov_type *) xcalloc (min_escape_l, sizeof (HOST_WIDE_INT));
  while (VARRAY_ACTIVE_SIZE (mi->access_l))
    {
      struct access_site_info *acc_info;
      acc_info = VARRAY_TOP_GENERIC_PTR (mi->access_l);
      if (TREE_CODE (TREE_OPERAND (acc_info->stmt, 1)) == PLUS_EXPR
	  && acc_info->level < min_escape_l)
	{
	  loop = loop_containing_stmt (acc_info->stmt);
	  if (loop->inner)
	    {
	      free (acc_info);
	      VARRAY_POP (mi->access_l);
	      continue;
	    }
	  if (simple_iv (loop, acc_info->stmt, acc_info->offset, &iv, true))
	    {
	      if (iv.step != NULL)
		{
		  acc_info->iterated_by_inner_most_loop_p = 1;
		  mi->dim_hot_level[acc_info->level] +=
		    bb_for_stmt (acc_info->stmt)->count;
		}
	    }
	}
      free (acc_info);
      VARRAY_POP (mi->access_l);
    }
  return 1;
}

/* Given a use statement (STMT) of the matrix, follow its
   uses and level of indirection and find out the minimum indirection
   level it escapes in (the highest dimention) and the maximum
   level it is accessed in (this will be the actual dimention of the
   matrix).  The inforamtion is accumaled in MI.
   We look at the immediate uses, if one escapes we finish; if not,
   we make a recursive call for each one of the immediate uses of the
   resulting SSA name.  */
static void
analyze_matrix_accesses (struct matrix_info *mi, tree stmt, tree ssa_var,
			 int current_indirect_level, bool last_op,
			 sbitmap visited)
{
  imm_use_iterator imm_iter;
  use_operand_p use_p;
  HOST_WIDE_INT type_size = int_size_in_bytes (TREE_TYPE (ssa_var));

  /* Record the size of elements accessed (as a whole)
     in the current indirection level (dimention).  If the size of
     elements is not known at compile time, mark it as escaping.  */
  if (type_size <= 0)
    mark_min_matrix_escape_level (mi, current_indirect_level, stmt);
  else
    {
      int l = current_indirect_level;

      if (!mi->dimension_type_size)
	{
	  mi->dimension_type_size
	    = (HOST_WIDE_INT *) xcalloc (l + 1, sizeof (HOST_WIDE_INT));
	  mi->dimension_type_size_len = l + 1;
	}
      else if (mi->dimension_type_size_len < l + 1)
	{
	  mi->dimension_type_size
	    = (HOST_WIDE_INT *) xrealloc (mi->dimension_type_size,
					  (l + 1) * sizeof (HOST_WIDE_INT));
	  memset (&mi->dimension_type_size[mi->dimension_type_size_len],
		  0, (l + 1 - mi->dimension_type_size_len)
		  * sizeof (HOST_WIDE_INT));
	  mi->dimension_type_size_len = l + 1;
	}

      /* Make sure all the accesses in the same level have the same size
         of the type.  */
      if (!mi->dimension_type_size[l])
	mi->dimension_type_size[l] = type_size;
      else if (mi->dimension_type_size[l] != type_size)
	mark_min_matrix_escape_level (mi, l, stmt);
    }

  /* We don't go beyond the escaping level when we are performing the
     flattening.  NOTE: we keep the last indirection level that doesn't
     escape.  */
  if (mi->min_indirect_level_escape > -1 &&
      mi->min_indirect_level_escape <= current_indirect_level)
    return;
  /* Now go over the uses of the SSA_NAME and check how it is used in
     each one of them.  We are mainly looking for the pattern INDIRECT_REF,
     then a PLUS_EXPR, then INDIRECT_REF etc.  while in between there could
     be any number of copies and casts.  */
  gcc_assert (TREE_CODE (ssa_var) == SSA_NAME);
  FOR_EACH_IMM_USE_FAST (use_p, imm_iter, ssa_var)
  {
    tree use_stmt = USE_STMT (use_p);
    if (TREE_CODE (use_stmt) == PHI_NODE)
      {
	/* We check all the escaping levels that get to the PHI node
	   and make sure they are all the same escaping;
	   if not (which is rare) we let the escaping level be the
	   minimum level that gets into that PHI because starting from
	   that level we cannot expect the behavior of the indirections.  */
	struct matrix_access_phi_node tmp_maphi, *maphi, **pmaphi;

	tmp_maphi.phi = use_stmt;
	if ((maphi = htab_find (htab_mat_acc_phi_nodes, &tmp_maphi)))
	  {
	    if (maphi->indirection_level == current_indirect_level)
	      return;
	    else
	      {
		int level = MIN (maphi->indirection_level,
				 current_indirect_level);
		int j;
		tree t = NULL_TREE;

		maphi->indirection_level = level;
		for (j = 0; j < PHI_NUM_ARGS (use_stmt); j++)
		  {
		    tree def = PHI_ARG_DEF (use_stmt, j);

		    if (TREE_CODE (SSA_NAME_DEF_STMT (def)) != PHI_NODE)
		      t = SSA_NAME_DEF_STMT (def);
		  }
		mark_min_matrix_escape_level (mi, level, t);
	      }
	    return;
	  }
	maphi = (struct matrix_access_phi_node *)
	  xcalloc (1, sizeof (struct matrix_access_phi_node));
	maphi->phi = use_stmt;
	maphi->indirection_level = current_indirect_level;

	/* Insert to hash table.  */
	pmaphi = (struct matrix_access_phi_node **)
	  htab_find_slot (htab_mat_acc_phi_nodes, maphi, INSERT);
	gcc_assert (pmaphi);
	*pmaphi = maphi;

	if (!TEST_BIT (visited, SSA_NAME_VERSION (PHI_RESULT (use_stmt))))
	  {
	    SET_BIT (visited, SSA_NAME_VERSION (PHI_RESULT (use_stmt)));
	    analyze_matrix_accesses (mi, use_stmt, PHI_RESULT (use_stmt),
				     current_indirect_level, false, visited);
	    RESET_BIT (visited, SSA_NAME_VERSION (PHI_RESULT (use_stmt)));
	  }
      }
    else if (TREE_CODE (use_stmt) == CALL_EXPR)
      {
	tree call = get_call_expr_in (use_stmt);
	if (call && get_callee_fndecl (call))
	  {
	    if (strcmp (get_name (get_callee_fndecl (call)), "free") != 0)
	      {
		if (dump_file)
		  fprintf (dump_file, "Function call %s, level %d escapes.\n",
			   get_name (get_callee_fndecl (call)),
			   current_indirect_level);
		mark_min_matrix_escape_level (mi, current_indirect_level,
					      use_stmt);
	      }
	    else if (mi->free_stmts[current_indirect_level].stmt != NULL
		     && mi->free_stmts[current_indirect_level].stmt !=
		     use_stmt)
	      mark_min_matrix_escape_level (mi, current_indirect_level,
					    use_stmt);
	    else
	      {
		/*Record the free statements so we can delete them
		   later. */
		int l = current_indirect_level;

		mi->free_stmts[l].stmt = use_stmt;
		mi->free_stmts[l].func = current_function_decl;
	      }
	    return;
	  }
      }
    else if (TREE_CODE (use_stmt) == MODIFY_EXPR)
      {
	tree lhs = TREE_OPERAND (use_stmt, 0);
	tree rhs = TREE_OPERAND (use_stmt, 1);
	struct ssa_acc_in_tree lhs_acc, rhs_acc;

	memset (&lhs_acc, 0, sizeof (lhs_acc));
	memset (&rhs_acc, 0, sizeof (rhs_acc));

	lhs_acc.ssa_var = ssa_var;
	lhs_acc.t_code = ERROR_MARK;
	ssa_accessed_in_tree (lhs, &lhs_acc);
	rhs_acc.ssa_var = ssa_var;
	rhs_acc.t_code = ERROR_MARK;
	ssa_accessed_in_tree (get_inner_of_cast_expr (rhs), &rhs_acc);

	/* The SSA must be either in the left side or in the right side,
	   to understand what is happening.
	   In case the SSA_NAME is found in both sides we should be escaping
	   at this level because in this case we cannot caclulate the
	   address correctly.  */
	if ((lhs_acc.var_found && rhs_acc.var_found
	     && lhs_acc.t_code == INDIRECT_REF)
	    || (!rhs_acc.var_found && !lhs_acc.var_found))
	  {
	    mark_min_matrix_escape_level (mi, current_indirect_level,
					  use_stmt);
	    return;
	  }
	gcc_assert (!rhs_acc.var_found || !lhs_acc.var_found);

	/* If we are storing to the matrix at some level, then mark it as
	   escaping at that level.  */
	if (lhs_acc.var_found)
	  {
	    tree def;
	    int l = current_indirect_level + 1;

	    gcc_assert (lhs_acc.t_code == INDIRECT_REF);
	    def = get_inner_of_cast_expr (rhs);
	    if (TREE_CODE (def) != SSA_NAME)
	      mark_min_matrix_escape_level (mi, l, use_stmt);
	    else
	      {
		def = SSA_NAME_DEF_STMT (def);
		analyze_matrix_allocation_site (mi, def, l, visited);
		record_access_alloc_site_info (mi, use_stmt, NULL_TREE, l,
					       true);
	      }
	    return;
	  }
	/* Now, check the right-hand-side, to see how the SSA variable 
	   is used.  */
	if (rhs_acc.var_found)
	  {
	    /* If we are passing the ssa name to a function call and 
	       the pointer escapes when passed to the function 
	       (not the case of free), then we mark the matrix as 
	       escaping at this level.  */
	    if (rhs_acc.t_code == CALL_EXPR)
	      {
		tree call = get_call_expr_in (use_stmt);
		if (call && get_callee_fndecl (call))
		  {
		    /* check if this is the case of call to free.  */
		    if (strcmp (get_name (rhs_acc.t_tree), "free") != 0)
		      {
			if (dump_file)
			  fprintf (dump_file,
				   "Function call %s, level %d escapes.\n",
				   get_name (get_callee_fndecl (call)),
				   current_indirect_level);
			mark_min_matrix_escape_level (mi,
						      current_indirect_level,
						      use_stmt);
		      }
		    else if (mi->free_stmts[current_indirect_level].stmt !=
			     NULL
			     && mi->free_stmts[current_indirect_level].stmt !=
			     use_stmt)
		      mark_min_matrix_escape_level (mi,
						    current_indirect_level,
						    use_stmt);
		    else
		      {
			/* Record the free statements so we can delete them
			   later.  */
			int l = current_indirect_level;

			mi->free_stmts[l].stmt = use_stmt;
			mi->free_stmts[l].func = current_function_decl;
		      }
		  }
		return;
	      }
	    if (rhs_acc.t_code != INDIRECT_REF
		&& rhs_acc.t_code != PLUS_EXPR && rhs_acc.t_code != SSA_NAME)
	      {
		mark_min_matrix_escape_level (mi, current_indirect_level,
					      use_stmt);
		return;
	      }
	    /* If the access in the RHS has an indirection increase the
	       indirection lavel.  */
	    if (rhs_acc.t_code == INDIRECT_REF)
	      {
		record_access_alloc_site_info (mi, use_stmt, NULL_TREE,
					       current_indirect_level, true);
		current_indirect_level += 1;
	      }
	    else if (rhs_acc.t_code == PLUS_EXPR)
	      {
		/* ??? maybe we should check
		   the type of the PLUS_EXP and make sure it's
		   integral type.  */
		gcc_assert (rhs_acc.second_op);
		if (last_op)
		  /* Currently we support only one PLUS expression on the
		     SSA_NAME that holds the base address of the current
		     indirection level; to support more general case there
		     is a need to hold a stack of expressions and regenerate
		     the calculation later.  */
		  mark_min_matrix_escape_level (mi, current_indirect_level,
						use_stmt);
		else
		  {
		    tree op1 = TREE_OPERAND (rhs, 0);
		    tree op2 = TREE_OPERAND (rhs, 1);

		    op2 = (op1 == ssa_var) ? op2 : op1;
		    record_access_alloc_site_info (mi, use_stmt, op2,
						   current_indirect_level,
						   false);
		  }
	      }

	    /* If we are storing this level of indirection mark it as
	       escaping.  */
	    if (lhs_acc.t_code == INDIRECT_REF || TREE_CODE (lhs) != SSA_NAME)
	      {
		int l = current_indirect_level;

		/* One exception is when we are storing to the matrix
		   varaible itself; this is the case of malloc, we must make
		   sure that it's the one and only one call to malloc so 
		   we call analyze_matrix_allocation_site to check 
		   this out.  */
		if (TREE_CODE (lhs) != VAR_DECL || lhs != mi->decl)
		  mark_min_matrix_escape_level (mi, current_indirect_level,
						use_stmt);
		else
		  {
		    /* Also update the escaping level.  */
		    analyze_matrix_allocation_site (mi, use_stmt, l, visited);
		    record_access_alloc_site_info (mi, use_stmt, NULL_TREE, l,
						   true);
		  }
	      }
	    else
	      {
		/* We are placing it in an SSA, follow that SSA.  */
		analyze_matrix_accesses (mi, use_stmt, lhs,
					 current_indirect_level,
					 rhs_acc.t_code == PLUS_EXPR,
					 visited);
	      }
	  }
      }
  }
}

/* A walk_tree function to go over the VAR_DECL, PARM_DECL nodes of
   the malloc size expression and check that those aren't changed
   over the function.  */
static tree
check_var_notmodified_p (tree * tp, int *walk_subtrees, void *data)
{
  basic_block bb;
  tree t = *tp;
  tree fn = data;
  block_stmt_iterator bsi;
  tree stmt;

  if (TREE_CODE (t) != VAR_DECL && TREE_CODE (t) != PARM_DECL)
    return NULL_TREE;

  FOR_EACH_BB_FN (bb, DECL_STRUCT_FUNCTION (fn))
  {
    for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
      {
	stmt = bsi_stmt (bsi);
	if (TREE_CODE (stmt) != MODIFY_EXPR)
	  continue;
	if (TREE_OPERAND (stmt, 0) == t)
	  return stmt;
      }
  }
  *walk_subtrees = 1;
  return NULL_TREE;
}

/* Go backwords in the use-def chains and find out the expresion
   represented by the possible SSA name in EXPR, until it is composed
   of only VAR_DECL, PARM_DECL and INT_CST.  In case of phi nodes
   we make sure that all the arguments represet the same subexpresion,
   otherwise we fail.  */
static tree
can_calculate_expr_before_stmt (tree expr, sbitmap visited)
{
  tree def_stmt, op1, op2, res;

  switch (TREE_CODE (expr))
    {
    case SSA_NAME:
      /* Case of loop, we don't know to represent this expression.  */
      if (TEST_BIT (visited, SSA_NAME_VERSION (expr)))
	return NULL_TREE;

      SET_BIT (visited, SSA_NAME_VERSION (expr));
      def_stmt = SSA_NAME_DEF_STMT (expr);
      res = can_calculate_expr_before_stmt (def_stmt, visited);
      RESET_BIT (visited, SSA_NAME_VERSION (expr));
      return res;
    case VAR_DECL:
    case PARM_DECL:
    case INTEGER_CST:
      return expr;
    case PLUS_EXPR:
    case MINUS_EXPR:
    case MULT_EXPR:
      op1 = TREE_OPERAND (expr, 0);
      op2 = TREE_OPERAND (expr, 1);

      op1 = can_calculate_expr_before_stmt (op1, visited);
      if (!op1)
	return NULL_TREE;
      op2 = can_calculate_expr_before_stmt (op2, visited);
      if (op2)
	return build2 (TREE_CODE (expr), TREE_TYPE (expr), op1, op2);
      return NULL_TREE;
    case MODIFY_EXPR:
      return can_calculate_expr_before_stmt (TREE_OPERAND (expr, 1), visited);
    case PHI_NODE:
      {
	int j;

	res = NULL_TREE;
	/* Make sure all the arguments represent the same value.  */
	for (j = 0; j < PHI_NUM_ARGS (expr); j++)
	  {
	    tree new_res;
	    tree def = PHI_ARG_DEF (expr, j);

	    new_res = can_calculate_expr_before_stmt (def, visited);
	    if (res == NULL_TREE)
	      res = new_res;
	    else if (!expressions_equal_p (res, new_res))
	      return NULL_TREE;
	  }
	return res;
      }
    case NOP_EXPR:
    case CONVERT_EXPR:
      res = can_calculate_expr_before_stmt (TREE_OPERAND (expr, 0), visited);
      if (res != NULL_TREE)
	return build1 (TREE_CODE (expr), TREE_TYPE (expr), res);
      else
	return NULL_TREE;

    default:
      return NULL_TREE;
    }
}

/* There should be only one allocation function for the dimensions
   that don't escape. Here we check the allocation sites in this
   function. We must make sure that all the dimensions are allocated
   using malloc and that the malloc size parameter expression could be
   pre-calculated before the call to the malloc of dimension 0.

   Given a candidate matrix for flattening -- MI -- check if it's
   appropriate for flattening -- we analyze the allocation
   sites that we recorded in the previous analysis.  The result of the
   analysis is a level of indirection (matrix dimension) in which the
   flattening is safe.  We check the following conditions:
   1. There is only one allocation site for each dimension.
   2. The allocation sites of all the dimentions are in the same
      function.
      (The above two are being taken care of during the analysis when
      we check the allocation site).
   3. All the dimensions that we flatten are allocated at once; thus
      the total size must be known before the allocation of the
      dimension 0 (top level) -- we must make sure we represent the
      size of the allocation as an expression of globals parameters or
      constants and that those doesn't change over the function.  */

static int
check_allocation_function (void **slot, void *data ATTRIBUTE_UNUSED)
{
  int level;
  block_stmt_iterator bsi;
  basic_block bb_level_0;
  struct matrix_info *mi = *slot;
  sbitmap visited = sbitmap_alloc (num_ssa_names);

  if (!mi->malloc_for_level)
    return 1;
  /* Do nothing if the current function is not the allocation
     function of MI.  */
  if (mi->allocation_function_decl != current_function_decl
      /* We aren't in the main allocation function yet.  */
      || !mi->malloc_for_level[0])
    return 1;

  for (level = 1; level < mi->max_malloced_level; level++)
    if (!mi->malloc_for_level[level])
      break;

  mark_min_matrix_escape_level (mi, level, NULL_TREE);

  bsi = bsi_for_stmt (mi->malloc_for_level[0]->stmt);
  bb_level_0 = bsi.bb;

  /* Check if the expression of the size passed to malloc could be
     pre-calculated before the malloc of level 0.  */
  for (level = 1; level < mi->min_indirect_level_escape; level++)
    {
      tree call_stmt, size;
      struct malloc_call_data mcd;

      call_stmt = mi->malloc_for_level[level]->stmt;

      /* Find the correct malloc (realloc/calloc) information.  */
      collect_data_for_malloc_call (call_stmt, &mcd);

      /* Can be only malloc (not calloc nor realloc).  */
      gcc_assert (!mcd.ptr_var && !mcd.num_elts);

      /* No need to check anticipation for constants.  */
      if (TREE_CODE (mcd.size_var) == INTEGER_CST)
	{
	  if (!mi->dimension_size)
	    {
	      mi->dimension_size =
		(tree *) xcalloc (mi->min_indirect_level_escape,
				  sizeof (tree));
	      mi->dimension_size_orig =
		(tree *) xcalloc (mi->min_indirect_level_escape,
				  sizeof (tree));
	    }
	  mi->dimension_size[level] = mcd.size_var;
	  mi->dimension_size_orig[level] = mcd.size_var;
	  continue;
	}
      /* ??? Here we should also add the way to caclulate the size
         expresion not only know that it is anticipated.  */
      sbitmap_zero (visited);
      size = can_calculate_expr_before_stmt (mcd.size_var, visited);
      if (size == NULL_TREE)
	{
	  mark_min_matrix_escape_level (mi, level, call_stmt);
	  break;
	}
      if (!mi->dimension_size)
	{
	  mi->dimension_size =
	    (tree *) xcalloc (mi->min_indirect_level_escape, sizeof (tree));
	  mi->dimension_size_orig =
	    (tree *) xcalloc (mi->min_indirect_level_escape, sizeof (tree));
	}
      mi->dimension_size[level] = size;
      mi->dimension_size_orig[level] = size;
    }

  /* We don't need those anymore.  */
  for (level = mi->min_indirect_level_escape;
       level < mi->max_malloced_level; level++)
    {
      if (mi->malloc_for_level[level])
	free (mi->malloc_for_level[level]);
      mi->malloc_for_level[level] = NULL;
    }

  return 1;
}

/* Traverse the use-def chains to see if there are matrices that
   are passed through pointers and we cannot know how they are accessed.
   For each SSA-name defined by a global variable of our interest,
   we traverse the use-def chains of the SSA and follow the indirections,
   and record in what level of indirection the use of the varaible
   escapes.  A use of a pointer escapes when it is passed to a function,
   stored into memory or assigned (except in malloc and free calls).  
   if transform_accesse is true, this function calls 
   transform_access_sites() for all relevant matrices.  */
void
find_escaping_matrices_and_flatten (void)
{
  unsigned i;
  sbitmap visited_stmts_1;
  if (!gimple_in_ssa_p (cfun))
    return;
#ifdef ENABLE_CHECKING
  verify_flow_info ();
#endif
  visited_stmts_1 = sbitmap_alloc (num_ssa_names);

  if (!matrices_to_reorg)
    return;
  /* Create htap for phi nodes.  */
  htab_mat_acc_phi_nodes = htab_create (37, mat_acc_phi_hash,
					mat_acc_phi_eq, free);
  /* traverse ssa_names.  */
  for (i = 0; i < num_ssa_names; i++)
    {
      struct matrix_info tmpmi, *mi;
      tree ssa_var = ssa_name (i);
      tree rhs, lhs;

      if (!ssa_var || TREE_CODE (SSA_NAME_DEF_STMT (ssa_var)) != MODIFY_EXPR)
	continue;
      rhs = TREE_OPERAND (SSA_NAME_DEF_STMT (ssa_var), 1);
      lhs = TREE_OPERAND (SSA_NAME_DEF_STMT (ssa_var), 0);
      if (TREE_CODE (rhs) != VAR_DECL && TREE_CODE (lhs) != VAR_DECL)
	continue;

      /* If the RHS is a matrix that we want to analyze, follow the def-use
         chain for this SSA_VAR and check for escapes or apply the
         flattening.  */
      tmpmi.decl = rhs;
      if ((mi = htab_find (matrices_to_reorg, &tmpmi)))
	{
	  /* This variable will track the visited PHI nodes, so we can limit
	     its size to the maximum number of SSA names.  */
	  sbitmap_zero (visited_stmts_1);
	  analyze_matrix_accesses (mi, SSA_NAME_DEF_STMT (ssa_var), ssa_var,
				   0, false, visited_stmts_1);

	  continue;
	}

      if (transform_accesses)
	continue;

      /* If the LHS is a matrix that we want to analyze follow the def-use
         chains of all the ssa names that this statement uses and make sure
         that it leads to one and only one malloc site.
         ??? This maybe not possible.  */
      tmpmi.decl = lhs;
      if ((mi = htab_find (matrices_to_reorg, &tmpmi)))
	{
	  tree def = SSA_NAME_DEF_STMT (ssa_var);

	  sbitmap_zero (visited_stmts_1);
	  analyze_matrix_allocation_site (mi, def, 0, visited_stmts_1);
	  /* NOTE: no need to record allocation site for deletion for 
	     level 0 because this is the one that we want to keep.  */
	}

    }
  sbitmap_free (visited_stmts_1);
  if (!transform_accesses && analyze_transpose_p)
    {
      loop_optimizer_init (LOOPS_NORMAL
			   | LOOPS_HAVE_MARKED_SINGLE_EXITS);
      if (current_loops)
	scev_initialize ();
      htab_traverse (matrices_to_reorg, analyze_transpose, NULL);
      if (current_loops)
	{
	  scev_finalize ();
	  loop_optimizer_finalize ();
	  current_loops = NULL;
	}
    }
  if (transform_accesses)
    htab_traverse (matrices_to_reorg, transform_access_sites, NULL);
  else
    /* If the current function is the allocation function for any of
       the matrices we check its allocation and the escaping level.  */
    htab_traverse (matrices_to_reorg, check_allocation_function, NULL);
}

/* We know that we are allowed to perform matrix flattening (according to the
   escapre analysis), so we traverse the use-def chains of the SSA vars
   defined by the global variables pointing to the matrixes of our interest.
   in each use of the SSA we calculate the offset from the base address
   accoding to the following equation:

     a[I1][I2]...[Ik] , where D1..Dk is the length of each dimention and the
     escaping level is m <= k, and a' is the new allocated matrix, 
     will be translated to :
       
       b[I(m+1)]...[Ik]	
       
       where 
       b = a' + I1*D2...*Dm + I2*D3...Dm + ... + Im  
                                                      */

static int
transform_access_sites (void **slot, void *data ATTRIBUTE_UNUSED)
{
  tree stmts;
  block_stmt_iterator bsi;
  struct matrix_info *mi = *slot;
  int min_escape_l = mi->min_indirect_level_escape;

  if (min_escape_l < 2 || !mi->access_l)
    return 1;
  while (VARRAY_ACTIVE_SIZE (mi->access_l))
    {
      struct access_site_info *acc_info;
      tree orig, type;

      acc_info = VARRAY_TOP_GENERIC_PTR (mi->access_l);

      /* This is possible because we collect the access sites before
         we determine the final minimum indirection level.  */
      if (acc_info->level >= min_escape_l)
	{
	  free (acc_info);
	  VARRAY_POP (mi->access_l);
	  continue;
	}
      if (acc_info->is_alloc)
	{
	  if (acc_info->level >= 0 && bb_for_stmt (acc_info->stmt))
	    {
	      ssa_op_iter iter;
	      tree def;
	      tree stmt = acc_info->stmt;

	      FOR_EACH_SSA_TREE_OPERAND (def, stmt, iter, SSA_OP_VMAYDEF)
		mark_sym_for_renaming (SSA_NAME_VAR (def));
	      bsi = bsi_for_stmt (stmt);
	      if (TREE_CODE (TREE_OPERAND (acc_info->stmt, 0)) == SSA_NAME
		  && acc_info->level < min_escape_l - 1)
		{
		  imm_use_iterator imm_iter;
		  use_operand_p use_p;
		  tree use_stmt;

		  FOR_EACH_IMM_USE_STMT (use_stmt, imm_iter,
					 TREE_OPERAND (acc_info->stmt,
						       0))
		    FOR_EACH_IMM_USE_ON_STMT (use_p, imm_iter)
		  {
		    tree conv, tmp, stmts;

		    /* Emit convert statement to convert to type of use.  */
		    conv =
		      fold_build1 (CONVERT_EXPR,
				   TREE_TYPE (TREE_OPERAND
					      (acc_info->stmt, 0)),
				   TREE_OPERAND (TREE_OPERAND
						 (acc_info->stmt, 1), 0));
		    tmp =
		      create_tmp_var (TREE_TYPE
				      (TREE_OPERAND (acc_info->stmt, 0)),
				      "new");
		    add_referenced_var (tmp);
		    stmts =
		      build2 (MODIFY_EXPR,
			      TREE_TYPE (TREE_OPERAND (acc_info->stmt, 0)),
			      tmp, conv);
		    tmp = make_ssa_name (tmp, stmts);
		    TREE_OPERAND (stmts, 0) = tmp;
		    bsi = bsi_for_stmt (acc_info->stmt);
		    bsi_insert_after (&bsi, stmts, BSI_SAME_STMT);
		    SET_USE (use_p, tmp);
		  }
		}
	      if (acc_info->level < min_escape_l - 1)
		bsi_remove (&bsi, true);
	    }
	  free (acc_info);
	  VARRAY_POP (mi->access_l);
	  continue;
	}
      orig = TREE_OPERAND (acc_info->stmt, 1);
      type = TREE_TYPE (orig);
      if (TREE_CODE (orig) == INDIRECT_REF
	  && acc_info->level < min_escape_l - 1)
	{
	  /* Replace the INDIRECT_REF with NOP (cast) usually we are casting
	     from "pointer to type" to "type".  */
	  orig = build1 (NOP_EXPR, TREE_TYPE (orig), TREE_OPERAND (orig, 0));
	  TREE_OPERAND (acc_info->stmt, 1) = orig;
	}
      else if (TREE_CODE (orig) == PLUS_EXPR
	       && acc_info->level < (min_escape_l))
	{
	  imm_use_iterator imm_iter;
	  use_operand_p use_p;

	  int k = acc_info->level;
	  tree num_elements, total_elements, total_size;
	  tree tmp, tmp1;
	  tree d_size1, d_size2;

	  tree d_size = mi->dimension_size[k];
	  tree d_size_orig = mi->dimension_size_orig[k];
	  tree d_type_size =
	    build_int_cst (type, mi->dimension_type_size[k + 1]);
	  /* We already make sure in the analysis that the first operand
	     is the base and the second is the offset.  */
	  tree offset = acc_info->offset;
	  /* We transform BASE + OFFSET to:
	     TMP = OFFSET / DIMENSION_TYPE_SIZE
	     TMP = TMP * DIMENSION_SIZE
	     TMP = TMP * ELEMENT_SIZE
	     replace BASE + OFFSET with BASE + TMP in the statement.  */

	  tmp = create_tmp_var (TREE_TYPE (offset), "mtt_offset");
	  add_referenced_var (tmp);

	  /* OFFSET / DIMENSION_TYPE_SIZE  */
	  num_elements = build2 (TRUNC_DIV_EXPR, type, offset, d_type_size);
	  /* TMP = TMP * DIMENSION_SIZE */

	  d_size_orig =
	    build2 (TRUNC_DIV_EXPR, type, d_size_orig, d_type_size);
	  d_size1 =
	    build2 (TRUNC_DIV_EXPR, TREE_TYPE (d_size), d_size, d_size_orig);
	  d_size2 = force_gimple_operand (d_size1, &stmts, true, NULL);
	  if (stmts)
	    {
	      tree_stmt_iterator tsi;

	      for (tsi = tsi_start (stmts); !tsi_end_p (tsi); tsi_next (&tsi))
		mark_new_vars_to_rename (tsi_stmt (tsi));
	      bsi = bsi_for_stmt (acc_info->stmt);
	      bsi_insert_before (&bsi, stmts, BSI_SAME_STMT);
	    }
	  stmts = build2 (MODIFY_EXPR, type, tmp, d_size2);
	  tmp = make_ssa_name (tmp, stmts);
	  TREE_OPERAND (stmts, 0) = tmp;
	  add_referenced_var (d_size);
	  add_referenced_var (mi->dimension_size_orig[k]);
	  mark_new_vars_to_rename (stmts);
	  bsi = bsi_for_stmt (acc_info->stmt);
	  bsi_insert_before (&bsi, stmts, BSI_SAME_STMT);
	  SSA_NAME_DEF_STMT (tmp) = stmts;
	  total_elements = build2 (MULT_EXPR, type, num_elements, tmp);
	  total_size = total_elements;
	  tmp1 = force_gimple_operand (total_size, &stmts, true, NULL);
	  if (TREE_CODE (offset) == SSA_NAME)
	    {
	      tree use_stmt;

	      FOR_EACH_IMM_USE_STMT (use_stmt, imm_iter, offset)
		FOR_EACH_IMM_USE_ON_STMT (use_p, imm_iter)
		if (use_stmt == acc_info->stmt)
		  SET_USE (use_p, tmp1);
	    }
	  else
	    {
	      gcc_assert (TREE_CODE (offset) == INTEGER_CST);
	      TREE_OPERAND (orig, 1) = tmp1;
	    }
	  if (stmts)
	    bsi_insert_before (&bsi, stmts, BSI_SAME_STMT);
	}
      /* ??? meanwhile this happens because we record the same access
         site more than once; we should be using a hash table to
         avoid this and insert the STMT of the access site only
         once.
         else
         gcc_unreachable (); */
      free (acc_info);
      VARRAY_POP (mi->access_l);
    }
  update_ssa (TODO_update_ssa);
#ifdef ENABLE_CHECKING
  verify_ssa (true);
#endif
  return 1;
}

/* Sort A array of counts. Arrange DIM_MAP to reflect the new order.  */
static void
sort_dim_hot_level (gcov_type * a, int *dim_map, int n)
{
  int i, j, tmp;

  for (i = 0; i < n - 1; i++)
    {
      for (j = 0; j < n - 1 - i; j++)
	if (a[j + 1] < a[j])
	  {			/* compare the two neighbors */
	    tmp = a[j];		/* swap a[j] and a[j+1]      */
	    a[j] = a[j + 1];
	    a[j + 1] = tmp;
	    tmp = dim_map[j];
	    dim_map[j] = dim_map[j + 1];
	    dim_map[j + 1] = tmp;
	  }
    }
}


/* Replace multiple mallocs (one for each dimension) to one malloc
   with the size of DIM1*DIM2*...*DIMN*size_of_element
   Make sure that we hold the size in the malloc site inside a
   new global varaible; this way we ensure that the size doesn't
   change and it is accessible from all the other functions that
   uses the matrix.  */
static int
transform_allocation_sites (void **slot, void *data ATTRIBUTE_UNUSED)
{
  int i;
  struct matrix_info *mi;
  tree type, call_stmt_0, malloc_stmt, oldfn, stmts, prev_dim_size, use_stmt;
  struct cgraph_node *c_node;
  struct cgraph_edge *e;
  block_stmt_iterator bsi;
  struct malloc_call_data mcd;
  HOST_WIDE_INT element_size;

  imm_use_iterator imm_iter;
  use_operand_p use_p;
  tree old_size_0, tmp;
  int min_escape_l;

  mi = *slot;

  min_escape_l = mi->min_indirect_level_escape;

  if (mi->min_indirect_level_escape < 2)
    return 1;

  mi->dim_map = (int *) xcalloc (mi->min_indirect_level_escape, sizeof (int));
  for (i = 0; i < mi->min_indirect_level_escape; i++)
    mi->dim_map[i] = i;
  if (check_transpose_p)
    {
      int i;

      if (dump_file)
	for (i = 0; i < min_escape_l; i++)
	  {
	    fprintf (dump_file, "dim %d before sort\n", i);
	    if (mi->dim_hot_level)
	      fprintf (dump_file, "count is  " HOST_WIDE_INT_PRINT_DEC
		       "  \n", (HOST_WIDE_INT) mi->dim_hot_level[i]);
	  }

      sort_dim_hot_level (mi->dim_hot_level, mi->dim_map,
			  mi->min_indirect_level_escape);
      if (dump_file)
	for (i = 0; i < min_escape_l; i++)
	  {
	    fprintf (dump_file, "dim %d after sort\n", i);
	    if (mi->dim_hot_level)
	      fprintf (dump_file, "count is  " HOST_WIDE_INT_PRINT_DEC
		       "  \n", (HOST_WIDE_INT) mi->dim_hot_level[i]);
	  }
      if (dump_file)
	for (i = 0; i < mi->min_indirect_level_escape; i++)
	  {
	    fprintf (dump_file, "dim_map[%d] after sort %d\n", i,
		     mi->dim_map[i]);
	    if (mi->dim_map[i] != i)
	      fprintf (dump_file,
		       "Transposed dimensions: dim %d is now dim %d\n",
		       mi->dim_map[i], i);
	  }
    }
  else
    {
      for (i = 0; i < mi->min_indirect_level_escape; i++)
	mi->dim_map[i] = i;
    }
  /* Call statement of allocation site of level 0.  */
  call_stmt_0 = mi->malloc_for_level[0]->stmt;

  /* Finds the correct malloc (realloc/calloc) information.  */
  collect_data_for_malloc_call (call_stmt_0, &mcd);

  /* Can be only malloc (not calloc nor realloc).  */
  gcc_assert (!mcd.ptr_var && !mcd.num_elts);

  mi->dimension_size[0] = mcd.size_var;
  mi->dimension_size_orig[0] = mcd.size_var;
  /* Make sure that the varaibles in the size expression for
     all the dimensions (above level 0) aren't modified in
     the allocation function.  */
  for (i = 1; i < mi->min_indirect_level_escape; i++)
    {
      tree t;

      /* mi->dimension_size must contain the expression of the size calculated
         in check_allocation_function.  */
      gcc_assert (mi->dimension_size[i]);

      t = walk_tree_without_duplicates (&(mi->dimension_size[i]),
					check_var_notmodified_p,
					mi->allocation_function_decl);
      if (t != NULL_TREE)
	{
	  mark_min_matrix_escape_level (mi, i, t);
	  break;
	}
    }

  if (mi->min_indirect_level_escape < 2)
    return 1;

  /* Since we should make sure that the size expression is available
     before the call to malloc of level 0.  */
  bsi = bsi_for_stmt (call_stmt_0);

  /* Find out the size of each dimention by looking at the malloc
     sites and create a global varaible to hold it.
     We add the assigment to the global before the malloc of level 0.  */

  /* To be able to produce gimple temporaries.  */
  oldfn = current_function_decl;
  current_function_decl = mi->allocation_function_decl;
  cfun = DECL_STRUCT_FUNCTION (mi->allocation_function_decl);

  /* Set the dimension sizes as follows:
     DIM_SIZE[i] = DIM_SIZE[n] * ... * DIM_SIZE[i]
     where n is the maximum non escaping level.  */
  element_size = mi->dimension_type_size[mi->min_indirect_level_escape];
  prev_dim_size = NULL_TREE;

  /* Create the statements for the original dimension sizes.  */
  for (i = mi->min_indirect_level_escape - 1; i >= 0; i--)
    {
      tree tmp, dim_size_orig, dim_var_orig;
      tree_stmt_iterator tsi;

      type = TREE_TYPE (mi->dimension_size_orig[i]);
      dim_size_orig = mi->dimension_size_orig[i];
      dim_var_orig = add_new_global_for_decl (TREE_TYPE (dim_size_orig));

      dim_size_orig =
	force_gimple_operand (dim_size_orig, &stmts, true, NULL);
      if (stmts)
	{
	  for (tsi = tsi_start (stmts); !tsi_end_p (tsi); tsi_next (&tsi))
	    mark_new_vars_to_rename (tsi_stmt (tsi));
	  bsi_insert_before (&bsi, stmts, BSI_SAME_STMT);
	  bsi = bsi_for_stmt (call_stmt_0);
	}
      /* GLOBAL_HOLDING_THE_SIZE = DIM_SIZE.  */
      tmp = build2 (MODIFY_EXPR, type, dim_var_orig, dim_size_orig);
      TREE_OPERAND (tmp, 0) = dim_var_orig;
      mark_new_vars_to_rename (tmp);
      bsi_insert_before (&bsi, tmp, BSI_NEW_STMT);
      bsi = bsi_for_stmt (call_stmt_0);

      mi->dimension_size_orig[i] = dim_var_orig;
    }
  for (i = mi->min_indirect_level_escape - 1; i >= 0; i--)
    {
      tree dim_size, dim_var, tmp;
      tree d_type_size;
      tree_stmt_iterator tsi;

      /* Now put the size expression in a global variable and intialize it to
         the size expression before the malloc of level 0.  */
      dim_var =
	add_new_global_for_decl (TREE_TYPE
				 (mi->dimension_size_orig[mi->dim_map[i]]));
      type = TREE_TYPE (mi->dimension_size_orig[mi->dim_map[i]]);
      d_type_size =
	build_int_cst (type, mi->dimension_type_size[mi->dim_map[i] + 1]);

      /* DIM_SIZE = MALLOC_SIZE_PARAM / TYPE_SIZE.  */
      dim_size =
	build2 (TRUNC_DIV_EXPR, type, mi->dimension_size_orig[mi->dim_map[i]],
		d_type_size);
      if (!prev_dim_size)
	prev_dim_size = build_int_cst (type, element_size);
      dim_size = build2 (MULT_EXPR, type, dim_size, prev_dim_size);
      dim_size = force_gimple_operand (dim_size, &stmts, true, NULL);
      if (stmts)
	{
	  for (tsi = tsi_start (stmts); !tsi_end_p (tsi); tsi_next (&tsi))
	    mark_new_vars_to_rename (tsi_stmt (tsi));
	  bsi_insert_before (&bsi, stmts, BSI_SAME_STMT);
	  bsi = bsi_for_stmt (call_stmt_0);
	}
      /* GLOBAL_HOLDING_THE_SIZE = DIM_SIZE.  */
      tmp = build2 (MODIFY_EXPR, type, dim_var, dim_size);
      TREE_OPERAND (tmp, 0) = dim_var;
      mark_new_vars_to_rename (tmp);
      bsi_insert_before (&bsi, tmp, BSI_NEW_STMT);
      bsi = bsi_for_stmt (call_stmt_0);

      prev_dim_size = mi->dimension_size[mi->dim_map[i]] = dim_var;
    }
  update_ssa (TODO_update_ssa);
  /* Replace the malloc size argument in the malloc of level 0 to be
     the size of all the dimensions.  */
  malloc_stmt = TREE_OPERAND (call_stmt_0, 1);
  c_node = cgraph_node (mi->allocation_function_decl);
  old_size_0 = TREE_VALUE (TREE_OPERAND (malloc_stmt, 1));
  bsi = bsi_for_stmt (call_stmt_0);
  tmp =
    force_gimple_operand (mi->dimension_size[mi->dim_map[0]], &stmts, true,
			  NULL);
  if (stmts)
    {
      tree_stmt_iterator tsi;

      for (tsi = tsi_start (stmts); !tsi_end_p (tsi); tsi_next (&tsi))
	mark_new_vars_to_rename (tsi_stmt (tsi));
      bsi_insert_before (&bsi, stmts, BSI_SAME_STMT);
      bsi = bsi_for_stmt (call_stmt_0);
    }
  if (TREE_CODE (old_size_0) == SSA_NAME)
    {
      FOR_EACH_IMM_USE_STMT (use_stmt, imm_iter, old_size_0)
	FOR_EACH_IMM_USE_ON_STMT (use_p, imm_iter)
	if (use_stmt == call_stmt_0)
	  SET_USE (use_p, tmp);
    }
  /* When deleting the calls to malloc we need also to remove the edge from
     the call graph to keep it consistant.  Notice that cgraph_edge may
     create a new node in the call graph if there is no node for the given
     declaration; this shouldn't be the case but currently there is no way to
     check this outside of "cgraph.c".  */
  for (i = 1; i < mi->min_indirect_level_escape; i++)
    {
      block_stmt_iterator bsi;
      tree use_stmt1 = NULL;
      tree call;

      tree call_stmt = mi->malloc_for_level[i]->stmt;
      call = TREE_OPERAND (call_stmt, 1);
      gcc_assert (TREE_CODE (call) == CALL_EXPR);
      e = cgraph_edge (c_node, call_stmt);
      gcc_assert (e);
      cgraph_remove_edge (e);
      bsi = bsi_for_stmt (call_stmt);
      /* Remove the call stmt.  */
      bsi_remove (&bsi, true);
      /* remove the type cast stmt.  */
      FOR_EACH_IMM_USE_STMT (use_stmt, imm_iter, TREE_OPERAND (call_stmt, 0))
      {
	use_stmt1 = use_stmt;
	bsi = bsi_for_stmt (use_stmt);
	bsi_remove (&bsi, true);
      }
      /* Remove the assignment of the allocated area.  */
      FOR_EACH_IMM_USE_STMT (use_stmt, imm_iter, TREE_OPERAND (use_stmt1, 0))
      {
	bsi = bsi_for_stmt (use_stmt);
	bsi_remove (&bsi, true);
      }
    }
  update_ssa (TODO_update_ssa);
#ifdef ENABLE_CHECKING
  verify_ssa (true);
#endif
  /* Delete the calls to free.  */
  for (i = 1; i < mi->min_indirect_level_escape; i++)
    {
      block_stmt_iterator bsi;
      tree call;

      /* ??? wonder why this case is possible but we failed on it once.  */
      if (!mi->free_stmts[i].stmt)
	continue;

      call = TREE_OPERAND (mi->free_stmts[i].stmt, 1);
      c_node = cgraph_node (mi->free_stmts[i].func);

      gcc_assert (TREE_CODE (mi->free_stmts[i].stmt) == CALL_EXPR);
      e = cgraph_edge (c_node, mi->free_stmts[i].stmt);
      gcc_assert (e);
      cgraph_remove_edge (e);
      current_function_decl = mi->free_stmts[i].func;
      cfun = DECL_STRUCT_FUNCTION (mi->free_stmts[i].func);
      bsi = bsi_for_stmt (mi->free_stmts[i].stmt);
      bsi_remove (&bsi, true);
    }
  /* Return to the previous situation.  */
  current_function_decl = oldfn;
  cfun = oldfn ? DECL_STRUCT_FUNCTION (oldfn) : NULL;
  return 1;

}


/* Print out the results of the escaping analysis.  */
static int
dump_matrix_reorg_analysis (void **slot, void *data ATTRIBUTE_UNUSED)
{
  struct matrix_info *mi = *slot;

  if (!dump_file)
    return 1;
  fprintf (dump_file, "Matrix \"%s\"; Escaping Level: %d, Num Dims: %d,",
	   get_name (mi->decl), mi->min_indirect_level_escape, mi->num_dims);
  fprintf (dump_file, " Malloc Dims: %d, ", mi->max_malloced_level);
  fprintf (dump_file, "\n");
  if (mi->min_indirect_level_escape >= 2)
    fprintf (dump_file, "Flattened %d dimensions \n",
	     mi->min_indirect_level_escape);
  return 1;
}


/* Perform matrix flattening.  */

static unsigned int
matrix_reorg (void)
{
  struct cgraph_node *node;

  if (!flag_matrix_flattening)
    return 0;
  if (profile_info)
    check_transpose_p = 1;
  else
    check_transpose_p = 0;
  /* If there are hand written vectors, we skip this optimization.  */
  for (node = cgraph_nodes; node; node = node->next)
    if (!flatten_matrices (node))
      return 0;
  matrices_to_reorg = htab_create (37, mtt_info_hash, mtt_info_eq, mat_free);
  /* Find and record all potential matrices in the program.  */
  find_matrices_decl ();
  /* Analyze the accesses of the matrices (escaping analysis).  */
  transform_accesses = false;
  analyze_transpose_p = false;
  for (node = cgraph_nodes; node; node = node->next)
    if (node->analyzed)
      ipa_intra_ssa_passes (node->decl);
  if (check_transpose_p)
    {
      analyze_transpose_p = true;
      for (node = cgraph_nodes; node; node = node->next)
	if (node->analyzed)
	  ipa_intra_ssa_passes (node->decl);
      analyze_transpose_p = false;
    }
  htab_traverse (matrices_to_reorg, transform_allocation_sites, NULL);
  /* Now transform the accesses.  */
  transform_accesses = true;
  for (node = cgraph_nodes; node; node = node->next)
    if (node->analyzed)
      ipa_intra_ssa_passes (node->decl);
  htab_traverse (matrices_to_reorg, dump_matrix_reorg_analysis, NULL);

  current_function_decl = NULL;
  cfun = NULL;
  matrices_to_reorg = NULL;
  return 0;
}

/* Perform analysis or transformation for FN.  */
static void
ipa_intra_ssa_passes (tree fn)
{
  tree temp_fn;

  temp_fn = current_function_decl;
  current_function_decl = fn;
  push_cfun (DECL_STRUCT_FUNCTION (fn));
  bitmap_obstack_initialize (NULL);
  tree_register_cfg_hooks ();
  ssa_analyze_matrix ();
  free_dominance_info (CDI_DOMINATORS);
  free_dominance_info (CDI_POST_DOMINATORS);
  pop_cfun ();
  current_function_decl = temp_fn;
}

/* Perform analysis or transformation.  */
static void
ssa_analyze_matrix (void)
{
  find_escaping_matrices_and_flatten ();
}

/* The condition for matrix flattening to be performed.  */
static bool
gate_matt_flatten (void)
{
  return flag_matrix_flattening /*&& flag_whole_program */ ;
}

struct tree_opt_pass pass_ipa_matrix_flatten = {
  "mreorg",			/* name */
  gate_matt_flatten,		/* gate */
  matrix_reorg,			/* execute */
  NULL,				/* sub */
  NULL,				/* next */
  0,				/* static_pass_number */
  0,				/* tv_id */
  0,				/* properties_required */
  PROP_trees,			/* properties_provided */
  0,				/* properties_destroyed */
  0,				/* todo_flags_start */
  TODO_dump_cgraph | TODO_dump_func,	/* todo_flags_finish */
  0				/* letter */
};
