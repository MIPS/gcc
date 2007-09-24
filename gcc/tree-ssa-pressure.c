/* Calculate and Manage SSA_NAME pressure.
   Copyright (C) 2006 Free Software Foundation, Inc.
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
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "flags.h"
#include "diagnostic.h"
#include "bitmap.h"
#include "tree-flow.h"
#include "hashtab.h"
#include "tree-dump.h"
#include "tree-ssa-live.h"
#include "toplev.h"
#include "tree-ssa-pressure.h"


/* Initialize pressure unit object PTR with NUM stmt's.  */

static void
init_pressure_unit (pressure_unit_p ptr, unsigned num)
{
  ptr->bb_start_pressure = XCNEWVEC (int, n_basic_blocks);
  ptr->stmt_pressure = XCNEWVEC (struct stmt_pressure_unit_d, num);
  ptr->uid_limit = num;
}


/* Clear pressure unit PTR, freeing any memory associated with it.  */

static void
clear_pressure_unit (pressure_unit_p ptr)
{
  unsigned x;

  for (x = 0; x < ptr->uid_limit; x++)
    if (ptr->stmt_pressure[x].dead_partitions)
      VEC_free (int, heap, ptr->stmt_pressure[x].dead_partitions);

  free (ptr->stmt_pressure);
  free (ptr->bb_start_pressure);
}


/* Create a new pressure table with UID_LIMIT stmt's and using FLAGS to select
   functional units.  */

static pressure_table_p
new_pressure_table (int uid_limit, int flags)
{
  pressure_table_p tab;
  int unit_count = 0;
  int *ftu;
  int x;

  ftu = xmalloc (sizeof (int) * (PRESS_MAX_FLAG + 1));
  memset (ftu, 0xff, sizeof (int) * (PRESS_MAX_FLAG + 1));
  
  if (flags & PRESS_INTEGER)
    ftu[PRESS_INTEGER] = unit_count++;
  if (flags & PRESS_FLOAT)
    ftu[PRESS_FLOAT] = unit_count++;
  
  tab = xmalloc (sizeof (struct pressure_table_d) 
		  + sizeof (struct pressure_unit_d) * (unit_count - 1));

  tab->flags = flags;
  tab->unit_count = unit_count;
  tab->flag_to_unit = ftu;

  for (x = 0; x < unit_count; x++)
    init_pressure_unit (&(tab->unit[x]), uid_limit);
  
  return tab;
}


/* Delete pressure table TAB, freeing all associated memory.  */

void
delete_pressure_table (pressure_table_p tab)
{
  int x;

  for (x = 0; x < tab->unit_count; x++)
    clear_pressure_unit (&(tab->unit[x]));
  
  delete_tree_live_info (tab->live_info);
  if (tab->owns_map)
    delete_var_map (tab->map);
  free (tab->flag_to_unit);
  free (tab);
}


/* Get stmt pressure info for functional unit UNIT in STMT from table PTR.  */

static inline stmt_pressure_unit_p
get_stmt_pressure_from_table (tree stmt, pressure_table_p ptr, int unit)
{
  unsigned uid = stmt_ann (stmt)->uid;
  pressure_unit_p p;

  gcc_assert (unit < ptr->unit_count);
  
  p = &(ptr->unit[unit]);
  gcc_assert (uid < p->uid_limit);
  return &(p->stmt_pressure[uid]);
}



/* Add SSA_VAR to MAP if it is a type specified in FLAGS.  */

static void
register_ssa_name (var_map map, tree ssa_var, int flags)
{
  /* Checking to see if its a partition already is a lot cheaper than 
     determining the type first.  */
  if (var_to_partition (map, ssa_var) != NO_PARTITION)
    {
      if (unit_type (ssa_var) & flags)
	register_ssa_partition (map, ssa_var);
    }
}



/* Add PARTITION to thr dead list of SP if it isn't already there.  */

static void
add_to_dead_list (int partition, stmt_pressure_unit_p sp)
{
  if (sp->dead_partitions == NULL)
    sp->dead_partitions = VEC_alloc (int, heap, 3);
  else
    if (in_dead_list_p (partition, sp))
      return;

  VEC_safe_push (int, heap, sp->dead_partitions, partition);
}


/* Process a use of PARTITION in STMT.  If it is already in LIVE, then no action
   needs to be taken.  Otherwise this represents the last use of the partition.
   That means the partition is dead at this stmt, and should be added to the 
   live list (since the block is being processed in reverse.  TAB is the 
   pressure table.  */
   

