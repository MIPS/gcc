/* Gimple IR extentions for OpenACC support.

   Copyright (C) 2013 Free Software Foundation, Inc.
   Contributed by Evgeny Gavrin <e.gavrin@samsung.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GIMPLE_OACC_H
#define	GIMPLE_OACC_H

#define CASE_GIMPLE_OACC \
    case GIMPLE_OACC_PARALLEL: \
    case GIMPLE_OACC_KERNELS: \
    case GIMPLE_OACC_DATA: \
    case GIMPLE_OACC_CACHE: \
    case GIMPLE_OACC_WAIT: \
    case GIMPLE_OACC_HOST_DATA: \
    case GIMPLE_OACC_LOOP: \
    case GIMPLE_OACC_DECLARE: \
    case GIMPLE_OACC_UPDATE: \
    case GIMPLE_OACC_COMPUTE_REGION_END: \
    case GIMPLE_OACC_DATA_REGION_END

#define CASE_GIMPLE_OACC_ALTER_CF \
    case GIMPLE_OACC_PARALLEL: \
    case GIMPLE_OACC_KERNELS: \
    case GIMPLE_OACC_DATA: \
    case GIMPLE_OACC_HOST_DATA: \
    case GIMPLE_OACC_COMPUTE_REGION_END: \
    case GIMPLE_OACC_DATA_REGION_END

#define CASE_GIMPLE_OACC_NOT_ALTER_CF \
    case GIMPLE_OACC_CACHE: \
    case GIMPLE_OACC_WAIT: \
    case GIMPLE_OACC_LOOP: \
    case GIMPLE_OACC_DECLARE: \
    case GIMPLE_OACC_UPDATE


#define GIMPLE_OACC_CHILD_FN(g)  ((gimple_code(g) == GIMPLE_OACC_KERNELS) ? \
                                  gimple_oacc_kernels_child_fn(g) : \
                                  (gimple_code(g) == GIMPLE_OACC_PARALLEL) ? \
                                  gimple_oacc_parallel_child_fn(g) : NULL_TREE)

#define GIMPLE_OACC_SET_CHILD_FN(g, f) ((gimple_code(g) == GIMPLE_OACC_KERNELS)? \
                                    gimple_oacc_kernels_set_child_fn(g,f) :\
                                  (gimple_code(g) == GIMPLE_OACC_PARALLEL) ? \
                                  gimple_oacc_parallel_set_child_fn(g,f) \
                                  : (void)0)

#define GIMPLE_OACC_PARAMS_PTR(g) ((gimple_code(g) == GIMPLE_OACC_KERNELS) ? \
                                  gimple_oacc_kernels_params_ptr(g) : \
                                  (gimple_code(g) == GIMPLE_OACC_PARALLEL) ? \
                                  gimple_oacc_parallel_params_ptr(g) : 0)

#define GIMPLE_OACC_CLAUSES(g)   ((gimple_code(g) == GIMPLE_OACC_KERNELS) ? \
                                  gimple_oacc_kernels_clauses(g) : \
                                 (gimple_code(g) == GIMPLE_OACC_PARALLEL) ? \
                                  gimple_oacc_parallel_clauses(g) : \
                                 (gimple_code(g) == GIMPLE_OACC_DATA) ? \
                                 gimple_oacc_data_clauses(g) : \
                                 (gimple_code(g) == GIMPLE_OACC_LOOP) ? \
                                 gimple_oacc_loop_clauses(g) : NULL_TREE)
#define GIMPLE_OACC_STATEMENT(g) \
                        ((gimple_code(g) == GIMPLE_OACC_COMPUTE_REGION_END) ? \
                         gimple_oacc_compute_region_end_statement(g) : \
                        (gimple_code(g) == GIMPLE_OACC_DATA_REGION_END) ? \
                         gimple_oacc_data_region_end_statement(g) : NULL)

#define GIMPLE_OACC_SET_STATEMENT(g,s) \
                        ((gimple_code(g) == GIMPLE_OACC_COMPUTE_REGION_END) ? \
                         gimple_oacc_compute_region_end_set_statement(g,s) : \
                        (gimple_code(g) == GIMPLE_OACC_DATA_REGION_END) ? \
                         gimple_oacc_data_region_end_set_statement(g,s) : (void)0)

extern bool is_gimple_oacc (const_gimple);

extern void gimple_oacc_set_body (gimple, gimple_seq);
extern gimple_seq *gimple_oacc_body_ptr (gimple);
extern gimple_seq gimple_oacc_body (gimple);
extern unsigned gimple_oacc_nparams(const_gimple);

/* KERNELS */
extern tree gimple_oacc_kernels_clauses (const_gimple);
extern tree *gimple_oacc_kernels_clauses_ptr (gimple);
extern void gimple_oacc_kernels_set_clauses (gimple, tree);
extern tree gimple_oacc_kernels_child_fn (const_gimple);
extern tree *gimple_oacc_kernels_child_fn_ptr (gimple);
extern void gimple_oacc_kernels_set_child_fn (gimple, tree);
extern tree gimple_oacc_kernels_param(gimple, unsigned);
extern tree* gimple_oacc_kernels_params_ptr(gimple);

