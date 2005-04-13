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

/* 
 * Options SPEED - don't bother checking debugging flags eliminateRedundantConstraints - use expensive methods to
 * eliminate all redundant constraints singleResult - only produce a single simplified result APROX - approximate
 * inexact reductions verifySimplification (runtime) - if TRUE, simplifyProblem checks for problem with no solutions
 * reduceWithSubs (runtime) - if FALSE, convert substitutions back to EQs
 */

#include <config.h>
#include <system.h>
#define maxWildCards 18
#ifndef APROX
#define APROX 0
#endif

static int DD_DEBUG_OMEGA=99;
#include "omega.h"

#ifdef NDEBUG
#define checkPosMul(x,y) ((x)*(y))
#define checkMul(x,y) ((x)*(y))
#else

int
checkPosMul (int x, int y)
{
  if (x != 0)
    gcc_assert ((INT_MAX) / x > y);
  return x * y;
}

int
checkMul (int x, int y)
{
  if (x >= 0)
    {
      if (y >= 0)
	return checkPosMul (x, y);
      else
	return -checkPosMul (x, -y);
    }
  else if (y >= 0)
    return -checkPosMul (-x, y);
  else
    return checkPosMul (-x, -y);
}
#endif

#define EQ_type 1
#define GEQ_type 2
#define keyMult 31
#define indexMult 17
#define abs(x) (x >= 0? (x) : -(x))
#define max(x,y) (x>y?x:y)
#define min(x,y) (x<y?x:y)
#define setMax(m,x) {if (m < x) m = x;}
#define setMin(m,x) if (m > x) m = x
#define nVars (problemPtr->_nVars)
#define nEQ (problemPtr->_numEQs)
#define nGEQ (problemPtr->_numGEQs)
#define nSUB (problemPtr->_numSUBs)
#define SUBs (problemPtr->_SUBs)
#define GEQs (problemPtr->_GEQs)
#define EQs (problemPtr->_EQs)
#define safeVars (problemPtr->_safeVars)
#define printEQ(e) printEqn(problemPtr,e,0,0)
#define printGEQ(e) printEqn(problemPtr,e,1,0)
#define printGEQextra(e) printEqn(problemPtr,e,1,1)
#define variable(i) orgVariable(problemPtr->_var[i])
#define orgVariable(i) (i == 0 ? "1" : (i < 0?wildName[-i]: (*current_getVarName)(i)))
#define doTrace (trace && TRACE)
#define doDelete(e,nV) {if (DEBUG) {fprintf(outputFile,"Deleting %d (last:%d): ",e,nGEQ-1); printGEQ(&GEQs[e]);fprintf(outputFile,"\n");}; if (e < nGEQ-1) eqnncpy (&GEQs[e], &GEQs[nGEQ - 1],(nV)); (nGEQ)--;}
#define doDeleteExtra(e,nV) {if (DEBUG) {fprintf(outputFile,"Deleting %d: ",e); printGEQextra(&GEQs[e]);fprintf(outputFile,"\n");}; if (e < nGEQ-1) eqnncpy (&GEQs[e], &GEQs[nGEQ - 1],(nV)); (nGEQ)--;}
#define isRed(e) (desiredResult == SIMPLIFY && (e)->color)

#ifdef singleResult
#define returnSingleResult  1
#else
static int returnSingleResult = 0;
#endif

static int mayBeRed = 0;
#define hashTableSize 550
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
extern void addingEqualityConstraint (Problem * problemPtr, int e);

#define mallocProblem ((Problem *) xmalloc(sizeof(Problem)))

static struct _eqn hashMaster[hashTableSize];

int nonConvex = 0;
int omegaPrintResult = 0;

int firstCheckForRedundantEquations = 0;

static int doItAgain;

static int conservative = 0;

static FILE *outputFile;   /* printProblem writes its output to this file */

static int nextKey;
static char wildName[200][20];
static int nextWildCard = 0;
static int trace = 1;
static int depth = 0;
static int foundReduction;
static int packing[maxVars];

static int inApproximateMode = 0;

int reduceWithSubs = 1;

int verifySimplification = 0;

static int createColor = 0;
static int pleaseNoEqualitiesInSimplifiedProblems = 0;

int hashVersion = 0;

#define noProblem ((Problem *) 0)

Problem *originalProblem = noProblem;

typedef enum
{
  normalize_false, normalize_uncoupled, normalize_coupled
}
normalizeReturnType;

normalizeReturnType normalize (Problem * problemPtr);


void
noProcedure (Problem * problemPtr)
{
}

void (*whenReduced) (Problem *) = noProcedure;

static int
gcd (register int b, register int a)
{
  if (b == 1)
    return (1);
  while (b != 0)
    {
      register int t = b;
      b = a % b;
      a = t;
    };
  return (a);
}


int
checkIfSingleVar (Eqn e, int i)
{
  for (; i > 0; i--)
    if (e->coef[i])
      {
	i--;
	break;
      };
  for (; i > 0; i--)
    if (e->coef[i])
      break;
  return (i == 0);
}


void
initializeVariables (Problem * p)
{
  int i;
  for (i = p->_nVars; i >= 0; i--)
    p->forwardingAddress[i] = p->_var[i] = i;
  p->variablesInitialized = 1;
}

void
initializeProblem (Problem * p)
{
  p->_nVars = 0;
  p->hashVersion = hashVersion;
  p->variablesInitialized = 0;
  p->variablesFreed = 0;
  p->_safeVars = 0;
  p->_numEQs = 0;
  p->_numGEQs = 0;
  p->_numSUBs = 0;
}

void
problemcpy (Problem * p1, Problem * p2)
{
  int e, i;
  p1->_nVars = p2->_nVars;
  p1->hashVersion = p2->hashVersion;
  p1->variablesInitialized = p2->variablesInitialized;
  p1->variablesFreed = p2->variablesFreed;
  p1->_safeVars = p2->_safeVars;
  p1->_numEQs = p2->_numEQs;
  p1->_numSUBs = p2->_numSUBs;
  for (e = p2->_numEQs - 1; e >= 0; e--)
    eqnncpy (&(p1->_EQs[e]), &(p2->_EQs[e]), p2->_nVars);
  p1->_numGEQs = p2->_numGEQs;
  for (e = p2->_numGEQs - 1; e >= 0; e--)
    eqnncpy (&(p1->_GEQs[e]), &(p2->_GEQs[e]), p2->_nVars);
  for (e = p2->_numSUBs - 1; e >= 0; e--)
    eqnncpy (&(p1->_SUBs[e]), &(p2->_SUBs[e]), p2->_nVars);
  for (i = 0; i <= p2->_nVars; i++)
    p1->_var[i] = p2->_var[i];
  for (i = 0; i <= maxVars; i++)
    p1->forwardingAddress[i] = p2->forwardingAddress[i];


}



static void
printTerm (Problem * problemPtr, Eqn e, int c)
{
  int i;
  int first;
  int n = nVars;
  int wentFirst = -1;
  first = 1;
  for (i = 1; i <= n; i++)
    if (c * e->coef[i] > 0)
      {

	first = 0;
	wentFirst = i;

	if (c * e->coef[i] == 1)
	  fprintf (outputFile, "%s", variable (i));
	else
	  fprintf (outputFile, "%d * %s", c * e->coef[i], variable (i));
	break;
      };
  for (i = 1; i <= n; i++)
    if (i != wentFirst && c * e->coef[i] != 0)
      {
	if (!first && c * e->coef[i] > 0)
	  fprintf (outputFile, " + ");

	first = 0;

	if (c * e->coef[i] == 1)
	  fprintf (outputFile, "%s", variable (i));
	else if (c * e->coef[i] == -1)
	  fprintf (outputFile, " - %s", variable (i));
	else
	  fprintf (outputFile, "%d * %s", c * e->coef[i], variable (i));
      };
  if (!first && c * e->coef[0] > 0)
    fprintf (outputFile, " + ");
  if (first || c * e->coef[0] != 0)
    fprintf (outputFile, "%d", c * e->coef[0]);
}


void
printEqn (Problem * problemPtr, Eqn e, int test, int extra)
{
  char buf[maxVars * 12 + 80];

  sprintEqn (buf, problemPtr, e, test, extra);
  fprintf (outputFile, "%s", buf);
}

void
sprintEqn (char *str, Problem * problemPtr, Eqn e, int test, int extra)
{
  int i;
  int first;
  int n = nVars + extra;
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
      };
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

void
printVars (Problem * problemPtr)
{
  int i;
  fprintf (outputFile, "variables = ");
  if (problemPtr->_safeVars > 0)
    fprintf (outputFile, "(");
  for (i = 1; i <= nVars; i++)
    {
      fprintf (outputFile, "%s", variable (i));
      if (i == problemPtr->_safeVars)
	fprintf (outputFile, ")");
      if (i < nVars)
	fprintf (outputFile, ", ");
    };
  fprintf (outputFile, "\n");
}

void
printProblem (Problem * problemPtr)
{
  int e;

  if (!problemPtr->variablesInitialized)
    initializeVariables (problemPtr);
  printVars (problemPtr);
  for (e = 0; e < nEQ; e++)
    {
      printEQ (&EQs[e]);
      fprintf (outputFile, "\n");
    };
  fprintf (outputFile, "Done with EQ\n");
  for (e = 0; e < nGEQ; e++)
    {
      printGEQ (&GEQs[e]);
      fprintf (outputFile, "\n");
    };
  fprintf (outputFile, "Done with GEQ\n");
  for (e = 0; e < nSUB; e++)
    {
      Eqn eq = &SUBs[e];
      if (DBUG && eq->color)
	fprintf (outputFile, "[");
      if (eq->key > 0)
	fprintf (outputFile, "%s := ", orgVariable (eq->key));
      else
	fprintf (outputFile, "#%d := ", eq->key);
      printTerm (problemPtr, eq, 1);
      if (DBUG && eq->color)
	fprintf (outputFile, "]");
      fprintf (outputFile, "\n");
    };
  fflush (outputFile);
}


int
countRedEquations (Problem * problemPtr)
{
  int e, i;
  int result = 0;
  for (e = 0; e < nEQ; e++)
    if (EQs[e].color == red)
      {
	for (i = nVars; i > 0; i--)
	  if (GEQs[e].coef[i])
	    break;
	if (i == 0 && GEQs[e].coef[0] == 1)
	  return 0;
	else
	  result += 2;
      };
  for (e = 0; e < nGEQ; e++)
    if (GEQs[e].color == red)
      result += 1;
  for (e = 0; e < nSUB; e++)
    if (SUBs[e].color == red)
      result += 2;
  return result;
}


void
printRedEquations (Problem * problemPtr)
{
  int e;

  if (!problemPtr->variablesInitialized)
    initializeVariables (problemPtr);
  printVars (problemPtr);
  for (e = 0; e < nEQ; e++)
    {
      if (EQs[e].color == red)
	{
	  printEQ (&EQs[e]);
	  fprintf (outputFile, "\n");
	}
    };
  for (e = 0; e < nGEQ; e++)
    {
      if (GEQs[e].color == red)
	{
	  printGEQ (&GEQs[e]);
	  fprintf (outputFile, "\n");
	}
    };
  for (e = 0; e < nSUB; e++)
    {
      if (SUBs[e].color == red)
	{
	  Eqn eq = &SUBs[e];
	  if (DBUG && eq->color)
	    fprintf (outputFile, "[");
	  if (eq->key > 0)
	    fprintf (outputFile, "%s := ", orgVariable (eq->key));
	  else
	    fprintf (outputFile, "#%d := ", eq->key);
	  printTerm (problemPtr, eq, 1);
	  if (DBUG && eq->color)
	    fprintf (outputFile, "]");
	  fprintf (outputFile, "\n");
	}
    };
  fflush (outputFile);
}


void
prettyPrintProblem (Problem * problemPtr)
{
  int e;
  int v;
  int live[maxGEQs];
  int v1, v2, v3;
  int t, change;
  int stuffPrinted = 0;


  typedef enum
  {
    none, le, lt
  }
  partialOrderType;

  partialOrderType po[maxVars][maxVars];
  int poE[maxVars][maxVars];
  int lastLinks[maxVars];
  int firstLinks[maxVars];
  int chainLength[maxVars];
  int chain[maxVars];
  int i, m, multiprint;


  if (!problemPtr->variablesInitialized)
    initializeVariables (problemPtr);

  if (nVars > 0)
    {
      eliminateRedundant (problemPtr, 0);

      for (e = 0; e < nEQ; e++)
	{
	  if (stuffPrinted)
	    fprintf (outputFile, "; ");
	  stuffPrinted = 1;
	  printEQ (&EQs[e]);
	};

      for (e = 0; e < nGEQ; e++)
	live[e] = TRUE;

      while (1)
	{
	  for (v = 1; v <= nVars; v++)
	    {
	      lastLinks[v] = firstLinks[v] = 0;
	      chainLength[v] = 0;
	      for (v2 = 1; v2 <= nVars; v2++)
		po[v][v2] = none;
	    };

	  for (e = 0; e < nGEQ; e++)
	    if (live[e])
	      {
		for (v = 1; v <= nVars; v++)
		  {
		    if (GEQs[e].coef[v] == 1)
		      firstLinks[v]++;
		    else if (GEQs[e].coef[v] == -1)
		      lastLinks[v]++;
		  };

		v1 = nVars;
		while (v1 > 0 && GEQs[e].coef[v1] == 0)
		  v1--;
		v2 = v1 - 1;
		while (v2 > 0 && GEQs[e].coef[v2] == 0)
		  v2--;
		v3 = v2 - 1;
		while (v3 > 0 && GEQs[e].coef[v3] == 0)
		  v3--;

		if (GEQs[e].coef[0] > 0 || GEQs[e].coef[0] < -1
		    || v2 <= 0 || v3 > 0
		    || GEQs[e].coef[v1] * GEQs[e].coef[v2] != -1)
		  {
		    /* Not a partial order relation */

		  }
		else
		  {
		    if (GEQs[e].coef[v1] == 1)
		      {
			v3 = v2;
			v2 = v1;
			v1 = v3;
		      };
		    /* relation is v1 <= v2 or v1 < v2 */
		    po[v1][v2] = ((GEQs[e].coef[0] == 0) ? le : lt);
		    poE[v1][v2] = e;
		  };
	      }
	  for (v = 1; v <= nVars; v++)
	    chainLength[v] = lastLinks[v];


	  /*
	   * printf("\n\nPartial order:\n"); printf("         "); for (v1 = 1; v1 <= nVars; v1++)
	   * printf("%7s",variable(v1)); printf("\n"); for (v1 = 1; v1 <= nVars; v1++) { printf("%6s:
	   * ",variable(v1)); for (v2 = 1; v2 <= nVars; v2++) switch (po[v1][v2]) { case none: printf("       ");
	   * break; case le:   printf("    <= "); break; case lt:   printf("    <  "); break; } printf("\n"); }
	   */



	  /* Just in case nVars <= 0 */
	  change = FALSE;
	  for (t = 0; t < nVars; t++)
	    {
	      change = FALSE;
	      for (v1 = 1; v1 <= nVars; v1++)
		for (v2 = 1; v2 <= nVars; v2++)
		  if (po[v1][v2] != none &&
		      chainLength[v1] <= chainLength[v2])
		    {
		      chainLength[v1] = chainLength[v2] + 1;
		      change = TRUE;
		    }
	    };

	  if (change)
	    {
	      /* caught in cycle */
	      gcc_assert (0);
	    };

	  for (v1 = 1; v1 <= nVars; v1++)
	    if (chainLength[v1] == 0)
	      firstLinks[v1] = 0;

	  v = 1;
	  for (v1 = 2; v1 <= nVars; v1++)
	    if (chainLength[v1] + firstLinks[v1] >
		chainLength[v] + firstLinks[v])
	      v = v1;

	  if (chainLength[v] + firstLinks[v] == 0)
	    break;

	  if (stuffPrinted)
	    fprintf (outputFile, "; ");
	  stuffPrinted = 1;
	  /* chain starts at v */
	  /* print firstLinks */
	  {
	    int tmp, first;
	    first = 1;
	    for (e = 0; e < nGEQ; e++)
	      if (live[e] && GEQs[e].coef[v] == 1)
		{
		  if (!first)
		    fprintf (outputFile, ", ");
		  tmp = GEQs[e].coef[v];
		  GEQs[e].coef[v] = 0;
		  printTerm (problemPtr, &GEQs[e], -1);
		  GEQs[e].coef[v] = tmp;
		  live[e] = FALSE;
		  first = 0;
		};
	    if (!first)
	      fprintf (outputFile, " <= ");
	  };


	  /* find chain */
	  chain[0] = v;
	  m = 1;
	  while (1)
	    {
	      /* print chain */
	      for (v2 = 1; v2 <= nVars; v2++)
		if (po[v][v2] && chainLength[v] == 1 + chainLength[v2])
		  break;
	      if (v2 > nVars)
		break;
	      chain[m++] = v2;
	      v = v2;
	    }

	  fprintf (outputFile, "%s", variable (chain[0]));

	  multiprint = 0;
	  for (i = 1; i < m; i++)
	    {
	      v = chain[i - 1];
	      v2 = chain[i];
	      if (po[v][v2] == le)
		fprintf (outputFile, " <= ");
	      else
		fprintf (outputFile, " < ");
	      fprintf (outputFile, "%s", variable (v2));
	      live[poE[v][v2]] = FALSE;
	      if (!multiprint && i < m - 1)
		for (v3 = 1; v3 <= nVars; v3++)
		  {
		    if (v == v3 || v2 == v3)
		      continue;
		    if (po[v][v2] != po[v][v3])
		      continue;
		    if (po[v2][chain[i + 1]] != po[v3][chain[i + 1]])
		      continue;
		    fprintf (outputFile, ",%s", variable (v3));
		    live[poE[v][v3]] = FALSE;
		    live[poE[v3][chain[i + 1]]] = FALSE;
		    multiprint = 1;
		  }
	      else
		multiprint = 0;
	    };

	  v = chain[m - 1];
	  /* print lastLinks */
	  {
	    int tmp, first;
	    first = 1;
	    for (e = 0; e < nGEQ; e++)
	      if (live[e] && GEQs[e].coef[v] == -1)
		{
		  if (!first)
		    fprintf (outputFile, ", ");
		  else
		    fprintf (outputFile, " <= ");
		  tmp = GEQs[e].coef[v];
		  GEQs[e].coef[v] = 0;
		  printTerm (problemPtr, &GEQs[e], 1);
		  GEQs[e].coef[v] = tmp;
		  live[e] = FALSE;
		  first = 0;
		};
	  };
	};


      for (e = 0; e < nGEQ; e++)
	if (live[e])
	  {
	    if (stuffPrinted)
	      fprintf (outputFile, "; ");
	    stuffPrinted = 1;
	    printGEQ (&GEQs[e]);
	  };

      for (e = 0; e < nSUB; e++)
	{
	  Eqn eq = &SUBs[e];
	  if (stuffPrinted)
	    fprintf (outputFile, "; ");
	  stuffPrinted = 1;
	  if (eq->key > 0)
	    fprintf (outputFile, "%s := ", orgVariable (eq->key));
	  else
	    fprintf (outputFile, "#%d := ", eq->key);
	  printTerm (problemPtr, eq, 1);
	};
    };
  fflush (outputFile);
}