static inline void
maybe_goes_dead (int partition, tree stmt, bitmap live, pressure_table_p tab)
{
  pressure_unit_p press;
  stmt_pressure_unit_p ptr;

  /* If its already live, then it doesn't go dead at this stmt.  */
  if (bitmap_bit_p (live, partition))
    return;

  bitmap_set_bit (live, partition);
  press = get_partition_unit (partition, tab);
  ptr = get_stmt_pressure (stmt, press);
  ptr->pressure_change -= 1;
  add_to_dead_list (partition, ptr);
}


/* Process a def of PARTITION in STMT.  If the def is dead (has no uses) it
   will not be in LIVE.  In any case, it must be removed from LIVE since this 
   marks the beginning of a live range, and the block is being stepped through
   in reverse.  TAB is the pressure table.  */

static inline void
comes_alive (int partition, tree stmt, bitmap live, pressure_table_p tab) 
{
  pressure_unit_p press = get_partition_unit (partition, tab);
  stmt_pressure_unit_p ptr = get_stmt_pressure (stmt, press);

  if (bitmap_bit_p (live, partition))
    {
      bitmap_clear_bit (live, partition);
      ptr->pressure_change += 1;
    }
  else
    add_to_dead_list (partition, ptr);
}


/* Main entry point which creates a pressure table for the functional units
   specified in FLAGS.  */

pressure_table_p
calculate_name_pressure (int flags, var_map vmap)
{
  var_map map;
  tree var, stmt, phi;
  tree_live_info_p live_info;
  bitmap live;
  pressure_table_p press;
  basic_block bb;
  block_stmt_iterator bsi;
  ssa_op_iter iter;
  int i;
  unsigned uid = 0;
  unsigned x;
  bitmap_iterator bi;

  if (vmap == NULL)
    {
      map = init_var_map (num_ssa_names + 1);

      /* register all the partitions the pressure table cares about.  */
      FOR_EACH_BB (bb)
	{
	  for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	    {
	      if (!is_gimple_reg (PHI_RESULT (phi)))
		continue;

	      register_ssa_name (map, PHI_RESULT (phi), flags);
	      for (i = 0; i < PHI_NUM_ARGS (phi); i++)
		{
		  var = PHI_ARG_DEF (phi, i);
		  if (TREE_CODE (var) == SSA_NAME)
		    register_ssa_name (map, var, flags);
		}
	    }

	  for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	    {
	      stmt = bsi_stmt (bsi);
	      stmt_ann (stmt)->uid = uid++;
	      FOR_EACH_SSA_TREE_OPERAND (var, stmt, iter, 
					 (SSA_OP_DEF|SSA_OP_USE))
		register_ssa_name (map, var, flags);
	    }
	}
    }
  else
    {
      map = vmap;
      /* Calculate a UID for each stmt.  */
      FOR_EACH_BB (bb)
	for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	  stmt_ann (bsi_stmt (bsi))->uid = uid++;
    }

  /* Now view the partition normally, with all unused ssa_names removed.  */
  partition_view_normal (map, false);

  press = new_pressure_table (uid, flags);
  press->map = map;
  press->owns_map = (vmap == NULL);

  /* calculate the live range info.  */
  live_info = calculate_live_ranges (map);
  calculate_dead_on_exit (live_info);
  press->live_info = live_info;


  /* Walk through each basic block in reversem tracking which partitions are
     live, and creating entries in the pressure table for each stmt.  */
  live = BITMAP_ALLOC (NULL);
  FOR_EACH_BB (bb)
    {
      int p;

      /* Walk each stmt.  */
      bitmap_copy (live, live_on_exit (live_info, bb));
      for (bsi = bsi_last (bb); !bsi_end_p (bsi); bsi_prev (&bsi))
	{
	  stmt = bsi_stmt (bsi);
	  FOR_EACH_SSA_TREE_OPERAND (var, stmt, iter, SSA_OP_DEF)
	    if ((p = var_to_partition (map, var)) != NO_PARTITION)
	      comes_alive (p, stmt, live, press);
	  FOR_EACH_SSA_TREE_OPERAND (var, stmt, iter, SSA_OP_USE)
	    if ((p = var_to_partition (map, var)) != NO_PARTITION)
	      maybe_goes_dead (p, stmt, live, press);
	}

      /* Calculate starting register pressure in each block for each unit. */
      EXECUTE_IF_SET_IN_BITMAP (live_on_entry (live_info, bb), 0, x, bi)
        {
	  pressure_unit_p p = get_partition_unit (x, press);
	  p->bb_start_pressure[bb->index]++;
	}

      /* Add PHI results to the starting register pressure since they map to
         copies on incoming edges during out-of-ssa.  */
      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
        {
	  int part = var_to_partition (map, PHI_RESULT (phi));
	  if (part != NO_PARTITION)
	    {
	      pressure_unit_p p = get_partition_unit (part, press);
	      p->bb_start_pressure[bb->index]++;
	    }
	}
    }
  BITMAP_FREE (live);

  return press;
}


