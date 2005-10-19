/* Analysis Utilities for Loop Vectorization.
   Copyright (C) 2003,2004,2005 Free Software Foundation, Inc.
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
#include "tree-chrec.h"
#include "tree-data-ref.h"
#include "tree-scalar-evolution.h"
#include "tree-vectorizer.h"
#include "recog.h"

/* Main analysis functions.  */
static loop_vec_info vect_analyze_loop_form (struct loop *);
static bool vect_analyze_data_refs (loop_vec_info);
static bool vect_mark_stmts_to_be_vectorized (loop_vec_info);
static void vect_analyze_scalar_cycles (loop_vec_info);
static bool vect_analyze_data_ref_accesses (loop_vec_info);
static bool vect_analyze_data_ref_dependences (loop_vec_info);
static bool vect_compute_data_refs_alignment (loop_vec_info);
static bool vect_enhance_data_refs_alignment (loop_vec_info);
static bool vect_analyze_operations (loop_vec_info);
static bool vect_determine_vectorization_factor (loop_vec_info);
static void vect_pattern_recog (loop_vec_info);

/* Utility functions for the analyses.  */
static bool exist_non_indexing_operands_for_use_p (tree, tree);
static void vect_mark_relevant (VEC(tree,heap) **, tree, bool, bool);
static bool vect_stmt_relevant_p (tree, loop_vec_info, bool *, bool *);
static tree vect_get_loop_niters (struct loop *, tree *);
static bool vect_analyze_data_ref_dependence
  (struct data_dependence_relation *, loop_vec_info);
static bool vect_compute_data_ref_alignment (struct data_reference *); 
static bool vect_analyze_data_ref_access (struct data_reference *);
static bool vect_can_advance_ivs_p (loop_vec_info);
static void vect_update_misalignment_for_peel
  (struct data_reference *, struct data_reference *, int npeel);
static bool widened_name_p (tree, tree, tree *, tree *);

/* Pattern recognition functions  */
tree vect_recog_unsigned_subsat_pattern (tree, tree *, varray_type *);
_recog_func_ptr vect_pattern_recog_funcs[NUM_PATTERNS] = {
        vect_recog_unsigned_subsat_pattern,
	vect_recog_widen_mult_pattern,
	vect_recog_mult_hi_pattern,
	vect_recog_dot_prod_pattern,
	vect_recog_widen_sum_pattern};

/* Function vect_determine_vectorization_factor

   Determine the vectorization factor (VF). VF is the number of data elements
   that are operated upon in parallel in a single iteration of the vectorized
   loop. For example, when vectorizing a loop that operates on 4byte elements,
   on a target with vector size (VS) 16byte, the VF is set to 4, since 4
   elements can fit in a single vector register.
   
   We currently support vectorization of loops in which all types operated upon
   are of the same size. Therefore this function currently sets VF according to
   the size of the types operated upon, and fails if there are multiple sizes
   in the loop.
         
   VF is also the factor by which the loop iterations are strip-mined, e.g.:
   original loop:
        for (i=0; i<N; i++){
          a[i] = b[i] + c[i];
        }

   vectorized loop:
        for (i=0; i<N; i+=VF){
          a[i:VF] = b[i:VF] + c[i:VF];
        }
*/

static bool
vect_determine_vectorization_factor (loop_vec_info loop_vinfo)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  basic_block *bbs = LOOP_VINFO_BBS (loop_vinfo);
  int nbbs = loop->num_nodes;
  block_stmt_iterator si;
  unsigned int vectorization_factor = 0;
  int i;
  tree scalar_type;

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "=== vect_determine_vectorization_factor ===");

  for (i = 0; i < nbbs; i++)
    {
      basic_block bb = bbs[i];

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
        {
          tree stmt = bsi_stmt (si);
          unsigned int nunits;
          stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
          tree vectype;

          if (vect_print_dump_info (REPORT_DETAILS))
            {
              fprintf (vect_dump, "==> examining statement: ");
              print_generic_expr (vect_dump, stmt, TDF_SLIM);
            }

          gcc_assert (stmt_info);
          /* skip stmts which do not need to be vectorized.  */
          if (!STMT_VINFO_RELEVANT_P (stmt_info)
	      && !STMT_VINFO_LIVE_P (stmt_info))
            {
              if (vect_print_dump_info (REPORT_DETAILS))
                fprintf (vect_dump, "skip.");
              continue;
            }

          if (VECTOR_MODE_P (TYPE_MODE (TREE_TYPE (stmt))))
            {
              if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
                {
                  fprintf (vect_dump, "not vectorized: vector stmt in loop:");
                  print_generic_expr (vect_dump, stmt, TDF_SLIM);
                }
              return false;
            }
	
	  if (STMT_VINFO_VECTYPE (stmt_info))
            {
              vectype = STMT_VINFO_VECTYPE (stmt_info);
              scalar_type = TREE_TYPE (vectype);
            }
          else
            {
              if (STMT_VINFO_DATA_REF (stmt_info))
                scalar_type = 
			TREE_TYPE (DR_REF (STMT_VINFO_DATA_REF (stmt_info)));
              else if (TREE_CODE (stmt) == MODIFY_EXPR)
                scalar_type = TREE_TYPE (TREE_OPERAND (stmt, 0));
              else
                scalar_type = TREE_TYPE (stmt);

              if (vect_print_dump_info (REPORT_DETAILS))
                {
                  fprintf (vect_dump, "get vectype for scalar type:  ");
                  print_generic_expr (vect_dump, scalar_type, TDF_SLIM);
                }

              vectype = get_vectype_for_scalar_type (scalar_type);
              if (!vectype)
                {
                  if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
                    {
                      fprintf (vect_dump, 
				"not vectorized: unsupported data-type ");
                      print_generic_expr (vect_dump, scalar_type, TDF_SLIM);
                    }
                  return false;
                }
              STMT_VINFO_VECTYPE (stmt_info) = vectype;
	    }

          if (vect_print_dump_info (REPORT_DETAILS))
            {
              fprintf (vect_dump, "vectype: ");
              print_generic_expr (vect_dump, vectype, TDF_SLIM);
            }

          nunits = TYPE_VECTOR_SUBPARTS (vectype);
          if (vect_print_dump_info (REPORT_DETAILS))
            fprintf (vect_dump, "nunits = %d", nunits);
	
          if (!vectorization_factor 
	      || (nunits > vectorization_factor))
	    vectorization_factor = nunits;
#if 0
          gcc_assert (GET_MODE_SIZE (TYPE_MODE (scalar_type))
                        * vectorization_factor == UNITS_PER_SIMD_WORD);
#endif
        }
    }

  /* TODO: Analyze cost. Decide if worth while to vectorize.  */

  if (vectorization_factor <= 1)
    {
      if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
        fprintf (vect_dump, "not vectorized: unsupported data-type");
      return false;
    }
  LOOP_VINFO_VECT_FACTOR (loop_vinfo) = vectorization_factor;

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
  unsigned int vectorization_factor = 0;
  int i;
  bool ok;
  tree phi;
  stmt_vec_info stmt_info;
  bool need_to_vectorize = false;

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "=== vect_analyze_operations ===");

  gcc_assert (LOOP_VINFO_VECT_FACTOR (loop_vinfo));
  vectorization_factor = LOOP_VINFO_VECT_FACTOR (loop_vinfo);

  for (i = 0; i < nbbs; i++)
    {
      basic_block bb = bbs[i];

      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
        {
	  stmt_info = vinfo_for_stmt (phi);
	  if (vect_print_dump_info (REPORT_DETAILS))
	    {
	      fprintf (vect_dump, "examining phi: ");
	      print_generic_expr (vect_dump, phi, TDF_SLIM);
	    }

	  gcc_assert (stmt_info);

	  if (STMT_VINFO_LIVE_P (stmt_info))
	    {
	      /* FORNOW: not yet supported.  */
	      if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
		fprintf (vect_dump, "not vectorized: value used after loop.");
	    return false;
	  }

	  if (STMT_VINFO_RELEVANT_P (stmt_info))
	    {
	      /* Most likely a reduction-like computation that is used
	         in the loop.  */
	      if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
	        fprintf (vect_dump, "not vectorized: unsupported pattern.");
 	     return false;
	    }
	}

      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  tree stmt = bsi_stmt (si);
          stmt_vec_info stmt_info = vinfo_for_stmt (stmt);

	  if (vect_print_dump_info (REPORT_DETAILS))
	    {
	      fprintf (vect_dump, "examining statement: ");
	      print_generic_expr (vect_dump, stmt, TDF_SLIM);
	    }

	  gcc_assert (stmt_info);

	  /* skip stmts which do not need to be vectorized.
	     this is expected to include:
	     - the COND_EXPR which is the loop exit condition
	     - any LABEL_EXPRs in the loop
	     - computations that are used only for array indexing or loop
	     control  */

	  if (!STMT_VINFO_RELEVANT_P (stmt_info)
	      && !STMT_VINFO_LIVE_P (stmt_info))
	    {
	      if (vect_print_dump_info (REPORT_DETAILS))
	        fprintf (vect_dump, "irrelevant.");
	      continue;
	    }

          if (STMT_VINFO_RELEVANT_P (stmt_info))
            {
              gcc_assert (!VECTOR_MODE_P (TYPE_MODE (TREE_TYPE (stmt))));
              gcc_assert (STMT_VINFO_VECTYPE (stmt_info));
 
              ok = (vectorizable_type_promotion (stmt, NULL, NULL)
		    || vectorizable_type_demotion (stmt, NULL, NULL)
		    || vectorizable_operation (stmt, NULL, NULL)
		    || vectorizable_assignment (stmt, NULL, NULL)
		    || vectorizable_load (stmt, NULL, NULL)
		    || vectorizable_store (stmt, NULL, NULL)
		    || vectorizable_condition (stmt, NULL, NULL));

              if (!ok)
                {
                  if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
                    {
                      fprintf (vect_dump,
                               "not vectorized: relevant stmt not supported: ");
                      print_generic_expr (vect_dump, stmt, TDF_SLIM);
                    }
                  return false;
                }
              need_to_vectorize = true;
            }
          if (STMT_VINFO_LIVE_P (stmt_info)) 
            { 
              ok = vectorizable_target_reduction_pattern (stmt, NULL, NULL)
		   || vectorizable_reduction (stmt, NULL, NULL);

	      if (ok)
		need_to_vectorize = true;
	      else
		ok = vectorizable_live_operation (stmt, NULL, NULL);
 
              if (!ok)
                {
                  if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
                    {
                      fprintf (vect_dump,  
                               "not vectorized: live stmt not supported: ");
                      print_generic_expr (vect_dump, stmt, TDF_SLIM);
                    }
                  return false;
                }
            }
	} /* stmts in bb */
    } /* bbs */

  /* TODO: Analyze cost. Decide if worth while to vectorize.  */

  /* All operations in the loop are either irrelevant (deal with loop
     control, or dead), or only used outside the loop and can be moved
     out of the loop (e.g. invariants, inductions).  The loop can be 
     optimized away by scalar optimizations.  We're better off not 
     touching this loop.  */
  if (!need_to_vectorize)
    {
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, 
		 "All the computation can be taken out of the loop.");
      if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
        fprintf (vect_dump, 
		 "not vectorized: redundant loop. no profit to vectorize.");
      return false;
    }

  if (LOOP_VINFO_NITERS_KNOWN_P (loop_vinfo)
      && vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump,
        "vectorization_factor = %d, niters = " HOST_WIDE_INT_PRINT_DEC,
        vectorization_factor, LOOP_VINFO_INT_NITERS (loop_vinfo));

  if (LOOP_VINFO_NITERS_KNOWN_P (loop_vinfo)
      && LOOP_VINFO_INT_NITERS (loop_vinfo) < vectorization_factor)
    {
      if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
	fprintf (vect_dump, "not vectorized: iteration count too small.");
      return false;
    }

  if (!LOOP_VINFO_NITERS_KNOWN_P (loop_vinfo)
      || LOOP_VINFO_INT_NITERS (loop_vinfo) % vectorization_factor != 0
      || LOOP_PEELING_FOR_ALIGNMENT (loop_vinfo))
    {
      if (vect_print_dump_info (REPORT_DETAILS))
        fprintf (vect_dump, "epilog loop required.");
      if (!vect_can_advance_ivs_p (loop_vinfo))
        {
          if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
            fprintf (vect_dump,
                     "not vectorized: can't create epilog loop 1.");
          return false;
        }
      if (!slpeel_can_duplicate_loop_p (loop, loop->single_exit))
        {
          if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
            fprintf (vect_dump,
                     "not vectorized: can't create epilog loop 2.");
          return false;
        }
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

  if (operand == use)
    return true;

  return false;
}


