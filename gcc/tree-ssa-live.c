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

static void live_worklist (tree_live_info_p, varray_type, int);
static tree_live_info_p new_tree_live_info (var_map);
static inline void set_if_valid (var_map, bitmap, tree);
static inline void add_livein_if_notdef (tree_live_info_p, bitmap,
					 tree, basic_block);
static inline void register_ssa_partition (var_map, tree);
static inline void add_conflicts_if_valid (tpa_p, conflict_graph,
					   var_map, bitmap, tree);
static partition_pair_p find_partition_pair (coalesce_list_p, int, int, bool);

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
init_var_map (int size)
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
delete_var_map (var_map map)
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
register_ssa_partition (var_map map, tree ssa_var)
{
  tree stmt, arg;
  int version, i;

#if defined ENABLE_CHECKING
  if (TREE_CODE (ssa_var) != SSA_NAME)
    abort ();
#endif

  version = SSA_NAME_VERSION (ssa_var);
  if (map->partition_to_var[version] == NULL_TREE)
    {
      map->partition_to_var[SSA_NAME_VERSION (ssa_var)] = ssa_var;

      /* If this is a PHI node, register all the PHI elements as well.  */
      stmt = SSA_NAME_DEF_STMT (ssa_var);
      if (stmt && TREE_CODE (stmt) == PHI_NODE)
	for (i = 0; i < PHI_NUM_ARGS (stmt); i++)
	  {
	    arg = PHI_ARG_DEF (stmt, i);
	    if (phi_ssa_name_p (arg))
	      register_ssa_partition (map, arg);
	  }
    }
}

/* This function will combine 2 partitions.  Returns the partition which 
   represents the new partition. If the two partitions cannot be combined, 
   NO_PARTITION is returned. */

int
var_union (var_map map, tree var1, tree var2)
{
  int p1, p2, p3;
  tree root_var = NULL_TREE;
  tree other_var = NULL_TREE;

  /* This is independent of partition_to_compact. If partition_to_compact is 
     on, then whichever one of these partitions is absorbed will never have a
     dereference into the partition_to_compact array any more.  */

  if (TREE_CODE (var1) == SSA_NAME)
    p1 = partition_find (map->var_partition, SSA_NAME_VERSION (var1));
  else
    {
      p1 = var_to_partition (map, var1);
      if (map->compact_to_partition)
        p1 = map->compact_to_partition[p1];
      root_var = var1;
    }
  
  if (TREE_CODE (var2) == SSA_NAME)
    p2 = partition_find (map->var_partition, SSA_NAME_VERSION (var2));
  else
    {
      p2 = var_to_partition (map, var2);
      if (map->compact_to_partition)
        p2 = map->compact_to_partition[p2];

      /* If there is no root_var set, or its not a user variable, set the
	 root_var to this one.  */
      if (!root_var || is_gimple_tmp_var (root_var))
        {
	  other_var = root_var;
	  root_var = var2;
	}
      else 
	other_var = var2;
    }

  if (p1 == NO_PARTITION || p2 == NO_PARTITION)
    abort ();

  if (p1 == p2)
    p3 = p1;
  else
    p3 = partition_union (map->var_partition, p1, p2);

  if (map->partition_to_compact)
    p3 = map->partition_to_compact[p3];

  if (root_var)
    change_partition_var (map, root_var, p3);
  if (other_var)
    change_partition_var (map, other_var, p3);

  return p3;
}


/* Compress the partition numbers such that they fall in the range 
   0..(num_partitions-1) instead of wherever they turned out during
   the partitioning exercise. This removes any references to unused
   partitions, thereby allowing bitmaps and other vectors to be much
   denser.

   This is implemented such that compaction doesn't affect partitioning.
   That is, once partitions are created and possibly merged, running one
   or more different kind of compaction will not affect the partitions
   themselves. Their index might change, but all the same variables will
   still be members of the same partition group. This allows work on reduced
   sets, and no lose of information when a larger set is desired.

   In particular, coalescing can work on partitions which have 2 or more
   definitions, and then 'recompact' later to include all the single
   definitions for assignment to program variables.  */

void 
compact_var_map (var_map map, int flags)
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

  if (flags & VARMAP_NO_SINGLE_DEFS)
    rv = root_var_init (map);

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
	      root = root_var_find (rv, tmp);
	      root_i = root_var_first_partition (rv, root);
	      /* If there is only one, don't include this in the compaction.  */
	      if (root_var_next_partition (rv, root_i) == ROOT_VAR_NONE)
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
    root_var_delete (rv);
  sbitmap_free (used);
}


/* This routine can be used to change the representative variable for a 
   partition from an SSA variable to a regular variable. Since SSA variables
   are versioned, that association cannot be changed. This allows partitions
   to be mapped back to real variables.  */
  
