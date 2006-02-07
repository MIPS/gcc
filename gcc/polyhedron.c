/* Linear constraint systems.
   Copyright (C) 2005 Free Software Foundation, Inc.
   Contributed by Sebastian Pop <sebastian.pop@cri.ensmp.fr>

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

/* This file defines a structure for polyhedra and operations on
   polyhedra.  Polyhedra have two possible representations: 
   
   - polyhedra as a set of linear equalities and inequalities,
   aka. linear constraint systems, defined in this file,

   - polyhedra as a set of vertices, rays, and lines, aka. generating
     systems.

   References:

   - IRISA Publication Interne 785: "A library for doing polyhedral
   operations" Doran Wilde.

*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "errors.h"
#include "ggc.h"
#include "tree.h"
#include "varray.h"
#include "sbitmap.h"
#include "vec.h"
#include "polyhedron.h"

/* Return a new constraint system.  */

csys
csys_new (int dimension, int nb_eqs, int nb_constraints)
{
  csys res = ggc_alloc_cleared (sizeof (struct csys));

  CSYS_DIMENSION (res) = dimension;
  CSYS_NB_EQS (res) = nb_eqs;
  CSYS_NB_CONSTRAINTS (res) = nb_constraints;
  CSYS_M (res) = lambda_matrix_new (nb_constraints, dimension + 2);
  return res;
}

/* Return a copy of CY.  */

csys 
csys_copy (csys cy)
{
  csys res = ggc_alloc_cleared (sizeof (csys));

  CSYS_DIMENSION (res) = CSYS_DIMENSION (cy);
  CSYS_NB_EQS (res) = CSYS_NB_EQS (cy);
  CSYS_NB_CONSTRAINTS (res) = CSYS_NB_CONSTRAINTS (cy);
  lambda_matrix_copy (CSYS_M (cy), CSYS_M (res),
		      CSYS_NB_CONSTRAINTS (cy), CSYS_DIMENSION (cy) + 2);
  return res;
}

/* Print to FILE the constraint system CY.  */

void 
csys_print (FILE *file, csys cy)
{
  int i, j;

  if (cy == NULL)
    {
      fprintf (file, "null\n");
      return;
    }

  fprintf (file, "(constraint_system \n");
  fprintf (file, "  dim = %d\n", CSYS_DIMENSION (cy));
  fprintf (file, "  nb_eqs = %d\n", CSYS_NB_EQS (cy));
  fprintf (file, "  nb_constraints = %d\n", CSYS_NB_CONSTRAINTS (cy));
  fprintf (file, "  constraints_matrix = (\n");
  fprintf (file, "%d %d\n", CSYS_NB_CONSTRAINTS (cy), CSYS_DIMENSION (cy) + 2);
  print_lambda_matrix (file, CSYS_M (cy), CSYS_NB_CONSTRAINTS (cy),
		       CSYS_DIMENSION (cy) + 2);
  fprintf (file, "  )\n");
  fprintf (file, "  constraints = (\n");

  for (i = 0; i < CSYS_NB_CONSTRAINTS (cy); i++)
    {
      if (CSYS_IS_EQ (cy, i))
	fprintf (file, "  0  = ");
      else
	fprintf (file, "  0 <= ");

      for (j = 0; j < CSYS_DIMENSION (cy); j++)
	if (CSYS_VEC (cy, i, j))
	  fprintf (file, "%3d.x%d + ", CSYS_VEC (cy, i, j), j);
      fprintf (file, "%3d\n", CSYS_CST (cy, i));
    }
  fprintf (file, "  )\n");
  fprintf (file, ")\n");
}

/* Debugging function for gdb calls.  */

void
debug_csys (csys cy)
{
  csys_print (stderr, cy);
}

/* Return a new generating system.  */

gsys
gsys_new (int dimension, int nb_lines, int nb_rays)
{
  gsys res = ggc_alloc_cleared (sizeof (struct gsys));

  GSYS_DIMENSION (res) = dimension;
  GSYS_NB_LINES (res) = nb_lines;
  GSYS_NB_RAYS (res) = nb_rays;
  GSYS_M (res) = lambda_matrix_new (nb_rays, dimension + 2);
  return res;
}

/* Return a copy of GY.  */

