/* Loop Vectorization
   Copyright (C) 2003, 2004 Free Software Foundation, Inc.
   Contributed by Dorit Naishlos <dorit@il.ibm.com>

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

/* Loop Vectorization Pass.

   This pass tries to vectorize loops. This first implementation focuses on
   simple inner-most loops, with no conditional control flow, and a set of
   simple operations which vector form can be expressed using existing
   tree codes (PLUS, MULT etc).

   For example, the vectorizer transforms the following simple loop:

	short a[N]; short b[N]; short c[N]; int i;

	for (i=0; i<N; i++){
	  a[i] = b[i] + c[i];
	}

   as if it was manually vectorized by rewriting the source code into:

	typedef int __attribute__((mode(V8HI))) v8hi;
	short a[N];  short b[N]; short c[N];   int i;
	v8hi *pa = (v8hi*)a, *pb = (v8hi*)b, *pc = (v8hi*)c;
	v8hi va, vb, vc;

	for (i=0; i<N/8; i++){
	  vb = pb[i];
	  vc = pc[i];
	  va = vb + vc;
	  pa[i] = va;
	}

	The main entry to this pass is vectorize_loops(), in which for each
   the vectorizer applies a set of analyses on a given set of loops,
   followed by the actual vectorization transformation for the loops that
   had successfully passed the analysis phase.

	Throughout this pass we make a distinction between two types of
   data: scalars (which are represented by SSA_NAMES), and data-refs. These
   are handled separately both by the analyzer and the loop-transformer.
   Currently, the vectorizer only supports simple data-refs which are
   limited to ARRAY_REFS that represent one dimensional arrays which base is
   an array (not a pointer), and have a simple (consecutive) access pattern.

   Analysis phase:
   ===============
	The driver for the analysis phase is vect_analyze_loop_nest().
   which applies a set of loop analyses. Some of the analyses rely on the
   monotonic evolution analyzer developed by Sebastian Pop.

	During the analysis phase the vectorizer records some information
   per stmt in a stmt_vec_info which is attached to each stmt in the loop,
   as well as general information about the loop as a whole, which is
   recorded in a loop_vec_info struct attached to each loop.

   Transformation phase:
   =====================
	The loop transformation phase scans all the stmts in the loop, and
   creates a vector stmt (or a sequence of stmts) for each scalar stmt S in
   the loop that needs to be vectorized. It insert the vector code sequence
   just before the scalar stmt S, and records a pointer to the vector code
   in STMT_VINFO_VEC_STMT (stmt_info) (where stmt_info is the stmt_vec_info
   struct that is attached to S). This pointer is used for the vectorization
   of following stmts which use the defs of stmt S. Stmt S is removed
   only if it has side effects (like changing memory). If stmt S does not
   have side effects, we currently rely on dead code elimination for
   removing it.

	For example, say stmt S1 was vectorized into stmt VS1:

   VS1: vb = px[i];
   S1:	b = x[i];    STMT_VINFO_VEC_STMT (stmt_info (S1)) = VS1
   S2:  a = b;

   To vectorize stmt S2, the vectorizer first finds the stmt that defines
   the operand 'b' (S1), and gets the relevant vector def 'vb' from the
   vector stmt VS1 pointed by STMT_VINFO_VEC_STMT (stmt_info (S1)). The
   resulting sequence would be:

   VS1: vb = px[i];
   S1:	b = x[i];	STMT_VINFO_VEC_STMT (stmt_info (S1)) = VS1
   VS2: va = vb;
   S2:  a = b;          STMT_VINFO_VEC_STMT (stmt_info (S2)) = VS2

	Operands that are not SSA_NAMEs, are currently limited to array
   references appearing in load/store operations (like 'x[i]' in S1), and
   are handled differently.

   Target modelling:
   =================
	Currently the only target specific information that is used is the
   size of the vector (in bytes) - "UNITS_PER_SIMD_WORD", and a target hook
   "vectype_for_scalar_type" that for a given (scalar) machine mode returns
   the vector machine_mode to be used. Targets that can support different
   sizes of vectors, for now will need to specify one value for
   "UNITS_PER_SIMD_WORD". More flexibility will be added in the future.

	Since we only vectorize operations which vector form can be
   expressed using existing tree codes, to verify that an operation is
   supported the vectorizer checks the relevant optab at the relevant
   machine_mode (e.g, add_optab->handlers[(int) V8HImode].insn_code).  If
   the value found is CODE_FOR_nothing, then there's no target support, and
   we can't vectorize the stmt. Otherwise - the stmt is transformed.


   For additional information on this project see:
   http://gcc.gnu.org/projects/tree-ssa/vectorization.html
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "errors.h"
#include "ggc.h"
#include "tree.h"
#include "target.h"

#include "rtl.h"
#include "basic-block.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "timevar.h"
#include "cfgloop.h"
#include "tree-fold-const.h"
#include "expr.h"
#include "optabs.h"
#include "tree-chrec.h"
#include "tree-data-ref.h"
#include "tree-scalar-evolution.h"
#include "tree-vectorizer.h"
#include "tree-pass.h"

/* CHECKME: check for unnecessary include files.  */

/* Main analysis functions.  */
static loop_vec_info vect_analyze_loop (struct loop *);
static loop_vec_info vect_analyze_loop_form (struct loop *);
static bool vect_analyze_data_refs (loop_vec_info);
static bool vect_mark_stmts_to_be_vectorized (loop_vec_info);
static bool vect_analyze_scalar_cycles (loop_vec_info);
static bool vect_analyze_data_ref_dependences (loop_vec_info);
static bool vect_analyze_data_ref_accesses (loop_vec_info);
static bool vect_analyze_operations (loop_vec_info);

/* Main code transformation functions.  */
static void vect_transform_loop (loop_vec_info);
static void vect_transform_loop_bound (loop_vec_info);
static bool vect_transform_stmt (tree, block_stmt_iterator *);
static tree vect_transform_load (tree, block_stmt_iterator *);
static tree vect_transform_store (tree, block_stmt_iterator *);
static tree vect_transform_binop (tree, block_stmt_iterator *);
static void vect_align_data_ref (tree, tree);

/* Utility functions for the analyses.  */
static bool vect_is_supportable_binop (tree);
static bool vect_is_supportable_store (tree);
static bool vect_is_supportable_load (tree);
static bool vect_is_simple_use (tree , struct loop *);
static bool exist_non_indexing_operands_for_use_p (tree, tree);
static bool vect_is_simple_iv_evolution (unsigned, tree, tree *, tree *, bool);
static void vect_mark_relevant (varray_type, tree);
static bool vect_stmt_relevant_p (tree, loop_vec_info);
static tree vect_get_loop_niters (struct loop *, int *);
static bool vect_analyze_data_ref_access (struct data_reference *);
static bool vect_analyze_data_ref_dependence
  (struct data_reference *, struct data_reference *);
static bool vect_get_array_first_index (tree, int *);

/* Utility functions for the code transformation.  */
static tree vect_create_destination_var (tree, tree);
static tree vect_create_data_ref (tree, tree, block_stmt_iterator *);
static tree vect_create_index_for_array_ref (tree, block_stmt_iterator *);
static tree get_vectype_for_scalar_type (tree);
static tree vect_get_new_vect_var (tree, enum vect_var_kind, const char *);
static tree vect_get_vec_def_for_operand (tree, tree);

/* General untility functions (CHECKME: where do they belong).  */
static tree get_array_base (tree);

/* Utilities for creation and deletion of vec_info structs.  */
loop_vec_info new_loop_vec_info (struct loop *loop);
void destroy_loop_vec_info (loop_vec_info);
stmt_vec_info new_stmt_vec_info (tree stmt, struct loop *loop);


/* Function new_stmt_vec_info.

   Create and initialize a new stmt_vec_info struct for STMT.  */

stmt_vec_info
new_stmt_vec_info (tree stmt, struct loop *loop)
{
  stmt_vec_info res;
  res = (stmt_vec_info) xcalloc (1, sizeof (struct _stmt_vec_info));

  STMT_VINFO_TYPE (res) = undef_vec_info_type;
  STMT_VINFO_STMT (res) = stmt;
  STMT_VINFO_LOOP (res) = loop;
  STMT_VINFO_RELEVANT_P (res) = 0;
  STMT_VINFO_VECTYPE (res) = NULL;
  STMT_VINFO_VEC_STMT (res) = NULL;
  STMT_VINFO_DATA_REF (res) = NULL;

  return res;
}


/* Function new_loop_vec_info.

   Create and initialize a new loop_vec_info struct for LOOP, as well as
   stmt_vec_info structs for all the stmts in LOOP.  */

loop_vec_info
new_loop_vec_info (struct loop *loop)
{
  loop_vec_info res;
  basic_block *bbs;
  block_stmt_iterator si;
  unsigned int i;

  res = (loop_vec_info) xcalloc (1, sizeof (struct _loop_vec_info));

  bbs = get_loop_body (loop);

  /* Create stmt_info for all stmts in the loop.  */
  for (i = 0; i < loop->num_nodes; i++)
    {
      basic_block bb = bbs[i];
      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  tree stmt = bsi_stmt (si);
	  stmt_ann_t ann;

	  get_stmt_operands (stmt);
	  ann = stmt_ann (stmt);
	  set_stmt_info (ann, new_stmt_vec_info (stmt, loop));
	}
    }

  LOOP_VINFO_LOOP (res) = loop;
  LOOP_VINFO_BBS (res) = bbs;
  LOOP_VINFO_EXIT_COND (res) = NULL;
  LOOP_VINFO_NITERS (res) = -1;
  LOOP_VINFO_VECTORIZABLE_P (res) = 0;
  LOOP_VINFO_VECT_FACTOR (res) = 0;
  VARRAY_GENERIC_PTR_INIT (LOOP_VINFO_DATAREF_WRITES (res), 20,
			   "loop_write_datarefs");
  VARRAY_GENERIC_PTR_INIT (LOOP_VINFO_DATAREF_READS (res), 20,
			   "loop_read_datarefs");
  return res;
}


/* Function destroy_loop_vec_info.  */

void
destroy_loop_vec_info (loop_vec_info loop_vinfo)
{
  struct loop *loop;
  basic_block *bbs;
  int nbbs;
  block_stmt_iterator si;
  int j;

  if (!loop_vinfo)
    return;

  loop = LOOP_VINFO_LOOP (loop_vinfo);

  bbs = LOOP_VINFO_BBS (loop_vinfo);
  nbbs = loop->num_nodes;

  for (j = 0; j < nbbs; j++)
    {
      basic_block bb = bbs[j];
      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  tree stmt = bsi_stmt (si);
	  stmt_ann_t ann = stmt_ann (stmt);
	  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
	  free (stmt_info);
	  set_stmt_info (ann, NULL);
	}
    }

  free (LOOP_VINFO_BBS (loop_vinfo));
  varray_clear (LOOP_VINFO_DATAREF_WRITES (loop_vinfo));
  varray_clear (LOOP_VINFO_DATAREF_READS (loop_vinfo));

  free (loop_vinfo);
}


