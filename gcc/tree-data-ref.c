/* Data references and dependences detectors.
   Copyright (C) 2003 Free Software Foundation, Inc.
   Contributed by Sebastian Pop <s.pop@laposte.net>

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

/* This pass walks a loop searching for array references.  The array
   accesses are recorded in DATA_REFERENCE nodes.  Since the
   information in the DATA_REFERENCE nodes is too precise, the
   dependence testers abstract this information into classic
   representations: distance vectors, direction vectors, affine
   dependence functions, ...  Both the precise and more abstract
   informations are then exposed to the other passes.
   
   The basic test for determining the dependences is: 
   given two access functions chrec1 and chrec2 to a same array, and 
   x and y two vectors from the iteration domain, the same element of 
   the array is accessed twice at iterations x and y if and only if:
   |             chrec1 (x) == chrec2 (y).
   
   The goals of this analysis are:
   - to determine the independence: the relation between two independent 
     accesses is qualified with the chrec_bot (this information allows a 
     loop parallelization),
   - when two data references access the same data, to qualify the 
     dependence relation with classic dependence representations: 
       - distance vectors
       - direction vectors
       - loop carried level dependence
       - polyhedron dependence
     or with the chains of recurrences based representation,
   - to define a knowledge base for the data dependeces information,
   - to define an interface to access this data.
   
   
   Definitions:
   
   - What is a subscript?  Given two array accesses a subscript is the
   tuple composed of the access functions for a given dimension.
   Example: Given A[f1][f2][f3] and B[g1][g2][g3], there are three
   subscripts: (f1, g1), (f2, g2), (f3, g3).
   
   - Vertical and horizontal couplings.  In some of the comments of
   this analysis, I refer to the overlapping elements of a subscript
   as the vertical coupling, in opposition to the horizontal coupling
   that refers to the coupling between subscripts.
   
   
   References:
   
   - "Advanced Compilation for High Performance Computing" by Randy Allen 
   and Ken Kennedy.
   
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "errors.h"
#include "ggc.h"
#include "tree.h"

/* These RTL headers are needed for basic-block.h.  */
#include "rtl.h"
#include "basic-block.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "timevar.h"
#include "cfgloop.h"
#include "tree-fold-const.h"
#include "tree-chrec.h"
#include "tree-data-ref.h"
#include "tree-scalar-evolution.h"

static void analyze_array (struct loop *, varray_type, tree, tree);
static tree analyze_array_indexes (struct loop *, varray_type, tree);
static bool access_functions_are_affine_or_constant_p (struct data_reference *);

static struct data_dependence_relation *
initialize_data_dependence_relation (struct data_reference *, 
				     struct data_reference *);
static void compute_all_dependences (varray_type, varray_type);

static inline bool array_base_name_differ_p (struct data_reference *, struct data_reference *);

static void subscript_dependence_tester (struct data_dependence_relation *);
static void subscript_coupling_tester (struct data_dependence_relation *);

static unsigned int data_ref_id = 0;

/* Debugging dumps.  */
static FILE *dump_file;
static int dump_flags;



/* Debugging functions.  */

/* Debugging function for a DATA_REFERENCE structure.  */

void 
debug_data_reference (struct data_reference *dr)
{
  dump_data_reference (stderr, dr);
}

void 
debug_data_references (varray_type dr)
{
  dump_data_references (stderr, dr);
}

void 
debug_data_dependence_relation (struct data_dependence_relation *ddr)
{
  dump_data_dependence_relation (stderr, ddr);
}

void 
debug_data_dependence_relations (varray_type ddr)
{
  dump_data_dependence_relations (stderr, ddr);
}

/* Dump into FILE all the data references from DATAREFS.  */ 

void 
dump_data_references (FILE *file, 
		      varray_type datarefs)
{
  unsigned int i;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (datarefs); i++)
    dump_data_reference (file, VARRAY_GENERIC_PTR (datarefs, i));
}

/* Dump into FILE all the dependence relations from DDR.  */ 

void 
dump_data_dependence_relations (FILE *file, 
				varray_type ddr)
{
  unsigned int i;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (ddr); i++)
    dump_data_dependence_relation (file, VARRAY_GENERIC_PTR (ddr, i));
}


