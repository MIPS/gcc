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

static void live_worklist			PARAMS ((tree_live_info_p, varray_type, int));
static tree_live_info_p new_tree_live_info	PARAMS ((var_map));
static inline void set_if_valid			PARAMS ((var_map, sbitmap, tree));
static inline void add_livein_if_notdef		PARAMS ((tree_live_info_p, sbitmap, tree, basic_block));
static inline void register_ssa_partition	PARAMS ((var_map, tree));


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
compact_var_map (map, use_singles)
     var_map map;
     int use_singles;
{
  sbitmap used;
  int x, limit, count, tmp, root, root_i;
  tree var;
  root_var_p rv = NULL;

  limit = map->partition_size;
  used = sbitmap_alloc (limit);
  sbitmap_zero (used);

  /* Already compressed? Abandon the old one.  */
  if (map->partition_to_compact)
    {
      free (map->partition_to_compact);
      map->partition_to_compact = NULL;
    }
  if (map->compact_to_partition)
    {
      free (map->compact_to_partition);
      map->partition_to_compact = NULL;
    }

  if (use_singles)
    rv = init_root_var (map);

  map->partition_to_compact = (int *)xmalloc (limit * sizeof (int));
  memset (map->partition_to_compact, 0xff, (limit * sizeof (int)));

  /* Find out which partitions are actually referenced.  */
  count = 0;
  for (x = 0; x < limit; x++)
    {
      tmp = partition_find (map->var_partition, x);
      if (!TEST_BIT (used, tmp) && map->partition_to_var[tmp] != NULL_TREE)
        {
	  /* It is referenced, check to see if there is more than one version
	     in the root_var table, if one is available.  */
	  if (rv)
	    {
	      root = find_root_var (rv, tmp);
	      root_i = first_root_var_partition (rv, root);
	      /* If there is only one, don't include this in the compaction.  */
	      if (next_root_var_partition (rv, root_i) == ROOT_VAR_NONE)
	        continue;
	    }
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

  if (rv)
    delete_root_var (rv);
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
  ann->out_of_ssa_tag = 1;
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
  tree *dest, *use;
  tree stmt, phi;
  varray_type ops;
  unsigned x;
  int i;
  var_map map;

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
	      use = VARRAY_GENERIC_PTR (ops, x);
	      register_ssa_partition (map, *use);
	    }

	  dest = def_op (stmt);
	  if (dest)
	    {
	      register_ssa_partition (map, *dest);
	    }
	}

      /* Now register elements of PHI nodes.  */
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
        {
	  register_ssa_partition (map, PHI_RESULT (phi));
	  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	    register_ssa_partition (map, PHI_ARG_DEF (phi, i));
	}

    }

  return map;
}

/* Create a new live range information object.  */

static tree_live_info_p
new_tree_live_info (map)
     var_map map;
{
  tree_live_info_p live;

  live = (tree_live_info_p) xmalloc (sizeof (struct tree_live_info_d));
  live->map = map;
  live->num_blocks = n_basic_blocks;

  live->global = sbitmap_alloc (num_var_partitions (map));
  sbitmap_zero (live->global);

  live->livein = sbitmap_vector_alloc (num_var_partitions (map), 
				       live->num_blocks);
  sbitmap_vector_zero (live->livein, num_var_partitions (map));

  /* liveout is deferred until it is actually requested.  */
  live->liveout = NULL;
  return live;
}

/* Free storage for a live range info object.  */

void 
delete_tree_live_info (live)
     tree_live_info_p live;
{
  if (live->liveout)
    sbitmap_vector_free (live->liveout);
  if (live->livein)
    sbitmap_vector_free (live->livein);
  if (live->global)
    sbitmap_vector_free (live->global);
  
  free (live);
}

/* Fill in all the live-on-entry blocks between the def's and uses for a
   given partition.  */

static void
live_worklist (live, stack, i)
     tree_live_info_p live;
     varray_type stack;
     int i;
{
  int b;
  tree var;
  basic_block def_bb = NULL;
  edge e;
  var_map map = live->map;

  var = partition_to_var (map, i);
  if (SSA_NAME_DEF_STMT (var))
    def_bb = bb_for_stmt (SSA_NAME_DEF_STMT (var));

  EXECUTE_IF_SET_IN_SBITMAP (live->livein[i], 0, b,
    {
      VARRAY_PUSH_INT (stack, b);
    });

  while (VARRAY_ACTIVE_SIZE (stack) > 0)
    {
      b = VARRAY_TOP_INT (stack);
      VARRAY_POP (stack);

      for (e = BASIC_BLOCK (b)->pred; e; e = e->pred_next)
        if (e->src != ENTRY_BLOCK_PTR)
	  {
	    /* Its not live on entry to the block its defined in.  */
	    if (e->src == def_bb)
	      continue;
	    if (!TEST_BIT (live->livein[i], e->src->index))
	      {
	        SET_BIT (live->livein[i], e->src->index);
		VARRAY_PUSH_INT (stack, e->src->index);
	      }
	  }
    }
}


