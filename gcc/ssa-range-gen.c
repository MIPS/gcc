/* On-demand ssa range generator.
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
#include "ssa-range-gen.h"
#include "ssa-range-stmt.h"


gori::gori () : range_generator (def_chain)
{
  gori_map.create (0);
  gori_map.safe_grow_cleared (last_basic_block_for_fn (cfun));
}

/* Is the last stmt in a block interesting to look at for range info.  */

gimple *
gori::last_stmt_gori (basic_block bb)
{
  gimple *stmt;

  stmt = last_stmt (bb);
  if (stmt && gimple_code (stmt) == GIMPLE_COND)
    return stmt;
  return NULL;
}

void
gori::build (basic_block bb)
{
  gimple *stmt;

  /* Non-NULL means its already been processed.  */
  if (gori_map[bb->index])
    return;

  gori_map[bb->index] = BITMAP_ALLOC (NULL);

  /* Look for a branch of some sort at the end of the block, and build the
     GORI map and definition dependcies for it.  */
  stmt = last_stmt_gori (bb);
  if (stmt)
    {
      range_stmt rn (stmt);
      if (rn.valid())
	{
	  tree ssa1,ssa2;
	  if (rn.ssa_required (&ssa1, &ssa2))
	    {
	      if (ssa1)
	        {
		  bitmap_copy (gori_map[bb->index],
			       def_chain[SSA_NAME_VERSION (ssa1)]);
		  bitmap_set_bit (gori_map[bb->index], SSA_NAME_VERSION (ssa1));
		}
	      if (ssa2)
	        {
		  bitmap_ior_into (gori_map[bb->index],
				   def_chain[SSA_NAME_VERSION (ssa2)]);
		  bitmap_set_bit (gori_map[bb->index], SSA_NAME_VERSION (ssa2));
		}
	      return;
	    }
	}
    }
}


void
gori::build ()
{
  basic_block bb;

  FOR_EACH_BB_FN (bb, cfun)
    {
      build (bb);
    }
}

void
gori::dump (FILE *f)
{
  basic_block bb;
  bitmap_iterator bi;
  unsigned y;

  if (!f)
    return;

  fprintf (f, "\nDUMPING DEF_CHAIN\n");
  def_chain.dump (f);

  fprintf (f, "\nDUMPING GORI MAP\n");
  FOR_EACH_BB_FN (bb, cfun)
    {
      if (gori_map[bb->index])
        {
          fprintf (f, "basic block %3d  : ", bb->index);
	  EXECUTE_IF_SET_IN_BITMAP (gori_map[bb->index], 0, y, bi)
	    {
	      print_generic_expr (f, ssa_name (y), TDF_SLIM);
	      fprintf (f, "  ");
	    }
	  fprintf (f, "\n");
	}
    }
  fprintf (f, "\n");

}

bool 
gori::remove_from_gori_map (basic_block bb, tree name)
{
  bitmap_clear_bit (gori_map[bb->index], SSA_NAME_VERSION (name));
  return false;
}


/* Name is not directly mentioned in the gori map, but if one of the
   compnents it is built from are, we can derive the value from that. ie
	 a_3 = c_2 * 2
	 b_6 = a_3 + 5
	 if (a_3 > 10)
   The GORI map will only have c_2 and a_3 since they comprise the
   calculation of a_3.   b_6 will not be there as the only way to know
   that b_6 can be calculated from a_3 would be to visit all the uses of
   a_3 and see whether it is used to help define b_6.
   Far easier now that b_6 is requested would be to simply check now
   if a_3 is used to construct b_6. If it is get the expression for a_3
   and adjust it to b_6.  */
   
bool
gori::get_derived_range_stmt (range_stmt& stmt, tree name, basic_block bb)
{
  gimple *s;
  tree n1,n2;
  unsigned name_v = SSA_NAME_VERSION (name);

  s = last_stmt_gori (bb);
  gcc_assert (s);

  stmt = s;
  if (!stmt.valid ())
    return false;

  if (!stmt.ssa_required (&n1, &n2))
    return false;

  if (n1 && !bitmap_bit_p (def_chain[name_v], SSA_NAME_VERSION (n1)))
    n1 = NULL_TREE;
    
  if (n2 && !bitmap_bit_p (def_chain[name_v], SSA_NAME_VERSION (n2)))
    n2 = NULL_TREE;
  
  /* Non-null n1 or n2 indicates it is used in the calculating name.  */

  /* Used on both sides too complicated.  */
  if (n1 && n2)
    return false;

  /* Well we aren't actually DOING it yet... :-)  */
  return false;
}