/* Dump function for a DATA_REFERENCE structure.  */

void 
dump_data_reference (FILE *outf, 
		     struct data_reference *dr)
{
  unsigned int i;
  
  fprintf (outf, "\n(Data Ref %d: \n  expr: ", dr->id);
  print_generic_stmt (outf, dr->expr, 0);
  fprintf (outf, "  ref: ");
  print_generic_stmt (outf, dr->ref, 0);
  fprintf (outf, "  base_name: ");
  print_generic_stmt (outf, dr->base_name, 0);
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (dr->access_fns); i++)
    {
      tree chrec = VARRAY_TREE (dr->access_fns, i);
      
      fprintf (outf, "  Access function %d: ", i);
      print_generic_stmt (outf, chrec, 0);
    }
  fprintf (outf, ")");
}

/* Dump function for a DATA_DEPENDENCE_RELATION structure.  */

void 
dump_data_dependence_relation (FILE *outf, 
			       struct data_dependence_relation *ddr)
{
  unsigned int i;
  struct data_reference *dra, *drb;
  
  dra = DDR_A (ddr);
  drb = DDR_B (ddr);
  
  fprintf (outf, "\n(Data Dep (A = %d, B = %d):", DR_ID (dra), DR_ID (drb));
  
  if (DDR_ARE_DEPENDENT (ddr) == chrec_top)
    {
      fprintf (outf, "    (don't know)\n");
    }
  
  else if (DDR_ARE_DEPENDENT (ddr) == chrec_bot)
    {
      fprintf (outf, "    (no dependence)\n");
    }
  
  else
    {
      for (i = 0; i < DDR_NUM_SUBSCRIPTS (ddr); i++)
	{
	  tree chrec;
	  struct subscript *subscript = DDR_SUBSCRIPT (ddr, i);
	  
	  fprintf (outf, "\n (subscript %d:\n", i);
	  fprintf (outf, "  base_name_A: ");
	  print_generic_stmt (outf, DR_BASE_NAME (dra), 0);
	  fprintf (outf, "  base_name_B: ");
	  print_generic_stmt (outf, DR_BASE_NAME (drb), 0);
	  fprintf (outf, "  access_fn_A: ");
	  print_generic_stmt (outf, DR_ACCESS_FN (dra, i), 0);
	  fprintf (outf, "  access_fn_B: ");
	  print_generic_stmt (outf, DR_ACCESS_FN (drb, i), 0);
	  
	  chrec = SUB_CONFLICTS_IN_A (subscript);
	  fprintf (outf, "  iterations_that_access_an_element_twice_in_A: ");
	  print_generic_stmt (outf, chrec, 0);
	  if (chrec == chrec_bot)
	    fprintf (outf, "    (no dependence)\n");
	  else if (chrec == chrec_top)
	    fprintf (outf, "    (don't know)\n");
	  else
	    {
	      tree last_iteration = SUB_LAST_CONFLICT_IN_A (subscript);
	      fprintf (outf, "  last_iteration_that_access_an_element_twice_in_A: ");
	      print_generic_stmt (outf, last_iteration, 0);
	    }
	  
	  chrec = SUB_CONFLICTS_IN_B (subscript);
	  fprintf (outf, "  iterations_that_access_an_element_twice_in_B: ");
	  print_generic_stmt (outf, chrec, 0);
	  if (chrec == chrec_bot)
	    fprintf (outf, "    (no dependence)\n");
	  else if (chrec == chrec_top)
	    fprintf (outf, "    (don't know)\n");
	  else
	    {
	      tree last_iteration = SUB_LAST_CONFLICT_IN_B (subscript);
	      fprintf (outf, "  last_iteration_that_access_an_element_twice_in_B: ");
	      print_generic_stmt (outf, last_iteration, 0);
	    }
      
	  fprintf (outf, " )\n");
	}
  
      fprintf (outf, " (Distance Vector: \n");
      for (i = 0; i < DDR_NUM_SUBSCRIPTS (ddr); i++)
	{
	  struct subscript *subscript = DDR_SUBSCRIPT (ddr, i);
      
	  fprintf (outf, "(");
	  print_generic_stmt (outf, SUB_DISTANCE (subscript), 0);
	  fprintf (outf, ")\n");
	}
      fprintf (outf, " )\n");
  
      fprintf (outf, " (Direction Vector: \n");
      for (i = 0; i < DDR_NUM_SUBSCRIPTS (ddr); i++)
	{
	  struct subscript *subscript = DDR_SUBSCRIPT (ddr, i);
	  
	  fprintf (outf, "(");
	  dump_data_dependence_direction (outf, SUB_DIRECTION (subscript));
	  fprintf (outf, ")\n");
	}
      fprintf (outf, " )\n");
    }
  