/* If a variable is in a partition, set the bit for that partition.  */

static inline void
set_if_valid (map, vec, var)
     var_map map;
     sbitmap vec;
     tree var;
{
  int p = var_to_partition (map, var);
  if (p != NO_PARTITION)
    SET_BIT (vec, p);
}

/* If a variable is in a partition and it isn't defined, set the livein and 
   global bit for it.  */

static inline void
add_livein_if_notdef (live, def_vec, var, bb)
     tree_live_info_p live;
     sbitmap def_vec;
     tree var;
     basic_block bb;
{
  int p = var_to_partition (live->map, var);
  if (p == NO_PARTITION || bb == ENTRY_BLOCK_PTR)
    return;
  if (!TEST_BIT (def_vec, p))
    {
      SET_BIT (live->livein[p], bb->index);
      SET_BIT (live->global, p);
    }
}

/* Given a partition map, calculate all the live on entry bitmaps for 
   each basic block.  */

tree_live_info_p 
calculate_live_on_entry (map)
     var_map map;
{
  tree_live_info_p live;
  int num, i;
  basic_block bb;
  sbitmap saw_def;
  tree phi, var, stmt;
  tree *vec;
  edge e;
  varray_type stack;
  block_stmt_iterator bsi;
  varray_type ops;

  saw_def = sbitmap_alloc (num_var_partitions (map));

  live = new_tree_live_info (map);

  FOR_EACH_BB (bb)
    {
      sbitmap_zero (saw_def);

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	{
	  var = PHI_RESULT (phi);
	  set_if_valid (map, saw_def, var);
	  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	    {
	      var = PHI_ARG_DEF (phi, i);
	      stmt = SSA_NAME_DEF_STMT (var);
	      e = PHI_ARG_EDGE (phi, i);
	      /* Any uses in PHIs which either don't have def's or are not
	         defined in the block from which the def comes, will be live
		 on entry to that block.  */
	      if (!stmt || e->src != bb_for_stmt (stmt))
		add_livein_if_notdef (live, saw_def, var, e->src);
	    }
        }

      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
	  stmt = bsi_stmt (bsi);
	  get_stmt_operands (stmt);

	  ops = use_ops (stmt);
	  num = (ops ? VARRAY_ACTIVE_SIZE (ops) : 0);
	  for (i = 0; i < num; i++)
	    {
	      vec = VARRAY_GENERIC_PTR (ops, i);
	      add_livein_if_notdef (live, saw_def, *vec, bb);
	    }

	  ops = vuse_ops (stmt);
	  num = (ops ? VARRAY_ACTIVE_SIZE (ops) : 0);
	  for (i = 0; i < num; i++)
	    {
	      var = VARRAY_TREE (ops, i);
	      add_livein_if_notdef (live, saw_def, var, bb);
	    }

	  ops = vdef_ops (stmt);
	  num = (ops ? VARRAY_ACTIVE_SIZE (ops) : 0);
	  for (i = 0; i < num; i++)
	    {
	      var = VDEF_OP (VARRAY_TREE (ops, i));
	      add_livein_if_notdef (live, saw_def, var, bb);
	    }

	  vec = def_op (stmt);
	  if (vec)
	    {
	      set_if_valid (map, saw_def, *vec);
	    }

	  ops = vdef_ops (stmt);
	  num = (ops ? VARRAY_ACTIVE_SIZE (ops) : 0);
	  for (i = 0; i < num; i++)
	    {
	      var = VDEF_RESULT (VARRAY_TREE (ops, i));
	      set_if_valid (map, saw_def, var);
	    }
	}
    }

  VARRAY_INT_INIT (stack, n_basic_blocks, "stack");
  EXECUTE_IF_SET_IN_SBITMAP (live->global, 0, i,
    {
      live_worklist (live, stack, i);
    });

  return live;
}


/* Calculate the live on exit vectors.  */

void
calculate_live_on_exit (liveinfo)
     tree_live_info_p liveinfo;
{
  unsigned b;
  int i;
  sbitmap *on_exit;
  basic_block bb;
  edge e;
  tree t, phi;
  sbitmap on_entry;
  var_map map = liveinfo->map;

  on_exit = sbitmap_vector_alloc (n_basic_blocks, num_var_partitions (map));
  sbitmap_vector_zero (on_exit, n_basic_blocks);

  /* Set all the live-on-exit bits for uses in PHIs.  */
  FOR_EACH_BB (bb)
    {
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
        {
	  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	    { 
	      t = PHI_ARG_DEF (phi, i);
	      e = PHI_ARG_EDGE (phi, i);
	      if (e->src == ENTRY_BLOCK_PTR)
	        continue;
	      set_if_valid (map, on_exit[e->src->index], t);
	    }

	}
    }

  /* Set live on exit for all predecessors of live on entry's.  */
  for (i = 0; i < num_var_partitions (map); i++)
    {
      on_entry = live_entry_blocks (liveinfo, i);
      EXECUTE_IF_SET_IN_SBITMAP (on_entry, 0, b,
        {
	  for (e = BASIC_BLOCK(b)->pred; e; e = e->pred_next)
	    if (e->src != ENTRY_BLOCK_PTR)
	      SET_BIT (on_exit[e->src->index], i);
	});
    }

  liveinfo->liveout = on_exit;
}