static void
nameWildcard (Problem * problemPtr, int i)
{
  --nextWildCard;
  if (nextWildCard < -maxWildCards)
    nextWildCard = -1;
  problemPtr->_var[i] = nextWildCard;
}

static int
addNewWildcard (Problem * problemPtr)
{
  int e;
  int i = ++safeVars;
  nVars++;
  if (nVars != i)
    {
      for (e = nGEQ - 1; e >= 0; e--)
	{
	  if (GEQs[e].coef[i] != 0)
	    GEQs[e].touched = TRUE;
	  GEQs[e].coef[nVars] = GEQs[e].coef[i];

	};
      for (e = nEQ - 1; e >= 0; e--)
	{
	  EQs[e].coef[nVars] = EQs[e].coef[i];
	};
      for (e = nSUB - 1; e >= 0; e--)
	{
	  SUBs[e].coef[nVars] = SUBs[e].coef[i];
	};
      problemPtr->_var[nVars] = problemPtr->_var[i];
    };
  for (e = nGEQ - 1; e >= 0; e--)
    GEQs[e].coef[i] = 0;
  for (e = nEQ - 1; e >= 0; e--)
    EQs[e].coef[i] = 0;
  for (e = nSUB - 1; e >= 0; e--)
    SUBs[e].coef[i] = 0;
  nameWildcard (problemPtr, i);
  return (i);
}

extern void substitute (Problem * problemPtr, Eqn sub, int i, int c);
extern void deleteVariable (Problem * problemPtr, int i);

static void
doMod (Problem * problemPtr, int factor, int e, int j)
/* Solve e = factor alpha for x_j and substitute */
{
  int k, i;
  struct _eqn eq;
  int nFactor;
  int killJ = FALSE;
  eqncpy (&eq, &EQs[e]);
  for (k = nVars; k >= 0; k--)
    {
      eq.coef[k] = intMod (eq.coef[k], factor);
      if (2 * eq.coef[k] >= factor)
	eq.coef[k] -= factor;
    };
  nFactor = eq.coef[j];
  if (j <= safeVars && problemPtr->_var[j] > 0)
    {
      i = addNewWildcard (problemPtr);
      eq.coef[nVars] = eq.coef[i];
      eq.coef[j] = 0;
      eq.coef[i] = -factor;
      killJ = TRUE;
    }
  else
    {
      eq.coef[j] = -factor;
      if (problemPtr->_var[j] > 0)
	nameWildcard (problemPtr, j);
    };
  substitute (problemPtr, &eq, j, nFactor);
  for (k = nVars; k >= 0; k--)
    EQs[e].coef[k] = EQs[e].coef[k] / factor;
  if (killJ)
    deleteVariable (problemPtr, j);
  if (DEBUG)
    {
      fprintf (outputFile, "Mod-ing and normalizing produces:\n");
      printProblem (problemPtr);
    };
}

void
negateGEQ (Problem * problemPtr, int e)
{

  int i;
  for (i = nVars; i >= 0; i--)
    GEQs[e].coef[i] = -GEQs[e].coef[i];
  GEQs[e].coef[0]--;
  GEQs[e].touched = TRUE;
}

static int
verifyProblem (Problem * problemPtr)
{
  int result, e, anyColor;
  Problem *tmpProblem;
  tmpProblem = mallocProblem;
  problemcpy (tmpProblem, problemPtr);
  tmpProblem->_safeVars = 0;
  tmpProblem->_numSUBs = 0;
  anyColor = 0;
  for (e = nGEQ - 1; e >= 0; e--)
    anyColor |= GEQs[e].color;
  anyColor |= pleaseNoEqualitiesInSimplifiedProblems;
  if (anyColor)
    {
      originalProblem = noProblem;
    }
  else
    originalProblem = problemPtr;
  if (TRACE)
    {
      fprintf (outputFile, "verifying problem");
      if (anyColor)
	fprintf (outputFile, " (color mode)");
      fprintf (outputFile, " :\n");
      printProblem (problemPtr);
    };

  result = solve (tmpProblem, UNKNOWN);
  originalProblem = noProblem;
  free (tmpProblem);
  if (TRACE)
    {
      if (result)
	fprintf (outputFile, "verified problem\n");
      else
	fprintf (outputFile, "disproved problem\n");
      printProblem (problemPtr);
    };
  return result;
}


#define implies(A,B) (A==(A&B))

void resurrectSubs (Problem * problemPtr);

int
eliminateRedundant (Problem * problemPtr, bool expensive)
{
  int e, e1, e2, e3, p, q, i, k, alpha, alpha1, alpha2, alpha3;
  int c;
  int isDead[maxGEQs];
  unsigned int P[maxGEQs], Z[maxGEQs], N[maxGEQs];
  unsigned int PP, PZ, PN;	/* possible Positives, possible zeros & possible negatives */

  if (DBUG)
    {
      fprintf (outputFile, "in eliminate Redudant:\n");
      printProblem (problemPtr);
    };
  for (e = nGEQ - 1; e >= 0; e--)
    {
      int tmp = 1;
      isDead[e] = 0;
      P[e] = Z[e] = N[e] = 0;
      for (i = nVars; i >= 1; i--)
	{
	  if (GEQs[e].coef[i] > 0)
	    P[e] |= tmp;
	  else if (GEQs[e].coef[i] < 0)
	    N[e] |= tmp;
	  else
	    Z[e] |= tmp;
	  tmp <<= 1;
	}
    }


  for (e1 = nGEQ - 1; e1 >= 0; e1--)
    if (!isDead[e1])
      for (e2 = e1 - 1; e2 >= 0; e2--)
	if (!isDead[e2])
	  {
	    for (p = nVars; p > 1; p--)
	      {
		for (q = p - 1; q > 0; q--)
		  {
		    alpha =
		      (GEQs[e1].coef[p] * GEQs[e2].coef[q] -
		       GEQs[e2].coef[p] * GEQs[e1].coef[q]);
		    if (alpha != 0)
		      goto foundPQ;
		  };
	      };
	    continue;

	  foundPQ:
	    PZ = (Z[e1] & Z[e2]) | (P[e1] & N[e2]) | (N[e1] & P[e2]);
	    PP = P[e1] | P[e2];
	    PN = N[e1] | N[e2];

	    for (e3 = nGEQ - 1; e3 >= 0; e3--)
	      if (e3 != e1 && e3 != e2)
		{

		  if (!implies (Z[e3], PZ))
		    goto nextE3;

		  alpha1 =
		    GEQs[e2].coef[q] * GEQs[e3].coef[p] -
		    GEQs[e2].coef[p] * GEQs[e3].coef[q];
		  alpha2 =
		    -(GEQs[e1].coef[q] * GEQs[e3].coef[p] -
		      GEQs[e1].coef[p] * GEQs[e3].coef[q]);
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
		      if (!GEQs[e3].color
			  && (GEQs[e1].color || GEQs[e2].color))
			goto nextE3;

		      /* verify alpha1*v1+alpha2*v2 = alpha3*v3 */
		      for (k = nVars; k >= 1; k--)
			if (alpha3 * GEQs[e3].coef[k]
			    !=
			    alpha1 * GEQs[e1].coef[k] +
			    alpha2 * GEQs[e2].coef[k])
			  goto nextE3;

		      c =
			alpha1 * GEQs[e1].coef[0] + alpha2 * GEQs[e2].coef[0];
		      if (c < alpha3 * (GEQs[e3].coef[0] + 1))
			{
			  if (DBUG)
			    {
			      fprintf (outputFile,
				       "found redundant inequality\n");
			      fprintf (outputFile,
				       "alpha1, alpha2, alpha3 = %d,%d,%d\n",
				       alpha1, alpha2, alpha3);

			      printGEQ (&(GEQs[e1]));
			      fprintf (outputFile, "\n");
			      printGEQ (&(GEQs[e2]));
			      fprintf (outputFile, "\n=> ");
			      printGEQ (&(GEQs[e3]));
			      fprintf (outputFile, "\n\n");
			    };

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
		      if (GEQs[e1].color || GEQs[e2].color || GEQs[e3].color)
			goto nextE3;

		      alpha3 = alpha3;
		      /* verify alpha1*v1+alpha2*v2 = alpha3*v3 */
		      for (k = nVars; k >= 1; k--)
			if (alpha3 * GEQs[e3].coef[k]
			    !=
			    alpha1 * GEQs[e1].coef[k] +
			    alpha2 * GEQs[e2].coef[k])
			  goto nextE3;

		      c =
			alpha1 * GEQs[e1].coef[0] + alpha2 * GEQs[e2].coef[0];
		      if (c < alpha3 * (GEQs[e3].coef[0]))
			{

			  /* We just proved e3 < 0, so no solutions exist */
			  if (DBUG)
			    {
			      fprintf (outputFile,
				       "found implied over tight inequality\n");
			      fprintf (outputFile,
				       "alpha1, alpha2, alpha3 = %d,%d,%d\n",
				       alpha1, alpha2, -alpha3);
			      printGEQ (&(GEQs[e1]));
			      fprintf (outputFile, "\n");
			      printGEQ (&(GEQs[e2]));
			      fprintf (outputFile, "\n=> not ");
			      printGEQ (&(GEQs[e3]));
			      fprintf (outputFile, "\n\n");
			    };
			  return (0);

			}
		      else if (c < alpha3 * (GEQs[e3].coef[0] - 1))
			{

			  /* We just proved that e3 <=0, so e3 = 0 */
			  if (DBUG)
			    {
			      fprintf (outputFile,
				       "found implied tight inequality\n");
			      fprintf (outputFile,
				       "alpha1, alpha2, alpha3 = %d,%d,%d\n",
				       alpha1, alpha2, -alpha3);
			      printGEQ (&(GEQs[e1]));
			      fprintf (outputFile, "\n");
			      printGEQ (&(GEQs[e2]));
			      fprintf (outputFile, "\n=> inverse ");
			      printGEQ (&(GEQs[e3]));
			      fprintf (outputFile, "\n\n");
			    };
			  eqncpy (&EQs[nEQ++], &GEQs[e3]);
			  gcc_assert (nEQ <= maxEQs);
			  addingEqualityConstraint (problemPtr, nEQ - 1);
			  isDead[e3] = 1;

			}
		    }
		nextE3:;
		}
	  }
  for (e = nGEQ - 1; e >= 0; e--)
    if (isDead[e])
      doDelete (e, nVars);

  /* if (nEQ) return (solve(problemPtr, SIMPLIFY)); */

  if (!expensive)
    return (1);
  {
    Problem *tmpProblem;
    int oldTrace = trace;
    trace = 0;
    tmpProblem = mallocProblem;
    conservative++;
    for (e = nGEQ - 1; e >= 0; e--)
      {
	if (DEBUG)
	  {
	    fprintf (outputFile,
		     "checking equation %d to see if it is redundant: ", e);
	    printGEQ (&(GEQs[e]));
	    fprintf (outputFile, "\n");
	  };
	problemcpy (tmpProblem, problemPtr);
	negateGEQ (tmpProblem, e);
	tmpProblem->_safeVars = 0;
	tmpProblem->variablesFreed = 0;
	if (!solve (tmpProblem, FALSE))
	  doDelete (e, nVars);
      };
    trace = oldTrace;
    free (tmpProblem);
    conservative--;
  };
  if (reduceWithSubs)
    {
    }
  else
    {
      resurrectSubs (problemPtr);
      gcc_assert (pleaseNoEqualitiesInSimplifiedProblems || nSUB == 0);
    }
  return (1);
}


int
smoothWeirdEquations (Problem * problemPtr)
{
  int e1, e2, e3, p, q, k, alpha, alpha1, alpha2, alpha3;
  int c;
  int v;
  int result = 0;

  for (e1 = nGEQ - 1; e1 >= 0; e1--)
    if (!GEQs[e1].color)
      {
	int g = 999999;
	for (v = nVars; v >= 1; v--)
	  if (GEQs[e1].coef[v] != 0 && abs (GEQs[e1].coef[v]) < g)
	    g = abs (GEQs[e1].coef[v]);
	if (g > 20)
	  {

	    e3 = nGEQ;
	    for (v = nVars; v >= 1; v--)
	      GEQs[e3].coef[v] = intDiv (6 * GEQs[e1].coef[v] + g / 2, g);
	    GEQs[e3].color = 0;
	    GEQs[e3].touched = 1;
	    GEQs[e3].coef[0] = 9997;
	    if (DBUG)
	      {
		fprintf (outputFile, "Checking to see if we can derive: ");
		printGEQ (&GEQs[e3]);
		fprintf (outputFile, "\n from: ");
		printGEQ (&GEQs[e1]);
		fprintf (outputFile, "\n");
	      };


	    for (e2 = nGEQ - 1; e2 >= 0; e2--)
	      if (e1 != e2 && !GEQs[e2].color)
		{
		  for (p = nVars; p > 1; p--)
		    {
		      for (q = p - 1; q > 0; q--)
			{
			  alpha =
			    (GEQs[e1].coef[p] * GEQs[e2].coef[q] -
			     GEQs[e2].coef[p] * GEQs[e1].coef[q]);
			  if (alpha != 0)
			    goto foundPQ;
			};
		    };
		  continue;

		foundPQ:

		  alpha1 =
		    GEQs[e2].coef[q] * GEQs[e3].coef[p] -
		    GEQs[e2].coef[p] * GEQs[e3].coef[q];
		  alpha2 =
		    -(GEQs[e1].coef[q] * GEQs[e3].coef[p] -
		      GEQs[e1].coef[p] * GEQs[e3].coef[q]);
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
		      for (k = nVars; k >= 1; k--)
			if (alpha3 * GEQs[e3].coef[k]
			    !=
			    alpha1 * GEQs[e1].coef[k] +
			    alpha2 * GEQs[e2].coef[k])
			  goto nextE2;

		      c =
			alpha1 * GEQs[e1].coef[0] + alpha2 * GEQs[e2].coef[0];
		      if (c < alpha3 * (GEQs[e3].coef[0] + 1))
			GEQs[e3].coef[0] = intDiv (c, alpha3);

		    }
		nextE2:;
		}
	    if (GEQs[e3].coef[0] < 9997)
	      {
		result++;
		nGEQ++;
		if (DBUG)
		  {
		    fprintf (outputFile,
			     "Smoothing wierd equations; adding:\n");
		    printGEQ (&GEQs[e3]);
		    fprintf (outputFile, "\nto:\n");
		    printProblem (problemPtr);
		    fprintf (outputFile, "\n\n");
		  };
	      };
	  }
      }
  return (result);
}

void
coalesce (Problem * problemPtr)
{
  int e, e2, colors;
  int isDead[maxGEQs];
  int foundSomething = 0;


  colors = 0;
  for (e = 0; e < nGEQ; e++)
    if (GEQs[e].color)
      colors++;
  if (colors < 2)
    return;
  for (e = 0; e < nGEQ; e++)
    isDead[e] = 0;
  for (e = 0; e < nGEQ; e++)
    if (GEQs[e].color & !GEQs[e].touched)
      for (e2 = e + 1; e2 < nGEQ; e2++)
	if (!GEQs[e2].touched && GEQs[e].key == -GEQs[e2].key
	    && GEQs[e].coef[0] == -GEQs[e2].coef[0] && GEQs[e2].color)
	  {
	    eqncpy (&EQs[nEQ++], &GEQs[e]);
	    gcc_assert (nEQ <= maxEQs);
	    foundSomething++;
	    isDead[e] = 1;
	    isDead[e2] = 1;
	  };
  for (e = nGEQ - 1; e >= 0; e--)
    if (isDead[e])
      {
	doDelete (e, nVars);
      }
  if (DEBUG && foundSomething)
    {
      fprintf (outputFile, "Coalesced GEQs into %d EQ's:\n", foundSomething);
      printProblem (problemPtr);
    };

}