/* Function vect_get_new_vect_var.

   Return a name for a new variable.
   The current naming scheme appends the prefix "vect_" or "vect_p" to 
   vectorizer generated variables, and appends that to NAME if given. 

   CHECKME: naming scheme ok?  */

static tree
vect_get_new_vect_var (tree type, enum vect_var_kind var_kind, const char *name)
{
  const char *prefix;
  int prefix_len;
  char *vect_var_name;
  tree new_vect_var;

  if (var_kind == vect_simple_var)
    prefix = "vect_"; 
  else
    prefix = "vect_p";

  prefix_len = strlen (prefix);

  if (name)
    {
      vect_var_name = (char *) xmalloc (strlen (name) + prefix_len + 1);
      sprintf (vect_var_name, "%s%s", prefix, name);
    }
  else
    {
      vect_var_name = (char *) xmalloc (prefix_len + 1);
      sprintf (vect_var_name, "%s", prefix);
    }

  new_vect_var = create_tmp_var (type, vect_var_name);

  free (vect_var_name);
  return new_vect_var;
}


/* POINTER_ARITHMETIC

   CHECKME: The RTL expander does not like an ARRAY_REF where the base is a
   pointer; Until this is fixed, instead of generating

   var = p[i]

   we generate the following pointer arithmetic sequence:

   1. T0 = (unsigned int)i
   2. T1 = T0 * N	(N is the size of the vector in bytes)
   3. T2 = p + T1
   4. var = (*T2)
*/
#define POINTER_ARITHMETIC 1


/* Function create_index_for_array_ref.

   Create an offset/index to be used to access a memory location.
   Input:

   STMT: The stmt that contains a data reference to the memory location.

   BSI: The block_stmt_iterator where STMT is. Any new stmts created by this
        function can be added here, or in the loop pre-header.

   Output:
   If POINTER_ARITHMETIC is defined, this functions returns an offset that
   will be added to a base pointer and used to refer to a memory location.
   E.g., it will generate stmts 1 and 2 above, and return T1.

   FORNOW: we are not trying to be efficient, and just creating the code
   sequence each time from scratch, even if the same offset can be reused.
   TODO: record the index in the array_ref_info or the stmt info and reuse
   it.

   If POINTER_ARITHMETIC is undefined, this functions returns an index that
   will be used to index an array, using a pointer as a base.

   FORNOW: We are only handling array accesses with step 1, so the same
   index as for the scalar access can be reused.

   CHECKME: consider using a new index with step = vectorization_factor.
   This depends on how we want to handle the loop bound.  */

static tree
vect_create_index_for_array_ref (tree stmt, block_stmt_iterator *bsi)
{
  tree T0, T1, vec_stmt, mult_expr, new_temp;
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  struct loop *loop = STMT_VINFO_LOOP (stmt_info);
  struct data_reference *dr = STMT_VINFO_DATA_REF (stmt_info);
  tree expr = DR_REF (dr);
  varray_type access_fns = DR_ACCESS_FNS (dr);
  tree access_fn;
  tree new_indx, scalar_indx; 
  int init_val, step_val;
  tree init, step;
  const char *new_name;
  block_stmt_iterator pre_header_bsi;
  loop_vec_info loop_info = loop->aux;
  int vectorization_factor = LOOP_VINFO_VECT_FACTOR (loop_info);
  tree step_stmt;
  bool ok;
  int array_first_index;
  int vec_init_val;

  if (TREE_CODE (expr) != ARRAY_REF)
    abort ();

  /* FORNOW: handle only one dimensional arrays.
     This restriction will be relaxed in the future.  */
  /* CHECKME */
  if (VARRAY_ACTIVE_SIZE (access_fns) != 1)
    abort ();

  access_fn = DR_ACCESS_FN (dr, 0);

  if (!vect_is_simple_iv_evolution (loop_num (loop), access_fn, &init, &step, 
	true))
    abort ();

  /** Handle initialization.  **/

  if (TREE_CODE (init) != INTEGER_CST
      || TREE_CODE (step) != INTEGER_CST)
    abort ();	

  /* CHECKME */
  if (TREE_INT_CST_HIGH (init) != 0
      || TREE_INT_CST_HIGH (step) != 0)
    abort ();

  init_val = TREE_INT_CST_LOW (init); 
  step_val = TREE_INT_CST_LOW (step);

  /** Handle initialization.  **/

  scalar_indx =  TREE_OPERAND (expr, 1); 

  /* Create a new variable and initialize it in the loop pre-header.  */

  new_indx = vect_get_new_vect_var (unsigned_intSI_type_node, 
					vect_simple_var, "iv_");
  add_referenced_tmp_var (new_indx); 	
  bitmap_set_bit (vars_to_rename, var_ann (new_indx)->uid);

  /* The actual index depends on the (mis)alignment of the access.
     FORNOW: we verify that both the array base and the access are
     aligned, so the index in the vectorized access is simply
     init_val/vectorization_factor.  */

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "creating update chain:\n");

  ok = vect_get_array_first_index (expr, &array_first_index);
  if (!ok)
    abort ();
  vec_init_val = (init_val - array_first_index) / vectorization_factor 
	+ array_first_index;
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "vec_init_indx = %d\n", vec_init_val);

  vec_stmt = build (MODIFY_EXPR, unsigned_intSI_type_node, new_indx,
		build_int_2 (vec_init_val, 0));
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      print_generic_expr (tree_dump_file, vec_stmt, TDF_SLIM);
      fprintf (tree_dump_file, "\n");
    }

  pre_header_bsi = bsi_last (loop->pre_header);
  if (!bsi_end_p (pre_header_bsi)
      && is_ctrl_stmt (bsi_stmt (pre_header_bsi)))
    bsi_insert_before (&pre_header_bsi, vec_stmt, BSI_NEW_STMT);	
  else
    bsi_insert_after (&pre_header_bsi, vec_stmt, BSI_NEW_STMT);	

  step_stmt = build (MODIFY_EXPR, unsigned_intSI_type_node, new_indx,
			build (PLUS_EXPR, unsigned_intSI_type_node,
			TREE_OPERAND (vec_stmt, 0), integer_one_node)); 
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      print_generic_expr (tree_dump_file, step_stmt, TDF_SLIM);
      fprintf (tree_dump_file, "\n");
    }
  /* Have bsi point to new stmt.  */
  bsi_insert_before (bsi, step_stmt, BSI_NEW_STMT);	


#ifndef POINTER_ARITHMETIC

  /* CHECKME: for now return the same index. May want to handle this
     differently in the future.  */
  return new_indx;


#else /* POINTER_ARITHMETIC */


  /* Given the array reference array[i], create
     'new_idx = (unsigned int)i * N'
     where N = UNITS_PER_SIMD_WORD

     FORNOW: We only handle loops in which all stmts operate on the same
             data type; therefore, for all the array accesses in the loop,
             the following should hold:
             UNITS_PER_SIMD_WORD =
                      vectorization_factor * sizeof (data_type (array)).

     FORNOW: The access pattern of all arrays in the loop is step 1.  */

  /*** create: unsigned int T0; ***/

  new_name = get_name (scalar_indx);
  if (!new_name)
    new_name = "iv_";
  T0 = vect_get_new_vect_var (unsigned_intSI_type_node, 
				vect_simple_var, new_name);
  add_referenced_tmp_var (T0);


  /*** create: T0 = (unsigned int)i; ***/

  vec_stmt = build (MODIFY_EXPR, unsigned_intSI_type_node, T0,
		    build1 (NOP_EXPR, unsigned_intSI_type_node, new_indx));
  new_temp = make_ssa_name (T0, vec_stmt);
  TREE_OPERAND (vec_stmt, 0) = new_temp;
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      print_generic_expr (tree_dump_file, vec_stmt, TDF_SLIM);
      fprintf (tree_dump_file, "\n");
    }
  bsi_insert_before (bsi, vec_stmt, BSI_SAME_STMT);


  /*** create: unsigned int T1; ***/

  new_name = get_name (scalar_indx);
  if (!new_name)
    new_name = "iv_";
  T1 = vect_get_new_vect_var (unsigned_intSI_type_node,
                                vect_simple_var, new_name);
  add_referenced_tmp_var (T1);


  /*** create: T1 = T0 * N; ***/

  mult_expr = build (MULT_EXPR, unsigned_intSI_type_node,
		     TREE_OPERAND (vec_stmt, 0),
		     build_int_2 (UNITS_PER_SIMD_WORD, 0));
  vec_stmt = build (MODIFY_EXPR, unsigned_intSI_type_node, T1, mult_expr);
  new_temp = make_ssa_name (T1, vec_stmt);
  TREE_OPERAND (vec_stmt, 0) = new_temp;
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      print_generic_expr (tree_dump_file, vec_stmt, TDF_SLIM);
      fprintf (tree_dump_file, "\n");
    }
  bsi_insert_before (bsi, vec_stmt, BSI_SAME_STMT);

  return (TREE_OPERAND (vec_stmt, 0));

#endif /* POINTER_ARITHMETIC */
}


/* Function get_vectype_for_scalar_type  */

static tree
get_vectype_for_scalar_type (tree scalar_type)
{
  enum machine_mode inner_mode;
  enum machine_mode vec_mode;
  int nbytes;
  int nunits;

  /* FORNOW: Only a single vector size per target is expected.  */

  inner_mode = TYPE_MODE (scalar_type);
  nbytes = GET_MODE_SIZE (inner_mode);
  nunits = UNITS_PER_SIMD_WORD / nbytes;

  if (GET_MODE_CLASS (inner_mode) == MODE_FLOAT)
    vec_mode = MIN_MODE_VECTOR_FLOAT;
  else
    vec_mode = MIN_MODE_VECTOR_INT;

  /* CHECKME: This duplicates some of te functionality in build_vector_type;
     could have directly called build_vector_type_for_mode if exposed.  */

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "\nget vectype for scalar type:  ");
      print_generic_expr (tree_dump_file, scalar_type, TDF_SLIM);
      fprintf (tree_dump_file, "\n");
    }

  for (; vec_mode != VOIDmode ; vec_mode = GET_MODE_WIDER_MODE (vec_mode))
    if (GET_MODE_NUNITS (vec_mode) == nunits
	&& GET_MODE_INNER (vec_mode) == inner_mode
	&& VECTOR_MODE_SUPPORTED_P (vec_mode))
      return build_vector_type (scalar_type, nunits);

  return NULL_TREE;
}


/* Function vect_align_data_ref

   Handle alignment of a memory accesses.

   FORNOW: Make sure the array is properly aligned. The vectorizer
           currently does not handle unaligned memory accesses.
           This restriction will be relaxed in the future.

   FORNOW: data_ref is an array_ref which alignment can be forced; i.e.,
           the base of the ARRAY_REF is not a pointer but an array.
           This restriction will be relaxed in the future.

   FORNOW: The array is being accessed starting at location 'init';
           We limit vectorization to cases in which init % NUNITS == 0
           (where NUNITS = GET_MODE_NUNITS (TYPE_MODE (vectype))).
           This restriction will be relaxed in the future.  */

