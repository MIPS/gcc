/* LTO function reader.
   Copyright 2006 Free Software Foundation, Inc.

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
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "tree-gimple.h"
#include "tree-iterator.h"
#include "lto.h"

void
lto_read_function_body (lto_info_fd *fd ATTRIBUTE_UNUSED,
			lto_context *context ATTRIBUTE_UNUSED,
			tree fn,
			const void *data ATTRIBUTE_UNUSED)
{
  tree return_expr;
  tree stmt_list;
  tree_stmt_iterator i;
  tree body;

  /* ??? For now, just pretend the function returns immediately.  */
  return_expr = build1 (RETURN_EXPR, void_type_node, NULL_TREE);
  stmt_list = alloc_stmt_list ();
  i = tsi_start (stmt_list);
  tsi_link_after (&i, return_expr, TSI_NEW_STMT);
  body = build3 (BIND_EXPR, NULL_TREE, NULL_TREE, stmt_list, NULL_TREE);
  DECL_SAVED_TREE (fn) = body;
  DECL_INITIAL (fn) = make_node (BLOCK);

  return;
}