void
eliminateRed (Problem * problemPtr, bool eliminateAll)
{
  int e, e2, e3, i, j, k, a, alpha1, alpha2;
  int c = 0;
  int isDead[maxGEQs];
  int deadCount = 0;

  if (DBUG)
    {
      fprintf (outputFile, "in eliminate RED:\n");
      printProblem (problemPtr);
    };
  if (nEQ > 0)
    {
      simplifyProblem (problemPtr);
    };

  for (e = nGEQ - 1; e >= 0; e--)
    isDead[e] = 0;
  for (e = nGEQ - 1; e >= 0; e--)
    if (!GEQs[e].color && !isDead[e])
      for (e2 = e - 1; e2 >= 0; e2--)
	if (!GEQs[e2].color && !isDead[e2])
	  {
	    a = 0;
	    for (i = nVars; i > 1; i--)
	      {
		for (j = i - 1; j > 0; j--)
		  {
		    a =
		      (GEQs[e].coef[i] * GEQs[e2].coef[j] -
		       GEQs[e2].coef[i] * GEQs[e].coef[j]);
		    if (a != 0)
		      goto foundPair;
		  };
	      };
	    continue;

	  foundPair:
	    if (DEBUG)
	      {
		fprintf (outputFile,
			 "found two equations to combine, i = %s, ",
			 variable (i));
		fprintf (outputFile, "j = %s, alpha = %d\n", variable (j), a);
		printGEQ (&(GEQs[e]));
		fprintf (outputFile, "\n");
		printGEQ (&(GEQs[e2]));
		fprintf (outputFile, "\n");
	      };
	    for (e3 = nGEQ - 1; e3 >= 0; e3--)
	      if (GEQs[e3].color)
		{
		  alpha1 =
		    GEQs[e2].coef[j] * GEQs[e3].coef[i] -
		    GEQs[e2].coef[i] * GEQs[e3].coef[j];
		  alpha2 =
		    -(GEQs[e].coef[j] * GEQs[e3].coef[i] -
		      GEQs[e].coef[i] * GEQs[e3].coef[j]);
		  if ((a > 0 && alpha1 > 0 && alpha2 > 0)
		      || (a < 0 && alpha1 < 0 && alpha2 < 0))
		    {
		      if (DEBUG)
			{
			  fprintf (outputFile,
				   "alpha1 = %d, alpha2 = %d; comparing against: ",
				   alpha1, alpha2);
			  printGEQ (&(GEQs[e3]));
			  fprintf (outputFile, "\n");
			};
		      for (k = nVars; k >= 0; k--)
			{
			  c =
			    alpha1 * GEQs[e].coef[k] +
			    alpha2 * GEQs[e2].coef[k];
			  if (c != a * GEQs[e3].coef[k])
			    break;
			  if (DEBUG && k > 0)
			    fprintf (outputFile, " %s: %d, %d\n",
				     variable (k), c, a * GEQs[e3].coef[k]);
			};
		      if (k < 0
			  || (k == 0 &&
			      ((a > 0 && c < a * GEQs[e3].coef[k])
			       || (a < 0 && c > a * GEQs[e3].coef[k]))))
			{
			  if (DEBUG)
			    {
			      deadCount++;
			      fprintf (outputFile,
				       "red equation#%d is dead (%d dead so far, %d remain)\n",
				       e3, deadCount, nGEQ - deadCount);
			      printGEQ (&(GEQs[e]));
			      fprintf (outputFile, "\n");
			      printGEQ (&(GEQs[e2]));
			      fprintf (outputFile, "\n");
			      printGEQ (&(GEQs[e3]));
			      fprintf (outputFile, "\n");
			    };
			  isDead[e3] = 1;
			};
		    };
		};
	  };
  for (e = nGEQ - 1; e >= 0; e--)
    if (isDead[e])
      doDelete (e, nVars);


  if (DBUG)
    {
      fprintf (outputFile, "in eliminate RED, easy tests done:\n");
      printProblem (problemPtr);
    };

  {
    int redFound = 0;
    for (e = nGEQ - 1; e >= 0; e--)
      if (GEQs[e].color)
	redFound = 1;
    if (!redFound)
      {
	if (DBUG)
	  fprintf (outputFile, "fast checks worked\n");
	if (reduceWithSubs)
	  {
	  }
	else
	  {
	    gcc_assert (pleaseNoEqualitiesInSimplifiedProblems || nSUB == 0);
	  }
	return;
      };
  }

  if (!verifySimplification)
    {
      if (!verifyProblem (problemPtr))
	return;
    }
  {
    Problem *tmpProblem;
    int oldTrace = trace;
    trace = 0;
    conservative++;
    tmpProblem = mallocProblem;
    for (e = nGEQ - 1; e >= 0; e--)
      if (GEQs[e].color)
	{
	  if (DBUG)
	    {
	      fprintf (outputFile,
		       "checking equation %d to see if it is redundant: ", e);
	      printGEQ (&(GEQs[e]));
	      fprintf (outputFile, "\n");
	    };
	  problemcpy (tmpProblem, problemPtr);
	  negateGEQ (tmpProblem, e);
	  tmpProblem->_safeVars = 0;
	  tmpProblem->variablesFreed = 0;
	  tmpProblem->_numSUBs = 0;
	  if (!solve (tmpProblem, FALSE))
	    {
	      if (DBUG)
		fprintf (outputFile, "it is redundant\n");
	      doDelete (e, nVars);
	    }
	  else
	    {
	      if (DBUG)
		fprintf (outputFile, "it is not redundant\n");
	      if (!eliminateAll)
		{
		  if (DBUG)
		    fprintf (outputFile,
			     "no need to check other red equations\n");
		  break;
		}
	    };
	};
    trace = oldTrace;
    conservative--;
    free (tmpProblem);
  };
  /* simplifyProblem(problemPtr); */
  if (reduceWithSubs)
    {
    }
  else
    {
      gcc_assert (pleaseNoEqualitiesInSimplifiedProblems || nSUB == 0);
    }
}



void
swap (int *i, int *j)
{
  int tmp;
  tmp = *i;
  *i = *j;
  *j = tmp;
}

void
chainUnprotect (Problem * problemPtr)
{
  int i, e;
  int unprotect[maxVars];
  for (i = 1; i <= problemPtr->_safeVars; i++)
    {
      unprotect[i] = (problemPtr->_var[i] < 0);
      for (e = nSUB - 1; e >= 0; e--)
	if (SUBs[e].coef[i])
	  unprotect[i] = 0;
    };
  if (DBUG)
    {
      fprintf (outputFile, "Doing chain reaction unprotection\n");
      printProblem (problemPtr);
      for (i = 1; i <= problemPtr->_safeVars; i++)
	if (unprotect[i])
	  fprintf (outputFile, "unprotecting %s\n", variable (i));
    };
  for (i = 1; i <= problemPtr->_safeVars; i++)
    if (unprotect[i])
      {
	/* wild card */
	if (i < problemPtr->_safeVars)
	  {
	    int j = problemPtr->_safeVars;
	    swap (&problemPtr->_var[i], &problemPtr->_var[j]);
	    for (e = nGEQ - 1; e >= 0; e--)
	      {
		GEQs[e].touched = 1;
		swap (&GEQs[e].coef[i], &GEQs[e].coef[j]);
	      };
	    for (e = nEQ - 1; e >= 0; e--)
	      swap (&EQs[e].coef[i], &EQs[e].coef[j]);
	    for (e = nSUB - 1; e >= 0; e--)
	      swap (&SUBs[e].coef[i], &SUBs[e].coef[j]);
	    swap (&unprotect[i], &unprotect[j]);
	    i--;
	  };
	problemPtr->_safeVars--;
      };
  if (DBUG)
    {
      fprintf (outputFile, "After chain reactions\n");
      printProblem (problemPtr);
    };
}


void
cleanoutWildcards (Problem * problemPtr)
{
  int e, e2, i, j;
  int a, c, nV;
  int g;
  int renormalize = 0;
  struct _eqn *sub;
  nV = nVars;
  for (e = nEQ - 1; e >= 0; e--)
    {
      for (i = nV; i >= problemPtr->_safeVars + 1; i--)
	if (EQs[e].coef[i])
	  {
	    for (j = i - 1; j >= problemPtr->_safeVars + 1; j--)
	      if (EQs[e].coef[j])
		break;
	    if (j < problemPtr->_safeVars + 1)
	      {
		/* Found a single wild card equality */
		if (DBUG)
		  {
		    fprintf (outputFile,
			     "Found a single wild card equality: ");
		    printEQ (&EQs[e]);
		    fprintf (outputFile, "\n");
		    printProblem (problemPtr);
		  }
		c = EQs[e].coef[i];
		a = abs (c);
		sub = &(EQs[e]);

		for (e2 = nEQ - 1; e2 >= 0; e2--)
		  if (e != e2 && EQs[e2].coef[i]
		      && EQs[e].color <= EQs[e2].color)
		    {
		      register Eqn eqn = &(EQs[e2]);
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

		    };
		for (e2 = nGEQ - 1; e2 >= 0; e2--)
		  if (GEQs[e2].coef[i] && EQs[e].color <= GEQs[e2].color)
		    {
		      register Eqn eqn = &(GEQs[e2]);
		      int j, k;
		      for (j = nV; j >= 0; j--)
			eqn->coef[j] *= a;
		      k = eqn->coef[i];
		      for (j = nV; j >= 0; j--)
			eqn->coef[j] -= sub->coef[j] * k / c;
		      eqn->coef[i] = 0;
		      eqn->touched = 1;
		      renormalize = 1;
		    };
		for (e2 = nSUB - 1; e2 >= 0; e2--)
		  if (SUBs[e2].coef[i] && EQs[e].color <= SUBs[e2].color)
		    {
		      register Eqn eqn = &(SUBs[e2]);
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
		    };

		if (DBUG)
		  {
		    fprintf (outputFile, "cleaned-out wildcard: ");
		    printProblem (problemPtr);
		  }
		break;
	      }
	  }
    }
  if (renormalize)
    normalize (problemPtr);
}

void
resurrectSubs (Problem * problemPtr)
{
  if (problemPtr->_numSUBs > 0 && !pleaseNoEqualitiesInSimplifiedProblems)
    {
      int i, e, n, m;
      if (DBUG)
	{
	  fprintf (outputFile,
		   "problem reduced, bringing variables back to life\n");
	  printProblem (problemPtr);
	};
      for (i = 1; i <= problemPtr->_safeVars; i++)
	if (problemPtr->_var[i] < 0)
	  {
	    /* wild card */
	    if (i < problemPtr->_safeVars)
	      {
		int j = problemPtr->_safeVars;
		swap (&problemPtr->_var[i], &problemPtr->_var[j]);
		for (e = nGEQ - 1; e >= 0; e--)
		  {
		    GEQs[e].touched = 1;
		    swap (&GEQs[e].coef[i], &GEQs[e].coef[j]);
		  };
		for (e = nEQ - 1; e >= 0; e--)
		  swap (&EQs[e].coef[i], &EQs[e].coef[j]);
		for (e = nSUB - 1; e >= 0; e--)
		  swap (&SUBs[e].coef[i], &SUBs[e].coef[j]);
		i--;
	      };
	    problemPtr->_safeVars--;
	  };

      m = problemPtr->_numSUBs;
      n = max (nVars, problemPtr->_safeVars + m);
      for (e = nGEQ - 1; e >= 0; e--)
	{
	  if (singleVarGEQ (GEQs[e], nVars))
	    {
	      i = abs (GEQs[e].key);
	      if (i >= problemPtr->_safeVars + 1)
		GEQs[e].key += (GEQs[e].key > 0 ? m : -m);
	    }
	  else
	    {
	      GEQs[e].touched = TRUE;
	      GEQs[e].key = 0;
	    }
	};
      for (i = nVars; i >= problemPtr->_safeVars + 1; i--)
	{
	  problemPtr->_var[i + m] = problemPtr->_var[i];
	  for (e = nGEQ - 1; e >= 0; e--)
	    GEQs[e].coef[i + m] = GEQs[e].coef[i];
	  for (e = nEQ - 1; e >= 0; e--)
	    EQs[e].coef[i + m] = EQs[e].coef[i];
	  for (e = nSUB - 1; e >= 0; e--)
	    SUBs[e].coef[i + m] = SUBs[e].coef[i];
	};
      for (i = problemPtr->_safeVars + m; i >= problemPtr->_safeVars + 1; i--)
	{
	  for (e = nGEQ - 1; e >= 0; e--)
	    GEQs[e].coef[i] = 0;
	  for (e = nEQ - 1; e >= 0; e--)
	    EQs[e].coef[i] = 0;
	  for (e = nSUB - 1; e >= 0; e--)
	    SUBs[e].coef[i] = 0;
	};
      nVars += m;
      for (e = nSUB - 1; e >= 0; e--)
	{
	  problemPtr->_var[problemPtr->_safeVars + 1 + e] = SUBs[e].key;
	  eqncpy (&(EQs[nEQ]), &(SUBs[e]));
	  EQs[nEQ].coef[problemPtr->_safeVars + 1 + e] = -1;
	  EQs[nEQ].color = black;
	  if (DBUG)
	    {
	      fprintf (outputFile, "brought back: ");
	      printEQ (&EQs[nEQ]);
	      fprintf (outputFile, "\n");
	    };


	  nEQ++;
	  gcc_assert (nEQ <= maxEQs);
	};
      problemPtr->_safeVars += m;
      nSUB = 0;

      if (DBUG)
	{
	  fprintf (outputFile, "variables brought back to life\n");
	  printProblem (problemPtr);
	};

      cleanoutWildcards (problemPtr);

    }
}


static void
problemReduced (Problem * problemPtr)
{
  if (verifySimplification)
    {
      int result;
      if (inApproximateMode)
	result = 1;
      else
	result = verifyProblem (problemPtr);
      if (!result)
	return;
      if (nEQ > 0)
	doItAgain = 1;
    }
#ifdef eliminateRedundantConstraints
  if (!eliminateRedundant (problemPtr, 1))
    return;
#endif
  foundReduction = TRUE;
  if (!pleaseNoEqualitiesInSimplifiedProblems)
    coalesce (problemPtr);
  if (reduceWithSubs || pleaseNoEqualitiesInSimplifiedProblems)
    chainUnprotect (problemPtr);
  else
    resurrectSubs (problemPtr);

  if (!returnSingleResult)
    {
      int i;
      for (i = 1; i <= safeVars; i++)
	problemPtr->forwardingAddress[problemPtr->_var[i]] = i;
      for (i = 0; i < nSUB; i++)
	problemPtr->forwardingAddress[SUBs[i].key] = -i - 1;
      (*whenReduced) (problemPtr);
    }
  if (omegaPrintResult == 1)
    {
      fprintf (outputFile, "-------------------------------------------\n");
      fprintf (outputFile, "problem reduced:\n");
      printProblem (problemPtr);
      fprintf (outputFile, "-------------------------------------------\n");
    };


}


static void
freeEliminations (Problem * problemPtr, int fv)
/* do free eliminations */
{
  int tryAgain = 1;
  int i, e, e2;
  int nV = nVars;
  while (tryAgain)
    {
      tryAgain = 0;
      for (i = nV; i > fv; i--)
	{
	  for (e = nGEQ - 1; e >= 0; e--)
	    if (GEQs[e].coef[i])
	      break;
	  if (e < 0)
	    e2 = e;
	  else if (GEQs[e].coef[i] > 0)
	    {
	      for (e2 = e - 1; e2 >= 0; e2--)
		if (GEQs[e2].coef[i] < 0)
		  break;
	    }
	  else
	    {
	      for (e2 = e - 1; e2 >= 0; e2--)
		if (GEQs[e2].coef[i] > 0)
		  break;
	    };
	  if (e2 < 0)
	    {
	      int e3;
	      for (e3 = nSUB - 1; e3 >= 0; e3--)
		if (SUBs[e3].coef[i])
		  break;
	      if (e3 >= 0)
		continue;
	      for (e3 = nEQ - 1; e3 >= 0; e3--)
		if (EQs[e3].coef[i])
		  break;
	      if (e3 >= 0)
		continue;
	      if (DBUG)
		fprintf (outputFile, "a free elimination of %s\n",
			 variable (i));
	      if (e >= 0)
		{
		  doDelete (e, nV);
		  for (e--; e >= 0; e--)
		    if (GEQs[e].coef[i])
		      doDelete (e, nV);
		  tryAgain = (i < nV);
		};
	      deleteVariable (problemPtr, i);
	      nV = nVars;
	    };
	};
    };

  if (DEBUG)
    {
      fprintf (outputFile, "\nafter free eliminations:\n");
      printProblem (problemPtr);
      fprintf (outputFile, "\n");
    };
}

static void
freeRedEliminations (Problem * problemPtr)
/* do free red eliminations */
{
  int tryAgain = 1;
  int i, e, e2;
  int nV = nVars;
  int isRedVar[maxVars];
  int isDeadVar[maxVars];
  int isDeadGEQ[maxGEQs];
  for (i = nV; i > 0; i--)
    {
      isRedVar[i] = 0;
      isDeadVar[i] = 0;
    };
  for (e = nGEQ - 1; e >= 0; e--)
    {
      isDeadGEQ[e] = 0;
      if (GEQs[e].color)
	for (i = nV; i > 0; i--)
	  if (GEQs[e].coef[i] != 0)
	    isRedVar[i] = 1;
    };

  while (tryAgain)
    {
      tryAgain = 0;
      for (i = nV; i > 0; i--)
	if (!isRedVar[i] && !isDeadVar[i])
	  {
	    for (e = nGEQ - 1; e >= 0; e--)
	      if (!isDeadGEQ[e] && GEQs[e].coef[i])
		break;
	    if (e < 0)
	      e2 = e;
	    else if (GEQs[e].coef[i] > 0)
	      {
		for (e2 = e - 1; e2 >= 0; e2--)
		  if (!isDeadGEQ[e2] && GEQs[e2].coef[i] < 0)
		    break;
	      }
	    else
	      {
		for (e2 = e - 1; e2 >= 0; e2--)
		  if (!isDeadGEQ[e2] && GEQs[e2].coef[i] > 0)
		    break;
	      };
	    if (e2 < 0)
	      {
		int e3;
		for (e3 = nSUB - 1; e3 >= 0; e3--)
		  if (SUBs[e3].coef[i])
		    break;
		if (e3 >= 0)
		  continue;
		for (e3 = nEQ - 1; e3 >= 0; e3--)
		  if (EQs[e3].coef[i])
		    break;
		if (e3 >= 0)
		  continue;
		if (DBUG)
		  fprintf (outputFile, "a free red elimination of %s\n",
			   variable (i));
		for (; e >= 0; e--)
		  if (GEQs[e].coef[i])
		    isDeadGEQ[e] = 1;
		tryAgain = 1;
		isDeadVar[i] = 1;
	      };
	  };
    };

  for (e = nGEQ - 1; e >= 0; e--)
    if (isDeadGEQ[e])
      doDelete (e, nV);

  for (i = nV; i > 0; i--)
    if (isDeadVar[i])
      deleteVariable (problemPtr, i);


  if (DEBUG)
    {
      fprintf (outputFile, "\nafter free red eliminations:\n");
      printProblem (problemPtr);
      fprintf (outputFile, "\n");
    };
}

