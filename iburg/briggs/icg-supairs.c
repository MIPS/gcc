/*
 * Copyright (c) 2008 Google Inc. All rights reserved.
 *
 * $Header: $
 */
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "function.h"
#include "basic-block.h"
#include "tree-pass.h"

#include "icg.h"

/*
 * TODO: the perm vector is only defined for subject nodes that are matched
 * by the root of a pattern tree, where the pattern tree has >= 2 non-terminals.
 * This lets us avoid overwriting legitimate permutations with the 0:0 permutation
 * induced by chain rules.
 */

static
void suOrder2(icg_node *p, icg_node *x, icg_node *y,
  icg_node **kid, int kids) {
  int i, j;
  int cost0, cost1;
  icg_node *left;
  icg_node *right;

  p->perm_kids = 2;
  j = 0;
  for (i = 0; i < kids; i++)
    if (kid[i] == x ||
        kid[i] == y)
      p->perm[j++] = i;

  /* j should equal 2 */
  if (j != 2)
    fprintf(stderr, "\n" "j != 2\n");


  left  = kid[p->perm[0]];
  right = kid[p->perm[1]];
  cost0 = left->extra  + left->freed  > right->extra ?
          left->extra  : right->extra - left->freed  + 1;
  cost1 = right->extra + right->freed > left->extra  ?
          right->extra : left->extra  - right->freed + 1;
  if (cost0 > cost1) {
    p->extra = cost1;
    i = p->perm[0];
    p->perm[0] = p->perm[1];
    p->perm[1] = i;
  }
  else
    p->extra = cost0;
  p->freed = x->freed + y->freed;
}


/* see Knuth, Volume 4, Section 7.2.1.2, Algorithm L */

static
int next_perm(short *a, int n) {
  int j, k, l, t;

  j = n - 1;
  while (a[j] >= a[j + 1])
    j--;
  if (j == 0) return 0;

  l = n;
  while (a[j] >= a[l])
    l--;
  t = a[j]; a[j] = a[l]; a[l] = t;

  k = j + 1;
  l = n;
  while (k < l) {
    t = a[k]; a[k] = a[l]; a[l] = t;
    k++;
    l--;
  }
  return 1;
}


/* Is this correct?  Check closely. */

static
int cost_of_visit(icg_node *p, int n, icg_node **kid) {
  int i;
  short *perm = p->perm;
  int cost = 0;
  int spare = 0;
  for (i = 0; i < n; i++) {
    int freed = kid[perm[i]]->freed;
    int extra = kid[perm[i]]->extra;
    if (spare < extra) {
      cost += extra - spare;
      spare = freed + extra - 1;
    }
    else
      spare += freed - 1;
  }
  return cost;
}



/* This is pretty gross.  Can I find a better approach? */

static
void suOrder3(icg_node *p,
  icg_node *x, icg_node *y, icg_node *z,
  icg_node **kid, int kids) {
  int i, j;
  int best_index, best_cost;
  int cost[MAX_KIDS];

  p->perm_kids = 3;
  j = 0;
  for (i = 0; i < kids; i++)
    if (kid[i] == x ||
        kid[i] == y ||
        kid[i] == z)
      p->perm[j++] = i;

  /* j should equal 3 */
  if (j != 3)
    fprintf(stderr, "\n" "j != 3\n");

  i = 0;
  do {
    cost[i++] = cost_of_visit(p, 3, kid);
  } while (next_perm(p->perm, 3));
  best_cost = cost[0];
  best_index = 0;
  for (j = 1; j < i; j++)
    if (cost[j] < best_cost) {
      best_cost = cost[j];
      best_index = j;
    }
  i = p->perm[0];
  p->perm[0] = p->perm[2];
  p->perm[2] = i;
  for (i = 0; i < best_index; i++)
    (void) next_perm(p->perm, 3);
  p->extra = best_cost;
  p->freed = x->freed + y->freed + z->freed;
}


static
void reduce_supairs(icg_node *p, NT goalNT) {
  icg_node *kid[MAX_KIDS];
  const RuleNumber rule = icg_burm_rule(p->state_label, goalNT);
  const NT *nts = icg_burm_nts[rule];
  int i, kids;

  icg_burm_kids(p, rule, kid);
  for (i = 0; nts[i]; i++)
    reduce_supairs(kid[i], nts[i]);
  kids = i;
  switch (rule) {
#include "icg-supairs.cases"
  }
}



void icg_supairs(void)
{
  basic_block bb;
  (void)suOrder3;	/* this function is not currently used; shut up gcc */
  FOR_EACH_BB(bb)
    {
      rtx insn;
      FOR_BB_INSNS(bb, insn)
        {
	  int id = INSN_UID(insn);
	  if (icg_insn2dirty[id])
	    {
	      icg_node *tree = icg_insn2tree[id];
	      if (tree) {
		reduce_supairs(tree, burm_goal_NT);
	      }
	    }
        }
    }
}