void 
change_partition_var (var_map map, tree var, int part)
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
create_ssa_var_map (void)
{
  block_stmt_iterator bsi;
  basic_block bb;
  tree *dest, *use;
  tree stmt;
  stmt_ann_t ann;
  varray_type ops;
  unsigned x;
  var_map map;
#if defined ENABLE_CHECKING
  sbitmap used_in_real_ops;
  sbitmap used_in_virtual_ops;
#endif

  map = init_var_map (next_ssa_version + 1);

#if defined ENABLE_CHECKING
  used_in_real_ops = sbitmap_alloc (num_referenced_vars);
  sbitmap_zero (used_in_real_ops);

  used_in_virtual_ops = sbitmap_alloc (num_referenced_vars);
  sbitmap_zero (used_in_virtual_ops);
#endif

  FOR_EACH_BB (bb)
    {
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
	  stmt = bsi_stmt (bsi);
	  get_stmt_operands (stmt);
	  ann = stmt_ann (stmt);

	  /* Register USE and DEF operands in each statement.  */
	  ops = use_ops (ann);
	  for (x = 0; ops && x < VARRAY_ACTIVE_SIZE (ops); x++)
	    {
	      use = VARRAY_TREE_PTR (ops, x);
	      register_ssa_partition (map, *use);
#if defined ENABLE_CHECKING
	      SET_BIT (used_in_real_ops, var_ann (SSA_NAME_VAR (*use))->uid);
#endif
	    }

	  ops = def_ops (ann);
	  for (x = 0; ops && x < VARRAY_ACTIVE_SIZE (ops); x++)
	    {
	      dest = VARRAY_TREE_PTR (ops, x);
	      register_ssa_partition (map, *dest);
#if defined ENABLE_CHECKING
	      SET_BIT (used_in_real_ops, var_ann (SSA_NAME_VAR (*dest))->uid);
#endif
	    }

	  /* While we do not care about virtual operands for
	     out of SSA, we do need to look at them to make sure
	     we mark all the variables which are used.  */
	  ops = vuse_ops (ann);
	  for (x = 0; ops && x < VARRAY_ACTIVE_SIZE (ops); x++)
	    {
	      tree var = VARRAY_TREE (ops, x);
	      set_is_used (var);
#if defined ENABLE_CHECKING
	      SET_BIT (used_in_virtual_ops, var_ann (SSA_NAME_VAR (var))->uid);
#endif
	    }

	  ops = vdef_ops (ann);
	  for (x = 0; ops && x < VARRAY_ACTIVE_SIZE (ops); x++)
	    {
	      tree var = VDEF_OP (VARRAY_TREE (ops, x));
	      set_is_used (var);
#if defined ENABLE_CHECKING
	      SET_BIT (used_in_virtual_ops, var_ann (SSA_NAME_VAR (var))->uid);
#endif
	    }
	}
    }

#if defined ENABLE_CHECKING
  {
    unsigned i;
    sbitmap both = sbitmap_alloc (num_referenced_vars);
    sbitmap_a_and_b (both, used_in_real_ops, used_in_virtual_ops);
    if (sbitmap_first_set_bit (both) >= 0)
      {
	EXECUTE_IF_SET_IN_SBITMAP (both, 0, i,
	  fprintf (stderr, "Variable %s used in real and virtual operands\n",
		   get_name (referenced_var (i))));
	abort ();
      }

    sbitmap_free (used_in_real_ops);
    sbitmap_free (used_in_virtual_ops);
    sbitmap_free (both);
  }
#endif

  return map;
}

/* Create a new live range information object.  */

static tree_live_info_p
new_tree_live_info (var_map map)
{
  tree_live_info_p live;
  int x;

  live = (tree_live_info_p) xmalloc (sizeof (struct tree_live_info_d));
  live->map = map;
  live->num_blocks = n_basic_blocks;

  live->global = BITMAP_XMALLOC ();

  live->livein = (bitmap *)xmalloc (num_var_partitions (map) * sizeof (bitmap));
  for (x = 0; x < num_var_partitions (map); x++)
    live->livein[x] = BITMAP_XMALLOC ();

  /* liveout is deferred until it is actually requested.  */
  live->liveout = NULL;
  return live;
}

/* Free storage for a live range info object.  */

void 
delete_tree_live_info (tree_live_info_p live)
{
  int x;
  if (live->liveout)
    {
      for (x = live->num_blocks - 1; x >= 0; x--)
        BITMAP_XFREE (live->liveout[x]);
      free (live->liveout);
    }
  if (live->livein)
    {
      for (x = num_var_partitions (live->map) - 1; x >= 0; x--)
        BITMAP_XFREE (live->livein[x]);
      free (live->livein);
    }
  if (live->global)
    BITMAP_XFREE (live->global);
  
  free (live);
}

/* Fill in all the live-on-entry blocks between the def's and uses for a
   given partition.  */

static void
live_worklist (tree_live_info_p live, varray_type stack, int i)
{
  int b;
  tree var;
  basic_block def_bb = NULL;
  edge e;
  var_map map = live->map;

  var = partition_to_var (map, i);
  if (SSA_NAME_DEF_STMT (var))
    def_bb = bb_for_stmt (SSA_NAME_DEF_STMT (var));

  EXECUTE_IF_SET_IN_BITMAP (live->livein[i], 0, b,
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
	    if (!bitmap_bit_p (live->livein[i], e->src->index))
	      {
	        bitmap_set_bit (live->livein[i], e->src->index);
		VARRAY_PUSH_INT (stack, e->src->index);
	      }
	  }
    }
}


/* If a variable is in a partition, set the bit for that partition.  */

static inline void
set_if_valid (var_map map, bitmap vec, tree var)
{
  int p = var_to_partition (map, var);
  if (p != NO_PARTITION)
    bitmap_set_bit (vec, p);
}

/* If a variable is in a partition and it isn't defined, set the livein and 
   global bit for it.  */

static inline void
add_livein_if_notdef (tree_live_info_p live, bitmap def_vec,
		      tree var, basic_block bb)
{
  int p = var_to_partition (live->map, var);
  if (p == NO_PARTITION || bb == ENTRY_BLOCK_PTR)
    return;
  if (!bitmap_bit_p (def_vec, p))
    {
      bitmap_set_bit (live->livein[p], bb->index);
      bitmap_set_bit (live->global, p);
    }
}

/* Given a partition map, calculate all the live on entry bitmaps for 
   each basic block.  */

