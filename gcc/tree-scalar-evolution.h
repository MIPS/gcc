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

extern tree number_of_iterations_in_loop (struct loop *);
extern void number_of_iterations_for_all_loops (varray_type *);
extern tree get_loop_exit_condition (struct loop *);

extern tree analyze_scalar_evolution (unsigned, tree);
extern tree instantiate_parameters (unsigned, tree);

extern void eliminate_redundant_checks (void);


/* For a given function with well formed loops, a vector of SCEV_INFOs
   is built.
   
   Example:
   
   |  a_0 = 0
   |  loop_1
   |    a_1 = phi (a_0, a_2)
   |    if (a_1 > 9)
   |      goto exit_loop
   |    a_2 = a_1 + 1
   |  endloop
   |  exit_loop:
   
   ___________________________________________________________________
   |var                |  a_0  |   a_1   |  a_2  |        a           |
   |loop_num           |   0   |    1    |   1   |        0           |
   |inner_loops_chrec  |   0   |    0    |   1   |    {0, +, 1}_1     |
   |outer_loops_chrec  |   0   |   10    |  nay  |    {0, +, 1}_1     |
   -------------------------------------------------------------------
   
   A loop phi node (a_1) has two visible values: (see the comments in
   the "get_chrec" function)
   - a value exposed to the statements in the inner loops, 
   - the other value is seen by the statements after the loop.
   
   The loop_num is used only for versions of a variable.  For the main 
   variable it is always equal to 0.  
*/

struct scev_info_str {
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
};

#define SCHED_SCC(S) S->strongly_connected_component



/* This section contains constructors for the objects used by the scalar 
   evolution analyzer.  */

static inline struct scev_info_str *new_scev_info_str (tree);
static inline struct schedule_elt *new_schedule_elt (void);

/* Constructs a new SCEV_INFO_STR structure.  */

static inline struct scev_info_str *
new_scev_info_str (tree var)
{
  struct scev_info_str *res;
  
  res = ggc_alloc (sizeof (struct scev_info_str));
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
  VARRAY_TREE_INIT (SCHED_SCC (res), 37, "sched_scc");
  
  return res;
}

#endif  /* GCC_TREE_SCALAR_EVOLUTION_H  */
