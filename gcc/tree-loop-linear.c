/* Linear Loop transforms
   Copyright (C) 2003, 2004 Free Software Foundation, Inc.
   Contributed by Daniel Berlin <dberlin@dberlin.org>.

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
#include "expr.h"
#include "optabs.h"
#include "tree-chrec.h"
#include "tree-data-ref.h"
#include "tree-scalar-evolution.h"
#include "tree-pass.h"
#include "varray.h"
#include "lambda.h"

/* Linear loop transforms include any composition of interchange,
   scaling, skewing, and reversal.  They are used to change the
   iteration order of loop nests in order to optimize data locality of
   traversals, or remove dependences that prevent
   parallelization/vectorization/etc.  

   TODO: Determine reuse vectors/matrix and use it to determine optimal
   transform matrix for locality purposes.
   TODO: Completion of partial transforms.  */

/* Returns the sum of all the data dependence distances carried by
   loop COL.  */

static int 
sum_distances_on_loop (varray_type dists, 
		       unsigned int loop_number)
{
  int res = 0;
  unsigned int i;
  
  for (i = 0; i < VARRAY_ACTIVE_SIZE (dists); i++)
    {
      lambda_vector distance = VARRAY_GENERIC_PTR (dists, i);
      res += distance[loop_number];
    }

  return res;
}

/* Apply to TRANS any loop interchange that minimize inner loop steps.
   Returns the new transform matrix.  The smaller the reuse vector
   distances in the inner loops, the fewer the cache misses.  */

static lambda_trans_matrix
try_interchange_loops (lambda_trans_matrix trans, 
		       unsigned int depth,		       
		       varray_type classic_dir, 
		       varray_type classic_dist)
{
  unsigned int loop_i, loop_j;
  int sum_i, sum_j;

  for (loop_i = 0; loop_i < depth; loop_i++)
    for (loop_j = 0; loop_j < depth; loop_j++)
      {
	if (loop_i != loop_j)
	  {
	    /* Try to permute rows LOOP_I and LOOP_J.  The basic
	       profitability model is the minimization of the steps in
	       the inner loops.  */
	    sum_i = sum_distances_on_loop (classic_dist, loop_i);
	    sum_j = sum_distances_on_loop (classic_dist, loop_j);

	    /* When LOOP_I contains smaller steps than LOOP_J, and
	       LOOP_I is outer than LOOP_J, then interchange
	       loops.  */
	    if (sum_i < sum_j 
		&& loop_i < loop_j)
	      lambda_matrix_row_exchange (LTM_MATRIX (trans), loop_i, loop_j);
	    
	    /* Validate the resulting matrix.  When the transformation
	       is not valid, reverse to the previous matrix.  
	       
	       FIXME: In this case of transformation it could be
	       faster to verify the validity of the interchange
	       without applying the transform to the matrix.  But for
	       the moment do it cleanly: this is just a prototype.  */
	    if (!lambda_transform_legal_p (trans, depth, classic_dir, classic_dist))
	      lambda_matrix_row_exchange (LTM_MATRIX (trans), loop_i, loop_j);
	  }
      }
  
  return trans;
}

/* Perform a set of linear transforms on LOOPS.  */

void
linear_transform_loops (struct loops *loops)
{
  unsigned int i;  
  unsigned int depth = 0;
  unsigned int j;
  varray_type classic_dist;
  varray_type classic_dir;
  varray_type datarefs;
  varray_type dependence_relations;

  for (i = 1; i < loops->num; i++)
    {
      struct loop *loop_nest = loops->parray[i];
      struct loop *temp;
      varray_type oldivs;
      varray_type invariants;
      lambda_loopnest before, after;
      lambda_trans_matrix trans;
      bool problem = false;
      /* If it's not a loop nest, we don't want it.
         We also don't handle sibling loops properly, 
         which are loops of the following form:
         for (i = 0; i < 50; i++)
           {
             for (j = 0; j < 50; j++)
               {
	        ...
               }
           for (j = 0; j < 50; j++)
               {
                ...
               }
           } */
      if (!loop_nest->inner)
	continue;
      for (temp = loop_nest; temp; temp = temp->inner)
	{
	  flow_loop_scan (temp, LOOP_ALL);
	  /* If we have a sibling loop or multiple exit edges, jump ship.  */
	  if (temp->next || temp->num_exits != 1)
	    {
	      problem = true;
	      break;
	    }
	  depth ++;
	}
      if (problem)
	continue;

      /* Analyze data references and dependence relations using scev.  */      
 
      VARRAY_GENERIC_PTR_INIT (classic_dist, 10, "classic_dist");
      VARRAY_GENERIC_PTR_INIT (classic_dir, 10, "classic_dir");
      VARRAY_GENERIC_PTR_INIT (datarefs, 10, "datarefs");
      VARRAY_GENERIC_PTR_INIT (dependence_relations, 10,
			       "dependence_relations");
      
  
      compute_data_dependences_for_loop (depth, loop_nest,
					 &datarefs, &dependence_relations, 
					 &classic_dist, &classic_dir);
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  for (j = 0; j < VARRAY_ACTIVE_SIZE (classic_dist); j++)
	    {
	      fprintf (dump_file, "DISTANCE_V (");
	      print_lambda_vector (dump_file, 
				   VARRAY_GENERIC_PTR (classic_dist, j),
				   depth);
	      fprintf (dump_file, ")\n");
	    }
	  for (j = 0; j < VARRAY_ACTIVE_SIZE (classic_dir); j++)
	    {
	      fprintf (dump_file, "DIRECTION_V (");
	      print_lambda_vector (dump_file, 
				   VARRAY_GENERIC_PTR (classic_dir, j),
				   depth);
	      fprintf (dump_file, ")\n");
	    }
	  fprintf (dump_file, "\n\n");
	}
      /* Build the transformation matrix.  */
      trans = lambda_trans_matrix_new (depth, depth);
#if 1
      lambda_matrix_id (LTM_MATRIX (trans), depth);
      trans = try_interchange_loops (trans, depth, classic_dir, classic_dist);
#else
      /* This is a 2x2 interchange matrix.  */
      LTM_MATRIX (trans)[0][0] = 0;
      LTM_MATRIX (trans)[0][1] = 1;
      LTM_MATRIX (trans)[1][0] = 1;
      LTM_MATRIX (trans)[1][1] = 0;
#endif
      /* Check whether the transformation is legal.  */
      if (!lambda_transform_legal_p (trans, depth, classic_dir, classic_dist))
	{
	  if (dump_file)
	    fprintf (dump_file, "Can't transform loop, transform is illegal:\n");
	  continue;
	}
      before = gcc_loopnest_to_lambda_loopnest (loop_nest, &oldivs, &invariants);
      if (!before)
	continue;
            
      if (dump_file)
	{
	  fprintf (dump_file, "Before:\n");
	  print_lambda_loopnest (dump_file, before, 'i');
	}
  
      after = lambda_loopnest_transform (before, trans);
      if (dump_file)
	{
	  fprintf (dump_file, "After:\n");
	  print_lambda_loopnest (dump_file, after, 'u');
	}
      lambda_loopnest_to_gcc_loopnest (loop_nest, oldivs, invariants,
				       after, trans);
    }
}
