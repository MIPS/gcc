/* Instruction scheduling pass.  Log dumping infrastructure.  
   Copyright (C) 2006, 2007, 2008 Free Software Foundation, Inc.

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


#ifndef GCC_SEL_SCHED_DUMP_H
#define GCC_SEL_SCHED_DUMP_H

#include "sel-sched-ir.h"
/* Debug and dumping functions.  */

/* Dump only current region.  */
#define SEL_DUMP_CFG_CURRENT_REGION (2)
/* Dump note_list for each bb.  */
#define SEL_DUMP_CFG_BB_NOTES_LIST (4)
/* Dump av_set from the bb_header.  */
#define SEL_DUMP_CFG_AV_SET (8)
#define SEL_DUMP_CFG_LV_SET (16)
#define SEL_DUMP_CFG_BB_INSNS (32)
#define SEL_DUMP_CFG_FENCES (64)
#define SEL_DUMP_CFG_INSN_SEQNO (128)
#define SEL_DUMP_CFG_INSN_FLAGS (0)
#define SEL_DUMP_CFG_FUNCTION_NAME (256)
#define SEL_DUMP_CFG_BB_LIVE (512)
#define SEL_DUMP_CFG_BB_LOOP (1024)
/* The default flags for cfg dumping.  */
#define SEL_DUMP_CFG_FLAGS (SEL_DUMP_CFG_CURRENT_REGION \
			    | SEL_DUMP_CFG_BB_NOTES_LIST \
                            | SEL_DUMP_CFG_AV_SET \
                            | SEL_DUMP_CFG_LV_SET \
			    | SEL_DUMP_CFG_BB_INSNS \
                            | SEL_DUMP_CFG_FENCES \
                            | SEL_DUMP_CFG_INSN_SEQNO \
                            | SEL_DUMP_CFG_INSN_FLAGS \
			    | SEL_DUMP_CFG_BB_LOOP)

enum _dump_insn_rtx
  {
    DUMP_INSN_RTX_UID = 2,
    DUMP_INSN_RTX_PATTERN = 4,
    DUMP_INSN_RTX_BBN = 8,

    DUMP_INSN_RTX_ALL = (DUMP_INSN_RTX_UID | DUMP_INSN_RTX_PATTERN
			 | DUMP_INSN_RTX_BBN)
  };

extern void dump_insn_rtx_1 (rtx, int);
extern void dump_insn_rtx (rtx);
extern void debug_insn_rtx (rtx);

enum _dump_idata
  {
    DUMP_IDATA_TYPE = 2,
    DUMP_IDATA_LHS = 4,
    DUMP_IDATA_RHS = 8,
    DUMP_IDATA_REG_SETS = 16,
    DUMP_IDATA_REG_USES = 32,

    DUMP_IDATA_ALL = (DUMP_IDATA_TYPE | DUMP_IDATA_LHS | DUMP_IDATA_RHS
		      | DUMP_IDATA_REG_SETS | DUMP_IDATA_REG_USES)
  };

extern void dump_idata_1 (idata_t id, int);
extern void dump_idata (idata_t id);
extern void debug_idata (idata_t id);

enum _dump_vinsn
  {
    DUMP_VINSN_INSN_RTX = 2,
    DUMP_VINSN_TYPE = 4,
    DUMP_VINSN_COUNT = 8,
    DUMP_VINSN_COST = 16,

    DUMP_VINSN_ALL = (DUMP_VINSN_INSN_RTX | DUMP_VINSN_TYPE | DUMP_VINSN_COUNT
		      | DUMP_VINSN_COST)
  };

extern void dump_vinsn_1 (vinsn_t, int);
extern void dump_vinsn (vinsn_t);
extern void debug_vinsn (vinsn_t);

enum _dump_expr
  {
    DUMP_EXPR_VINSN = 2,
    DUMP_EXPR_SPEC = 4,
    DUMP_EXPR_PRIORITY = 8,
    DUMP_EXPR_SCHED_TIMES = 16,
    DUMP_EXPR_SPEC_DONE_DS = 32,
    DUMP_EXPR_ORIG_BB = 64,
    DUMP_EXPR_USEFULNESS = 128,

    DUMP_EXPR_ALL = (DUMP_EXPR_VINSN | DUMP_EXPR_SPEC | DUMP_EXPR_PRIORITY
		     | DUMP_EXPR_SCHED_TIMES | DUMP_EXPR_SPEC_DONE_DS
		     | DUMP_EXPR_ORIG_BB | DUMP_EXPR_USEFULNESS)
  };

