/* A simple aproximate ATSP solver.
   Copyright (C) 2003 Free Software Foundation, Inc.

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
#include "rtl.h"
#include "basic-block.h"
#include "cfglayout.h"
#include "output.h"

/* A simple assymetric tsp solver.  It uses the following heuristic (referred
   as OPT-3 + HyperOPT in literature):

   (Tour is made so that it always starts in 0 and ends in n - 1).
   
   All improving 3-swaps (reordering of tour ABCD into ACBD, where A, B,
      C and D are parts of the tour) are performed.  To decrease the amount
      of work done, we use a small observation -- there are at most two cheap
      edges coming from each vertex (all other edges out of the vertex
      have the same cost) and at least one such edge must obviously be
      involved in the swap; moreover, it must be one of the newly added edges.
   
   For every two non-overlapping segments of length 3, relink them in
   an optimal manner.  */

#define HOPT_H 3
#define HOPT_H_NORDS 6
#define HOPT_H_VERT (2 * HOPT_H - 2)
#define ORDS_PER_EDGE (HOPT_H_NORDS / (HOPT_H_VERT - 1))

/* All orders HyperOPT heuristics tries.  */
static int hopt_h_ords[HOPT_H_NORDS][HOPT_H_VERT] =
{
  {0, 1, 2, 3},
  {0, 1, 3, 2},
  {0, 2, 1, 3},
  {0, 2, 3, 1},
  {0, 3, 1, 2},
  {0, 3, 2, 1}
};

/* For each edge indices of sums in the HyperOPT heuristics to that the
   weight of the edge should be added.  */
static int hopt_h_sums[HOPT_H_VERT][HOPT_H_VERT][ORDS_PER_EDGE] =
{
  {					/* (0, *) */
    {HOPT_H_NORDS, HOPT_H_NORDS},	/* (0, 0) */
    {0, 1},				/* (0, 1) */
    {2, 3},				/* (0, 2) */
    {4, 5}				/* (0, 3) */
  },
  {					/* (1, *) */
    {3, 5},				/* (1, 0) */
    {HOPT_H_NORDS, HOPT_H_NORDS}, 	/* (1, 1) */
    {0, 4},				/* (1, 2) */
    {1, 2}				/* (1, 3) */
  },
  {					/* (2, *) */
    {1, 4},				/* (2, 0) */
    {2, 5},				/* (2, 1) */
    {HOPT_H_NORDS, HOPT_H_NORDS}, 	/* (2, 2) */
    {0, 3}				/* (2, 3) */
  },
  {					/* (3, *) */
    {0, 2},				/* (3, 0) */
    {3, 4},				/* (3, 1) */
    {1, 5},				/* (3, 2) */
    {HOPT_H_NORDS, HOPT_H_NORDS}	/* (3, 3) */
  }
};

/* Weights of edges are cached here.  */

static int graph[MAX_TSP_SIZE + 2][MAX_TSP_SIZE + 2];

/* Returns weight of edge A --> B.  */

#define edge_weight(A, B) graph[A][B]

static void optimize_tsp (int, int *, struct vertex *, int);
static int tsp_value (int, int *);
static int three_swap_delta (int *, int, int, int);
void dump_tour (FILE *, int *, int);
static int hopt_delta (int [], int [], int [], sbitmap, int *, struct vertex *);
static void best_three_swap (int, int, int *, int *, int *, int *, int, int *);
static void hopt_perform (int *, int, int, int);

/* Solves a tsp instance of size N.  WEIGHTS is a matrix of weights of
   edges.  Result is retunrned in TOUR; the initial tour is also obtained
   from there.  */