tree_live_info_p 
calculate_live_on_entry (var_map map)
{
  tree_live_info_p live;
  int num, i;
  basic_block bb;
  bitmap saw_def;
  tree phi, var, stmt;
  tree *vec;
  edge e;
  varray_type stack;
  block_stmt_iterator bsi;
  varray_type ops;
  stmt_ann_t ann;

  saw_def = BITMAP_XMALLOC ();

  live = new_tree_live_info (map);

  FOR_EACH_BB (bb)
    {
      bitmap_clear (saw_def);

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	{
	  var = PHI_RESULT (phi);
	  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	    {
	      var = PHI_ARG_DEF (phi, i);
	      if (!phi_ssa_name_p (var))
	        continue;
	      stmt = SSA_NAME_DEF_STMT (var);
	      e = PHI_ARG_EDGE (phi, i);
	      /* Any uses in PHIs which either don't have def's or are not
	         defined in the block from which the def comes, will be live
		 on entry to that block.  */
	      if (!stmt || e->src != bb_for_stmt (stmt))
		add_livein_if_notdef (live, saw_def, var, e->src);
	    }
        }

      /* Don't mark PHI results as defined until all the PHI nodes have
	 been processed. If the PHI sequence is:
	    a_3 = PHI <a_1, a_2>
	    b_3 = PHI <b_1, a_3>
	 The a_3 referred to in b_3's PHI node is the one incoming on the
	 edge, *not* the PHI node just seen.  */

      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
        {
	  var = PHI_RESULT (phi);
	  set_if_valid (map, saw_def, var);
	}

      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
	  stmt = bsi_stmt (bsi);
	  get_stmt_operands (stmt);
	  ann = stmt_ann (stmt);

	  ops = use_ops (ann);
	  num = (ops ? VARRAY_ACTIVE_SIZE (ops) : 0);
	  for (i = 0; i < num; i++)
	    {
	      vec = VARRAY_TREE_PTR (ops, i);
	      add_livein_if_notdef (live, saw_def, *vec, bb);
	    }

	  ops = vuse_ops (ann);
	  num = (ops ? VARRAY_ACTIVE_SIZE (ops) : 0);
	  for (i = 0; i < num; i++)
	    {
	      var = VARRAY_TREE (ops, i);
	      add_livein_if_notdef (live, saw_def, var, bb);
	    }

	  ops = vdef_ops (ann);
	  num = (ops ? VARRAY_ACTIVE_SIZE (ops) : 0);
	  for (i = 0; i < num; i++)
	    {
	      var = VDEF_OP (VARRAY_TREE (ops, i));
	      add_livein_if_notdef (live, saw_def, var, bb);
	    }

	  ops = def_ops (ann);
	  num = (ops ? VARRAY_ACTIVE_SIZE (ops) : 0);
	  for (i = 0; i < num; i++)
	    {
	      vec = VARRAY_TREE_PTR (ops, i);
	      set_if_valid (map, saw_def, *vec);
	    }

	  ops = vdef_ops (ann);
	  num = (ops ? VARRAY_ACTIVE_SIZE (ops) : 0);
	  for (i = 0; i < num; i++)
	    {
	      var = VDEF_RESULT (VARRAY_TREE (ops, i));
	      set_if_valid (map, saw_def, var);
	    }
	}
    }

  VARRAY_INT_INIT (stack, n_basic_blocks, "stack");
  EXECUTE_IF_SET_IN_BITMAP (live->global, 0, i,
    {
      live_worklist (live, stack, i);
    });

#ifdef ENABLE_CHECKING
   /* Check for live on entry partitions and report those with a DEF in
      the program. This will typically mean an optimization has done
      something wrong.  */

  bb = ENTRY_BLOCK_PTR;
  num = 0;
  for (e = bb->succ; e; e = e->succ_next)
    {
      int entry_block = e->dest->index;
      if (e->dest == EXIT_BLOCK_PTR)
        continue;
      for (i = 0; i < num_var_partitions (map); i++)
	{
	  basic_block tmp;
	  tree d;
	  var = partition_to_var (map, i);
	  stmt = SSA_NAME_DEF_STMT (var);
	  tmp = bb_for_stmt (stmt);
	  d = default_def (SSA_NAME_VAR (var));

	  if (bitmap_bit_p (live_entry_blocks (live, i), entry_block))
	    {
	      if (!IS_EMPTY_STMT (stmt))
		{
		  num++;
		  print_generic_expr (stderr, var, TDF_SLIM);
		  fprintf (stderr, " is defined ");
		  if (tmp)
		    fprintf (stderr, " in BB%d, ", tmp->index);
		  fprintf (stderr, "by:\n");
		  print_generic_expr (stderr, stmt, TDF_SLIM);
		  fprintf (stderr, "\nIt is also live-on-entry to entry BB %d", 
			   entry_block);
		  fprintf (stderr, " So it appears to have multiple defs.\n");
		}
	      else
	        {
		  if (d != var)
		    {
		      num++;
		      print_generic_expr (stderr, var, TDF_SLIM);
		      fprintf (stderr, " is live-on-entry to BB%d ",entry_block);
		      if (d)
		        {
			  fprintf (stderr, " but is not the default def of ");
			  print_generic_expr (stderr, d, TDF_SLIM);
			  fprintf (stderr, "\n");
			}
		      else
			fprintf (stderr, " and there is no default def.\n");
		    }
		}
	    }
	  else
	    if (d == var)
	      {
		/* The only way this var shouldn't be marked live on entry to 
		   this block is if it occurs in a PHI argument of the block.  */
		int z, ok = 0;
		for (phi = phi_nodes (e->dest); 
		     phi && !ok; 
		     phi = TREE_CHAIN (phi))
		  {
		    for (z = 0; z < PHI_NUM_ARGS (phi); z++)
		      if (var == PHI_ARG_DEF (phi, z))
			{
			  ok = 1;
			  break;
			}
		  }
		if (ok)
		  continue;
	        num++;
		print_generic_expr (stderr, var, TDF_SLIM);
		fprintf (stderr, " is not marked live-on-entry to entry BB%d ", 
			 entry_block);
		fprintf (stderr, "but it is the default def so it should be.\n");
	      }
	}
    }
  if (num > 0)
    abort ();