extern void dump_expr_1 (expr_t, int);
extern void dump_expr (expr_t);
extern void debug_expr (expr_t);

/* A enumeration for dumping flags of an insn.  */
enum _dump_insn
{
  DUMP_INSN_ASM_P = 2,
  DUMP_INSN_SCHED_NEXT = 4,
  DUMP_INSN_EXPR = 8,
  DUMP_INSN_AV = 16,
  DUMP_INSN_SEQNO = 32,
  DUMP_INSN_AFTER_STALL_P = 64,
  DUMP_INSN_SCHED_CYCLE = 128,
  DUMP_INSN_UID = 256,
  DUMP_INSN_BBN = 512,
  DUMP_INSN_PATTERN = 1024,

  DUMP_INSN_ALL = (DUMP_INSN_ASM_P | DUMP_INSN_SCHED_NEXT | DUMP_INSN_EXPR
		   | DUMP_INSN_AV | DUMP_INSN_SEQNO | DUMP_INSN_AFTER_STALL_P
		   | DUMP_INSN_SCHED_CYCLE | DUMP_INSN_UID | DUMP_INSN_BBN
		   | DUMP_INSN_PATTERN)
};

extern void dump_insn_1 (insn_t, int);
extern void dump_insn (insn_t);
extern void debug_insn (insn_t);

extern void sel_prepare_string_for_dot_label (char *);
/* When this flag is on, we are dumping to the .dot file.
   When it is off, we are dumping to log.  */
extern bool sched_dump_to_dot_p;

#define sel_print_to_dot(...)                           \
  do {                                                  \
    int __j = 1 + 2 * snprintf (NULL, 0, __VA_ARGS__);  \
    char *__s = XALLOCAVEC (char, __j);                 \
    snprintf (__s, __j, __VA_ARGS__);                   \
    sel_prepare_string_for_dot_label (__s);             \
    fprintf (sched_dump, "%s", __s);                    \
  } while (0)

#define sel_print(...)					\
  do {							\
    if (sched_dump_to_dot_p)                            \
      sel_print_to_dot (__VA_ARGS__);                   \
    else                                                \
      fprintf (sched_dump, __VA_ARGS__);                \
  } while (0)


/* Functions from sel-sched-dump.c.  */
extern const char * sel_print_insn (const_rtx, int);
extern void free_sel_dump_data (void);

extern void block_start (void);
extern void block_finish (void);
extern int get_print_blocks_num (void);
extern void line_start (void);
extern void line_finish (void);
extern void print_marker_to_log (void);

extern void sel_print_rtl (rtx x);
extern void dump_insn_1 (insn_t, int);
extern void dump_insn (insn_t);
extern void dump_insn_vector (rtx_vec_t);
extern void dump_expr (expr_t);
extern void dump_used_regs (bitmap);
extern void dump_av_set (av_set_t);
extern void dump_lv_set (regset);
extern void dump_blist (blist_t);
extern void dump_flist (flist_t);
extern void dump_hard_reg_set (const char *, HARD_REG_SET);
extern void sel_debug_cfg_1 (int);
extern void sel_debug_cfg (void);
extern void setup_dump_cfg_params (void);

/* Debug functions.  */
extern int insn_uid (rtx);
extern basic_block block_for_insn (rtx insn);
extern av_set_t bb_av_set (basic_block bb);
extern rtx insn_pattern (rtx);
extern int insn_code (rtx);
extern bool insn_is_set_p (rtx);
extern int hard_regno_rename_ok (int, int);

extern void debug_expr (expr_t);
extern void debug_av_set (av_set_t);
extern void debug_lv_set (regset);
extern void debug_ilist (ilist_t);
extern void debug_blist (blist_t);
extern void debug_insn_vector (rtx_vec_t);
extern void debug_hard_reg_set (HARD_REG_SET);
extern rtx debug_mem_addr_value (rtx);
extern bool debug_find_insn_loop (void);
extern bool debug_find_unreachable_blocks (void);
extern void sel_debug_cfg (void);
extern void mem_test (int);
extern bool in_range_p (int, const char *, bool *);
extern bool av_set_contains_insn_with_uid (av_set_t av, int insn_uid);
extern av_set_t av_set_for_bb_n (int n);
extern FILE *sel_get_stat_file (void);

#endif
