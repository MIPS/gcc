/* Structure of polyhedra.
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

#ifndef GCC_POLYHEDRON_H
#define GCC_POLYHEDRON_H

#include "lambda.h"
#include "omega.h"

/* A constraint system is composed of a system of linear equalities
   and inequalities.  Example: the following constraints system
   
   0  = x + 0y + 5z + 9
   0 <= x + 2y + 3z - 8

   is encoded by the matrix: 

   |IS_INEQ  VECTORS  CST
   |   0      1 0 5    9
   |   1      1 2 3   -8
*/

typedef struct csys
{
  /* NB_CONSTRAINTS = NB_EQS + nb_ineqs.  */
  int dimension, nb_eqs, nb_constraints;

  /* CONSTRAINTS is NB_CONSTRAINTS lines by DIMENSION + 2 columns.  */
  lambda_matrix constraints;

} *csys;

#define CSYS_DIMENSION(C) ((C)->dimension)
#define CSYS_NB_EQS(C) ((C)->nb_eqs)
#define CSYS_NB_CONSTRAINTS(C) ((C)->nb_constraints)

/* The raw matrix.  */
#define CSYS_M(C) ((C)->constraints)
/* An element.  */
#define CSYS_ELT(C, I, J) (CSYS_M (C)[I][J])
/* The constant.  */
#define CSYS_CST(C, I) (CSYS_ELT (C, I, CSYS_DIMENSION (C) + 1))
/* The vector.  */
#define CSYS_VECTOR(C, I) (CSYS_M (C)[I] + 1)
/* An element of the vector.  */
#define CSYS_VEC(C, I, J) (CSYS_VECTOR (C, I)[J])
/* Test for an equality constraint.  */
#define CSYS_IS_EQ(C, I) ((CSYS_ELT (C, I, 0)) == 0)
/* Test for an inequality constraint.  */
#define CSYS_IS_INEQ(C, I) ((CSYS_ELT (C, I, 0)) != 0)

/* A generating system is composed of a set of rays, and lines.  */

typedef struct gsys
{
  int dimension, nb_lines, nb_rays;

  /* RAYS is NB_RAYS lines by DIMENSION + 2 columns.  */
  lambda_matrix rays;

} *gsys;

#define GSYS_DIMENSION(P) ((P)->dimension)
#define GSYS_NB_LINES(P) ((P)->nb_lines)
#define GSYS_NB_RAYS(P) ((P)->nb_rays)
#define GSYS_M(P) ((P)->rays)

/* A polyhedron is described by two dual representations:
   - a set of linear constraints, aka. constraint system,
   - a set of rays and lines, aka. generating system.
*/

typedef struct polyhedron
{
  csys cy;
  gsys gy;
  lambda_matrix saturation;
} *polyhedron;

#define POLYH_CSYS(P) (P->cy)
#define POLYH_GSYS(P) (P->gy)
#define POLYH_SAT(P) (P->saturation)
#define POLYH_CONS(P) CSYS_M (POLYH_CSYS (P))
#define POLYH_RAYS(P) GSYS_M (POLYH_GSYS (P))
#define POLYH_NB_RAYS(P) GSYS_NB_RAYS (POLYH_GSYS (P))
#define POLYH_NB_LINES(P) GSYS_NB_LINES (POLYH_GSYS (P))
#define POLYH_NB_CONS(P) CSYS_NB_CONSTRAINTS (POLYH_CSYS (P))
#define POLYH_DIM(P) CSYS_DIMENSION (POLYH_CSYS (P))

extern csys csys_new (int, int, int);
extern csys csys_copy (csys);
extern void csys_print (FILE *, csys);
extern void debug_csys (csys);
extern omega_pb csys_to_omega (csys);

extern gsys gsys_new (int, int, int);
extern gsys gsys_copy (gsys);
extern void gsys_print (FILE *, gsys);
extern void debug_gsys (gsys);

extern polyhedron polyhedron_new (csys, gsys, lambda_matrix);
extern polyhedron polyhedron_from_csys (csys);
extern polyhedron polyhedron_from_gsys (gsys);
extern polyhedron polyhedron_copy (polyhedron);
extern void polyhedron_print (FILE *, polyhedron);
extern void debug_polyhedron (polyhedron);

extern polyhedron polyhedron_image (polyhedron, lambda_matrix, int, int);
extern polyhedron polyhedron_preimage (polyhedron, lambda_matrix, int, int);
extern polyhedron rays_to_polyhedron (lambda_matrix, int, int);
extern polyhedron cons_to_polyhedron (lambda_matrix, int, int, int);

#endif