/* Initialize a root_var object.  */

root_var_p
init_root_var (map)
     var_map map;
{
  root_var_p rv;
  int num_partitions = num_var_partitions (map);
  int x;
  tree t;
  var_ann_t ann;

  if (num_partitions == 0)
    return NULL;

  rv = (root_var_p) xmalloc (sizeof (struct root_var_d));
  rv->num_root_vars = 0;
  rv->map = map;
  rv->next_partition = (int *)xmalloc (num_partitions * sizeof (int));
  memset (rv->next_partition, ROOT_VAR_NONE, num_partitions * sizeof (int));

  x = MAX (40, (num_partitions / 20));
  VARRAY_TREE_INIT (rv->root_var, x, "root_var");
  VARRAY_INT_INIT (rv->first_partition, x, "first_partition");

  /* Start at the end and work towards the front. This will provide a list
     that is ordered from smallest to largest.  */
  for (x = num_partitions - 1; x >= 0; x--)
    {
      t = partition_to_var (map, x);
      /* The var map may not be compacted yet, so check for NULL.  */
      if (!t) 
        continue;
      if (TREE_CODE (t) == SSA_NAME)
	t = SSA_NAME_VAR (t);
      ann = var_ann (t);
      if (ann->out_of_ssa_tag)
        {
	  rv->next_partition[x] = VARRAY_INT (rv->first_partition, 
					      VAR_ANN_PARTITION (ann));
	  VARRAY_INT (rv->first_partition, VAR_ANN_PARTITION (ann)) = x;
	}
      else
        {
	  ann->out_of_ssa_tag = 1;
	  VAR_ANN_PARTITION (ann) = rv->num_root_vars++;
	  VARRAY_PUSH_TREE (rv->root_var, t);
	  VARRAY_PUSH_INT (rv->first_partition, x);
	}
    }

  /* Reset the out_of_ssa_tag flag on each variable for later use.  */
  for (x = 0; x < rv->num_root_vars; x++)
    {
      t = VARRAY_TREE (rv->root_var, x);
      var_ann (t)->out_of_ssa_tag = 0;
    }

  return rv;
}

/* Remove a partition form a root_var's list.  */

void
remove_root_var_partition (rv, root_index, partition_index)
     root_var_p rv;
     int root_index, partition_index;
{
  int i;

  i = first_root_var_partition (rv, root_index);
  if (i == partition_index)
    {
      VARRAY_INT (rv->first_partition, root_index) = rv->next_partition[i];
    }
  else
    {
      for ( ; i != ROOT_VAR_NONE; i = next_root_var_partition (rv, i))
        {
	  if (rv->next_partition[i] == partition_index)
	    {
	      rv->next_partition[i] = rv->next_partition[partition_index];
	      break;
	    }
	}
    }
}

/* Free the memory used by a root_var object.  */
void
delete_root_var (rv)
     root_var_p rv;
{
  if (!rv)
    return;

  free (rv->next_partition);
  free (rv);
}


/* Output a root_var object.  */
void
dump_root_var (f, rv)
     FILE *f;
     root_var_p rv;
{
  int x, i;

  if (!rv)
    return;

  fprintf (f, "Root Var dump\n");
  for (x = 0; x < num_root_vars (rv); x++)
    {
      print_generic_expr (stderr, root_var (rv, x), TDF_SLIM);
      fprintf (f, " : (");
      for (i = first_root_var_partition (rv, x); 
	   i != ROOT_VAR_NONE;
	   i = next_root_var_partition (rv, i))
	{
	  print_generic_expr (stderr, partition_to_var (rv->map, i), TDF_SLIM);
	  fprintf (f, " ");
	}
      fprintf (f, ")\n");
    }
  fprintf (f, ")\n");
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
	    {
	      print_generic_expr (f, partition_to_var (map, p), TDF_SLIM);
	      if (TREE_CODE (partition_to_var (map, p)) == SSA_NAME)
	        {
	          fprintf (f, "  -  \n");
		  print_generic_stmt (f, SSA_NAME_DEF_STMT (partition_to_var (map, p)), TDF_SLIM);
		}
	    }
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