gsys
gsys_copy (gsys gy)
{
  gsys res = ggc_alloc_cleared (sizeof (gsys));

  GSYS_DIMENSION (res) = GSYS_DIMENSION (gy);
  GSYS_NB_LINES (res) = GSYS_NB_LINES (gy);
  GSYS_NB_RAYS (res) = GSYS_NB_RAYS (gy);
  lambda_matrix_copy (GSYS_M (gy), GSYS_M (res),
		      GSYS_NB_RAYS (gy), GSYS_DIMENSION (gy) + 2);
  return res;
}

/* Print to FILE the generating system GY.  */

void
gsys_print (FILE *file, gsys gy)
{
  if (gy == NULL)
    {
      fprintf (file, "null\n");
      return;
    }

  fprintf (file, "(gsys \n");
  fprintf (file, "  dim = %d\n", GSYS_DIMENSION (gy));
  fprintf (file, "  nb_lines = %d\n", GSYS_NB_LINES (gy));
  fprintf (file, "  nb_rays = %d\n", GSYS_NB_RAYS (gy));
  fprintf (file, "  rays_matrix = (\n");
  fprintf (file, "%d %d\n", GSYS_NB_RAYS (gy), GSYS_DIMENSION (gy) + 2);
  print_lambda_matrix (file, GSYS_M (gy), GSYS_NB_RAYS (gy), 
		       GSYS_DIMENSION (gy) + 2);
  fprintf (file, "  )\n");
  fprintf (file, ")\n");
}

/* Debugging function for gdb calls.  */

void debug_gsys (gsys gy)
{
  gsys_print (stderr, gy);
}

/* Return a new polyhedron from a constraint system CY and a
   generating system GY.  */

polyhedron
polyhedron_new (csys cy, gsys gy, lambda_matrix sat)
{
  polyhedron pol = ggc_alloc_cleared (sizeof (struct polyhedron));

  POLYH_CSYS (pol) = cy;
  POLYH_GSYS (pol) = gy;
  POLYH_SAT (pol) = sat;
  return pol;
}

/* Return the omega problem corresponding to the constraint system
   CY.  */

omega_pb
csys_to_omega (csys cy)
{
  int i, j;

  /* Omega expects the protected variables (those that have to be kept
     after elimination) to appear first in the constraint system.
     These variables are the distance variables.  */
  int nb_protected_vars = CSYS_DIMENSION (cy) / 2;
  omega_pb res = omega_alloc_problem (CSYS_DIMENSION (cy), nb_protected_vars);

  for (i = 0; i < CSYS_NB_CONSTRAINTS (cy); i++)
    {
      if (CSYS_IS_EQ (cy, i))
	{
	  unsigned idx = omega_add_zero_eq (res, omega_black);

	  for (j = 1; j <= CSYS_DIMENSION (cy); j++)
	    res->eqs[idx].coef[j] = CSYS_ELT (cy, i, j);
	  res->eqs[idx].coef[0] = CSYS_CST (cy, i);
	}
      else
	{
	  unsigned idx = omega_add_zero_geq (res, omega_black);

	  for (j = 1; j <= CSYS_DIMENSION (cy); j++)
	    res->geqs[idx].coef[j] = CSYS_ELT (cy, i, j);
	  res->geqs[idx].coef[0] = CSYS_CST (cy, i);
	}
    }
  return res;
}



/* To generating systems.  */

/* Compute RES as a linear combination of R1 and R2 such that 
   RES [POS] == 0.  All vectors have length SIZE.  */

static inline void 
linear_combine (lambda_vector r1,
		lambda_vector r2,
		lambda_vector res,
		int pos, int size)
{
  int lambda1 = r2[pos];
  int lambda2 = r1[pos];
  int g = gcd (lambda1, lambda2);

  lambda_vector_add_mc (r1 + 1, lambda1 / g,
			r2 + 1, - (lambda2 / g),
			res + 1, size);
  lambda_vector_normalize (res + 1, size);
}

/* Compute a minimal system of equations using Gausian elimination
   method.  MAT is a matrix of constraints in which the first NB_EQ
   constraints are equations.  The dimension of the homogenous system
   is DIMENSION.  The function returns the rank of the matrix.  */

