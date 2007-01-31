/* Instruction scheduling pass.  This file contains interface to 
   the dependency analyzer. 
   Copyright (C) 2006, 2007 Free Software Foundation, Inc.

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
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#ifndef GCC_SCHED_DEPS_H
#define GCC_SCHED_DEPS_H

struct haifa_deps_insn_data
{
  /* The number of incoming edges in the forward dependency graph.
     As scheduling proceeds, counts are decreased.  An insn moves to
     the ready queue when its counter reaches zero.  */
  int dep_count;

  /* Nonzero if instruction has internal dependence
     (e.g. add_dependence was invoked with (insn == elem)).  */
  unsigned int has_internal_dep;
};

struct deps_insn_data
{
  /* A list of insns which depend on the instruction.  Unlike LOG_LINKS,
     it represents forward dependencies.  

     Note that we want to use this in selective scheduling also, so
     it has moved from h_d_i_d.  */
  rtx depend;

  /* A priority for each insn.  */
  int priority;

  /* A cost of this instruction.  */
  short cost;

  /* Nonzero if priority has been computed already.  */
  unsigned int priority_known : 1;

  /* Some insns (e.g. call) are not allowed to move across blocks.  */
  unsigned int cant_move : 1;
};

extern struct haifa_deps_insn_data *h_d_i_d;
extern struct deps_insn_data *d_i_d;

#define DID(INSN) (d_i_d[INSN_LUID (INSN)])
#define HDID(INSN) (h_d_i_d[INSN_LUID (INSN)])

#define INSN_DEPEND(INSN)	(DID (INSN).depend)
#define INSN_DEP_COUNT(INSN)	(HDID (INSN).dep_count)
#define HAS_INTERNAL_DEP(INSN)  (HDID (INSN).has_internal_dep)
#define CANT_MOVE(INSN)		(DID (INSN).cant_move)

struct sched_deps_info_def
{
  /* Called when computing dependencies for a JUMP_INSN.  This function
     should store the set of registers that must be considered as set by
     the jump in the regset.  */
  void (*compute_jump_reg_dependencies) (rtx, regset, regset, regset);

  /* Start analyzing insn.  */
  void (*start_insn) (rtx);

  /* Finish analyzing insn.  */
  void (*finish_insn) (void);

  /* Start analyzing insn subexpression.  */
  void (*start_x) (rtx);

  /* Finish analyzing insn subexpression.  */
  void (*finish_x) (void);

  /* Start analyzing insn LHS (Left Hand Side).  */
  void (*start_lhs) (rtx);

  /* Finish analyzing insn LHS.  */
  void (*finish_lhs) (void);

  /* Start analyzing insn RHS (Right Hand Side).  */
  void (*start_rhs) (rtx);

  /* Finish analyzing insn RHS.  */
  void (*finish_rhs) (void);

  /* Note set of the register.  */
  void (*note_reg_set) (int);

  /* Note clobber of the register.  */
  void (*note_reg_clobber) (int);

  /* Note use of the register.  */
  void (*note_reg_use) (int);

  /* Note memory dependence of type DS between MEM1 and MEM2 (which is
     in the INSN2).  */
  void (*note_mem_dep) (rtx mem1, rtx mem2, rtx insn2, ds_t ds);

  /* Note a dependence of type DS from the INSN.  */
  void (*note_dep) (rtx insn, ds_t ds);

  /* Nonzero if we should use cselib for better alias analysis.  This
     must be 0 if the dependency information is used after sched_analyze
     has completed, e.g. if we're using it to initialize state for successor
     blocks in region scheduling.  */
  unsigned int use_cselib : 1;

  /* If set, generate links between instruction as DEPS_LIST.
     Otherwise, generate usual INSN_LIST links.  */
  unsigned int use_deps_list : 1;

  /* Generate data and control speculative dependencies.
     Requires USE_DEPS_LIST set.  */
  unsigned int generate_spec_deps : 1;
};

extern struct sched_deps_info_def *sched_deps_info;

extern bool sched_insns_conditions_mutex_p (rtx, rtx);
extern void maybe_add_or_update_back_dep (rtx, rtx, enum reg_note);
extern void deps_analyze_insn (struct deps *, rtx);
extern void sched_analyze (struct deps *, rtx, rtx);
extern void init_deps (struct deps *);
extern void free_deps (struct deps *);
extern void init_deps_global (void);
extern void finish_deps_global (void);
extern void add_forw_dep (rtx, rtx);
extern void compute_forward_dependences (rtx, rtx);
extern rtx find_insn_list (rtx, rtx);
extern enum DEPS_ADJUST_RESULT add_or_update_back_dep (rtx, rtx, 
						       enum reg_note, ds_t);
extern void add_or_update_back_forw_dep (rtx, rtx, enum reg_note, ds_t);
extern void add_back_forw_dep (rtx, rtx, enum reg_note, ds_t);
extern void delete_back_forw_dep (rtx, rtx);
extern dw_t get_dep_weak (ds_t, ds_t);
extern ds_t set_dep_weak (ds_t, ds_t, dw_t);
extern ds_t ds_merge (ds_t, ds_t);

extern void sched_deps_local_init (bool);
extern void sched_deps_local_finish (void);

extern void haifa_note_reg_set (int);
extern void haifa_note_reg_clobber (int);
extern void haifa_note_reg_use (int);

extern void deps_start_bb (struct deps *, rtx);
extern enum reg_note ds_to_dt (ds_t);

#endif /* GCC_SCHED_DEPS_H */
