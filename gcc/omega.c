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

/* Options SPEED - don't bother checking debugging flags
   eliminateRedundantConstraints - use expensive methods to eliminate
   all redundant constraints singleResult - only produce a single
   simplified result APROX - approximate inexact reductions
   verifySimplification (runtime) - if TRUE, omega_simplify_problem
   checks for problem with no solutions reduceWithSubs (runtime) - if
   FALSE, convert substitutions back to EQs
 */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "errors.h"
#include "ggc.h"
#include "tree.h"
#include "diagnostic.h"
#include "varray.h"
#include "tree-pass.h"


#define maxWildCards 18
#ifndef APROX
#define APROX 0
#endif

static int DD_DEBUG_OMEGA=99;
#include "omega.h"

#ifdef NDEBUG
#define check_pos_mul(x,y) ((x)*(y))
#define check_mul(x,y) ((x)*(y))
#else

static inline int
check_pos_mul (int x, int y)
{
  if (x != 0)
    gcc_assert ((INT_MAX) / x > y);
  return x * y;
}

static inline int
check_mul (int x, int y)
{
  if (x >= 0)
    {
      if (y >= 0)
	return check_pos_mul (x, y);
      else
	return -check_pos_mul (x, -y);
    }
  else if (y >= 0)
    return -check_pos_mul (-x, y);
  else
    return check_pos_mul (-x, -y);
}
#endif

#define EQ_type 1
#define GEQ_type 2
#define keyMult 31
#define indexMult 17
#define abs(x) (x >= 0? (x) : -(x))
#define max(x,y) (x>y?x:y)
#define min(x,y) (x<y?x:y)

static inline void
setMax (int *m, int x)
{
  if (*m < x) 
    *m = x;
}

static inline void
setMin (int *m, int x)
{
  if (*m > x)
    *m = x;
}

#define omega_print_eq(file, problem_ptr, e) omega_print_eqn (file, problem_ptr, e, 0, 0)
#define omega_print_geq(file, problem_ptr, e) omega_print_eqn (file, problem_ptr, e, 1, 0)
#define omega_print_geq_extra(file, problem_ptr, e) omega_print_eqn (file, problem_ptr, e, 1, 1)
#define variable(i) orgVariable(problem_ptr->var[i])
#define orgVariable(i) (i == 0 ? "1" : (i < 0 ? wild_name[-i] : (*current_getVarName)(i)))

#define eqncpy(e1, e2) eqnncpy (e1, e2, problem_ptr->num_vars)
#define eqnzero(e) eqnnzero(e, problem_ptr->num_vars)
#define int_div(a,b) ((a) > 0?(a)/(b):-((-(a)+(b)-1)/(b)))
#define int_mod(a,b) ((a)-(b)*int_div(a,b))

static inline void
eqnncpy (eqn e1, eqn e2, int s)
{
  int *p00, *q00, *r00;
  p00 = (int *) e1;
  q00 = (int *) e2;
  r00 = &p00[headerWords + 1 + s];
  while (p00 < r00)
    *p00++ = *q00++;
}

static inline void
eqnnzero (eqn e, int s)
{
  int *p00, *r00;
  p00 = (int *) e;
  r00 = &p00[headerWords + 1 + s];
  while (p00 < r00)
    *p00++ = 0;
}

static inline bool
single_var_geq (struct eqn e, int nV ATTRIBUTE_UNUSED)
{
  return (e.key != 0 && -max_vars <= e.key && e.key <= max_vars);
}

static inline void
do_delete (omega_problem *problem_ptr, int e, int nV)
{
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Deleting %d (last:%d): ", e, problem_ptr->num_geqs-1);
      omega_print_geq (dump_file, problem_ptr, &problem_ptr->geqs[e]);
      fprintf (dump_file, "\n");
    }

  if (e < problem_ptr->num_geqs-1)
    eqnncpy (&problem_ptr->geqs[e], &problem_ptr->geqs[problem_ptr->num_geqs - 1], nV);
  problem_ptr->num_geqs--;
}


static inline void
do_delete_extra (omega_problem *problem_ptr, int e, int nV)
{
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Deleting %d: ",e);
      omega_print_geq_extra (dump_file, problem_ptr, &problem_ptr->geqs[e]);
      fprintf (dump_file, "\n");
    }

  if (e < problem_ptr->num_geqs-1)
    eqnncpy (&problem_ptr->geqs[e], &problem_ptr->geqs[problem_ptr->num_geqs - 1],(nV));
  problem_ptr->num_geqs--;
}

#define isRed(e) (desired_result == SIMPLIFY && (e)->color)

#ifdef singleResult
#define returnSingleResult  1
#else
static int returnSingleResult = 0;
#endif

static int mayBeRed = 0;
#define hash_table_size 550
#define maxKeys 500

#undef TRACE
#undef DBUG
#undef DEBUG

#ifdef SPEED
#define TRACE 0
#define DBUG 0
#define DEBUG 0
#else
#define TRACE (DD_DEBUG_OMEGA)
#define DBUG  (DD_DEBUG_OMEGA > 1)
#define DEBUG (DD_DEBUG_OMEGA > 2)
#endif

static struct eqn hash_master[hash_table_size];

int nonConvex = 0;

static int doItAgain;

static int conservative = 0;

static int next_key;
static char wild_name[200][20];
static int next_wild_card = 0;
static int trace = 1;
static int depth = 0;
static int foundReduction;
static int packing[max_vars];

static int inApproximateMode = 0;

int reduceWithSubs = 1;
int verifySimplification = 0;

static int createColor = 0;
static int please_no_equalities_in_simplified_problems = 0;

int hashVersion = 0;

#define no_problem ((omega_problem *) 0)

omega_problem *original_problem = no_problem;

void
noProcedure (omega_problem *problem_ptr ATTRIBUTE_UNUSED)
{
}

void (*whenReduced) (omega_problem *) = noProcedure;

static inline int
gcd (int b, int a)
{
  if (b == 1)
    return 1;
  while (b != 0)
    {
      int t = b;
      b = a % b;
      a = t;
    }
  return a;
}

int
omega_check_if_single_var (eqn e, int i)
{
  for (; i > 0; i--)
    if (e->coef[i])
      {
	i--;
	break;
      }
  for (; i > 0; i--)
    if (e->coef[i])
      break;
  return (i == 0);
}

void
omega_initialize_variables (omega_problem * p)
{
  int i;

  for (i = p->num_vars; i >= 0; i--)
    p->forwarding_address[i] = p->var[i] = i;

  p->variablesInitialized = 1;
}

void
omega_initialize_problem (omega_problem * p)
{
  p->num_vars = 0;
  p->hashVersion = hashVersion;
  p->variablesInitialized = 0;
  p->variablesFreed = 0;
  p->safe_vars = 0;
  p->num_eqs = 0;
  p->num_geqs = 0;
  p->num_subs = 0;
}

void
omega_copy_problem (omega_problem * p1, omega_problem * p2)
{
  int e, i;
  p1->num_vars = p2->num_vars;
  p1->hashVersion = p2->hashVersion;
  p1->variablesInitialized = p2->variablesInitialized;
  p1->variablesFreed = p2->variablesFreed;
  p1->safe_vars = p2->safe_vars;
  p1->num_eqs = p2->num_eqs;
  p1->num_subs = p2->num_subs;
  for (e = p2->num_eqs - 1; e >= 0; e--)
    eqnncpy (&(p1->eqs[e]), &(p2->eqs[e]), p2->num_vars);
  p1->num_geqs = p2->num_geqs;
  for (e = p2->num_geqs - 1; e >= 0; e--)
    eqnncpy (&(p1->geqs[e]), &(p2->geqs[e]), p2->num_vars);
  for (e = p2->num_subs - 1; e >= 0; e--)
    eqnncpy (&(p1->subs[e]), &(p2->subs[e]), p2->num_vars);
  for (i = 0; i <= p2->num_vars; i++)
    p1->var[i] = p2->var[i];
  for (i = 0; i <= max_vars; i++)
    p1->forwarding_address[i] = p2->forwarding_address[i];


}



static void
printTerm (FILE *file, omega_problem *problem_ptr, eqn e, int c)
{
  int i;
  int first;
  int n = problem_ptr->num_vars;
  int wentFirst = -1;
  first = 1;
  for (i = 1; i <= n; i++)
    if (c * e->coef[i] > 0)
      {

	first = 0;
	wentFirst = i;

	if (c * e->coef[i] == 1)
	  fprintf (file, "%s", variable (i));
	else
	  fprintf (file, "%d * %s", c * e->coef[i], variable (i));
	break;
      }
  for (i = 1; i <= n; i++)
    if (i != wentFirst && c * e->coef[i] != 0)
      {
	if (!first && c * e->coef[i] > 0)
	  fprintf (file, " + ");

	first = 0;

	if (c * e->coef[i] == 1)
	  fprintf (file, "%s", variable (i));
	else if (c * e->coef[i] == -1)
	  fprintf (file, " - %s", variable (i));
	else
	  fprintf (file, "%d * %s", c * e->coef[i], variable (i));
      }
  if (!first && c * e->coef[0] > 0)
    fprintf (file, " + ");
  if (first || c * e->coef[0] != 0)
    fprintf (file, "%d", c * e->coef[0]);
}


void
omega_print_eqn (FILE *file, omega_problem *problem_ptr, eqn e, int test, int extra)
{
  char buf[max_vars * 12 + 80];

  omega_sprint_eqn (buf, problem_ptr, e, test, extra);
  fprintf (file, "%s", buf);
}

void
omega_sprint_eqn (char *str, omega_problem *problem_ptr, eqn e, int test, int extra)
{
  int i;
  int first;
  int n = problem_ptr->num_vars + extra;
  int isLT;

  isLT = test && e->coef[0] == -1;
  if (isLT)
    isLT = 1;
  if (test)
    {
      if (DEBUG && e->touched)
	{
	  sprintf (str, "!");
	  while (*str)
	    str++;
	}
      else if (DBUG && !e->touched && e->key != 0)
	{
	  sprintf (str, "%d: ", e->key);
	  while (*str)
	    str++;
	}
    }
  if (e->color)
    {
      sprintf (str, "[");
      while (*str)
	str++;
    }
  first = 1;
  for (i = isLT; i <= n; i++)
    if (e->coef[i] < 0)
      {
	if (!first)
	  {
	    sprintf (str, " + ");
	    while (*str)
	      str++;
	  }
	else
	  first = 0;
	if (i == 0)
	  {
	    sprintf (str, "%d", -e->coef[i]);
	    while (*str)
	      str++;
	  }
	else if (e->coef[i] == -1)
	  {
	    sprintf (str, "%s", variable (i));
	    while (*str)
	      str++;
	  }
	else
	  {
	    sprintf (str, "%d * %s", -e->coef[i], variable (i));
	    while (*str)
	      str++;
	  }
      }
  if (first)
    {
      if (isLT)
	{
	  sprintf (str, "1");
	  isLT = 0;
	}
      else
	sprintf (str, "0");
      while (*str)
	str++;
    }
  if (test == 0)
    {
      sprintf (str, " = ");
      while (*str)
	str++;
    }
  else
    {
      if (isLT)
	sprintf (str, " < ");
      else
	sprintf (str, " <= ");
      while (*str)
	str++;
    }

  first = 1;
  for (i = 0; i <= n; i++)
    if (e->coef[i] > 0)
      {
	if (!first)
	  {
	    sprintf (str, " + ");
	    while (*str)
	      str++;
	  }
	else
	  first = 0;
	if (i == 0)
	  {
	    sprintf (str, "%d", e->coef[i]);
	    while (*str)
	      str++;
	  }
	else if (e->coef[i] == 1)
	  {
	    sprintf (str, "%s", variable (i));
	    while (*str)
	      str++;
	  }
	else
	  {
	    sprintf (str, "%d * %s", e->coef[i], variable (i));
	    while (*str)
	      str++;
	  }
      }
  if (first)
    {
      sprintf (str, "0");
      while (*str)
	str++;
    }
  if (e->color)
    {
      sprintf (str, "]");
      while (*str)
	str++;
    }
}

static void
printVars (FILE *file, omega_problem *problem_ptr)
{
  int i;
  fprintf (file, "variables = ");
  if (problem_ptr->safe_vars > 0)
    fprintf (file, "(");
  for (i = 1; i <= problem_ptr->num_vars; i++)
    {
      fprintf (file, "%s", variable (i));
      if (i == problem_ptr->safe_vars)
	fprintf (file, ")");
      if (i < problem_ptr->num_vars)
	fprintf (file, ", ");
    }
  fprintf (file, "\n");
}

void
omega_print_problem (FILE *file, omega_problem *problem_ptr)
{
  int e;

  if (!problem_ptr->variablesInitialized)
    omega_initialize_variables (problem_ptr);

  printVars (file, problem_ptr);
  for (e = 0; e < problem_ptr->num_eqs; e++)
    {
      omega_print_eq (file, problem_ptr, &problem_ptr->eqs[e]);
      fprintf (file, "\n");
    }
  fprintf (file, "Done with EQ\n");
  for (e = 0; e < problem_ptr->num_geqs; e++)
    {
      omega_print_geq (file, problem_ptr, &problem_ptr->geqs[e]);
      fprintf (file, "\n");
    }
  fprintf (file, "Done with GEQ\n");
  for (e = 0; e < problem_ptr->num_subs; e++)
    {
      eqn eq = &problem_ptr->subs[e];
      if (DBUG && eq->color)
	fprintf (file, "[");
      if (eq->key > 0)
	fprintf (file, "%s := ", orgVariable (eq->key));
      else
	fprintf (file, "#%d := ", eq->key);
      printTerm (file, problem_ptr, eq, 1);
      if (DBUG && eq->color)
	fprintf (file, "]");
      fprintf (file, "\n");
    }
}


int
omega_count_red_equations (omega_problem *problem_ptr)
{
  int e, i;
  int result = 0;
  for (e = 0; e < problem_ptr->num_eqs; e++)
    if (problem_ptr->eqs[e].color == red)
      {
	for (i = problem_ptr->num_vars; i > 0; i--)
	  if (problem_ptr->geqs[e].coef[i])
	    break;
	if (i == 0 && problem_ptr->geqs[e].coef[0] == 1)
	  return 0;
	else
	  result += 2;
      }
  for (e = 0; e < problem_ptr->num_geqs; e++)
    if (problem_ptr->geqs[e].color == red)
      result += 1;
  for (e = 0; e < problem_ptr->num_subs; e++)
    if (problem_ptr->subs[e].color == red)
      result += 2;
  return result;
}


void
omega_print_red_equations (FILE *file, omega_problem *problem_ptr)
{
  int e;

  if (!problem_ptr->variablesInitialized)
    omega_initialize_variables (problem_ptr);

  printVars (file, problem_ptr);
  for (e = 0; e < problem_ptr->num_eqs; e++)
    {
      if (problem_ptr->eqs[e].color == red)
	{
	  omega_print_eq (file, problem_ptr, &problem_ptr->eqs[e]);
	  fprintf (file, "\n");
	}
    }
  for (e = 0; e < problem_ptr->num_geqs; e++)
    {
      if (problem_ptr->geqs[e].color == red)
	{
	  omega_print_geq (file, problem_ptr, &problem_ptr->geqs[e]);
	  fprintf (file, "\n");
	}
    }
  for (e = 0; e < problem_ptr->num_subs; e++)
    {
      if (problem_ptr->subs[e].color == red)
	{
	  eqn eq = &problem_ptr->subs[e];
	  if (DBUG && eq->color)
	    fprintf (file, "[");
	  if (eq->key > 0)
	    fprintf (file, "%s := ", orgVariable (eq->key));
	  else
	    fprintf (file, "#%d := ", eq->key);
	  printTerm (file, problem_ptr, eq, 1);
	  if (DBUG && eq->color)
	    fprintf (file, "]");
	  fprintf (file, "\n");
	}
    }
}


void
omega_pretty_print_problem (FILE *file, omega_problem *problem_ptr)
{
  int e;
  int v;
  int live[max_geqs];
  int v1, v2, v3;
  int t, change;
  int stuffPrinted = 0;


  typedef enum
  {
    none, le, lt
  }
  partialOrderType;

  partialOrderType po[max_vars][max_vars];
  int poE[max_vars][max_vars];
  int lastLinks[max_vars];
  int firstLinks[max_vars];
  int chain_length[max_vars];
  int chain[max_vars];
  int i, m, multiprint;


  if (!problem_ptr->variablesInitialized)
    omega_initialize_variables (problem_ptr);

  if (problem_ptr->num_vars > 0)
    {
      omega_eliminate_redundant (problem_ptr, 0);

      for (e = 0; e < problem_ptr->num_eqs; e++)
	{
	  if (stuffPrinted)
	    fprintf (file, "; ");
	  stuffPrinted = 1;
	  omega_print_eq (file, problem_ptr, &problem_ptr->eqs[e]);
	}

      for (e = 0; e < problem_ptr->num_geqs; e++)
	live[e] = TRUE;

      while (1)
	{
	  for (v = 1; v <= problem_ptr->num_vars; v++)
	    {
	      lastLinks[v] = firstLinks[v] = 0;
	      chain_length[v] = 0;
	      for (v2 = 1; v2 <= problem_ptr->num_vars; v2++)
		po[v][v2] = none;
	    }

	  for (e = 0; e < problem_ptr->num_geqs; e++)
	    if (live[e])
	      {
		for (v = 1; v <= problem_ptr->num_vars; v++)
		  {
		    if (problem_ptr->geqs[e].coef[v] == 1)
		      firstLinks[v]++;
		    else if (problem_ptr->geqs[e].coef[v] == -1)
		      lastLinks[v]++;
		  }

		v1 = problem_ptr->num_vars;
		while (v1 > 0 && problem_ptr->geqs[e].coef[v1] == 0)
		  v1--;
		v2 = v1 - 1;
		while (v2 > 0 && problem_ptr->geqs[e].coef[v2] == 0)
		  v2--;
		v3 = v2 - 1;
		while (v3 > 0 && problem_ptr->geqs[e].coef[v3] == 0)
		  v3--;

		if (problem_ptr->geqs[e].coef[0] > 0 || problem_ptr->geqs[e].coef[0] < -1
		    || v2 <= 0 || v3 > 0
		    || problem_ptr->geqs[e].coef[v1] * problem_ptr->geqs[e].coef[v2] != -1)
		  {
		    /* Not a partial order relation */

		  }
		else
		  {
		    if (problem_ptr->geqs[e].coef[v1] == 1)
		      {
			v3 = v2;
			v2 = v1;
			v1 = v3;
		      }
		    /* relation is v1 <= v2 or v1 < v2 */
		    po[v1][v2] = ((problem_ptr->geqs[e].coef[0] == 0) ? le : lt);
		    poE[v1][v2] = e;
		  }
	      }
	  for (v = 1; v <= problem_ptr->num_vars; v++)
	    chain_length[v] = lastLinks[v];


	  /*
	   * printf("\n\nPartial order:\n"); printf("         "); for (v1 = 1; v1 <= problem_ptr->num_vars; v1++)
	   * printf("%7s",variable(v1)); printf("\n"); for (v1 = 1; v1 <= problem_ptr->num_vars; v1++) { printf("%6s:
	   * ",variable(v1)); for (v2 = 1; v2 <= problem_ptr->num_vars; v2++) switch (po[v1][v2]) { case none: printf("       ");
	   * break; case le:   printf("    <= "); break; case lt:   printf("    <  "); break; } printf("\n"); }
	   */



	  /* Just in case problem_ptr->num_vars <= 0 */
	  change = FALSE;
	  for (t = 0; t < problem_ptr->num_vars; t++)
	    {
	      change = FALSE;
	      for (v1 = 1; v1 <= problem_ptr->num_vars; v1++)
		for (v2 = 1; v2 <= problem_ptr->num_vars; v2++)
		  if (po[v1][v2] != none &&
		      chain_length[v1] <= chain_length[v2])
		    {
		      chain_length[v1] = chain_length[v2] + 1;
		      change = TRUE;
		    }
	    }

	  if (change)
	    {
	      /* caught in cycle */
	      gcc_assert (0);
	    }

	  for (v1 = 1; v1 <= problem_ptr->num_vars; v1++)
	    if (chain_length[v1] == 0)
	      firstLinks[v1] = 0;

	  v = 1;
	  for (v1 = 2; v1 <= problem_ptr->num_vars; v1++)
	    if (chain_length[v1] + firstLinks[v1] >
		chain_length[v] + firstLinks[v])
	      v = v1;

	  if (chain_length[v] + firstLinks[v] == 0)
	    break;

	  if (stuffPrinted)
	    fprintf (file, "; ");
	  stuffPrinted = 1;
	  /* chain starts at v */
	  /* print firstLinks */
	  {
	    int tmp, first;
	    first = 1;
	    for (e = 0; e < problem_ptr->num_geqs; e++)
	      if (live[e] && problem_ptr->geqs[e].coef[v] == 1)
		{
		  if (!first)
		    fprintf (file, ", ");
		  tmp = problem_ptr->geqs[e].coef[v];
		  problem_ptr->geqs[e].coef[v] = 0;
		  printTerm (file, problem_ptr, &problem_ptr->geqs[e], -1);
		  problem_ptr->geqs[e].coef[v] = tmp;
		  live[e] = FALSE;
		  first = 0;
		}
	    if (!first)
	      fprintf (file, " <= ");
	  }


	  /* find chain */
	  chain[0] = v;
	  m = 1;
	  while (1)
	    {
	      /* print chain */
	      for (v2 = 1; v2 <= problem_ptr->num_vars; v2++)
		if (po[v][v2] && chain_length[v] == 1 + chain_length[v2])
		  break;
	      if (v2 > problem_ptr->num_vars)
		break;
	      chain[m++] = v2;
	      v = v2;
	    }

	  fprintf (file, "%s", variable (chain[0]));

	  multiprint = 0;
	  for (i = 1; i < m; i++)
	    {
	      v = chain[i - 1];
	      v2 = chain[i];
	      if (po[v][v2] == le)
		fprintf (file, " <= ");
	      else
		fprintf (file, " < ");
	      fprintf (file, "%s", variable (v2));
	      live[poE[v][v2]] = FALSE;
	      if (!multiprint && i < m - 1)
		for (v3 = 1; v3 <= problem_ptr->num_vars; v3++)
		  {
		    if (v == v3 || v2 == v3)
		      continue;
		    if (po[v][v2] != po[v][v3])
		      continue;
		    if (po[v2][chain[i + 1]] != po[v3][chain[i + 1]])
		      continue;
		    fprintf (file, ",%s", variable (v3));
		    live[poE[v][v3]] = FALSE;
		    live[poE[v3][chain[i + 1]]] = FALSE;
		    multiprint = 1;
		  }
	      else
		multiprint = 0;
	    }

	  v = chain[m - 1];
	  /* print lastLinks */
	  {
	    int tmp, first;
	    first = 1;
	    for (e = 0; e < problem_ptr->num_geqs; e++)
	      if (live[e] && problem_ptr->geqs[e].coef[v] == -1)
		{
		  if (!first)
		    fprintf (file, ", ");
		  else
		    fprintf (file, " <= ");
		  tmp = problem_ptr->geqs[e].coef[v];
		  problem_ptr->geqs[e].coef[v] = 0;
		  printTerm (file, problem_ptr, &problem_ptr->geqs[e], 1);
		  problem_ptr->geqs[e].coef[v] = tmp;
		  live[e] = FALSE;
		  first = 0;
		}
	  }
	}


      for (e = 0; e < problem_ptr->num_geqs; e++)
	if (live[e])
	  {
	    if (stuffPrinted)
	      fprintf (file, "; ");
	    stuffPrinted = 1;
	    omega_print_geq (file, problem_ptr, &problem_ptr->geqs[e]);
	  }

      for (e = 0; e < problem_ptr->num_subs; e++)
	{
	  eqn eq = &problem_ptr->subs[e];
	  if (stuffPrinted)
	    fprintf (file, "; ");
	  stuffPrinted = 1;
	  if (eq->key > 0)
	    fprintf (file, "%s := ", orgVariable (eq->key));
	  else
	    fprintf (file, "#%d := ", eq->key);
	  printTerm (file, problem_ptr, eq, 1);
	}
    }
}