static int
csys_Gauss (lambda_matrix mat,
	    int nb_eq,
	    int dimension,
	    int nb_rows)
{
  int i, j, k, rank;
  lambda_vector column_index = lambda_vector_new (dimension);

  rank=0;
  for (j = 1; j <= dimension; j++)
    {
      for (i = rank; i < nb_eq; i++)
	if (mat[i][j])
	  break;

      if (i != nb_eq)
	{
	  if (i != rank)
	    lambda_vector_exchange (mat[rank] + 1, mat[i] + 1, dimension);

	  lambda_vector_normalize (mat[rank] + 1, dimension);

	  if (mat[rank][j] < 0)
	    lambda_vector_negate (mat[rank] + 1, mat[rank] + 1, dimension);

	  for (i = i + 1; i < nb_eq; i++)
	    if (mat[i][j])
	      linear_combine (mat[i], mat[rank], mat[i], j, dimension);

	  column_index[rank] = j;
	  rank++;
	}
    }

  for (k = rank - 1; k >= 0; k--)
    {
      j = column_index[k];
      
      for (i = 0; i < k; i++)
	if (mat[i][j])
	  linear_combine (mat[i], mat[k], mat[i], j, dimension);
      
      for (i = nb_eq; i < nb_rows; i++)
	if (mat[i][j])
	  linear_combine (mat[i], mat[k], mat[i], j, dimension);
    }

  return rank;
}

/* Initialize CONS_P, RES_P, NB_CONS_P and NB_RAYS_P to the empty
   polyhedron in dimension DIM.  */

static polyhedron
empty_polyhedron (polyhedron pol)
{
  int i;
  int dim = POLYH_DIM (pol);

  POLYH_CONS (pol) = lambda_matrix_new (dim + 1, dim + 2);
  for (i = 0; i <= dim; i++)
    POLYH_CONS (pol)[i][i+1] = 1;

  POLYH_NB_CONS (pol) = dim + 1;
  POLYH_NB_RAYS (pol) = 0;
  POLYH_RAYS (pol) = NULL;

  return pol;
}

#define MARK_SATURATED(S, R, C) (S[R][C] = 0)
#define SATURATED_P(S, R, C) (S[R][C] == 0)
#define MARK_NOT_SATURATED(S, R, C) (S[R][C] = 1)
#define NOT_SATURATED_P(S, R, C) (S[R][C] == 1)

/* R_TO_C should be true when translating from rays to constraints.

   Compute the dual matrix representation RAYS_P starting from a an
   initial representation START.  Because the rays and constraints
   representations are dual, this algorithm can translate from one to
   the other and vice versa.  The algorithm has been proposed by
   Motzkin, then improved by Doran Wilde.

   Notes: 
   
   - a ray "I" does not saturate a constraint "K" iff the scalar
   product is not null: "I dot K != 0".

   - matrix layouts: 
   START has the following structure:
   
   |IS_INEQ  VECTORS  CST
   |   0      1 0 5    9
   |   1      1 2 3   -8

   RES has the following structure:
   
   |IS_RAY   VECTORS  CST
   |   0      1 0 5    0
   |   1      1 2 3    3
   
   When IS_RAY is 0 VECTORS represent a line and constant is 0.
   
   When IS_RAY is 1 VECTORS represent: a ray if CST is 0, otherwise it
   is a vertex represented in the homogeneous space.

   Reference: IRISA Publication Interne 785: "A library for doing
   polyhedral operations" Doran Wilde, pages 29 to 34.
*/