bool
gori::range_p (basic_block bb, tree name)
{
  gcc_checking_assert (TREE_CODE (name) == SSA_NAME);

  if (!gori_map[bb->index])
    build (bb);

  return bitmap_bit_p (gori_map[bb->index], SSA_NAME_VERSION (name));
}


/* Known range on an edge.  */
bool
gori::range_on_edge (irange& r, tree name, edge e)
{
  gimple *stmt;
  basic_block bb = e->src;

  gcc_checking_assert (TREE_CODE (name) == SSA_NAME);

  if (!INTEGRAL_TYPE_P (TREE_TYPE (name)) && !POINTER_TYPE_P (TREE_TYPE (name)))
    return false;

  if (!range_p (bb, name))
    return false;

  stmt = last_stmt_gori (bb);
  gcc_assert (stmt);

  if (e->flags & EDGE_TRUE_VALUE)
    {
      irange bool_true (boolean_type_node,  1 , 1);
      return range_generator.get_range_from_stmt (stmt, r, name, bool_true);
    }

  if (e->flags & EDGE_FALSE_VALUE)
    {
      irange bool_false (boolean_type_node,  0 , 0);
      return range_generator.get_range_from_stmt (stmt, r, name, bool_false);
    }

  return false;
}




void
gori::exercise (FILE *output)
{

  basic_block bb;
  irange range;
  
  if (output)
    dump (output);

  FOR_EACH_BB_FN (bb, cfun)
    {
      edge_iterator ei;
      edge e;
      bool printed = false;
      FOR_EACH_EDGE (e, ei, bb->succs)
        {
	  unsigned x;
	  for (x = 1; x < num_ssa_names; x++)
	    {
	      tree name = ssa_name (x);
	      if (name && range_p (bb, name))
		{
		  if (range_on_edge (range, name, e))
		    {
		      if (output)
			{
			  printed = true;
			  fprintf (output, "BB%3d: ", bb->index);
			  if (e->flags & EDGE_TRUE_VALUE)
			    fprintf (output, " T: ");
			  else if (e->flags & EDGE_FALSE_VALUE)
			    fprintf (output, " F: ");
			  print_generic_expr (output, name, TDF_SLIM);
			  fprintf(output, "  \t");
			  range.dump(output);
			  fprintf(output, "\n");
			}
		    }
		}
	    }
	}
      if (printed)
        fprintf (output, "\n");

    }
    
}


#if ANDREW_SAID_WRONG_CLASS

/* Known Range at the beginning of a statment.  */
bool
gori::get_range (irange& r, tree name, gimple *s)
{
  gimple *def_stmt = SSA_NAME_DEF_STMT (name);
  /* If def is in the same block as stmt, then only the def provides range.  */
  if (def_stmt && gimple_bb (def_stmt) == gimple_bb (s))
    {
      range_stmt rn (def_stmt);
      /* IF it isnt a definition  that is understood, we know nothing.  */
      if (!rn.valid())
        return false;
      
      /* We wont try to find dynamic ranges for anything in the def as yet.
         A mechanism is neded to prevent infinite recursion when a cycle
	 of uisage occurs.  */
      return rn.fold (r);
    }
  /* The defintion is not in this block, so whatever range is on entry.  */
  return get_range (r, name, gimple_bb (s));
}



/* Known range on entry to a basic block.  */
bool
gori::get_range (irange& r, tree name, basic_block bb)
{
  edge_iterator ei;
  edge e;
  irange er;

  r.clear ();
  gcc_checking_assert (TREE_CODE (name) == SSA_NAME);
  /* Union all the ranges on the incoming edges.  */
  FOR_EACH_EDGE (e, ei, bb->preds)
    {
      /* If we can't get a range on an edge, it defaults range_for_type
         and union of that with anything will be range_for_type. 
	 Simply report we can't get a range.  */
      if (!get_range (er, name, e))
        return false;
      r.Union (er);
    }

  /* If range isn't supported, don't bother.  */
  er.set_range_for_type (TREE_TYPE (name));

  /* If the result is not valid or is range_for_type, report no range.  */
   if (r == er)
     return false;

  return true;
}