static void
nameWildcard (omega_problem *problem_ptr, int i)
{
  --next_wild_card;
  if (next_wild_card < -maxWildCards)
    next_wild_card = -1;
  problem_ptr->var[i] = next_wild_card;
}

static int
addNewWildcard (omega_problem *problem_ptr)
{
  int e;
  int i = ++problem_ptr->safe_vars;
  problem_ptr->num_vars++;
  if (problem_ptr->num_vars != i)
    {
      for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	{
	  if (problem_ptr->geqs[e].coef[i] != 0)
	    problem_ptr->geqs[e].touched = TRUE;
	  problem_ptr->geqs[e].coef[problem_ptr->num_vars] = problem_ptr->geqs[e].coef[i];

	}

      for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
	problem_ptr->eqs[e].coef[problem_ptr->num_vars] = problem_ptr->eqs[e].coef[i];

      for (e = problem_ptr->num_subs - 1; e >= 0; e--)
	problem_ptr->subs[e].coef[problem_ptr->num_vars] = problem_ptr->subs[e].coef[i];

      problem_ptr->var[problem_ptr->num_vars] = problem_ptr->var[i];
    }

  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    problem_ptr->geqs[e].coef[i] = 0;

  for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
    problem_ptr->eqs[e].coef[i] = 0;

  for (e = problem_ptr->num_subs - 1; e >= 0; e--)
    problem_ptr->subs[e].coef[i] = 0;

  nameWildcard (problem_ptr, i);
  return i;
}

extern void substitute (omega_problem *problem_ptr, eqn sub, int i, int c);
extern void deleteVariable (omega_problem *problem_ptr, int i);

/* Solve e = factor alpha for x_j and substitute */

static void
doMod (omega_problem *problem_ptr, int factor, int e, int j)
{
  int k, i;
  struct eqn eq;
  int nFactor;
  int killJ = FALSE;
  eqncpy (&eq, &problem_ptr->eqs[e]);
  for (k = problem_ptr->num_vars; k >= 0; k--)
    {
      eq.coef[k] = int_mod (eq.coef[k], factor);
      if (2 * eq.coef[k] >= factor)
	eq.coef[k] -= factor;
    }
  nFactor = eq.coef[j];
  if (j <= problem_ptr->safe_vars && problem_ptr->var[j] > 0)
    {
      i = addNewWildcard (problem_ptr);
      eq.coef[problem_ptr->num_vars] = eq.coef[i];
      eq.coef[j] = 0;
      eq.coef[i] = -factor;
      killJ = TRUE;
    }
  else
    {
      eq.coef[j] = -factor;
      if (problem_ptr->var[j] > 0)
	nameWildcard (problem_ptr, j);
    }
  substitute (problem_ptr, &eq, j, nFactor);
  for (k = problem_ptr->num_vars; k >= 0; k--)
    problem_ptr->eqs[e].coef[k] = problem_ptr->eqs[e].coef[k] / factor;
  if (killJ)
    deleteVariable (problem_ptr, j);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Mod-ing and normalizing produces:\n");
      omega_print_problem (dump_file, problem_ptr);
    }
}

void
omega_negate_geq (omega_problem *problem_ptr, int e)
{

  int i;
  for (i = problem_ptr->num_vars; i >= 0; i--)
    problem_ptr->geqs[e].coef[i] = -problem_ptr->geqs[e].coef[i];
  problem_ptr->geqs[e].coef[0]--;
  problem_ptr->geqs[e].touched = TRUE;
}

static int
verify_omega_problem (omega_problem *problem_ptr)
{
  int result, e, anyColor;
  omega_problem *tmp_problem;
  tmp_problem = (omega_problem *) xmalloc (sizeof (omega_problem));
  omega_copy_problem (tmp_problem, problem_ptr);
  tmp_problem->safe_vars = 0;
  tmp_problem->num_subs = 0;
  anyColor = 0;
  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    anyColor |= problem_ptr->geqs[e].color;
  anyColor |= please_no_equalities_in_simplified_problems;
  if (anyColor)
    {
      original_problem = no_problem;
    }
  else
    original_problem = problem_ptr;
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "verifying problem");
      if (anyColor)
	fprintf (dump_file, " (color mode)");
      fprintf (dump_file, " :\n");
      omega_print_problem (dump_file, problem_ptr);
    }

  result = omega_solve_problem (tmp_problem, UNKNOWN);
  original_problem = no_problem;
  free (tmp_problem);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      if (result)
	fprintf (dump_file, "verified problem\n");
      else
	fprintf (dump_file, "disproved problem\n");
      omega_print_problem (dump_file, problem_ptr);
    }
  return result;
}

static void
adding_equality_constraint (omega_problem *problem_ptr, int e)
{
  int e2, i, j;

  if (original_problem != no_problem && original_problem != problem_ptr
      && !conservative)
    {
      e2 = original_problem->num_eqs++;

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file,
		 "adding equality constraint %d to outer problem\n", e2);
      eqnnzero (&original_problem->eqs[e2], original_problem->num_vars);
      for (i = problem_ptr->num_vars; i >= 1; i--)
	{
	  for (j = original_problem->num_vars; j >= 1; j--)
	    if (original_problem->var[j] == problem_ptr->var[i])
	      break;
	  if (j <= 0)
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "retracting\n");
	      original_problem->num_eqs--;
	      return;
	    }
	  original_problem->eqs[e2].coef[j] = problem_ptr->eqs[e].coef[i];
	}
      original_problem->eqs[e2].coef[0] = problem_ptr->eqs[e].coef[0];
      if (dump_file && (dump_flags & TDF_DETAILS))
	omega_print_problem (dump_file, original_problem);
    }
}


static int fastLookup[maxKeys * 2];
static int fastLookupRed[maxKeys * 2];

typedef enum {
  normalize_false,
  normalize_uncoupled,
  normalize_coupled
} normalize_return_type;

static normalize_return_type
normalize_omega_problem (omega_problem *problem_ptr)
{
  int e, i, j, k, nV;
  int coupled_subscripts = 0;

  nV = problem_ptr->num_vars;
  for (e = 0; e < problem_ptr->num_geqs; e++)
    {
      if (!problem_ptr->geqs[e].touched)
	{
	  if (!single_var_geq (problem_ptr->geqs[e], nV))
	    coupled_subscripts = 1;
	}
      else
	{
	  int g;
	  int topVar;
	  int i0;
	  int hashCode;


	  {
	    int *p = &packing[0];
	    for (k = 1; k <= nV; k++)
	      if (problem_ptr->geqs[e].coef[k])
		{
		  *(p++) = k;
		}
	    topVar = (p - &packing[0]) - 1;
	  }

	  if (topVar == -1)
	    {
	      if (problem_ptr->geqs[e].coef[0] < 0)
		{
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    {
		      omega_print_geq (dump_file, problem_ptr, &problem_ptr->geqs[e]);
		      fprintf (dump_file, "\nequations have no solution \n");
		    }
		  return normalize_false;
		}
	      do_delete (problem_ptr, e, nV);
	      e--;
	      continue;
	    }
	  else if (topVar == 0)
	    {
	      int singlevar = packing[0];
	      g = problem_ptr->geqs[e].coef[singlevar];
	      if (g > 0)
		{
		  problem_ptr->geqs[e].coef[singlevar] = 1;
		  problem_ptr->geqs[e].key = singlevar;
		}
	      else
		{
		  g = -g;
		  problem_ptr->geqs[e].coef[singlevar] = -1;
		  problem_ptr->geqs[e].key = -singlevar;
		}
	      if (g > 1)
		problem_ptr->geqs[e].coef[0] = int_div (problem_ptr->geqs[e].coef[0], g);
	    }
	  else
	    {

	      coupled_subscripts = 1;
	      i0 = topVar;
	      i = packing[i0--];
	      g = problem_ptr->geqs[e].coef[i];
	      hashCode = g * (i + 3);
	      if (g < 0)
		g = -g;
	      for (; i0 >= 0; i0--)
		{
		  int x;
		  i = packing[i0];
		  x = problem_ptr->geqs[e].coef[i];
		  hashCode = hashCode * keyMult * (i + 3) + x;
		  if (x < 0)
		    x = -x;
		  if (x == 1)
		    {
		      g = 1;
		      i0--;
		      break;
		    }
		  else
		    g = gcd (x, g);
		}
	      for (; i0 >= 0; i0--)
		{
		  int x;
		  i = packing[i0];
		  x = problem_ptr->geqs[e].coef[i];
		  hashCode = hashCode * keyMult * (i + 3) + x;
		}
	      if (g > 1)
		{
		  problem_ptr->geqs[e].coef[0] = int_div (problem_ptr->geqs[e].coef[0], g);
		  i0 = topVar;
		  i = packing[i0--];
		  problem_ptr->geqs[e].coef[i] = problem_ptr->geqs[e].coef[i] / g;
		  hashCode = problem_ptr->geqs[e].coef[i] * (i + 3);
		  for (; i0 >= 0; i0--)
		    {
		      i = packing[i0];
		      problem_ptr->geqs[e].coef[i] = problem_ptr->geqs[e].coef[i] / g;
		      hashCode =
			hashCode * keyMult * (i + 3) + problem_ptr->geqs[e].coef[i];
		    }
		}

	      {
		int g2 = abs (hashCode);

		if (dump_file && (dump_flags & TDF_DETAILS))
		  {
		    fprintf (dump_file, "Hash code = %d, eqn = ", hashCode);
		    omega_print_geq (dump_file, problem_ptr, &problem_ptr->geqs[e]);
		    fprintf (dump_file, "\n");
		  }

		j = g2 % hash_table_size;
		while (1)
		  {
		    eqn proto = &(hash_master[j]);
		    if (proto->touched == g2)
		      {
			if (proto->coef[0] == topVar)
			  {
			    if (hashCode >= 0)
			      for (i0 = topVar; i0 >= 0; i0--)
				{
				  i = packing[i0];
				  if (problem_ptr->geqs[e].coef[i] != proto->coef[i])
				    break;
				}
			    else
			      for (i0 = topVar; i0 >= 0; i0--)
				{
				  i = packing[i0];
				  if (problem_ptr->geqs[e].coef[i] != -proto->coef[i])
				    break;
				}
			    if (i0 < 0)
			      {
				if (hashCode >= 0)
				  problem_ptr->geqs[e].key = proto->key;
				else
				  problem_ptr->geqs[e].key = -proto->key;
				break;
			      }
			  }
		      }
		    else if (proto->touched < 0)
		      {
			eqnzero (proto);
			if (hashCode >= 0)
			  for (i0 = topVar; i0 >= 0; i0--)
			    {
			      i = packing[i0];
			      proto->coef[i] = problem_ptr->geqs[e].coef[i];
			    }
			else
			  for (i0 = topVar; i0 >= 0; i0--)
			    {
			      i = packing[i0];
			      proto->coef[i] = -problem_ptr->geqs[e].coef[i];
			    }
			proto->coef[0] = topVar;
			proto->touched = g2;

			if (dump_file && (dump_flags & TDF_DETAILS))
			  fprintf (dump_file, " constraint key = %d\n",
				   next_key);

			proto->key = next_key++;
			if (proto->key > maxKeys)
			  {
			    if (dump_file && (dump_flags & TDF_DETAILS))
			      fprintf (dump_file,
				       "too many hash keys generated \n");
			    exit (2);
			  }
			if (hashCode >= 0)
			  problem_ptr->geqs[e].key = proto->key;
			else
			  problem_ptr->geqs[e].key = -proto->key;
			break;
		      }
		    j = (j + 1) % hash_table_size;
		  }
	      }
	    }

	  problem_ptr->geqs[e].touched = FALSE;
	}

      {
	int eKey = problem_ptr->geqs[e].key;
	int e2;
	if (e > 0)
	  {
	    int cTerm = problem_ptr->geqs[e].coef[0];
	    e2 = fastLookup[maxKeys - eKey];
	    if (e2 < e && problem_ptr->geqs[e2].key == -eKey && !problem_ptr->geqs[e2].color)
	      {
		if (problem_ptr->geqs[e2].coef[0] < -cTerm)
		  {
		    if (dump_file && (dump_flags & TDF_DETAILS))
		      {
			omega_print_geq (dump_file, problem_ptr, &problem_ptr->geqs[e]);
			fprintf (dump_file, "\n");
			omega_print_geq (dump_file, problem_ptr, &problem_ptr->geqs[e2]);
			fprintf (dump_file,
				 "\nequations have no solution \n");
		      }
		    return normalize_false;
		  }
		if (problem_ptr->geqs[e2].coef[0] == -cTerm
		    && (createColor || !problem_ptr->geqs[e].color))
		  {
		    eqncpy (&problem_ptr->eqs[problem_ptr->num_eqs], &problem_ptr->geqs[e]);
		    if (!problem_ptr->geqs[e].color)
		      adding_equality_constraint (problem_ptr, problem_ptr->num_eqs);
		    problem_ptr->num_eqs++;
		    gcc_assert (problem_ptr->num_eqs <= max_eqs);
		  }
	      }
	    e2 = fastLookupRed[maxKeys - eKey];
	    if (e2 < e && problem_ptr->geqs[e2].key == -eKey && problem_ptr->geqs[e2].color)
	      {
		if (problem_ptr->geqs[e2].coef[0] < -cTerm)
		  {
		    if (dump_file && (dump_flags & TDF_DETAILS))
		      {
			omega_print_geq (dump_file, problem_ptr, &problem_ptr->geqs[e]);
			fprintf (dump_file, "\n");
			omega_print_geq (dump_file, problem_ptr, &problem_ptr->geqs[e2]);
			fprintf (dump_file,
				 "\nequations have no solution \n");
		      }
		    return normalize_false;
		  }
		if (problem_ptr->geqs[e2].coef[0] == -cTerm && createColor)
		  {
		    eqncpy (&problem_ptr->eqs[problem_ptr->num_eqs], &problem_ptr->geqs[e]);
		    problem_ptr->eqs[problem_ptr->num_eqs].color = 1;
		    problem_ptr->num_eqs++;
		    gcc_assert (problem_ptr->num_eqs <= max_eqs);
		  }
	      }

	    e2 = fastLookup[maxKeys + eKey];
	    if (e2 < e && problem_ptr->geqs[e2].key == eKey && !problem_ptr->geqs[e2].color)
	      {
		if (problem_ptr->geqs[e2].coef[0] > cTerm)
		  {
		    if (!problem_ptr->geqs[e].color)
		      {
			if (dump_file && (dump_flags & TDF_DETAILS))
			  {
			    fprintf (dump_file,
				     "Removing Redudant Equation: ");
			    omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e2]));
			    fprintf (dump_file, "\n");
			    fprintf (dump_file,
				     "[a]      Made Redundant by: ");
			    omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e]));
			    fprintf (dump_file, "\n");
			  }
			problem_ptr->geqs[e2].coef[0] = cTerm;
			do_delete (problem_ptr, e, nV);
			e--;
			continue;
		      }
		  }
		else
		  {
		    if (dump_file && (dump_flags & TDF_DETAILS))
		      {
			fprintf (dump_file, "Removing Redudant Equation: ");
			omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e]));
			fprintf (dump_file, "\n");
			fprintf (dump_file, "[b]      Made Redundant by: ");
			omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e2]));
			fprintf (dump_file, "\n");
		      }
		    do_delete (problem_ptr, e, nV);
		    e--;
		    continue;
		  }
	      }
	    e2 = fastLookupRed[maxKeys + eKey];
	    if (e2 < e && problem_ptr->geqs[e2].key == eKey && problem_ptr->geqs[e2].color)
	      {
		if (problem_ptr->geqs[e2].coef[0] >= cTerm)
		  {
		    if (dump_file && (dump_flags & TDF_DETAILS))
		      {
			fprintf (dump_file, "Removing Redudant Equation: ");
			omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e2]));
			fprintf (dump_file, "\n");
			fprintf (dump_file, "[c]      Made Redundant by: ");
			omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e]));
			fprintf (dump_file, "\n");
		      }
		    problem_ptr->geqs[e2].coef[0] = cTerm;
		    problem_ptr->geqs[e2].color = problem_ptr->geqs[e].color;
		  }
		else if (problem_ptr->geqs[e].color)
		  {
		    if (dump_file && (dump_flags & TDF_DETAILS))
		      {
			fprintf (dump_file, "Removing Redudant Equation: ");
			omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e]));
			fprintf (dump_file, "\n");
			fprintf (dump_file, "[d]      Made Redundant by: ");
			omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e2]));
			fprintf (dump_file, "\n");
		      }
		  }
		do_delete (problem_ptr, e, nV);
		e--;
		continue;

	      }
	  }
	if (problem_ptr->geqs[e].color)
	  fastLookupRed[maxKeys + eKey] = e;
	else
	  fastLookup[maxKeys + eKey] = e;
      }
    }
  createColor = 0;
  return coupled_subscripts ? normalize_coupled : normalize_uncoupled;
}

