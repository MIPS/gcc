/* Scalar evolution detector.
   Copyright (C) 2003 Free Software Foundation, Inc.
   Contributed by Sebastian Pop <s.pop@laposte.net>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#ifndef GCC_TREE_SCALAR_EVOLUTION_H
#define GCC_TREE_SCALAR_EVOLUTION_H

/* Debugging macros.  Put an A after the macro definition if you want the 
   debug informations.  */
#define DBG_SDD_S(A) 
#define DBG_SCHEDULE_S(A) 

extern void initialize_scalar_evolutions_analyzer (void);
extern void finalize_scalar_evolutions_analyzer (void);
extern void analyze_scalar_evolutions (struct loops *, struct loop *,
				       varray_type, varray_type);
extern void select_loop_nests_for_scalar_evolutions_analyzer (struct loops *, 
							      varray_type, 
							      varray_type);
extern tree iccp_determine_evolution_function (struct loop *, tree);

/* For a given function with well formed loops, a vector of MONEV_INFOs
   is built.
   
   Example:
   
   |  a_0 = 0
   |  loop 
   |    a_1 = phi (a_0, a_2)
   |    if (a_1 > 9)
   |      goto exit_loop
   |    a_2 = a_1 + 1
   |  endloop
   |  exit_loop:
   
   ___________________________________________________________________
   |var                |  a_0  |   a_1   |  a_2  |        a           |
   |loop_num           |   0   |    1    |   1   |        0           |
   |inner_loops_chrec  | [0,0] |  [0,0]  | [1,1] |{[0,0], +, [1,1]}_1 |
   |outer_loops_chrec  | [0,0] | [10,10] | [1,1] |{[0,0], +, [1,1]}_1 |
   -------------------------------------------------------------------
   
   A loop phi node (a_1) has two visible values: (see "get_chrec")
   - one exposed to the statements in the inner loops, 
   - the other is seen by the statements after the loop.
   The loop_num is used only for versions of a variable.  For the main 
   variable it is 0.  
*/

struct monev_info_str {
  int loop_num;
  tree var;
  tree inner_loops_chrec;
  tree outer_loops_chrec;
};

#define MI_LOOP_NUM(MI) MI->loop_num
#define MI_VAR(MI) MI->var
#define MI_INNER_LOOPS_CHREC(MI) MI->inner_loops_chrec
#define MI_OUTER_LOOPS_CHREC(MI) MI->outer_loops_chrec

/* This structure defines the elements that are analyzed by the induction 
   variable detector: one SCHEDULE_ELT at a time.  
   
   A first function gathers SCHEDULE_ELTs into an imperative list.  
   This list is then sorted in a topological order, such that all the scalar 
   dependences are satisfied.  The scheduler constructs new SCHEDULE_ELTs that 
   are stored into a schedule list in topological order.  
   
   The imperative lists and the schedule lists contain only SCHEDULE_ELTs.  */

struct schedule_elt {
  /* The SCC to be analyzed: an array of SSA_NAMEs to be analyzed at once.  */
  varray_type strongly_connected_component;
  
  /* The loop nest where to analyze the SCC.  */
  struct loop *loop_nest;
};

#define SCHED_SCC(S) S->strongly_connected_component
#define SCHED_LOOP_NEST(S) S->loop_nest




/* This section contains constructors for the objects used by the scalar 
   evolution analyzer.  */

static inline struct monev_info_str *new_monev_info_str (tree);
static inline struct schedule_elt *new_schedule_elt (void);

/* Constructs a new MONEV_INFO_STR structure.  */

static inline struct monev_info_str *
new_monev_info_str (tree var)
{
  struct monev_info_str *res;
  
  res = ggc_alloc (sizeof (struct monev_info_str));
  MI_VAR (res) = var;
  MI_LOOP_NUM (res) = 0;
  MI_INNER_LOOPS_CHREC (res) = chrec_not_analyzed_yet;
  MI_OUTER_LOOPS_CHREC (res) = chrec_not_analyzed_yet;
  
  return res;
}

/* Constructs a new SCHEDULE_ELT structure.  */

static inline struct schedule_elt *
new_schedule_elt (void)
{
  struct schedule_elt *res;
  
  res = ggc_alloc (sizeof (struct schedule_elt));
  SCHED_LOOP_NEST (res) = NULL;
  VARRAY_TREE_INIT (SCHED_SCC (res), 37, "sched_scc");
  
  return res;
}

#endif  /* GCC_TREE_SCALAR_EVOLUTION_H  */
