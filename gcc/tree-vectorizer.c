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

	The main entry to this pass is vectorize_loops(), in which
   the vectorizer applies a set of analyses on a given set of loops,
   followed by the actual vectorization transformation for the loops that
   had successfully passed the analysis phase.

	Throughout this pass we make a distinction between two types of
   data: scalars (which are represented by SSA_NAMES), and memory references
   ("data-refs"). These two types of data require different handling both 
   during analysis and transformation. The types of data-refs that the 
   vectorizer currently supports are ARRAY_REFS that are one dimensional 
   arrays which base is an array DECL (not a pointer), and INDIRECT_REFS 
   through pointers; both array and pointer accesses are required to have a 
   simple (consecutive) access pattern.

   Analysis phase:
   ===============
	The driver for the analysis phase is vect_analyze_loop_nest().
   It applies a set of analyses, some of which rely on the scalar evolution 
   analyzer (scev) developed by Sebastian Pop.

	During the analysis phase the vectorizer records some information
   per stmt in a "stmt_vec_info" struct which is attached to each stmt in the 
   loop, as well as general information about the loop as a whole, which is
   recorded in a "loop_vec_info" struct attached to each loop.

   Transformation phase:
   =====================
	The loop transformation phase scans all the stmts in the loop, and
   creates a vector stmt (or a sequence of stmts) for each scalar stmt S in
   the loop that needs to be vectorized. It insert the vector code sequence
   just before the scalar stmt S, and records a pointer to the vector code
   in STMT_VINFO_VEC_STMT (stmt_info) (stmt_info is the stmt_vec_info struct 
   attached to S). This pointer will be used for the vectorization of following
   stmts which use the def of stmt S. Stmt S is removed if it writes to memory;
   otherwise, we rely on dead code elimination for removing it.

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

	Operands that are not SSA_NAMEs, are data-refs that appear in 
   load/store operations (like 'x[i]' in S1), and are handled differently.

   Target modeling:
   =================
	Currently the only target specific information that is used is the
   size of the vector (in bytes) - "UNITS_PER_SIMD_WORD". Targets that can 
   support different sizes of vectors, for now will need to specify one value 
   for "UNITS_PER_SIMD_WORD". More flexibility will be added in the future.

	Since we only vectorize operations which vector form can be
   expressed using existing tree codes, to verify that an operation is
   supported, the vectorizer checks the relevant optab at the relevant
   machine_mode (e.g, add_optab->handlers[(int) V8HImode].insn_code). If
   the value found is CODE_FOR_nothing, then there's no target support, and
   we can't vectorize the stmt.

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
#include "cfglayout.h"
#include "expr.h"
#include "optabs.h"
#include "tree-chrec.h"
#include "tree-data-ref.h"
#include "tree-scalar-evolution.h"
#include "tree-vectorizer.h"
#include "tree-pass.h"

/* Main analysis functions.  */
static loop_vec_info vect_analyze_loop (struct loop *);
static loop_vec_info vect_analyze_loop_form (struct loop *);
static bool vect_analyze_data_refs (loop_vec_info);
static bool vect_mark_stmts_to_be_vectorized (loop_vec_info);
static bool vect_analyze_scalar_cycles (loop_vec_info);
static bool vect_analyze_data_ref_accesses (loop_vec_info);
static bool vect_analyze_data_refs_alignment (loop_vec_info);
static void vect_compute_data_refs_alignment (loop_vec_info);
static bool vect_analyze_operations (loop_vec_info);
/* APPLE LOCAL begin AV vmul_uch --haifa  */
static void vect_pattern_recog (loop_vec_info);
static void vect_recog_mul_uch_to_uch (loop_vec_info);
/* APPLE LOCAL end AV vmul_uch --haifa  */

/* Main code transformation functions.  */
static void vect_transform_loop (loop_vec_info, struct loops *);
static void vect_transform_loop_bound (loop_vec_info, tree);
static bool vect_transform_stmt (tree, block_stmt_iterator *);
static tree vect_transform_load (tree, block_stmt_iterator *);
static tree vect_transform_store (tree, block_stmt_iterator *);
static tree vect_transform_op (tree, block_stmt_iterator *);
static tree vect_transform_assignment (tree, block_stmt_iterator *);
/* APPLE LOCAL begin AV if-conversion --dpatel  */
static tree vect_transform_select (tree, block_stmt_iterator *);
static tree vect_transform_compare (tree, block_stmt_iterator *);
/* APPLE LOCAL end AV if-conversion --dpatel  */
static void vect_align_data_ref (tree);
static void vect_enhance_data_refs_alignment (loop_vec_info);

/* Utility functions for the analyses.  */
static bool vect_is_supportable_op (tree);
/* APPLE LOCAL AV if-conversion --dpatel  */
static bool vect_is_supportable_operation (tree, tree, struct loop *);
static bool vect_is_supportable_store (tree);
static bool vect_is_supportable_load (tree);
static bool vect_is_supportable_assignment (tree);
/* APPLE LOCAL begin AV if-conversion --dpatel  */
static bool vect_is_supportable_compare (tree);
static bool vect_is_supportable_select (tree);
/* APPLE LOCAL end AV if-conversion --dpatel  */
static bool vect_is_simple_use (tree , struct loop *, tree *);
static bool exist_non_indexing_operands_for_use_p (tree, tree);
static bool vect_is_simple_iv_evolution (unsigned, tree, tree *, tree *, bool);
static void vect_mark_relevant (varray_type, tree);
static bool vect_stmt_relevant_p (tree, loop_vec_info);
static tree vect_get_loop_niters (struct loop *, int *);
static void vect_compute_data_ref_alignment 
  (struct data_reference *, loop_vec_info);
static bool vect_analyze_data_ref_access (struct data_reference *);
static bool vect_get_array_first_index (tree, int *);
static bool vect_force_dr_alignment_p (struct data_reference *);
static bool vect_analyze_loop_with_symbolic_num_of_iters 
		(tree *, struct loop *);
static struct data_reference * vect_analyze_pointer_ref_access (tree, tree);

/* Utility functions for the code transformation.  */
static tree vect_create_destination_var (tree, tree);
/* APPLE LOCAL begin AV misaligned --haifa  */
/* APPLE LOCAL remove params til we fix vect_create_data_ref. */
/* static tree vect_create_data_ref 
   (tree, block_stmt_iterator *, bool, tree *, bool);*/
static tree vect_create_data_ref (tree, block_stmt_iterator *);
/* APPLE LOCAL MERGE FIXME revert to version on lno branch */
/* static tree vect_create_index_for_array_ref (tree, block_stmt_iterator *,
   tree *, bool);*/
static tree vect_create_index_for_array_ref (tree, block_stmt_iterator *);

static void vect_finish_stmt_generation_in_preheader (tree, struct loop *);
/* APPLE LOCAL end AV misaligned --haifa  */
static tree get_vectype_for_scalar_type (tree);
static tree vect_get_new_vect_var (tree, enum vect_var_kind, const char *);
static tree vect_get_vec_def_for_operand (tree, tree);
/* APPLE LOCAL AV if-conversion -dpatel  */
static tree vect_init_vector (tree, tree, tree);
static tree vect_build_symbl_bound (tree n, int vf, struct loop * loop);
static basic_block vect_gen_if_guard (edge, tree, basic_block, edge);
static void vect_finish_stmt_generation 
  (tree stmt, tree vec_stmt, block_stmt_iterator *bsi);

/* Utility functions for loop duplication.  */
static basic_block vect_tree_split_edge (edge);
static void vect_update_initial_conditions_of_duplicated_loop 
	(loop_vec_info, tree, basic_block, edge, edge);

/* Utilities for creation and deletion of vec_info structs.  */
loop_vec_info new_loop_vec_info (struct loop *loop);
void destroy_loop_vec_info (loop_vec_info);
stmt_vec_info new_stmt_vec_info (tree stmt, struct loop *loop);

static bool vect_debug_stats (struct loop *loop);
static bool vect_debug_details (struct loop *loop);

/* APPLE LOCAL begin AV if-conversion --dpatel  */
static void vect_loop_version (struct loops *, struct loop *, basic_block *);
static bool second_loop_vers_available;
static bool if_converted_loop;
/* APPLE LOCAL end AV if-conversion --dpatel  */
/* Define number of arguments for each tree code.  */
#define DEFTREECODE(SYM, STRING, TYPE, NARGS)   NARGS,

int tree_nargs[] = {
#include "tree.def"

};

#undef DEFTREECODE


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
  /* APPLE LOCAL begin AV vmul_uch --haifa  */
  STMT_VINFO_RELEVANT_P (res) = false;
  STMT_VINFO_IN_PATTERN_P (res) = false;
  STMT_VINFO_RELATED_STMT (res) = NULL;
  /* APPLE LOCAL end AV vmul_uch --haifa  */
  STMT_VINFO_VECTYPE (res) = NULL;
  STMT_VINFO_VEC_STMT (res) = NULL;
  STMT_VINFO_DATA_REF (res) = NULL;
  STMT_VINFO_MEMTAG (res) = NULL;

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
  LOOP_VINFO_SYMB_NUM_OF_ITERS (res) = NULL;
  VARRAY_GENERIC_PTR_INIT (LOOP_VINFO_DATAREF_WRITES (res), 20,
			   "loop_write_datarefs");
  VARRAY_GENERIC_PTR_INIT (LOOP_VINFO_DATAREF_READS (res), 20,
			   "loop_read_datarefs");
  return res;
}


/* Function destroy_loop_vec_info.
 
   Free LOOP_VINFO struct, as well as all the stmt_vec_info structs of all the 
   stmts in the loop.  */

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


/* Function debug_loop_stats.

   For vectorization statistics dumps.  */

static bool
vect_debug_stats (struct loop *loop)
{
  basic_block bb;
  block_stmt_iterator si;
  tree node = NULL_TREE;

  if (!dump_file || !(dump_flags & TDF_STATS))
    return false;

  if (!loop)
    {
      fprintf (dump_file, "\n");
      return true;
    }

  if (!loop->header)
    return false;

  bb = loop->header;

  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    {
      node = bsi_stmt (si);
      if (node && EXPR_P (node) && EXPR_LOCUS (node))
        break;
    }

  if (node && EXPR_P (node) && EXPR_LOCUS (node) 
      && EXPR_FILENAME (node) && EXPR_LINENO (node))
    {
      fprintf (dump_file, "\nloop at %s:%d: ", 
	EXPR_FILENAME (node), EXPR_LINENO (node));
      return true;
    }

  return false;
}


/* Function debug_loop_details.

   For vectorization debug dumps.  */

static bool
vect_debug_details (struct loop *loop)
{
   basic_block bb;
   block_stmt_iterator si;
   tree node = NULL_TREE;

  if (!dump_file || !(dump_flags & TDF_DETAILS))
    return false;

  if (!loop)
    {
      fprintf (dump_file, "\n");
      return true;
    }

  if (!loop->header)
    return false;

  bb = loop->header;

  for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
    {
      node = bsi_stmt (si);
      if (node && EXPR_P (node) && EXPR_LOCUS (node))
	break;
    }

  if (node && EXPR_P (node) && EXPR_LOCUS (node)
      && EXPR_FILENAME (node) && EXPR_LINENO (node))
    {
      fprintf (dump_file, "\nloop at %s:%d: ", 
               EXPR_FILENAME (node), EXPR_LINENO (node));
      return true;
    }

  return false;
}


/* Function vect_force_dr_alignment_p.

   Returns whether the alignment of a the data reference DR can be forced.
   TRUE is DR is a local (i.e, non ectern) array.
   FALSE otherwise.  */

static bool
vect_force_dr_alignment_p (struct data_reference *dr)
{
  tree ref = DR_REF (dr);
  tree array_base = DR_BASE_NAME (dr);

  if ((TREE_CODE (ref) != ARRAY_REF)
      || (TREE_CODE (TREE_TYPE (array_base)) != ARRAY_TYPE)
      || (TREE_CODE (array_base) == VAR_DECL && DECL_EXTERNAL (array_base)))
    return false;

  if (TREE_CODE (array_base) == VAR_DECL)
    return true;

  if (TREE_CODE (array_base) == COMPONENT_REF)
    {
      tree oprnd0 = TREE_OPERAND (array_base, 0);
      tree oprnd1 = TREE_OPERAND (array_base, 1);
      if (TREE_CODE (oprnd1) != FIELD_DECL
          || TREE_CODE (TREE_TYPE (oprnd1)) != ARRAY_TYPE)
	return false;

      if (vect_debug_details (NULL))
	{
	  fprintf (dump_file, "ok to force alignment? ");
	  print_generic_expr (dump_file, array_base, TDF_SLIM);
	}

      /* CHECKME */
      if (TREE_CODE (oprnd0) != VAR_DECL || DECL_EXTERNAL (oprnd0))
        return false;

      return true;
    }

  return false;
}


/* Function vect_get_new_vect_var.

   Returns a name for a new variable. The current naming scheme appends the 
   prefix "vect_" or "vect_p" (depending on the value of VAR_KIND) to 
   the name of vectorizer generated variables, and appends that to NAME if 
   provided.  */

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


/* Function create_index_for_array_ref.

   Create (and return) an index variable, along with it's update chain in the
   loop. This variable will be used to access a memory location in a vector
   operation.

   Input:
   STMT: The stmt that contains a memory data-ref.
   BSI: The block_stmt_iterator where STMT is. Any new stmts created by this
        function can be added here, or in the loop pre-header.

   FORNOW: We are only handling array accesses with step 1.  */
/* APPLE LOCAL begin MERGE FIXME revert to lno-branch version */
#if (0)
/* APPLE LOCAL AV misaligned --haifa  */
/* Additional parameter: init  */
/* Additional parameter: only_init  */
static tree
vect_create_index_for_array_ref (tree stmt, block_stmt_iterator *bsi,
				 tree *init, bool only_init)
{
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  struct loop *loop = STMT_VINFO_LOOP (stmt_info);
  struct data_reference *dr = STMT_VINFO_DATA_REF (stmt_info);
  tree expr = DR_REF (dr);
  tree access_fn;
  /* APPLE LOCAL AV misaligned --haifa  */
  tree init0;
  tree step;
  int array_first_index;
  tree indx_before_incr, indx_after_incr;
  int loopnum = loop->num;
  bool ok;
#ifdef ENABLE_CHECKING
  varray_type access_fns = DR_ACCESS_FNS (dr);

  /* FORNOW: handling only one dimensional arrays.  */
  if (VARRAY_ACTIVE_SIZE (access_fns) != 1)
    abort ();
#endif

  access_fn = DR_ACCESS_FN (dr, 0);
  /* APPLE LOCAL begin AV misaligned --haifa  */
  ok = vect_is_simple_iv_evolution (loopnum, access_fn, init, &step, true);

#ifdef ENABLE_CHECKING
  if (!ok)
    abort ();

  if (TREE_CODE (step) != INTEGER_CST)
    abort ();	

  if (TREE_INT_CST_HIGH (step) != 0)
    abort ();
#endif
  /* APPLE LOCAL end AV misaligned --haifa  */

  /* Calculate the 'init' of the new index.
     'array_first_index' (usually 0) is the TYPE_MIN_VALUE of the DOMAIN of
     'dr' (if it has a DOMAIN).  */
  ok = vect_get_array_first_index (expr, &array_first_index);
#ifdef ENABLE_CHECKING
  if (!ok)
    abort ();
#endif

  /* APPLE LOCAL begin AV misaligned --haifa  */
  init0 = integer_zero_node;

  /* Calculate the 'step' of the new index.
     FORNOW: always 1.  */
  step = integer_one_node;

  if (only_init)
    return 0;

  create_iv (init0, step, NULL_TREE, loop, bsi, false, 
	&indx_before_incr, &indx_after_incr); 
  /* APPLE LOCAL end AV misaligned --haifa  */

  return indx_before_incr;
}
#endif

static tree
vect_create_index_for_array_ref (tree stmt, block_stmt_iterator *bsi)
{
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  struct loop *loop = STMT_VINFO_LOOP (stmt_info);
  struct data_reference *dr = STMT_VINFO_DATA_REF (stmt_info);
  tree expr = DR_REF (dr);
  tree access_fn;
  int init_val, step_val;
  tree init, step;
  loop_vec_info loop_info = loop->aux;
  int vectorization_factor = LOOP_VINFO_VECT_FACTOR (loop_info);
  int array_first_index;
  int vec_init_val;
  tree indx_before_incr, indx_after_incr;
  int loopnum = loop->num;
  bool ok;
#ifdef ENABLE_CHECKING
  varray_type access_fns = DR_ACCESS_FNS (dr);

  /* FORNOW: handling only one dimensional arrays.  */
  if (VARRAY_ACTIVE_SIZE (access_fns) != 1)
    abort ();
#endif

  access_fn = DR_ACCESS_FN (dr, 0);
  ok = vect_is_simple_iv_evolution (loopnum, access_fn, &init, &step, true);

#ifdef ENABLE_CHECKING
  if (!ok)
    abort ();

  /* FORNOW: Handling only constant 'init'.  */
  if (TREE_CODE (init) != INTEGER_CST || TREE_CODE (step) != INTEGER_CST)
    abort ();

  if (TREE_INT_CST_HIGH (init) != 0 || TREE_INT_CST_HIGH (step) != 0)
    abort ();
#endif

  /* Calculate the 'init' of the new index.
     'array_first_index' (usually 0) is the TYPE_MIN_VALUE of the DOMAIN of
     'dr' (if it has a DOMAIN).  */
  init_val = TREE_INT_CST_LOW (init);
  ok = vect_get_array_first_index (expr, &array_first_index);
#ifdef ENABLE_CHECKING
  if (!ok)
    abort ();
#endif
  vec_init_val = (init_val - array_first_index)/vectorization_factor;
  init = build_int_2 (vec_init_val, 0);

  /* Calculate the 'step' of the new index.
     FORNOW: always 1.  */
  step_val = TREE_INT_CST_LOW (step);
  step = integer_one_node;

  create_iv (init, step, NULL_TREE, loop, bsi, false, 
	     &indx_before_incr, &indx_after_incr); 

  return indx_before_incr;
}
/* APPLE LOCAL end MERGE FIXME revert to lno-branch version */

/* Function get_vectype_for_scalar_type.

   Returns the vector type corresponding to SCALAR_TYPE as supported
   by the target.  */

static tree
get_vectype_for_scalar_type (tree scalar_type)
{
  enum machine_mode inner_mode = TYPE_MODE (scalar_type);
  int nbytes = GET_MODE_SIZE (inner_mode);
  int nunits;

  if (nbytes == 0)
    return NULL_TREE;

  /* FORNOW: Only a single vector size per target (UNITS_PER_SIMD_WORD)
     is expected.  */
  nunits = UNITS_PER_SIMD_WORD / nbytes;

  return build_vector_type (scalar_type, nunits);
}


/* Function vect_align_data_ref.

   Handle alignment of a memory accesses.

   FORNOW: Make sure the array is properly aligned. The vectorizer
           currently does not handle unaligned memory accesses.
           This restriction will be relaxed in the future.

   FORNOW: data_ref is an array_ref which alignment can be forced; i.e.,
           the base of the ARRAY_REF is not a pointer but an array.
           This restriction will be relaxed in the future.

   FORNOW: The array is being accessed starting at location 'init'; we limit 
           vectorization to cases in which 'init % nunits == 0'
           (NUNITS is the GET_MODE_NUNITS (TYPE_MODE (vectype))).
           This restriction will be relaxed in the future.  */

static void
vect_align_data_ref (tree stmt)
{
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);
  struct data_reference *dr = STMT_VINFO_DATA_REF (stmt_info);
  tree array_base = DR_BASE_NAME (dr);

#ifdef ENABLE_CHECKING
  /* FORNOW: can't handle misaligned accesses; 
             all accesses expected to be aligned.  */
  if (!aligned_access_p (dr))
    abort ();
#endif

  /* Check if the alignment of the base of the data structure needs to be 
     forced:  */ 

  if (vect_force_dr_alignment_p (dr))
    {
      tree decl = NULL_TREE;
      if (TREE_CODE (array_base) == VAR_DECL)
	decl = array_base;
      else if (TREE_CODE (array_base) == COMPONENT_REF)
	decl = TREE_OPERAND (array_base, 1);
#ifdef ENABLE_CHECKING
      if (!decl || (TREE_CODE (decl) != VAR_DECL && TREE_CODE (decl) != FIELD_DECL))
	abort ();
#endif
      if (DECL_ALIGN (decl) < TYPE_ALIGN (vectype))
	{
	  if (vect_debug_details (NULL))
            fprintf (dump_file, "force alignment. before: %d, after: %d",
		DECL_ALIGN (decl), TYPE_ALIGN (vectype));
          DECL_ALIGN (decl) = TYPE_ALIGN (vectype);
        }
   }
}


