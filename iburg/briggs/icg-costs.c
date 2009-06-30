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
#include "sparseset.h"
#include "tree-pass.h"

#include "icg.h"

#define forall(i, s) EXECUTE_IF_SET_IN_SPARSESET(s, i)


/*
 * See Preston's thesis, section 8.7
 *
 * Note that here we're treating each expression tree
 * as a single instruction, with interior registers
 * given infinite spill cost.
 *
 * May want to reconsider this some day.
 *
 * This is a very simple implementation, so we can get going.
 * Currently it assumes spilling will add loads and stores
 * for each definition and use.  Later, we can extend it
 * to include all of Chaitin's clever ideas.
 *
 */


static
float frequency;

static
unsigned pass;

#define find(r) (icg_reg_vector[r].root)

static
void cost_copy(const unsigned original_x, const unsigned original_y) {
  const unsigned x = find(original_x);
  const unsigned y = find(original_y);
  if (x != y &&
      (pass > 0 ||
       (x >= FIRST_PSEUDO_REGISTER && y >= FIRST_PSEUDO_REGISTER))) {
    icg_reg_vector[x].copies += frequency;
    icg_reg_vector[y].copies += frequency;
  }
}


static
void cost_load(const unsigned original_x) {
  const unsigned x = find(original_x);
  icg_reg_vector[x].loads += frequency;
  icg_reg_vector[x].leaf = true;
}


static
void cost_store(const unsigned original_x) {
  const unsigned x = find(original_x);
  icg_reg_vector[x].stores += frequency;
}


static
void memorable(const unsigned original_x) {
  const unsigned x = find(original_x);
  icg_reg_vector[x].leaf = false;
}


static
void forgettable(const unsigned original_x) {
  const unsigned x = find(original_x);
  if (icg_reg_vector[x].leaf) {
    icg_reg_vector[x].points += frequency;
    icg_reg_vector[x].leaf = false;
  }
}


static
void reduce_costs(icg_node *p, NT goalNT)
{
  icg_node *kid[MAX_KIDS];
  const RuleNumber rule = icg_burm_rule(p->state_label, goalNT);
  const NT *nts = icg_burm_nts[rule];
  int i;

  icg_burm_kids(p, rule, kid);
  for (i = 0; nts[i]; i++)
    reduce_costs(kid[i], nts[i]);
  switch (rule) {
#include "icg-costs.cases"
  }
}



void icg_costs(unsigned p)
{
  unsigned r;
  basic_block bb;

  pass = p;
  for (r = 0; r < FIRST_PSEUDO_REGISTER; r++)
    {
      icg_reg_vector[r].cost   = 1.0/0.0;
      icg_reg_vector[r].loads  = 0.0;
      icg_reg_vector[r].stores = 0.0;
      icg_reg_vector[r].points = 0.0;
      icg_reg_vector[r].copies = 0.0;
      icg_reg_vector[r].leaf = false;
    }
  for (r = FIRST_PSEUDO_REGISTER; r < icg_live_ranges; r++)
    {
      icg_reg_vector[r].cost   = 0.0;
      icg_reg_vector[r].loads  = 0.0;
      icg_reg_vector[r].stores = 0.0;
      icg_reg_vector[r].points = 0.0;
      icg_reg_vector[r].copies = 0.0;
      icg_reg_vector[r].leaf = false;
    }
  for (r = icg_live_ranges; r < icg_interior_regs; r++)
    {
      icg_reg_vector[r].cost   = 1.0/0.0;
      icg_reg_vector[r].loads  = 0.0;
      icg_reg_vector[r].stores = 0.0;
      icg_reg_vector[r].points = 0.0;
      icg_reg_vector[r].copies = 0.0;
      icg_reg_vector[r].leaf = false;
    }

  FOR_EACH_BB(bb)
    {
      rtx insn;
      frequency = bb->frequency;
      if (frequency < 1.0)
	frequency = 1.0;
      FOR_BB_INSNS_REVERSE(bb, insn)
        {
	  unsigned id = INSN_UID(insn);
	  icg_node *tree = icg_insn2tree[id];
	  if (tree) {
	    reduce_costs(tree, burm_goal_NT);
	  }
        }
    }

  for (r = FIRST_PSEUDO_REGISTER; r < icg_live_ranges; r++)
    icg_reg_vector[r].cost = 3*icg_reg_vector[r].loads
	                   + 3*icg_reg_vector[r].stores
	                   + icg_reg_vector[r].points
	                   - icg_reg_vector[r].copies;
  if (dump_file)
    {
      fprintf(dump_file, "\n" "spill costs\n");
      for (r = FIRST_PSEUDO_REGISTER; r < icg_live_ranges; r++)
	if (icg_reg_vector[r].root == r)
	  fprintf(dump_file, "    r%-2d = %6.0f (%6.0f, %6.0f, %6.0f, %6.0f)\n", r,
		  icg_reg_vector[r].cost,
		  icg_reg_vector[r].loads,
		  icg_reg_vector[r].points,
		  icg_reg_vector[r].stores,
		  icg_reg_vector[r].copies);
    }
}
