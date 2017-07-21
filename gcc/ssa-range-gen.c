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
gori::get_range (irange& r, tree name, edge e)
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
    return range_generator.get_range_from_stmt (stmt, r, name,
						SRO_NON_ZERO_DEF);

  if (e->flags & EDGE_FALSE_VALUE)
    return range_generator.get_range_from_stmt (stmt, r, name, SRO_ZERO_DEF);

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
		  if (get_range (range, name, e))
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
  er.set_range (TREE_TYPE (name));

  /* If the result is not valid or is range_for_type, report no range.  */
   if (r == er)
     return false;

  return true;
}

#endif

/*  ---------------------------------------------------------------------  */


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
	r = TREE_TYPE (op);
	return true;
      }

  /* Unary expressions often set the type for the operand, get that range.  */
  if (TYPE_P (op))
    r.set_range (op);
  else
    r.set_range (TREE_TYPE (op));
  return true;
}


/*
     b_6 = a_3 + 2
     if (b_6 < 20)
   adjust ((b_6 < 20), b_6, a_3) adjusts the expression from b_6 to a_3.
     (a_3 + 2 < 20), normalized to (a_3 < (20- 2)) folded to (a_3 < 18).  
    True is returned if successful.  */

bool
ranger::adjust_back (irange &r, tree def, tree name, sro_truth truth)
{
  irange op1_range, op2_range;
  range_stmt stmt = SSA_NAME_DEF_STMT (def);

  for ( ; stmt.valid (); stmt = SSA_NAME_DEF_STMT (def))
    {
      tree op1 = stmt.operand1 ();
      tree op2 = stmt.operand2 ();
      if (op1 == name)
	{ 
	  if (get_operand_range (op2_range, op2))
	    return stmt.handler ()->op1_adjust (r, op2_range, truth);
	  else
	    return false;
	}

      if (op2 == name)
	{
	  if (get_operand_range (op1_range, op1))
	    return stmt.handler ()->op2_adjust (r, op1_range, truth);
	  else
	    return false;
	}
     

      if (def_chain.in_chain_p (op1, name))
	{
	  /* Cant be derived on both sides.  */
	  if (def_chain.in_chain_p (op2, name))
	    return false;  // remove_from_gori

	  /* Evaluate range now.  */
	  if (!get_operand_range (op2_range, op2))
	    return false;

	  if (!stmt.handler ()->op1_adjust (r, op2_range, truth))
	    return false;
     
          def = op1;
	}
      else
        {
	  if (!get_operand_range (op1_range, op1))
	    return false;
	  
	  if (!def_chain.in_chain_p (op2, name))
	    return false;  //remove_from_gori

	  if (!stmt.handler ()->op1_adjust (r, op1_range, truth))
	    return false;
     
          def = op2;
	}
    }
  return false;
}

bool
ranger::evaluate_bool_op (tree op, irange& op_range, tree name,
				     sro_truth truth)
{
  range_stmt op_stmt;
  gimple *s;

  gcc_assert (TREE_TYPE (op) == boolean_type_node);

  if (TREE_CODE (op) == INTEGER_CST)
    {
      op_range.set_range (boolean_type_node, op, op);
      return true;
    }

  s = SSA_NAME_DEF_STMT (op);
  op_stmt = s;
  if (!op_stmt.valid ())
    return false;  // remove_from_gori

  return !get_range_from_stmt (op_stmt, op_range, name, truth);
}
 
enum bool_value { bool_is_zero, bool_is_one, bool_is_unknown };


bool
ranger::handle_boolean_mix (range_stmt& stmt, irange& r, tree name,
			  sro_truth truth, tree name_op, tree non_name_op)
{
  irange boolop_range;
  enum bool_value  bv;
  range_stmt op_stmt;
  gimple *s;

  irange boolrange_false (boolean_type_node, boolean_false_node,
			  boolean_false_node);
  irange boolrange_true (boolean_type_node, boolean_true_node,
			 boolean_true_node);

  if (non_name_op)
    {
      evaluate_bool_op (non_name_op, boolop_range, name, truth);

      /* set the value of the boolean operator.  */
      if (boolop_range == boolrange_false)
	bv = bool_is_zero;
      else
	if (boolop_range == boolrange_true)
	  bv = bool_is_one;
	else
	  bv = bool_is_unknown;
    }
  else
    bv = bool_is_unknown;

  /* Here we special case each of the possible operator mixes.  All of these
     operations are order independent. */

  switch (stmt.get_code ())
    {
      case NE_EXPR:
      case EQ_EXPR:
        // Equal or not-uequal to zero reverses the logic.
        if (bv == bool_is_zero)
	  truth = SRO_TRUTH_NEGATE (truth);
        break;

      case BIT_AND_EXPR:
      case TRUTH_AND_EXPR:
        // op && 0 is always an empty range on the true side.
	// op && 1 if always an empty range on the false size.
        if ((bv == bool_is_zero && (truth == SRO_NON_ZERO_DEF))
	    || (bv == bool_is_one && (truth == SRO_ZERO_DEF)))
	  {
	    r.clear (TREE_TYPE (name));
	    return true;
	  }
        break;

      case BIT_IOR_EXPR:
      case TRUTH_OR_EXPR:
        // op || 1 is always an empty range on the false side.
        if (bv == bool_is_one && (truth == SRO_ZERO_DEF))
	  {
	    r.clear (TREE_TYPE (name));
	    return true;
	  }
        break;

      case TRUTH_NOT_EXPR:
      case BIT_NOT_EXPR:
        truth = SRO_TRUTH_NEGATE (truth);
	break;

      default:
        error ("unexpected boolean mix operator");
    }

  s = SSA_NAME_DEF_STMT (name_op);
  op_stmt = s;
  if (!op_stmt.valid ())
    return false;  // remove_from_gori

  return !get_range_from_stmt (op_stmt, r, name, truth);
}


