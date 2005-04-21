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

/* Options:
   
   ELIMINATE_REDUNDANT_CONSTRAINTS
   - use expensive methods to eliminate all redundant constraints
   
   singleResult
   - only produce a single simplified result.

   APROX 
   - approximate inexact reductions omega_verify_simplification (runtime),
   - if TRUE, omega_simplify_problem checks for problem with no
   solutions reduceWithSubs (runtime),
   - if FALSE, convert substitutions back to EQs.
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
#include "omega.h"

bool omega_reduce_with_subs = true;
bool omega_verify_simplification = false;


#define MAX_WILD_CARDS 18
#ifndef APROX
#define APROX 0
#endif

#define EQ_type 1
#define GEQ_type 2
#define keyMult 31
#define indexMult 17

#ifdef singleResult
#define returnSingleResult  1
#else
static int returnSingleResult = 0;
#endif

static int mayBeRed = 0;
#define hash_table_size 550
#define maxKeys 500

static struct eqn hash_master[hash_table_size];

int nonConvex = 0;

static int doItAgain;

static int conservative = 0;

static int next_key;
static char wild_name[200][40];
static int next_wild_card = 0;
static int trace = 1;
static int depth = 0;
static bool omega_found_reduction;
static int packing[max_vars];

static int inApproximateMode = 0;
static int createColor = 0;
static int please_no_equalities_in_simplified_problems = 0;
static int hash_version = 0;

omega_pb no_problem = (omega_pb) 0;
omega_pb original_problem = (omega_pb) 0;

/* Return the integer A divided by B.  */

static inline int
int_div (int a, int b)
{
  if (a > 0)
    return a/b;
  else
    return -((-a + b - 1)/b);
}

/* Return the integer A modulo B.  */

static inline int
int_mod (int a, int b)
{
  return a - b * int_div (a, b);
}

/* For X and Y positive integers, return X multiplied by Y and check
   that the result does not overflow.  */

static inline int
check_pos_mul (int x, int y)
{
  if (x != 0)
    gcc_assert ((INT_MAX) / x > y);
  return x * y;
}

/* Return X multiplied by Y and check that the result does not
   overflow.  */

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

/* Return the absolute value of X.  */
#if 0
static inline int
abs (int x)
{
  return (x >= 0) ? x : -x;
}
#endif

/* Set *M to the maximum of *M and X.  */

static inline void
set_max (int *m, int x)
{
  if (*m < x) 
    *m = x;
}

/* Set *M to the minimum of *M and X.  */

static inline void
set_min (int *m, int x)
{
  if (*m > x)
    *m = x;
}

/* Test whether equation E is red.  */

static inline bool
omega_eqn_is_red (eqn e, int desired_result)
{
  return (desired_result == omega_simplify && e->color == omega_red);
}

/* Return a string for variable I.  */

static inline char *
omega_variable_to_str (int i)
{
  if (0 <= i && i < 20)
    return wild_name[i];

  if (-20 < i && i < 0)
    return wild_name[40 - i];

  gcc_unreachable ();
}

/* Do nothing function: used for default initializations.  */

void
omega_no_procedure (omega_pb pb ATTRIBUTE_UNUSED)
{
}

void (*omega_when_reduced) (omega_pb) = omega_no_procedure;

/* Compute the greatest common divisor of A and B.  */

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

/* Initialize variables for problem P.  */

void
omega_initialize_variables (omega_pb p)
{
  int i;

  for (i = p->num_vars; i >= 0; i--)
    p->forwarding_address[i] = p->var[i] = i;

  p->variablesInitialized = 1;
}

/* Initialize problem P.  */

void
omega_initialize_problem (omega_pb p)
{
  p->num_vars = 0;
  p->hash_version = hash_version;
  p->variablesInitialized = 0;
  p->variablesFreed = 0;
  p->safe_vars = 0;
  p->num_eqs = 0;
  p->num_geqs = 0;
  p->num_subs = 0;
}

/* Copy omega problems: P1 = P2.  */

void
omega_copy_problem (omega_pb p1, omega_pb p2)
{
  int e, i;

  p1->num_vars = p2->num_vars;
  p1->hash_version = p2->hash_version;
  p1->variablesInitialized = p2->variablesInitialized;
  p1->variablesFreed = p2->variablesFreed;
  p1->safe_vars = p2->safe_vars;
  p1->num_eqs = p2->num_eqs;
  p1->num_subs = p2->num_subs;
  p1->num_geqs = p2->num_geqs;

  for (e = p2->num_eqs - 1; e >= 0; e--)
    omega_copy_eqn (&(p1->eqs[e]), &(p2->eqs[e]), p2->num_vars);

  for (e = p2->num_geqs - 1; e >= 0; e--)
    omega_copy_eqn (&(p1->geqs[e]), &(p2->geqs[e]), p2->num_vars);

  for (e = p2->num_subs - 1; e >= 0; e--)
    omega_copy_eqn (&(p1->subs[e]), &(p2->subs[e]), p2->num_vars);

  for (i = 0; i <= p2->num_vars; i++)
    p1->var[i] = p2->var[i];

  for (i = 0; i <= max_vars; i++)
    p1->forwarding_address[i] = p2->forwarding_address[i];
}

/* Print to FILE from PB equation E with all its coefficients
   multiplied by C.  */

static void
omega_print_term (FILE *file, omega_pb pb, eqn e, int c)
{
  int i;
  int first = 1;
  int n = pb->num_vars;
  int wentFirst = -1;

  for (i = 1; i <= n; i++)
    if (c * e->coef[i] > 0)
      {

	first = 0;
	wentFirst = i;

	if (c * e->coef[i] == 1)
	  fprintf (file, "%s", omega_variable_to_str (pb->var[i]));
	else
	  fprintf (file, "%d * %s", c * e->coef[i], omega_variable_to_str (pb->var[i]));
	break;
      }

  for (i = 1; i <= n; i++)
    if (i != wentFirst && c * e->coef[i] != 0)
      {
	if (!first && c * e->coef[i] > 0)
	  fprintf (file, " + ");

	first = 0;

	if (c * e->coef[i] == 1)
	  fprintf (file, "%s", omega_variable_to_str (pb->var[i]));
	else if (c * e->coef[i] == -1)
	  fprintf (file, " - %s", omega_variable_to_str (pb->var[i]));
	else
	  fprintf (file, "%d * %s", c * e->coef[i], omega_variable_to_str (pb->var[i]));
      }

  if (!first && c * e->coef[0] > 0)
    fprintf (file, " + ");

  if (first || c * e->coef[0] != 0)
    fprintf (file, "%d", c * e->coef[0]);
}

/* Print to FILE the equation E of problem PB.  */

void
omega_print_eqn (FILE *file, omega_pb pb, eqn e, int test, int extra)
{
  char buf[max_vars * 12 + 80];

  omega_sprint_eqn (buf, pb, e, test, extra);
  fprintf (file, "%s", buf);
}

/* Print into buffer STR the equation E of problem PB.
   FIXME: Rewrite all these functions to directly output into file.  */

void
omega_sprint_eqn (char *str, omega_pb pb, eqn e, int test, int extra)
{
  int i;
  int first;
  int n = pb->num_vars + extra;
  int isLT;

  isLT = test && e->coef[0] == -1;

  if (isLT)
    isLT = 1;

  if (test)
    {
      if (e->touched)
	{
	  sprintf (str, "!");
	  while (*str)
	    str++;
	}
      else if (!e->touched && e->key != 0)
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
	    sprintf (str, "%s", omega_variable_to_str (pb->var[i]));
	    while (*str)
	      str++;
	  }
	else
	  {
	    sprintf (str, "%d * %s", -e->coef[i], omega_variable_to_str (pb->var[i]));
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
	    sprintf (str, "%s", omega_variable_to_str (pb->var[i]));
	    while (*str)
	      str++;
	  }
	else
	  {
	    sprintf (str, "%d * %s", e->coef[i], omega_variable_to_str (pb->var[i]));
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

/* Print to FILE all the variables of problem PB.  */

static void
printVars (FILE *file, omega_pb pb)
{
  int i;

  fprintf (file, "variables = ");

  if (pb->safe_vars > 0)
    fprintf (file, "(");

  for (i = 1; i <= pb->num_vars; i++)
    {
      fprintf (file, "%s", omega_variable_to_str (pb->var[i]));

      if (i == pb->safe_vars)
	fprintf (file, ")");

      if (i < pb->num_vars)
	fprintf (file, ", ");
    }

  fprintf (file, "\n");
}

/* Print to FILE problem PB.  */

void
omega_print_problem (FILE *file, omega_pb pb)
{
  int e;

  if (!pb->variablesInitialized)
    omega_initialize_variables (pb);

  printVars (file, pb);

  for (e = 0; e < pb->num_eqs; e++)
    {
      omega_print_eq (file, pb, &pb->eqs[e]);
      fprintf (file, "\n");
    }

  fprintf (file, "Done with EQ\n");

  for (e = 0; e < pb->num_geqs; e++)
    {
      omega_print_geq (file, pb, &pb->geqs[e]);
      fprintf (file, "\n");
    }

  fprintf (file, "Done with GEQ\n");

  for (e = 0; e < pb->num_subs; e++)
    {
      eqn eq = &pb->subs[e];

      if (eq->color)
	fprintf (file, "[");

      if (eq->key > 0)
	fprintf (file, "%s := ", omega_variable_to_str (eq->key));
      else
	fprintf (file, "#%d := ", eq->key);

      omega_print_term (file, pb, eq, 1);

      if (eq->color)
	fprintf (file, "]");

      fprintf (file, "\n");
    }
}

/* Return the number of equations in PB tagged in red.  */

int
omega_count_red_equations (omega_pb pb)
{
  int e, i;
  int result = 0;

  for (e = 0; e < pb->num_eqs; e++)
    if (pb->eqs[e].color == omega_red)
      {
	for (i = pb->num_vars; i > 0; i--)
	  if (pb->geqs[e].coef[i])
	    break;

	if (i == 0 && pb->geqs[e].coef[0] == 1)
	  return 0;
	else
	  result += 2;
      }

  for (e = 0; e < pb->num_geqs; e++)
    if (pb->geqs[e].color == omega_red)
      result += 1;

  for (e = 0; e < pb->num_subs; e++)
    if (pb->subs[e].color == omega_red)
      result += 2;

  return result;
}

/* Print to FILE all the equations in PB that are tagged in red.  */

void
omega_print_red_equations (FILE *file, omega_pb pb)
{
  int e;

  if (!pb->variablesInitialized)
    omega_initialize_variables (pb);

  printVars (file, pb);

  for (e = 0; e < pb->num_eqs; e++)
    if (pb->eqs[e].color == omega_red)
      {
	omega_print_eq (file, pb, &pb->eqs[e]);
	fprintf (file, "\n");
      }

  for (e = 0; e < pb->num_geqs; e++)
    if (pb->geqs[e].color == omega_red)
      {
	omega_print_geq (file, pb, &pb->geqs[e]);
	fprintf (file, "\n");
      }

  for (e = 0; e < pb->num_subs; e++)
    if (pb->subs[e].color == omega_red)
      {
	eqn eq = &pb->subs[e];
	if (eq->color)
	  fprintf (file, "[");

	if (eq->key > 0)
	  fprintf (file, "%s := ", omega_variable_to_str (eq->key));
	else
	  fprintf (file, "#%d := ", eq->key);

	omega_print_term (file, pb, eq, 1);

	if (eq->color)
	  fprintf (file, "]");

	fprintf (file, "\n");
      }
}

/* Pretty print PB to FILE.  */

void
omega_pretty_print_problem (FILE *file, omega_pb pb)
{
  int e;
  int v;
  int live[max_geqs];
  int v1, v2, v3;
  int t, change;
  int stuffPrinted = 0;

  typedef enum {
    none, le, lt
  } partialOrderType;

  partialOrderType po[max_vars][max_vars];
  int poE[max_vars][max_vars];
  int lastLinks[max_vars];
  int firstLinks[max_vars];
  int chain_length[max_vars];
  int chain[max_vars];
  int i, m, multiprint;

  if (!pb->variablesInitialized)
    omega_initialize_variables (pb);

  if (pb->num_vars > 0)
    {
      omega_eliminate_redundant (pb, 0);

      for (e = 0; e < pb->num_eqs; e++)
	{
	  if (stuffPrinted)
	    fprintf (file, "; ");
	  stuffPrinted = 1;
	  omega_print_eq (file, pb, &pb->eqs[e]);
	}

      for (e = 0; e < pb->num_geqs; e++)
	live[e] = TRUE;

      while (1)
	{
	  for (v = 1; v <= pb->num_vars; v++)
	    {
	      lastLinks[v] = firstLinks[v] = 0;
	      chain_length[v] = 0;
	      for (v2 = 1; v2 <= pb->num_vars; v2++)
		po[v][v2] = none;
	    }

	  for (e = 0; e < pb->num_geqs; e++)
	    if (live[e])
	      {
		for (v = 1; v <= pb->num_vars; v++)
		  if (pb->geqs[e].coef[v] == 1)
		    firstLinks[v]++;
		  else if (pb->geqs[e].coef[v] == -1)
		    lastLinks[v]++;

		v1 = pb->num_vars;

		while (v1 > 0 && pb->geqs[e].coef[v1] == 0)
		  v1--;

		v2 = v1 - 1;

		while (v2 > 0 && pb->geqs[e].coef[v2] == 0)
		  v2--;

		v3 = v2 - 1;

		while (v3 > 0 && pb->geqs[e].coef[v3] == 0)
		  v3--;

		if (pb->geqs[e].coef[0] > 0 || pb->geqs[e].coef[0] < -1
		    || v2 <= 0 || v3 > 0
		    || pb->geqs[e].coef[v1] * pb->geqs[e].coef[v2] != -1)
		  {
		    /* Not a partial order relation */

		  }
		else
		  {
		    if (pb->geqs[e].coef[v1] == 1)
		      {
			v3 = v2;
			v2 = v1;
			v1 = v3;
		      }
		    /* relation is v1 <= v2 or v1 < v2 */
		    po[v1][v2] = ((pb->geqs[e].coef[0] == 0) ? le : lt);
		    poE[v1][v2] = e;
		  }
	      }
	  for (v = 1; v <= pb->num_vars; v++)
	    chain_length[v] = lastLinks[v];


	  /*
	   * printf("\n\nPartial order:\n"); printf("         "); for (v1 = 1; v1 <= pb->num_vars; v1++)
	   * printf("%7s",variable(v1)); printf("\n"); for (v1 = 1; v1 <= pb->num_vars; v1++) { printf("%6s:
	   * ",variable(v1)); for (v2 = 1; v2 <= pb->num_vars; v2++) switch (po[v1][v2]) { case none: printf("       ");
	   * break; case le:   printf("    <= "); break; case lt:   printf("    <  "); break; } printf("\n"); }
	   */



	  /* Just in case pb->num_vars <= 0 */
	  change = FALSE;
	  for (t = 0; t < pb->num_vars; t++)
	    {
	      change = FALSE;
	      for (v1 = 1; v1 <= pb->num_vars; v1++)
		for (v2 = 1; v2 <= pb->num_vars; v2++)
		  if (po[v1][v2] != none &&
		      chain_length[v1] <= chain_length[v2])
		    {
		      chain_length[v1] = chain_length[v2] + 1;
		      change = TRUE;
		    }
	    }

	  if (change)
	    /* caught in cycle */
	    gcc_assert (0);

	  for (v1 = 1; v1 <= pb->num_vars; v1++)
	    if (chain_length[v1] == 0)
	      firstLinks[v1] = 0;

	  v = 1;
	  for (v1 = 2; v1 <= pb->num_vars; v1++)
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
	    for (e = 0; e < pb->num_geqs; e++)
	      if (live[e] && pb->geqs[e].coef[v] == 1)
		{
		  if (!first)
		    fprintf (file, ", ");
		  tmp = pb->geqs[e].coef[v];
		  pb->geqs[e].coef[v] = 0;
		  omega_print_term (file, pb, &pb->geqs[e], -1);
		  pb->geqs[e].coef[v] = tmp;
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
	      for (v2 = 1; v2 <= pb->num_vars; v2++)
		if (po[v][v2] && chain_length[v] == 1 + chain_length[v2])
		  break;
	      if (v2 > pb->num_vars)
		break;
	      chain[m++] = v2;
	      v = v2;
	    }

	  fprintf (file, "%s", omega_variable_to_str (pb->var[chain[0]]));

	  multiprint = 0;
	  for (i = 1; i < m; i++)
	    {
	      v = chain[i - 1];
	      v2 = chain[i];

	      if (po[v][v2] == le)
		fprintf (file, " <= ");
	      else
		fprintf (file, " < ");

	      fprintf (file, "%s", omega_variable_to_str (pb->var[v2]));
	      live[poE[v][v2]] = FALSE;
	      if (!multiprint && i < m - 1)
		for (v3 = 1; v3 <= pb->num_vars; v3++)
		  {
		    if (v == v3 || v2 == v3)
		      continue;
		    if (po[v][v2] != po[v][v3])
		      continue;
		    if (po[v2][chain[i + 1]] != po[v3][chain[i + 1]])
		      continue;
		    fprintf (file, ",%s", omega_variable_to_str (pb->var[v3]));
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
	    for (e = 0; e < pb->num_geqs; e++)
	      if (live[e] && pb->geqs[e].coef[v] == -1)
		{
		  if (!first)
		    fprintf (file, ", ");
		  else
		    fprintf (file, " <= ");
		  tmp = pb->geqs[e].coef[v];
		  pb->geqs[e].coef[v] = 0;
		  omega_print_term (file, pb, &pb->geqs[e], 1);
		  pb->geqs[e].coef[v] = tmp;
		  live[e] = FALSE;
		  first = 0;
		}
	  }
	}


      for (e = 0; e < pb->num_geqs; e++)
	if (live[e])
	  {
	    if (stuffPrinted)
	      fprintf (file, "; ");
	    stuffPrinted = 1;
	    omega_print_geq (file, pb, &pb->geqs[e]);
	  }

      for (e = 0; e < pb->num_subs; e++)
	{
	  eqn eq = &pb->subs[e];

	  if (stuffPrinted)
	    fprintf (file, "; ");

	  stuffPrinted = 1;

	  if (eq->key > 0)
	    fprintf (file, "%s := ", omega_variable_to_str (eq->key));
	  else
	    fprintf (file, "#%d := ", eq->key);

	  omega_print_term (file, pb, eq, 1);
	}
    }
}

/* */

static void
omega_name_wild_card (omega_pb pb, int i)
{
  --next_wild_card;
  if (next_wild_card < -MAX_WILD_CARDS)
    next_wild_card = -1;
  pb->var[i] = next_wild_card;
}

/* */

static int
omega_add_new_wild_card (omega_pb pb)
{
  int e;
  int i = ++pb->safe_vars;
  pb->num_vars++;
  if (pb->num_vars != i)
    {
      for (e = pb->num_geqs - 1; e >= 0; e--)
	{
	  if (pb->geqs[e].coef[i] != 0)
	    pb->geqs[e].touched = true;
	  pb->geqs[e].coef[pb->num_vars] = pb->geqs[e].coef[i];

	}

      for (e = pb->num_eqs - 1; e >= 0; e--)
	pb->eqs[e].coef[pb->num_vars] = pb->eqs[e].coef[i];

      for (e = pb->num_subs - 1; e >= 0; e--)
	pb->subs[e].coef[pb->num_vars] = pb->subs[e].coef[i];

      pb->var[pb->num_vars] = pb->var[i];
    }

  for (e = pb->num_geqs - 1; e >= 0; e--)
    pb->geqs[e].coef[i] = 0;

  for (e = pb->num_eqs - 1; e >= 0; e--)
    pb->eqs[e].coef[i] = 0;

  for (e = pb->num_subs - 1; e >= 0; e--)
    pb->subs[e].coef[i] = 0;

  omega_name_wild_card (pb, i);
  return i;
}

/* Delete inequality E from problem PB that has NV variables.  */

static void
omega_delete_geq (omega_pb pb, int e, int nv)
{
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Deleting %d (last:%d): ", e, pb->num_geqs-1);
      omega_print_geq (dump_file, pb, &pb->geqs[e]);
      fprintf (dump_file, "\n");
    }

  if (e < pb->num_geqs-1)
    omega_copy_eqn (&pb->geqs[e], &pb->geqs[pb->num_geqs - 1], nv);

  pb->num_geqs--;
}

/* Delete extra inequality E from problem PB that has NV variables.  */

static void
omega_delete_geq_extra (omega_pb pb, int e, int nV)
{
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Deleting %d: ",e);
      omega_print_geq_extra (dump_file, pb, &pb->geqs[e]);
      fprintf (dump_file, "\n");
    }

  if (e < pb->num_geqs-1)
    omega_copy_eqn (&pb->geqs[e], &pb->geqs[pb->num_geqs - 1],(nV));
  pb->num_geqs--;
}


/* Remove variable I from problem PB.  */

static void
omega_delete_variable (omega_pb pb, int i)
{
  int nV = pb->num_vars;
  int e;

  if (i < pb->safe_vars)
    {
      int j = pb->safe_vars;

      for (e = pb->num_geqs - 1; e >= 0; e--)
	{
	  pb->geqs[e].touched = true;
	  pb->geqs[e].coef[i] = pb->geqs[e].coef[j];
	  pb->geqs[e].coef[j] = pb->geqs[e].coef[nV];
	}

      for (e = pb->num_eqs - 1; e >= 0; e--)
	{
	  pb->eqs[e].coef[i] = pb->eqs[e].coef[j];
	  pb->eqs[e].coef[j] = pb->eqs[e].coef[nV];
	}

      for (e = pb->num_subs - 1; e >= 0; e--)
	{
	  pb->subs[e].coef[i] = pb->subs[e].coef[j];
	  pb->subs[e].coef[j] = pb->subs[e].coef[nV];
	}

      pb->var[i] = pb->var[j];
      pb->var[j] = pb->var[nV];
    }
  else if (i < nV)
    {
      for (e = pb->num_geqs - 1; e >= 0; e--)
	if (pb->geqs[e].coef[nV])
	  {
	    pb->geqs[e].coef[i] = pb->geqs[e].coef[nV];
	    pb->geqs[e].touched = true;
	  }

      for (e = pb->num_eqs - 1; e >= 0; e--)
	pb->eqs[e].coef[i] = pb->eqs[e].coef[nV];

      for (e = pb->num_subs - 1; e >= 0; e--)
	pb->subs[e].coef[i] = pb->subs[e].coef[nV];

      pb->var[i] = pb->var[nV];
    }

  if (i <= pb->safe_vars)
    pb->safe_vars--;

  pb->num_vars--;
}

/* */

static void
omega_substitute (omega_pb pb, eqn sub, int i, int c)
{
  int e, j, j0, k;
  int topVar;

  {
    int *p = &packing[0];
    for (k = pb->num_vars; k >= 0; k--)
      if (sub->coef[k])
	*(p++) = k;
    topVar = (p - &packing[0]) - 1;
  }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "substituting using %s := ", omega_variable_to_str (pb->var[i]));
      omega_print_term (dump_file, pb, sub, -c);
      fprintf (dump_file, "\n");
      printVars (dump_file, pb);
    }

  if (topVar < 0)
    {
      for (e = pb->num_eqs - 1; e >= 0; e--)
	pb->eqs[e].coef[i] = 0;

      for (e = pb->num_geqs - 1; e >= 0; e--)
	if (pb->geqs[e].coef[i] != 0)
	  {
	    pb->geqs[e].touched = true;
	    pb->geqs[e].coef[i] = 0;
	  }

      for (e = pb->num_subs - 1; e >= 0; e--)
	pb->subs[e].coef[i] = 0;

      if (i <= pb->safe_vars && pb->var[i] >= 0)
	{
	  eqn eqn = &(pb->subs[pb->num_subs++]);
	  for (k = pb->num_vars; k >= 0; k--)
	    eqn->coef[k] = 0;
	  eqn->key = pb->var[i];
	  eqn->color = 0;
	}
    }
  else if (topVar == 0 && packing[0] == 0)
    {
      c = -sub->coef[0] * c;

      for (e = pb->num_eqs - 1; e >= 0; e--)
	{
	  pb->eqs[e].coef[0] += pb->eqs[e].coef[i] * c;
	  pb->eqs[e].coef[i] = 0;
	}

      for (e = pb->num_geqs - 1; e >= 0; e--)
	if (pb->geqs[e].coef[i] != 0)
	  {
	    pb->geqs[e].coef[0] += pb->geqs[e].coef[i] * c;
	    pb->geqs[e].coef[i] = 0;
	    pb->geqs[e].touched = true;
	  }

      for (e = pb->num_subs - 1; e >= 0; e--)
	{
	  pb->subs[e].coef[0] += pb->subs[e].coef[i] * c;
	  pb->subs[e].coef[i] = 0;
	}

      if (i <= pb->safe_vars && pb->var[i] >= 0)
	{
	  eqn eqn = &(pb->subs[pb->num_subs++]);

	  for (k = pb->num_vars; k >= 1; k--)
	    eqn->coef[k] = 0;

	  eqn->coef[0] = c;
	  eqn->key = pb->var[i];
	  eqn->color = 0;
	}

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "---\n\n");
	  omega_print_problem (dump_file, pb);
	  fprintf (dump_file, "===\n\n");
	}
    }
  else
    {
      for (e = pb->num_eqs - 1; e >= 0; e--)
	{
	  eqn eqn = &(pb->eqs[e]);

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
	      omega_print_eq (dump_file, pb, eqn);
	      fprintf (dump_file, "\n");
	    }
	}

      for (e = pb->num_geqs - 1; e >= 0; e--)
	{
	  eqn eqn = &(pb->geqs[e]);
	  k = eqn->coef[i];
	  if (k != 0)
	    {
	      k = c * k;
	      eqn->touched = true;
	      eqn->coef[i] = 0;

	      for (j = topVar; j >= 0; j--)
		{
		  j0 = packing[j];
		  eqn->coef[j0] -= sub->coef[j0] * k;
		}
	    }

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      omega_print_geq (dump_file, pb, eqn);
	      fprintf (dump_file, "\n");
	    }
	}

      for (e = pb->num_subs - 1; e >= 0; e--)
	{
	  eqn eqn = &(pb->subs[e]);

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
	      fprintf (dump_file, "%s := ", omega_variable_to_str (eqn->key));
	      omega_print_term (dump_file, pb, eqn, 1);
	      fprintf (dump_file, "\n");
	    }
	}

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "---\n\n");
	  omega_print_problem (dump_file, pb);
	  fprintf (dump_file, "===\n\n");
	}

      if (i <= pb->safe_vars && pb->var[i] >= 0)
	{
	  eqn eqn;
	  eqn = &(pb->subs[pb->num_subs++]);
	  c = -c;

	  for (k = pb->num_vars; k >= 0; k--)
	    eqn->coef[k] = c * (sub->coef[k]);

	  eqn->key = pb->var[i];
	  eqn->color = sub->color;
	}
    }
}