  fprintf (outf, "\n)");
}



/* Dump function for a DATA_DEPENDENCE_DIRECTION structure.  */

void
dump_data_dependence_direction (FILE *file, 
				enum data_dependence_direction dir)
{
  switch (dir)
    {
    case dir_positive: 
      fprintf (file, "+");
      break;
      
    case dir_negative:
      fprintf (file, "-");
      break;
      
    case dir_equal:
      fprintf (file, "=");
      break;
      
    case dir_positive_or_negative:
      fprintf (file, "+-");
      break;
      
    case dir_positive_or_equal: 
      fprintf (file, "+=");
      break;
      
    case dir_negative_or_equal: 
      fprintf (file, "-=");
      break;
      
    case dir_star: 
      fprintf (file, "*"); 
      break;
      
    default: 
      break;
    }
}



/* Determine whether the access functions are affine functions, in
   which case the dependence tester can be runned.  */

static bool 
access_functions_are_affine_or_constant_p (struct data_reference *a)
{
  unsigned int i;
  varray_type fns = DR_ACCESS_FNS (a);
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (fns); i++)
    if (!evolution_function_is_constant_p (VARRAY_TREE (fns, i))
	&& !evolution_function_is_affine_multivariate_p (VARRAY_TREE (fns, i)))
      return false;
  
  return true;
}

/* Given an ARRAY_REF node REF, records its access functions.
   Example: given A[i][3], record the opnd1 function, ie. the constant
   "3", then recursively call the function on opnd0, ie. the ARRAY_REF
   "A[i]".  The function returns the base name: "A".  */

static tree
analyze_array_indexes (struct loop *loop_nest,
		       varray_type access_fns, 
		       tree ref)
{
  tree opnd0, opnd1;
  tree access_fn, icond;
  
  opnd0 = TREE_OPERAND (ref, 0);
  opnd1 = TREE_OPERAND (ref, 1);
  
  /* Detect the evolution function for this access, and push it in the
     access_fns stack.  */
  access_fn = iccp_determine_evolution_function (loop_nest, opnd1);
  icond = initial_condition (access_fn);
  
  /* FIXME: Maybe this condition could be developed into an ARRAY_REF
     bounds checker.  */
  if (0 
      && !automatically_generated_chrec_p (icond)
      && chrec_is_negative (icond))
    {
      fprintf (stderr, "Warning: I have detected a negative access function:");
      debug_generic_expr (access_fn);
      debug_generic_expr (ref);
    }
  
  VARRAY_PUSH_TREE (access_fns, access_fn);
  
  /* Recursively record other array access functions.  */
  if (TREE_CODE (opnd0) == ARRAY_REF)
    return analyze_array_indexes (loop_nest, access_fns, opnd0);
  else
    return opnd0;
}

/* For a data reference REF contained in the expression EXPR,
   initialize a DATA_REFERENCE structure, and push it in the DATAREFS
   array.  */

static void
analyze_array (struct loop *loop_nest, 
	       varray_type datarefs, 
	       tree expr, 
	       tree ref)
{
  struct data_reference *res;
  res = ggc_alloc (sizeof (struct data_reference));
  
  DR_ID (res) = data_ref_id++;
  DR_EXPR (res) = expr;
  DR_REF (res) = ref;
  VARRAY_TREE_INIT (DR_ACCESS_FNS (res), 5, "access_fns");
  DR_BASE_NAME (res) = analyze_array_indexes (loop_nest, DR_ACCESS_FNS (res), ref);
  
  VARRAY_PUSH_GENERIC_PTR (datarefs, res);
}

