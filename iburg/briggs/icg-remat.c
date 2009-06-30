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
#include "flags.h"
#include "tm_p.h"

#include "icg.h"
#include "icg-opcode.h"



#define find(r) (icg_reg_vector[r].root)

#define RHS_REMAT (1 << 0)
#define LHS_REG   (1 << 1)

static
unsigned reduce_remat(icg_node *p, NT goalNT)
{
  unsigned flags = 0;
  icg_node *kid[MAX_KIDS];
  const RuleNumber rule = icg_burm_rule(p->state_label, goalNT);
  const NT *nts = icg_burm_nts[rule];
  int i;

  icg_burm_kids(p, rule, kid);
  for (i = 0; nts[i]; i++) {
    flags |= reduce_remat(kid[i], nts[i]);
  }

  switch (rule) {
#include "icg-remat.cases"
  }
  return flags;
}



void icg_remat(void)
{
  basic_block bb;

  if (dump_file) {
    fprintf(dump_file, "\n");
    fprintf(dump_file, "remat: started {\n");
    fprintf(dump_file, "remat: looking for opportunities\n");
    fprintf(dump_file, "\n");
  }

  FOR_EACH_BB(bb) {
    rtx insn;
    FOR_BB_INSNS_REVERSE(bb, insn) {
      unsigned id = INSN_UID(insn);
      icg_node *tree = icg_insn2tree[id];
      if (tree) {
	unsigned flags = reduce_remat(tree, burm_goal_NT);
	if (dump_file &&
	    (flags & (LHS_REG|RHS_REMAT)) == (LHS_REG|RHS_REMAT))   
	  fprintf(dump_file, "    found remat possibility\n");
      }
    }
  }

  if (dump_file) {
    fprintf(dump_file, "remat: finished}\n");
  }
}
