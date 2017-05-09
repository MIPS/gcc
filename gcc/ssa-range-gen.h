/* Header file for ssa-range generator.
   Copyright (C) 2017 Free Software Foundation, Inc.
   Contributed by Andrew MacLeod <amacleod@redhat.com>.

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

#ifndef GCC_SSA_RANGE_GEN_H
#define GCC_SSA_RANGE_GEN_H

#include "range.h"
#include "range-op.h"
#include "ssa-def-chain.h"

/* This class is used summarize expressions that are supported by the
   irange_operator class.  

   A gimple stmt can be summarized and cached if it is supported.

   And expression can also be resolved to a range if required information
   is provided.  */


/* These are the various states a range_stmt can be in.  */
enum range_stmt_state { 
  RS_INV, /* Invalid rage node.  */
  RS_I,   /* Unary integer op, immediate resolution available.  */
  RS_S,   /* Unary ssa_name, requires a range to resolve  */
  RS_II,  /* Binary integer op, immediate resolution available.  */
  RS_SI,  /* Binary expression, op1 requires range to resolve.  */
  RS_IS,  /* Binary expression, op2 requires range to resolve. */
  RS_SS   /* Binary expression, op1 & op2 require range to resolve. */
};

class range_stmt
{
private:
  enum range_stmt_state state;
  tree_code code;
  tree op1;
  tree op2;
  enum range_stmt_state determine_state (tree op1, tree op2);
  void from_stmt (gimple *s);
  bool fold (irange& res, irange* value1, irange* value2);
public:
  range_stmt ();
  range_stmt (gimple *stmt);

  bool valid () const;
  tree_code get_code () const;
  bool is_relational ();
  class irange_operator *handler();
  range_stmt &operator= (gimple *stmt);
  bool ssa_required (tree *name1, tree *name2);
  tree operand1 () const;
  tree operand2 () const;
  bool fold (irange& res);

  void dump (FILE *f);
};

class gori
{
  vec<bitmap> gori_map; 	/* Generates Outgoing Range Info.  */
  ssa_define_chain def_chain;
  bool remove_from_gori_map (basic_block bb, tree name);

  bool get_operand_range (irange& r, tree op);

  bool adjust_back (irange &r, tree def, tree name, sro_truth truth);
  bool evaluate_bool_op (tree op, irange& op_range, tree name, sro_truth truth);
  bool handle_boolean_mix (range_stmt& stmt, irange& r, tree name,
                           sro_truth truth, tree name_op, tree non_name_op);
  bool get_range_from_stmt (range_stmt& stmt, irange& r, tree name,
			    sro_truth truth);

  bool get_derived_range_stmt (range_stmt& stmt, tree name, basic_block bb);
  gimple *last_stmt_gori (basic_block bb);

public:
  gori ();
  void build ();
  void build (basic_block bb);

  /* True if NAME Generates range info on one or more outgoing edges of BB.  */
  bool range_p (basic_block bb, tree name);
  /* What is the static calculated range of NAME on outgoing edge E.  */
  bool get_range (irange& r, tree name, edge e);

  void dump (FILE *f);
  void exercise (FILE *f);   /* do a full mapping pass, dump if provided.  */
};


inline bool
range_stmt::valid () const
{
  return state != RS_INV;
}

inline tree_code
range_stmt::get_code () const
{
  return code;
}

inline tree
range_stmt::operand1 () const
{
  return op1;
}

inline tree
range_stmt::operand2 () const
{
  return op2;
}

#endif /* GCC_SSA_RANGE_GEN_H */