void
solve_tsp (int n, int *tour, struct vertex *weights, int iterate)
{
  int tspvalue;
  int i, j;

  if (rtl_dump_file)
    fprintf (rtl_dump_file, "TSP:\n");

  for (i = 0; i < n; i++)
    {
      for (j = 0; j < n; j++)
	graph[i][j] = weights[i].def_cost;
      for (j = 0; j < weights[i].n_spec; j++)
	graph[i][weights[i].spec_tgt[j]] = weights[i].spec_cost[j];
    }

  tspvalue = tsp_value (n, tour);

  if (rtl_dump_file)
    {
      dump_tour (rtl_dump_file, tour, n);
      fprintf (rtl_dump_file, "%d\n", tspvalue);
    }

  if (tspvalue)
    optimize_tsp (n, tour, weights, iterate);

  if (rtl_dump_file)
    {
      dump_tour (rtl_dump_file, tour, n);
      fprintf (rtl_dump_file, "FINAL: %d\n", tsp_value (n, tour));
    }
}

/* Dumps TOUR of length N to FILE.  */

void
dump_tour (FILE *file, int *tour, int n)
{
  int i, a;

  for (i = 0, a = 0; i < n - 1; i++, a = tour[a])
    fprintf (file, "%d ", a);
  fprintf (file, "%d\n", a);
}

/* Return value of a tsp TOUR on N vertices.  */

static int
tsp_value (int n, int *tour)
{
  int i, act, sum = 0;

  act = 0;
  for (i = 0; i < n - 1; i++, act = tour[act])
    sum += edge_weight (act, tour[act]);

  return sum;
}

/* Returns gain of performing a 3-swap on TOUR in graph WEIGHTS at
   positions I1, I2 and I3.  */

static int
three_swap_delta (int *tour, int i1, int i2, int i3)
{
  int old, new;

  old = (edge_weight (i1, tour[i1])
	 + edge_weight (i2, tour[i2])
	 + edge_weight (i3, tour[i3]));

  new = (edge_weight (i1, tour[i2])
	 + edge_weight (i2, tour[i3])
	 + edge_weight (i3, tour[i1]));

  return new - old;
}

/* Returns gain of performing a HyperOPT optimization on segments stored in V1 
   and V2.  Index of the best permutation is returned in BEST_PERM.  WEIGHTS
   is the graph in that we work.  V2S is a bitmap of members of V2.  TOUR is
   the current tour.  */

static int
hopt_delta (int tour[], int v1[], int v2[], sbitmap v2s, int *best_perm,
	    struct vertex *weights)
{
  int i, j, old, best, bi, a, av, ew;
  int sums[HOPT_H_NORDS + 1];

  for (i = 0; i < HOPT_H_VERT; i++)
    for (j = 0; j < weights[v1[i]].n_spec; j++)
      {
	a = weights[v1[i]].spec_tgt[j];
	if (a != tour[v1[i]] && TEST_BIT (v2s, a))
	  goto cont;
      }
  return 0;

cont: ;

  /* Loop is manually unrolled here based on assumption that noone will ever
     increase the value of HOPT_H.  Just make sure that if someone insane
     enough does it, he notices this place.  */
#if (ORDS_PER_EDGE != 2)
  abort ();
#endif

  memset (sums, 0, sizeof (sums));
  for (i = 0; i < HOPT_H_VERT; i++)
    {
      av = v1[i];
      for (j = 0; j < HOPT_H_VERT; j++)
	{
	  ew = edge_weight (av, v2[j]);
	  sums[hopt_h_sums[i][j][0]] += ew;
	  sums[hopt_h_sums[i][j][1]] += ew;
	}
    }

  bi = 0;
  old = best = sums[0];
  for (i = 1; i < HOPT_H_NORDS; i++)
    {
      a = sums[i];

      if (a >= best)
	continue;

      best = a;
      bi = i;
    }

  *best_perm = bi;
  return best - old;
}

/* Perform PERMth HyperOPT swap on TOUR at positions I1 and I2.  */

static void
hopt_perform (int *tour, int i1, int i2, int perm)
{
  int i, j, a;
  int v1[HOPT_H_VERT], v2[HOPT_H_VERT];

  for (a = i1, i = 0; i < HOPT_H - 1; i++, a = tour[a])
    v1[i] = a;
  for (a = i2; i < HOPT_H_VERT; i++, a = tour[a])
    v1[i] = a;

  for (a = tour[i1], i = 1; i < HOPT_H; i++, a = tour[a])
    v2[i] = a;
  for (a = tour[i2]; i < HOPT_H_VERT; i++, a = tour[a])
    v2[i] = a;
  v2[0] = a;

  a = hopt_h_ords[perm][0];
  for (i = 1; i < HOPT_H_VERT; i++, a = j)
    {
      j = hopt_h_ords[perm][i];
      tour[v1[a]] = v2[j];
    }
  tour[v1[a]] = v2[hopt_h_ords[perm][0]];
}

