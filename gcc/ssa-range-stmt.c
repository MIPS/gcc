/* On-demand ssa range statement summary.
   Copyright (C) 2017 Free Software Foundation, Inc.
   Contributed by Andrew MacLeod <amacleod@redhat.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "insn-codes.h"
#include "rtl.h"
#include "tree.h"
#include "gimple.h"
#include "cfghooks.h"
#include "tree-pass.h"
#include "ssa.h"
#include "optabs-tree.h"
#include "gimple-pretty-print.h"
#include "diagnostic-core.h"
#include "flags.h"
#include "fold-const.h"
#include "stor-layout.h"
#include "calls.h"
#include "cfganal.h"
#include "gimple-fold.h"
#include "tree-eh.h"
#include "gimple-iterator.h"
#include "gimple-walk.h"
#include "tree-cfg.h"
#include "wide-int.h"
#include "ssa-range-stmt.h"

/* The defintion dependency vectors are used to indicate which ssa_names may 
   require range info in order to fully resolve the range. 

   d_7 = a_3 + 2
   d_8 = d_7 * 2
   c_9 = d_8 < b_6
   if (c_9)

   when determining range information for the if, we require knowledge of what
   c_9 is built from.  c_9 can utilize range information from both d_8 and b_6
   d_8 in turn can use info from d_7, and d_7 uses info from a_3.

   Range can also be determined for any of these components by adjusting 
   the operations. ie, on the TRUE side of the branch, we can also view the
   expression as :
   c_9 = (d_7 * 2) < b_6,   --> c_9 = (d_7 < b_6 / 2)
   c_9 = (a_3 + 2) < b_6 / 2   -->  c_9 = (a_3 < (b_6 / 2) - 2)

   The definition dependency map will provide a list of the ssa_names which 
   are used in the contruction of a given value, but only for those which
   we have the ablity to do thes transformations.  Ie, have entries in the
   range_operator_table.

   For the above example, the def_Dep vectors for each name will look like:
   a_3 ->  NULL
   d_7 -> a_3
   d_8 -> a_3, d_7
   c_9 -> d_8, a_3, d_7, b_6
   b_6 -> NULL

   with a quick check of an ssa_name we can determine:
     a) Any name in a branch operand provides the list of names which that
	branch can generate range info for.
     b) which operand to look for a specific name, ie:
     	d_8 < b_6 , if we are looking for range in terms of a_3, the bit for
	a_3 is set in d_8, so thats the operand to follow calling the
	op_adjust routines to get the range adjustment in terms of a_3
     c) Which ssa-name's range info could be useful to refine other ranges.

   We also limit the depth of these to some value from the actual branch so
   we dont look too far back and involve too many names.  

   Names are reprocessed as they may be used at differfent depths from different
   branches, and this ensures we alwasy get at least that depth from each
   branch.  */





irange_operator *
range_stmt::handler ()
{
  return irange_op_handler (code);
}

/* Intialize the state based on the operands to the expression.  */
enum range_stmt_state
range_stmt::determine_state (tree t1, tree t2)
{
  enum range_stmt_state st = RS_INV;
  if (!t2 || TYPE_P (t2))
    {
      /* Check for unary teration cases.  */
      if (TREE_CODE (t1) == INTEGER_CST && !TREE_OVERFLOW (t1))
        st = RS_I;
      else
        if (TREE_CODE (t1) == SSA_NAME)
	  st = RS_S;
    }
  else
    {
      /* Binary teration cases.  */
      if (TREE_CODE (t1) == INTEGER_CST && !TREE_OVERFLOW (t1))
        {
	  if (TREE_CODE (t2) == INTEGER_CST && !TREE_OVERFLOW (t2))
	    st = RS_II;
	  else
	    if (TREE_CODE (t2) == SSA_NAME)
	      st = RS_IS;
	}
      else
        if (TREE_CODE (t1) == SSA_NAME)
	  {
	    if (TREE_CODE (t2) == INTEGER_CST && !TREE_OVERFLOW (t2))
	      st = RS_SI;
	    else
	      if (TREE_CODE (t2) == SSA_NAME)
		st = RS_SS;
	  }
    }
  return st;
}

/* Build a range node from a stmt, if it possible.  */
void
range_stmt::from_stmt (gimple *s)
{
  state = RS_INV;  /* Assume invalid state.  */
  switch (gimple_code (s))
    {
      case GIMPLE_COND:
        {
	  gcond *g = as_a <gcond *> (s);
	  code  = gimple_cond_code (g);
	  if (irange_op_handler (code))
	    {
	      op1 = gimple_cond_lhs (g);
	      op2 = gimple_cond_rhs (g);
	      state = determine_state (op1, op2);
	    }
	  break;
	}
      case GIMPLE_ASSIGN:
	{
	  gassign *g = as_a <gassign *> (s);
	  code = gimple_assign_rhs_code (g);
	  if (irange_op_handler (code))
	    {
	      op1 = gimple_assign_rhs1 (g);
	      if (get_gimple_rhs_class (code) == GIMPLE_BINARY_RHS)
		op2 = gimple_assign_rhs2 (g);
	      else
	        /* Unary operations often need the type, think casting.  */
		if (get_gimple_rhs_class (code) == GIMPLE_UNARY_RHS)
		  op2 = TREE_TYPE (op1);
		else
		  op2 = NULL;
	      state = determine_state (op1, op2);
	    }
	  break;
	}

      default:
        break;
    }
}


