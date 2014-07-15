/* Gimple routines required for functions declared in tree.[ch]

   Copyright (C) 2014 Free Software Foundation, Inc.
   Contributed by Andrew MacLeod  <amacleod@redhat.com>

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "gimple-tree.h"
#include "gimple-expr.h"

/* Define the actual instances for the varoious gimple nodes required.  */

G::value_ptr boolean_true_node_ptr (&global_trees[TI_BOOLEAN_TRUE]);
G::value_ptr boolean_false_node_ptr (&global_trees[TI_BOOLEAN_FALSE]);
G::integer_cst_ptr integer_zero_node_ptr (&global_trees[TI_INTEGER_ZERO]);
G::type_ptr void_type_node_ptr (&global_trees[TI_VOID_TYPE]);
G::type_ptr boolean_type_node_ptr (&global_trees[TI_BOOLEAN_TYPE]);
G::type_ptr pointer_type_node_ptr (&global_trees[TI_PTR_TYPE]);
G::type_ptr char_type_node_ptr (&integer_types[itk_char]);
G::type_ptr integer_type_node_ptr (&integer_types[itk_int]);

const char *
gimple_tree_printable_name (tree decl, int verbosity)
{
  return gimple_decl_printable_name (decl, verbosity);
}