/* Find the best 3-swap that introduces I1 --> I2 into TOUR in graph WEIGHTS
   on N vertices.  Endpoints of the segments are returned in BI1, BI2 and BI3,
   value in BEST_3OPT.  */

static void
best_three_swap (int i1, int i2, int *bi1, int *bi2, int *bi3, int *best_3opt,
		 int n, int *tour)
{
  int a, seen_i1 = false, m, v;

  if (tour[i1] == i2)
    return;

  for (a = 0; tour[a] != i2; a = tour[a])
    if (a == i1)
      seen_i1 = true;

  if (!seen_i1)
    {
      /* Backward edge.  The middle region must be between i1 and i2.  */
      for (m = i2; m != i1; m = tour[m])
	{
	  v = three_swap_delta (tour, a, m, i1);
	  if (v >= *best_3opt)
	    continue;

	  *bi1 = a;
	  *bi2 = m;
	  *bi3 = i1;
	  *best_3opt = v;
	}
    }
  else
    {
      /* Either this is the arc from A to C...  */
      for (m = i2; tour[m] != n; m = tour[m])
	{
	  v = three_swap_delta (tour, i1, a, m);
	  if (v >= *best_3opt)
	    continue;

	  *bi1 = i1;
	  *bi2 = a;
	  *bi3 = m;
	  *best_3opt = v;
	}

      /* ...or from B to D.  */
      for (m = 0; m != i1; m = tour[m])
	{
	  v = three_swap_delta (tour, m, i1, a);
	  if (v >= *best_3opt)
	    continue;

	  *bi1 = m;
	  *bi2 = i1;
	  *bi3 = a;
	  *best_3opt = v;
	}
    }
}

/* Improves TOUR using the heuristics.  N is number of vertices, the graph is
   stored in WEIGHTS.  ITERATE is true if we should iterate until convergence
   rather than making just one pass.  */

