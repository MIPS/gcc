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

/* This pass walks the whole program searching for array references.
   The array accesses are recorded in DATA_REFERENCE nodes.  Since the
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
   
   - to determine the independence: the relation between two
     independent accesses is qualified with the chrec_bot (this
     information allows a loop parallelization),
     
   - when two data references access the same data, to qualify the
     dependence relation with classic dependence representations:
     
       - distance vectors
       - direction vectors
       - loop carried level dependence
       - polyhedron dependence
     or with the chains of recurrences based representation,
     
     
   - to define a knowledge base for storing the data dependeces 
     information,
     
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
#include "tree-pass.h"

static tree analyze_array_indexes (unsigned, varray_type, tree);
static bool access_functions_are_affine_or_constant_p (struct data_reference *);

static struct data_dependence_relation *
initialize_data_dependence_relation (struct data_reference *, 
				     struct data_reference *);

static void subscript_dependence_tester (struct data_dependence_relation *);
static void subscript_coupling_tester (struct data_dependence_relation *);

static unsigned int data_ref_id = 0;



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
  
  fprintf (outf, "(Data Ref %d: \n  stmt: ", DR_ID (dr));
  print_generic_stmt (outf, DR_STMT (dr), 0);
  fprintf (outf, "  ref: ");
  print_generic_stmt (outf, DR_REF (dr), 0);
  fprintf (outf, "  base_name: ");
  print_generic_stmt (outf, DR_BASE_NAME (dr), 0);
  
  for (i = 0; i < DR_NUM_DIMENSIONS (dr); i++)
    {
      fprintf (outf, "  Access function %d: ", i);
      print_generic_stmt (outf, DR_ACCESS_FN (dr, i), 0);
    }
  fprintf (outf, ")\n");
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
    fprintf (outf, "    (don't know)\n");
  
  else if (DDR_ARE_DEPENDENT (ddr) == chrec_bot)
    fprintf (outf, "    (no dependence)\n");
  
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
analyze_array_indexes (unsigned loop_nb, 
		       varray_type access_fns, 
		       tree ref)
{
  tree opnd0, opnd1;
  tree access_fn;
  
  opnd0 = TREE_OPERAND (ref, 0);
  opnd1 = TREE_OPERAND (ref, 1);
  
  /* The detection of the evolution function for this data access is
     postponed until the dependence test.  This lazy strategy avoids
     the computation of access functions that are of no interest for
     the optimizers.  */
  access_fn = instantiate_parameters 
    (loop_nb, analyze_scalar_evolution (loop_nb, opnd1));
  
  VARRAY_PUSH_TREE (access_fns, access_fn);
  
  /* Recursively record other array access functions.  */
  if (TREE_CODE (opnd0) == ARRAY_REF)
    return analyze_array_indexes (loop_nb, access_fns, opnd0);
  
  /* Return the base name of the data access.  */
  else
    return opnd0;
}

/* For a data reference REF contained in the statemet STMT, initialize
   a DATA_REFERENCE structure, and return it.  */

struct data_reference *
analyze_array (tree stmt, 
	       tree ref)
{
  struct data_reference *res;

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "(analyze_array \n");
      fprintf (tree_dump_file, "  (ref = ");
      print_generic_stmt (tree_dump_file, ref, 0);
      fprintf (tree_dump_file, ")\n");
    }
  
  res = ggc_alloc (sizeof (struct data_reference));
  
  DR_ID (res) = data_ref_id++;
  DR_STMT (res) = stmt;
  DR_REF (res) = ref;
  VARRAY_TREE_INIT (DR_ACCESS_FNS (res), 5, "access_fns");
  DR_BASE_NAME (res) = analyze_array_indexes 
    (loop_num (loop_of_stmt (stmt)), DR_ACCESS_FNS (res), ref);

  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, ")\n");
  
  return res;
}



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
  if (DR_NUM_DIMENSIONS (a) != DR_NUM_DIMENSIONS (b)
      || array_base_name_differ_p (a, b))
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
 	  tree distance;
 	  struct subscript *subscript;
	  bool value;
 	  
 	  subscript = DDR_SUBSCRIPT (ddr, i);
 	  distance = SUB_DISTANCE (subscript);
 	  
	  if (distance == chrec_top)
	    SUB_DIRECTION (subscript) = dir_star;
	  
 	  else if (chrec_zerop (distance))
 	    SUB_DIRECTION (subscript) = dir_equal;
 	  
 	  else if (!chrec_is_positive (distance, &value))
 	    SUB_DIRECTION (subscript) = dir_star;
	  
	  else if (value == true)
 	    SUB_DIRECTION (subscript) = dir_positive;
	  
	  else
	    SUB_DIRECTION (subscript) = dir_negative;
	}
    }
}

/* Compute all the data dependence relations.  */

void 
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

/* This is the subscript dependence tester (SubDT).  It computes the
   conflicting iterations.  */