static void
cleanout_wildcards (omega_problem *problem_ptr)
{
  int e, e2, i, j;
  int a, c, nV;
  int g;
  int renormalize = 0;
  struct eqn *sub;
  nV = problem_ptr->num_vars;
  for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
    {
      for (i = nV; i >= problem_ptr->safe_vars + 1; i--)
	if (problem_ptr->eqs[e].coef[i])
	  {
	    for (j = i - 1; j >= problem_ptr->safe_vars + 1; j--)
	      if (problem_ptr->eqs[e].coef[j])
		break;
	    if (j < problem_ptr->safe_vars + 1)
	      {
		/* Found a single wild card equality */
		if (dump_file && (dump_flags & TDF_DETAILS))
		  {
		    fprintf (dump_file,
			     "Found a single wild card equality: ");
		    omega_print_eq (dump_file, problem_ptr, &problem_ptr->eqs[e]);
		    fprintf (dump_file, "\n");
		    omega_print_problem (dump_file, problem_ptr);
		  }
		c = problem_ptr->eqs[e].coef[i];
		a = abs (c);
		sub = &(problem_ptr->eqs[e]);

		for (e2 = problem_ptr->num_eqs - 1; e2 >= 0; e2--)
		  if (e != e2 && problem_ptr->eqs[e2].coef[i]
		      && problem_ptr->eqs[e].color <= problem_ptr->eqs[e2].color)
		    {
		      eqn eqn = &(problem_ptr->eqs[e2]);
		      int j, k;
		      for (j = nV; j >= 0; j--)
			eqn->coef[j] *= a;
		      k = eqn->coef[i];
		      for (j = nV; j >= 0; j--)
			eqn->coef[j] -= sub->coef[j] * k / c;
		      eqn->coef[i] = 0;
		      g = 0;
		      for (j = nV; j >= 0; j--)
			g = gcd (abs (eqn->coef[j]), g);
		      if (g)
			for (j = nV; j >= 0; j--)
			  eqn->coef[j] = eqn->coef[j] / g;

		    }
		for (e2 = problem_ptr->num_geqs - 1; e2 >= 0; e2--)
		  if (problem_ptr->geqs[e2].coef[i] && problem_ptr->eqs[e].color <= problem_ptr->geqs[e2].color)
		    {
		      eqn eqn = &(problem_ptr->geqs[e2]);
		      int j, k;
		      for (j = nV; j >= 0; j--)
			eqn->coef[j] *= a;
		      k = eqn->coef[i];
		      for (j = nV; j >= 0; j--)
			eqn->coef[j] -= sub->coef[j] * k / c;
		      eqn->coef[i] = 0;
		      eqn->touched = 1;
		      renormalize = 1;
		    }
		for (e2 = problem_ptr->num_subs - 1; e2 >= 0; e2--)
		  if (problem_ptr->subs[e2].coef[i] && problem_ptr->eqs[e].color <= problem_ptr->subs[e2].color)
		    {
		      eqn eqn = &(problem_ptr->subs[e2]);
		      int j, k;
		      for (j = nV; j >= 0; j--)
			eqn->coef[j] *= a;
		      k = eqn->coef[i];
		      for (j = nV; j >= 0; j--)
			eqn->coef[j] -= sub->coef[j] * k / c;
		      eqn->coef[i] = 0;
		      g = 0;
		      for (j = nV; j >= 0; j--)
			g = gcd (abs (eqn->coef[j]), g);
		      if (g)
			for (j = nV; j >= 0; j--)
			  eqn->coef[j] = eqn->coef[j] / g;
		    }

		if (dump_file && (dump_flags & TDF_DETAILS))
		  {
		    fprintf (dump_file, "cleaned-out wildcard: ");
		    omega_print_problem (dump_file, problem_ptr);
		  }
		break;
	      }
	  }
    }
  if (renormalize)
    normalize_omega_problem (problem_ptr);
}

static inline void
swap (int *i, int *j)
{
  int tmp;
  tmp = *i;
  *i = *j;
  *j = tmp;
}

static void
resurrectSubs (omega_problem *problem_ptr)
{
  if (problem_ptr->num_subs > 0 && !please_no_equalities_in_simplified_problems)
    {
      int i, e, n, m;

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file,
		   "problem reduced, bringing variables back to life\n");
	  omega_print_problem (dump_file, problem_ptr);
	}
      for (i = 1; i <= problem_ptr->safe_vars; i++)
	if (problem_ptr->var[i] < 0)
	  {
	    /* wild card */
	    if (i < problem_ptr->safe_vars)
	      {
		int j = problem_ptr->safe_vars;
		swap (&problem_ptr->var[i], &problem_ptr->var[j]);
		for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
		  {
		    problem_ptr->geqs[e].touched = 1;
		    swap (&problem_ptr->geqs[e].coef[i], &problem_ptr->geqs[e].coef[j]);
		  }
		for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
		  swap (&problem_ptr->eqs[e].coef[i], &problem_ptr->eqs[e].coef[j]);
		for (e = problem_ptr->num_subs - 1; e >= 0; e--)
		  swap (&problem_ptr->subs[e].coef[i], &problem_ptr->subs[e].coef[j]);
		i--;
	      }
	    problem_ptr->safe_vars--;
	  }

      m = problem_ptr->num_subs;
      n = max (problem_ptr->num_vars, problem_ptr->safe_vars + m);
      for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	{
	  if (single_var_geq (problem_ptr->geqs[e], problem_ptr->num_vars))
	    {
	      i = abs (problem_ptr->geqs[e].key);
	      if (i >= problem_ptr->safe_vars + 1)
		problem_ptr->geqs[e].key += (problem_ptr->geqs[e].key > 0 ? m : -m);
	    }
	  else
	    {
	      problem_ptr->geqs[e].touched = TRUE;
	      problem_ptr->geqs[e].key = 0;
	    }
	}
      for (i = problem_ptr->num_vars; i >= problem_ptr->safe_vars + 1; i--)
	{
	  problem_ptr->var[i + m] = problem_ptr->var[i];
	  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	    problem_ptr->geqs[e].coef[i + m] = problem_ptr->geqs[e].coef[i];
	  for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
	    problem_ptr->eqs[e].coef[i + m] = problem_ptr->eqs[e].coef[i];
	  for (e = problem_ptr->num_subs - 1; e >= 0; e--)
	    problem_ptr->subs[e].coef[i + m] = problem_ptr->subs[e].coef[i];
	}
      for (i = problem_ptr->safe_vars + m; i >= problem_ptr->safe_vars + 1; i--)
	{
	  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	    problem_ptr->geqs[e].coef[i] = 0;
	  for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
	    problem_ptr->eqs[e].coef[i] = 0;
	  for (e = problem_ptr->num_subs - 1; e >= 0; e--)
	    problem_ptr->subs[e].coef[i] = 0;
	}
      problem_ptr->num_vars += m;
      for (e = problem_ptr->num_subs - 1; e >= 0; e--)
	{
	  problem_ptr->var[problem_ptr->safe_vars + 1 + e] = problem_ptr->subs[e].key;
	  eqncpy (&(problem_ptr->eqs[problem_ptr->num_eqs]), &(problem_ptr->subs[e]));
	  problem_ptr->eqs[problem_ptr->num_eqs].coef[problem_ptr->safe_vars + 1 + e] = -1;
	  problem_ptr->eqs[problem_ptr->num_eqs].color = black;

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "brought back: ");
	      omega_print_eq (dump_file, problem_ptr, &problem_ptr->eqs[problem_ptr->num_eqs]);
	      fprintf (dump_file, "\n");
	    }


	  problem_ptr->num_eqs++;
	  gcc_assert (problem_ptr->num_eqs <= max_eqs);
	}
      problem_ptr->safe_vars += m;
      problem_ptr->num_subs = 0;

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "variables brought back to life\n");
	  omega_print_problem (dump_file, problem_ptr);
	}

      cleanout_wildcards (problem_ptr);
    }
}

#define implies(A,B) (A==(A&B))

int
omega_eliminate_redundant (omega_problem *problem_ptr, bool expensive)
{
  int e, e1, e2, e3, p, q, i, k, alpha, alpha1, alpha2, alpha3;
  int c;
  int isDead[max_geqs];
  unsigned int P[max_geqs], Z[max_geqs], N[max_geqs];
  unsigned int PP, PZ, PN;	/* possible Positives, possible zeros & possible negatives */

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "in eliminate Redudant:\n");
      omega_print_problem (dump_file, problem_ptr);
    }

  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    {
      int tmp = 1;
      isDead[e] = 0;
      P[e] = Z[e] = N[e] = 0;
      for (i = problem_ptr->num_vars; i >= 1; i--)
	{
	  if (problem_ptr->geqs[e].coef[i] > 0)
	    P[e] |= tmp;
	  else if (problem_ptr->geqs[e].coef[i] < 0)
	    N[e] |= tmp;
	  else
	    Z[e] |= tmp;
	  tmp <<= 1;
	}
    }


  for (e1 = problem_ptr->num_geqs - 1; e1 >= 0; e1--)
    if (!isDead[e1])
      for (e2 = e1 - 1; e2 >= 0; e2--)
	if (!isDead[e2])
	  {
	    for (p = problem_ptr->num_vars; p > 1; p--)
	      {
		for (q = p - 1; q > 0; q--)
		  {
		    alpha =
		      (problem_ptr->geqs[e1].coef[p] * problem_ptr->geqs[e2].coef[q] -
		       problem_ptr->geqs[e2].coef[p] * problem_ptr->geqs[e1].coef[q]);
		    if (alpha != 0)
		      goto foundPQ;
		  }
	      }
	    continue;

	  foundPQ:
	    PZ = (Z[e1] & Z[e2]) | (P[e1] & N[e2]) | (N[e1] & P[e2]);
	    PP = P[e1] | P[e2];
	    PN = N[e1] | N[e2];

	    for (e3 = problem_ptr->num_geqs - 1; e3 >= 0; e3--)
	      if (e3 != e1 && e3 != e2)
		{

		  if (!implies (Z[e3], PZ))
		    goto nextE3;

		  alpha1 =
		    problem_ptr->geqs[e2].coef[q] * problem_ptr->geqs[e3].coef[p] -
		    problem_ptr->geqs[e2].coef[p] * problem_ptr->geqs[e3].coef[q];
		  alpha2 =
		    -(problem_ptr->geqs[e1].coef[q] * problem_ptr->geqs[e3].coef[p] -
		      problem_ptr->geqs[e1].coef[p] * problem_ptr->geqs[e3].coef[q]);
		  alpha3 = alpha;

		  if (alpha1 * alpha2 <= 0)
		    goto nextE3;
		  if (alpha1 < 0)
		    {
		      alpha1 = -alpha1;
		      alpha2 = -alpha2;
		      alpha3 = -alpha3;
		    }
		  if (alpha3 > 0)
		    {
		      /* Trying to prove e3 is redundant */
		      if (!implies (P[e3], PP) | !implies (N[e3], PN))
			goto nextE3;
		      if (!problem_ptr->geqs[e3].color
			  && (problem_ptr->geqs[e1].color || problem_ptr->geqs[e2].color))
			goto nextE3;

		      /* verify alpha1*v1+alpha2*v2 = alpha3*v3 */
		      for (k = problem_ptr->num_vars; k >= 1; k--)
			if (alpha3 * problem_ptr->geqs[e3].coef[k]
			    !=
			    alpha1 * problem_ptr->geqs[e1].coef[k] +
			    alpha2 * problem_ptr->geqs[e2].coef[k])
			  goto nextE3;

		      c =
			alpha1 * problem_ptr->geqs[e1].coef[0] + alpha2 * problem_ptr->geqs[e2].coef[0];
		      if (c < alpha3 * (problem_ptr->geqs[e3].coef[0] + 1))
			{
			  if (dump_file && (dump_flags & TDF_DETAILS))
			    {
			      fprintf (dump_file,
				       "found redundant inequality\n");
			      fprintf (dump_file,
				       "alpha1, alpha2, alpha3 = %d,%d,%d\n",
				       alpha1, alpha2, alpha3);

			      omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e1]));
			      fprintf (dump_file, "\n");
			      omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e2]));
			      fprintf (dump_file, "\n=> ");
			      omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e3]));
			      fprintf (dump_file, "\n\n");
			    }

			  isDead[e3] = 1;
			}
		    }
		  else
		    {
		      /*
		       * trying to prove e3 <= 0 and therefore e3 = 0, or trying to prove e3 < 0, and
		       * therefore the problem has no solutions
		       * 
		       */
		      if (!implies (P[e3], PN) | !implies (N[e3], PP))
			goto nextE3;
		      if (problem_ptr->geqs[e1].color || problem_ptr->geqs[e2].color || problem_ptr->geqs[e3].color)
			goto nextE3;

		      alpha3 = alpha3;
		      /* verify alpha1*v1+alpha2*v2 = alpha3*v3 */
		      for (k = problem_ptr->num_vars; k >= 1; k--)
			if (alpha3 * problem_ptr->geqs[e3].coef[k]
			    !=
			    alpha1 * problem_ptr->geqs[e1].coef[k] +
			    alpha2 * problem_ptr->geqs[e2].coef[k])
			  goto nextE3;

		      c =
			alpha1 * problem_ptr->geqs[e1].coef[0] + alpha2 * problem_ptr->geqs[e2].coef[0];
		      if (c < alpha3 * (problem_ptr->geqs[e3].coef[0]))
			{

			  /* We just proved e3 < 0, so no solutions exist */
			  if (dump_file && (dump_flags & TDF_DETAILS))
			    {
			      fprintf (dump_file,
				       "found implied over tight inequality\n");
			      fprintf (dump_file,
				       "alpha1, alpha2, alpha3 = %d,%d,%d\n",
				       alpha1, alpha2, -alpha3);
			      omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e1]));
			      fprintf (dump_file, "\n");
			      omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e2]));
			      fprintf (dump_file, "\n=> not ");
			      omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e3]));
			      fprintf (dump_file, "\n\n");
			    }
			  return 0;

			}
		      else if (c < alpha3 * (problem_ptr->geqs[e3].coef[0] - 1))
			{

			  /* We just proved that e3 <=0, so e3 = 0 */
			  if (dump_file && (dump_flags & TDF_DETAILS))
			    {
			      fprintf (dump_file,
				       "found implied tight inequality\n");
			      fprintf (dump_file,
				       "alpha1, alpha2, alpha3 = %d,%d,%d\n",
				       alpha1, alpha2, -alpha3);
			      omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e1]));
			      fprintf (dump_file, "\n");
			      omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e2]));
			      fprintf (dump_file, "\n=> inverse ");
			      omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e3]));
			      fprintf (dump_file, "\n\n");
			    }
			  eqncpy (&problem_ptr->eqs[problem_ptr->num_eqs++], &problem_ptr->geqs[e3]);
			  gcc_assert (problem_ptr->num_eqs <= max_eqs);
			  adding_equality_constraint (problem_ptr, problem_ptr->num_eqs - 1);
			  isDead[e3] = 1;

			}
		    }
		nextE3:;
		}
	  }
  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    if (isDead[e])
      do_delete (problem_ptr, e, problem_ptr->num_vars);

  /* if (problem_ptr->num_eqs) return solve(problem_ptr, SIMPLIFY); */

  if (!expensive)
    return 1;
  {
    omega_problem *tmp_problem;
    int oldTrace = trace;
    trace = 0;
    tmp_problem = (omega_problem *) xmalloc (sizeof (omega_problem));
    conservative++;
    for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
      {
	if (dump_file && (dump_flags & TDF_DETAILS))
	  {
	    fprintf (dump_file,
		     "checking equation %d to see if it is redundant: ", e);
	    omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e]));
	    fprintf (dump_file, "\n");
	  }
	omega_copy_problem (tmp_problem, problem_ptr);
	omega_negate_geq (tmp_problem, e);
	tmp_problem->safe_vars = 0;
	tmp_problem->variablesFreed = 0;
	if (!omega_solve_problem (tmp_problem, FALSE))
	  do_delete (problem_ptr, e, problem_ptr->num_vars);
      }
    trace = oldTrace;
    free (tmp_problem);
    conservative--;
  }
  if (reduceWithSubs)
    {
    }
  else
    {
      resurrectSubs (problem_ptr);
      gcc_assert (please_no_equalities_in_simplified_problems || problem_ptr->num_subs == 0);
    }
  return 1;
}

static int
smooth_weird_equations (omega_problem *problem_ptr)
{
  int e1, e2, e3, p, q, k, alpha, alpha1, alpha2, alpha3;
  int c;
  int v;
  int result = 0;

  for (e1 = problem_ptr->num_geqs - 1; e1 >= 0; e1--)
    if (!problem_ptr->geqs[e1].color)
      {
	int g = 999999;
	for (v = problem_ptr->num_vars; v >= 1; v--)
	  if (problem_ptr->geqs[e1].coef[v] != 0 && abs (problem_ptr->geqs[e1].coef[v]) < g)
	    g = abs (problem_ptr->geqs[e1].coef[v]);
	if (g > 20)
	  {

	    e3 = problem_ptr->num_geqs;
	    for (v = problem_ptr->num_vars; v >= 1; v--)
	      problem_ptr->geqs[e3].coef[v] = int_div (6 * problem_ptr->geqs[e1].coef[v] + g / 2, g);
	    problem_ptr->geqs[e3].color = 0;
	    problem_ptr->geqs[e3].touched = 1;
	    problem_ptr->geqs[e3].coef[0] = 9997;

	    if (dump_file && (dump_flags & TDF_DETAILS))
	      {
		fprintf (dump_file, "Checking to see if we can derive: ");
		omega_print_geq (dump_file, problem_ptr, &problem_ptr->geqs[e3]);
		fprintf (dump_file, "\n from: ");
		omega_print_geq (dump_file, problem_ptr, &problem_ptr->geqs[e1]);
		fprintf (dump_file, "\n");
	      }


	    for (e2 = problem_ptr->num_geqs - 1; e2 >= 0; e2--)
	      if (e1 != e2 && !problem_ptr->geqs[e2].color)
		{
		  for (p = problem_ptr->num_vars; p > 1; p--)
		    {
		      for (q = p - 1; q > 0; q--)
			{
			  alpha =
			    (problem_ptr->geqs[e1].coef[p] * problem_ptr->geqs[e2].coef[q] -
			     problem_ptr->geqs[e2].coef[p] * problem_ptr->geqs[e1].coef[q]);
			  if (alpha != 0)
			    goto foundPQ;
			}
		    }
		  continue;

		foundPQ:

		  alpha1 =
		    problem_ptr->geqs[e2].coef[q] * problem_ptr->geqs[e3].coef[p] -
		    problem_ptr->geqs[e2].coef[p] * problem_ptr->geqs[e3].coef[q];
		  alpha2 =
		    -(problem_ptr->geqs[e1].coef[q] * problem_ptr->geqs[e3].coef[p] -
		      problem_ptr->geqs[e1].coef[p] * problem_ptr->geqs[e3].coef[q]);
		  alpha3 = alpha;

		  if (alpha1 * alpha2 <= 0)
		    continue;
		  if (alpha1 < 0)
		    {
		      alpha1 = -alpha1;
		      alpha2 = -alpha2;
		      alpha3 = -alpha3;
		    }
		  if (alpha3 > 0)
		    {
		      /* Trying to prove e3 is redundant */

		      /* verify alpha1*v1+alpha2*v2 = alpha3*v3 */
		      for (k = problem_ptr->num_vars; k >= 1; k--)
			if (alpha3 * problem_ptr->geqs[e3].coef[k]
			    !=
			    alpha1 * problem_ptr->geqs[e1].coef[k] +
			    alpha2 * problem_ptr->geqs[e2].coef[k])
			  goto nextE2;

		      c =
			alpha1 * problem_ptr->geqs[e1].coef[0] + alpha2 * problem_ptr->geqs[e2].coef[0];
		      if (c < alpha3 * (problem_ptr->geqs[e3].coef[0] + 1))
			problem_ptr->geqs[e3].coef[0] = int_div (c, alpha3);

		    }
		nextE2:;
		}
	    if (problem_ptr->geqs[e3].coef[0] < 9997)
	      {
		result++;
		problem_ptr->num_geqs++;

		if (dump_file && (dump_flags & TDF_DETAILS))
		  {
		    fprintf (dump_file,
			     "Smoothing wierd equations; adding:\n");
		    omega_print_geq (dump_file, problem_ptr, &problem_ptr->geqs[e3]);
		    fprintf (dump_file, "\nto:\n");
		    omega_print_problem (dump_file, problem_ptr);
		    fprintf (dump_file, "\n\n");
		  }
	      }
	  }
      }
  return result;
}

