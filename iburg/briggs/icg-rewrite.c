/* Rewrite the ICG trees as a result of constant folding
 and memory update discovery

   Copyright (C) 2008  Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "obstack.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "flags.h"
#include "function.h"
#include "basic-block.h"
#include "tree-pass.h"
#include "icg.h"
#include "icg-opcode.h"
#include "insn-constants.h"

static
bool icg_shallow_equal(const icg_node *p, const icg_node *q)
{
  if (p == q) {
    return true;
  }
  if (p && q) {
    if (1
    && (p->op  == q->op)
    && (p->val == q->val)
    && (p->r   == q->r)
    && (p->rx  == q->rx)
    ) {
      return true;
    }
  }
  return false;
}

static
bool icg_deep_equal(const icg_node *p, const icg_node *q)
{
  bool eq = true;
  if (p == q) {
    return eq;
  }
  eq &= icg_shallow_equal(p, q);
  if (!eq) {
    return false;
  }
  if (p->left && q->left) {
    eq &= icg_deep_equal(p->left, q->left);
  }
  if (!eq) {
    return false;
  }
  if (p->right && q->right) {
    eq &= icg_deep_equal(p->right, q->right);
  }
  return eq;
}
/*
 * Rewrite P to extract a subtree deeply matching PATTERN.
 * Return by reference through *RVAL the matched subtree, if any.
 */
static
icg_node *icg_update_finder1(icg_opcodes op, bool comm,
  icg_node *p, const icg_node *pattern, icg_node **rval)
{
  if (p == 0) {
    return p;
  }
  if (p->op == op) {
    if (p->left && icg_deep_equal(p->left, pattern)) {
      if (rval) {
	*rval = p->left;	/*   matched portion */
      }
      return p->right;		/* unmatched portion */
    }
    if (comm && p->right && icg_deep_equal(p->right, pattern)) {
      if (rval) {
	*rval = p->right;	/*   matched portion */
      }
      return p->left;		/* unmatched portion */
    }
    if (p->left) {
      p->left  = icg_update_finder1(op, comm, p->left,  pattern, rval);
    }
    if (p->right) {
      p->right = icg_update_finder1(op, comm, p->right, pattern, rval);
    }
  }
  return p;
}

typedef struct _comm_op_info {
  icg_opcodes mem;
  icg_opcodes op;
  icg_opcodes mem_op;
  bool comm;
} comm_op_info;

static const comm_op_info optable[] = {
  {MEM_DI, AND_DI,      MEM_AND_DI,      true},
  {MEM_HI, AND_HI,      MEM_AND_HI,      true},
  {MEM_QI, AND_QI,      MEM_AND_QI,      true},
  {MEM_SI, AND_SI,      MEM_AND_SI,      true},

  {MEM_DI, ASHIFTRT_DI, MEM_ASHIFTRT_DI, false},
  {MEM_HI, ASHIFTRT_HI, MEM_ASHIFTRT_HI, false},
  {MEM_QI, ASHIFTRT_QI, MEM_ASHIFTRT_QI, false},
  {MEM_SI, ASHIFTRT_SI, MEM_ASHIFTRT_SI, false},

  {MEM_DI, ASHIFT_DI,   MEM_ASHIFT_DI,   false},
  {MEM_HI, ASHIFT_HI,   MEM_ASHIFT_HI,   false},
  {MEM_QI, ASHIFT_QI,   MEM_ASHIFT_QI,   false},
  {MEM_SI, ASHIFT_SI,   MEM_ASHIFT_SI,   false},

  {MEM_DI, IOR_DI,      MEM_IOR_DI,      true},
  {MEM_HI, IOR_HI,      MEM_IOR_HI,      true},
  {MEM_QI, IOR_QI,      MEM_IOR_QI,      true},
  {MEM_SI, IOR_SI,      MEM_IOR_SI,      true},

  {MEM_SI, LSHIFTRT_SI, MEM_LSHIFTRT_SI, false},
  {MEM_DI, LSHIFTRT_DI, MEM_LSHIFTRT_DI, false},
  {MEM_HI, LSHIFTRT_HI, MEM_LSHIFTRT_HI, false},
  {MEM_QI, LSHIFTRT_QI, MEM_LSHIFTRT_QI, false},

  {MEM_DI, MINUS_DI,    MEM_MINUS_DI,    false},
  {MEM_HI, MINUS_HI,    MEM_MINUS_HI,    false},
  {MEM_QI, MINUS_QI,    MEM_MINUS_QI,    false},
  {MEM_SI, MINUS_SI,    MEM_MINUS_SI,    false},

  {MEM_DI, NEG_DI,      MEM_NEG_DI,      false},
  {MEM_HI, NEG_HI,      MEM_NEG_HI,      false},
  {MEM_QI, NEG_QI,      MEM_NEG_QI,      false},
  {MEM_SI, NEG_SI,      MEM_NEG_SI,      false},

  {MEM_DI, NOT_DI,      MEM_NOT_DI,      false},
  {MEM_HI, NOT_HI,      MEM_NOT_HI,      false},
  {MEM_QI, NOT_QI,      MEM_NOT_QI,      false},
  {MEM_SI, NOT_SI,      MEM_NOT_SI,      false},

  {MEM_DI, PLUS_DI,     MEM_PLUS_DI,     true},
  {MEM_HI, PLUS_HI,     MEM_PLUS_HI,     true},
  {MEM_QI, PLUS_QI,     MEM_PLUS_QI,     true},
  {MEM_SI, PLUS_SI,     MEM_PLUS_SI,     true},

  {MEM_DI, ROTATERT_DI, MEM_ROTATERT_DI, false},
  {MEM_HI, ROTATERT_HI, MEM_ROTATERT_HI, false},
  {MEM_QI, ROTATERT_QI, MEM_ROTATERT_QI, false},
  {MEM_SI, ROTATERT_SI, MEM_ROTATERT_SI, false},

  {MEM_DI, ROTATE_DI,   MEM_ROTATE_DI,   false},
  {MEM_HI, ROTATE_HI,   MEM_ROTATE_HI,   false},
  {MEM_QI, ROTATE_QI,   MEM_ROTATE_QI,   false},
  {MEM_SI, ROTATE_SI,   MEM_ROTATE_SI,   false},

  {MEM_DI, XOR_DI,      MEM_XOR_DI,      true},
  {MEM_HI, XOR_HI,      MEM_XOR_HI,      true},
  {MEM_QI, XOR_QI,      MEM_XOR_QI,      true},
  {MEM_SI, XOR_SI,      MEM_XOR_SI,      true},
};

