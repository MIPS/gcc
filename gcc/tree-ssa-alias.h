/* Tree based alias analysis and alias oracle.
   Copyright (C) 2008 Free Software Foundation, Inc.
   Contributed by Richard Guenther  <rguenther@suse.de>

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   GCC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef TREE_SSA_ALIAS_H
#define TREE_SSA_ALIAS_H

#include "coretypes.h"

/* In tree-ssa-alias.c  */
void *walk_non_aliased_vuses (tree, tree,
			      void *(*)(tree, tree, void *), void *);

/* In tree-dfa.c  */
extern bool refs_may_alias_p (tree, tree);

#endif /* TREE_SSA_ALIAS_H  */
