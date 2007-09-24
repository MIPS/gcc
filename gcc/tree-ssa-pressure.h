/* Header file for calculating and Managing SSA_NAME pressure.
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

#include "vec.h"

#ifndef _TREE_SSA_PRESSURE_H
#define _TREE_SSA_PRESSURE_H


/* This structure represents the changes in register pressure within a stmt.  */

typedef struct stmt_pressure_unit_d
{
  int pressure_change;			/* Change in register pressure.  */
  VEC(int,heap) *dead_partitions;	/* List of dead registers.  */
} *stmt_pressure_unit_p;


/* Dynamic vector used to represent register pressure changes for any
   new stmt's which have been created since the table was created.  */
DEF_VEC_P(stmt_pressure_unit_p);
DEF_VEC_ALLOC_P(stmt_pressure_unit_p,heap);


/* This represents the register pressure in the function for a single register
   type.  */
   
typedef struct pressure_unit_d
{
  unsigned uid_limit;			/* Upper UID of original stmts.  */
  int *bb_start_pressure;		/* Vector of starting pressures.  */
  stmt_pressure_unit_p stmt_pressure;	/* Stmt pressure vector.  */
} *pressure_unit_p;


/* This represents register pressure for the entire function for multiple
   functional units.  It also contains all the live range info.  */
 
typedef struct pressure_table_d
{
  int flags;				/* Flag for selecting units.  */
  int unit_count;			/* Number of functional units.  */
  int *flag_to_unit;			/* Convert flag bit to unit number.  */
  bool owns_map;			/* Created var_map.  */
  var_map map;				/* Partition map.  */
  tree_live_info_p live_info;		/* Live range info.  */
  struct pressure_unit_d unit[1];	/* Stmt pressure units. */
} *pressure_table_p;

extern pressure_table_p calculate_name_pressure (int, var_map);
extern void delete_pressure_table (pressure_table_p);
extern void dump_ssa_pressure (FILE *, pressure_table_p);

/* Funcitonal unit flags.  */

#define PRESS_INTEGER	0x01
#define PRESS_FLOAT	0x02
#define PRESS_MAX_FLAG  (PRESS_FLOAT|PRESS_INTEGER)


/* Return the BB starting pressure for UNIT in TAB.  */

static inline int
get_start_unit_pressure (pressure_table_p tab, basic_block bb, int unit)
{
#ifdef ENABLE_CHECKING
  gcc_assert (unit >= 0 && unit < tab->unit_count);
#endif
  return tab->unit[unit].bb_start_pressure[bb->index];
}


/* Return the BB starting pressure for the unit specified by FLAG in TAB.  */

static inline int
get_start_pressure (pressure_table_p tab, basic_block bb, int flag)
{
  int unit;

#ifdef ENABLE_CHECKING
  /* Make sure 'flags' is in a valid range.  */
  gcc_assert ((flag  & ~PRESS_MAX_FLAG) == 0);
#endif

  unit = tab->flag_to_unit[flag];
  return get_start_unit_pressure (tab, bb, unit);
}



/* Return the flag representing the functional unit for SSA_NAME.  */
static int
unit_type (tree ssa_name)
{
  tree type;
  int ret = 0;
  gcc_assert (TREE_CODE (ssa_name) == SSA_NAME);

  type = TREE_TYPE (SSA_NAME_VAR (ssa_name));

  if (INTEGRAL_TYPE_P (type) || POINTER_TYPE_P (type))
    ret = PRESS_INTEGER;
  else
    if (SCALAR_FLOAT_TYPE_P (type) || COMPLEX_FLOAT_TYPE_P (type))
      ret = PRESS_FLOAT;

  return ret;
}


/* Return the unit number PARTITION belongs to in table PRESS.  */

static inline int
partition_unit (int partition, pressure_table_p press)
{
  tree name = partition_to_var (press->map, partition);
  int ret = unit_type (name);

  gcc_assert (ret != 0);
  return press->flag_to_unit[ret];
}


/* Return the pressure unit pointer which PARITION belongs to in TAB.  */

static pressure_unit_p
get_partition_unit (int partition, pressure_table_p tab)
{
  int u = partition_unit (partition, tab);
  return &(tab->unit[u]);
}


/* Get stmt pressure info from function unit PTR for STMT.  */

static inline stmt_pressure_unit_p
get_stmt_pressure (tree stmt, pressure_unit_p ptr)
{
  unsigned uid = stmt_ann (stmt)->uid;
  gcc_assert (uid < ptr->uid_limit);
  return &(ptr->stmt_pressure[uid]);
}


/* Return TRUE if PARTITION is in the dead list of SP.  */

static inline bool
in_dead_list_p (int partition, stmt_pressure_unit_p sp)
{
  unsigned x;

  for (x = 0; x < VEC_length (int, sp->dead_partitions); x++)
    if (VEC_index (int, sp->dead_partitions, x) == partition)
      return true;

  return false;
}


static inline bool
dead_partition_in_stmt_p (pressure_table_p tab, tree stmt, int partition)
{
  pressure_unit_p unit_p;
  stmt_pressure_unit_p stmt_press;

#ifdef ENABLE_CHECKING
  gcc_assert (partition != NO_PARTITION);
#endif

  unit_p = get_partition_unit (partition, tab);
  stmt_press = get_stmt_pressure (stmt, unit_p);
  return in_dead_list_p (partition, stmt_press);
}

static inline bool
dead_name_in_stmt_p (pressure_table_p tab, tree stmt, tree ssa_name)
{
  int p = var_to_partition (tab->map, ssa_name);
  return dead_partition_in_stmt_p (tab, stmt, p);
}

#endif