/* Given the expression in STMT, return an evaluation in R for NAME. */
bool
ranger::get_range_from_stmt (range_stmt& stmt, irange& r, tree name,
			   sro_truth truth)
{
  range_stmt op_stmt;
  irange op1_range, op2_range;
  tree op1, op2;
  tree op1_type, op2_type;
  bool op1_in_chain, op2_in_chain;

  op1 = stmt.operand1 ();
  op2 = stmt.operand2 ();

  if (op1 == name)
    { 
      if (get_operand_range (op2_range, op2))
	return stmt.handler ()-> op1_range (r, op2_range, truth);
      else
        return false;
    }

  if (op2 == name)
    {
      if (get_operand_range (op1_range, op1))
	return stmt.handler ()-> op2_range (r, op1_range, truth);
      else
        return false;
    }

  op1_in_chain = def_chain.in_chain_p (op1, name);
  op2_in_chain = def_chain.in_chain_p (op2, name);

  /* If neither operand is derived, then this stmt tells us nothing. */
  if (!op1_in_chain && !op2_in_chain)
    return false;   // remove_from_gori  

  /* At this point, check for the case where we have a boolean expression and 
     we are looking thru it for a non-boolean range. 
     results.  consider
        c_2 = b_3 < 100
	d_2 = c2 != 0

     if we are querying the range of b_3 on the non-zero side (true),
     evaluation of c_2 will return the range [0,99] for unsigned.
     the 0 on the RHS of c2 != 0 is a boolean, we we'll see a boolean
     range [0,0] for that.

     In order to evaluate this properly, we need to consider the boolean range
     and the operator and adjust the truth value accordingly.  */

  if (op1_in_chain)
    op1_type = TREE_TYPE (name);
  else
    op1_type = TREE_TYPE (op1);

  if (op2_in_chain)
    op2_type = TREE_TYPE (name);
  else
    op2_type = TREE_TYPE (op2);

  /* When we have a mix and one type is a boolean we need to look at the logic
     and see if there are any adjustments neccessary.  */
  if (op1_type != op2_type && (op1_type == boolean_type_node ||
					    op2_type == boolean_type_node))
    {
      /* And if the types are different, the one we are looking for cannot
	 be a boolean.  */
      gcc_assert (TREE_TYPE (name) != boolean_type_node);

      if (op1_in_chain)
	return handle_boolean_mix (stmt, r, name, truth, op1, op2);
      else
	return handle_boolean_mix (stmt, r, name, truth, op2, op1);
    }

  /* At this point we know the types are the same.  */

  if (op1_in_chain)
    {
      if (TREE_TYPE (op2) == boolean_type_node)
        return handle_boolean_mix (stmt, r, name, truth, op1, op2);

      /* Evaluate range now.  */
      if (!get_operand_range (op2_range, op2))
	return false;

      if (!stmt.handler ()-> op1_range (r, op2_range, truth))
	return false;

      /* Adjust range to be in terms of NAME.  */
      return adjust_back (r, op1, name, truth);
    }

  /* must be op2 in chain.  */
  if (TREE_TYPE (op1) == boolean_type_node)
    return handle_boolean_mix (stmt, r, name, truth, op2, op1);

  /* evaluate a riange for op1.  */
  if (!get_operand_range (op1_range, op1))
    return false;
    
  if (!stmt.handler ()->op2_range (r, op1_range, truth))
    return false;

  /* Adjust range to be in terms of NAME.  */
  return adjust_back (r, op2, name, truth);
}

/* Given the expression in STMT, return an evaluation in R for NAME. */
bool
ranger::get_range_from_stmt (gimple *stmt, irange& r, tree name,
			   sro_truth truth)
{
  range_stmt rn;
  rn = stmt;

  /* If it isnt an expression that is understood, we know nothing.  */
  if (!rn.valid())
    return false;

  return get_range_from_stmt (rn, r, name, truth);
}