static void
optimize_tsp (int n, int *tour, struct vertex *weights, int iterate)
{
  int i1, i2, best_3opt, bi1, bi2, bi3, a, best_hopt, hi2, hperm, aperm, f, i;
  int changed = true;
  int temp;
  int value = 0, new_value;
  int work[MAX_TSP_SIZE + 2];
  int wbot, wtop;
  sbitmap in_work, v2s;
  int v1[HOPT_H_VERT], v2[HOPT_H_VERT];

#ifndef ENABLE_CHECKING
  if (rtl_dump_file)
    {
#endif
      value = tsp_value (n, tour);
#ifndef ENABLE_CHECKING
    }
#endif

  in_work = sbitmap_alloc (n);
  v2s = sbitmap_alloc (n);
  sbitmap_zero (v2s);

#define PUT(X)					\
  do						\
    {						\
      if (!TEST_BIT (in_work, (X)))		\
	{					\
	  SET_BIT (in_work, (X));		\
	  work[wtop++] = (X);			\
	  if (wtop == MAX_TSP_SIZE + 2)		\
	    wtop = 0;				\
	}					\
    }						\
  while (0)

  while (changed)
    {
      changed = false;

      for (i1 = 0; i1 < n - 1; i1++)
	work[i1] = i1;
      sbitmap_ones (in_work);
      wbot = 0;
      wtop = n - 1;

      while (wbot != wtop)
	{
	  i1 = work[wbot++];
	  if (wbot == MAX_TSP_SIZE + 2)
	    wbot = 0;
	  RESET_BIT (in_work, i1);
	  
	  /* Find a best 3-OPT move starting with I1.  */
	  best_3opt = 0;
	  bi1 = bi2 = bi3 = 0;

	  for (a = 0; a < weights[i1].n_spec; a++)
	    {
	      i2 = weights[i1].spec_tgt[a];

	      best_three_swap (i1, i2, &bi1, &bi2, &bi3, &best_3opt,
			       n, tour);
	    }

	  /* Find a best HyperOPT move starting with I1.  */
	  best_hopt = 0;
	  hperm = 0;
	  hi2 = 0;
	  for (i2 = i1, a = 0; i2 != n && a < HOPT_H - 1; a++)
	    i2 = tour[i2];
	  for (f = i2, a = 0; f != n && a < HOPT_H - 1; a++)
	    f = tour[f];

	  if (f != n)
	    {
	      for (a = i1, i = 0; i < HOPT_H - 1; i++, a = tour[a])
		v1[i] = a;
	      for (a = i2; i < HOPT_H_VERT; i++, a = tour[a])
		v1[i] = a;

	      for (a = tour[i1], i = 1; i < HOPT_H; i++, a = tour[a])
		{
		  v2[i] = a;
		  SET_BIT (v2s, a);
		}
	      for (a = tour[i2]; i < HOPT_H_VERT; i++, a = tour[a])
		{
		  v2[i] = a;
		  SET_BIT (v2s, a);
		}
	      v2[0] = a;
	      SET_BIT (v2s, a);

	      while (1)
		{
		  a = hopt_delta (tour, v1, v2, v2s, &aperm, weights);
		  if (a < best_hopt)
		    {
		      hi2 = i2;
		      best_hopt = a;
		      hperm = aperm;
		    }

		  i2 = tour[i2];
		  f = tour[f];
		  if (f == n)
		    break;

		  RESET_BIT (v2s, i2);
		  SET_BIT (v2s, f);

		  for (a = i2, i = HOPT_H - 1; i < HOPT_H_VERT; i++, a = tour[a])
		    v1[i] = a;
		  for (a = tour[i2], i = HOPT_H; i < HOPT_H_VERT; i++, a = tour[a])
		    v2[i] = a;
		  v2[0] = a;
		}

	      for (i = 0; i < HOPT_H_VERT; i++)
		RESET_BIT (v2s, v2[i]);
	    }

	  if (best_3opt < best_hopt)
	    {
	      temp = tour[bi1];
	      tour[bi1] = tour[bi2];
	      tour[bi2] = tour[bi3];
	      tour[bi3] = temp;

#ifndef ENABLE_CHECKING
	      if (rtl_dump_file)
		{
#endif
    		  new_value = value + best_3opt;
    		  value = tsp_value (n, tour);
#ifndef ENABLE_CHECKING
		}
#endif

	      PUT (bi1);
	      PUT (bi2);
	      PUT (bi3);

	      if (rtl_dump_file)
		{
		  fprintf (rtl_dump_file,
			   "3 swap %d %d %d -- new tour value %d\n",
			   bi1, bi2, bi3, value);
		  dump_tour (rtl_dump_file, tour, n);
		}
	    }
	  else if (best_hopt < 0)
	    {
	      for (a = i1, i2 = 0; i2 < HOPT_H; i2++, a = tour[a])
		PUT (a);
	      for (a = hi2, i2 = 0; i2 < HOPT_H; i2++, a = tour[a])
		PUT (a);

	      hopt_perform (tour, i1, hi2, hperm);

#ifndef ENABLE_CHECKING
	      if (rtl_dump_file)
		{
#endif
    		  new_value = value + best_hopt;
    		  value = tsp_value (n, tour);
#ifndef ENABLE_CHECKING
		}
#endif

	      if (rtl_dump_file)
		{
		  fprintf (rtl_dump_file,
			   "hopt %d %d -- new tour value %d\n", i1, hi2, value);
		  dump_tour (rtl_dump_file, tour, n);
		}
	    }
	  else
	    continue;

#ifdef ENABLE_CECKING
	  if (value != new_value)
	    abort ();
#endif
	  changed = true;
	}

      if (!iterate)
	break;
    }

  sbitmap_free (in_work);
  sbitmap_free (v2s);
}