/* DN */
struct data_reference *
vec_analyze_array (struct loop *loop_nest,
                   tree expr,
                   tree ref)
{
  struct data_reference *res;
  res = ggc_alloc (sizeof (struct data_reference));

  res->id = data_ref_id++;
  res->expr = expr;
  res->ref = ref;
  VARRAY_TREE_INIT (res->access_fns, 3, "access_fns");
  res->base_name = analyze_array_indexes (loop_nest, res->access_fns, ref);

  return res;
}
/* DN */



/* Initialize a ddr.  */

static struct data_dependence_relation *
initialize_data_dependence_relation (struct data_reference *a, 
				     struct data_reference *b)
{
  struct data_dependence_relation *res;
  
  res = ggc_alloc (sizeof (struct data_dependence_relation));
  DDR_A (res) = a;
  DDR_B (res) = b;
  
  /* When the dimensions of A and B differ, we directly initialize
     the relation to "there is no dependence": chrec_bot.  */
  if (DR_NUM_DIMENSIONS (a) != DR_NUM_DIMENSIONS (b))
    DDR_ARE_DEPENDENT (res) = chrec_bot;
  
  else
    {
      unsigned int i;
      DDR_ARE_DEPENDENT (res) = NULL_TREE;
      DDR_SUBSCRIPTS_VECTOR_INIT (res, DR_NUM_DIMENSIONS (a));
      
      for (i = 0; i < DR_NUM_DIMENSIONS (a); i++)
	{
	  struct subscript *subscript;
	  
	  subscript = ggc_alloc (sizeof (struct subscript));
	  SUB_CONFLICTS_IN_A (subscript) = chrec_top;
	  SUB_CONFLICTS_IN_B (subscript) = chrec_top;
	  SUB_LAST_CONFLICT_IN_A (subscript) = chrec_top;
	  SUB_LAST_CONFLICT_IN_B (subscript) = chrec_top;
	  SUB_DISTANCE (subscript) = chrec_top;
	  SUB_DIRECTION (subscript) = dir_star;
	  VARRAY_PUSH_GENERIC_PTR (DDR_SUBSCRIPTS (res), subscript);
	}
    }
  
  return res;
}

/* When there exists a dependence relation, determine its distance
   vector.  */

void
compute_distance_vector (struct data_dependence_relation *ddr)
{
  if (DDR_ARE_DEPENDENT (ddr) == NULL_TREE)
    {
      unsigned int i;
      
      for (i = 0; i < DDR_NUM_SUBSCRIPTS (ddr); i++)
 	{
 	  tree conflicts_a, conflicts_b, difference;
 	  struct subscript *subscript;
 	  
 	  subscript = DDR_SUBSCRIPT (ddr, i);
 	  conflicts_a = SUB_CONFLICTS_IN_A (subscript);
 	  conflicts_b = SUB_CONFLICTS_IN_B (subscript);
 	  difference = chrec_fold_minus (conflicts_b, conflicts_a);
 	  
 	  if (evolution_function_is_constant_p (difference))
 	    SUB_DISTANCE (subscript) = difference;
 	  
 	  else
 	    SUB_DISTANCE (subscript) = chrec_top;
 	}
    }
}

/* When there exists a dependence relation, determine its direction
   vector.  */

void
compute_direction_vector (struct data_dependence_relation *ddr)
{
  if (DDR_ARE_DEPENDENT (ddr) == NULL_TREE)
    {
      unsigned int i;
      
      for (i = 0; i < DDR_NUM_SUBSCRIPTS (ddr); i++)
 	{
 	  tree difference;
 	  struct subscript *subscript;
 	  
 	  subscript = DDR_SUBSCRIPT (ddr, i);
 	  difference = SUB_DISTANCE (subscript);
 	  
 	  if (integer_zerop (difference))
 	    SUB_DIRECTION (subscript) = dir_equal;
 	  
 	  else if (chrec_is_negative (difference))
 	    SUB_DIRECTION (subscript) = dir_negative;
 	  
 	  else if (chrec_is_positive (difference))
 	    SUB_DIRECTION (subscript) = dir_positive;
 	  
 	  else
 	    SUB_DIRECTION (subscript) = dir_star;
 	}
    }
}