/* Function vect_analyze_scalar_cycles.

   Examine the cross iteration def-use cycles of scalar variables, by
   analyzing the loop (scalar) PHIs; Classify each cycle as one of the
   following: invariant, induction, reduction, unknown.
   
   Some forms of scalar cycles are not yet supported.

   Example1: reduction: (unsupported yet)

              loop1:
              for (i=0; i<N; i++)
                 sum += a[i];

   Example2: induction: (unsupported yet)

              loop2:
              for (i=0; i<N; i++)
                 a[i] = i;

   Note: the following loop *is* vectorizable:

              loop3:
              for (i=0; i<N; i++)
                 a[i] = b[i];

         even though it has a def-use cycle caused by the induction variable i:

              loop: i_2 = PHI (i_0, i_1)
                    a[i_2] = ...;
                    i_1 = i_2 + 1;
                    GOTO loop;

         because the def-use cycle in loop3 is considered "not relevant" - i.e.,
         it does not need to be vectorized because it is only used for array
         indexing (see 'mark_stmts_to_be_vectorized'). The def-use cycle in
         loop2 on the other hand is relevant (it is being written to memory).
*/

static void
vect_analyze_scalar_cycles (loop_vec_info loop_vinfo)
{
  tree phi;
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  basic_block bb = loop->header;
  tree dummy;

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "=== vect_analyze_scalar_cycles ===");

  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      tree access_fn = NULL;
      tree def = PHI_RESULT (phi);
      stmt_vec_info stmt_vinfo = vinfo_for_stmt (phi);
      tree reduc_stmt;

      if (vect_print_dump_info (REPORT_DETAILS))
	{
          fprintf (vect_dump, "Analyze phi: ");
          print_generic_expr (vect_dump, phi, TDF_SLIM);
	}

      /* Skip virtual phi's. The data dependences that are associated with
         virtual defs/uses (i.e., memory accesses) are analyzed elsewhere.  */

      if (!is_gimple_reg (SSA_NAME_VAR (def)))
	{
	  if (vect_print_dump_info (REPORT_DETAILS))
	    fprintf (vect_dump, "virtual phi. skip.");
          continue;
        }

      STMT_VINFO_DEF_TYPE (stmt_vinfo) = vect_unknown_def_type;

      /* Analyze the evolution function.  */

      access_fn = analyze_scalar_evolution (loop, def);

      if (!access_fn)
	continue;

      if (vect_print_dump_info (REPORT_DETAILS))
        {
           fprintf (vect_dump, "Access function of PHI: ");
           print_generic_expr (vect_dump, access_fn, TDF_SLIM);
        }

      if (vect_is_simple_iv_evolution (loop->num, access_fn, &dummy, &dummy))
	{
	  if (vect_print_dump_info (REPORT_DETAILS))
	    fprintf (vect_dump, "Detected induction.");
	  STMT_VINFO_DEF_TYPE (stmt_vinfo) = vect_induction_def;
          continue;
	}

      /* TODO: handle invariant phis  */

      reduc_stmt = vect_is_simple_reduction (loop, phi);
      if (reduc_stmt)
        {
          if (vect_print_dump_info (REPORT_DETAILS))
            fprintf (vect_dump, "Detected reduction.");
          STMT_VINFO_DEF_TYPE (stmt_vinfo) = vect_reduction_def;
          STMT_VINFO_DEF_TYPE (vinfo_for_stmt (reduc_stmt)) =
                                                        vect_reduction_def;
        }
      else
        if (vect_print_dump_info (REPORT_DETAILS))
          fprintf (vect_dump, "Unknown def-use cycle pattern.");
   }
}


/* Function vect_analyze_data_ref_dependence.

   Return TRUE if there (might) exist a dependence between a memory-reference
   DRA and a memory-reference DRB of DDR.  */

static bool
vect_analyze_data_ref_dependence (struct data_dependence_relation *ddr,
                                  loop_vec_info loop_vinfo)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  int vectorization_factor = LOOP_VINFO_VECT_FACTOR (loop_vinfo);
  int dist = 0;
  unsigned int loop_depth = 0;
  struct loop *loop_nest = loop;
  struct data_reference *dra = DDR_A (ddr);
  struct data_reference *drb = DDR_B (ddr);
  stmt_vec_info stmtinfo_a = vinfo_for_stmt (DR_STMT (dra)); 
  stmt_vec_info stmtinfo_b = vinfo_for_stmt (DR_STMT (drb));
  int dra_size = GET_MODE_SIZE (TYPE_MODE (TREE_TYPE (DR_REF (dra))));
  int drb_size = GET_MODE_SIZE (TYPE_MODE (TREE_TYPE (DR_REF (drb))));
         
  if (DDR_ARE_DEPENDENT (ddr) == chrec_known)
    return false;
  
  if (DDR_ARE_DEPENDENT (ddr) == chrec_dont_know)
    {
      if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
        {
          fprintf (vect_dump,
                   "not vectorized: can't determine dependence between ");
          print_generic_expr (vect_dump, DR_REF (dra), TDF_SLIM);
          fprintf (vect_dump, " and ");
          print_generic_expr (vect_dump, DR_REF (drb), TDF_SLIM);
        }
      return true;
    }

  if (!DDR_DIST_VECT (ddr))
    {
      if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
        {
          fprintf (vect_dump, "not vectorized: bad dist vector for ");
          print_generic_expr (vect_dump, DR_REF (dra), TDF_SLIM);
          fprintf (vect_dump, " and ");
          print_generic_expr (vect_dump, DR_REF (drb), TDF_SLIM);
        }
      return true;
    }    

  /* Find loop depth.  */
  while (loop_nest && loop_nest->outer && loop_nest->outer->outer)
    {
      loop_nest = loop_nest->outer;
      loop_depth++;
    }
         
  dist = DDR_DIST_VECT (ddr)[loop_depth];
  if (vect_print_dump_info (REPORT_DR_DETAILS))
    fprintf (vect_dump, "dependence distance  = %d.",dist);

  /* Same loop iteration.  */
  if (dist % vectorization_factor == 0 && dra_size == drb_size)
    {
      /* Two references with distance zero have the same alignment.  */
      VEC_safe_push (dr_p, heap, STMT_VINFO_SAME_ALIGN_REFS (stmtinfo_a), drb);
      VEC_safe_push (dr_p, heap, STMT_VINFO_SAME_ALIGN_REFS (stmtinfo_b), dra);
      if (vect_print_dump_info (REPORT_ALIGNMENT))
        fprintf (vect_dump, "accesses have the same alignment.");
      if (vect_print_dump_info (REPORT_DR_DETAILS))
        {
          fprintf (vect_dump, "dependence distance modulo vf == 0 between ");
          print_generic_expr (vect_dump, DR_REF (dra), TDF_SLIM);
          fprintf (vect_dump, " and ");
          print_generic_expr (vect_dump, DR_REF (drb), TDF_SLIM);
        }
      return false;
    }    

  if (abs (dist) >= vectorization_factor)
    {
      /* Dependence distance does not create dependence, as far as vectorization
         is concerned, in this case.  */
      if (vect_print_dump_info (REPORT_DR_DETAILS))
        fprintf (vect_dump, "dependence distance >= VF.");
       return false;
    }
  
  if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
    {
      fprintf (vect_dump,
        "not vectorized: possible dependence between data-refs ");
      print_generic_expr (vect_dump, DR_REF (dra), TDF_SLIM);
      fprintf (vect_dump, " and ");
      print_generic_expr (vect_dump, DR_REF (drb), TDF_SLIM);
    }
        
  return true;
}


/* Function vect_analyze_data_ref_dependences.
          
   Examine all the data references in the loop, and make sure there do not
   exist any data dependences between them.  */
         