/* Solve e = factor alpha for x_j and substitute.
   FIXME: What is this function doing?  */

static void
omega_do_mod (omega_pb pb, int factor, int e, int j)
{
  int k, i;
  struct eqn eq;
  int nFactor;
  int killJ = FALSE;

  omega_copy_eqn (&eq, &pb->eqs[e], pb->num_vars);

  for (k = pb->num_vars; k >= 0; k--)
    {
      eq.coef[k] = int_mod (eq.coef[k], factor);
      if (2 * eq.coef[k] >= factor)
	eq.coef[k] -= factor;
    }

  nFactor = eq.coef[j];

  if (j <= pb->safe_vars && pb->var[j] > 0)
    {
      i = omega_add_new_wild_card (pb);
      eq.coef[pb->num_vars] = eq.coef[i];
      eq.coef[j] = 0;
      eq.coef[i] = -factor;
      killJ = TRUE;
    }
  else
    {
      eq.coef[j] = -factor;
      if (pb->var[j] > 0)
	omega_name_wild_card (pb, j);
    }

  omega_substitute (pb, &eq, j, nFactor);

  for (k = pb->num_vars; k >= 0; k--)
    pb->eqs[e].coef[k] = pb->eqs[e].coef[k] / factor;

  if (killJ)
    omega_delete_variable (pb, j);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Mod-ing and normalizing produces:\n");
      omega_print_problem (dump_file, pb);
    }
}

/* Multiplies by -1 inequality E.  */

void
omega_negate_geq (omega_pb pb, int e)
{
  int i;

  for (i = pb->num_vars; i >= 0; i--)
    pb->geqs[e].coef[i] = -pb->geqs[e].coef[i];

  pb->geqs[e].coef[0]--;
  pb->geqs[e].touched = true;
}

/* Returns OMEGA_TRUE when problem PB has a solution.  */

static enum omega_result
verify_omega_pb (omega_pb pb)
{
  enum omega_result result;
  int e, anyColor;
  omega_pb tmp_problem = (omega_pb) xmalloc (sizeof (struct omega_pb));

  omega_copy_problem (tmp_problem, pb);
  tmp_problem->safe_vars = 0;
  tmp_problem->num_subs = 0;
  anyColor = 0;

  for (e = pb->num_geqs - 1; e >= 0; e--)
    anyColor |= pb->geqs[e].color;
  anyColor |= please_no_equalities_in_simplified_problems;

  if (anyColor)
    original_problem = no_problem;
  else
    original_problem = pb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "verifying problem");
      if (anyColor)
	fprintf (dump_file, " (color mode)");
      fprintf (dump_file, " :\n");
      omega_print_problem (dump_file, pb);
    }

  result = omega_solve_problem (tmp_problem, omega_unknown);
  original_problem = no_problem;
  free (tmp_problem);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      if (result != omega_false)
	fprintf (dump_file, "verified problem\n");
      else
	fprintf (dump_file, "disproved problem\n");
      omega_print_problem (dump_file, pb);
    }

  return result;
}

/* Add a new equality to problem PB at last position E.  */

static void
adding_equality_constraint (omega_pb pb, int e)
{
  int e2, i, j;

  if (original_problem != no_problem && original_problem != pb
      && !conservative)
    {
      e2 = original_problem->num_eqs++;

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file,
		 "adding equality constraint %d to outer problem\n", e2);
      omega_init_eqn_zero (&original_problem->eqs[e2],
			   original_problem->num_vars);

      for (i = pb->num_vars; i >= 1; i--)
	{
	  for (j = original_problem->num_vars; j >= 1; j--)
	    if (original_problem->var[j] == pb->var[i])
	      break;

	  if (j <= 0)
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "retracting\n");
	      original_problem->num_eqs--;
	      return;
	    }
	  original_problem->eqs[e2].coef[j] = pb->eqs[e].coef[i];
	}

      original_problem->eqs[e2].coef[0] = pb->eqs[e].coef[0];

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

/* */

static normalize_return_type
normalize_omega_problem (omega_pb pb)
{
  int e, i, j, k, nV;
  int coupled_subscripts = 0;

  nV = pb->num_vars;
  for (e = 0; e < pb->num_geqs; e++)
    {
      if (!pb->geqs[e].touched)
	{
	  if (!single_var_geq (pb->geqs[e], nV))
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
	      if (pb->geqs[e].coef[k])
		{
		  *(p++) = k;
		}
	    topVar = (p - &packing[0]) - 1;
	  }

	  if (topVar == -1)
	    {
	      if (pb->geqs[e].coef[0] < 0)
		{
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    {
		      omega_print_geq (dump_file, pb, &pb->geqs[e]);
		      fprintf (dump_file, "\nequations have no solution \n");
		    }
		  return normalize_false;
		}
	      omega_delete_geq (pb, e, nV);
	      e--;
	      continue;
	    }
	  else if (topVar == 0)
	    {
	      int singlevar = packing[0];
	      g = pb->geqs[e].coef[singlevar];
	      if (g > 0)
		{
		  pb->geqs[e].coef[singlevar] = 1;
		  pb->geqs[e].key = singlevar;
		}
	      else
		{
		  g = -g;
		  pb->geqs[e].coef[singlevar] = -1;
		  pb->geqs[e].key = -singlevar;
		}
	      if (g > 1)
		pb->geqs[e].coef[0] = int_div (pb->geqs[e].coef[0], g);
	    }
	  else
	    {

	      coupled_subscripts = 1;
	      i0 = topVar;
	      i = packing[i0--];
	      g = pb->geqs[e].coef[i];
	      hashCode = g * (i + 3);
	      if (g < 0)
		g = -g;
	      for (; i0 >= 0; i0--)
		{
		  int x;
		  i = packing[i0];
		  x = pb->geqs[e].coef[i];
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
		  x = pb->geqs[e].coef[i];
		  hashCode = hashCode * keyMult * (i + 3) + x;
		}
	      if (g > 1)
		{
		  pb->geqs[e].coef[0] = int_div (pb->geqs[e].coef[0], g);
		  i0 = topVar;
		  i = packing[i0--];
		  pb->geqs[e].coef[i] = pb->geqs[e].coef[i] / g;
		  hashCode = pb->geqs[e].coef[i] * (i + 3);
		  for (; i0 >= 0; i0--)
		    {
		      i = packing[i0];
		      pb->geqs[e].coef[i] = pb->geqs[e].coef[i] / g;
		      hashCode =
			hashCode * keyMult * (i + 3) + pb->geqs[e].coef[i];
		    }
		}

	      {
		int g2 = abs (hashCode);

		if (dump_file && (dump_flags & TDF_DETAILS))
		  {
		    fprintf (dump_file, "Hash code = %d, eqn = ", hashCode);
		    omega_print_geq (dump_file, pb, &pb->geqs[e]);
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
				  if (pb->geqs[e].coef[i] != proto->coef[i])
				    break;
				}
			    else
			      for (i0 = topVar; i0 >= 0; i0--)
				{
				  i = packing[i0];
				  if (pb->geqs[e].coef[i] != -proto->coef[i])
				    break;
				}
			    if (i0 < 0)
			      {
				if (hashCode >= 0)
				  pb->geqs[e].key = proto->key;
				else
				  pb->geqs[e].key = -proto->key;
				break;
			      }
			  }
		      }
		    else if (proto->touched < 0)
		      {
			omega_init_eqn_zero (proto, pb->num_vars);
			if (hashCode >= 0)
			  for (i0 = topVar; i0 >= 0; i0--)
			    {
			      i = packing[i0];
			      proto->coef[i] = pb->geqs[e].coef[i];
			    }
			else
			  for (i0 = topVar; i0 >= 0; i0--)
			    {
			      i = packing[i0];
			      proto->coef[i] = -pb->geqs[e].coef[i];
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
			  pb->geqs[e].key = proto->key;
			else
			  pb->geqs[e].key = -proto->key;
			break;
		      }
		    j = (j + 1) % hash_table_size;
		  }
	      }
	    }

	  pb->geqs[e].touched = false;
	}

      {
	int eKey = pb->geqs[e].key;
	int e2;
	if (e > 0)
	  {
	    int cTerm = pb->geqs[e].coef[0];
	    e2 = fastLookup[maxKeys - eKey];
	    if (e2 < e && pb->geqs[e2].key == -eKey && !pb->geqs[e2].color)
	      {
		if (pb->geqs[e2].coef[0] < -cTerm)
		  {
		    if (dump_file && (dump_flags & TDF_DETAILS))
		      {
			omega_print_geq (dump_file, pb, &pb->geqs[e]);
			fprintf (dump_file, "\n");
			omega_print_geq (dump_file, pb, &pb->geqs[e2]);
			fprintf (dump_file,
				 "\nequations have no solution \n");
		      }
		    return normalize_false;
		  }
		if (pb->geqs[e2].coef[0] == -cTerm
		    && (createColor || !pb->geqs[e].color))
		  {
		    omega_copy_eqn (&pb->eqs[pb->num_eqs], &pb->geqs[e], pb->num_vars);
		    if (!pb->geqs[e].color)
		      adding_equality_constraint (pb, pb->num_eqs);
		    pb->num_eqs++;
		    gcc_assert (pb->num_eqs <= max_eqs);
		  }
	      }
	    e2 = fastLookupRed[maxKeys - eKey];
	    if (e2 < e && pb->geqs[e2].key == -eKey && pb->geqs[e2].color)
	      {
		if (pb->geqs[e2].coef[0] < -cTerm)
		  {
		    if (dump_file && (dump_flags & TDF_DETAILS))
		      {
			omega_print_geq (dump_file, pb, &pb->geqs[e]);
			fprintf (dump_file, "\n");
			omega_print_geq (dump_file, pb, &pb->geqs[e2]);
			fprintf (dump_file,
				 "\nequations have no solution \n");
		      }
		    return normalize_false;
		  }
		if (pb->geqs[e2].coef[0] == -cTerm && createColor)
		  {
		    omega_copy_eqn (&pb->eqs[pb->num_eqs], &pb->geqs[e], pb->num_vars);
		    pb->eqs[pb->num_eqs].color = 1;
		    pb->num_eqs++;
		    gcc_assert (pb->num_eqs <= max_eqs);
		  }
	      }

	    e2 = fastLookup[maxKeys + eKey];
	    if (e2 < e && pb->geqs[e2].key == eKey && !pb->geqs[e2].color)
	      {
		if (pb->geqs[e2].coef[0] > cTerm)
		  {
		    if (!pb->geqs[e].color)
		      {
			if (dump_file && (dump_flags & TDF_DETAILS))
			  {
			    fprintf (dump_file,
				     "Removing Redudant Equation: ");
			    omega_print_geq (dump_file, pb, &(pb->geqs[e2]));
			    fprintf (dump_file, "\n");
			    fprintf (dump_file,
				     "[a]      Made Redundant by: ");
			    omega_print_geq (dump_file, pb, &(pb->geqs[e]));
			    fprintf (dump_file, "\n");
			  }
			pb->geqs[e2].coef[0] = cTerm;
			omega_delete_geq (pb, e, nV);
			e--;
			continue;
		      }
		  }
		else
		  {
		    if (dump_file && (dump_flags & TDF_DETAILS))
		      {
			fprintf (dump_file, "Removing Redudant Equation: ");
			omega_print_geq (dump_file, pb, &(pb->geqs[e]));
			fprintf (dump_file, "\n");
			fprintf (dump_file, "[b]      Made Redundant by: ");
			omega_print_geq (dump_file, pb, &(pb->geqs[e2]));
			fprintf (dump_file, "\n");
		      }
		    omega_delete_geq (pb, e, nV);
		    e--;
		    continue;
		  }
	      }
	    e2 = fastLookupRed[maxKeys + eKey];
	    if (e2 < e && pb->geqs[e2].key == eKey && pb->geqs[e2].color)
	      {
		if (pb->geqs[e2].coef[0] >= cTerm)
		  {
		    if (dump_file && (dump_flags & TDF_DETAILS))
		      {
			fprintf (dump_file, "Removing Redudant Equation: ");
			omega_print_geq (dump_file, pb, &(pb->geqs[e2]));
			fprintf (dump_file, "\n");
			fprintf (dump_file, "[c]      Made Redundant by: ");
			omega_print_geq (dump_file, pb, &(pb->geqs[e]));
			fprintf (dump_file, "\n");
		      }
		    pb->geqs[e2].coef[0] = cTerm;
		    pb->geqs[e2].color = pb->geqs[e].color;
		  }
		else if (pb->geqs[e].color)
		  {
		    if (dump_file && (dump_flags & TDF_DETAILS))
		      {
			fprintf (dump_file, "Removing Redudant Equation: ");
			omega_print_geq (dump_file, pb, &(pb->geqs[e]));
			fprintf (dump_file, "\n");
			fprintf (dump_file, "[d]      Made Redundant by: ");
			omega_print_geq (dump_file, pb, &(pb->geqs[e2]));
			fprintf (dump_file, "\n");
		      }
		  }
		omega_delete_geq (pb, e, nV);
		e--;
		continue;

	      }
	  }
	if (pb->geqs[e].color)
	  fastLookupRed[maxKeys + eKey] = e;
	else
	  fastLookup[maxKeys + eKey] = e;
      }
    }
  createColor = 0;
  return coupled_subscripts ? normalize_coupled : normalize_uncoupled;
}