/* Dump table PRESS to output file F in a readable form.  */

void
dump_ssa_pressure (FILE *f, pressure_table_p press)
{
  basic_block bb;
  block_stmt_iterator bsi;
  tree stmt, phi, var;
  int i;
  unsigned x;
  stmt_pressure_unit_p sp;
  bitmap_iterator bi;
  int *curr_press;
  
  curr_press = alloca (press->unit_count * sizeof (int));
  FOR_EACH_BB (bb)
    {
      for (i = 0; i < press->unit_count; i++)
        {
	  curr_press[i] = get_start_unit_pressure (press, bb, i);
	  fprintf (f, "[%3d] ", curr_press[i]);
	}
      fprintf (f, "Basic Block %d.  Live on entry: ",
	       bb->index);

      EXECUTE_IF_SET_IN_BITMAP (live_on_entry (press->live_info, bb), 0, x, bi)
        {
	  print_generic_expr (f, partition_to_var (press->map, x), TDF_SLIM);
	  fprintf(f, "  ");
	}
      fprintf(f, "\n");
      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
        {
	  for (i = 0; i < press->unit_count; i++)
	    fprintf (f, "      ");
	  if (var_to_partition (press->map, PHI_RESULT (phi)) == NO_PARTITION)
	    print_generic_stmt (f, phi, TDF_SLIM);
	  else
	    {
	      print_generic_expr (f, PHI_RESULT(phi), TDF_SLIM);
	      fprintf (f, " = PHI <");
	      for (i = 0; i < PHI_NUM_ARGS (phi); i++)
		{
		  basic_block  bb_src= PHI_ARG_EDGE (phi, i)->src;
		  if (i != 0)
		    fprintf (f, ", ");
		  var = PHI_ARG_DEF (phi, i);
		  print_generic_expr (f, var, TDF_SLIM);
		  if (TREE_CODE (var) == SSA_NAME && 
		      var_to_partition (press->map, var) != NO_PARTITION)
		    {
		      if (bitmap_bit_p (dead_on_exit(press->live_info, bb_src), 
					var_to_partition (press->map, var)))
			fprintf (f, "##");
		    }
		  fprintf (f, "(%1d)", bb_src->index);
		}
	      fprintf (f, ">;\n");
	    }
	}

      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
	  bool dead_printed = false;
	  stmt = bsi_stmt (bsi);
	  for (i = 0; i < press->unit_count; i++)
	    {
	      sp = get_stmt_pressure_from_table (stmt, press, i);
	      curr_press[i] += sp->pressure_change;
	      fprintf (f, "[%3d] ", curr_press[i]);
	    }
	  print_generic_expr (f, stmt, TDF_SLIM);
	  for (i = 0; i < press->unit_count; i++)
	    {
	      sp = get_stmt_pressure_from_table (stmt, press, i);
	      if (sp->dead_partitions)
		{
		  if (!dead_printed)
		    {
		      fprintf (f, "       ##dead list## - ");
		      dead_printed = true;
		    }
		  for (x = 0; x < VEC_length (int, sp->dead_partitions); x++)
		    {
		      if (x != 0)
			fprintf (f, ", ");
		      print_generic_expr (f, 
					  partition_to_var (press->map, 
					       VEC_index (int, 
							  sp->dead_partitions, 
							  x)),
					  TDF_SLIM);
		    }
		}
	    }
	  fprintf (f, "\n");
	}

      fprintf (f, "Dead on exit : ");
      EXECUTE_IF_SET_IN_BITMAP (dead_on_exit (press->live_info, bb), 0, x, bi)
        {
	  print_generic_expr (f, partition_to_var (press->map, x), TDF_SLIM);
	  fprintf(f, "  ");
	}
      fprintf (f, "\n\n");
    }
}
