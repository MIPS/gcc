/* Instruction scheduling pass.  Log dumping infrastructure.  

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
/* The default flags for cfg dumping.  */
#define SEL_DUMP_CFG_FLAGS (SEL_DUMP_CFG_CURRENT_REGION \
			    | SEL_DUMP_CFG_BB_NOTES_LIST \
                            | SEL_DUMP_CFG_AV_SET \
                            | SEL_DUMP_CFG_LV_SET \
			    | SEL_DUMP_CFG_BB_INSNS \
                            | SEL_DUMP_CFG_FENCES \
                            | SEL_DUMP_CFG_INSN_SEQNO \
                            | SEL_DUMP_CFG_INSN_FLAGS)

/* A enumeration for dumping flags of an insn.  */
enum dump_insn_enum
{ 
  DUMP_INSN_UID = 1, 
  DUMP_INSN_BBN = 2, 
  DUMP_INSN_SEQNO = 4,
  DUMP_INSN_PATTERN = 8, 
  DUMP_INSN_COUNT = 16, 
  DUMP_INSN_CYCLE = 32 
};


extern void sel_prepare_string_for_dot_label (char *);
/* When this flag is on, we are dumping to the .dot file.
   When it is off, we are dumping to log.  */
extern bool sched_dump_to_dot_p;

extern int indent;
extern bool print_block;
extern bool new_line;
#define print_to_dot(...) \
  do { \
    int ____j = 1 + 2 * snprintf (NULL, 0, __VA_ARGS__); \
    char *____s = xmalloc (____j * sizeof (*____s)); \
    snprintf (____s, ____j, __VA_ARGS__); \
    sel_prepare_string_for_dot_label (____s); \
    fprintf (sched_dump, "%s", ____s); \
    free (____s); \
  } while (0)

#define print1(...) \
  do { \
    if (sched_dump_to_dot_p) \
      print_to_dot (__VA_ARGS__); \
    else \
      fprintf (sched_dump, __VA_ARGS__); \
  } while (0)

#define print(...)					\
  do {							\
    if (print_block)					\
      {							\
	int ____i;					\
							\
	for (____i = 0; ____i < indent; ____i++)	\
	  print1 ("  ");			        \
							\
	print1 (__VA_ARGS__);		                \
							\
	if (new_line)					\
	  print1 ("\n");			        \
      }							\
  } while (0)


/* Functions from sel-sched-dump.c.  */
extern const char * sel_print_insn (rtx, int);
extern void free_sel_dump_data (void);
extern void setup_sched_dump_to_stderr (void);

extern void block_start (void);
extern void block_finish (void);
extern void line_start (void);
extern void line_finish (void);
extern void print_marker_to_log (void);

extern void sel_print_rtl (rtx x);
extern void dump_insn_1 (insn_t, int);
extern void dump_insn (insn_t);
extern void dump_insn_array (rtx *, int);
extern void dump_rhs (rhs_t);
extern void dump_used_regs (bitmap);
extern void dump_av_set (av_set_t);
extern void dump_lv_set (regset);
extern void dump_blist (blist_t);
extern void dump_flist (flist_t);
extern void dump_insn_array (rtx *, int);
extern void dump_hard_reg_set (const char *, HARD_REG_SET);
extern void sel_dump_cfg_insn (insn_t, int);
extern void sel_dump_cfg_edge (FILE *, edge);
extern void sel_dump_cfg_2 (FILE *, int);
extern void sel_dump_cfg_1 (const char *, int);
extern void sel_dump_cfg (const char *);
extern void sel_debug_cfg_1 (int);
extern void sel_debug_cfg (void);
extern void setup_dump_cfg_params (bool);

/* Debug functions.  */
extern int insn_uid (rtx);
extern rtx insn_pattern (rtx);
extern int insn_code (rtx);
extern bool insn_is_set_p (rtx);
extern int hard_regno_rename_ok (int, int);

extern void debug_rhs (rhs_t);
extern void debug_av_set (av_set_t);
extern void debug_lv_set (regset);
extern void debug_ilist (ilist_t);
extern void debug_blist (blist_t);
extern void debug_insn_array (rtx *, int);
extern void debug_hard_reg_set (HARD_REG_SET);
extern rtx debug_mem_addr_value (rtx);
extern bool debug_find_insn_loop (void);
extern bool debug_find_unreachable_blocks (void);
extern void sel_debug_cfg (void);
extern void mem_test (int);

#endif