/* */

static void
cleanout_wildcards (omega_pb pb)
{
  int e, e2, i, j;
  int a, c, nV;
  int g;
  int renormalize = 0;
  struct eqn *sub;
  nV = pb->num_vars;
  for (e = pb->num_eqs - 1; e >= 0; e--)
    {
      for (i = nV; i >= pb->safe_vars + 1; i--)
	if (pb->eqs[e].coef[i])
	  {
	    for (j = i - 1; j >= pb->safe_vars + 1; j--)
	      if (pb->eqs[e].coef[j])
		break;
	    if (j < pb->safe_vars + 1)
	      {
		/* Found a single wild card equality */
		if (dump_file && (dump_flags & TDF_DETAILS))
		  {
		    fprintf (dump_file,
			     "Found a single wild card equality: ");
		    omega_print_eq (dump_file, pb, &pb->eqs[e]);
		    fprintf (dump_file, "\n");
		    omega_print_problem (dump_file, pb);
		  }
		c = pb->eqs[e].coef[i];
		a = abs (c);
		sub = &(pb->eqs[e]);

		for (e2 = pb->num_eqs - 1; e2 >= 0; e2--)
		  if (e != e2 && pb->eqs[e2].coef[i]
		      && pb->eqs[e].color <= pb->eqs[e2].color)
		    {
		      eqn eqn = &(pb->eqs[e2]);
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
		for (e2 = pb->num_geqs - 1; e2 >= 0; e2--)
		  if (pb->geqs[e2].coef[i] && pb->eqs[e].color <= pb->geqs[e2].color)
		    {
		      eqn eqn = &(pb->geqs[e2]);
		      int j, k;
		      for (j = nV; j >= 0; j--)
			eqn->coef[j] *= a;
		      k = eqn->coef[i];
		      for (j = nV; j >= 0; j--)
			eqn->coef[j] -= sub->coef[j] * k / c;
		      eqn->coef[i] = 0;
		      eqn->touched = true;
		      renormalize = 1;
		    }
		for (e2 = pb->num_subs - 1; e2 >= 0; e2--)
		  if (pb->subs[e2].coef[i] && pb->eqs[e].color <= pb->subs[e2].color)
		    {
		      eqn eqn = &(pb->subs[e2]);
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
		    omega_print_problem (dump_file, pb);
		  }
		break;
	      }
	  }
    }
  if (renormalize)
    normalize_omega_problem (pb);
}

/* */

static inline void
swap (int *i, int *j)
{
  int tmp;
  tmp = *i;
  *i = *j;
  *j = tmp;
}

/* */

static void
resurrectSubs (omega_pb pb)
{
  if (pb->num_subs > 0 && !please_no_equalities_in_simplified_problems)
    {
      int i, e, n, m;

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file,
		   "problem reduced, bringing variables back to life\n");
	  omega_print_problem (dump_file, pb);
	}
      for (i = 1; i <= pb->safe_vars; i++)
	if (pb->var[i] < 0)
	  {
	    /* wild card */
	    if (i < pb->safe_vars)
	      {
		int j = pb->safe_vars;
		swap (&pb->var[i], &pb->var[j]);
		for (e = pb->num_geqs - 1; e >= 0; e--)
		  {
		    pb->geqs[e].touched = true;
		    swap (&pb->geqs[e].coef[i], &pb->geqs[e].coef[j]);
		  }
		for (e = pb->num_eqs - 1; e >= 0; e--)
		  swap (&pb->eqs[e].coef[i], &pb->eqs[e].coef[j]);
		for (e = pb->num_subs - 1; e >= 0; e--)
		  swap (&pb->subs[e].coef[i], &pb->subs[e].coef[j]);
		i--;
	      }
	    pb->safe_vars--;
	  }

      m = pb->num_subs;
      n = MAX (pb->num_vars, pb->safe_vars + m);
      for (e = pb->num_geqs - 1; e >= 0; e--)
	{
	  if (single_var_geq (pb->geqs[e], pb->num_vars))
	    {
	      i = abs (pb->geqs[e].key);
	      if (i >= pb->safe_vars + 1)
		pb->geqs[e].key += (pb->geqs[e].key > 0 ? m : -m);
	    }
	  else
	    {
	      pb->geqs[e].touched = true;
	      pb->geqs[e].key = 0;
	    }
	}
      for (i = pb->num_vars; i >= pb->safe_vars + 1; i--)
	{
	  pb->var[i + m] = pb->var[i];
	  for (e = pb->num_geqs - 1; e >= 0; e--)
	    pb->geqs[e].coef[i + m] = pb->geqs[e].coef[i];
	  for (e = pb->num_eqs - 1; e >= 0; e--)
	    pb->eqs[e].coef[i + m] = pb->eqs[e].coef[i];
	  for (e = pb->num_subs - 1; e >= 0; e--)
	    pb->subs[e].coef[i + m] = pb->subs[e].coef[i];
	}
      for (i = pb->safe_vars + m; i >= pb->safe_vars + 1; i--)
	{
	  for (e = pb->num_geqs - 1; e >= 0; e--)
	    pb->geqs[e].coef[i] = 0;
	  for (e = pb->num_eqs - 1; e >= 0; e--)
	    pb->eqs[e].coef[i] = 0;
	  for (e = pb->num_subs - 1; e >= 0; e--)
	    pb->subs[e].coef[i] = 0;
	}
      pb->num_vars += m;
      for (e = pb->num_subs - 1; e >= 0; e--)
	{
	  pb->var[pb->safe_vars + 1 + e] = pb->subs[e].key;
	  omega_copy_eqn (&(pb->eqs[pb->num_eqs]), &(pb->subs[e]), pb->num_vars);
	  pb->eqs[pb->num_eqs].coef[pb->safe_vars + 1 + e] = -1;
	  pb->eqs[pb->num_eqs].color = omega_black;

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "brought back: ");
	      omega_print_eq (dump_file, pb, &pb->eqs[pb->num_eqs]);
	      fprintf (dump_file, "\n");
	    }


	  pb->num_eqs++;
	  gcc_assert (pb->num_eqs <= max_eqs);
	}
      pb->safe_vars += m;
      pb->num_subs = 0;

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "variables brought back to life\n");
	  omega_print_problem (dump_file, pb);
	}

      cleanout_wildcards (pb);
    }
}

#define implies(A,B) (A==(A&B))

/* */

int
omega_eliminate_redundant (omega_pb pb, bool expensive)
{
  int e, e1, e2, e3, p, q, i, k, alpha, alpha1, alpha2, alpha3;
  int c;
  int isDead[max_geqs];
  unsigned int P[max_geqs], Z[max_geqs], N[max_geqs];
  unsigned int PP, PZ, PN;	/* possible Positives, possible zeros & possible negatives */

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "in eliminate Redudant:\n");
      omega_print_problem (dump_file, pb);
    }

  for (e = pb->num_geqs - 1; e >= 0; e--)
    {
      int tmp = 1;
      isDead[e] = 0;
      P[e] = Z[e] = N[e] = 0;
      for (i = pb->num_vars; i >= 1; i--)
	{
	  if (pb->geqs[e].coef[i] > 0)
	    P[e] |= tmp;
	  else if (pb->geqs[e].coef[i] < 0)
	    N[e] |= tmp;
	  else
	    Z[e] |= tmp;
	  tmp <<= 1;
	}
    }


  for (e1 = pb->num_geqs - 1; e1 >= 0; e1--)
    if (!isDead[e1])
      for (e2 = e1 - 1; e2 >= 0; e2--)
	if (!isDead[e2])
	  {
	    for (p = pb->num_vars; p > 1; p--)
	      {
		for (q = p - 1; q > 0; q--)
		  {
		    alpha =
		      (pb->geqs[e1].coef[p] * pb->geqs[e2].coef[q] -
		       pb->geqs[e2].coef[p] * pb->geqs[e1].coef[q]);
		    if (alpha != 0)
		      goto foundPQ;
		  }
	      }
	    continue;

	  foundPQ:
	    PZ = (Z[e1] & Z[e2]) | (P[e1] & N[e2]) | (N[e1] & P[e2]);
	    PP = P[e1] | P[e2];
	    PN = N[e1] | N[e2];

	    for (e3 = pb->num_geqs - 1; e3 >= 0; e3--)
	      if (e3 != e1 && e3 != e2)
		{

		  if (!implies (Z[e3], PZ))
		    goto nextE3;

		  alpha1 =
		    pb->geqs[e2].coef[q] * pb->geqs[e3].coef[p] -
		    pb->geqs[e2].coef[p] * pb->geqs[e3].coef[q];
		  alpha2 =
		    -(pb->geqs[e1].coef[q] * pb->geqs[e3].coef[p] -
		      pb->geqs[e1].coef[p] * pb->geqs[e3].coef[q]);
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
		      if (!pb->geqs[e3].color
			  && (pb->geqs[e1].color || pb->geqs[e2].color))
			goto nextE3;

		      /* verify alpha1*v1+alpha2*v2 = alpha3*v3 */
		      for (k = pb->num_vars; k >= 1; k--)
			if (alpha3 * pb->geqs[e3].coef[k]
			    !=
			    alpha1 * pb->geqs[e1].coef[k] +
			    alpha2 * pb->geqs[e2].coef[k])
			  goto nextE3;

		      c =
			alpha1 * pb->geqs[e1].coef[0] + alpha2 * pb->geqs[e2].coef[0];
		      if (c < alpha3 * (pb->geqs[e3].coef[0] + 1))
			{
			  if (dump_file && (dump_flags & TDF_DETAILS))
			    {
			      fprintf (dump_file,
				       "found redundant inequality\n");
			      fprintf (dump_file,
				       "alpha1, alpha2, alpha3 = %d,%d,%d\n",
				       alpha1, alpha2, alpha3);

			      omega_print_geq (dump_file, pb, &(pb->geqs[e1]));
			      fprintf (dump_file, "\n");
			      omega_print_geq (dump_file, pb, &(pb->geqs[e2]));
			      fprintf (dump_file, "\n=> ");
			      omega_print_geq (dump_file, pb, &(pb->geqs[e3]));
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
		      if (pb->geqs[e1].color || pb->geqs[e2].color || pb->geqs[e3].color)
			goto nextE3;

		      alpha3 = alpha3;
		      /* verify alpha1*v1+alpha2*v2 = alpha3*v3 */
		      for (k = pb->num_vars; k >= 1; k--)
			if (alpha3 * pb->geqs[e3].coef[k]
			    !=
			    alpha1 * pb->geqs[e1].coef[k] +
			    alpha2 * pb->geqs[e2].coef[k])
			  goto nextE3;

		      c =
			alpha1 * pb->geqs[e1].coef[0] + alpha2 * pb->geqs[e2].coef[0];
		      if (c < alpha3 * (pb->geqs[e3].coef[0]))
			{

			  /* We just proved e3 < 0, so no solutions exist */
			  if (dump_file && (dump_flags & TDF_DETAILS))
			    {
			      fprintf (dump_file,
				       "found implied over tight inequality\n");
			      fprintf (dump_file,
				       "alpha1, alpha2, alpha3 = %d,%d,%d\n",
				       alpha1, alpha2, -alpha3);
			      omega_print_geq (dump_file, pb, &(pb->geqs[e1]));
			      fprintf (dump_file, "\n");
			      omega_print_geq (dump_file, pb, &(pb->geqs[e2]));
			      fprintf (dump_file, "\n=> not ");
			      omega_print_geq (dump_file, pb, &(pb->geqs[e3]));
			      fprintf (dump_file, "\n\n");
			    }
			  return 0;

			}
		      else if (c < alpha3 * (pb->geqs[e3].coef[0] - 1))
			{

			  /* We just proved that e3 <=0, so e3 = 0 */
			  if (dump_file && (dump_flags & TDF_DETAILS))
			    {
			      fprintf (dump_file,
				       "found implied tight inequality\n");
			      fprintf (dump_file,
				       "alpha1, alpha2, alpha3 = %d,%d,%d\n",
				       alpha1, alpha2, -alpha3);
			      omega_print_geq (dump_file, pb, &(pb->geqs[e1]));
			      fprintf (dump_file, "\n");
			      omega_print_geq (dump_file, pb, &(pb->geqs[e2]));
			      fprintf (dump_file, "\n=> inverse ");
			      omega_print_geq (dump_file, pb, &(pb->geqs[e3]));
			      fprintf (dump_file, "\n\n");
			    }
			  omega_copy_eqn (&pb->eqs[pb->num_eqs++], &pb->geqs[e3], pb->num_vars);
			  gcc_assert (pb->num_eqs <= max_eqs);
			  adding_equality_constraint (pb, pb->num_eqs - 1);
			  isDead[e3] = 1;

			}
		    }
		nextE3:;
		}
	  }
  for (e = pb->num_geqs - 1; e >= 0; e--)
    if (isDead[e])
      omega_delete_geq (pb, e, pb->num_vars);

  /* if (pb->num_eqs) return solve(pb, SIMPLIFY); */

  if (!expensive)
    return 1;
  {
    omega_pb tmp_problem;
    int oldTrace = trace;
    trace = 0;
    tmp_problem = (omega_pb) xmalloc (sizeof (struct omega_pb));
    conservative++;
    for (e = pb->num_geqs - 1; e >= 0; e--)
      {
	if (dump_file && (dump_flags & TDF_DETAILS))
	  {
	    fprintf (dump_file,
		     "checking equation %d to see if it is redundant: ", e);
	    omega_print_geq (dump_file, pb, &(pb->geqs[e]));
	    fprintf (dump_file, "\n");
	  }
	omega_copy_problem (tmp_problem, pb);
	omega_negate_geq (tmp_problem, e);
	tmp_problem->safe_vars = 0;
	tmp_problem->variablesFreed = 0;
	if (!omega_solve_problem (tmp_problem, omega_false))
	  omega_delete_geq (pb, e, pb->num_vars);
      }
    trace = oldTrace;
    free (tmp_problem);
    conservative--;
  }
  if (!omega_reduce_with_subs)
    {
      resurrectSubs (pb);
      gcc_assert (please_no_equalities_in_simplified_problems
		  || pb->num_subs == 0);
    }
  return 1;
}