/* This section contains the functions that perform the dependence
   tests.  */

static void 
compute_all_dependences (varray_type datarefs, 
			 varray_type dependence_relations)
{
  unsigned int i, j;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (datarefs); i++)
    for (j = 0; j < VARRAY_ACTIVE_SIZE (datarefs); j++)
      {
	struct data_reference *a, *b;
	struct data_dependence_relation *ddr;

	a = VARRAY_GENERIC_PTR (datarefs, i);
	b = VARRAY_GENERIC_PTR (datarefs, j);
	ddr = initialize_data_dependence_relation (a, b);
	
	VARRAY_PUSH_GENERIC_PTR (dependence_relations, ddr);
	compute_affine_dependence (ddr);
	compute_distance_vector (ddr);
	compute_direction_vector (ddr);
      }
}



/* This section contains the affine functions dependences detector.  */

/* This is the simplest data dependence test: determines whether the
   data references A and B access the same array.  */

static inline bool
array_base_name_differ_p (struct data_reference *a, 
			  struct data_reference *b)
{
  if (DR_BASE_NAME (a) == DR_BASE_NAME (b))
    return false;
  
  if (TREE_CODE (DR_BASE_NAME (a)) == INDIRECT_REF
      && TREE_CODE (DR_BASE_NAME (b)) == INDIRECT_REF
      && TREE_OPERAND (DR_BASE_NAME (a), 0) 
      == TREE_OPERAND (DR_BASE_NAME (b), 0))
    return false;
  
  return true;
}

/* DN */
bool
vec_array_base_name_differ_p (struct data_reference *a,
                              struct data_reference *b)
{
  if (DR_BASE_NAME (a) == DR_BASE_NAME (b))
    return false;

  if (TREE_CODE (DR_BASE_NAME (a)) == INDIRECT_REF
      && TREE_CODE (DR_BASE_NAME (b)) == INDIRECT_REF
      && TREE_OPERAND (DR_BASE_NAME (a), 0)
      == TREE_OPERAND (DR_BASE_NAME (b), 0))
    return false;

  return true;
}
/* DN */

/* This is the subscript dependence tester (SubDT).  */

static void
subscript_dependence_tester (struct data_dependence_relation *ddr)
{
  unsigned int i;
  struct data_reference *dra = DDR_A (ddr);
  struct data_reference *drb = DDR_B (ddr);
  
  DBG_S (fprintf (stderr, "(subscript_dependence_tester \n"));
  
#if defined ENABLE_CHECKING
  if (dra == NULL 
      || drb == NULL)
    abort ();
#endif
  
  for (i = 0; i < DDR_NUM_SUBSCRIPTS (ddr); i++)
    {
      tree overlaps_a, overlaps_b;
      struct subscript *subscript = DDR_SUBSCRIPT (ddr, i);
      
      analyze_overlapping_iterations (DR_ACCESS_FN (dra, i), 
				      DR_ACCESS_FN (drb, i),
				      &overlaps_a, &overlaps_b);
      
      if (overlaps_a == chrec_top
 	  || overlaps_b == chrec_top)
 	{
 	  DDR_ARE_DEPENDENT (ddr) = chrec_top;
 	  DDR_SUBSCRIPTS_VECTOR_FINALIZE (ddr);
 	  break;
 	}
      
      else if (overlaps_a == chrec_bot
 	       || overlaps_b == chrec_bot)
 	{
 	  DDR_ARE_DEPENDENT (ddr) = chrec_bot;
 	  DDR_SUBSCRIPTS_VECTOR_FINALIZE (ddr);
 	  break;
 	}
      
      else
 	{
 	  SUB_CONFLICTS_IN_A (subscript) = overlaps_a;
 	  SUB_CONFLICTS_IN_B (subscript) = overlaps_b;
 	}
    }
  
  DBG_S (fprintf (stderr, ")\n"));
}