void
addingEqualityConstraint (Problem * problemPtr, int e)
{
  int e2, i, j;

  if (originalProblem != noProblem && originalProblem != problemPtr
      && !conservative)
    {
      e2 = originalProblem->_numEQs++;
      if (TRACE)
	fprintf (outputFile,
		 "adding equality constraint %d to outer problem\n", e2);
      eqnnzero (&originalProblem->_EQs[e2], originalProblem->_nVars);
      for (i = nVars; i >= 1; i--)
	{
	  for (j = originalProblem->_nVars; j >= 1; j--)
	    if (originalProblem->_var[j] == problemPtr->_var[i])
	      break;
	  if (j <= 0)
	    {
	      if (DBUG)
		fprintf (outputFile, "retracting\n");
	      originalProblem->_numEQs--;
	      return;
	    };
	  originalProblem->_EQs[e2].coef[j] = EQs[e].coef[i];
	};
      originalProblem->_EQs[e2].coef[0] = EQs[e].coef[0];
      if (DBUG)
	printProblem (originalProblem);
    };
}




void
substitute (Problem * problemPtr, Eqn sub, int i, int c)
{
  int e, j, j0, k;
  int topVar;

  {
    int *p = &packing[0];
    for (k = nVars; k >= 0; k--)
      if (sub->coef[k])
	*(p++) = k;
    topVar = (p - &packing[0]) - 1;
  };


  if (DBUG || doTrace)
    {
      fprintf (outputFile, "substituting using %s := ", variable (i));
      printTerm (problemPtr, sub, -c);
      fprintf (outputFile, "\n");
      printVars (problemPtr);
    };

  if (topVar < 0)
    {
      for (e = nEQ - 1; e >= 0; e--)
	EQs[e].coef[i] = 0;
      for (e = nGEQ - 1; e >= 0; e--)
	if (GEQs[e].coef[i] != 0)
	  {
	    GEQs[e].touched = TRUE;
	    GEQs[e].coef[i] = 0;
	  };
      for (e = nSUB - 1; e >= 0; e--)
	SUBs[e].coef[i] = 0;

      if (i <= problemPtr->_safeVars && problemPtr->_var[i] >= 0)
	{
	  register Eqn eqn = &(SUBs[nSUB++]);
	  for (k = nVars; k >= 0; k--)
	    eqn->coef[k] = 0;
	  eqn->key = problemPtr->_var[i];
	  eqn->color = 0;
	};
    }
  else if (topVar == 0 && packing[0] == 0)
    {
      c = -sub->coef[0] * c;
      for (e = nEQ - 1; e >= 0; e--)
	{
	  EQs[e].coef[0] += EQs[e].coef[i] * c;
	  EQs[e].coef[i] = 0;
	};
      for (e = nGEQ - 1; e >= 0; e--)
	if (GEQs[e].coef[i] != 0)
	  {
	    GEQs[e].coef[0] += GEQs[e].coef[i] * c;
	    GEQs[e].coef[i] = 0;
	    GEQs[e].touched = TRUE;
	  };
      for (e = nSUB - 1; e >= 0; e--)
	{
	  SUBs[e].coef[0] += SUBs[e].coef[i] * c;
	  SUBs[e].coef[i] = 0;
	};
      if (i <= problemPtr->_safeVars && problemPtr->_var[i] >= 0)
	{
	  register Eqn eqn = &(SUBs[nSUB++]);
	  for (k = nVars; k >= 1; k--)
	    eqn->coef[k] = 0;
	  eqn->coef[0] = c;
	  eqn->key = problemPtr->_var[i];
	  eqn->color = 0;
	};
      if (DEBUG)
	{
	  fprintf (outputFile, "---\n\n");
	  printProblem (problemPtr);
	  fprintf (outputFile, "===\n\n");
	};
    }
  else
    {
      for (e = nEQ - 1; e >= 0; e--)
	{
	  register Eqn eqn = &(EQs[e]);
	  k = eqn->coef[i];
	  if (k != 0)
	    {
	      k = c * k;
	      eqn->coef[i] = 0;
	      for (j = topVar; j >= 0; j--)
		{
		  j0 = packing[j];
		  eqn->coef[j0] -= sub->coef[j0] * k;
		};
	    };
	  if (DEBUG)
	    {
	      printEQ (eqn);
	      fprintf (outputFile, "\n");
	    };
	};
      for (e = nGEQ - 1; e >= 0; e--)
	{
	  register Eqn eqn = &(GEQs[e]);
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
		};
	    };
	  if (DEBUG)
	    {
	      printGEQ (eqn);
	      fprintf (outputFile, "\n");
	    };
	};
      for (e = nSUB - 1; e >= 0; e--)
	{
	  register Eqn eqn = &(SUBs[e]);
	  k = eqn->coef[i];
	  if (k != 0)
	    {
	      k = c * k;
	      eqn->coef[i] = 0;
	      for (j = topVar; j >= 0; j--)
		{
		  j0 = packing[j];
		  eqn->coef[j0] -= sub->coef[j0] * k;
		};
	    };
	  if (DEBUG)
	    {
	      fprintf (outputFile, "%s := ", orgVariable (eqn->key));
	      printTerm (problemPtr, eqn, 1);
	      fprintf (outputFile, "\n");
	    };
	};

      if (DEBUG)
	{
	  fprintf (outputFile, "---\n\n");
	  printProblem (problemPtr);
	  fprintf (outputFile, "===\n\n");
	};
      if (i <= problemPtr->_safeVars && problemPtr->_var[i] >= 0)
	{
	  register Eqn eqn;
	  eqn = &(SUBs[nSUB++]);
	  c = -c;
	  for (k = nVars; k >= 0; k--)
	    eqn->coef[k] = c * (sub->coef[k]);
	  eqn->key = problemPtr->_var[i];
	  eqn->color = sub->color;
	};
    };

}

void
substituteRed (Problem * problemPtr, Eqn sub, int i, int c, bool * foundBlack)
{
  int e, j, j0, k;
  int topVar;

  {
    int *p = &packing[0];
    for (k = nVars; k >= 0; k--)
      if (sub->coef[k])
	*(p++) = k;
    topVar = (p - &packing[0]) - 1;
  };

  *foundBlack = 0;

  if (DBUG || doTrace)
    {
      if (sub->color)
	fprintf (outputFile, "[");
      fprintf (outputFile, "substituting using %s := ", variable (i));
      printTerm (problemPtr, sub, -c);
      if (sub->color)
	fprintf (outputFile, "]");
      fprintf (outputFile, "\n");
      printVars (problemPtr);
    };

  for (e = nEQ - 1; e >= 0; e--)
    {
      register Eqn eqn = &(EQs[e]);
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
		};
	    };
	};
      if (DEBUG)
	{
	  printEQ (eqn);
	  fprintf (outputFile, "\n");
	};
    };
  for (e = nGEQ - 1; e >= 0; e--)
    {
      register Eqn eqn = &(GEQs[e]);
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
		};
	    };
	};
      if (DEBUG)
	{
	  printGEQ (eqn);
	  fprintf (outputFile, "\n");
	};
    };
  for (e = nSUB - 1; e >= 0; e--)
    {
      register Eqn eqn = &(SUBs[e]);
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
		};
	    };
	};
      if (DEBUG)
	{
	  fprintf (outputFile, "%s := ", orgVariable (eqn->key));
	  printTerm (problemPtr, eqn, 1);
	  fprintf (outputFile, "\n");
	};
    };

  if (DEBUG)
    fprintf (outputFile, "---\n\n");
  if (i <= problemPtr->_safeVars && problemPtr->_var[i] >= 0)
    {
      *foundBlack = 1;
    };

}



void
deleteVariable (Problem * problemPtr, int i)
{
  int nV = nVars;
  int e;
  if (i < safeVars)
    {
      int j = safeVars;
      for (e = nGEQ - 1; e >= 0; e--)
	{
	  GEQs[e].touched = TRUE;
	  GEQs[e].coef[i] = GEQs[e].coef[j];
	  GEQs[e].coef[j] = GEQs[e].coef[nV];
	};
      for (e = nEQ - 1; e >= 0; e--)
	{
	  EQs[e].coef[i] = EQs[e].coef[j];
	  EQs[e].coef[j] = EQs[e].coef[nV];
	};
      for (e = nSUB - 1; e >= 0; e--)
	{
	  SUBs[e].coef[i] = SUBs[e].coef[j];
	  SUBs[e].coef[j] = SUBs[e].coef[nV];
	};
      problemPtr->_var[i] = problemPtr->_var[j];
      problemPtr->_var[j] = problemPtr->_var[nV];
    }
  else if (i < nV)
    {
      for (e = nGEQ - 1; e >= 0; e--)
	if (GEQs[e].coef[nV])
	  {
	    GEQs[e].coef[i] = GEQs[e].coef[nV];
	    GEQs[e].touched = TRUE;
	  };
      for (e = nEQ - 1; e >= 0; e--)
	EQs[e].coef[i] = EQs[e].coef[nV];
      for (e = nSUB - 1; e >= 0; e--)
	SUBs[e].coef[i] = SUBs[e].coef[nV];
      problemPtr->_var[i] = problemPtr->_var[nV];
    };
  if (i <= safeVars)
    safeVars--;
  nVars--;
}

void
convertEQtoGEQs (Problem * problemPtr, int eq)
{
  int i;
  if (DBUG)
    fprintf (outputFile, "Converting Eq to GEQs\n");
  eqncpy (&GEQs[nGEQ], &EQs[eq]);
  GEQs[nGEQ].touched = 1;
  nGEQ++;
  eqncpy (&GEQs[nGEQ], &EQs[eq]);
  GEQs[nGEQ].touched = 1;
  for (i = 0; i <= nVars; i++)
    GEQs[nGEQ].coef[i] = -GEQs[nGEQ].coef[i];
  nGEQ++;
  if (DBUG)
    printProblem (problemPtr);
}


static void
doElimination (Problem * problemPtr, int e, int i)
{
  struct _eqn sub;
  int c;
  int nV = nVars;
  if (DBUG || doTrace)
    fprintf (outputFile, "eliminating variable %s\n", variable (i));
  eqncpy (&sub, &EQs[e]);
  c = sub.coef[i];
  sub.coef[i] = 0;
  if (c == 1 || c == -1)
    {
      if (EQs[e].color)
	{
	  bool fB;
	  substituteRed (problemPtr, &sub, i, c, &fB);
	  if (fB)
	    convertEQtoGEQs (problemPtr, e);
	  else
	    deleteVariable (problemPtr, i);
	}
      else
	{
	  substitute (problemPtr, &sub, i, c);
	  deleteVariable (problemPtr, i);
	}
    }
  else
    {
      int a = abs (c);
      int e2 = e;
      if (TRACE)
	fprintf (outputFile, "performing non-exact elimination, c = %d\n", c);
      for (e = nEQ - 1; e >= 0; e--)
	if (EQs[e].coef[i])
	  {
	    register Eqn eqn = &(EQs[e]);
	    int j, k;
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] *= a;
	    k = eqn->coef[i];
	    eqn->coef[i] = 0;
	    eqn->color |= sub.color;
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] -= sub.coef[j] * k / c;
	  };
      for (e = nGEQ - 1; e >= 0; e--)
	if (GEQs[e].coef[i])
	  {
	    register Eqn eqn = &(GEQs[e]);
	    int j, k;
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] *= a;
	    k = eqn->coef[i];
	    eqn->coef[i] = 0;
	    eqn->color |= sub.color;
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] -= sub.coef[j] * k / c;
	    eqn->touched = 1;
	  };
      for (e = nSUB - 1; e >= 0; e--)
	if (SUBs[e].coef[i])
	  {
	    register Eqn eqn = &(SUBs[e]);
	    int j, k;
	    gcc_assert (0);
	    gcc_assert (!sub.color);
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] *= a;
	    k = eqn->coef[i];
	    eqn->coef[i] = 0;
	    for (j = nV; j >= 0; j--)
	      eqn->coef[j] -= sub.coef[j] * k / c;
	  };
      if (inApproximateMode)
	deleteVariable (problemPtr, i);
      else
	convertEQtoGEQs (problemPtr, e2);
    };
}

static int
solveEQ (register Problem * problemPtr, int desiredResult)
{
  int i, j, e;
  int g, g2;
  g = 0;


  if (DBUG && nEQ > 0)
    {
      fprintf (outputFile, "\nSolveEQ(%d,%d)\n", desiredResult, mayBeRed);
      printProblem (problemPtr);
      fprintf (outputFile, "\n");
    };

  if (mayBeRed)
    {
      i = 0;
      j = nEQ - 1;
      while (1)
	{
	  struct _eqn eq;
	  while (i <= j && EQs[i].color)
	    i++;
	  while (i <= j && !EQs[j].color)
	    j--;
	  if (i >= j)
	    break;
	  eqncpy (&eq, &EQs[i]);
	  eqncpy (&EQs[i], &EQs[j]);
	  eqncpy (&EQs[j], &eq);
	  i++;
	  j--;
	};
    };



  /* Eliminate all EQ equations */
  for (e = nEQ - 1; e >= 0; e--)
    {
      register Eqn eqn = &(EQs[e]);
      int sv;

      /*
       * huh? if (inApproximateMode && !nGEQ && safeVars == nVars) return (TRUE);
       */

      if (DEBUG)
	fprintf (outputFile, "----\n");
      for (i = nVars; i > 0; i--)
	if ((g = eqn->coef[i]) != 0)
	  break;
      /*
       * if (isRed(eqn)) { if (DBUG) fprintf(outputFile, "Can't handle Red equality\n");
       * convertEQtoGEQs(problemPtr,eqn); if (DBUG) printProblem(problemPtr); continue; };
       */
      for (j = i - 1; j > 0; j--)
	if (eqn->coef[j])
	  break;
      if (j == 0)
	{
	  if (eqn->coef[0] % g != 0)
	    {
	      if (DBUG)
		printEQ (eqn);
	      if (DBUG)
		fprintf (outputFile, "\nequations have no solution \n");
	      return (FALSE);
	    };
	  eqn->coef[0] = eqn->coef[0] / g;
	  eqn->coef[i] = 1;
	  nEQ--;
	  doElimination (problemPtr, e, i);
	  continue;
	}
      else if (j == -1)
	{
	  if (eqn->coef[0] != 0)
	    {
	      if (DBUG)
		printEQ (eqn);
	      if (DBUG)
		fprintf (outputFile, "\nequations have no solution \n");
	      return (FALSE);
	    };
	  nEQ--;
	  continue;
	};
      /* i == position of last non-zero coef */
      /* g == coef of i */
      /* j == position of next non-zero coef */

      if (g < 0)
	g = -g;
      if (g == 1)
	{
	  nEQ--;
	  doElimination (problemPtr, e, i);
	}
      else
	{
	  int k = j;
	  bool promotionPossible =
	    (j <= safeVars && safeVars + 1 == i
	     && !isRed (eqn) && !inApproximateMode);
	  if (DEBUG && promotionPossible)
	    fprintf (outputFile, " Promotion possible\n");
	normalizeEQ:
	  if (j > safeVars)
	    {
	      for (; g != 1 && j > safeVars; j--)
		g = gcd (abs (eqn->coef[j]), g);
	      g2 = g;
	    }
	  else if (i > safeVars)
	    g2 = g;
	  else
	    g2 = 0;
	  for (; g != 1 && j > 0; j--)
	    g = gcd (abs (eqn->coef[j]), g);
	  if (g > 1)
	    {
	      if (eqn->coef[0] % g != 0)
		{
		  if (DBUG)
		    printEQ (eqn);
		  if (DBUG)
		    fprintf (outputFile, "\nequations have no solution \n");
		  return (FALSE);
		};

	      for (j = 0; j <= nVars; j++)
		eqn->coef[j] /= g;
	      g2 = g2 / g;
	    };
	  if (g2 > 1)
	    {
	      int e2;
	      for (e2 = e - 1; e2 >= 0; e2--)
		if (EQs[e2].coef[i])
		  break;
	      if (e2 == -1)
		for (e2 = nGEQ - 1; e2 >= 0; e2--)
		  if (GEQs[e2].coef[i])
		    break;
	      if (e2 == -1)
		for (e2 = nSUB - 1; e2 >= 0; e2--)
		  if (SUBs[e2].coef[i])
		    break;
	      if (e2 == -1)
		{
		  bool change = 0;
		  if (DBUG)
		    fprintf (outputFile, "Ha! We own it! \n");
		  if (DEBUG)
		    printEQ (eqn);
		  if (DEBUG)
		    fprintf (outputFile, " \n");
		  g = eqn->coef[i];
		  g = abs (g);
		  for (j = i - 1; j >= 0; j--)
		    {
		      int t = eqn->coef[j];
		      t = intMod (t, g);
		      if (2 * t >= g)
			t -= g;
		      if (t != eqn->coef[j])
			{
			  eqn->coef[j] = t;
			  change = 1;
			};
		    };
		  if (!change)
		    {
		      if (DBUG)
			fprintf (outputFile, "So what?\n");
		    }
		  else
		    {
		      nameWildcard (problemPtr, i);
		      if (DEBUG)
			printEQ (eqn);
		      if (DEBUG)
			fprintf (outputFile, " \n");
		      e++;
		      continue;
		    };
		};
	    }
	  if (promotionPossible)
	    {
	      if (DEBUG)
		{
		  fprintf (outputFile, "promoting %s to safety\n",
			   variable (i));
		  printVars (problemPtr);
		};
	      safeVars++;
	      if (problemPtr->_var[i] > 0)
		nameWildcard (problemPtr, i);
	      promotionPossible = 0;
	      j = k;
	      goto normalizeEQ;
	    };

	  if (g2 > 1 && !inApproximateMode)
	    {
	      if (EQs[e].color)
		{
		  if (DEBUG)
		    fprintf (outputFile, "handling red equality\n");
		  nEQ--;
		  doElimination (problemPtr, e, i);
		  continue;
		}
	      if (DEBUG)
		fprintf (outputFile,
			 "adding equation to handle safe variable \n");
	      if (DEBUG)
		printEQ (eqn);
	      if (DEBUG)
		fprintf (outputFile, "\n----\n");
	      if (DEBUG)
		printProblem (problemPtr);
	      if (DEBUG)
		fprintf (outputFile, "\n----\n");
	      if (DEBUG)
		fprintf (outputFile, "\n----\n");
	      i = addNewWildcard (problemPtr);
	      nEQ++;
	      gcc_assert (nEQ <= maxEQs);
	      eqnzero (&EQs[e + 1]);
	      eqnncpy (&EQs[e + 1], eqn, safeVars);
	      for (j = nVars; j >= 0; j--)
		{
		  EQs[e + 1].coef[j] = intMod (EQs[e + 1].coef[j], g2);
		  if (2 * EQs[e + 1].coef[j] >= g2)
		    EQs[e + 1].coef[j] -= g2;
		};
	      EQs[e + 1].coef[i] = g2;
	      e += 2;
	      if (DEBUG)
		printProblem (problemPtr);
	      continue;
	    };

	  sv = safeVars;
	  if (g2 == 0)
	    sv = 0;

	  /* find variable to eliminate */
	  if (g2 > 1)
	    {
	      gcc_assert (inApproximateMode);
	      if (TRACE)
		{
		  fprintf (outputFile, "non-exact elimination: ");
		  printEQ (eqn);
		  fprintf (outputFile, "\n");
		  printProblem (problemPtr);
		  fflush (outputFile);
		};
	      for (i = nVars; i > sv; i--)
		if (EQs[e].coef[i] != 0)
		  break;
	    }
	  else
	    for (i = nVars; i > sv; i--)
	      if (EQs[e].coef[i] == 1 || EQs[e].coef[i] == -1)
		break;

	  if (i > sv)
	    {
	      nEQ--;
	      doElimination (problemPtr, e, i);
	      if (g2 > 1 && TRACE)
		{
		  fprintf (outputFile, "result of non-exact elimination:\n");
		  printProblem (problemPtr);
		  fflush (outputFile);
		};
	    }
	  else
	    {
	      int factor = (INT_MAX);
	      j = 0;
	      if (DEBUG)
		fprintf (outputFile, "doing moding\n");
	      for (i = nVars; i != sv; i--)
		if ((EQs[e].coef[i] & 1) != 0)
		  {
		    j = i;
		    i--;
		    for (; i != sv; i--)
		      if ((EQs[e].coef[i] & 1) != 0)
			break;
		    break;
		  };
	      if (j != 0 && i == sv)
		{
		  doMod (problemPtr, 2, e, j);
		  e++;
		  continue;
		};

	      j = 0;
	      for (i = nVars; i != sv; i--)
		if (EQs[e].coef[i] != 0 && factor > abs (EQs[e].coef[i]) + 1)
		  {
		    factor = abs (EQs[e].coef[i]) + 1;
		    j = i;
		  };
	      if (j == sv)
		{
		  fprintf (outputFile, "should not have happened\n");
		  exit (2);
		};
	      doMod (problemPtr, factor, e, j);
	      /* go back and try this equation again */
	      e++;
	    };
	};


    };
  nEQ = 0;
  return (UNKNOWN);
}