/* Function vect_create_data_ref.

   Create a memory reference expression for vector access, to be used in a
   vector load/store stmt.

   Input:
   STMT: a stmt that references memory. expected to be of the form
	 MODIFY_EXPR <name, data-ref> or MODIFY_EXPR <data-ref, name>.
   BSI: block_stmt_iterator where new stmts can be added.

   Output:
   1. Declare a new ptr to vector_type, and have it point to the array base.
      For example, for vector of type V8HI:
      v8hi *p0;
      p0 = (v8hi *)&a;
   2. Create a data-reference based on the new vector pointer p0, and using
      a new index variable 'idx'. Return the expression '(*p0)[idx]'.

   FORNOW: handle only aligned and consecutive accesses.  */

/* APPLE LOCAL MERGE FIXME this is the lno-branch version of the function
   which differs significantly from the Apple version. */

static tree
vect_create_data_ref (tree stmt, block_stmt_iterator *bsi )
{
  tree new_base;
  tree data_ref;
  tree idx;
  tree vec_stmt;
  tree new_temp;
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);
  tree ptr_type;
  tree vect_ptr;
  tree addr_ref;
  v_may_def_optype v_may_defs = STMT_V_MAY_DEF_OPS (stmt);
  v_must_def_optype v_must_defs = STMT_V_MUST_DEF_OPS (stmt);
  vuse_optype vuses = STMT_VUSE_OPS (stmt);
  int nvuses, nv_may_defs, nv_must_defs;
  int i;
  struct data_reference *dr = STMT_VINFO_DATA_REF (stmt_info);
  tree array_type;
  tree base_addr = NULL_TREE;
  struct loop *loop = STMT_VINFO_LOOP (stmt_info);
  edge pe;
  basic_block new_bb;
  tree tag;
  tree new_stmt, addr_expr;

  if (vect_debug_details (NULL))
    {
      fprintf (dump_file, "create array_ref of type: ");
      print_generic_expr (dump_file, vectype, TDF_SLIM);
    }

  /* FORNOW: make sure the data reference is aligned.  */
  vect_align_data_ref (stmt);

  addr_ref = DR_BASE_NAME (dr);

  /*** create: vectype *p;  ***/
  ptr_type = build_pointer_type (vectype);
  vect_ptr = vect_get_new_vect_var (ptr_type, vect_pointer_var, 
		get_name (addr_ref));
  add_referenced_tmp_var (vect_ptr);

#ifdef ENABLE_CHECKING
  if (TREE_CODE (addr_ref) != VAR_DECL
      && TREE_CODE (addr_ref) != COMPONENT_REF
      && TREE_CODE (addr_ref) != SSA_NAME)
    abort ();
#endif

  if (vect_debug_details (NULL))
    {
      if (TREE_CODE (addr_ref) == VAR_DECL)
	fprintf (dump_file, "vectorizing an array ref: ");
      else if (TREE_CODE (addr_ref) == SSA_NAME)
	fprintf (dump_file, "vectorizing a pointer ref: ");
      else if (TREE_CODE (addr_ref) == COMPONENT_REF)
	fprintf (dump_file, "vectorizing a record ref: ");
      print_generic_expr (dump_file, addr_ref, TDF_SLIM);
    }

  /* Get base address:  */
  if (TREE_CODE (addr_ref) == VAR_DECL || TREE_CODE (addr_ref) == COMPONENT_REF)
    base_addr = build1 (ADDR_EXPR,
        build_pointer_type (TREE_TYPE (addr_ref)), addr_ref);
  else if (TREE_CODE (addr_ref) == SSA_NAME)
    base_addr = addr_ref;

  /* Handle aliasing:  */ 
  tag = STMT_VINFO_MEMTAG (stmt_info);
#ifdef ENABLE_CHECKING
  if (!tag)
    abort ();
#endif
  get_var_ann (vect_ptr)->type_mem_tag = tag;
  
  /* Mark for renaming all aliased variables
     (i.e, the may-aliases of the type-mem-tag) */
  nvuses = NUM_VUSES (vuses);
  nv_may_defs = NUM_V_MAY_DEFS (v_may_defs);
  nv_must_defs = NUM_V_MUST_DEFS (v_must_defs);
  for (i = 0; i < nvuses; i++)
    {
      tree use = VUSE_OP (vuses, i);
      if (TREE_CODE (use) == SSA_NAME)
        bitmap_set_bit (vars_to_rename, var_ann (SSA_NAME_VAR (use))->uid);
    }
  for (i = 0; i < nv_may_defs; i++)
    {
      tree def = V_MAY_DEF_RESULT (v_may_defs, i);
      if (TREE_CODE (def) == SSA_NAME)
        bitmap_set_bit (vars_to_rename, var_ann (SSA_NAME_VAR (def))->uid);
    }
  for (i = 0; i < nv_must_defs; i++)
    {
      tree def = V_MUST_DEF_OP (v_must_defs, i);
      if (TREE_CODE (def) == SSA_NAME)
        bitmap_set_bit (vars_to_rename, var_ann (SSA_NAME_VAR (def))->uid);
    }

  pe = loop_preheader_edge (loop);

  /*** create: p = (vectype *)&a; ***/
  TREE_ADDRESSABLE (addr_ref) = 1;
  vec_stmt = build1 (NOP_EXPR, ptr_type, base_addr);
  addr_expr = create_tmp_var (ptr_type, "addr");
  add_referenced_tmp_var (addr_expr);
  new_temp = force_gimple_operand (vec_stmt, &new_stmt, false, addr_expr);
  new_bb = bsi_insert_on_edge_immediate (pe, new_stmt);
#ifdef ENABLE_CHECKING
  if (new_bb)
    abort ();
#endif
  vec_stmt = build (MODIFY_EXPR, void_type_node, vect_ptr, new_temp);
  new_temp = make_ssa_name (vect_ptr, vec_stmt);
  TREE_OPERAND (vec_stmt, 0) = new_temp;
  new_bb = bsi_insert_on_edge_immediate (pe, vec_stmt);
#ifdef ENABLE_CHECKING
  if (new_bb)
    abort ();
#endif

  /*** create data ref: '(*p)[idx]' ***/

  idx = vect_create_index_for_array_ref (stmt, bsi);

  array_type = build_array_type (vectype, 0);
  TYPE_ALIGN (array_type) = TYPE_ALIGN (TREE_TYPE (addr_ref)); /* CHECKME */
  new_base = build1 (INDIRECT_REF, array_type, TREE_OPERAND (vec_stmt, 0)); 
  data_ref = build (ARRAY_REF, vectype, new_base, idx);

  if (vect_debug_details (NULL))
    {
      fprintf (dump_file, "created new data-ref: ");
      print_generic_expr (dump_file, data_ref, TDF_SLIM);
    }

  return data_ref;
}


/* Function vect_create_destination_var.

   Create a new temporary of type VECTYPE.  */

static tree
vect_create_destination_var (tree scalar_dest, tree vectype)
{
  tree vec_dest;
  const char *new_name;

#ifdef ENABLE_CHECKING
  if (TREE_CODE (scalar_dest) != SSA_NAME)
    abort ();
#endif

  new_name = get_name (scalar_dest);
  if (!new_name)
    new_name = "var_";
  vec_dest = vect_get_new_vect_var (vectype, vect_simple_var, new_name);
  add_referenced_tmp_var (vec_dest);

  TYPE_ALIAS_SET (TREE_TYPE (vec_dest)) =
    TYPE_ALIAS_SET (TREE_TYPE (scalar_dest));

  return vec_dest;
}


/* Function vect_init_vector.

   Insert a new stmt (INIT_STMT) that initializes a new vector variable with
   the vector elements of VECTOR_VAR. Return the DEF of INIT_STMT. It will be
   used in the vectorization of STMT.  */
/* APPLE LOCAL AV if-conversion -dpatel  */
/* Additional vectype parameter.  */
static tree
vect_init_vector (tree stmt, tree vector_var, tree vectype)
{
  stmt_vec_info stmt_vinfo = vinfo_for_stmt (stmt);
  struct loop *loop = STMT_VINFO_LOOP (stmt_vinfo);
  tree new_var;
  tree init_stmt;
  /* APPLE LOCAL AV if-conversion -dpatel  */
  /* Remove local variable vectype.  */
  tree vec_oprnd;
  edge pe;
  basic_block new_bb;
 
  new_var = vect_get_new_vect_var (vectype, vect_simple_var, "cst_");
  add_referenced_tmp_var (new_var); 
 
  init_stmt = build (MODIFY_EXPR, vectype, new_var, vector_var);

  pe = loop_preheader_edge (loop);
  new_bb = bsi_insert_on_edge_immediate (pe, init_stmt);
#ifdef ENABLE_CHECKING
  if (new_bb)
    abort ();
#endif

  if (vect_debug_details (NULL))
    {
      fprintf (dump_file, "created new init_stmt: ");
      print_generic_expr (dump_file, init_stmt, TDF_SLIM);
    }

  vec_oprnd = TREE_OPERAND (init_stmt, 0);
  return vec_oprnd;
}


/* Function vect_get_vec_def_for_operand.

   OP is an operand in STMT. This function returns a (vector) def that will be
   used in the vectorized stmt for STMT.

   In the case that OP is an SSA_NAME which is defined in the loop, then
   STMT_VINFO_VEC_STMT of the defining stmt holds the relevant def.

   In case OP is an invariant or constant, a new stmt that creates a vector def
   needs to be introduced.  */

static tree
vect_get_vec_def_for_operand (tree op, tree stmt)
{
  tree vec_oprnd;
  tree vec_stmt;
  tree def_stmt;
  stmt_vec_info def_stmt_info = NULL;
  stmt_vec_info stmt_vinfo = vinfo_for_stmt (stmt);
  tree vectype = STMT_VINFO_VECTYPE (stmt_vinfo);
  int nunits = GET_MODE_NUNITS (TYPE_MODE (vectype));
  struct loop *loop = STMT_VINFO_LOOP (stmt_vinfo);
  basic_block bb;
  tree vec_inv;
  tree t = NULL_TREE;
  tree def;
  int i;


  /** ===> Case 1: operand is a constant.  **/

  if (TREE_CODE (op) == INTEGER_CST || TREE_CODE (op) == REAL_CST)
    {
      /* Create 'vect_cst_ = {cst,cst,...,cst}'  */

      tree vec_cst;
      stmt_vec_info stmt_vinfo = vinfo_for_stmt (stmt);
      tree vectype = STMT_VINFO_VECTYPE (stmt_vinfo);
      int nunits = GET_MODE_NUNITS (TYPE_MODE (vectype));
      tree t = NULL_TREE;
      int i;

      /* Build a tree with vector elements.  */
      if (vect_debug_details (NULL))
        fprintf (dump_file, "Create vector_cst.");
      for (i = nunits - 1; i >= 0; --i)
        {
          t = tree_cons (NULL_TREE, op, t);
        }
      vec_cst = build_vector (vectype, t);
      /* APPLE LOCAL AV if-conversion -dpatel  */
      return vect_init_vector (stmt, vec_cst, vectype);
    }

#ifdef ENABLE_CHECKING
  if (TREE_CODE (op) != SSA_NAME)
    abort ();
#endif
 
  /** ===> Case 2: operand is an SSA_NAME - find the stmt that defines it.  **/

  def_stmt = SSA_NAME_DEF_STMT (op);
  def_stmt_info = vinfo_for_stmt (def_stmt);

  if (vect_debug_details (NULL))
    {
      fprintf (dump_file, "vect_get_vec_def_for_operand: def_stmt: ");
      print_generic_expr (dump_file, def_stmt, TDF_SLIM);
    }


  /** ==> Case 2.1: operand is defined inside the loop.  **/

  if (def_stmt_info)
    {
      /* Get the def from the vectorized stmt.  */

      vec_stmt = STMT_VINFO_VEC_STMT (def_stmt_info);
#ifdef ENABLE_CHECKING
      if (!vec_stmt)
        abort ();
#endif
      vec_oprnd = TREE_OPERAND (vec_stmt, 0);
      return vec_oprnd;
    }


  /** ==> Case 2.2: operand is defined by the loop-header phi-node - 
                    it is a reduction/induction.  **/

  bb = bb_for_stmt (def_stmt);
  if (TREE_CODE (def_stmt) == PHI_NODE && flow_bb_inside_loop_p (loop, bb))
    {
      if (vect_debug_details (NULL))
	fprintf (dump_file, "reduction/induction - unsupported.");
      abort (); /* FORNOW no support for reduction/induction.  */
    }


  /** ==> Case 2.3: operand is defined outside the loop - 
                    it is a loop invariant.  */

  switch (TREE_CODE (def_stmt))
    {
    case PHI_NODE:
      def = PHI_RESULT (def_stmt);
      break;
    case MODIFY_EXPR:
      def = TREE_OPERAND (def_stmt, 0);
      break;
    case NOP_EXPR:
      def = TREE_OPERAND (def_stmt, 0);
#ifdef ENABLE_CHECKING
      if (TREE_CODE (def) != INTEGER_CST && TREE_CODE (def) != REAL_CST)
	abort ();
#endif
      def = op;
      break;
    default:
      if (vect_debug_details (NULL))
	{
          fprintf (dump_file, "unsupported defining stmt: ");
	  print_generic_expr (dump_file, def_stmt, TDF_SLIM);
	}
      abort ();
    }

  /* APPLE LOCAL begin AV vector_init --haifa  */
  /* Let the target generated the vector initialization
     code, if such hook available.  */
  if (targetm.vect.support_vector_init_p (vectype))
    {
      edge pe = loop_preheader_edge (STMT_VINFO_LOOP (stmt_vinfo));
      return targetm.vect.build_vector_init (vectype, def, pe,
                                             vars_to_rename);
    }
  /* APPLE LOCAL end AV vector_init --haifa  */

  /* Build a tree with vector elements. Create 'vec_inv = {inv,inv,..,inv}'  */

  if (vect_debug_details (NULL))
    fprintf (dump_file, "Create vector_inv.");

  for (i = nunits - 1; i >= 0; --i)
    {
      t = tree_cons (NULL_TREE, def, t);
    }

  vec_inv = build_constructor (vectype, t);
  /* APPLE LOCAL AV if-conversion -dpatel  */
  return vect_init_vector (stmt, vec_inv, vectype);
}


/* Function vect_finish_stmt_generation.

   Insert a new stmt.  */

static void
vect_finish_stmt_generation (tree stmt, tree vec_stmt, block_stmt_iterator *bsi)
{
  bsi_insert_before (bsi, vec_stmt, BSI_SAME_STMT);

  if (vect_debug_details (NULL))
    {
      fprintf (dump_file, "add new stmt: ");
      print_generic_expr (dump_file, vec_stmt, TDF_SLIM);
    }

  /* Make sure bsi points to the stmt that is being vectorized.  */

  /* Assumption: any stmts created for the vectorization of smtmt S are
     inserted before S. BSI may point to S or some new stmt before it.  */

  while (stmt != bsi_stmt (*bsi) && !bsi_end_p (*bsi))
    bsi_next (bsi);
#ifdef ENABLE_CHECKING
  if (stmt != bsi_stmt (*bsi))
    abort ();
#endif
}


/* Function vect_transform_assignment.

   STMT performs an assignment (copy). Create a vectorized stmt to replace it,
   and insert it at BSI.  */

static tree
vect_transform_assignment (tree stmt, block_stmt_iterator *bsi)
{
  tree vec_stmt;
  tree vec_dest;
  tree scalar_dest;
  tree op;
  tree vec_oprnd;
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);
  tree new_temp;

  if (vect_debug_details (NULL))
    fprintf (dump_file, "transform assignment.");
#ifdef ENABLE_CHECKING
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    abort ();
#endif

  /** Handle def. **/
  scalar_dest = TREE_OPERAND (stmt, 0);
#ifdef ENABLE_CHECKING
  if (TREE_CODE (scalar_dest) != SSA_NAME)
    abort ();
#endif
  vec_dest = vect_create_destination_var (scalar_dest, vectype);

  /** Handle use.  **/
  op = TREE_OPERAND (stmt, 1);
  vec_oprnd = vect_get_vec_def_for_operand (op, stmt);
  if (! vec_oprnd)
    abort ();

  /** Arguments are ready. create the new vector stmt.  **/
  vec_stmt = build (MODIFY_EXPR, vectype, vec_dest, vec_oprnd);
  new_temp = make_ssa_name (vec_dest, vec_stmt);
  TREE_OPERAND (vec_stmt, 0) = new_temp;
  vect_finish_stmt_generation (stmt, vec_stmt, bsi);
  return vec_stmt;
}

/* APPLE LOCAL begin AV if-conversion --dpatel  */
/* Function vect_transfom_compare

   STMT performs comparison. Create a vectorized stmt to replace it,
   and insert it at BSI.  */

static tree
vect_transform_compare (tree stmt, block_stmt_iterator *bsi)
{
  tree vec_stmt;
  tree vec_dest;
  tree scalar_dest;
  tree operand;
  tree vec_oprnd;
  tree operand1;
  tree vec_oprnd1;
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);
  tree new_temp;

  if (vect_debug_details (NULL))
    fprintf (dump_file, "transform compare");
#ifdef ENABLE_CHECKING
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    abort ();
#endif

  /** Handle def. **/

  scalar_dest = TREE_OPERAND (stmt, 0);
#ifdef ENABLE_CHECKING
  if (TREE_CODE (scalar_dest) != SSA_NAME)
    abort ();
#endif
  vec_dest = vect_create_destination_var (scalar_dest, vectype);

  /** Handle condition.  **/
#ifdef ENABLE_CHECKING
  if (TREE_CODE_CLASS (TREE_CODE (TREE_OPERAND (stmt, 1))) != '<')
    abort ();
#endif

  /** Handle use - get the vectorized def from the defining stmt.  **/

  operand = TREE_OPERAND (TREE_OPERAND (stmt, 1), 0);
  vec_oprnd = vect_get_vec_def_for_operand (operand, stmt);
  operand1 = TREE_OPERAND (TREE_OPERAND (stmt, 1), 1);
  vec_oprnd1 = vect_get_vec_def_for_operand (operand1, stmt);
  if (! vec_oprnd)
    abort ();

  /** Arguments are ready. create the new vector stmt.  **/

  vec_stmt = targetm.vect.vector_compare_stmt (vectype, vec_dest, 
	       vec_oprnd, vec_oprnd1, TREE_CODE (TREE_OPERAND (stmt, 1)));

  new_temp = make_ssa_name (vec_dest, vec_stmt);
  TREE_OPERAND (vec_stmt, 0) = new_temp;
  vect_finish_stmt_generation (stmt, vec_stmt, bsi);
  return vec_stmt;
}


/* Function vect_transfom_select
   
   STMT performs select. Create a vectorized stmt to replace it,
   and insert it at BSI.  */

static tree
vect_transform_select (tree stmt, block_stmt_iterator *bsi)
{
  tree vec_stmt;
  tree vec_dest;
  tree scalar_dest;
  tree op;
  tree vec_oprnd;
  tree op2;
  tree vec_oprnd2;
  tree cond;
  tree vec_cond;
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);
  tree new_temp;
  /* APPLE LOCAL MERGE FIXME comment out to avoid warnings */
  /* tree t; */

  if (vect_debug_details (NULL))
    fprintf (dump_file, "transform select\n");
#ifdef ENABLE_CHECKING
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    abort ();
#endif
  
  /** Handle def. **/

  scalar_dest = TREE_OPERAND (stmt, 0);
  if (TREE_CODE (scalar_dest) == SSA_NAME)
    vec_dest = vect_create_destination_var (scalar_dest, vectype);
  else if (TREE_CODE (scalar_dest) == ARRAY_REF)
    {
      vect_align_data_ref (stmt);
      /* APPLE LOCAL remove params til we fix vect_create_data_ref. */
      /* vec_dest = vect_create_data_ref (stmt, bsi, false, &t, false);*/
      vec_dest = vect_create_data_ref (stmt, bsi);
    }
  else
    abort ();

  /** Handle condition.  **/
#ifdef ENABLE_CHECKING
  if (TREE_CODE (TREE_OPERAND (stmt, 1)) != COND_EXPR)
    abort ();
