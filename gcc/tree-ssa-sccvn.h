/* Tree SCC value numbering
   Copyright (C) 2007 Free Software Foundation, Inc.
   Contributed by Daniel Berlin <dberlin@dberlin.org>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef TREE_SSA_SCCVN_H
#define TREE_SSA_SCCVN_H

/* TOP of the VN lattice.  */
extern tree VN_TOP;

typedef struct vn_ssa_aux
{
  /* SCC information.  */
  unsigned int dfsnum;
  bool visited;
  unsigned int low;
  bool on_sccstack;

  /* Value number.
     Note: This should always be an SSA_NAME or VN_TOP.
     Constants are not allowed.*/
  tree valnum;
  /* Representative expression, if not a direct constant. */
  tree expr;
  /* Whether the representative expression contains constants.  */
  bool has_constants;
} *vn_ssa_aux_t;

/* Return the value numbering info for an SSA_NAME.  */
extern vn_ssa_aux_t VN_INFO (tree);
void run_scc_vn (void);
void free_scc_vn (void);

#endif /* TREE_SSA_SCCVN_H  */