static polyhedron
csys_gsys_compute_dual (polyhedron pol, bool r_to_c)
{
  lambda_matrix cons = POLYH_CONS (pol);
  lambda_matrix rays = POLYH_RAYS (pol);
  lambda_matrix sat = POLYH_SAT (pol);

  int nb_constraints = POLYH_NB_CONS (pol);
  int nb_rays = POLYH_NB_RAYS (pol);
  int max_ray = nb_rays;
  int nb_lines = POLYH_NB_LINES (pol);
  int dim = POLYH_DIM (pol);

  int i, j, k, l;

  /* From now on, working in the homogeneous space.  */
  dim++;

  /* Initialize a basis for the space.  */
  for (i = 0; i < nb_lines; i++)
    rays[i][i + 1] = 1;

  /* Include the point at the origin.  */
  rays[nb_lines][0] = 1;
  rays[nb_lines][dim] = 1;

  for (k = 0; k < nb_constraints; k++) 
    {
      int first_non_saturating = nb_rays;

      for (i = 0; i < nb_rays; i++)
	{
	  rays[i][0] = lambda_vector_scalar_product (rays[i] + 1, cons[k] + 1,
						    dim);
	  if (rays[i][0] && i < first_non_saturating)
	    first_non_saturating = i;
	}

      if (first_non_saturating < nb_lines)
	{
	  nb_lines--;
	  lambda_vector_exchange (rays[nb_lines], rays[first_non_saturating],
				  dim + 1);

	  for (i = 0; i < nb_rays; i++)
	    {
	      if (i == nb_lines)
		{
		  if (rays[nb_lines][0] < 0)
		    lambda_vector_negate (rays[nb_lines], rays[nb_lines],
					  dim + 1);
		}
	      else if (rays[i][0] != 0)
		linear_combine (rays[i], rays[nb_lines], rays[i], 0, dim);
	    }

	  if (cons[k][0] != 0)
	    {
	      for (i = 0; i < nb_constraints; i++)
		MARK_SATURATED (sat, nb_lines, i);
	      MARK_NOT_SATURATED (sat, nb_lines, k);
	    }
	  else
	    {
	      nb_rays--;
	      lambda_vector_copy (rays[nb_rays], rays[nb_lines], dim + 1);
	      lambda_vector_copy (sat[nb_rays], sat[nb_lines], nb_constraints);
	    }
	}

      else 
	{
	  int saved_nb_rays = nb_rays;
	  int inf_bound = nb_rays;
	  int sup_bound = nb_lines;
	  int equal_bound = nb_lines;

	  /* Sort the rays matrix.  */
	  while (inf_bound > sup_bound)
	    {
	      if (rays[sup_bound][0] == 0)
		{
		  /* R0 = rays that saturate the constraint.  */
		  lambda_vector_exchange (rays[equal_bound], rays[sup_bound],
					  dim + 1);
		  lambda_vector_exchange (sat[equal_bound], sat[sup_bound],
					  nb_constraints);
		  equal_bound++;
		  sup_bound++;
		}
	      else
		{
		  MARK_NOT_SATURATED (sat, sup_bound, k);

		  if (rays[sup_bound][0] < 0)
		    {
		      /* R- = rays that don't verify the constraint.  */
		      inf_bound--;
		      if (inf_bound != sup_bound)
			{
			  lambda_vector_exchange (rays[inf_bound], 
						  rays[sup_bound],
						  dim + 1);
			  lambda_vector_exchange (sat[inf_bound], 
						  sat[sup_bound],
						  nb_constraints);
			}
		    }
		  else 
		    /* R+ = rays that verify the constraint.  */
		    sup_bound++;
		}
	    }

	  for (i = equal_bound; i < sup_bound; i++)
	    for (j = sup_bound; j < saved_nb_rays; j++)
	      {
		int common_constraints = 0;
		bool ray_only;

		for (l = 0; l <= nb_constraints; l++)
		  if (SATURATED_P (sat, i, l)
		      && SATURATED_P (sat, j, l))
		    common_constraints++;

		ray_only = (rays[i][dim] == 0 && rays[j][dim] == 0 
			    && r_to_c == 0);

		if (ray_only)
		  common_constraints++;

		if (common_constraints + nb_lines >= dim - 2)
		  {
		    int iter;

		    for (iter = nb_lines; iter < saved_nb_rays; iter++)
		      if ((iter != i) && (iter != j) 
			  && !(ray_only && rays[iter][dim]))
			{
			  for (l = 0; l < nb_constraints; l++)
			    if (NOT_SATURATED_P (sat, iter, l)
				&& SATURATED_P (sat, i, l)
				&& SATURATED_P (sat, j, l))
			      break;

			  if (l == nb_constraints)
			    /* The ray is redundant.  */
			    goto next_ray;
			}

		    if (nb_rays == max_ray)
		      {
			max_ray *= 2;
			lambda_matrix_extend (rays, nb_rays, dim + 1,
					      &rays, max_ray, dim + 1);
			lambda_matrix_extend (sat, nb_rays, nb_constraints,
					      &sat, max_ray, nb_constraints);
		      }

		    linear_combine (rays[j], rays[i], rays[nb_rays], 0, dim);

		    for (l = 0; l < nb_constraints; l++)
		      if (NOT_SATURATED_P (sat, i, l)
			  || NOT_SATURATED_P (sat, j, l))
			MARK_NOT_SATURATED (sat, nb_rays, l);
		      else
			MARK_SATURATED (sat, nb_rays, l);
		    MARK_SATURATED (sat, nb_rays, k);

		    nb_rays++;
		  }

	      next_ray:;
	      }

	  /* Eliminates all non extremal rays.  Motzkin showed that a
	     convex combination of a pair (r+ from R+, r- from R-)
	     results in an extreme ray iff the minimal face that
	     contains them:
	     - is dimension one greater than r- and r+, 
	     - and only contains the two rays r- and r+.
	  */
	  j = (cons[k][0] != 0) ? sup_bound : equal_bound;
	  i = nb_rays;

	  while ((j < saved_nb_rays) && (i > saved_nb_rays)) 
	    {
	      i--;
	      lambda_vector_copy (rays[i], rays[j], dim + 1);
	      lambda_vector_copy (sat[i], sat[j], nb_constraints);
	      j++;
	    }

	  if (j == saved_nb_rays) 
	    nb_rays = i;
	  else 
	    nb_rays = j;
	}
    }

  POLYH_RAYS (pol) = rays;
  POLYH_NB_RAYS (pol) = nb_rays;
  POLYH_SAT (pol) = sat;
  return pol;
}