/* */

static int
smooth_weird_equations (omega_pb pb)
{
  int e1, e2, e3, p, q, k, alpha, alpha1, alpha2, alpha3;
  int c;
  int v;
  int result = 0;

  for (e1 = pb->num_geqs - 1; e1 >= 0; e1--)
    if (!pb->geqs[e1].color)
      {
	int g = 999999;
	for (v = pb->num_vars; v >= 1; v--)
	  if (pb->geqs[e1].coef[v] != 0 && abs (pb->geqs[e1].coef[v]) < g)
	    g = abs (pb->geqs[e1].coef[v]);
	if (g > 20)
	  {

	    e3 = pb->num_geqs;
	    for (v = pb->num_vars; v >= 1; v--)
	      pb->geqs[e3].coef[v] = int_div (6 * pb->geqs[e1].coef[v] + g / 2, g);
	    pb->geqs[e3].color = 0;
	    pb->geqs[e3].touched = true;
	    pb->geqs[e3].coef[0] = 9997;

	    if (dump_file && (dump_flags & TDF_DETAILS))
	      {
		fprintf (dump_file, "Checking to see if we can derive: ");
		omega_print_geq (dump_file, pb, &pb->geqs[e3]);
		fprintf (dump_file, "\n from: ");
		omega_print_geq (dump_file, pb, &pb->geqs[e1]);
		fprintf (dump_file, "\n");
	      }


	    for (e2 = pb->num_geqs - 1; e2 >= 0; e2--)
	      if (e1 != e2 && !pb->geqs[e2].color)
		{
		  for (p = pb->num_vars; p > 1; p--)
		    {
		      for (q = p - 1; q > 0; q--)
			{
			  alpha =
			    (pb->geqs[e1].coef[p] * pb->geqs[e2].coef[q] -
			     pb->geqs[e2].coef[p] * pb->geqs[e1].coef[q]);
			  if (alpha != 0)
			    goto foundPQ;
			}
		    }
		  continue;

		foundPQ:

		  alpha1 =
		    pb->geqs[e2].coef[q] * pb->geqs[e3].coef[p] -
		    pb->geqs[e2].coef[p] * pb->geqs[e3].coef[q];
		  alpha2 =
		    -(pb->geqs[e1].coef[q] * pb->geqs[e3].coef[p] -
		      pb->geqs[e1].coef[p] * pb->geqs[e3].coef[q]);
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
		      for (k = pb->num_vars; k >= 1; k--)
			if (alpha3 * pb->geqs[e3].coef[k]
			    !=
			    alpha1 * pb->geqs[e1].coef[k] +
			    alpha2 * pb->geqs[e2].coef[k])
			  goto nextE2;

		      c =
			alpha1 * pb->geqs[e1].coef[0] + alpha2 * pb->geqs[e2].coef[0];
		      if (c < alpha3 * (pb->geqs[e3].coef[0] + 1))
			pb->geqs[e3].coef[0] = int_div (c, alpha3);

		    }
		nextE2:;
		}
	    if (pb->geqs[e3].coef[0] < 9997)
	      {
		result++;
		pb->num_geqs++;

		if (dump_file && (dump_flags & TDF_DETAILS))
		  {
		    fprintf (dump_file,
			     "Smoothing wierd equations; adding:\n");
		    omega_print_geq (dump_file, pb, &pb->geqs[e3]);
		    fprintf (dump_file, "\nto:\n");
		    omega_print_problem (dump_file, pb);
		    fprintf (dump_file, "\n\n");
		  }
	      }
	  }
      }
  return result;
}

/* */

static void
coalesce (omega_pb pb)
{
  int e, e2, colors;
  int isDead[max_geqs];
  int foundSomething = 0;

  colors = 0;
  for (e = 0; e < pb->num_geqs; e++)
    if (pb->geqs[e].color)
      colors++;

  if (colors < 2)
    return;

  for (e = 0; e < pb->num_geqs; e++)
    isDead[e] = 0;

  for (e = 0; e < pb->num_geqs; e++)
    if (pb->geqs[e].color & !pb->geqs[e].touched)
      for (e2 = e + 1; e2 < pb->num_geqs; e2++)
	if (!pb->geqs[e2].touched && pb->geqs[e].key == -pb->geqs[e2].key
	    && pb->geqs[e].coef[0] == -pb->geqs[e2].coef[0] && pb->geqs[e2].color)
	  {
	    omega_copy_eqn (&pb->eqs[pb->num_eqs++], &pb->geqs[e], pb->num_vars);
	    gcc_assert (pb->num_eqs <= max_eqs);
	    foundSomething++;
	    isDead[e] = 1;
	    isDead[e2] = 1;
	  }

  for (e = pb->num_geqs - 1; e >= 0; e--)
    if (isDead[e])
      omega_delete_geq (pb, e, pb->num_vars);

  if (dump_file && (dump_flags & TDF_DETAILS) && foundSomething)
    {
      fprintf (dump_file, "Coalesced pb->geqs into %d EQ's:\n", foundSomething);
      omega_print_problem (dump_file, pb);
    }

}

/* */

void
omega_eliminate_red (omega_pb pb, bool eliminateAll)
{
  int e, e2, e3, i, j, k, a, alpha1, alpha2;
  int c = 0;
  int isDead[max_geqs];
  int deadCount = 0;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "in eliminate RED:\n");
      omega_print_problem (dump_file, pb);
    }
  if (pb->num_eqs > 0)
    omega_simplify_problem (pb);

  for (e = pb->num_geqs - 1; e >= 0; e--)
    isDead[e] = 0;
  for (e = pb->num_geqs - 1; e >= 0; e--)
    if (!pb->geqs[e].color && !isDead[e])
      for (e2 = e - 1; e2 >= 0; e2--)
	if (!pb->geqs[e2].color && !isDead[e2])
	  {
	    a = 0;
	    for (i = pb->num_vars; i > 1; i--)
	      {
		for (j = i - 1; j > 0; j--)
		  {
		    a =
		      (pb->geqs[e].coef[i] * pb->geqs[e2].coef[j] -
		       pb->geqs[e2].coef[i] * pb->geqs[e].coef[j]);
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
			 omega_variable_to_str (pb->var[i]));
		fprintf (dump_file, "j = %s, alpha = %d\n", omega_variable_to_str (pb->var[j]), a);
		omega_print_geq (dump_file, pb, &(pb->geqs[e]));
		fprintf (dump_file, "\n");
		omega_print_geq (dump_file, pb, &(pb->geqs[e2]));
		fprintf (dump_file, "\n");
	      }
	    for (e3 = pb->num_geqs - 1; e3 >= 0; e3--)
	      if (pb->geqs[e3].color)
		{
		  alpha1 =
		    pb->geqs[e2].coef[j] * pb->geqs[e3].coef[i] -
		    pb->geqs[e2].coef[i] * pb->geqs[e3].coef[j];
		  alpha2 =
		    -(pb->geqs[e].coef[j] * pb->geqs[e3].coef[i] -
		      pb->geqs[e].coef[i] * pb->geqs[e3].coef[j]);
		  if ((a > 0 && alpha1 > 0 && alpha2 > 0)
		      || (a < 0 && alpha1 < 0 && alpha2 < 0))
		    {
		      if (dump_file && (dump_flags & TDF_DETAILS))
			{
			  fprintf (dump_file,
				   "alpha1 = %d, alpha2 = %d; comparing against: ",
				   alpha1, alpha2);
			  omega_print_geq (dump_file, pb, &(pb->geqs[e3]));
			  fprintf (dump_file, "\n");
			}
		      for (k = pb->num_vars; k >= 0; k--)
			{
			  c =
			    alpha1 * pb->geqs[e].coef[k] +
			    alpha2 * pb->geqs[e2].coef[k];
			  if (c != a * pb->geqs[e3].coef[k])
			    break;
			  if (dump_file && (dump_flags & TDF_DETAILS) && k > 0)
			    fprintf (dump_file, " %s: %d, %d\n",
				     omega_variable_to_str (pb->var[k]), c, a * pb->geqs[e3].coef[k]);
			}
		      if (k < 0
			  || (k == 0 &&
			      ((a > 0 && c < a * pb->geqs[e3].coef[k])
			       || (a < 0 && c > a * pb->geqs[e3].coef[k]))))
			{
			  if (dump_file && (dump_flags & TDF_DETAILS))
			    {
			      deadCount++;
			      fprintf (dump_file,
				       "red equation#%d is dead (%d dead so far, %d remain)\n",
				       e3, deadCount, pb->num_geqs - deadCount);
			      omega_print_geq (dump_file, pb, &(pb->geqs[e]));
			      fprintf (dump_file, "\n");
			      omega_print_geq (dump_file, pb, &(pb->geqs[e2]));
			      fprintf (dump_file, "\n");
			      omega_print_geq (dump_file, pb, &(pb->geqs[e3]));
			      fprintf (dump_file, "\n");
			    }
			  isDead[e3] = 1;
			}
		    }
		}
	  }
  for (e = pb->num_geqs - 1; e >= 0; e--)
    if (isDead[e])
      omega_delete_geq (pb, e, pb->num_vars);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "in eliminate RED, easy tests done:\n");
      omega_print_problem (dump_file, pb);
    }

  {
    int redFound = 0;
    for (e = pb->num_geqs - 1; e >= 0; e--)
      if (pb->geqs[e].color)
	redFound = 1;
    if (!redFound)
      {
	if (dump_file && (dump_flags & TDF_DETAILS))
	  fprintf (dump_file, "fast checks worked\n");

	if (!omega_reduce_with_subs)
	  gcc_assert (please_no_equalities_in_simplified_problems
		      || pb->num_subs == 0);

	return;
      }
  }

  if (!omega_verify_simplification)
    {
      if (!verify_omega_pb (pb))
	return;
    }
  {
    omega_pb tmp_problem;
    int oldTrace = trace;
    trace = 0;
    conservative++;
    tmp_problem = (omega_pb) xmalloc (sizeof (struct omega_pb));
    for (e = pb->num_geqs - 1; e >= 0; e--)
      if (pb->geqs[e].color)
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file,
		       "checking equation %d to see if it is redundant: ", e);
	      omega_print_geq (dump_file, pb, &(pb->geqs[e]));
	      fprintf (dump_file, "\n");
	    }
	  omega_copy_problem (tmp_problem, pb);
	  omega_negate_geq (tmp_problem, e);
	  tmp_problem->safe_vars = 0;
	  tmp_problem->variablesFreed = 0;
	  tmp_problem->num_subs = 0;
	  if (!omega_solve_problem (tmp_problem, omega_false))
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "it is redundant\n");
	      omega_delete_geq (pb, e, pb->num_vars);
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
  /* omega_simplify_problem (pb); */
  if (!omega_reduce_with_subs)
    gcc_assert (please_no_equalities_in_simplified_problems || pb->num_subs == 0);
}

/* */

static void
chainUnprotect (omega_pb pb)
{
  int i, e;
  int unprotect[max_vars];

  for (i = 1; i <= pb->safe_vars; i++)
    {
      unprotect[i] = (pb->var[i] < 0);
      for (e = pb->num_subs - 1; e >= 0; e--)
	if (pb->subs[e].coef[i])
	  unprotect[i] = 0;
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Doing chain reaction unprotection\n");
      omega_print_problem (dump_file, pb);
      for (i = 1; i <= pb->safe_vars; i++)
	if (unprotect[i])
	  fprintf (dump_file, "unprotecting %s\n", omega_variable_to_str (pb->var[i]));
    }

  for (i = 1; i <= pb->safe_vars; i++)
    if (unprotect[i])
      {
	/* wild card */
	if (i < pb->safe_vars)
	  {
	    int j = pb->safe_vars;
	    swap (&pb->var[i], &pb->var[j]);
	    for (e = pb->num_geqs - 1; e >= 0; e--)
	      {
		pb->geqs[e].touched = true;
		swap (&pb->geqs[e].coef[i], &pb->geqs[e].coef[j]);
	      }
	    for (e = pb->num_eqs - 1; e >= 0; e--)
	      swap (&pb->eqs[e].coef[i], &pb->eqs[e].coef[j]);
	    for (e = pb->num_subs - 1; e >= 0; e--)
	      swap (&pb->subs[e].coef[i], &pb->subs[e].coef[j]);
	    swap (&unprotect[i], &unprotect[j]);
	    i--;
	  }
	pb->safe_vars--;
      }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "After chain reactions\n");
      omega_print_problem (dump_file, pb);
    }
}

/* */

static void
omega_problem_reduced (omega_pb pb)
{
  if (omega_verify_simplification)
    {
      int result;
      if (inApproximateMode)
	result = 1;
      else
	result = verify_omega_pb (pb);
      if (!result)
	return;
      if (pb->num_eqs > 0)
	doItAgain = 1;
    }

#ifdef eliminateRedundantConstraints
  if (!omega_eliminate_redundant (pb, 1))
    return;
#endif

  omega_found_reduction = true;

  if (!please_no_equalities_in_simplified_problems)
    coalesce (pb);

  if (omega_reduce_with_subs || please_no_equalities_in_simplified_problems)
    chainUnprotect (pb);
  else
    resurrectSubs (pb);

  if (!returnSingleResult)
    {
      int i;

      for (i = 1; i <= pb->safe_vars; i++)
	pb->forwarding_address[pb->var[i]] = i;

      for (i = 0; i < pb->num_subs; i++)
	pb->forwarding_address[pb->subs[i].key] = -i - 1;

      (*omega_when_reduced) (pb);
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "-------------------------------------------\n");
      fprintf (dump_file, "problem reduced:\n");
      omega_print_problem (dump_file, pb);
      fprintf (dump_file, "-------------------------------------------\n");
    }
}

/* Eliminates all the free variables from PB.  */

static void
omega_free_eliminations (omega_pb pb, int fv)
{
  bool try_again = true;
  int i, e, e2;
  int nV = pb->num_vars;

  while (try_again)
    {
      try_again = false;

      for (i = nV; i > fv; i--)
	{
	  for (e = pb->num_geqs - 1; e >= 0; e--)
	    if (pb->geqs[e].coef[i])
	      break;

	  if (e < 0)
	    e2 = e;
	  else if (pb->geqs[e].coef[i] > 0)
	    {
	      for (e2 = e - 1; e2 >= 0; e2--)
		if (pb->geqs[e2].coef[i] < 0)
		  break;
	    }
	  else
	    {
	      for (e2 = e - 1; e2 >= 0; e2--)
		if (pb->geqs[e2].coef[i] > 0)
		  break;
	    }

	  if (e2 < 0)
	    {
	      int e3;
	      for (e3 = pb->num_subs - 1; e3 >= 0; e3--)
		if (pb->subs[e3].coef[i])
		  break;

	      if (e3 >= 0)
		continue;

	      for (e3 = pb->num_eqs - 1; e3 >= 0; e3--)
		if (pb->eqs[e3].coef[i])
		  break;

	      if (e3 >= 0)
		continue;

	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "a free elimination of %s\n",
			 omega_variable_to_str (pb->var[i]));

	      if (e >= 0)
		{
		  omega_delete_geq (pb, e, nV);

		  for (e--; e >= 0; e--)
		    if (pb->geqs[e].coef[i])
		      omega_delete_geq (pb, e, nV);

		  try_again = (i < nV);
		}

	      omega_delete_variable (pb, i);
	      nV = pb->num_vars;
	    }
	}
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nafter free eliminations:\n");
      omega_print_problem (dump_file, pb);
      fprintf (dump_file, "\n");
    }
}

/* Do free red eliminations.  */

static void
freeRedEliminations (omega_pb pb)
{
  int tryAgain = 1;
  int i, e, e2;
  int nV = pb->num_vars;
  int isRedVar[max_vars];
  int isDeadVar[max_vars];
  int isDeadGEQ[max_geqs];
  for (i = nV; i > 0; i--)
    {
      isRedVar[i] = 0;
      isDeadVar[i] = 0;
    }
  for (e = pb->num_geqs - 1; e >= 0; e--)
    {
      isDeadGEQ[e] = 0;
      if (pb->geqs[e].color)
	for (i = nV; i > 0; i--)
	  if (pb->geqs[e].coef[i] != 0)
	    isRedVar[i] = 1;
    }

  while (tryAgain)
    {
      tryAgain = 0;
      for (i = nV; i > 0; i--)
	if (!isRedVar[i] && !isDeadVar[i])
	  {
	    for (e = pb->num_geqs - 1; e >= 0; e--)
	      if (!isDeadGEQ[e] && pb->geqs[e].coef[i])
		break;
	    if (e < 0)
	      e2 = e;
	    else if (pb->geqs[e].coef[i] > 0)
	      {
		for (e2 = e - 1; e2 >= 0; e2--)
		  if (!isDeadGEQ[e2] && pb->geqs[e2].coef[i] < 0)
		    break;
	      }
	    else
	      {
		for (e2 = e - 1; e2 >= 0; e2--)
		  if (!isDeadGEQ[e2] && pb->geqs[e2].coef[i] > 0)
		    break;
	      }
	    if (e2 < 0)
	      {
		int e3;
		for (e3 = pb->num_subs - 1; e3 >= 0; e3--)
		  if (pb->subs[e3].coef[i])
		    break;
		if (e3 >= 0)
		  continue;
		for (e3 = pb->num_eqs - 1; e3 >= 0; e3--)
		  if (pb->eqs[e3].coef[i])
		    break;
		if (e3 >= 0)
		  continue;

		if (dump_file && (dump_flags & TDF_DETAILS))
		  fprintf (dump_file, "a free red elimination of %s\n",
			   omega_variable_to_str (pb->var[i]));
		for (; e >= 0; e--)
		  if (pb->geqs[e].coef[i])
		    isDeadGEQ[e] = 1;
		tryAgain = 1;
		isDeadVar[i] = 1;
	      }
	  }
    }

  for (e = pb->num_geqs - 1; e >= 0; e--)
    if (isDeadGEQ[e])
      omega_delete_geq (pb, e, nV);

  for (i = nV; i > 0; i--)
    if (isDeadVar[i])
      omega_delete_variable (pb, i);


  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\nafter free red eliminations:\n");
      omega_print_problem (dump_file, pb);
      fprintf (dump_file, "\n");
    }
}