static int solveGEQ (register Problem * problemPtr, int desiredResult);

static int solveDepth = 0;

int
solve (Problem * problemPtr, int desiredResult)
{
  int result;

  gcc_assert (nVars >= safeVars);
  if (desiredResult != SIMPLIFY)
    safeVars = 0;

  solveDepth++;
  if (solveDepth > 50)
    {
      fprintf (outputFile, "Solve depth = %d, inApprox = %d, aborting\n",
	       solveDepth, inApproximateMode);
      printProblem (problemPtr);
      fflush (outputFile);
#ifndef SPEED
      DD_DEBUG_OMEGA = 3;
#else
      exit (2);
#endif
      if (solveDepth > 60)
	exit (2);
    };

  do
    {
      doItAgain = 0;
      if (solveEQ (problemPtr, desiredResult) == FALSE)
	{
	  solveDepth--;
	  return (FALSE);
	};
      if (inApproximateMode && !nGEQ)
	{
	  result = TRUE;
	  nVars = safeVars;
	  problemReduced (problemPtr);
	  break;
	}
      else
	result = solveGEQ (problemPtr, desiredResult);
    }
  while (doItAgain && desiredResult == SIMPLIFY);
  solveDepth--;

  if (reduceWithSubs)
    {
    }
  else
    {
      resurrectSubs (problemPtr);
      gcc_assert (pleaseNoEqualitiesInSimplifiedProblems || !result || nSUB == 0);
    }

  return (result);
}

static int fastLookup[maxKeys * 2];
static int fastLookupRed[maxKeys * 2];

normalizeReturnType normalize (Problem * problemPtr)
{
  int e, i, j, k, nV;
  int coupledSubscripts = 0;

  nV = nVars;
  for (e = 0; e < nGEQ; e++)
    {
      if (!GEQs[e].touched)
	{
	  if (!singleVarGEQ (GEQs[e], nV))
	    coupledSubscripts = 1;
	}
      else
	{
	  register int g;
	  int topVar;
	  int i0;
	  int hashCode;


	  {
	    int *p = &packing[0];
	    for (k = 1; k <= nV; k++)
	      if (GEQs[e].coef[k])
		{
		  *(p++) = k;
		};
	    topVar = (p - &packing[0]) - 1;
	  };

	  if (topVar == -1)
	    {
	      if (GEQs[e].coef[0] < 0)
		{
		  if (DBUG)
		    printGEQ (&GEQs[e]);
		  if (DBUG)
		    fprintf (outputFile, "\nequations have no solution \n");
		  return (normalize_false);
		};
	      doDelete (e, nV);
	      e--;
	      continue;
	    }
	  else if (topVar == 0)
	    {
	      int singleVar = packing[0];
	      g = GEQs[e].coef[singleVar];
	      if (g > 0)
		{
		  GEQs[e].coef[singleVar] = 1;
		  GEQs[e].key = singleVar;
		}
	      else
		{
		  g = -g;
		  GEQs[e].coef[singleVar] = -1;
		  GEQs[e].key = -singleVar;
		};
	      if (g > 1)
		GEQs[e].coef[0] = intDiv (GEQs[e].coef[0], g);
	    }
	  else
	    {

	      coupledSubscripts = 1;
	      i0 = topVar;
	      i = packing[i0--];
	      g = GEQs[e].coef[i];
	      hashCode = g * (i + 3);
	      if (g < 0)
		g = -g;
	      for (; i0 >= 0; i0--)
		{
		  register int x;
		  i = packing[i0];
		  x = GEQs[e].coef[i];
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
		};
	      for (; i0 >= 0; i0--)
		{
		  register int x;
		  i = packing[i0];
		  x = GEQs[e].coef[i];
		  hashCode = hashCode * keyMult * (i + 3) + x;
		};
	      if (g > 1)
		{
		  GEQs[e].coef[0] = intDiv (GEQs[e].coef[0], g);
		  i0 = topVar;
		  i = packing[i0--];
		  GEQs[e].coef[i] = GEQs[e].coef[i] / g;
		  hashCode = GEQs[e].coef[i] * (i + 3);
		  for (; i0 >= 0; i0--)
		    {
		      i = packing[i0];
		      GEQs[e].coef[i] = GEQs[e].coef[i] / g;
		      hashCode =
			hashCode * keyMult * (i + 3) + GEQs[e].coef[i];
		    };
		};

	      {
		register int g2 = abs (hashCode);
		if (DEBUG)
		  {
		    fprintf (outputFile, "Hash code = %d, eqn = ", hashCode);
		    printGEQ (&GEQs[e]);
		    fprintf (outputFile, "\n");
		  };
		j = g2 % hashTableSize;
		while (1)
		  {
		    Eqn proto = &(hashMaster[j]);
		    if (proto->touched == g2)
		      {
			if (proto->coef[0] == topVar)
			  {
			    if (hashCode >= 0)
			      for (i0 = topVar; i0 >= 0; i0--)
				{
				  i = packing[i0];
				  if (GEQs[e].coef[i] != proto->coef[i])
				    break;
				}
			    else
			      for (i0 = topVar; i0 >= 0; i0--)
				{
				  i = packing[i0];
				  if (GEQs[e].coef[i] != -proto->coef[i])
				    break;
				};
			    if (i0 < 0)
			      {
				if (hashCode >= 0)
				  GEQs[e].key = proto->key;
				else
				  GEQs[e].key = -proto->key;
				break;
			      };
			  };
		      }
		    else if (proto->touched < 0)
		      {
			eqnzero (proto);
			if (hashCode >= 0)
			  for (i0 = topVar; i0 >= 0; i0--)
			    {
			      i = packing[i0];
			      proto->coef[i] = GEQs[e].coef[i];
			    }
			else
			  for (i0 = topVar; i0 >= 0; i0--)
			    {
			      i = packing[i0];
			      proto->coef[i] = -GEQs[e].coef[i];
			    }
			proto->coef[0] = topVar;
			proto->touched = g2;
			if (DEBUG)
			  fprintf (outputFile, " constraint key = %d\n",
				   nextKey);
			proto->key = nextKey++;
			if (proto->key > maxKeys)
			  {
			    fprintf (outputFile,
				     "too many hash keys generated \n");
			    fflush (outputFile);
			    exit (2);
			  };
			if (hashCode >= 0)
			  GEQs[e].key = proto->key;
			else
			  GEQs[e].key = -proto->key;
			break;
		      };
		    j = (j + 1) % hashTableSize;
		  };
	      };
	    };

	  GEQs[e].touched = FALSE;
	};

      {
	int eKey = GEQs[e].key;
	int e2;
	if (e > 0)
	  {
	    int cTerm = GEQs[e].coef[0];
	    e2 = fastLookup[maxKeys - eKey];
	    if (e2 < e && GEQs[e2].key == -eKey && !GEQs[e2].color)
	      {
		if (GEQs[e2].coef[0] < -cTerm)
		  {
		    if (DBUG)
		      {
			printGEQ (&GEQs[e]);
			fprintf (outputFile, "\n");
			printGEQ (&GEQs[e2]);
			fprintf (outputFile,
				 "\nequations have no solution \n");
		      };
		    return (normalize_false);
		  };
		if (GEQs[e2].coef[0] == -cTerm
		    && (createColor || !GEQs[e].color))
		  {
		    eqncpy (&EQs[nEQ], &GEQs[e]);
		    if (!GEQs[e].color)
		      addingEqualityConstraint (problemPtr, nEQ);
		    nEQ++;
		    gcc_assert (nEQ <= maxEQs);
		  };
	      };
	    e2 = fastLookupRed[maxKeys - eKey];
	    if (e2 < e && GEQs[e2].key == -eKey && GEQs[e2].color)
	      {
		if (GEQs[e2].coef[0] < -cTerm)
		  {
		    if (DBUG)
		      {
			printGEQ (&GEQs[e]);
			fprintf (outputFile, "\n");
			printGEQ (&GEQs[e2]);
			fprintf (outputFile,
				 "\nequations have no solution \n");
		      };
		    return (normalize_false);
		  };
		if (GEQs[e2].coef[0] == -cTerm && createColor)
		  {
		    eqncpy (&EQs[nEQ], &GEQs[e]);
		    EQs[nEQ].color = 1;
		    nEQ++;
		    gcc_assert (nEQ <= maxEQs);
		  };
	      };

	    e2 = fastLookup[maxKeys + eKey];
	    if (e2 < e && GEQs[e2].key == eKey && !GEQs[e2].color)
	      {
		if (GEQs[e2].coef[0] > cTerm)
		  {
		    if (!GEQs[e].color)
		      {
			if (DEBUG)
			  {
			    fprintf (outputFile,
				     "Removing Redudant Equation: ");
			    printGEQ (&(GEQs[e2]));
			    fprintf (outputFile, "\n");
			    fprintf (outputFile,
				     "[a]      Made Redundant by: ");
			    printGEQ (&(GEQs[e]));
			    fprintf (outputFile, "\n");
			  }
			GEQs[e2].coef[0] = cTerm;
			doDelete (e, nV);
			e--;
			continue;
		      }
		  }
		else
		  {
		    if (DEBUG)
		      {
			fprintf (outputFile, "Removing Redudant Equation: ");
			printGEQ (&(GEQs[e]));
			fprintf (outputFile, "\n");
			fprintf (outputFile, "[b]      Made Redundant by: ");
			printGEQ (&(GEQs[e2]));
			fprintf (outputFile, "\n");
		      }
		    doDelete (e, nV);
		    e--;
		    continue;
		  }
	      }
	    e2 = fastLookupRed[maxKeys + eKey];
	    if (e2 < e && GEQs[e2].key == eKey && GEQs[e2].color)
	      {
		if (GEQs[e2].coef[0] >= cTerm)
		  {
		    if (DEBUG)
		      {
			fprintf (outputFile, "Removing Redudant Equation: ");
			printGEQ (&(GEQs[e2]));
			fprintf (outputFile, "\n");
			fprintf (outputFile, "[c]      Made Redundant by: ");
			printGEQ (&(GEQs[e]));
			fprintf (outputFile, "\n");
		      }
		    GEQs[e2].coef[0] = cTerm;
		    GEQs[e2].color = GEQs[e].color;
		  }
		else if (GEQs[e].color)
		  {
		    if (DEBUG)
		      {
			fprintf (outputFile, "Removing Redudant Equation: ");
			printGEQ (&(GEQs[e]));
			fprintf (outputFile, "\n");
			fprintf (outputFile, "[d]      Made Redundant by: ");
			printGEQ (&(GEQs[e2]));
			fprintf (outputFile, "\n");
		      }
		  }
		doDelete (e, nV);
		e--;
		continue;

	      }
	  };
	if (GEQs[e].color)
	  fastLookupRed[maxKeys + eKey] = e;
	else
	  fastLookup[maxKeys + eKey] = e;
      };
    };
  createColor = 0;
  return coupledSubscripts ? normalize_coupled : normalize_uncoupled;
}


int parallelSplinter (Problem * problemPtr, int e, int diff,
		      int desiredResult);

