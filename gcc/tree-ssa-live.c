/* Liveness for SSA trees.
   Copyright (C) 2003 Free Software Foundation, Inc.
   Contributed by Andrew MacLeod <amacleod@redhat.com>

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
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "flags.h"
#include "basic-block.h"
#include "function.h"
#include "diagnostic.h"
#include "bitmap.h"
#include "tree-flow.h"
#include "tree-simple.h"
#include "tree-inline.h"
#include "varray.h"
#include "timevar.h"
#include "tree-alias-common.h"
#include "hashtab.h"
#include "tree-dump.h"
#include "tree-ssa-live.h"

extern unsigned long next_ssa_version;


/* This is where the mapping from SSA version number to real storage variable
   is tracked.  

   All SSA versions of the same variable may not ultimately be mapped back to
   the same real variable. In that instance, we need to detect the live
   range overlap, and give one of the variable new storage. The vector
   'partition_to_var' tracks which partition maps to which variable.

   Given a VAR, it is sometimes desirable to know which partition that VAR
   represents.  There is an additional field in the variable annotation to
   track that information.  */

/* Create the variable partition and initialize it.  */

var_map
init_var_map (size)
     int size;
{
  var_map map;

  map = (var_map) xmalloc (sizeof (struct _var_map));
  map->var_partition = partition_new (size);
  map->partition_to_var 
	      = (tree *)xmalloc (size * sizeof (tree));
  memset (map->partition_to_var, 0, size * sizeof (tree));

  map->partition_to_compact = NULL;
  map->compact_to_partition = NULL;
  map->num_partitions = size;
  map->partition_size = size;
  return map;
}

/* Free memory associated with a var_map.  */

void
delete_var_map (map)
     var_map map;
{
  free (map->partition_to_var);
  partition_delete (map->var_partition);
  if (map->partition_to_compact)
    free (map->partition_to_compact);
  if (map->compact_to_partition)
    free (map->compact_to_partition);
  free (map);
}

/* This function will combine 2 partitions.  */

int
var_union (map, var1, var2)
     var_map map;
     tree var1, var2;
{
  int p1, p2, p3;

  /* This is independant of partition_to_compact. If partition_to_compact is 
     on, then whichever one of these partitions is absorbed will never have a
     dereference into the partition_to_compact array any more.  */

  p1 = partition_find (map->var_partition, SSA_NAME_VERSION (var1));
  p2 = partition_find (map->var_partition, SSA_NAME_VERSION (var2));

  if (p1 == p2)
    p3 = p1;
  else
    p3 = partition_union (map->var_partition, p1, p2);

  if (map->partition_to_compact)
    p3 = map->partition_to_compact[p3];

  return p3;
}


/* Compress the partition numbers such that they fall in the range 
   0..(num_partitions-1) instead of whereever they turned out during
   the partitioning exercise. This removes any references to unused
   partitions, thereby allowing bitmaps and other vectors to be much
   denser.  */

void 
compact_var_map (map)
     var_map map;
{
  sbitmap used;
  int x, limit, count, tmp;
  tree var;

  limit = map->partition_size;
  used = sbitmap_alloc (limit);
  sbitmap_zero (used);

  /* Already compressed? Abandon the old one.  */
  if (map->partition_to_compact)
    free (map->partition_to_compact);
  if (map->compact_to_partition)
    free (map->compact_to_partition);

  map->partition_to_compact = (int *)xmalloc (limit * sizeof (int));
  memset (map->partition_to_compact, 0xff, (limit * sizeof (int)));

  /* Find out which partitions are actually referenced.  */
  count = 0;
  for (x = 0; x < limit; x++)
    {
      tmp = partition_find (map->var_partition, x);
      if (!TEST_BIT (used, tmp) && map->partition_to_var[tmp] != NULL_TREE)
        {
	  SET_BIT (used, tmp);
	  count++;
	}
    }
  

  /* Build a compacted partitioning.  */
  if (count != limit)
    {
      map->compact_to_partition = (int *)xmalloc (count * sizeof (int));
      count = 0;
      /* SSA renaming begins at 1, so skip 0 when compacting.  */
      EXECUTE_IF_SET_IN_SBITMAP (used, 1, x,
	{
	  map->partition_to_compact[x] = count;
	  map->compact_to_partition[count] = x;
	  var = map->partition_to_var[x];
	  if (TREE_CODE (var) != SSA_NAME)
	    change_partition_var (map, var, count);
	  count++;
	});
    }
  else
    {
      free (map->partition_to_compact);
      map->partition_to_compact = NULL;
    }

  map->num_partitions = count;
  sbitmap_free (used);
}