/* */

static void
omega_substitute_red (omega_pb pb, eqn sub, int i, int c, bool * foundBlack)
{
  int e, j, j0, k;
  int topVar;

  {
    int *p = &packing[0];
    for (k = pb->num_vars; k >= 0; k--)
      if (sub->coef[k])
	*(p++) = k;
    topVar = (p - &packing[0]) - 1;
  }

  *foundBlack = 0;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      if (sub->color)
	fprintf (dump_file, "[");
      fprintf (dump_file, "substituting using %s := ", omega_variable_to_str (pb->var[i]));
      omega_print_term (dump_file, pb, sub, -c);
      if (sub->color)
	fprintf (dump_file, "]");
      fprintf (dump_file, "\n");
      printVars (dump_file, pb);
    }

  for (e = pb->num_eqs - 1; e >= 0; e--)
    {
      eqn eqn = &(pb->eqs[e]);
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
	  omega_print_eq (dump_file, pb, eqn);
	  fprintf (dump_file, "\n");
	}
    }
  for (e = pb->num_geqs - 1; e >= 0; e--)
    {
      eqn eqn = &(pb->geqs[e]);
      k = eqn->coef[i];
      if (k != 0)
	{
	  if (!eqn->color)
	    *foundBlack = 1;
	  else
	    {
	      k = c * k;
	      eqn->touched = true;
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
	  omega_print_geq (dump_file, pb, eqn);
	  fprintf (dump_file, "\n");
	}
    }
  for (e = pb->num_subs - 1; e >= 0; e--)
    {
      eqn eqn = &(pb->subs[e]);
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
	  fprintf (dump_file, "%s := ", omega_variable_to_str (eqn->key));
	  omega_print_term (dump_file, pb, eqn, 1);
	  fprintf (dump_file, "\n");
	}
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "---\n\n");

  if (i <= pb->safe_vars && pb->var[i] >= 0)
    {
      *foundBlack = 1;
    }

}

/* */

void
omega_convert_eq_to_geqs (omega_pb pb, int eq)
{
  int i;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Converting Eq to Gpb->eqs\n");
  omega_copy_eqn (&pb->geqs[pb->num_geqs], &pb->eqs[eq], pb->num_vars);
  pb->geqs[pb->num_geqs].touched = true;
  pb->num_geqs++;
  omega_copy_eqn (&pb->geqs[pb->num_geqs], &pb->eqs[eq], pb->num_vars);
  pb->geqs[pb->num_geqs].touched = true;
  for (i = 0; i <= pb->num_vars; i++)
    pb->geqs[pb->num_geqs].coef[i] = -pb->geqs[pb->num_geqs].coef[i];
  pb->num_geqs++;

  if (dump_file && (dump_flags & TDF_DETAILS))
    omega_print_problem (dump_file, pb);
}

/* */

static void
omega_do_elimination (omega_pb pb, int e, int i)
{
  struct eqn sub;
  int c;
  int nV = pb->num_vars;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "eliminating variable %s\n", omega_variable_to_str (pb->var[i]));

  omega_copy_eqn (&sub, &pb->eqs[e], pb->num_vars);
  c = sub.coef[i];
  sub.coef[i] = 0;
  if (c == 1 || c == -1)
    {
      if (pb->eqs[e].color)
	{
	  bool fB;
	  omega_substitute_red (pb, &sub, i, c, &fB);
	  if (fB)
	    omega_convert_eq_to_geqs (pb, e);
	  else
	    omega_delete_variable (pb, i);
	}
      else
	{
	  omega_substitute (pb, &sub, i, c);
	  omega_delete_variable (pb, i);
	}
    }
  else
    {
      int a = abs (c);
      int e2 = e;

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "performing non-exact elimination, c = %d\n", c);

      for (e = pb->num_eqs - 1; e >= 0; e--)
	if (pb->eqs[e].coef[i])
	  {
	    eqn eqn = &(pb->eqs[e]);
	    int j, k;
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] *= a;
	    k = eqn->coef[i];
	    eqn->coef[i] = 0;
	    eqn->color |= sub.color;
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] -= sub.coef[j] * k / c;
	  }
      for (e = pb->num_geqs - 1; e >= 0; e--)
	if (pb->geqs[e].coef[i])
	  {
	    eqn eqn = &(pb->geqs[e]);
	    int j, k;
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] *= a;
	    k = eqn->coef[i];
	    eqn->coef[i] = 0;
	    eqn->color |= sub.color;
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] -= sub.coef[j] * k / c;
	    eqn->touched = true;
	  }
      for (e = pb->num_subs - 1; e >= 0; e--)
	if (pb->subs[e].coef[i])
	  {
	    eqn eqn = &(pb->subs[e]);
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
	omega_delete_variable (pb, i);
      else
	omega_convert_eq_to_geqs (pb, e2);
    }
}

/* */

static enum omega_result
omega_solve_eq (omega_pb pb, enum omega_result desired_result)
{
  int i, j, e;
  int g, g2;
  g = 0;


  if (dump_file && (dump_flags & TDF_DETAILS) && pb->num_eqs > 0)
    {
      fprintf (dump_file, "\nomega_solve_eq (%d, %d)\n", desired_result, mayBeRed);
      omega_print_problem (dump_file, pb);
      fprintf (dump_file, "\n");
    }

  if (mayBeRed)
    {
      i = 0;
      j = pb->num_eqs - 1;
      while (1)
	{
	  struct eqn eq;
	  while (i <= j && pb->eqs[i].color)
	    i++;
	  while (i <= j && !pb->eqs[j].color)
	    j--;
	  if (i >= j)
	    break;
	  omega_copy_eqn (&eq, &pb->eqs[i], pb->num_vars);
	  omega_copy_eqn (&pb->eqs[i], &pb->eqs[j], pb->num_vars);
	  omega_copy_eqn (&pb->eqs[j], &eq, pb->num_vars);
	  i++;
	  j--;
	}
    }

  /* Eliminate all EQ equations */
  for (e = pb->num_eqs - 1; e >= 0; e--)
    {
      eqn eqn = &(pb->eqs[e]);
      int sv;

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "----\n");

      for (i = pb->num_vars; i > 0; i--)
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
		  omega_print_eq (dump_file, pb, eqn);
		  fprintf (dump_file, "\nequations have no solution \n");
		}

	      return omega_false;
	    }
	  eqn->coef[0] = eqn->coef[0] / g;
	  eqn->coef[i] = 1;
	  pb->num_eqs--;
	  omega_do_elimination (pb, e, i);
	  continue;
	}
      else if (j == -1)
	{
	  if (eqn->coef[0] != 0)
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  omega_print_eq (dump_file, pb, eqn);
		  fprintf (dump_file, "\nequations have no solution \n");
		}

	      return omega_false;
	    }
	  pb->num_eqs--;
	  continue;
	}
      /* i == position of last non-zero coef */
      /* g == coef of i */
      /* j == position of next non-zero coef */

      if (g < 0)
	g = -g;
      if (g == 1)
	{
	  pb->num_eqs--;
	  omega_do_elimination (pb, e, i);
	}
      else
	{
	  int k = j;
	  bool promotionPossible =
	    (j <= pb->safe_vars && pb->safe_vars + 1 == i
	     && !omega_eqn_is_red (eqn, desired_result) && !inApproximateMode);
	  if (dump_file && (dump_flags & TDF_DETAILS) && promotionPossible)
	    fprintf (dump_file, " Promotion possible\n");
	normalizeEQ:
	  if (j > pb->safe_vars)
	    {
	      for (; g != 1 && j > pb->safe_vars; j--)
		g = gcd (abs (eqn->coef[j]), g);
	      g2 = g;
	    }
	  else if (i > pb->safe_vars)
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
		      omega_print_eq (dump_file, pb, eqn);
		      fprintf (dump_file, "\nequations have no solution \n");
		    }
		  return omega_false;
		}

	      for (j = 0; j <= pb->num_vars; j++)
		eqn->coef[j] /= g;
	      g2 = g2 / g;
	    }
	  if (g2 > 1)
	    {
	      int e2;
	      for (e2 = e - 1; e2 >= 0; e2--)
		if (pb->eqs[e2].coef[i])
		  break;
	      if (e2 == -1)
		for (e2 = pb->num_geqs - 1; e2 >= 0; e2--)
		  if (pb->geqs[e2].coef[i])
		    break;
	      if (e2 == -1)
		for (e2 = pb->num_subs - 1; e2 >= 0; e2--)
		  if (pb->subs[e2].coef[i])
		    break;
	      if (e2 == -1)
		{
		  bool change = 0;

		  if (dump_file && (dump_flags & TDF_DETAILS))
		    {
		      fprintf (dump_file, "Ha! We own it! \n");
		      omega_print_eq (dump_file, pb, eqn);
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
		      omega_name_wild_card (pb, i);

		      if (dump_file && (dump_flags & TDF_DETAILS))
			{
			  omega_print_eq (dump_file, pb, eqn);
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
			   omega_variable_to_str (pb->var[i]));
		  printVars (dump_file, pb);
		}
	      pb->safe_vars++;
	      if (pb->var[i] > 0)
		omega_name_wild_card (pb, i);
	      promotionPossible = 0;
	      j = k;
	      goto normalizeEQ;
	    }

	  if (g2 > 1 && !inApproximateMode)
	    {
	      if (pb->eqs[e].color)
		{
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    fprintf (dump_file, "handling red equality\n");

		  pb->num_eqs--;
		  omega_do_elimination (pb, e, i);
		  continue;
		}
	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  fprintf (dump_file,
			   "adding equation to handle safe variable \n");
		  omega_print_eq (dump_file, pb, eqn);
		  fprintf (dump_file, "\n----\n");
		  omega_print_problem (dump_file, pb);
		  fprintf (dump_file, "\n----\n");
		  fprintf (dump_file, "\n----\n");
		}

	      i = omega_add_new_wild_card (pb);
	      pb->num_eqs++;
	      gcc_assert (pb->num_eqs <= max_eqs);
	      omega_init_eqn_zero (&pb->eqs[e + 1], pb->num_vars);
	      omega_copy_eqn (&pb->eqs[e + 1], eqn, pb->safe_vars);
	      for (j = pb->num_vars; j >= 0; j--)
		{
		  pb->eqs[e + 1].coef[j] = int_mod (pb->eqs[e + 1].coef[j], g2);
		  if (2 * pb->eqs[e + 1].coef[j] >= g2)
		    pb->eqs[e + 1].coef[j] -= g2;
		}
	      pb->eqs[e + 1].coef[i] = g2;
	      e += 2;

	      if (dump_file && (dump_flags & TDF_DETAILS))
		omega_print_problem (dump_file, pb);

	      continue;
	    }

	  sv = pb->safe_vars;
	  if (g2 == 0)
	    sv = 0;

	  /* find variable to eliminate */
	  if (g2 > 1)
	    {
	      gcc_assert (inApproximateMode);

	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  fprintf (dump_file, "non-exact elimination: ");
		  omega_print_eq (dump_file, pb, eqn);
		  fprintf (dump_file, "\n");
		  omega_print_problem (dump_file, pb);
		}

	      for (i = pb->num_vars; i > sv; i--)
		if (pb->eqs[e].coef[i] != 0)
		  break;
	    }
	  else
	    for (i = pb->num_vars; i > sv; i--)
	      if (pb->eqs[e].coef[i] == 1 || pb->eqs[e].coef[i] == -1)
		break;

	  if (i > sv)
	    {
	      pb->num_eqs--;
	      omega_do_elimination (pb, e, i);

	      if (dump_file && (dump_flags & TDF_DETAILS) && g2 > 1)
		{
		  fprintf (dump_file, "result of non-exact elimination:\n");
		  omega_print_problem (dump_file, pb);
		}
	    }
	  else
	    {
	      int factor = (INT_MAX);
	      j = 0;

	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "doing moding\n");

	      for (i = pb->num_vars; i != sv; i--)
		if ((pb->eqs[e].coef[i] & 1) != 0)
		  {
		    j = i;
		    i--;
		    for (; i != sv; i--)
		      if ((pb->eqs[e].coef[i] & 1) != 0)
			break;
		    break;
		  }
	      if (j != 0 && i == sv)
		{
		  omega_do_mod (pb, 2, e, j);
		  e++;
		  continue;
		}

	      j = 0;
	      for (i = pb->num_vars; i != sv; i--)
		if (pb->eqs[e].coef[i] != 0 && factor > abs (pb->eqs[e].coef[i]) + 1)
		  {
		    factor = abs (pb->eqs[e].coef[i]) + 1;
		    j = i;
		  }
	      if (j == sv)
		{
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    fprintf (dump_file, "should not have happened\n");
		  exit (2);
		}
	      omega_do_mod (pb, factor, e, j);
	      /* go back and try this equation again */
	      e++;
	    }
	}


    }

  pb->num_eqs = 0;
  return omega_unknown;
}

/* */

static enum omega_result
parallel_splinter (omega_pb pb, int e, int diff,
		   enum omega_result desired_result)
{
  omega_pb tmp_problem;
  int i;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Using parallel splintering\n");
      omega_print_problem (dump_file, pb);
    }

  tmp_problem = (omega_pb) xmalloc (sizeof (struct omega_pb));
  omega_copy_eqn (&pb->eqs[0], &pb->geqs[e], pb->num_vars);
  pb->num_eqs = 1;

  for (i = 0; i <= diff; i++)
    {
      omega_copy_problem (tmp_problem, pb);

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Splinter # %i\n", i);
	  omega_print_problem (dump_file, pb);
	}

      if (omega_solve_problem (tmp_problem, desired_result))
	{
	  free (tmp_problem);
	  return omega_true;
	}
      pb->eqs[0].coef[0]--;
    }

  free (tmp_problem);
  return omega_false;
}

/* */