static void
vect_align_data_ref (tree ref, tree stmt)
{
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);
  tree array_base = get_array_base (ref);

  if (TREE_CODE (TREE_TYPE (array_base)) != ARRAY_TYPE)
    abort ();

  if (TYPE_ALIGN (TREE_TYPE (array_base)) < TYPE_ALIGN (vectype))
    {
      /* CHECKME: is this the way to force the alignment of an array base?  */

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        fprintf (tree_dump_file,
		  "\nforce alignment. before: scalar/vec type_align = %d/%d\n",
		  TYPE_ALIGN (TREE_TYPE (array_base)),
		  TYPE_ALIGN (vectype));

      TYPE_ALIGN (TREE_TYPE (array_base)) = TYPE_ALIGN (vectype);
    }
}


/* Function vect_create_data_ref.

   Create a memory reference expression for vector access, to be used in a
   vector load/store stmt.

   Input:
   STMT: the stmt that references memory
         FORNOW: a load/store of the form 'var = a[i]'/'a[i] = var'.
   OP: the operand in STMT that is the memory reference
       FORNOW: an array_ref.
   BSI: the block_stmt_iterator where STMT is. Any new stmts created by this
        function can be added here.

   Output:
   1. Declare a new ptr to vector_type, and have it point to the array base.
      For example, for vector of type V8HI:
      v8hi *p0;
      p0 = (v8hi *)&a;

   3. If pointer arithmetic is defined, return '*(p0 + idx)'.
         where idx is the offset in bytes.
      Otherwise return the expression 'p0[idx]',
         where idx is the index used for the scalar expr.

   FORNOW: handle only simple array accesses (step 1).  */

static tree
vect_create_data_ref (tree ref, tree stmt, block_stmt_iterator *bsi)
{
  tree data_ref;
  tree idx;
  tree base = get_array_base (ref);
  tree vec_stmt, T0;
  tree new_temp;
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);
  tree ptr_type;
  tree array_ptr;
  tree array_base;
  vdef_optype vdefs = STMT_VDEF_OPS (stmt);
  vuse_optype vuses = STMT_VUSE_OPS (stmt);
  int nvuses = 0, nvdefs = 0;
  int i;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "create array_ref of type:\n");
      print_generic_expr (tree_dump_file, vectype, TDF_SLIM);
    }

  vect_align_data_ref (ref, stmt);
  array_base = get_array_base (ref);

  /*** create: vectype *p;  ***/
  ptr_type = build_pointer_type (vectype);
  array_ptr = vect_get_new_vect_var (ptr_type, vect_pointer_var,
		 get_name (array_base));
  add_referenced_tmp_var (array_ptr);
  get_var_ann (array_ptr)->mem_tag = array_base;


  /*** create: p = (vectype *)&a; ***/
  vec_stmt = build (MODIFY_EXPR, ptr_type, array_ptr,
		    build1 (NOP_EXPR, ptr_type,
			    build1 (ADDR_EXPR, ptr_type, base)));
  TREE_ADDRESSABLE (base) = 1;
  new_temp = make_ssa_name (array_ptr, vec_stmt);
  TREE_OPERAND (vec_stmt, 0) = new_temp;
  bsi_insert_before (bsi, vec_stmt, BSI_SAME_STMT);

  idx = vect_create_index_for_array_ref (stmt, bsi);

#ifdef POINTER_ARITHMETIC

  /*** create: vectype *T0; ***/

  T0 = vect_get_new_vect_var (ptr_type, vect_pointer_var, 
		get_name (array_base));
  add_referenced_tmp_var (T0);

  if (TREE_CODE (array_base) == VAR_DECL)
    {
      get_var_ann (T0)->mem_tag = array_base;
      bitmap_set_bit (vars_to_rename, var_ann (array_base)->uid);
    }
  else
    {
      /* FORNOW. This restriction will be relaxed in the future.  */
      abort ();
    }

  /* Also mark for renaming all aliased variables:  */
  /* CHECKME */
  if (vuses) 
    nvuses = NUM_VUSES (vuses);
  if (vdefs) 
    nvdefs = NUM_VDEFS (vdefs);
  for (i = 0; i < nvuses; i++)
    {
      tree use = VUSE_OP (vuses, i);;
      if (TREE_CODE (use) == SSA_NAME)
        bitmap_set_bit (vars_to_rename, var_ann (SSA_NAME_VAR (use))->uid);
    } 
  for (i = 0; i < nvdefs; i++)
    {
      tree def = VDEF_RESULT (vdefs, i);
      if (TREE_CODE (def) == SSA_NAME)
        bitmap_set_bit (vars_to_rename, var_ann (SSA_NAME_VAR (def))->uid);
    }


  /*** create: T0 = idx + p; ***/

  vec_stmt = build (MODIFY_EXPR, ptr_type, T0,
		    build (PLUS_EXPR, ptr_type, TREE_OPERAND (vec_stmt, 0),
			   idx));
  new_temp = make_ssa_name (T0, vec_stmt);
  TREE_OPERAND (vec_stmt, 0) = new_temp;
  bsi_insert_before (bsi, vec_stmt, BSI_SAME_STMT);


  /*** create dataref: '*T0' ***/

  data_ref = build1 (INDIRECT_REF, vectype, TREE_OPERAND (vec_stmt, 0));


#else /* POINTER_ARITHMETIC */


  /*** create data ref: 'p[idx]' ***/

  data_ref = build (ARRAY_REF, ptr_type, TREE_OPERAND (vec_stmt, 0), idx);


#endif /* POINTER_ARITHMETIC */

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    print_generic_expr (tree_dump_file, data_ref, TDF_SLIM);

  return data_ref;
}


/* Function vect_create_destination_var

   Create a new temporary of type VECTYPE.  */

static tree
vect_create_destination_var (tree scalar_dest, tree vectype)
{
  tree vec_dest;
  const char *new_name;

  if (TREE_CODE (scalar_dest) != SSA_NAME)
    abort ();

  new_name = get_name (scalar_dest);
  if (!new_name)
    new_name = "var_";
  vec_dest = vect_get_new_vect_var (vectype, vect_simple_var, new_name);
  add_referenced_tmp_var (vec_dest);

  /* FIXME: introduce new type.   */
  TYPE_ALIAS_SET (TREE_TYPE (vec_dest)) =
    TYPE_ALIAS_SET (TREE_TYPE (scalar_dest));

  return vec_dest;
}


/* Function vect_get_vec_def_for_operand.  */

static tree
vect_get_vec_def_for_operand (tree op, tree stmt)
{
  tree vec_oprnd;

  if (!op) 
    abort ();

  if (TREE_CODE (op) == SSA_NAME)
    {
      tree vec_stmt;
      stmt_vec_info stmt_info = NULL;

       /* FORNOW - we assume that the defining stmt is not a PHI node. This
          restriction will be relaxed in the future.  */

      stmt_info = vinfo_for_stmt (SSA_NAME_DEF_STMT (op));
      if (!stmt_info)
        abort ();

      vec_stmt = STMT_VINFO_VEC_STMT (stmt_info);

      if (!vec_stmt)
        abort ();

      /* CHECKME: any cases where the def we want is not TREE_OPERAND 0?  */
      vec_oprnd = TREE_OPERAND (vec_stmt, 0);

      return vec_oprnd;
    }

  if (TREE_CODE (op) == INTEGER_CST ||
      TREE_CODE (op) == REAL_CST)
    {
      /* Create 'vect_cst_ = {cst,cst,...,cst}'  */

      stmt_vec_info stmt_vinfo = vinfo_for_stmt (stmt);
      struct loop *loop = STMT_VINFO_LOOP (stmt_vinfo);
      block_stmt_iterator pre_header_bsi;
      tree new_var;
      tree init_stmt;
      tree vec_cst;
      tree vectype = STMT_VINFO_VECTYPE (stmt_vinfo);
      int nunits = GET_MODE_NUNITS (TYPE_MODE (vectype));
      tree t = NULL_TREE; 
      int i;

      new_var = vect_get_new_vect_var (vectype, vect_simple_var, "cst_");
      add_referenced_tmp_var (new_var);
      bitmap_set_bit (vars_to_rename, var_ann (new_var)->uid);

      /* Build a tree with vector elements.  */
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        fprintf (tree_dump_file, "\nCreate vector_cst.\n");
      for (i = nunits - 1; i >= 0; --i)
        { 
	  t = tree_cons (NULL_TREE, op, t);
        }
      vec_cst = build_vector (vectype, t);
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        {
          print_generic_expr (tree_dump_file, vec_cst, TDF_SLIM);
          fprintf (tree_dump_file, "\n");
 	}

      init_stmt = build (MODIFY_EXPR, vectype, new_var, vec_cst);
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        {
          print_generic_expr (tree_dump_file, init_stmt, TDF_SLIM);
          fprintf (tree_dump_file, "\n");
        }

      pre_header_bsi = bsi_last (loop->pre_header);
      if (!bsi_end_p (pre_header_bsi)
          && is_ctrl_stmt (bsi_stmt (pre_header_bsi)))
        bsi_insert_before (&pre_header_bsi, init_stmt, BSI_NEW_STMT); 
      else 
        bsi_insert_after (&pre_header_bsi, init_stmt, BSI_NEW_STMT); 

      vec_oprnd = TREE_OPERAND (init_stmt, 0);
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        {
          print_generic_expr (tree_dump_file, vec_oprnd, TDF_SLIM);
          fprintf (tree_dump_file, "\n");
	}

      return vec_oprnd;
    }

  return NULL_TREE;
}


/* Function vect_transfom_binop.  */

static tree
vect_transform_binop (tree stmt, block_stmt_iterator *bsi)
{
  tree vec_stmt;
  tree vec_dest;
  tree scalar_dest;
  tree operation;
  tree op0, op1;
  tree vec_oprnd0, vec_oprnd1;
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);
  enum tree_code code;
  tree new_temp;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "transform binop\n");

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    abort ();

  operation = TREE_OPERAND (stmt, 1);

  /** Handle def. **/

  scalar_dest = TREE_OPERAND (stmt, 0);
  if (TREE_CODE (scalar_dest) != SSA_NAME)
    abort ();
  vec_dest = vect_create_destination_var (scalar_dest, vectype);

  /** Handle uses - get the vectorized defs from the defining stmts.  **/

  op0 = TREE_OPERAND (operation, 0);
  op1 = TREE_OPERAND (operation, 1);

  vec_oprnd0 = vect_get_vec_def_for_operand (op0, stmt);
  if (! vec_oprnd0)
    abort ();

  vec_oprnd1 = vect_get_vec_def_for_operand (op1, stmt);
  if (! vec_oprnd1)
    abort ();

  /** arguments are ready. create the new vector stmt.  **/

  code = TREE_CODE (operation);
  vec_stmt = build (MODIFY_EXPR, vectype, vec_dest,
		    build (code, vectype, vec_oprnd0, vec_oprnd1));
  new_temp = make_ssa_name (vec_dest, vec_stmt);
  TREE_OPERAND (vec_stmt, 0) = new_temp;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "add new stmt\n");
      print_generic_stmt (tree_dump_file, vec_stmt, TDF_SLIM);
    }
  bsi_insert_before (bsi, vec_stmt, BSI_SAME_STMT);

  return vec_stmt;
}