/* Returns a polyhedron after having reduced the constraints system
   and the rays of polyhedron POL.  For reference, see: IRISA
   Publication Interne 785: "A library for doing polyhedral
   operations" Doran Wilde.  */

static polyhedron
reduce_system (polyhedron pol)
{
  int i, j, k;
  int dim_rayspace;
  int NbBid, NbUni, NbEq, NbIneq;
  int NbBid2, NbUni2, NbEq2, NbIneq2;
  int aux;

  lambda_vector trace;
  lambda_matrix tmp_rays, tmp_cons;

  /* Step0.  */
  for (i = 0, aux = 0; i < POLYH_NB_RAYS (pol); i++)
    {  
      POLYH_RAYS (pol)[i][0] = 0;
      if (POLYH_RAYS (pol)[i][POLYH_DIM (pol) + 1])
	aux++;              
    }
  if (aux == 0)
    return empty_polyhedron (pol);

  /* Step1.  */
  for (j = 0, NbEq = 0; j < POLYH_NB_CONS (pol); j++)
    {
      POLYH_CONS (pol)[j][0] = 0;

      for (i = 1; i < POLYH_DIM (pol) + 1; i++)
	if (POLYH_CONS (pol)[j][i])
	  break;

      if (i == POLYH_DIM (pol) + 1)
	{
	  for (i = 0; i < POLYH_NB_RAYS (pol); i++)
	    if (SATURATED_P (POLYH_SAT (pol), i, j))
	      POLYH_CONS (pol)[j][0]++;

	  if (POLYH_CONS (pol)[j][0] == POLYH_NB_RAYS (pol)
	      && POLYH_CONS (pol)[j][POLYH_DIM (pol) + 1])
	    return empty_polyhedron (pol);
	
	  POLYH_NB_CONS (pol)--;
	  if (j == POLYH_NB_CONS (pol))
	    continue;

	  lambda_vector_exchange (POLYH_CONS (pol)[j],
				  POLYH_CONS (pol)[POLYH_NB_CONS (pol)],
				  POLYH_DIM (pol) + 2);
	  lambda_matrix_exchange_columns (POLYH_SAT (pol), j,
					  POLYH_NB_CONS (pol), 
					  POLYH_NB_RAYS (pol));
	  j--;
	  continue;
	}
      
      for (i = 0; i < POLYH_NB_RAYS (pol); i++)
	if (SATURATED_P (POLYH_SAT (pol), i, j))
	  {
	    POLYH_CONS (pol)[j][0]++;
	    POLYH_RAYS (pol)[i][0]++;
	  }

      if (POLYH_CONS (pol)[j][0] == POLYH_NB_RAYS (pol))
	NbEq++;
    }

  for (i = 0, NbBid = 0; i < POLYH_NB_RAYS (pol); i++)
    {
      if (POLYH_RAYS (pol)[i][POLYH_DIM (pol) + 1] == 0)
	POLYH_RAYS (pol)[i][0]++;

      if (POLYH_RAYS (pol)[i][0] == POLYH_NB_CONS (pol) + 1)
	NbBid++;
    }

  /* Step2.  */
  for (i = 0; i < NbEq; i++)
    if (POLYH_CONS (pol)[i][0] != POLYH_NB_RAYS (pol))
      {
	lambda_vector temp1 = lambda_vector_new (POLYH_DIM (pol) + 2);

	k = i + 1;
	while (POLYH_CONS (pol)[k][0] != POLYH_NB_RAYS (pol) 
	       && k < POLYH_NB_CONS (pol))
	  k++;

	if (k == POLYH_NB_CONS (pol))
	  break;

	lambda_vector_copy (POLYH_CONS (pol)[k], temp1, POLYH_DIM (pol) + 2);
	lambda_matrix_copy_c2v (POLYH_SAT (pol), k, temp1, POLYH_NB_RAYS (pol));
	for (; k > i; k--)
	  {
	    lambda_vector_copy (POLYH_CONS (pol)[k-1],
				POLYH_CONS (pol)[k], POLYH_DIM (pol) + 2);
	    lambda_matrix_copy_columns (POLYH_SAT (pol), k-1, k,
					POLYH_NB_RAYS (pol));
	  }
	lambda_vector_copy (temp1, POLYH_CONS (pol)[i], POLYH_DIM (pol) + 2);
	lambda_matrix_copy_v2c (temp1, POLYH_SAT (pol), i, POLYH_NB_RAYS (pol));
      }

  /* Step3.  */
  NbEq2 = csys_Gauss (POLYH_CONS (pol), NbEq, POLYH_DIM (pol) + 1,
		      POLYH_NB_CONS (pol));
  if (NbEq2 >= POLYH_DIM (pol) + 1)
    return empty_polyhedron (pol);

  /* Step4.  */
  for (i = 0, k = POLYH_NB_RAYS (pol); i < NbBid && k > i; i++)
    if (POLYH_RAYS (pol)[i][0] != POLYH_NB_CONS (pol) + 1)
      {
	while (--k > i && POLYH_RAYS (pol)[k][0] != POLYH_NB_CONS (pol) + 1)
	  ;

	lambda_vector_exchange (POLYH_RAYS (pol)[i],
				POLYH_RAYS (pol)[k], POLYH_DIM (pol) + 2);
	lambda_vector_exchange (POLYH_SAT (pol)[i],
				POLYH_SAT (pol)[k], POLYH_NB_CONS (pol));
      }

  /* Step5.  */
  NbBid2 = csys_Gauss (POLYH_RAYS (pol), NbBid, POLYH_DIM (pol) + 1,
		       POLYH_NB_RAYS (pol));
  if (NbBid2 >= POLYH_DIM (pol) + 1)
    return empty_polyhedron (pol);

  /* Dimension of non-homogenous ray space.  */
  dim_rayspace = POLYH_DIM (pol) - NbEq2 - NbBid2;

  /* Step6.  */
  for (j = 0, NbIneq = 0; j < POLYH_NB_CONS (pol); j++)
    {
      for (i = 1; i < POLYH_DIM (pol) + 1; i++)
	if (POLYH_CONS (pol)[j][i] != 0)
	  break;
      
      if (i == POLYH_DIM (pol) + 1)
	{  
	  if (POLYH_CONS (pol)[j][0] == POLYH_NB_RAYS (pol) 
	      && POLYH_CONS (pol)[j][POLYH_DIM (pol) + 1] != 0)
	    return empty_polyhedron (pol);
	
	  /* Constraint is redundant.  */
	  POLYH_CONS (pol)[j][0] = 2;
	  continue;
	}

      if (POLYH_CONS (pol)[j][0] == 0
	  || POLYH_CONS (pol)[j][0] < dim_rayspace)
	/* Constraint is redundant.  */
	POLYH_CONS (pol)[j][0] = 2;

      else if (POLYH_CONS (pol)[j][0] == POLYH_NB_RAYS (pol))
	/* Constraint is an equality.  */
	POLYH_CONS (pol)[j][0] = 0;

      else
	{
	  /* Constraint is an inequality.  */
	  NbIneq++; 	
	  POLYH_CONS (pol)[j][0] = 1;
	}
    }
    
  /* Step7.  */
  for (j = 0, NbUni = 0; j < POLYH_NB_RAYS (pol); j++)
    if (POLYH_RAYS (pol)[j][0] < dim_rayspace)
      POLYH_RAYS (pol)[j][0] = 2;
    else if (POLYH_RAYS (pol)[j][0] == POLYH_NB_CONS (pol) + 1)
      POLYH_RAYS (pol)[j][0] = 0;
    else
      {
	NbUni++;
	POLYH_RAYS (pol)[j][0] = 1;
      }

  /* Step8.  */
  tmp_rays = lambda_matrix_new (NbUni + NbBid2, POLYH_DIM (pol) + 2);
  tmp_cons = lambda_matrix_new (NbIneq + NbEq2 + 1, POLYH_DIM (pol) + 2);

  if (NbBid2)
    lambda_matrix_copy (POLYH_RAYS (pol), tmp_rays, NbBid2,
			POLYH_DIM (pol) + 2);
  if (NbEq2)
    lambda_matrix_copy (POLYH_CONS (pol), tmp_cons, NbEq2,
			POLYH_DIM (pol) + 2);

  /* Step9.  */
  trace = lambda_vector_new (POLYH_NB_CONS (pol));

  for (j = NbEq, NbIneq2 = 0; j < POLYH_NB_CONS (pol); j++)
    if (POLYH_CONS (pol)[j][0] == 1)
      { 
	bool redundant;
	for (k = 0; k < POLYH_NB_CONS (pol); k++)
	  trace[k]=0;

	for (i = NbBid; i < POLYH_NB_RAYS (pol); i++) 
	  if (POLYH_RAYS (pol)[i][0] == 1)
	    if (SATURATED_P (POLYH_SAT (pol), i, j))
	      for (k = 0; k < POLYH_NB_CONS (pol); k++)
		trace[k] |= POLYH_SAT (pol)[i][k];
	
	for (i = NbEq, redundant = false; i < POLYH_NB_CONS (pol); i++)
	  if (POLYH_CONS (pol)[i][0] == 1 && i != j && !trace[i])
	    {
	      redundant = true;
	      break;
	    }

	if (redundant)
	  POLYH_CONS (pol)[j][0] = 2;
	else
	  {
	    lambda_vector_copy (POLYH_CONS (pol)[j],
				tmp_cons[NbEq2 + NbIneq2],
				POLYH_DIM (pol) + 2);
	    NbIneq2++;
	  }
      }

  /* Step10.  */
  trace = lambda_vector_new (POLYH_NB_RAYS (pol));

  for (i = NbBid, NbUni2 = 0, aux = 0; i < POLYH_NB_RAYS (pol); i++)
    if (POLYH_RAYS (pol)[i][0] == 1)
      { 
	bool redundant;
	if (POLYH_RAYS (pol)[i][POLYH_DIM (pol) + 1] != 0)
	  for (k = NbBid; k < POLYH_NB_RAYS (pol); k++)
	    trace[k]=0;
	else
	  for (k = NbBid; k < POLYH_NB_RAYS (pol); k++)
	    trace[k] = (POLYH_RAYS (pol)[k][POLYH_DIM (pol) + 1] != 0);

	for (j = NbEq; j < POLYH_NB_CONS (pol); j++)
	  if (POLYH_CONS (pol)[j][0] == 1
	      && SATURATED_P (POLYH_SAT (pol), i, j))
	    for (k = NbBid; k < POLYH_NB_RAYS (pol); k++)
	      trace[k] |= POLYH_SAT (pol)[k][j];

	for (j = NbBid, redundant = false; j < POLYH_NB_RAYS (pol); j++)
	  if (POLYH_RAYS (pol)[j][0] == 1 && i != j && !trace[j])
	    { 
	      redundant = true;
	      break;
	    }

	if (redundant)
	  POLYH_RAYS (pol)[i][0] = 2;	
	else
	  {
	    lambda_vector_copy (POLYH_RAYS (pol)[i],
				tmp_rays[NbBid2 + NbUni2],
				POLYH_DIM (pol) + 2);
	    NbUni2++;
	    if (POLYH_RAYS (pol)[i][POLYH_DIM (pol) + 1] == 0)
	      aux++;
	  }
      }

  if (aux >= dim_rayspace)
    {
      lambda_vector_clear (tmp_cons[NbEq2 + NbIneq2], POLYH_DIM (pol) + 2);
      tmp_cons[NbEq2 + NbIneq2][0] = 1;
      tmp_cons[NbEq2 + NbIneq2][POLYH_DIM (pol) + 1] = 1;
      NbIneq2++;
    }

  POLYH_CONS (pol) = tmp_cons;
  POLYH_RAYS (pol) = tmp_rays;
  POLYH_NB_RAYS (pol) = NbBid2 + NbUni2;
  POLYH_NB_CONS (pol) = NbEq2 + NbIneq2;
  return pol;
}