range_stmt::range_stmt ()
{
  state = RS_INV;
}

range_stmt::range_stmt (gimple *s)
{
  from_stmt (s);
}

range_stmt&
range_stmt::operator= (gimple *s)
{
  from_stmt (s);
  return *this;
}

bool
range_stmt::is_relational()
{
  if (code >= TRUTH_ANDIF_EXPR && code <= NE_EXPR)
    return true;
  return false;
}

/* THis function will attempt to resolve the expression to a constant. 
   If the expression can be resolved, true is returned, and the range is
   returned in RES.
   If one or more SSA_NAME's needs to be resolved to a range first, their
   values are passed in as parameters.  It is an error to attempt to resolve
   an ssa_name with no range.  */

bool
range_stmt::fold (irange &res, irange* value1, irange* value2)
{
  bool result = false;
  irange r1, r2;
  irange_operator *handler = irange_op_handler (code);
  
  gcc_assert (handler != NULL);

  switch (state)
    {
      case RS_I:
        gcc_assert (!value1 && !value2);
	r1.set_range (TREE_TYPE (operand1 ()), operand1 (), operand1 ());
	r2.clear ();
	result = handler->fold_range (res, r1, r2);

	break;

      case RS_II:
        gcc_assert (!value1 && !value2);
	r1.set_range (TREE_TYPE (operand1 ()), operand1 (), operand1 ());
	r2.set_range (TREE_TYPE (operand2 ()), operand2 (), operand2 ());
	result = handler->fold_range (res, r1, r2);
	break;

      case RS_S:
        gcc_assert (value1 && !value2);
	r2.clear ();
	result = handler->fold_range (res, *value1, r2);
	break;

      case RS_SI:
        gcc_assert (value1 && !value2);
	r2.set_range (TREE_TYPE (operand2 ()), operand2 (), operand2 ());
	result = handler->fold_range(res, *value1, r2);
	break;

      case RS_IS:
	/* Allows calling with (NULL, range) in addition to (range).  */
        /* One param or the other needs to be non-null.  */
        gcc_assert (!value1 != !value2);
	r1.set_range (TREE_TYPE (operand1 ()), operand1 (), operand1 ());
	if (value1)
	  result = handler->fold_range (res, r1, *value1);
	else
	  result = handler->fold_range (res, r1, *value2);
	break;

      case RS_SS:
        gcc_assert (value1 && value2);
	result = handler->fold_range(res, *value1, *value2);
	break;
  
      default:
        gcc_unreachable ();
	break;
    }

  return result;
}


/* Resolve the expression using whatever we know about the global state of
   any SSA_NAMEs involved.  This will ensure we can always get a range,
   if it is possible. 
   Return TRUE if a range is calculated. */
   
bool
range_stmt::fold (irange& r)
{
  tree n1, n2;
  irange r1, r2;
  irange_p r1p = NULL, r2p = NULL;
  

  if (!ssa_required (&n1, &n2))
    return false;

  if (n1)
    {
      r1 = n1;
      r1p = &r1;
    }

  if (n2)
    {
      r2 = n2;
      r2p = &r2;
    }

  return fold (r, r1p, r2p);
}


/* Return the ssa operands as a list.  NAME1 will be set it there are any
   ssa_names. NAME2 will only be set if there are 2 ssa_names.  */
bool
range_stmt::ssa_required (tree *name1, tree *name2)
{
  switch (state)
    {
      case RS_S:
      case RS_SI:
        *name1 = op1;
	*name2 = NULL_TREE;
	return true;

      case RS_IS:
        *name1 = NULL_TREE;
	*name2 = op2;
	return true;

      case RS_SS:
        *name1 = op1;
	*name2 = op2;
	return true;

      default:
        return false;
    }
  gcc_unreachable ();
}



void
range_stmt::dump (FILE *f)
{
  switch (state)
  {
    case RS_INV:
      fprintf (f, "RS_INV  ");
      return;

    case RS_I:
      fprintf (f, "RS_I  ");
      irange_op_handler (code)->dump (f);
      break;
    case RS_S:
      fprintf (f, "RS_S  ");
      irange_op_handler (code)->dump (f);
      break;
    case RS_II:
      fprintf (f, "RS_II ");
      break;
    case RS_SI:
      fprintf (f, "RS_SI ");
      break;
    case RS_IS:
      fprintf (f, "RS_IS ");
      break;
    case RS_SS:
      fprintf (f, "RS_SS ");
      break;
    default:
      gcc_unreachable ();
  }

  if (op2 && (state == RS_I || state == RS_S))
    {
      irange_op_handler (code)->dump (f);
      fprintf (f, " (");
      print_generic_expr (f, op2, TDF_SLIM);
      fprintf (f, ") ");
    }

  print_generic_expr (stderr, op1, TDF_SLIM);

  if (state != RS_I && state != RS_S)
    {
      irange_op_handler (code)->dump (f);
      print_generic_expr (f, op2, TDF_SLIM);
    }
}