/* Function vect_transfom_store.  */

static tree
vect_transform_store (tree stmt, block_stmt_iterator *bsi)
{
  tree scalar_dest;
  tree vec_stmt;
  tree data_ref;
  tree op;
  tree vec_oprnd1;
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "transform store\n");

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    abort ();

  /** Handle def.  **/

  scalar_dest = TREE_OPERAND (stmt, 0);

  if (TREE_CODE (scalar_dest) != ARRAY_REF)
    abort ();

  data_ref = vect_create_data_ref (scalar_dest, stmt, bsi);
  if (!data_ref)
    abort ();

  /** Handle use - get the vectorized def from the defining stmt.  **/

  op = TREE_OPERAND (stmt, 1);

  vec_oprnd1 = vect_get_vec_def_for_operand (op, stmt);
  if (! vec_oprnd1)
    abort ();

  /** Arguments are ready. create the new vector stmt.  **/

  vec_stmt = build (MODIFY_EXPR, vectype, data_ref, vec_oprnd1);
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "add new stmt\n");
      print_generic_stmt (tree_dump_file, vec_stmt, TDF_SLIM);
    }
  bsi_insert_before (bsi, vec_stmt, BSI_SAME_STMT);

  if (stmt != bsi_stmt (*bsi))
    {
      /* This is expected when an update chain for a data-ref index has been
         created (by vect_create_index_for_array_ref). The current stmt 
	 sequence is as follows:

	 (i)   some stmt
	 (i+1) vec_stmt (with a data_ref that uses index)
	 (i+2) stmt_to_update_index    <-- bsi
	 (i+3) stmt

	 The iterator bsi should be bumped to point to stmt at location (i+3)
	 because this is what the driver vect_transform_loop expects.  */

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        {
          fprintf (tree_dump_file, "update chain:\n");
          print_generic_stmt (tree_dump_file, bsi_stmt (*bsi), TDF_SLIM);
        }
      bsi_next (bsi);
    }

  /* The driver function vect_transform_loop expects bsi to point the last
     scalar stmt that was vectorized.  */
  if (stmt != bsi_stmt (*bsi))
    abort ();

  return vec_stmt;
}


/* Function vect_transform_load.  */

static tree
vect_transform_load (tree stmt, block_stmt_iterator *bsi)
{
  tree vec_stmt;
  tree scalar_dest;
  tree vec_dest = NULL;
  tree data_ref = NULL;
  tree op;
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);
  tree new_temp;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "transform load\n");

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    abort ();

  /** Handle def.  **/

  scalar_dest = TREE_OPERAND (stmt, 0);
  if (TREE_CODE (scalar_dest) != SSA_NAME)
    abort ();
  vec_dest = vect_create_destination_var (scalar_dest, vectype);
  if (!vec_dest)
    abort ();

  /** Handle use.  **/

  op = TREE_OPERAND (stmt, 1);

  if (TREE_CODE (op) != ARRAY_REF)
    abort ();

  data_ref = vect_create_data_ref (op, stmt, bsi);

  if (!data_ref)
    abort ();

  /** Arguments are ready. create the new vector stmt.  **/

  vec_stmt = build (MODIFY_EXPR, vectype, vec_dest, data_ref);
  new_temp = make_ssa_name (vec_dest, vec_stmt);
  TREE_OPERAND (vec_stmt, 0) = new_temp;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "add new stmt\n");
      print_generic_stmt (tree_dump_file, vec_stmt, TDF_SLIM);
    }
  bsi_insert_before (bsi, vec_stmt, BSI_SAME_STMT);

  if (stmt != bsi_stmt (*bsi))
    {
      /* This is expected when an update chain for a data-ref index has been
         created (by vect_create_index_for_array_ref). The current stmt
         sequence is as follows:
  
         (i)   some stmt
         (i+1) vec_stmt (with a data_ref that uses index)
         (i+2) stmt_to_update_index    <-- bsi
         (i+3) stmt 
  
         The iterator bsi should be bumped to point to stmt at location (i+3)
         because this is what the driver vect_transform_loop expects.  */

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        {
          fprintf (tree_dump_file, "update chain:\n");
          print_generic_stmt (tree_dump_file, bsi_stmt (*bsi), TDF_SLIM);
        }
      bsi_next (bsi);
    }

  /* The driver function vect_transform_loop expects bsi to point the last
     scalar stmt that was vectorized.  */
  if (stmt != bsi_stmt (*bsi))
    abort ();

  return vec_stmt;
}


/* Function vect_transform_stmt.  */

static bool
vect_transform_stmt (tree stmt, block_stmt_iterator *bsi)
{
  bool is_store = false;
  tree vec_stmt = NULL;
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);

  switch (STMT_VINFO_TYPE (stmt_info))
    {
    case binop_vec_info_type:
      vec_stmt = vect_transform_binop (stmt, bsi);
      break;

    case load_vec_info_type:
      vec_stmt = vect_transform_load (stmt, bsi);
      break;

    case store_vec_info_type:
      vec_stmt = vect_transform_store (stmt, bsi);
      is_store = true;
      break;

    default:
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        fprintf (tree_dump_file, "stmt not supported\n");
      abort ();
    }

  STMT_VINFO_VEC_STMT (stmt_info) = vec_stmt;

  return is_store;
}


/* Function vect_transform_loop_bound  */

static void
vect_transform_loop_bound (loop_vec_info loop_vinfo)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  edge exit_edge = loop_exit_edge (loop, 0);
  block_stmt_iterator loop_exit_bsi = bsi_last (exit_edge->src);
  block_stmt_iterator pre_header_bsi;
  tree orig_cond_expr;
  int old_N, vf;
  tree new_indx, init_stmt, step_stmt, cond_stmt;
  tree new_loop_bound;

  /* FORNOW: assuming the loop bound is known.  */
  if (!LOOP_VINFO_NITERS_KNOWN_P (loop_vinfo))
    abort ();

  old_N = LOOP_VINFO_NITERS (loop_vinfo);
  vf = LOOP_VINFO_VECT_FACTOR (loop_vinfo);

  /* FORNOW:
     assuming number-of-iterations divides by the vectorization factor.  */
  if (old_N % vf)
    abort ();

  orig_cond_expr = LOOP_VINFO_EXIT_COND (loop_vinfo);
  if (!orig_cond_expr)
    abort ();
  if (orig_cond_expr != bsi_stmt (loop_exit_bsi))
    abort ();

  /* create new induction var:  */
  new_indx = vect_get_new_vect_var (unsigned_intSI_type_node,
                                        vect_simple_var, "iv_");
  add_referenced_tmp_var (new_indx);
  bitmap_set_bit (vars_to_rename, var_ann (new_indx)->uid);
  

  /* induction var initialization in loop prolog:  */
  init_stmt = build (MODIFY_EXPR, unsigned_intSI_type_node, new_indx, 
			integer_zero_node);

  pre_header_bsi = bsi_last (loop->pre_header);
  if (!bsi_end_p (pre_header_bsi)
      && is_ctrl_stmt (bsi_stmt (pre_header_bsi))) 
    bsi_insert_before (&pre_header_bsi, init_stmt, BSI_NEW_STMT);
  else
    bsi_insert_after (&pre_header_bsi, init_stmt, BSI_NEW_STMT);

  /* induction var update in loop:  */
  step_stmt = build (MODIFY_EXPR, unsigned_intSI_type_node, new_indx,
                        build (PLUS_EXPR, unsigned_intSI_type_node,
			TREE_OPERAND (init_stmt, 0), integer_one_node));
  bsi_insert_before (&loop_exit_bsi, step_stmt, BSI_SAME_STMT);   


  /* new loop exit test:  */
  new_loop_bound = build_int_2 (old_N/vf, 0);
  cond_stmt = build (COND_EXPR, TREE_TYPE (orig_cond_expr),
		build (LT_EXPR, boolean_type_node, new_indx, new_loop_bound),
		TREE_OPERAND (orig_cond_expr, 1),
		TREE_OPERAND (orig_cond_expr, 2));
  bsi_insert_before (&loop_exit_bsi, cond_stmt, BSI_SAME_STMT);   

  /* remove old loop exit test:  */
  bsi_remove (&loop_exit_bsi);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    print_generic_expr (tree_dump_file, cond_stmt, TDF_SLIM);
}


/* Function vect_transform_loop.  */

static void
vect_transform_loop (loop_vec_info loop_vinfo)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  basic_block *bbs = LOOP_VINFO_BBS (loop_vinfo);
  int nbbs = loop->num_nodes;
  int vectorization_factor = LOOP_VINFO_VECT_FACTOR (loop_vinfo);
  block_stmt_iterator si;
  int i;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "\n<<vec_transform_loop>>\n");

  /* CHECKME: FORNOW the vectorizer supports only loops which body consist
     of one basic block + header. When the vectorizer will support more
     involved loop forms, the order by which the BBs are traversed need
     to be considered.  */

  for (i = 0; i < nbbs; i++)
    {
      basic_block bb = bbs[i];

      for (si = bsi_start (bb); !bsi_end_p (si);)
	{
	  tree stmt = bsi_stmt (si);
	  stmt_vec_info stmt_info;
	  tree vectype;
	  bool is_store;

	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    {
	      fprintf (tree_dump_file, "\n-----\nvectorizing statement:\n");
	      print_generic_stmt (tree_dump_file, stmt, TDF_SLIM);
	    }	

	  stmt_info = vinfo_for_stmt (stmt);
	  if (!stmt_info)
	    abort ();

	  if (!STMT_VINFO_RELEVANT_P (stmt_info))
	    {
	      bsi_next (&si);
	      continue;
	    }

	  /* FORNOW: Verify that all stmts operate on the same number of
	             units and no inner unrolling is necessary.  */
	  vectype = STMT_VINFO_VECTYPE (stmt_info);
	  if (GET_MODE_NUNITS (TYPE_MODE (vectype)) != vectorization_factor)
	    abort ();

	  /* -------- vectorize statement ------------ */
	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    fprintf (tree_dump_file, "transform statement.\n");

	  is_store = vect_transform_stmt (stmt, &si);

	  if (is_store)
	    {
	      /* free the attched stmt_vec_info and remove the stmt.  */
	      stmt_ann_t ann = stmt_ann (stmt);
	      free (stmt_info);
	      set_stmt_info (ann, NULL);

	      bsi_remove (&si);
	      continue;
	    }

	  bsi_next (&si);

	}			/* stmts in BB */
    }				/* BBs in loop */


  vect_transform_loop_bound (loop_vinfo);
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "\n<<Success! loop vectorized.>>\n");
}


/* Function vect_is_simple_use.  */