static enum omega_result
omega_solve_geq (omega_pb pb, enum omega_result desired_result)
{
  int i, j, k, e;
  int nV, fv;
  int result;
  int coupled_subscripts = 0;
  int eliminate_again;
  int smoothed = 0;
  int tried_eliminating_redundant = 0;
  j = 0;

  if (desired_result != omega_simplify)
    {
      pb->num_subs = 0;
      pb->safe_vars = 0;
    }

 solve_geq_start:
  while (1)
    {
      gcc_assert (desired_result == omega_simplify || pb->num_subs == 0);
      if (pb->num_geqs > max_geqs)
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "TOO MANY EQUATIONS GENERATED\n");
	  exit (2);
	}
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\nomega_solve_geq (%d,%d):\n",
		   desired_result, please_no_equalities_in_simplified_problems);
	  omega_print_problem (dump_file, pb);
	  fprintf (dump_file, "\n");
	}

      nV = pb->num_vars;

      if (nV == 1)
	{
	  int u_color = omega_black;
	  int l_color = omega_black;
	  int upper_bound = pos_infinity;
	  int lower_bound = neg_infinity;

	  for (e = pb->num_geqs - 1; e >= 0; e--)
	    {
	      int a = pb->geqs[e].coef[1];
	      int c = pb->geqs[e].coef[0];
	      /* our equation is ax + c >= 0, or ax >= -c, or c >= -ax */
	      if (a == 0)
		{
		  if (c < 0)
		    {
		      if (dump_file && (dump_flags & TDF_DETAILS))
			fprintf (dump_file, "equations have no solution \n");
		      return omega_false;
		    }
		}
	      else if (a > 0)
		{
		  if (a != 1)
		    c = int_div (c, a);
		  if (lower_bound < -c ||
		      (lower_bound == -c && !omega_eqn_is_red (&pb->geqs[e], desired_result)))
		    {
		      lower_bound = -c;
		      l_color = pb->geqs[e].color;
		    }
		}
	      else
		{
		  if (a != -1)
		    c = int_div (c, -a);
		  if (upper_bound > c ||
		      (upper_bound == c && !omega_eqn_is_red (&pb->geqs[e], desired_result)))
		    {
		      upper_bound = c;
		      u_color = pb->geqs[e].color;
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
	      return omega_false;
	    }
	  if (desired_result == omega_simplify)
	    {
	      pb->num_geqs = 0;
	      if (pb->safe_vars == 1)
		{

		  if (lower_bound == upper_bound && !u_color && !l_color)
		    {
		      pb->eqs[0].coef[0] = -lower_bound;
		      pb->eqs[0].coef[1] = 1;
		      pb->eqs[0].color = 0;
		      pb->num_eqs = 1;
		      return omega_solve_problem (pb, desired_result);
		    }
		  else
		    {
		      if (lower_bound > neg_infinity)
			{
			  pb->geqs[0].coef[0] = -lower_bound;
			  pb->geqs[0].coef[1] = 1;
			  pb->geqs[0].key = 1;
			  pb->geqs[0].color = l_color;
			  pb->geqs[0].touched = false;
			  pb->num_geqs = 1;
			}
		      if (upper_bound < pos_infinity)
			{
			  pb->geqs[pb->num_geqs].coef[0] = upper_bound;
			  pb->geqs[pb->num_geqs].coef[1] = -1;
			  pb->geqs[pb->num_geqs].key = -1;
			  pb->geqs[pb->num_geqs].color = u_color;
			  pb->geqs[pb->num_geqs].touched = false;
			  pb->num_geqs++;
			}
		    }
		}
	      else
		pb->num_vars = 0;
	      omega_problem_reduced (pb);
	      return omega_false;
	    }
	  if (original_problem != no_problem && !l_color && !u_color
	      && !conservative && lower_bound == upper_bound)
	    {
	      pb->eqs[0].coef[0] = -lower_bound;
	      pb->eqs[0].coef[1] = 1;
	      pb->num_eqs = 1;
	      adding_equality_constraint (pb, 0);
	    }
	  return omega_true;
	}

      if (!pb->variablesFreed)
	{
	  pb->variablesFreed = 1;
	  if (desired_result != omega_simplify)
	    omega_free_eliminations (pb, 0);
	  else
	    omega_free_eliminations (pb, pb->safe_vars);
	  nV = pb->num_vars;
	  if (nV == 1)
	    continue;
	}


      switch (normalize_omega_problem (pb))
	{
	case normalize_false:
	  return omega_false;
	  break;
	case normalize_coupled:
	  coupled_subscripts = TRUE;
	  break;
	case normalize_uncoupled:
	  coupled_subscripts = FALSE;
	  break;
	}

      nV = pb->num_vars;

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "\nafter normalization:\n");
	  omega_print_problem (dump_file, pb);
	  fprintf (dump_file, "\n");
	  fprintf (dump_file,
		   "eliminating variable using fourier-motzkin elimination\n");
	}

      do
	{
	  eliminate_again = 0;

	  if (pb->num_eqs > 0)
	    return omega_solve_problem (pb, desired_result);

	  if (!coupled_subscripts)
	    {
	      if (pb->safe_vars == 0)
		pb->num_geqs = 0;
	      else
		for (e = pb->num_geqs - 1; e >= 0; e--)
		  if (pb->geqs[e].key > pb->safe_vars || -pb->safe_vars > pb->geqs[e].key)
		    omega_delete_geq (pb, e, nV);
	      pb->num_vars = pb->safe_vars;
	      if (desired_result == omega_simplify)
		{
		  omega_problem_reduced (pb);
		  return omega_false;
		}
	      return omega_true;
	    }

	  if (desired_result != omega_simplify)
	    fv = 0;
	  else
	    fv = pb->safe_vars;

	  if (pb->num_geqs == 0)
	    {
	      if (desired_result == omega_simplify)
		{
		  pb->num_vars = pb->safe_vars;
		  omega_problem_reduced (pb);
		  return omega_false;
		}
	      return omega_true;
	    }
	  if (desired_result == omega_simplify && nV == pb->safe_vars)
	    {
	      omega_problem_reduced (pb);
	      return omega_false;
	    }


	  if (pb->num_geqs > max_geqs - 30 || pb->num_geqs > 2 * nV * nV + 4 * nV + 10)
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file,
			 "TOO MANY EQUATIONS; %d equations, %d variables, ELIMINATING REDUNDANT ONES\n",
			 pb->num_geqs, nV);
	      if (!omega_eliminate_redundant (pb, 0))
		return omega_false;
	      nV = pb->num_vars;
	      if (pb->num_eqs > 0)
		return omega_solve_problem (pb, desired_result);
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file,
			 "END ELIMINATION OF REDUNDANT EQUATIONS\n");
	    }



	  {
	    int parallel_difference = INT_MAX;
	    int best_parallel_eqn = -1;
	    int minC, maxC, minCj;
	    int lower_bound_count;
	    int e2, Le, Ue;
	    bool possible_easy_int_solution;
	    int max_splinters = 1;
	    bool exact = false;
	    bool luckyExact = false;
	    int neweqns = 0;
	    minCj = 0;
	    Le = 0;
	    lower_bound_count = 0;

	    if (desired_result != omega_simplify)
	      fv = 0;
	    else
	      fv = pb->safe_vars;

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
		  bool lucky = false;

		  minC = maxC = 0;
		  lower_bound_count = 0;
		  upper_bound_count = 0;

		  for (e = pb->num_geqs - 1; e >= 0; e--)
		    if (pb->geqs[e].coef[i] < 0)
		      {
			set_min (&minC, pb->geqs[e].coef[i]);
			upper_bound_count++;
			if (pb->geqs[e].coef[i] < -1)
			  {
			    if (ub == -2)
			      ub = e;
			    else
			      ub = -1;
			  }
		      }
		    else if (pb->geqs[e].coef[i] > 0)
		      {
			set_max (&maxC, pb->geqs[e].coef[i]);
			lower_bound_count++;
			Le = e;
			if (pb->geqs[e].coef[i] > 1)
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

		  if (ub >= 0 && lb >= 0 && pb->geqs[lb].key == -pb->geqs[ub].key)
		    {
		      int Lc = pb->geqs[lb].coef[i];
		      int Uc = -pb->geqs[ub].coef[i];
		      int diff =
			Lc * pb->geqs[ub].coef[0] + Uc * pb->geqs[lb].coef[0];
		      lucky = (diff >= (Uc - 1) * (Lc - 1));
		    }

		  if (maxC == 1 || minC == -1 || lucky || APROX
		      || inApproximateMode)
		    {
		      neweqns = score = upper_bound_count * lower_bound_count;

		      if (dump_file && (dump_flags & TDF_DETAILS))
			fprintf (dump_file,
				 "For %s, exact, score = %d*%d, range = %d ... %d, \nlucky = %d, APROX = %d, inApproximateMode=%d \n",
				 omega_variable_to_str (pb->var[i]), upper_bound_count,
				 lower_bound_count, minC, maxC, lucky, APROX,
				 inApproximateMode);

		      if (!exact || score < best)
			{

			  best = score;
			  j = i;
			  minCj = minC;
			  jLe = Le;
			  jLowerBoundCount = lower_bound_count;
			  exact = true;
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
				 omega_variable_to_str (pb->var[i]), upper_bound_count,
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
		  omega_free_eliminations (pb, pb->safe_vars);
		  nV = pb->num_vars;
		  eliminate_again = 1;
		  continue;
		}

	      i = j;
	      minC = minCj;
	      Le = jLe;
	      lower_bound_count = jLowerBoundCount;

	      for (e = pb->num_geqs - 1; e >= 0; e--)
		if (pb->geqs[e].coef[i] > 0)
		  {
		    if (pb->geqs[e].coef[i] == -minC)
		      max_splinters += -minC - 1;
		    else
		      max_splinters +=
			check_pos_mul ((pb->geqs[e].coef[i] - 1),
				       (-minC - 1)) / (-minC) + 1;
		  }
	    }

#ifdef Omega3
	    if (!exact && !tried_eliminating_redundant)
	      {
		if (dump_file && (dump_flags & TDF_DETAILS))
		  fprintf (dump_file,
			   "Trying to produce exact elimination by finding redundant constraints\n");
		omega_eliminate_redundant (pb, 0);
		if (dump_file && (dump_flags & TDF_DETAILS))
		  fprintf (dump_file,
			   "Done trying to produce exact elimination by finding redundant constraints\n");
		tried_eliminating_redundant = 1;
		eliminate_again = 1;
		continue;
	      }
	    tried_eliminating_redundant = 0;
#endif

	    if (returnSingleResult && desired_result == omega_simplify && !exact)
	      {
		nonConvex = 1;
		omega_problem_reduced (pb);
		return omega_true;
	      }

#ifndef Omega3
	    if (!exact && !tried_eliminating_redundant)
	      {
		if (dump_file && (dump_flags & TDF_DETAILS))
		  fprintf (dump_file,
			   "Trying to produce exact elimination by finding redundant constraints\n");
		omega_eliminate_redundant (pb, 0);
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

		for (e1 = pb->num_geqs - 1; e1 >= 0; e1--)
		  if (!pb->geqs[e1].color)
		    for (e2 = e1 - 1; e2 >= 0; e2--)
		      if (!pb->geqs[e2].color
			  && pb->geqs[e1].key == -pb->geqs[e2].key
			  && ((pb->geqs[e1].coef[0] + pb->geqs[e2].coef[0])
			      * (3 - single_var_geq (pb->geqs[e1], pb->num_vars))
			      / 2 < parallel_difference))
			{
			  parallel_difference =
			    (pb->geqs[e1].coef[0] + pb->geqs[e2].coef[0])
			    * (3 - single_var_geq (pb->geqs[e1], pb->num_vars))
			    / 2;
			  best_parallel_eqn = e1;
			}

		if (dump_file && (dump_flags & TDF_DETAILS)
		    && best_parallel_eqn >= 0)
		  {
		    fprintf (dump_file,
			     "Possible parallel projection, diff = %d, in ",
			     parallel_difference);
		    omega_print_geq (dump_file, pb, &(pb->geqs[best_parallel_eqn]));
		    fprintf (dump_file, "\n");
		    omega_print_problem (dump_file, pb);
		  }
	      }

	    if (dump_file && (dump_flags & TDF_DETAILS))
	      {
		fprintf (dump_file, "going to eliminate %s, (%d,%d,%d)\n",
			 omega_variable_to_str (pb->var[i]), i, minC, lower_bound_count);
		omega_print_problem (dump_file, pb);
		if (luckyExact)
		  fprintf (dump_file, "(a lucky exact elimination)\n");
		else if (exact)
		  fprintf (dump_file, "(an exact elimination)\n");
		fprintf (dump_file, "Max # of splinters = %d\n",
			 max_splinters);
	      }

	    gcc_assert (max_splinters >= 1);

	    if (!exact && desired_result == omega_simplify && best_parallel_eqn >= 0
		&& parallel_difference <= max_splinters)
	      return parallel_splinter (pb, best_parallel_eqn,
					parallel_difference, desired_result);

	    smoothed = 0;

	    if (i != nV)
	      {
		int t;
		j = pb->num_vars;

		if (dump_file && (dump_flags & TDF_DETAILS))
		  {
		    fprintf (dump_file, "Swapping %d and %d\n", i, j);
		    omega_print_problem (dump_file, pb);
		  }
		swap (&pb->var[i], &pb->var[j]);
		for (e = pb->num_geqs - 1; e >= 0; e--)
		  if (pb->geqs[e].coef[i] != pb->geqs[e].coef[j])
		    {
		      pb->geqs[e].touched = true;
		      t = pb->geqs[e].coef[i];
		      pb->geqs[e].coef[i] = pb->geqs[e].coef[j];
		      pb->geqs[e].coef[j] = t;
		    }
		for (e = pb->num_subs - 1; e >= 0; e--)
		  if (pb->subs[e].coef[i] != pb->subs[e].coef[j])
		    {
		      t = pb->subs[e].coef[i];
		      pb->subs[e].coef[i] = pb->subs[e].coef[j];
		      pb->subs[e].coef[j] = t;
		    }
		if (dump_file && (dump_flags & TDF_DETAILS))
		  {
		    fprintf (dump_file, "Swapping complete \n");
		    omega_print_problem (dump_file, pb);
		    fprintf (dump_file, "\n");
		  }

		i = j;
	      }
	    else if (dump_file && (dump_flags & TDF_DETAILS))
	      {
		fprintf (dump_file, "No swap needed\n");
		omega_print_problem (dump_file, pb);
	      }

	    pb->num_vars--;
	    nV = pb->num_vars;

	    if (exact)
	      {
		if (nV == 1)
		  {
		    int upper_bound = pos_infinity;
		    int lower_bound = neg_infinity;
		    int ub_color = 0;
		    int lb_color = 0;
		    int constantTerm, coefficient;
		    int topeqn = pb->num_geqs - 1;
		    int Lc = pb->geqs[Le].coef[i];

		    for (Le = topeqn; Le >= 0; Le--)
		      if ((Lc = pb->geqs[Le].coef[i]) == 0)
			{
			  if (pb->geqs[Le].coef[1] == 1)
			    {
			      constantTerm = -pb->geqs[Le].coef[0];
			      if (constantTerm > lower_bound ||
				  (constantTerm == lower_bound &&
				   !omega_eqn_is_red (&pb->geqs[Le], desired_result)))
				{
				  lower_bound = constantTerm;
				  lb_color = pb->geqs[Le].color;
				}

			      if (dump_file && (dump_flags & TDF_DETAILS))
				{
				  if (pb->geqs[Le].color == omega_black)
				    fprintf (dump_file, " :::=> %s >= %d\n",
					     omega_variable_to_str (pb->var[1]), constantTerm);
				  else
				    fprintf (dump_file,
					     " :::=> [%s >= %d]\n",
					     omega_variable_to_str (pb->var[1]), constantTerm);
				}
			    }
			  else
			    {
			      constantTerm = pb->geqs[Le].coef[0];
			      if (constantTerm < upper_bound ||
				  (constantTerm == upper_bound
				   && !omega_eqn_is_red (&pb->geqs[Le], desired_result)))
				{
				  upper_bound = constantTerm;
				  ub_color = pb->geqs[Le].color;
				}

			      if (dump_file && (dump_flags & TDF_DETAILS))
				{
				  if (pb->geqs[Le].color == omega_black)
				    fprintf (dump_file, " :::=> %s <= %d\n",
					     omega_variable_to_str (pb->var[1]), constantTerm);
				  else
				    fprintf (dump_file,
					     " :::=> [%s <= %d]\n",
					     omega_variable_to_str (pb->var[1]), constantTerm);
				}
			    }
			}
		      else if (Lc > 0)
			{
			  for (Ue = topeqn; Ue >= 0; Ue--)
			    if (pb->geqs[Ue].coef[i] < 0)
			      {
				if (pb->geqs[Le].key != -pb->geqs[Ue].key)
				  {
				    int Uc = -pb->geqs[Ue].coef[i];
				    coefficient =
				      pb->geqs[Ue].coef[1] * Lc +
				      pb->geqs[Le].coef[1] * Uc;
				    constantTerm =
				      pb->geqs[Ue].coef[0] * Lc +
				      pb->geqs[Le].coef[0] * Uc;

				    if (dump_file && (dump_flags & TDF_DETAILS))
				      {
					omega_print_geq_extra (dump_file, pb, &(pb->geqs[Ue]));
					fprintf (dump_file, "\n");
					omega_print_geq_extra (dump_file, pb, &(pb->geqs[Le]));
					fprintf (dump_file, "\n");
				      }

				    if (coefficient > 0)
				      {
					constantTerm =
					  -(int_div
					    (constantTerm, coefficient));
					/* gcc_assert(omega_black == 0) */
					if (constantTerm > lower_bound ||
					    (constantTerm == lower_bound &&
					     (desired_result != omega_simplify ||
					      (pb->geqs[Ue].color == omega_black
					       && pb->geqs[Le].color == omega_black))))
					  {
					    lower_bound = constantTerm;
					    lb_color = pb->geqs[Ue].color
					      || pb->geqs[Le].color;
					  }

					if (dump_file && (dump_flags & TDF_DETAILS))
					  {
					    if (pb->geqs[Ue].color
						|| pb->geqs[Le].color)
					      fprintf (dump_file,
						       " ::=> [%s >= %d]\n",
						       omega_variable_to_str (pb->var[1]),
						       constantTerm);
					    else
					      fprintf (dump_file,
						       " ::=> %s >= %d\n",
						       omega_variable_to_str (pb->var[1]),
						       constantTerm);
					  }
				      }
				    else
				      {
					constantTerm =
					  int_div (constantTerm, -coefficient);
					if (constantTerm < upper_bound
					    || (constantTerm == upper_bound
						&& pb->geqs[Ue].color == omega_black
						&& pb->geqs[Le].color == omega_black))
					  {
					    upper_bound = constantTerm;
					    ub_color = pb->geqs[Ue].color
					      || pb->geqs[Le].color;
					  }

					if (dump_file && (dump_flags & TDF_DETAILS))
					  {
					    if (pb->geqs[Ue].color
						|| pb->geqs[Le].color)
					      fprintf (dump_file,
						       " ::=> [%s <= %d]\n",
						       omega_variable_to_str (pb->var[1]),
						       constantTerm);
					    else
					      fprintf (dump_file,
						       " ::=> %s <= %d\n",
						       omega_variable_to_str (pb->var[1]),
						       constantTerm);
					  }
				      }
				  }
			      }
			}
		    pb->num_geqs = 0;

		    if (dump_file && (dump_flags & TDF_DETAILS))
		      fprintf (dump_file,
			       " therefore, %c%d <= %c%s%c <= %d%c\n",
			       lb_color ? '[' : ' ', lower_bound,
			       (lb_color && !ub_color) ? ']' : ' ',
			       omega_variable_to_str (pb->var[1]),
			       (!lb_color && ub_color) ? '[' : ' ',
			       upper_bound, ub_color ? ']' : ' ');
		    if (lower_bound > upper_bound)
		      return omega_false;
		    if (pb->safe_vars == 1)
		      {
			if (upper_bound == lower_bound
			    && !(ub_color | lb_color)
			    && !please_no_equalities_in_simplified_problems)
			  {
			    pb->num_eqs++;
			    pb->eqs[0].coef[1] = -1;
			    pb->eqs[0].coef[0] = upper_bound;
			    pb->eqs[0].color = ub_color | lb_color;
			    if (desired_result == omega_simplify && !pb->eqs[0].color)
			      return omega_solve_problem (pb, desired_result);
			  }
			if (upper_bound != pos_infinity)
			  {
			    pb->geqs[0].coef[1] = -1;
			    pb->geqs[0].coef[0] = upper_bound;
			    pb->geqs[0].color = ub_color;
			    pb->geqs[0].key = -1;
			    pb->geqs[0].touched = false;
			    pb->num_geqs++;
			  }
			if (lower_bound != neg_infinity)
			  {
			    pb->geqs[pb->num_geqs].coef[1] = 1;
			    pb->geqs[pb->num_geqs].coef[0] = -lower_bound;
			    pb->geqs[pb->num_geqs].color = lb_color;
			    pb->geqs[pb->num_geqs].key = 1;
			    pb->geqs[pb->num_geqs].touched = false;
			    pb->num_geqs++;
			  }
		      }
		    if (desired_result == omega_simplify)
		      {
			omega_problem_reduced (pb);
			return omega_false;
		      }
		    else
		      {
			if (!conservative &&
			    (desired_result != omega_simplify ||
			     (!lb_color && !ub_color))
			    && original_problem != no_problem
			    && lower_bound == upper_bound)
			  {
			    for (i = original_problem->num_vars; i >= 0; i--)
			      if (original_problem->var[i] ==
				  pb->var[1])
				break;
			    if (i == 0)
			      break;
			    e = original_problem->num_eqs++;
			    omega_init_eqn_zero (&original_problem->eqs[e],
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
			return omega_true;
		      }
		  }
		eliminate_again = 1;

		if (lower_bound_count == 1)
		  {
		    struct eqn lbeqn;
		    int Lc = pb->geqs[Le].coef[i];

		    if (dump_file && (dump_flags & TDF_DETAILS))
		      fprintf (dump_file, "an inplace elimination\n");

		    omega_copy_eqn (&lbeqn, &pb->geqs[Le], (nV + 1));
		    omega_delete_geq_extra (pb, Le, nV + 1);
		    for (Ue = pb->num_geqs - 1; Ue >= 0; Ue--)
		      if (pb->geqs[Ue].coef[i] < 0)
			{
			  if (lbeqn.key == -pb->geqs[Ue].key)
			    omega_delete_geq_extra (pb, Ue, nV + 1);
			  else
			    {
			      int Uc = -pb->geqs[Ue].coef[i];
			      pb->geqs[Ue].touched = true;
			      pb->geqs[Ue].color |= lbeqn.color;
			      eliminate_again = 0;
			      for (k = 0; k <= nV; k++)
				pb->geqs[Ue].coef[k] =
				  check_mul (pb->geqs[Ue].coef[k],
					    Lc) + check_mul (lbeqn.coef[k],
							    Uc);
			      if (dump_file && (dump_flags & TDF_DETAILS))
				{
				  omega_print_geq (dump_file, pb, &(pb->geqs[Ue]));
				  fprintf (dump_file, "\n");
				}
			    }
			}
		    continue;
		  }
		else
		  {
		    int dead_eqns[max_geqs];
		    int num_dead = 0;
		    int top_eqn = pb->num_geqs - 1;
		    lower_bound_count--;

		    if (dump_file && (dump_flags & TDF_DETAILS))
		      fprintf (dump_file, "lower bound count = %d\n",
			       lower_bound_count);

		    for (Le = top_eqn; Le >= 0; Le--)
		      if (pb->geqs[Le].coef[i] > 0)
			{
			  int Lc = pb->geqs[Le].coef[i];
			  for (Ue = top_eqn; Ue >= 0; Ue--)
			    if (pb->geqs[Ue].coef[i] < 0)
			      {
				if (pb->geqs[Le].key != -pb->geqs[Ue].key)
				  {
				    int Uc = -pb->geqs[Ue].coef[i];
				    if (num_dead == 0)
				      e2 = pb->num_geqs++;
				    else
				      e2 = dead_eqns[--num_dead];
				    gcc_assert (e2 < max_geqs);

				    if (dump_file && (dump_flags & TDF_DETAILS))
				      {
					fprintf (dump_file,
						 "Le = %d, Ue = %d, gen = %d\n",
						 Le, Ue, e2);
					omega_print_geq_extra (dump_file, pb, &(pb->geqs[Le]));
					fprintf (dump_file, "\n");
					omega_print_geq_extra (dump_file, pb, &(pb->geqs[Ue]));
					fprintf (dump_file, "\n");
				      }
				    eliminate_again = 0;

				    for (k = nV; k >= 0; k--)
				      pb->geqs[e2].coef[k] =
					check_mul (pb->geqs[Ue].coef[k],
						  Lc) +
					check_mul (pb->geqs[Le].coef[k], Uc);
				    pb->geqs[e2].coef[nV + 1] = 0;
				    pb->geqs[e2].touched = true;
				    pb->geqs[e2].color =
				      pb->geqs[Ue].color | pb->geqs[Le].color;

				    if (dump_file && (dump_flags & TDF_DETAILS))
				      {
					omega_print_geq (dump_file, pb, &(pb->geqs[e2]));
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
		      for (e = pb->num_geqs - 1; e >= 0; e--)
			isDead[e] = FALSE;
		      while (num_dead > 0)
			{
			  e = dead_eqns[--num_dead];
			  isDead[e] = TRUE;
			}
		      for (e = pb->num_geqs - 1; e >= 0; e--)
			if (isDead[e])
			  omega_delete_geq_extra (pb, e, nV + 1);
		    }
		    continue;
		  }
	      }
	    else
	      {
		omega_pb rS, iS;

		rS = (omega_pb) xmalloc (sizeof (struct omega_pb));
		omega_initialize_problem (rS);
		iS = (omega_pb) xmalloc (sizeof (struct omega_pb));
		omega_initialize_problem (iS);

		e2 = 0;
		possible_easy_int_solution = true;
		for (e = 0; e < pb->num_geqs; e++)
		  if (pb->geqs[e].coef[i] == 0)
		    {
		      omega_copy_eqn (&(rS->geqs[e2]), &pb->geqs[e], pb->num_vars);
		      omega_copy_eqn (&(iS->geqs[e2]), &pb->geqs[e], pb->num_vars);

		      if (dump_file && (dump_flags & TDF_DETAILS))
			{
			  int t;
			  fprintf (dump_file, "Copying (%d, %d): ", i,
				   pb->geqs[e].coef[i]);
			  omega_print_geq_extra (dump_file, pb, &pb->geqs[e]);
			  fprintf (dump_file, "\n");
			  for (t = 0; t <= nV + 1; t++)
			    fprintf (dump_file, "%d ", pb->geqs[e].coef[t]);
			  fprintf (dump_file, "\n");

			}
		      e2++;
		      gcc_assert (e2 < max_geqs);
		    }
		for (Le = pb->num_geqs - 1; Le >= 0; Le--)
		  if (pb->geqs[Le].coef[i] > 0)
		    {
		      int Lc = pb->geqs[Le].coef[i];
		      for (Ue = pb->num_geqs - 1; Ue >= 0; Ue--)
			if (pb->geqs[Ue].coef[i] < 0)
			  {
			    if (pb->geqs[Le].key != -pb->geqs[Ue].key)
			      {
				int Uc = -pb->geqs[Ue].coef[i];
				rS->geqs[e2].touched =
				  iS->geqs[e2].touched = true;

				if (dump_file && (dump_flags & TDF_DETAILS))
				  {
				    fprintf (dump_file, "---\n");
				    fprintf (dump_file,
					     "Le(Lc) = %d(%d_, Ue(Uc) = %d(%d), gen = %d\n",
					     Le, Lc, Ue, Uc, e2);
				    omega_print_geq_extra (dump_file, pb, &pb->geqs[Le]);
				    fprintf (dump_file, "\n");
				    omega_print_geq_extra (dump_file, pb, &pb->geqs[Ue]);
				    fprintf (dump_file, "\n");
				  }

				if (Uc == Lc)
				  {
				    for (k = nV; k >= 0; k--)
				      iS->geqs[e2].coef[k] =
					rS->geqs[e2].coef[k] =
					pb->geqs[Ue].coef[k] + pb->geqs[Le].coef[k];
				    iS->geqs[e2].coef[0] -= (Uc - 1);
				  }
				else
				  {
				    for (k = nV; k >= 0; k--)
				      iS->geqs[e2].coef[k] =
					rS->geqs[e2].coef[k] =
					check_mul (pb->geqs[Ue].coef[k],
						  Lc) +
					check_mul (pb->geqs[Le].coef[k], Uc);
				    iS->geqs[e2].coef[0] -=
				      (Uc - 1) * (Lc - 1);
				  }

				iS->geqs[e2].color = rS->geqs[e2].color
				  = pb->geqs[Ue].color || pb->geqs[Le].color;

				if (dump_file && (dump_flags & TDF_DETAILS))
				  {
				    omega_print_geq (dump_file, pb, &(rS->geqs[e2]));
				    fprintf (dump_file, "\n");
				  }
				e2++;
				gcc_assert (e2 < max_geqs);
			      }
			    else
			      {
				int Uc = -pb->geqs[Ue].coef[i];
				if (pb->geqs[Ue].coef[0] * Lc +
				    pb->geqs[Le].coef[0] * Uc - (Uc - 1) * (Lc -
									1) <
				    0)
				  possible_easy_int_solution = false;
			      }
			  }

		    }
		iS->variablesInitialized = rS->variablesInitialized = 1;
		iS->num_vars = rS->num_vars = pb->num_vars;
		iS->num_geqs = rS->num_geqs = e2;
		iS->num_eqs = rS->num_eqs = 0;
		iS->num_subs = rS->num_subs = pb->num_subs;
		iS->safe_vars = rS->safe_vars = pb->safe_vars;
		{
		  int t;
		  for (t = nV; t >= 0; t--)
		    rS->var[t] = pb->var[t];
		  for (t = nV; t >= 0; t--)
		    iS->var[t] = pb->var[t];
		  for (e = pb->num_subs - 1; e >= 0; e--)
		    {
		      omega_copy_eqn (&(rS->subs[e]), &(pb->subs[e]), pb->num_vars);
		      omega_copy_eqn (&(iS->subs[e]), &(pb->subs[e]), pb->num_vars);
		    }
		}
		pb->num_vars++;
		nV = pb->num_vars;
		if (desired_result != omega_true)
		  {
		    int t = trace;

		    if (dump_file && (dump_flags & TDF_DETAILS))
		      fprintf (dump_file, "\nreal solution(%d):\n", depth);

		    depth++;
		    trace = 0;
		    if (original_problem == no_problem)
		      {
			original_problem = pb;
			result = omega_solve_geq (rS, omega_false);
			original_problem = no_problem;
		      }
		    else
		      result = omega_solve_geq (rS, omega_false);
		    trace = t;
		    depth--;
		    if (result == omega_false)
		      {
			free (rS);
			free (iS);
			return result;
		      }

		    if (pb->num_eqs > 0)
		      {
			/* An equality constraint must have been found */
			free (rS);
			free (iS);
			return omega_solve_problem (pb, desired_result);
		      }
		  }
		if (desired_result != omega_false)
		  {
		    if (possible_easy_int_solution)
		      {
			if (dump_file && (dump_flags & TDF_DETAILS))
			  fprintf (dump_file, "\ninteger solution(%d):\n",
				   depth);
			depth++;
			conservative++;
			result = omega_solve_geq (iS, desired_result);
			conservative--;
			depth--;
			if (result != omega_false)
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
			return parallel_splinter (pb, best_parallel_eqn,
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
		      for (e = 0; e < pb->num_geqs; e++)
			if (pb->geqs[e].coef[i] > 1)
			  lower_bound[lower_bounds++] = e;
		      /* sort array */
		      for (j = 0; j < lower_bounds; j++)
			{
			  smallest = j;
			  for (k = j + 1; k < lower_bounds; k++)
			    if (pb->geqs[lower_bound[smallest]].coef[i] >
				pb->geqs[lower_bound[k]].coef[i])
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
				     pb->geqs[lower_bound[j]].coef[i]);
			  fprintf (dump_file, "\n");
			}


		      for (j = 0; j < lower_bounds; j++)
			{
			  int max_incr;
			  int c;
			  e = lower_bound[j];
			  max_incr =
			    ((pb->geqs[e].coef[i] - 1) *
			     (worst_lower_bound_constant - 1) -
			     1) / worst_lower_bound_constant;

			  /* max_incr += 2; */
			  if (dump_file && (dump_flags & TDF_DETAILS))
			    {
			      fprintf (dump_file, "for equation ");
			      omega_print_geq (dump_file, pb, &pb->geqs[e]);
			      fprintf (dump_file,
				       "\ntry decrements from 0 to %d\n",
				       max_incr);
			      omega_print_problem (dump_file, pb);
			    }
			  if (max_incr > 50)
			    {
			      if (!smoothed
				  && smooth_weird_equations (pb))
				{
				  conservative--;
				  free (rS);
				  free (iS);
				  smoothed = 1;
				  goto solve_geq_start;
				}
			    }
			  omega_copy_eqn (&pb->eqs[0], &pb->geqs[e], pb->num_vars);
			  /*
			   * if (pb->geqs[e].color) fprintf(dump_file,"warning: adding black equality constraint
			   * based on red inequality\n");
			   */
			  pb->eqs[0].color = omega_black;
			  omega_init_eqn_zero (&pb->geqs[e], pb->num_vars);
			  pb->geqs[e].touched = true;
			  pb->num_eqs = 1;
			  for (c = max_incr; c >= 0; c--)
			    {
			      if (dump_file && (dump_flags & TDF_DETAILS))
				{
				  fprintf (dump_file,
					   "trying next decrement of %d\n",
					   max_incr - c);
				  omega_print_problem (dump_file, pb);
				}

			      omega_copy_problem (rS, pb);

			      if (dump_file && (dump_flags & TDF_DETAILS))
				omega_print_problem (dump_file, rS);

			      result = omega_solve_problem (rS, desired_result);
			      if (result == omega_true)
				{
				  free (rS);
				  free (iS);
				  conservative--;
				  return omega_true;
				}
			      pb->eqs[0].coef[0]--;
			    }
			  if (j + 1 < lower_bounds)
			    {
			      pb->num_eqs = 0;
			      omega_copy_eqn (&pb->geqs[e], &pb->eqs[0], pb->num_vars);
			      pb->geqs[e].touched = true;
			      pb->geqs[e].color = omega_black;
			      omega_copy_problem (rS, pb);

			      if (dump_file && (dump_flags & TDF_DETAILS))
				fprintf (dump_file,
					 "exhausted lower bound, checking if still feasible ");
			      result = omega_solve_problem (rS, omega_false);
			      if (result == omega_false)
				break;
			    }
			}

		      if (dump_file && (dump_flags & TDF_DETAILS))
			fprintf (dump_file, "fall-off the end\n");
		      free (rS);
		      free (iS);

		      conservative--;
		      return omega_false;
		    }
		  }
		free (rS);
		free (iS);
	      }
	    return omega_unknown;
	  }
	}
      while (eliminate_again);
    }
}

/* Because the omega solver is recursive, this counter limits the
   recursion depth.  */
static int omega_solve_depth = 0;

/* Return true when the problem PB has a solution following the
   DESIRED_RESULT.  */

enum omega_result
omega_solve_problem (omega_pb pb, enum omega_result desired_result)
{
  enum omega_result result;

  gcc_assert (pb->num_vars >= pb->safe_vars);
  if (desired_result != omega_simplify)
    pb->safe_vars = 0;

  omega_solve_depth++;
  if (omega_solve_depth > 50)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Solve depth = %d, inApprox = %d, aborting\n",
		   omega_solve_depth, inApproximateMode);
	  omega_print_problem (dump_file, pb);
	}
      exit (2);
    }

  do
    {
      doItAgain = 0;
      if (omega_solve_eq (pb, desired_result) == omega_false)
	{
	  omega_solve_depth--;
	  return omega_false;
	}
      if (inApproximateMode && !pb->num_geqs)
	{
	  result = omega_true;
	  pb->num_vars = pb->safe_vars;
	  omega_problem_reduced (pb);
	  break;
	}
      else
	result = omega_solve_geq (pb, desired_result);
    }
  while (doItAgain && desired_result == omega_simplify);
  omega_solve_depth--;

  if (!omega_reduce_with_subs)
    {
      resurrectSubs (pb);
      gcc_assert (please_no_equalities_in_simplified_problems 
		  || !result || pb->num_subs == 0);
    }

  return result;
}