/* This is the subscript {coupling | conflict} tester (SubCT).  
   
   What is subscript coupling?  In the following example, the analyzer
   is able to determine that there is no dependence between the two
   references.
   
   | loop i = 0, N, 1
   |   T[i+1][i] = ...
   |   ... = T[i][i]
   | endloop
   
   For a complete discussion on the subscript coupling, read the
   chapter 3 in "Advanced Compilation for High Performance Computing"
   by Randy Allen and Ken Kennedy.
   
   How it works?  Solving this problem is similar to finding
   conflicting iterations tuples in the per subscript dependence
   tester (SubDT).  Instead of looking to the conflicting accessed
   elements, here we look for conflicting iterations between the
   conflicting tuples given by the SubDT.  SubDT applies the
   chrec_intersection vertically (see example above), while SubCT
   applies the chrec_intersection horizontally on the SubDT result.
   
   For the example above:
   - SubDT constructs the following conflicting tuples:
   
   A = DataRef_0: T[i+1][i]
   B = DataRef_1: T[i][i]
   (Affine Dep (A = 0, B = 1):
     (subscript 0: 
       iterations_that_access_an_element_twice_in_A : {0, +, 1}_1
       iterations_that_access_an_element_twice_in_B : {0, +, 1}_1
     )
     (subscript 1: 
       iterations_that_access_an_element_twice_in_A : {0, +, 1}_1
       iterations_that_access_an_element_twice_in_B : {1, +, 1}_1
     )
   )
   
   From this information, the analyzer constructs the intersections:
   chrec_intersection (sub_0_twice_in_A, sub_1_twice_in_A) = {0, +, 1}_1
   chrec_intersection (sub_0_twice_in_B, sub_1_twice_in_B) = {1, +, 1}_1
   FIXME...
*/

static void
subscript_coupling_tester (struct data_dependence_relation *res ATTRIBUTE_UNUSED)
{
  DBG_S (fprintf (stderr, "(subscript_coupling_tester \n"));
  
  DBG_S (fprintf (stderr, ")\n"));
}

/* For all the subscripts, set the same value: CHREC.  */

static void
set_all_subscripts_to (struct data_dependence_relation *ddr, 
		       tree chrec)
{
  unsigned int i;
  
  for (i = 0; i < DDR_NUM_SUBSCRIPTS (ddr); i++)
    {
      struct subscript *subscript = DDR_SUBSCRIPT (ddr, i);
      SUB_CONFLICTS_IN_A (subscript) = chrec;
      SUB_CONFLICTS_IN_B (subscript) = chrec;
    }
}

/* This computes the affine dependence relation between A and B.
   CHREC_BOT is used for representing the independence between two
   accesses, while CHREC_TOP is used for representing the unknown
   relation.
   
   Note that it is possible to stop the computation of the dependence
   relation the first time we detect a CHREC_BOT element for a given
   subscript.  */

void
compute_affine_dependence (struct data_dependence_relation *ddr)
{
  struct data_reference *dra = DDR_A (ddr);
  struct data_reference *drb = DDR_B (ddr);
  
  DBG_S (fprintf (stderr, "(compute_affine_dependence (%d, %d)\n", 
		  DR_ID (dra), DR_ID (drb));
	 fprintf (stderr, "  expr_a = \n");
	 debug_generic_expr (DR_EXPR (dra));
	 fprintf (stderr, "  expr_b = \n");
	 debug_generic_expr (DR_EXPR (drb)));
  
  /* When the base name differs, there is no dependence.  */
  if (array_base_name_differ_p (dra, drb))
    {
      DBG_S (fprintf (stderr, "Base name differs.  \n"));
      set_all_subscripts_to (ddr, chrec_bot);
    }
  
  else if (access_functions_are_affine_or_constant_p (dra)
	   && access_functions_are_affine_or_constant_p (drb))
    {
      subscript_dependence_tester (ddr);
      subscript_coupling_tester (ddr);
    }
  
  /* As a last case, if the dependence cannot be determined, or if
     the dependence is considered too difficult to determine, answer
     "don't know".  */
  else
    {
      DBG_S (fprintf (stderr, "I'm not smart enough for this dependence test," 
		      "please teach me what I should answer.  \n"));
      set_all_subscripts_to (ddr, chrec_top);
    }
  
  DBG_S (fprintf (stderr, ")\n"));
}



