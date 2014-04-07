/* Gimple decl, type, and expression support functions.

   Copyright (C) 2007-2014 Free Software Foundation, Inc.
   Contributed by Aldy Hernandez <aldyh@redhat.com>

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
#include "tm.h"
#include "gimple-value.h"
#include "gimple-tree.h"
#include "langhooks.h"


Gimple::value_ptr boolean_true_node_ptr (&global_trees[TI_BOOLEAN_TRUE]);
Gimple::value_ptr boolean_false_node_ptr (&global_trees[TI_BOOLEAN_FALSE]);
Gimple::integer_cst_ptr integer_zero_node_ptr (&global_trees[TI_INTEGER_ZERO]);
Gimple::type_ptr void_type_node_ptr (&global_trees[TI_VOID_TYPE]);
Gimple::type_ptr boolean_type_node_ptr (&global_trees[TI_BOOLEAN_TYPE]);
Gimple::type_ptr char_type_node_ptr (&integer_types[itk_char]);
Gimple::type_ptr integer_type_node_ptr (&integer_types[itk_int]);

location_t 
expr_location (Gimple::value v)
{
  if (v)
    return v->expr_location();
  else
    return UNKNOWN_LOCATION;
}

void
strip_nops (Gimple::value_ptr gv)
{ 
  *gv = tree_strip_nop_conversions (CONST_CAST_TREE ((tree)(*gv)));
}

extern tree copy_node_stat (tree MEM_STAT_DECL);
extern tree make_node_stat (enum tree_code MEM_STAT_DECL);

namespace Gimple {

void *
gimple_copy_node (const void *t)
{
  return (void *) copy_node_stat
		  (reinterpret_cast<tree>(const_cast<void *>(t)) MEM_STAT_INFO);
}

void *
gimple_make_node (enum tree_code t)
{
  return (void *) make_node_stat (t MEM_STAT_INFO);
}


Gimple::identifier
decl_with_viz_desc::assembler_name()
{
  if (!assembler_name_set_p ())
    lang_hooks.set_decl_assembler_name (&node);
  return node.decl_with_vis.assembler_name;
}

}  // namespace Gimple