/* Return true if red equations constrain the set of possible solutions.
   We assume that there are solutions to the black equations by
   themselves, so if there is no solution to the combined problem, we
   return true.  */

bool
omega_problem_has_red_equations (omega_pb pb)
{
  bool result;
  int e;
  int i;


  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Checking for red equations:\n");
      omega_print_problem (dump_file, pb);
    }

  please_no_equalities_in_simplified_problems++;
  mayBeRed++;
#ifndef singleResult
  returnSingleResult++;
#endif
  createColor = 1;
  result = !omega_simplify_problem (pb);
#ifndef singleResult
  returnSingleResult--;
#endif
  mayBeRed--;
  please_no_equalities_in_simplified_problems--;
  if (result)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
      	fprintf (dump_file, "Gist is FALSE\n");
      pb->num_subs = 0;
      pb->num_geqs = 0;
      pb->num_eqs = 1;
      pb->eqs[0].color = omega_red;
      for (i = pb->num_vars; i > 0; i--)
	pb->eqs[0].coef[i] = 0;
      pb->eqs[0].coef[0] = 1;
      return true;
    }
  freeRedEliminations (pb);

  gcc_assert (pb->num_eqs == 0);

  for (e = pb->num_geqs - 1; e >= 0; e--)
    if (pb->geqs[e].color == omega_red)
      result = true;

  if (!result)
    return false;

  for (i = pb->safe_vars; i >= 1; i--)
    {
      int ub = 0;
      int lb = 0;
      for (e = pb->num_geqs - 1; e >= 0; e--)
	{
	  if (pb->geqs[e].coef[i])
	    {
	      if (pb->geqs[e].coef[i] > 0)
		lb |= (1 + pb->geqs[e].color);
	      else		/* (pb->geqs[e].coef[i] < 0) */
		ub |= (1 + pb->geqs[e].color);
	    }
	}
      if (ub == 2 || lb == 2)
	{

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "checks for upper/lower bounds worked!\n");

	  if (!omega_reduce_with_subs)
	    {
	      resurrectSubs (pb);
	      gcc_assert (pb->num_subs == 0);
	    }
	  return true;
	}
    }


  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file,
	     "*** Doing potentially expensive elimination tests for red equations\n");
  please_no_equalities_in_simplified_problems++;
  omega_eliminate_red (pb, 1);
  please_no_equalities_in_simplified_problems--;
  result = false;
  gcc_assert (pb->num_eqs == 0);
  for (e = pb->num_geqs - 1; e >= 0; e--)
    if (pb->geqs[e].color == omega_red)
      result = true;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      if (!result)
	fprintf (dump_file,
		 "******************** Redudant Red Equations eliminated!!\n");
      else
	fprintf (dump_file, "******************** Red Equations remain\n");

      omega_print_problem (dump_file, pb);
    }

  if (!omega_reduce_with_subs)
    {
      resurrectSubs (pb);
      {
	normalize_return_type r = normalize_omega_problem (pb);
	gcc_assert (r != normalize_false);
      }
      coalesce (pb);
      cleanout_wildcards (pb);
      gcc_assert (pb->num_subs == 0);
    }

  return result;
}