static void
coalesce (omega_problem *problem_ptr)
{
  int e, e2, colors;
  int isDead[max_geqs];
  int foundSomething = 0;

  colors = 0;
  for (e = 0; e < problem_ptr->num_geqs; e++)
    if (problem_ptr->geqs[e].color)
      colors++;

  if (colors < 2)
    return;

  for (e = 0; e < problem_ptr->num_geqs; e++)
    isDead[e] = 0;

  for (e = 0; e < problem_ptr->num_geqs; e++)
    if (problem_ptr->geqs[e].color & !problem_ptr->geqs[e].touched)
      for (e2 = e + 1; e2 < problem_ptr->num_geqs; e2++)
	if (!problem_ptr->geqs[e2].touched && problem_ptr->geqs[e].key == -problem_ptr->geqs[e2].key
	    && problem_ptr->geqs[e].coef[0] == -problem_ptr->geqs[e2].coef[0] && problem_ptr->geqs[e2].color)
	  {
	    eqncpy (&problem_ptr->eqs[problem_ptr->num_eqs++], &problem_ptr->geqs[e]);
	    gcc_assert (problem_ptr->num_eqs <= max_eqs);
	    foundSomething++;
	    isDead[e] = 1;
	    isDead[e2] = 1;
	  }

  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    if (isDead[e])
      do_delete (problem_ptr, e, problem_ptr->num_vars);

  if (dump_file && (dump_flags & TDF_DETAILS) && foundSomething)
    {
      fprintf (dump_file, "Coalesced problem_ptr->geqs into %d EQ's:\n", foundSomething);
      omega_print_problem (dump_file, problem_ptr);
    }

}

void
omega_eliminate_red (omega_problem *problem_ptr, bool eliminateAll)
{
  int e, e2, e3, i, j, k, a, alpha1, alpha2;
  int c = 0;
  int isDead[max_geqs];
  int deadCount = 0;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "in eliminate RED:\n");
      omega_print_problem (dump_file, problem_ptr);
    }
  if (problem_ptr->num_eqs > 0)
    omega_simplify_problem (problem_ptr);

  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    isDead[e] = 0;
  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    if (!problem_ptr->geqs[e].color && !isDead[e])
      for (e2 = e - 1; e2 >= 0; e2--)
	if (!problem_ptr->geqs[e2].color && !isDead[e2])
	  {
	    a = 0;
	    for (i = problem_ptr->num_vars; i > 1; i--)
	      {
		for (j = i - 1; j > 0; j--)
		  {
		    a =
		      (problem_ptr->geqs[e].coef[i] * problem_ptr->geqs[e2].coef[j] -
		       problem_ptr->geqs[e2].coef[i] * problem_ptr->geqs[e].coef[j]);
		    if (a != 0)
		      goto foundPair;
		  }
	      }
	    continue;

	  foundPair:
	    if (dump_file && (dump_flags & TDF_DETAILS))
	      {
		fprintf (dump_file,
			 "found two equations to combine, i = %s, ",
			 variable (i));
		fprintf (dump_file, "j = %s, alpha = %d\n", variable (j), a);
		omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e]));
		fprintf (dump_file, "\n");
		omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e2]));
		fprintf (dump_file, "\n");
	      }
	    for (e3 = problem_ptr->num_geqs - 1; e3 >= 0; e3--)
	      if (problem_ptr->geqs[e3].color)
		{
		  alpha1 =
		    problem_ptr->geqs[e2].coef[j] * problem_ptr->geqs[e3].coef[i] -
		    problem_ptr->geqs[e2].coef[i] * problem_ptr->geqs[e3].coef[j];
		  alpha2 =
		    -(problem_ptr->geqs[e].coef[j] * problem_ptr->geqs[e3].coef[i] -
		      problem_ptr->geqs[e].coef[i] * problem_ptr->geqs[e3].coef[j]);
		  if ((a > 0 && alpha1 > 0 && alpha2 > 0)
		      || (a < 0 && alpha1 < 0 && alpha2 < 0))
		    {
		      if (dump_file && (dump_flags & TDF_DETAILS))
			{
			  fprintf (dump_file,
				   "alpha1 = %d, alpha2 = %d; comparing against: ",
				   alpha1, alpha2);
			  omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e3]));
			  fprintf (dump_file, "\n");
			}
		      for (k = problem_ptr->num_vars; k >= 0; k--)
			{
			  c =
			    alpha1 * problem_ptr->geqs[e].coef[k] +
			    alpha2 * problem_ptr->geqs[e2].coef[k];
			  if (c != a * problem_ptr->geqs[e3].coef[k])
			    break;
			  if (dump_file && (dump_flags & TDF_DETAILS) && k > 0)
			    fprintf (dump_file, " %s: %d, %d\n",
				     variable (k), c, a * problem_ptr->geqs[e3].coef[k]);
			}
		      if (k < 0
			  || (k == 0 &&
			      ((a > 0 && c < a * problem_ptr->geqs[e3].coef[k])
			       || (a < 0 && c > a * problem_ptr->geqs[e3].coef[k]))))
			{
			  if (dump_file && (dump_flags & TDF_DETAILS))
			    {
			      deadCount++;
			      fprintf (dump_file,
				       "red equation#%d is dead (%d dead so far, %d remain)\n",
				       e3, deadCount, problem_ptr->num_geqs - deadCount);
			      omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e]));
			      fprintf (dump_file, "\n");
			      omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e2]));
			      fprintf (dump_file, "\n");
			      omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e3]));
			      fprintf (dump_file, "\n");
			    }
			  isDead[e3] = 1;
			}
		    }
		}
	  }
  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    if (isDead[e])
      do_delete (problem_ptr, e, problem_ptr->num_vars);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "in eliminate RED, easy tests done:\n");
      omega_print_problem (dump_file, problem_ptr);
    }

  {
    int redFound = 0;
    for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
      if (problem_ptr->geqs[e].color)
	redFound = 1;
    if (!redFound)
      {
	if (dump_file && (dump_flags & TDF_DETAILS))
	  fprintf (dump_file, "fast checks worked\n");
	if (reduceWithSubs)
	  {
	  }
	else
	  {
	    gcc_assert (please_no_equalities_in_simplified_problems || problem_ptr->num_subs == 0);
	  }
	return;
      }
  }

  if (!verifySimplification)
    {
      if (!verify_omega_problem (problem_ptr))
	return;
    }
  {
    omega_problem *tmp_problem;
    int oldTrace = trace;
    trace = 0;
    conservative++;
    tmp_problem = (omega_problem *) xmalloc (sizeof (omega_problem));
    for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
      if (problem_ptr->geqs[e].color)
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file,
		       "checking equation %d to see if it is redundant: ", e);
	      omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e]));
	      fprintf (dump_file, "\n");
	    }
	  omega_copy_problem (tmp_problem, problem_ptr);
	  omega_negate_geq (tmp_problem, e);
	  tmp_problem->safe_vars = 0;
	  tmp_problem->variablesFreed = 0;
	  tmp_problem->num_subs = 0;
	  if (!omega_solve_problem (tmp_problem, FALSE))
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "it is redundant\n");
	      do_delete (problem_ptr, e, problem_ptr->num_vars);
	    }
	  else
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "it is not redundant\n");
	      if (!eliminateAll)
		{
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    fprintf (dump_file,
			     "no need to check other red equations\n");
		  break;
		}
	    }
	}
    trace = oldTrace;
    conservative--;
    free (tmp_problem);
  }
  /* omega_simplify_problem (problem_ptr); */
  if (reduceWithSubs)
    {
    }
  else
    {
      gcc_assert (please_no_equalities_in_simplified_problems || problem_ptr->num_subs == 0);
    }
}

static void
chainUnprotect (omega_problem *problem_ptr)
{
  int i, e;
  int unprotect[max_vars];

  for (i = 1; i <= problem_ptr->safe_vars; i++)
    {
      unprotect[i] = (problem_ptr->var[i] < 0);
      for (e = problem_ptr->num_subs - 1; e >= 0; e--)
	if (problem_ptr->subs[e].coef[i])
	  unprotect[i] = 0;
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Doing chain reaction unprotection\n");
      omega_print_problem (dump_file, problem_ptr);
      for (i = 1; i <= problem_ptr->safe_vars; i++)
	if (unprotect[i])
	  fprintf (dump_file, "unprotecting %s\n", variable (i));
    }

  for (i = 1; i <= problem_ptr->safe_vars; i++)
    if (unprotect[i])
      {
	/* wild card */
	if (i < problem_ptr->safe_vars)
	  {
	    int j = problem_ptr->safe_vars;
	    swap (&problem_ptr->var[i], &problem_ptr->var[j]);
	    for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	      {
		problem_ptr->geqs[e].touched = 1;
		swap (&problem_ptr->geqs[e].coef[i], &problem_ptr->geqs[e].coef[j]);
	      }
	    for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
	      swap (&problem_ptr->eqs[e].coef[i], &problem_ptr->eqs[e].coef[j]);
	    for (e = problem_ptr->num_subs - 1; e >= 0; e--)
	      swap (&problem_ptr->subs[e].coef[i], &problem_ptr->subs[e].coef[j]);
	    swap (&unprotect[i], &unprotect[j]);
	    i--;
	  }
	problem_ptr->safe_vars--;
      }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "After chain reactions\n");
      omega_print_problem (dump_file, problem_ptr);
    }
}

static void
omega_problem_reduced (omega_problem *problem_ptr)
{
  if (verifySimplification)
    {
      int result;
      if (inApproximateMode)
	result = 1;
      else
	result = verify_omega_problem (problem_ptr);
      if (!result)
	return;
      if (problem_ptr->num_eqs > 0)
	doItAgain = 1;
    }
#ifdef eliminateRedundantConstraints
  if (!omega_eliminate_redundant (problem_ptr, 1))
    return;
#endif
  foundReduction = TRUE;
  if (!please_no_equalities_in_simplified_problems)
    coalesce (problem_ptr);
  if (reduceWithSubs || please_no_equalities_in_simplified_problems)
    chainUnprotect (problem_ptr);
  else
    resurrectSubs (problem_ptr);

  if (!returnSingleResult)
    {
      int i;
      for (i = 1; i <= problem_ptr->safe_vars; i++)
	problem_ptr->forwarding_address[problem_ptr->var[i]] = i;
      for (i = 0; i < problem_ptr->num_subs; i++)
	problem_ptr->forwarding_address[problem_ptr->subs[i].key] = -i - 1;
      (*whenReduced) (problem_ptr);
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "-------------------------------------------\n");
      fprintf (dump_file, "problem reduced:\n");
      omega_print_problem (dump_file, problem_ptr);
      fprintf (dump_file, "-------------------------------------------\n");
    }


}


static void
freeEliminations (omega_problem *problem_ptr, int fv)
/* do free eliminations */
{
  int tryAgain = 1;
  int i, e, e2;
  int nV = problem_ptr->num_vars;
  while (tryAgain)
    {
      tryAgain = 0;
      for (i = nV; i > fv; i--)
	{
	  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	    if (problem_ptr->geqs[e].coef[i])
	      break;
	  if (e < 0)
	    e2 = e;
	  else if (problem_ptr->geqs[e].coef[i] > 0)
	    {
	      for (e2 = e - 1; e2 >= 0; e2--)
		if (problem_ptr->geqs[e2].coef[i] < 0)
		  break;
	    }
	  else
	    {
	      for (e2 = e - 1; e2 >= 0; e2--)
		if (problem_ptr->geqs[e2].coef[i] > 0)
		  break;
	    }
	  if (e2 < 0)
	    {
	      int e3;
	      for (e3 = problem_ptr->num_subs - 1; e3 >= 0; e3--)
		if (problem_ptr->subs[e3].coef[i])
		  break;
	      if (e3 >= 0)
		continue;
	      for (e3 = problem_ptr->num_eqs - 1; e3 >= 0; e3--)
		if (problem_ptr->eqs[e3].coef[i])
		  break;
	      if (e3 >= 0)
		continue;

	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "a free elimination of %s\n",
			 variable (i));
	      if (e >= 0)
		{
		  do_delete (problem_ptr, e, nV);
		  for (e--; e >= 0; e--)
		    if (problem_ptr->geqs[e].coef[i])
		      do_delete (problem_ptr, e, nV);
		  tryAgain = (i < nV);
		}
	      deleteVariable (problem_ptr, i);
	      nV = problem_ptr->num_vars;
	    }
	}
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nafter free eliminations:\n");
      omega_print_problem (dump_file, problem_ptr);
      fprintf (dump_file, "\n");
    }
}

static void
freeRedEliminations (omega_problem *problem_ptr)
/* do free red eliminations */
{
  int tryAgain = 1;
  int i, e, e2;
  int nV = problem_ptr->num_vars;
  int isRedVar[max_vars];
  int isDeadVar[max_vars];
  int isDeadGEQ[max_geqs];
  for (i = nV; i > 0; i--)
    {
      isRedVar[i] = 0;
      isDeadVar[i] = 0;
    }
  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    {
      isDeadGEQ[e] = 0;
      if (problem_ptr->geqs[e].color)
	for (i = nV; i > 0; i--)
	  if (problem_ptr->geqs[e].coef[i] != 0)
	    isRedVar[i] = 1;
    }

  while (tryAgain)
    {
      tryAgain = 0;
      for (i = nV; i > 0; i--)
	if (!isRedVar[i] && !isDeadVar[i])
	  {
	    for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	      if (!isDeadGEQ[e] && problem_ptr->geqs[e].coef[i])
		break;
	    if (e < 0)
	      e2 = e;
	    else if (problem_ptr->geqs[e].coef[i] > 0)
	      {
		for (e2 = e - 1; e2 >= 0; e2--)
		  if (!isDeadGEQ[e2] && problem_ptr->geqs[e2].coef[i] < 0)
		    break;
	      }
	    else
	      {
		for (e2 = e - 1; e2 >= 0; e2--)
		  if (!isDeadGEQ[e2] && problem_ptr->geqs[e2].coef[i] > 0)
		    break;
	      }
	    if (e2 < 0)
	      {
		int e3;
		for (e3 = problem_ptr->num_subs - 1; e3 >= 0; e3--)
		  if (problem_ptr->subs[e3].coef[i])
		    break;
		if (e3 >= 0)
		  continue;
		for (e3 = problem_ptr->num_eqs - 1; e3 >= 0; e3--)
		  if (problem_ptr->eqs[e3].coef[i])
		    break;
		if (e3 >= 0)
		  continue;

		if (dump_file && (dump_flags & TDF_DETAILS))
		  fprintf (dump_file, "a free red elimination of %s\n",
			   variable (i));
		for (; e >= 0; e--)
		  if (problem_ptr->geqs[e].coef[i])
		    isDeadGEQ[e] = 1;
		tryAgain = 1;
		isDeadVar[i] = 1;
	      }
	  }
    }

  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    if (isDeadGEQ[e])
      do_delete (problem_ptr, e, nV);

  for (i = nV; i > 0; i--)
    if (isDeadVar[i])
      deleteVariable (problem_ptr, i);


  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nafter free red eliminations:\n");
      omega_print_problem (dump_file, problem_ptr);
      fprintf (dump_file, "\n");
    }
}

void
substitute (omega_problem *problem_ptr, eqn sub, int i, int c)
{
  int e, j, j0, k;
  int topVar;

  {
    int *p = &packing[0];
    for (k = problem_ptr->num_vars; k >= 0; k--)
      if (sub->coef[k])
	*(p++) = k;
    topVar = (p - &packing[0]) - 1;
  }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "substituting using %s := ", variable (i));
      printTerm (dump_file, problem_ptr, sub, -c);
      fprintf (dump_file, "\n");
      printVars (dump_file, problem_ptr);
    }

  if (topVar < 0)
    {
      for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
	problem_ptr->eqs[e].coef[i] = 0;
      for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	if (problem_ptr->geqs[e].coef[i] != 0)
	  {
	    problem_ptr->geqs[e].touched = TRUE;
	    problem_ptr->geqs[e].coef[i] = 0;
	  }
      for (e = problem_ptr->num_subs - 1; e >= 0; e--)
	problem_ptr->subs[e].coef[i] = 0;

      if (i <= problem_ptr->safe_vars && problem_ptr->var[i] >= 0)
	{
	  eqn eqn = &(problem_ptr->subs[problem_ptr->num_subs++]);
	  for (k = problem_ptr->num_vars; k >= 0; k--)
	    eqn->coef[k] = 0;
	  eqn->key = problem_ptr->var[i];
	  eqn->color = 0;
	}
    }
  else if (topVar == 0 && packing[0] == 0)
    {
      c = -sub->coef[0] * c;
      for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
	{
	  problem_ptr->eqs[e].coef[0] += problem_ptr->eqs[e].coef[i] * c;
	  problem_ptr->eqs[e].coef[i] = 0;
	}
      for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	if (problem_ptr->geqs[e].coef[i] != 0)
	  {
	    problem_ptr->geqs[e].coef[0] += problem_ptr->geqs[e].coef[i] * c;
	    problem_ptr->geqs[e].coef[i] = 0;
	    problem_ptr->geqs[e].touched = TRUE;
	  }
      for (e = problem_ptr->num_subs - 1; e >= 0; e--)
	{
	  problem_ptr->subs[e].coef[0] += problem_ptr->subs[e].coef[i] * c;
	  problem_ptr->subs[e].coef[i] = 0;
	}
      if (i <= problem_ptr->safe_vars && problem_ptr->var[i] >= 0)
	{
	  eqn eqn = &(problem_ptr->subs[problem_ptr->num_subs++]);
	  for (k = problem_ptr->num_vars; k >= 1; k--)
	    eqn->coef[k] = 0;
	  eqn->coef[0] = c;
	  eqn->key = problem_ptr->var[i];
	  eqn->color = 0;
	}

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "---\n\n");
	  omega_print_problem (dump_file, problem_ptr);
	  fprintf (dump_file, "===\n\n");
	}
    }
  else
    {
      for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
	{
	  eqn eqn = &(problem_ptr->eqs[e]);
	  k = eqn->coef[i];
	  if (k != 0)
	    {
	      k = c * k;
	      eqn->coef[i] = 0;
	      for (j = topVar; j >= 0; j--)
		{
		  j0 = packing[j];
		  eqn->coef[j0] -= sub->coef[j0] * k;
		}
	    }

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      omega_print_eq (dump_file, problem_ptr, eqn);
	      fprintf (dump_file, "\n");
	    }
	}
      for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	{
	  eqn eqn = &(problem_ptr->geqs[e]);
	  k = eqn->coef[i];
	  if (k != 0)
	    {
	      k = c * k;
	      eqn->touched = TRUE;
	      eqn->coef[i] = 0;
	      for (j = topVar; j >= 0; j--)
		{
		  j0 = packing[j];
		  eqn->coef[j0] -= sub->coef[j0] * k;
		}
	    }

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      omega_print_geq (dump_file, problem_ptr, eqn);
	      fprintf (dump_file, "\n");
	    }
	}
      for (e = problem_ptr->num_subs - 1; e >= 0; e--)
	{
	  eqn eqn = &(problem_ptr->subs[e]);
	  k = eqn->coef[i];
	  if (k != 0)
	    {
	      k = c * k;
	      eqn->coef[i] = 0;
	      for (j = topVar; j >= 0; j--)
		{
		  j0 = packing[j];
		  eqn->coef[j0] -= sub->coef[j0] * k;
		}
	    }

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "%s := ", orgVariable (eqn->key));
	      printTerm (dump_file, problem_ptr, eqn, 1);
	      fprintf (dump_file, "\n");
	    }
	}

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "---\n\n");
	  omega_print_problem (dump_file, problem_ptr);
	  fprintf (dump_file, "===\n\n");
	}
      if (i <= problem_ptr->safe_vars && problem_ptr->var[i] >= 0)
	{
	  eqn eqn;
	  eqn = &(problem_ptr->subs[problem_ptr->num_subs++]);
	  c = -c;
	  for (k = problem_ptr->num_vars; k >= 0; k--)
	    eqn->coef[k] = c * (sub->coef[k]);
	  eqn->key = problem_ptr->var[i];
	  eqn->color = sub->color;
	}
    }

}

