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


#ifdef SMALL_OMEGA_PROB
#define max_vars 38
#define max_geqs 150
#define max_eqs 27
#else
#define max_vars 128
#define max_geqs 256
#define max_eqs 128
#endif

typedef struct eqn
{
  int key;
  int touched;
  int color;
  int coef[max_vars + 1];
} *eqn;

#define headerWords 3

typedef struct _problem
{
  int num_vars, num_eqs, num_geqs, safe_vars, num_subs;
  int hashVersion;
  int variablesInitialized;
  int variablesFreed;
  int var[max_vars + 2];
  int forwarding_address[max_vars + 2];
  struct eqn geqs[max_geqs];
  struct eqn eqs[max_eqs];
  struct eqn subs[max_vars + 1];
}
omega_problem;

extern char *(*current_getVarName) (unsigned int);	/* getVarName function */

#define getVarName (*current_getVarName)

#define UNKNOWN 2
#define SIMPLIFY 3
#define pos_infinity (0x7ffffff)
#define neg_infinity (-0x7ffffff)
#define red 1
#define black 0

extern void omega_initialize (void);
extern void omega_initialize_problem (omega_problem *);
extern void omega_initialize_variables (omega_problem *);
extern int omega_solve_problem (omega_problem *, int);

extern void omega_copy_problem (omega_problem *, omega_problem *);
extern void omega_print_problem (FILE *, omega_problem *);
extern void omega_print_red_equations (FILE *, omega_problem *);
extern int omega_count_red_equations (omega_problem *);
extern void omega_pretty_print_problem (FILE *, omega_problem *);
extern int omega_simplify_problem (omega_problem *);
extern int omega_simplify_approximate (omega_problem *);
extern void omega_unprotect_variable (omega_problem *, int var);
extern void omega_negate_geq (omega_problem *, int);
extern void omega_convert_eq_to_geqs (omega_problem *, int eq);
extern void omega_print_eqn (FILE *, omega_problem *, eqn, int, int);
extern void omega_sprint_eqn (char *, omega_problem *, eqn, int, int);
extern int omega_check_if_single_var (eqn, int);
extern bool omega_problem_has_red_equations (omega_problem *);
extern int omega_eliminate_redundant (omega_problem *, bool);
extern void omega_eliminate_red (omega_problem *, bool);
extern int omega_constrain_variable_sign (omega_problem *, int, int, int);
extern void omega_constrain_variable_value (omega_problem *, int, int, int);
extern int omega_query_variable (omega_problem *, int, int *, int *);
extern int omega_query_variable_signs (omega_problem *, int, int, int, int,
				       int, int, bool *, int *);
extern int omega_query_variable_bounds (omega_problem *, int, int *, int *);

extern int reduceWithSubs;
extern int verifySimplification;

extern void (*whenReduced) (omega_problem *);

extern void noProcedure (omega_problem *);

#endif