#endif

  cond = TREE_OPERAND (TREE_OPERAND (stmt, 1), 0);
#ifdef ENABLE_CHECKING
  if (TREE_CODE (cond) != SSA_NAME)
    abort ();
#endif

  vec_cond = vect_get_vec_def_for_operand (cond, stmt);

  /** Handle use - get the vectorized def from the defining stmt.  **/

  op = TREE_OPERAND (TREE_OPERAND (stmt, 1), 1);
  if (TREE_CODE (op) == NOP_EXPR)
    {
      tree ztype = TREE_TYPE (TREE_OPERAND (stmt, 0));
      if (TREE_CODE (ztype) == INTEGER_TYPE)
	op = integer_zero_node;
      else if (TREE_CODE (ztype) == REAL_TYPE)
	op = build_real_from_int_cst (ztype, integer_zero_node);
    }
  
  vec_oprnd = vect_get_vec_def_for_operand (op, stmt);
  op2 = TREE_OPERAND (TREE_OPERAND (stmt, 1), 2);
  if (TREE_CODE (op2) == NOP_EXPR)
    {
      tree ztype = TREE_TYPE (TREE_OPERAND (stmt, 0));
      if (TREE_CODE (ztype) == INTEGER_TYPE)
	op2 = integer_zero_node;
      else if (TREE_CODE (ztype) == REAL_TYPE)
	op2 = build_real_from_int_cst (ztype, integer_zero_node);
    }
  vec_oprnd2 = vect_get_vec_def_for_operand (op2, stmt);

  /** Arguments are ready. create the new vector stmt.  **/

  vec_stmt = targetm.vect.vector_select_stmt (vectype, vec_dest,
 					 vec_cond, vec_oprnd2, vec_oprnd);
  if (TREE_CODE (vec_dest) == VAR_DECL)
    {
      new_temp = make_ssa_name (vec_dest, vec_stmt);
      TREE_OPERAND (vec_stmt, 0) = new_temp;
    }
  vect_finish_stmt_generation (stmt, vec_stmt, bsi);
  return vec_stmt;
}
/* APPLE LOCAL end AV if-conversion --dpatel  */


/* Function vect_transform_op.

   STMT performs a binary or unary operation. Create a vectorized stmt to
   replace it, and insert it at BSI.  */

static tree
vect_transform_op (tree stmt, block_stmt_iterator *bsi)
{
  tree vec_stmt;
  tree vec_dest;
  tree scalar_dest;
  tree operation;
  tree op0, op1=NULL;
  tree vec_oprnd0, vec_oprnd1=NULL;
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);
  /* APPLE LOCAL MERGE FIXME initialize to avoid warning */
  enum tree_code code=0;
  tree new_temp;
  int op_type;
  /* APPLE LOCAL AV vmul_uch --haifa  */
  tree (* hook) (tree, tree, tree, edge, block_stmt_iterator *);

  if (vect_debug_details (NULL))
    fprintf (dump_file, "transform binary/unary operation.");
#ifdef ENABLE_CHECKING
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    abort ();
#endif
  operation = TREE_OPERAND (stmt, 1);

  /** Handle def.  **/
  scalar_dest = TREE_OPERAND (stmt, 0);
#ifdef ENABLE_CHECKING
  if (TREE_CODE (scalar_dest) != SSA_NAME)
    abort ();
#endif
  vec_dest = vect_create_destination_var (scalar_dest, vectype);

  /** Handle uses.  **/
  op_type = tree_nargs[TREE_CODE (operation)];
#ifdef ENABLE_CHECKING 
  if (op_type != unary_op && op_type != binary_op)
    abort ();
#endif
  op0 = TREE_OPERAND (operation, 0);
  if (op_type == binary_op)
    op1 = TREE_OPERAND (operation, 1);

  vec_oprnd0 = vect_get_vec_def_for_operand (op0, stmt);

  if (op_type == binary_op)
    vec_oprnd1 = vect_get_vec_def_for_operand (op1, stmt); 

  /** Arguments are ready. create the new vector stmt.  **/

  /* APPLE LOCAL begin AV vmul_uch --haifa  */
  /* Is this a special pattern with target specific support?  */
  hook = STMT_VINFO_VEC_HOOK (stmt_info);
  if (hook)
    {
      edge pe = loop_preheader_edge (STMT_VINFO_LOOP (stmt_info));
      vec_stmt = (*hook) (vec_oprnd0, vec_oprnd1, vec_dest, pe, bsi); 
      STMT_VINFO_VEC_STMT 
	(vinfo_for_stmt (STMT_VINFO_RELATED_STMT (stmt_info))) = vec_stmt;
    }
  else
  /* APPLE LOCAL end AV vmul_uch --haifa  */
  code = TREE_CODE (operation);
  if (op_type == binary_op)
    vec_stmt = build (MODIFY_EXPR, vectype, vec_dest,
		build (code, vectype, vec_oprnd0, vec_oprnd1));
  else
    vec_stmt = build (MODIFY_EXPR, vectype, vec_dest,
		build1 (code, vectype, vec_oprnd0));
  new_temp = make_ssa_name (vec_dest, vec_stmt);
  TREE_OPERAND (vec_stmt, 0) = new_temp;
  vect_finish_stmt_generation (stmt, vec_stmt, bsi);
  return vec_stmt;
}


/* Function vect_transform_store.

   STMT defines a non scalar data-ref (array/pointer/structure). Create a 
   vectorized stmt to replace it, and insert it at BSI.  */

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
  /* APPLE LOCAL AV misaligned --haifa  */
  /* APPLE LOCAL MERGE HACK comment out because of unused warning */
  /* tree t;*/

  if (vect_debug_details (NULL))
    fprintf (dump_file, "transform store");
#ifdef ENABLE_CHECKING
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    abort ();
#endif

  /** Handle def.  **/
  scalar_dest = TREE_OPERAND (stmt, 0);

  /* APPLE LOCAL begin AV misaligned --haifa  */
  vect_align_data_ref (stmt);
  /* APPLE LOCAL remove params til we fix vect_create_data_ref. */
  /* data_ref = vect_create_data_ref (stmt, bsi, false, &t, false); */
  data_ref = vect_create_data_ref (stmt, bsi);
  /* APPLE LOCAL end AV misaligned --haifa  */

  /** Handle use - get the vectorized def from the defining stmt.  **/
  op = TREE_OPERAND (stmt, 1);
  vec_oprnd1 = vect_get_vec_def_for_operand (op, stmt);

  /** Arguments are ready. create the new vector stmt.  **/
  vec_stmt = build (MODIFY_EXPR, vectype, data_ref, vec_oprnd1);
  vect_finish_stmt_generation (stmt, vec_stmt, bsi);
  return vec_stmt;
}

/* APPLE LOCAL begin AV misaligned --haifa  */
static void
vect_finish_stmt_generation_in_preheader (tree vec_stmt,
					  struct loop *loop)
{
  edge pe;
  basic_block new_bb;

  if (vect_debug_details (NULL))
    {
      fprintf (dump_file, "add new stmt in preheader: ");
      print_generic_expr (dump_file, vec_stmt, TDF_SLIM);
    }

  pe = loop_preheader_edge (loop);
  new_bb = bsi_insert_on_edge_immediate (pe, vec_stmt);
#ifdef ENABLE_CHECKING
  if (new_bb)
    abort ();
#endif
}
/* APPLE LOCAL end AV misaligned --haifa  */


/* Function vect_transform_load.

   STMT is a load from memory. Create a vectorized stmt to replace it,
   and insert it at BSI.  */

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
  /* APPLE LOCAL AV misaligned --haifa  */
  tree ptr;

  if (vect_debug_details (NULL))
    fprintf (dump_file, "transform load.");
#ifdef ENABLE_CHECKING
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    abort ();
#endif

  /** Handle def.  **/
  scalar_dest = TREE_OPERAND (stmt, 0);
#ifdef ENABLE_CHECKING
  if (TREE_CODE (scalar_dest) != SSA_NAME)
    abort ();
#endif
  /* APPLE LOCAL begin AV misaligned --haifa  */
  /** Handle use.  **/

  op = TREE_OPERAND (stmt, 1);

  if (!aligned_access_p (STMT_VINFO_DATA_REF (stmt_info))
      && (!targetm.vect.support_misaligned_loads
          || !(*targetm.vect.support_misaligned_loads) ()))
    abort ();

  if (aligned_access_p (STMT_VINFO_DATA_REF (stmt_info)))
    vect_align_data_ref (stmt);

  if (!aligned_access_p (STMT_VINFO_DATA_REF (stmt_info))
      && targetm.vect.permute_misaligned_loads
      && (*targetm.vect.permute_misaligned_loads) ())
    {
      /* Create a series of:
         1. MSQ = vec_ld (0, target);    -- Most significant quadword.
                                         -- It is placed in the preheader.
         2. LSQ = vec_ld (15+i, target);     -- Least significant quadword.
         3. mask = vec_lvsr (0, 16-target);  -- Create the permute mask.
                                             -- It is placed in the preheader.
         4. return vec_perm (MSQ, LSQ, mask);  -- Align the data.
	 5. copy MSQ = LSQ;		       -- For next iteration.
      */
      tree lsq, mask, tmp, result, arg;
      tree lsq_data_ref;
      tree vec_ld_lsq_stmt;
      tree vec_lvsr_stmt;
      tree vec_perm_stmt;
      tree minus_stmt;
      tree phi_stmt, msq;
      tree V16QI_type_node;
      /* APPLE LOCAL MERGE FIXME comment out to avoid warning */
      /* tree lsq_ptr; */
      struct loop *loop = STMT_VINFO_LOOP (stmt_info);

      /* 1. Build the msq_load.  */
      /* APPLE LOCAL remove params til we fix vect_create_data_ref. */
      /*data_ref = vect_create_data_ref (stmt, bsi, false, &ptr, true);*/ ptr = NULL;

      data_ref = vect_create_data_ref (stmt, bsi);

      /*** create: msq = (vectype) *msq_data_ref; ***/
      vec_dest = vect_create_destination_var (scalar_dest, vectype);

      /** Arguments are ready. create the new vector stmt.  **/
      vec_stmt = build2 (MODIFY_EXPR, vectype, vec_dest, data_ref);
      new_temp = make_ssa_name (vec_dest, vec_stmt);
      TREE_OPERAND (vec_stmt, 0) = new_temp;

      vect_finish_stmt_generation_in_preheader (vec_stmt, loop);

      /* Create the ssa_name (msq) for the phi.  */
      vec_dest = vect_create_destination_var (scalar_dest, vectype);
      msq = make_ssa_name (vec_dest, NULL_TREE);

      /* 2. Build the lsq_load.  */
      /* APPLE LOCAL remove params til we fix vect_create_data_ref. */
      /* lsq_data_ref = vect_create_data_ref (stmt, bsi, true, &lsq_ptr,
	 false); */
      lsq_data_ref = vect_create_data_ref (stmt, bsi);

      /*** create: lsq = (vectype) *lsq_data_ref; ***/
      vec_dest = vect_create_destination_var (scalar_dest, vectype);
  
      vec_ld_lsq_stmt = build2 (MODIFY_EXPR, vectype, vec_dest, lsq_data_ref);
      lsq = make_ssa_name (vec_dest, vec_ld_lsq_stmt);
      TREE_OPERAND (vec_ld_lsq_stmt, 0) = lsq;

      vect_finish_stmt_generation (stmt, vec_ld_lsq_stmt, bsi);

      /* 3. Build the call to vec_lvsr.  */
      /*** create: ptr = 16 - ptr; ***/
      vec_dest = vect_create_destination_var (scalar_dest, TREE_TYPE (ptr));
  
      minus_stmt = build2 (MODIFY_EXPR, TREE_TYPE (ptr), vec_dest,
		    build2 (MINUS_EXPR, TREE_TYPE (ptr),
		           build_int_2 (16, 0), ptr));
      ptr = make_ssa_name (vec_dest, minus_stmt);
      TREE_OPERAND (minus_stmt, 0) = ptr;
      vect_finish_stmt_generation_in_preheader (minus_stmt, loop);


      V16QI_type_node = build_vector_type (intQI_type_node, 16);
      vec_dest = vect_create_destination_var (scalar_dest, V16QI_type_node);

      arg = tree_cons (NULL, ptr, NULL);
      arg = tree_cons (NULL, integer_zero_node, arg);
      if (!targetm.vect.build_builtin_lvsr)
	abort ();
      tmp = (*targetm.vect.build_builtin_lvsr) ();
      if (tmp == NULL_TREE)
	abort ();

      vec_lvsr_stmt = build_function_call_expr (tmp, arg);
      vec_lvsr_stmt = build2 (MODIFY_EXPR, vectype, vec_dest, vec_lvsr_stmt);
      mask = make_ssa_name (vec_dest, vec_lvsr_stmt);
      TREE_OPERAND (vec_lvsr_stmt, 0) = mask;

      vect_finish_stmt_generation_in_preheader (vec_lvsr_stmt, loop);

      /* 4. Build the call to vec_perm.  */
      vec_dest = vect_create_destination_var (scalar_dest, vectype);

      arg = tree_cons (NULL, mask, NULL);
      arg = tree_cons (NULL, lsq, arg);
      arg = tree_cons (NULL, msq, arg);
      if (!targetm.vect.build_builtin_vperm)
	abort ();
      tmp = (*targetm.vect.build_builtin_vperm) (TYPE_MODE (vectype));
      if (tmp == NULL_TREE)
	abort ();

      vec_perm_stmt = build_function_call_expr (tmp, arg);
      vec_perm_stmt = build2 (MODIFY_EXPR, vectype, vec_dest, vec_perm_stmt);
      result = make_ssa_name (vec_dest, vec_perm_stmt);
      TREE_OPERAND (vec_perm_stmt, 0) = result;
 
      vect_finish_stmt_generation (stmt, vec_perm_stmt, bsi);
 
      vec_stmt = vec_perm_stmt;

      /* 5. Copy LSQ over to MSQ for next iteration.  */
      phi_stmt = create_phi_node (msq, loop->header);
      SSA_NAME_DEF_STMT (msq) = phi_stmt;
      add_phi_arg (&phi_stmt, new_temp, loop_preheader_edge (loop));
      add_phi_arg (&phi_stmt, lsq, loop_latch_edge (loop));
    }
  else /* aligned_access_p (STMT_VINFO_DATA_REF (stmt_info))
          || ((*targetm.vect.support_misaligned_loads) ()
              && !(*targetm.vect.permute_misaligned_loads) ())
          In this case we generate a regular load.  */
    {
      vec_dest = vect_create_destination_var (scalar_dest, vectype);

      /* APPLE LOCAL remove params til we fix vect_create_data_ref. */
      /* data_ref = vect_create_data_ref (stmt, bsi, false, &ptr, false); */
      data_ref = vect_create_data_ref (stmt, bsi);

      /** Arguments are ready. create the new vector stmt.  **/
    
      vec_stmt = build2 (MODIFY_EXPR, vectype, vec_dest, data_ref);
      new_temp = make_ssa_name (vec_dest, vec_stmt);
      TREE_OPERAND (vec_stmt, 0) = new_temp;

      vect_finish_stmt_generation (stmt, vec_stmt, bsi);
    }

  /* APPLE LOCAL end AV misaligned --haifa  */
  return vec_stmt;
}


/* Function vect_transform_stmt.

   Create a vectorized stmt to replace STMT, and insert it at BSI.  */

static bool
vect_transform_stmt (tree stmt, block_stmt_iterator *bsi)
{
  bool is_store = false;
  tree vec_stmt = NULL;
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);

  switch (STMT_VINFO_TYPE (stmt_info))
    {
    case op_vec_info_type:
      vec_stmt = vect_transform_op (stmt, bsi);
      break;

    case assignment_vec_info_type:
      vec_stmt = vect_transform_assignment (stmt, bsi);
      break;

    case load_vec_info_type:
      vec_stmt = vect_transform_load (stmt, bsi);
      break;

    case store_vec_info_type:
      vec_stmt = vect_transform_store (stmt, bsi);
      is_store = true;
      break;

      /* APPLE LOCAL begin AV if-conversion --dpatel  */
    case select_vec_info_type:
      vec_stmt = vect_transform_select (stmt, bsi);
      break;

    case compare_vec_info_type:
      vec_stmt = vect_transform_compare (stmt, bsi);
      break;
      /* APPLE LOCAL end AV if-conversion --dpatel  */

    default:
      if (vect_debug_details (NULL))
        fprintf (dump_file, "stmt not supported.");
      abort ();
    }

  STMT_VINFO_VEC_STMT (stmt_info) = vec_stmt;

  return is_store;
}


/* This function generate the following statements:

 ni_name = number of iterations loop executes
 ratio = ni_name / vf
 ratio_mult_vf_name = ratio * vf

 and locate them at the loop preheader edge.  */

static void 
vect_generate_tmps_on_preheader (loop_vec_info loop_vinfo, tree *ni_name_p,
				 tree *ratio_mult_vf_name_p, tree *ratio_p)
{

  edge pe;
  basic_block new_bb;
  tree stmt, var, ni, ni_name;
  tree ratio;
  tree ratio_mult_vf_name, ratio_mult_vf;
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  
  int vf, i = -1;

  /* Generate temporary variable that contains 
     number of iterations loop executes.  */

  ni = LOOP_VINFO_SYMB_NUM_OF_ITERS(loop_vinfo);
  var = create_tmp_var (TREE_TYPE (ni), "niters");
  add_referenced_tmp_var (var);

  ni_name = force_gimple_operand (ni, &stmt, false, var);
  pe = loop_preheader_edge (loop);
  new_bb = bsi_insert_on_edge_immediate (pe, stmt);
  if (new_bb)
    add_bb_to_loop (new_bb, new_bb->pred->src->loop_father);
      
  /* ratio = ni / vf  */

  vf = LOOP_VINFO_VECT_FACTOR (loop_vinfo);
  ratio = vect_build_symbl_bound (ni_name, vf, loop);
       
  /* Update initial conditions of loop copy.  */
       
  /* ratio_mult_vf = ration * vf;  */

  /* vf is power of 2; thus if vf = 2^k, then n/vf = n >> k.   */
  while (vf)
    {
      vf = vf >> 1;
      i++;
    }

  ratio_mult_vf = create_tmp_var (TREE_TYPE (ni), "ratio_mult_vf");
  add_referenced_tmp_var (ratio_mult_vf);

  ratio_mult_vf_name = make_ssa_name (ratio_mult_vf, NULL_TREE);

  stmt = build (MODIFY_EXPR, void_type_node, ratio_mult_vf_name,
		build (LSHIFT_EXPR, TREE_TYPE (ratio),
		       ratio, build_int_2(i,0)));

  SSA_NAME_DEF_STMT (ratio_mult_vf_name) = stmt;

  pe = loop_preheader_edge (loop);
  new_bb = bsi_insert_on_edge_immediate (pe, stmt);
  if (new_bb)
    add_bb_to_loop (new_bb, new_bb->pred->src->loop_father);

  *ni_name_p = ni_name;
  *ratio_mult_vf_name_p = ratio_mult_vf_name;
  *ratio_p = ratio;
    
  return;  
}


/* This function:
        
        1. splits EE edge generating new bb
	2. locates the following statement as last statement of new bb:

	    if ( COND ) 
	      goto EXIT_BB.
	    else 
	      goto EE->dest (as it was before split).
	
	3. updates phis of EXIT_BB with 
	   values comming from false edge

   The function also updates phis of EXIT_BB. 
   
   We suppose that  E->dest == EXIT_BB and 
   that EE is preheader edge of loop.  */

