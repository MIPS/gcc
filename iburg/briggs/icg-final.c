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
#include "flags.h"

#include "icg.h"

#define find(r) (icg_reg_vector[r].root)

static
void reduce_final(icg_node *p, NT goalNT) {
  icg_node *kid[MAX_KIDS];
  const RuleNumber rule = icg_burm_rule(p->state_label, goalNT);
  const NT *nts = icg_burm_nts[rule];
  int i;

  icg_burm_kids(p, rule, kid);
  for (i = 0; nts[i]; i++)
    reduce_final(kid[i], nts[i]);
  switch (rule) {
#include "icg-final.cases"
  }
}


void icg_final(void)
{
  basic_block bb;
  if (frame_pointer_needed) {
    icg_reg_vector[REG_FP].root = REG_RBP;
    icg_reg_vector[REG_ARGP].root = REG_RBP;
  }
  else {
    icg_reg_vector[REG_FP].root = REG_RSP;
    icg_reg_vector[REG_ARGP].root = REG_RSP;
  }

  if (dump_file) {
    unsigned r;
    for (r = 0; r < icg_interior_regs; r++)
      fprintf(dump_file, "r = %3d, root = %3d, color = %3d\n",
	      r, icg_reg_vector[r].root, icg_reg_vector[r].color);
  }

  FOR_EACH_BB(bb) {
    rtx insn;
    FOR_BB_INSNS(bb, insn) {
      int id = INSN_UID(insn);
      icg_node *tree = icg_insn2tree[id];
      if (tree) {
	reduce_final(tree, burm_goal_NT);
      }
    }
  }
}
