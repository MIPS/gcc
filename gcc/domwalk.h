/* Generic dominator tree walker
   Copyright (C) 2003 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* This is the main data structure for the dominator walker.  It provides
   the callback hooks as well as a convenient place to hang block local
   data.  Eventually it will probably include pass global data as well.  */

struct dom_walk_data
{
  /* Function to call before the statement walk occurring before the
     recursive walk of the dominator children. 

     This typically initializes an block local data and pushes that
     data onto BLOCK_DATA_STACK.  */
  void (*before_dom_children_before_stmts) (struct dom_walk_data *,
					    basic_block, tree);

  /* Function to call to walk statements before the recursive walk
     of the dominator children.  */
  void (*before_dom_children_walk_stmts) (struct dom_walk_data *,
					  basic_block, tree);

  /* Function to call after the statement walk occurring before the
     recursive walk of the dominator children.  */
  void (*before_dom_children_after_stmts) (struct dom_walk_data *,
					   basic_block, tree);

  /* Function to call before the statement walk occurring after the
     recursive walk of the dominator children.  */
  void (*after_dom_children_before_stmts) (struct dom_walk_data *,
					   basic_block, tree);

  /* Function to call to walk statements after the recursive walk
     of the dominator children.  */
  void (*after_dom_children_walk_stmts) (struct dom_walk_data *,
					 basic_block, tree);

  /* Function to call after the statement walk occurring after the
     recursive walk of the dominator children. 

     This typically finalizes any block local data and pops
     that data from BLOCK_DATA_STACK.  */
  void (*after_dom_children_after_stmts) (struct dom_walk_data *,
					  basic_block, tree);

  /* Stack of any data we need to keep on a per-block basis.  */
  varray_type block_data_stack;
};

void walk_dominator_tree (struct dom_walk_data *, basic_block, tree);