static basic_block 
vect_gen_if_guard (edge ee, tree cond, basic_block exit_bb, edge e)
{
  tree cond_expr;
  tree then_clause,else_clause;
  basic_block new_bb; 
  edge true_edge, false_edge;
  tree phi, phi1;
  basic_block header_of_loop;
  int num_elem1, num_elem2;
  edge e0;

  block_stmt_iterator interm_bb_last_bsi;

  new_bb = vect_tree_split_edge (ee); 
  if(!new_bb)
    {
      if (vect_debug_details (NULL))
	fprintf (dump_file, "Failed to generate new_bb.");
      abort ();
    }
 
  header_of_loop = new_bb->succ->dest;

  then_clause = build1 (GOTO_EXPR, void_type_node, tree_block_label (exit_bb));
  else_clause = build1 (GOTO_EXPR, void_type_node, 
			tree_block_label (header_of_loop)); 
  cond_expr = build (COND_EXPR, void_type_node, cond, then_clause, else_clause);

  /* Insert condition as a last statement in new bb. */
  interm_bb_last_bsi = bsi_last (new_bb);
  bsi_insert_after (&interm_bb_last_bsi, cond_expr, BSI_NEW_STMT);   

  /* Remember old edge to update phis.  */
  e0 = new_bb->succ;

  /* Remove edge from new bb to header of loop.  */  
  remove_edge (e0); 

  /* Generate new edges according to condition.  */
  true_edge = make_edge (new_bb, exit_bb, EDGE_TRUE_VALUE);
  set_immediate_dominator (CDI_DOMINATORS, exit_bb, new_bb);
  false_edge = make_edge (new_bb, header_of_loop, EDGE_FALSE_VALUE);
  set_immediate_dominator (CDI_DOMINATORS, header_of_loop, new_bb);

  /* Update phis in loop header as coming from false edge.  */
  for (phi = phi_nodes (header_of_loop); phi; phi = TREE_CHAIN (phi))
    {
      int i;
      num_elem1 = PHI_NUM_ARGS (phi);
      for (i = 0; i < num_elem1; i++)
	if (PHI_ARG_EDGE (phi, i) == e0)
	  {
	    PHI_ARG_EDGE (phi, i) = false_edge;
	    break;
	  }
    }

  /* Update phis of exit bb as coming from true_edge.  */
  for (phi = phi_nodes (exit_bb); phi; phi = TREE_CHAIN (phi))
    {
      int i;
      num_elem1 = PHI_NUM_ARGS (phi);
      for (i = 0; i < num_elem1; i++)
	{
	  if (PHI_ARG_EDGE (phi, i) == e)
	    {
	      tree def = PHI_ARG_DEF (phi, i);
	      for (phi1 = phi_nodes (header_of_loop); phi1; 
		   phi1 = TREE_CHAIN (phi1))
		{
		  int k;
		  num_elem2 = PHI_NUM_ARGS (phi1);
		  for (k = 0; k < num_elem2; k++)
		    {
		      if (PHI_ARG_DEF (phi1, k) == def)
			{
			  int j;
			  for (j = 0; j < num_elem2; j++)
			    {
			      if (PHI_ARG_EDGE (phi1, j) == false_edge)
				{
				  tree def1 = PHI_ARG_DEF (phi1, j);
				  add_phi_arg (&phi, def1, true_edge);
				  break;
				}
			    }
			  break;
			}
		    }
		}		
	    }
	}
    }  

  return new_bb;
}


/* This function generates stmt 
   
   tmp = n / vf;

   and attaches it to preheader of LOOP.  */

static tree 
vect_build_symbl_bound (tree n, int vf, struct loop * loop)
{
  tree var, stmt, var_name;
  edge pe;
  basic_block new_bb;
  int i = -1;

  /* create temporary variable */
  var = create_tmp_var (TREE_TYPE (n), "bnd");
  add_referenced_tmp_var (var);

  var_name = make_ssa_name (var, NULL_TREE);

  /* vf is power of 2; thus if vf = 2^k, then n/vf = n >> k.   */
  while (vf)
    {
      vf = vf >> 1;
      i++;
    }

  stmt = build (MODIFY_EXPR, void_type_node, var_name,
		build (RSHIFT_EXPR, TREE_TYPE (n),
		       n, build_int_2(i,0)));

  SSA_NAME_DEF_STMT (var_name) = stmt;

  pe = loop_preheader_edge (loop);
  new_bb = bsi_insert_on_edge_immediate (pe, stmt);
  if (new_bb)
    add_bb_to_loop (new_bb, new_bb->pred->src->loop_father);
  else	
    if (vect_debug_details (NULL))
      fprintf (dump_file, "New bb on preheader edge was not generated.");

  return var_name;
}


/* This function updates the initial conditions of the loop copy (second loop).
 
   LOOP_VINFO is the vinfo of the loop to be vectorized.
   NITERS is a variable that contains number of iterations the loop executes 
   before vectorization.

   When loop is vectorized, its IVs not always preserved so 
   that to be used for initialization of loop copy (second loop). 
   Here we use access functions of IVs and number of iteration 
   loop executes in order to bring IVs to correct position.  

   Function also update phis of epilog loop header and NEW_LOOP_EXIT->dest.  */

static void 
vect_update_initial_conditions_of_duplicated_loop (loop_vec_info loop_vinfo, 
						   tree niters, 
						   basic_block new_loop_header,
						   edge new_loop_latch, 
						   edge new_loop_exit)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo); 
  /* Preheader edge of duplicated loop.  */
  edge pe;
  edge latch = loop_latch_edge (loop);
  tree phi;
  block_stmt_iterator interm_bb_last_bsi;
  basic_block intermediate_bb = loop->exit_edges[0]->dest;
  edge inter_bb_true_edge;
  basic_block exit_bb;

  /* Find edge from intermediate bb to new loop header.  */
  pe = find_edge (loop->exit_edges[0]->dest, new_loop_header);
  inter_bb_true_edge = find_edge (intermediate_bb, new_loop_exit->dest);
  exit_bb = new_loop_exit->dest;
  
  for (phi = phi_nodes (loop->header); phi; phi = TREE_CHAIN (phi))
    {
      tree access_fn = NULL;
      tree evolution_part;
      tree init_expr;
      tree step_expr;
      tree var, stmt, ni, ni_name;
      int i, j, k, m, num_elem1, num_elem2, num_elem3;
      tree phi1, phi2;


      /* Skip virtual phi's. The data dependences that are associated with
         virtual defs/uses (i.e., memory accesses) are analyzed elsewhere.  */

      if (!is_gimple_reg (SSA_NAME_VAR (PHI_RESULT (phi))))
	{
	  if (vect_debug_details (NULL))
	    fprintf (dump_file, "virtual phi. skip.");
	  continue;
	}

      access_fn = instantiate_parameters
	(loop,
	 analyze_scalar_evolution (loop, PHI_RESULT (phi)));

      evolution_part = evolution_part_in_loop_num (access_fn, loop->num);
      
      /* FORNOW: We do not transform initial conditions of IVs 
	 which evolution functions are a polynomial of degree >= 2 or
	 exponential.  */

      step_expr = evolution_part;
      init_expr = initial_condition (access_fn);

      ni = build (PLUS_EXPR, TREE_TYPE (init_expr),
		  build (MULT_EXPR, TREE_TYPE (niters),
		       niters, step_expr), init_expr);

      var = create_tmp_var (TREE_TYPE (init_expr), "tmp");
      add_referenced_tmp_var (var);

      ni_name = force_gimple_operand (ni, &stmt, false, var);
      
      /* Insert stmt into intermediate bb before condition.  */
      interm_bb_last_bsi = bsi_last (intermediate_bb);
      bsi_insert_before (&interm_bb_last_bsi, stmt, BSI_NEW_STMT);   

      /* Fix phi expressions in duplicated loop.   */
      num_elem1 = PHI_NUM_ARGS (phi);
      for (i = 0; i < num_elem1; i++)
	if (PHI_ARG_EDGE (phi, i) == latch)
	  {
	    tree def;
	    
	    def = PHI_ARG_DEF (phi, i);
	    for (phi1 = phi_nodes (new_loop_header); phi1; 
		 phi1 = TREE_CHAIN (phi1))
	      {
		num_elem2 = PHI_NUM_ARGS (phi1);
		for (j = 0; j < num_elem2; j++)
		  if (PHI_ARG_DEF (phi1, i) == def)
		    {
		      for (k = 0; k < num_elem2; k++)
			if (PHI_ARG_EDGE (phi1, k) == new_loop_latch)
			  {
			    tree def1 = PHI_ARG_DEF (phi1, k);
			    for (phi2 = phi_nodes (exit_bb); phi2; 
				 phi2 = TREE_CHAIN (phi2))
			      {
				num_elem3 = PHI_NUM_ARGS (phi2);
				for (m = 0; m < num_elem3; m++)
				  {
				    if (PHI_ARG_DEF (phi2, m) == def1 && 
					PHI_ARG_EDGE (phi2, m) == new_loop_exit)
				      {
					add_phi_arg (&phi2, ni_name, 
							inter_bb_true_edge);
					break;
				      }
				  }
			      }
			  }

		      SET_PHI_ARG_DEF (phi1, j, ni_name);
		      PHI_ARG_EDGE (phi1, j) = pe;
		      
		      break;
 		    }		    
	      }
	    break;
	  }
    }
}


/* Split edge EDGE_IN.  Return the new block.
   Abort on abnormal edges.  */

static basic_block
vect_tree_split_edge (edge edge_in)
{
  basic_block new_bb, dest, src;
  edge new_edge;
  tree phi;
  int i, num_elem;

  /* Abnormal edges cannot be split.  */
  if (edge_in->flags & EDGE_ABNORMAL)
    abort ();

  src = edge_in->src;
  dest = edge_in->dest;

  new_bb = create_empty_bb (src);
  new_edge = make_edge (new_bb, dest, EDGE_FALLTHRU);

  /* Find all the PHI arguments on the original edge, and change them to
     the new edge.  Do it before redirection, so that the argument does not
     get removed.  */
  for (phi = phi_nodes (dest); phi; phi = TREE_CHAIN (phi))
    {
      num_elem = PHI_NUM_ARGS (phi);
      for (i = 0; i < num_elem; i++)
	if (PHI_ARG_EDGE (phi, i) == edge_in)
	  {
	    PHI_ARG_EDGE (phi, i) = new_edge;
	    break;
	  }
    }

  if (!redirect_edge_and_branch (edge_in, new_bb))
    abort ();

  set_immediate_dominator (CDI_DOMINATORS, new_bb, src);
  set_immediate_dominator (CDI_DOMINATORS, dest, new_bb);
  
  new_bb->loop_father = src->loop_father->outer;

  if (PENDING_STMT (edge_in))
    abort ();

  return new_bb;
}


/* Function vect_transform_loop_bound.

   Create a new exit condition for the loop.  */

static void
vect_transform_loop_bound (loop_vec_info loop_vinfo, tree niters)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  edge exit_edge = loop->exit_edges[0];
  block_stmt_iterator loop_exit_bsi = bsi_last (exit_edge->src);
  tree indx_before_incr, indx_after_incr;
  tree orig_cond_expr;
  int old_N = 0, vf;
  tree cond_stmt;
  tree new_loop_bound;
  bool symbl_niters;
  tree cond;

  if (LOOP_VINFO_NITERS_KNOWN_P (loop_vinfo))
    symbl_niters = false;
  else if (LOOP_VINFO_SYMB_NUM_OF_ITERS (loop_vinfo) != NULL)
    symbl_niters = true;
  else  
    abort ();

  if (!symbl_niters)
    old_N = LOOP_VINFO_NITERS (loop_vinfo);

  vf = LOOP_VINFO_VECT_FACTOR (loop_vinfo);

  /* FORNOW:
     assuming number-of-iterations divides by the vectorization factor.  */
  if (!symbl_niters && old_N % vf)
    abort ();

  orig_cond_expr = LOOP_VINFO_EXIT_COND (loop_vinfo);
  if (!orig_cond_expr)
    abort ();
  if (orig_cond_expr != bsi_stmt (loop_exit_bsi))
    abort ();

  create_iv (integer_zero_node, integer_one_node, NULL_TREE, loop, 
	&loop_exit_bsi, false, &indx_before_incr, &indx_after_incr);

  /* bsi_insert is using BSI_NEW_STMT. We need to bump it back 
     to point to the exit condition. */
  bsi_next (&loop_exit_bsi);
  if (bsi_stmt (loop_exit_bsi) != orig_cond_expr)
    abort ();

  /* new loop exit test:  */
  if(!symbl_niters)
    new_loop_bound = build_int_2 (old_N/vf, 0);
  else
    new_loop_bound = niters;

  if (exit_edge->flags & EDGE_TRUE_VALUE) /* 'then' edge exits the loop.  */
    cond = build (GE_EXPR, boolean_type_node, indx_after_incr, new_loop_bound);
  else /* 'then' edge loops back.   */
    cond = build (LT_EXPR, boolean_type_node, indx_after_incr, new_loop_bound);

  cond_stmt = build (COND_EXPR, TREE_TYPE (orig_cond_expr), cond,
	TREE_OPERAND (orig_cond_expr, 1), TREE_OPERAND (orig_cond_expr, 2));

  bsi_insert_before (&loop_exit_bsi, cond_stmt, BSI_SAME_STMT);   

  /* remove old loop exit test:  */
  bsi_remove (&loop_exit_bsi);

  if (vect_debug_details (NULL))
    print_generic_expr (dump_file, cond_stmt, TDF_SLIM);
}


/* Function vect_transform_loop.

   The analysis phase has determined that the loop is vectorizable.
   Vectorize the loop - created vectorized stmts to replace the scalar
   stmts in the loop, and update the loop exit condition.  */

static void
vect_transform_loop (loop_vec_info loop_vinfo, struct loops *loops)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  basic_block *bbs = LOOP_VINFO_BBS (loop_vinfo);
  int nbbs = loop->num_nodes;
  block_stmt_iterator si;
  int i;
  tree ratio = NULL;
#ifdef ENABLE_CHECKING
  int vectorization_factor = LOOP_VINFO_VECT_FACTOR (loop_vinfo);
#endif
  
  if (vect_debug_details (NULL))
    fprintf (dump_file, "\n<<vec_transform_loop>>\n");

  /* If the loop has a symbolic number of iterations 'n' 
     (i.e. it's not a compile time constant), 
     then an epilog loop needs to be created. We therefore duplicate 
     the initial loop. The original loop will be vectorized, and will compute
     the first (n/VF) iterations. The second copy of the loop will remain 
     serial and will compute the remaining (n%VF) iterations. 
     (VF is the vectorization factor).  */

  if (LOOP_VINFO_NITERS (loop_vinfo) == -1
      && LOOP_VINFO_SYMB_NUM_OF_ITERS (loop_vinfo) != NULL )
    {
      basic_block inter_bb, exit_bb, prolog_bb;
      tree ni_name, ratio_mult_vf_name;
      basic_block new_loop_header;
      tree cond;
      int vf;
      edge e, exit_ep, phead_epilog, ee;
      struct loop *outer_loop = 
	LOOP_VINFO_LOOP (loop_vinfo)->pre_header->loop_father;

      /* Remember exit bb before duplication.  */
      exit_bb = loop->exit_edges[0]->dest;

      /* Duplicate loop. 
	 New (epilog) loop is concatenated to the exit of original loop.  */
      tree_duplicate_loop_to_exit (loop, loops);

      new_loop_header = loop->exit_edges[0]->dest;
      
      /* Generate the following variables on the preheader of original loop:
	 
	 ni_name = number of iteration the original loop executes
	 ratio = ni_name / vf
	 ration_mult_vf_name = ration * vf
      */
      vect_generate_tmps_on_preheader (loop_vinfo, &ni_name,
				       &ratio_mult_vf_name, &ratio);

      /* Update loop info.  */
      loop->pre_header = loop_preheader_edge (loop)->src;
      loop->pre_header_edges[0] = loop_preheader_edge (loop);

      /* Build conditional expr before epilog loop.  */
      cond = build (EQ_EXPR, boolean_type_node, ratio_mult_vf_name, ni_name);

      /* Find exit edge of epilog loop.  */
      exit_ep = find_edge (new_loop_header, exit_bb);

      /* Generate intermediate bb between original loop and epilog loop 
	 with guard if statement: 
	 
	 if ( ni_name == ratio_mult_vf_name ) skip epilog loop.  */
      inter_bb = vect_gen_if_guard 
	(loop->exit_edges[0], cond, exit_bb, exit_ep);
      add_bb_to_loop (inter_bb, outer_loop);

      loop->exit_edges[0] = inter_bb->pred;

      /* Build conditional expr before loop to be vectorized.  */
      vf = LOOP_VINFO_VECT_FACTOR (loop_vinfo);
      cond = build (LT_EXPR, boolean_type_node, ni_name, build_int_2 (vf,0));

      /* Find preheader edge of epilog loop.  */
      phead_epilog = find_edge (inter_bb, new_loop_header);


      /* Generate new bb before original loop  
	 with guard if statement: 
	 
	 if ( ni_name < vf) goto epilog loop.  */
      prolog_bb = vect_gen_if_guard (loop->pre_header_edges[0], cond, 
				     new_loop_header, phead_epilog);

      loop->pre_header = prolog_bb;
      add_bb_to_loop (prolog_bb, outer_loop);
      
      /* Find loop preheader edge of original loop.  */
      loop->pre_header_edges[0] = find_edge (prolog_bb, loop->header);


      /* Find true edge of first if stmt.  */
      for (ee = prolog_bb->succ; ee; ee = ee->succ_next)
	if(ee->dest != loop->header)
	  break;

      if (!ee)
	abort ();

      /* Find new loop latch edge. */      
      for (e = new_loop_header->pred; e; e = e->pred_next)
	if(e != ee && e != phead_epilog)
	  break;

      if (!e)
	abort ();

      /* Update IVs of original loop as if they were advanced 
	 by ratio_mult_vf_name steps. Locate them in intermediate 
	 bb before if stmt.  */
      vect_update_initial_conditions_of_duplicated_loop (loop_vinfo, 
	ratio_mult_vf_name, new_loop_header, e, exit_ep);
    }


  /* 1) Make sure the loop header has exactly two entries
     2) Make sure we have a preheader basic block.  */

  if (!loop->header->pred->pred_next
      || loop->header->pred->pred_next->pred_next)
    abort ();

  loop_split_edge_with (loop_preheader_edge (loop), NULL);


  /* FORNOW: the vectorizer supports only loops which body consist
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
	  bool is_store;
#ifdef ENABLE_CHECKING
	  tree vectype;
#endif

	  if (vect_debug_details (NULL))
	    {
	      fprintf (dump_file, "------>vectorizing statement: ");
	      print_generic_expr (dump_file, stmt, TDF_SLIM);
	    }	
	  stmt_info = vinfo_for_stmt (stmt);
#ifdef ENABLE_CHECKING
	  if (!stmt_info)
	    abort ();
#endif
	  if (!STMT_VINFO_RELEVANT_P (stmt_info))
	    {
	      bsi_next (&si);
	      continue;
	    }
#ifdef ENABLE_CHECKING
	  /* FORNOW: Verify that all stmts operate on the same number of
	             units and no inner unrolling is necessary.  */
	  vectype = STMT_VINFO_VECTYPE (stmt_info);
	  if (GET_MODE_NUNITS (TYPE_MODE (vectype)) != vectorization_factor)
	    abort ();
#endif
	  /* -------- vectorize statement ------------ */
	  if (vect_debug_details (NULL))
	    fprintf (dump_file, "transform statement.");

	  is_store = vect_transform_stmt (stmt, &si);
	  if (is_store)
	    {
	      /* free the attached stmt_vec_info and remove the stmt.  */
	      stmt_ann_t ann = stmt_ann (stmt);
	      free (stmt_info);
	      set_stmt_info (ann, NULL);
	      bsi_remove (&si);
	      continue;
	    }

	  bsi_next (&si);
	}		        /* stmts in BB */
    }				/* BBs in loop */

  vect_transform_loop_bound (loop_vinfo, ratio);

  if (vect_debug_details (loop))
    fprintf (dump_file,"Success! loop vectorized.");
  if (vect_debug_stats (loop))
    fprintf (dump_file, "LOOP VECTORIZED.");
}


/* Function vect_is_simple_use.

   Input:
   LOOP - the loop that is being vectorized.
   OPERAND - operand of a stmt in LOOP.
   DEF - the defining stmt in case OPERAND is an SSA_NAME.

   Returns whether a stmt with OPERAND can be vectorized.
   Supportable operands are constants, loop invariants, and operands that are
   defined by the current iteration of the loop. Unsupportable opernads are 
   those that are defined by a previous iteration of the loop (as is the case
   in reduction/induction computations).  */