#endif

  return live;
}


/* Calculate the live on exit vectors.  */

void
calculate_live_on_exit (tree_live_info_p liveinfo)
{
  unsigned b;
  int i, x;
  bitmap *on_exit;
  basic_block bb;
  edge e;
  tree t, phi;
  bitmap on_entry;
  var_map map = liveinfo->map;

  on_exit = (bitmap *)xmalloc (n_basic_blocks * sizeof (bitmap));
  for (x = 0; x < n_basic_blocks; x++)
    on_exit[x] = BITMAP_XMALLOC ();

  /* Set all the live-on-exit bits for uses in PHIs.  */
  FOR_EACH_BB (bb)
    {
      for (phi = phi_nodes (bb); phi; phi = TREE_CHAIN (phi))
	for (i = 0; i < PHI_NUM_ARGS (phi); i++)
	  { 
	    t = PHI_ARG_DEF (phi, i);
	    e = PHI_ARG_EDGE (phi, i);
	    if (!phi_ssa_name_p (t) || e->src == ENTRY_BLOCK_PTR)
	      continue;
	    set_if_valid (map, on_exit[e->src->index], t);
	  }
    }

  /* Set live on exit for all predecessors of live on entry's.  */
  for (i = 0; i < num_var_partitions (map); i++)
    {
      on_entry = live_entry_blocks (liveinfo, i);
      EXECUTE_IF_SET_IN_BITMAP (on_entry, 0, b,
        {
	  for (e = BASIC_BLOCK(b)->pred; e; e = e->pred_next)
	    if (e->src != ENTRY_BLOCK_PTR)
	      bitmap_set_bit (on_exit[e->src->index], i);
	});
    }

  liveinfo->liveout = on_exit;
}


/* Initialize a tree_partition_associator object.  */

tpa_p
tpa_init (var_map map)
{
  tpa_p tpa;
  int num_partitions = num_var_partitions (map);
  int x;

  if (num_partitions == 0)
    return NULL;

  tpa = (tpa_p) xmalloc (sizeof (struct tree_partition_associator_d));
  tpa->num_trees = 0;
  tpa->uncompressed_num = -1;
  tpa->map = map;
  tpa->next_partition = (int *)xmalloc (num_partitions * sizeof (int));
  memset (tpa->next_partition, TPA_NONE, num_partitions * sizeof (int));

  tpa->partition_to_tree_map = (int *)xmalloc (num_partitions * sizeof (int));
  memset (tpa->partition_to_tree_map, TPA_NONE, num_partitions * sizeof (int));

  x = MAX (40, (num_partitions / 20));
  VARRAY_TREE_INIT (tpa->trees, x, "trees");
  VARRAY_INT_INIT (tpa->first_partition, x, "first_partition");

  return tpa;

}

/* Remove a partition from a TPA's list.  */

void
tpa_remove_partition (tpa_p tpa, int tree_index, int partition_index)
{
  int i;

  i = tpa_first_partition (tpa, tree_index);
  if (i == partition_index)
    {
      VARRAY_INT (tpa->first_partition, tree_index) = tpa->next_partition[i];
    }
  else
    {
      for ( ; i != TPA_NONE; i = tpa_next_partition (tpa, i))
        {
	  if (tpa->next_partition[i] == partition_index)
	    {
	      tpa->next_partition[i] = tpa->next_partition[partition_index];
	      break;
	    }
	}
    }
}

/* Free the memory used by a tree_partition_associator object.  */
void
tpa_delete (tpa_p tpa)
{
  if (!tpa)
    return;

  free (tpa->partition_to_tree_map);
  free (tpa->next_partition);
  free (tpa);
}


/* This routine will remove any tree entires which have only a single
   element.  This will help keep the size of the conflict graph down.  
   The function returns the number of remaining tree lists.  */

int 
tpa_compact (tpa_p tpa)
{
  int last, x, y, first, swap_i;
  tree swap_t;

  /* Find the last list which has more than 1 partition.  */
  for (last = tpa->num_trees - 1; last > 0; last--)
    {
      first = tpa_first_partition (tpa, last);
      if (tpa_next_partition (tpa, first) != NO_PARTITION)
        break;
    }

  x = 0;
  while (x < last)
    {
      first = tpa_first_partition (tpa, x);
      /* If there is not more than one partition, swap with the current end
	 of the tree list.  */
      if (tpa_next_partition (tpa, first) == NO_PARTITION)
        {
	  swap_t = VARRAY_TREE (tpa->trees, last);
	  swap_i = VARRAY_INT (tpa->first_partition, last);

	  /* Update the last entry. Since it is known to only have one
	     partition, there is nothing else to update.  */
	  VARRAY_TREE (tpa->trees, last) = VARRAY_TREE (tpa->trees, x);
	  VARRAY_INT (tpa->first_partition, last) 
	    = VARRAY_INT (tpa->first_partition, x);
	  tpa->partition_to_tree_map[tpa_first_partition (tpa, last)] = last;

	  /* Since this list is known to have more than one partition, update
	     the list owner entries.  */
	  VARRAY_TREE (tpa->trees, x) = swap_t;
	  VARRAY_INT (tpa->first_partition, x) = swap_i;
	  for (y = tpa_first_partition (tpa, x); 
	       y != NO_PARTITION; 
	       y = tpa_next_partition (tpa, y))
	    tpa->partition_to_tree_map[y] = x;

	  /* Ensure last is a list with more than one partition.  */
	  last--;
	  for (; last > x; last--)
	    {
	      first = tpa_first_partition (tpa, last);
	      if (tpa_next_partition (tpa, first) != NO_PARTITION)
		break;
	    }
	}
      x++;
    }

  first = tpa_first_partition (tpa, x);
  if (tpa_next_partition (tpa, first) != NO_PARTITION)
    x++;
  tpa->uncompressed_num = tpa->num_trees;
  tpa->num_trees = x;
  return last;
}