#endif

/*  ---------------------------------------------------------------------  */


bool
ranger::combine_range (range_stmt& stmt, irange& r, tree name,
		       const irange& lhs, bool op1_in_chain, bool op2_in_chain)
{
  tree op1, op2;
  irange r1, r2;
  irange op1_range, op2_range;

  irange op1_true, op1_false, op2_true, op2_false;

  /* Look for boolean and/or condition.  */
  switch (stmt.get_code ())
    {
      case TRUTH_AND_EXPR:
      case TRUTH_OR_EXPR:
        break;

      case BIT_AND_EXPR:
      case BIT_IOR_EXPR:
        if (!types_compatible_p (TREE_TYPE (stmt.operand1 ()),
				 boolean_type_node))
	  return false;
	break;

      default:
        return false;
    }


  if (dump_file)
    {
      fprintf (dump_file, "\nIn Combine_range for ");
      print_gimple_stmt (dump_file, stmt.get_gimple (), 0 ,0);
    }
  /* If the LHS can be TRUE OR FALSE, then we cant really tell anything.  */
  if (!wi::eq_p (lhs.lower_bound(), lhs.upper_bound()))
    {
      r.set_range_for_type (TREE_TYPE (name));
      if (dump_file)
	fprintf (dump_file, " : LHS can be true or false, know nothing.\n");
      return true;
    }

  op1 = stmt.operand1 ();
  op2 = stmt.operand2 ();

  irange bool_zero (boolean_type_node, 0, 0);
  irange bool_one (boolean_type_node, 1, 1);

  /* The false path is not always a simple inversion of the true side.
     Calulate ranges for true and false on both sides. */

  if (op1_in_chain)
    {
      get_range_from_stmt (SSA_NAME_DEF_STMT (op1), op1_true, name, bool_one);
      get_range_from_stmt (SSA_NAME_DEF_STMT (op1), op1_false, name, bool_zero);
    }
  else
    {
      get_operand_range (op1_true, TREE_TYPE (name));
      get_operand_range (op1_false, TREE_TYPE (name));
    }

  if (op2_in_chain)
    {
      get_range_from_stmt (SSA_NAME_DEF_STMT (op2), op2_true, name, bool_one);
      get_range_from_stmt (SSA_NAME_DEF_STMT (op2), op2_false, name, bool_zero);
    }
  else
    {
      get_operand_range (op2_true, TREE_TYPE (name));
      get_operand_range (op2_false, TREE_TYPE (name));
    }

  if (dump_file)
    {
      fprintf (dump_file, "  combining following 4 ranges:\n");
      fprintf (dump_file, "op1_true : ");
      op1_true.dump (dump_file);
      fprintf (dump_file, "op1_false : ");
      op1_false.dump (dump_file);
      fprintf (dump_file, "op2_true : ");
      op2_true.dump (dump_file);
      fprintf (dump_file, "op2_false : ");
      op2_false.dump (dump_file);
    }
  /* Now combine based on the result.  */
  switch (stmt.get_code ())
    {

      /* A logical AND of two ranges is executed when we are walking forward
	 with ranges that have been determined.   x_8 is an unsigned char.
	       b_1 = x_8 < 20
	       b_2 = x_8 > 5
	       c_2 = b_1 && b_2
	 if we are looking for the range of x_8, the ranges on each side 
	 will be:   b_1 carries x_8 = [0, 19],   b_2 carries [6, 255]
	 the result of the AND is the intersection of the 2 ranges, [6, 255]. */
      case TRUTH_AND_EXPR:
      case BIT_AND_EXPR:
        if (lhs != bool_zero)
	  r = irange_intersect (op1_true, op2_true);
	else
	  {
	    irange ff = irange_intersect (op1_false, op2_false);
	    irange tf = irange_intersect (op1_true, op2_false);
	    irange ft = irange_intersect (op1_false, op2_true);
	    r = irange_union (ff, tf);
	    r.union_ (ft);
	  }
        break;

      /* A logical OR of two ranges is executed when we are walking forward with
	 ranges that have been determined.   x_8 is an unsigned char.
	       b_1 = x_8 > 20
	       b_2 = x_8 < 5
	       c_2 = b_1 || b_2
	 if we are looking for the range of x_8, the ranges on each side
	 will be:   b_1 carries x_8 = [21, 255],   b_2 carries [0, 4]
	 the result of the OR is the union_ of the 2 ranges, [0,4][21,255].  */
      case TRUTH_OR_EXPR:
      case BIT_IOR_EXPR:
        if (lhs == bool_zero)
	  r = irange_intersect (op1_false, op2_false);
	else
	  {
	    irange tt = irange_intersect (op1_true, op2_true);
	    irange tf = irange_intersect (op1_true, op2_false);
	    irange ft = irange_intersect (op1_false, op2_true);
	    r = irange_union (tt, tf);
	    r.union_ (ft);
	  }
	break;

      default:
        gcc_unreachable ();
    }

  if (dump_file)
    {
      fprintf (dump_file, "result range is ");
      r.dump (dump_file);
    }
  return true;
}


