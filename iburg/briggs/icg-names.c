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

unsigned icg_interior_regs = 0;
static
void hook_names(void) __attribute__((noinline));

static
void hook_names(void)
{
}

static
int new_reg(void) {
  return icg_interior_regs++;
}


static
void reduce_names(icg_node *p, NT goalNT) {
  icg_node *kid[MAX_KIDS];
  const RuleNumber rule = icg_burm_rule(p->state_label, goalNT);
  const NT *nts = icg_burm_nts[rule];
  int i;

  p->goalNT = goalNT;

  icg_burm_kids(p, rule, kid);
  for (i = 0; nts[i]; i++) {
    reduce_names(kid[i], nts[i]);
  }
  if (i >= 2) {
    /*
     * Do NOT clobber p->perm_kids unless this is really a node with a non trivial cover
     * Subsequent icg passes will only consult p->perm_kids if nts[0] && nts[1]
     * for doing permutations.
     */
    p->perm_kids = i; /* mark number of non-chainrule kids */
    for (i = 0; nts[i]; i++) {
      p->perm[i] = i; /* set up initial 1:1 permutation */
    }
  }
  switch (rule) {
#include "icg-names.cases"
  }
}


static
void reduce_kinds(icg_node *p, NT goalNT) {
  icg_node *kid[MAX_KIDS];
  const RuleNumber rule = icg_burm_rule(p->state_label, goalNT);
  const NT *nts = icg_burm_nts[rule];
  int i;

  p->goalNT = goalNT;

  icg_burm_kids(p, rule, kid);
  for (i = 0; nts[i]; i++)
    reduce_kinds(kid[i], nts[i]);
  switch (rule) {
#include "icg-kinds.cases"
  }
}


void icg_find_names(void)
{
  basic_block bb;
  unsigned r;
  icg_interior_regs = icg_live_ranges;
  FOR_EACH_BB(bb)
    {
      rtx insn;
      FOR_BB_INSNS(bb, insn)
        {
	  int id = INSN_UID(insn);
	  icg_node *tree = icg_insn2tree[id];
	  if (tree) {
	    reduce_names(tree, burm_goal_NT);
	  }
        }
    }

  icg_reg_vector = (icg_reg_info *)obstack_alloc(&icg_graph_obstack, icg_interior_regs*sizeof(icg_reg_info));
  for (r = 0; r < FIRST_PSEUDO_REGISTER; r++)
    icg_reg_vector[r].kind = 0;
  for (r = FIRST_PSEUDO_REGISTER; r < icg_interior_regs; r++)
    icg_reg_vector[r].kind = ~1;
  for (r = 0; r < icg_interior_regs; r++)
    icg_reg_vector[r].root = r;

  if (dump_file)
    {
      fprintf(dump_file, "\n");
      fprintf(dump_file, "first       = %4d\n", FIRST_PSEUDO_REGISTER);
      fprintf(dump_file, "live ranges = %4d\n", icg_live_ranges);
      fprintf(dump_file, "interior    = %4d\n", icg_interior_regs);
    }

  FOR_EACH_BB(bb)
    {
      rtx insn;
      FOR_BB_INSNS(bb, insn)
        {
	  int id = INSN_UID(insn);
	  icg_node *tree = icg_insn2tree[id];
	  if (tree)
	    reduce_kinds(tree, 1);
        }
    }

  for (r = FIRST_PSEUDO_REGISTER; r < icg_interior_regs; r++) {
    if (icg_reg_vector[r].kind == ~((unsigned)1)) {
      icg_reg_vector[r].root = 0;
    }
  }
}