/*   */

int
omega_simplify_approximate (omega_pb pb)
{
  int result;

  inApproximateMode = 1;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Entering Approximate Mode\n");

  result = omega_simplify_problem (pb);

  if (pb->num_vars != pb->safe_vars)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "gcc_assertion blown!\n");
	  omega_print_problem (dump_file, pb);
	}
      gcc_assert (pb->num_vars == pb->safe_vars);
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Leaving Approximate Mode\n");

  inApproximateMode = 0;

  if (!omega_reduce_with_subs)
    gcc_assert (pb->num_subs == 0);

  return result;
}

/* Simplifies problem PB.  */

bool
omega_simplify_problem (omega_pb pb)
{
  int i;
  omega_found_reduction = false;

  if (!pb->variablesInitialized)
    omega_initialize_variables (pb);

#ifdef clearForwardingPointers
  for (i = 1; i <= pb->num_vars; i++)
    if (pb->var[i] > 0)
      pb->forwarding_address[pb->var[i]] = 0;
#endif

  if (next_key * 3 > maxKeys)
    {
      int e;
      hash_version++;
      next_key = max_vars + 1;
      for (e = pb->num_geqs - 1; e >= 0; e--)
	pb->geqs[e].touched = true;
      for (i = 0; i < hash_table_size; i++)
	hash_master[i].touched = -1;
      pb->hash_version = hash_version;
    }
  else if (pb->hash_version != hash_version)
    {
      int e;
      for (e = pb->num_geqs - 1; e >= 0; e--)
	pb->geqs[e].touched = true;
      pb->hash_version = hash_version;
    }

  nonConvex = 0;

  if (pb->num_vars > pb->num_eqs + 3 * pb->safe_vars)
    omega_free_eliminations (pb, pb->safe_vars);

  if (!mayBeRed && pb->num_subs == 0 && pb->safe_vars == 0)
    {
      omega_found_reduction = omega_solve_problem (pb, omega_unknown);
      if (omega_found_reduction && !returnSingleResult)
	{
	  pb->num_geqs = 0;
	  pb->num_eqs = 0;
	  (*omega_when_reduced) (pb);
	}
      return omega_found_reduction;
    }

  omega_solve_problem (pb, omega_simplify);

  if (omega_found_reduction)
    {
      for (i = 1; i <= pb->safe_vars; i++)
	pb->forwarding_address[pb->var[i]] = i;
      for (i = 0; i < pb->num_subs; i++)
	pb->forwarding_address[pb->subs[i].key] = -i - 1;
    }

  if (!omega_reduce_with_subs)
    gcc_assert (please_no_equalities_in_simplified_problems
		|| !omega_found_reduction || pb->num_subs == 0);

  return omega_found_reduction;
}

/*   */

void
omega_unprotect_variable (omega_pb pb, int v)
{
  int e, t, j, i;
  i = pb->forwarding_address[v];
  if (i < 0)
    {
      i = -1 - i;
      pb->num_subs--;
      if (i < pb->num_subs)
	{
	  omega_copy_eqn (&pb->subs[i], &pb->subs[pb->num_subs], pb->num_vars);
	  pb->forwarding_address[pb->subs[i].key] = -i - 1;
	}
    }
  else
    {
      int bringToLife[max_vars];
      int comingBack = 0;
      int e2;
      for (e = pb->num_subs - 1; e >= 0; e--)
	if ((bringToLife[e] = (pb->subs[e].coef[i] != 0)))
	  comingBack++;

      for (e2 = pb->num_subs - 1; e2 >= 0; e2--)
	if (bringToLife[e2])
	  {

	    pb->num_vars++;
	    pb->safe_vars++;
	    if (pb->safe_vars < pb->num_vars)
	      {
		for (e = pb->num_geqs - 1; e >= 0; e--)
		  {
		    pb->geqs[e].coef[pb->num_vars] = pb->geqs[e].coef[pb->safe_vars];
		    pb->geqs[e].coef[pb->safe_vars] = 0;
		  }
		for (e = pb->num_eqs - 1; e >= 0; e--)
		  {
		    pb->eqs[e].coef[pb->num_vars] = pb->eqs[e].coef[pb->safe_vars];
		    pb->eqs[e].coef[pb->safe_vars] = 0;
		  }
		for (e = pb->num_subs - 1; e >= 0; e--)
		  {
		    pb->subs[e].coef[pb->num_vars] = pb->subs[e].coef[pb->safe_vars];
		    pb->subs[e].coef[pb->safe_vars] = 0;
		  }
		pb->var[pb->num_vars] = pb->var[pb->safe_vars];
		pb->forwarding_address[pb->var[pb->num_vars]] =
		  pb->num_vars;
	      }
	    else
	      {
		for (e = pb->num_geqs - 1; e >= 0; e--)
		  {
		    pb->geqs[e].coef[pb->safe_vars] = 0;
		  }
		for (e = pb->num_eqs - 1; e >= 0; e--)
		  {
		    pb->eqs[e].coef[pb->safe_vars] = 0;
		  }
		for (e = pb->num_subs - 1; e >= 0; e--)
		  {
		    pb->subs[e].coef[pb->safe_vars] = 0;
		  }
	      }

	    pb->var[pb->safe_vars] = pb->subs[e2].key;
	    pb->forwarding_address[pb->subs[e2].key] = pb->safe_vars;

	    omega_copy_eqn (&(pb->eqs[pb->num_eqs]), &(pb->subs[e2]), pb->num_vars);
	    pb->eqs[pb->num_eqs++].coef[pb->safe_vars] = -1;
	    gcc_assert (pb->num_eqs <= max_eqs);
	    if (e2 < pb->num_subs - 1)
	      omega_copy_eqn (&(pb->subs[e2]), &(pb->subs[pb->num_subs - 1]), pb->num_vars);
	    pb->num_subs--;
	  }




      if (i < pb->safe_vars)
	{
	  j = pb->safe_vars;
	  for (e = pb->num_subs - 1; e >= 0; e--)
	    {
	      t = pb->subs[e].coef[j];
	      pb->subs[e].coef[j] = pb->subs[e].coef[i];
	      pb->subs[e].coef[i] = t;
	    }
	  for (e = pb->num_geqs - 1; e >= 0; e--)
	    if (pb->geqs[e].coef[j] != pb->geqs[e].coef[i])
	      {
		pb->geqs[e].touched = true;
		t = pb->geqs[e].coef[j];
		pb->geqs[e].coef[j] = pb->geqs[e].coef[i];
		pb->geqs[e].coef[i] = t;
	      }
	  for (e = pb->num_eqs - 1; e >= 0; e--)
	    {
	      t = pb->eqs[e].coef[j];
	      pb->eqs[e].coef[j] = pb->eqs[e].coef[i];
	      pb->eqs[e].coef[i] = t;
	    }
	  t = pb->var[j];
	  pb->var[j] = pb->var[i];
	  pb->var[i] = t;
	  pb->forwarding_address[pb->var[i]] = i;
	  pb->forwarding_address[pb->var[j]] = j;
	}
      pb->safe_vars--;
    }
  chainUnprotect (pb);

}

/* Unprotects var and simplifies the problem.  */

int
omega_constrain_variable_sign (omega_pb pb, int color, int i,
			       int sign)
{
  int nV = pb->num_vars;
  int e, k, j;

  k = pb->forwarding_address[i];
  if (k < 0)
    {
      k = -1 - k;

      if (sign != 0)
	{
	  e = pb->num_geqs++;
	  omega_copy_eqn (&pb->geqs[e], &pb->subs[k],
			  pb->num_vars);
	  for (j = 0; j <= nV; j++)
	    pb->geqs[e].coef[j] *= sign;
	  pb->geqs[e].coef[0]--;
	  pb->geqs[e].touched = true;
	  pb->geqs[e].color = color;
	}
      else
	{
	  e = pb->num_eqs++;
	  gcc_assert (pb->num_eqs <= max_eqs);
	  omega_copy_eqn (&pb->eqs[e], &pb->subs[k],
			  pb->num_vars);
	  pb->eqs[e].color = color;
	}

    }
  else if (sign != 0)
    {
      e = pb->num_geqs++;
      omega_init_eqn_zero (&pb->geqs[e], pb->num_vars);
      pb->geqs[e].coef[k] = sign;
      pb->geqs[e].coef[0] = -1;
      pb->geqs[e].touched = true;
      pb->geqs[e].color = color;
    }
  else
    {
      e = pb->num_eqs++;
      gcc_assert (pb->num_eqs <= max_eqs);
      omega_init_eqn_zero (&pb->eqs[e], pb->num_vars);
      pb->eqs[e].coef[k] = 1;
      pb->eqs[e].color = color;
    }
  omega_unprotect_variable (pb, i);
  return omega_simplify_problem (pb);
}

/*   */

void
omega_constrain_variable_value (omega_pb pb,
				int color, int i, int value)
{
  int e, k;

  k = pb->forwarding_address[i];
  if (k < 0)
    {
      k = -1 - k;

      e = pb->num_eqs++;
      gcc_assert (pb->num_eqs <= max_eqs);
      omega_copy_eqn (&pb->eqs[e], &pb->subs[k],
		      pb->num_vars);
      pb->eqs[e].coef[0] -= value;
    }
  else
    {
      e = pb->num_eqs++;
      omega_init_eqn_zero (&pb->eqs[e], pb->num_vars);
      pb->eqs[e].coef[k] = 1;
      pb->eqs[e].coef[0] = -value;
    }
  pb->eqs[e].color = color;
}

/* Return the bounds LOWER_BOUND and UPPER_BOUND for the values of
   variable I.  */

int
omega_query_variable (omega_pb pb, int i,
		      int *lower_bound, int *upper_bound)
{
  int nV = pb->num_vars;
  int e, j;
  int isSimple;
  int coupled = FALSE;
  i = pb->forwarding_address[i];

  (*lower_bound) = neg_infinity;
  (*upper_bound) = pos_infinity;

  if (i < 0)
    {
      int easy = TRUE;
      i = -i - 1;
      for (j = 1; j <= nV; j++)
	if (pb->subs[i].coef[j] != 0)
	  easy = FALSE;

      if (easy)
	{
	  *upper_bound = *lower_bound = pb->subs[i].coef[0];
	  return FALSE;
	}
      return TRUE;
    }

  for (e = pb->num_subs - 1; e >= 0; e--)
    if (pb->subs[e].coef[i] != 0)
      coupled = TRUE;

  for (e = pb->num_eqs - 1; e >= 0; e--)
    if (pb->eqs[e].coef[i] != 0)
      {
	isSimple = TRUE;
	for (j = 1; j <= nV; j++)
	  if (i != j && pb->eqs[e].coef[j] != 0)
	    {
	      isSimple = FALSE;
	      coupled = TRUE;
	      break;
	    }
	if (!isSimple)
	  continue;
	else
	  {
	    *lower_bound = *upper_bound = -pb->eqs[e].coef[i] * pb->eqs[e].coef[0];
	    return FALSE;
	  }
      }

  for (e = pb->num_geqs - 1; e >= 0; e--)
    if (pb->geqs[e].coef[i] != 0)
      {
	if (pb->geqs[e].key == i)
	  set_max (lower_bound, -pb->geqs[e].coef[0]);
	else if (pb->geqs[e].key == -i)
	  set_min (upper_bound, pb->geqs[e].coef[0]);
	else
	  coupled = TRUE;
      }

  return coupled;
}

/*   */

static void
query_coupled_variable (omega_pb pb, int i, int *l, int *u,
			int *could_be_zero, int lower_bound, int upper_bound)
{
  int e, b1, b2;
  eqn eqn;
  int sign;
  int v;

  if (abs (pb->forwarding_address[i]) != 1 || pb->num_vars + pb->num_subs != 2
      || pb->num_eqs + pb->num_subs != 1)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file,
		   "query_coupled_variable called with bad parameters\n");
	  omega_print_problem (dump_file, pb);
	}
      exit (2);
    }

  if (pb->forwarding_address[i] == -1)
    {
      eqn = &pb->subs[0];
      sign = 1;
      v = 1;
    }
  else
    {
      eqn = &pb->eqs[0];
      sign = -eqn->coef[1];
      v = 2;
    }

  /* Variable i is defined in terms of variable v */

  for (e = pb->num_geqs - 1; e >= 0; e--)
    if (pb->geqs[e].coef[v] != 0)
      {
	if (pb->geqs[e].coef[v] == 1)
	  set_max (&lower_bound, -pb->geqs[e].coef[0]);
	else
	  set_min (&upper_bound, pb->geqs[e].coef[0]);
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
      set_max (l, b1);
      set_min (u, b2);
    }
  else
    {
      set_max (l, b2);
      set_min (u, b1);
    }

  *could_be_zero = (*l <= 0 && 0 <= *u
		    && int_mod (eqn->coef[0], abs (eqn->coef[v])) == 0);
}

/* Return a lower bound L and an upper bound U for variable I in
   problem PB.  */

int
omega_query_variable_bounds (omega_pb pb, int i, int *l, int *u)
{
  int coupled;

  *l = neg_infinity;
  *u = pos_infinity;

  coupled = omega_query_variable (pb, i, l, u);
  if (!coupled || (pb->num_vars == 1 && pb->forwarding_address[i] == 1))
    return 0;

  if (abs (pb->forwarding_address[i]) == 1 && pb->num_vars + pb->num_subs == 2
      && pb->num_eqs + pb->num_subs == 1)
    {
      int could_be_zero;
      query_coupled_variable (pb, i, l, u, &could_be_zero, neg_infinity,
			      pos_infinity);
      return 0;
    }
  return 1;
}

/* For problem PB, return the classic data dependence direction, and
   the distance DIST when *DIST_KNOWN is true.  DD_LT, DD_EQ and DD_GT
   are bit masks that added to the result.  LOWER_BOUND and
   UPPER_BOUND are are bounds on the value of variable I.  */

int
omega_query_variable_signs (omega_pb pb, int i, int dd_lt,
			    int dd_eq, int dd_gt, int lower_bound,
			    int upper_bound, bool *dist_known, int *dist)
{
  int result;
  int l, u;
  int could_be_zero;

  l = neg_infinity;
  u = pos_infinity;

  omega_query_variable (pb, i, &l, &u);
  query_coupled_variable (pb, i, &l, &u, &could_be_zero, lower_bound,
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
      *dist_known = true;
      *dist = l;
    }
  else
    *dist_known = false;

  return result;
}

/* Keeps the state of the initialization.  */
static bool omega_initialized = false;

/* Initialization of the Omega solver.  */

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

  sprintf (wild_name[0], "1");
  sprintf (wild_name[1], "a");
  sprintf (wild_name[2], "b");
  sprintf (wild_name[3], "c");
  sprintf (wild_name[4], "d");
  sprintf (wild_name[5], "e");
  sprintf (wild_name[6], "f");
  sprintf (wild_name[7], "g");
  sprintf (wild_name[8], "h");
  sprintf (wild_name[9], "i");
  sprintf (wild_name[10], "j");
  sprintf (wild_name[11], "k");
  sprintf (wild_name[12], "l");
  sprintf (wild_name[13], "m");
  sprintf (wild_name[14], "n");
  sprintf (wild_name[15], "o");
  sprintf (wild_name[16], "p");
  sprintf (wild_name[17], "q");
  sprintf (wild_name[18], "r");
  sprintf (wild_name[19], "s");
  sprintf (wild_name[20], "t");
  sprintf (wild_name[40 - 1], "alpha");
  sprintf (wild_name[40 - 2], "beta");
  sprintf (wild_name[40 - 3], "gamma");
  sprintf (wild_name[40 - 4], "delta");
  sprintf (wild_name[40 - 5], "tau");
  sprintf (wild_name[40 - 6], "sigma");
  sprintf (wild_name[40 - 7], "chi");
  sprintf (wild_name[40 - 8], "omega");
  sprintf (wild_name[40 - 9], "pi");
  sprintf (wild_name[40 - 10], "ni");
  sprintf (wild_name[40 - 11], "Alpha");
  sprintf (wild_name[40 - 12], "Beta");
  sprintf (wild_name[40 - 13], "Gamma");
  sprintf (wild_name[40 - 14], "Delta");
  sprintf (wild_name[40 - 15], "Tau");
  sprintf (wild_name[40 - 16], "Sigma");
  sprintf (wild_name[40 - 17], "Chi");
  sprintf (wild_name[40 - 18], "Omega");
  sprintf (wild_name[40 - 19], "Pi");

  omega_initialized = true;
}

