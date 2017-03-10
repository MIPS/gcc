/* Header file for ssa definition chain.
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

#ifndef GCC_SSA_DEF_CHAIN_H
#define GCC_SSA_DEF_CHAIN_H


/* This class is used to maintain a definition chain of SSA_NAMEs that all
   share the attribute that they are supported by the irange_operator
   table.  This means they can be used to reevaluate expressions. ie
   a_3 = b_6 + 4
   c_7 = a_3 * 2
   q_9 = -c_7

   If an ssa_name is in the definition_chain for another, then an expression
   using one can be re-expressed in terms of the other.
   
   the definition chain for a_3 contains only b_6.
   the definition chain for c_7 contains both a_3 and b_6.
   the definition chain for q_9 contains c_7, a_3, and b_6.
   These are calculated on demand and cached.  */
   
class ssa_define_chain
{
  vec<bitmap> def_chain;
  vec<tree> terminal;
  tree process_op (tree operand, unsigned version);
  tree generate_def_chain (unsigned index);
public:
  ssa_define_chain ();
  bitmap operator[](tree name);
  bitmap operator[](unsigned index);
  tree terminal_name (tree name);
  tree terminal_name (unsigned index);
  bool in_chain_p (tree def, tree name);
  void dump (FILE *f);
};

#endif /* GCC_SSA_DEF_CHAIN_H */