/* Returns a polyhedron built from the RAYS matrix with M rows by N
   columns.  */

polyhedron
rays_to_polyhedron (lambda_matrix rays ATTRIBUTE_UNUSED,
		    int m ATTRIBUTE_UNUSED,
		    int n ATTRIBUTE_UNUSED)
{
  polyhedron res = NULL;

  

  return res;
}

/* Returns a polyhedron built from the CONSTRAINT matrix with
   NB_CONSTRAINTS rows by DIMENSION + 2 columns.  */

polyhedron
cons_to_polyhedron (lambda_matrix constraints, int nb_constraints,
		    int dimension, int nb_eqs)
{
  csys res = ggc_alloc_cleared (sizeof (struct csys));

  CSYS_DIMENSION (res) = dimension;
  CSYS_NB_EQS (res) = nb_eqs;
  CSYS_NB_CONSTRAINTS (res) = nb_constraints;
  CSYS_M (res) = constraints;

  return polyhedron_from_csys (res);
}

/* Returns a polyhedron built from the constraint system CY.  */

polyhedron
polyhedron_from_csys (csys cy)
{
  int dimension = CSYS_DIMENSION (cy);
  int nb_lines = dimension;
  int nb_rays = dimension + 1;
  gsys gy = gsys_new (dimension, nb_lines, nb_rays);
  lambda_matrix sat = lambda_matrix_new (nb_rays, CSYS_NB_CONSTRAINTS (cy));
  polyhedron pol = polyhedron_new (cy, gy, sat);

  pol = csys_gsys_compute_dual (pol, false);
  pol = reduce_system (pol);

  return pol;
}


