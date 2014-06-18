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
#include "gimple-tree.h"
#include "langhooks.h"
#include "stor-layout.h"


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
  extern tree tree_strip_nop_conversions (tree);
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

Gimple::identifier
decl_desc::assembler_name() const
{
  decl_with_viz viz(&node);
  return viz->assembler_name ();
}

enum machine_mode
type_desc::vector_mode() const
{
  return vector_type_mode(&node);
}

}  // namespace Gimple