static bool
vect_is_simple_use (tree operand, struct loop *loop)
{ 
  tree def_stmt;
  basic_block bb;

  if (!operand)
    return false;

  if (TREE_CODE (operand) == SSA_NAME)
    {
      def_stmt = SSA_NAME_DEF_STMT (operand);

      if (def_stmt == NULL_TREE || TREE_CODE (def_stmt) == NOP_EXPR)
        return false;

      bb = bb_for_stmt (def_stmt);

      /* FORNOW: Support only operations which arguments are defined inside 
         the loop. This is also verified by analyze_scalar_cycles.  */
      if (TREE_CODE (def_stmt) == PHI_NODE
          || !flow_bb_inside_loop_p (loop, bb))
        return false;

      return true;  
    }

  if (TREE_CODE (operand) == INTEGER_CST ||
      TREE_CODE (operand) == REAL_CST)
    {
      return true;
    }

  return false;
}


/* Function vect_is_supportable_binop.

   Verify that STMT performs a binary operation and can be vectorized.  */

static bool
vect_is_supportable_binop (tree stmt)
{
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree operation;
  enum tree_code code;
  tree op0, op1;
  enum machine_mode vec_mode;
  optab binoptab;
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);
  struct loop *loop = STMT_VINFO_LOOP (stmt_info);

  /* Is binop? */

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return false;

  if (TREE_CODE (TREE_OPERAND (stmt, 0)) != SSA_NAME)
    return false;

  operation = TREE_OPERAND (stmt, 1);
  code = TREE_CODE (operation);

  switch (code)
    {
    case PLUS_EXPR:
      binoptab = add_optab;
      break;
    case MULT_EXPR:
      binoptab = smul_optab;
      break;
    case MINUS_EXPR:
      binoptab = sub_optab;
      break;
    default:
      return false;
    }

  op0 = TREE_OPERAND (operation, 0);
  op1 = TREE_OPERAND (operation, 1);

  if (!vect_is_simple_use (op0, loop) || !vect_is_simple_use (op1, loop))
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        fprintf (tree_dump_file, 
		"use not simple (not SSA_NAME or defined out of loop).\n");
      return false;
    }	

  /* Supportable by target?  */

  if (!binoptab)
    return false;

  vec_mode = TYPE_MODE (vectype);

  if (binoptab->handlers[(int) vec_mode].insn_code == CODE_FOR_nothing)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "op not supported by target\n");
      return false;
    }

  /* FORNOW: Not considering the cost.  */

  STMT_VINFO_TYPE (stmt_info) = binop_vec_info_type;

  return true;
}


/* Function vect_is_supportable_store.

   Verify that STMT performs a store to memory operation,
   and can be vectorized.  */

static bool
vect_is_supportable_store (tree stmt)
{
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree scalar_dest;
  tree op;
  struct loop *loop = STMT_VINFO_LOOP (stmt_info);

  /* Is vectorizable store? */

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return false;

  scalar_dest = TREE_OPERAND (stmt, 0);

  if (TREE_CODE (scalar_dest) != ARRAY_REF)
    return false;

  op = TREE_OPERAND (stmt, 1);

  if (!vect_is_simple_use (op, loop))
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        fprintf (tree_dump_file,
		"use not simple (not SSA_NAME or defined out of loop).\n");
      return false;
    }

  if (!STMT_VINFO_DATA_REF (stmt_info))
    return false;

  /* Previous analysis steps have already verified that the data ref is
     vectorizable (w.r.t data dependences, access pattern, etc).  */

  /* FORNOW: Not considering the cost.  */

  STMT_VINFO_TYPE (stmt_info) = store_vec_info_type;

  return true;
}


/* Function vect_is_supportable_load.

   Verify that STMT performs a load from memory operation,
   and can be vectorized.  */

static bool
vect_is_supportable_load (tree stmt)
{
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree scalar_dest;
  tree op;

  /* Is vectorizable load? */

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return false;

  scalar_dest = TREE_OPERAND (stmt, 0);
  if (TREE_CODE (scalar_dest) != SSA_NAME)
    return false;

  op = TREE_OPERAND (stmt, 1);

  if (TREE_CODE (op) != ARRAY_REF)
    return false;

  if (!STMT_VINFO_DATA_REF (stmt_info))
    return false;

  /* Previous analysis steps have already verified that the data ref is
     vectorizable (w.r.t data dependences, access pattern, etc).  */

  /* FORNOW: Not considering the cost.  */

  STMT_VINFO_TYPE (stmt_info) = load_vec_info_type;

  return true;
}


/* Function vect_analyze_operations.

   Scan the loop stmts and make sure they are all vectorizable.  */

static bool
vect_analyze_operations (loop_vec_info loop_vinfo)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  basic_block *bbs = LOOP_VINFO_BBS (loop_vinfo);
  int nbbs = loop->num_nodes;
  block_stmt_iterator si;
  int vectorization_factor = 0;
  int i;
  bool ok;
  tree scalar_type;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "\n<<vect_analyze_operations>>\n");

  for (i = 0; i < nbbs; i++)
    {
      basic_block bb = bbs[i];

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  tree stmt = bsi_stmt (si);
	  int nunits;
	  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
	  tree vectype;
	  dataflow_t df;
	  int j, num_uses;

	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    {
	      fprintf (tree_dump_file, "\n-------\nexamining statement:\n");
	      print_generic_stmt (tree_dump_file, stmt, TDF_SLIM);
	    }

	  if (!stmt_info)
	    abort ();

	  /* skip stmts which do not need to be vectorized.
	     this is expected to include:
	     - the COND_EXPR which is the loop exit condition
	     - any LABEL_EXPRs in the loop
	     - computations that are used only for array indexing or loop
	     control  */

	  if (!STMT_VINFO_RELEVANT_P (stmt_info))
	    {
	      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	        fprintf (tree_dump_file, "irrelevant.\n");
	      continue;
	    }

	  if (TREE_CODE (stmt) != MODIFY_EXPR)
	    {
	      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		{
		  fprintf (tree_dump_file, "not a MODIFY_EXPR\n");
		  print_generic_stmt (tree_dump_file, stmt, TDF_SLIM);
		}
	      return false;
	    }

	  if (VECTOR_MODE_P (TYPE_MODE (TREE_TYPE (stmt))))
	    {
	      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		{
		  fprintf (tree_dump_file, "vector stmt in loop!\n");
		  print_generic_stmt (tree_dump_file, stmt, TDF_SLIM);
		}
	      return false;
	    }

          if (STMT_VINFO_DATA_REF (stmt_info))
            scalar_type = TREE_TYPE (DR_REF (STMT_VINFO_DATA_REF (stmt_info)));    
          else
	    scalar_type = TREE_TYPE (stmt);
	  vectype = get_vectype_for_scalar_type (scalar_type);
	  if (!vectype)
	    {
	      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		{
		  fprintf (tree_dump_file, "no vectype for stmt.\n");
		  print_generic_stmt (tree_dump_file, stmt, TDF_SLIM);
		}
	      return false;
	    }
	  STMT_VINFO_VECTYPE (stmt_info) = vectype;

	  ok = (vect_is_supportable_binop (stmt)
		|| vect_is_supportable_load (stmt)
		|| vect_is_supportable_store (stmt));

	  if (!ok)
	    {
	      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		{
		  fprintf (tree_dump_file, "stmt not supported.\n");
		  print_generic_stmt (tree_dump_file, stmt, TDF_SLIM);
		}
	      return false;
	    }

	  /* FORNOW: Make sure that the def of this stmt is not used out
	     side the loop. This restriction will be relaxed in the future.  */

	  df = get_immediate_uses (stmt);
	  num_uses = num_immediate_uses (df);
	  for (j = 0; j < num_uses; j++)
	    {
	      tree use = immediate_use (df, j);
	      basic_block bb = bb_for_stmt (use);
     	      if (!flow_bb_inside_loop_p (loop, bb))
        	{
		  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		    {
		      fprintf (tree_dump_file, "def used out of loop:\n");
		      print_generic_stmt (tree_dump_file, use, TDF_SLIM);
		    }
		  return false;
		}
	    }

	  nunits = GET_MODE_NUNITS (TYPE_MODE (vectype));
	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    fprintf (tree_dump_file, "nunits = %d\n", nunits);

	  if (vectorization_factor)
	    {
	      /* FORNOW: don't allow mixed units.
	         This restriction will be relaxed in the future.  */
	      if (nunits != vectorization_factor)
		{
		  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		    {
		      fprintf (tree_dump_file, "mixed types unsupported.\n");
		      print_generic_stmt (tree_dump_file, stmt, TDF_SLIM);
		    }
		  return false;
		}
	    }
	  else
	    vectorization_factor = nunits;
	}
    }

  /* TODO: Analayze cost. Decide if worth while to vectorize.  */

  LOOP_VINFO_VECT_FACTOR (loop_vinfo) = vectorization_factor;

  /* FORNOW: handle only cases where the loop bound divides by the
     vectorization factor.  */

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "vectorization_factor = %d, niters = %d\n",
	vectorization_factor, LOOP_VINFO_NITERS (loop_vinfo));

  if (vectorization_factor == 0
      || !LOOP_VINFO_NITERS_KNOWN_P (loop_vinfo)
      || LOOP_VINFO_NITERS (loop_vinfo) % vectorization_factor != 0)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, 
		"loop bound unknown or doesn't divide by %d\n",
		 vectorization_factor);
      return false;
    }

  return true;
}


/* Function exist_non_indexing_operands_for_use_p 

   USE is one of the uses attched to STMT. Check if USE is 
   used in STMT for anything other than indexing an array.  */

static bool
exist_non_indexing_operands_for_use_p (tree use, tree stmt)
{
  tree operand;
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
 
  /* USE corresponds to som operand in STMT. Since there is no
     data reference in STMT, any operand that corresponds to USE
     is not indexing an array.  */
  if (!STMT_VINFO_DATA_REF (stmt_info))
    return true;
 
  /* STMT had a data_ref. FORNOW this means that its of one of
     the following forms:
     -1- ARRAY_REF = var
     -2- var = ARRAY_REF
     (This should have been verified in analyze_data_refs).

     'var' in the second case corresponds to a def, not a use,
     so USE cannot corespond to any operands that are not used 
     for array indexing.

     Therefore, all we need to check is if STMT falls into the
     first case, and whether var corresponds to USE.  */
 
  if (TREE_CODE (TREE_OPERAND (stmt, 0)) == SSA_NAME)
    return false;

  operand = TREE_OPERAND (stmt, 1);

  if (TREE_CODE (operand) != SSA_NAME)
    return false;

  if (operand == use)
    return true;

  return false;
}


/* Function vect_is_simple_iv_evolution.

   FORNOW: A simple evolution of an induction variables in the loop is
   considered a polynomial evolution with step 1.  */