static bool
vect_analyze_data_ref_dependences (loop_vec_info loop_vinfo)
{
  unsigned int i;
  varray_type ddrs = LOOP_VINFO_DDRS (loop_vinfo);

  if (vect_print_dump_info (REPORT_DETAILS)) 
    fprintf (vect_dump, "=== vect_analyze_dependences ===");
     
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ddrs); i++)
    {
      struct data_dependence_relation *ddr = VARRAY_GENERIC_PTR (ddrs, i);
     
      if (vect_analyze_data_ref_dependence (ddr, loop_vinfo))
        return false;
    }
  return true;
}


/* Function vect_compute_data_ref_alignment

   Compute the misalignment of the data reference DR.

   Output:
   1. If during the misalignment computation it is found that the data reference
      cannot be vectorized then false is returned.
   2. DR_MISALIGNMENT (DR) is defined.

   FOR NOW: No analysis is actually performed. Misalignment is calculated
   only for trivial cases. TODO.  */

static bool
vect_compute_data_ref_alignment (struct data_reference *dr)
{
  tree stmt = DR_STMT (dr);
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);  
  tree ref = DR_REF (dr);
  tree vectype;
  tree base, base_addr;
  bool base_aligned;
  tree misalign;
  tree aligned_to, alignment;
   
  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "vect_compute_data_ref_alignment:");

  /* Initialize misalignment to unknown.  */
  DR_MISALIGNMENT (dr) = -1;

  misalign = DR_OFFSET_MISALIGNMENT (dr);
  aligned_to = DR_ALIGNED_TO (dr);
  base_addr = DR_BASE_ADDRESS (dr);
  base = build_fold_indirect_ref (base_addr);
  vectype = STMT_VINFO_VECTYPE (stmt_info);
  alignment = ssize_int (TYPE_ALIGN (vectype)/BITS_PER_UNIT);

  if ((aligned_to && tree_int_cst_compare (aligned_to, alignment) < 0)
      || !misalign)
    {
      if (vect_print_dump_info (REPORT_DETAILS))
	{
	  fprintf (vect_dump, "Unknown alignment for access: ");
	  print_generic_expr (vect_dump, base, TDF_SLIM);
	}
      return true;
    }

  if ((DECL_P (base) 
       && tree_int_cst_compare (ssize_int (DECL_ALIGN_UNIT (base)),
				alignment) >= 0)
      || (TREE_CODE (base_addr) == SSA_NAME
	  && tree_int_cst_compare (ssize_int (TYPE_ALIGN_UNIT (TREE_TYPE (
						      TREE_TYPE (base_addr)))),
				   alignment) >= 0))
    base_aligned = true;
  else
    base_aligned = false;   

  if (!base_aligned) 
    {
      if (!vect_can_force_dr_alignment_p (base, TYPE_ALIGN (vectype)))
	{
	  if (vect_print_dump_info (REPORT_DETAILS))
	    {
	      fprintf (vect_dump, "can't force alignment of ref: ");
	      print_generic_expr (vect_dump, ref, TDF_SLIM);
	    }
	  return true;
	}
      
      /* Force the alignment of the decl.
	 NOTE: This is the only change to the code we make during
	 the analysis phase, before deciding to vectorize the loop.  */
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, "force alignment");
      DECL_ALIGN (base) = TYPE_ALIGN (vectype);
      DECL_USER_ALIGN (base) = 1;
    }

  /* At this point we assume that the base is aligned.  */
  gcc_assert (base_aligned
	      || (TREE_CODE (base) == VAR_DECL 
		  && DECL_ALIGN (base) >= TYPE_ALIGN (vectype)));

  /* Modulo alignment.  */
  misalign = size_binop (TRUNC_MOD_EXPR, misalign, alignment);

  if (tree_int_cst_sgn (misalign) < 0)
    {
      /* Negative misalignment value.  */
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, "unexpected misalign value");
      return false;
    }

  DR_MISALIGNMENT (dr) = tree_low_cst (misalign, 1);

  if (vect_print_dump_info (REPORT_DETAILS))
    {
      fprintf (vect_dump, "misalign = %d bytes of ref ", DR_MISALIGNMENT (dr));
      print_generic_expr (vect_dump, ref, TDF_SLIM);
    }

  return true;
}


/* Function vect_compute_data_refs_alignment

   Compute the misalignment of data references in the loop.
   Return FALSE if a data reference is found that cannot be vectorized.  */

static bool
vect_compute_data_refs_alignment (loop_vec_info loop_vinfo)
{
  varray_type datarefs = LOOP_VINFO_DATAREFS (loop_vinfo);
  unsigned int i;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (datarefs); i++)
    {
      struct data_reference *dr = VARRAY_GENERIC_PTR (datarefs, i);
      if (!vect_compute_data_ref_alignment (dr))
	return false;
    }

  return true;
}


/* Function vect_update_misalignment_for_peel

   DR - the data reference whose misalignment is to be adjusted.
   DR_PEEL - the data reference whose misalignment is being made
             zero in the vector loop by the peel.
   NPEEL - the number of iterations in the peel loop if the misalignment
           of DR_PEEL is known at compile time.  */

static void
vect_update_misalignment_for_peel (struct data_reference *dr,
                                   struct data_reference *dr_peel, int npeel)
{
  unsigned int i;
  VEC(dr_p,heap) *same_align_drs;
  struct data_reference *current_dr;
  int dr_size = GET_MODE_SIZE (TYPE_MODE (TREE_TYPE (DR_REF (dr))));
  int dr_peel_size = GET_MODE_SIZE (TYPE_MODE (TREE_TYPE (DR_REF (dr_peel))));

  if (known_alignment_for_access_p (dr)
      && known_alignment_for_access_p (dr_peel)
      && (DR_MISALIGNMENT (dr)/dr_size == 
	  DR_MISALIGNMENT (dr_peel)/dr_peel_size))
    {
      DR_MISALIGNMENT (dr) = 0;
      return;
    }

  /* It can be assumed that the data refs with the same alignment as dr_peel
     are aligned in the vector loop.  */
  same_align_drs
    = STMT_VINFO_SAME_ALIGN_REFS (vinfo_for_stmt (DR_STMT (dr_peel)));
  for (i = 0; VEC_iterate (dr_p, same_align_drs, i, current_dr); i++)
    {
      if (current_dr != dr)
        continue;
      gcc_assert (DR_MISALIGNMENT (dr)/dr_size == 
		  DR_MISALIGNMENT (dr_peel)/dr_peel_size);
      DR_MISALIGNMENT (dr) = 0;
      return;
    }

  if (known_alignment_for_access_p (dr)
      && known_alignment_for_access_p (dr_peel))
    {  
      DR_MISALIGNMENT (dr) += npeel * dr_size;
      DR_MISALIGNMENT (dr) %= UNITS_PER_SIMD_WORD;
      return;
    }

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "Setting misalignment to -1.");
  DR_MISALIGNMENT (dr) = -1;
}


/* Function vect_verify_datarefs_alignment

   Return TRUE if all data references in the loop can be
   handled with respect to alignment.  */

static bool
vect_verify_datarefs_alignment (loop_vec_info loop_vinfo)
{
  varray_type datarefs = LOOP_VINFO_DATAREFS (loop_vinfo);
  enum dr_alignment_support supportable_dr_alignment;
  unsigned int i;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (datarefs); i++)
    {
      struct data_reference *dr = VARRAY_GENERIC_PTR (datarefs, i);
      supportable_dr_alignment = vect_supportable_dr_alignment (dr);
      if (!supportable_dr_alignment)
        {
          if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
            {
              if (DR_IS_READ (dr))
                fprintf (vect_dump, 
                         "not vectorized: unsupported unaligned load.");
              else
                fprintf (vect_dump, 
                         "not vectorized: unsupported unaligned store.");
            }
          return false;
        }
      if (supportable_dr_alignment != dr_aligned
          && vect_print_dump_info (REPORT_ALIGNMENT))
        fprintf (vect_dump, "Vectorizing an unaligned access.");
    }
  return true;
}


/* Function vect_enhance_data_refs_alignment

   This pass will use loop versioning and loop peeling in order to enhance
   the alignment of data references in the loop.

   FOR NOW: we assume that whatever versioning/peeling takes place, only the
   original loop is to be vectorized; Any other loops that are created by
   the transformations performed in this pass - are not supposed to be
   vectorized. This restriction will be relaxed.

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

   Here are the general steps involved in alignment enhancements:

     -- original loop, before alignment analysis:
        for (i=0; i<N; i++){
          x = q[i];                     # DR_MISALIGNMENT(q) = unknown
          p[i] = x;                     # DR_MISALIGNMENT(p) = unknown

        }

     -- After vect_compute_data_refs_alignment:
        for (i=0; i<N; i++){
          x = q[i];                     # DR_MISALIGNMENT(q) = 3
          p[i] = x;                     # DR_MISALIGNMENT(p) = unknown
        }

     -- Possibility 1: we do loop versioning:
     if (p is aligned) {
        for (i=0; i<N; i++){    # loop 1A
          x = q[i];                     # DR_MISALIGNMENT(q) = 3
	  p[i] = y;			# DR_MISALIGNMENT(p) = 0
	}
     }
     else {
        for (i=0; i<N; i++){    # loop 1B
          x = q[i];                     # DR_MISALIGNMENT(q) = 3
          p[i] = x;                     # DR_MISALIGNMENT(p) = unaligned
        }
     }

     -- Possibility 2: we do loop peeling:
     for (i = 0; i < 3; i++){   # (scalar loop, not to be vectorized).
        x = q[i];
        p[i] = x;
     }
     for (i = 3; i < N; i++){   # loop 2A
        x = q[i];                       # DR_MISALIGNMENT(q) = 0
        p[i] = x;                       # DR_MISALIGNMENT(p) = unknown
     }

     -- Possibility 3: combination of loop peeling and versioning:
     for (i = 0; i < 3; i++){   # (scalar loop, not to be vectorized).
        x = q[i];
        p[i] = x;
     }
     if (p is aligned) {
	for (i = 3; i<N; i++){	# loop 3A
	  x = q[i];			# DR_MISALIGNMENT(q) = 0
	  p[i] = y;			# DR_MISALIGNMENT(p) = 0
	}
     }
     else {
        for (i = 3; i<N; i++){  # loop 3B
          x = q[i];                     # DR_MISALIGNMENT(q) = 0
          p[i] = x;                     # DR_MISALIGNMENT(p) = unaligned

     These loops are later passed to loop_transform to be vectorized. The
     vectorizer will use the alignment information to guide the transformation
     (whether to generate regular loads/stores, or with special handling for
     misalignment).  */

