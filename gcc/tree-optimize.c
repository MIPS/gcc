/* Control and data flow functions for trees.
   Copyright (C) 2001 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "expr.h"
#include "c-common.h"
#include "diagnostic.h"
#include "basic-block.h"
#include "tree-optimize.h"
#include "tree-flow.h"

/* {{{ optimize_tree()

   Main entry point to the tree-based optimizer.  */

void
optimize_tree (t)
     tree t;
{
  /* Flush out existing data.  */
  VARRAY_TREE_INIT (referenced_symbols, 20, "function_symbols");

  tree_find_basic_blocks (t);

  if (n_basic_blocks > 0)
    {
      tree_find_varrefs ();
      tree_build_ssa ();
    }

  /* Flush out DFA and SSA data.  */
  referenced_symbols = NULL;
  delete_cfg ();
}

/* }}} */
