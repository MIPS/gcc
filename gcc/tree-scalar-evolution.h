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
extern tree get_loop_exit_condition (struct loop *);

extern void scev_initialize (struct loops *loops);
extern void scev_finalize (void);
extern tree analyze_scalar_evolution (unsigned, tree);
extern tree instantiate_parameters (unsigned, tree);
extern void eliminate_redundant_checks (void);


struct scev_info_str {
  tree var;
  tree inner_loops_chrec;
  tree outer_loops_chrec;
};

#define MI_VAR(MI) MI->var
#define MI_INNER_LOOPS_CHREC(MI) MI->inner_loops_chrec
#define MI_OUTER_LOOPS_CHREC(MI) MI->outer_loops_chrec

/* Constructs a new SCEV_INFO_STR structure.  */

static inline struct scev_info_str *
new_scev_info_str (tree var)
{
  struct scev_info_str *res;
  
  res = ggc_alloc (sizeof (struct scev_info_str));
  MI_VAR (res) = var;
  MI_INNER_LOOPS_CHREC (res) = chrec_not_analyzed_yet;
  MI_OUTER_LOOPS_CHREC (res) = chrec_not_analyzed_yet;
  
  return res;
}

#endif  /* GCC_TREE_SCALAR_EVOLUTION_H  */
