/* Routines for expanding from SSA form to RTL.
   Copyright (C) 2009 Free Software Foundation, Inc.

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


#ifndef _SSAEXPAND_H
#define _SSAEXPAND_H 1

#include "tree-ssa-live.h"

struct ssaexpand
{
  var_map map;
  gimple *values;
  rtx *partition_to_pseudo;
  void *elim_graph;
};

extern struct ssaexpand SA;

static inline rtx
get_rtx_for_ssa_name (tree exp)
{
  int p = partition_find (SA.map->var_partition, SSA_NAME_VERSION (exp));
  if (SA.map->partition_to_view)
    p = SA.map->partition_to_view[p];
  gcc_assert (p != NO_PARTITION);
  return SA.partition_to_pseudo[p];
}

static inline gimple
get_gimple_for_ssa_name (tree exp)
{
  int v = SSA_NAME_VERSION (exp);
  if (SA.values)
    return SA.values[v];
  return NULL;
}

/* In tree-outof-ssa.c.  */
void finish_out_of_ssa (struct ssaexpand *sa);
unsigned int rewrite_out_of_ssa (struct ssaexpand *sa);
void expand_phi_nodes (struct ssaexpand *sa);

#endif
