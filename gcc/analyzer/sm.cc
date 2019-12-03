/* Modeling API uses and misuses via state machines.
   Copyright (C) 2019 Free Software Foundation, Inc.
   Contributed by David Malcolm <dmalcolm@redhat.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "gcc-plugin.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "gimple.h"
#include "analyzer/analyzer.h"
#include "analyzer/sm.h"

////////////////////////////////////////////////////////////////////////////

/* If STMT is an assignment to zero, return the LHS.  */

tree
is_zero_assignment (const gimple *stmt)
{
  const gassign *assign_stmt = dyn_cast <const gassign *> (stmt);
  if (!assign_stmt)
    return NULL_TREE;

  enum tree_code op = gimple_assign_rhs_code (assign_stmt);
  if (op != INTEGER_CST)
    return NULL_TREE;

  if (!zerop (gimple_assign_rhs1 (assign_stmt)))
    return NULL_TREE;

  return gimple_assign_lhs (assign_stmt);
}

/* If COND_STMT is a comparison against zero of the form (LHS OP 0),
   return true and write what's being compared to *OUT_LHS and the kind of
   the comparison to *OUT_OP.  */

bool
is_comparison_against_zero (const gcond *cond_stmt,
			    tree *out_lhs, enum tree_code *out_op)
{
  enum tree_code op = gimple_cond_code (cond_stmt);
  tree lhs = gimple_cond_lhs (cond_stmt);
  tree rhs = gimple_cond_rhs (cond_stmt);
  if (!zerop (rhs))
    return false;
  // TODO: make it symmetric?

  switch (op)
    {
    case NE_EXPR:
    case EQ_EXPR:
      *out_lhs = lhs;
      *out_op = op;
      return true;

    default:
      return false;
    }
}

bool
any_pointer_p (tree var)
{
  if (TREE_CODE (TREE_TYPE (var)) != POINTER_TYPE)
    return false;

  return true;
}

////////////////////////////////////////////////////////////////////////////

state_machine::state_t
state_machine::add_state (const char *name)
{
  m_state_names.safe_push (name);
  return m_state_names.length () - 1;
}

const char *
state_machine::get_state_name (state_t s) const
{
  return m_state_names[s];
}

void
state_machine::validate (state_t s) const
{
  gcc_assert (s < m_state_names.length ());
}

////////////////////////////////////////////////////////////////////////////

void
make_checkers (auto_delete_vec <state_machine> &out, logger *logger)
{
  out.safe_push (make_malloc_state_machine (logger));
  out.safe_push (make_fileptr_state_machine (logger));
  out.safe_push (make_taint_state_machine (logger));
  out.safe_push (make_sensitive_state_machine (logger));
  out.safe_push (make_signal_state_machine (logger));

  /* We only attempt to run the pattern tests if it might have been manually
     enabled (for DejaGnu purposes).  */
  if (flag_analyzer_checker)
    out.safe_push (make_pattern_test_state_machine (logger));

  if (flag_analyzer_checker)
    {
      unsigned read_index, write_index;
      state_machine **sm;

      /* TODO: this leaks the machines
	 Would be nice to log the things that were removed.  */
      VEC_ORDERED_REMOVE_IF (out, read_index, write_index, sm,
			     0 != strcmp (flag_analyzer_checker,
					  (*sm)->get_name ()));
    }
}