static bool
vect_is_simple_iv_evolution (unsigned loop_nb, tree access_fn, tree * init, 
				tree * step, bool strict)
{
  tree init_expr;
  tree step_expr;
  
  tree evolution_part = evolution_part_in_loop_num (access_fn, loop_nb);

  /* When there is no evolution in this loop, the evolution function
     is not "simple".  */  
  if (evolution_part == NULL_TREE)
    return false;
  
  /* When the evolution is a polynomial of degree >= 2 or
     exponential, the evolution function is not "simple".  */
  if (TREE_CODE (evolution_part) == POLYNOMIAL_CHREC
      || TREE_CODE (evolution_part) == EXPONENTIAL_CHREC)
    return false;
  
  step_expr = evolution_part;
  init_expr = initial_condition (access_fn);
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "\nstep: ");
      print_generic_expr (tree_dump_file, step_expr, TDF_SLIM);
      fprintf (tree_dump_file, "\ninit: ");
      print_generic_expr (tree_dump_file, init_expr, TDF_SLIM);
      fprintf (tree_dump_file, "\n");
    }

  *init = init_expr;
  *step = step_expr;

  if (TREE_CODE (step_expr) != INTEGER_CST)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        fprintf (tree_dump_file, "\nstep unknown.\n");
      return false;
    }

  if (strict)
    if (!integer_onep (step_expr))
      {
        if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	  print_generic_expr (tree_dump_file, step_expr, TDF_SLIM);
        return false;
      }

  return true;
}


/* Function vect_analyze_scalar_cycles.

   Examine the cross iteration def-use cycles of scalar variables, by
   analyzing the loop (scalar) PHIs; verify that the cross iteration def-use
   cycles that they represent do not impede vectorization.

   FORNOW: Reduction as in the following loop, is not supported yet:
              loop1:
              for (i=0; i<N; i++)
                 sum += a[i];
	   The cross-iteration cycle corresponding to variable 'sum' will be
	   considered too complicated and will impede vectorization.

   FORNOW: Induction as in the following loop, is not supported yet:
              loop2:
              for (i=0; i<N; i++)
                 a[i] = i;

           However, the following loop *is* vectorizable:
              loop3:
              for (i=0; i<N; i++)
                 a[i] = b[i];

           In both loops there exists a def-use cycle for the variable i:
              loop: i_2 = PHI (i_0, i_1)
                    a[i_2] = ...;
                    i_1 = i_2 + 1;
                    GOTO loop;

           The evolution of the above cycle is considered simple enough,
	   however, we also check that the cycle does not need to be
	   vectorized, i.e - we check that the variable that this cycle
	   defines is only used for array indexing or in stmts that do not
	   need to be vectorized. This is not the case in loop2, but it
	   *is* the case in loop3.  */

static bool
vect_analyze_scalar_cycles (loop_vec_info loop_vinfo)
{
  tree phi;
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  basic_block bb = loop->header;
  dataflow_t df;
  int num_uses;
  tree dummy;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "\n<<vect_analyze_scalar_evolutions>>\n");

  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
    {
      int i;
      tree access_fn = NULL;

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
          fprintf (tree_dump_file, "Analyze phi\n");
          print_generic_expr (tree_dump_file, phi, TDF_SLIM);
	}

      /* Skip virtual phi's. The data dependences that are associated with
         virtual defs/uses (i.e., memory accesses) are analyzed elsewhere.  */

      /* CHECKME: correct way to check for a virtual phi?  */

      if (!is_gimple_reg (SSA_NAME_VAR (PHI_RESULT (phi))))
	{
	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    fprintf (tree_dump_file, "virtual phi. skip.\n");
	  continue;
	}

      /* Analyze the evolution function.  */

      /* FORNOW: The only scalar cross-iteration cycles that we allow are
         those of the loop induction variable;
         Furthermore, if that induction variable is used in an operation
         that needs to be vectorized (i.e, is not solely used to index
         arrays and check the exit condition) - we do not support its
         vectorization Yet.  */

      /* 1. Verify that it is an IV with a simple enough access pattern.  */

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS)) 
        fprintf (tree_dump_file, "analyze cycles: call monev analyzer!\n");

      access_fn = instantiate_parameters
	(loop_num (loop),
	analyze_scalar_evolution (loop_num (loop), PHI_RESULT (phi)),
	PHI_RESULT (phi));

      if (!access_fn)
	{
	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    fprintf (tree_dump_file, "No Access function.");
	  return false;
	}

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        {
           fprintf (tree_dump_file, "Access function of PHI: ");
           print_generic_expr (tree_dump_file, access_fn, TDF_SLIM);
        }

      if (!vect_is_simple_iv_evolution (loop_num (loop), access_fn, &dummy, 
					&dummy, false))
	{
	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    fprintf (tree_dump_file, "unsupported cross iter cycle.\n");
	  return false;
	}

      /* 2. Verify that this variable is only used in stmts that do not need
         to be vectorized.  */

      df = get_immediate_uses (phi);
      num_uses = num_immediate_uses (df);
      for (i = 0; i < num_uses; i++)
	{
	  tree use = immediate_use (df, i);
	  stmt_vec_info stmt_info = vinfo_for_stmt (use);

	  if (!stmt_info)
	    {
	      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		{
		  fprintf (tree_dump_file, "\nused out side the loop??\n");
		  print_generic_expr (tree_dump_file, use, TDF_SLIM);
		}
	      return false;
	    }

	  if (STMT_VINFO_RELEVANT_P (stmt_info)
	      && exist_non_indexing_operands_for_use_p (PHI_RESULT (phi), use))
	    {
	      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		{
		  fprintf (tree_dump_file,
			   "\ninduction vectorization. Unsupported.\n");
		  print_generic_expr (tree_dump_file, use, TDF_SLIM);
		}
	      return false;
	    }
	}
    }

  return true;
}


/* Function get_array_base.  */

static tree
get_array_base (tree expr)
{
  tree expr1;
  if (TREE_CODE (expr) != ARRAY_REF)
    abort ();

  expr1 = TREE_OPERAND (expr, 0);
  while (TREE_CODE (expr1) == ARRAY_REF)
    expr1 = TREE_OPERAND (expr1, 0);

  return expr1;
}


/* Function vect_analyze_data_ref_dependence  */

static bool
vect_analyze_data_ref_dependence (struct data_reference *dra,
				  struct data_reference *drb)
{
  /* FORNOW: use most trivial and conservative test.  */

  /* CHECKME: this test holds only if the array base is not a pointer.
     This had been verified by analyze_data_refs.
     This restriction will be relaxed in the future.  */

  if (!array_base_name_differ_p (dra, drb))
    {
      enum data_dependence_direction ddd =
	ddg_direction_between_stmts (DR_STMT (dra), DR_STMT (drb), 
				      loop_num (loop_of_stmt (DR_STMT (dra))));

      if (ddd == dir_independent)
	return true;

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, 
		"vect_analyze_data_ref_dependence: same base\n");
      return false;
    }

  return true;
}


/* Function vect_analyze_data_ref_dependences.

   Examine all the data references in the loop, and make sure there do not
   exist any data dependences between them.

   FORNOW: We do not construct a data dependence graph and try to deal with
           dependences, but fail at the first data dependence that we
	   encounter.

   FORNOW: We only handle array references.

   FORNOW: We apply a trivial conservative dependence test.  */

static bool
vect_analyze_data_ref_dependences (loop_vec_info loop_vinfo)
{
  unsigned int i, j;
  varray_type loop_write_refs = LOOP_VINFO_DATAREF_WRITES (loop_vinfo);
  varray_type loop_read_refs = LOOP_VINFO_DATAREF_READS (loop_vinfo);

  /* examine store-store (output) dependences */

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "compare all store-store pairs\n");

  for (i = 0; i < VARRAY_ACTIVE_SIZE (loop_write_refs); i++)
    {
      for (j = i + 1; j < VARRAY_ACTIVE_SIZE (loop_write_refs); j++)
	{
	  struct data_reference *dra =
	    VARRAY_GENERIC_PTR (loop_write_refs, i);
	  struct data_reference *drb =
	    VARRAY_GENERIC_PTR (loop_write_refs, j);
	  bool ok = vect_analyze_data_ref_dependence (dra, drb);
	  if (!ok)
	    return false;
	}
    }

  /* examine load-store (true/anti) dependences */

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "compare all load-store pairs\n");

  for (i = 0; i < VARRAY_ACTIVE_SIZE (loop_read_refs); i++)
    {
      for (j = 0; j < VARRAY_ACTIVE_SIZE (loop_write_refs); j++)
	{
	  struct data_reference *dra = VARRAY_GENERIC_PTR (loop_read_refs, i);
	  struct data_reference *drb =
	    VARRAY_GENERIC_PTR (loop_write_refs, j);
	  bool ok = vect_analyze_data_ref_dependence (dra, drb);
	  if (!ok)
	    return false;
	}
    }

  return true;
}


/* Function vect_get_array_first_index.  */

static bool
vect_get_array_first_index (tree ref, int *array_first_index)
{
  tree array_start;
  tree array_base_type;
  int array_start_val;

  array_base_type = TREE_TYPE (TREE_OPERAND (ref, 0));
  if (! TYPE_DOMAIN (array_base_type))
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        {
          fprintf (tree_dump_file, "no domain for array base type\n");
          print_generic_expr (tree_dump_file, array_base_type, TDF_DETAILS);
        }
      return false;
    }

  array_start = TYPE_MIN_VALUE (TYPE_DOMAIN (array_base_type));
  if (TREE_CODE (array_start) != INTEGER_CST)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        {
          fprintf (tree_dump_file, "array min val not integer cst\n");
          print_generic_expr (tree_dump_file, array_start, TDF_DETAILS);
        }
      return false;
    }

  if (TREE_INT_CST_HIGH (array_start) != 0)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        fprintf (tree_dump_file, "array min val CST_HIGH != 0\n");
      return false;
    }

  array_start_val = TREE_INT_CST_LOW (array_start);
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      print_generic_expr (tree_dump_file, array_start, TDF_DETAILS);
      fprintf (tree_dump_file, "\narray min val = %d\n", array_start_val);
    }

  *array_first_index = array_start_val;

  return true;
}


/* Function vect_analyze_data_ref_access.  */