static void
substituteRed (omega_problem *problem_ptr, eqn sub, int i, int c, bool * foundBlack)
{
  int e, j, j0, k;
  int topVar;

  {
    int *p = &packing[0];
    for (k = problem_ptr->num_vars; k >= 0; k--)
      if (sub->coef[k])
	*(p++) = k;
    topVar = (p - &packing[0]) - 1;
  }

  *foundBlack = 0;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      if (sub->color)
	fprintf (dump_file, "[");
      fprintf (dump_file, "substituting using %s := ", variable (i));
      printTerm (dump_file, problem_ptr, sub, -c);
      if (sub->color)
	fprintf (dump_file, "]");
      fprintf (dump_file, "\n");
      printVars (dump_file, problem_ptr);
    }

  for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
    {
      eqn eqn = &(problem_ptr->eqs[e]);
      k = eqn->coef[i];
      if (k != 0)
	{
	  if (!eqn->color)
	    *foundBlack = 1;
	  else
	    {
	      k = c * k;
	      eqn->coef[i] = 0;
	      for (j = topVar; j >= 0; j--)
		{
		  j0 = packing[j];
		  eqn->coef[j0] -= sub->coef[j0] * k;
		}
	    }
	}

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  omega_print_eq (dump_file, problem_ptr, eqn);
	  fprintf (dump_file, "\n");
	}
    }
  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    {
      eqn eqn = &(problem_ptr->geqs[e]);
      k = eqn->coef[i];
      if (k != 0)
	{
	  if (!eqn->color)
	    *foundBlack = 1;
	  else
	    {
	      k = c * k;
	      eqn->touched = TRUE;
	      eqn->coef[i] = 0;
	      for (j = topVar; j >= 0; j--)
		{
		  j0 = packing[j];
		  eqn->coef[j0] -= sub->coef[j0] * k;
		}
	    }
	}

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  omega_print_geq (dump_file, problem_ptr, eqn);
	  fprintf (dump_file, "\n");
	}
    }
  for (e = problem_ptr->num_subs - 1; e >= 0; e--)
    {
      eqn eqn = &(problem_ptr->subs[e]);
      k = eqn->coef[i];
      if (k != 0)
	{
	  if (!eqn->color)
	    *foundBlack = 1;
	  else
	    {
	      k = c * k;
	      eqn->coef[i] = 0;
	      for (j = topVar; j >= 0; j--)
		{
		  j0 = packing[j];
		  eqn->coef[j0] -= sub->coef[j0] * k;
		}
	    }
	}

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "%s := ", orgVariable (eqn->key));
	  printTerm (dump_file, problem_ptr, eqn, 1);
	  fprintf (dump_file, "\n");
	}
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "---\n\n");

  if (i <= problem_ptr->safe_vars && problem_ptr->var[i] >= 0)
    {
      *foundBlack = 1;
    }

}



void
deleteVariable (omega_problem *problem_ptr, int i)
{
  int nV = problem_ptr->num_vars;
  int e;
  if (i < problem_ptr->safe_vars)
    {
      int j = problem_ptr->safe_vars;
      for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	{
	  problem_ptr->geqs[e].touched = TRUE;
	  problem_ptr->geqs[e].coef[i] = problem_ptr->geqs[e].coef[j];
	  problem_ptr->geqs[e].coef[j] = problem_ptr->geqs[e].coef[nV];
	}
      for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
	{
	  problem_ptr->eqs[e].coef[i] = problem_ptr->eqs[e].coef[j];
	  problem_ptr->eqs[e].coef[j] = problem_ptr->eqs[e].coef[nV];
	}
      for (e = problem_ptr->num_subs - 1; e >= 0; e--)
	{
	  problem_ptr->subs[e].coef[i] = problem_ptr->subs[e].coef[j];
	  problem_ptr->subs[e].coef[j] = problem_ptr->subs[e].coef[nV];
	}
      problem_ptr->var[i] = problem_ptr->var[j];
      problem_ptr->var[j] = problem_ptr->var[nV];
    }
  else if (i < nV)
    {
      for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	if (problem_ptr->geqs[e].coef[nV])
	  {
	    problem_ptr->geqs[e].coef[i] = problem_ptr->geqs[e].coef[nV];
	    problem_ptr->geqs[e].touched = TRUE;
	  }
      for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
	problem_ptr->eqs[e].coef[i] = problem_ptr->eqs[e].coef[nV];
      for (e = problem_ptr->num_subs - 1; e >= 0; e--)
	problem_ptr->subs[e].coef[i] = problem_ptr->subs[e].coef[nV];
      problem_ptr->var[i] = problem_ptr->var[nV];
    }
  if (i <= problem_ptr->safe_vars)
    problem_ptr->safe_vars--;
  problem_ptr->num_vars--;
}

void
omega_convert_eq_to_geqs (omega_problem *problem_ptr, int eq)
{
  int i;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Converting Eq to Gproblem_ptr->eqs\n");
  eqncpy (&problem_ptr->geqs[problem_ptr->num_geqs], &problem_ptr->eqs[eq]);
  problem_ptr->geqs[problem_ptr->num_geqs].touched = 1;
  problem_ptr->num_geqs++;
  eqncpy (&problem_ptr->geqs[problem_ptr->num_geqs], &problem_ptr->eqs[eq]);
  problem_ptr->geqs[problem_ptr->num_geqs].touched = 1;
  for (i = 0; i <= problem_ptr->num_vars; i++)
    problem_ptr->geqs[problem_ptr->num_geqs].coef[i] = -problem_ptr->geqs[problem_ptr->num_geqs].coef[i];
  problem_ptr->num_geqs++;

  if (dump_file && (dump_flags & TDF_DETAILS))
    omega_print_problem (dump_file, problem_ptr);
}


static void
doElimination (omega_problem *problem_ptr, int e, int i)
{
  struct eqn sub;
  int c;
  int nV = problem_ptr->num_vars;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "eliminating variable %s\n", variable (i));

  eqncpy (&sub, &problem_ptr->eqs[e]);
  c = sub.coef[i];
  sub.coef[i] = 0;
  if (c == 1 || c == -1)
    {
      if (problem_ptr->eqs[e].color)
	{
	  bool fB;
	  substituteRed (problem_ptr, &sub, i, c, &fB);
	  if (fB)
	    omega_convert_eq_to_geqs (problem_ptr, e);
	  else
	    deleteVariable (problem_ptr, i);
	}
      else
	{
	  substitute (problem_ptr, &sub, i, c);
	  deleteVariable (problem_ptr, i);
	}
    }
  else
    {
      int a = abs (c);
      int e2 = e;

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "performing non-exact elimination, c = %d\n", c);

      for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
	if (problem_ptr->eqs[e].coef[i])
	  {
	    eqn eqn = &(problem_ptr->eqs[e]);
	    int j, k;
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] *= a;
	    k = eqn->coef[i];
	    eqn->coef[i] = 0;
	    eqn->color |= sub.color;
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] -= sub.coef[j] * k / c;
	  }
      for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	if (problem_ptr->geqs[e].coef[i])
	  {
	    eqn eqn = &(problem_ptr->geqs[e]);
	    int j, k;
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] *= a;
	    k = eqn->coef[i];
	    eqn->coef[i] = 0;
	    eqn->color |= sub.color;
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] -= sub.coef[j] * k / c;
	    eqn->touched = 1;
	  }
      for (e = problem_ptr->num_subs - 1; e >= 0; e--)
	if (problem_ptr->subs[e].coef[i])
	  {
	    eqn eqn = &(problem_ptr->subs[e]);
	    int j, k;
	    gcc_assert (0);
	    gcc_assert (!sub.color);
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] *= a;
	    k = eqn->coef[i];
	    eqn->coef[i] = 0;
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] -= sub.coef[j] * k / c;
	  }
      if (inApproximateMode)
	deleteVariable (problem_ptr, i);
      else
	omega_convert_eq_to_geqs (problem_ptr, e2);
    }
}

static int
solveEQ (omega_problem *problem_ptr, int desired_result)
{
  int i, j, e;
  int g, g2;
  g = 0;


  if (dump_file && (dump_flags & TDF_DETAILS) && problem_ptr->num_eqs > 0)
    {
      fprintf (dump_file, "\nSolveEQ(%d,%d)\n", desired_result, mayBeRed);
      omega_print_problem (dump_file, problem_ptr);
      fprintf (dump_file, "\n");
    }

  if (mayBeRed)
    {
      i = 0;
      j = problem_ptr->num_eqs - 1;
      while (1)
	{
	  struct eqn eq;
	  while (i <= j && problem_ptr->eqs[i].color)
	    i++;
	  while (i <= j && !problem_ptr->eqs[j].color)
	    j--;
	  if (i >= j)
	    break;
	  eqncpy (&eq, &problem_ptr->eqs[i]);
	  eqncpy (&problem_ptr->eqs[i], &problem_ptr->eqs[j]);
	  eqncpy (&problem_ptr->eqs[j], &eq);
	  i++;
	  j--;
	}
    }



  /* Eliminate all EQ equations */
  for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
    {
      eqn eqn = &(problem_ptr->eqs[e]);
      int sv;

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "----\n");

      for (i = problem_ptr->num_vars; i > 0; i--)
	if ((g = eqn->coef[i]) != 0)
	  break;

      for (j = i - 1; j > 0; j--)
	if (eqn->coef[j])
	  break;
      if (j == 0)
	{
	  if (eqn->coef[0] % g != 0)
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  omega_print_eq (dump_file, problem_ptr, eqn);
		  fprintf (dump_file, "\nequations have no solution \n");
		}

	      return FALSE;
	    }
	  eqn->coef[0] = eqn->coef[0] / g;
	  eqn->coef[i] = 1;
	  problem_ptr->num_eqs--;
	  doElimination (problem_ptr, e, i);
	  continue;
	}
      else if (j == -1)
	{
	  if (eqn->coef[0] != 0)
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  omega_print_eq (dump_file, problem_ptr, eqn);
		  fprintf (dump_file, "\nequations have no solution \n");
		}

	      return FALSE;
	    }
	  problem_ptr->num_eqs--;
	  continue;
	}
      /* i == position of last non-zero coef */
      /* g == coef of i */
      /* j == position of next non-zero coef */

      if (g < 0)
	g = -g;
      if (g == 1)
	{
	  problem_ptr->num_eqs--;
	  doElimination (problem_ptr, e, i);
	}
      else
	{
	  int k = j;
	  bool promotionPossible =
	    (j <= problem_ptr->safe_vars && problem_ptr->safe_vars + 1 == i
	     && !isRed (eqn) && !inApproximateMode);
	  if (dump_file && (dump_flags & TDF_DETAILS) && promotionPossible)
	    fprintf (dump_file, " Promotion possible\n");
	normalizeEQ:
	  if (j > problem_ptr->safe_vars)
	    {
	      for (; g != 1 && j > problem_ptr->safe_vars; j--)
		g = gcd (abs (eqn->coef[j]), g);
	      g2 = g;
	    }
	  else if (i > problem_ptr->safe_vars)
	    g2 = g;
	  else
	    g2 = 0;
	  for (; g != 1 && j > 0; j--)
	    g = gcd (abs (eqn->coef[j]), g);
	  if (g > 1)
	    {
	      if (eqn->coef[0] % g != 0)
		{
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    {
		      omega_print_eq (dump_file, problem_ptr, eqn);
		      fprintf (dump_file, "\nequations have no solution \n");
		    }
		  return FALSE;
		}

	      for (j = 0; j <= problem_ptr->num_vars; j++)
		eqn->coef[j] /= g;
	      g2 = g2 / g;
	    }
	  if (g2 > 1)
	    {
	      int e2;
	      for (e2 = e - 1; e2 >= 0; e2--)
		if (problem_ptr->eqs[e2].coef[i])
		  break;
	      if (e2 == -1)
		for (e2 = problem_ptr->num_geqs - 1; e2 >= 0; e2--)
		  if (problem_ptr->geqs[e2].coef[i])
		    break;
	      if (e2 == -1)
		for (e2 = problem_ptr->num_subs - 1; e2 >= 0; e2--)
		  if (problem_ptr->subs[e2].coef[i])
		    break;
	      if (e2 == -1)
		{
		  bool change = 0;

		  if (dump_file && (dump_flags & TDF_DETAILS))
		    {
		      fprintf (dump_file, "Ha! We own it! \n");
		      omega_print_eq (dump_file, problem_ptr, eqn);
		      fprintf (dump_file, " \n");
		    }

		  g = eqn->coef[i];
		  g = abs (g);
		  for (j = i - 1; j >= 0; j--)
		    {
		      int t = eqn->coef[j];
		      t = int_mod (t, g);
		      if (2 * t >= g)
			t -= g;
		      if (t != eqn->coef[j])
			{
			  eqn->coef[j] = t;
			  change = 1;
			}
		    }
		  if (!change)
		    {
		      if (dump_file && (dump_flags & TDF_DETAILS))
			fprintf (dump_file, "So what?\n");
		    }
		  else
		    {
		      nameWildcard (problem_ptr, i);

		      if (dump_file && (dump_flags & TDF_DETAILS))
			{
			  omega_print_eq (dump_file, problem_ptr, eqn);
			  fprintf (dump_file, " \n");
			}

		      e++;
		      continue;
		    }
		}
	    }
	  if (promotionPossible)
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  fprintf (dump_file, "promoting %s to safety\n",
			   variable (i));
		  printVars (dump_file, problem_ptr);
		}
	      problem_ptr->safe_vars++;
	      if (problem_ptr->var[i] > 0)
		nameWildcard (problem_ptr, i);
	      promotionPossible = 0;
	      j = k;
	      goto normalizeEQ;
	    }

	  if (g2 > 1 && !inApproximateMode)
	    {
	      if (problem_ptr->eqs[e].color)
		{
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    fprintf (dump_file, "handling red equality\n");

		  problem_ptr->num_eqs--;
		  doElimination (problem_ptr, e, i);
		  continue;
		}
	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  fprintf (dump_file,
			   "adding equation to handle safe variable \n");
		  omega_print_eq (dump_file, problem_ptr, eqn);
		  fprintf (dump_file, "\n----\n");
		  omega_print_problem (dump_file, problem_ptr);
		  fprintf (dump_file, "\n----\n");
		  fprintf (dump_file, "\n----\n");
		}

	      i = addNewWildcard (problem_ptr);
	      problem_ptr->num_eqs++;
	      gcc_assert (problem_ptr->num_eqs <= max_eqs);
	      eqnzero (&problem_ptr->eqs[e + 1]);
	      eqnncpy (&problem_ptr->eqs[e + 1], eqn, problem_ptr->safe_vars);
	      for (j = problem_ptr->num_vars; j >= 0; j--)
		{
		  problem_ptr->eqs[e + 1].coef[j] = int_mod (problem_ptr->eqs[e + 1].coef[j], g2);
		  if (2 * problem_ptr->eqs[e + 1].coef[j] >= g2)
		    problem_ptr->eqs[e + 1].coef[j] -= g2;
		}
	      problem_ptr->eqs[e + 1].coef[i] = g2;
	      e += 2;

	      if (dump_file && (dump_flags & TDF_DETAILS))
		omega_print_problem (dump_file, problem_ptr);

	      continue;
	    }

	  sv = problem_ptr->safe_vars;
	  if (g2 == 0)
	    sv = 0;

	  /* find variable to eliminate */
	  if (g2 > 1)
	    {
	      gcc_assert (inApproximateMode);

	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  fprintf (dump_file, "non-exact elimination: ");
		  omega_print_eq (dump_file, problem_ptr, eqn);
		  fprintf (dump_file, "\n");
		  omega_print_problem (dump_file, problem_ptr);
		}

	      for (i = problem_ptr->num_vars; i > sv; i--)
		if (problem_ptr->eqs[e].coef[i] != 0)
		  break;
	    }
	  else
	    for (i = problem_ptr->num_vars; i > sv; i--)
	      if (problem_ptr->eqs[e].coef[i] == 1 || problem_ptr->eqs[e].coef[i] == -1)
		break;

	  if (i > sv)
	    {
	      problem_ptr->num_eqs--;
	      doElimination (problem_ptr, e, i);

	      if (dump_file && (dump_flags & TDF_DETAILS) && g2 > 1)
		{
		  fprintf (dump_file, "result of non-exact elimination:\n");
		  omega_print_problem (dump_file, problem_ptr);
		}
	    }
	  else
	    {
	      int factor = (INT_MAX);
	      j = 0;

	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "doing moding\n");

	      for (i = problem_ptr->num_vars; i != sv; i--)
		if ((problem_ptr->eqs[e].coef[i] & 1) != 0)
		  {
		    j = i;
		    i--;
		    for (; i != sv; i--)
		      if ((problem_ptr->eqs[e].coef[i] & 1) != 0)
			break;
		    break;
		  }
	      if (j != 0 && i == sv)
		{
		  doMod (problem_ptr, 2, e, j);
		  e++;
		  continue;
		}

	      j = 0;
	      for (i = problem_ptr->num_vars; i != sv; i--)
		if (problem_ptr->eqs[e].coef[i] != 0 && factor > abs (problem_ptr->eqs[e].coef[i]) + 1)
		  {
		    factor = abs (problem_ptr->eqs[e].coef[i]) + 1;
		    j = i;
		  }
	      if (j == sv)
		{
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    fprintf (dump_file, "should not have happened\n");
		  exit (2);
		}
	      doMod (problem_ptr, factor, e, j);
	      /* go back and try this equation again */
	      e++;
	    }
	}


    }
  problem_ptr->num_eqs = 0;
  return UNKNOWN;
}


static int solveGEQ (omega_problem *problem_ptr, int desired_result);

static int solveDepth = 0;

int
omega_solve_problem (omega_problem *problem_ptr, int desired_result)
{
  int result;

  gcc_assert (problem_ptr->num_vars >= problem_ptr->safe_vars);
  if (desired_result != SIMPLIFY)
    problem_ptr->safe_vars = 0;

  solveDepth++;
  if (solveDepth > 50)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Solve depth = %d, inApprox = %d, aborting\n",
		   solveDepth, inApproximateMode);
	  omega_print_problem (dump_file, problem_ptr);
	}
      exit (2);
    }

  do
    {
      doItAgain = 0;
      if (solveEQ (problem_ptr, desired_result) == FALSE)
	{
	  solveDepth--;
	  return FALSE;
	}
      if (inApproximateMode && !problem_ptr->num_geqs)
	{
	  result = TRUE;
	  problem_ptr->num_vars = problem_ptr->safe_vars;
	  omega_problem_reduced (problem_ptr);
	  break;
	}
      else
	result = solveGEQ (problem_ptr, desired_result);
    }
  while (doItAgain && desired_result == SIMPLIFY);
  solveDepth--;

  if (reduceWithSubs)
    {
    }
  else
    {
      resurrectSubs (problem_ptr);
      gcc_assert (please_no_equalities_in_simplified_problems || !result || problem_ptr->num_subs == 0);
    }

  return result;
}

static int
parallel_splinter (omega_problem *problem_ptr, int e, int diff,
		   int desired_result)
{
  omega_problem *tmp_problem;
  int i;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Using parallel splintering\n");
      omega_print_problem (dump_file, problem_ptr);
    }

  tmp_problem = (omega_problem *) xmalloc (sizeof (omega_problem));
  eqncpy (&problem_ptr->eqs[0], &problem_ptr->geqs[e]);
  problem_ptr->num_eqs = 1;

  for (i = 0; i <= diff; i++)
    {
      omega_copy_problem (tmp_problem, problem_ptr);

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Splinter # %i\n", i);
	  omega_print_problem (dump_file, problem_ptr);
	}

      if (omega_solve_problem (tmp_problem, desired_result))
	{
	  free (tmp_problem);
	  return TRUE;
	}
      problem_ptr->eqs[0].coef[0]--;
    }
  free (tmp_problem);
  return FALSE;
}