/* Transform a polyhedron POL into another polyhedron according to a
   given affine mapping function FUN.  FUN is a FUN_ROWS by FUN_COLS
   matrix.  */

polyhedron
polyhedron_image (polyhedron pol, lambda_matrix fun,
		  int fun_rows, int fun_cols)
{
  int i, j, k;
  lambda_matrix rays;

  if (POLYH_DIM (pol) + 1 != fun_cols)
    {
      POLYH_DIM (pol) = fun_rows;
      return empty_polyhedron (pol);
    }

  rays = lambda_matrix_new (POLYH_NB_RAYS (pol), fun_rows + 1);

  for (i = 0; i < POLYH_NB_RAYS (pol); i++)
    {
      rays[i][0] = POLYH_RAYS (pol)[i][0];
      for (j = 0; j < fun_rows; j++)
	{
	  int sum = 0;
	  for (k = 0; k < POLYH_DIM (pol) + 1; k++)
	    sum += POLYH_RAYS (pol)[i][k+1] * fun[j][k];
	  rays[i][j+1] = sum;
	}
    }

  return rays_to_polyhedron (rays, POLYH_NB_RAYS (pol), fun_rows + 1);
}

/* Given a polyhedron POL and a transformation matrix FUN, return the
   polyhedron which when transformed by mapping function FUN gives
   POL.  FUN is a FUN_ROWS by FUN_COLS matrix.  */

polyhedron
polyhedron_preimage (polyhedron pol, lambda_matrix fun,
		     int fun_rows, int fun_cols)
{
  int i, j, k;
  lambda_matrix constraints;
  
  if (POLYH_DIM (pol) + 1 != fun_rows)
    {
      POLYH_DIM (pol) = fun_cols;
      return empty_polyhedron (pol);
    }

  constraints = lambda_matrix_new (POLYH_NB_CONS (pol), fun_cols + 1);

  for (i = 0; i < POLYH_NB_CONS (pol); i++)
    {
      constraints[i][0] = POLYH_CONS (pol)[i][0];
      for (j = 0; j < fun_cols; j++)
	{
	  int sum = 0;
	  for (k = 0; k < POLYH_DIM (pol) + 1; k++)
	    sum += POLYH_CONS (pol)[i][k+1] * fun[k][j];
	  constraints[i][j+1] = sum;
	}
    }

  /* return cons_to_polyhedron (constraints); */
  return NULL;
}