/* Initialize a root_var object with SSA partitions which are based on each root
   variable.  */

root_var_p
root_var_init (var_map map)
{
  root_var_p rv;
  int num_partitions = num_var_partitions (map);
  int x, p;
  tree t;
  var_ann_t ann;
  sbitmap seen;

  rv = tpa_init (map);
  if (!rv)
    return NULL;

  seen = sbitmap_alloc (num_partitions);
  sbitmap_zero (seen);

  /* Start at the end and work towards the front. This will provide a list
     that is ordered from smallest to largest.  */
  for (x = num_partitions - 1; x >= 0; x--)
    {
      t = partition_to_var (map, x);
      /* The var map may not be compacted yet, so check for NULL.  */
      if (!t) 
        continue;

      p = var_to_partition (map, t);
#ifdef ENABLE_CHECKING
      if (p == NO_PARTITION)
        abort ();
#endif
      /* Make sure we only put coalesced partitions into the list once.  */
      if (TEST_BIT (seen, p))
        continue;
      SET_BIT (seen, p);
      if (TREE_CODE (t) == SSA_NAME)
	t = SSA_NAME_VAR (t);
      ann = var_ann (t);
      if (ann->root_var_processed)
        {
	  rv->next_partition[p] = VARRAY_INT (rv->first_partition, 
					      VAR_ANN_ROOT_INDEX (ann));
	  VARRAY_INT (rv->first_partition, VAR_ANN_ROOT_INDEX (ann)) = p;
	}
      else
        {
	  ann->root_var_processed = 1;
	  VAR_ANN_ROOT_INDEX (ann) = rv->num_trees++;
	  VARRAY_PUSH_TREE (rv->trees, t);
	  VARRAY_PUSH_INT (rv->first_partition, p);
	}
      rv->partition_to_tree_map[p] = VAR_ANN_ROOT_INDEX (ann);
    }

  /* Reset the out_of_ssa_tag flag on each variable for later use.  */
  for (x = 0; x < rv->num_trees; x++)
    {
      t = VARRAY_TREE (rv->trees, x);
      var_ann (t)->root_var_processed = 0;
    }

  sbitmap_free (seen);
  return rv;
}


/* Initialize a type_var structure which associates all the partitions of the
   same type to the type node. Volatiles are ignored.  */

type_var_p
type_var_init (var_map map)
{
  type_var_p tv;
  int x, y, p;
  int num_partitions = num_var_partitions (map);
  tree t;
  sbitmap seen;

  seen = sbitmap_alloc (num_partitions);
  sbitmap_zero (seen);

  tv = tpa_init (map);
  if (!tv)
    return NULL;

  for (x = num_partitions - 1; x >= 0; x--)
    {
      t = partition_to_var (map, x);
      /* Disallow coalescing of these types of variables.  */
      if (!t
	  || TREE_THIS_VOLATILE (t)
	  || TREE_CODE (t) == RESULT_DECL
      	  || TREE_CODE (t) == PARM_DECL 
	  || (DECL_P (t)
	      && (DECL_REGISTER (t)
		  || !DECL_ARTIFICIAL (t)
		  || DECL_RTL_SET_P (t))))
        continue;

      p = var_to_partition (map, t);
#ifdef ENABLE_CHECKING
      if (p == NO_PARTITION)
        abort ();
#endif
      /* If partitions have been coalesced, only add the representative for the
	 partition to the list once.  */
      if (TEST_BIT (seen, p))
        continue;
      SET_BIT (seen, p);
      t = TREE_TYPE (t);

      /* Find the list for this type.  */
      for (y = 0; y < tv->num_trees; y++)
        if (t == VARRAY_TREE (tv->trees, y))
	  break;
      if (y == tv->num_trees)
        {
	  tv->num_trees++;
	  VARRAY_PUSH_TREE (tv->trees, t);
	  VARRAY_PUSH_INT (tv->first_partition, p);
	}
      else
        {
	  tv->next_partition[p] = VARRAY_INT (tv->first_partition, y);
	  VARRAY_INT (tv->first_partition, y) = p;
	}
      tv->partition_to_tree_map[p] = y;
    }
  sbitmap_free (seen);
  return tv;
}


/* Create a coalesce list object that is empty.  */

coalesce_list_p 
create_coalesce_list (var_map map)
{
  coalesce_list_p list;

  list = (coalesce_list_p) xmalloc (sizeof (struct coalesce_list_d));

  list->map = map;
  list->add_mode = true;
  list->list = (partition_pair_p *) xcalloc (num_var_partitions (map),
					     sizeof (struct partition_pair_d));
  return list;
}


/* Delete a coalesce list.  */

void 
delete_coalesce_list (coalesce_list_p cl)
{
  free (cl->list);
  free (cl);
}

/* Find a matching coalesce pair. If one isn't found, return NULL if 'create' 
   is false, otherwise create a new coalesce object and return it.  */

static partition_pair_p
find_partition_pair (coalesce_list_p cl, int p1, int p2, bool create)
{
  partition_pair_p node, tmp;
  int s;
    
  /* Normalize so that p1 is the smaller value.  */
  if (p2 < p1)
    {
      s = p1;
      p1 = p2;
      p2 = s;
    }
  
  tmp = NULL;

  /* The list is sorted such that if we find a value greater than p2,
     p2 is not in the list.  */
  for (node = cl->list[p1]; node; node = node->next)
    {
      if (node->second_partition == p2)
        return node;
      else
        if (node->second_partition > p2)
	  break;
     tmp = node;
    }

  if (!create)
    return NULL;

  node = (partition_pair_p) xmalloc (sizeof (struct partition_pair_d));
  node->first_partition = p1;
  node->second_partition = p2;
  node->cost = 0;
    
  if (tmp != NULL)
    {
      node->next = tmp->next;
      tmp->next = node;
    }
  else
    {
      /* This is now the first node in the list.  */
      node->next = cl->list[p1];
      cl->list[p1] = node;
    }

  return node;
}