static bool
vect_is_simple_use (tree operand, struct loop *loop, tree *def)
{ 
  tree def_stmt;
  basic_block bb;

  if (def)
    *def = NULL_TREE;

  if (TREE_CODE (operand) == INTEGER_CST || TREE_CODE (operand) == REAL_CST)
    return true;

  if (TREE_CODE (operand) != SSA_NAME)
    return false;

  def_stmt = SSA_NAME_DEF_STMT (operand);
  if (def_stmt == NULL_TREE )
    {
      if (vect_debug_details (NULL))
        fprintf (dump_file, "no def_stmt.");
      return false;
    }

  /* nop_expr is expected only in case of a function argument.
     (Otherwise - we expect a phi_node or a modify_expr).  */
  if (TREE_CODE (def_stmt) == NOP_EXPR)
    {
      tree arg = TREE_OPERAND (def_stmt, 0);
      if (TREE_CODE (arg) == INTEGER_CST || TREE_CODE (arg) == REAL_CST)
	return true;
      if (vect_debug_details (NULL))
	{
	  fprintf (dump_file, "Unexpected form of NOP_EXPR: ");
	  print_generic_expr (dump_file, def_stmt, TDF_SLIM);
	}
      return false;  
    }

  /* phi_node inside the loop indicates an induction/reduction pattern.
     This is not supported yet.  */
  bb = bb_for_stmt (def_stmt);
  if (TREE_CODE (def_stmt) == PHI_NODE && flow_bb_inside_loop_p (loop, bb))
    {
      if (vect_debug_details (NULL))
	fprintf (dump_file, "reduction/induction - unsupported.");
      return false; /* FORNOW: not supported yet.  */
    }

  /* Expecting a modify_expr or a phi_node.  */
  if (TREE_CODE (def_stmt) == MODIFY_EXPR
      || TREE_CODE (def_stmt) == PHI_NODE)
    {
      if (def)
        *def = def_stmt; 	
      return true;
    }

  return false;
}


/* Function vect_is_supportable_op.

   Check is STMT performs an unary/binary operation that can be vectorized.  */

static bool
vect_is_supportable_op (tree stmt)
{
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree operation;
  /* APPLE LOCAL AV if-conversion --dpatel  */
  /* Remove local variables: code, op, vec_mode, optab.  */
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);
  struct loop *loop = STMT_VINFO_LOOP (stmt_info);
  /* APPLE LOCAL AV if-conversion --dpatel  */
  /* Remove local variables: i, op_type.  */
  /* APPLE LOCAL AV vmul_uch --haifa  */
  enum tree_code code;

  /* Is STMT a binary/unary operation?  */

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return false;

  if (TREE_CODE (TREE_OPERAND (stmt, 0)) != SSA_NAME)
    return false;

  operation = TREE_OPERAND (stmt, 1);
  /* APPLE LOCAL begin AV if-conversion --dpatel  */
  /* This patch breaks this function into two function.
     So APPLE LOCAL markers are OK.  */

  /* APPLE LOCAL begin AV vmul_uch --haifa  */
  code = TREE_CODE (operation);
  switch (code)
    {
    case MULT_UCH_EXPR:
      /* Todo: replace target hook with optab.  */
      if (!targetm.vect.support_vmul_uch_p ())
        return false;
      STMT_VINFO_VEC_HOOK (vinfo_for_stmt (stmt)) =
        targetm.vect.build_vmul_uch;
      STMT_VINFO_TYPE (stmt_info) = op_vec_info_type;
      return true;

    default:
      if (vect_is_supportable_operation (operation, vectype, loop))
        {
          /* FORNOW: Not considering the cost.  */
          STMT_VINFO_TYPE (stmt_info) = op_vec_info_type;
          return true;
        }
      else
        return false;
    }
  /* APPLE LOCAL end AV vmul_uch --haifa  */
}


/* Function vect_is_supportable_operation.

    Verify that STMT performs an operation that can be vectorized.  */

static bool
vect_is_supportable_operation (tree operation, tree vectype, struct loop *loop)
{
  enum tree_code code;
  tree op;
  enum machine_mode vec_mode;
  optab optab;
  int i, op_type;
  /* APPLE LOCAL end AV if-conversion --dpatel  */ 

  /* Is STMT a binary/unary operation?  */

  code = TREE_CODE (operation);

  switch (code)
    {
    case PLUS_EXPR:
      optab = add_optab;
      break;
    case MULT_EXPR:
      optab = smul_optab;
      break;
    case MINUS_EXPR:
      optab = sub_optab;
      break;
    case BIT_AND_EXPR:
      optab = and_optab;
      break;
    case BIT_XOR_EXPR:
      optab = xor_optab;
      break;
    case BIT_IOR_EXPR:
      optab = ior_optab;
      break;
    case BIT_NOT_EXPR:
      optab = one_cmpl_optab;
      break;
    default:
      return false;
    }
  
  /* Support only unary or binary operations.  */

  op_type = tree_nargs[code];
  if (op_type != unary_op && op_type != binary_op)
    {
      if (vect_debug_details (NULL))
	fprintf (dump_file, "num. args = %d (not unary/binary op).", op_type);
      return false;
    }
  
  for (i = 0; i < op_type; i++)
    {
      op = TREE_OPERAND (operation, i);
      if (!vect_is_simple_use (op, loop, NULL))
	{
	  if (vect_debug_details (NULL))
	    fprintf (dump_file, "use not simple.");
	  return false;
	}	
    } 

  /* Supportable by target?  */

  if (!optab)
    {
      if (vect_debug_details (NULL))
	fprintf (dump_file, "no optab.");
      return false;
    }

  vec_mode = TYPE_MODE (vectype);

  if (optab->handlers[(int) vec_mode].insn_code == CODE_FOR_nothing)
    {
      if (vect_debug_details (NULL))
	fprintf (dump_file, "op not supported by target.");
      return false;
    }

  return true;
}


/* Function vect_is_supportable_store.

   Check if STMT performs a "store to memory" operation - i.e, writes into a
   non scalar data-ref, and can be vectorized.  */

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

  if (TREE_CODE (scalar_dest) != ARRAY_REF
      && TREE_CODE (scalar_dest) != INDIRECT_REF)
    return false;

  op = TREE_OPERAND (stmt, 1);

  if (!vect_is_simple_use (op, loop, NULL))
    {
      if (vect_debug_details (NULL))
        fprintf (dump_file, "use not simple.");
      return false;
    }

  if (!STMT_VINFO_DATA_REF (stmt_info))
    return false;

  STMT_VINFO_TYPE (stmt_info) = store_vec_info_type;

  return true;
}


/* Function vect_is_supportable_load.

   Check if STMT performs a "load from memory" operation - i.e, reads from a
   non scalar data-ref, and can be vectorized.  */

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

  if (TREE_CODE (op) != ARRAY_REF && TREE_CODE (op) != INDIRECT_REF)
    return false;

  if (!STMT_VINFO_DATA_REF (stmt_info))
    return false;

  STMT_VINFO_TYPE (stmt_info) = load_vec_info_type;

  return true;
}


/* Function vect_is_supportable_assignment.

   Check is STMT performs an assignment (copy), and can be vectorized.  */

static bool
vect_is_supportable_assignment (tree stmt)
{
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree scalar_dest;
  tree op;
  struct loop *loop = STMT_VINFO_LOOP (stmt_info);

  /* Is vectorizable assignment? */

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return false;

  scalar_dest = TREE_OPERAND (stmt, 0);
  if (TREE_CODE (scalar_dest) != SSA_NAME)
    return false;

  op = TREE_OPERAND (stmt, 1);

  if (!vect_is_simple_use (op, loop, NULL))
    {
      if (vect_debug_details (NULL))
        fprintf (dump_file, "use not simple.");
      return false;
    }

  STMT_VINFO_TYPE (stmt_info) = assignment_vec_info_type;

  return true;
}

/* APPLE LOCAL begin AV if-conversion --dpatel  */
/* Function vect_is_supportable_compare.

   Verify that STMT performs comparision, and can be vectorized.  */

static bool
vect_is_supportable_compare (tree stmt)
{
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree op, op0, op1;
  tree vectype0;
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);
  struct loop *loop = STMT_VINFO_LOOP (stmt_info);

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return false;

  op = TREE_OPERAND (stmt, 1);

  if (TREE_CODE_CLASS (TREE_CODE (op)) != '<')
    return false;

  op0 = TREE_OPERAND (op, 0);
  op1 = TREE_OPERAND (op, 1);
  vectype0 = get_vectype_for_scalar_type (TREE_TYPE (op0));

  if (!vect_is_simple_use (op0, loop, NULL))
    {
      if (vect_debug_details (NULL))
        fprintf (dump_file, "use not simple.");
      return false;
    }

  if (!vect_is_simple_use (op1, loop, NULL))
    {
      if (vect_debug_details (NULL))
        fprintf (dump_file, "use not simple.");
      return false;
    }

  if (!targetm.vect.support_vector_compare_for_p (vectype, vectype0, TREE_CODE (op)))
    {
      if (vect_debug_details (NULL))
        fprintf (dump_file, "target does not support vector compare.");
      return false;
    }
  STMT_VINFO_TYPE (stmt_info) = compare_vec_info_type;
  return true;
}

/* Function vect_is_supportable_select.

   Verify that STMT performs a selection, and can be vectorized.  */

static bool
vect_is_supportable_select (tree stmt)
{
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  tree op, op0, op1, op2;
  tree vectype = STMT_VINFO_VECTYPE (stmt_info);
  struct loop *loop = STMT_VINFO_LOOP (stmt_info);

  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return false;

  op = TREE_OPERAND (stmt, 1);

  if (TREE_CODE (op) != COND_EXPR)
    return false;

  op0 = TREE_OPERAND (op, 0);  /* Condition */
  op1 = TREE_OPERAND (op, 1);
  op2 = TREE_OPERAND (op, 2);

  if (!vect_is_simple_use (op0, loop, NULL))
    {
      if (vect_debug_details (NULL))
        fprintf (dump_file, "use not simple.");
      return false;
    }

  if (TREE_CODE (op1) == SSA_NAME)
    {
      if (!vect_is_simple_use (op1, loop, NULL))
        {
          if (vect_debug_details (NULL))
            fprintf (dump_file, "use not simple.");
          return false;
        }
    }
  else if ( TREE_CODE (op1) != INTEGER_CST
            && TREE_CODE (op1) != REAL_CST
	    && TREE_CODE (op1) != NOP_EXPR 
            && !vect_is_supportable_operation (op1, vectype, loop))
    {
      if (vect_debug_details (NULL))
        fprintf (dump_file, "use not simple.");
      return false;
    }

  if (op2)
    {
      if (TREE_CODE (op2) == SSA_NAME)
	{
	  if (!vect_is_simple_use (op2, loop, NULL))
	    {
	      if (vect_debug_details (NULL))
		fprintf (dump_file, "use not simple.");
	      return false;
	    }
	}
      else if (TREE_CODE (op2) != NOP_EXPR 
	       && TREE_CODE (op2) != INTEGER_CST
	       && TREE_CODE (op2) != REAL_CST
	       && !vect_is_supportable_operation (op2, vectype, loop))
	{
	  if (vect_debug_details (NULL))
	    fprintf (dump_file, "use not simple.");
	  return false;
	}
    }

  STMT_VINFO_TYPE (stmt_info) = select_vec_info_type;
  return true;
}

/* APPLE LOCAL end AV if-conversion --dpatel  */

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

  if (vect_debug_details (NULL))
    fprintf (dump_file, "\n<<vect_analyze_operations>>\n");

  for (i = 0; i < nbbs; i++)
    {
      basic_block bb = bbs[i];

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  tree stmt = bsi_stmt (si);
	  int nunits;
	  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
	  tree vectype;

	  if (vect_debug_details (NULL))
	    {
	      fprintf (dump_file, "==> examining statement: ");
	      print_generic_expr (dump_file, stmt, TDF_SLIM);
	    }
#ifdef ENABLE_CHECKING
	  if (!stmt_info)
	    abort ();
#endif
	  /* skip stmts which do not need to be vectorized.
	     this is expected to include:
	     - the COND_EXPR which is the loop exit condition
	     - any LABEL_EXPRs in the loop
	     - computations that are used only for array indexing or loop
	     control  */

	  if (!STMT_VINFO_RELEVANT_P (stmt_info))
	    {
	      if (vect_debug_details (NULL))
	        fprintf (dump_file, "irrelevant.");
	      continue;
	    }

	  if (VECTOR_MODE_P (TYPE_MODE (TREE_TYPE (stmt))))
	    {
	      if (vect_debug_stats (loop) || vect_debug_details (loop))
		{
                  fprintf (dump_file, "not vectorized: vector stmt in loop:");
		  print_generic_expr (dump_file, stmt, TDF_SLIM);
		}
	      return false;
	    }

          if (STMT_VINFO_DATA_REF (stmt_info))
            scalar_type = TREE_TYPE (DR_REF (STMT_VINFO_DATA_REF (stmt_info)));    
          else if (TREE_CODE (stmt) == MODIFY_EXPR)
	    scalar_type = TREE_TYPE (TREE_OPERAND (stmt, 0));
	  else
	    scalar_type = TREE_TYPE (stmt);

	  if (vect_debug_details (NULL))
	    {
	      fprintf (dump_file, "get vectype for scalar type:  ");
	      print_generic_expr (dump_file, scalar_type, TDF_SLIM);
	    }

	  vectype = get_vectype_for_scalar_type (scalar_type);
	  if (!vectype)
	    {
	      if (vect_debug_stats (loop) || vect_debug_details (loop))
		{
                  fprintf (dump_file, "not vectorized: unsupported data-type ");
		  print_generic_expr (dump_file, scalar_type, TDF_SLIM);
		}
	      return false;
	    }
	  STMT_VINFO_VECTYPE (stmt_info) = vectype;

	  ok = (vect_is_supportable_op (stmt)
		|| vect_is_supportable_assignment (stmt)
		|| vect_is_supportable_load (stmt)
		/* APPLE LOCAL begin AV if-conversion --dpatel  */
 		|| vect_is_supportable_store (stmt)
 		|| vect_is_supportable_select (stmt)
 		|| vect_is_supportable_compare (stmt));
	        /* APPLE LOCAL end AV if-conversion --dpatel  */

	  if (!ok)
	    {
	      if (vect_debug_stats (loop) || vect_debug_details (loop))
		{
                  fprintf (dump_file, "not vectorized: stmt not supported: ");
		  print_generic_expr (dump_file, stmt, TDF_SLIM);
		}
	      return false;
	    }

	  nunits = GET_MODE_NUNITS (TYPE_MODE (vectype));
	  if (vect_debug_details (NULL))
	    fprintf (dump_file, "nunits = %d", nunits);

	  if (vectorization_factor)
	    {
	      /* FORNOW: don't allow mixed units.
	         This restriction will be relaxed in the future.  */
	      if (nunits != vectorization_factor)
		{
	          if (vect_debug_stats (loop) || vect_debug_details (loop))
		    fprintf (dump_file, "not vectorized: mixed data-types");
		  return false;
		}
	    }
	  else
	    vectorization_factor = nunits;
	}
    }

  /* TODO: Analyze cost. Decide if worth while to vectorize.  */

  LOOP_VINFO_VECT_FACTOR (loop_vinfo) = vectorization_factor;

  /* FORNOW: handle only cases where the loop bound divides by the
     vectorization factor.  */

  if (vect_debug_details (NULL))
    fprintf (dump_file, "vectorization_factor = %d, niters = %d",
	vectorization_factor, LOOP_VINFO_NITERS (loop_vinfo));

  if (vectorization_factor == 0
      || (!LOOP_VINFO_NITERS_KNOWN_P (loop_vinfo) 
          && !LOOP_VINFO_SYMB_NUM_OF_ITERS(loop_vinfo)))
    {
      if (vect_debug_stats (loop) || vect_debug_details (loop))
	fprintf (dump_file, "not vectorized: Complicated loop bound.");
      return false;
    }

  if (!LOOP_VINFO_NITERS_KNOWN_P (loop_vinfo)) /* Disabled temporarily */
    {
      if (vect_debug_stats (loop) || vect_debug_details (loop))
        fprintf (dump_file, "not vectorized: unknown loop bound.");
      return false;
    }

  if (LOOP_VINFO_NITERS_KNOWN_P (loop_vinfo) 
      && LOOP_VINFO_NITERS (loop_vinfo) % vectorization_factor != 0)
    {
      if (vect_debug_stats (loop) || vect_debug_details (loop))
        fprintf (dump_file, "not vectorized: loop bound doesn't divided by %d.",
		 vectorization_factor);
      return false;
    }

  return true;
}


/* Function exist_non_indexing_operands_for_use_p 

   USE is one of the uses attached to STMT. Check if USE is 
   used in STMT for anything other than indexing an array.  */

static bool
exist_non_indexing_operands_for_use_p (tree use, tree stmt)
{
  tree operand;
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
 
  /* USE corresponds to some operand in STMT. If there is no data
     reference in STMT, then any operand that corresponds to USE
     is not indexing an array.  */
  if (!STMT_VINFO_DATA_REF (stmt_info))
    return true;
 
  /* STMT has a data_ref. FORNOW this means that its of one of
     the following forms:
     -1- ARRAY_REF = var
     -2- var = ARRAY_REF
     (This should have been verified in analyze_data_refs).

     'var' in the second case corresponds to a def, not a use,
     so USE cannot correspond to any operands that are not used 
     for array indexing.

     Therefore, all we need to check is if STMT falls into the
     first case, and whether var corresponds to USE.  */
 
  if (TREE_CODE (TREE_OPERAND (stmt, 0)) == SSA_NAME)
    return false;

  operand = TREE_OPERAND (stmt, 1);

  if (TREE_CODE (operand) != SSA_NAME)
    return false;

  /* APPLE LOCAL begin AV if-conversion -dpatel  */
  if (TREE_CODE (operand) == SSA_NAME && operand == use)
    return true;

  if (TREE_CODE (operand) == COND_EXPR)
    {
      int i;
      for (i = 0; i < 3; i++)
	if (TREE_CODE (TREE_OPERAND (operand, i)) == SSA_NAME
	    && TREE_OPERAND (operand, i) == use)
	  return true;
    }
  /* APPLE LOCAL end AV if-conversion -dpatel  */
  return false;
}


/* Function vect_is_simple_iv_evolution.

   FORNOW: A simple evolution of an induction variables in the loop is
   considered a polynomial evolution with constant step.  */

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
  
  /* When the evolution is a polynomial of degree >= 2
     the evolution function is not "simple".  */
  if (tree_is_chrec (evolution_part))
    return false;
  
  step_expr = evolution_part;
  init_expr = initial_condition (access_fn);

  if (vect_debug_details (NULL))
    {
      fprintf (dump_file, "step: ");
      print_generic_expr (dump_file, step_expr, TDF_SLIM);
      fprintf (dump_file, ",  init: ");
      print_generic_expr (dump_file, init_expr, TDF_SLIM);
    }

  *init = init_expr;
  *step = step_expr;

  if (TREE_CODE (step_expr) != INTEGER_CST)
    {
      if (vect_debug_details (NULL))
        fprintf (dump_file, "step unknown.");
      return false;
    }

  if (strict)
    if (!integer_onep (step_expr))
      {
        if (vect_debug_details (NULL))
	  print_generic_expr (dump_file, step_expr, TDF_SLIM);
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
  tree dummy;

  if (vect_debug_details (NULL))
    fprintf (dump_file, "\n<<vect_analyze_scalar_cycles>>\n");

  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
    {
      tree access_fn = NULL;

      if (vect_debug_details (NULL))
	{
          fprintf (dump_file, "Analyze phi: ");
          print_generic_expr (dump_file, phi, TDF_SLIM);
	}

      /* Skip virtual phi's. The data dependences that are associated with
         virtual defs/uses (i.e., memory accesses) are analyzed elsewhere.  */

      if (!is_gimple_reg (SSA_NAME_VAR (PHI_RESULT (phi))))
	{
	  if (vect_debug_details (NULL))
	    fprintf (dump_file, "virtual phi. skip.");
	  continue;
	}

      /* Analyze the evolution function.  */

      /* FORNOW: The only scalar cross-iteration cycles that we allow are
         those of loop induction variables; This property is verified here.

         Furthermore, if that induction variable is used in an operation
         that needs to be vectorized (i.e, is not solely used to index
         arrays and check the exit condition) - we do not support its
         vectorization yet. This property is verified in vect_is_simple_use,
         during vect_analyze_operations.  */

      access_fn = instantiate_parameters
	(loop,
	 analyze_scalar_evolution (loop, PHI_RESULT (phi)));

      if (!access_fn)
	{
	  if (vect_debug_stats (loop) || vect_debug_details (loop))
	    fprintf (dump_file, "not vectorized: unsupported scalar cycle.");
	  return false;
	}

      if (vect_debug_details (NULL))
        {
           fprintf (dump_file, "Access function of PHI: ");
           print_generic_expr (dump_file, access_fn, TDF_SLIM);
        }

      if (!vect_is_simple_iv_evolution (loop->num, access_fn, &dummy, 
					&dummy, false))
	{
	  if (vect_debug_stats (loop) || vect_debug_details (loop))
	    fprintf (dump_file, "not vectorized: unsupported scalar cycle.");
	  return false;
	}
    }

  return true;
}