static int
solveGEQ (register Problem * problemPtr, int desiredResult)
{
  int i, j, k, e;
  int nV, fv;
  int result;
  int coupledSubscripts = 0;
  int eliminateAgain;
  int smoothed = 0;
  int triedEliminatingRedundant = 0;
  j = 0;

  if (desiredResult != SIMPLIFY)
    {
      nSUB = 0;
      safeVars = 0;
    }
solveGEQstart:
  while (1)
    {
      gcc_assert (desiredResult == SIMPLIFY || nSUB == 0);
      if (nGEQ > maxGEQs)
	{
	  fprintf (outputFile, "TOO MANY EQUATIONS GENERATED\n");
	  exit (2);
	};
      if (DEBUG)
	fprintf (outputFile, "\nSolveGEQ(%d,%d):\n",
		 desiredResult, pleaseNoEqualitiesInSimplifiedProblems);
      if (DEBUG)
	printProblem (problemPtr);
      if (DEBUG)
	fprintf (outputFile, "\n");

      nV = nVars;

      if (nV == 1)
	{
	  int uColor = black;
	  int lColor = black;
	  int upperBound = posInfinity;
	  int lowerBound = negInfinity;
	  for (e = nGEQ - 1; e >= 0; e--)
	    {
	      int a = GEQs[e].coef[1];
	      int c = GEQs[e].coef[0];
	      /* our equation is ax + c >= 0, or ax >= -c, or c >= -ax */
	      if (a == 0)
		{
		  if (c < 0)
		    {
		      if (TRACE)
			fprintf (outputFile, "equations have no solution \n");
		      return (FALSE);
		    };
		}
	      else if (a > 0)
		{
		  if (a != 1)
		    c = intDiv (c, a);
		  if (lowerBound < -c ||
		      (lowerBound == -c && !isRed (&GEQs[e])))
		    {
		      lowerBound = -c;
		      lColor = GEQs[e].color;
		    }
		}
	      else
		{
		  if (a != -1)
		    c = intDiv (c, -a);
		  if (upperBound > c ||
		      (upperBound == c && !isRed (&GEQs[e])))
		    {
		      upperBound = c;
		      uColor = GEQs[e].color;
		    }
		};
	    };
	  if (DEBUG)
	    fprintf (outputFile, "upper bound = %d\n", upperBound);
	  if (DEBUG)
	    fprintf (outputFile, "lower bound = %d\n", lowerBound);
	  if (lowerBound > upperBound)
	    {
	      if (TRACE)
		fprintf (outputFile, "equations have no solution \n");
	      return (FALSE);
	    };
	  if (desiredResult == SIMPLIFY)
	    {
	      nGEQ = 0;
	      if (safeVars == 1)
		{

		  if (lowerBound == upperBound && !uColor && !lColor)
		    {
		      EQs[0].coef[0] = -lowerBound;
		      EQs[0].coef[1] = 1;
		      EQs[0].color = 0;
		      nEQ = 1;
		      return (solve (problemPtr, desiredResult));
		    }
		  else
		    {
		      if (lowerBound > negInfinity)
			{
			  GEQs[0].coef[0] = -lowerBound;
			  GEQs[0].coef[1] = 1;
			  GEQs[0].key = 1;
			  GEQs[0].color = lColor;
			  GEQs[0].touched = 0;
			  nGEQ = 1;
			};
		      if (upperBound < posInfinity)
			{
			  GEQs[nGEQ].coef[0] = upperBound;
			  GEQs[nGEQ].coef[1] = -1;
			  GEQs[nGEQ].key = -1;
			  GEQs[nGEQ].color = uColor;
			  GEQs[nGEQ].touched = 0;
			  nGEQ++;
			};
		    };
		}
	      else
		nVars = 0;
	      problemReduced (problemPtr);
	      return (FALSE);
	    };
	  if (originalProblem != noProblem && !lColor && !uColor
	      && !conservative && lowerBound == upperBound)
	    {
	      EQs[0].coef[0] = -lowerBound;
	      EQs[0].coef[1] = 1;
	      nEQ = 1;
	      addingEqualityConstraint (problemPtr, 0);
	    };
	  return (TRUE);
	};

      if (!problemPtr->variablesFreed)
	{
	  problemPtr->variablesFreed = 1;
	  if (desiredResult != SIMPLIFY)
	    freeEliminations (problemPtr, 0);
	  else
	    freeEliminations (problemPtr, safeVars);
	  nV = nVars;
	  if (nV == 1)
	    continue;
	};


      switch (normalize (problemPtr))
	{
	case normalize_false:
	  return (FALSE);
	  break;
	case normalize_coupled:
	  coupledSubscripts = TRUE;
	  break;
	case normalize_uncoupled:
	  coupledSubscripts = FALSE;
	  break;
	}

      nV = nVars;

      if ((doTrace && desiredResult == SIMPLIFY) || DBUG)
	{
	  fprintf (outputFile, "\nafter normalization:\n");
	  printProblem (problemPtr);
	  fprintf (outputFile, "\n");
	  fprintf (outputFile,
		   "eliminating variable using fourier-motzkin elimination\n");
	};

      do
	{
	  eliminateAgain = 0;

	  if (nEQ > 0)
	    return (solve (problemPtr, desiredResult));

	  if (!coupledSubscripts)
	    {
	      if (safeVars == 0)
		nGEQ = 0;
	      else
		for (e = nGEQ - 1; e >= 0; e--)
		  if (GEQs[e].key > safeVars || -safeVars > GEQs[e].key)
		    doDelete (e, nV);
	      nVars = safeVars;
	      if (desiredResult == SIMPLIFY)
		{
		  problemReduced (problemPtr);
		  return (FALSE);
		};
	      return (TRUE);
	    };

	  if (desiredResult != SIMPLIFY)
	    fv = 0;
	  else
	    fv = safeVars;

	  if (nGEQ == 0)
	    {
	      if (desiredResult == SIMPLIFY)
		{
		  nVars = safeVars;
		  problemReduced (problemPtr);
		  return (FALSE);
		};
	      return (TRUE);
	    };
	  if (desiredResult == SIMPLIFY && nV == safeVars)
	    {
	      problemReduced (problemPtr);
	      return (FALSE);
	    };


	  if (nGEQ > maxGEQs - 30 || nGEQ > 2 * nV * nV + 4 * nV + 10)
	    {
	      if (TRACE)
		fprintf (outputFile,
			 "TOO MANY EQUATIONS; %d equations, %d variables, ELIMINATING REDUNDANT ONES\n",
			 nGEQ, nV);
	      if (!eliminateRedundant (problemPtr, 0))
		return 0;
	      nV = nVars;
	      if (nEQ > 0)
		return (solve (problemPtr, desiredResult));
	      if (TRACE)
		fprintf (outputFile,
			 "END ELIMINATION OF REDUNDANT EQUATIONS\n");
	    };



	  {
	    int parallelDifference = (INT_MAX);
	    int bestParallelEqn = -1;
	    int minC, maxC, minCj;
	    int lowerBoundCount;
	    int e2, Le, Ue;
	    int possibleEasyIntSolution;
	    int maxSplinters = 1;
	    int exact = 0;
	    int luckyExact = 0;
	    int newEqns = 0;
	    minCj = 0;
	    Le = 0;
	    lowerBoundCount = 0;

	    if (desiredResult != SIMPLIFY)
	      fv = 0;
	    else
	      fv = safeVars;

	    {
	      int best = (INT_MAX);
	      int j = 0, jLe, jLowerBoundCount, upperBoundCount;

	      jLe = 0;
	      jLowerBoundCount = 0;


	      for (i = nV; i != fv; i--)
		{
		  int score;
		  int ub = -2;
		  int lb = -2;
		  int lucky = 0;
		  minC = maxC = 0;
		  lowerBoundCount = 0;
		  upperBoundCount = 0;
		  for (e = nGEQ - 1; e >= 0; e--)
		    if (GEQs[e].coef[i] < 0)
		      {
			setMin (minC, GEQs[e].coef[i]);
			upperBoundCount++;
			if (GEQs[e].coef[i] < -1)
			  {
			    if (ub == -2)
			      ub = e;
			    else
			      ub = -1;
			  };
		      }
		    else if (GEQs[e].coef[i] > 0)
		      {
			setMax (maxC, GEQs[e].coef[i]);
			lowerBoundCount++;
			Le = e;
			if (GEQs[e].coef[i] > 1)
			  {
			    if (lb == -2)
			      lb = e;
			    else
			      lb = -1;
			  };
		      };
		  if (lowerBoundCount == 0 || upperBoundCount == 0)
		    {
		      lowerBoundCount = 0;
		      break;
		    };
		  if (ub >= 0 && lb >= 0 && GEQs[lb].key == -GEQs[ub].key)
		    {
		      int Lc = GEQs[lb].coef[i];
		      int Uc = -GEQs[ub].coef[i];
		      int diff =
			Lc * GEQs[ub].coef[0] + Uc * GEQs[lb].coef[0];
		      lucky = (diff >= (Uc - 1) * (Lc - 1));
		    };
		  if (maxC == 1 || minC == -1 || lucky || APROX
		      || inApproximateMode)
		    {
		      newEqns = score = upperBoundCount * lowerBoundCount;
		      if (DEBUG)
			fprintf (outputFile,
				 "For %s, exact, score = %d*%d, range = %d ... %d, \nlucky = %d, APROX = %d, inApproximateMode=%d \n",
				 variable (i), upperBoundCount,
				 lowerBoundCount, minC, maxC, lucky, APROX,
				 inApproximateMode);
		      if (!exact || score < best)
			{

			  best = score;
			  j = i;
			  minCj = minC;
			  jLe = Le;
			  jLowerBoundCount = lowerBoundCount;
			  exact = 1;
			  luckyExact = lucky;
			  if (score == 1)
			    break;
			};
		    }
		  else if (!exact)
		    {
		      if (DEBUG)
			fprintf (outputFile,
				 "For %s, non-exact, score = %d*%d, range = %d ... %d \n",
				 variable (i), upperBoundCount,
				 lowerBoundCount, minC, maxC);
		      newEqns = upperBoundCount * lowerBoundCount;
		      score = maxC - minC;
		      if (best > score)
			{
			  best = score;
			  j = i;
			  minCj = minC;
			  jLe = Le;
			  jLowerBoundCount = lowerBoundCount;
			};
		    };
		};
	      if (lowerBoundCount == 0)
		{
		  freeEliminations (problemPtr, safeVars);
		  nV = nVars;
		  eliminateAgain = 1;
		  continue;
		};
	      i = j;
	      minC = minCj;
	      Le = jLe;
	      lowerBoundCount = jLowerBoundCount;
	      for (e = nGEQ - 1; e >= 0; e--)
		if (GEQs[e].coef[i] > 0)
		  {
		    if (GEQs[e].coef[i] == -minC)
		      maxSplinters += -minC - 1;
		    else
		      maxSplinters +=
			checkPosMul ((GEQs[e].coef[i] - 1),
				     (-minC - 1)) / (-minC) + 1;
		  }
	    };

#ifdef Omega3
	    if (!exact && !triedEliminatingRedundant)
	      {
		if (TRACE)
		  fprintf (outputFile,
			   "Trying to produce exact elimination by finding redundant constraints\n");
		eliminateRedundant (problemPtr, 0);
		if (TRACE)
		  fprintf (outputFile,
			   "Done trying to produce exact elimination by finding redundant constraints\n");
		triedEliminatingRedundant = 1;
		eliminateAgain = 1;
		continue;
	      }
	    triedEliminatingRedundant = 0;
#endif

	    if (returnSingleResult && desiredResult == SIMPLIFY && !exact)
	      {
		nonConvex = 1;
		problemReduced (problemPtr);
		return (TRUE);
	      };

#ifndef Omega3
	    if (!exact && !triedEliminatingRedundant)
	      {
		if (TRACE)
		  fprintf (outputFile,
			   "Trying to produce exact elimination by finding redundant constraints\n");
		eliminateRedundant (problemPtr, 0);
		if (TRACE)
		  fprintf (outputFile,
			   "Done trying to produce exact elimination by finding redundant constraints\n");
		triedEliminatingRedundant = 1;
		continue;
	      }
	    triedEliminatingRedundant = 0;
#endif

	    if (!exact)
	      {
		int e1, e2;

		for (e1 = nGEQ - 1; e1 >= 0; e1--)
		  if (!GEQs[e1].color)
		    for (e2 = e1 - 1; e2 >= 0; e2--)
		      if (!GEQs[e2].color)
			{
			  if (GEQs[e1].key == -GEQs[e2].key &&
			      (GEQs[e1].coef[0] + GEQs[e2].coef[0]) * (3 -
								       singleVarGEQ
								       (GEQs
									[e1],
									nVars))
			      / 2 < parallelDifference)
			    {
			      parallelDifference =
				(GEQs[e1].coef[0] + GEQs[e2].coef[0]) * (3 -
									 singleVarGEQ
									 (GEQs
									  [e1],
									  nVars))
				/ 2;
			      bestParallelEqn = e1;
			    };
			}
		if (DBUG && bestParallelEqn >= 0)
		  {
		    fprintf (outputFile,
			     "Possible parallel projection, diff = %d, in ",
			     parallelDifference);
		    printGEQ (&(GEQs[bestParallelEqn]));
		    fprintf (outputFile, "\n");
		    printProblem (problemPtr);
		  }
	      }
	    if ((doTrace && desiredResult == SIMPLIFY) || DBUG)
	      {
		fprintf (outputFile, "going to eliminate %s, (%d,%d,%d)\n",
			 variable (i), i, minC, lowerBoundCount);
		if (DEBUG)
		  printProblem (problemPtr);
		if (luckyExact)
		  fprintf (outputFile, "(a lucky exact elimination)\n");
		else if (exact)
		  fprintf (outputFile, "(an exact elimination)\n");
		fprintf (outputFile, "Max # of splinters = %d\n",
			 maxSplinters);
	      };
	    gcc_assert (maxSplinters >= 1);


	    if (!exact && desiredResult == SIMPLIFY && bestParallelEqn >= 0
		&& parallelDifference <= maxSplinters)
	      {
		return parallelSplinter (problemPtr, bestParallelEqn,
					 parallelDifference, desiredResult);
	      }
	    smoothed = 0;

	    if (i != nV)
	      {
		int t;
		j = nVars;
		if (DEBUG)
		  {
		    fprintf (outputFile, "Swapping %d and %d\n", i, j);
		    printProblem (problemPtr);
		  };
		swap (&problemPtr->_var[i], &problemPtr->_var[j]);
		for (e = nGEQ - 1; e >= 0; e--)
		  if (GEQs[e].coef[i] != GEQs[e].coef[j])
		    {
		      GEQs[e].touched = TRUE;
		      t = GEQs[e].coef[i];
		      GEQs[e].coef[i] = GEQs[e].coef[j];
		      GEQs[e].coef[j] = t;
		    };
		for (e = nSUB - 1; e >= 0; e--)
		  if (SUBs[e].coef[i] != SUBs[e].coef[j])
		    {
		      t = SUBs[e].coef[i];
		      SUBs[e].coef[i] = SUBs[e].coef[j];
		      SUBs[e].coef[j] = t;
		    };
		if (DEBUG)
		  {
		    fprintf (outputFile, "Swapping complete \n");
		    printProblem (problemPtr);
		    fprintf (outputFile, "\n");
		  };

		i = j;
	      }
	    else if (DEBUG)
	      {
		fprintf (outputFile, "No swap needed\n");
		printProblem (problemPtr);
	      };
	    nVars--;
	    nV = nVars;

	    if (exact)
	      {
#define maxDead maxGEQs
		if (nV == 1)
		  {
		    int upperBound = posInfinity;
		    int lowerBound = negInfinity;
		    int ub_color = 0;
		    int lb_color = 0;
		    int constantTerm, coefficient;
		    int topEqn = nGEQ - 1;
		    int Lc = GEQs[Le].coef[i];
		    for (Le = topEqn; Le >= 0; Le--)
		      if ((Lc = GEQs[Le].coef[i]) == 0)
			{
			  if (GEQs[Le].coef[1] == 1)
			    {
			      constantTerm = -GEQs[Le].coef[0];
			      if (constantTerm > lowerBound ||
				  (constantTerm == lowerBound &&
				   !isRed (&GEQs[Le])))
				{
				  lowerBound = constantTerm;
				  lb_color = GEQs[Le].color;
				}
			      if (DEBUG)
				{
				  if (GEQs[Le].color == black)
				    fprintf (outputFile, " :::=> %s >= %d\n",
					     variable (1), constantTerm);
				  else
				    fprintf (outputFile,
					     " :::=> [%s >= %d]\n",
					     variable (1), constantTerm);
				}
			    }
			  else
			    {
			      constantTerm = GEQs[Le].coef[0];
			      if (constantTerm < upperBound ||
				  (constantTerm == upperBound
				   && !isRed (&GEQs[Le])))
				{
				  upperBound = constantTerm;
				  ub_color = GEQs[Le].color;
				}
			      if (DEBUG)
				{
				  if (GEQs[Le].color == black)
				    fprintf (outputFile, " :::=> %s <= %d\n",
					     variable (1), constantTerm);
				  else
				    fprintf (outputFile,
					     " :::=> [%s <= %d]\n",
					     variable (1), constantTerm);
				}
			    };
			}
		      else if (Lc > 0)
			{
			  for (Ue = topEqn; Ue >= 0; Ue--)
			    if (GEQs[Ue].coef[i] < 0)
			      {
				if (GEQs[Le].key != -GEQs[Ue].key)
				  {
				    int Uc = -GEQs[Ue].coef[i];
				    coefficient =
				      GEQs[Ue].coef[1] * Lc +
				      GEQs[Le].coef[1] * Uc;
				    constantTerm =
				      GEQs[Ue].coef[0] * Lc +
				      GEQs[Le].coef[0] * Uc;
				    if (DEBUG)
				      {
					printGEQextra (&(GEQs[Ue]));
					fprintf (outputFile, "\n");
					printGEQextra (&(GEQs[Le]));
					fprintf (outputFile, "\n");
				      };
				    if (coefficient > 0)
				      {
					constantTerm =
					  -(intDiv
					    (constantTerm, coefficient));
					/* gcc_assert(black == 0) */
					if (constantTerm > lowerBound ||
					    (constantTerm == lowerBound &&
					     (desiredResult != SIMPLIFY ||
					      (GEQs[Ue].color == black
					       && GEQs[Le].color == black))))
					  {
					    lowerBound = constantTerm;
					    lb_color = GEQs[Ue].color
					      || GEQs[Le].color;
					  }
					if (DEBUG)
					  {
					    if (GEQs[Ue].color
						|| GEQs[Le].color)
					      fprintf (outputFile,
						       " ::=> [%s >= %d]\n",
						       variable (1),
						       constantTerm);
					    else
					      fprintf (outputFile,
						       " ::=> %s >= %d\n",
						       variable (1),
						       constantTerm);
					  }
				      }
				    else
				      {
					constantTerm =
					  (intDiv
					   (constantTerm, -coefficient));
					if (constantTerm < upperBound
					    || (constantTerm == upperBound
						&& GEQs[Ue].color == black
						&& GEQs[Le].color == black))
					  {
					    upperBound = constantTerm;
					    ub_color = GEQs[Ue].color
					      || GEQs[Le].color;
					  }
					if (DEBUG)
					  {
					    if (GEQs[Ue].color
						|| GEQs[Le].color)
					      fprintf (outputFile,
						       " ::=> [%s <= %d]\n",
						       variable (1),
						       constantTerm);
					    else
					      fprintf (outputFile,
						       " ::=> %s <= %d\n",
						       variable (1),
						       constantTerm);
					  }
				      }
				  };
			      };
			};
		    nGEQ = 0;
		    if (DEBUG)
		      fprintf (outputFile,
			       " therefore, %c%d <= %c%s%c <= %d%c\n",
			       lb_color ? '[' : ' ', lowerBound,
			       (lb_color && !ub_color) ? ']' : ' ',
			       variable (1),
			       (!lb_color && ub_color) ? '[' : ' ',
			       upperBound, ub_color ? ']' : ' ');
		    if (lowerBound > upperBound)
		      return (FALSE);
		    if (safeVars == 1)
		      {
			if (upperBound == lowerBound
			    && !(ub_color | lb_color)
			    && !pleaseNoEqualitiesInSimplifiedProblems)
			  {
			    nEQ++;
			    EQs[0].coef[1] = -1;
			    EQs[0].coef[0] = upperBound;
			    EQs[0].color = ub_color | lb_color;
			    if (desiredResult == SIMPLIFY && !EQs[0].color)
			      return (solve (problemPtr, desiredResult));
			  };
			if (upperBound != posInfinity)
			  {
			    GEQs[0].coef[1] = -1;
			    GEQs[0].coef[0] = upperBound;
			    GEQs[0].color = ub_color;
			    GEQs[0].key = -1;
			    GEQs[0].touched = 0;
			    nGEQ++;
			  };
			if (lowerBound != negInfinity)
			  {
			    GEQs[nGEQ].coef[1] = 1;
			    GEQs[nGEQ].coef[0] = -lowerBound;
			    GEQs[nGEQ].color = lb_color;
			    GEQs[nGEQ].key = 1;
			    GEQs[nGEQ].touched = 0;
			    nGEQ++;
			  };
		      };
		    if (desiredResult == SIMPLIFY)
		      {
			problemReduced (problemPtr);
			return (FALSE);
		      }
		    else
		      {
			if (!conservative &&
			    (desiredResult != SIMPLIFY ||
			     (!lb_color && !ub_color))
			    && originalProblem != noProblem
			    && lowerBound == upperBound)
			  {
			    for (i = originalProblem->_nVars; i >= 0; i--)
			      if (originalProblem->_var[i] ==
				  problemPtr->_var[1])
				break;
			    if (i == 0)
			      break;
			    e = originalProblem->_numEQs++;
			    eqnnzero (&originalProblem->_EQs[e],
				      originalProblem->_nVars);
			    originalProblem->_EQs[e].coef[i] = -1;
			    originalProblem->_EQs[e].coef[0] = upperBound;
			    if (DEBUG)
			      {
				fprintf (outputFile,
					 "adding equality constraint %d to outer problem\n",
					 e);
				printProblem (originalProblem);
			      };
			  };
			return (TRUE);
		      };
		  };
		eliminateAgain = 1;

		if (lowerBoundCount == 1)
		  {
		    struct _eqn lbEqn;
		    int Lc = GEQs[Le].coef[i];
		    if (DBUG)
		      fprintf (outputFile, "an inplace elimination\n");

		    eqnncpy (&lbEqn, &GEQs[Le], (nV + 1));
		    doDeleteExtra (Le, nV + 1);
		    for (Ue = nGEQ - 1; Ue >= 0; Ue--)
		      if (GEQs[Ue].coef[i] < 0)
			{
			  if (lbEqn.key == -GEQs[Ue].key)
			    {
			      doDeleteExtra (Ue, nV + 1);
			    }
			  else
			    {
			      int Uc = -GEQs[Ue].coef[i];
			      GEQs[Ue].touched = TRUE;
			      GEQs[Ue].color |= lbEqn.color;
			      eliminateAgain = 0;
			      for (k = 0; k <= nV; k++)
				GEQs[Ue].coef[k] =
				  checkMul (GEQs[Ue].coef[k],
					    Lc) + checkMul (lbEqn.coef[k],
							    Uc);
			      if (DEBUG)
				{
				  printGEQ (&(GEQs[Ue]));
				  fprintf (outputFile, "\n");
				}
			    }
			}
		    continue;
		  }
		else
		  {
		    int deadEqns[maxDead];
		    int numDead = 0;
		    int topEqn = nGEQ - 1;
		    lowerBoundCount--;
		    if (DEBUG)
		      fprintf (outputFile, "lower bound count = %d\n",
			       lowerBoundCount);
		    for (Le = topEqn; Le >= 0; Le--)
		      if (GEQs[Le].coef[i] > 0)
			{
			  int Lc = GEQs[Le].coef[i];
			  for (Ue = topEqn; Ue >= 0; Ue--)
			    if (GEQs[Ue].coef[i] < 0)
			      {
				if (GEQs[Le].key != -GEQs[Ue].key)
				  {
				    int Uc = -GEQs[Ue].coef[i];
				    if (numDead == 0)
				      e2 = nGEQ++;
				    else
				      e2 = deadEqns[--numDead];
				    gcc_assert (e2 < maxGEQs);
				    if (DEBUG)
				      fprintf (outputFile,
					       "Le = %d, Ue = %d, gen = %d\n",
					       Le, Ue, e2);
				    if (DEBUG)
				      {
					printGEQextra (&(GEQs[Le]));
					fprintf (outputFile, "\n");
					printGEQextra (&(GEQs[Ue]));
					fprintf (outputFile, "\n");
				      };
				    eliminateAgain = 0;

				    for (k = nV; k >= 0; k--)
				      GEQs[e2].coef[k] =
					checkMul (GEQs[Ue].coef[k],
						  Lc) +
					checkMul (GEQs[Le].coef[k], Uc);
				    GEQs[e2].coef[nV + 1] = 0;
				    GEQs[e2].touched = TRUE;
				    GEQs[e2].color =
				      GEQs[Ue].color | GEQs[Le].color;

				    if (DEBUG)
				      {
					printGEQ (&(GEQs[e2]));
					fprintf (outputFile, "\n");
				      };
				  };
				if (lowerBoundCount == 0)
				  {
				    deadEqns[numDead++] = Ue;
				    if (DEBUG)
				      fprintf (outputFile, "Killed %d\n", Ue);
				  };
			      };
			  lowerBoundCount--;
			  deadEqns[numDead++] = Le;
			  if (DEBUG)
			    fprintf (outputFile, "Killed %d\n", Le);
			};
		    {
		      int isDead[maxGEQs];
		      for (e = nGEQ - 1; e >= 0; e--)
			isDead[e] = FALSE;
		      while (numDead > 0)
			{
			  e = deadEqns[--numDead];
			  isDead[e] = TRUE;
			};
		      for (e = nGEQ - 1; e >= 0; e--)
			if (isDead[e])
			  doDeleteExtra (e, nV + 1);
		    };
		    continue;
		  };
	      }
	    else
	      {
		Problem *rS, *iS;

		rS = mallocProblem;
		initializeProblem (rS);
		iS = mallocProblem;
		initializeProblem (iS);

		e2 = 0;
		possibleEasyIntSolution = TRUE;
		for (e = 0; e < nGEQ; e++)
		  if (GEQs[e].coef[i] == 0)
		    {
		      eqncpy (&(rS->_GEQs[e2]), &GEQs[e]);
		      eqncpy (&(iS->_GEQs[e2]), &GEQs[e]);
		      if (DEBUG)
			{
			  int t;
			  fprintf (outputFile, "Copying (%d, %d): ", i,
				   GEQs[e].coef[i]);
			  printGEQextra (&GEQs[e]);
			  fprintf (outputFile, "\n");
			  for (t = 0; t <= nV + 1; t++)
			    fprintf (outputFile, "%d ", GEQs[e].coef[t]);
			  fprintf (outputFile, "\n");

			};
		      e2++;
		      gcc_assert (e2 < maxGEQs);
		    };
		for (Le = nGEQ - 1; Le >= 0; Le--)
		  if (GEQs[Le].coef[i] > 0)
		    {
		      int Lc = GEQs[Le].coef[i];
		      for (Ue = nGEQ - 1; Ue >= 0; Ue--)
			if (GEQs[Ue].coef[i] < 0)
			  {
			    if (GEQs[Le].key != -GEQs[Ue].key)
			      {
				int Uc = -GEQs[Ue].coef[i];
				rS->_GEQs[e2].touched =
				  iS->_GEQs[e2].touched = TRUE;
				if (DEBUG)
				  {
				    fprintf (outputFile, "---\n");
				    fprintf (outputFile,
					     "Le(Lc) = %d(%d_, Ue(Uc) = %d(%d), gen = %d\n",
					     Le, Lc, Ue, Uc, e2);
				    printGEQextra (&GEQs[Le]);
				    fprintf (outputFile, "\n");
				    printGEQextra (&GEQs[Ue]);
				    fprintf (outputFile, "\n");
				  };

				if (Uc == Lc)
				  {
				    for (k = nV; k >= 0; k--)
				      iS->_GEQs[e2].coef[k] =
					rS->_GEQs[e2].coef[k] =
					GEQs[Ue].coef[k] + GEQs[Le].coef[k];
				    iS->_GEQs[e2].coef[0] -= (Uc - 1);
				  }
				else
				  {
				    for (k = nV; k >= 0; k--)
				      iS->_GEQs[e2].coef[k] =
					rS->_GEQs[e2].coef[k] =
					checkMul (GEQs[Ue].coef[k],
						  Lc) +
					checkMul (GEQs[Le].coef[k], Uc);
				    iS->_GEQs[e2].coef[0] -=
				      (Uc - 1) * (Lc - 1);
				  }

				iS->_GEQs[e2].color = rS->_GEQs[e2].color
				  = GEQs[Ue].color || GEQs[Le].color;

				if (DEBUG)
				  {
				    printGEQ (&(rS->_GEQs[e2]));
				    fprintf (outputFile, "\n");
				  };
				e2++;
				gcc_assert (e2 < maxGEQs);
			      }
			    else
			      {
				int Uc = -GEQs[Ue].coef[i];
				if (GEQs[Ue].coef[0] * Lc +
				    GEQs[Le].coef[0] * Uc - (Uc - 1) * (Lc -
									1) <
				    0)
				  possibleEasyIntSolution = FALSE;
			      }
			  }

		    }
		iS->variablesInitialized = rS->variablesInitialized = 1;
		iS->_nVars = rS->_nVars = nVars;
		iS->_numGEQs = rS->_numGEQs = e2;
		iS->_numEQs = rS->_numEQs = 0;
		iS->_numSUBs = rS->_numSUBs = nSUB;
		iS->_safeVars = rS->_safeVars = safeVars;
		{
		  int t;
		  for (t = nV; t >= 0; t--)
		    rS->_var[t] = problemPtr->_var[t];
		  for (t = nV; t >= 0; t--)
		    iS->_var[t] = problemPtr->_var[t];
		  for (e = nSUB - 1; e >= 0; e--)
		    {
		      eqnncpy (&(rS->_SUBs[e]), &(SUBs[e]), nVars);
		      eqnncpy (&(iS->_SUBs[e]), &(SUBs[e]), nVars);
		    };
		};
		nVars++;
		nV = nVars;
		if (desiredResult != TRUE)
		  {
		    int t = trace;
		    if (TRACE)
		      fprintf (outputFile, "\nreal solution(%d):\n", depth);
		    depth++;
		    trace = 0;
		    if (originalProblem == noProblem)
		      {
			originalProblem = problemPtr;
			result = solveGEQ (rS, FALSE);
			originalProblem = noProblem;
		      }
		    else
		      result = solveGEQ (rS, FALSE);
		    trace = t;
		    depth--;
		    if (result == FALSE)
		      {
			free (rS);
			free (iS);
			return (result);
		      };

		    if (nEQ > 0)
		      {
			/* An equality constraint must have been found */
			free (rS);
			free (iS);
			return (solve (problemPtr, desiredResult));
		      };
		  };
		if (desiredResult != FALSE)
		  {
		    if (possibleEasyIntSolution)
		      {
			if (TRACE)
			  fprintf (outputFile, "\ninteger solution(%d):\n",
				   depth);
			depth++;
			conservative++;
			result = solveGEQ (iS, desiredResult);
			conservative--;
			depth--;
			if (result != FALSE)
			  {
			    free (rS);
			    free (iS);
			    return (result);
			  };
		      };
		    if (!exact && bestParallelEqn >= 0
			&& parallelDifference <= maxSplinters)
		      {
			free (rS);
			free (iS);
			return parallelSplinter (problemPtr, bestParallelEqn,
						 parallelDifference,
						 desiredResult);
		      }
		    if (TRACE)
		      fprintf (outputFile, "have to do exact analysis\n");
		    {
		      int worstLowerBoundConstant = -minC;
		      int lowerBounds = 0;
		      int lowerBound[maxGEQs];
		      int smallest;
		      int t;
		      conservative++;
		      for (e = 0; e < nGEQ; e++)
			if (GEQs[e].coef[i] > 1)
			  lowerBound[lowerBounds++] = e;
		      /* sort array */
		      for (j = 0; j < lowerBounds; j++)
			{
			  smallest = j;
			  for (k = j + 1; k < lowerBounds; k++)
			    if (GEQs[lowerBound[smallest]].coef[i] >
				GEQs[lowerBound[k]].coef[i])
			      smallest = k;
			  t = lowerBound[smallest];
			  lowerBound[smallest] = lowerBound[j];
			  lowerBound[j] = t;
			};
		      if (DEBUG)
			{
			  fprintf (outputFile, "lower bound coeeficients = ");
			  for (j = 0; j < lowerBounds; j++)
			    fprintf (outputFile, " %d",
				     GEQs[lowerBound[j]].coef[i]);
			  fprintf (outputFile, "\n");
			};


		      for (j = 0; j < lowerBounds; j++)
			{
			  int maxIncr;
			  int c;
			  e = lowerBound[j];
			  maxIncr =
			    ((GEQs[e].coef[i] - 1) *
			     (worstLowerBoundConstant - 1) -
			     1) / worstLowerBoundConstant;

			  /* maxIncr += 2; */
			  if ((doTrace && desiredResult == SIMPLIFY) || DBUG)
			    {
			      fprintf (outputFile, "for equation ");
			      printGEQ (&GEQs[e]);
			      fprintf (outputFile,
				       "\ntry decrements from 0 to %d\n",
				       maxIncr);
			      printProblem (problemPtr);
			    };
			  if (maxIncr > 50)
			    {
			      if (!smoothed
				  && smoothWeirdEquations (problemPtr))
				{
				  conservative--;
				  free (rS);
				  free (iS);
				  smoothed = 1;
				  goto solveGEQstart;
				};
			    };
			  eqncpy (&EQs[0], &GEQs[e]);
			  /*
			   * if (GEQs[e].color) fprintf(outputFile,"warning: adding black equality constraint
			   * based on red inequality\n");
			   */
			  EQs[0].color = black;
			  eqnzero (&GEQs[e]);
			  GEQs[e].touched = TRUE;
			  nEQ = 1;
			  for (c = maxIncr; c >= 0; c--)
			    {
			      if (DBUG)
				fprintf (outputFile,
					 "trying next decrement of %d\n",
					 maxIncr - c);
			      if (DBUG)
				printProblem (problemPtr);
			      problemcpy (rS, problemPtr);
			      if (DEBUG)
				printProblem (rS);
			      result = solve (rS, desiredResult);
			      if (result == TRUE)
				{
				  free (rS);
				  free (iS);
				  conservative--;
				  return (TRUE);
				};
			      EQs[0].coef[0]--;
			    };
			  if (j + 1 < lowerBounds)
			    {
			      nEQ = 0;
			      eqncpy (&GEQs[e], &EQs[0]);
			      GEQs[e].touched = 1;
			      GEQs[e].color = black;
			      problemcpy (rS, problemPtr);
			      if (DEBUG)
				fprintf (outputFile,
					 "exhausted lower bound, checking if still feasible ");
			      result = solve (rS, FALSE);
			      if (result == FALSE)
				break;
			    };
			};
		      if ((doTrace && desiredResult == SIMPLIFY) || DBUG)
			fprintf (outputFile, "fall-off the end\n");
		      free (rS);
		      free (iS);

		      conservative--;
		      return (FALSE);
		    };
		  };
		free (rS);
		free (iS);
	      };
	    return (UNKNOWN);
	  };
	}
      while (eliminateAgain);
    };
}