/* Add a pair of partitions to the coalesce list with a specified cost.  */

void 
add_coalesce (coalesce_list_p cl, int p1, int p2, int value)
{
  partition_pair_p node;

#ifdef ENABLE_CHECKING
  if (!cl->add_mode)
    abort();
#endif

  if (p1 == p2)
    return;

  node = find_partition_pair (cl, p1, p2, true);

  node->cost += value;
}


/* Prepare the coalesce list for removal of preferred pairs.  When finished,
   list element 0 has all the coalesce pairs, sorted in order from most
   important coalesce to least important.  */

void
sort_coalesce_list (coalesce_list_p cl)
{
  int x, num, last, val, odd;
  partition_pair_p n1, n1_prev ,n2, n2_end, n2_last;

  if (!cl->add_mode)
    abort();

  cl->add_mode = false;
  last = 0;

  /* Compact the list so we know how many lists there are.  */
  num = num_var_partitions (cl->map);
  for (x = 0; x < num; x++)
    if (cl->list[x] != NULL)
      {
        if (x != last)
	  cl->list[last] = cl->list[x];
	last++;
      }

  if (last == 0)
    return;

  num = last / 2;
  odd = last % 2;

  /* While there is more than one list, merge lists in pairs until only
     1 list remains.  */
  while (num >= 1)
    {
      last = 0;
      for (x = 0; x < num; x++)
        {
	  n1 = cl->list[x * 2];
	  n2 = cl->list[x * 2 + 1];
	  for (n1_prev = NULL; n1 && n2; n1 = n1->next)
	    {
	      val = n1->second_partition;
	      if (n2->second_partition  <= val)
		{
		  n2_last = n2;
		  /* Merge as many as will fit before n1.  */
		  for (n2_end = n2; n2_end; n2_end = n2_end->next)
		    {
		      if (n2_end->second_partition > val)
			break;
		      n2_last = n2_end;
		    }
		  if (n1_prev)
		    {
		      n2_last->next = n1;
		      n1_prev->next = n2;
		    }
		  else
		    {
		      n2_last->next = n1;
		      cl->list[x * 2] = n2;
		    }
		  n2 = n2_end;
		}
	      n1_prev = n1;
	    }
	  /* Append anything left over should be appended to the end.  */
	  if (n2)
	    n1_prev->next = n2;
	  cl->list[last++] = cl->list[x * 2];
	}

      /* If there were an odd number of lists, move the last one up as well.  */
      if (odd)
	cl->list[last++] = cl->list[num * 2];

      num = last / 2;
      odd = last % 2;
    }
}


/* Retrieve the best remaining pair to coalesce.  Returns the 2 partitions
   via parameters, and their calculated cost via the return value. The return
   value is NO_BEST_COALESCE if the coalesce list is empty.  */

int 
pop_best_coalesce (coalesce_list_p cl, int *p1, int *p2)
{
  partition_pair_p node;
  int ret;

  if (cl->add_mode)
    abort();

  node = cl->list[0];
  if (!node)
    return NO_BEST_COALESCE;

  cl->list[0] = node->next;

  *p1 = node->first_partition;
  *p2 = node->second_partition;
  ret = node->cost;
  free (node);

  return ret;
}

/* If a variable is in a partition, and it's not already live, add a 
   conflict between it and any other live partition.  Reset the live bit.  */

static inline void 
add_conflicts_if_valid (tpa_p tpa, conflict_graph graph,
			var_map map, bitmap vec, tree var)
{ 
  int p, y, first;
  p = var_to_partition (map, var);
  if (p != NO_PARTITION)
    { 
      bitmap_clear_bit (vec, p);
      first = tpa_find_tree (tpa, p);
      /* If find returns nothing, this object isn't interesting.  */
      if (first == TPA_NONE)
        return;
      /* Only add interferences between objects in the same list.  */
      for (y = tpa_first_partition (tpa, first);
	   y != TPA_NONE;
	   y = tpa_next_partition (tpa, y))
	{
	  if (bitmap_bit_p (vec, y))
	    conflict_graph_add (graph, p, y);
	}
    }
}


/* Build a conflict graph for the information contained in a live range
   information structure.  If a coalesce list is passed in, any copies
   discovered are added to the list.  */

