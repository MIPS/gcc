/* Source code for an implementation of the Omega test, a integer 
   programming algorithm for dependence analysis, by William Pugh, 
   appeared in Supercomputing '91 and CACM Aug 92.

   This code has no license restrictions, and is considered public
   domain.

   Changes copyright (C) 2005 Free Software Foundation, Inc.

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

#include <config.h>

#ifndef Already_Included_IP
#define Already_Included_IP 1

#define max_vars 128
#define max_geqs 256
#define max_eqs 128

#define pos_infinity (0x7ffffff)
#define neg_infinity (-0x7ffffff)

/* Results of the Omega solver.  */
enum omega_result {
  omega_false = 0,
  omega_true = 1,
  omega_unknown = 2,
  omega_simplify = 3
};

/* Used for labeling equations.  */
enum omega_eqn_color {
  omega_black = 0,
  omega_red = 1
};

typedef struct eqn
{
  int key;
  int touched;
  enum omega_eqn_color color;
  int coef[max_vars + 1];
} *eqn;

typedef struct omega_pb
{
  int num_vars, num_eqs, num_geqs, safe_vars, num_subs;
  int hash_version;
  int variablesInitialized;
  int variablesFreed;
  int var[max_vars + 2];
  int forwarding_address[max_vars + 2];
  struct eqn geqs[max_geqs];
  struct eqn eqs[max_eqs];
  struct eqn subs[max_vars + 1];
} *omega_pb;

extern bool omega_reduce_with_subs;
extern bool omega_verify_simplification;

extern void omega_initialize (void);
extern void omega_initialize_problem (omega_pb);
extern void omega_initialize_variables (omega_pb);
extern enum omega_result omega_solve_problem (omega_pb, enum omega_result);

extern void omega_copy_problem (omega_pb, omega_pb);
extern void omega_print_problem (FILE *, omega_pb);
extern void omega_print_red_equations (FILE *, omega_pb);
extern int omega_count_red_equations (omega_pb);
extern void omega_pretty_print_problem (FILE *, omega_pb);
extern bool omega_simplify_problem (omega_pb);
extern int omega_simplify_approximate (omega_pb);
extern void omega_unprotect_variable (omega_pb, int var);
extern void omega_negate_geq (omega_pb, int);
extern void omega_convert_eq_to_geqs (omega_pb, int eq);
extern void omega_print_eqn (FILE *, omega_pb, eqn, int, int);
extern void omega_sprint_eqn (char *, omega_pb, eqn, int, int);
extern bool omega_problem_has_red_equations (omega_pb);
extern int omega_eliminate_redundant (omega_pb, bool);
extern void omega_eliminate_red (omega_pb, bool);
extern int omega_constrain_variable_sign (omega_pb, int, int, int);
extern void omega_constrain_variable_value (omega_pb, int, int, int);
extern int omega_query_variable (omega_pb, int, int *, int *);
extern int omega_query_variable_signs (omega_pb, int, int, int, int,
				       int, int, bool *, int *);
extern int omega_query_variable_bounds (omega_pb, int, int *, int *);

extern int reduceWithSubs;
extern int verifySimplification;

extern void (*omega_when_reduced) (omega_pb);

extern void omega_no_procedure (omega_pb);

/* Print to FILE equality E from PB.  */

static inline void
omega_print_eq (FILE *file, omega_pb pb, eqn e)
{
  omega_print_eqn (file, pb, e, 0, 0);
}

/* Print to FILE inequality E from PB.  */

static inline void
omega_print_geq (FILE *file, omega_pb pb, eqn e)
{
  omega_print_eqn (file, pb, e, 1, 0);
}

/* Print to FILE inequality E from PB.  */

static inline void
omega_print_geq_extra (FILE *file, omega_pb pb, eqn e)
{
  omega_print_eqn (file, pb, e, 1, 1);
}

/* FIXME: What is this used for?  */
#define headerWords 3

/* Copy equations: E1 = E2.  Equations contain S variables.  */

static inline void
omega_copy_eqn (eqn e1, eqn e2, int s)
{
  int *p00, *q00, *r00;
  p00 = (int *) e1;
  q00 = (int *) e2;
  r00 = &p00[headerWords + 1 + s];
  while (p00 < r00)
    *p00++ = *q00++;
}

/* Intialize E = 0.  Equation E contains S variables.  */

static inline void
omega_init_eqn_zero (eqn e, int s)
{
  int *p00, *r00;
  p00 = (int *) e;
  r00 = &p00[headerWords + 1 + s];
  while (p00 < r00)
    *p00++ = 0;
}

/* Returns true when E is an inequality that contains a single
   variable.  */

static inline bool
single_var_geq (struct eqn e, int nv ATTRIBUTE_UNUSED)
{
  return (e.key != 0 && -max_vars <= e.key && e.key <= max_vars);
}

#define cant_do_omega abort

/* Initialize P as an Omega problem with NVARS variables and
   NPROT safe variables.  */

static inline void
init_problem (omega_pb p, unsigned nvars, unsigned nprot)
{
  omega_initialize_problem (p);

  if (nvars > max_vars)
    cant_do_omega ();

  p->num_vars = nvars;
  p->safe_vars = nprot;
  p->num_eqs = 0;
  p->num_geqs = 0;
}

/* Allocate a new equality with all coefficients 0.  */

static inline int
prob_add_zero_eq (omega_pb p, int color)
{
  int c = p->num_eqs;

  if (++p->num_eqs > max_eqs)
    cant_do_omega ();

  omega_init_eqn_zero (&p->eqs[c], p->num_vars);
  p->eqs[c].color = color;

  return c;
}

/* Allocate a new inequality with all coefficients 0.  */

static inline int
prob_add_zero_geq (omega_pb p, int color)
{
  int c = p->num_geqs;

  if (++p->num_geqs > max_geqs)
    cant_do_omega ();

  omega_init_eqn_zero (&p->geqs[c], p->num_vars);
  p->geqs[c].touched = 1;
  p->geqs[c].color = color;

  return c;
}

#endif
