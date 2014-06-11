/* Merge common code between branches.
   Copyright (C) 2012 Cavium

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

/* This is a simple pass which optimizes away some common code between two
   branches of an if statement.
   It takes code like:
   if (best_score_1237 < this_score_1274)
    goto <bb 107>;
  else
    goto <bb 106>;

<bb 106>:
  right_2241 = delta[2];
  goto <bb 112>;

<bb 107>:
  right_2240 = delta[2];
  goto <bb 112>;

  # best_score_1277 = PHI <this_score_1274(107), best_score_1237(106)>
  # best_scoring_move_1278 = PHI <pos_556(107), best_scoring_move_1238(106)>
  # right_2 = PHI <right_2240(107), right_2241(106)>

 and converts it into:

   right_123 = delta[2]
   if (best_score_1237 < this_score_1274)
    goto <bb 107>;
  else
    goto <bb 106>;

<bb 106>:
  goto <bb 112>;

<bb 107>:
  goto <bb 112>;

  # best_score_1277 = PHI <this_score_1274(107), best_score_1237(106)>
  # best_scoring_move_1278 = PHI <pos_556(107), best_scoring_move_1238(106)>
  # right_2 = PHI <right_123(107), right_123(106)>

Which then allows PHI-OPT to convert this code into two conditional moves.

 */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "basic-block.h"
#include "gimple-pretty-print.h"
#include "tree-inline.h"
#include "hash-table.h"
#include "tree-ssa-alias.h"
#include "internal-fn.h"
#include "gimple-fold.h"
#include "tree-eh.h"
#include "gimple-expr.h"
#include "is-a.h"
#include "gimple.h"
#include "gimplify.h"
#include "gimple-iterator.h"
#include "gimplify-me.h"
#include "gimple-ssa.h"
#include "tree-cfg.h"
#include "tree-phinodes.h"
#include "ssa-iterators.h"
#include "stringpool.h"
#include "tree-ssanames.h"
#include "tree-ssa-loop.h"
#include "tree-into-ssa.h"
#include "expr.h"
#include "tree-dfa.h"
#include "tree-ssa.h"
#include "tree-iterator.h"
#include "alloc-pool.h"
#include "obstack.h"
#include "tree-pass.h"
#include "flags.h"
#include "langhooks.h"
#include "cfgloop.h"
#include "tree-ssa-sccvn.h"
#include "tree-scalar-evolution.h"
#include "params.h"
#include "dbgcnt.h"
#include "domwalk.h"
#include "ipa-prop.h"
#include "tree-ssa-propagate.h"
#include "gimple-pretty-print.h"

static bool moved = false;

/* Merge possible common code from BB1 and BB2 into BB0. */

static void
merge_code_bb (basic_block bb0, basic_block bb1, basic_block bb2)
{
  gimple_stmt_iterator gsi1 = gsi_start_bb (bb1);
  gimple_stmt_iterator gsi2 = gsi_start_bb (bb2);
  gimple stmt1;
  gimple stmt2;
  tree t1, t2;
  enum tree_code code;

  if (gsi_end_p (gsi1) || gsi_end_p (gsi2))
    return;

  stmt1 = gsi_stmt (gsi1);
  stmt2 = gsi_stmt (gsi2);

  /* Skip over debugging statements since they are not real statements. */
  while (is_gimple_debug (stmt1))
    {
      gsi_next (&gsi1);
      if (gsi_end_p (gsi1))
	return;
      stmt1 = gsi_stmt (gsi1);
    }

  while (is_gimple_debug (stmt2))
    {
      gsi_next (&gsi2);
      if (gsi_end_p (gsi2))
	return;
      stmt2 = gsi_stmt (gsi2);
    }

  if (stmt1 == NULL || stmt2 == NULL)
    return;

  /* Neither statements can throw internal.
     FIXME: handle this case too so we can move loads when
     -fnon-call-exceptions is enabled. */
  if (stmt_can_throw_internal (stmt1) || stmt_can_throw_internal (stmt2))
    return;

  /* FIXME: handle calls too. */
  if (!is_gimple_assign (stmt1) || !is_gimple_assign (stmt2))
    return;

  /* Don't handle stores. */
  if (TREE_CODE (gimple_assign_lhs (stmt1)) != SSA_NAME
      || TREE_CODE (gimple_assign_lhs (stmt2)) != SSA_NAME)
    return;

  /* Don't handle code differences. */
  code = gimple_assign_rhs_code (stmt1);
  if (code != gimple_assign_rhs_code (stmt2))
    return;

  switch (get_gimple_rhs_class (code))
    {
      default:
	return;
      case GIMPLE_TERNARY_RHS:
	t1 = gimple_assign_rhs3 (stmt1);
	t2 = gimple_assign_rhs3 (stmt2);
	if (!operand_equal_p (t1, t2, 0))
	  return;
	/* FALLTHROUGH */
      case GIMPLE_BINARY_RHS:
	t1 = gimple_assign_rhs2 (stmt1);
	t2 = gimple_assign_rhs2 (stmt2);
	if (!operand_equal_p (t1, t2, 0))
	  return;
	/* FALLTHROUGH */
      case GIMPLE_UNARY_RHS:
      case GIMPLE_SINGLE_RHS:
	t1 = gimple_assign_rhs1 (stmt1);
	t2 = gimple_assign_rhs1 (stmt2);
	if (!operand_equal_p (t1, t2, 0))
	  return;
    }
  t1 = gimple_assign_lhs (stmt1);
  t2 = gimple_assign_lhs (stmt2);

  /* If the left hand side of the t1 is a variable decl and has the ignore bit,
     then move statement 2 instead of statement 1. */

    if (((SSA_NAME_VAR (t1) == NULL
          || (TREE_CODE (SSA_NAME_VAR (t1)) == VAR_DECL
             && DECL_IGNORED_P (SSA_NAME_VAR (t1))))
       && (SSA_NAME_VAR (t2) 
            && TREE_CODE (SSA_NAME_VAR (t2)) == VAR_DECL
            && !DECL_IGNORED_P (SSA_NAME_VAR (t2))))
       || !may_propagate_copy (t2, t1))
    {
      tree t = t1;
      gimple_stmt_iterator gsi = gsi1;
      gimple stmt = stmt1;
      t1 = t2;
      t2 = t;
      gsi1 = gsi2;
      gsi2 = gsi;
      stmt1 = stmt2;
      stmt2 = stmt;
    }

  if (dump_file && (dump_flags & TDF_DETAILS) != 0)
    {
      fprintf (dump_file, "Merging the following two statements.\n");
      print_gimple_stmt (dump_file, stmt1, 0, 0);
      print_gimple_stmt (dump_file, stmt2, 0, 0);
    }
  /* Move one of the statements to the end of the common basic block. */
  gsi_move_to_bb_end (&gsi1, bb0);
  /* Mark the vuses if there is any for renaming. */
  if (gimple_vuse (stmt1) != NULL
      && TREE_CODE (gimple_vuse (stmt1)) == SSA_NAME)
    mark_virtual_operand_for_renaming (gimple_vuse (stmt1));
  if (gimple_vuse (stmt2) != NULL
      && TREE_CODE (gimple_vuse (stmt2)) == SSA_NAME)
    mark_virtual_operand_for_renaming (gimple_vuse (stmt2));
  update_stmt (stmt1);

  moved = true;
  if (may_propagate_copy (t2, t1))
    {
      /* Replace the current usage of t2 with t1. */
      replace_uses_by (t2, t1);
      /* Remove stmt2. */
      gsi_remove (&gsi2, true);
      release_defs (stmt2);
      /* Try again to see if we have another common statement. */
      merge_code_bb (bb0, bb1, bb2);
    }
}