static bool
vect_analyze_data_ref_access (struct data_reference *dr)
{
  varray_type access_fns = DR_ACCESS_FNS (dr);
  tree stmt = DR_STMT (dr);
  tree ref = DR_REF (dr);
  tree vectype;
  tree access_fn;
  tree init, step;
  int init_val;
  tree scalar_type;
  int misalign;
  int array_start_val;
  bool ok;

  /* FORNOW: handle only one dimensional arrays.
     This restriction will be relaxed in the future.  */
  if (VARRAY_ACTIVE_SIZE (access_fns) != 1)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "multi dimensional array reference.\n");
      return false;
    }
  access_fn = DR_ACCESS_FN (dr, 0);

  if (!vect_is_simple_iv_evolution (loop_num (loop_of_stmt (DR_STMT (dr))), 
	access_fn, &init, &step, true))
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "too complicated access function\n");
	  print_generic_expr (tree_dump_file, access_fn, TDF_SLIM);
	}
      return false;
    }

  /* FORNOW: In order to simplify the handling of alignment, in addition
     to the above we also make sure that the first location
     at which the array is accessed ('init') is on an 'NUNITS'
     boundary, since we are also making sure that the array base
     is aligned. This restriction will be relaxed in the future.  */
  if (TREE_CODE (init) != INTEGER_CST)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "init not INTEGER_CST\n");
      return false;
    }

  /* CHECKME */
  if (TREE_INT_CST_HIGH (init) != 0)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "init CST_HIGH != 0\n");
      return false;
    }

  init_val = TREE_INT_CST_LOW (init);

  scalar_type = TREE_TYPE (ref);
  vectype = get_vectype_for_scalar_type (scalar_type);
  if (!vectype)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "no vectype for stmt: ");
	  print_generic_expr (tree_dump_file, stmt, TDF_SLIM);
	  fprintf (tree_dump_file, "\nscalar_type: ");
          print_generic_expr (tree_dump_file, scalar_type, TDF_DETAILS);
	  fprintf (tree_dump_file, "\n");
	}
      return false;
    }

  ok = vect_get_array_first_index (ref, &array_start_val);
  if (!ok)
    return false;

  misalign = (init_val - array_start_val) %
        GET_MODE_NUNITS (TYPE_MODE (vectype));

  if (misalign)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "first access not aligned.\n");
      return false;
    }

  return true;
}


/* Function vect_analyze_data_ref_accesses.

   Analyze the access pattern of all the data references in the loop.

   FORNOW: the only access pattern that is considered vectorizable is a
	   simple step 1 (consecutive) access.

   FORNOW: handle only one dimensional arrays.  */

static bool
vect_analyze_data_ref_accesses (loop_vec_info loop_vinfo)
{
  unsigned int i;
  varray_type loop_write_datarefs = LOOP_VINFO_DATAREF_WRITES (loop_vinfo);
  varray_type loop_read_datarefs = LOOP_VINFO_DATAREF_READS (loop_vinfo);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "\n<<vect_analyze_data_ref_accesses>>\n");

  for (i = 0; i < VARRAY_ACTIVE_SIZE (loop_write_datarefs); i++)
    {
      struct data_reference *dr = VARRAY_GENERIC_PTR (loop_write_datarefs, i);
      bool ok = vect_analyze_data_ref_access (dr);
      if (!ok)
	return false;
    }

  for (i = 0; i < VARRAY_ACTIVE_SIZE (loop_read_datarefs); i++)
    {
      struct data_reference *dr = VARRAY_GENERIC_PTR (loop_read_datarefs, i);
      bool ok = vect_analyze_data_ref_access (dr);
      if (!ok)
	return false;
    }

  return true;
}


/* Function vect_analyze_data_refs.

   Find all the data references in the loop.

   FORNOW: Handle only one dimensional ARRAY_REFs which base is really an
           array (not a pointer).  */

static bool
vect_analyze_data_refs (loop_vec_info loop_vinfo)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  basic_block *bbs = LOOP_VINFO_BBS (loop_vinfo);
  int nbbs = loop->num_nodes;
  block_stmt_iterator si;
  int j;
  struct data_reference *dr;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "\n<<vect_analyze_data_refs>>\n");

  for (j = 0; j < nbbs; j++)
    {
      basic_block bb = bbs[j];
      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  tree stmt = bsi_stmt (si);
	  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
	  vdef_optype vdefs = STMT_VDEF_OPS (stmt);
	  vuse_optype vuses = STMT_VUSE_OPS (stmt);
	  varray_type datarefs = NULL;
	  int nvuses = 0, nvdefs = 0;
	  tree ref = NULL;
	  tree array_base;

	  /* CHECKME: Relying on the fact that there exists a data-ref
	     in stmt, if and only if it has vuses/vdefs.  */

	  if (!vuses && !vdefs)
	    continue;

	  if (vuses)
	    nvuses = NUM_VUSES (vuses);
	  if (vdefs)
	    nvdefs = NUM_VDEFS (vdefs);

	  if (nvuses + nvdefs != 1)
	    {
	      /* CHECKME: multiple vdefs/vuses in a GIMPLE stmt are
	         assumed to indicate a non vectorizable stmt (e.g, ASM,
	         CALL_EXPR) or the presence of an aliasing problem. The
	         first case is ruled out during vect_analyze_operations;
	         As for the second case, currently the vuses/vdefs are
	         meaningless as they are too conservative. We therefore
	         ignore them.  */

	      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		{
	          fprintf (tree_dump_file, "Warning: multiple vops!\n");
	          print_generic_stmt (tree_dump_file, stmt, 
			~(TDF_RAW | TDF_SLIM | TDF_LINENO));
		}
	    }

	  if (TREE_CODE (stmt) != MODIFY_EXPR)
	    {
	      /* CHECKME: a vdef/vuse in a GIMPLE stmt is assumed to
	         appear only in a MODIFY_EXPR.  */

	      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		{
		  fprintf (tree_dump_file, "unexpected vops in stmt\n");
		  print_generic_stmt (tree_dump_file, stmt, TDF_SLIM);
		}
	      return false;
	    }

	  if (vuses)
	    {
	      if (TREE_CODE (TREE_OPERAND (stmt, 1)) == ARRAY_REF)
		{
		  ref = TREE_OPERAND (stmt, 1);
		  datarefs = LOOP_VINFO_DATAREF_READS (loop_vinfo);
		}
	    }

	  if (vdefs)
	    {
	      if (TREE_CODE (TREE_OPERAND (stmt, 0)) == ARRAY_REF)
		{
		  ref = TREE_OPERAND (stmt, 0);
		  datarefs = LOOP_VINFO_DATAREF_WRITES (loop_vinfo);
		}
	    }

	  if (!ref)
	    {
	      /* A different type of data reference (pointer?, struct?)
	         FORNOW: Do not attempt to handle.  */
	      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		{
		  fprintf (tree_dump_file, "unhandled non-array data ref\n");
		  print_generic_stmt (tree_dump_file, stmt, TDF_SLIM);
		}
	      return false;
	    }

	  dr = analyze_array (stmt, ref);

	  array_base = TREE_OPERAND (ref, 0);

	  /* FORNOW: make sure that the array is one dimensional.
	     This restriction will be relaxed in the future.  */
	  if (TREE_CODE (array_base) == ARRAY_REF)
	    {
	      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		{
		  fprintf (tree_dump_file, "unhandled 2D-array data ref\n");
		  print_generic_stmt (tree_dump_file, stmt, TDF_SLIM);
		}
	      return false;
	    }

	  /* CHECKME: We want to make sure that we can force alignment of
	     the data structure that is being accessed, because we do not
	     handle misalignment yet. 
	     CHECKME: Is this a correct check for this purpose?
	     CHECKME: This is a very strict check.  
	   */

	  if ( TREE_CODE (array_base) != VAR_DECL 
	       || TREE_CODE (TREE_TYPE (array_base)) != ARRAY_TYPE)
	    {
	      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		{
		  fprintf (tree_dump_file, "unhandled ptr-based array ref\n");
		  print_generic_stmt (tree_dump_file, stmt, TDF_SLIM);
		}
	      return false;
	    }

	  /* In addition to the above, we also check that the first
	     location in the array that is being accessed is aligned
	     (in analyze_data_ref_accesses).  */

	  VARRAY_PUSH_GENERIC_PTR (datarefs, dr);
	  STMT_VINFO_DATA_REF (stmt_info) = dr;
	}
    }

  return true;
}


/* Utility functions used by vect_mark_stmts_to_be_vectorized.
   Implementation inspired by tree-ssa-dce.c.  */

/* Function vect_mark_relevant.

   Mark STMT as "relevant for vectorization" and add it to WORKLIST.  */

static void
vect_mark_relevant (varray_type worklist, tree stmt)
{
  stmt_vec_info stmt_info;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "mark relevant.\n");

  if (TREE_CODE (stmt) == PHI_NODE)
    {
      VARRAY_PUSH_TREE (worklist, stmt);
      return;
    }

  stmt_info = vinfo_for_stmt (stmt);

  if (!stmt_info)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file, "mark relevant: no stmt info!!\n");
	  print_generic_expr (tree_dump_file, stmt, TDF_SLIM);
	}
      return;
    }

  if (STMT_VINFO_RELEVANT_P (stmt_info))
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        fprintf (tree_dump_file, "already marked relevant.\n");
      return;
    }

  STMT_VINFO_RELEVANT_P (stmt_info) = 1;
  VARRAY_PUSH_TREE (worklist, stmt);
}


/* Function vect_stmt_relevant_p.

   Return true if STMT in loop that is represented by LOOP_VINFO is
   "relevant for vectorization".

   A stmt is considered "relevant for vectorization" if:
   - it has uses outside the loop.
   - it has vdefs (it alters memory).
   - control stmts in the loop (except for the exit condition).

   CHECKME: what other side effects would the vectorizer allow?  */

static bool
vect_stmt_relevant_p (tree stmt, loop_vec_info loop_vinfo)
{
  vdef_optype vdefs;
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  int i;
  dataflow_t df;
  int num_uses;

  /* cond stmt other than loop exit cond.  */
  if (is_ctrl_stmt (stmt) && (stmt != LOOP_VINFO_EXIT_COND (loop_vinfo)))
    return true;

  /* changing memory.  */
  get_stmt_operands (stmt);
  vdefs = STMT_VDEF_OPS (stmt);
  if (vdefs)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        fprintf (tree_dump_file, "vec_stmt_relevant_p: stmt has vdefs:\n");
      return true;
    }

  /* uses outside the loop.  */
  df = get_immediate_uses (stmt);
  num_uses = num_immediate_uses (df);
  for (i = 0; i < num_uses; i++)
    {
      tree use = immediate_use (df, i);
      basic_block bb = bb_for_stmt (use);
      if (!flow_bb_inside_loop_p (loop, bb))
	{
	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    fprintf (tree_dump_file, 
		"vec_stmt_relevant_p: used out of loop:\n");
	  return true;
	}
    }

  return false;
}


/* Function vect_mark_stmts_to_be_vectorized.

   Not all stmts in the loop need to be vectorized. For example:

     for i...
       for j...
   1.    T0 = i + j
   2.	 T1 = a[T0]

   3.    j = j + 1

   Stmt 1 and 3 do not need to be vectorized, because loop control and
   addressing of vectorized data-refs are handled differently.

   This pass detects such stmts.  */

