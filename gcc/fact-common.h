/* Local factoring (code hoisting/sinking) on SSA trees.
   Copyright (C) 2004 Free Software Foundation, Inc.
 
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
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#ifndef GCC_FACT_COMMON
#define GCC_FACT_COMMON
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "basic-block.h"
#include "resource.h"
#include "ggc.h"
#include "tree.h"
#include "tree-flow.h"

typedef struct bb_decorator_def
{
  /* Pointer to the next basic block in the sibling set.  */
  struct bb_decorator_def *next_sibling;
  struct bb_decorator_def *prev_sibling;

  /* The decorated basic block.  */
  basic_block curr;
  struct bb_decorator_def *prev_decorator, *next_decorator;
  struct bb_decorator_def *prev_analogous, *next_analogous;
  block_stmt_iterator moveable_node;
  rtx moveable_insn;

  /* The depth of the last investigated stmt.  */
  int last_depth;

  /* The number of pred. basic blocks of CURR basic block.  */
  int num_pred_bb;

  /* The number of succ. basic blocks of CURR basic block.  */
  int num_succ_bb;
} *bb_decorator;

enum lfact_direction
{
  LFD_HOISTING,
  LFD_SINKING
};

extern void collect_family_sibling (bb_decorator, enum lfact_direction);
extern bb_decorator init_factoring (bb_decorator);
extern void free_bb_decorator_list (bb_decorator);
extern void collect_full_sibling (bb_decorator, enum lfact_direction);
extern void collect_sibling (bb_decorator, enum lfact_direction);
extern void cost_analyzer (bb_decorator, enum lfact_direction);
extern void cost_analyzer_1 (bb_decorator, enum lfact_direction);
extern void delete_siblings (bb_decorator);
extern void dump_cfg_info (FILE * fp);
extern void dump_siblings (FILE *, bb_decorator, enum lfact_direction);
extern bool is_tree_lfact (void);
extern bool is_seqabstr (void);

/* Main factoring functions */
extern void rtl_seqabstr (void);
extern int rtl_lfact (int, enum lfact_direction);
extern void tree_ssa_local_factoring (void);
extern void tree_seqabstr (void);
#endif