/* Function vect_analyze_data_ref_dependence.

   Return TRUE if there (might) exist a dependence between a memory-reference
   DRA and a memory-reference DRB.  */

static bool
vect_analyze_data_ref_dependence (struct data_reference *dra,
				  struct data_reference *drb, 
				  struct loop *loop)
{
  tree refa = DR_REF (dra);
  tree refb = DR_REF (drb);
  tree ptra = TREE_OPERAND (refa, 0);
  tree ptrb = TREE_OPERAND (refb, 0);
  tree ta = TREE_TYPE (ptra);
  tree tb = TREE_TYPE (ptrb);

  /* Both refs are array decls:  */
  if (TREE_CODE (refa) == ARRAY_REF 
      && (TREE_CODE (ptra) == VAR_DECL 
	  || (TREE_CODE (ptra) == COMPONENT_REF 
	      && TREE_CODE (TREE_OPERAND (ptra, 0)) == VAR_DECL))
      && TREE_CODE (refb) == ARRAY_REF
      && (TREE_CODE (ptrb) == VAR_DECL 
	  || (TREE_CODE (ptrb) == COMPONENT_REF 
	      && TREE_CODE (TREE_OPERAND (ptrb, 0)) == VAR_DECL)))
    {
      if (array_base_name_differ_p (dra, drb))
	return false;
      else
	{
	  struct data_dependence_relation *ddr = 
		initialize_data_dependence_relation (dra, drb);
	  compute_affine_dependence (ddr);

	  if (DDR_ARE_DEPENDENT (ddr) == chrec_known)
	    return false;
  
	  if (vect_debug_stats (loop) || vect_debug_details (loop))
	    {
	      fprintf (dump_file,
		"not vectorized: dependence between refs to array <");
	      print_generic_expr (dump_file, DR_BASE_NAME (dra), TDF_SLIM);
	      fprintf (dump_file, ">");
	    }

          return true;
	}
    }

  /* At least one of the refs is a pointer access:  */
  if (TREE_CODE (ta) == POINTER_TYPE || TREE_CODE (tb) == POINTER_TYPE)
    {
      if (vect_debug_details (NULL))
  	fprintf (dump_file, 
	      "analyze_data_ref_dependence: alias sets (%d,%d)",
	      (int)get_alias_set (TREE_OPERAND (refa, 0)), 
	      (int)get_alias_set (TREE_OPERAND (refb, 0)));

      if (alias_sets_conflict_p (get_alias_set (ptra), get_alias_set (ptrb)))
	{
          if ((TREE_CODE (ta) == POINTER_TYPE && !TYPE_RESTRICT (ta)) 
	      || (TREE_CODE (tb) == POINTER_TYPE && !TYPE_RESTRICT (tb))) 	
	    {
	      if (vect_debug_stats (loop) || vect_debug_details (loop))
		fprintf (dump_file,
			"not vectorized: dependence between pointer-refs.");
	      return true;
            }
	  else
	    {
	      /* ptra (ptrb) is either an array, or a restricted pointer.  */
	      if (vect_debug_details (NULL))
		fprintf (dump_file,"restricted pointers.");
	      return false;
	    }
	}
      else
	{
          if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "alias sets don't conflict\n");
          return false;
	}
    }

  /* Not handling other cases at the moment.  */
  if (vect_debug_stats (loop) || vect_debug_details (loop))
    {
      fprintf (dump_file, "not vectorized: data-refs unsupported: ");
      print_generic_expr (dump_file, DR_REF (dra), TDF_SLIM);
      fprintf (dump_file, " and ");
      print_generic_expr (dump_file, DR_REF (drb), TDF_SLIM);
    }
  return true;
}


/* Function vect_analyze_data_ref_dependences.

   Examine all the data references in the loop, and make sure there do not
   exist any data dependences between them.

   TODO: dependences which distance is greater than the vectorization factor
         can be ignored.   */

static bool
vect_analyze_data_ref_dependences (loop_vec_info loop_vinfo)
{
  unsigned int i, j;
  varray_type loop_write_refs = LOOP_VINFO_DATAREF_WRITES (loop_vinfo);
  varray_type loop_read_refs = LOOP_VINFO_DATAREF_READS (loop_vinfo);
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);

  /* Examine store-store (output) dependences.  */

  if (vect_debug_details (NULL))
    fprintf (dump_file, "\n<<vect_analyze_dependences>>\n");

  if (vect_debug_details (NULL))
    fprintf (dump_file, "compare all store-store pairs.");

  for (i = 0; i < VARRAY_ACTIVE_SIZE (loop_write_refs); i++)
    {
      for (j = i + 1; j < VARRAY_ACTIVE_SIZE (loop_write_refs); j++)
	{
	  struct data_reference *dra =
	    VARRAY_GENERIC_PTR (loop_write_refs, i);
	  struct data_reference *drb =
	    VARRAY_GENERIC_PTR (loop_write_refs, j);
	  if (vect_analyze_data_ref_dependence (dra, drb, loop))
	    return false;
	}
    }

  /* Examine load-store (true/anti) dependences.  */

  if (vect_debug_details (NULL))
    fprintf (dump_file, "compare all load-store pairs.");

  for (i = 0; i < VARRAY_ACTIVE_SIZE (loop_read_refs); i++)
    {
      for (j = 0; j < VARRAY_ACTIVE_SIZE (loop_write_refs); j++)
	{
	  struct data_reference *dra = VARRAY_GENERIC_PTR (loop_read_refs, i);
	  struct data_reference *drb =
	    VARRAY_GENERIC_PTR (loop_write_refs, j);
	  if (vect_analyze_data_ref_dependence (dra, drb, loop))
	    return false;
	}
    }

  return true;
}


/* Function vect_get_array_first_index.

   REF is an array reference. Find the lower bound of the array dimension and
   return it in ARRAY_FIRST_INDEX. If REF is not of an ARRAY_TYPE or doesn't
   have a TYPE_DOMAIN, then ARRAY_FIRST_INDEX will be 0, and the function 
   returns TRUE. If a TYPE_DOMAIN is found, return TRUE if a simple (constant)
   lower bound was found, and FALSE otherwise.  */ 

static bool
vect_get_array_first_index (tree ref, int *array_first_index)
{
  tree array_start;
  tree array_base_type;

  *array_first_index = 0;

  array_base_type = TREE_TYPE (TREE_OPERAND (ref, 0));
  if (TREE_CODE (TREE_TYPE (array_base_type)) != ARRAY_TYPE)
    return true;
  if (! TYPE_DOMAIN (array_base_type))
    return true;

  array_start = TYPE_MIN_VALUE (TYPE_DOMAIN (array_base_type));
  if (TREE_CODE (array_start) != INTEGER_CST
      || TREE_INT_CST_HIGH (array_start) != 0)
    {
      if (vect_debug_details (NULL))
        {
          fprintf (dump_file, "array min val not simple integer cst.");
          print_generic_expr (dump_file, array_start, TDF_DETAILS);
        }
      return false;
    }

  *array_first_index = TREE_INT_CST_LOW (array_start);
  if (vect_debug_details (NULL))
    {
      print_generic_expr (dump_file, array_start, TDF_DETAILS);
      fprintf (dump_file, "array min val = %d.", *array_first_index);
    }

  return true;
}


/* Function vect_compute_data_ref_alignment

   Compute the misalignment of the data reference DR.

   FOR NOW: No analysis is actually performed. Misalignment is calculated
   only for trivial cases. TODO.  */

static void
vect_compute_data_ref_alignment (struct data_reference *dr, 
				 loop_vec_info loop_vinfo ATTRIBUTE_UNUSED)
{
  tree stmt = DR_STMT (dr);
  tree ref = DR_REF (dr);
  tree vectype;
  tree access_fn = DR_ACCESS_FN (dr, 0); /* FORNOW: single access_fn.  */
  tree init;
  int init_val;
  tree scalar_type;
  int misalign;
  int array_start_val;
  tree array_base = DR_BASE_NAME (dr);

  /* Initialize misalignment to unknown.  */
  DR_MISALIGNMENT (dr) = -1;


  scalar_type = TREE_TYPE (ref);
  vectype = get_vectype_for_scalar_type (scalar_type);
  if (!vectype)
    {
      if (vect_debug_details (NULL))
        {
          fprintf (dump_file, "no vectype for stmt: ");
          print_generic_expr (dump_file, stmt, TDF_SLIM);
          fprintf (dump_file, "scalar_type: ");
          print_generic_expr (dump_file, scalar_type, TDF_DETAILS);
        }
      return;
    }


  /* Check the base of the array_ref.   */
  /* CHECKME */
  if (TYPE_ALIGN (TREE_TYPE (TREE_TYPE (array_base))) < TYPE_ALIGN (vectype))
    {
      if (!vect_force_dr_alignment_p (dr))
	{
	  if (vect_debug_details (NULL))
	    {
	      fprintf (dump_file, "can't force alignment of ref: "); 
	      print_generic_expr (dump_file, array_base, TDF_SLIM);
	    }
          return;
	}
    }
  if (!vect_get_array_first_index (ref, &array_start_val))
    return;
  
  /* Check the index of the array_ref.  */

  init = initial_condition (access_fn);

  /* FORNOW: In order to simplify the handling of alignment, we make sure 
     that the first location at which the array is accessed ('init') is on an 
     'NUNITS' boundary, since we are assuming here that the alignment of the
     'array base' is aligned. This is too conservative, since we require that 
     both {'array_base' is a multiple of NUNITS} && {'init' is a multiple of 
     NUNITS}, instead of just {('array_base' + 'init') is a multiple of NUNITS}.
     This should be relaxed in the future.  */

  if (init && 
      (TREE_CODE (init) != INTEGER_CST || TREE_INT_CST_HIGH (init) != 0))
    {
      if (vect_debug_details (NULL))
        fprintf (dump_file, "init not simple INTEGER_CST.");
      return;
    }

  init_val = TREE_INT_CST_LOW (init);

  misalign = (init_val - array_start_val) % 
		GET_MODE_NUNITS (TYPE_MODE (vectype));

  DR_MISALIGNMENT (dr) = misalign;
}


/* Function vect_compute_data_refs_alignment

   Compute the misalignment of data references in the loop.
   This pass may take place at function granularity instead of at loop
   granularity.

   FOR NOW: No analysis is actually performed. Misalignment is calculated
   only for trivial cases. TODO.  */

static void
vect_compute_data_refs_alignment (loop_vec_info loop_vinfo)
{
  varray_type loop_write_datarefs = LOOP_VINFO_DATAREF_WRITES (loop_vinfo);
  varray_type loop_read_datarefs = LOOP_VINFO_DATAREF_READS (loop_vinfo);
  unsigned int i;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (loop_write_datarefs); i++)
    {
      struct data_reference *dr = VARRAY_GENERIC_PTR (loop_write_datarefs, i);
      vect_compute_data_ref_alignment (dr, loop_vinfo);
    }

  for (i = 0; i < VARRAY_ACTIVE_SIZE (loop_read_datarefs); i++)
    {
      struct data_reference *dr = VARRAY_GENERIC_PTR (loop_read_datarefs, i);
      vect_compute_data_ref_alignment (dr, loop_vinfo);
    }
}

/* Function vect_enhance_data_refs_alignment

   This pass will use loop versioning and loop peeling in order to enhance
   the alignment of data references in the loop.

   FOR NOW: we assume that whatever versioning/peeling takes place, only the
   original loop is to be vectorized; Any other loops that are created by
   the transformations performed in this pass - are not supposed to be
   vectorized. This restriction will be relaxed.

   FOR NOW: No transformation is actually performed. TODO.  */

static void
vect_enhance_data_refs_alignment (loop_vec_info loop_vinfo ATTRIBUTE_UNUSED)
{
  /*
     This pass will require a cost model to guide it whether to apply peeling 
     or versioning or a combination of the two. For example, the scheme that
     intel uses when given a loop with several memory accesses, is as follows:
     choose one memory access ('p') which alignment you want to force by doing 
     peeling. Then, either (1) generate a loop in which 'p' is aligned and all 
     other accesses are not necessarily aligned, or (2) use loop versioning to 
     generate one loop in which all accesses are aligned, and another loop in 
     which only 'p' is necessarily aligned. 

     ("Automatic Intra-Register Vectorization for the Intel Architecture",
      Aart J.C. Bik, Milind Girkar, Paul M. Grey and Ximmin Tian, International
      Journal of Parallel Programming, Vol. 30, No. 2, April 2002.)	

     Devising a cost model is the most critical aspect of this work. It will 
     guide us on which access to peel for, whether to use loop versioning, how 
     many versions to create, etc. The cost model will probably consist of 
     generic considerations as well as target specific considerations (on 
     powerpc for example, misaligned stores are more painful than misaligned 
     loads). 

     Here is the general steps involved in alignment enhancements:
    
     -- original loop, before alignment analysis:
	for (i=0; i<N; i++){
	  x = q[i];			# DR_MISALIGNMENT(q) = unknown
	  p[i] = y;			# DR_MISALIGNMENT(p) = unknown
	}

     -- After vect_compute_data_refs_alignment:
	for (i=0; i<N; i++){
	  x = q[i];			# DR_MISALIGNMENT(q) = 3
	  p[i] = y;			# DR_MISALIGNMENT(p) = unknown
	}

     -- Possibility 1: we do loop versioning:
     if (p is aligned) {
	for (i=0; i<N; i++){	# loop 1A
	  x = q[i];			# DR_MISALIGNMENT(q) = 3
	  p[i] = y;			# DR_MISALIGNMENT(p) = 0
	}
     } 
     else {
	for (i=0; i<N; i++){	# loop 1B
	  x = q[i];			# DR_MISALIGNMENT(q) = 3
	  p[i] = y;			# DR_MISALIGNMENT(p) = unaligned
	}
     }
   
     -- Possibility 2: we do loop peeling:
     for (i = 0; i < 3; i++){	# (scalar loop, not to be vectorized).
	x = q[i];
	p[i] = y;
     }
     for (i = 3; i < N; i++){	# loop 2A
	x = q[i];			# DR_MISALIGNMENT(q) = 0
	p[i] = y;			# DR_MISALIGNMENT(p) = unknown
     }

     -- Possibility 3: combination of loop peeling and versioning:
     for (i = 0; i < 3; i++){	# (scalar loop, not to be vectorized).
	x = q[i];
	p[i] = y;
     }
     if (p is aligned) {
	for (i = 3; i<N; i++){  # loop 3A
	  x = q[i];			# DR_MISALIGNMENT(q) = 0
	  p[i] = y;			# DR_MISALIGNMENT(p) = 0
	}
     } 
     else {
	for (i = 3; i<N; i++){	# loop 3B
	  x = q[i];			# DR_MISALIGNMENT(q) = 0
	  p[i] = y;			# DR_MISALIGNMENT(p) = unaligned
	}
     }

     These loops are later passed to loop_transform to be vectorized. The 
     vectorizer will use the alignment information to guide the transformation 
     (whether to generate regular loads/stores, or with special handling for 
     misalignment). 
   */
}


/* Function vect_analyze_data_refs_alignment

   Analyze the alignment of the data-references in the loop.
   FOR NOW: Until support for misliagned accesses is in place, only if all
   accesses are aligned can the loop be vectorized. This restriction will be 
   relaxed.  */ 

static bool
vect_analyze_data_refs_alignment (loop_vec_info loop_vinfo)
{
  varray_type loop_write_datarefs = LOOP_VINFO_DATAREF_WRITES (loop_vinfo);
  varray_type loop_read_datarefs = LOOP_VINFO_DATAREF_READS (loop_vinfo);
  unsigned int i;

  if (vect_debug_details (NULL))
    fprintf (dump_file, "\n<<vect_analyze_data_refs_alignment>>\n");


  /* This pass may take place at function granularity instead of at loop
     granularity.  */

  vect_compute_data_refs_alignment (loop_vinfo);


  /* This pass will use loop versioning and loop peeling in order to enhance
     the alignment of data references in the loop.
     FOR NOW: we assume that whatever versioning/peeling took place, the 
     original loop is to be vectorized. Any other loops that were created by
     the transformations performed in this pass - are not supposed to be 
     vectorized. This restriction will be relaxed.  */

  vect_enhance_data_refs_alignment (loop_vinfo);


  /* Finally, check that loop can be vectorized. 
     FOR NOW: Until support for misaligned accesses is in place, only if all
     accesses are aligned can the loop be vectorized. This restriction will be 
     relaxed.  */

  for (i = 0; i < VARRAY_ACTIVE_SIZE (loop_write_datarefs); i++)
    {
      struct data_reference *dr = VARRAY_GENERIC_PTR (loop_write_datarefs, i);
      if (!aligned_access_p (dr))
	{
	  if (vect_debug_stats (LOOP_VINFO_LOOP (loop_vinfo))
	      || vect_debug_details (LOOP_VINFO_LOOP (loop_vinfo)))
	    fprintf (dump_file, "not vectorized: unaligned store.");
	  return false;
	}
    }

  /* APPLE LOCAL begin AV misaligned --haifa  */
  if (!targetm.vect.support_misaligned_loads
      || !(*targetm.vect.support_misaligned_loads) ())
    for (i = 0; i < VARRAY_ACTIVE_SIZE (loop_read_datarefs); i++)
      {
	struct data_reference *dr = VARRAY_GENERIC_PTR (loop_read_datarefs, i);
	if (!aligned_access_p (dr))
	  {
	    if (vect_debug_stats (LOOP_VINFO_LOOP (loop_vinfo))
		|| vect_debug_details (LOOP_VINFO_LOOP (loop_vinfo)))
	      fprintf (dump_file, "not vectorized: unaligned load.");
	    return false;
	  }
      }
  /* APPLE LOCAL end AV misaligned --haifa  */

  return true;
}


/* Function vect_analyze_data_ref_access.

   Analyze the access pattern of the data-reference DR. For now, a data access
   has to consecutive and aligned to be considered vectorizable.  */

static bool
vect_analyze_data_ref_access (struct data_reference *dr)
{
  varray_type access_fns = DR_ACCESS_FNS (dr);
  tree access_fn;
  tree init, step;

  /* FORNOW: handle only one dimensional arrays.
     This restriction will be relaxed in the future.  */
  if (VARRAY_ACTIVE_SIZE (access_fns) != 1)
    {
      if (vect_debug_details (NULL))
	fprintf (dump_file, "multi dimensional array reference.");
      return false;
    }
  access_fn = DR_ACCESS_FN (dr, 0);

  if (!vect_is_simple_iv_evolution (loop_containing_stmt (DR_STMT (dr))->num, 
				        access_fn, &init, &step, true))
    {
      if (vect_debug_details (NULL))
	{
	  fprintf (dump_file, "too complicated access function.");
	  print_generic_expr (dump_file, access_fn, TDF_SLIM);
	}
      return false;
    }

  return true;
}


/* Function vect_analyze_data_ref_accesses.

   Analyze the access pattern of all the data references in the loop.

   FORNOW: the only access pattern that is considered vectorizable is a
	   simple step 1 (consecutive) access.

   FORNOW: handle only one dimensional arrays, and pointer accesses.  */

static bool
vect_analyze_data_ref_accesses (loop_vec_info loop_vinfo)
{
  unsigned int i;
  varray_type loop_write_datarefs = LOOP_VINFO_DATAREF_WRITES (loop_vinfo);
  varray_type loop_read_datarefs = LOOP_VINFO_DATAREF_READS (loop_vinfo);

  if (vect_debug_details (NULL))
    fprintf (dump_file, "\n<<vect_analyze_data_ref_accesses>>\n");

  for (i = 0; i < VARRAY_ACTIVE_SIZE (loop_write_datarefs); i++)
    {
      struct data_reference *dr = VARRAY_GENERIC_PTR (loop_write_datarefs, i);
      bool ok = vect_analyze_data_ref_access (dr);
      if (!ok)
	{
	  if (vect_debug_stats (LOOP_VINFO_LOOP (loop_vinfo))
	      || vect_debug_details (LOOP_VINFO_LOOP (loop_vinfo)))
	    fprintf (dump_file, "not vectorized: complicated access pattern.");
	  return false;
	}
    }

  for (i = 0; i < VARRAY_ACTIVE_SIZE (loop_read_datarefs); i++)
    {
      struct data_reference *dr = VARRAY_GENERIC_PTR (loop_read_datarefs, i);
      bool ok = vect_analyze_data_ref_access (dr);
      if (!ok)
	{
	  if (vect_debug_stats (LOOP_VINFO_LOOP (loop_vinfo))
	      || vect_debug_details (LOOP_VINFO_LOOP (loop_vinfo))) 
	    fprintf (dump_file, "not vectorized: complicated access pattern.");
	  return false;
	}
    }

  return true;
}