/* PARALLEL */
extern tree gimple_oacc_parallel_clauses (const_gimple);
extern tree *gimple_oacc_parallel_clauses_ptr (gimple);
extern void gimple_oacc_parallel_set_clauses (gimple, tree);
extern tree gimple_oacc_parallel_child_fn (const_gimple);
extern tree *gimple_oacc_parallel_child_fn_ptr (gimple);
extern void gimple_oacc_parallel_set_child_fn (gimple, tree);
extern tree *gimple_oacc_parallel_params_ptr(gimple);
extern tree gimple_oacc_parallel_param(gimple, unsigned);

/* DATA */
extern tree gimple_oacc_data_clauses (const_gimple);
extern void gimple_oacc_data_set_clauses (gimple, tree);

/* LOOP */
extern tree gimple_oacc_loop_child_fn (const_gimple);
extern tree *gimple_oacc_loop_params_ptr(gimple);
extern tree gimple_oacc_loop_param(gimple, unsigned);
extern void gimple_oacc_loop_set_child_fn (gimple, tree);
extern void gimple_oacc_loop_set_clauses (gimple, tree);
extern tree gimple_oacc_loop_clauses (const_gimple);
extern void gimple_oacc_loop_set_pre_body (gimple, gimple_seq);
extern gimple_seq gimple_oacc_loop_pre_body (const_gimple);
extern void gimple_oacc_loop_set_collapse (gimple, size_t);
extern size_t gimple_oacc_loop_collapse (const_gimple);
extern void gimple_oacc_loop_set_index (gimple, size_t i, tree index);
extern tree gimple_oacc_loop_index (const_gimple, size_t i);
extern void gimple_oacc_loop_set_initial (gimple, size_t i, tree initial);
extern tree gimple_oacc_loop_initial (const_gimple, size_t i);
extern void gimple_oacc_loop_set_final (gimple, size_t i, tree final);
extern tree gimple_oacc_loop_final (const_gimple, size_t i);
extern void gimple_oacc_loop_set_incr (gimple, size_t i, tree incr);
extern tree gimple_oacc_loop_incr (const_gimple, size_t i);
extern void gimple_oacc_loop_set_cond (gimple, size_t i, enum tree_code cond);
extern enum tree_code gimple_oacc_loop_cond (const_gimple, size_t i);

/* REGION END */
extern gimple gimple_oacc_data_region_end_statement (const_gimple);
extern void gimple_oacc_data_region_end_set_statement(gimple, gimple);
extern gimple gimple_oacc_compute_region_end_statement(const_gimple);
extern void gimple_oacc_compute_region_end_set_statement(gimple, gimple);

/* BUILD */
extern gimple gimple_build_oacc_parallel (gimple_seq, tree, tree, tree);
extern gimple gimple_build_oacc_kernels (gimple_seq, tree, tree, tree);
extern gimple gimple_build_oacc_data (gimple_seq, tree, tree, tree);
extern gimple gimple_build_oacc_cache (gimple_seq, tree, tree, tree);
extern gimple gimple_build_oacc_wait (gimple_seq, tree, tree, tree);
extern gimple gimple_build_oacc_host_data (gimple_seq, tree, tree, tree);
extern gimple gimple_build_oacc_loop (gimple_seq, tree, tree, tree);
extern gimple gimple_build_oacc_declare (gimple_seq, tree, tree, tree);
extern gimple gimple_build_oacc_update (gimple_seq, tree, tree, tree);
extern gimple gimple_build_oacc_compute_region_end ();
extern gimple gimpale_build_oacc_data_region_end ();

#endif	/* GIMPLE_OACC_H */