int
parallelSplinter (Problem * problemPtr, int e, int diff, int desiredResult)
{
  Problem *tmpProblem;
  int i;
  if (DBUG)
    {
      fprintf (outputFile, "Using parallel splintering\n");
      printProblem (problemPtr);
    }
  tmpProblem = mallocProblem;
  eqncpy (&EQs[0], &GEQs[e]);
  nEQ = 1;
  for (i = 0; i <= diff; i++)
    {
      problemcpy (tmpProblem, problemPtr);
      if (DBUG)
	{
	  fprintf (outputFile, "Splinter # %i\n", i);
	  printProblem (problemPtr);
	}
      if (solve (tmpProblem, desiredResult))
	{
	  free (tmpProblem);
	  return TRUE;
	}
      EQs[0].coef[0]--;
    }
  free (tmpProblem);
  return FALSE;
}






/*
 * Return 1 if red equations constrain the set of possible solutions. We assume that there are solutions to the black
 * equations by themselves, so if there is no solution to the combined problem, we return 1.
 */

int
hasRedEquations (Problem * problemPtr)
{
  int result;
  int e;
  int i;

  if (TRACE)
    {
      fprintf (outputFile, "Checking for red equations:\n");
      printProblem (problemPtr);
    };

  pleaseNoEqualitiesInSimplifiedProblems++;
  mayBeRed++;
#ifndef singleResult
  returnSingleResult++;
#endif
  createColor = 1;
  result = !simplifyProblem (problemPtr);
#ifndef singleResult
  returnSingleResult--;
#endif
  mayBeRed--;
  pleaseNoEqualitiesInSimplifiedProblems--;
  if (result)
    {
      if (TRACE)
	fprintf (outputFile, "Gist is FALSE\n");
      nSUB = 0;
      nGEQ = 0;
      nEQ = 1;
      EQs[0].color = red;
      for (i = nVars; i > 0; i--)
	EQs[0].coef[i] = 0;
      EQs[0].coef[0] = 1;
      return 1;
    }
  freeRedEliminations (problemPtr);


#ifndef NDEBUG
  if (nEQ != 0)
    {
      fprintf (outputFile, "in hasRedEquations, didn't want any EQ's:\n");
      printProblem (problemPtr);
      gcc_assert (nEQ == 0);
    }
#endif

  for (e = nGEQ - 1; e >= 0; e--)
    if (GEQs[e].color == red)
      result = 1;
  if (!result)
    return 0;

  for (i = safeVars; i >= 1; i--)
    {
      int ub = 0;
      int lb = 0;
      for (e = nGEQ - 1; e >= 0; e--)
	{
	  if (GEQs[e].coef[i])
	    {
	      if (GEQs[e].coef[i] > 0)
		lb |= (1 + GEQs[e].color);
	      else		/* (GEQs[e].coef[i] < 0) */
		ub |= (1 + GEQs[e].color);
	    }
	}
      if (ub == 2 || lb == 2)
	{
	  if (DBUG)
	    fprintf (outputFile, "checks for upper/lower bounds worked!\n");
	  if (!reduceWithSubs)
	    {
	      resurrectSubs (problemPtr);
	      gcc_assert (nSUB == 0);
	    }
	  return 1;
	};
    };


  if (TRACE)
    fprintf (outputFile,
	     "*** Doing potentially expensive elimination tests for red equations\n");
  pleaseNoEqualitiesInSimplifiedProblems++;
  eliminateRed (problemPtr, 1);
  pleaseNoEqualitiesInSimplifiedProblems--;
  result = 0;
  gcc_assert (nEQ == 0);
  for (e = nGEQ - 1; e >= 0; e--)
    if (GEQs[e].color == red)
      result = 1;
  if (TRACE)
    {
      if (!result)
	fprintf (outputFile,
		 "******************** Redudant Red Equations eliminated!!\n");
      else
	fprintf (outputFile, "******************** Red Equations remain\n");
    }
  if (DEBUG)
    printProblem (problemPtr);

  if (reduceWithSubs)
    {
    }
  else
    {
      resurrectSubs (problemPtr);
      {
	normalizeReturnType r = normalize (problemPtr);
	gcc_assert (r != normalize_false);
      };
      coalesce (problemPtr);
      cleanoutWildcards (problemPtr);
      gcc_assert (nSUB == 0);
    }
  return result;
}