static void
subscript_dependence_tester (struct data_dependence_relation *ddr)
{
  unsigned int i;
  struct data_reference *dra = DDR_A (ddr);
  struct data_reference *drb = DDR_B (ddr);
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "(subscript_dependence_tester \n");
  
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
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, ")\n");
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
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, "(subscript_coupling_tester \n");
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, ")\n");
}

/* For all the subscripts, set the same value: CHREC.  */

static void
set_all_subscripts_to (struct data_dependence_relation *ddr, 
		       tree chrec)
{
  unsigned int i;
  
  if (chrec == chrec_top
      || chrec == chrec_bot)
    {
      DDR_ARE_DEPENDENT (ddr) = chrec;
      DDR_SUBSCRIPTS_VECTOR_FINALIZE (ddr);
    }
  
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
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    {
      fprintf (tree_dump_file, "(compute_affine_dependence (%d, %d)\n", 
	       DR_ID (dra), DR_ID (drb));
      fprintf (tree_dump_file, "  (stmt_a = \n");
      print_generic_expr (tree_dump_file, DR_STMT (dra), 0);
      fprintf (tree_dump_file, ")\n  (stmt_b = \n");
      print_generic_expr (tree_dump_file, DR_STMT (drb), 0);
      fprintf (tree_dump_file, ")\n");
    }
  
  /* Analyze only when the dependence relation is not yet known.  */
  if (DDR_ARE_DEPENDENT (ddr) == NULL_TREE)
    {
      if (access_functions_are_affine_or_constant_p (dra)
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
	  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
	    fprintf (tree_dump_file, "I'm not smart enough for this dependence test," 
		     "please teach me what I should answer.  \n");
	  
	  set_all_subscripts_to (ddr, chrec_top);
	}
    }
  
  if (tree_dump_file && (tree_dump_flags & TDF_DETAILS))
    fprintf (tree_dump_file, ")\n");
}



/* This section contains all the entry points.  */

/* Entry point.  Search the data references in a loop nest.  Record
   the information into a list of DATA_REFERENCE structures.
   
   FIXME: This is a "dumb" walker over all the trees in the loop body.
   Find another technique that avoids this costly walk.  This is
   acceptable for the moment, since this function is used only for
   debugging purposes.  */

void 
find_data_references (varray_type datarefs)
{
  basic_block bb;
  block_stmt_iterator bsi;
  
  FOR_EACH_BB (bb)
    {
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
		  VARRAY_PUSH_GENERIC_PTR 
		    (datarefs, analyze_array (expr, TREE_OPERAND (expr, 0)));
		
		if (TREE_CODE (TREE_OPERAND (expr, 1)) == ARRAY_REF)
		  VARRAY_PUSH_GENERIC_PTR 
		    (datarefs, analyze_array (expr, TREE_OPERAND (expr, 1)));
		
		break;
		
	      case COND_EXPR:
	      case CALL_EXPR:
	      case VA_ARG_EXPR:
	      case ASM_EXPR:
	      case RETURN_EXPR:
		/* In the GIMPLE representation, these nodes do not
		   contain ARRAY_REFs in their operands.  */
		break;
		
	      default:
		debug_tree (expr);
		/* abort (); */
		fprintf (tree_dump_file, "Find ARRAY_REFs: abort.\n");
		break;
	      }
	}
    }
}

/* Entry point.  Analyze all the data references and the dependence
   relations.

   The data references are computed first.  
   
   A relation on these nodes is represented by a complete graph.  Some
   of the relations could be of no interest, thus the relations can be
   computed on demand.
   
   In the following function we compute all the relations.  This is
   just a first implementation that is here for:
   - for showing how to ask for the dependence relations, 
   - for the debugging the whole dependence graph,
   - for the dejagnu testcases and maintenance.
   
   It is possible to ask only for a part of the graph, avoiding to
   compute the whole dependence graph.  The computed dependences are
   stored in a knowledge base (KB) such that later queries don't
   recompute the same information.  The implementation of this KB is
   transparent to the optimizer, and thus the KB can be changed with a
   more efficient implementation, or the KB could be disabled.  */

void 
analyze_all_data_dependences (void)
{
  varray_type datarefs;
  varray_type dependence_relations;
  
  /* When computing the whole data dependence graph, this is the
     maximum number of nodes that we want to compute.  */
  int alldd_max_size = 100;
  
  VARRAY_GENERIC_PTR_INIT (datarefs, alldd_max_size, "datarefs");
  VARRAY_GENERIC_PTR_INIT (dependence_relations, 
			   alldd_max_size * alldd_max_size,
			   "dependence_relations");
  
  find_data_references (datarefs);
  compute_all_dependences (datarefs, dependence_relations);
  
  if (tree_dump_file)
    {
      dump_data_dependence_relations (tree_dump_file, dependence_relations);
      fprintf (tree_dump_file, "\n\n");
    }
  
  varray_clear (dependence_relations);
  varray_clear (datarefs);
}