static int
solveGEQ (omega_problem *problem_ptr, int desired_result)
{
  int i, j, k, e;
  int nV, fv;
  int result;
  int coupled_subscripts = 0;
  int eliminate_again;
  int smoothed = 0;
  int tried_eliminating_redundant = 0;
  j = 0;

  if (desired_result != SIMPLIFY)
    {
      problem_ptr->num_subs = 0;
      problem_ptr->safe_vars = 0;
    }

 solve_geq_start:
  while (1)
    {
      gcc_assert (desired_result == SIMPLIFY || problem_ptr->num_subs == 0);
      if (problem_ptr->num_geqs > max_geqs)
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "TOO MANY EQUATIONS GENERATED\n");
	  exit (2);
	}
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\nSolveGEQ(%d,%d):\n",
		   desired_result, please_no_equalities_in_simplified_problems);
	  omega_print_problem (dump_file, problem_ptr);
	  fprintf (dump_file, "\n");
	}

      nV = problem_ptr->num_vars;

      if (nV == 1)
	{
	  int u_color = black;
	  int l_color = black;
	  int upper_bound = pos_infinity;
	  int lower_bound = neg_infinity;
	  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	    {
	      int a = problem_ptr->geqs[e].coef[1];
	      int c = problem_ptr->geqs[e].coef[0];
	      /* our equation is ax + c >= 0, or ax >= -c, or c >= -ax */
	      if (a == 0)
		{
		  if (c < 0)
		    {
		      if (dump_file && (dump_flags & TDF_DETAILS))
			fprintf (dump_file, "equations have no solution \n");
		      return FALSE;
		    }
		}
	      else if (a > 0)
		{
		  if (a != 1)
		    c = int_div (c, a);
		  if (lower_bound < -c ||
		      (lower_bound == -c && !isRed (&problem_ptr->geqs[e])))
		    {
		      lower_bound = -c;
		      l_color = problem_ptr->geqs[e].color;
		    }
		}
	      else
		{
		  if (a != -1)
		    c = int_div (c, -a);
		  if (upper_bound > c ||
		      (upper_bound == c && !isRed (&problem_ptr->geqs[e])))
		    {
		      upper_bound = c;
		      u_color = problem_ptr->geqs[e].color;
		    }
		}
	    }
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "upper bound = %d\n", upper_bound);
	      fprintf (dump_file, "lower bound = %d\n", lower_bound);
	    }

	  if (lower_bound > upper_bound)
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "equations have no solution \n");
	      return FALSE;
	    }
	  if (desired_result == SIMPLIFY)
	    {
	      problem_ptr->num_geqs = 0;
	      if (problem_ptr->safe_vars == 1)
		{

		  if (lower_bound == upper_bound && !u_color && !l_color)
		    {
		      problem_ptr->eqs[0].coef[0] = -lower_bound;
		      problem_ptr->eqs[0].coef[1] = 1;
		      problem_ptr->eqs[0].color = 0;
		      problem_ptr->num_eqs = 1;
		      return omega_solve_problem (problem_ptr, desired_result);
		    }
		  else
		    {
		      if (lower_bound > neg_infinity)
			{
			  problem_ptr->geqs[0].coef[0] = -lower_bound;
			  problem_ptr->geqs[0].coef[1] = 1;
			  problem_ptr->geqs[0].key = 1;
			  problem_ptr->geqs[0].color = l_color;
			  problem_ptr->geqs[0].touched = 0;
			  problem_ptr->num_geqs = 1;
			}
		      if (upper_bound < pos_infinity)
			{
			  problem_ptr->geqs[problem_ptr->num_geqs].coef[0] = upper_bound;
			  problem_ptr->geqs[problem_ptr->num_geqs].coef[1] = -1;
			  problem_ptr->geqs[problem_ptr->num_geqs].key = -1;
			  problem_ptr->geqs[problem_ptr->num_geqs].color = u_color;
			  problem_ptr->geqs[problem_ptr->num_geqs].touched = 0;
			  problem_ptr->num_geqs++;
			}
		    }
		}
	      else
		problem_ptr->num_vars = 0;
	      omega_problem_reduced (problem_ptr);
	      return FALSE;
	    }
	  if (original_problem != no_problem && !l_color && !u_color
	      && !conservative && lower_bound == upper_bound)
	    {
	      problem_ptr->eqs[0].coef[0] = -lower_bound;
	      problem_ptr->eqs[0].coef[1] = 1;
	      problem_ptr->num_eqs = 1;
	      adding_equality_constraint (problem_ptr, 0);
	    }
	  return TRUE;
	}

      if (!problem_ptr->variablesFreed)
	{
	  problem_ptr->variablesFreed = 1;
	  if (desired_result != SIMPLIFY)
	    freeEliminations (problem_ptr, 0);
	  else
	    freeEliminations (problem_ptr, problem_ptr->safe_vars);
	  nV = problem_ptr->num_vars;
	  if (nV == 1)
	    continue;
	}


      switch (normalize_omega_problem (problem_ptr))
	{
	case normalize_false:
	  return FALSE;
	  break;
	case normalize_coupled:
	  coupled_subscripts = TRUE;
	  break;
	case normalize_uncoupled:
	  coupled_subscripts = FALSE;
	  break;
	}

      nV = problem_ptr->num_vars;

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\nafter normalization:\n");
	  omega_print_problem (dump_file, problem_ptr);
	  fprintf (dump_file, "\n");
	  fprintf (dump_file,
		   "eliminating variable using fourier-motzkin elimination\n");
	}

      do
	{
	  eliminate_again = 0;

	  if (problem_ptr->num_eqs > 0)
	    return omega_solve_problem (problem_ptr, desired_result);

	  if (!coupled_subscripts)
	    {
	      if (problem_ptr->safe_vars == 0)
		problem_ptr->num_geqs = 0;
	      else
		for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
		  if (problem_ptr->geqs[e].key > problem_ptr->safe_vars || -problem_ptr->safe_vars > problem_ptr->geqs[e].key)
		    do_delete (problem_ptr, e, nV);
	      problem_ptr->num_vars = problem_ptr->safe_vars;
	      if (desired_result == SIMPLIFY)
		{
		  omega_problem_reduced (problem_ptr);
		  return FALSE;
		}
	      return TRUE;
	    }

	  if (desired_result != SIMPLIFY)
	    fv = 0;
	  else
	    fv = problem_ptr->safe_vars;

	  if (problem_ptr->num_geqs == 0)
	    {
	      if (desired_result == SIMPLIFY)
		{
		  problem_ptr->num_vars = problem_ptr->safe_vars;
		  omega_problem_reduced (problem_ptr);
		  return FALSE;
		}
	      return TRUE;
	    }
	  if (desired_result == SIMPLIFY && nV == problem_ptr->safe_vars)
	    {
	      omega_problem_reduced (problem_ptr);
	      return FALSE;
	    }


	  if (problem_ptr->num_geqs > max_geqs - 30 || problem_ptr->num_geqs > 2 * nV * nV + 4 * nV + 10)
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file,
			 "TOO MANY EQUATIONS; %d equations, %d variables, ELIMINATING REDUNDANT ONES\n",
			 problem_ptr->num_geqs, nV);
	      if (!omega_eliminate_redundant (problem_ptr, 0))
		return 0;
	      nV = problem_ptr->num_vars;
	      if (problem_ptr->num_eqs > 0)
		return omega_solve_problem (problem_ptr, desired_result);
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file,
			 "END ELIMINATION OF REDUNDANT EQUATIONS\n");
	    }



	  {
	    int parallel_difference = (INT_MAX);
	    int best_parallel_eqn = -1;
	    int minC, maxC, minCj;
	    int lower_bound_count;
	    int e2, Le, Ue;
	    int possibleEasyIntSolution;
	    int max_splinters = 1;
	    int exact = 0;
	    int luckyExact = 0;
	    int neweqns = 0;
	    minCj = 0;
	    Le = 0;
	    lower_bound_count = 0;

	    if (desired_result != SIMPLIFY)
	      fv = 0;
	    else
	      fv = problem_ptr->safe_vars;

	    {
	      int best = (INT_MAX);
	      int j = 0, jLe, jLowerBoundCount, upper_bound_count;

	      jLe = 0;
	      jLowerBoundCount = 0;


	      for (i = nV; i != fv; i--)
		{
		  int score;
		  int ub = -2;
		  int lb = -2;
		  int lucky = 0;
		  minC = maxC = 0;
		  lower_bound_count = 0;
		  upper_bound_count = 0;
		  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
		    if (problem_ptr->geqs[e].coef[i] < 0)
		      {
			setMin (&minC, problem_ptr->geqs[e].coef[i]);
			upper_bound_count++;
			if (problem_ptr->geqs[e].coef[i] < -1)
			  {
			    if (ub == -2)
			      ub = e;
			    else
			      ub = -1;
			  }
		      }
		    else if (problem_ptr->geqs[e].coef[i] > 0)
		      {
			setMax (&maxC, problem_ptr->geqs[e].coef[i]);
			lower_bound_count++;
			Le = e;
			if (problem_ptr->geqs[e].coef[i] > 1)
			  {
			    if (lb == -2)
			      lb = e;
			    else
			      lb = -1;
			  }
		      }
		  if (lower_bound_count == 0 || upper_bound_count == 0)
		    {
		      lower_bound_count = 0;
		      break;
		    }
		  if (ub >= 0 && lb >= 0 && problem_ptr->geqs[lb].key == -problem_ptr->geqs[ub].key)
		    {
		      int Lc = problem_ptr->geqs[lb].coef[i];
		      int Uc = -problem_ptr->geqs[ub].coef[i];
		      int diff =
			Lc * problem_ptr->geqs[ub].coef[0] + Uc * problem_ptr->geqs[lb].coef[0];
		      lucky = (diff >= (Uc - 1) * (Lc - 1));
		    }
		  if (maxC == 1 || minC == -1 || lucky || APROX
		      || inApproximateMode)
		    {
		      neweqns = score = upper_bound_count * lower_bound_count;
		      if (dump_file && (dump_flags & TDF_DETAILS))
			fprintf (dump_file,
				 "For %s, exact, score = %d*%d, range = %d ... %d, \nlucky = %d, APROX = %d, inApproximateMode=%d \n",
				 variable (i), upper_bound_count,
				 lower_bound_count, minC, maxC, lucky, APROX,
				 inApproximateMode);
		      if (!exact || score < best)
			{

			  best = score;
			  j = i;
			  minCj = minC;
			  jLe = Le;
			  jLowerBoundCount = lower_bound_count;
			  exact = 1;
			  luckyExact = lucky;
			  if (score == 1)
			    break;
			}
		    }
		  else if (!exact)
		    {
		      if (dump_file && (dump_flags & TDF_DETAILS))
			fprintf (dump_file,
				 "For %s, non-exact, score = %d*%d, range = %d ... %d \n",
				 variable (i), upper_bound_count,
				 lower_bound_count, minC, maxC);
		      neweqns = upper_bound_count * lower_bound_count;
		      score = maxC - minC;
		      if (best > score)
			{
			  best = score;
			  j = i;
			  minCj = minC;
			  jLe = Le;
			  jLowerBoundCount = lower_bound_count;
			}
		    }
		}
	      if (lower_bound_count == 0)
		{
		  freeEliminations (problem_ptr, problem_ptr->safe_vars);
		  nV = problem_ptr->num_vars;
		  eliminate_again = 1;
		  continue;
		}
	      i = j;
	      minC = minCj;
	      Le = jLe;
	      lower_bound_count = jLowerBoundCount;
	      for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
		if (problem_ptr->geqs[e].coef[i] > 0)
		  {
		    if (problem_ptr->geqs[e].coef[i] == -minC)
		      max_splinters += -minC - 1;
		    else
		      max_splinters +=
			check_pos_mul ((problem_ptr->geqs[e].coef[i] - 1),
				       (-minC - 1)) / (-minC) + 1;
		  }
	    }

#ifdef Omega3
	    if (!exact && !tried_eliminating_redundant)
	      {
		if (dump_file && (dump_flags & TDF_DETAILS))
		  fprintf (dump_file,
			   "Trying to produce exact elimination by finding redundant constraints\n");
		omega_eliminate_redundant (problem_ptr, 0);
		if (dump_file && (dump_flags & TDF_DETAILS))
		  fprintf (dump_file,
			   "Done trying to produce exact elimination by finding redundant constraints\n");
		tried_eliminating_redundant = 1;
		eliminate_again = 1;
		continue;
	      }
	    tried_eliminating_redundant = 0;
#endif

	    if (returnSingleResult && desired_result == SIMPLIFY && !exact)
	      {
		nonConvex = 1;
		omega_problem_reduced (problem_ptr);
		return TRUE;
	      }

#ifndef Omega3
	    if (!exact && !tried_eliminating_redundant)
	      {
		if (dump_file && (dump_flags & TDF_DETAILS))
		  fprintf (dump_file,
			   "Trying to produce exact elimination by finding redundant constraints\n");
		omega_eliminate_redundant (problem_ptr, 0);
		if (dump_file && (dump_flags & TDF_DETAILS))
		  fprintf (dump_file,
			   "Done trying to produce exact elimination by finding redundant constraints\n");
		tried_eliminating_redundant = 1;
		continue;
	      }
	    tried_eliminating_redundant = 0;
#endif

	    if (!exact)
	      {
		int e1, e2;

		for (e1 = problem_ptr->num_geqs - 1; e1 >= 0; e1--)
		  if (!problem_ptr->geqs[e1].color)
		    for (e2 = e1 - 1; e2 >= 0; e2--)
		      if (!problem_ptr->geqs[e2].color
			  && problem_ptr->geqs[e1].key == -problem_ptr->geqs[e2].key
			  && ((problem_ptr->geqs[e1].coef[0] + problem_ptr->geqs[e2].coef[0])
			      * (3 - single_var_geq (problem_ptr->geqs[e1], problem_ptr->num_vars))
			      / 2 < parallel_difference))
			{
			  parallel_difference =
			    (problem_ptr->geqs[e1].coef[0] + problem_ptr->geqs[e2].coef[0])
			    * (3 - single_var_geq (problem_ptr->geqs[e1], problem_ptr->num_vars))
			    / 2;
			  best_parallel_eqn = e1;
			}

		if (dump_file && (dump_flags & TDF_DETAILS)
		    && best_parallel_eqn >= 0)
		  {
		    fprintf (dump_file,
			     "Possible parallel projection, diff = %d, in ",
			     parallel_difference);
		    omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[best_parallel_eqn]));
		    fprintf (dump_file, "\n");
		    omega_print_problem (dump_file, problem_ptr);
		  }
	      }

	    if (dump_file && (dump_flags & TDF_DETAILS))
	      {
		fprintf (dump_file, "going to eliminate %s, (%d,%d,%d)\n",
			 variable (i), i, minC, lower_bound_count);
		if (DEBUG)
		  omega_print_problem (dump_file, problem_ptr);
		if (luckyExact)
		  fprintf (dump_file, "(a lucky exact elimination)\n");
		else if (exact)
		  fprintf (dump_file, "(an exact elimination)\n");
		fprintf (dump_file, "Max # of splinters = %d\n",
			 max_splinters);
	      }
	    gcc_assert (max_splinters >= 1);


	    if (!exact && desired_result == SIMPLIFY && best_parallel_eqn >= 0
		&& parallel_difference <= max_splinters)
	      {
		return parallel_splinter (problem_ptr, best_parallel_eqn,
					  parallel_difference, desired_result);
	      }
	    smoothed = 0;

	    if (i != nV)
	      {
		int t;
		j = problem_ptr->num_vars;

		if (dump_file && (dump_flags & TDF_DETAILS))
		  {
		    fprintf (dump_file, "Swapping %d and %d\n", i, j);
		    omega_print_problem (dump_file, problem_ptr);
		  }
		swap (&problem_ptr->var[i], &problem_ptr->var[j]);
		for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
		  if (problem_ptr->geqs[e].coef[i] != problem_ptr->geqs[e].coef[j])
		    {
		      problem_ptr->geqs[e].touched = TRUE;
		      t = problem_ptr->geqs[e].coef[i];
		      problem_ptr->geqs[e].coef[i] = problem_ptr->geqs[e].coef[j];
		      problem_ptr->geqs[e].coef[j] = t;
		    }
		for (e = problem_ptr->num_subs - 1; e >= 0; e--)
		  if (problem_ptr->subs[e].coef[i] != problem_ptr->subs[e].coef[j])
		    {
		      t = problem_ptr->subs[e].coef[i];
		      problem_ptr->subs[e].coef[i] = problem_ptr->subs[e].coef[j];
		      problem_ptr->subs[e].coef[j] = t;
		    }
		if (dump_file && (dump_flags & TDF_DETAILS))
		  {
		    fprintf (dump_file, "Swapping complete \n");
		    omega_print_problem (dump_file, problem_ptr);
		    fprintf (dump_file, "\n");
		  }

		i = j;
	      }
	    else if (dump_file && (dump_flags & TDF_DETAILS))
	      {
		fprintf (dump_file, "No swap needed\n");
		omega_print_problem (dump_file, problem_ptr);
	      }
	    problem_ptr->num_vars--;
	    nV = problem_ptr->num_vars;

	    if (exact)
	      {
#define maxDead max_geqs
		if (nV == 1)
		  {
		    int upper_bound = pos_infinity;
		    int lower_bound = neg_infinity;
		    int ub_color = 0;
		    int lb_color = 0;
		    int constantTerm, coefficient;
		    int topeqn = problem_ptr->num_geqs - 1;
		    int Lc = problem_ptr->geqs[Le].coef[i];
		    for (Le = topeqn; Le >= 0; Le--)
		      if ((Lc = problem_ptr->geqs[Le].coef[i]) == 0)
			{
			  if (problem_ptr->geqs[Le].coef[1] == 1)
			    {
			      constantTerm = -problem_ptr->geqs[Le].coef[0];
			      if (constantTerm > lower_bound ||
				  (constantTerm == lower_bound &&
				   !isRed (&problem_ptr->geqs[Le])))
				{
				  lower_bound = constantTerm;
				  lb_color = problem_ptr->geqs[Le].color;
				}

			      if (dump_file && (dump_flags & TDF_DETAILS))
				{
				  if (problem_ptr->geqs[Le].color == black)
				    fprintf (dump_file, " :::=> %s >= %d\n",
					     variable (1), constantTerm);
				  else
				    fprintf (dump_file,
					     " :::=> [%s >= %d]\n",
					     variable (1), constantTerm);
				}
			    }
			  else
			    {
			      constantTerm = problem_ptr->geqs[Le].coef[0];
			      if (constantTerm < upper_bound ||
				  (constantTerm == upper_bound
				   && !isRed (&problem_ptr->geqs[Le])))
				{
				  upper_bound = constantTerm;
				  ub_color = problem_ptr->geqs[Le].color;
				}

			      if (dump_file && (dump_flags & TDF_DETAILS))
				{
				  if (problem_ptr->geqs[Le].color == black)
				    fprintf (dump_file, " :::=> %s <= %d\n",
					     variable (1), constantTerm);
				  else
				    fprintf (dump_file,
					     " :::=> [%s <= %d]\n",
					     variable (1), constantTerm);
				}
			    }
			}
		      else if (Lc > 0)
			{
			  for (Ue = topeqn; Ue >= 0; Ue--)
			    if (problem_ptr->geqs[Ue].coef[i] < 0)
			      {
				if (problem_ptr->geqs[Le].key != -problem_ptr->geqs[Ue].key)
				  {
				    int Uc = -problem_ptr->geqs[Ue].coef[i];
				    coefficient =
				      problem_ptr->geqs[Ue].coef[1] * Lc +
				      problem_ptr->geqs[Le].coef[1] * Uc;
				    constantTerm =
				      problem_ptr->geqs[Ue].coef[0] * Lc +
				      problem_ptr->geqs[Le].coef[0] * Uc;

				    if (dump_file && (dump_flags & TDF_DETAILS))
				      {
					omega_print_geq_extra (dump_file, problem_ptr, &(problem_ptr->geqs[Ue]));
					fprintf (dump_file, "\n");
					omega_print_geq_extra (dump_file, problem_ptr, &(problem_ptr->geqs[Le]));
					fprintf (dump_file, "\n");
				      }

				    if (coefficient > 0)
				      {
					constantTerm =
					  -(int_div
					    (constantTerm, coefficient));
					/* gcc_assert(black == 0) */
					if (constantTerm > lower_bound ||
					    (constantTerm == lower_bound &&
					     (desired_result != SIMPLIFY ||
					      (problem_ptr->geqs[Ue].color == black
					       && problem_ptr->geqs[Le].color == black))))
					  {
					    lower_bound = constantTerm;
					    lb_color = problem_ptr->geqs[Ue].color
					      || problem_ptr->geqs[Le].color;
					  }

					if (dump_file && (dump_flags & TDF_DETAILS))
					  {
					    if (problem_ptr->geqs[Ue].color
						|| problem_ptr->geqs[Le].color)
					      fprintf (dump_file,
						       " ::=> [%s >= %d]\n",
						       variable (1),
						       constantTerm);
					    else
					      fprintf (dump_file,
						       " ::=> %s >= %d\n",
						       variable (1),
						       constantTerm);
					  }
				      }
				    else
				      {
					constantTerm =
					  int_div (constantTerm, -coefficient);
					if (constantTerm < upper_bound
					    || (constantTerm == upper_bound
						&& problem_ptr->geqs[Ue].color == black
						&& problem_ptr->geqs[Le].color == black))
					  {
					    upper_bound = constantTerm;
					    ub_color = problem_ptr->geqs[Ue].color
					      || problem_ptr->geqs[Le].color;
					  }

					if (dump_file && (dump_flags & TDF_DETAILS))
					  {
					    if (problem_ptr->geqs[Ue].color
						|| problem_ptr->geqs[Le].color)
					      fprintf (dump_file,
						       " ::=> [%s <= %d]\n",
						       variable (1),
						       constantTerm);
					    else
					      fprintf (dump_file,
						       " ::=> %s <= %d\n",
						       variable (1),
						       constantTerm);
					  }
				      }
				  }
			      }
			}
		    problem_ptr->num_geqs = 0;

		    if (dump_file && (dump_flags & TDF_DETAILS))
		      fprintf (dump_file,
			       " therefore, %c%d <= %c%s%c <= %d%c\n",
			       lb_color ? '[' : ' ', lower_bound,
			       (lb_color && !ub_color) ? ']' : ' ',
			       variable (1),
			       (!lb_color && ub_color) ? '[' : ' ',
			       upper_bound, ub_color ? ']' : ' ');
		    if (lower_bound > upper_bound)
		      return FALSE;
		    if (problem_ptr->safe_vars == 1)
		      {
			if (upper_bound == lower_bound
			    && !(ub_color | lb_color)
			    && !please_no_equalities_in_simplified_problems)
			  {
			    problem_ptr->num_eqs++;
			    problem_ptr->eqs[0].coef[1] = -1;
			    problem_ptr->eqs[0].coef[0] = upper_bound;
			    problem_ptr->eqs[0].color = ub_color | lb_color;
			    if (desired_result == SIMPLIFY && !problem_ptr->eqs[0].color)
			      return omega_solve_problem (problem_ptr, desired_result);
			  }
			if (upper_bound != pos_infinity)
			  {
			    problem_ptr->geqs[0].coef[1] = -1;
			    problem_ptr->geqs[0].coef[0] = upper_bound;
			    problem_ptr->geqs[0].color = ub_color;
			    problem_ptr->geqs[0].key = -1;
			    problem_ptr->geqs[0].touched = 0;
			    problem_ptr->num_geqs++;
			  }
			if (lower_bound != neg_infinity)
			  {
			    problem_ptr->geqs[problem_ptr->num_geqs].coef[1] = 1;
			    problem_ptr->geqs[problem_ptr->num_geqs].coef[0] = -lower_bound;
			    problem_ptr->geqs[problem_ptr->num_geqs].color = lb_color;
			    problem_ptr->geqs[problem_ptr->num_geqs].key = 1;
			    problem_ptr->geqs[problem_ptr->num_geqs].touched = 0;
			    problem_ptr->num_geqs++;
			  }
		      }
		    if (desired_result == SIMPLIFY)
		      {
			omega_problem_reduced (problem_ptr);
			return FALSE;
		      }
		    else
		      {
			if (!conservative &&
			    (desired_result != SIMPLIFY ||
			     (!lb_color && !ub_color))
			    && original_problem != no_problem
			    && lower_bound == upper_bound)
			  {
			    for (i = original_problem->num_vars; i >= 0; i--)
			      if (original_problem->var[i] ==
				  problem_ptr->var[1])
				break;
			    if (i == 0)
			      break;
			    e = original_problem->num_eqs++;
			    eqnnzero (&original_problem->eqs[e],
				      original_problem->num_vars);
			    original_problem->eqs[e].coef[i] = -1;
			    original_problem->eqs[e].coef[0] = upper_bound;
			    if (dump_file && (dump_flags & TDF_DETAILS))
			      {
				fprintf (dump_file,
					 "adding equality constraint %d to outer problem\n",
					 e);
				omega_print_problem (dump_file, original_problem);
			      }
			  }
			return TRUE;
		      }
		  }
		eliminate_again = 1;

		if (lower_bound_count == 1)
		  {
		    struct eqn lbeqn;
		    int Lc = problem_ptr->geqs[Le].coef[i];

		    if (dump_file && (dump_flags & TDF_DETAILS))
		      fprintf (dump_file, "an inplace elimination\n");

		    eqnncpy (&lbeqn, &problem_ptr->geqs[Le], (nV + 1));
		    do_delete_extra (problem_ptr, Le, nV + 1);
		    for (Ue = problem_ptr->num_geqs - 1; Ue >= 0; Ue--)
		      if (problem_ptr->geqs[Ue].coef[i] < 0)
			{
			  if (lbeqn.key == -problem_ptr->geqs[Ue].key)
			    do_delete_extra (problem_ptr, Ue, nV + 1);
			  else
			    {
			      int Uc = -problem_ptr->geqs[Ue].coef[i];
			      problem_ptr->geqs[Ue].touched = TRUE;
			      problem_ptr->geqs[Ue].color |= lbeqn.color;
			      eliminate_again = 0;
			      for (k = 0; k <= nV; k++)
				problem_ptr->geqs[Ue].coef[k] =
				  check_mul (problem_ptr->geqs[Ue].coef[k],
					    Lc) + check_mul (lbeqn.coef[k],
							    Uc);
			      if (dump_file && (dump_flags & TDF_DETAILS))
				{
				  omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[Ue]));
				  fprintf (dump_file, "\n");
				}
			    }
			}
		    continue;
		  }
		else
		  {
		    int dead_eqns[maxDead];
		    int num_dead = 0;
		    int top_eqn = problem_ptr->num_geqs - 1;
		    lower_bound_count--;

		    if (dump_file && (dump_flags & TDF_DETAILS))
		      fprintf (dump_file, "lower bound count = %d\n",
			       lower_bound_count);

		    for (Le = top_eqn; Le >= 0; Le--)
		      if (problem_ptr->geqs[Le].coef[i] > 0)
			{
			  int Lc = problem_ptr->geqs[Le].coef[i];
			  for (Ue = top_eqn; Ue >= 0; Ue--)
			    if (problem_ptr->geqs[Ue].coef[i] < 0)
			      {
				if (problem_ptr->geqs[Le].key != -problem_ptr->geqs[Ue].key)
				  {
				    int Uc = -problem_ptr->geqs[Ue].coef[i];
				    if (num_dead == 0)
				      e2 = problem_ptr->num_geqs++;
				    else
				      e2 = dead_eqns[--num_dead];
				    gcc_assert (e2 < max_geqs);

				    if (dump_file && (dump_flags & TDF_DETAILS))
				      {
					fprintf (dump_file,
						 "Le = %d, Ue = %d, gen = %d\n",
						 Le, Ue, e2);
					omega_print_geq_extra (dump_file, problem_ptr, &(problem_ptr->geqs[Le]));
					fprintf (dump_file, "\n");
					omega_print_geq_extra (dump_file, problem_ptr, &(problem_ptr->geqs[Ue]));
					fprintf (dump_file, "\n");
				      }
				    eliminate_again = 0;

				    for (k = nV; k >= 0; k--)
				      problem_ptr->geqs[e2].coef[k] =
					check_mul (problem_ptr->geqs[Ue].coef[k],
						  Lc) +
					check_mul (problem_ptr->geqs[Le].coef[k], Uc);
				    problem_ptr->geqs[e2].coef[nV + 1] = 0;
				    problem_ptr->geqs[e2].touched = TRUE;
				    problem_ptr->geqs[e2].color =
				      problem_ptr->geqs[Ue].color | problem_ptr->geqs[Le].color;

				    if (dump_file && (dump_flags & TDF_DETAILS))
				      {
					omega_print_geq (dump_file, problem_ptr, &(problem_ptr->geqs[e2]));
					fprintf (dump_file, "\n");
				      }
				  }
				if (lower_bound_count == 0)
				  {
				    dead_eqns[num_dead++] = Ue;

				    if (dump_file && (dump_flags & TDF_DETAILS))
				      fprintf (dump_file, "Killed %d\n", Ue);
				  }
			      }

			  lower_bound_count--;
			  dead_eqns[num_dead++] = Le;

			  if (dump_file && (dump_flags & TDF_DETAILS))
			    fprintf (dump_file, "Killed %d\n", Le);
			}
		    {
		      int isDead[max_geqs];
		      for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
			isDead[e] = FALSE;
		      while (num_dead > 0)
			{
			  e = dead_eqns[--num_dead];
			  isDead[e] = TRUE;
			}
		      for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
			if (isDead[e])
			  do_delete_extra (problem_ptr, e, nV + 1);
		    }
		    continue;
		  }
	      }
	    else
	      {
		omega_problem *rS, *iS;

		rS = (omega_problem *) xmalloc (sizeof (omega_problem));
		omega_initialize_problem (rS);
		iS = (omega_problem *) xmalloc (sizeof (omega_problem));
		omega_initialize_problem (iS);

		e2 = 0;
		possibleEasyIntSolution = TRUE;
		for (e = 0; e < problem_ptr->num_geqs; e++)
		  if (problem_ptr->geqs[e].coef[i] == 0)
		    {
		      eqncpy (&(rS->geqs[e2]), &problem_ptr->geqs[e]);
		      eqncpy (&(iS->geqs[e2]), &problem_ptr->geqs[e]);

		      if (dump_file && (dump_flags & TDF_DETAILS))
			{
			  int t;
			  fprintf (dump_file, "Copying (%d, %d): ", i,
				   problem_ptr->geqs[e].coef[i]);
			  omega_print_geq_extra (dump_file, problem_ptr, &problem_ptr->geqs[e]);
			  fprintf (dump_file, "\n");
			  for (t = 0; t <= nV + 1; t++)
			    fprintf (dump_file, "%d ", problem_ptr->geqs[e].coef[t]);
			  fprintf (dump_file, "\n");

			}
		      e2++;
		      gcc_assert (e2 < max_geqs);
		    }
		for (Le = problem_ptr->num_geqs - 1; Le >= 0; Le--)
		  if (problem_ptr->geqs[Le].coef[i] > 0)
		    {
		      int Lc = problem_ptr->geqs[Le].coef[i];
		      for (Ue = problem_ptr->num_geqs - 1; Ue >= 0; Ue--)
			if (problem_ptr->geqs[Ue].coef[i] < 0)
			  {
			    if (problem_ptr->geqs[Le].key != -problem_ptr->geqs[Ue].key)
			      {
				int Uc = -problem_ptr->geqs[Ue].coef[i];
				rS->geqs[e2].touched =
				  iS->geqs[e2].touched = TRUE;

				if (dump_file && (dump_flags & TDF_DETAILS))
				  {
				    fprintf (dump_file, "---\n");
				    fprintf (dump_file,
					     "Le(Lc) = %d(%d_, Ue(Uc) = %d(%d), gen = %d\n",
					     Le, Lc, Ue, Uc, e2);
				    omega_print_geq_extra (dump_file, problem_ptr, &problem_ptr->geqs[Le]);
				    fprintf (dump_file, "\n");
				    omega_print_geq_extra (dump_file, problem_ptr, &problem_ptr->geqs[Ue]);
				    fprintf (dump_file, "\n");
				  }

				if (Uc == Lc)
				  {
				    for (k = nV; k >= 0; k--)
				      iS->geqs[e2].coef[k] =
					rS->geqs[e2].coef[k] =
					problem_ptr->geqs[Ue].coef[k] + problem_ptr->geqs[Le].coef[k];
				    iS->geqs[e2].coef[0] -= (Uc - 1);
				  }
				else
				  {
				    for (k = nV; k >= 0; k--)
				      iS->geqs[e2].coef[k] =
					rS->geqs[e2].coef[k] =
					check_mul (problem_ptr->geqs[Ue].coef[k],
						  Lc) +
					check_mul (problem_ptr->geqs[Le].coef[k], Uc);
				    iS->geqs[e2].coef[0] -=
				      (Uc - 1) * (Lc - 1);
				  }

				iS->geqs[e2].color = rS->geqs[e2].color
				  = problem_ptr->geqs[Ue].color || problem_ptr->geqs[Le].color;

				if (dump_file && (dump_flags & TDF_DETAILS))
				  {
				    omega_print_geq (dump_file, problem_ptr, &(rS->geqs[e2]));
				    fprintf (dump_file, "\n");
				  }
				e2++;
				gcc_assert (e2 < max_geqs);
			      }
			    else
			      {
				int Uc = -problem_ptr->geqs[Ue].coef[i];
				if (problem_ptr->geqs[Ue].coef[0] * Lc +
				    problem_ptr->geqs[Le].coef[0] * Uc - (Uc - 1) * (Lc -
									1) <
				    0)
				  possibleEasyIntSolution = FALSE;
			      }
			  }

		    }
		iS->variablesInitialized = rS->variablesInitialized = 1;
		iS->num_vars = rS->num_vars = problem_ptr->num_vars;
		iS->num_geqs = rS->num_geqs = e2;
		iS->num_eqs = rS->num_eqs = 0;
		iS->num_subs = rS->num_subs = problem_ptr->num_subs;
		iS->safe_vars = rS->safe_vars = problem_ptr->safe_vars;
		{
		  int t;
		  for (t = nV; t >= 0; t--)
		    rS->var[t] = problem_ptr->var[t];
		  for (t = nV; t >= 0; t--)
		    iS->var[t] = problem_ptr->var[t];
		  for (e = problem_ptr->num_subs - 1; e >= 0; e--)
		    {
		      eqnncpy (&(rS->subs[e]), &(problem_ptr->subs[e]), problem_ptr->num_vars);
		      eqnncpy (&(iS->subs[e]), &(problem_ptr->subs[e]), problem_ptr->num_vars);
		    }
		}
		problem_ptr->num_vars++;
		nV = problem_ptr->num_vars;
		if (desired_result != TRUE)
		  {
		    int t = trace;

		    if (dump_file && (dump_flags & TDF_DETAILS))
		      fprintf (dump_file, "\nreal solution(%d):\n", depth);

		    depth++;
		    trace = 0;
		    if (original_problem == no_problem)
		      {
			original_problem = problem_ptr;
			result = solveGEQ (rS, FALSE);
			original_problem = no_problem;
		      }
		    else
		      result = solveGEQ (rS, FALSE);
		    trace = t;
		    depth--;
		    if (result == FALSE)
		      {
			free (rS);
			free (iS);
			return result;
		      }

		    if (problem_ptr->num_eqs > 0)
		      {
			/* An equality constraint must have been found */
			free (rS);
			free (iS);
			return omega_solve_problem (problem_ptr, desired_result);
		      }
		  }
		if (desired_result != FALSE)
		  {
		    if (possibleEasyIntSolution)
		      {
			if (dump_file && (dump_flags & TDF_DETAILS))
			  fprintf (dump_file, "\ninteger solution(%d):\n",
				   depth);
			depth++;
			conservative++;
			result = solveGEQ (iS, desired_result);
			conservative--;
			depth--;
			if (result != FALSE)
			  {
			    free (rS);
			    free (iS);
			    return result;
			  }
		      }
		    if (!exact && best_parallel_eqn >= 0
			&& parallel_difference <= max_splinters)
		      {
			free (rS);
			free (iS);
			return parallel_splinter (problem_ptr, best_parallel_eqn,
						  parallel_difference,
						  desired_result);
		      }

		    if (dump_file && (dump_flags & TDF_DETAILS))
		      fprintf (dump_file, "have to do exact analysis\n");

		    {
		      int worst_lower_bound_constant = -minC;
		      int lower_bounds = 0;
		      int lower_bound[max_geqs];
		      int smallest;
		      int t;
		      conservative++;
		      for (e = 0; e < problem_ptr->num_geqs; e++)
			if (problem_ptr->geqs[e].coef[i] > 1)
			  lower_bound[lower_bounds++] = e;
		      /* sort array */
		      for (j = 0; j < lower_bounds; j++)
			{
			  smallest = j;
			  for (k = j + 1; k < lower_bounds; k++)
			    if (problem_ptr->geqs[lower_bound[smallest]].coef[i] >
				problem_ptr->geqs[lower_bound[k]].coef[i])
			      smallest = k;
			  t = lower_bound[smallest];
			  lower_bound[smallest] = lower_bound[j];
			  lower_bound[j] = t;
			}

		      if (dump_file && (dump_flags & TDF_DETAILS))
			{
			  fprintf (dump_file, "lower bound coeeficients = ");
			  for (j = 0; j < lower_bounds; j++)
			    fprintf (dump_file, " %d",
				     problem_ptr->geqs[lower_bound[j]].coef[i]);
			  fprintf (dump_file, "\n");
			}


		      for (j = 0; j < lower_bounds; j++)
			{
			  int max_incr;
			  int c;
			  e = lower_bound[j];
			  max_incr =
			    ((problem_ptr->geqs[e].coef[i] - 1) *
			     (worst_lower_bound_constant - 1) -
			     1) / worst_lower_bound_constant;

			  /* max_incr += 2; */
			  if (dump_file && (dump_flags & TDF_DETAILS))
			    {
			      fprintf (dump_file, "for equation ");
			      omega_print_geq (dump_file, problem_ptr, &problem_ptr->geqs[e]);
			      fprintf (dump_file,
				       "\ntry decrements from 0 to %d\n",
				       max_incr);
			      omega_print_problem (dump_file, problem_ptr);
			    }
			  if (max_incr > 50)
			    {
			      if (!smoothed
				  && smooth_weird_equations (problem_ptr))
				{
				  conservative--;
				  free (rS);
				  free (iS);
				  smoothed = 1;
				  goto solve_geq_start;
				}
			    }
			  eqncpy (&problem_ptr->eqs[0], &problem_ptr->geqs[e]);
			  /*
			   * if (problem_ptr->geqs[e].color) fprintf(dump_file,"warning: adding black equality constraint
			   * based on red inequality\n");
			   */
			  problem_ptr->eqs[0].color = black;
			  eqnzero (&problem_ptr->geqs[e]);
			  problem_ptr->geqs[e].touched = TRUE;
			  problem_ptr->num_eqs = 1;
			  for (c = max_incr; c >= 0; c--)
			    {
			      if (dump_file && (dump_flags & TDF_DETAILS))
				{
				  fprintf (dump_file,
					   "trying next decrement of %d\n",
					   max_incr - c);
				  omega_print_problem (dump_file, problem_ptr);
				}

			      omega_copy_problem (rS, problem_ptr);

			      if (dump_file && (dump_flags & TDF_DETAILS))
				omega_print_problem (dump_file, rS);

			      result = omega_solve_problem (rS, desired_result);
			      if (result == TRUE)
				{
				  free (rS);
				  free (iS);
				  conservative--;
				  return TRUE;
				}
			      problem_ptr->eqs[0].coef[0]--;
			    }
			  if (j + 1 < lower_bounds)
			    {
			      problem_ptr->num_eqs = 0;
			      eqncpy (&problem_ptr->geqs[e], &problem_ptr->eqs[0]);
			      problem_ptr->geqs[e].touched = 1;
			      problem_ptr->geqs[e].color = black;
			      omega_copy_problem (rS, problem_ptr);

			      if (dump_file && (dump_flags & TDF_DETAILS))
				fprintf (dump_file,
					 "exhausted lower bound, checking if still feasible ");
			      result = omega_solve_problem (rS, FALSE);
			      if (result == FALSE)
				break;
			    }
			}

		      if (dump_file && (dump_flags & TDF_DETAILS))
			fprintf (dump_file, "fall-off the end\n");
		      free (rS);
		      free (iS);

		      conservative--;
		      return FALSE;
		    }
		  }
		free (rS);
		free (iS);
	      }
	    return UNKNOWN;
	  }
	}
      while (eliminate_again);
    }
}