static bool
vect_enhance_data_refs_alignment (loop_vec_info loop_vinfo)
{
  varray_type datarefs = LOOP_VINFO_DATAREFS (loop_vinfo);
  enum dr_alignment_support supportable_dr_alignment;
  struct data_reference *dr0 = NULL;
  struct data_reference *dr;
  unsigned int i;
  bool do_peeling = false;
  bool do_versioning = false;
  bool stat;

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "=== vect_enhance_data_refs_alignment ===");

  /* While cost model enhancements are expected in the future, the high level
     view of the code at this time is as follows:

     A) If there is a misaligned write then see if peeling to align this write
        can make all data references satisfy vect_supportable_dr_alignment.
        If so, update data structures as needed and return true.  Note that
        at this time vect_supportable_dr_alignment is known to return false
        for a a misaligned write.

     B) If peeling wasn't possible and there is a data reference with an
        unknown misalignment that does not satisfy vect_supportable_dr_alignment
        then see if loop versioning checks can be used to make all data
        references satisfy vect_supportable_dr_alignment.  If so, update
        data structures as needed and return true.

     C) If neither peeling nor versioning were successful then return false if
        any data reference does not satisfy vect_supportable_dr_alignment.

     D) Return true (all data references satisfy vect_supportable_dr_alignment).

     Note, Possibility 3 above (which is peeling and versioning together) is not
     being done at this time.  */

  /* (1) Peeling to force alignment.  */

  /* (1.1) Decide whether to perform peeling, and how many iterations to peel:
     Considerations:
     + How many accesses will become aligned due to the peeling
     - How many accesses will become unaligned due to the peeling,
       and the cost of misaligned accesses.
     - The cost of peeling (the extra runtime checks, the increase 
       in code size).

     The scheme we use FORNOW: peel to force the alignment of the first
     misaligned store in the loop.
     Rationale: misaligned stores are not yet supported.

     TODO: Use a cost model.  */

  for (i = 0; i < VARRAY_ACTIVE_SIZE (datarefs); i++)
    {
      dr = VARRAY_GENERIC_PTR (datarefs, i);
      if (!DR_IS_READ (dr) && !aligned_access_p (dr))
        {
          dr0 = dr;
          do_peeling = true;
          break;
        }
    }

  /* Often peeling for alignment will require peeling for loop-bound, which in 
     turn requires that we know how to adjust the loop ivs after the loop.  */
  if (!vect_can_advance_ivs_p (loop_vinfo))
    do_peeling = false;

  if (do_peeling)
    {
      int mis;
      int npeel = 0;

      if (known_alignment_for_access_p (dr0))
        {
          /* Since it's known at compile time, compute the number of iterations
             in the peeled loop (the peeling factor) for use in updating
             DR_MISALIGNMENT values.  The peeling factor is the vectorization
             factor minus the misalignment as an element count.  */
          mis = DR_MISALIGNMENT (dr0);
          mis /= GET_MODE_SIZE (TYPE_MODE (TREE_TYPE (DR_REF (dr0))));
          npeel = LOOP_VINFO_VECT_FACTOR (loop_vinfo) - mis;
          if (vect_print_dump_info (REPORT_DETAILS))
            fprintf (vect_dump, "Try peeling by %d",npeel);
        }

      /* Ensure that all data refs can be vectorized after the peel.  */
      for (i = 0; i < VARRAY_ACTIVE_SIZE (datarefs); i++)
        {
          int save_misalignment;

	  dr = VARRAY_GENERIC_PTR (datarefs, i);
	  if (dr == dr0)
	    continue;
	  save_misalignment = DR_MISALIGNMENT (dr);
	  vect_update_misalignment_for_peel (dr, dr0, npeel);
	  supportable_dr_alignment = vect_supportable_dr_alignment (dr);
	  DR_MISALIGNMENT (dr) = save_misalignment;
	  
	  if (!supportable_dr_alignment)
	    {
	      do_peeling = false;
	      break;
	    }
	}

      if (do_peeling)
        {
          /* (1.2) Update the DR_MISALIGNMENT of each data reference DR_i.
             If the misalignment of DR_i is identical to that of dr0 then set
             DR_MISALIGNMENT (DR_i) to zero.  If the misalignment of DR_i and
             dr0 are known at compile time then increment DR_MISALIGNMENT (DR_i)
             by the peeling factor times the element size of DR_i (MOD the
             vectorization factor times the size).  Otherwise, the
             misalignment of DR_i must be set to unknown.  */
	  for (i = 0; i < VARRAY_ACTIVE_SIZE (datarefs); i++)
	    {
	      dr = VARRAY_GENERIC_PTR (datarefs, i);
	      if (dr == dr0)
	        continue;
	      vect_update_misalignment_for_peel (dr, dr0, npeel);
              if (vect_print_dump_info (REPORT_DETAILS))
                fprintf (vect_dump, "New alignment for dr = %d",
				    DR_MISALIGNMENT (dr));
	    }
          LOOP_VINFO_UNALIGNED_DR (loop_vinfo) = dr0;
          LOOP_PEELING_FOR_ALIGNMENT (loop_vinfo) = DR_MISALIGNMENT (dr0);
          DR_MISALIGNMENT (dr0) = 0;
	  if (vect_print_dump_info (REPORT_ALIGNMENT))
            fprintf (vect_dump, "Alignment of access forced using peeling.");

          if (vect_print_dump_info (REPORT_DETAILS))
            fprintf (vect_dump, "Peeling for alignment will be applied.");

	  stat = vect_verify_datarefs_alignment (loop_vinfo);
	  gcc_assert (stat);
          return stat;
        }
    }

  /* (2) Versioning to force alignment.  */

  /* Try versioning if:
     1) flag_tree_vect_loop_version is TRUE
     2) optimize_size is FALSE
     3) there is at least one unsupported misaligned data ref with an unknown
        misalignment, and
     4) all misaligned data refs with a known misalignment are supported, and
     5) the number of runtime alignment checks is within reason.  */

  do_versioning = flag_tree_vect_loop_version && (!optimize_size);

  if (do_versioning)
    {
      for (i = 0; i < VARRAY_ACTIVE_SIZE (datarefs); i++)
        {
          dr = VARRAY_GENERIC_PTR (datarefs, i);

          if (aligned_access_p (dr))
            continue;

          supportable_dr_alignment = vect_supportable_dr_alignment (dr);

          if (!supportable_dr_alignment)
            {
              tree stmt;
              int mask;
              tree vectype;

              if (known_alignment_for_access_p (dr)
                  || VEC_length (tree,
                                 LOOP_VINFO_MAY_MISALIGN_STMTS (loop_vinfo))
                     >= (unsigned) PARAM_VALUE (PARAM_VECT_MAX_VERSION_CHECKS))
                {
                  do_versioning = false;
                  break;
                }

              stmt = DR_STMT (dr);
              vectype = STMT_VINFO_VECTYPE (vinfo_for_stmt (stmt));
              gcc_assert (vectype);
  
              /* The rightmost bits of an aligned address must be zeros.
                 Construct the mask needed for this test.  For example,
                 GET_MODE_SIZE for the vector mode V4SI is 16 bytes so the
                 mask must be 15 = 0xf. */
              mask = GET_MODE_SIZE (TYPE_MODE (vectype)) - 1;

              /* FORNOW: use the same mask to test all potentially unaligned
                 references in the loop.  The vectorizer currently supports
                 a single vector size, see the reference to
                 GET_MODE_NUNITS (TYPE_MODE (vectype)) where the
                 vectorization factor is computed.  */
              gcc_assert (!LOOP_VINFO_PTR_MASK (loop_vinfo)
                          || LOOP_VINFO_PTR_MASK (loop_vinfo) == mask);
              LOOP_VINFO_PTR_MASK (loop_vinfo) = mask;
              VEC_safe_push (tree, heap,
                             LOOP_VINFO_MAY_MISALIGN_STMTS (loop_vinfo),
                             DR_STMT (dr));
            }
        }
      
      /* Versioning requires at least one misaligned data reference.  */
      if (VEC_length (tree, LOOP_VINFO_MAY_MISALIGN_STMTS (loop_vinfo)) == 0)
        do_versioning = false;
      else if (!do_versioning)
        VEC_truncate (tree, LOOP_VINFO_MAY_MISALIGN_STMTS (loop_vinfo), 0);
    }

  if (do_versioning)
    {
      VEC(tree,heap) *may_misalign_stmts
        = LOOP_VINFO_MAY_MISALIGN_STMTS (loop_vinfo);
      tree stmt;

      /* It can now be assumed that the data references in the statements
         in LOOP_VINFO_MAY_MISALIGN_STMTS will be aligned in the version
         of the loop being vectorized.  */
      for (i = 0; VEC_iterate (tree, may_misalign_stmts, i, stmt); i++)
        {
          stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
          dr = STMT_VINFO_DATA_REF (stmt_info);
          DR_MISALIGNMENT (dr) = 0;
	  if (vect_print_dump_info (REPORT_ALIGNMENT))
            fprintf (vect_dump, "Alignment of access forced using versioning.");
        }

      if (vect_print_dump_info (REPORT_DETAILS))
        fprintf (vect_dump, "Versioning for alignment will be applied.");

      /* Peeling and versioning can't be done together at this time.  */
      gcc_assert (! (do_peeling && do_versioning));

      stat = vect_verify_datarefs_alignment (loop_vinfo);
      gcc_assert (stat);
      return stat;
    }

  /* This point is reached if neither peeling nor versioning is being done.  */
  gcc_assert (! (do_peeling || do_versioning));

  stat = vect_verify_datarefs_alignment (loop_vinfo);
  return stat;
}


/* Function vect_analyze_data_refs_alignment

   Analyze the alignment of the data-references in the loop.
   Return FALSE if a data reference is found that cannot be vectorized.  */

static bool
vect_analyze_data_refs_alignment (loop_vec_info loop_vinfo)
{
  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "=== vect_analyze_data_refs_alignment ===");

  if (!vect_compute_data_refs_alignment (loop_vinfo))
    {
      if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
	fprintf (vect_dump, 
		 "not vectorized: can't calculate alignment for data ref.");
      return false;
    }

  return true;
}