/* This section contains all the entry points.  */

/* Entry point.  Search the data references in a loop nest.  Record
   the information into a list of DATA_REFERENCE structures.
   
   FIXME: This is a dumb walker over all the trees in the loop body.
   Try to use the SSA representation for speed it up.  */

void 
find_data_references (struct loop *loop_nest, varray_type datarefs)
{
  unsigned int j;
  basic_block bb, *bbs;
  block_stmt_iterator bsi;
  
  bbs = get_loop_body (loop_nest);
  
  for (j = loop_nest->num_nodes; j > 0; j--)
    {
      bb = bbs[j - 1];
      
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
          tree expr = bsi_stmt (bsi);
	  vdef_optype vdefs = VDEF_OPS (stmt_ann (expr));
	  vuse_optype vuses = VUSE_OPS (stmt_ann (expr));
	  
	  if (vuses || vdefs)
	    switch (TREE_CODE (expr))
	      {
	      case MODIFY_EXPR:
		/* In the GIMPLE representation, the left-hand side of
		   a modify expression is either an ARRAY_REF or
		   otherwise it does not contain other ARRAY_REFs.  */
		if (TREE_CODE (TREE_OPERAND (expr, 0)) == ARRAY_REF)
		  analyze_array (loop_nest, datarefs, expr, 
				 TREE_OPERAND (expr, 0));
		if (TREE_CODE (TREE_OPERAND (expr, 1)) == ARRAY_REF)
		  analyze_array (loop_nest, datarefs, expr, 
				 TREE_OPERAND (expr, 1));
		break;
		
	      case COND_EXPR:
	      case CALL_EXPR:
	      case VA_ARG_EXPR:
	      case ASM_EXPR:
		/* In the GIMPLE representation, these nodes do not
		   contain ARRAY_REFs in their operands.  */
		break;
		
	      default:
		debug_tree (expr);
		/* abort (); */
		fprintf (stderr, "Find ARRAY_REFs: abort.\n");
		break;
	      }
	}
    }
  free (bbs);
}

/* Entry point.  Analyze all the data references and the dependence
   relations in the given LOOP_NEST.

   The data references are computed first.  
   
   A relation on these nodes is represented by a complete graph.  Some
   of the relations could be of no interest, thus the relations are
   computed on demand.
   
   In the following function we compute all the relations.  This is
   just a first implementation that is here for:
   - for showing how to ask for the dependence relations, 
   - for the debugging the whole dependence graphs,
   - for the dejagnu testcases and maintenance.
   
   It is possible to ask only for a part of the graph, avoiding to
   compute the whole dependence graph.  The computed dependences are
   stored in a knowledge base (KB) such that later queries don't
   recompute the same information.  The implementation of this KB is
   transparent to the optimizer, and thus the KB can be changed with a
   more efficient implementation, or the KB could be disabled.  */

void 
analyze_all_data_dependences (struct loop *loop_nest)
{
  varray_type datarefs;
  varray_type dependence_relations;
  
  timevar_push (TV_ALL_DATA_DEPS);
  dump_file = dump_begin (TDI_alldd, &dump_flags);
  VARRAY_GENERIC_PTR_INIT (datarefs, 10, "datarefs");
  VARRAY_GENERIC_PTR_INIT (dependence_relations, 10*10,
			   "dependence_relations");
  
  find_data_references (loop_nest, datarefs);
  compute_all_dependences (datarefs, dependence_relations);
  
  if (dump_file)
    {
      dump_data_references (dump_file, datarefs);
      dump_data_dependence_relations (dump_file, dependence_relations);
      fprintf (dump_file, "\n\n");
      dump_end (TDI_alldd, dump_file);
    }
  
  varray_clear (dependence_relations);
  varray_clear (datarefs);
  timevar_pop (TV_ALL_DATA_DEPS);
}