namespace {

const pass_data pass_data_merge_common_code =
{
  GIMPLE_PASS, /* type */
  "merge_code", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  true, /* has_execute */
  TV_TREE_SSA_MERGE_COMMON, /* tv_id */
  ( PROP_cfg | PROP_ssa ), /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_merge_common_code : public gimple_opt_pass
{
public:
  pass_merge_common_code (gcc::context *ctxt) 
    : gimple_opt_pass (pass_data_merge_common_code, ctxt)
  {}

  /* opt_pass methods: */
  opt_pass * clone () { return new pass_merge_common_code (m_ctxt); }
  virtual unsigned int execute (function*);

};  // class pass_merge_common_code

} // anon namespace

unsigned int
pass_merge_common_code::execute (function*)
{
  basic_block bb;
  auto_vec<basic_block, 20> queue;
  moved = false;


  /* Find all the basic blocks which match if(a) { B } else { C }. */
  FOR_EACH_BB_FN (bb, cfun)
    {
      gimple cond_stmt;
      basic_block bb1, bb2;
      edge e1, e2;

      cond_stmt = last_stmt (bb);
      /* Check to see if the last statement is a GIMPLE_COND.  */
      if (!cond_stmt
          || gimple_code (cond_stmt) != GIMPLE_COND)
        continue;

      /* Only two successors for the basic block. */
      if (EDGE_COUNT (bb->succs) != 2)
	continue;

      e1 = EDGE_SUCC (bb, 0);
      bb1 = e1->dest;
      e2 = EDGE_SUCC (bb, 1);
      bb2 = e2->dest;

      /* We cannot do the optimization on abnormal edges.  */
      if ((e1->flags & EDGE_ABNORMAL) != 0
          || (e2->flags & EDGE_ABNORMAL) != 0)
       continue;

      /* We don't handle the case where the inner basic blocks are
	 reachable  */
      if (EDGE_COUNT (bb1->preds) != 1
	  || EDGE_COUNT (bb2->preds) != 1)
	continue;

      queue.safe_push (bb);
    }

  while (queue.length () > 0)
    {
      basic_block bb0, bb1, bb2;

      /* Pull the basic block off the worklist. */
      bb0 = queue.pop ();
      bb1 = EDGE_SUCC (bb0, 0)->dest;
      bb2 = EDGE_SUCC (bb0, 1)->dest;
      merge_code_bb (bb0, bb1, bb2);
    }

  queue.release ();
  if (moved)
    return TODO_cleanup_cfg | TODO_update_ssa_only_virtuals;
  return 0;
}


gimple_opt_pass *
make_pass_merge_common_code (gcc::context *ctxt)
{
  return new pass_merge_common_code (ctxt);
}