/* Function vect_analyze_data_ref_access.

   Analyze the access pattern of the data-reference DR. For now, a data access
   has to be consecutive to be considered vectorizable.  */

static bool
vect_analyze_data_ref_access (struct data_reference *dr)
{
  tree step = DR_STEP (dr);
  tree scalar_type = TREE_TYPE (DR_REF (dr));

  /* FORNOW: Check that STEP is equal to type size. 
     In the future, we'll allow STEP to be -1 or multiple of type size.  */

  if (!step || tree_int_cst_compare (step, TYPE_SIZE_UNIT (scalar_type)))
    {
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, "not consecutive access");
      return false;
    }
  return true;
}


/* Function vect_analyze_data_ref_accesses.

   Analyze the access pattern of all the data references in the loop.

   FORNOW: the only access pattern that is considered vectorizable is a
	   simple step 1 (consecutive) access.

   FORNOW: handle only arrays and pointer accesses.  */

static bool
vect_analyze_data_ref_accesses (loop_vec_info loop_vinfo)
{
  unsigned int i;
  varray_type datarefs = LOOP_VINFO_DATAREFS (loop_vinfo);

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "=== vect_analyze_data_ref_accesses ===");

  for (i = 0; i < VARRAY_ACTIVE_SIZE (datarefs); i++)
    {
      struct data_reference *dr = VARRAY_GENERIC_PTR (datarefs, i);
      if (!vect_analyze_data_ref_access (dr))
	{
	  if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
	    fprintf (vect_dump, "not vectorized: complicated access pattern.");
	  return false;
	}
    }
  return true;
}


/* Function vect_analyze_data_refs.

  Find all the data references in the loop.

   The general structure of the analysis of data refs in the vectorizer is as
   follows:
   1- vect_analyze_data_refs(loop): call compute_data_dependences_for_loop to
      find and analyze all data-refs in the loop and their dependences.
   2- vect_analyze_dependences(): apply dependence testing using ddrs.
   3- vect_analyze_drs_alignment(): check that ref_stmt.alignment is ok.
   4- vect_analyze_drs_access(): check that ref_stmt.step is ok.

*/

static bool
vect_analyze_data_refs (loop_vec_info loop_vinfo)  
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  unsigned int i;
  varray_type datarefs;
  tree scalar_type;

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "=== vect_analyze_data_refs ===");

  compute_data_dependences_for_loop (loop, false,
                                     &(LOOP_VINFO_DATAREFS (loop_vinfo)),
                                     &(LOOP_VINFO_DDRS (loop_vinfo)));

  /* Go through the data-refs, check that the analysis succeeded. Update pointer
     from stmt_vec_info struct to DR and vectype.  */
  datarefs = LOOP_VINFO_DATAREFS (loop_vinfo);
  for (i = 0; i < VARRAY_ACTIVE_SIZE (datarefs); i++)
    {
      struct data_reference *dr = VARRAY_GENERIC_PTR (datarefs, i);
      tree stmt;
      stmt_vec_info stmt_info;
   
      if (!dr || !DR_REF (dr))
        {
          if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
	    fprintf (vect_dump, "not vectorized: unhandled data-ref ");
          return false;
        }
 
      /* Update DR field in stmt_vec_info struct.  */
      stmt = DR_STMT (dr);
      stmt_info = vinfo_for_stmt (stmt);
  
      if (STMT_VINFO_DATA_REF (stmt_info))
        {
          if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
            {
              fprintf (vect_dump,
                       "not vectorized: more than one data ref in stmt: ");
              print_generic_expr (vect_dump, stmt, TDF_SLIM);
            }
          return false;
        }
      STMT_VINFO_DATA_REF (stmt_info) = dr;
     
      /* Check that analysis of the data-ref succeeded.  */
      if (!DR_BASE_ADDRESS (dr) || !DR_OFFSET (dr) || !DR_INIT (dr)
          || !DR_STEP (dr))   
        {
          if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
            {
              fprintf (vect_dump, "not vectorized: data ref analysis failed ");
              print_generic_expr (vect_dump, stmt, TDF_SLIM);
            }
          return false;
        }
      if (!DR_MEMTAG (dr))
        {
          if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
            {
              fprintf (vect_dump, "not vectorized: no memory tag for ");
              print_generic_expr (vect_dump, DR_REF (dr), TDF_SLIM);
            }
          return false;
        }
                       
      /* Set vectype for STMT.  */
      scalar_type = TREE_TYPE (DR_REF (dr));
      STMT_VINFO_VECTYPE (stmt_info) =
                get_vectype_for_scalar_type (scalar_type);
      if (!STMT_VINFO_VECTYPE (stmt_info)) 
        {
          if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
            {
              fprintf (vect_dump,
                       "not vectorized: no vectype for stmt: ");
              print_generic_expr (vect_dump, stmt, TDF_SLIM);
              fprintf (vect_dump, " scalar_type: ");
              print_generic_expr (vect_dump, scalar_type, TDF_DETAILS);
            }
          return false;
        }
    }
  return true;
}
				     

/* Utility functions used by vect_mark_stmts_to_be_vectorized.  */

/* Function vect_mark_relevant.

   Mark STMT as "relevant for vectorization" and add it to WORKLIST.  */

static void
vect_mark_relevant (VEC(tree,heap) **worklist, tree stmt,
		    bool relevant_p, bool live_p)
{
  stmt_vec_info stmt_info = vinfo_for_stmt (stmt);
  bool save_relevant_p = STMT_VINFO_RELEVANT_P (stmt_info);
  bool save_live_p = STMT_VINFO_LIVE_P (stmt_info);

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "mark relevant %d, live %d.",relevant_p, live_p);

  if (STMT_VINFO_IN_PATTERN_P (stmt_info) 
      && STMT_VINFO_RELATED_STMT (stmt_info)
      && STMT_VINFO_RELATED_STMT (vinfo_for_stmt 
			(STMT_VINFO_RELATED_STMT (stmt_info))) == stmt)
    {
      /* This is the last stmt in a sequence that was detected as a 
	 pattern that can potentially be vectorized.  Don't mark the stmt
	 as relevant/live because it's not going to vectorized.
	 Instead mark the pattern-stmt that replaces it.  */
      if (vect_print_dump_info (REPORT_DETAILS))
        fprintf (vect_dump, "last stmt in pattern. don't mark relevant/live.");
      stmt = STMT_VINFO_RELATED_STMT (stmt_info);
      stmt_info = vinfo_for_stmt (stmt);
      save_relevant_p = STMT_VINFO_RELEVANT_P (stmt_info);
      save_live_p = STMT_VINFO_LIVE_P (stmt_info);
    }

  STMT_VINFO_LIVE_P (stmt_info) |= live_p;
  STMT_VINFO_RELEVANT_P (stmt_info) |= relevant_p;

  if (TREE_CODE (stmt) == PHI_NODE)
    /* Don't put phi-nodes in the worklist. Phis that are marked relevant
       or live will fail vectorization later on.  */
    return;

  if (STMT_VINFO_RELEVANT_P (stmt_info) == save_relevant_p
      && STMT_VINFO_LIVE_P (stmt_info) == save_live_p)
    {
      if (vect_print_dump_info (REPORT_DETAILS))
        fprintf (vect_dump, "already marked relevant/live.");
      return;
    }

  VEC_safe_push (tree, heap, *worklist, stmt);
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
vect_stmt_relevant_p (tree stmt, loop_vec_info loop_vinfo,
		      bool *relevant_p, bool *live_p)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  ssa_op_iter op_iter;
  imm_use_iterator imm_iter;
  use_operand_p use_p;
  def_operand_p def_p;

  *relevant_p = false;
  *live_p = false;

  *relevant_p = false;
  *live_p = false;

  /* cond stmt other than loop exit cond.  */
  if (is_ctrl_stmt (stmt) && (stmt != LOOP_VINFO_EXIT_COND (loop_vinfo)))
    *relevant_p = true;

  /* changing memory.  */
  if (TREE_CODE (stmt) != PHI_NODE)
    if (!ZERO_SSA_OPERANDS (stmt, SSA_OP_VIRTUAL_DEFS))
      {
	if (vect_print_dump_info (REPORT_DETAILS))
	  fprintf (vect_dump, "vec_stmt_relevant_p: stmt has vdefs.");
	*relevant_p = true;
      }

  /* uses outside the loop.  */
  FOR_EACH_PHI_OR_STMT_DEF (def_p, stmt, op_iter, SSA_OP_DEF)
    {
      FOR_EACH_IMM_USE_FAST (use_p, imm_iter, DEF_FROM_PTR (def_p))
	{
	  basic_block bb = bb_for_stmt (USE_STMT (use_p));
	  if (!flow_bb_inside_loop_p (loop, bb))
	    {
	      if (vect_print_dump_info (REPORT_DETAILS))
		fprintf (vect_dump, "vec_stmt_relevant_p: used out of loop.");

	      /* We expect all such uses to be in the loop exit phis
		 (because of loop closed form)   */
	      gcc_assert (TREE_CODE (USE_STMT (use_p)) == PHI_NODE);
	      gcc_assert (bb == loop->single_exit->dest);

              *live_p = true;
	    }
	}
    }

  return (*live_p || *relevant_p);
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
  VEC(tree,heap) *worklist;
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  basic_block *bbs = LOOP_VINFO_BBS (loop_vinfo);
  unsigned int nbbs = loop->num_nodes;
  block_stmt_iterator si;
  tree stmt, use;
  stmt_ann_t ann;
  ssa_op_iter iter;
  unsigned int i;
  stmt_vec_info stmt_vinfo;
  basic_block bb;
  tree phi;
  bool relevant_p, live_p;
  tree def, def_stmt;
  enum vect_def_type dt;

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "=== vect_mark_stmts_to_be_vectorized ===");

  worklist = VEC_alloc (tree, heap, 64);

  /* 1. Init worklist.  */

  bb = loop->header;
  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      if (vect_print_dump_info (REPORT_DETAILS))
        {
          fprintf (vect_dump, "init: phi relevant? ");
          print_generic_expr (vect_dump, phi, TDF_SLIM);
        }

      if (vect_stmt_relevant_p (phi, loop_vinfo, &relevant_p, &live_p))
	vect_mark_relevant (&worklist, phi, relevant_p, live_p);
    }

  for (i = 0; i < nbbs; i++)
    {
      bb = bbs[i];
      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
	{
	  stmt = bsi_stmt (si);

	  if (vect_print_dump_info (REPORT_DETAILS))
	    {
	      fprintf (vect_dump, "init: stmt relevant? ");
	      print_generic_expr (vect_dump, stmt, TDF_SLIM);
	    } 

	  if (vect_stmt_relevant_p (stmt, loop_vinfo, &relevant_p, &live_p))
            vect_mark_relevant (&worklist, stmt, relevant_p, live_p);
	}
    }


  /* 2. Process_worklist */

  while (VEC_length (tree, worklist) > 0)
    {
      stmt = VEC_pop (tree, worklist);

      if (vect_print_dump_info (REPORT_DETAILS))
	{
          fprintf (vect_dump, "worklist: examine stmt: ");
          print_generic_expr (vect_dump, stmt, TDF_SLIM);
	}

      /* Examine the USEs of STMT. For each ssa-name USE that is defined
         in the loop, mark the stmt that defines it (DEF_STMT) as
         relevant/irrelevant and live/dead according to the liveness and
         relevance properties of STMT.
       */

      gcc_assert (TREE_CODE (stmt) != PHI_NODE);

      ann = stmt_ann (stmt);
      stmt_vinfo = vinfo_for_stmt (stmt);
      
      relevant_p = STMT_VINFO_RELEVANT_P (stmt_vinfo);
      live_p = STMT_VINFO_LIVE_P (stmt_vinfo);

      /* Generally, the liveness and relevance properties of STMT are
         propagated to the DEF_STMTs of its USEs:
             STMT_VINFO_LIVE_P (DEF_STMT_info) <-- live_p
             STMT_VINFO_RELEVANT_P (DEF_STMT_info) <-- relevant_p

         Exceptions:

	 (case 1)
           If USE is used only for address computations (e.g. array indexing),
           which does not need to be directly vectorized, then the
           liveness/relevance of the respective DEF_STMT is left unchanged.

	 (case 2)
           If STMT has been identified as defining a reduction variable, then
	   we have two cases:
	   (case 2.1)
	     The last use of STMT is the reduction-variable, which is defined
	     by a loop-header-phi. We don't want to mark the phi as live or
	     relevant (because it does not need to be vectorized, it is handled
             as part of the vectorization of the reduction), so in this case we
	     skip the call to vect_mark_relevant.
	   (case 2.2)
	     The rest of the uses of STMT are defined in the loop body. For
             the def_stmt of these uses we want to set liveness/relevance
             as follows:
               STMT_VINFO_LIVE_P (DEF_STMT_info) <-- false
               STMT_VINFO_RELEVANT_P (DEF_STMT_info) <-- true
             because even though STMT is classified as live (since it defines a
             value that is used across loop iterations) and irrelevant (since it
             is not used inside the loop), it will be vectorized, and therefore
             the corresponding DEF_STMTs need to marked as relevant.
       */

      /* case 2.2:  */
      if (STMT_VINFO_DEF_TYPE (stmt_vinfo) == vect_reduction_def)
        {
          gcc_assert (!relevant_p && live_p);
          relevant_p = true;
          live_p = false;
        }

      FOR_EACH_SSA_TREE_OPERAND (use, stmt, iter, SSA_OP_USE)
	{
	  /* case 1: we are only interested in uses that need to be vectorized. 
	     Uses that are used for address computation are not considered 
	     relevant.
	   */
	  if (!exist_non_indexing_operands_for_use_p (use, stmt))
	    continue;

	  if (!vect_is_simple_use (use, loop_vinfo, &def_stmt, &def, &dt))
            {
              if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
                fprintf (vect_dump, "not vectorized: unsupported use in stmt.");
	      VEC_free (tree, heap, worklist);
              return false;
            }

	  if (!def_stmt || IS_EMPTY_STMT (def_stmt))
	    continue;

          if (vect_print_dump_info (REPORT_DETAILS))
            {
              fprintf (vect_dump, "worklist: examine use %d: ", i);
              print_generic_expr (vect_dump, use, TDF_SLIM);
            }

	  bb = bb_for_stmt (def_stmt);
          if (!flow_bb_inside_loop_p (loop, bb))
            continue;
	  
	  /* case 2.1: the reduction-use does not mark the defining-phi
	     as relevant.  */
	  if (STMT_VINFO_DEF_TYPE (stmt_vinfo) == vect_reduction_def
	      && TREE_CODE (def_stmt) == PHI_NODE)
	    continue;
	  
	  vect_mark_relevant (&worklist, def_stmt, relevant_p, live_p);
	}
    }				/* while worklist */

  VEC_free (tree, heap, worklist);
  return true;
}


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

  if (dt == vect_loop_def)
    {
      if (STMT_VINFO_IN_PATTERN_P (vinfo_for_stmt (*def_stmt)))
        return false;
    }
  else if (dt != vect_invariant_def && dt != vect_constant_def)
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