bool
ranger::get_operand_range (irange& r, tree op)
{
  /* This check allows unary operations to be handled without having to 
     make an explicit check for the existence of a second operand.  */
  if (!op)
    return false;

  if (TREE_CODE (op) == INTEGER_CST)
    {
      r.set_range (TREE_TYPE (op), op, op);
      return true;
    }
  else
    if (TREE_CODE (op) == SSA_NAME)
      {
	/* Eventually we may go look for an on-demand range... */
	/* But at least should look for what we currently know. */
	r.set_range_for_type (TREE_TYPE (op));
	return true;
      }

  /* Unary expressions often set the type for the operand, get that range.  */
  if (TYPE_P (op))
    r.set_range_for_type (op);
  else
    r.set_range_for_type (TREE_TYPE (op));
  return true;
}

/* Given the expression in STMT, return an evaluation in R for NAME.
   Returning false means the name being looked for is NOT resolvable, and
   can be removed from the GORI map to qavoid future searches.  */
bool
ranger::get_range (range_stmt& stmt, irange& r, tree name,
			     const irange& lhs)
{
  range_stmt op_stmt;
  irange op1_range, op2_range;
  tree op1, op2;
  bool op1_in_chain, op2_in_chain;

  op1 = stmt.operand1 ();
  op2 = stmt.operand2 ();

  if (op1 == name)
    { 
      if (get_operand_range (op2_range, op2))
	return stmt.op1_irange (r, lhs, op2_range, dump_file);
      else
        return false;
    }

  if (op2 == name)
    {
      if (get_operand_range (op1_range, op1))
	return stmt.op2_irange (r, lhs, op1_range, dump_file);
      else
        return false;
    }

  /* Reaching this point means NAME is not in this stmt, but one of the
     names in it ought to be derived from it.  */
  op1_in_chain = def_chain.in_chain_p (op1, name);
  op2_in_chain = def_chain.in_chain_p (op2, name);

  /* If neither operand is derived, then this stmt tells us nothing. */
  if (!op1_in_chain && !op2_in_chain)
    return false;
  
  /* Check for boolean cases which require developing ranges and combining.  */
  if (combine_range (stmt, r, name, lhs, op1_in_chain, op2_in_chain))
    return true;

  /* Don't look thru expressions with more than one in_chain argument.  */
  if (op1_in_chain && op2_in_chain)
    return false;

  if (op1_in_chain)
    {
      get_operand_range (op2_range, op2);
      stmt.op1_irange (op1_range, lhs, op2_range, dump_file);
      return get_range_from_stmt (SSA_NAME_DEF_STMT (op1), r, name, op1_range);
    }

  get_operand_range (op1_range, op1);
  stmt.op2_irange (op2_range, lhs, op1_range, dump_file);
  return get_range_from_stmt (SSA_NAME_DEF_STMT (op2), r, name, op2_range);
}
 
/* Given the expression in STMT, return an evaluation in R for NAME. */
bool
ranger::get_range_from_stmt (gimple *stmt, irange& r, tree name,
			     const irange& lhs)
{
  range_stmt rn;
  rn = stmt;

  /* If it isnt an expression that is understood, we know nothing.  */
  if (!rn.valid())
    return false;

  /* If the lhs has no range, ie , it cannot be executed, then the query
     has no range either.  */
  if (lhs.empty_p ())
    {
      r = lhs;
      return true;
    }

  return get_range (rn, r, name, lhs);
}