/* Function vect_analyze_pointer_ref_access.

   Input:
   STMT - a stmt that contains a data-ref
   MEMREF - a data-ref in STMT, which is an INDIRECT_REF.

   If the data-ref access is vectorizable, return a data_reference structure
   that represents it (DR). Otherwise - return NULL.   */

static struct data_reference *
vect_analyze_pointer_ref_access (tree memref, tree stmt)
{
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  struct loop *loop = STMT_VINFO_LOOP (stmt_info);
  tree access_fn = analyze_scalar_evolution (loop, TREE_OPERAND (memref, 0));
  tree init, step;	
  int step_val;
  tree reftype, innertype;
  enum machine_mode innermode;
  tree indx_access_fn; 
  int loopnum = loop->num;
  struct data_reference *dr;

  if (!access_fn)
    {
      if (vect_debug_stats (loop) || vect_debug_details (loop))
        fprintf (dump_file, "not vectorized: complicated pointer access.");	
      return NULL;
    }

  if (vect_debug_details (NULL))
    {
      fprintf (dump_file, "Access function of ptr: ");
      print_generic_expr (dump_file, access_fn, TDF_SLIM);
    }

  if (!vect_is_simple_iv_evolution (loopnum, access_fn, &init, &step, false))
    {
      if (vect_debug_stats (loop) || vect_debug_details (loop)) 
	fprintf (dump_file, "not vectorized: pointer access is not simple.");	
      return NULL;
    }
		
  if (TREE_CODE (init) != SSA_NAME 	   /* FORNOW */
      || TREE_CODE (step) != INTEGER_CST
      || TREE_INT_CST_HIGH (step) != 0)
    {
      if (vect_debug_stats (loop) || vect_debug_details (loop)) 
	fprintf (dump_file, 
		"not vectorized: non constant init/step for pointer access.");	
      return NULL;
    }

  step_val = TREE_INT_CST_LOW (step);

  reftype = TREE_TYPE (TREE_OPERAND (memref, 0));
  if (TREE_CODE (reftype) != POINTER_TYPE) 
    {
      if (vect_debug_stats (loop) || vect_debug_details (loop))
	fprintf (dump_file, "not vectorized: unexpected pointer access form.");	
      return NULL;
    }

  reftype = TREE_TYPE (init);
  if (TREE_CODE (reftype) != POINTER_TYPE) 
    {
      if (vect_debug_stats (loop) || vect_debug_details (loop)) 
	fprintf (dump_file, "not vectorized: unexpected pointer access form.");
      return NULL;
    }

  innertype = TREE_TYPE (reftype);
  innermode = TYPE_MODE (innertype);
  if (GET_MODE_SIZE (innermode) != step_val) 
    {
      /* FORNOW: support only consecutive access */
      if (vect_debug_stats (loop) || vect_debug_details (loop)) 
	fprintf (dump_file, "not vectorized: non consecutive access.");	
      return NULL;
    }

  indx_access_fn = 
	build_polynomial_chrec (loopnum, integer_zero_node, integer_one_node);
  if (vect_debug_details (NULL)) 
    {
      fprintf (dump_file, "Access function of ptr indx: ");
      print_generic_expr (dump_file, indx_access_fn, TDF_SLIM);
    }
  dr = init_data_ref (stmt, memref, init, indx_access_fn);
  return dr;
}


/* Function vect_analyze_data_refs.

   Find all the data references in the loop.

   FORNOW: Handle aligned INDIRECT_REFs and one dimensional ARRAY_REFs 
	   which base is really an array (not a pointer) and which alignment 
	   can be forced. This restriction will be relaxed.   */

static bool
vect_analyze_data_refs (loop_vec_info loop_vinfo)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  basic_block *bbs = LOOP_VINFO_BBS (loop_vinfo);
  int nbbs = loop->num_nodes;
  block_stmt_iterator si;
  int j;
  struct data_reference *dr;

  if (vect_debug_details (NULL))
    fprintf (dump_file, "\n<<vect_analyze_data_refs>>\n");

  for (j = 0; j < nbbs; j++)
    {
      basic_block bb = bbs[j];
      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  bool is_read = false;
	  tree stmt = bsi_stmt (si);
	  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
	  v_may_def_optype v_may_defs = STMT_V_MAY_DEF_OPS (stmt);
	  v_must_def_optype v_must_defs = STMT_V_MUST_DEF_OPS (stmt);
	  vuse_optype vuses = STMT_VUSE_OPS (stmt);
	  varray_type *datarefs = NULL;
	  int nvuses, nv_may_defs, nv_must_defs;
	  tree memref = NULL;
	  tree array_base;
	  tree symbl;

	  /* Assumption: there exists a data-ref in stmt, if and only if 
             it has vuses/vdefs.  */

	  if (!vuses && !v_may_defs && !v_must_defs)
	    continue;

	  nvuses = NUM_VUSES (vuses);
	  nv_may_defs = NUM_V_MAY_DEFS (v_may_defs);
	  nv_must_defs = NUM_V_MUST_DEFS (v_must_defs);

	  if (nvuses && (nv_may_defs || nv_must_defs))
	    {
	      if (vect_debug_details (NULL))
		{
		  fprintf (dump_file, "unexpected vdefs and vuses in stmt: ");
		  print_generic_expr (dump_file, stmt, TDF_SLIM);
		}
	      return false;
	    }

	  if (TREE_CODE (stmt) != MODIFY_EXPR)
	    {
	      if (vect_debug_details (NULL))
		{
		  fprintf (dump_file, "unexpected vops in stmt: ");
		  print_generic_expr (dump_file, stmt, TDF_SLIM);
		}
	      return false;
	    }

	  if (vuses)
	    {
	      memref = TREE_OPERAND (stmt, 1);
	      datarefs = &(LOOP_VINFO_DATAREF_READS (loop_vinfo));
	      is_read = true;
	    } 
	  else /* vdefs */
	    {
	      memref = TREE_OPERAND (stmt, 0);
	      datarefs = &(LOOP_VINFO_DATAREF_WRITES (loop_vinfo));
	      is_read = false;
	    }

	  if (TREE_CODE (memref) == INDIRECT_REF)
            {
              dr = vect_analyze_pointer_ref_access (memref, stmt);
              if (! dr)
                return false; 
	      symbl = DR_BASE_NAME (dr);	
            }
	  else if (TREE_CODE (memref) == ARRAY_REF)
	    {
	      tree base, oprnd0;		
	      array_base = TREE_OPERAND (memref, 0);
   
              /* FORNOW: make sure that the array is one dimensional.
                 This restriction will be relaxed in the future.  */
              if (TREE_CODE (array_base) == ARRAY_REF)
                {
                  if (vect_debug_stats (loop) || vect_debug_details (loop))
		    {
                      fprintf (dump_file, 
				"not vectorized: multi-dimensional array.");
                      print_generic_expr (dump_file, stmt, TDF_SLIM);
		    }
                  return false;
                }

              dr = analyze_array (stmt, memref, is_read);

	      /* Find the relevant symbol for aliasing purposes.  */	
	      base = DR_BASE_NAME (dr);
	      switch (TREE_CODE (base))	
		{
		case VAR_DECL:
		  symbl = base;
		  break;
		case INDIRECT_REF:
		  symbl = TREE_OPERAND (base, 0); 
		  break;
		case COMPONENT_REF:
		  oprnd0 = TREE_OPERAND (base, 0);
		  if (TREE_CODE (oprnd0) == VAR_DECL)
		    {	
		      symbl = base;
		      break;
		    }	
		  if (TREE_CODE (oprnd0) == INDIRECT_REF)
		    {
		      symbl = TREE_OPERAND (oprnd0, 0);
		      break;
		    }	
		  /* fall through */	
		default:
                  if (vect_debug_stats (loop) || vect_debug_details (loop))
		    {
                      fprintf (dump_file,
                        "not vectorized: unhandled struct/class field access ");
                      print_generic_expr (dump_file, stmt, TDF_SLIM);
		    }
                  return false;
		} /* switch */
	    }
	  else
	    {
	      if (vect_debug_stats (loop) || vect_debug_details (loop))
		{
		  fprintf (dump_file, "not vectorized: unhandled data ref: ");
		  print_generic_expr (dump_file, stmt, TDF_SLIM);
		}
	      return false;
	    }
	
	  /* Find and record the memtag assigned to this data-ref.  */
	  if (TREE_CODE (symbl) == VAR_DECL 
	      || (TREE_CODE (symbl) == COMPONENT_REF 
		  && TREE_CODE (TREE_OPERAND (symbl, 0)) == VAR_DECL))
	    STMT_VINFO_MEMTAG (stmt_info) = symbl;
	  else if (TREE_CODE (symbl) == SSA_NAME)
	    {
	      tree tag;
	      symbl = SSA_NAME_VAR (symbl);
	      tag = get_var_ann (symbl)->type_mem_tag;
	      if (!tag)
		{
		  tree ptr = TREE_OPERAND (memref, 0);
		  if (TREE_CODE (ptr) == SSA_NAME)
		    tag = get_var_ann (SSA_NAME_VAR (ptr))->type_mem_tag;
		}
	      if (!tag)
		{
		  if (vect_debug_stats (loop) || vect_debug_details (loop))
		    fprintf (dump_file, "not vectorized: no memtag for ref.");
		  return false;
		}
	      STMT_VINFO_MEMTAG (stmt_info) = tag;
	    }
	  else
	    {
	      if (vect_debug_stats (loop) || vect_debug_details (loop))
		{
		  fprintf (dump_file, "not vectorized: unsupported data-ref: ");
		  print_generic_expr (dump_file, memref, TDF_SLIM);
		}
	      return false;
            }

	  VARRAY_PUSH_GENERIC_PTR (*datarefs, dr);
	  STMT_VINFO_DATA_REF (stmt_info) = dr;
	}
    }

  return true;
}


/* Utility functions used by vect_mark_stmts_to_be_vectorized. */

/* Function vect_mark_relevant.

   Mark STMT as "relevant for vectorization" and add it to WORKLIST.  */