static
icg_node *icg_update_finder(icg_node *p)
{
  if (p->op == SET_ALL) {
    size_t i;
    if (0
    || p->left->op == MEM_DI
    || p->left->op == MEM_SI
    || p->left->op == MEM_HI
    || p->left->op == MEM_QI
    ) {
      for (i = 0; i < sizeof(optable)/sizeof(optable[0]); i++) {
	if (p->left->op == optable[i].mem) {
	  const icg_opcodes op = optable[i].op;
	  if (p->right->op == op) {
	    icg_node *rval = 0;
	    p->right = icg_update_finder1(op, optable[i].comm,
	      p->right, p->left, &rval);
	    if (rval) {
	      icg_node *n = (icg_node *)icg_calloc(sizeof(icg_node));
	      if (dump_file) {
		fprintf(dump_file, "%16s ==> %16s update found\n",
		  icg_burm_opname[op],
		  icg_burm_opname[optable[i].mem_op]);
	      }
	      n->op = optable[i].mem_op;
	      n->insn = p->insn;
	      n->rtl = p->rtl;
	      n->left = p->left;
	      n->right = p->right;	/* rewritten from icg_update_finder1 */
	      return n;
	    }
	  }
	}
      }
    }
  }
  return p;
}

static
icg_node *icg_constant_folder(icg_node *p, bool *isconstp)
{
  bool l_isconst = false;
  bool r_isconst = false;
  if (isconstp) {
    *isconstp = false;
  }
  if (p == 0) {
    return p;
  }
  if (p->left) {
    icg_constant_folder(p->left, &l_isconst);
  }
  if (p->right) {
    icg_constant_folder(p->right, &r_isconst);
  }
  if (p->left && l_isconst && r_isconst) {
    if (isconstp) {
      *isconstp = true;
    }
    print_icg_tree(stdout, p, "constant");
  }
  return p;
}

icg_node *icg_tree_rewrite (icg_node *p)
{
  if (0) {
    return p;
  } else {
    if (0) {
      /* TODO: nyi */
      bool is_const = false;
      return icg_constant_folder(p, &is_const);
    } else {
      return icg_update_finder(p);
    }
  }
}



void icg_find_updates(void) {
  basic_block bb;

  if (dump_file)
    fprintf(dump_file, "\nstarted finding updates\n");

  FOR_EACH_BB(bb) {
    rtx insn;
    FOR_BB_INSNS(bb, insn) {
      unsigned id = INSN_UID(insn);
      if (icg_insn2dirty[id]) {
	icg_node *tree = icg_insn2tree[id];
	if (tree)
	  icg_insn2tree[id] = icg_tree_rewrite(tree);
      }
    }
  }


  if (dump_file)
    fprintf(dump_file, "finished finding updates\n\n");
}