/* Function vect_recog_mult_hi_pattern  */

tree
vect_recog_mult_hi_pattern (tree last_stmt ATTRIBUTE_UNUSED,
                            tree *pattern_vectype ATTRIBUTE_UNUSED,
                            varray_type *stmt_list ATTRIBUTE_UNUSED)
{
  /* TODO */
  return NULL;
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
vect_recog_dot_prod_pattern (tree last_stmt,
                             tree *pattern_vectype,
                             varray_type *stmt_list)
{
  tree stmt, expr;
  tree oprnd0, oprnd1;
  tree oprnd00, oprnd01;
  stmt_vec_info stmt_vinfo = vinfo_for_stmt (last_stmt);
  tree type, half_type;
  tree def_stmt;
  tree pattern_expr;
  enum machine_mode vec_mode;
  enum insn_code icode;
  optab optab;
  tree vectype;
  tree prod_type;

  if (STMT_VINFO_IN_PATTERN_P (vinfo_for_stmt (last_stmt)))
    return NULL;

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
     Left to check that oprnd0 is defined by a (widen_)mult_expr, possibly
     followed by a cast  */

  if (widened_name_p (oprnd0, last_stmt, &prod_type, &def_stmt))
    {
      VARRAY_PUSH_TREE (*stmt_list, def_stmt);
      expr = TREE_OPERAND (def_stmt, 1);
      oprnd0 = TREE_OPERAND (expr, 0);
    }
  else
    prod_type = type;

  stmt = SSA_NAME_DEF_STMT (oprnd0);
  gcc_assert (stmt);
  stmt_vinfo = vinfo_for_stmt (stmt);
  gcc_assert (stmt_vinfo);
  if (STMT_VINFO_IN_PATTERN_P (stmt_vinfo))
    return NULL;
  gcc_assert (STMT_VINFO_DEF_TYPE (stmt_vinfo) == vect_loop_def);
  gcc_assert (TREE_CODE (stmt) == MODIFY_EXPR);

  expr = TREE_OPERAND (stmt, 1);
  if (TREE_CODE (expr) != WIDEN_MULT_EXPR
      && TREE_CODE (expr) != MULT_EXPR)
   return NULL;

  VARRAY_PUSH_TREE (*stmt_list, stmt);

  if (TREE_CODE (expr) == WIDEN_MULT_EXPR)
    {
      oprnd00 = TREE_OPERAND (expr, 0);
      oprnd01 = TREE_OPERAND (expr, 1);
    }
  else /* MULT_EXPR */
    {
      tree oprnd0 = TREE_OPERAND (expr, 0);
      tree oprnd1 = TREE_OPERAND (expr, 1);
      tree half_type0, half_type1;
      tree def_stmt0, def_stmt1;

      if (TYPE_MAIN_VARIANT (TREE_TYPE (oprnd0)) 
	  != TYPE_MAIN_VARIANT (prod_type)
	  || TYPE_MAIN_VARIANT (TREE_TYPE (oprnd1)) 
             != TYPE_MAIN_VARIANT (prod_type))
	return NULL;

      /* Check argument 0 */
      if (!widened_name_p (oprnd0, stmt, &half_type0, &def_stmt0))
	return NULL;

      /* Check argument 1 */
      if (!widened_name_p (oprnd1, stmt, &half_type1, &def_stmt1))
	return NULL;

      if (TYPE_MAIN_VARIANT (half_type0) != TYPE_MAIN_VARIANT (half_type1))
        return NULL;

      if (TYPE_PRECISION (prod_type) != (TYPE_PRECISION (half_type0) * 2))
	return NULL;

      VARRAY_PUSH_TREE (*stmt_list, def_stmt0);
      VARRAY_PUSH_TREE (*stmt_list, def_stmt1);

      oprnd00 = TREE_OPERAND (TREE_OPERAND (def_stmt0, 1), 0);
      oprnd01 = TREE_OPERAND (TREE_OPERAND (def_stmt1, 1), 0);
    }

  /* Check target support  */
  half_type = TREE_TYPE (oprnd00);
  vectype = get_vectype_for_scalar_type (half_type);
  optab = optab_for_tree_code (DOT_PROD_EXPR, vectype);
  vec_mode = TYPE_MODE (vectype);
  if (!optab)
    {
      if (vect_print_dump_info (REPORT_DETAILS))
        fprintf (vect_dump, "vect_recog_dot_prod_pattern: no optab");
      return NULL;
    }
  if ((icode = optab->handlers[(int) vec_mode].insn_code) == CODE_FOR_nothing
      || insn_data[icode].operand[0].mode !=
         TYPE_MODE (get_vectype_for_scalar_type (type)))
    {
      if (vect_print_dump_info (REPORT_DETAILS))
        fprintf (vect_dump,
                 "vect_recog_dot_prod_pattern: mode not supported.");
      return NULL;
    }

  /* Pattern detected. Create a stmt to be used to replace the pattern: */
  pattern_expr = build (DOT_PROD_EXPR, type, oprnd00, oprnd01, oprnd1);
  if (vect_print_dump_info (REPORT_DETAILS))
    {
      fprintf (vect_dump, "vect_recog_dot_prod_pattern: detected: ");
      print_generic_expr (vect_dump, pattern_expr, TDF_SLIM);
    }
  *pattern_vectype = vectype;
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
vect_recog_widen_mult_pattern (tree last_stmt,
                               tree *pattern_vectype,
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
  if (TREE_TYPE (oprnd0) != type || TREE_TYPE (oprnd1) != type)
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

  /* Check target support  */
  vectype = get_vectype_for_scalar_type (half_type0);
  if (!supportable_widening_operation (WIDEN_MULT_EXPR, last_stmt, vectype,
                                       &dummy, &dummy, &dummy_code, &dummy_code))
    return NULL;

  VARRAY_PUSH_TREE (*stmt_list, def_stmt0);
  VARRAY_PUSH_TREE (*stmt_list, def_stmt1);
  
  /* Pattern detected. Create a stmt to be used to replace the pattern: */
  pattern_expr = build (WIDEN_MULT_EXPR, type, oprnd0, oprnd1);

  if (vect_print_dump_info (REPORT_DETAILS))
    {
      fprintf (vect_dump, "vect_recog_widen_mult_pattern: detected: ");
      print_generic_expr (vect_dump, pattern_expr, TDF_SLIM);
    }
  *pattern_vectype = vectype;
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
vect_recog_widen_sum_pattern (tree last_stmt,
			       tree *pattern_vectype,
			       varray_type *stmt_list)
{
  tree stmt, expr;
  tree oprnd0, oprnd1;
  stmt_vec_info stmt_vinfo = vinfo_for_stmt (last_stmt);
  tree type, half_type;
  tree pattern_expr;
  enum machine_mode vec_mode;
  enum insn_code icode;
  optab optab;
  tree vectype;

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

  /* Check target support  */
  vectype = get_vectype_for_scalar_type (half_type);
  optab = optab_for_tree_code (WIDEN_SUM_EXPR, vectype);
  vec_mode = TYPE_MODE (vectype);
  if (!optab)
    {
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, "vect_recog_widen_sum_pattern: no optab.");
      return NULL;
    }
  if ((icode = optab->handlers[(int) vec_mode].insn_code) == CODE_FOR_nothing
      || insn_data[icode].operand[0].mode != 
		TYPE_MODE (get_vectype_for_scalar_type (type)))
    {
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, 
		 "vect_recog_widen_sum_pattern: mode not supported.");
      return NULL;
    }

  VARRAY_PUSH_TREE (*stmt_list, stmt);
  
  /* Pattern detected. Create a stmt to be used to replace the pattern: */
  pattern_expr = build (WIDEN_SUM_EXPR, type, oprnd0, oprnd1);
  if (vect_print_dump_info (REPORT_DETAILS))
    {
      fprintf (vect_dump, "vect_recog_widen_sum_pattern: detected: ");
      print_generic_expr (vect_dump, pattern_expr, TDF_SLIM);
    }
  *pattern_vectype = vectype;
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
vect_recog_unsigned_subsat_pattern (tree last_stmt, tree *pattern_vectype, 
				    varray_type *stmt_list)
{
  tree stmt, expr;
  tree type, vectype;
  tree cond_expr;
  tree then_clause = NULL_TREE;
  tree else_clause = NULL_TREE;
  enum tree_code code;
  tree zero;
  tree a, b, a_minus_b, b_minus_1;
  tree pattern_expr;
  tree new;
  optab optab;
  enum machine_mode vec_mode;

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
	
  if (TREE_TYPE (a) != type)
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

  /* Check that the pattern is supported in vector form  */
  vectype = get_vectype_for_scalar_type (type);
  optab = optab_for_tree_code (code, vectype);
  vec_mode = TYPE_MODE (vectype);

  if (!optab
      || optab->handlers[(int) vec_mode].insn_code == CODE_FOR_nothing)
    return NULL;

  /* Pattern detected. Create a stmt to be used to replace the pattern: */
  pattern_expr = build (SAT_MINUS_EXPR, type, a, b);
  if (vect_print_dump_info (REPORT_DETAILS))
    {
      fprintf (vect_dump, "vect_recog_unsigned_subsat_pattern: detetced: ");
      print_generic_expr (vect_dump, pattern_expr, TDF_SLIM);
    }
  *pattern_vectype = vectype;
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

void
vect_pattern_recog_1 (tree (* pattern_recog_func) (tree, tree *, varray_type *),
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
  tree pattern_type;
  enum tree_code code;
  tree var, var_name;
  stmt_ann_t ann;

  VARRAY_TREE_INIT (stmt_list, 10, "stmt list");
  pattern_expr = (* pattern_recog_func) (stmt, &pattern_vectype, &stmt_list);

  if (!pattern_expr)
    {
      varray_clear (stmt_list);
      return;
    }

  /* Found a vectorizable pattern! */
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
  pattern_expr = build (MODIFY_EXPR, void_type_node, var_name, pattern_expr);
  SSA_NAME_DEF_STMT (var_name) = pattern_expr;
  bsi_insert_before (&si, pattern_expr, BSI_SAME_STMT);
  ann = stmt_ann (pattern_expr);
  set_stmt_info ((tree_ann_t)ann, new_stmt_vec_info (pattern_expr, loop_vinfo));
  pattern_stmt_info = vinfo_for_stmt (pattern_expr);

  STMT_VINFO_RELATED_STMT (pattern_stmt_info) = stmt;
  STMT_VINFO_DEF_TYPE (pattern_stmt_info) = STMT_VINFO_DEF_TYPE (stmt_info);

  /* We've just created the following new stmt:
     	X = pattern_expr (args)

     The vectype of this new stmt (i.e., the vector type that is used when 
     vectorizing the stmt), is obtained by:
        get_vectype_for_scalar_type (TREE_TYPE (X)).
     Usually we record this vectype in STMT_VINFO_VECTYPE. This field 
     is used (1) when creating the vector version of the stmt, and (2) when 
     determining the vectorization factor.

     However, there are some computation-idioms for which the vectype that is 
     used for vectorization cannot be used to determine the vectorization 
     factor, because it consists of a different number of elements than the 
     actual number of elements that are being operated upon in parallel.  
     So far we have this situation only in cases of reduction patterns: 
     	X = pattern_expr (arg0, arg1, ..., X)
     in which the type of the reduction variable X is 
     different than the type of the other arguments to the stmt.

     For example, consider an accumulation of shorts into an int accumulator. 
     On some targets it's possible to vectorize this pattern operating on 8
     short's at a time (hence, the vectype for purposes of determining the
     vectorization factor should be V8HI); on the other hand, the vectype that
     is used to create the vector form is actually V4SI (the type of the 
     result). 

     In STMT_VINFO_VECTYPE we record the vectype that indicates what is the
     actual level of parallelism (V8HI in the example), so that the subsequent 
     analyses passes will be able to derive the right vectorization factor.
     This means that the following:
      STMT_VINFO_VECTYPE == get_vectype_for_scalar_type (TREE_TYPE (X))
     doesn't hold for reduction patterns.
     Therefore in the transformation routines for reduction patterns,
     we don't use the vectype recorded in STMT_VINFO_VECTYPE, but get the
     vectype from the type of the result (X).
  */
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
	stmts, as explained below through an example:

   At the entry to this function we have the following stmts, with the
   following initial value in the STMT_VINFO fields:

         stmt                     in_pattern_p	related_stmt	vec_stmt
         S1: a_i = ....			false
         S2: a_2 = ..use(a_i)..		false
         S3: a_1 = ..use(a_2)..		false
         S4: a_0 = ..use(a_1)..		false
         S5: ... = ..use(a_0)..		false


   Say the sequence {S1,S2,S3,S4} was detected as a pattern that can be
   represented by a single stmt. We then:
   - create a new stmt S6 that will replace the pattern.
   - insert the new stmt S6 before the last stmt in the pattern
   - fill in the STMT_VINFO fields as follows:

                                  in_pattern_p	related_stmt	vec_stmt
         S1: a_i = ....			true
         S2: a_2 = ..use(a_i)..		true
         S3: a_1 = ..use(a_2)..		true
       > S6: a_new = ....		false	S4
         S4: a_0 = ..use(a_1)..		true	S6
         S5: ... = ..use(a_0)..		false

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

                                  in_pattern_p	related_stmt	vec_stmt
         S1: a_i = ....         	true            
         S2: a_2 = ..use(a_i).. 	true    
         S3: a_1 = ..use(a_2).. 	true    
       > VS6: va_new = ....	
         S6: a_new = ....       	false	S4		VS6
         S4: a_0 = ..use(a_1).. 	true	S6		VS6
       > VS5: ... = ..vuse(va_new)..
         S5: ... = ..use(a_0).. false   

   DCE could then get rid of {S1,S2,S3,S4,S5,S6} (if their defs are not used
   elsewhere), and we'll end up with:

	VS6: va_new = .... 
	VS5: ... = ..vuse(va_new)..

   If vectorization does not succeed, DCE will clean S6 away (its def is
   not used), and we'll end up with the original sequence.
*/

static void
vect_pattern_recog (loop_vec_info loop_vinfo)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  basic_block *bbs = LOOP_VINFO_BBS (loop_vinfo);
  unsigned int nbbs = loop->num_nodes;
  block_stmt_iterator si;
  tree stmt;
  unsigned int i, j;
  tree (* pattern_recog_func) (tree, tree *, varray_type *);

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "=== vect_pattern_recog ===");

  /* Scan through the loop stmts, trying to apply the pattern recognition
     utility starting at each stmt visited:  */
  for (i = 0; i < nbbs; i++)
    {
      basic_block bb = bbs[i];
      for (si = bsi_start (bb); !bsi_end_p (si); bsi_next (&si))
        {
          stmt = bsi_stmt (si);

	  /* Scan over all target specific vect_recog_xxx_pattern functions
	     if available.  */
	  if (targetm.vectorize.builtin_vect_pattern_recog)
	    targetm.vectorize.builtin_vect_pattern_recog (stmt);

	  /* Scan over all generic vect_recog_xxx_pattern functions.  */
	  for (j = 0; j < NUM_PATTERNS; j++)
	    {
	      pattern_recog_func = vect_pattern_recog_funcs[j];
	      vect_pattern_recog_1 (pattern_recog_func, si);
	    }
	}
    }
}


/* Function vect_can_advance_ivs_p

   In case the number of iterations that LOOP iterates is unknown at compile 
   time, an epilog loop will be generated, and the loop induction variables 
   (IVs) will be "advanced" to the value they are supposed to take just before 
   the epilog loop.  Here we check that the access function of the loop IVs
   and the expression that represents the loop bound are simple enough.
   These restrictions will be relaxed in the future.  */

static bool 
vect_can_advance_ivs_p (loop_vec_info loop_vinfo)
{
  struct loop *loop = LOOP_VINFO_LOOP (loop_vinfo);
  basic_block bb = loop->header;
  tree phi;

  /* Analyze phi functions of the loop header.  */

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "vect_can_advance_ivs_p:");

  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
    {
      tree access_fn = NULL;
      tree evolution_part;

      if (vect_print_dump_info (REPORT_DETAILS))
	{
          fprintf (vect_dump, "Analyze phi: ");
          print_generic_expr (vect_dump, phi, TDF_SLIM);
	}

      /* Skip virtual phis. The data dependences that are associated with
         virtual defs/uses (i.e., memory accesses) are analyzed elsewhere.  */

      if (!is_gimple_reg (SSA_NAME_VAR (PHI_RESULT (phi))))
	{
	  if (vect_print_dump_info (REPORT_DETAILS))
	    fprintf (vect_dump, "virtual phi. skip.");
	  continue;
	}

      /* Skip reduction phis.  */

      if (STMT_VINFO_DEF_TYPE (vinfo_for_stmt (phi)) == vect_reduction_def)
        {
          if (vect_print_dump_info (REPORT_DETAILS))
            fprintf (vect_dump, "reduc phi. skip.");
          continue;
        }

      /* Analyze the evolution function.  */

      access_fn = instantiate_parameters
	(loop, analyze_scalar_evolution (loop, PHI_RESULT (phi)));

      if (!access_fn)
	{
	  if (vect_print_dump_info (REPORT_DETAILS))
	    fprintf (vect_dump, "No Access function.");
	  return false;
	}

      if (vect_print_dump_info (REPORT_DETAILS))
        {
	  fprintf (vect_dump, "Access function of PHI: ");
	  print_generic_expr (vect_dump, access_fn, TDF_SLIM);
        }

      evolution_part = evolution_part_in_loop_num (access_fn, loop->num);
      
      if (evolution_part == NULL_TREE)
        {
	  if (vect_print_dump_info (REPORT_DETAILS))
	    fprintf (vect_dump, "No evolution.");
	  return false;
        }
  
      /* FORNOW: We do not transform initial conditions of IVs 
	 which evolution functions are a polynomial of degree >= 2.  */

      if (tree_is_chrec (evolution_part))
	return false;  
    }

  return true;
}


/* Function vect_get_loop_niters.

   Determine how many iterations the loop is executed.
   If an expression that represents the number of iterations
   can be constructed, place it in NUMBER_OF_ITERATIONS.
   Return the loop exit condition.  */

static tree
vect_get_loop_niters (struct loop *loop, tree *number_of_iterations)
{
  tree niters;

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "=== get_loop_niters ===");

  niters = number_of_iterations_in_loop (loop);

  if (niters != NULL_TREE
      && niters != chrec_dont_know)
    {
      *number_of_iterations = niters;

      if (vect_print_dump_info (REPORT_DETAILS))
	{
	  fprintf (vect_dump, "==> get_loop_niters:" );
	  print_generic_expr (vect_dump, *number_of_iterations, TDF_SLIM);
	}
    }

  return get_loop_exit_condition (loop);
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
  tree number_of_iterations = NULL;

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "=== vect_analyze_loop_form ===");

  if (loop->inner)
    {
      if (vect_print_dump_info (REPORT_OUTER_LOOPS))
        fprintf (vect_dump, "not vectorized: nested loop.");
      return NULL;
    }
  
  if (!loop->single_exit 
      || loop->num_nodes != 2
      || EDGE_COUNT (loop->header->preds) != 2)
    {
      if (vect_print_dump_info (REPORT_BAD_FORM_LOOPS))
        {
          if (!loop->single_exit)
            fprintf (vect_dump, "not vectorized: multiple exits.");
          else if (loop->num_nodes != 2)
            fprintf (vect_dump, "not vectorized: too many BBs in loop.");
          else if (EDGE_COUNT (loop->header->preds) != 2)
            fprintf (vect_dump, "not vectorized: too many incoming edges.");
        }

      return NULL;
    }

  /* We assume that the loop exit condition is at the end of the loop. i.e,
     that the loop is represented as a do-while (with a proper if-guard
     before the loop if needed), where the loop header contains all the
     executable statements, and the latch is empty.  */
  if (!empty_block_p (loop->latch))
    {
      if (vect_print_dump_info (REPORT_BAD_FORM_LOOPS))
        fprintf (vect_dump, "not vectorized: unexpected loop form.");
      return NULL;
    }

  /* Make sure there exists a single-predecessor exit bb:  */
  if (!single_pred_p (loop->single_exit->dest))
    {
      edge e = loop->single_exit;
      if (!(e->flags & EDGE_ABNORMAL))
	{
	  split_loop_exit_edge (e);
	  if (vect_print_dump_info (REPORT_DETAILS))
	    fprintf (vect_dump, "split exit edge.");
	}
      else
	{
	  if (vect_print_dump_info (REPORT_BAD_FORM_LOOPS))
	    fprintf (vect_dump, "not vectorized: abnormal loop exit edge.");
	  return NULL;
	}
    }
    
  if (empty_block_p (loop->header))
    {
      if (vect_print_dump_info (REPORT_BAD_FORM_LOOPS))
        fprintf (vect_dump, "not vectorized: empty loop.");
      return NULL;
    }

  loop_cond = vect_get_loop_niters (loop, &number_of_iterations);
  if (!loop_cond)
    {
      if (vect_print_dump_info (REPORT_BAD_FORM_LOOPS))
	fprintf (vect_dump, "not vectorized: complicated exit condition.");
      return NULL;
    }
  
  if (!number_of_iterations) 
    {
      if (vect_print_dump_info (REPORT_BAD_FORM_LOOPS))
	fprintf (vect_dump, 
		 "not vectorized: number of iterations cannot be computed.");
      return NULL;
    }

  if (chrec_contains_undetermined (number_of_iterations))
    {
      if (vect_print_dump_info (REPORT_BAD_FORM_LOOPS))
        fprintf (vect_dump, "Infinite number of iterations.");
      return false;
    }

  loop_vinfo = new_loop_vec_info (loop);
  LOOP_VINFO_NITERS (loop_vinfo) = number_of_iterations;

  if (!LOOP_VINFO_NITERS_KNOWN_P (loop_vinfo))
    {
      if (vect_print_dump_info (REPORT_DETAILS))
        {
          fprintf (vect_dump, "Symbolic number of iterations is ");
          print_generic_expr (vect_dump, number_of_iterations, TDF_DETAILS);
        }
    }
  else
  if (LOOP_VINFO_INT_NITERS (loop_vinfo) == 0)
    {
      if (vect_print_dump_info (REPORT_UNVECTORIZED_LOOPS))
        fprintf (vect_dump, "not vectorized: number of iterations = 0.");
      return NULL;
    }

  LOOP_VINFO_EXIT_COND (loop_vinfo) = loop_cond;

  return loop_vinfo;
}