static bool
vect_mark_stmts_to_be_vectorized (loop_vec_info loop_vinfo)
{
  varray_type worklist;
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  basic_block *bbs = LOOP_VINFO_BBS (loop_vinfo);
  unsigned int nbbs = loop->num_nodes;
  block_stmt_iterator si;
  tree stmt;
  stmt_ann_t ann;
  unsigned int i;
  int j;
  use_optype use_ops;
  stmt_vec_info stmt_info;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "\n<<vect_mark_stmts_to_be_vectorized>>\n");

  VARRAY_TREE_INIT (worklist, 64, "work list");

  /* 1. Init worklist.  */

  for (i = 0; i < nbbs; i++)
    {
      basic_block bb = bbs[i];
      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  stmt = bsi_stmt (si);

	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    {
	      fprintf (tree_dump_file, "init: stmt relevant?\n");
	      print_generic_stmt (tree_dump_file, stmt, TDF_SLIM);
	    } 

	  stmt_info = vinfo_for_stmt (stmt);
	  STMT_VINFO_RELEVANT_P (stmt_info) = 0;

	  if (vect_stmt_relevant_p (stmt, loop_vinfo))
	    vect_mark_relevant (worklist, stmt);
	}
    }


  /* 2. Process_worklist */

  while (VARRAY_ACTIVE_SIZE (worklist) > 0)
    {
      stmt = VARRAY_TOP_TREE (worklist);
      VARRAY_POP (worklist);

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
          fprintf (tree_dump_file, "worklist: examine stmt:\n");
          print_generic_stmt (tree_dump_file, stmt, TDF_SLIM);
	}

      /* Examine the USES in this statement. Mark all the statements which
         feed this statement's uses as "relevant", unless the USE is used as
         an array index.  */

      if (TREE_CODE (stmt) == PHI_NODE)
	{
	  /* follow the def-use chain inside the loop.  */
	  for (j = 0; j < PHI_NUM_ARGS (stmt); j++)
	    {
	      tree arg = PHI_ARG_DEF (stmt, j);
	      if (TREE_CODE (arg) == SSA_NAME)
		{
		  tree def_stmt = NULL_TREE;
		  basic_block bb;

		  if (TREE_CODE (arg) == SSA_NAME)
		    def_stmt = SSA_NAME_DEF_STMT (arg);

		  if (def_stmt == NULL_TREE 
		      || TREE_CODE (def_stmt) == NOP_EXPR)
		    {
		      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		        fprintf (tree_dump_file, "\nworklist: no def_stmt!\n");
		      varray_clear (worklist);
		      return false;
		    }

		  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		    {
		      fprintf (tree_dump_file, "\nworklist: def_stmt:\n");
		      print_generic_expr (tree_dump_file, def_stmt, TDF_SLIM);
		    }

		  bb = bb_for_stmt (def_stmt);
		  if (flow_bb_inside_loop_p (loop, bb))
		    vect_mark_relevant (worklist, def_stmt);
		}
	    }

	  continue;
	}

      get_stmt_operands (stmt);
      ann = stmt_ann (stmt);
      use_ops = USE_OPS (ann);

      for (i = 0; i < NUM_USES (use_ops); i++)
	{
	  tree use = USE_OP (use_ops, i);
	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    {
	      fprintf (tree_dump_file, "\nworklist: examine use %d:\n", i);
	      print_generic_expr (tree_dump_file, use, TDF_SLIM);
	    }

	  if (exist_non_indexing_operands_for_use_p (use, stmt))
	    {
	      tree def_stmt = NULL_TREE;
	      basic_block bb;

	      if (TREE_CODE (use) == SSA_NAME)
		def_stmt = SSA_NAME_DEF_STMT (use);

	      if (def_stmt == NULL_TREE || TREE_CODE (def_stmt) == NOP_EXPR)
		{
		  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
		    fprintf (tree_dump_file, "\nworklist: no def_stmt!\n");
		  varray_clear (worklist);
		  return false;
		}

	      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))	
		{
	          fprintf (tree_dump_file, "\nworklist: def_stmt:\n");
	          print_generic_expr (tree_dump_file, def_stmt, TDF_SLIM);
		}

	      bb = bb_for_stmt (def_stmt);
	      if (flow_bb_inside_loop_p (loop, bb))
		vect_mark_relevant (worklist, def_stmt);
	    }
	}

    }				/* while worklist */

  varray_clear (worklist);
  return true;
}


/* Function vect_get_loop_niters.

   Determine How many iterations the loop is executed.

   FORNOW: Handling a simple limited set of loop forms. In the future - use
           a more general implementation.  */

static tree
vect_get_loop_niters (struct loop *loop, int *number_of_iterations)
{
  tree niters;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "\n<<get_loop_niters>>\n");

  niters = number_of_iterations_in_loop (loop);

  if (niters != NULL_TREE
      && TREE_CODE (niters) == INTEGER_CST)
    {
      *number_of_iterations = TREE_INT_CST_LOW (niters);

      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
        fprintf (tree_dump_file, "get_loop_niters: %d.\n",
		*number_of_iterations);
    }

  return get_loop_exit_condition (loop);
}


/* Function vect_analyze_loop_form.

   FORNOW: Verify the following restrictions:

   - it's an inner-most loop
   - number of BBs = 2 (which are the loop header and the latch)
   - the loop has a pre header
   - the loop has a single entry and exit
   - the loop exit condition is simple enough  */

static loop_vec_info
vect_analyze_loop_form (struct loop *loop)
{
  loop_vec_info loop_vinfo;
  tree loop_cond;
  int number_of_iterations = -1;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "\n<<vect_analyze_loop_form>>\n");

  if (loop->level > 1		/* FORNOW: inner-most loop (CHECKME)  */
      || loop->num_exits > 1 || loop->num_entries > 1 || loop->num_nodes != 2
      || !loop->pre_header || !loop->header || !loop->latch)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	{
	  fprintf (tree_dump_file,
	   "loop_analyzer: bad loop form (entry/exit, nbbs, level...)\n");
	  flow_loop_dump (loop, tree_dump_file, NULL, 1);
	}

      return NULL;
    }

  loop_cond = vect_get_loop_niters (loop, &number_of_iterations);
  if (!loop_cond)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "Complicated exit condition.\n");
      return NULL;
    }

  if (number_of_iterations < 0)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "Can't determine num iters.\n");
      return NULL;
    }

  /* CHECKME: check monev analyzer.  */
  if (number_of_iterations == 0)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "0 iterations??\n");
      return NULL;
    }

  loop_vinfo = new_loop_vec_info (loop);

  LOOP_VINFO_EXIT_COND (loop_vinfo) = loop_cond;
  LOOP_VINFO_NITERS (loop_vinfo) = number_of_iterations;

  return loop_vinfo;
}


/* Function vect_analyze_loop.

   Apply a set of analyses on LOOP, and create a loop_vec_info struct
   for it. The different analyses will record information in the
   loop_vec_info struct.  */

static loop_vec_info
vect_analyze_loop (struct loop *loop)
{
  bool ok;
  loop_vec_info loop_vinfo;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "\n\n\n<<<<<<< analyze_loop_nest >>>>>>>\n");

  /* Check the CFG characteristics of the loop (nesting, entry/exit, etc.  */

  loop_vinfo = vect_analyze_loop_form (loop);
  if (!loop_vinfo)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "loop_analyzer: bad loop form.\n");
      return NULL;
    }

  /* Find all data references in the loop (which correspond to vdefs/vuses)
     and analyze their evolution in the loop.

     FORNOW: Handle only simple, one-dimensional, array references, which
     alignment can be forced.  */

  ok = vect_analyze_data_refs (loop_vinfo);
  if (!ok)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "loop_analyzer: bad data references.\n");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }


  /* Data-flow analysis to detect stmts that do not need to be vectorized.  */

  ok = vect_mark_stmts_to_be_vectorized (loop_vinfo);
  if (!ok)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "loop_analyzer: unexpected pattern.\n");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }


  /* Check that all cross-iteration scalar data-flow cycles are OK.
     Cross-iteration cycles caused by virtual phis are analyzed separately.  */

  ok = vect_analyze_scalar_cycles (loop_vinfo);
  if (!ok)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "loop_analyzer: bad scalar cycle.\n");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }


  /* Analyze data dependences between the data-refs in the loop.
     FORNOW: We do not construct a data dependence graph and try to deal
     with dependences, but fail at the first data dependence that
     we encounter.  */

  ok = vect_analyze_data_ref_dependences (loop_vinfo);

  /* TODO: May want to generate run time pointer aliasing checks and
     loop versioning.  */

  /* TODO: May want to perform loop transformations to break dependence
     cycles.  */

  if (!ok)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "loop_analyzer: bad data dependence.\n");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }


  /* Analyze the access patterns of the data-refs in the loop (consecutive,
     complex, etc.). FORNOW: Only handle consecutive access pattern.  */

  ok = vect_analyze_data_ref_accesses (loop_vinfo);
  if (!ok)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "loop_analyzer: bad data access.\n");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }


  /* Scan all the operations in the loop and make sure they are
     vectorizable.  */

  ok = vect_analyze_operations (loop_vinfo);
  if (!ok)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file, "loop_analyzer: bad operations.\n");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }

  /* TODO: May want to collapse conditional code and loop versioning.   */

  /* TODO: Alignment: May want to perform loop peeling and/or run time
     tests and loop versioning.  */

  LOOP_VINFO_VECTORIZABLE_P (loop_vinfo) = 1;

  return loop_vinfo;
}


/* Function vectorize_loops.
   Entry Point to loop vectorization phase.  */

void
vectorize_loops (struct loops *loops,
		 varray_type ev_info ATTRIBUTE_UNUSED)
{
  unsigned int i;
  unsigned int num_vectorized_loops = 0;

  /* Does the target support SIMD?  */
  /* FORNOW: until more sophisticated machine modelling is in place.  */
  if (!UNITS_PER_SIMD_WORD)
    {
      if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	fprintf (tree_dump_file,
		 "vectorizer: target vector size is not defined.\n");
      return;
    }

  /*  ----------- Analyze loops. -----------  */
  /* CHECKME */
  for (i = 1; i < loops->num; i++)
    {
      loop_vec_info loop_vinfo;
      struct loop *loop = loops->parray[i];

      flow_loop_scan (loop, LOOP_ALL);

      loop_vinfo = vect_analyze_loop (loop);
      loop->aux = loop_vinfo;

#ifndef ANALYZE_ALL_THEN_VECTORIZE_ALL
      if (!loop_vinfo || !LOOP_VINFO_VECTORIZABLE_P (loop_vinfo))
	continue;

      vect_transform_loop (loop_vinfo);
      num_vectorized_loops++;
#endif
    }

#ifdef ANALYZE_ALL_THEN_VECTORIZE_ALL
  for (i = 1; i < loops->num; i++)
    {
      struct loop *loop = loops->parray[i];
      loop_vec_info loop_vinfo = loop->aux;

      if (!loop_vinfo || !LOOP_VINFO_VECTORIZABLE_P (loop_vinfo))
	continue;

      vect_transform_loop (loop_vinfo);
      num_vectorized_loops++;
    }
#endif

  if (tree_dump_file && (tree_dump_flags & TDF_STATS))
    fprintf (tree_dump_file, "vectorized %u loops in function.\n",
	     num_vectorized_loops);

  /*  ----------- Finialize. -----------  */

  for (i = 1; i < loops->num; i++)
    {
      struct loop *loop = loops->parray[i];
      loop_vec_info loop_vinfo = loop->aux;
      destroy_loop_vec_info (loop_vinfo);
      loop->aux = NULL;
    }
}