int
simplifyApproximate (Problem * problemPtr)
{
  int result;

  inApproximateMode = 1;
  if (TRACE)
    fprintf (outputFile, "Entering Approximate Mode\n");

  result = simplifyProblem (problemPtr);

  if (nVars != safeVars)
    {
      fprintf (outputFile, "gcc_assertion blown!\n");
      printProblem (problemPtr);
      fflush (outputFile);
      gcc_assert (nVars == safeVars);
    };

  if (TRACE)
    fprintf (outputFile, "Leaving Approximate Mode\n");
  inApproximateMode = 0;

  if (reduceWithSubs)
    {
    }
  else
    {
      gcc_assert (nSUB == 0);
    }
  return (result);
}

int
simplifyProblem (Problem * problemPtr)
{
  int i;
  foundReduction = FALSE;

  if (!problemPtr->variablesInitialized)
    {
      initializeVariables (problemPtr);
    };


#ifdef clearForwardingPointers
  for (i = 1; i <= nVars; i++)
    if (problemPtr->_var[i] > 0)
      problemPtr->forwardingAddress[problemPtr->_var[i]] = 0;
#endif

  if (nextKey * 3 > maxKeys)
    {
      int e;
      hashVersion++;
      nextKey = maxVars + 1;
      for (e = nGEQ - 1; e >= 0; e--)
	GEQs[e].touched = TRUE;
      for (i = 0; i < hashTableSize; i++)
	hashMaster[i].touched = -1;
      problemPtr->hashVersion = hashVersion;
    }
  else if (problemPtr->hashVersion != hashVersion)
    {
      int e;
      for (e = nGEQ - 1; e >= 0; e--)
	GEQs[e].touched = TRUE;
      problemPtr->hashVersion = hashVersion;
    };

  nonConvex = 0;
  if (nVars > nEQ + 3 * safeVars)
    freeEliminations (problemPtr, safeVars);

  if (!mayBeRed && problemPtr->_numSUBs == 0 && problemPtr->_safeVars == 0)
    {
      foundReduction = solve (problemPtr, UNKNOWN);
      if (foundReduction && !returnSingleResult)
	{
	  problemPtr->_numGEQs = 0;
	  problemPtr->_numEQs = 0;
	  (*whenReduced) (problemPtr);
	}
      return foundReduction;
    }
  solve (problemPtr, SIMPLIFY);
  if (foundReduction)
    {
      for (i = 1; i <= safeVars; i++)
	problemPtr->forwardingAddress[problemPtr->_var[i]] = i;
      for (i = 0; i < nSUB; i++)
	problemPtr->forwardingAddress[SUBs[i].key] = -i - 1;
    };
  if (reduceWithSubs)
    {
    }
  else
    {
      gcc_assert (pleaseNoEqualitiesInSimplifiedProblems
	      || !foundReduction || nSUB == 0);
    }
  return (foundReduction);
}


void
unprotectVariable (Problem * problemPtr, int v)
{
  int e, t, j, i;
  i = problemPtr->forwardingAddress[v];
  if (i < 0)
    {
      i = -1 - i;
      nSUB--;
      if (i < nSUB)
	{
	  eqncpy (&SUBs[i], &SUBs[nSUB]);
	  problemPtr->forwardingAddress[SUBs[i].key] = -i - 1;
	};
    }
  else
    {
      int bringToLife[maxVars];
      int comingBack = 0;
      int e2;
      for (e = nSUB - 1; e >= 0; e--)
	if ((bringToLife[e] = (SUBs[e].coef[i] != 0)))
	  comingBack++;

      for (e2 = nSUB - 1; e2 >= 0; e2--)
	if (bringToLife[e2])
	  {

	    nVars++;
	    safeVars++;
	    if (safeVars < nVars)
	      {
		for (e = nGEQ - 1; e >= 0; e--)
		  {
		    GEQs[e].coef[nVars] = GEQs[e].coef[safeVars];
		    GEQs[e].coef[safeVars] = 0;
		  };
		for (e = nEQ - 1; e >= 0; e--)
		  {
		    EQs[e].coef[nVars] = EQs[e].coef[safeVars];
		    EQs[e].coef[safeVars] = 0;
		  };
		for (e = nSUB - 1; e >= 0; e--)
		  {
		    SUBs[e].coef[nVars] = SUBs[e].coef[safeVars];
		    SUBs[e].coef[safeVars] = 0;
		  };
		problemPtr->_var[nVars] = problemPtr->_var[safeVars];
		problemPtr->forwardingAddress[problemPtr->_var[nVars]] =
		  nVars;
	      }
	    else
	      {
		for (e = nGEQ - 1; e >= 0; e--)
		  {
		    GEQs[e].coef[safeVars] = 0;
		  };
		for (e = nEQ - 1; e >= 0; e--)
		  {
		    EQs[e].coef[safeVars] = 0;
		  };
		for (e = nSUB - 1; e >= 0; e--)
		  {
		    SUBs[e].coef[safeVars] = 0;
		  };
	      };

	    problemPtr->_var[safeVars] = SUBs[e2].key;
	    problemPtr->forwardingAddress[SUBs[e2].key] = safeVars;

	    eqncpy (&(EQs[nEQ]), &(SUBs[e2]));
	    EQs[nEQ++].coef[problemPtr->_safeVars] = -1;
	    gcc_assert (nEQ <= maxEQs);
	    if (e2 < nSUB - 1)
	      eqncpy (&(SUBs[e2]), &(SUBs[nSUB - 1]));
	    nSUB--;
	  };




      if (i < safeVars)
	{
	  j = safeVars;
	  for (e = nSUB - 1; e >= 0; e--)
	    {
	      t = SUBs[e].coef[j];
	      SUBs[e].coef[j] = SUBs[e].coef[i];
	      SUBs[e].coef[i] = t;
	    };
	  for (e = nGEQ - 1; e >= 0; e--)
	    if (GEQs[e].coef[j] != GEQs[e].coef[i])
	      {
		GEQs[e].touched = TRUE;
		t = GEQs[e].coef[j];
		GEQs[e].coef[j] = GEQs[e].coef[i];
		GEQs[e].coef[i] = t;
	      };
	  for (e = nEQ - 1; e >= 0; e--)
	    {
	      t = EQs[e].coef[j];
	      EQs[e].coef[j] = EQs[e].coef[i];
	      EQs[e].coef[i] = t;
	    };
	  t = problemPtr->_var[j];
	  problemPtr->_var[j] = problemPtr->_var[i];
	  problemPtr->_var[i] = t;
	  problemPtr->forwardingAddress[problemPtr->_var[i]] = i;
	  problemPtr->forwardingAddress[problemPtr->_var[j]] = j;
	};
      safeVars--;
    };
  chainUnprotect (problemPtr);

}


int
constrainVariableSign (Problem * problemPtr, int color, int i, int sign)
{
  int nV = nVars;
  int e, k, j;

  k = problemPtr->forwardingAddress[i];
  if (k < 0)
    {
      k = -1 - k;

      if (sign != 0)
	{
	  e = nGEQ++;
	  eqncpy (&GEQs[e], &SUBs[k]);
	  for (j = 0; j <= nV; j++)
	    GEQs[e].coef[j] *= sign;
	  GEQs[e].coef[0]--;
	  GEQs[e].touched = 1;
	  GEQs[e].color = color;
	}
      else
	{
	  e = nEQ++;
	  gcc_assert (nEQ <= maxEQs);
	  eqncpy (&EQs[e], &SUBs[k]);
	  EQs[e].color = color;
	};

    }
  else if (sign != 0)
    {
      e = nGEQ++;
      eqnzero (&GEQs[e]);
      GEQs[e].coef[k] = sign;
      GEQs[e].coef[0] = -1;
      GEQs[e].touched = 1;
      GEQs[e].color = color;
    }
  else
    {
      e = nEQ++;
      gcc_assert (nEQ <= maxEQs);
      eqnzero (&EQs[e]);
      EQs[e].coef[k] = 1;
      EQs[e].color = color;
    };
  unprotectVariable (problemPtr, i);
  return (simplifyProblem (problemPtr));
}

void
constrainVariableValue (Problem * problemPtr, int color, int i, int value)
{
  int e, k;

  k = problemPtr->forwardingAddress[i];
  if (k < 0)
    {
      k = -1 - k;

      e = nEQ++;
      gcc_assert (nEQ <= maxEQs);
      eqncpy (&EQs[e], &SUBs[k]);
      EQs[e].coef[0] -= value;

    }
  else
    {
      e = nEQ++;
      eqnzero (&EQs[e]);
      EQs[e].coef[k] = 1;
      EQs[e].coef[0] = -value;
    };
  EQs[e].color = color;
}

int
queryVariable (Problem * problemPtr, int i, int *lowerBound, int *upperBound)
{
  int nV = nVars;
  int e, j;
  int isSimple;
  int coupled = FALSE;
  i = problemPtr->forwardingAddress[i];

  (*lowerBound) = negInfinity;
  (*upperBound) = posInfinity;

  if (i < 0)
    {
      int easy = TRUE;
      i = -i - 1;
      for (j = 1; j <= nV; j++)
	if (SUBs[i].coef[j] != 0)
	  easy = FALSE;
      if (easy)
	{
	  *upperBound = *lowerBound = SUBs[i].coef[0];
	  return (FALSE);
	};
      return (TRUE);
    };

  for (e = nSUB - 1; e >= 0; e--)
    if (SUBs[e].coef[i] != 0)
      coupled = TRUE;

  for (e = nEQ - 1; e >= 0; e--)
    if (EQs[e].coef[i] != 0)
      {
	isSimple = TRUE;
	for (j = 1; j <= nV; j++)
	  if (i != j && EQs[e].coef[j] != 0)
	    {
	      isSimple = FALSE;
	      coupled = TRUE;
	      break;
	    };
	if (!isSimple)
	  continue;
	else
	  {
	    *lowerBound = *upperBound = -EQs[e].coef[i] * EQs[e].coef[0];
	    return (FALSE);
	  };
      };
  for (e = nGEQ - 1; e >= 0; e--)
    if (GEQs[e].coef[i] != 0)
      {
	if (GEQs[e].key == i)
	  {
	    setMax (*lowerBound, -GEQs[e].coef[0]);
	  }
	else if (GEQs[e].key == -i)
	  {
	    setMin (*upperBound, GEQs[e].coef[0]);
	  }
	else
	  coupled = TRUE;
      };
  return (coupled);
}

extern void queryCoupledVariable (Problem * problemPtr, int i, int *l, int *u,
				  int *couldBeZero, int lowerBound,
				  int upperBound);

int
queryVariableBounds (Problem * problemPtr, int i, int *l, int *u)
{
  int coupled;
  *l = negInfinity;
  *u = posInfinity;
  coupled = queryVariable (problemPtr, i, l, u);
  if (!coupled || (nVars == 1 && problemPtr->forwardingAddress[i] == 1))
    return 0;
  if (abs (problemPtr->forwardingAddress[i]) == 1 && nVars + nSUB == 2
      && nEQ + nSUB == 1)
    {
      int couldBeZero;
      queryCoupledVariable (problemPtr, i, l, u, &couldBeZero, negInfinity,
			    posInfinity);
      return 0;
    };
  return 1;
}

void
queryCoupledVariable (Problem * problemPtr, int i, int *l, int *u,
		      int *couldBeZero, int lowerBound, int upperBound)
{
  int e, b1, b2;
  Eqn eqn;
  int sign;
  int v;

  if (abs (problemPtr->forwardingAddress[i]) != 1 || nVars + nSUB != 2
      || nEQ + nSUB != 1)
    {
      fprintf (outputFile,
	       "queryCoupledVariablecalled with bad parameters\n");
      printProblem (problemPtr);
      exit (2);
    };

  if (problemPtr->forwardingAddress[i] == -1)
    {
      eqn = &SUBs[0];
      sign = 1;
      v = 1;
    }
  else
    {
      eqn = &EQs[0];
      sign = -eqn->coef[1];
      v = 2;
    };

  /* Variable i is defined in terms of variable v */

  for (e = nGEQ - 1; e >= 0; e--)
    if (GEQs[e].coef[v] != 0)
      {
	if (GEQs[e].coef[v] == 1)
	  {
	  setMax (lowerBound, -GEQs[e].coef[0])}
	else
	  {
	    setMin (upperBound, GEQs[e].coef[0]);
	  };
      };
  /* lowerBound and upperBound are bounds on the value of v */

  if (lowerBound > upperBound)
    {
      *l = posInfinity;
      *u = negInfinity;
      *couldBeZero = 0;
      return;
    };
  if (lowerBound == negInfinity)
    {
      if (eqn->coef[v] > 0)
	b1 = sign * negInfinity;
      else
	b1 = -sign * negInfinity;
    }
  else
    b1 = sign * (eqn->coef[0] + eqn->coef[v] * lowerBound);
  if (upperBound == posInfinity)
    {
      if (eqn->coef[v] > 0)
	b2 = sign * posInfinity;
      else
	b2 = -sign * posInfinity;
    }
  else
    b2 = sign * (eqn->coef[0] + eqn->coef[v] * upperBound);

  /* b1 and b2 are bounds on the value of i (don't know which is upper bound) */
  if (b1 <= b2)
    {
      setMax (*l, b1);
      setMin (*u, b2);
    }
  else
    {
      setMax (*l, b2);
      setMin (*u, b1);
    };
  *couldBeZero = *l <= 0 && 0 <= *u
    && intMod (eqn->coef[0], abs (eqn->coef[v])) == 0;
}



int
queryVariableSigns (Problem * problemPtr, int i, int dd_lt, int dd_eq,
		    int dd_gt, int lowerBound, int upperBound,
		    bool * distKnown, int *dist)
{
  int result;
  int l, u;
  int couldBeZero;

  l = negInfinity;
  u = posInfinity;

  queryVariable (problemPtr, i, &l, &u);
  queryCoupledVariable (problemPtr, i, &l, &u, &couldBeZero, lowerBound,
			upperBound);
  result = 0;
  if (l < 0)
    result |= dd_gt;
  if (u > 0)
    result |= dd_lt;
  if (couldBeZero)
    result |= dd_eq;
  if (l == u)
    {
      *distKnown = 1;
      *dist = l;
    }
  else
    {
      *distKnown = 0;
    };
  return (result);
}


static int omegaInitialized = 0;

void
initializeOmega (void)
{
  int i;
  if (omegaInitialized)
    return;

  nextWildCard = 0;
  nextKey = maxVars + 1;
  for (i = 0; i < hashTableSize; i++)
    hashMaster[i].touched = -1;

  sprintf (wildName[1], "alpha");
  sprintf (wildName[2], "beta");
  sprintf (wildName[3], "gamma");
  sprintf (wildName[4], "delta");
  sprintf (wildName[5], "tau");
  sprintf (wildName[6], "sigma");
  sprintf (wildName[7], "chi");
  sprintf (wildName[8], "omega");
  sprintf (wildName[9], "pi");
  sprintf (wildName[10], "ni");
  sprintf (wildName[11], "Alpha");
  sprintf (wildName[12], "Beta");
  sprintf (wildName[13], "Gamma");
  sprintf (wildName[14], "Delta");
  sprintf (wildName[15], "Tau");
  sprintf (wildName[16], "Sigma");
  sprintf (wildName[17], "Chi");
  sprintf (wildName[18], "Omega");
  sprintf (wildName[19], "Pi");

  omegaInitialized = 1;
}


void
setOutputFile (FILE * file)
{
  /* sets the file to which printProblem should send its output to "file" */

  outputFile = file;

}				/* end setOutputFile(FILE *file) */