/* Function vect_analyze_loop.

   Apply a set of analyses on LOOP, and create a loop_vec_info struct
   for it. The different analyses will record information in the
   loop_vec_info struct.  */

loop_vec_info
vect_analyze_loop (struct loop *loop)
{
  bool ok;
  loop_vec_info loop_vinfo;

  if (vect_print_dump_info (REPORT_DETAILS))
    fprintf (vect_dump, "===== analyze_loop_nest =====");

  /* Check the CFG characteristics of the loop (nesting, entry/exit, etc.  */

  loop_vinfo = vect_analyze_loop_form (loop);
  if (!loop_vinfo)
    {
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, "bad loop form.");
      return NULL;
    }

  /* Find all data references in the loop (which correspond to vdefs/vuses)
     and analyze their evolution in the loop.

     FORNOW: Handle only simple, array references, which
     alignment can be forced, and aligned pointer-references.  */

  ok = vect_analyze_data_refs (loop_vinfo);
  if (!ok)
    {
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, "bad data references.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }

  /* Classify all cross-iteration scalar data-flow cycles.
     Cross-iteration cycles caused by virtual phis are analyzed separately.  */

  vect_analyze_scalar_cycles (loop_vinfo);

  vect_pattern_recog (loop_vinfo);

  /* Data-flow analysis to detect stmts that do not need to be vectorized.  */

  ok = vect_mark_stmts_to_be_vectorized (loop_vinfo);
  if (!ok)
    {
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, "unexpected pattern.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }

  /* Analyze the alignment of the data-refs in the loop.
     Fail if a data reference is found that cannot be vectorized.  */

  ok = vect_analyze_data_refs_alignment (loop_vinfo);
  if (!ok)
    {
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, "bad data alignment.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }
  ok = vect_determine_vectorization_factor (loop_vinfo);
  if (!ok)
    {      
      if (vect_print_dump_info (REPORT_DETAILS))
        fprintf (vect_dump, "can't determine vectorization factor.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }

  /* Scan all the operations in the loop and make sure they are
     vectorizable.  */

  ok = vect_determine_vectorization_factor (loop_vinfo);
  if (!ok)
    {
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, "can't determine vectorization factor.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }

  ok = vect_analyze_data_ref_dependences (loop_vinfo);
  if (!ok)
    {
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, "bad data dependence.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }

  /* Analyze the access patterns of the data-refs in the loop (consecutive,
     complex, etc.). FORNOW: Only handle consecutive access pattern.  */

  ok = vect_analyze_data_ref_accesses (loop_vinfo);
  if (!ok)
    {
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, "bad data access.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }

  /* This pass will decide on using loop versioning and/or loop peeling in
     order to enhance the alignment of data references in the loop.  */

  ok = vect_enhance_data_refs_alignment (loop_vinfo);
  if (!ok)
    {
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, "bad data alignment.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    } 

  /* Scan all the operations in the loop and make sure they are
     vectorizable.  */

  ok = vect_analyze_operations (loop_vinfo);
  if (!ok)
    {
      if (vect_print_dump_info (REPORT_DETAILS))
	fprintf (vect_dump, "bad operation or unsupported loop bound.");
      destroy_loop_vec_info (loop_vinfo);
      return NULL;
    }

  LOOP_VINFO_VECTORIZABLE_P (loop_vinfo) = 1;

  return loop_vinfo;
}