static void
vect_mark_relevant (varray_type worklist, tree stmt)
{
  stmt_vec_info stmt_info;

  if (vect_debug_details (NULL))
    fprintf (dump_file, "mark relevant.");

  if (TREE_CODE (stmt) == PHI_NODE)
    {
      VARRAY_PUSH_TREE (worklist, stmt);
      return;
    }

  stmt_info = vinfo_for_stmt (stmt);

  if (!stmt_info)
    {
      if (vect_debug_details (NULL))
	{
	  fprintf (dump_file, "mark relevant: no stmt info!!.");
	  print_generic_expr (dump_file, stmt, TDF_SLIM);
	}
      return;
    }

  if (STMT_VINFO_RELEVANT_P (stmt_info))
    {
      if (vect_debug_details (NULL))
        fprintf (dump_file, "already marked relevant.");
      return;
    }

  /* APPLE LOCAL begin AV vmul_uch --haifa  */
  if (STMT_VINFO_IN_PATTERN_P (stmt_info))
    {
      /* leave as irrelevant, but add to worklist.  */
      if (vect_debug_details (NULL))
        fprintf (dump_file, "part of pattern.");
      VARRAY_PUSH_TREE (worklist, stmt);
      return;
    }
  /* APPLE LOCAL end AV vmul_uch --haifa  */

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
  v_may_def_optype v_may_defs;
  v_must_def_optype v_must_defs;
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  int i;
  dataflow_t df;
  int num_uses;

  /* cond stmt other than loop exit cond.  */
  if (is_ctrl_stmt (stmt) && (stmt != LOOP_VINFO_EXIT_COND (loop_vinfo)))
    return true;

  /* changing memory.  */
  v_may_defs = STMT_V_MAY_DEF_OPS (stmt);
  v_must_defs = STMT_V_MUST_DEF_OPS (stmt);
  if (v_may_defs || v_must_defs)
    {
      if (vect_debug_details (NULL))
        fprintf (dump_file, "vec_stmt_relevant_p: stmt has vdefs.");
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
	  if (vect_debug_details (NULL))
	    fprintf (dump_file, "vec_stmt_relevant_p: used out of loop.");
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

  if (vect_debug_details (NULL))
    fprintf (dump_file, "\n<<vect_mark_stmts_to_be_vectorized>>\n");

  VARRAY_TREE_INIT (worklist, 64, "work list");

  /* 1. Init worklist.  */

  for (i = 0; i < nbbs; i++)
    {
      basic_block bb = bbs[i];
      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  stmt = bsi_stmt (si);

	  if (vect_debug_details (NULL))
	    {
	      fprintf (dump_file, "init: stmt relevant? ");
	      print_generic_expr (dump_file, stmt, TDF_SLIM);
	    } 

	  stmt_info = vinfo_for_stmt (stmt);

	  /* APPLE LOCAL AV vmul_uch --haifa  */
	  /* Not needed, done in new_stmt_vec_info.
	  STMT_VINFO_RELEVANT_P (stmt_info) = 0; */

	  if (vect_stmt_relevant_p (stmt, loop_vinfo))
	    vect_mark_relevant (worklist, stmt);
	}
    }


  /* 2. Process_worklist */

  while (VARRAY_ACTIVE_SIZE (worklist) > 0)
    {
      stmt = VARRAY_TOP_TREE (worklist);
      VARRAY_POP (worklist);

      if (vect_debug_details (NULL))
	{
          fprintf (dump_file, "worklist: examine stmt: ");
          print_generic_expr (dump_file, stmt, TDF_SLIM);
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
	      tree def_stmt = NULL_TREE;
	      basic_block bb;
	      if (!vect_is_simple_use (arg, loop, &def_stmt))
		{
		  if (vect_debug_details (NULL))	
		    fprintf (dump_file, "worklist: unsupported use.");
		  varray_clear (worklist);
		  return false;
		}
	      if (!def_stmt)
		continue;

	      if (vect_debug_details (NULL))
	        {
	          fprintf (dump_file, "worklist: def_stmt: ");
		  print_generic_expr (dump_file, def_stmt, TDF_SLIM);
		}

	      bb = bb_for_stmt (def_stmt);
	      if (flow_bb_inside_loop_p (loop, bb))
	        vect_mark_relevant (worklist, def_stmt);
	    }
	} 

      ann = stmt_ann (stmt);
      use_ops = USE_OPS (ann);

      for (i = 0; i < NUM_USES (use_ops); i++)
	{
	  tree use = USE_OP (use_ops, i);

	  /* We are only interested in uses that need to be vectorized. Uses 
	     that are used for address computation are not considered relevant.
	   */
	  if (exist_non_indexing_operands_for_use_p (use, stmt))
	    {
              tree def_stmt = NULL_TREE;
              basic_block bb;
              if (!vect_is_simple_use (use, loop, &def_stmt))
                {
                  if (vect_debug_details (NULL))        
                    fprintf (dump_file, "worklist: unsupported use.");
                  varray_clear (worklist);
                  return false;
                }

	      if (!def_stmt)
		continue;

              if (vect_debug_details (NULL))
                {
                  fprintf (dump_file, "worklist: examine use %d: ", i);
                  print_generic_expr (dump_file, use, TDF_SLIM);
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


/* APPLE LOCAL begin AV vmul_uch --haifa  */
/* Function vect_recog_mul_uch_to_uch1

   Try to find a pattern of multiplication of two uchars, which product is 
   converted back to uchar, as follows: 
   (we use 'uchar' for unsigned char and 'ushort' for unsigned short)
   
   uchar x' = (ushort) x;  
   uchar y' = (ushort) y;  
   ushort prod = mul (x`, y`);
   ushort z` = prod >> 8;
   uchar z = (uchar) z`;

   If this pattern is detected, the stmts that are part of the pattern
   will be marked as such, and a new stmt that represents the pattern
   will be generated.
*/

static tree
vect_recog_mul_uch_to_uch1 (tree stmt0_ush_to_uch)
{   
  struct loop *loop = STMT_VINFO_LOOP (vinfo_for_stmt (stmt0_ush_to_uch));
  tree stmt;
  tree oprnd0 = NULL_TREE, oprnd1, operation;
  tree stmt1_ush_shft_8 = NULL;
  tree stmt2_mul_ush = NULL;
  tree stmt3_arg1_uch_to_ush = NULL;
  tree stmt4_arg2_uch_to_ush = NULL;
  tree pattern_stmt = NULL;
  tree var, var_name;
  tree arg1, arg2;
  stmt_ann_t ann;

  oprnd0 = TREE_OPERAND (TREE_OPERAND (stmt0_ush_to_uch, 1),0);

  /* found stmt0; check if the def-use chain that defines this
     stmt complies with the pattern.  */

  stmt = SSA_NAME_DEF_STMT (oprnd0);  
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return NULL_TREE;
  operation = TREE_OPERAND (stmt, 1);
  if (TREE_CODE (operation) != RSHIFT_EXPR)
    return NULL_TREE;
  oprnd0 = TREE_OPERAND (operation, 0);
  oprnd1 = TREE_OPERAND (operation, 1);
  if (TREE_CODE (TREE_TYPE (oprnd0)) == INTEGER_TYPE
      && TYPE_MAIN_VARIANT (TREE_TYPE (oprnd0)) == short_unsigned_type_node
      && TREE_CODE (oprnd0) == SSA_NAME
      && TREE_CODE (oprnd1) == INTEGER_CST
      && TREE_INT_CST_HIGH (oprnd1) == 0
      && TREE_INT_CST_LOW (oprnd1) == 8)
    stmt1_ush_shft_8 = stmt;
  else
    return NULL_TREE;


  stmt = SSA_NAME_DEF_STMT (oprnd0);  
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return NULL_TREE;
  operation = TREE_OPERAND (stmt, 1);
  if (TREE_CODE (operation) != MULT_EXPR)
    return NULL_TREE;
  oprnd0 = TREE_OPERAND (operation, 0);
  oprnd1 = TREE_OPERAND (operation, 1);
  if (TREE_CODE (TREE_TYPE (oprnd0)) == INTEGER_TYPE
      && TYPE_MAIN_VARIANT (TREE_TYPE (oprnd0)) == short_unsigned_type_node
      && TREE_CODE (oprnd0) == SSA_NAME
      && TREE_CODE (TREE_TYPE (oprnd1)) == INTEGER_TYPE
      && TYPE_MAIN_VARIANT (TREE_TYPE (oprnd1)) == short_unsigned_type_node
      && TREE_CODE (oprnd1) == SSA_NAME)
    stmt2_mul_ush = stmt;
  else
    return NULL_TREE;


  stmt = SSA_NAME_DEF_STMT (oprnd0);  
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return NULL_TREE;
  operation = TREE_OPERAND (stmt, 1);
  if (TREE_CODE (operation) != NOP_EXPR)
    return NULL_TREE;
  arg1 = TREE_OPERAND (operation, 0);
  if (TREE_CODE (TREE_TYPE (arg1)) == INTEGER_TYPE
      && TYPE_MAIN_VARIANT (TREE_TYPE (arg1)) == unsigned_char_type_node)
    stmt3_arg1_uch_to_ush = stmt;
  else
    return NULL_TREE;


  stmt = SSA_NAME_DEF_STMT (oprnd1);  
  if (TREE_CODE (stmt) != MODIFY_EXPR)
    return NULL_TREE;
  operation = TREE_OPERAND (stmt, 1);
  if (TREE_CODE (operation) != NOP_EXPR)
    return NULL_TREE;
  arg2 = TREE_OPERAND (operation, 0);
  if (TREE_CODE (TREE_TYPE (arg2)) == INTEGER_TYPE
      && TYPE_MAIN_VARIANT (TREE_TYPE (arg2)) == unsigned_char_type_node)
    stmt4_arg2_uch_to_ush = stmt;
  else
    return NULL_TREE;

  if (vect_debug_details (NULL))
    fprintf (dump_file, "pattern mul_uch recognized.");

  /* found the pattern! 
     mark the stmts that are involved in the pattern,
     and create a new stmt to express the pattern.  */

  var = create_tmp_var (unsigned_char_type_node, "patt");
  add_referenced_tmp_var (var);
  var_name = make_ssa_name (var, NULL_TREE);
  pattern_stmt = build2 (MODIFY_EXPR, unsigned_char_type_node, var_name,
	build2 (MULT_UCH_EXPR, unsigned_char_type_node, arg1, arg2));
  get_stmt_operands (pattern_stmt);
  ann = stmt_ann (pattern_stmt);
  set_stmt_info (ann, new_stmt_vec_info (pattern_stmt, loop));
  STMT_VINFO_RELATED_STMT (vinfo_for_stmt (pattern_stmt)) = stmt0_ush_to_uch;
  STMT_VINFO_RELEVANT_P (vinfo_for_stmt (pattern_stmt)) = true;

  STMT_VINFO_IN_PATTERN_P (vinfo_for_stmt (stmt4_arg2_uch_to_ush)) = true;
  STMT_VINFO_IN_PATTERN_P (vinfo_for_stmt (stmt3_arg1_uch_to_ush)) = true;
  STMT_VINFO_IN_PATTERN_P (vinfo_for_stmt (stmt2_mul_ush)) = true;
  STMT_VINFO_IN_PATTERN_P (vinfo_for_stmt (stmt1_ush_shft_8)) = true;
  STMT_VINFO_IN_PATTERN_P (vinfo_for_stmt (stmt0_ush_to_uch)) = true;

  STMT_VINFO_RELATED_STMT (vinfo_for_stmt (stmt0_ush_to_uch)) = pattern_stmt;

  return pattern_stmt;
}


/* Function vect_mul_uch_to_uch

   Try to find computation idioms.  */

static void
vect_recog_mul_uch_to_uch (loop_vec_info loop_vinfo)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  basic_block *bbs = LOOP_VINFO_BBS (loop_vinfo);
  unsigned int nbbs = loop->num_nodes;
  block_stmt_iterator si;
  tree stmt;
  unsigned int i;
  tree def_oprnd, oprnd0 = NULL_TREE, operation;
  tree pattern_stmt = NULL_TREE;

  for (i = 0; i < nbbs; i++)
    {
      basic_block bb = bbs[i];
      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
        {
          stmt = bsi_stmt (si); 
          if (TREE_CODE (stmt) != MODIFY_EXPR)
            continue;
          def_oprnd = TREE_OPERAND (stmt, 0);
          operation = TREE_OPERAND (stmt, 1);
          if (TREE_CODE (operation) != CONVERT_EXPR)
            continue;
          oprnd0 = TREE_OPERAND (operation, 0);
          if (TREE_CODE (TREE_TYPE (def_oprnd)) != INTEGER_TYPE)
            continue;
          if (TYPE_MAIN_VARIANT (TREE_TYPE (def_oprnd)) != unsigned_char_type_node)
            continue;
          if (TREE_CODE (TREE_TYPE (oprnd0)) != INTEGER_TYPE
              || TYPE_MAIN_VARIANT (TREE_TYPE (oprnd0)) != short_unsigned_type_node)
            continue;
          if (TREE_CODE (oprnd0) != SSA_NAME)
            continue;
 	  pattern_stmt = vect_recog_mul_uch_to_uch1 (stmt);
	  if (pattern_stmt != NULL_TREE)
	    {
	      bsi_insert_before (&si, pattern_stmt, BSI_SAME_STMT);
	      pattern_stmt = NULL_TREE;
	    }
        }
    }
}


/* Function vect_pattern_recog

   Try to find computation idioms.  */

static void
vect_pattern_recog (loop_vec_info loop_vinfo)
{
  if (vect_debug_details (NULL))
    fprintf (dump_file, "\n<<vect_pattern_recog>>\n");

  vect_recog_mul_uch_to_uch (loop_vinfo);

  /* In the future, additional idioms will be recognized here,
     possibly through target hooks, to allow targets to recognize
     patterns that have special architectural support.  */
}
/* APPLE LOCAL end AV vmul_uch --haifa  */


/* Function vect_analyze_loop_with_symbolic_num_of_iters.

   In case the number of iterations that LOOP iterates in unknown at compile 
   time, an epilog loop will be generated, and the loop induction variables 
   (IVs) will be "advanced" to the value they are supposed to take just before 
   the epilog loop. Here we check that the access function of the loop IVs
   and the expression that represents the loop bound are simple enough.
   These restrictions will be relxed in the future.  */

static bool 
vect_analyze_loop_with_symbolic_num_of_iters (tree *symb_num_of_iters, 
					      struct loop *loop)
{
  tree niters;
  basic_block bb = loop->header;
  tree phi;

  if (vect_debug_details (NULL))
    fprintf (dump_file, "\n<<vect_analyze_loop_with_symbolic_num_of_iters>>\n");
  
  niters = number_of_iterations_in_loop (loop);

  /* APPLE LOCAL begin AV --haifa  */
  if (niters == NULL_TREE || niters == chrec_dont_know)
    {
      struct tree_niter_desc niter_desc;
      if (number_of_iterations_exit
	  (loop, loop->exit_edges[0], &niter_desc))
        niters = build2 (PLUS_EXPR, TREE_TYPE (niter_desc.niter), 
		niter_desc.niter, integer_one_node);
    }
  /* APPLE LOCAL end AV --haifa  */

  if (chrec_contains_undetermined (niters))
    {
      if (vect_debug_details (NULL))
          fprintf (dump_file, "Infinite number of iterations.");
      return false;
    }

  if (!niters)
    {
      if (vect_debug_details (NULL))
          fprintf (dump_file, "niters is NULL pointer.");
      return false;
    }

  if (vect_debug_details (NULL))
    {
      fprintf (dump_file, "Symbolic number of iterations is ");
      print_generic_expr (dump_file, niters, TDF_DETAILS);
    }

  /* Analyze phi functions of the loop header.  */

  for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
    {
      tree access_fn = NULL;
      tree evolution_part;

      if (vect_debug_details (NULL))
	{
          fprintf (dump_file, "Analyze phi: ");
          print_generic_expr (dump_file, phi, TDF_SLIM);
	}

      /* Skip virtual phi's. The data dependences that are associated with
         virtual defs/uses (i.e., memory accesses) are analyzed elsewhere.  */

      if (!is_gimple_reg (SSA_NAME_VAR (PHI_RESULT (phi))))
	{
	  if (vect_debug_details (NULL))
	    fprintf (dump_file, "virtual phi. skip.");
	  continue;
	}

      /* Analyze the evolution function.  */

      access_fn = instantiate_parameters
	(loop, analyze_scalar_evolution (loop, PHI_RESULT (phi)));

      if (!access_fn)
	{
	  if (vect_debug_details (NULL))
	    fprintf (dump_file, "No Access function.");
	  return false;
	}

      if (vect_debug_details (NULL))
        {
           fprintf (dump_file, "Access function of PHI: ");
           print_generic_expr (dump_file, access_fn, TDF_SLIM);
        }

      evolution_part = evolution_part_in_loop_num (access_fn, loop->num);
      
      if (evolution_part == NULL_TREE)
	return false;
  
      /* FORNOW: We do not transform initial conditions of IVs 
	 which evolution functions are a polynomial of degree >= 2 */

      if (tree_is_chrec (evolution_part))
	return false;  
    }

  *symb_num_of_iters = niters;
  return  true;
}


/* Function vect_get_loop_niters.

   Determine how many iterations the loop is executed.  */

static tree
vect_get_loop_niters (struct loop *loop, int *number_of_iterations)
{
  tree niters;
  /* APPLE LOCAL begin AV --haifa  */
  tree loop_exit;
  bool analyzable_loop_bound = false;
  /* APPLE LOCAL end AV --haifa  */

  if (vect_debug_details (NULL))
    fprintf (dump_file, "\n<<get_loop_niters>>\n");

  /* APPLE LOCAL begin AV --haifa  */
  loop_exit = get_loop_exit_condition (loop);

  /* First, use the scev information about the number of iterations.  */
  niters = number_of_iterations_in_loop (loop);
  if (niters != NULL_TREE && niters != chrec_dont_know)
    {
      if (vect_debug_details (NULL))
        {
          fprintf (dump_file, "scev niters: ");
          print_generic_expr (dump_file, niters, TDF_SLIM);
        }

      if (TREE_CODE (niters) == INTEGER_CST)
        *number_of_iterations = TREE_INT_CST_LOW (niters);

      if (vect_debug_details (NULL))
        fprintf (dump_file, "scev niters: %d", *number_of_iterations);
      analyzable_loop_bound = true;
    }
  else
    {
      struct tree_niter_desc niter_desc;
      if (number_of_iterations_exit
                (loop, loop->exit_edges[0], &niter_desc))
        {
          if (vect_debug_details (NULL))
            {
              fprintf (dump_file, "number_of_iterations_exit: ");
              print_generic_expr (dump_file, niter_desc.niter, TDF_SLIM);
            }

          if (TREE_CODE (niter_desc.niter) == INTEGER_CST)
            {
              int niters  = TREE_INT_CST_LOW (niter_desc.niter);
              *number_of_iterations = niters + 1;

              if (vect_debug_details (NULL))
                fprintf (dump_file,
                  "number_of_iterations_exit: %d", *number_of_iterations);
            }
          analyzable_loop_bound = true;
        }
    }

  return loop_exit;
  /* APPLE LOCAL end --haifa  */
}


/* Function vect_analyze_loop_form.

   Verify the following restrictions (some may be relaxed in the future):
   - it's an inner-most loop
   - number of BBs = 2 (which are the loop header and the latch)
   - the loop has a pre-header
   - the loop has a single entry and exit
   - the loop exit condition is simple enough, and the number of iterations
     can be analyzed (a countable loop).  */

static loop_vec_info
vect_analyze_loop_form (struct loop *loop)
{
  loop_vec_info loop_vinfo;
  tree loop_cond;
  int number_of_iterations = -1;
  tree symb_num_of_iters = NULL_TREE;

  if (vect_debug_details (loop))
    fprintf (dump_file, "\n<<vect_analyze_loop_form>>\n");

  /* APPLE LOCAL AV if-conversion --dpatel  */
  /* Do not check loop->num_nodes here.  */
  if (loop->level > 1		/* FORNOW: inner-most loop  */
      || loop->num_exits > 1 || loop->num_entries > 1 
      || !loop->pre_header || !loop->header || !loop->latch)
    {
      if (vect_debug_stats (loop) || vect_debug_details (loop))
	{
	  fprintf (dump_file, "not vectorized: bad loop form. ");
	  if (loop->level > 1)
	    fprintf (dump_file, "nested loop.");
	  else if (loop->num_exits > 1 || loop->num_entries > 1)
	    fprintf (dump_file, "multiple entries or exits.");
	  else if (loop->num_nodes != 2 || !loop->header || !loop->latch)
	        fprintf (dump_file, "too many BBs in loop.");
	  else if (!loop->pre_header)
	    fprintf (dump_file, "no pre-header BB for loop.");
	}

      return NULL;
    }

  loop_cond = vect_get_loop_niters (loop, &number_of_iterations);
  if (!loop_cond)
    {
      if (vect_debug_stats (loop) || vect_debug_details (loop))
	fprintf (dump_file, "not vectorized: complicated exit condition.");
      return NULL;
    }

  /* APPLE LOCAL begin AV if-conversion --dpatel  */
  /* Do if-conversion, before checking num of nodes.  */
  if (number_of_iterations > 0 && second_loop_vers_available)
    if_converted_loop = tree_if_conversion (loop, true);

  if (loop->num_nodes != 3 && loop->num_nodes != 2)
    {
      if (vect_debug_details (NULL))
 	{
 	  fprintf (dump_file,
		   "loop_analyzer: bad loop form (no of nodes...)\n");
 	  flow_loop_dump (loop, dump_file, NULL, 1);
 	}
      
      return NULL;
    }
  /* APPLE LOCAL end AV if-conversion --dpatel  */

  if (number_of_iterations < 0)
    {
      if (vect_debug_details (NULL))
	fprintf (dump_file, "loop bound unknown.");

      /* Unknown loop bound.  */
      /* APPLE LOCAL begin AV if-conversion --dpatel  */
      /* Do not handle if-converted loop.  */
      if (if_converted_loop)
 	{
 	  if (vect_debug_details (NULL))
 	    fprintf (dump_file, "Can't handle unknown loop bound in if converted loop.");
 	  return NULL;
	}
      /* APPLE LOCAL end AV if-conversion --dpatel  */

      if (!vect_analyze_loop_with_symbolic_num_of_iters 
	  (&symb_num_of_iters, loop))
	{
	  if (vect_debug_stats (loop) || vect_debug_details (loop)) 
	    fprintf (dump_file, "not vectorized: can't determine loop bound.");
	  return NULL;
	}
    }

  if (number_of_iterations == 0) /* CHECKME: can this happen? */
    {
      if (vect_debug_stats (loop) || vect_debug_details (loop))
	fprintf (dump_file, "not vectorized: number of iterations = 0.");
      return NULL;
    }

  loop_vinfo = new_loop_vec_info (loop);
  LOOP_VINFO_EXIT_COND (loop_vinfo) = loop_cond;
  LOOP_VINFO_NITERS (loop_vinfo) = number_of_iterations;
  LOOP_VINFO_SYMB_NUM_OF_ITERS(loop_vinfo) = symb_num_of_iters;

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

  if (vect_debug_details (NULL))
    fprintf (dump_file, "\n<<<<<<< analyze_loop_nest >>>>>>>\n");

  /* Check the CFG characteristics of the loop (nesting, entry/exit, etc.  */

  loop_vinfo = vect_analyze_loop_form (loop);
  if (!loop_vinfo)
    {
      if (vect_debug_details (loop))
	fprintf (dump_file, "bad loop form.");
      return NULL;
    }

  /* Find all data references in the loop (which correspond to vdefs/vuses)
     and analyze their evolution in the loop.

     FORNOW: Handle only simple, one-dimensional, array references, which
     alignment can be forced, and aligned pointer-references.  */

  ok = vect_analyze_data_refs (loop_vinfo);
  if (!ok)
    {
      if (vect_debug_details (loop))
	fprintf (dump_file, "bad data references.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }

  /* APPLE LOCAL AV vmul_uch --haifa  */
  vect_pattern_recog (loop_vinfo);

  /* Data-flow analysis to detect stmts that do not need to be vectorized.  */

  ok = vect_mark_stmts_to_be_vectorized (loop_vinfo);
  if (!ok)
    {
      if (vect_debug_details (loop))
	fprintf (dump_file, "unexpected pattern.");
      if (vect_debug_details (loop))
	fprintf (dump_file, "not vectorized: unexpected pattern.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }


  /* Check that all cross-iteration scalar data-flow cycles are OK.
     Cross-iteration cycles caused by virtual phis are analyzed separately.  */

  ok = vect_analyze_scalar_cycles (loop_vinfo);
  if (!ok)
    {
      if (vect_debug_details (loop))
	fprintf (dump_file, "bad scalar cycle.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }


  /* Analyze data dependences between the data-refs in the loop. 
     FORNOW: fail at the first data dependence that we encounter.  */

  ok = vect_analyze_data_ref_dependences (loop_vinfo);
  if (!ok)
    {
      if (vect_debug_details (loop))
	fprintf (dump_file, "bad data dependence.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }


  /* Analyze the access patterns of the data-refs in the loop (consecutive,
     complex, etc.). FORNOW: Only handle consecutive access pattern.  */

  ok = vect_analyze_data_ref_accesses (loop_vinfo);
  if (!ok)
    {
      if (vect_debug_details (loop))
	fprintf (dump_file, "bad data access.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }


  /* Analyze the alignment of the data-refs in the loop.
     FORNOW: Only aligned accesses are handled.  */

  ok = vect_analyze_data_refs_alignment (loop_vinfo);
  if (!ok)
    {
      if (vect_debug_details (loop))
	fprintf (dump_file, "bad data alignment.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }


  /* Scan all the operations in the loop and make sure they are
     vectorizable.  */

  ok = vect_analyze_operations (loop_vinfo);
  if (!ok)
    {
      if (vect_debug_details (loop))
	fprintf (dump_file, "bad operation or unsupported loop bound.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }

  LOOP_VINFO_VECTORIZABLE_P (loop_vinfo) = 1;

  return loop_vinfo;
}


/* Function need_imm_uses_for.

   Return whether we ought to include information for 'var'
   when calculating immediate uses.  For this pass we only want use
   information for non-virtual variables.  */

static bool
need_imm_uses_for (tree var)
{
  return is_gimple_reg (var);
}

/* APPLE LOCAL begin AV if-conversion --dpatel  */
/* Create second version of the loop.  */

static void
vect_loop_version (struct loops *loops, struct loop *loop, basic_block *bb)
{
  tree cond_expr;
  struct loop *nloop;

  cond_expr = build2 (EQ_EXPR, boolean_type_node,
		     integer_one_node, integer_one_node);

  nloop = tree_ssa_loop_version (loops, loop, cond_expr, bb);
  if (nloop)
    second_loop_vers_available = true;
  else
    second_loop_vers_available = false;
}
/* APPLE LOCAL end AV if-conversion --dpatel  */


/* Function vectorize_loops.
   
   Entry Point to loop vectorization phase.  */

void
vectorize_loops (struct loops *loops)
{
  unsigned int i, loops_num;
  unsigned int num_vectorized_loops = 0;

  /* Does the target support SIMD?  */
  /* FORNOW: until more sophisticated machine modelling is in place.  */
  if (!UNITS_PER_SIMD_WORD)
    {
      if (vect_debug_details (NULL))
	fprintf (dump_file, "vectorizer: target vector size is not defined.");
      return;
    }

  compute_immediate_uses (TDFA_USE_OPS, need_imm_uses_for);

  /*  ----------- Analyze loops. -----------  */

  /* If some loop was duplicated, it gets bigger number 
     than all previously defined loops. This fact allows us to run 
     only over initial loops skipping newly generated ones.  */
  loops_num = loops->num;
  for (i = 1; i < loops_num; i++)
    {
      loop_vec_info loop_vinfo;
      /* APPLE LOCAL AV if-conversion --dpatel  */
      basic_block bb;
      struct loop *loop = loops->parray[i];

      /* APPLE LOCAL begin AV if-conversion --dpatel  */
      /* Create second version of the loop in advance. This allows vectorizer
  	 to be more aggressive.  */
      vect_loop_version (loops, loop, &bb);
      if_converted_loop = false;
      /* APPLE LOCAL end AV if-conversion --dpatel  */
       
       flow_loop_scan (loop, LOOP_ALL);

      loop_vinfo = vect_analyze_loop (loop);
      loop->aux = loop_vinfo;

#ifndef ANALYZE_ALL_THEN_VECTORIZE_ALL
      if (!loop_vinfo || !LOOP_VINFO_VECTORIZABLE_P (loop_vinfo))
	/* APPLE LOCAL begin AV if-conversion --dpatel  */
  	{
  	  if (second_loop_vers_available)
  	    {
  	      if (vect_debug_details (NULL))
  		fprintf (dump_file, "removing second loop version.");
  	      update_lv_condition (&bb, boolean_false_node);
	    }
  	  continue;
  	}

      if (second_loop_vers_available)
  	{
  	  if (vect_debug_details (NULL))
  	    fprintf (dump_file, "vectorizing first loop version.");
  	}

      vect_transform_loop (loop_vinfo, loops); 
      num_vectorized_loops++;

      if (second_loop_vers_available)
  	{
 	  if_converted_loop = false;
  	  rewrite_into_ssa (false);
  	  bitmap_clear (vars_to_rename);
 	  rewrite_into_loop_closed_ssa ();
  	}
      /* APPLE LOCAL end AV if-conversion --dpatel  */
#endif
    }

#ifdef ANALYZE_ALL_THEN_VECTORIZE_ALL
  for (i = 1; i < loops_num; i++)
    {
      struct loop *loop = loops->parray[i];
      loop_vec_info loop_vinfo = loop->aux;

      if (!loop_vinfo || !LOOP_VINFO_VECTORIZABLE_P (loop_vinfo))
	continue;

      vect_transform_loop (loop_vinfo,loops);
      num_vectorized_loops++;
    }
#endif

  if (vect_debug_stats (NULL) || vect_debug_details (NULL))
    fprintf (dump_file, "\nvectorized %u loops in function.\n",
	     num_vectorized_loops);

  /*  ----------- Finalize. -----------  */

  free_df ();
  for (i = 1; i < loops_num; i++)
    {
      struct loop *loop = loops->parray[i];
      loop_vec_info loop_vinfo = loop->aux;
      destroy_loop_vec_info (loop_vinfo);
      loop->aux = NULL;
    }
}