/* Return true if red equations constrain the set of possible solutions.
   We assume that there are solutions to the black equations by
   themselves, so if there is no solution to the combined problem, we
   return true.  */

bool
omega_problem_has_red_equations (omega_problem *problem_ptr)
{
  bool result;
  int e;
  int i;


  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Checking for red equations:\n");
      omega_print_problem (dump_file, problem_ptr);
    }

  please_no_equalities_in_simplified_problems++;
  mayBeRed++;
#ifndef singleResult
  returnSingleResult++;
#endif
  createColor = 1;
  result = !omega_simplify_problem (problem_ptr);
#ifndef singleResult
  returnSingleResult--;
#endif
  mayBeRed--;
  please_no_equalities_in_simplified_problems--;
  if (result)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
      	fprintf (dump_file, "Gist is FALSE\n");
      problem_ptr->num_subs = 0;
      problem_ptr->num_geqs = 0;
      problem_ptr->num_eqs = 1;
      problem_ptr->eqs[0].color = red;
      for (i = problem_ptr->num_vars; i > 0; i--)
	problem_ptr->eqs[0].coef[i] = 0;
      problem_ptr->eqs[0].coef[0] = 1;
      return true;
    }
  freeRedEliminations (problem_ptr);

  gcc_assert (problem_ptr->num_eqs == 0);

  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    if (problem_ptr->geqs[e].color == red)
      result = true;

  if (!result)
    return false;

  for (i = problem_ptr->safe_vars; i >= 1; i--)
    {
      int ub = 0;
      int lb = 0;
      for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	{
	  if (problem_ptr->geqs[e].coef[i])
	    {
	      if (problem_ptr->geqs[e].coef[i] > 0)
		lb |= (1 + problem_ptr->geqs[e].color);
	      else		/* (problem_ptr->geqs[e].coef[i] < 0) */
		ub |= (1 + problem_ptr->geqs[e].color);
	    }
	}
      if (ub == 2 || lb == 2)
	{

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "checks for upper/lower bounds worked!\n");

	  if (!reduceWithSubs)
	    {
	      resurrectSubs (problem_ptr);
	      gcc_assert (problem_ptr->num_subs == 0);
	    }
	  return true;
	}
    }


  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file,
	     "*** Doing potentially expensive elimination tests for red equations\n");
  please_no_equalities_in_simplified_problems++;
  omega_eliminate_red (problem_ptr, 1);
  please_no_equalities_in_simplified_problems--;
  result = false;
  gcc_assert (problem_ptr->num_eqs == 0);
  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    if (problem_ptr->geqs[e].color == red)
      result = true;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      if (!result)
	fprintf (dump_file,
		 "******************** Redudant Red Equations eliminated!!\n");
      else
	fprintf (dump_file, "******************** Red Equations remain\n");

      omega_print_problem (dump_file, problem_ptr);
    }

  if (reduceWithSubs)
    {
    }
  else
    {
      resurrectSubs (problem_ptr);
      {
	normalize_return_type r = normalize_omega_problem (problem_ptr);
	gcc_assert (r != normalize_false);
      }
      coalesce (problem_ptr);
      cleanout_wildcards (problem_ptr);
      gcc_assert (problem_ptr->num_subs == 0);
    }
  return result;
}

