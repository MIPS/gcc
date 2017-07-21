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
#include "ssa-range-stmt.h"


class ranger
{
  ssa_define_chain& def_chain;
  bool get_operand_range (irange& r, tree op);
  bool adjust_back (irange &r, tree def, tree name, sro_truth truth);
  bool evaluate_bool_op (tree op, irange& op_range, tree name, sro_truth truth);
  bool handle_boolean_mix (range_stmt& stmt, irange& r, tree name,
                           sro_truth truth, tree name_op, tree non_name_op);
  bool get_range_from_stmt (range_stmt& stmt, irange& r, tree name,
			    sro_truth truth);
public:
  ranger (ssa_define_chain & dc);
  bool get_range_from_stmt (gimple *stmt, irange& r, tree name,
			    sro_truth truth);
};

inline
ranger::ranger (ssa_define_chain& dc) : def_chain (dc)
{
}

class gori
{
  vec<bitmap> gori_map; 	/* Generates Outgoing Range Info.  */
  ssa_define_chain def_chain;
  ranger range_generator;
  bool remove_from_gori_map (basic_block bb, tree name);

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


#endif /* GCC_SSA_RANGE_GEN_H */
