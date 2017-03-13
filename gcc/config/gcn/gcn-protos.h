/* Copyright (C) 2016-2017 Free Software Foundation, Inc.

   This file is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3 of the License, or (at your option)
   any later version.

   This file is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef _GCN_PROTOS_
#define _GCN_PROTOS_

extern bool gcn_hard_regno_mode_ok (int regno, machine_mode mode);
extern int gcn_hard_regno_nregs(int regno, enum machine_mode mode);
extern enum reg_class gcn_regno_reg_class (int regno);
extern bool gcn_cannot_change_mode_class (machine_mode, machine_mode, int);
extern int gcn_inline_fp_constant_p (rtx, bool);
extern bool gcn_fp_constant_p (rtx, bool);
extern bool gcn_inline_constant_p (rtx);
extern bool gcn_constant_p (rtx);
extern bool gcn_vgpr_move_p (rtx, rtx);
extern bool gcn_sgpr_move_p (rtx, rtx);

extern bool gcn_regno_mode_code_ok_for_base_p
	 (int, machine_mode, addr_space_t, int, int);
extern reg_class gcn_mode_code_base_reg_class
	 (machine_mode, addr_space_t, int, int);
extern bool regno_ok_for_index_p (int);
extern void print_operand_address (FILE * file, register rtx addr);
extern void print_operand (FILE * file, rtx x, int code);

extern rtx gcn_operand_part (machine_mode, rtx, int);
extern rtx gcn_operand_doublepart (machine_mode, rtx, int);
extern void gcn_split_operands (machine_mode, rtx *, int, int);
extern bool gcn_can_split_p (machine_mode, rtx);
extern bool gcn_can_split_operands_p (machine_mode, rtx *, int);
extern rtx gcn_vec_constant (machine_mode, int);
extern rtx gcn_vec_constant (machine_mode, rtx);
extern bool gcn_expand_mov (machine_mode, rtx, rtx);
extern void gcn_expand_vector_init (rtx, rtx);

extern void gcn_hsa_declare_function_name (FILE *file,
					   const char *name, tree decl);

extern rtx gcn_gen_undef (enum machine_mode);
extern rtx gcn_scalar_exec ();
extern rtx gcn_default_exec ();
extern rtx gcn_full_exec ();
extern rtx gcn_full_exec_reg ();

extern void gcn_expand_prologue ();

extern int gcn_regmove_natural_size (enum machine_mode);
extern unsigned gcn_frame_pointer_regnum ();

#ifdef TREE_CODE
extern void gcn_init_cumulative_args (CUMULATIVE_ARGS *, tree, rtx, tree, int);
#endif

/* Return true if MODE is valid for 1 VGPR register.  */

inline bool
vgpr_1reg_mode_p (enum machine_mode mode)
{
  return mode == SImode || mode == SFmode || mode == HImode
       /*|| mode == V32BImode*/
	 || mode == V64HImode || mode == V64SImode
	 || mode == V64HFmode || mode == V64SFmode || mode == BImode;
}

/* Return true if MODE is valid for 1 SGPR register.  */

static inline bool
sgpr_1reg_mode_p (enum machine_mode mode)
{
  return mode == SImode || mode == SFmode || mode == HImode || mode == BImode
	 /*|| mode == V32BImode*/;
}

#endif
