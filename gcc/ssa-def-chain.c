/* SSA definition Chains.
   Copyright (C) 2017 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>.

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
#include "ssa-def-chain.h"
#include "ssa-range-gen.h"



ssa_define_chain::ssa_define_chain ()
{
  def_chain.create (0);
  def_chain.safe_grow_cleared (num_ssa_names);
  terminal.create (0);
  terminal.safe_grow_cleared (num_ssa_names);
}

bitmap
ssa_define_chain::operator[] (unsigned index)
{
  gcc_assert (index < num_ssa_names);

      /* Dynamically calculate defintion chain if it hasnt been done yet.  */
  if (!def_chain[index])
    generate_def_chain (index);
  return def_chain[index];
}

bitmap
ssa_define_chain::operator[] (tree name)
{
  return operator[] (SSA_NAME_VERSION (name));
}

tree 
ssa_define_chain::terminal_name (unsigned index)
{
  return terminal[index];
}

tree
ssa_define_chain::terminal_name (tree name)
{
  return terminal_name (SSA_NAME_VERSION (name));
}

bool
ssa_define_chain::in_chain_p (tree def, tree name)
{
  if (TREE_CODE (def) != SSA_NAME || TREE_CODE (name) != SSA_NAME)
    return false;
  unsigned v = SSA_NAME_VERSION (name);
  bitmap b = operator[](def);
  return (b && bitmap_bit_p (b, v));
}


/* If operand is an SSA_NAME, try to generate info for it and update the 
   bitmaps.  */
tree
ssa_define_chain::process_op (tree operand, unsigned version)
{
  bitmap ret;
  tree term;
  unsigned op_index = SSA_NAME_VERSION (operand);

  /* Make sure defintion chain exists for operand. */
  term = generate_def_chain (op_index);
  terminal[op_index] = term;

  /* If it has a chain, add them to this one. */
  ret = def_chain [op_index];
  if (!bitmap_empty_p (ret))
    bitmap_ior_into (def_chain[version], ret);

  /* Finally add operand itself to the chain.  */
  bitmap_set_bit (def_chain[version], op_index);
  return term;
}


tree
ssa_define_chain::generate_def_chain (unsigned index)
{
  gimple *stmt;
  range_stmt rn;
  tree ssa1, ssa2, tmp = NULL_TREE;
  tree index_name = ssa_name(index);
  tree ret = index_name;

  /* If bitmap has been allocated, version has already been processed.  */
  if (def_chain[index])
    return terminal[index];

  def_chain[index] = BITMAP_ALLOC (NULL);
  
  stmt = SSA_NAME_DEF_STMT (index_name);
  rn = stmt;

  /* If a valid range stmt and there are ssa names, process args.  */
  if (rn.valid () && rn.ssa_required (&ssa1, &ssa2))
    {
      if (rn.is_relational ())
        {
	  /* Can look thru both operators  */
	  if (ssa1)
	    ret = process_op (ssa1, index);
	  if (ssa2)
	    tmp = process_op (ssa2, index);
	  /* If there are 2 terminal results, dont bother tracking it.  */
	  if (ret && tmp)
	    ret = NULL_TREE;
	  else
	    if (!ret)
	      ret = tmp;
	}
      else
        {
	  /* Look back until there are 2 ssa names.  */
	  if (ssa1 && ssa2)
	    ret = index_name;
	  else
	    if (ssa1)
	      ret = process_op (ssa1, index);
	    else
	      ret = process_op (ssa2, index);
	}
    }
  terminal[index] = ret;
  return ret;
}

void
ssa_define_chain::dump (FILE *f)
{
  unsigned x, y;
  bitmap_iterator bi;
  for (x = 1; x< num_ssa_names; x++)
    {
      if (def_chain[x] && !bitmap_empty_p (def_chain[x]))
        {
	  print_generic_expr (f, ssa_name (x), TDF_SLIM);
	  fprintf (f, "  : (terminal term: ");
	  if (terminal_name (x))
	    print_generic_expr (f, terminal_name(x), TDF_SLIM);
	  else
	    fprintf (f, "none");
	  fprintf (f, ")  :");
	  EXECUTE_IF_SET_IN_BITMAP (def_chain[x], 0, y, bi)
	    {
	      print_generic_expr (f, ssa_name (y), TDF_SLIM);
	      fprintf (f, "  ");
	    }
	  fprintf (f, "\n");
	}
    }
}
