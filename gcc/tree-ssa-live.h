/* Routines for liveness in SSA trees.
   Copyright (C) 2003 Free Software Foundation, Inc.
   Contributed by Andrew MacLeod  <amacleod@redhat.com>

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


#ifndef _TREE_SSA_LIVE_H__
#define _TREE_SSA_LIVE_H 1


/* Used to create the variable mapping when we go out of SSA form.  */
typedef struct _var_map
{
  /* The partition of all variables.  */
  partition var_partition;

  /* Vector for compacting partitions.  */
  int *partition_to_compact;
  int *compact_to_partition;

  /* Mapping of partition numbers to vars.  */
  tree *partition_to_var;

  /* Current number of partitions.  */
  unsigned int num_partitions;

  /* Original partition size.  */
  unsigned int partition_size;
} *var_map;

#define VAR_ANN_PARTITION(ann) (ann->partition)

extern var_map init_var_map		PARAMS ((int));
extern void delete_var_map		PARAMS ((var_map));
extern void dump_var_map		PARAMS ((FILE *, var_map, int));
extern void compact_var_map		PARAMS ((var_map));
extern int var_union			PARAMS ((var_map, tree, tree));
extern void change_partition_var	PARAMS ((var_map, tree, int));
extern var_map create_ssa_var_map	PARAMS ((void));

static inline int num_var_partitions	PARAMS ((var_map));
static inline tree var_to_partition_to_var	PARAMS ((var_map, tree));
static inline tree partition_to_var	PARAMS ((var_map, int));
static inline void register_ssa_partition	PARAMS ((var_map, tree));
static inline int var_to_partition		PARAMS ((var_map, tree));

/* Number of partitions.  */

static inline int 
num_var_partitions (map)
     var_map map;
{
  return map->num_partitions;
}

 
/* Given a partition number, return the variable which represents that 
   partition.  */
 
static inline tree
partition_to_var (map, i)
     var_map map;
     int i;
{
  if (map->compact_to_partition)
    i = map->compact_to_partition[i];
  return map->partition_to_var[i];
}

/* Given a variable, return the partition number which contains it.  */

static inline int
var_to_partition (map, var)
     var_map map;
     tree var;
{
  var_ann_t ann;
  int part;

  if (TREE_CODE (var) == SSA_NAME)
    {
      part = partition_find (map->var_partition, SSA_NAME_VERSION (var));
      if (map->partition_to_compact)
	part = map->partition_to_compact[part];
    }
  else
    {
      ann = var_ann (var);
      part = VAR_ANN_PARTITION (ann);
    }
  return part;
}

/* Given a variable, return the variable which represents the entire partition
   the specified one is a member of.  */

static inline tree
var_to_partition_to_var (map, var)
     var_map map;
     tree var;
{
  int part;

  part = var_to_partition (map, var);
  return partition_to_var (map, part);
}

/* This routine registers an SSA versioned variable with the partition
   manager. Any unregistered partitions may be compacted out later.  */

static inline void
register_ssa_partition (map, ssa_var)
     var_map map;
     tree ssa_var;
{
  if (TREE_CODE (ssa_var) != SSA_NAME)
    abort ();
  map->partition_to_var[SSA_NAME_VERSION (ssa_var)] = ssa_var;
}



#endif /* _TREE_SSA_LIVE_H  */