conflict_graph
build_tree_conflict_graph (tree_live_info_p liveinfo, tpa_p tpa, 
			   coalesce_list_p cl)
{
  conflict_graph graph;
  var_map map;
  bitmap live;
  int num, x, y, i;
  basic_block bb;
  varray_type ops, partition_link, tpa_to_clear, tpa_nodes;
  unsigned l;

  map = live_var_map (liveinfo);
  graph = conflict_graph_new (num_var_partitions (map));

  if (tpa_num_trees (tpa) == 0)
    return graph;

  live = BITMAP_XMALLOC ();

  VARRAY_INT_INIT (partition_link, num_var_partitions (map) + 1, "part_link");
  VARRAY_INT_INIT (tpa_nodes, tpa_num_trees (tpa), "tpa nodes");
  VARRAY_INT_INIT (tpa_to_clear, 50, "tpa to clear");

  FOR_EACH_BB (bb)
    {
      block_stmt_iterator bsi;

      /* Start with live on exit temporaries.  */
      bitmap_copy (live, live_on_exit (liveinfo, bb));

      for (bsi = bsi_last (bb); !bsi_end_p (bsi); bsi_prev (&bsi))
        {
	  tree important_copy_rhs_partition = NULL_TREE;
	  tree stmt = bsi_stmt (bsi);
	  stmt_ann_t ann;

	  get_stmt_operands (stmt);
	  ann = stmt_ann (stmt);

	  /* Copies between 2 partitions do not introduce an interference 
	     by itself.  If they did, you would never be able to coalesce 
	     two things which are copied. If the two variables really do 
	     conflict, they will conflict elsewhere in the program.  
	     
	     This is handled specially here since we may also be interested 
	     in copies between real variables and SSA_NAME variables. We may
	     be interested in trying to coalesce SSA_NAME variables with
	     root variables in some cases.   */

	  if (TREE_CODE (stmt) == MODIFY_EXPR)
	    {
	      tree lhs = TREE_OPERAND (stmt, 0);
	      tree rhs = TREE_OPERAND (stmt, 1);
	      int p1, p2;
	      int bit;

	      if (DECL_P (lhs) || TREE_CODE (lhs) == SSA_NAME)
		p1 = var_to_partition (map, lhs);
	      else 
		p1 = NO_PARTITION;

	      if (DECL_P (rhs) || TREE_CODE (rhs) == SSA_NAME)
		p2 = var_to_partition (map, rhs);
	      else 
		p2 = NO_PARTITION;

	      if (p1 != NO_PARTITION && p2 != NO_PARTITION)
		{
		  important_copy_rhs_partition = rhs;
		  bit = bitmap_bit_p (live, p2);
		  /* If the RHS is live, make it not live while we add
		     the conflicts, then make it live again.  */
		  if (bit)
		    bitmap_clear_bit (live, p2);
		  add_conflicts_if_valid (tpa, graph, map, live, lhs);
		  if (bit)
		    bitmap_set_bit (live, p2);
		  if (cl)
		    add_coalesce (cl, p1, p2, 1);
		  set_if_valid (map, live, important_copy_rhs_partition);
		}
	    }

	  if (!important_copy_rhs_partition)
	    {
	      tree *var_p;

	      ops = def_ops (ann);
	      num = ((ops) ? VARRAY_ACTIVE_SIZE (ops) : 0);
	      for (x = 0; x < num; x++)
		{
		  var_p = VARRAY_TREE_PTR (ops, x);
		  add_conflicts_if_valid (tpa, graph, map, live, *var_p);
		}

	      ops = use_ops (ann);
	      num = ((ops) ? VARRAY_ACTIVE_SIZE (ops) : 0);
	      for (x = 0; x < num; x++)
		{
		  var_p = VARRAY_TREE_PTR (ops, x);
		  set_if_valid (map, live, *var_p);
		}
	    }
	}

      /* Anything which is still live at this point interferes.  
	 In order to implement this efficiently, only conflicts between
	 partitions which have the same TPA root need be added.
	 TPA roots which have been seen are tracked in 'tpa_nodes'. A non-zero
	 entry points to an index into 'partition_link', which then indexes 
	 into itself forming a linked list of partitions sharing a tpa root 
	 which have been seen as live up to this point.  Since partitions start
	 at index zero, all entries in partition_link are (partition + 1).

	 Conflicts are added between the current partition and any already seen.
	 tpa_clear contains all the tpa_roots processed, and these are the only
	 entries which need to be zero'd out for a clean restart.  */

      EXECUTE_IF_SET_IN_BITMAP (live, 0, x,
        {
	  i = tpa_find_tree (tpa, x);
	  if (i != TPA_NONE)
	    {
	      int start = VARRAY_INT (tpa_nodes, i);
	      /* If start is 0, a new root reference list is being started.
		 Register it to be cleared.  */
	      if (!start)
	        VARRAY_PUSH_INT (tpa_to_clear, i);

	      /* Add interferences to other tpa members seen.  */
	      for (y = start; y != 0; y = VARRAY_INT (partition_link, y))
		conflict_graph_add (graph, x, y - 1);
	      VARRAY_INT (tpa_nodes, i) = x + 1;
	      VARRAY_INT (partition_link, x + 1) = start;
	    }
	});

	/* Now clear the used tpa root references.  */
	for (l = 0; l < VARRAY_ACTIVE_SIZE (tpa_to_clear); l++)
	  VARRAY_INT (tpa_nodes, VARRAY_INT (tpa_to_clear, l)) = 0;
	VARRAY_POP_ALL (tpa_to_clear);
    }

  BITMAP_XFREE (live);
  return graph;
}


/* This routine will attempt to coalesce the elements in a TPA list.
   If a coalesce_list is provided, those coalesces are attempted first.
   If a file pointer is provided, debug output will be sent there.  */