int
omega_simplify_approximate (omega_problem *problem_ptr)
{
  int result;

  inApproximateMode = 1;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Entering Approximate Mode\n");

  result = omega_simplify_problem (problem_ptr);

  if (problem_ptr->num_vars != problem_ptr->safe_vars)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "gcc_assertion blown!\n");
	  omega_print_problem (dump_file, problem_ptr);
	}
      gcc_assert (problem_ptr->num_vars == problem_ptr->safe_vars);
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Leaving Approximate Mode\n");

  inApproximateMode = 0;

  if (reduceWithSubs)
    {
    }
  else
    {
      gcc_assert (problem_ptr->num_subs == 0);
    }
  return result;
}

int
omega_simplify_problem (omega_problem *problem_ptr)
{
  int i;
  foundReduction = FALSE;

  if (!problem_ptr->variablesInitialized)
    omega_initialize_variables (problem_ptr);

#ifdef clearForwardingPointers
  for (i = 1; i <= problem_ptr->num_vars; i++)
    if (problem_ptr->var[i] > 0)
      problem_ptr->forwarding_address[problem_ptr->var[i]] = 0;
#endif

  if (next_key * 3 > maxKeys)
    {
      int e;
      hashVersion++;
      next_key = max_vars + 1;
      for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	problem_ptr->geqs[e].touched = TRUE;
      for (i = 0; i < hash_table_size; i++)
	hash_master[i].touched = -1;
      problem_ptr->hashVersion = hashVersion;
    }
  else if (problem_ptr->hashVersion != hashVersion)
    {
      int e;
      for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	problem_ptr->geqs[e].touched = TRUE;
      problem_ptr->hashVersion = hashVersion;
    }

  nonConvex = 0;
  if (problem_ptr->num_vars > problem_ptr->num_eqs + 3 * problem_ptr->safe_vars)
    freeEliminations (problem_ptr, problem_ptr->safe_vars);

  if (!mayBeRed && problem_ptr->num_subs == 0 && problem_ptr->safe_vars == 0)
    {
      foundReduction = omega_solve_problem (problem_ptr, UNKNOWN);
      if (foundReduction && !returnSingleResult)
	{
	  problem_ptr->num_geqs = 0;
	  problem_ptr->num_eqs = 0;
	  (*whenReduced) (problem_ptr);
	}
      return foundReduction;
    }
  omega_solve_problem (problem_ptr, SIMPLIFY);
  if (foundReduction)
    {
      for (i = 1; i <= problem_ptr->safe_vars; i++)
	problem_ptr->forwarding_address[problem_ptr->var[i]] = i;
      for (i = 0; i < problem_ptr->num_subs; i++)
	problem_ptr->forwarding_address[problem_ptr->subs[i].key] = -i - 1;
    }
  if (reduceWithSubs)
    {
    }
  else
    {
      gcc_assert (please_no_equalities_in_simplified_problems
	      || !foundReduction || problem_ptr->num_subs == 0);
    }
  return foundReduction;
}


void
omega_unprotect_variable (omega_problem *problem_ptr, int v)
{
  int e, t, j, i;
  i = problem_ptr->forwarding_address[v];
  if (i < 0)
    {
      i = -1 - i;
      problem_ptr->num_subs--;
      if (i < problem_ptr->num_subs)
	{
	  eqncpy (&problem_ptr->subs[i], &problem_ptr->subs[problem_ptr->num_subs]);
	  problem_ptr->forwarding_address[problem_ptr->subs[i].key] = -i - 1;
	}
    }
  else
    {
      int bringToLife[max_vars];
      int comingBack = 0;
      int e2;
      for (e = problem_ptr->num_subs - 1; e >= 0; e--)
	if ((bringToLife[e] = (problem_ptr->subs[e].coef[i] != 0)))
	  comingBack++;

      for (e2 = problem_ptr->num_subs - 1; e2 >= 0; e2--)
	if (bringToLife[e2])
	  {

	    problem_ptr->num_vars++;
	    problem_ptr->safe_vars++;
	    if (problem_ptr->safe_vars < problem_ptr->num_vars)
	      {
		for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
		  {
		    problem_ptr->geqs[e].coef[problem_ptr->num_vars] = problem_ptr->geqs[e].coef[problem_ptr->safe_vars];
		    problem_ptr->geqs[e].coef[problem_ptr->safe_vars] = 0;
		  }
		for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
		  {
		    problem_ptr->eqs[e].coef[problem_ptr->num_vars] = problem_ptr->eqs[e].coef[problem_ptr->safe_vars];
		    problem_ptr->eqs[e].coef[problem_ptr->safe_vars] = 0;
		  }
		for (e = problem_ptr->num_subs - 1; e >= 0; e--)
		  {
		    problem_ptr->subs[e].coef[problem_ptr->num_vars] = problem_ptr->subs[e].coef[problem_ptr->safe_vars];
		    problem_ptr->subs[e].coef[problem_ptr->safe_vars] = 0;
		  }
		problem_ptr->var[problem_ptr->num_vars] = problem_ptr->var[problem_ptr->safe_vars];
		problem_ptr->forwarding_address[problem_ptr->var[problem_ptr->num_vars]] =
		  problem_ptr->num_vars;
	      }
	    else
	      {
		for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
		  {
		    problem_ptr->geqs[e].coef[problem_ptr->safe_vars] = 0;
		  }
		for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
		  {
		    problem_ptr->eqs[e].coef[problem_ptr->safe_vars] = 0;
		  }
		for (e = problem_ptr->num_subs - 1; e >= 0; e--)
		  {
		    problem_ptr->subs[e].coef[problem_ptr->safe_vars] = 0;
		  }
	      }

	    problem_ptr->var[problem_ptr->safe_vars] = problem_ptr->subs[e2].key;
	    problem_ptr->forwarding_address[problem_ptr->subs[e2].key] = problem_ptr->safe_vars;

	    eqncpy (&(problem_ptr->eqs[problem_ptr->num_eqs]), &(problem_ptr->subs[e2]));
	    problem_ptr->eqs[problem_ptr->num_eqs++].coef[problem_ptr->safe_vars] = -1;
	    gcc_assert (problem_ptr->num_eqs <= max_eqs);
	    if (e2 < problem_ptr->num_subs - 1)
	      eqncpy (&(problem_ptr->subs[e2]), &(problem_ptr->subs[problem_ptr->num_subs - 1]));
	    problem_ptr->num_subs--;
	  }




      if (i < problem_ptr->safe_vars)
	{
	  j = problem_ptr->safe_vars;
	  for (e = problem_ptr->num_subs - 1; e >= 0; e--)
	    {
	      t = problem_ptr->subs[e].coef[j];
	      problem_ptr->subs[e].coef[j] = problem_ptr->subs[e].coef[i];
	      problem_ptr->subs[e].coef[i] = t;
	    }
	  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
	    if (problem_ptr->geqs[e].coef[j] != problem_ptr->geqs[e].coef[i])
	      {
		problem_ptr->geqs[e].touched = TRUE;
		t = problem_ptr->geqs[e].coef[j];
		problem_ptr->geqs[e].coef[j] = problem_ptr->geqs[e].coef[i];
		problem_ptr->geqs[e].coef[i] = t;
	      }
	  for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
	    {
	      t = problem_ptr->eqs[e].coef[j];
	      problem_ptr->eqs[e].coef[j] = problem_ptr->eqs[e].coef[i];
	      problem_ptr->eqs[e].coef[i] = t;
	    }
	  t = problem_ptr->var[j];
	  problem_ptr->var[j] = problem_ptr->var[i];
	  problem_ptr->var[i] = t;
	  problem_ptr->forwarding_address[problem_ptr->var[i]] = i;
	  problem_ptr->forwarding_address[problem_ptr->var[j]] = j;
	}
      problem_ptr->safe_vars--;
    }
  chainUnprotect (problem_ptr);

}

/* constrainVariableSign also unprotects var & simplifies the problem */

int
omega_constrain_variable_sign (omega_problem *problem_ptr, int color, int i,
			       int sign)
{
  int nV = problem_ptr->num_vars;
  int e, k, j;

  k = problem_ptr->forwarding_address[i];
  if (k < 0)
    {
      k = -1 - k;

      if (sign != 0)
	{
	  e = problem_ptr->num_geqs++;
	  eqncpy (&problem_ptr->geqs[e], &problem_ptr->subs[k]);
	  for (j = 0; j <= nV; j++)
	    problem_ptr->geqs[e].coef[j] *= sign;
	  problem_ptr->geqs[e].coef[0]--;
	  problem_ptr->geqs[e].touched = 1;
	  problem_ptr->geqs[e].color = color;
	}
      else
	{
	  e = problem_ptr->num_eqs++;
	  gcc_assert (problem_ptr->num_eqs <= max_eqs);
	  eqncpy (&problem_ptr->eqs[e], &problem_ptr->subs[k]);
	  problem_ptr->eqs[e].color = color;
	}

    }
  else if (sign != 0)
    {
      e = problem_ptr->num_geqs++;
      eqnzero (&problem_ptr->geqs[e]);
      problem_ptr->geqs[e].coef[k] = sign;
      problem_ptr->geqs[e].coef[0] = -1;
      problem_ptr->geqs[e].touched = 1;
      problem_ptr->geqs[e].color = color;
    }
  else
    {
      e = problem_ptr->num_eqs++;
      gcc_assert (problem_ptr->num_eqs <= max_eqs);
      eqnzero (&problem_ptr->eqs[e]);
      problem_ptr->eqs[e].coef[k] = 1;
      problem_ptr->eqs[e].color = color;
    }
  omega_unprotect_variable (problem_ptr, i);
  return omega_simplify_problem (problem_ptr);
}

void
omega_constrain_variable_value (omega_problem *problem_ptr,
				int color, int i, int value)
{
  int e, k;

  k = problem_ptr->forwarding_address[i];
  if (k < 0)
    {
      k = -1 - k;

      e = problem_ptr->num_eqs++;
      gcc_assert (problem_ptr->num_eqs <= max_eqs);
      eqncpy (&problem_ptr->eqs[e], &problem_ptr->subs[k]);
      problem_ptr->eqs[e].coef[0] -= value;

    }
  else
    {
      e = problem_ptr->num_eqs++;
      eqnzero (&problem_ptr->eqs[e]);
      problem_ptr->eqs[e].coef[k] = 1;
      problem_ptr->eqs[e].coef[0] = -value;
    }
  problem_ptr->eqs[e].color = color;
}

int
omega_query_variable (omega_problem *problem_ptr, int i, int *lower_bound, int *upper_bound)
{
  int nV = problem_ptr->num_vars;
  int e, j;
  int isSimple;
  int coupled = FALSE;
  i = problem_ptr->forwarding_address[i];

  (*lower_bound) = neg_infinity;
  (*upper_bound) = pos_infinity;

  if (i < 0)
    {
      int easy = TRUE;
      i = -i - 1;
      for (j = 1; j <= nV; j++)
	if (problem_ptr->subs[i].coef[j] != 0)
	  easy = FALSE;
      if (easy)
	{
	  *upper_bound = *lower_bound = problem_ptr->subs[i].coef[0];
	  return FALSE;
	}
      return TRUE;
    }

  for (e = problem_ptr->num_subs - 1; e >= 0; e--)
    if (problem_ptr->subs[e].coef[i] != 0)
      coupled = TRUE;

  for (e = problem_ptr->num_eqs - 1; e >= 0; e--)
    if (problem_ptr->eqs[e].coef[i] != 0)
      {
	isSimple = TRUE;
	for (j = 1; j <= nV; j++)
	  if (i != j && problem_ptr->eqs[e].coef[j] != 0)
	    {
	      isSimple = FALSE;
	      coupled = TRUE;
	      break;
	    }
	if (!isSimple)
	  continue;
	else
	  {
	    *lower_bound = *upper_bound = -problem_ptr->eqs[e].coef[i] * problem_ptr->eqs[e].coef[0];
	    return FALSE;
	  }
      }
  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    if (problem_ptr->geqs[e].coef[i] != 0)
      {
	if (problem_ptr->geqs[e].key == i)
	  {
	    setMax (lower_bound, -problem_ptr->geqs[e].coef[0]);
	  }
	else if (problem_ptr->geqs[e].key == -i)
	  {
	    setMin (upper_bound, problem_ptr->geqs[e].coef[0]);
	  }
	else
	  coupled = TRUE;
      }
  return coupled;
}

static void
query_coupled_variable (omega_problem *problem_ptr, int i, int *l, int *u,
			int *could_be_zero, int lower_bound, int upper_bound)
{
  int e, b1, b2;
  eqn eqn;
  int sign;
  int v;

  if (abs (problem_ptr->forwarding_address[i]) != 1 || problem_ptr->num_vars + problem_ptr->num_subs != 2
      || problem_ptr->num_eqs + problem_ptr->num_subs != 1)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file,
		   "query_coupled_variable called with bad parameters\n");
	  omega_print_problem (dump_file, problem_ptr);
	}
      exit (2);
    }

  if (problem_ptr->forwarding_address[i] == -1)
    {
      eqn = &problem_ptr->subs[0];
      sign = 1;
      v = 1;
    }
  else
    {
      eqn = &problem_ptr->eqs[0];
      sign = -eqn->coef[1];
      v = 2;
    }

  /* Variable i is defined in terms of variable v */

  for (e = problem_ptr->num_geqs - 1; e >= 0; e--)
    if (problem_ptr->geqs[e].coef[v] != 0)
      {
	if (problem_ptr->geqs[e].coef[v] == 1)
	  setMax (&lower_bound, -problem_ptr->geqs[e].coef[0]);
	else
	  setMin (&upper_bound, problem_ptr->geqs[e].coef[0]);
      }
  /* lower_bound and upper_bound are bounds on the value of v */

  if (lower_bound > upper_bound)
    {
      *l = pos_infinity;
      *u = neg_infinity;
      *could_be_zero = 0;
      return;
    }
  if (lower_bound == neg_infinity)
    {
      if (eqn->coef[v] > 0)
	b1 = sign * neg_infinity;
      else
	b1 = -sign * neg_infinity;
    }
  else
    b1 = sign * (eqn->coef[0] + eqn->coef[v] * lower_bound);
  if (upper_bound == pos_infinity)
    {
      if (eqn->coef[v] > 0)
	b2 = sign * pos_infinity;
      else
	b2 = -sign * pos_infinity;
    }
  else
    b2 = sign * (eqn->coef[0] + eqn->coef[v] * upper_bound);

  /* b1 and b2 are bounds on the value of i (don't know which is upper bound) */
  if (b1 <= b2)
    {
      setMax (l, b1);
      setMin (u, b2);
    }
  else
    {
      setMax (l, b2);
      setMin (u, b1);
    }
  *could_be_zero = *l <= 0 && 0 <= *u
    && int_mod (eqn->coef[0], abs (eqn->coef[v])) == 0;
}

int
omega_query_variable_bounds (omega_problem *problem_ptr, int i, int *l, int *u)
{
  int coupled;

  *l = neg_infinity;
  *u = pos_infinity;

  coupled = omega_query_variable (problem_ptr, i, l, u);
  if (!coupled || (problem_ptr->num_vars == 1 && problem_ptr->forwarding_address[i] == 1))
    return 0;

  if (abs (problem_ptr->forwarding_address[i]) == 1 && problem_ptr->num_vars + problem_ptr->num_subs == 2
      && problem_ptr->num_eqs + problem_ptr->num_subs == 1)
    {
      int could_be_zero;
      query_coupled_variable (problem_ptr, i, l, u, &could_be_zero, neg_infinity,
			      pos_infinity);
      return 0;
    }
  return 1;
}

int
omega_query_variable_signs (omega_problem *problem_ptr, int i, int dd_lt,
			    int dd_eq, int dd_gt, int lower_bound,
			    int upper_bound, bool * dist_known, int *dist)
{
  int result;
  int l, u;
  int could_be_zero;

  l = neg_infinity;
  u = pos_infinity;

  omega_query_variable (problem_ptr, i, &l, &u);
  query_coupled_variable (problem_ptr, i, &l, &u, &could_be_zero, lower_bound,
			  upper_bound);
  result = 0;

  if (l < 0)
    result |= dd_gt;

  if (u > 0)
    result |= dd_lt;

  if (could_be_zero)
    result |= dd_eq;

  if (l == u)
    {
      *dist_known = 1;
      *dist = l;
    }
  else
    *dist_known = 0;

  return result;
}

static bool omega_initialized = false;

void
omega_initialize (void)
{
  int i;

  if (omega_initialized)
    return;

  next_wild_card = 0;
  next_key = max_vars + 1;

  for (i = 0; i < hash_table_size; i++)
    hash_master[i].touched = -1;

  sprintf (wild_name[1], "alpha");
  sprintf (wild_name[2], "beta");
  sprintf (wild_name[3], "gamma");
  sprintf (wild_name[4], "delta");
  sprintf (wild_name[5], "tau");
  sprintf (wild_name[6], "sigma");
  sprintf (wild_name[7], "chi");
  sprintf (wild_name[8], "omega");
  sprintf (wild_name[9], "pi");
  sprintf (wild_name[10], "ni");
  sprintf (wild_name[11], "Alpha");
  sprintf (wild_name[12], "Beta");
  sprintf (wild_name[13], "Gamma");
  sprintf (wild_name[14], "Delta");
  sprintf (wild_name[15], "Tau");
  sprintf (wild_name[16], "Sigma");
  sprintf (wild_name[17], "Chi");
  sprintf (wild_name[18], "Omega");
  sprintf (wild_name[19], "Pi");

  omega_initialized = true;
}


