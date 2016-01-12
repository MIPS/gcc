/* Define UPC pointer-to-shared representation-independent operations
   Copyright (C) 2008-2016 Free Software Foundation, Inc.
   Contributed by Gary Funck <gary@intrepid.com>
     and Nenad Vukicevic <nenad@intrepid.com>.

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

#ifndef GCC_C_UPC_PTS_OPS_H
#define GCC_C_UPC_PTS_OPS_H 1

extern tree upc_pts_build_cond_expr (location_t, tree);
extern tree upc_pts_build_constant (location_t, tree);
extern tree upc_pts_build_cvt (location_t, tree);
extern tree upc_pts_build_diff (location_t, tree);
extern tree upc_pts_build_sum (location_t, tree);
extern tree upc_pts_build_threadof (location_t, tree);
extern tree upc_pts_build_value (location_t, tree, tree, tree, tree);
extern void upc_pts_init (void);
extern int upc_pts_is_null_p (tree);

#endif /* !GCC_C_UPC_PTS_OPS_H */