void
coalesce_tpa_members (tpa_p tpa, conflict_graph graph, var_map map, 
		      coalesce_list_p cl, FILE *debug)
{
  int x, y, z;
  tree var, tmp;

  /* Attempt to coalesce any items in a coalesce list first.  */
  if (cl)
    {
      while (pop_best_coalesce (cl, &x, &y) != NO_BEST_COALESCE)
        {
	  if (!conflict_graph_conflict_p (graph, x, y))
	    {
	      if (tpa_find_tree (tpa, x) == TPA_NONE 
		  || tpa_find_tree (tpa, y) == TPA_NONE)
		continue;
	      var = partition_to_var (map, x);
	      tmp = partition_to_var (map, y);
	      z = var_union (map, var, tmp);
	      if (z == NO_PARTITION)
	        continue;
	      conflict_graph_merge_regs (graph, x, y);

	      /* z is the new combined partition. We need to remove the other
	         partition from the list. Set x to be that other partition.  */
	      if (z == x)
	        x = y;
	      z = tpa_find_tree (tpa, x);
	      tpa_remove_partition (tpa, z, x);
	      if (debug)
	        {
		  fprintf (debug, "Coalesce (list): ");
		  print_generic_expr (debug, var, TDF_SLIM);
		  fprintf (debug, " and ");
		  print_generic_expr (debug, tmp, TDF_SLIM);
		  fprintf (debug, "\n");
		}
	    }
	}

    }

  for (x = 0; x < tpa_num_trees (tpa); x++)
    {
      while (tpa_first_partition (tpa, x) != TPA_NONE)
        {
	  /* Coalesce first partition with everything that doesn't conflict.  */
	  y = tpa_first_partition (tpa, x);
	  tpa_remove_partition (tpa, x, y);
	  var = partition_to_var (map, y);
	  for (z = tpa_next_partition (tpa, y); 
	       z != TPA_NONE; 
	       z = tpa_next_partition (tpa, z))
	    {
	      tmp = partition_to_var (map, z);
	      /* If partitions are already merged, don't check for conflict.  */
	      if (tmp == var)
	        tpa_remove_partition (tpa, x, z);
	      else if (!conflict_graph_conflict_p (graph, y, z))
		{
		  if (tpa_find_tree (tpa, y) == TPA_NONE 
		      || tpa_find_tree (tpa, z) == TPA_NONE)
		    continue;
		  if (var_union (map, var, tmp) == NO_PARTITION)
		    continue;

		  tpa_remove_partition (tpa, x, z);
		  conflict_graph_merge_regs (graph, y, z);
		  if (debug)
		    {
		      fprintf (debug, "Coalesce : ");
		      print_generic_expr (debug, var, TDF_SLIM);
		      fprintf (debug, " and ");
		      print_generic_expr (debug, tmp, TDF_SLIM);
		      fprintf (debug, "\n");
		    }
		}
	    }
	}
    }
}

/* Show debug info for a coalesce list.  */
void 
dump_coalesce_list (FILE *f, coalesce_list_p cl)
{
  partition_pair_p node;
  int x, num;
  tree var;

  if (cl->add_mode)
    {
      fprintf (f, "Coalesce List:\n");
      num = num_var_partitions (cl->map);
      for (x = 0; x < num; x++)
        {
	  node = cl->list[x];
	  if (node)
	    {
	      fprintf (f, "[");
	      print_generic_expr (f, partition_to_var (cl->map, x), TDF_SLIM);
	      fprintf (f, "] - ");
	      for ( ; node; node = node->next)
	        {
		  var = partition_to_var (cl->map, node->second_partition);
		  print_generic_expr (f, var, TDF_SLIM);
		  fprintf (f, "(%1d), ", node->cost);
		}
	      fprintf (f, "\n");
	    }
	}
    }
  else
    {
      fprintf (f, "Sorted Coalesce list:\n");
      for (node = cl->list[0]; node; node = node->next)
        {
	  fprintf (f, "(%d) ", node->cost);
	  var = partition_to_var (cl->map, node->first_partition);
	  print_generic_expr (f, var, TDF_SLIM);
	  fprintf (f, " : ");
	  var = partition_to_var (cl->map, node->second_partition);
	  print_generic_expr (f, var, TDF_SLIM);
	  fprintf (f, "\n");
	}
    }
}


/* Output a tree_partition_associator object.  */
void
tpa_dump (FILE *f, tpa_p tpa)
{
  int x, i;

  if (!tpa)
    return;

  for (x = 0; x < tpa_num_trees (tpa); x++)
    {
      print_generic_expr (f, tpa_tree (tpa, x), TDF_SLIM);
      fprintf (f, " : (");
      for (i = tpa_first_partition (tpa, x); 
	   i != TPA_NONE;
	   i = tpa_next_partition (tpa, i))
	{
	  print_generic_expr (f, partition_to_var (tpa->map, i), TDF_SLIM);
	  fprintf (f, " ");
#ifdef ENABLE_CHECKING
	  if (tpa_find_tree (tpa, i) != x)
	    fprintf (f, "**find tree incorrectly set** ");
#endif
	}
      fprintf (f, ")\n");
    }
  fflush (f);
}

/* Output a partition.  */

void
dump_var_map (FILE *f, var_map map)
{
  int t;
  unsigned x, y;
  int p;

  fprintf (f, "\nPartition map \n\n");

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
		  fprintf(f, "Partition %d (", x);
		  print_generic_expr (f, partition_to_var (map, p), TDF_SLIM);
		  fprintf (f, " - ");
		}
	      fprintf (f, "%d ", y);
	    }
	}
      if (t != 0)
	fprintf (f, ")\n");
    }
  fprintf (f, "\n");
}

/* Output live range info.  */

void
dump_live_info (FILE *f, tree_live_info_p live, int flag)
{
  basic_block bb;
  int i;
  var_map map = live->map;

  if ((flag & LIVEDUMP_ENTRY) && live->livein)
    {
      FOR_EACH_BB (bb)
	{
	  fprintf (f, "\nLive on entry to BB%d : ", bb->index);
	  for (i = 0; i < num_var_partitions (map); i++)
	    {
	      if (bitmap_bit_p (live_entry_blocks (live, i), bb->index))
	        {
		  print_generic_expr (f, partition_to_var (map, i), TDF_SLIM);
		  fprintf (f, "  ");
		}
	    }
	  fprintf (f, "\n");
	}
    }

  if ((flag & LIVEDUMP_EXIT) && live->liveout)
    {
      FOR_EACH_BB (bb)
	{
	  fprintf (f, "\nLive on exit from BB%d : ", bb->index);
	  EXECUTE_IF_SET_IN_BITMAP (live->liveout[bb->index], 0, i,
	    {
	      print_generic_expr (f, partition_to_var (map, i), TDF_SLIM);
	      fprintf (f, "  ");
	    });
	  fprintf (f, "\n");
	}
    }
}