/* This routine can be used to change the representative variable for a 
   partition from an SSA variable to a regular variable. Since SSA variables
   are versioned, that association cannot be changed. This allows partitions
   to be mapped back to real variables.  */
  
void 
change_partition_var (map, var, part)
     var_map map;
     tree var;
     int part;
{
  var_ann_t ann;

  if (TREE_CODE (var) == SSA_NAME)
    abort();

  ann = var_ann (var);
  ann->processed_out_of_ssa = 1;
  VAR_ANN_PARTITION (ann) = part;
  if (map->compact_to_partition)
    map->partition_to_var[map->compact_to_partition[part]] = var;
}


/* This looks through the function to determine what SSA versioned variables
   need to have entries in the partition table.  */

var_map
create_ssa_var_map ()
{
  block_stmt_iterator bsi;
  basic_block bb;
  tree *dest;
  tree stmt;
  varray_type ops;
  unsigned x;
  int i;
  var_map map;
  tree phi;

  map = init_var_map (next_ssa_version + 1);
  FOR_EACH_BB (bb)
    {
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
	  stmt = bsi_stmt (bsi);
	  get_stmt_operands (stmt);

	  ops = use_ops (stmt);
	  for (x = 0; ops && x < VARRAY_ACTIVE_SIZE (ops); x++)
	    {
	      tree *use = VARRAY_GENERIC_PTR (ops, x);
	      register_ssa_partition (map, *use);
	    }

	  dest = def_op (stmt);
	  if (dest)
	    register_ssa_partition (map, *dest);
	}
      
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	{
	  register_ssa_partition (map, PHI_RESULT (phi));
	  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	    register_ssa_partition (map, PHI_ARG_DEF (phi, i));
	}
    }

  return map;
}


/* Output a partition.  */

void
dump_var_map (f, map, dump_flags)
     FILE *f;
     var_map map;
     int dump_flags;
{
  int t;
  unsigned x, y;
  int p;

  fprintf (f, "\nPartition map \n\n");
  if (dump_flags & TDF_DETAILS)
    {
      for (x = 1; x <= next_ssa_version; x++)
	{
	  p = partition_find (map->var_partition, x);
	  if (map->partition_to_compact)
	    p = map->partition_to_compact[p];
	  fprintf (f, "ver %3d -> partition %3d  (", x, p);
	  if (p >= 0)
	    print_generic_expr (f, partition_to_var (map, p), TDF_SLIM);
	  fprintf (f, ")\n");
	}
      fprintf (f, "\n\n");
    }

  for (x = 0; x < map->num_partitions; x++)
    {
      if (map->compact_to_partition != NULL)
	p = map->compact_to_partition[x];
      else
	p = x;

      if (map->partition_to_var[p] == NULL_TREE)
        continue;
      t = 0;
      for (y = 1; y < next_ssa_version; y++)
        {
	  p = partition_find (map->var_partition, y);
	  if (map->partition_to_compact)
	    p = map->partition_to_compact[p];
	  if (p == (int)x)
	    {
	      if (t++ == 0)
	        {
		  fprintf(stderr, "Partition %d (", x);
		  print_generic_expr (f, partition_to_var (map, p), TDF_SLIM);
		  fprintf (stderr, " - ");
		}
	      fprintf (stderr, "%d ", y);
	    }
	}
      if (t != 0)
	fprintf (stderr, ")\n");
    }
  fprintf (f, "\n");
}
