/* Functions to analyze and validate SIMPLE trees.
   Copyright (C) 2002 Free Software Foundation, Inc.

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

#ifndef _TREE_SIMPLE_H
#define _TREE_SIMPLE_H 1

/* Interface used in [break/goto]-elimination : to be declared in a .h file. */
extern void insert_before_continue_end PARAMS ((tree, tree, int));
extern void tree_build_scope           PARAMS ((tree *));
extern tree create_tmp_var             PARAMS ((tree, tree *));
extern tree declare_tmp_vars           PARAMS ((tree, tree));
extern tree copy_stmt_chain            PARAMS ((tree));
extern tree copy_stmt                  PARAMS ((tree));
extern tree update_line_number         PARAMS ((tree, int));

#endif /* _TREE_SIMPLE_H */

